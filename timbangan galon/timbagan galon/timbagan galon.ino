#include <ESP8266Firebase.h>
#include <ESP8266WiFi.h>
#include "HX711.h"

HX711 scale;

#define _SSID "AiraCerdas2"                // Your WiFi SSID
#define _PASSWORD "11111111"      // Your WiFi Password
const int max_attempts = 10;

#define REFERENCE_URL "https://airacerdas-98096-default-rtdb.firebaseio.com/"
Firebase firebase(REFERENCE_URL);

WiFiClient client;

float weight;
float calibration_factor = 109293.91;

bool isScaleConnected = false;

void setup() {
  Serial.begin(9600);

  scale.begin(D5, D6);
  scale.set_scale();
  scale.tare();
  long zero_factor = scale.read_average();

  // Check if the scale is connected by reading a value
  scale.set_scale(calibration_factor);
  weight = scale.get_units(5);
  if (weight == 0) {
    Serial.println("HX711 tidak terkoneksi atau berat terbaca 0.");
  } else {
    isScaleConnected = true;
  }

  Serial.println("Menghubungkan ke WiFi...");
  WiFi.begin(_SSID, _PASSWORD);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < max_attempts) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi terhubung");
    Serial.println("Alamat IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("Gagal menghubungkan ke WiFi");
  }
}

void loop() {
  if (isScaleConnected) {
    scale.set_scale(calibration_factor);
    weight = scale.get_units(5);
    if (weight < 0) {
      weight = 0;
    }

    String status;
    if (weight < 0.5) {
      status = "Air Galon Habis!";
    } else if (weight >= 0.5 && weight < 4.0) {
      status = "Air Galon Hampir Habis!";
    } else {
      status = "Yeay air galon sudah terisii :)";
    }

    Serial.print("Berat: ");
    Serial.print(weight);
    firebase.setFloat("Ruang_dosen/berat", weight);  

    Serial.print(" KG, Status: ");
    Serial.println(status);
    firebase.setString("Ruang_dosen/status", status);
    Serial.println();
  } else {
    Serial.println("HX711 tidak terkoneksi, tidak dapat mengukur berat.");
  }

  delay(200); // Optional: delay to avoid spamming the serial monitor
}
