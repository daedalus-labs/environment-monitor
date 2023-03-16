/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#include "connectivity/connection-status.hpp"
#include "connectivity/wifi-connection.hpp"
#include "generated/configuration.hpp"
#include "sensors/constants.hpp"
#include "sensors/dht.hpp"
#include "utilities.hpp"

#include <hardware/gpio.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>

#include <cstdint>
#include <cstdio>


int main(int argc, char** argv)
{
    stdio_init_all();

    WifiConnection wifi(SERVER_IP, SERVER_PORT, SSID, PASSPHRASE);
    printf("Wifi Connection Status: %s\n", toString(wifi.status()).data());

    DHT sensor(DHTType::DHT22, DHT_DATA_PIN, DHT_FEEDBACK_PIN);

    sleep_ms(10000);
    DHT sensor(DHTType::DHT22, DHT_DATA_PIN, DHT_FEEDBACK_PIN);

    while (true) {
        sensor.read();
        printf("Temperature: %.1fC (%.1fF), Humidity: %.1f%%\n", sensor.celsius(), sensor.fahrenheit(), sensor.humidity());

        wifi.poll();
        printf("Wifi Connection Status: %s (%s | %s | %s)\n", toString(wifi.status()).data(), wifi.ipAddress().c_str(), wifi.gateway().c_str(), wifi.netmask().c_str());

        sleep_ms(10000);
    }

    return 0;
}
