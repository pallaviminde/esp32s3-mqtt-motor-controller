#include "can_handler.h"
#include "temp_sensor.h"
#include "esp_log.h"
#include "fault.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "SENSOR_MAIN";

#define TEMP_LIMIT_C 50

void app_main() 
{
    ESP_LOGI(TAG, "Sensor node starting...");

    can_init();
    temp_sensor_init();

    ESP_LOGI(TAG, "Initialisation complete. Sending CAN frames every %d ms.",
             500);

    while (1) {
        uint8_t temp = read_temperature();
        uint8_t fault = FAULT_NORMAL;

        if (temp > TEMP_LIMIT_C) {
            fault = FAULT_OVERHEAT;
            ESP_LOGW(TAG, "OVERHEAT detected! temp=%d°C", temp);
        }

        can_send(temp, fault);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

 
    