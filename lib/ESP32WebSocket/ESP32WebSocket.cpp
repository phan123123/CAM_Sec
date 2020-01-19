#include "ESP32WebSocket.h"

ESP32WebSocket::ESP32WebSocket(char * ssid,char * password,char * address,int port,void (*pWebSocketEvent)(WStype_t type, uint8_t * payload, size_t length)){
    this-> ssid = ssid;
    this-> password = password;
    this-> address = address;
    this -> port = port;
    WiFi.begin(this-> ssid, this-> password);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("Connecting to wifi...");
        delay(1000);
    }
    Serial.println("Conneted wifi");
    // server address, port and URL
    webSocket.begin(this-> address, this -> port);
    // event handler
    webSocket.onEvent(pWebSocketEvent);
}

void ESP32WebSocket::catchPackage(){
    this-> webSocket.loop();
}
