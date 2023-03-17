/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/

#include <cfloat>
#include <climits>
#include <cstdint>


inline constexpr float DEFAULT_HUMIDITY = FLT_MIN;
inline constexpr float DEFAULT_TEMPERATURE = FLT_MIN;
inline constexpr uint32_t ON = 1;
inline constexpr uint32_t OFF = 0;
inline constexpr uint32_t HIGH = ON;
inline constexpr uint32_t LOW = OFF;
inline constexpr uint32_t BITS_IN_BYTE = (sizeof(uint8_t) * CHAR_BIT);
