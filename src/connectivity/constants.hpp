/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/

#include <cstdint>
#include <string_view>

inline constexpr uint32_t WIFI_TIMEOUT_MS = 30000;
inline constexpr uint16_t SERVER_MANAGE_PORT = 5009;
inline constexpr std::string_view WIFI_STATUS_CONNECTED = "CONNECTED";
inline constexpr std::string_view WIFI_STATUS_CONNECTING = "CONNECTING";
inline constexpr std::string_view WIFI_STATUS_INITIALIZATION_FAILURE = "INITIALIZATION_FAILURE";
inline constexpr std::string_view WIFI_STATUS_CONNECTION_FAILURE = "CONNECTION_FAILURE";
inline constexpr std::string_view WIFI_STATUS_UNKNOWN = "UNKNOWN";
