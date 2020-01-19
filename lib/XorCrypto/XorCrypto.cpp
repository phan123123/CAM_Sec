#include "XorCrypto.h"

void XorCrypto::repeatingEncrypt(int size, char* input, char* key, char * bufferOutput){
    int key_size         = strlen(key);
    for (int i = 0; i < size; i++) {
        bufferOutput[i] = input[i] ^ key[i % key_size];
    }
}

void XorCrypto::repeatingDecrypt(int size, char* input, char* key, char * bufferOutput){
    int key_size         = strlen(key);
    for (int i = 0; i < size; i++) {
        bufferOutput[i] = input[i] ^ key[i % key_size];
    }
}
