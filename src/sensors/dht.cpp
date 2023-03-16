/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/

#include "sensors/dht.hpp"

#include "sensors/constants.hpp"
#include "utilities.hpp"

#include <hardware/gpio.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <pico/time.h>
#include <pico/types.h>

#include <array>
#include <cfloat>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <thread>

inline constexpr size_t HUMIDITY_MSB_INDEX = 0;
inline constexpr size_t HUMIDITY_LSB_INDEX = 1;
inline constexpr size_t TEMP_MSB_INDEX = 2;
inline constexpr size_t TEMP_LSB_INDEX = 3;
inline constexpr size_t PARITY_INDEX = 4;
inline constexpr float DATA_FACTOR = 10.0;
inline constexpr float C_TO_F_SCALE = 9.0 / 5.0;
inline constexpr float C_TO_F_OFFSET = 32.0;
inline constexpr uint8_t MAX_READ_CHECKS = 100;
inline constexpr uint64_t LOGICAL_ZERO_THRESHOLD = 40;

DHT::DHT(DHTType type, uint8_t data_pin, uint8_t feedback_led_pin)
    : _humidity(DEFAULT_HUMIDITY), _temperature(DEFAULT_TEMPERATURE), _type(type), _data_pin(data_pin), _feedback_led_pin(feedback_led_pin)
{
    if (_feedback_led_pin < NUM_BANK0_GPIOS) {
        gpio_init(_feedback_led_pin);
        gpio_set_dir(_feedback_led_pin, GPIO_OUT);
        gpio_put(_feedback_led_pin, LOW);
    }
    else {
        printf("Feedback disabled, LED PIN %u is invalid\n", _feedback_led_pin);
    }

    gpio_init(_data_pin);
}

float DHT::celsius() const
{
    return _temperature;
}

float DHT::fahrenheit() const
{
    return (C_TO_F_SCALE * _temperature) + C_TO_F_OFFSET;
}

float DHT::humidity() const
{
    return _humidity;
}

DHTType DHT::type() const
{
    return _type;
}

void DHT::read()
{
    _read();
}

bool DHT::_checkResponse() const
{
    /*
     * The check behavior here is taken from Step 2 in DHT communication
     * It is looking to verify the sensor response signal.
     */

    uint8_t read_count = 0;
    gpio_set_dir(_data_pin, GPIO_IN);
    while (gpio_get(_data_pin) && read_count < MAX_READ_CHECKS) // DHT22 Pull down 40~80us
    {
        read_count++;
        sleep_us(1);
    };
    if (read_count >= MAX_READ_CHECKS)
        return 1;
    else
        read_count = 0;
    while (!gpio_get(_data_pin) && read_count < MAX_READ_CHECKS) // DHT22 Pull up 40~80us
    {
        read_count++;
        sleep_us(1);
    };
    if (read_count >= MAX_READ_CHECKS)
        return false;
    return true;
}

bool DHT::_getDataBit() const
{
    uint read_count = 0;
    while (gpio_get(_data_pin) && read_count < MAX_READ_CHECKS) // wait become Low level
    {
        read_count++;
        sleep_us(1);
    }
    read_count = 0;
    while (!gpio_get(_data_pin) && read_count < MAX_READ_CHECKS) // wait become High level
    {
        read_count++;
        sleep_us(1);
    }
    sleep_us(40); // wait 40us
    if (gpio_get(_data_pin))
        return true;
    else
        return false;

    // /*
    //  * The DHT sensors are a bit strange.
    //  * Rather than having data come in as it should be interpreted in fixed time
    //  * intervals, whether a bit is "0" or "1" is defined by how long the data line
    //  * is high following it being low for 50 us:
    //  *  - 0: Data line is high for 26-28 us after the low period.
    //  *  - 1: Data line is high for 70 us after the low period.
    //  */

    // uint8_t read_count = 0;

    // // First section of code will wait until the data line goes high, returning out
    // // in error if there was a timeout.
    // while (!gpio_get(_data_pin) && read_count < MAX_READ_CHECKS) {
    //     read_count++;
    //     sleep_us(1);
    // }

    // printf("Read Count on wait: %u\n", read_count);

    // if (read_count >= MAX_READ_CHECKS) {
    //     printf("Failed to read bit from %u due to time-out\n", _data_pin);
    //     return false;
    // }

    // // At this point, the code will measure the amount of time the data line
    // // is high by capturing the microseconds since boot at the start and end
    // // of the pulse. As above, this code will return out in error if there was
    // // a timeout.
    // uint64_t start = microseconds();
    // read_count = 0;
    // while (gpio_get(_data_pin) && read_count < MAX_READ_CHECKS) {
    //     read_count++;
    //     sleep_us(1);
    // }
    // uint64_t end = microseconds();

    // printf("Pulse Length: %lu us\n", end - start);

    // if (read_count >= MAX_READ_CHECKS) {
    //     printf("Failed to read bit from %u due to time-out\n", _data_pin);
    //     return false;
    // }

    // // Assume if the time is less than the threshold for a `0`, it must be
    // // a `1`.
    // return (end - start) > LOGICAL_ZERO_THRESHOLD;
}

uint8_t DHT::_getDataByte() const
{
    uint8_t data = 0;
    for (size_t i = 0; i < BITS_IN_BYTE; i++) {
        data <<= 1;
        data |= _getDataBit();
    }
    return data;
}

void DHT::_parse(const Frame& data)
{
    switch (_type) {
    case DHTType::DHT11:
        _humidity = static_cast<float>(data[HUMIDITY_MSB_INDEX]);
        _temperature = static_cast<float>(data[TEMP_MSB_INDEX]);
        break;
    case DHTType::DHT21:
    case DHTType::DHT22:
    default:
        _humidity = ((data[HUMIDITY_MSB_INDEX] << BITS_IN_BYTE) + data[HUMIDITY_LSB_INDEX]) / DATA_FACTOR;
        _temperature = (((data[TEMP_MSB_INDEX] & 0x7F) << BITS_IN_BYTE) + data[TEMP_LSB_INDEX]) / DATA_FACTOR;
        break;
    }

    if (_humidity > 100) {
        _humidity = data[HUMIDITY_MSB_INDEX];
    }

    if (_temperature > 125) {
        _temperature = data[TEMP_MSB_INDEX];
    }

    if (data[TEMP_MSB_INDEX] & 0x80) {
        _temperature = -1 * _temperature;
    }
}

void DHT::_read()
{
    Frame data;

    _setLED(ON);
    _start();

    if (!_checkResponse()) {
        _setLED(OFF);
        _temperature = DEFAULT_TEMPERATURE;
        _humidity = DEFAULT_HUMIDITY;
        printf("DHT Sensor did not respond to reset\n");
        return;
    }

    for (size_t index = 0; index < data.size(); index++) {
        data[index] = _getDataByte();
    }

    uint8_t calculated_parity = data[HUMIDITY_MSB_INDEX] + data[HUMIDITY_LSB_INDEX] + data[TEMP_MSB_INDEX] + data[TEMP_LSB_INDEX];
    if (calculated_parity != data[PARITY_INDEX]) {
        printf("DHT data parity check failed (%u != %u)\n", calculated_parity, data[PARITY_INDEX]);
        _setLED(OFF);
        return;
    }

    _setLED(OFF);
    _parse(data);
}

void DHT::_setLED(uint8_t state) const
{
    if (_feedback_led_pin >= NUM_BANK0_GPIOS) {
        return;
    }

    gpio_put(_feedback_led_pin, state);
}

void DHT::_start()
{
    /*
     * The reset behavior here is taken from Step 2 in DHT communication
     * Although some of the reference integration code provided by Waveshare show holding the data line low for longer.
     * Basic flow here is to pull down the data line for 20 milliseconds then high for 20 microseconds.
     */
    printf("Starting on %lu\n", _data_pin);

    gpio_set_dir(_data_pin, GPIO_OUT);
    gpio_put(_data_pin, LOW);
    sleep_ms(20);
    gpio_put(_data_pin, HIGH);
    sleep_us(30);
}
