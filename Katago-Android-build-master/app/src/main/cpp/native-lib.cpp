#include <jni.h>
#include <android/log.h>
#include "main.h"
#include "tests/tests.h"
#include "game/board.h"
#include "command/commandline.h"
#include "main.h"
#include "core/makedir.h"
#include "core/datetime.h"
#include "program/setup.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>


using namespace std;

void printToNativeLog(const char* message) {
    __android_log_print(ANDROID_LOG_ERROR, "NativeLog", "%s", message);

}

void readFileTest(const string& filepath) {
    ifstream file;
    file.open(filepath);

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            printToNativeLog(line.c_str());
        }
        file.close();
    } else{
        printToNativeLog("无法打开文件");
    }

}



extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_katago_Native_getKatagoVersion(JNIEnv *env, jclass clazz) {
    string version = Version::getKataGoVersion();

    const char* versionStr = version.c_str();

    jstring jVersionStr = env->NewStringUTF(versionStr);

    return jVersionStr;

}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_katago_Native_getKatagoFullInfo(JNIEnv *env, jclass clazz) {

    string info = Version::getKataGoVersionFullInfo();

    const char* versionStr = info.c_str();

    jstring jVersionStr = env->NewStringUTF(versionStr);

    return jVersionStr;
}


extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_katago_Native_test(JNIEnv *env, jclass clazz) {

    string filePath = "configs/gtp_example.cfg";
    readFileTest(filePath);


    string version = Version::getKataGoVersion();
    const char* versionStr = version.c_str();
    jstring jVersionStr = env->NewStringUTF(versionStr);
    return jVersionStr;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_katago_Native_getCfgFromAssets(JNIEnv *env, jclass clazz, jobject asset_manager) {

    //从assets中读取cfg文件并逐行解析
    AAssetManager *mgr = AAssetManager_fromJava(env,asset_manager);
    AAsset *asset = AAssetManager_open(mgr,"gtp_example.cfg",AASSET_MODE_BUFFER);
    const char *buffer = static_cast<const char*>(AAsset_getBuffer(asset));
    off_t length = AAsset_getLength(asset);
    string content(buffer,static_cast<size_t>(length));

    vector<string> lines;
    istringstream iss(content);
    string line;

    size_t commentPos = line.find("#");

    while(getline(iss,line)){
        if (!line.empty()){
            //删除注释，即’#‘之后的内容 并删除空白行
            if (commentPos != string::npos){
                line = line.substr(0,commentPos);
            }
            printToNativeLog(line.c_str());
            lines.push_back(line);
        }

    }


    AAsset_close(asset);


}