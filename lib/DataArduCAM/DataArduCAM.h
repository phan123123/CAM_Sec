#ifndef DataArduCAM_H
    #define DataArduCAM_H
    #include <SPI.h>
    #include <ArduCAM.h>
#endif

// redefine from ArduCAM.h
// define type of data
    #define BMP 	0
    #define JPEG	1
    #define RAW	  2
// end define type of data

// define type of module
    #define OV7670		0
    #define MT9D111_A	1
    #define OV7675		2
    #define OV5642		3
    #define OV3640  	4
    #define OV2640  	5
    #define OV9655		6
    #define MT9M112		7
    #define OV7725		8
    #define OV7660		9
    #define MT9M001 	10
    #define OV5640 		11
    #define MT9D111_B	12
    #define OV9650		13
    #define MT9V111		14
    #define MT9T112		15
    #define MT9D112		16
    #define MT9V034 	17
    #define MT9M034   18
// end define type of module

// define type of size
    #define OV2640_160x120 		0	//160x120
    #define OV2640_176x144 		1	//176x144
    #define OV2640_320x240 		2	//320x240
    #define OV2640_352x288 		3	//352x288
    #define OV2640_640x480		4	//640x480
    #define OV2640_800x600 		5	//800x600
    #define OV2640_1024x768		6	//1024x768
    #define OV2640_1280x1024	7	//1280x1024
    #define OV2640_1600x1200	8	//1600x1200



    #define OV3640_176x144 		0	//176x144
    #define OV3640_320x240 		1	//320x240
    #define OV3640_352x288 		2	//352x288
    #define OV3640_640x480		3	//640x480
    #define OV3640_800x600 		4	//800x600
    #define OV3640_1024x768		5 //1024x768
    #define OV3640_1280x960	  6	//1280x960
    #define OV3640_1600x1200	7	//1600x1200
    #define OV3640_2048x1536	8	//2048x1536


    #define OV5642_320x240 		0	//320x240
    #define OV5642_640x480		1	//640x480
    #define OV5642_1024x768		2	//1024x768
    #define OV5642_1280x960 	3	//1280x960
    #define OV5642_1600x1200	4	//1600x1200
    #define OV5642_2048x1536	5	//2048x1536
    #define OV5642_2592x1944	6	//2592x1944
    #define OV5642_1920x1080  7


    #define OV5640_320x240 		0	//320x240
    #define OV5640_352x288		1	//352x288
    #define OV5640_640x480 	  2	//640x480
    #define OV5640_800x480	  3	//800x480
    #define OV5640_1024x768	  4	//1024x768
    #define OV5640_1280x960	  5	//1280x960
    #define OV5640_1600x1200	6	 //1600x1200
    #define OV5640_2048x1536	7  //2048x1536
    #define OV5640_2592x1944	8	 //2592x1944
// end define type of size

// define type of picture data
    #define BMP 	0
    #define JPEG	1
    #define RAW	  2
// end define

class DataArduCAM
{
public:
    DataArduCAM(void);
    DataArduCAM(byte model ,int CS, int sizeOfImage,byte formatData,
        int sck,int  miso,int  mosi,int ss, int sda, int scl); //init the cam and SPI protocol
    ~DataArduCAM();


    /*  processData fuction:
            - pHeaderProc: process when start cap a new picture
            - pFunc: process with data of per picture
            - checkBreak: to check break loop (true -> break and false-> continue)
    */
    void processJPEG(uint8_t* buffer,size_t bufferSize,void (*pHeaderProc)(),void(*pFunc)(uint8_t *buffer,size_t size),bool (*checkBreak)());
protected:

private:
    ArduCAM *arduCAM;
    long SPIFrequency;          // default = 8000000
    uint8_t temp, temp_last;
    int i;//count
    byte model,formatData;
    int CS,sizeOfImage,sck, miso, mosi,ss, sda,scl;

    //set size of picture
    void setSizeImage(byte model, byte formatData,int sizeOfImage);
    void setSizeJPEG(byte model,int sizeOfImage);

    //check init
    bool checkCAM(byte model);
    bool OV2640check();
    bool checkSPI();
};
