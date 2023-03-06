/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
------------------------------------------------------------------------------*/

#include "sensors/dht.hpp"

#include "sensors/constants.hpp"

#include <hardware/gpio.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>

#include <array>
#include <cfloat>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <thread>

inline constexpr size_t BUFFER_SIZE = 5;
inline constexpr size_t HUMIDITY_MSB_INDEX = 0;
inline constexpr size_t HUMIDITY_LSB_INDEX = 1;
inline constexpr size_t TEMP_MSB_INDEX = 2;
inline constexpr size_t TEMP_LSB_INDEX = 3;
inline constexpr size_t PARITY_INDEX = 4;
inline constexpr float DATA_FACTOR = 10.0;
inline constexpr uint8_t MAX_READ_CHECKS = 100;

static uint8_t readByte(uint8_t pin)
{}

DHT::DHT(DHTType type, uint8_t data_pin, uint8_t feedback_led_pin)
    : _humidity(DEFAULT_HUMIDITY), _temperature(DEFAULT_TEMPERATURE), _type(type), _data_pin(data_pin), _feedback_led_pin(feedback_led_pin)
{
    if (_feedback_led_pin < NUM_BANK0_GPIOS) {
        gpio_init(_feedback_led_pin);
        gpio_set_dir(_feedback_led_pin, GPIO_OUT);
        gpio_put(_feedback_led_pin, LOW);
    }
}

float DHT::humidity() const
{
    return _humidity;
}

float DHT::temperature() const
{
    return _temperature;
}

DHTType DHT::type() const
{
    return _type;
}

bool DHT::_check() const
{
    // The check behavior here is taken from Step 2 in DHT communication
    // It is looking to verify the sensor response signal.
    //
    // Reference: https://www.waveshare.com/wiki/DHT22_Temperature-Humidity_Sensor
    uint8_t read_count = 0;
    gpio_set_dir(_data_pin, GPIO_IN);
    while (gpio_get(_data_pin) && read_count < 100) {
        read_count++;
        sleep_us(1);
    }

    if (read_count >= MAX_READ_CHECKS) {
        return false;
    }

    read_count = 0;
    while (!gpio_get(_data_pin) && read_count < 100) {
        read_count++;
        sleep_us(1);
    }

    return read_count < MAX_READ_CHECKS;
}

void DHT::_read()
{
    std::array<uint8_t, BUFFER_SIZE> buffer;

    _setLED(ON);
    _reset();

    if (!_check()) {
        _setLED(OFF);
        _temperature = DEFAULT_TEMPERATURE;
        _humidity = DEFAULT_HUMIDITY;
        printf("DHT Sensor did not respond to reset\n");
        return;
    }

    for (size_t index; index < buffer.size(); index++) {
        buffer[index] = readByte(_data_pin);
    }

    uint8_t calculated_parity = buffer[HUMIDITY_MSB_INDEX] + buffer[HUMIDITY_LSB_INDEX] + buffer[TEMP_MSB_INDEX] + buffer[TEMP_LSB_INDEX];
    if (calculated_parity != buffer[PARITY_INDEX]) {
        printf("DHT data parity check failed (%u != %u)\n", calculated_parity, buffer[PARITY_INDEX]);
        _setLED(OFF);
        return;
    }

    _setLED(OFF);

    _humidity = ((buffer[HUMIDITY_MSB_INDEX] << BITS_IN_BYTE) + buffer[HUMIDITY_LSB_INDEX]) / DATA_FACTOR;
    _temperature = (((buffer[TEMP_MSB_INDEX] & 0x7F) << BITS_IN_BYTE) + buffer[TEMP_LSB_INDEX]) / DATA_FACTOR;

    if (_humidity > 100) {
        _humidity = buffer[HUMIDITY_MSB_INDEX];
    }

    if (_temperature > 125) {
        _temperature = buffer[TEMP_MSB_INDEX];
    }

    if (buffer[TEMP_MSB_INDEX] & 0x80) {
        _temperature = -1 * _temperature;
    }
}

void DHT::_reset()
{
    // The reset behavior here is taken from Step 2 in DHT communication
    // Although some of the reference integration code provided by Waveshare show holding the data line low for longer.
    // Basic flow here is to pull down the data line for 20 milliseconds then high for 20 microseconds.
    //
    // Reference: https://www.waveshare.com/wiki/DHT22_Temperature-Humidity_Sensor
    gpio_set_dir(_data_pin, GPIO_OUT);
    gpio_put(_data_pin, LOW);
    sleep_ms(20);
    gpio_put(_data_pin, HIGH);
    sleep_us(20);
    gpio_set_dir(_data_pin, GPIO_IN);
}

void DHT::_setLED(uint8_t state) const
{
    if (_feedback_led_pin >= NUM_BANK0_GPIOS) {
        return;
    }

    gpio_put(_feedback_led_pin, state);
}
