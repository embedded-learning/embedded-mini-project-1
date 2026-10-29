#include "light_sensor.h"

static light_sensor_config_t lightSensorConfig;
static adc_oneshot_unit_handle_t lightSensorAdcHandle = NULL;

esp_err_t light_sensor_init(const light_sensor_config_t *config)
{
    if (config->full_light_raw_value <= 0) {
        return ESP_ERR_INVALID_ARG;
    }
    lightSensorConfig = *config;
    adc_oneshot_unit_init_cfg_t unitConfig = {
        .unit_id = lightSensorConfig.adc_unit,
    };
    esp_err_t unitStatus = adc_oneshot_new_unit(&unitConfig, &lightSensorAdcHandle);
    if (unitStatus != ESP_OK) {
        return unitStatus;
    }

    adc_oneshot_chan_cfg_t channelConfig = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    return adc_oneshot_config_channel(lightSensorAdcHandle, lightSensorConfig.adc_channel, &channelConfig);
}

static uint32_t convertRawValueToLightPercent(int rawValue)
{
    uint32_t voltagePercent = rawValue * 100 / lightSensorConfig.full_light_raw_value;
    if (voltagePercent > 100) {
        voltagePercent = 100;
    }
    return lightSensorConfig.is_higher_voltage_darker ? 100 - voltagePercent : voltagePercent;
}

esp_err_t light_sensor_read(int *raw_value, uint32_t *light_percent)
{
    esp_err_t readStatus = adc_oneshot_read(lightSensorAdcHandle, lightSensorConfig.adc_channel, raw_value);
    if (readStatus != ESP_OK) {
        return readStatus;
    }
    *light_percent = convertRawValueToLightPercent(*raw_value);
    return ESP_OK;
}
