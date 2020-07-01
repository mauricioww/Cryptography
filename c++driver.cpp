#include "javaDriver.h"
#include "Handler.h"
#include "AES.h"
#include "Menu.h"
#include "Library.h"

 
using namespace std;


/*
        @input = source file without extension, destiny file to be created
        @output = Message sayin' "Finish"
*/
JNIEXPORT jstring JNICALL Java_javaDriver_Compute(JNIEnv *env, jobject thisObj, jstring source, jstring destiny, jstring ext){
    const char *SOURCE = env->GetStringUTFChars(source, NULL);
    const char *DESTINY = env->GetStringUTFChars(destiny, NULL);
    const char *EXT = env->GetStringUTFChars(ext, NULL);
    ECB(SOURCE, DESTINY, EXT);
    char mssg[] = "Finished";
    return env->NewStringUTF(mssg);
}