/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#pragma once

#include "connectivity/connection-status.hpp"

#include <cstdint>
#include <string_view>


struct WifiState; // Forward declaration of WifiState

class WifiConnection
{
public:
    WifiConnection(std::string_view ip_address, uint16_t port, std::string_view ssid, std::string_view passphrase);
    ~WifiConnection();

    ConnectionStatus status() const;

    void poll();

private:
    void _connectToWireless(std::string_view ssid, std::string_view passphrase);
    void _connectToServer(std::string_view ip_address, uint16_t port);

    WifiState* _state;
};
