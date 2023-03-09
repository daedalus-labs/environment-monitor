/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/

#include "utilities.hpp"

#include <pico/time.h>
#include <pico/types.h>

#include <cstdint>


uint64_t microseconds()
{
    return to_us_since_boot(get_absolute_time());
}
