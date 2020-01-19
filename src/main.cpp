#include <Arduino.h>
#include "mbedtls/rsa.h"

mbedtls_rsa_context rsa;
static unsigned char buffer[1024];

void setup(){
    Serial.begin(115200);
    mbedtls_rsa_init(&rsa,MBEDTLS_RSA_PKCS_V15,0);
    //unsigned char n[] = {0x0f};
    //unsigned char d[] = {0x0d};
    char* n = "key_key_key_key_";
    char* d = "123";
    mbedtls_rsa_import_raw(&rsa,(const unsigned char*)n,1,NULL,0,NULL,0,(const unsigned char*)d,1,NULL,0);
    //mbedtls_rsa_complete(&rsa);
    Serial.println("\n########\n");
    char * input = "123123123123123123123123";
    int len = strlen(input);
    int suc = mbedtls_rsa_public(&rsa,(const unsigned char *)input,buffer);
    switch (suc) {
        case MBEDTLS_ERR_RSA_BAD_INPUT_DATA:
            Serial.println("MBEDTLS_ERR_RSA_BAD_INPUT_DATA");
            break;
        case MBEDTLS_ERR_RSA_INVALID_PADDING:
            Serial.println("MBEDTLS_ERR_RSA_INVALID_PADDING");
            break;
        case MBEDTLS_ERR_RSA_KEY_GEN_FAILED:
            Serial.println("MBEDTLS_ERR_RSA_KEY_GEN_FAILED");
            break;
        case MBEDTLS_ERR_RSA_KEY_CHECK_FAILED:
            Serial.println("MBEDTLS_ERR_RSA_KEY_CHECK_FAILED");
            break;
        case MBEDTLS_ERR_RSA_PUBLIC_FAILED:
            Serial.println("MBEDTLS_ERR_RSA_PUBLIC_FAILED");
            break;
        case MBEDTLS_ERR_RSA_PRIVATE_FAILED:
            Serial.println("MBEDTLS_ERR_RSA_PRIVATE_FAILED");
            break;
        case MBEDTLS_ERR_RSA_VERIFY_FAILED:
            Serial.println("MBEDTLS_ERR_RSA_VERIFY_FAILED");
            break;
        case MBEDTLS_ERR_RSA_OUTPUT_TOO_LARGE:
            Serial.println("MBEDTLS_ERR_RSA_OUTPUT_TOO_LARGE");
            break;
        case MBEDTLS_ERR_RSA_RNG_FAILED:
            Serial.println("MBEDTLS_ERR_RSA_RNG_FAILED");
            break;
        case MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION:
            Serial.println("MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION");
            break;
        case MBEDTLS_ERR_RSA_HW_ACCEL_FAILED:
            Serial.println("MBEDTLS_ERR_RSA_HW_ACCEL_FAILED");
            break;
        default:
            Serial.println("Success!!");
            break;
    }
    Serial.println("########");
    for(int i =0;i<len;i++){
        Serial.printf("%2x ",input[i]);
    }
    Serial.println("\n->>>>>>>>>");
    for(int i =0;i<len;i++){
        Serial.printf("%2x ",buffer[i]);
    }
}

void loop(){

}
