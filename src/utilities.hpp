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

/**
 * Waits on the to become @a desired_state.
 *
 * Will timeout after @a wait_length, and return false.
 *
 * @note This is a blocking wait.
 * @param[in] gpio_pin The pin to wait on.
 * @param[in] desired_state The desired state of @a gpio_pin.
 * @param[in] wait_length The time to wait for it to enter this state in microseconds (us).
 * @return True if the wait was successful, false otherwise.
 */
bool wait(uint8_t gpio_pin, bool desired_state, uint64_t wait_length);
