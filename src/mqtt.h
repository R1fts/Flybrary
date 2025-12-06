#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <WiFi.h>
#include <PubSubClient.h>

// ===== CONFIG =====
extern const char* WIFI_SSID;
extern const char* WIFI_PASS;

extern const char* MQTT_SERVER;
extern const char* MQTT_TOPIC;

// ===== MQTT STATE =====
extern bool shouldVend;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// ---------------------------
// Dummy function — replace later
// ---------------------------
void vendBook() {
    Serial.println(">>> VENDING BOOK <<<");
}

// ---------------------------
// MQTT Callback
// ---------------------------
void mqttCallback(char* topic, byte* payload, unsigned int length) {
    Serial.print("[MQTT] Message received on: ");
    Serial.println(topic);

    shouldVend = true;  // activate vending flag

    Serial.print("[MQTT] Payload: ");
    for (int i = 0; i < length; i++) Serial.print((char)payload[i]);
    Serial.println();
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

        if (mqttClient.connect("FlybraryClient")) {
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
        vendBook();
        shouldVend = false;
    }
}

#endif
