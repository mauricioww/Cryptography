#include "Handler.h"

Handler::Handler(const char *source, const char *destiny){
    SOURCE.open(source, ifstream::binary);
    DESTINY.open(destiny, ofstream::binary);
    auxBuffer = new char[16];
    realBuffer = new uint8_t[16];
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

uint8_t Handler::getLength(){
    return length;
}

void Handler::extract(){
    bzero(auxBuffer, 16);
    bzero(realBuffer, 16);
    chunk = (16 + seen < length) ? 16 : length - seen;
    SOURCE.read(auxBuffer, chunk);
    memcpy(realBuffer, auxBuffer, chunk);
    seen += chunk;
}

bool Handler::canExtract(){
    return seen < length;
}

void Handler::setOffset(uint8_t OF){
    offset = OF;
}

void Handler::drop(uint8_t *str, uint8_t chunk2){
    bzero(auxBuffer, 16);
    memcpy(auxBuffer, str, chunk2);
    DESTINY.write(auxBuffer, chunk2);
}

void Handler::clear(uint32_t len){
    char *temp = new char[len];
    SOURCE.read(auxBuffer, len);
    DESTINY.write(auxBuffer, len);
    delete[] temp;
}

void Handler::close(){
    SOURCE.close();
    DESTINY.close();
    delete[] auxBuffer;
    delete[] realBuffer;
} 