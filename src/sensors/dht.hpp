/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#pragma once


#include <cstdint>

enum class DHTType : uint8_t
{
    DHT11,
    DHT21,
    DHT22
};

class DHT
{
public:
    DHT(DHTType type, uint8_t data_pin, uint8_t feedback_led_pin);

    float humidity() const;
    float temperature() const;
    DHTType type() const;

private:
    bool _checkResponse() const;

    /**
     * Reads a bit from the sensor's data pin according to the protocol used by DHT sensors.
     *
     * @note See https://www.waveshare.com/wiki/DHT22_Temperature-Humidity_Sensor
     * @return true if the bit is high, false if the bit is low.
     */
    bool _getDataBit() const;
    uint8_t _getDataByte() const;
    void _read();
    void _setLED(uint8_t state) const;
    void _start();

    float _humidity;
    float _temperature;
    DHTType _type;
    uint8_t _data_pin;
    uint8_t _feedback_led_pin;
};
