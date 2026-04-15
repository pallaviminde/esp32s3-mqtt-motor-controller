#include "can_handler.h"
#include "esp_log.h"

static const char *TAG = "CAN_MOTOR";

#define CAN_TX_GPIO 16
#define CAN_RX_GPIO 15

/* Minimum expected data bytes in a sensor frame  */
#define CAN_MIN_DLC     2   /* data[0]=temp, data[1]=fault */

void can_init() {
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(CAN_TX_GPIO, CAN_RX_GPIO, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config, &f_config));
    ESP_ERROR_CHECK(twai_start());

    ESP_LOGI(TAG, "TWAI driver started – TX GPIO%d  RX GPIO%d  500 kbps",
             CAN_TX_GPIO, CAN_RX_GPIO);
}

bool can_receive(uint8_t *temp, uint8_t *fault) {
    twai_message_t msg;

    esp_err_t err = twai_receive(&msg, pdMS_TO_TICKS(600));

    if (err != ESP_OK) {
        ESP_LOGW(TAG, "CAN RX timeout/error: %s", esp_err_to_name(err));
        return false;
    }

    /* Guard: make sure the frame carries the bytes we expect */
    if (msg.data_length_code < CAN_MIN_DLC) {
        ESP_LOGW(TAG, "CAN RX frame too short: DLC=%d (expected >=%d)",
                 msg.data_length_code, CAN_MIN_DLC);
        return false;
    }

    *temp  = msg.data[0];
    *fault = msg.data[1];

    ESP_LOGD(TAG, "CAN RX – temp=%d°C  fault=%d", *temp, *fault);
    return true;
}

