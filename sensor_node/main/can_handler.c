#include "can_handler.h"
#include "esp_log.h"

static const char *TAG = "CAN_SENSOR";

#define CAN_TX_GPIO 16
#define CAN_RX_GPIO 15

void can_init(void)
{
    twai_general_config_t g_config =
        TWAI_GENERAL_CONFIG_DEFAULT(CAN_TX_GPIO, CAN_RX_GPIO, TWAI_MODE_NORMAL);
 
    twai_timing_config_t  t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t  f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
 
    ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config, &f_config));
    ESP_ERROR_CHECK(twai_start());
 
    ESP_LOGI(TAG, "TWAI driver started – TX GPIO%d  RX GPIO%d  500 kbps",
             CAN_TX_GPIO, CAN_RX_GPIO);
}

void can_send(uint8_t temp, uint8_t fault) {
    twai_message_t msg;

    msg.identifier = 0x100;
    msg.data_length_code = 2;

    msg.data[0] = temp;
    msg.data[1] = fault;
 
    esp_err_t err = twai_transmit(&msg, pdMS_TO_TICKS(10));
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "CAN TX failed: %s", esp_err_to_name(err));
    } else {
        ESP_LOGD(TAG, "CAN TX – temp=%d°C  fault=%d", temp, fault);
    }
}

