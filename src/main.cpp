// /*------------------------------------------------------------------------------
// Copyright 2023 Joe Porembski
// SPDX-License-Identifier: BSD-3-Clause
// ------------------------------------------------------------------------------*/
#include "generated/configuration.hpp"
#include "sensors/constants.hpp"
#include "sensors/dht.hpp"
#include "utilities.hpp"

#include <boards/pico_w.h>
#include <hardware/gpio.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>

#include <cstdint>
#include <cstdio>

#ifdef RASPBERRYPI_PICO_W
inline constexpr uint8_t LED_PIN = 13;
#else
inline constexpr uint8_t LED_PIN = PICO_DEFAULT_LED_PIN;
#endif

inline constexpr uint8_t DHT_DATA_PIN = 18;


int main(int argc, char** argv)
{
    stdio_init_all();

    DHT sensor(DHTType::DHT22, DHT_DATA_PIN, LED_PIN);

    while (true) {
        sleep_ms(10000);
        sensor.read();
        printf("Temperature: %.1fC (%.1fF), Humidity: %.1f%%\n", sensor.celsius(), sensor.fahrenheit(), sensor.humidity());
    }

    return 0;
}
