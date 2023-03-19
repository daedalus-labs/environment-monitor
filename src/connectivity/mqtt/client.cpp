/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/

#include "connectivity/mqtt/client.hpp"

#include "connectivity/dns/resolver.hpp"

#include <lwip/apps/mqtt.h>
#include <pico/cyw43_arch.h>

#include <cstdio>


static void mqttConnectionCallback(mqtt_client_t* /* unused */, void* arg, mqtt_connection_status_t status)
{
    const mqtt_connect_client_info_t* client_info = (const mqtt_connect_client_info_t*)arg;
}

namespace mqtt {
class ClientInfo
{};

Client::Client(std::string_view broker, uint16_t port) : _mqtt(mqtt_client_new()), _info(), _broker(broker), _port(port), _mqtt_client_info()
{
    if (!dns::resolve(broker.data())) {
        printf("Failed to resolve %s when connecting to MQTT\n", broker.data());
    }
}

Client::~Client()
{
    mqtt_client_free(_mqtt);
    free(_info);
}

bool Client::connect()
{
    cyw43_arch_lwip_begin();
    mqtt_client_connect(_mqtt, &_broker_address, _port, mqttConnectionCallback, LWIP_CONST_CAST(void*, &_mqtt_client_info), &_mqtt_client_info);
    cyw43_arch_lwip_end();
}


bool Client::disconnect()
{
    cyw43_arch_lwip_begin();
    mqtt_disconnect(_mqtt);
    cyw43_arch_lwip_end();
}
} // namespace mqtt
