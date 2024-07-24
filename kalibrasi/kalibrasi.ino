#include "HX711.h"

// Pin definitions
const int LOADCELL_DOUT_PIN = D5;
const int LOADCELL_SCK_PIN = D6;

HX711 scale;

void setup() {
  Serial.begin(9600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();    // Initialize the scale
  scale.tare();         // Reset the scale to 0

  Serial.println("Load cell initialized.");
}

void loop() {
  if (scale.is_ready()) {
    long reading = scale.read(); // Get raw reading
    Serial.print("Raw reading: ");
    Serial.println(reading);

    float weight = scale.get_units(10); // Get weight in kg
    Serial.print("Weight: ");
    Serial.print(weight);
    Serial.println(" kg");
  } else {
    Serial.println("HX711 not found.");
  }

  delay(500);
}
