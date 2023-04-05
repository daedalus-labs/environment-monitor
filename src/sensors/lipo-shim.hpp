/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#pragma once

#include <cstddef>
#include <cstdint>


namespace sensors {
/**
 * Battery state sensor based on the Pimoroni LiPo Shim.
 *
 * @see https://shop.pimoroni.com/products/pico-lipo-shim
 */
class LiPoShim
{
public:
    /**
     * Constructor.
     *
     * @param[in] voltage_adc_pin The ADC Pin used to read the current battery voltage.
     * @param[in] charging_pin The GPIO Pin used to indicate the charging state of the shim.
     */
    LiPoShim(uint8_t voltage_adc_pin, uint8_t charging_pin);

    /**
     * @return The remaining battery charge as a percentage.
     */
    uint8_t chargeRemaining() const;

    /**
     * @return Whether the battery is charging (true) or discharging (false).
     */
    bool charging() const;

    /**
     * @return The current supply voltage in volts.
     */
    float voltage() const;

private:
    uint8_t _voltage_adc_pin;
    uint8_t _charging_pin;
};
} // namespace sensors
