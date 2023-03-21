/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#include "connectivity/mqtt.hpp"
#include "connectivity/wireless.hpp"
#include "generated/configuration.hpp"
#include "sensors/board.hpp"
#include "sensors/constants.hpp"
#include "sensors/dht.hpp"
#include "sensors/lipo-shim.hpp"
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
    mqtt::Client mqtt(MQTT_BROKER, CONFIGURED_MQTT_PORT, DEVICE_NAME);
    DHT sensor(DHTType::DHT22, DHT_DATA_PIN, DHT_FEEDBACK_PIN);
    sensors::LiPoShim battery(BATTERY_VOLTAGE_PIN, BATTERY_CHARGING_PIN);
    sensors::Board board;

    uint32_t count = 0;
    float board_temp = 0.0f;
    float sensor_temp = 0.0f;

    sleep_ms(10000);

    while (true) {
        printf("\n----------------- [%u]\n", count);

        board_temp = board.temperature();
        sensor_temp = sensor.temperature();
        sensor.read();
        wifi.poll();

        printf("Temperature: %.1fC (%.1fF), Humidity: %.1f%%\n", sensor_temp, toFahrenheit(sensor_temp), sensor.humidity());
        printf("Wifi Connection Status: %s (%s)\n", toString(wifi.status()).data(), wifi.ipAddress().c_str());
        printf("Battery Charge: %.3fV (%u%%)\n", battery.voltage(), battery.chargeRemaining());
        printf("Battery is %s\n", battery.charging() ? "charging" : "not charging");
        printf("CPU Temperature: %.1fC (%.1fF)\n", board_temp, toFahrenheit(board_temp));

        printf("-----------------\n");

        sleep_ms(10000);
        count++;
    }

    return 0;
}
