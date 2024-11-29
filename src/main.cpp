#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "WiFiHandler.h"
#include "MQTTHandler.h"

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long previousMillis = 0;
const unsigned long interval = 10 * 60 * 1000;

#define FC28PIN 32

/*
 * FC32 토양 습윤 센서 값을 측정하고 
 * 그 값을 MQTT 브로커 서버로 값을 전달합니다.
 */
void setup() {
  Serial.begin(115200);
  Serial.println("FC28 MQTT Test Start");

  setup_wifi();                              // WiFi 연결 설정
  setupMQTT(client, "broker.emqx.io", 1883); // MQTT 초기화
}// end of setup

void loop() {

  // 반드시 필요한 부분이고 계속 호출 처리
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  // delay 함수를 사용하지 않고 delay를 거는 blocking되지 않는 코드
  if ((millis() - previousMillis) > interval) // 30 초에 1번씩 수행
  {
    previousMillis = millis();

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
