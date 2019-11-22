/*
 * pin arducam mini  2M       esp32
 *
 * cs                           GPIO16 -RX2
 * mosi                         GPIO13
 * misc                         GPIO27
 * sck                          GPIO14
 * gnd                          gnd
 * vcc                          3.3v
 * sda                          GPIO04
 * scl                          GPIO05
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include "DataArduCAM.h"

WebSocketsClient webSocket;
const int CS = 16;
const char* ssid = "InternetHV-02"; //Enter SSID
const char* password = "1234567890"; //Enter Password
const char* address = "192.168.8.12";
const int port = 1337;



DataArduCAM *dataCAM;
static const size_t bufferSize = 30096; // set default buffer size
static uint8_t buffer[bufferSize] = {0xFF};  // buffer for get data
//
// void headerProc(){
//  Serial.println(">>>>>>>>>>>>>>>>>>>");
// }
// bool checkBreak(){
//  return false;
// }
// void process(uint8_t *buffer, size_t size){
//  Serial.println("ok");
//  // Serial.println(size);
//  Serial.println("<<<<<<<<<<<<<<<<<<<<<<");
//  delay(200);
// }

// ###########################################################
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
	switch(type) {
		case WStype_DISCONNECTED:
			Serial.printf("[WSc] Disconnected!\n");
			break;
		case WStype_CONNECTED:
			Serial.printf("[WSc] Connected to url: %s\n", payload);
			// send message to server when Connected
			webSocket.sendTXT("Connected");
			break;
		case WStype_TEXT:
			Serial.printf("[WSc] get text: %s\n", payload);

			// send message to server
			// webSocket.sendTXT("message here");
			break;
		case WStype_BIN:
			Serial.printf("[WSc] get binary length: %u\n", length);

			// send data to server
			// webSocket.sendBIN(payload, length);
			break;
		case WStype_ERROR:
		case WStype_FRAGMENT_TEXT_START:
		case WStype_FRAGMENT_BIN_START:
		case WStype_FRAGMENT:
		case WStype_FRAGMENT_FIN:
			break;
	}

}

void sendHeader(){
	webSocket.loop();
    // webSocket.sendTXT("Start");
    // Serial.println("send start image");
}

void sendBinary(uint8_t *buffer, size_t size){
    webSocket.sendBIN(buffer,size);
    // Serial.printf("Send data with size: %d \n",size);
}

bool checkConnect(){
	return false;
}

void setup(){
	Serial.begin(115200);
	dataCAM = new DataArduCAM(OV2640, CS,OV2640_640x480,JPEG,14,27,13,16,4,5);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		Serial.println("Connecting to wifi...");
		delay(1000);
	}
	Serial.println("Conneted wifi");
	// server address, port and URL
	webSocket.begin(address, port);
	// event handler
	webSocket.onEvent(webSocketEvent);
	// dataCAM->processJPEG(buffer, bufferSize);
}

void loop(){
	dataCAM->processJPEG(buffer, bufferSize, &sendHeader, &sendBinary, &checkConnect);
}
