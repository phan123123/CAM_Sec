#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "DataArduCAM.h"
#include "ArduCAM.h"


DataArduCAM::DataArduCAM(){
};

DataArduCAM::DataArduCAM(byte model,int CS, int sizeOfImage,byte formatData,
                         int sck,int miso,int mosi,int ss, int sda, int scl){
    this->model=model;
    this->CS = CS;
    this->sizeOfImage=sizeOfImage;
    this->formatData=formatData;
    this->sck=sck;
    this->miso=miso;
    this->mosi=mosi;
    this->ss=ss;
    this->sda=sda;
    this->scl=scl;
	arduCAM = new ArduCAM(model,CS);
	Serial.println("Created CAM object");
	pinMode(CS,OUTPUT);
	//I2C START SDA, SCL
	Wire.begin(sda,scl);
	SPIFrequency = 8000000;
	SPI.begin(sck, miso, mosi, ss);
	SPI.setFrequency(SPIFrequency);
	if(checkSPI()) {
		Serial.println("SPI check done");
	}
	else{
		Serial.println("SPI error!!");
	}
	if (checkCAM(model)) {
		Serial.println("CAM check done");
	}
	else{
		Serial.println("CAM error!!");
	}
	arduCAM->set_format(formatData);
	Serial.println("Inited format");
	arduCAM->InitCAM();
	Serial.println("Inited CAM object");
	setSizeImage(model,formatData,sizeOfImage);
	Serial.println("Setted size");
	arduCAM->clear_fifo_flag();
	Serial.println("Init done");
	delay(500);
}

DataArduCAM::~DataArduCAM(){
	delete arduCAM;
};

void DataArduCAM::setSizeJPEG(byte model,int sizeOfImage){
	switch (model) {
	case OV2640:
		arduCAM->OV2640_set_JPEG_size(sizeOfImage);
		break;
	}
}

void DataArduCAM::setSizeImage(byte model, byte formatData,int sizeOfImage){
	switch(formatData) {
	case JPEG:
		setSizeJPEG(model,sizeOfImage);
		break;
	}
}

bool DataArduCAM::checkCAM(byte model){
	switch (model) {
	case OV2640:
		return OV2640check();
		break;
	}
	return false;
}

bool DataArduCAM::OV2640check(){
	uint8_t vid, pid;
	arduCAM->wrSensorReg8_8(0xff, 0x01);
	arduCAM->rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
	arduCAM->rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
	if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 )))
		return false;
	else
		return true;
}

bool DataArduCAM::checkSPI(){
	uint8_t temp;
	temp = arduCAM->read_reg(ARDUCHIP_TEST1);
	arduCAM->write_reg(ARDUCHIP_TEST1, 0x55);
	if (temp != 0x55) {
		return false;
	}
	else
		return true;
}


void DataArduCAM::processJPEG(uint8_t* buffer,size_t bufferSize){
	delay(1000);
	temp=0;
	temp_last=0;
	i=0;
	bool is_header = false;
	uint32_t len  = arduCAM->read_fifo_length();
	if (len >= MAX_FIFO_SIZE)     //8M
	{
		Serial.println(F("Over size."));
	}
	if (len == 0 )     //
	{

		Serial.println(F("Size is 0."));
	}
	arduCAM->flush_fifo();
	arduCAM->clear_fifo_flag();
	arduCAM->start_capture();
	Serial.println("Start cap");
	while (!arduCAM->get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));
	Serial.println("Cap done");
	arduCAM->CS_LOW();
	arduCAM->set_fifo_burst();
	i = 0;
	while ( len-- )
	{
		temp_last = temp;
		temp =  SPI.transfer(0x00);
		// Serial.printf("0x%2x\n",temp);
		// delay(200);
		//Read JPEG data from FIFO
		if ( (temp == 0xD9) && (temp_last == 0xFF) )         //If find the end ,break while,
		{
			buffer[i++] = temp;             //save the last  0XD9
			//Write the remain bytes in the buffer
			Serial.println("CATCH!!");
			is_header = false;
			i = 0;
			arduCAM->CS_HIGH();
			break;
		}
		if (is_header == true)
		{
			//Write image data to buffer if not full
			if (i < bufferSize)
				buffer[i++] = temp;
			else
			{
				//Write bufferSize bytes image data to file
				Serial.println("CATCH!!");
				i = 0;
				buffer[i++] = temp;
			}
		}
		else if ((temp == 0xD8) & (temp_last == 0xFF))
		{
			is_header = true;
			buffer[i++] = temp_last;
			buffer[i++] = temp;
		}
	}
}

void DataArduCAM::processJPEG(uint8_t* buffer,size_t bufferSize,void (*pHeaderProc)(),void (*pProc)(uint8_t *buffer, size_t size), bool (*pcheckBreak)()){
    delay(1000);
	temp=0;
	temp_last=0;
	i=0;
	bool is_header = false;
	while (1) {
        arduCAM->flush_fifo();
    	arduCAM->clear_fifo_flag();
    	arduCAM->start_capture();
    	while (!arduCAM->get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));
    	arduCAM->CS_LOW();
    	arduCAM->set_fifo_burst();
        uint32_t len  = arduCAM->read_fifo_length();
		if (len >= MAX_FIFO_SIZE)         //8M
		{
			Serial.println(F("Over size."));
			continue;
		}
		if (len == 0 )         //0 kb
		{
			Serial.println(F("Size is 0."));
			continue;
		}
		arduCAM->CS_LOW();
		arduCAM->set_fifo_burst();
		if (pcheckBreak()) break;
		while ( len-- )
		{
			temp_last = temp;
			temp =  SPI.transfer(0x00);
            // Serial.printf("0x%2x\n",temp);
            // delay(200);
//Read JPEG data from FIFO
			if ( (temp == 0xD9) && (temp_last == 0xFF) )             //If find the end ,break while,
			{
                pHeaderProc();
				buffer[i++] = temp;                 //save the last  0xsD9
//Write the remain bytes in the buffer
				arduCAM->CS_HIGH();
				if (pcheckBreak()) break;
				pProc(&buffer[0], i);
				is_header = false;
				i = 0;
			}
			if (is_header == true)
			{
//Write image data to buffer if not full
				if (i < bufferSize)
					buffer[i++] = temp;
				else
				{
//Write bufferSize bytes image data to file
					arduCAM->CS_HIGH();
                    if (pcheckBreak()) break;
    				pProc(&buffer[0], i);
					i = 0;
					buffer[i++] = temp;
					arduCAM->CS_LOW();
					arduCAM->set_fifo_burst();
				}
			}
			else if ((temp == 0xD8) & (temp_last == 0xFF))
			{
				is_header = true;
				buffer[i++] = temp_last;
				buffer[i++] = temp;
			}
		}
		if (pcheckBreak()) break;
	}
}
