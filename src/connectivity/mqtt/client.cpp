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

static void onConnectionComplete(mqtt_client_t* /* unused */, void* arg, mqtt_connection_status_t status)
{}

static void onDataReceived(void* arg, const u8_t* data, u16_t len, u8_t flags)
{}

static void onTopicUpdated(void* arg, const char* topic, u32_t tot_len)
{}

static void onRequestComplete(void* arg, err_t err)
{}

namespace mqtt {

Client::Client(std::string_view broker, uint16_t port, std::string_view client_name, std::string_view user, std::string_view password)
    : _mqtt(mqtt_client_new()),
      _broker(broker),
      _port(port),
      _broker_address(),
      _name(client_name),
      _user(user),
      _password(password),
      _info()
{
    _info.client_id = _name.c_str();
    _info.client_pass = _password.c_str();
    _info.client_user = _user.c_str();
    _info.keep_alive = KEEP_ALIVE_TIMEOUT_S;
    _info.will_msg = NULL;
    _info.will_qos = 0;
    _info.will_retain = 0;
    _info.will_topic = NULL;
#if LWIP_ALTCP && LWIP_ALTCP_TLS
    _info.tls_config = NULL;
#endif
}

Client::~Client()
{
    // Can I disconnect twice safely? Can I check if I'm connected?
    cyw43_arch_lwip_begin();
    mqtt_disconnect(_mqtt);
    mqtt_client_free(_mqtt);
    cyw43_arch_lwip_end();
}

bool Client::connect()
{
    if (!dns::resolve(_broker.c_str())) {
        printf("Failed to resolve %s when connecting to MQTT\n", _broker.c_str());
        return false;
    }

    mqtt_set_inpub_callback(_mqtt, onTopicUpdated, onDataReceived, LWIP_CONST_CAST(void*, &_info));

    cyw43_arch_lwip_begin();
    err_t error = mqtt_client_connect(_mqtt, &_broker_address, _port, onConnectionComplete, LWIP_CONST_CAST(void*, &_info), &_info);
    cyw43_arch_lwip_end();

    if (error != ERR_OK) {
        printf("Connection to %s unsuccessful (%d)\n", _broker.c_str(), error);
        return false;
    }
    return true;
}

bool Client::disconnect()
{
    cyw43_arch_lwip_begin();
    mqtt_disconnect(_mqtt);
    cyw43_arch_lwip_end();
    return true;
}

bool Client::publish(std::string_view topic, void* payload, uint16_t size, QoS qos, bool retain)
{
    uint8_t qos_value = static_cast<uint8_t>(qos);
    uint8_t retain_value = static_cast<uint8_t>(retain);

    cyw43_arch_lwip_begin();
    err_t error = mqtt_publish(_mqtt, topic.data(), payload, size, qos_value, retain_value, onRequestComplete, NULL);
    cyw43_arch_lwip_end();

    if (error != ERR_OK) {
        printf("Publish of %s unsuccessful (%d)\n", topic.data(), error);
        return false;
    }
    return true;
}
} // namespace mqtt
