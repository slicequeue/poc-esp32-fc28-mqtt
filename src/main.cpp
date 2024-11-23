#include <Arduino.h>

#define FC28PIN 32

/*
 * FC32 토양 습윤 센서 값을 측정하고 
 * 그 값을 MQTT 브로커 서버로 값을 전달합니다.
 */
void setup() {
  Serial.begin(115200);
  Serial.println("fc28 test start");
}

void loop() {

  int soilMoistureValue = analogRead(FC28PIN);
  Serial.print("Soil Moisture Value: ");
  Serial.println(soilMoistureValue);


  // 값 변환 (0 ~ 100%)
  float soilMoisturePercent = map(soilMoistureValue, 4095, 0, 0, 100);
  Serial.print("Soil Moisture (%): ");
  Serial.println(soilMoisturePercent);

  delay(1000);

}