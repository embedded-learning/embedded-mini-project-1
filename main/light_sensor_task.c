#include "light_sensor_task.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

#include "app_config.h"
#include "esp_err.h"
#include "esp_log.h"
#include "light_sensor.h"

static const char *LOG_TAG = "light-sensor";

static bool decideRelayState(uint32_t lightPercent, bool isRelayOnNow)
{
    if (lightPercent < LIGHT_PERCENT_TURN_ON_THRESHOLD) {
        return true;
    }
    if (lightPercent > LIGHT_PERCENT_TURN_OFF_THRESHOLD) {
        return false;
    }
    return isRelayOnNow;
}

static void lightSensorTask(void *taskParameter)
{
    TaskHandle_t relayControlTaskHandle = (TaskHandle_t)taskParameter;
    bool isRelayOn = false;
    TickType_t lastLogTick = 0;
    uint32_t consecutiveReadFailureCount = 0;

    while (true) {
        int rawValue = 0;
        uint32_t lightPercent = 0;
        esp_err_t readStatus = light_sensor_read(&rawValue, &lightPercent);
        if (readStatus != ESP_OK) {
            consecutiveReadFailureCount++;
            ESP_LOGW(LOG_TAG, "Чтение ADC не удалось (%s), попытка %" PRIu32 " из %d",
                     esp_err_to_name(readStatus), consecutiveReadFailureCount,
                     LIGHT_SENSOR_MAX_CONSECUTIVE_READ_FAILURES);
            if (consecutiveReadFailureCount >= LIGHT_SENSOR_MAX_CONSECUTIVE_READ_FAILURES) {
                ESP_ERROR_CHECK(readStatus);
            }
            vTaskDelay(pdMS_TO_TICKS(LIGHT_SENSOR_POLL_INTERVAL_MS));
            continue;
        }
        consecutiveReadFailureCount = 0;

        TickType_t nowTick = xTaskGetTickCount();
        if (nowTick - lastLogTick >= pdMS_TO_TICKS(LOG_INTERVAL_MS)) {
            lastLogTick = nowTick;
            ESP_LOGI(LOG_TAG, "raw=%d свет=%" PRIu32 "%% реле=%s",
                     rawValue, lightPercent, isRelayOn ? "вкл" : "выкл");
        }

        bool shouldRelayBeOn = decideRelayState(lightPercent, isRelayOn);
        if (shouldRelayBeOn != isRelayOn) {
            isRelayOn = shouldRelayBeOn;
            xTaskNotify(relayControlTaskHandle, isRelayOn ? 1 : 0, eSetValueWithOverwrite);
        }

        vTaskDelay(pdMS_TO_TICKS(LIGHT_SENSOR_POLL_INTERVAL_MS));
    }
}

void createLightSensorTask(TaskHandle_t relayControlTaskHandle)
{
    xTaskCreate(lightSensorTask, "light_sensor", 4096, (void *)relayControlTaskHandle, 10, NULL);
}
