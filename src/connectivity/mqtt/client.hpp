/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#pragma once

#include <lwip/apps/mqtt.h>

#include <cstdint>
#include <string>
#include <string_view>


namespace mqtt {
class Client
{
public:
    Client(std::string_view broker, uint16_t port);
    ~Client();

    bool connect();
    bool disconnect();
    bool publish();

private:
    mqtt_client_t* _mqtt;
    std::string _broker;
    uint16_t _port;
};
} // namespace mqtt
