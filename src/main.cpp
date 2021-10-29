#include <Arduino.h>

#if defined(ESP8266)
	#pragma message "ESP8266"
	#include <ESP8266WiFi.h>
	#include <espnow.h>
#elif defined(ESP32)
	#pragma message "ESP32"
	#include "WiFi.h"
	#include <esp_now.h>
#else
	#error "Plataforma incorrecta"
#endif

//Esctructura de datos
typedef struct struct_message 
{
	char a[32];
	int b;
	float c;
	bool d;
} struct_message;

//Variable de tipo estructura 
struct_message myData;

// callback function that will be executed when data is received
#if defined(ESP8266)
	void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
#elif defined(ESP32)
	void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
#endif
	memcpy(&myData, incomingData, sizeof(myData));
	Serial.print("Bytes recibidos: ");
	Serial.println(len);
	Serial.print("Char: ");
	Serial.println(myData.a);
	Serial.print("Int: ");
	Serial.println(myData.b);
	Serial.print("Float: ");
	Serial.println(myData.c);
	Serial.print("Bool: ");
	Serial.println(myData.d);
	Serial.println();
}
 
void setup() 
{
	Serial.begin(115200);
	Serial.println("\n\n\nInicializando..");

	WiFi.mode(WIFI_STA);
	delay(2000);

	Serial.println("MAC ADDRESS: "+String(WiFi.macAddress()));

	#if defined(ESP8266)
		if (esp_now_init() != 0) {
			Serial.println("Error inicializando ESP-NOW");
			return;
		}
		esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  		esp_now_register_recv_cb(OnDataRecv);
	#elif defined(ESP32)
		if (esp_now_init() != ESP_OK) {
			Serial.println("Error inicializando ESP-NOW");
			return;
		}
		esp_now_register_recv_cb(OnDataRecv);
	#endif

	delay(1000);
	Serial.println("Comenzando la escucha..");
}

void loop() 
{ }