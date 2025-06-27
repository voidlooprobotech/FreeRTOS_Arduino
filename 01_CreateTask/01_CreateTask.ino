/*
  ------------------------------------------------------------
  FreeRTOS LED Blinking Tasks on Arduino Nano
  ------------------------------------------------------------
  Created for: Void Loop Robotech and Automation (YouTube Channel)
  Tutorial Title: How to Create Tasks using FreeRTOS on Arduino Nano
  Description : This sketch demonstrates multitasking using FreeRTOS.
                Three tasks are created to blink Red, Green, and Blue LEDs.
  ------------------------------------------------------------
*/

// Include the FreeRTOS library for Arduino
#include <Arduino_FreeRTOS.h>

// Define the digital pins connected to the Red, Green, and Blue LEDs
#define RLED   10
#define GLED   11
#define BLED   12

// Function prototypes for the LED control tasks
void vRedLedTask(void * pvParameters);
void vGreenLedTask(void * pvParameters);
void vBlueLedTask(void * pvParameters);

void setup() {
  // Create three tasks for controlling the Red, Green, and Blue LEDs
  // Arguments: task function, task name (for debugging), stack size, parameters (none), priority, and task handle (none)
  xTaskCreate(vRedLedTask, "Red Led Controller", 100, NULL, 1, NULL);
  xTaskCreate(vGreenLedTask, "Green Led Controller", 100, NULL, 1, NULL);
  xTaskCreate(vBlueLedTask, "Blue Led Controller", 100, NULL, 1, NULL);

  // No setup code needed after task creation, tasks will run independently
}

void loop() {
  // Empty loop – FreeRTOS takes control after setup
  // All logic is handled by the tasks created above
}

// Task to toggle Red LED at regular intervals
void vRedLedTask(void * pvParameters)
{
  // Set the Red LED pin as output
  pinMode(RLED, OUTPUT);

  // Infinite loop for task
  while (1)
  {
    // Toggle Red LED
    digitalWrite(RLED, !digitalRead(RLED));
    // Wait for 100 milliseconds
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// Task to toggle Green LED at regular intervals
void vGreenLedTask(void * pvParameters)
{
  // Set the Green LED pin as output
  pinMode(GLED, OUTPUT);

  // Infinite loop for task
  while (1)
  {
    // Toggle Green LED
    digitalWrite(GLED, !digitalRead(GLED));
    // Wait for 100 milliseconds
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// Task to toggle Blue LED at regular intervals
void vBlueLedTask(void * pvParameters)
{
  // Set the Blue LED pin as output
  pinMode(BLED, OUTPUT);

  // Infinite loop for task
  while (1)
  {
    // Toggle Blue LED
    digitalWrite(BLED, !digitalRead(BLED));
    // Wait for 100 milliseconds
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
