/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#include "connectivity/connection-status.hpp"
#include "connectivity/wifi-connection.hpp"
#include "generated/configuration.hpp"
#include "sensors/battery.hpp"
#include "sensors/board.hpp"
#include "sensors/constants.hpp"
#include "sensors/dht.hpp"
#include "utilities.hpp"

#include <hardware/adc.h>
#include <hardware/gpio.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>

#include <cstdint>
#include <cstdio>


int main(int argc, char** argv)
{
    stdio_init_all();
    adc_init();

    WifiConnection wifi(SSID, PASSPHRASE);
    printf("Wifi Connection Status: %s\n", toString(wifi.status()).data());

    DHT sensor(DHTType::DHT22, DHT_DATA_PIN, DHT_FEEDBACK_PIN);
    sensors::Battery battery(BATTERY_VOLTAGE_PIN, BATTERY_CHARGING_PIN);
    sensors::Board board;

    sleep_ms(10000);

    while (true) {
        printf("\n-----------------\n");
        sensor.read();
        wifi.poll();
        printf("Temperature: %.1fC (%.1fF), Humidity: %.1f%%\n", sensor.celsius(), sensor.fahrenheit(), sensor.humidity());
        printf("Wifi Connection Status: %s (%s)\n", toString(wifi.status()).data(), wifi.ipAddress().c_str());
        printf("Battery Charge: %u%% (%s)\n", battery.chargeRemaining(), battery.charging() ? "Charging" : "Not Charging");
        printf("CPU Temperature: %.1fC\n", board.celsius());
        printf("-----------------\n");

        sleep_ms(10000);
    }

    return 0;
}
