/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#pragma once

#include <cstdint>
#include <string_view>


enum class ConnectionStatus : uint8_t
{
    CONNECTED = 0x00,
    WIFI_CONNECTING,
    CONNECTING_TO_SERVER,
    INITIALIZATION_FAILURE = 0xC0,
    WIFI_CONNECTION_FAILURE,
    SERVER_CONNECTION_FAILURE
};

std::string_view toString(ConnectionStatus status);
