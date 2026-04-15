#include "mqtt.h"
#include "mqtt_client.h"
#include "fault.h"
#include "esp_log.h"

static const char *TAG        = "MQTT";

static esp_mqtt_client_handle_t client;


/*  Fault code → human-readable string  */
static const char *fault_to_str(uint8_t fault)
{
    switch (fault) {
        case FAULT_NORMAL:    return "NORMAL";
        case FAULT_DRIVER_IC: return "DRIVER_IC_FAULT";
        case FAULT_OVERHEAT:  return "OVERHEAT";
        default:              return "UNKNOWN";
    }
}

/* mqtt_init  */
void mqtt_init(void)
{
    esp_mqtt_client_config_t config = {
        .broker.address.uri = "mqtt://broker.hivemq.com",
    };

    client = esp_mqtt_client_init(&config);
    ESP_ERROR_CHECK(esp_mqtt_client_start(client));

    ESP_LOGI(TAG, "MQTT client started – broker: %s", "mqtt://broker.hivemq.com");
}

/* mqtt_publish_data */
void mqtt_publish_data(uint8_t temp, uint8_t fault)
{
    char msg[64];

    /* snprintf prevents buffer overflow */
    snprintf(msg, sizeof(msg), "Temp:%u,Fault:%s", temp, fault_to_str(fault));

    int msg_id = esp_mqtt_client_publish(client, "esp32/data", msg, 0, 1, 0);

    if (msg_id < 0) {
        ESP_LOGW(TAG, "MQTT publish failed");
    } else {
        ESP_LOGD(TAG, "Published [%s] → %s  (msg_id=%d)", "esp32/data", msg, msg_id);
    }
}