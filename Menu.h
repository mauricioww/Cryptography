#include "Handler.h"
#include "AES.h"
    /*            WRAPPER FOR ALL THE MODE OPERATIONS                 */
AES cipher(128);

uint8_t *key = new uint8_t[16]{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
                            // 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
                            // 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};

uint8_t iv[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };


/*
        @input = name of file without extension, mode of operation, file extension
        @output = The new name which indecates the mode operation and the extension file
*/
char *cipherName(const char *name, const char *mode, const char *ext){
    char *newName = new char[strlen(name) + strlen(mode) + strlen(ext) + 10]; // New length
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
    Handler first(F1, CIPHERTEXT);
    while(first.canExtract()){
        first.extract();
        uint8_t *out = cipher.Encrypt_ECB(first.getBuffer(), key, first.getChunk());
        // printf("\n Plain text: |%s| \t <---> \t CipherText: |%s|",  first.getBuffer(), out);
        first.drop(out, 16);
    }
    first.close();
    Handler second(CIPHERTEXT, verifyName("ECB", ext));
    while(second.canExtract()){
        second.extract();
        uint8_t *out = cipher.Decrypt_ECB(second.getBuffer(), key, second.getChunk() );
        // printf("\n Plain text: |%s| \t <---> \t CipherText: |%s|",  second.getBuffer(), out);
        second.drop(out, second.getChunk());
    }
    second.close();
}

void CBC(const char *F1, const char *F2, const char *ext){
    const char *CIPHERTEXT = cipherName(F2, "CBC", ext);
    Handler first(F1, CIPHERTEXT);
    while(first.canExtract()){
        first.extract();
        uint8_t *out = cipher.Encrypt_CBC(first.getBuffer(), key, iv, first.getChunk());
        // printf("\n Plain text: |%s| \t <---> \t CipherText: |%s|",  first.getBuffer(), out);
        first.drop(out, 16);
    }
    first.close();
    Handler second(CIPHERTEXT, verifyName("CBC", ext));
    while(second.canExtract()){
        second.extract();
        uint8_t *out = cipher.Decrypt_CBC(second.getBuffer(), key, iv, second.getChunk() );
        // printf("\n Plain text: |%s| \t <---> \t CipherText: |%s|",  second.getBuffer(), out);
        second.drop(out, second.getChunk());
    }
    second.close();
}

void CFB(const char *F1, const char *F2, const char *ext){
    const char *CIPHERTEXT = cipherName(F2, "CFB", ext);
    Handler first(F1, CIPHERTEXT);
    while(first.canExtract()){
        first.extract();
        uint8_t *out = cipher.Encrypt_CFB(first.getBuffer(), key, iv, first.getChunk());
        // printf("\n Plain text: |%s| \t <---> \t CipherText: |%s|",  first.getBuffer(), out);
        first.drop(out, 16);
    }
    first.close();
    Handler second(CIPHERTEXT, verifyName("CFB", ext));
    while(second.canExtract()){
        second.extract();
        uint8_t *out = cipher.Decrypt_CFB(second.getBuffer(), key, iv, second.getChunk() );
        // printf("\n Plain text: |%s| \t <---> \t CipherText: |%s|",  second.getBuffer(), out);
        second.drop(out, second.getChunk());
    }
    second.close();
}

void OFB(const char *F1, const char *F2, const char *ext){
    const char *CIPHERTEXT = cipherName(F2, "OFB", ext);
    Handler first(F1, CIPHERTEXT);
    while(first.canExtract()){
        first.extract();
        uint8_t *out = cipher.Encrypt_OFB(first.getBuffer(), key, iv, first.getChunk());
        // printf("\n Plain text: |%s| \t <---> \t CipherText: |%s|",  first.getBuffer(), out);
        first.drop(out, 16);
    }
    first.close();
    Handler second(CIPHERTEXT, verifyName("OFB", ext));
    while(second.canExtract()){
        second.extract();
        uint8_t *out = cipher.Decrypt_OFB(second.getBuffer(), key, iv, second.getChunk() );
        // printf("\n Plain text: |%s| \t <---> \t CipherText: |%s|",  second.getBuffer(), out);
        second.drop(out, second.getChunk());
    }
    second.close();
}

void CTR(const char *F1, const char *F2, const char *ext){
    const char *CIPHERTEXT = cipherName(F2, "CTR", ext);
    Handler first(F1, CIPHERTEXT);
    while(first.canExtract()){
        first.extract();
        uint8_t *out = cipher.Encrypt_CTR(first.getBuffer(), key, iv, first.getChunk());
        // printf("\n Plain text: |%s| \t <---> \t CipherText: |%s|",  first.getBuffer(), out);
        first.drop(out, 16);
    }
    first.close();
    Handler second(CIPHERTEXT, verifyName("CTR", ext));
    while(second.canExtract()){
        second.extract();
        uint8_t *out = cipher.Decrypt_CTR(second.getBuffer(), key, iv, second.getChunk() );
        // printf("\n Plain text: |%s| \t <---> \t CipherText: |%s|",  second.getBuffer(), out);
        second.drop(out, second.getChunk());
    }
    second.close();
}