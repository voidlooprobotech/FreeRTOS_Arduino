#include <Arduino_FreeRTOS.h>  // Include FreeRTOS library for multitasking on Arduino
#include <LedControl.h>        // Library to control MAX7219-based LED matrix

// --- Pin definitions ---
#define TRIG_PIN 3            // Ultrasonic sensor trigger pin
#define ECHO_PIN 2            // Ultrasonic sensor echo pin
#define DIN 11                // MAX7219 Data In pin
#define CS 10                 // MAX7219 Chip Select pin
#define CLK 13                // MAX7219 Clock pin
#define BUZZER_PIN 7          // Piezo buzzer pin

// Initialize the LED matrix control (1 device)
LedControl lc = LedControl(DIN, CLK, CS, 1);

// Shared distance variable (volatile to prevent compiler optimization)
volatile int distance_cm = -1;  

float duration_us;  // Variable to store echo pulse duration

// --- 8x8 bitmap representations for digits 0–9 ---
byte digits[10][8] = {
  {0x3C, 0x66, 0x6E, 0x76, 0x66, 0x66, 0x3C, 0x00}, // 0
  {0x18, 0x38, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00}, // 1
  {0x3C, 0x66, 0x06, 0x1C, 0x30, 0x66, 0x7E, 0x00}, // 2
  {0x3C, 0x66, 0x06, 0x1C, 0x06, 0x66, 0x3C, 0x00}, // 3
  {0x0C, 0x1C, 0x3C, 0x6C, 0x7E, 0x0C, 0x0C, 0x00}, // 4
  {0x7E, 0x60, 0x7C, 0x06, 0x06, 0x66, 0x3C, 0x00}, // 5
  {0x1C, 0x30, 0x60, 0x7C, 0x66, 0x66, 0x3C, 0x00}, // 6
  {0x7E, 0x66, 0x0C, 0x18, 0x18, 0x18, 0x18, 0x00}, // 7
  {0x3C, 0x66, 0x66, 0x3C, 0x66, 0x66, 0x3C, 0x00}, // 8
  {0x3C, 0x66, 0x66, 0x3E, 0x06, 0x0C, 0x38, 0x00}  // 9
};

void setup() {
  Serial.begin(9600);                   // Start serial communication for debugging
  pinMode(TRIG_PIN, OUTPUT);           // Set trigger pin as output
  pinMode(ECHO_PIN, INPUT);            // Set echo pin as input

  lc.shutdown(0, false);               // Wake up the LED matrix from power-saving mode
  lc.setIntensity(0, 8);               // Set brightness level (0–15)
  lc.clearDisplay(0);                  // Clear the LED matrix display

  pinMode(BUZZER_PIN, OUTPUT);         // Set buzzer pin as output

  // --- Create FreeRTOS tasks ---
  xTaskCreate(ultrasonicTask, "Ultrasonic", 128, NULL, 1, NULL); // Distance measurement
  xTaskCreate(displayTask,   "Display",    128, NULL, 1, NULL); // LED display
  xTaskCreate(buzzerTask,    "Buzzer",     128, NULL, 1, NULL); // Buzzer warning
}

void loop() {
  // FreeRTOS handles tasks; no need to use loop()
}

// 📏 Task: Measures distance using ultrasonic sensor and updates shared variable
void ultrasonicTask(void *pvParameters) {
  while (1) {
    // Send a 10 µs HIGH pulse to trigger pin
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Read echo duration in microseconds
    duration_us = pulseIn(ECHO_PIN, HIGH);

    // Convert time to distance in centimeters (speed of sound ≈ 0.034 cm/us, half for to-and-fro)
    distance_cm = 0.017 * duration_us;

    // Output the measured distance to the Serial Monitor
    Serial.print("distance: ");
    Serial.print(distance_cm);
    Serial.println(" cm");

    // Delay before the next measurement
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

// 🔁 Task: Displays the measured distance on the 8x8 LED matrix
void displayTask(void *pvParameters) {
  while (1) {
    int d = distance_cm;  // Read the shared distance value

    if (d >= 0 && d <= 9) {
      displayDigit(d);  // Display digit if valid
    } else {
      lc.clearDisplay(0);  // Clear display for out-of-range values
    }

    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

// 🔊 Task: Controls buzzer frequency based on distance (closer = faster beep)
void buzzerTask(void *pvParameters) {
  while (1) {
    int d = distance_cm;

    if (d > 0 && d <= 9) {
      // Map distance to delay: closer objects = shorter delay (faster beep)
      int beepDelay = map(d, 9, 2, 500, 50);  // 9 cm = 500ms, 2 cm = 50ms

      digitalWrite(BUZZER_PIN, HIGH);       // Turn buzzer ON
      vTaskDelay(100 / portTICK_PERIOD_MS); // Buzzer ON duration
      digitalWrite(BUZZER_PIN, LOW);        // Turn buzzer OFF
      vTaskDelay(beepDelay / portTICK_PERIOD_MS); // Delay between beeps
    } else {
      digitalWrite(BUZZER_PIN, LOW);        // Turn buzzer OFF if out of range
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
}

// 🔢 Function: Displays a single digit rotated 90° clockwise on the LED matrix
void displayDigit(int num) {
  for (int x = 0; x < 8; x++) {
    byte colData = 0;

    // Transpose and flip bits to rotate digit 90° clockwise
    for (int y = 0; y < 8; y++) {
      if (digits[num][y] & (1 << x)) {
        colData |= (1 << (7 - y));
      }
    }

    // Set the row (originally column before rotation) on the display
    lc.setRow(0, x, colData);
  }
}
