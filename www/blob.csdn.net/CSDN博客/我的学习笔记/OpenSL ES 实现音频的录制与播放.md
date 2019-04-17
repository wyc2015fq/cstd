# OpenSL ES 实现音频的录制与播放 - 我的学习笔记 - CSDN博客





2018年07月28日 21:26:36[zouzhiheng](https://me.csdn.net/u011330638)阅读数：135








# 前言

网上关于 OpenSL ES 的资料相当少，最好的学习办法似乎是抄写 [googlesamples/android-ndk](https://github.com/googlesamples/android-ndk) 里的代码示例。这里打算使用 OpenSL ES 实现三个功能： 

1) 播放 MP3 文件 

2) 播放 PCM 文件 

3) 录制 PCM 文件
其中第 2 个功能和第 1 个雷同，这篇博客里不会给出代码示例，需要的可以看 [GitHub](https://github.com/zouzhiheng/AVGraphics)。

备注：CSDN 的样式太难看了，需要的可以到简书阅读本文：[https://www.jianshu.com/p/5513a181580f](https://www.jianshu.com/p/5513a181580f)

# OpenSL ES 概述

OpenSL ES(Open Sound Library for Embedded Systems)，是一种针对嵌入式系统特别优化过的硬件音频加速 API，免费并且可以跨平台使用。OpenSL ES 使用缓冲区队列机制，可以提供比系统 API 更高的性能。

在 OpenSL ES 中，所有的操作都是通过接口来完成的，尽管它使用了 C 语言来编写，但它的接口采用的是面向对象的，重要的几个接口是： 

    1) SLObjectItf ：对象接口 

    2) SLEngineItf ：引擎接口 

    3) SLPlayItf：播放接口 

    4) SLRecordItf：录制接口 

    5) SLBufferQueueItf :缓冲队列接口
其中： 

    1) Engine 是程序的入口，用于创建各种 Object 

    2) 一个 Object 对应一个或多个 Interface，Object 提供 Realize、Deatory、GetInterface 等基本接口 

    3) 实际的功能由各个 Interface 完成，比如播放音频，实际上是通过 SLPlayItf 来完成的
OpenSL ES 在不同系统上有不同的实现，有些接口在一些平台上可能无法使用，比如 Android 平台可以参考官方文档：[面向 Android 的 OpenSL ES](https://developer.android.com/ndk/guides/audio/opensl-for-android?hl=zh-cn) 来查看 Android 支持/不支持哪些功能。

以播放音频为例，最简单的代码编写流程为： 

1) Engine::CreateAudioPlayer 

2) Object::Realize 

3) Object::GetInterface 

4) Play::SetPlayState
其中，Object 可能的状态有：

```
/* Object state definitions */
#define SL_OBJECT_STATE_UNREALIZED ((SLuint32) 0x00000001)
#define SL_OBJECT_STATE_REALIZED      ((SLuint32) 0x00000002)
#define SL_OBJECT_STATE_SUSPENDED  ((SLuint32) 0x00000003)
```

使用 Engine 创建了一个 Object 时，它的状态是 UNREALIZED，必须使用方法 Realize 完成初始化，进入 REALIZED 状态，才可以进行下一步操作。如果出现错误或其它问题，它就会进入 SUSPENDED 状态，需要使用 Resume 方法才可以回到 REALIZED 状态。

# 播放 MP3 文件

这里以播放 Android 平台上的 asset 文件为例。

上面说过，Engine 是程序的入口，因此第一件事是创建 SLEngineItf：

```
void createEngine() {
    SLresult result;

    result = slCreateEngine(&engineObj, 0, nullptr, 0, nullptr, nullptr);
    assert(result == SL_RESULT_SUCCESS);
    (void) result;

    result = (*engineObj)->Realize(engineObj, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);
    (void) result;

    result = (*engineObj)->GetInterface(engineObj, SL_IID_ENGINE, &engine);
    assert(SL_RESULT_SUCCESS == result);
    (void) result;

    const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean req[1] = {SL_BOOLEAN_FALSE};
    (*engine)->CreateOutputMix(engine, &outputMixObj, 1, ids, req); // outputMixObj 用于输出声音数据

    result = (*outputMixObj)->Realize(outputMixObj, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);
    (void) result;
}
```

接着初始化播放器：

```
void AssetAudioPlayer::initPlayer(AAsset *asset) {
    SLresult result;

    off_t start, length;
    int fd = AAsset_openFileDescriptor(asset, &start, &length); // Android 平台的接口，获取文件描述符
    assert(fd >= 0);
    AAsset_close(asset);

    SLDataLocator_AndroidFD locFD = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
    SLDataFormat_MIME formatMime = {SL_DATAFORMAT_MIME, nullptr, SL_CONTAINERTYPE_UNSPECIFIED};
    SLDataSource audioSrc = {&locFD, &formatMime}; // 配置音频数据源

    SLDataLocator_OutputMix locOutputMix = {SL_DATALOCATOR_OUTPUTMIX, mAudioEngine->outputMixObj};
    SLDataSink audioSink = {&locOutputMix, nullptr}; // 配置音频数据输出池

    const SLInterfaceID ids[3] = {SL_IID_SEEK, SL_IID_MUTESOLO, SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    result = (*mAudioEngine->engine)->CreateAudioPlayer(mAudioEngine->engine, &mPlayerObj, // mPlayerObj 类型为 SLObjectItf
                                                        &audioSrc, &audioSink, 3, ids, req);
    assert(SL_RESULT_SUCCESS == result);
    (void) result;

    result = (*mPlayerObj)->Realize(mPlayerObj, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);
    (void) result;

    result = (*mPlayerObj)->GetInterface(mPlayerObj, SL_IID_PLAY, &mPlayer); // mPlayer 类型为 SLPlayItf，以下接口同理
    assert(SL_RESULT_SUCCESS == result);
    (void) result;

    result = (*mPlayerObj)->GetInterface(mPlayerObj, SL_IID_SEEK, &mSeek);
    assert(SL_RESULT_SUCCESS == result);
    (void) result;

    result = (*mPlayerObj)->GetInterface(mPlayerObj, SL_IID_MUTESOLO, &mMuteSolo);
    assert(SL_RESULT_SUCCESS == result);
    (void) result;

    result = (*mPlayerObj)->GetInterface(mPlayerObj, SL_IID_VOLUME, &mVolume);
    assert(SL_RESULT_SUCCESS == result);
    (void) result;

    result = (*mSeek)->SetLoop(mSeek, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
    assert(SL_RESULT_SUCCESS == result);
    (void) result;
}
```

然后，即可通过 SLPlayItf::SetPlayState 来控制音频播放器的行为（开始播放、暂停播放、结束播放）：

```
void AssetAudioPlayer::setPlayerState(SLuint32 state) {
    SLresult result;

    if (mPlayer) {
        result = (*mPlayer)->SetPlayState(mPlayer, state);
        assert(SL_RESULT_SUCCESS == result);
        (void) result;
    }
}
```

最后，释放资源：

```
void AssetAudioPlayer::release() {
    if (mPlayerObj) {
        (*mPlayerObj)->Destroy(mPlayerObj);
        mPlayerObj = nullptr;
        mPlayer = nullptr;
        mSeek = nullptr;
        mMuteSolo = nullptr;
        mVolume = nullptr;
    }

    if (outputMixObj) {
        (*outputMixObj)->Destroy(outputMixObj);
        outputMixObj = nullptr;
    }

    if (engineObj) {
        (*engineObj)->Destroy(engineObj);
        engineObj = nullptr;
        engine = nullptr;
    }
}
```

# 录制 PCM 音频

和音频播放一样，录制音频前首先也需要创建 Engine 接口，这里略过。

先看音频录制器的初始化：

```
bool AudioRecorder::initRecorder() {
    SLresult result;

    // 和音频播放器一样，需要配置音频数据的输入/输出
    SLDataLocator_IODevice locDevice = {SL_DATALOCATOR_IODEVICE, SL_IODEVICE_AUDIOINPUT,
                                        SL_DEFAULTDEVICEID_AUDIOINPUT, nullptr};
    SLDataSource audioSrc = {&locDevice, nullptr};

    // Android 平台提供的接口
    // num buffers: 2
    SLDataLocator_AndroidSimpleBufferQueue locBQ = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    // format: PCM
    // channel: 1
    // sampleRate: 48000
    // sampleFormat: 16bit
    // endian: little endian
    SLDataFormat_PCM formatPcm = {SL_DATAFORMAT_PCM, 1, SL_SAMPLINGRATE_48,
                                  SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
                                  SL_SPEAKER_FRONT_CENTER, SL_BYTEORDER_LITTLEENDIAN};
    SLDataSink audioSink = {&locBQ, &formatPcm};

    const SLInterfaceID id[1] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE};
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};
    result = (*mAudioEngine->engine)->CreateAudioRecorder(mAudioEngine->engine, &mRecorderObj,
                                                          &audioSrc, &audioSink, 1, id, req);
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }

    result = (*mRecorderObj)->Realize(mRecorderObj, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }

    result = (*mRecorderObj)->GetInterface(mRecorderObj, SL_IID_RECORD, &mRecorder); 
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }

    result = (*mRecorderObj)->GetInterface(mRecorderObj, SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
                                           &mBufferQueue);
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }

    result = (*mBufferQueue)->RegisterCallback(mBufferQueue, recorderCallback, this);
    return SL_RESULT_SUCCESS == result;
}
```

接着设置录制器状态：

```
bool AudioRecorder::start() {
    SLresult result;

    // 如果已经在录制了，就先停止
    result = (*mRecorder)->SetRecordState(mRecorder, SL_RECORDSTATE_STOPPED);
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }

    result = (*mBufferQueue)->Clear(mBufferQueue);
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }

    // 将一个空的 buffer 入队，用于给 recorder 录制音频时填充数据
    result = (*mBufferQueue)->Enqueue(mBufferQueue, mBuffers[mIndex], mBufSize * sizeof(short));
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }

    // 开始录制
    result = (*mRecorder)->SetRecordState(mRecorder, SL_RECORDSTATE_RECORDING);
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }

    mIsRecording = true;
    LOGI("start recording...");

    return true;
}
```

注意初始化音频录制器时的 recorderCallback，每当 recorder 录制完一帧音频时，就会回调这个函数，此时就可以使用录制好的 PCM 数据了：

```
void AudioRecorder::recorderCallback(SLAndroidSimpleBufferQueueItf bq, void *context) {
    assert(context != nullptr);

    AudioRecorder *recorder = (AudioRecorder *) context;
    assert(bq == recorder->mBufferQueue);

    fwrite(recorder->mBuffers[recorder->mIndex], 1, recorder->mBufSize,
           recorder->mFile);
    if (recorder->mIsRecording) {
        // 修改 index，切换另一个 buffer 用于下一次录制
        recorder->mIndex = 1 - recorder->mIndex;
        (*recorder->mBufferQueue)->Enqueue(recorder->mBufferQueue,
                                           recorder->mBuffers[recorder->mIndex],
                                           recorder->mBufSize);
    } else {
        // 停止录制
        (*recorder->mRecorder)->SetRecordState(recorder->mRecorder, SL_RECORDSTATE_STOPPED);
        fclose(recorder->mFile);
        recorder->mFile = nullptr;
    }
}
```

释放资源的步骤略。

# 总结

可以看到，音频的播放和录制，其基本流程大致是一样的： 

    1) 创建 Engine 

    2) 根据 SL_IID_xxx 来获取对应的 Object 

    3) 使用该 Object 获取相关 Interface 

    4) 使用该 Interface 执行相关操作
需要注意的几个结构体有：

```
typedef struct SLDataSource_ {   
    void *pLocator;   
    void *pFormat;
} SLDataSource;
```

```
typedef struct SLDataSink_ {
   void *pLocator;
   void *pFormat;
} SLDataSink;
```

pLocator 指的是 SLDataLocator_xxx，有下面几种类型：

```
/** Data locator macros  */
#define SL_DATALOCATOR_URI       ((SLuint32) 0x00000001)
#define SL_DATALOCATOR_ADDRESS    ((SLuint32) 0x00000002)
#define SL_DATALOCATOR_IODEVICE       ((SLuint32) 0x00000003)
#define SL_DATALOCATOR_OUTPUTMIX      ((SLuint32) 0x00000004)
#define SL_DATALOCATOR_RESERVED5      ((SLuint32) 0x00000005)
#define SL_DATALOCATOR_BUFFERQUEUE ((SLuint32) 0x00000006)
#define SL_DATALOCATOR_MIDIBUFFERQUEUE ((SLuint32) 0x00000007)
#define SL_DATALOCATOR_RESERVED8      ((SLuint32) 0x00000008)
```

pFormat 指的是 SLDataFormat_xxx，有下面几种类型：

```java
/** Data format defines */
#define SL_DATAFORMAT_MIME    ((SLuint32) 0x00000001)
#define SL_DATAFORMAT_PCM     ((SLuint32) 0x00000002)
#define SL_DATAFORMAT_RESERVED3    ((SLuint32) 0x00000003)
```

此外，一个 Interface 对应一个 SL_IID_xxx（IID：Interface ID），具体可以自行查看 OpenSLES.h。

源码已上传到 [Github](https://github.com/zouzhiheng/AVGraphics)

参考链接： 
[googlesamples/android-ndk](https://github.com/googlesamples/android-ndk)
[面向 Android 的 OpenSL ES](https://developer.android.com/ndk/guides/audio/opensl-for-android?hl=zh-cn)
[OpenSL ES Programming Notes](https://developer.android.com/ndk/guides/audio/opensl/opensl-prog-notes?hl=zh-cn)
[OpenSL ES那些事](http://www.10tiao.com/html/223/201612/2651232298/1.html)
[Android音频开发（7）：使用 OpenSL ES API（下）](http://blog.51cto.com/ticktick/1771239)








