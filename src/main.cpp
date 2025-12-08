#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <map>

#define MQTT_MAX_PACKET_SIZE 512

#include "motor.h"
#include "loadcell.h"
#include "servo.h"
#include "mqtt.h"
#include "doorsensor.h"
#include "thermometer.h"
// ↓↓↓ DEFINITIONS HERE ↓↓↓
const char* WIFI_SSID  = "Day";
const char* WIFI_PASS  = "daydoiwifi";

const char* MQTT_SERVER = "broker.hivemq.com";
const char* MQTT_TOPIC  = "flybrary/F1/borrow";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

bool shouldVend = false;
String isbn;
bool gateOpen = false;
unsigned long gateOpenStart = 0;
unsigned long gateTimeout = 5000; // 5 seconds before auto-close

//edit this to fit the pos on the real thing
std::map<String, int> isbnToPosition = {
    {"9786165972000", 0}, //erika
    {"9786166241174", 1}, //muse 1
    {"9786166241327", 2}, //muse 2
    {"9786166241334", 3}, //muse 3
    {"9786166240771", 4}  //first 4
};

//Init sensors and actuators
Stepper st(18,19);
BorrowServo s(12);
DoorSensor ds(22,23);

Thermo thermo(34);

float distanceCheck = 20;

void receiveBook(int pos);

// MQTT Callback
void mqttCallback(char* topic, byte* payload, unsigned int length) {
    Serial.print("[MQTT] Message received on: ");
    Serial.println(topic);

    // Convert payload to string
    String jsonStr;
    for (int i = 0; i < length; i++) {
        jsonStr += (char)payload[i];
    }

    Serial.print("[MQTT] Payload String: ");
    Serial.println(jsonStr);

    // Allocate JSON document
    StaticJsonDocument<200> doc;

    // Parse JSON
    DeserializationError error = deserializeJson(doc, jsonStr);
    if (error) {
        Serial.print("JSON parse failed: ");
        Serial.println(error.f_str());
        return;
    }

    // Extract data
    isbn = doc["isbn"].as<String>();

    Serial.print("Extracted ISBN: ");
    Serial.println(isbn);

    // Use it inside your program
    shouldVend = true;
}


// ---------------------------
// WiFi Setup
// ---------------------------
void setupWiFi() {
    Serial.print("[WiFi] Connecting to ");
    Serial.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\n[WiFi] Connected!");
    Serial.print("[WiFi] IP: ");
    Serial.println(WiFi.localIP());
}

// ---------------------------
// MQTT Reconnect
// ---------------------------
void reconnectMQTT() {
    while (!mqttClient.connected()) {
        Serial.print("[MQTT] Attempting connection... ");
        String clientId = "FlybraryClient-" + String(WiFi.macAddress());
        if (mqttClient.connect(clientId.c_str())) {
            Serial.println("connected!");
            mqttClient.subscribe(MQTT_TOPIC);
            Serial.print("[MQTT] Subscribed to: ");
            Serial.println(MQTT_TOPIC);
        } else {
            Serial.print("failed (rc=");
            Serial.print(mqttClient.state());
            Serial.println(") retrying in 2s...");
            delay(2000);
        }
    }
}

// ---------------------------
// Initialize MQTT System
// ---------------------------
void initMQTT() {
    setupWiFi();

    mqttClient.setServer(MQTT_SERVER, 1883);
    mqttClient.setCallback(mqttCallback);

    reconnectMQTT();
}

// ---------------------------
// Must be called inside loop()
// ---------------------------
void mqttLoop() {
    if (!mqttClient.connected()) reconnectMQTT();
    mqttClient.loop();

    if (shouldVend) {
        Serial.println("Data received, systems run now");
        receiveBook(isbnToPosition[isbn]);
        shouldVend = false;
        
    }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Flybrary ESP32 initing.");
  initMQTT();
  Serial.println("Flybrary ESP32 started.");
}

bool isDetected = false;

void loop() {
    mqttLoop();  // always run
    delay(1000);

    float dis = ds.getDistance();

    if (gateOpen) {
        // If hand detected, reset timer (keep gate open)
        if (dis > 0.01 && dis <= 12.0) {
            Serial.println("Hand detected -> Keeping gate open");
            gateOpenStart = millis(); 
        }

        // Time to close?
        if (millis() - gateOpenStart >= gateTimeout) {
            Serial.println("Closing Gate");
            s.close();
            gateOpen = false;
        }
    }
}

bool sendTemperature(float temp);

void receiveBook(int pos) {
    Serial.print("Receiving book at slot: ");
    Serial.println(pos);

    Serial.println("Opening Gate");
    s.open();
    
    float t = thermo.readTempCelsius();
    Serial.print("Thermo: ");
    Serial.println(t);
    sendTemperature(t);
    gateOpen = true;
    gateOpenStart = millis();  // start countdown
}

bool sendTemperature(float temp) {
    if (WiFi.status() != WL_CONNECTED) return false;

    HTTPClient http;
    http.begin("https://flybrary-web-backend.vercel.app/api/v1/esp/postTemperature");
    http.addHeader("Content-Type", "application/json");

    // Correct JSON
    String json = "{\"temperature\": " + String(temp, 2) + "}";

    int code = http.POST(json);
    http.end();

    Serial.print("Sent body: ");
    Serial.println(json);
    Serial.print("Status code: ");
    Serial.println(code);

    return (code == 200 || code == 201);
}


/*
The user press return book in the website
It calls mqtt client
If esp receive the mqtt, it opens the gate
If gate is opened, it opens for 5 secs
then, detect with ultrasonic to prevent hand in gate
if no hand, close gate
*/