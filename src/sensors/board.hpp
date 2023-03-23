/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#pragma once

#include <cstddef>
#include <cstdint>


namespace sensors {
class Board
{
public:
    Board();

    /**
     * @return The board temperature in degrees Celsius.
     */
    float celsius() const;

    /**
     * @return The board temperature in degrees Fahrenheit.
     */
    float fahrenheit() const;
};
} // namespace sensors
