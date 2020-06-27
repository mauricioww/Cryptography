#include "AES.h"

#define UPDATE(to, from) { (to)[0] = (from)[0]; (to)[1] = (from)[1]; (to)[2] = (from)[2]; (to)[3] = (from)[3];}

// NOTE: state matrix always is 4x4 -> 16 * 8 = 128 input bits

AES::AES(int len = 128){
    if(len != 128 && len != 192 && len != 256){
        cout << "KEY LENGTH ERROR: 128, 192, 256." << endl;
        exit(-1);
    }
    keyLen = len / 8;
    Nk = len / 32;
    Nr += Nk;
    // printf("keyLen = %i, N rows = %i, N cols = %i, N rounds = %i\n", keyLen*8, Nk, Nb, Nr);
}

void AES::increment(uint8_t *ctr){
    for(int i = 15; i >= 0; i--){
        if(++ctr[i] != 0) break;
    }
}

uint8_t AES::xtime(uint8_t byte){
    return (byte & 0x80) ? (((byte << 1) ^ 0x1B) & 0xFF) : byte << 1;
}

/*
        @input = length of the input text to cipher
        @output = the closest number to one mult of 16 
*/
uint32_t AES::padding(uint32_t len){
    return (len % 16) ? (len/16 + 1) * 16 : len;
}

/*
        @input = input text, original leght of text, padding length
        @output = new padding text 
*/
uint8_t* AES::padInput(uint8_t *in, uint32_t oldL, uint32_t newL){
    uint8_t *newInput = new uint8_t[newL];
    memcpy(newInput, in, newL); // old to new
    return newInput;
}

/*
        @input = pointer A, pointer B, length of both pointers (it is the same)
        It computes the xor between these both and store the result in the first one
*/
void AES::xorChunks(uint8_t *A, uint8_t *B, uint8_t times){
    while(times--) A[times] ^= B[times];
}

/*
        @input = state matrix
        It computes the permutation according to the sBox previously defined 
*/
void AES::subBytes(uint8_t **state){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            uint8_t temp = state[i][j];
            state[i][j] = sBox[temp>>4][temp&0x0f];
        }
    }
}

/*
        @input = state matrix
        It makes a left-ring permutation according to the current number row.
        NOTE: I use an aux macro defined on the top.
*/
void AES::shiftRows(uint8_t **state){
    for(int i = 1; i < 4; i++){
        uint8_t row[4], index = 0;
        for(int j = i; j < i + 4; j++)
            row[index++] = state[i][j%4];
        UPDATE(state[i], row)
    }
}

/*      
        @input = Array of pointers to the state matrix column in order to update
        their values from the original address.
        The algorithm is based on 'The design of rijndael'.
        # see Sec 4.1.3 in The Design of Rijndael
*/
void AES::mixOneCol(uint8_t *col[4]){
    uint8_t t =  *col[0] ^ *col[1] ^ *col[2] ^ *col[3], aux = *col[0];
    for(int i = 0; i < 3; i++) 
        *col[i] ^= t ^ xtime(*col[i] ^ *col[i+1]);
    *col[3] ^= t ^ xtime(*col[3] ^ aux);
}

/*
        @input = state matrix
        It computes the the product between a column of state matrix and the 
        polynomial a(x) = {0x03}x³ + {0x01}x² + {0x01}x + {0x02} 
*/
void AES::mixColumns(uint8_t **state){
    for(int i = 0; i < 4; i++){
        uint8_t *col[4] = {&state[0][i], &state[1][i], &state[2][i], &state[3][i]};
        mixOneCol(col);
    }
}

/*
        @input = state matrix and key
        It just computes the xor for each input in the state matrix.
*/
void AES::addRoundKey(uint8_t **state, uint8_t *key){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            state[i][j] ^= key[i + 4*j];
        }
    }
}

/*
        @input = state matrix
        It set the original orden in the state matrix with the inverse sBox.
*/
void AES::invSubBytes(uint8_t **state){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            uint8_t temp = state[i][j];
            state[i][j] = i_sBox[temp>>4][temp&0x0f];
        }
    }
}

/*
        @input = state matrix
        It makes a rigth-ring permutation according to the current number row.
        NOTE: I use an aux macro defined on the top.
*/
void AES::invShiftRows(uint8_t **state){
    for(int i = 1; i < 4; i++){
        uint8_t row[4], index = 0;
        for(int j = 4 - i; j < 8 - i; j++)
            row[index++] = state[i][j%4];
        UPDATE(state[i], row);
    }
}

/*
        @input = state matrix
        It computes the the product between a column of state matrix and the 
        polynomial a(x) = {0x0b}x³ + {0x0d}x² + {0x09}x + {0x0de} 

        # see Sec 4.1.3 in The Design of Rijndael
*/

// ============================= NOTE =================================
// MixColumns polynomial -> c(x) & InvMixColumns polynomial -> d(x)
// d(x) = (04x2 + 05)c(x) (mod X4 + 01) . 

void AES::invMixColumns(uint8_t **state){
    for(int i = 0; i < 4; i++){
        uint8_t u = xtime(xtime(state[0][i] ^ state[2][i]));
        uint8_t v = xtime(xtime(state[1][i] ^ state[3][i]));
        state[0][i] ^= u;
        state[1][i] ^= v;
        state[2][i] ^= u;
        state[3][i] ^= v;
    }
    mixColumns(state);
}

/* 
        @input = A four-byte word
        It applies the S-Box to the input.
*/
void AES::subWord(uint8_t w[4]){
    for(int i = 0; i < 4; i++){
        uint8_t t = w[i];
        w[i] = sBox[t>>4][t&0x0f];
    }
}

/* 
        @input = A four-byte word
        It applies a left-ring permutation to the input.
*/
void AES::rotWord(uint8_t w[4]){
    uint8_t t = w[0];
    w[0] = w[1]; w[1] = w[2]; w[2] = w[3]; w[3] = t;
}


void printCol(unsigned char *str){
    for(int i = 0; i < 4; i++)
        // printf("%c ", in[i]);
        printf("0x%02x ", str[i]);
    puts("");
    
}

void AES::keyExpansion(uint8_t *key/*[keyLen]*/, uint8_t *keyExpanded/*[4 *  Nb * (Nr+1)]*/){
    uint8_t temp[4];
    int RCON = 1, bytesGenerated = 4 * Nk;
    memcpy(keyExpanded, key, 4*Nk); // Copy key to word
    while(bytesGenerated < 4 * Nb * (Nr+1)){
        for(int j = 0; j < 4; j++) temp[j] = keyExpanded[bytesGenerated + j - 4]; // Fill temporal var
        if(bytesGenerated/4 % Nk == 0){ // Apply aux functs to the temp var
            rotWord(temp);
            subWord(temp); 
            temp[0] ^= rcon[RCON++]; // Next RCON index
        }
        else if(Nk > 6 && bytesGenerated/4 % Nk == 4) // Only if keyLen = 256 bits
            subWord(temp);
        for(int j = 0; j < 4; j++){  // Add to the word
            keyExpanded[bytesGenerated] = keyExpanded[bytesGenerated - 4 * Nk] ^ temp[j];
            bytesGenerated++;
        }
    }
}

void printA(uint8_t **arr){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            // cout << (int)arr[i][j] << " ";
            printf("0x%02x,  ", arr[i][j]);
        }
        puts("");
    }    
    puts("");
}

void printKey(uint8_t *key, int len){
    for(int i = 0; i < len; i++){
        printf("0x%02x ", key[i]);
        if(i%4 == 0) puts("");
    }
}

void AES::Encrypt(uint8_t *in, uint8_t *out, uint8_t *key){
    uint8_t *keyExpanded = new uint8_t[4 * Nb * (Nr + 1)], **state = new uint8_t *[4];
    keyExpansion(key, keyExpanded);

    for(int i = 0; i < 4; i++){ // Copy input to the state matrix
        state[i] = new uint8_t[4];
        for(int j = 0; j < 4; j++)
            state[i][j] = in[i + 4*j]; // i*4 + j
    }

    addRoundKey(state, keyExpanded); // Initial round

    for(int i = 1; i <= Nr - 1; i++){ // From 1 until Nr
        // printA(state);
        subBytes(state);
        shiftRows(state);
        mixColumns(state);
        addRoundKey(state, keyExpanded + i * 4 * Nb);
    }

    subBytes(state); // Final round
    shiftRows(state);
    addRoundKey(state, keyExpanded + Nr * 4 * Nb);

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++) out[i + 4*j] = state[i][j]; // Fill output
    }

    delete[] keyExpanded;
    delete[] state;
}

void AES::Decrypt(uint8_t *in, uint8_t *out, uint8_t *key){
    uint8_t *keyExpanded = new uint8_t[4 * Nb * (Nr + 1)], **state = new uint8_t *[4];

    keyExpansion(key, keyExpanded);

    for(int i = 0; i < 4; i++){ // Copy input to the state matrix
        state[i] = new uint8_t[4];
        for(int j = 0; j < 4; j++)
            state[i][j] = in[i + 4*j]; // i*4 + j
    }

    addRoundKey(state, keyExpanded + Nr * 4 * Nb); // First round

    // printA(state);


    for(int i = Nr - 1; i >= 1; i--){ // From Nr until 1
        invSubBytes(state);
        invShiftRows(state);
        addRoundKey(state, keyExpanded + i * 4 * Nb);
        // printA(state);
        // cout << "\n" <<endl;
        invMixColumns(state);
        // printA(state);

    }

    invSubBytes(state); // Final round
    invShiftRows(state);
    addRoundKey(state, keyExpanded); 


    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++) out[i + 4*j] = state[i][j]; // Fill output
    }

    delete[] keyExpanded;
    delete[] state;
}


void printStr1(uint8_t *str){
    for(int i = 0; i < 16; i++)
        // printf("%c ", in[i]);
        printf("0x%02x ", str[i]);
    puts("");
}

// sizeof(in) = len ?
uint8_t* AES::Encrypt_ECB(uint8_t *plainT, uint8_t *key, uint32_t len){
    uint32_t real = padding(len);
    // cout << "real = " << real << "input " << len << endl;
    uint8_t *padding = padInput(plainT, len, real);
    // printStr1(padding);
    uint8_t *cipherT = new uint8_t[real];
    for(uint32_t i = 0; i < real; i += 16){
        Encrypt(padding + i, cipherT + i, key);
    }
    delete[] padding;
    return cipherT;
}

uint8_t* AES::Decrypt_ECB(uint8_t *cipherT, uint8_t *key, uint32_t len){
    uint8_t *plainT = new uint8_t[len];
    for(uint32_t i = 0; i < len; i += 16){
        Decrypt(cipherT + i, plainT + i, key);
    }
    return plainT;
}


uint8_t* AES::Encrypt_CBC(uint8_t *plainT, uint8_t *key, uint8_t *iv, uint32_t len){
    uint32_t real = padding(len);
    uint8_t *padding = padInput(plainT, len, real);
    uint8_t *cipherT = new uint8_t[real];
    uint8_t *aux = new uint8_t[16];
    memcpy(aux, iv, 16);
    for(uint32_t i = 0; i < real; i += 16){
        xorChunks(aux, padding + i, 16);
        Encrypt(aux, cipherT + i, key);
        memcpy(aux, cipherT + i, 16);
    }
    delete[] aux; delete[] padding;
    return cipherT;
}

uint8_t* AES::Decrypt_CBC(uint8_t *cipherT, uint8_t *key, uint8_t *iv, uint32_t len){
    uint8_t *plainT = new uint8_t[len];
    uint8_t *aux = new uint8_t[16];
    memcpy(aux, iv, 16);
    for(uint32_t i = 0; i < len; i += 16){
        Decrypt(cipherT + i, plainT + i, key);
        xorChunks(plainT + i, aux, 17);
        printStr1(plainT);

        memcpy(aux, cipherT + i, 16);
    }
    delete[] aux;
    return plainT;
}


uint8_t* AES::Encrypt_CFB(uint8_t *plainT, uint8_t *key, uint8_t *iv, uint32_t len){
    uint32_t real = padding(len);
    uint8_t *padding = padInput(plainT, len, real);
    uint8_t *cipherT = new uint8_t[real];
    uint8_t *auxIV = new uint8_t[16];
    uint8_t *encryptedIV = new uint8_t[16];
    memcpy(auxIV, iv, 16);
    for(uint32_t i = 0; i < real; i += 16){
        Encrypt(auxIV, encryptedIV, key);               // First cipher the IV or the previous output cipherText
        xorChunks(cipherT + i, encryptedIV, 16);      // Save the pre-cipher to the output cipherText
        xorChunks(cipherT + i, padding + i, 16);    // Now, xor the value in output with the plainText
        memcpy(auxIV, cipherT + i, 16);
    }
    delete[] padding; delete[] auxIV; delete[] encryptedIV;
    return cipherT;
}

uint8_t* AES::Decrypt_CFB(uint8_t *cipherT, uint8_t *key, uint8_t *iv, uint32_t len){
    uint8_t *plainT = new uint8_t[len];
    bzero(plainT, len);
    uint8_t *auxIV = new uint8_t[16];
    uint8_t *encryptedIV = new uint8_t[16];
    memcpy(auxIV, iv, 16);
    for(uint32_t i = 0; i < len; i += 16){
        Encrypt(auxIV, encryptedIV, key);
        xorChunks(plainT + i, encryptedIV, 16);
        xorChunks(plainT + i, cipherT + i, 16);
        memcpy(auxIV, cipherT + i, 16);
    }
    delete[] auxIV; delete[] encryptedIV;
    return plainT;
}

uint8_t* AES::Encrypt_OFB(uint8_t *plainT, uint8_t *key, uint8_t *iv, uint32_t len){
    uint32_t real = padding(len);
    uint8_t *padding = padInput(plainT, len, real);
    uint8_t *cipherT = new uint8_t[real];
    uint8_t *auxIV = new uint8_t[16];
    uint8_t *encryptedIV = new uint8_t[16];
    memcpy(auxIV, iv, 16);
    for(uint32_t i = 0; i < real; i += 16){
        Encrypt(auxIV, encryptedIV, key);
        memcpy(auxIV, encryptedIV, 16);
        xorChunks(cipherT + i, encryptedIV, 16);
        xorChunks(cipherT + i, padding + i, 16);
    }
    delete[] padding; delete[] auxIV; delete[] encryptedIV;
    return cipherT;
}

uint8_t* AES::Decrypt_OFB(uint8_t *cipherT, uint8_t *key, uint8_t *iv, uint32_t len){
     uint8_t *plainT = new uint8_t[len];
    bzero(plainT, len);
    uint8_t *auxIV = new uint8_t[16];
    uint8_t *encryptedIV = new uint8_t[16];
    memcpy(auxIV, iv, 16);
    for(uint32_t i = 0; i < len; i += 16){
        Encrypt(auxIV, encryptedIV, key);
        xorChunks(plainT + i, encryptedIV, 16);
        xorChunks(plainT + i, cipherT + i, 16);
        memcpy(auxIV, encryptedIV, 16);
    }
    delete[] auxIV; delete[] encryptedIV;
    return plainT;
}

uint8_t* AES::Encrypt_CTR(uint8_t *plainT, uint8_t *key, uint8_t *ctr, uint32_t len){
    uint32_t real = padding(len);
    uint8_t *padding = padInput(plainT, len, real);
    uint8_t *cipherT = new uint8_t[real];
    uint8_t *auxCTR = new uint8_t[16];
    uint8_t *encryptedCTR = new uint8_t[16];
    memcpy(auxCTR, ctr, 16);
    for(uint32_t i = 0; i < real; i += 16){
        Encrypt(auxCTR, encryptedCTR, key);
        xorChunks(cipherT + i, encryptedCTR, 16);
        xorChunks(cipherT + i, plainT + i, 16);
        increment(auxCTR);
    }
    delete[] padding; delete[] auxCTR; delete[] encryptedCTR;
    return cipherT;
}

uint8_t* AES::Decrypt_CTR(uint8_t *cipherT, uint8_t *key, uint8_t *ctr, uint32_t len){
    uint8_t *plainT = new uint8_t[len];
    uint8_t *auxCTR = new uint8_t[16];
    uint8_t *encryptedCTR = new uint8_t[16];
    memcpy(auxCTR, ctr, 16);
    for(uint32_t i = 0; i < len; i += 16){
        Decrypt(auxCTR, encryptedCTR, key);
        xorChunks(plainT + i, encryptedCTR, 16);
        xorChunks(plainT + i, cipherT + i, 16);
        increment(auxCTR);
    }
    delete[] auxCTR; delete[] encryptedCTR;
    return plainT;
}