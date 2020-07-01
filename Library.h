#ifndef Library_H_
#define Library_H_

#include <iostream>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>

using namespace std;

class Library{
    private:
        ifstream SOURCE;
        ofstream MIDDLE1;
        ifstream MIDDLE2;
        ofstream DESTINY;
        uint32_t length;
        uint32_t pad;
        uint8_t *byteBuffer;
        char *charBuffer;
        const char* name;
    public:
        Library(const char *, const char *, const char *);
        uint8_t *get_initial_bytes();
        void write_middle_bytes(uint8_t *);
        uint32_t getLen();
        uint8_t *get_middle_bytes();
        void write_final_bytes(uint8_t *);
        void close();
};

#endif 