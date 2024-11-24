#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "key.h"

const char* mqtt_server = "broker.emqx.io";
const char* mqtt_topic = "nockanda/esp32/input";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long mydelay = 0;

#define FC28PIN 32


void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  // MQTT 메세지 수신했을때 처리하는 부분
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // client.publish("outTopic", "hello world"); // 불필요
      // ... and resubscribe
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


/*
 * FC32 토양 습윤 센서 값을 측정하고 
 * 그 값을 MQTT 브로커 서버로 값을 전달합니다.
 */
void setup() {
  Serial.begin(115200);
  Serial.println("fc28 test mqtt start");

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}// end of setup

void loop() {

  // 반드시 필요한 부분이고 계속 호출 처리
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // delay 함수를 사용하지 않고 delay를 거는 blocking되지 않는 코드
  if (millis() - mydelay > 2000)
  {
    mydelay = millis();

    int soilMoistureValue = analogRead(FC28PIN);
    Serial.print("Soil Moisture Value: ");
    Serial.println(soilMoistureValue);

    // 값 변환 (0 ~ 100%)
    float soilMoisturePercent = map(soilMoistureValue, 4095, 0, 0, 100);
    Serial.print("Soil Moisture (%): ");
    Serial.println(soilMoisturePercent);

    // MQTT 발행
    String data = "토양 습도 (%): " + String(soilMoisturePercent) + "%";
    client.publish("nockanda/esp32/output", data.c_str());
  }
}// end of loop
