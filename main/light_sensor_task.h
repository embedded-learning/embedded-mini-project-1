#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void createLightSensorTask(TaskHandle_t relayControlTaskHandle);
