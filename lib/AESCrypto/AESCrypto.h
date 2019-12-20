#ifndef AESCrypto_H
    #define AESCrypto_H
    #include "mbedtls/aes.h"
#endif

class AESCrypto {
public:
    AESCrypto(void);
    // AESCrypto();
    ~AESCrypto();
    void init(char *key, unsigned char* iv);
    void free();
    int encryptCBC(unsigned char* input, int len_input, unsigned char* buffer);
    void decryptCBC(unsigned char* input, int len_input, unsigned char* bufferOutput);
protected:
private:
    char* key;
    int block_size;
    unsigned char *iv;
    mbedtls_aes_context aes;
};
