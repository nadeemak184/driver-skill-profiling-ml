#include "FS.h"
#include <SPIFFS.h>

#define SENSOR_PIN 12
#define LED_PIN 2
#define RESISTOR_VALUE 10000.0  // Replace with the value of your resistor in ohms
#define SAMPLE_INTERVAL_MS 10   // Sampling interval in milliseconds

File file;

void setup() {
  Serial.begin(115200);
  pinMode(SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  // Initialize SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialization failed!");
    return;
  }
  Serial.println("SPIFFS initialized.");
  
  // Open file for writing
  String filename = "/pressure-" + String(millis()) + ".txt";
  file = SPIFFS.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file");
  }
}

void loop() {
  float voltage = 0.0;

  // Read voltage across the piezo
  for (int i = 0; i < 10; i++) {
    voltage += analogRead(SENSOR_PIN);
    delay(SAMPLE_INTERVAL_MS);
  }
  voltage /= 10.0;
  voltage = (voltage / 4095.0) * 3.3; // Convert ADC value to voltage

  // Calculate pressure in pascal
  float pressure = (voltage / RESISTOR_VALUE) * 1000000.0;

  // Print pressure to serial console
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" Pa");

  // Save pressure to file with date and time
  if (file) {
    file.print(pressure);
    file.println(" Pa");
    file.flush();
  } else {
    Serial.println("Failed to write to file");
  }

  // Blink LED if pressure is above a threshold
  if (pressure > 50.0) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  delay(1000);
}