//
// Created by glumes on 2019/3/13.
//

#ifndef OPENSL_SAMPLE_OPENSL_H
#define OPENSL_SAMPLE_OPENSL_H


#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <android/log.h>
#include <stdio.h>

#include <LogClient.h>

#define LOG_TAG "NativeMethod"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

class OpenSL {

private:

//    // 引擎接口
//    SLObjectItf engineObject = nullptr;
//    SLEngineItf engineEngine = nullptr;
//
//    // 混音器
//    SLObjectItf outputMixObject = nullptr;
//    SLEnvironmentalReverbItf outputMixEnvironmentalReverb = nullptr;
//    SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;
//
//    // pcm
//    SLObjectItf pcmPlayerObject = NULL;
//    SLPlayItf pcmPlayerPlay = NULL;
//    SLVolumeItf pcmPlayerVolume = NULL;
//
//    //缓冲器队列接口
//    SLAndroidSimpleBufferQueueItf pcmBufferQueue;
//
//    // uri plaer
//    SLObjectItf uriPlayerObject = nullptr;
//    SLPlayItf uriPlayerPlay = nullptr;
//    SLVolumeItf uriPlayerVolume = nullptr;


public:
    void createEngine();

    void releaseEngine();

    void createAudioPlayer();


    void startPlay();

    void playFromUri();


    void playFromAsset(int fd, off_t start, off_t length);

    void playFromPcm(const char *path);

    void stopPlay();

    void stopRecord();

    void startRecord();

    void pcmBufferCallBack(SLAndroidSimpleBufferQueueItf bf, void *context);
};


#endif //OPENSL_SAMPLE_OPENSL_H
