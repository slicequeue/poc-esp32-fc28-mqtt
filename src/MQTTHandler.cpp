#include "MQTTHandler.h"
#include "key.h"

// MQTT 메시지 수신 콜백 함수
void mqttCallback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("]: ");
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}


// MQTT 초기화 함수
void setupMQTT(PubSubClient& mqttClient, const char* server, int port) {
    mqttClient.setServer(server, port);
    mqttClient.setCallback(mqttCallback);
}

// MQTT 연결 함수
void reconnectMQTT() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);

        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
            client.subscribe(mqtt_topic_input);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 5 seconds...");
            delay(5000);
        }
    }
}
