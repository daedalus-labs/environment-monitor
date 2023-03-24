/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#include "sensors/lipo-shim.hpp"

#include "sensors/constants.hpp"

#include <hardware/adc.h>
#include <hardware/gpio.h>

#include <cstdint>
#include <cstdio>


namespace sensors {
LiPoShim::LiPoShim(uint8_t voltage_adc_pin, uint8_t charging_pin) : _voltage_adc_pin(voltage_adc_pin), _charging_pin(charging_pin)
{
    adc_gpio_init(_voltage_adc_pin + GPIO_PIN_OFFSET);
    gpio_init(_charging_pin);
    gpio_set_dir(_charging_pin, GPIO_IN);
}

uint8_t LiPoShim::chargeRemaining() const
{
    uint8_t charge = ((voltage() - EMPTY_BATTERY_VOLTAGE) / (FULL_BATTERY_VOLTAGE - EMPTY_BATTERY_VOLTAGE)) * PERCENT_FACTOR;
    if (charge > PERCENT_FACTOR) {
        return PERCENT_FACTOR;
    }
    return charge;
}

bool LiPoShim::charging() const
{
    return gpio_get(_charging_pin);
}

float LiPoShim::voltage() const
{
    adc_select_input(_voltage_adc_pin);
    return adc_read() * VOLTAGE_DIVIDER_CONVERSION_FACTOR;
}
} // namespace sensors
