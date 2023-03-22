/*------------------------------------------------------------------------------
Copyright (c) 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#pragma once

#include <string_view>

inline constexpr std::string_view PROGRAM_TOPIC_NAME_FORMAT = "%s";
inline constexpr std::string_view VERSION_TOPIC_NAME_FORMAT = "%s/version";
inline constexpr std::string_view BUILD_DATE_TOPIC_NAME_FORMAT = "%s/build_date";
inline constexpr std::string_view START_DATE_TOPIC_NAME_FORMAT = "%s/start_date";
inline constexpr std::string_view BATTERY_TOPIC_NAME_FORMAT = "%s/battery";
inline constexpr std::string_view HUMIDITY_TOPIC_NAME_FORMAT = "%s/climate/humidity";
inline constexpr std::string_view TEMPERATURE_TOPIC_NAME_FORMAT = "%s/climate/temperate";
