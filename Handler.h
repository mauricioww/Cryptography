#ifndef Handler_H_
#define Handler_H_

#include <iostream>
#include <cstdint>
#include <stdio.h>
#include <cstring>
#include <fstream>

using namespace std;


class Handler{
    private:
        ifstream SOURCE;
        ofstream DESTINY;
        uint32_t length;
        uint32_t seen;
        uint8_t chunk;
        uint8_t *realBuffer;
        char *auxBuffer;
    public:
        Handler(const char *, const char *);
        uint8_t *getBuffer();

        char *getBuffer2();

        uint8_t getChunk();
        bool canExtract();

        void extract();
        // void drop(uint8_t *, uint8_t);
        void drop(char *, uint8_t);

        void drop2(uint8_t *, uint8_t);
        void close();
};

#endif 