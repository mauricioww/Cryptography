#include "Handler.h"

Handler::Handler(const char *source, const char *destiny){
    SOURCE.open(source);
    DESTINY.open(destiny);
    auxBuffer = new char[16];
    realBuffer = new uint8_t[16];
    // chunk = (16 + seen < length) ? 16 : length - seen;
    seen = 0;
    SOURCE.seekg(0, SOURCE.end);
    length = SOURCE.tellg();
    SOURCE.seekg(0, SOURCE.beg);
}

uint8_t Handler::getChunk(){ 
    return chunk;
}

uint8_t * Handler::getBuffer(){
    return realBuffer;
}

////////////////////////////
char * Handler::getBuffer2(){
    return auxBuffer;
}

////////////////////////////


void Handler::extract(){


    chunk = (16 + seen < length) ? 16 : length - seen;
    // cout << (int) chunk << "\t" << seen << "\t  bytes read" << endl;

    SOURCE.read(auxBuffer, chunk);
    // cout << auxBuffer << endl;

    // drop(auxBuffer, chunk);
    memcpy(realBuffer, auxBuffer, chunk);
    seen += chunk;
}


bool Handler::canExtract(){
    return seen < length;
}

// void Handler::drop(uint8_t * str, uint8_t chunk){
void Handler::drop(char *str, uint8_t chunk){
    DESTINY.write(str, chunk);
}

void Handler::drop2(uint8_t *str, uint8_t chunk){
    memcpy(auxBuffer, realBuffer, chunk);
    DESTINY.write(auxBuffer, chunk);
}

void Handler::close(){
    SOURCE.close();
    DESTINY.close();
    delete[] auxBuffer;
    delete[] realBuffer;
} 