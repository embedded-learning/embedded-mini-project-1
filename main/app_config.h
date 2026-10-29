#pragma once

#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "soc/adc_channel.h"

#define RELAY_GPIO GPIO_NUM_21
#define RELAY_ACTIVE_LEVEL 1

#define LIGHT_SENSOR_ADC_UNIT ADC_UNIT_1
#define LIGHT_SENSOR_ADC_CHANNEL ADC1_GPIO1_CHANNEL
#define IS_HIGHER_VOLTAGE_DARKER false
// Сырое значение ADC при полностью освещённой комнате, принимается за 100 %.
// Снять из лога `raw=` при полном освещении и подставить.
#define LIGHT_SENSOR_FULL_LIGHT_RAW_VALUE 3400

#define LIGHT_PERCENT_TURN_ON_THRESHOLD 60
#define LIGHT_PERCENT_TURN_OFF_THRESHOLD 80

#define LIGHT_SENSOR_POLL_INTERVAL_MS 100
#define LIGHT_SENSOR_MAX_CONSECUTIVE_READ_FAILURES 5
#define LOG_INTERVAL_MS 1000
