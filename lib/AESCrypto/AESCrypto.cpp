#include <Arduino.h>
#include "AESCrypto.h"

AESCrypto::AESCrypto(){}
AESCrypto::~AESCrypto(){
    free();
}

void AESCrypto::init(char *key, unsigned char* iv){
    this->key = key;
    this->block_size = strlen(key);
    mbedtls_aes_setkey_enc( &this->aes, (const unsigned char*) this->key, this->block_size * 8 );
    this->iv = iv;
    Serial.println("iv: ");
    for(int i =0;i< this->block_size; i++){
        Serial.printf("%2x",this->iv[i]);
    }
}

void AESCrypto::free(){
    mbedtls_aes_free( &aes );
}

int AESCrypto::encryptCBC(unsigned char* input, int len_input, unsigned char* bufferOutput){
    int residual = len_input%(this->block_size);
    if(residual > 0){
        int new_len = len_input+this->block_size-residual;
        unsigned char inputBuffer[new_len];
        memcpy(inputBuffer, input, sizeof(input[0])*len_input);
        int temp = 0x0 + residual;
        for(int i = len_input; i < new_len ; i++){
            inputBuffer[i] = temp;
        }
        mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, new_len, this->iv, inputBuffer, bufferOutput);
        return new_len;
    }
    else{
        mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, len_input, this->iv, input, bufferOutput);
        return len_input;
    }
}

void AESCrypto::decryptCBC(unsigned char* input, int len_input, unsigned char* bufferOutput){
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, len_input, this->iv, input, bufferOutput);
}
