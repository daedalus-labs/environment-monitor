/*------------------------------------------------------------------------------
Copyright (c) 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#pragma once

#include <string_view>


inline constexpr std::string_view HUMIDITY_TOPIC_NAME_FORMAT = "%s/climate/sensor/humidity";
inline constexpr std::string_view TEMPERATURE_TOPIC_NAME_FORMAT = "%s/climate/sensor/temperate";
