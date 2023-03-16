// /*------------------------------------------------------------------------------
// Copyright 2023 Joe Porembski
// SPDX-License-Identifier: BSD-3-Clause
// ------------------------------------------------------------------------------*/
// #include "connectivity/wifi-connection.hpp"
// #include "generated/configuration.hpp"
// #include "sensors/constants.hpp"
// #include "sensors/dht.hpp"

// #include <boards/pico_w.h>
// #include <hardware/gpio.h>
// #include <pico/stdio.h>
// #include <pico/stdlib.h>

// #include <cstdint>
// #include <cstdio>

// #ifdef RASPBERRYPI_PICO_W
// inline constexpr uint8_t LED_PIN = 13;
// #else
// inline constexpr uint8_t LED_PIN = PICO_DEFAULT_LED_PIN;
// #endif

// inline constexpr uint8_t DHT_DATA_PIN = 18;


// int main(int argc, char** argv)
// {
//     stdio_init_all();

//     // WifiConnection wifi(SSID, PASSPHRASE);
//     // printf("Wifi Connection Status: %s\n", toString(wifi.status()).data());

//     DHT sensor(DHTType::DHT22, DHT_DATA_PIN, LED_PIN);

//     while (true) {
//         sleep_ms(4000);
//         sensor.read();
//         printf("Temperature: %f, Humidity: %f\n", sensor.temperature(), sensor.humidity());
//     }

//     return 0;
// }

/**----------------------------------------------------------------**/

/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 **/

// #include "hardware/gpio.h"
// #include "pico/stdlib.h"

// #include <math.h>
// #include <stdio.h>

// #ifdef PICO_DEFAULT_LED_PIN
// #    define LED_PIN PICO_DEFAULT_LED_PIN
// #endif

// const uint DHT_PIN = 18;
// const uint MAX_TIMINGS = 85;
// #define LED_PIN 13

// typedef struct
// {
//     float humidity;
//     float temp_celsius;
// } dht_reading;

// void read_from_dht(dht_reading* result);

// int main()
// {
//     stdio_init_all();
//     sleep_ms(4000);
//     gpio_init(DHT_PIN);
// #ifdef LED_PIN
//     gpio_init(LED_PIN);
//     gpio_set_dir(LED_PIN, GPIO_OUT);
// #endif
//     while (1) {
//         dht_reading reading;
//         read_from_dht(&reading);
//         float fahrenheit = (reading.temp_celsius * 9 / 5) + 32;
//         printf("Humidity = %.1f%%, Temperature = %.1fC (%.1fF)\n", reading.humidity, reading.temp_celsius, fahrenheit);

//         sleep_ms(2000);
//     }
// }

// void read_from_dht(dht_reading* result)
// {
//     int data[5] = {0, 0, 0, 0, 0};
//     uint last = 1;
//     uint j = 0;

//     gpio_set_dir(DHT_PIN, GPIO_OUT);
//     gpio_put(DHT_PIN, 0);
//     sleep_ms(20);
//     gpio_set_dir(DHT_PIN, GPIO_IN);

// #ifdef LED_PIN
//     gpio_put(LED_PIN, 1);
// #endif
//     for (uint i = 0; i < MAX_TIMINGS; i++) {
//         uint count = 0;
//         while (gpio_get(DHT_PIN) == last) {
//             count++;
//             sleep_us(1);
//             if (count == 255)
//                 break;
//         }
//         last = gpio_get(DHT_PIN);
//         if (count == 255)
//             break;

//         if ((i >= 4) && (i % 2 == 0)) {
//             data[j / 8] <<= 1;
//             if (count > 16)
//                 data[j / 8] |= 1;
//             j++;
//         }
//     }
// #ifdef LED_PIN
//     gpio_put(LED_PIN, 0);
// #endif

//     printf("Data (%lu): [%d, %d, %d, %d, %d]\n", j, data[0], data[1], data[2], data[3], data[4]);

//     if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) {
//         result->humidity = (float)((data[0] << 8) + data[1]) / 10;
//         if (result->humidity > 100) {
//             result->humidity = data[0];
//         }
//         result->temp_celsius = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
//         if (result->temp_celsius > 125) {
//             result->temp_celsius = data[2];
//         }
//         if (data[2] & 0x80) {
//             result->temp_celsius = -result->temp_celsius;
//         }
//     }
//     else {
//         printf("Bad data\n");
//     }
// }
/**
 ******************************************************************************
 * @file    dht22.c
 * @author  Waveshare Team
 * @version
 * @date    2021-02-08
 * @brief   dht22.
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, WAVESHARE SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 ******************************************************************************
 */

/**----------------------------------------------------------------**/

#include "hardware/gpio.h"
#include "pico/stdlib.h"

#include <math.h>
#include <stdio.h>

#include <cstdint>

const uint LED_PIN = 13;
const uint DHT_PIN = 18;
const uint MAX_TIMINGS = 85;

typedef struct
{
    double humidity;
    double temp_celsius;
} dht_reading;

void read_from_dht(dht_reading* result);
uint DHT22_Read_Data(dht_reading* result);
int main()
{
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_init(DHT_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (1) {
        dht_reading reading;
        sleep_ms(4000);
        gpio_put(LED_PIN, 1);
        DHT22_Read_Data(&reading);
        gpio_put(LED_PIN, 0);
        double fahrenheit = (reading.temp_celsius * 9 / 5) + 32;
        printf("Humidity = %.1f%%, Temperature = %.1fC (%.1fF)\n", reading.humidity, reading.temp_celsius, fahrenheit);
    }
}

void DHT22_Rst(void)
{
    gpio_set_dir(DHT_PIN, GPIO_OUT); // SET OUTPUT
    gpio_put(DHT_PIN, 0);            // GPIOA.0=0
    sleep_ms(20);                    // Pull down Least 18ms
    gpio_put(DHT_PIN, 1);            // GPIOA.0=1
    sleep_us(30);                    // Pull up 20~40us
}

uint DHT22_Check(void)
{
    uint retry = 0;
    gpio_set_dir(DHT_PIN, GPIO_IN);          // SET INPUT
    while (gpio_get(DHT_PIN) && retry < 100) // DHT22 Pull down 40~80us
    {
        retry++;
        sleep_us(1);
    };
    if (retry >= 100)
        return 1;
    else
        retry = 0;
    while (!gpio_get(DHT_PIN) && retry < 100) // DHT22 Pull up 40~80us
    {
        retry++;
        sleep_us(1);
    };
    if (retry >= 100)
        return 1; // chack error
    return 0;
}

uint DHT22_Read_Bit(void)
{
    uint retry = 0;
    while (gpio_get(DHT_PIN) && retry < 100) // wait become Low level
    {
        retry++;
        sleep_us(1);
    }
    retry = 0;
    while (!gpio_get(DHT_PIN) && retry < 100) // wait become High level
    {
        retry++;
        sleep_us(1);
    }
    sleep_us(40); // wait 40us
    if (gpio_get(DHT_PIN))
        return 1;
    else
        return 0;
}

uint DHT22_Read_Byte(void)
{
    uint i, dat;
    dat = 0;
    for (i = 0; i < 8; i++) {
        dat <<= 1;
        dat |= DHT22_Read_Bit();
    }
    return dat;
}

uint DHT22_Read_Data(dht_reading* result)
{
    uint8_t buf[5];
    uint i;
    DHT22_Rst();
    if (DHT22_Check() == 0) {
        for (i = 0; i < 5; i++) {
            buf[i] = DHT22_Read_Byte();
        }
        printf("Data: [%d, %d, %d, %d, %d]\n", buf[0], buf[1], buf[2], buf[3], buf[4]);
        uint8_t calculated_parity = buf[0] + buf[1] + buf[2] + buf[3];
        if (calculated_parity == buf[4]) {
            result->humidity = ((buf[0] << 8) + buf[1]) / 10;
            result->temp_celsius = ((buf[2] << 8) + buf[3]) / 10;
        }
        else {
            printf("Parity failed: %lu != %lu\n", calculated_parity, buf[0]);
        }
    }
    else
        return 1;
    return 0;
}

uint DHT22_Init(void)
{
    DHT22_Rst();
    return DHT22_Check();
}
