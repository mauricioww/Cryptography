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

void ECB(const char *F1, const char *F2, const char *ext){
    const char *CIPHERTEXT = cipherName(F2, "ECB", ext);
    const char *verify = "./files/verify_ECB.class";

    Library handler(F1, CIPHERTEXT, verify);

    uint8_t *plaintextSRC = handler.get_initial_bytes();
    uint8_t *ciphertextSRC = cipher.Encrypt_ECB(plaintextSRC, key, handler.getLen());

    handler.write_middle_bytes(ciphertextSRC);

    uint8_t *ciphertextMDD = handler.get_middle_bytes();
    uint8_t *plaintextMDD = cipher.Decrypt_ECB(ciphertextMDD, key, handler.getLen());

    // printf("\n inverse %s \n", plaintextMDD);

    handler.write_final_bytes(plaintextMDD);
    handler.close();
}

void CBC(const char *F1, const char *F2, const char *ext){
    const char *CIPHERTEXT = cipherName(F2, "CBC", ext);
    const char *verify = "./files/verify_CBC.class";

    Library handler(F1, CIPHERTEXT, verify);

    uint8_t *plaintextSRC = handler.get_initial_bytes();
    uint8_t *ciphertextSRC = cipher.Encrypt_CBC(plaintextSRC, key, iv, handler.getLen());

    handler.write_middle_bytes(ciphertextSRC);

    uint8_t *ciphertextMDD = handler.get_middle_bytes();
    uint8_t *plaintextMDD = cipher.Decrypt_CBC(ciphertextMDD, key, iv, handler.getLen());

    // printf("\n inverse %s \n", plaintextMDD);

    handler.write_final_bytes(plaintextMDD);
    handler.close();
}

void CFB(const char *F1, const char *F2, const char *ext){
    const char *CIPHERTEXT = cipherName(F2, "CFB", ext);

    const char *verify = "./files/verify_CFB.class";

    Library handler(F1, CIPHERTEXT, verify);

    uint8_t *plaintextSRC = handler.get_initial_bytes();
    uint8_t *ciphertextSRC = cipher.Encrypt_CFB(plaintextSRC, key, iv, handler.getLen());

    handler.write_middle_bytes(ciphertextSRC);

    uint8_t *ciphertextMDD = handler.get_middle_bytes();
    uint8_t *plaintextMDD = cipher.Decrypt_CFB(ciphertextMDD, key, iv, handler.getLen());

    // printf("\n inverse %s \n", plaintextMDD);

    handler.write_final_bytes(plaintextMDD);
    handler.close();    
}

void OFB(const char *F1, const char *F2, const char *ext){
    const char *CIPHERTEXT = cipherName(F2, "OFB", ext);
    const char *verify = "./files/verify_OFB.class";

    Library handler(F1, CIPHERTEXT, verify);

    uint8_t *plaintextSRC = handler.get_initial_bytes();
    uint8_t *ciphertextSRC = cipher.Encrypt_OFB(plaintextSRC, key, iv, handler.getLen());

    handler.write_middle_bytes(ciphertextSRC);

    uint8_t *ciphertextMDD = handler.get_middle_bytes();
    uint8_t *plaintextMDD = cipher.Decrypt_OFB(ciphertextMDD, key, iv, handler.getLen());

    // printf("\n inverse %s \n", plaintextMDD);

    handler.write_final_bytes(plaintextMDD);
    handler.close();
}

void CTR(const char *F1, const char *F2, const char *ext){
    const char *CIPHERTEXT = cipherName(F2, "CTR", ext);
    const char *verify = "./files/verify_CTR.class";

    Library handler(F1, CIPHERTEXT, verify);

    uint8_t *plaintextSRC = handler.get_initial_bytes();
    uint8_t *ciphertextSRC = cipher.Encrypt_OFB(plaintextSRC, key, iv, handler.getLen());

    handler.write_middle_bytes(ciphertextSRC);

    uint8_t *ciphertextMDD = handler.get_middle_bytes();
    uint8_t *plaintextMDD = cipher.Decrypt_OFB(ciphertextMDD, key, iv, handler.getLen());

    // printf("\n inverse %s \n", plaintextMDD);

    handler.write_final_bytes(plaintextMDD);
    handler.close();
}