/*------------------------------------------------------------------------------
Copyright 2023 Joe Porembski
SPDX-License-Identifier: BSD-3-Clause
------------------------------------------------------------------------------*/

#include "connectivity/mqtt/client.hpp"

#include "connectivity/dns/resolver.hpp"

#include <lwip/apps/mqtt.h>
#include <pico/cyw43_arch.h>

#include <cstdint>
#include <cstdio>


inline constexpr uint16_t KEEP_ALIVE_TIMEOUT_S = 120;

static void onConnection(mqtt_client_t* /* unused */, void* arg, mqtt_connection_status_t status)
{
    const mqtt_connect_client_info_t* client_info = (const mqtt_connect_client_info_t*)arg;
}

static void onData(void* arg, const u8_t* data, u16_t len, u8_t flags)
{}

static void onPublish(void* arg, const char* topic, u32_t tot_len)
{}

static void onRequest(void* arg, err_t err)
{}

namespace mqtt {
class ClientInfo
{};

Client::Client(std::string_view broker, uint16_t port, std::string_view user, std::string_view password)
    : _mqtt(mqtt_client_new()), _info(), _broker(broker), _port(port), _user(user), _password(password), _mqtt_client_info()
{
    _mqtt_client_info.keep_alive = KEEP_ALIVE_TIMEOUT_S;
    _mqtt_client_info.will_msg = NULL;
    _mqtt_client_info.will_qos = 0;
    _mqtt_client_info.will_retain = 0;
    _mqtt_client_info.will_topic = NULL;
#if LWIP_ALTCP && LWIP_ALTCP_TLS
    _mqtt_client_info.tls_config = NULL;
#endif
}

Client::~Client()
{
    // Can I disconnect twice safely? Can I check if I'm connected?
    cyw43_arch_lwip_begin();
    mqtt_disconnect(_mqtt);
    mqtt_client_free(_mqtt);
    cyw43_arch_lwip_end();

    free(_info);
}

bool Client::connect()
{
    if (!dns::resolve(_broker.c_str())) {
        printf("Failed to resolve %s when connecting to MQTT\n", _broker.c_str());
        return false;
    }

    _mqtt_client_info.client_pass = _password.c_str();
    _mqtt_client_info.client_user = _user.c_str();

    mqtt_set_inpub_callback(_mqtt, onPublish, onData, LWIP_CONST_CAST(void*, &_mqtt_client_info));

    cyw43_arch_lwip_begin();
    mqtt_client_connect(_mqtt, &_broker_address, _port, onConnection, LWIP_CONST_CAST(void*, &_mqtt_client_info), &_mqtt_client_info);
    cyw43_arch_lwip_end();
    return true;
}

bool Client::disconnect()
{
    cyw43_arch_lwip_begin();
    mqtt_disconnect(_mqtt);
    cyw43_arch_lwip_end();
    return true;
}

bool Client::publish()
{
    return false;
}
} // namespace mqtt
