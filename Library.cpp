#include "Library.h"

Library::Library(const char *src, const char *mdd, const char *dst){
    SOURCE.open(src, ifstream::binary);
    MIDDLE1.open(mdd, ofstream::binary);
    name = mdd;
    DESTINY.open(dst, ofstream::binary);

    SOURCE.seekg(0, SOURCE.end);
    length = SOURCE.tellg(), pad = (len % 16) ? (len/16 + 1) * 16 : len;
    SOURCE.seekg(0, SOURCE.beg);    
}

uint8_t *Library::get_itinial_bytes(){
    byteBuffer = new uint8_t[length];
    charBuffer = new char[length];
    SOURCE.read(charBuffer, length);
    memcpy(byteBuffer, charBuffer, length);
    return byteBuffer;
}

uint32_t Library::getLen(){
    return length;
}

void Library::write_middle_bytes(uint8_t *bytes){
    charBuffer = new char[pad];
    memcpy(charBuffer, bytes, pad);
    MIDDLE1.write(charBuffer, pad);
    // MIDDLE.seekg(0, MIDDLE.beg); // rewind
    MIDDLE1.close();
}

uint8_t *Library::get_middle_bytes(){
    MIDDLE2.open(name, ifstream::binary);
    byteBuffer = new uint8_t[pad];
    charBuffer = new char[pad];
    SOURCE.read(charBuffer, pad);
    memcpy(byteBuffer, charBuffer, pad);
    MIDDLE2.close()
    return byteBuffer;
}

void Library::write_final_bytes(uint8_t *bytes){
    charBuffer = new char[len];
    memcpy(charBuffer, bytes, len);
    DESTINY.write(charBuffer);
}

void Library::close(){
    delete[] charBuffer;
    delete[] byteBuffer;
    SOURCE.close();
    MIDDLE.close();
    DESTINY.close();
}