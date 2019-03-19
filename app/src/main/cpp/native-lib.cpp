#include <jni.h>
#include <string>


#include <OpenSL.h>

#include <LogClient.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

static OpenSL *mOpenSL;

extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_opensl_1sample_OpenSLInstance_startRecord(JNIEnv *env, jclass type) {


}

extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_opensl_1sample_OpenSLInstance_stopRecord(JNIEnv *env, jclass type) {


}

extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_opensl_1sample_OpenSLInstance_startPlay(JNIEnv *env, jclass type) {

    mOpenSL = new OpenSL();

    if (mOpenSL != nullptr) {
        LogClient::LogD("stat play");
        mOpenSL->playFromUri();
    } else {
        LogClient::LogD("is null");
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_opensl_1sample_OpenSLInstance_stopPlay(JNIEnv *env, jclass type) {

    mOpenSL->stopPlay();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_opensl_1sample_OpenSLInstance_startPlayFromAssets(JNIEnv *env, jclass type,
                                                                  jobject assetManager,
                                                                  jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);

    AAssetManager *mgr = AAssetManager_fromJava(env, assetManager);
    AAsset *asset = AAssetManager_open(mgr, path, AASSET_MODE_UNKNOWN);
    env->ReleaseStringUTFChars(path_, path);

    off_t start, length;

    int fd = AAsset_openFileDescriptor(asset, &start, &length);

    AAsset_close(asset);

    mOpenSL = new OpenSL();

    if (mOpenSL != nullptr) {
        mOpenSL->playFromAsset(fd, start, length);
    }

}