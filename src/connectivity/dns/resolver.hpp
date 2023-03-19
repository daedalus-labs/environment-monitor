/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#pragma once

#include <lwip/ip_addr.h>

#include <cstdint>


namespace dns {
bool resolve(const char* hostname);
}
