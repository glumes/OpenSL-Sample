//
// Created by glumes on 2019/3/13.
//

#include "OpenSL.h"


void pcmBufferCallBack(SLAndroidSimpleBufferQueueItf bf, void *context) {
//    //assert(NULL == context);
//    getPcmData(&buffer);
//    // for streaming playback, replace this test by logic to find and fill the next buffer
//    if (NULL != buffer) {
//        SLresult result;
//        // enqueue another buffer
//        result = (*pcmBufferQueue)->Enqueue(pcmBufferQueue, buffer, 44100 * 2 * 2);
//        // the most likely other result is SL_RESULT_BUFFER_INSUFFICIENT,
//        // which for this code example would indicate a programming error
//    }
}

void OpenSL::createEngine() {
    SLresult result;

    // 创建引擎对象
    result = slCreateEngine(&engineObject, 0, nullptr, 0, nullptr, nullptr);
    // 实例化
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    // 获得引擎接口对象
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);

}


void OpenSL::releaseEngine() {
    if (engineObject != nullptr) {
        (*engineObject)->Destroy(engineObject);
        engineObject = nullptr;
        engineEngine = nullptr;
    }
}

void OpenSL::createAudioPlayer() {
    if (engineEngine != nullptr) {
        return;
    }

    // 配置 PCM 数据源格式
    SLDataLocator_AndroidSimpleBufferQueue android_queue = {
            SL_DATALOCATOR_ANDROIDBUFFERQUEUE,
            1
    };

    SLDataFormat_PCM data_pcm = {
            SL_DATAFORMAT_PCM,
            2,
            SL_SAMPLINGRATE_44_1,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN
    };

    SLDataSource dataSource = {
            &android_queue,
            &data_pcm
    };


    // 创建混音器
    SLresult result;

    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 0, 0, 0);

    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);

    result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
                                              &outputMixEnvironmentalReverb);

    if (result == SL_RESULT_SUCCESS) {
        result = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(
                outputMixEnvironmentalReverb, &reverbSettings);
    }


    SLDataLocator_OutputMix outputMix = {
            SL_DATALOCATOR_OUTPUTMIX,
            outputMixObject
    };

    SLDataSink dataSink = {
            &outputMix,
            nullptr
    };

    // 创建播放器
    SLInterfaceID interfaceIDs[] = {
            SL_IID_BUFFERQUEUE,
            SL_IID_EFFECTSEND,
            SL_IID_VOLUME
    };

    SLboolean requiredInterfaces[] = {
            SL_BOOLEAN_TRUE,
            SL_BOOLEAN_TRUE,
            SL_BOOLEAN_TRUE
    };

    result = (*engineEngine)->CreateAudioPlayer(
            engineEngine,
            &pcmPlayerObject,
            &dataSource,
            &dataSink,
            3,
            interfaceIDs,
            requiredInterfaces
    );

    //初始化播放器
    (*pcmPlayerObject)->Realize(pcmPlayerObject, SL_BOOLEAN_FALSE);

    //得到接口后调用  获取Player接口
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_PLAY, &pcmPlayerPlay);

    //注册回调缓冲区 获取缓冲队列接口
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_BUFFERQUEUE, &pcmBufferQueue);

    //缓冲接口回调
    (*pcmPlayerObject)->RegisterCallback(reinterpret_cast<SLObjectItf>(pcmBufferQueue),
                                         (slObjectCallback) pcmBufferQueue, nullptr);

    //获取音量接口
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_VOLUME, &pcmPlayerVolume);

    //获取播放状态接口
    (*pcmPlayerPlay)->SetPlayState(pcmPlayerPlay, SL_PLAYSTATE_PLAYING);

    //主动调用回调函数开始工作
    pcmBufferCallBack(pcmBufferQueue, NULL);


}
