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

#include <array>
#include <cstdint>
#include <cstdio>
#include <utility>


inline constexpr size_t SAMPLE_SIZE = 10;
inline constexpr uint64_t SAMPLE_PERIOD_MS = 120000 / SAMPLE_SIZE; // Sample over 2 minutes
inline constexpr uint64_t SENSOR_AVERAGE_INTERVAL_MS = 1800000;    // 30 minutes

double average(const std::array<double, SAMPLE_SIZE>& samples)
{
    double total = 0;
    for (const auto& sample : samples) {
        total += sample;
    }

    return total / SAMPLE_SIZE;
}

std::pair<double, double> averageDHT(DHT& sensor)
{
    std::array<double, SAMPLE_SIZE> humidity;
    std::array<double, SAMPLE_SIZE> temperature;

    printf("Waking sensor...\n");
    sensor.wake();

    for (size_t i = 0; i < SAMPLE_SIZE; i++) {
        sensor.refresh();
        humidity[i] = sensor.humidity();
        temperature[i] = sensor.fahrenheit();
        sleep_ms(SAMPLE_PERIOD_MS);
    }

    std::pair<double, double> result;
    result.first = average(humidity);
    result.second = average(temperature);

    printf("Sleeping sensor...\n");
    sensor.sleep();
    return result;
}

int main(int argc, char** argv)
{
    stdio_init_all();

    WifiConnection wifi(SERVER_IP, SERVER_PORT, SSID, PASSPHRASE);
    printf("Wifi Connection Status: %s\n", toString(wifi.status()).data());

    DHT sensor(DHTType::DHT22, DHT_POWER_PIN, DHT_DATA_PIN, DHT_FEEDBACK_PIN);
    std::pair<double, double> dht_reading;

    while (true) {
        dht_reading = averageDHT(sensor);
        printf("Temperature: %.1fF, Humidity: %.1f%%\n", dht_reading.second, dht_reading.first);
        sleep_ms(SENSOR_AVERAGE_INTERVAL_MS);
    }

    return 0;
}
