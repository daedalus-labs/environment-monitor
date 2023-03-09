/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#pragma once


#include <cstdint>
#include <string_view>

enum class WifiStatus : uint8_t
{
    CONNECTED,
    CONNECTING,
    INITIALIZATION_FAILURE,
    CONNECTION_FAILURE
};

std::string_view toString(WifiStatus status);

class WifiConnection
{
public:
    WifiConnection(std::string_view ssid, std::string_view passphrase);
    ~WifiConnection();

    WifiStatus status() const;

private:
    void _connect(std::string_view ssid, std::string_view passphrase);

    WifiStatus _state;
};
