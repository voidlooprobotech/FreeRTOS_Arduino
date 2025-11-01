#include <Arduino_FreeRTOS.h>
#include "timers.h"

#define ONE_SHOT_PERIOD pdMS_TO_TICKS(5000)

TimerHandle_t xTimer;

void setup() {
  Serial.begin(9600);

  xTimer = xTimerCreate(
    "One shot timer",         // Timer name (optional)
    ONE_SHOT_PERIOD,          // Timer period (in ticks)
    pdFALSE,                  // One-shot timer (not auto-reload)
    0,                        // Timer ID (not used here)
    prvOneShotTimerCallback   // Callback function
  );

  xTimerStart(xTimer, 0);     // Start the timer with no block time
}

void prvOneShotTimerCallback(TimerHandle_t xTimer) {
  Serial.println("One shot timer callback function executed......");
}

void loop() {
  // Main loop left empty for FreeRTOS task scheduling
}
