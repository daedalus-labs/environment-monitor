/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#include "connectivity/wifi-connection.hpp"

#include "connectivity/constants.hpp"

#include <pico/cyw43_arch.h>
#include <pico/error.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>


std::string_view toString(WifiStatus status)
{
    switch (status) {
    case WifiStatus::CONNECTED:
        return WIFI_STATUS_CONNECTED;
    case WifiStatus::CONNECTING:
        return WIFI_STATUS_CONNECTING;
    case WifiStatus::CONNECTION_FAILURE:
        return WIFI_STATUS_CONNECTION_FAILURE;
    case WifiStatus::INITIALIZATION_FAILURE:
        return WIFI_STATUS_INITIALIZATION_FAILURE;
    default:
        return WIFI_STATUS_UNKNOWN;
    }
}

WifiConnection::WifiConnection(std::string_view ssid, std::string_view passphrase) : _state(WifiStatus::INITIALIZATION_FAILURE)
{
    int32_t result = cyw43_arch_init();
    if (result != PICO_OK) {
        printf("Failed to initialize Wifi: %d\n", result);
        _state = WifiStatus::INITIALIZATION_FAILURE;
        return;
    }

    _state = WifiStatus::CONNECTING;
    _connect(ssid, passphrase);
}

WifiConnection::~WifiConnection()
{
    cyw43_arch_deinit();
}

WifiStatus WifiConnection::status() const
{
    return _state;
}

void WifiConnection::_connect(std::string_view ssid, std::string_view passphrase)
{
    cyw43_arch_enable_sta_mode();
    printf("Connecting to Wifi...\n");

    int32_t result = INT32_MIN;
    if (passphrase.size() == 0) {
        result = cyw43_arch_wifi_connect_timeout_ms(ssid.data(), NULL, CYW43_AUTH_OPEN, WIFI_TIMEOUT_MS);
    }
    else {
        result = cyw43_arch_wifi_connect_timeout_ms(ssid.data(), passphrase.data(), CYW43_AUTH_WPA2_AES_PSK, WIFI_TIMEOUT_MS);
    }

    if (result != PICO_OK) {
        printf("Failed to connect to Wifi: %d\n", result);
        _state = WifiStatus::CONNECTION_FAILURE;
    }

    printf("Connected to %s\n", ssid.data());
    _state = WifiStatus::CONNECTED;
}
