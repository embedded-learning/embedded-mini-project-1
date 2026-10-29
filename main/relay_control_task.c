#include "relay_control_task.h"

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"
#include "relay.h"

static void relayControlTask(void *taskParameter)
{
    while (true) {
        uint32_t requestedRelayState = 0;
        xTaskNotifyWait(0, ULONG_MAX, &requestedRelayState, portMAX_DELAY);
        ESP_ERROR_CHECK(relay_set_state(requestedRelayState != 0));
    }
}

TaskHandle_t createRelayControlTask(void)
{
    TaskHandle_t taskHandle = NULL;
    xTaskCreate(relayControlTask, "relay_control", 2048, NULL, 5, &taskHandle);
    return taskHandle;
}
