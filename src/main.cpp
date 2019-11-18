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
#include "DataArduCAM.h"

const int CS = 16;

DataArduCAM *dataCAM;
static const size_t bufferSize = 30096; // set default buffer size
static uint8_t buffer[bufferSize] = {0xFF};  // buffer for get data
//
void headerProc(){}
bool checkBreak(){return false;}
void process(uint8_t *buffer, size_t size){
    Serial.println(">>>>>>>>>>>>>>>>>>>");
    Serial.println("ok");
    // Serial.println(size);
    Serial.println("<<<<<<<<<<<<<<<<<<<<<<");
}

void setup(){
	Serial.begin(115200);
	dataCAM = new DataArduCAM(OV2640, CS,OV2640_160x120,JPEG,14,27,13,16,4,5);
    pinMode(CS,OUTPUT);
    dataCAM->processJPEG(buffer, bufferSize);
}

void loop(){
	// dataCAM.processData(&headerProc,&process,&checkBreak);

}
