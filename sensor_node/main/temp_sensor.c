#include "temp_sensor.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

static const char *TAG = "TEMP_SENSOR";
static adc_oneshot_unit_handle_t adc_handle;

#define ADC_REF_MV          3300.0f
#define ADC_MAX_COUNT       4095.0f
#define LM35_MV_PER_DEGREE  10.0f



void temp_sensor_init(void) {
    adc_oneshot_unit_init_cfg_t init_cfg = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_cfg, &adc_handle));

    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten    = ADC_ATTEN_DB_12,   /* 0–3.3 V input range */
        .bitwidth = ADC_BITWIDTH_12,   /* 12-bit = 0–4095     */
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_CHANNEL_0, &chan_cfg));

    ESP_LOGI(TAG, "Temperature sensor initialised on ADC1 CH0");
}

uint8_t read_temperature(void)
{
    int raw = 0;
    ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_CHANNEL_0, &raw));

    /* Convert ADC count → millivolts → °C */
    float voltage_mv = (raw / ADC_MAX_COUNT) * ADC_REF_MV;
    float temp_c     = voltage_mv / LM35_MV_PER_DEGREE;

    
    ESP_LOGD(TAG, "ADC raw=%d  voltage=%.1f mV  temp=%.1f °C", raw, voltage_mv, temp_c);

    /* Clamp to uint8 range (0–255), sensor is rated 0–100 °C */
    if (temp_c < 0.0f)   temp_c = 0.0f;
    if (temp_c > 255.0f) temp_c = 255.0f;

    return (uint8_t)temp_c;
}