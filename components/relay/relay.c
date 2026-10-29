#include "relay.h"

#include "esp_log.h"

static const char *LOG_TAG = "relay";
static relay_config_t relayConfig;

esp_err_t relay_init(const relay_config_t *config)
{
    relayConfig = *config;
    gpio_config_t relayGpioConfig = {
        .pin_bit_mask = 1ULL << relayConfig.gpio,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    esp_err_t configStatus = gpio_config(&relayGpioConfig);
    if (configStatus != ESP_OK) {
        return configStatus;
    }
    return gpio_set_level(relayConfig.gpio, !relayConfig.active_level);
}

esp_err_t relay_set_state(bool is_on)
{
    uint32_t pinLevel = is_on ? relayConfig.active_level : !relayConfig.active_level;
    esp_err_t setStatus = gpio_set_level(relayConfig.gpio, pinLevel);
    if (setStatus == ESP_OK) {
        ESP_LOGI(LOG_TAG, "Реле %s", is_on ? "включено" : "выключено");
    }
    return setStatus;
}
