#include "app_config.h"
#include "esp_err.h"
#include "esp_log.h"
#include "light_sensor.h"
#include "light_sensor_task.h"
#include "relay.h"
#include "relay_control_task.h"

static const char *LOG_TAG = "light-switch";

void app_main(void)
{
    relay_config_t relayConfig = {
        .gpio = RELAY_GPIO,
        .active_level = RELAY_ACTIVE_LEVEL,
    };
    ESP_ERROR_CHECK(relay_init(&relayConfig));

    light_sensor_config_t lightSensorConfig = {
        .adc_unit = LIGHT_SENSOR_ADC_UNIT,
        .adc_channel = LIGHT_SENSOR_ADC_CHANNEL,
        .is_higher_voltage_darker = IS_HIGHER_VOLTAGE_DARKER,
        .full_light_raw_value = LIGHT_SENSOR_FULL_LIGHT_RAW_VALUE,
    };
    ESP_ERROR_CHECK(light_sensor_init(&lightSensorConfig));

    TaskHandle_t relayControlTaskHandle = createRelayControlTask();
    createLightSensorTask(relayControlTaskHandle);

    ESP_LOGI(LOG_TAG, "Запущено: порог вкл < %d%%, порог выкл > %d%%",
             LIGHT_PERCENT_TURN_ON_THRESHOLD, LIGHT_PERCENT_TURN_OFF_THRESHOLD);
}
