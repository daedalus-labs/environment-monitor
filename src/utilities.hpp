/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#pragma once


#include <pico/time.h>
#include <pico/types.h>

#include <cstdint>


/**
 * @return The microseconds since boot.
 */
uint64_t microseconds();
