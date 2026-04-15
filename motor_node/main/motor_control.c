#include "motor_control.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "MOTOR";


/* Motor driver GPIO pins */
#define MOTOR_IN1   GPIO_NUM_10
#define MOTOR_IN2   GPIO_NUM_11

/*  motor_init */
void motor_init(void)
{
    ESP_ERROR_CHECK(gpio_set_direction(MOTOR_IN1, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(MOTOR_IN2, GPIO_MODE_OUTPUT));

    /* Start in safe stopped state */
    gpio_set_level(MOTOR_IN1, 0);
    gpio_set_level(MOTOR_IN2, 0);

    ESP_LOGI(TAG, "Motor driver initialised – IN1=GPIO%d  IN2=GPIO%d",
             MOTOR_IN1, MOTOR_IN2);
}

/* motor_start  */
void motor_start(void)
{
    gpio_set_level(MOTOR_IN1, 1);
    gpio_set_level(MOTOR_IN2, 0);
    ESP_LOGI(TAG, "Motor RUNNING");
}

/* motor_stop  */
void motor_stop(void)
{
    gpio_set_level(MOTOR_IN1, 0);
    gpio_set_level(MOTOR_IN2, 0);
    ESP_LOGI(TAG, "Motor STOPPED");
}