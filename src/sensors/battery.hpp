/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#pragma once

#include <cstddef>
#include <cstdint>


namespace sensors {
class Battery
{
public:
    Battery(uint8_t voltage_adc_pin, uint8_t charging_pin);


    uint8_t chargeRemaining() const;


    bool charging() const;

private:
    uint8_t _voltage_adc_pin;
    uint8_t _charging_pin;
};
} // namespace sensors
