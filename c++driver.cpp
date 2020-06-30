#include  <iostream>
#include "javaDriver.h"
#include "Handler.h"
#include "AES.h"
 
using namespace std;

JNIEXPORT jstring JNICALL Java_javaDriver_Compute(JNIEnv *env, jobject thisObj, jstring source, jstring destiny){
    const char *SOURCE = env->GetStringUTFChars(source, NULL);
    const char *DESTINY = env->GetStringUTFChars(destiny, NULL);

    AES cipher(128);
    Handler first(SOURCE, DESTINY);
    uint32_t len = first.getLength();
    char mssg[] = "Finished";

    uint8_t *key = new uint8_t[16]{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
                                    // 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
                                    // 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};

    printf("File '%s' is going to be encrypted, the ciphertext is in '%s'.\n", SOURCE, DESTINY);
    printf("Ciphering...");
    
    while(first.canExtract()){
        first.extract();
        uint8_t *out = cipher.Encrypt_ECB(first.getBuffer(), key, first.getChunk());
        // printf("\n Plain text: |%s| \t <---> \t CipherText: |%s|",  first.getBuffer(), out);
        first.drop(out, 16);
    }

    first.close();
    Handler second(DESTINY, "verify.class");
    second.setOffset(len%16);

    while(second.canExtract()){
        second.extract();
        uint8_t *out = cipher.Decrypt_ECB(second.getBuffer(), key, first.getChunk() );
        // printf("\n Plain text: |%s| \t <---> \t CipherText: |%s|",  second.getBuffer(), out);
        second.drop(out, second.getChunk());
    }

    second.close();
    return env->NewStringUTF(mssg);
}