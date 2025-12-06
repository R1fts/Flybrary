#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <BlynkSimpleEsp32.h>

#include "motor.h"
#include "loadcell.h"
#include "servo.h"

const int DIR_PIN = 19;
const int STEP_PIN = 18;
const int SERVO_PIN = 12;

Stepper st(18,19);
BorrowServo s(12);

char auth[] = "8nXrHpIG6jMAHm-d1YbgShXNQRlgQWgy";
char ssid[] = "YourWiFi";
char pass[] = "YourPassword";

String backendUrl = "";

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  Serial.println("Flybrary ESP32 started.");
}


void loop() {
  Blynk.run();
  checkBackendRequest();

  delay(2000); // poll every 3 sec (adjust as needed)
}

void checkBackendRequest() {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  http.begin(backendUrl);

  int code = http.GET();
  if (code <= 0) {
    Serial.println("HTTP request failed");
    http.end();
    return;
  }

  String response = http.getString();
  Serial.println("Backend response: " + response);
}
/*
The user first scans QR code near the Flybrary machine to access the web app.
The user then uses the web app to select a book to borrow.
If the transaction is valid, the main bookshelf (stepper) rotates and the gate (servo) opens for the user to pick up their book.	
There are sensors to detect if book is taken, and check if user’s hand is alright
*/