#ifndef ESP32WebSocket_H
    # define ESP32WebSocket_H
    # include <WiFi.h>
    # include <WebSocketsClient.h>
#endif

class ESP32WebSocket {
public:
    ESP32WebSocket(void);
    ESP32WebSocket(char * ssid,char * password,char * address,int port,void (*pWebSocketEvent)(WStype_t type, uint8_t * payload, size_t length));
    ~ESP32WebSocket();
    void catchPackage();

protected:

private:
    static WebSocketsClient webSocket;
    char * ssid;     // Enter SSID
    char * password; // Enter Password
    char * address;
    int port;
};
