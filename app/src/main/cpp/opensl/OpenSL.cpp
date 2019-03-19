//
// Created by glumes on 2019/3/13.
//

#include "OpenSL.h"


void *buffer;

FILE *pcmFile;

uint8_t *out_buffer;


// 引擎接口
SLObjectItf engineObject = nullptr;
SLEngineItf engineEngine = nullptr;

// 混音器
SLObjectItf outputMixObject = nullptr;
SLEnvironmentalReverbItf outputMixEnvironmentalReverb = nullptr;
SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

// pcm
SLObjectItf pcmPlayerObject = NULL;
SLPlayItf pcmPlayerPlay = NULL;
SLVolumeItf pcmPlayerVolume = NULL;

//缓冲器队列接口
SLAndroidSimpleBufferQueueItf pcmBufferQueue = nullptr;

// uri plaer
SLObjectItf uriPlayerObject = nullptr;
SLPlayItf uriPlayerPlay = nullptr;
SLVolumeItf uriPlayerVolume = nullptr;

void getPcmData(void **pcm) {
    while (!feof(pcmFile)) {
        fread(out_buffer, 44100 * 2 * 2, 1, pcmFile);

        if (out_buffer != nullptr) {
            LOGD("%s", "reading");
        } else {
            LOGD("%s", "read end");
            break;
        }
        *pcm = out_buffer;
        break;
    }
}

void OpenSL::pcmBufferCallBack(SLAndroidSimpleBufferQueueItf bf, void *context) {

//    getPcmData(&buffer);
//    if (buffer != nullptr) {
//        SLresult result;
//        result = (*pcmBufferQueue)->Enqueue(pcmBufferQueue, buffer, 44100 * 2 * 2);
//
//    }

    LogClient::LogD("call back");
}


void pcmBufferCallBack2(SLAndroidSimpleBufferQueueItf bf, void *context) {

    getPcmData(&buffer);
    if (buffer != nullptr) {
        SLresult result;
        result = (*pcmBufferQueue)->Enqueue(pcmBufferQueue, buffer, 44100 * 2 * 2);

    }

    LogClient::LogD("call back");
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

//
//    //缓冲接口回调
//    (*pcmBufferQueue)->RegisterCallback(pcmBufferQueue,
//                                        pcmBufferCallBack, nullptr);

    //获取音量接口
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_VOLUME, &pcmPlayerVolume);

    //获取播放状态接口
    (*pcmPlayerPlay)->SetPlayState(pcmPlayerPlay, SL_PLAYSTATE_PLAYING);

    //主动调用回调函数开始工作
//    pcmBufferCallBack(pcmBufferQueue, NULL);


}

void OpenSL::startPlay() {
    if (engineEngine == nullptr) {
        createEngine();
    }

    createAudioPlayer();
}

void OpenSL::stopPlay() {

}

void OpenSL::startRecord() {

}

void OpenSL::stopRecord() {

}

void OpenSL::playFromUri() {

    const char *url = "http://mpge.5nd.com/2015/2015-11-26/69708/1.mp3";

    LogClient::LogD("create engine");
    createEngine();

    SLresult lresult;

    const SLInterfaceID mids[1] = {SL_IID_ENVIRONMENTALREVERB};

    const SLboolean mreq[1] = {SL_BOOLEAN_FALSE};

    lresult = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, mids, mreq);

    lresult = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);

    lresult = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
                                               &outputMixEnvironmentalReverb);

    if (SL_RESULT_SUCCESS == lresult) {
        lresult = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(
                outputMixEnvironmentalReverb, &reverbSettings
        );

        LogClient::LogD(" result ");
    }

    LogClient::LogD("create audio src");
    // create audio src
    SLDataLocator_URI loc_uri = {SL_DATALOCATOR_URI, (SLchar *) url};
    SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, nullptr, SL_CONTAINERTYPE_UNSPECIFIED};
    SLDataSource audioSrc = {&loc_uri, &format_mime};

    LogClient::LogD("create audio sink");
    // create audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSink = {&loc_outmix, nullptr};


    const SLInterfaceID ids[3] = {SL_IID_SEEK, SL_IID_MUTESOLO, SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    lresult = (*engineEngine)->CreateAudioPlayer(engineEngine, &uriPlayerObject, &audioSrc,
                                                 &audioSink, 3, ids, req);

    if (uriPlayerObject == nullptr) {
        LogClient::LogD("player object is null");
    } else {
        LogClient::LogD("is not null");
    }
    lresult = (*uriPlayerObject)->Realize(uriPlayerObject, SL_BOOLEAN_FALSE);

    if (SL_RESULT_SUCCESS != lresult) {
        (*uriPlayerObject)->Destroy(uriPlayerObject);
        uriPlayerObject = nullptr;
        return;
    }

    lresult = (*uriPlayerObject)->GetInterface(uriPlayerObject, SL_IID_PLAY, &uriPlayerPlay);

    lresult = (*uriPlayerObject)->GetInterface(uriPlayerObject, SL_IID_VOLUME, &uriPlayerVolume);

    if (uriPlayerPlay != nullptr) {
        LogClient::LogD("set playing state");
        lresult = (*uriPlayerPlay)->SetPlayState(uriPlayerPlay, SL_PLAYSTATE_PLAYING);
    }


    (*uriPlayerVolume)->SetVolumeLevel(uriPlayerVolume, 20 * -50);

    LogClient::LogD("end call");


}

void OpenSL::playFromAsset(int fd, off_t start, off_t length) {
    const char *url = "http://mpge.5nd.com/2015/2015-11-26/69708/1.mp3";

    LogClient::LogD("create engine");

    createEngine();

    SLresult lresult;

    const SLInterfaceID mids[1] = {SL_IID_ENVIRONMENTALREVERB};

    const SLboolean mreq[1] = {SL_BOOLEAN_FALSE};

    lresult = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, mids, mreq);

    lresult = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);

    lresult = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
                                               &outputMixEnvironmentalReverb);

    if (SL_RESULT_SUCCESS == lresult) {
        lresult = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(
                outputMixEnvironmentalReverb, &reverbSettings
        );

        LogClient::LogD(" result ");
    }

    LogClient::LogD("create audio src");
    // create audio src

    SLDataLocator_AndroidFD loc_fd = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
    SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, nullptr, SL_CONTAINERTYPE_UNSPECIFIED};
    SLDataSource audioSrc = {&loc_fd, &format_mime};

    LogClient::LogD("create audio sink");
    // create audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSink = {&loc_outmix, nullptr};


    const SLInterfaceID ids[3] = {SL_IID_SEEK, SL_IID_MUTESOLO, SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    lresult = (*engineEngine)->CreateAudioPlayer(engineEngine, &uriPlayerObject, &audioSrc,
                                                 &audioSink, 3, ids, req);

    if (uriPlayerObject == nullptr) {
        LogClient::LogD("player object is null");
    } else {
        LogClient::LogD("is not null");
    }
    lresult = (*uriPlayerObject)->Realize(uriPlayerObject, SL_BOOLEAN_FALSE);

    if (SL_RESULT_SUCCESS != lresult) {
        (*uriPlayerObject)->Destroy(uriPlayerObject);
        uriPlayerObject = nullptr;
        return;
    }

    lresult = (*uriPlayerObject)->GetInterface(uriPlayerObject, SL_IID_PLAY, &uriPlayerPlay);

    lresult = (*uriPlayerObject)->GetInterface(uriPlayerObject, SL_IID_VOLUME, &uriPlayerVolume);

    lresult = (*uriPlayerObject)->GetInterface(uriPlayerObject, SL_IID_BUFFERQUEUE,
                                               &pcmBufferQueue);

    if (pcmBufferQueue == nullptr) {
        LogClient::LogD("is null");
    }
    (*pcmBufferQueue)->RegisterCallback(pcmBufferQueue, pcmBufferCallBack2, nullptr);

    if (uriPlayerPlay != nullptr) {
        LogClient::LogD("set playing state");
        lresult = (*uriPlayerPlay)->SetPlayState(uriPlayerPlay, SL_PLAYSTATE_PLAYING);
    }


    (*uriPlayerVolume)->SetVolumeLevel(uriPlayerVolume, 20 * -50);

}

void OpenSL::playFromPcm(const char *path) {


    LogClient::LogD("create engine");

    pcmFile = fopen(path, "r");

    out_buffer = (uint8_t *) malloc(44100 * 2 * 2);


    createEngine();

    SLresult lresult;

    const SLInterfaceID mids[1] = {SL_IID_ENVIRONMENTALREVERB};

    const SLboolean mreq[1] = {SL_BOOLEAN_FALSE};

    lresult = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, mids, mreq);

    lresult = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);

    lresult = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
                                               &outputMixEnvironmentalReverb);

    if (SL_RESULT_SUCCESS == lresult) {
        lresult = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(
                outputMixEnvironmentalReverb, &reverbSettings
        );

        LogClient::LogD(" result ");
    }

    LogClient::LogD("create audio sink");
    // create audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSink = {&loc_outmix, nullptr};


    LogClient::LogD("create audio src");
    // create audio src


    SLDataLocator_AndroidSimpleBufferQueue android_queue = {SL_DATALOCATOR_ANDROIDBUFFERQUEUE, 2};

    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            2,
            SL_SAMPLINGRATE_44_1,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN
    };

    SLDataSource audioSrc = {&android_queue, &pcm};


    const SLInterfaceID ids[3] = {SL_IID_SEEK, SL_IID_MUTESOLO, SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    lresult = (*engineEngine)->CreateAudioPlayer(engineEngine, &uriPlayerObject, &audioSrc,
                                                 &audioSink, 3, ids, req);

    if (uriPlayerObject == nullptr) {
        LogClient::LogD("player object is null");
    } else {
        LogClient::LogD("is not null");
    }
    lresult = (*uriPlayerObject)->Realize(uriPlayerObject, SL_BOOLEAN_FALSE);

    if (SL_RESULT_SUCCESS != lresult) {
        (*uriPlayerObject)->Destroy(uriPlayerObject);
        uriPlayerObject = nullptr;
        return;
    }

    lresult = (*uriPlayerObject)->GetInterface(uriPlayerObject, SL_IID_PLAY, &uriPlayerPlay);

    lresult = (*uriPlayerObject)->GetInterface(uriPlayerObject, SL_IID_VOLUME, &uriPlayerVolume);


    lresult = (*uriPlayerObject)->GetInterface(uriPlayerObject, SL_IID_BUFFERQUEUE,
                                               &pcmBufferQueue);


    if (pcmBufferQueue == nullptr) {
        LogClient::LogD("is null");
    }

    (*pcmBufferQueue)->RegisterCallback(pcmBufferQueue, pcmBufferCallBack2, nullptr);

    if (uriPlayerPlay != nullptr) {
        LogClient::LogD("set playing state");
        lresult = (*uriPlayerPlay)->SetPlayState(uriPlayerPlay, SL_PLAYSTATE_PLAYING);
    }

    pcmBufferCallBack2(pcmBufferQueue, nullptr);

    (*uriPlayerVolume)->SetVolumeLevel(uriPlayerVolume, 20 * -50);
}
