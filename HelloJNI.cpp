#include  <iostream>
#include "HelloJNI.h"
#include "Handler.h"
#include "AES.h"
 
using namespace std;


void printChunk(uint8_t *chunk, int size){
    for(int i = 0; i < size; i++){
        printf("%c", chunk[i]);
    }
    cout << endl;
}

JNIEXPORT jstring JNICALL Java_HelloJNI_sayHello(JNIEnv *env, jobject thisObj, jstring source, jstring destiny){
    
    const char *inCStr1 = env->GetStringUTFChars(source, NULL);

    const char *inCStr2 = env->GetStringUTFChars(destiny, NULL);


    printf("%s\n", inCStr1);
    printf("%s\n", inCStr2);


   
    char mssg[] = "Finish";


    Handler driver(inCStr1, inCStr2);

    AES cipher(128);

    uint8_t *in = new uint8_t[16]{0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                            0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}; 

    uint8_t *key = new uint8_t[16]{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
                                    // 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};


    while(driver.canExtract()){
        driver.extract();
        // printChunk(driver.getBuffer2(), driver.getChunk());

        uint8_t *out = cipher.Encrypt_ECB(driver.getBuffer(), key, driver.getChunk());
        
        printf("\n Cifrado %d:   |%s| \n", driver.getChunk(), out);

        uint8_t *sec = cipher.Decrypt_ECB(out, key, driver.getChunk());// driver.getChunk());

        printf("Descifrado arriba %d:   |%s| \n\n\n", driver.getChunk(), sec);

        // driver.drop2(driver.getBuffer(), driver.getChunk());
        
        // driver.drop2(out, driver.getChunk());

        driver.drop2(out, 16);


    }

    int len1 = driver.getLength();
    puts("=====================================");
    driver.close();



    Handler other(inCStr2, "Descifrado.txt");
    other.setOffset(len1%16);
    int len2 = other.getLength();

    printf("\n\nThe length 1: %d, 2: %d\n\n", len1, len2);

    while(other.canExtract()){
        other.extract();
        // printChunk(other.getBuffer2(), driver.getChunk());

        uint8_t *out = cipher.Decrypt_ECB(other.getBuffer(), key, driver.getChunk() ); // other.getChunk());

        printf("\n Descifrado abajo %d: |%s|  \n", other.getChunk(), out);

        // printChunk(out, other.getChunk());
 
        // driver.drop2(driver.getBuffer(), driver.getChunk());
        
        other.drop2(out, other.getChunk());

    }

    other.close();


    Handler third("Descifrado.txt", "final.txt");
    third.setOffset(len1%16);
    third.clear(len1);
    third.close();
    
    // env->ReleaseStringUTFChars(source, inCStr1);
    // env->ReleaseStringUTFChars(destiny, inCStr2);


    return env->NewStringUTF(mssg);

}