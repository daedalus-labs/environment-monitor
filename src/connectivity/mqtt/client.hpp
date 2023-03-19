/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/
#pragma once

#include <lwip/apps/mqtt.h>
#include <lwip/ip_addr.h>

#include <cstdint>
#include <string>
#include <string_view>


namespace mqtt {
class ClientInfo;

class Client
{
public:
    Client(std::string_view broker, uint16_t port, std::string_view user, std::string_view password);
    ~Client();

    bool connect();
    bool disconnect();
    bool publish();

private:
    mqtt_client_t* _mqtt;
    ClientInfo* _info;
    std::string _broker;
    uint16_t _port;
    ip_addr_t _broker_address;
    std::string _user;
    std::string _password;
    mqtt_connect_client_info_t _mqtt_client_info;
};
} // namespace mqtt
