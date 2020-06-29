#include <cstdio>
// #include <jni.h>
#include "AES.h"
#include "Handler.h"
// #include "HelloJNI.h"

void printStr(uint8_t *str){
    for(int i = 0; i < 16; i++)
        // printf("%c ", in[i]);
        printf("0x%02x ", str[i]);
    puts("");
}

void printChunk(char *chunk, int size){
    for(int i = 0; i < size; i++){
        printf("%c", chunk[i]);
    }
    cout << endl;
}

int main(int argc, char *argv[]){
    AES cipher(128);

    // uint8_t *in = new uint8_t[16]{0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
    //                              0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};

    // uint8_t *in = new uint8_t[16]{0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb,
    //                             0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32};

    // uint8_t *key = new uint8_t[16]{0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    //                                 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

    // uint8_t in[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    //                         0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};


    uint8_t *in = new uint8_t[16]{0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                            0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}; 

    uint8_t *key = new uint8_t[16]{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
                                    // 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};

    // unsigned char iv[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    //                      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

    // uint8_t ctr[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    //                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    // uint32_t len = 0;

    printStr(in);
    printStr(key);

    // uint8_t *out = new uint8_t[16];

    // cipher.Encrypt(in, out, key);
    
    // uint8_t *out = cipher.Encrypt_OFB(in, key, ctr, sizeof(in));
    // uint8_t *neww = cipher.Decrypt_OFB(out, key, ctr, 16);

    uint8_t *out = cipher.Encrypt_ECB(in, key, sizeof(in));
    uint8_t *neww = cipher.Decrypt_ECB(out, key, 16);

    // uint8_t *out = cipher.Encrypt_CBC(in, key, iv, sizeof(in));

    puts("\n");
    printStr(out);
    puts("\n");
    printStr(neww);

    // Handler driver("file.txt", "fromJNI.txt");
    // AES aes(128); 

    // while(driver.canExtract()){
    //     driver.extract();
    //     printChunk(driver.getBuffer2(), driver.getChunk());

    //     uint8_t *cipher = aes.Encrypt_ECB(driver.getBuffer(), key, driver.getChunk());

    //     driver.drop2(driver.getBuffer(), driver.getChunk());
    // }

    // driver.close();

    // return 0;
}

// For more information visit: https://autonome-antifa.org/IMG/pdf/Rijndael.pdf