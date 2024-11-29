#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include <PubSubClient.h>
#include "key.h"

// MQTT 클라이언트 객체 생성
extern PubSubClient client;
// MQTT 초기화 함수
void setupMQTT(PubSubClient& mqttClient, const char* server, int port);
// MQTT 연결 함수
void reconnectMQTT();
// MQTT 메시지 수신 콜백 함수
void mqttCallback(char* topic, byte* payload, unsigned int length);

#endif