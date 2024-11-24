# ESP32-WROOM-32 + FC-28 Proof of Concept

## 개요

이 프로젝트는 **ESP32-WROOM-32**와 **FC-28 토양 습윤 센서**를 활용하여 토양 습윤도를 측정하고, 이를 시리얼로 출력하거나 **MQTT 브로커**로 송신하는 과정을 다룹니다. 이를 통해 토양 습윤 데이터를 수집하고 IoT 기능을 구현합니다.


## 회로 구성

### 연결 정보

| FC-28 핀 | ESP32 핀 | 설명                  |
|----------|----------|-----------------------|
| VCC      | 3.3V     | ESP32의 3.3V 핀 연결 |
| GND      | GND      | ESP32의 GND 핀 연결  |
| AO       | GPIO 32  | ESP32의 아날로그 핀  |

![회로도](https://github.com/user-attachments/assets/ed169472-3413-4412-ab54-710e44bad18d)

## 주요 코드

### 토양 습윤 값 변환 및 출력
FC-28 센서의 값을 읽어 0~100%로 변환 후 시리얼로 출력합니다.

```cpp
// 값 변환 (0 ~ 100%)
float soilMoisturePercent = map(soilMoistureValue, 4095, 0, 0, 100);
Serial.print("Soil Moisture (%): ");
Serial.println(soilMoisturePercent);
```

- **특징:**  
  `map` 함수를 사용하여 값의 범위를 변환하며, 입력 범위를 반전시켜 원하는 결과를 얻을 수 있습니다.


### 결과

- **물에 넣지 않았을 때:**
  ![image](https://github.com/user-attachments/assets/0732035f-fba3-4d70-8a74-2bf4dc4e5e9c)

- **물에 넣었을 때:**
  ![image](https://github.com/user-attachments/assets/682f7222-4870-40ee-b0fa-9995039f217b)


### MQTT 데이터 송신
센서 값을 MQTT 브로커로 송신합니다. MQTT 설정은 소스 코드 내에서 지정하며, 상세한 설정 방법은 [PR 링크](https://github.com/slicequeue/poc-esp32-fc28-mqtt/pull/2#discussion_r1855326663)를 참고하세요.


## 환경 설정

### Wi-Fi 설정

Wi-Fi 정보는 프로젝트의 `key.h` 파일에서 설정합니다.

```cpp
const char* ssid = "<Wi-Fi 네트워크 이름>";
const char* password = "<Wi-Fi 비밀번호>";
```


## 학습 포인트

1. **아날로그 값 변환:**  
   `map` 함수로 센서의 값을 0~100% 비율로 변환하며, 입력 최대값과 최소값의 반전을 통해 다양한 매핑 방식 적용 가능.

2. **IoT 통신:**  
   MQTT 프로토콜을 사용하여 수집된 데이터를 브로커로 송신.


## 작업 단계별 소스 코드 확인

- https://github.com/slicequeue/poc-esp32-fc28-mqtt/pull/1
- https://github.com/slicequeue/poc-esp32-fc28-mqtt/pull/2

