/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#include "connectivity/wifi-connection.hpp"

#include "connectivity/constants.hpp"
#include "connectivity/detail/communication.hpp"
#include "connectivity/detail/wifi-state.hpp"

#include <lwip/pbuf.h>
#include <lwip/tcp.h>
#include <pico/cyw43_arch.h>
#include <pico/error.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>

#include <cstdint>


WifiConnection::WifiConnection(std::string_view ip_address, uint16_t port, std::string_view ssid, std::string_view passphrase) : _state(nullptr)
{
    int32_t result = cyw43_arch_init_with_country(CYW43_COUNTRY_USA);
    if (result != PICO_OK) {
        printf("Failed to initialize Wifi: %d\n", result);
        return;
    }

    _state = static_cast<WifiState*>(calloc(1, sizeof(WifiState)));
    if (!_state) {
        printf("Failed to allocate connection\n");
        return;
    }

    _connectToWireless(ssid, passphrase);
    _connectToServer(ip_address, port);
}

WifiConnection::~WifiConnection()
{
    if (_state) {
        free(_state);
    }

    cyw43_arch_deinit();
}

ConnectionStatus WifiConnection::status() const
{
    if (_state) {
        return _state->status;
    }
    return ConnectionStatus::INITIALIZATION_FAILURE;
}

void WifiConnection::poll()
{
    // This call is being used to maximize the windows of time in which this
    // application is available to do work for the CYW43.
    cyw43_arch_wait_for_work_until(make_timeout_time_ms(POLL_WAIT_TIME_MS));
}

void WifiConnection::_connectToWireless(std::string_view ssid, std::string_view passphrase)
{
    _state->status = ConnectionStatus::WIFI_CONNECTING;
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
        _state->status = ConnectionStatus::WIFI_CONNECTION_FAILURE;
        return;
    }

    printf("Connected to %s\n", ssid.data());
}

void WifiConnection::_connectToServer(std::string_view ip_address, uint16_t port)
{
    _state->status = ConnectionStatus::CONNECTING_TO_SERVER;
    printf("Establishing connection to server %s:%d... \n", ip_address.data(), port);
    ip4addr_aton(ip_address.data(), &_state->remote_address);
    _state->control = tcp_new_ip_type(IP_GET_TYPE(&_state->remote_addr));
    _state->remote_port = port;

    if (!_state->control) {
        printf("Failed to create TCP control object\n");
        _state->status = ConnectionStatus::SERVER_CONNECTION_FAILURE;
        return;
    }

    tcp_arg(_state->control, _state);
    tcp_poll(_state->control, clientPoll, POLL_TIME_S * 2);
    tcp_sent(_state->control, clientSent);
    tcp_recv(_state->control, clientReceive);
    tcp_err(_state->control, clientError);
    _state->buffer_length = 0;

    // cyw43_arch_lwip_begin/end should be used around calls into lwIP to ensure correct locking.
    // You can omit them if you are in a callback from lwIP. Note that when using pico_cyw_arch_poll
    // these calls are a no-op and can be omitted, but it is a good practice to use them in
    // case you switch the cyw43_arch type later.
    cyw43_arch_lwip_begin();
    err_t error = tcp_connect(_state->control, &_state->remote_address, _state->remote_port, clientConnected);
    cyw43_arch_lwip_end();

    if (error != ERR_OK) {
        printf("Failed to create TCP connection to server\n");
        _state->status = ConnectionStatus::SERVER_CONNECTION_FAILURE;
    }
}
