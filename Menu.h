#include "Handler.h"
#include "Library.h"
#include "AES.h"
    /*            WRAPPER FOR ALL THE MODE OPERATIONS                 */
AES cipher(128);

uint8_t *key = new uint8_t[16]{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
                            // 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
                            // 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};

uint8_t *iv = new uint8_t[16]{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

uint8_t ctr[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/*
        @input = name of file without extension, mode of operation, file extension
        @output = The new name which indecates the mode operation and the extension file
*/
char *cipherName(const char *name, const char *mode, const char *ext){
    char *newName = new char[25]; // New len
    sprintf(newName, "./files/%s_%s.%s", name, mode, ext);
    return newName;
}

/*
        @input = mode of operation, file extension
        @output = "verify_MODE.EXT"
*/
char *verifyName(const char* mode, const char* ext){
    char *verName = new char[strlen(mode) + strlen(ext) + 16];
    sprintf(verName, "./files/verify_%s.%s", mode, ext);
    return verName;
}

void ECB(const char *F1, const char *F2, const char *ext){
    const char *CIPHERTEXT = cipherName(F2, "ECB", ext);
    const char *verify = "./files/verify_ECB.class";

    Library handler(F1, CIPHERTEXT, verify);

    uint8_t *plaintextSRC = handler.get_initial_bytes();
    uint8_t *ciphertextSRC = cipher.Encrypt_ECB(plaintextSRC, key, handler.getLen());

    handler.write_middle_bytes(ciphertextSRC);

    uint8_t *ciphertextMDD = handler.get_middle_bytes();
    uint8_t *plaintextMDD = cipher.Decrypt_ECB(ciphertextMDD, key, handler.getLen());

    handler.write_final_bytes(plaintextMDD);

    handler.close();

    // ifstream SRC(F1, ifstream::binary);
    // SRC.seekg(0, SRC.end);
    // int len = SRC.tellg(), pad = (len % 16) ? (len/16 + 1) * 16 : len;
    // SRC.seekg(0, SRC.beg);
    // char *plainTEXT = new char[len], *cipherTEXT = new char[pad];
    // SRC.read(plainTEXT, len);
    // SRC.close(); 
    // uint8_t *bytes1 = new uint8_t[len];
    // memcpy(bytes1, plainTEXT, len);
    // uint8_t *CIPHER = cipher.Encrypt_ECB(bytes1, key, len);
    // memcpy(cipherTEXT, CIPHER, pad);

    // ofstream WRITER(CIPHERTEXT, ifstream::binary);
    // WRITER.write(cipherTEXT, pad);
    // WRITER.close();

    // ifstream READER(CIPHERTEXT, ifstream::binary);
    // char *noise = new char[pad];
    // READER.read(noise, pad);
    // READER.close();
    // uint8_t *bytes2 = new uint8_t[pad];
    // memcpy(bytes2, noise, pad);

    // uint8_t *inverse = cipher.Decrypt_ECB(bytes2, key, len);
    // bzero(plainTEXT, len);
    // memcpy(plainTEXT, inverse, len);
    // ofstream FINAL(verify, ifstream::binary);
    // FINAL.write(plainTEXT, len);
    // FINAL.close();

    // Handler first(F1, CIPHERTEXT);
    
    // while(first.canExtract()){
    //     first.extract();
    //     uint8_t *out = cipher.Encrypt_ECB(first.getBuffer(), key, first.getChunk());
    //     // printf("\n Plain text: |%s| \t <---> \t CipherText: |%s|",  first.getBuffer(), out);
    //     first.drop(out, 16);
    // }
    // first.close();
    // const char *verify = "./files/verify_ECB.class";
    // Handler second(CIPHERTEXT, verify);
    // while(second.canExtract()){
    //     second.extract();
    //     uint8_t *out = cipher.Decrypt_ECB(second.getBuffer(), key, second.getChunk() );
    //     // printf("\n Plain text: |%s| \t <---> \t CipherText: |%s|",  second.getBuffer(), out);
    //     second.drop(out, second.getChunk());
    // }

    // second.close();
}

void CBC(const char *F1, const char *F2, const char *ext){
    const char *CIPHERTEXT = cipherName(F2, "CBC", ext);
    const char *verify = "./files/verify_CBC.class";

    ifstream SRC(F1, ifstream::binary);
    SRC.seekg(0, SRC.end);
    int len = SRC.tellg(), pad = (len % 16) ? (len/16 + 1) * 16 : len;
    SRC.seekg(0, SRC.beg);
    char *plainTEXT = new char[len], *cipherTEXT = new char[pad];
    SRC.read(plainTEXT, len);
    SRC.close(); 
    uint8_t *bytes1 = new uint8_t[len];
    memcpy(bytes1, plainTEXT, len);
    uint8_t *CIPHER = cipher.Encrypt_CFB(bytes1, key, iv, len);
    memcpy(cipherTEXT, CIPHER, pad);

    ofstream WRITER(CIPHERTEXT, ifstream::binary);
    WRITER.write(cipherTEXT, pad);
    WRITER.close();

    ifstream READER(CIPHERTEXT, ifstream::binary);
    char *noise = new char[pad];
    READER.read(noise, pad);
    READER.close();
    uint8_t *bytes2 = new uint8_t[pad];
    memcpy(bytes2, noise, pad);

    uint8_t *inverse = cipher.Decrypt_CFB(bytes2, key, iv, len);
    bzero(plainTEXT, len);
    memcpy(plainTEXT, inverse, len);
    ofstream FINAL(verify, ifstream::binary);
    FINAL.write(plainTEXT, len);
    FINAL.close();
    
    // Handler first(F1, CIPHERTEXT);
    // while(first.canExtract()){
    //     first.extract();
    //     uint8_t *out = cipher.Encrypt_CBC(first.getBuffer(), key, iv, first.getChunk());
    //     // printf("\n Plain text: |%s| \t <---> \t CipherText: |%s|",  first.getBuffer(), out);
    //     first.drop(out, 16);
    // }
    // first.close();
    // const char *verify = "./files/verify_CBC.class";
    // Handler second(CIPHERTEXT, verify   );
    // while(second.canExtract()){
    //     second.extract();
    //     uint8_t *out = cipher.Decrypt_CBC(second.getBuffer(), key, iv, second.getChunk() );
    //     // printf("\n Plain text: |%s| \t <---> \t CipherText: |%s|",  second.getBuffer(), out);
    //     second.drop(out, second.getChunk());
    // }
    // second.close();
}

void CFB(const char *F1, const char *F2, const char *ext){
    const char *CIPHERTEXT = cipherName(F2, "CFB", ext);

    const char *verify = "./files/verify_CFB.class";

    // Handler first(F1, CIPHERTEXT);

    // uint32_t len = first.getLength(), pad = (len % 16) ? (len/16 + 1) * 16 : len;

    // uint8_t *buffer1 = first.getAll();

    // uint8_t *cipherText = cipher.Encrypt_CFB(buffer1, key, iv, len);

    // first.writeAll(cipherText, pad); // check
    
    // first.close();


    // Handler second(CIPHERTEXT, verify);

    // uint8_t *buffer2 = second.getAll();

    // uint8_t *plainText = cipher.Decrypt_CFB(buffer2, key, iv, len);

    // second.writeAll(plainText, len);

    // second.close();


    ifstream SRC(F1, ifstream::binary);
    SRC.seekg(0, SRC.end);
    int len = SRC.tellg(), pad = (len % 16) ? (len/16 + 1) * 16 : len;
    SRC.seekg(0, SRC.beg);
    char *plainTEXT = new char[len], *cipherTEXT = new char[pad];
    SRC.read(plainTEXT, len);
    SRC.close(); 
    // int pad = (len % 16) ? (len/16 + 1) * 16 : len;
    // printf("All \n %s", all);
    uint8_t *bytes1 = new uint8_t[len];
    memcpy(bytes1, plainTEXT, len);
    uint8_t *CIPHER = cipher.Encrypt_CFB(bytes1, key, iv, len);
    memcpy(cipherTEXT, CIPHER, pad);

    ofstream WRITER(CIPHERTEXT, ifstream::binary);
    WRITER.write(cipherTEXT, pad);
    WRITER.close();

    ifstream READER(CIPHERTEXT, ifstream::binary);
    char *noise = new char[pad];
    READER.read(noise, pad);
    READER.close();
    uint8_t *bytes2 = new uint8_t[pad];
    memcpy(bytes2, noise, pad);

    uint8_t *inverse = cipher.Decrypt_CFB(bytes2, key, iv, len);
    bzero(plainTEXT, len);
    memcpy(plainTEXT, inverse, len);
    ofstream FINAL(verify, ifstream::binary);
    FINAL.write(plainTEXT, len);
    FINAL.close();
    
}

void OFB(const char *F1, const char *F2, const char *ext){
    const char *CIPHERTEXT = cipherName(F2, "OFB", ext);
    const char *verify = "./files/verify_OFB.class";

    ifstream SRC(F1, ifstream::binary);
    SRC.seekg(0, SRC.end);
    int len = SRC.tellg(), pad = (len % 16) ? (len/16 + 1) * 16 : len;
    SRC.seekg(0, SRC.beg);
    char *plainTEXT = new char[len], *cipherTEXT = new char[pad];
    SRC.read(plainTEXT, len);
    SRC.close(); 
    uint8_t *bytes1 = new uint8_t[len];
    memcpy(bytes1, plainTEXT, len);
    uint8_t *CIPHER = cipher.Encrypt_CFB(bytes1, key, iv, len);
    memcpy(cipherTEXT, CIPHER, pad);

    ofstream WRITER(CIPHERTEXT, ifstream::binary);
    WRITER.write(cipherTEXT, pad);
    WRITER.close();

    ifstream READER(CIPHERTEXT, ifstream::binary);
    char *noise = new char[pad];
    READER.read(noise, pad);
    READER.close();
    uint8_t *bytes2 = new uint8_t[pad];
    memcpy(bytes2, noise, pad);

    uint8_t *inverse = cipher.Decrypt_CFB(bytes2, key, iv, len);
    bzero(plainTEXT, len);
    memcpy(plainTEXT, inverse, len);
    ofstream FINAL(verify, ifstream::binary);
    FINAL.write(plainTEXT, len);
    FINAL.close();
}

void CTR(const char *F1, const char *F2, const char *ext){
    const char *CIPHERTEXT = cipherName(F2, "CTR", ext);
    
    const char *verify = "./files/verify_CTR.class";

    ifstream SRC(F1, ifstream::binary);
    SRC.seekg(0, SRC.end);
    int len = SRC.tellg(), pad = (len % 16) ? (len/16 + 1) * 16 : len;
    SRC.seekg(0, SRC.beg);
    char *plainTEXT = new char[len], *cipherTEXT = new char[pad];
    SRC.read(plainTEXT, len);
    SRC.close(); 
    uint8_t *bytes1 = new uint8_t[len];
    memcpy(bytes1, plainTEXT, len);
    uint8_t *CIPHER = cipher.Encrypt_CTR(bytes1, key, iv, len);
    memcpy(cipherTEXT, CIPHER, pad);

    ofstream WRITER(CIPHERTEXT, ifstream::binary);
    WRITER.write(cipherTEXT, pad);
    WRITER.close();

    ifstream READER(CIPHERTEXT, ifstream::binary);
    char *noise = new char[pad];
    READER.read(noise, pad);
    READER.close();
    uint8_t *bytes2 = new uint8_t[pad];
    memcpy(bytes2, noise, pad);

    uint8_t *inverse = cipher.Decrypt_CTR(bytes2, key, iv, len);
    bzero(plainTEXT, len);
    memcpy(plainTEXT, inverse, len);
    ofstream FINAL(verify, ifstream::binary);
    FINAL.write(plainTEXT, len);
    FINAL.close();
    
    // Handler first(F1, CIPHERTEXT);
    // while(first.canExtract()){
    //     first.extract();
    //     uint8_t *out = cipher.Encrypt_CTR(first.getBuffer(), key, ctr, first.getChunk());
    //     printf("\n Plain text: |%s| \t <---> \t CipherText: |%s|",  first.getBuffer(), out);
    //     first.drop(out, 16);
    // }
    // puts("\n\n\n\n");
    // first.close();
    // Handler second(CIPHERTEXT, verifyName("CTR", ext));
    // while(second.canExtract()){
    //     second.extract();
    //     uint8_t *out = cipher.Decrypt_CTR(second.getBuffer(), key, ctr, second.getChunk() );
    //     printf("\n Plain text: |%s| \t <---> \t CipherText: |%s|",  second.getBuffer(), out);
    //     second.drop(out, second.getChunk());
    // }
    // second.close();
}