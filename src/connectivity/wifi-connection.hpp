/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#pragma once

#include "connectivity/connection-status.hpp"

#include <array>
#include <cstdint>
#include <string>
#include <string_view>


struct WifiState; // Forward declaration of WifiState

using MACAddress = std::array<uint8_t, 6>;

class WifiConnection
{
public:
    WifiConnection(std::string_view ip_address, uint16_t port, std::string_view ssid, std::string_view passphrase);
    ~WifiConnection();

    std::string ipAddress() const;
    std::string netmask() const;
    std::string gateway() const;
    const MACAddress& macAddress() const;

    ConnectionStatus status() const;

    void poll();

private:
    void _connectToWireless(std::string_view ssid, std::string_view passphrase);
    void _connectToServer(std::string_view ip_address, uint16_t port);

    WifiState* _state;
    MACAddress _address;
};
