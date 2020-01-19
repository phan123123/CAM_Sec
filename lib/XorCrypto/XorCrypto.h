#ifndef XorCrypto_H
    # define XorCrypto_H
#endif

class XorCrypto{
public:
    static void repeatingEncrypt(int size, char* input, char* key, char * bufferOutput);
    static void repeatingDecrypt(int size, char* input, char* key,  char * bufferOutput);
};
