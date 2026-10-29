#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "esp_adc/adc_oneshot.h"
#include "esp_err.h"

typedef struct {
    adc_unit_t adc_unit;
    adc_channel_t adc_channel;
    bool is_higher_voltage_darker;
    int full_light_raw_value;
} light_sensor_config_t;

esp_err_t light_sensor_init(const light_sensor_config_t *config);
esp_err_t light_sensor_read(int *raw_value, uint32_t *light_percent);
