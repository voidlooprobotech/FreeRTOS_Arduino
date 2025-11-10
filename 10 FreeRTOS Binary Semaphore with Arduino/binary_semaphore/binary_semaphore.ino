#include <Arduino_FreeRTOS.h>
#include "semphr.h"

SemaphoreHandle_t xBinarySemaphore;

void setup() {
  Serial.begin(9600);

  xBinarySemaphore = xSemaphoreCreateBinary();

  xTaskCreate(RedLedTask,   "Red LED Control",   100, NULL, 1, NULL);
  xTaskCreate(GreenLedTask, "Green LED Control", 100, NULL, 1, NULL);
  xTaskCreate(BlueLedTask,  "Blue LED Control",  100, NULL, 1, NULL);
}

void RedLedTask(void *pvParameters) 
{
  xSemaphoreGive(xBinarySemaphore);
  while (1) 
  {
    xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
    Serial.println("This is Red Led Task");
    xSemaphoreGive(xBinarySemaphore);
    vTaskDelay(1);
  }
}
void GreenLedTask(void *pvParameters) 
{
  while (1) 
  {
    xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
    Serial.println("This is Green Led Task");
    xSemaphoreGive(xBinarySemaphore);
    vTaskDelay(1);
  }
}
void BlueLedTask(void *pvParameters) 
{
  while (1) 
  {
    xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
    Serial.println("This is Blue Led Task");
    xSemaphoreGive(xBinarySemaphore);
    vTaskDelay(1);
  }
}

void loop() {

}
