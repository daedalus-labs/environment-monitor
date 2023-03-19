/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/

#include "connectivity/mqtt/client.hpp"

#include <lwip/apps/mqtt.h>
#include <pico/cyw43_arch.h>


namespace mqtt {
Client::Client(std::string_view broker, uint16_t port) : _mqtt(mqtt_client_new()), _broker(broker), _port(port)
{}

Client::~Client()
{
    mqtt_client_free(_mqtt);
}

bool Client::connect()
{
    cyw43_arch_lwip_begin();
    // What to do here?
    cyw43_arch_lwip_end();
}


bool Client::disconnect()
{
    cyw43_arch_lwip_begin();
    mqtt_disconnect(_mqtt);
    cyw43_arch_lwip_end();
}
} // namespace mqtt
