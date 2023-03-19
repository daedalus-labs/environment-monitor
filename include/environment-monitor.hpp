/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#pragma once

#include <string_view>


inline constexpr std::string_view ClimateStatus = "climate/status";

struct ClimateStatusMessage
{
    double humidity;
    double temperature;
};
