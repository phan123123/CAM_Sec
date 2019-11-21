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
#include <ArduinoWebsockets.h>
#include "DataArduCAM.h"


const int CS = 16;
const char* ssid = "InternetHV-02"; //Enter SSID
const char* password = "1234567890"; //Enter Password
const char* websockets_server = "192.168.8.12:1337";


websockets::WebsocketsClient client;
DataArduCAM *dataCAM;
static const size_t bufferSize = 30096; // set default buffer size
static uint8_t buffer[bufferSize] = {0xFF};  // buffer for get data
//
// void headerProc(){
// 	Serial.println(">>>>>>>>>>>>>>>>>>>");
// }
bool checkBreak(){
	return false;
}
// void process(uint8_t *buffer, size_t size){
// 	Serial.println("ok");
// 	// Serial.println(size);
// 	Serial.println("<<<<<<<<<<<<<<<<<<<<<<");
// 	delay(200);
// }

// ###########################################################

void onMessageCallback(websockets::WebsocketsMessage message) {
	Serial.print("Got Message: ");
	Serial.println(message.data());
}

void onEventsCallback(websockets::WebsocketsEvent event, String data) {
	if(event == websockets::WebsocketsEvent::ConnectionOpened) {
		Serial.println("Connnection Opened");
	} else if(event == websockets::WebsocketsEvent::ConnectionClosed) {
		Serial.println("Connnection Closed");
	} else if(event == websockets::WebsocketsEvent::GotPing) {
		Serial.println("Got a Ping!");
	} else if(event == websockets::WebsocketsEvent::GotPong) {
		Serial.println("Got a Pong!");
	}
}

void sendHeader(){
    client.send("BEGIN");
}

void sendBinary(uint8_t *buffer, size_t size){
    client.sendBinary((char *)buffer,size);
}

bool checkConnect(){
    if(WiFi.status() != WL_CONNECTED||!client.available()){
        return true;
    }
}

void setup(){
	Serial.begin(115200);
	dataCAM = new DataArduCAM(OV2640, CS,OV2640_160x120,JPEG,14,27,13,16,4,5);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		Serial.println("Connecting to wifi...");
		delay(1000);
	}
	Serial.println("Conneted wifi");
	client.onMessage(onMessageCallback);
	client.onEvent(onEventsCallback);
	client.connect(websockets_server);

	// dataCAM->processJPEG(buffer, bufferSize);
}

void loop(){
	// dataCAM.processData(&headerProc,&process,&checkBreak);
	dataCAM->processJPEG(buffer,bufferSize, &sendHeader, &sendBinary, &checkBreak);
}
