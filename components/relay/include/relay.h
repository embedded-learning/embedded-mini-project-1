#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "driver/gpio.h"
#include "esp_err.h"

typedef struct {
    gpio_num_t gpio;
    uint32_t active_level;
} relay_config_t;

esp_err_t relay_init(const relay_config_t *config);
esp_err_t relay_set_state(bool is_on);
