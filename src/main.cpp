/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#include "connectivity/wifi-connection.hpp"
#include "generated/configuration.hpp"
#include "sensors/constants.hpp"
#include "sensors/dht.hpp"

#include <boards/pico_w.h>
#include <hardware/gpio.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>

#include <cstdint>
#include <cstdio>

#ifdef RASPBERRYPI_PICO_W
inline constexpr uint8_t LED_PIN = UINT8_MAX;
#else
inline constexpr uint8_t LED_PIN = PICO_DEFAULT_LED_PIN;
#endif

inline constexpr uint8_t DHT_DATA_PIN = 22;


int main(int argc, char** argv)
{
    stdio_init_all();

    WifiConnection wifi(SSID, PASSPHRASE);
    printf("Wifi Connection Status: %s\n", toString(wifi.status()).data());

    DHT sensor(DHTType::DHT22, DHT_DATA_PIN, LED_PIN);
    return 0;
}
