#include <stdio.h>
#include "can_handler.h"
#include "motor_control.h"
#include "wifi.h"
#include "mqtt.h"
#include "fault.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "MOTOR_MAIN";
#define TEMP_LIMIT_C 50

void app_main()
 {
    ESP_LOGI(TAG, "Motor node starting...");
    can_init();
    motor_init();
    wifi_init();
    mqtt_init();

    ESP_LOGI(TAG, "Initialisation complete. Waiting for CAN frames...");

     uint8_t temp  = 0;
     uint8_t fault = 0;

    while (1)
    {
        /* Only act when a valid CAN frame arrives.
         * can_receive() blocks up to 600 ms; returns false on timeout.     */
        if (!can_receive(&temp, &fault)) {
            /* No frame received – keep the last known motor state.
             * Optionally escalate to a watchdog fault here.                 */
            ESP_LOGW(TAG, "No CAN frame received – holding last state");
            continue;
        }

        /* Motor decision ─────────────────────────────────────────────────── */
        if (fault == FAULT_OVERHEAT || temp > TEMP_LIMIT_C) {
            motor_stop();
        } else {
            motor_start();
        }

        /* Publish to cloud */
        mqtt_publish_data(temp, fault);
    }
}