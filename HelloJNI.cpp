#include  <iostream>
#include "Handler.h"
#include "HelloJNI.h"
 
using namespace std;


void printChunk(char *chunk, int size){
    for(int i = 0; i < size; i++){
        printf("%c", chunk[i]);
    }
    cout << endl;
}

JNIEXPORT jstring JNICALL Java_HelloJNI_sayHello(JNIEnv *env, jobject thisObj, jstring source, jstring destiny){
    
    const char *inCStr1 = env->GetStringUTFChars(source, NULL);

    const char *inCStr2 = env->GetStringUTFChars(destiny, NULL);


    printf("%s\n", inCStr1);
    printf("%s\n", inCStr2);


   
    char mssg[] = "Finish";


    Handler driver(inCStr1, inCStr2);

    while(driver.canExtract()){
        driver.extract();
        printChunk(driver.getBuffer2(), driver.getChunk());
        driver.drop2(driver.getBuffer(), driver.getChunk());
    }

    driver.close();

    // env->ReleaseStringUTFChars(source, inCStr1);
    // env->ReleaseStringUTFChars(destiny, inCStr2);


    return env->NewStringUTF(mssg);

}