#include "Handler.h"

Handler::Handler(const char *source, const char *destiny){
    SOURCE.open(source);
    DESTINY.open(destiny);
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

////////////////////////////
char * Handler::getBuffer2(){
    return auxBuffer;
}

////////////////////////////


void Handler::extract(){

    bzero(auxBuffer, 16);
    bzero(realBuffer, 16);

    chunk = (16 + seen < length) ? 16 : length - seen;

    SOURCE.read(auxBuffer, chunk);
    // cout << "Extrac func :|" <<  auxBuffer << "| -?> \t\t" << chunk << endl;
    printf("Extrac func: |%s| -> %d", auxBuffer, chunk);
    // drop(auxBuffer, chunk);
    memcpy(realBuffer, auxBuffer, chunk);
    seen += chunk;
}


bool Handler::canExtract(){
    return seen < length;
}

// void Handler::drop(uint8_t * str, uint8_t chunk){
void Handler::drop(char *str, uint8_t chunk2){
    DESTINY.write(str, chunk2);
}

void Handler::setOffset(uint8_t OF){
    offset = OF;
}

void Handler::drop2(uint8_t *str, uint8_t chunk2){
    bzero(auxBuffer, 16);

    // if(seen == length) chunk2 = 16;

    memcpy(auxBuffer, str, chunk2);

    printf("\t\t\tdrop2 str |%s| \t %d\n", str, chunk2);
    printf("\t\t\tdrop2 auxB aver |%s| %d\n", auxBuffer, sizeof(auxBuffer));

    // cout << "Aux:  " << str << endl;
    // if(seen == length){ 
    //     char *aux = new char[offset];
    //     memcpy(aux, str, offset);
    //     DESTINY.write(aux, offset);
    //     printf("\t\t\t Se escribio |%s| %d\n", aux, offset);
    //     // return;
    //     // memset(auxBuffer, 'a', 16);
    //     // bzero(auxBuffer, 16);
    // }
    
    // // else DESTINY.write(auxBuffer, chunk2);

    DESTINY.write(auxBuffer, chunk2);
}

void Handler::clear(uint32_t len){
    printf("Cleanin %d\n", len);
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