# MediaCodec、OpenGL、OpenSL/AudioTrack 实现一款简单的视频播放器 - 我的学习笔记 - CSDN博客





2018年08月18日 12:19:11[zouzhiheng](https://me.csdn.net/u011330638)阅读数：93








# 概述

功能很简单，大致流程为： 

1) MediaCodec 解码视频文件得到 YUV、PCM 数据 

2) OpenGL 将 YUV 转为 RGB，并渲染到 Surface 上 

3) OpenSL/AudoTrack 获取 PCM 数据并播放
需要的前置知识有： 

1) YUV、PCM 等基础音视频知识，如 YUV 转 RGB 

2) MediaCodec 的使用 

3) OpenGL，包括 EGL、纹理等 

4) OpenSL 或 AudioTrack 的使用
# MediaCodec 解码

之前写过相关的博客 [MediaCodec 实现硬件解码](https://blog.csdn.net/u011330638/article/details/81053472)，大致流程和普通的解码类似，在编写视频播放器这个功能时，需要注意的地方有两个：

1) 监听解码流程

```
public interface OnDecodeListener {
        void onImageDecoded(byte[] data);

        void onSampleDecoded(byte[] data);

        void onDecodeEnded();
    }
```

也可以加一个 onDecodeError() 的接口，看需要扩展即可。

2) 播放和解码同步

因为视频数据量很大，不可能把解码后的 YUV 数据保存在一个队列里，再慢慢拿出来使用 OpenGL 渲染（很容易就 OOM 了），因此，必须控制解码的速率，最简单的控制方式是和播放同步，如下所示：

```
ByteBuffer outputBuffer = outputBuffers[outIndex];
                    outputBuffer.position(bufferInfo.offset);
                    outputBuffer.limit(bufferInfo.offset + bufferInfo.size);
                    byte[] data = new byte[bufferInfo.size];
                    outputBuffer.get(data);

                    if (mIsDecodeWithPts) {
                        if (startTime == 0) {
                            startTime = System.nanoTime();
                        } else {
                            passTime = (System.nanoTime() - startTime) / 1000;
                            if (passTime < bufferInfo.presentationTimeUs) {
                                TimeUnit.MICROSECONDS.sleep(bufferInfo.presentationTimeUs - passTime);
                            }
                        }
                    }

                    if (mediaType == HWCodec.MEDIA_TYPE_VIDEO && listener != null) {
                        listener.onImageDecoded(data);
                    } else if (listener != null) {
                        listener.onSampleDecoded(data);
                    }
```

# OpenGL 渲染 YUV 数据

和渲染纹理的流程类似，不同的地方在于需要转换 YUV 数据为 RGB，而 YUV 数据又有 YUV420P、YUV420SP 等多种格式，因此在转换 RGB 之前，需要统一 YUV 数据的格式，这里使用的是 YUV420P。

YUV 数据格式之间的转换可以自己写，比如 YUV420SP 转换为 YUV420P，只需要把最后的 U、V 数据分别逐个放入到一个数组里即可，但考虑到视频裁剪、旋转，以及之后可能用到的各种 YUV 数据处理功能，因此这里引入了一个 libyuv 的库，使用非常简单：

```
Yuv* convertToI420(AVModel *model) {
    if (!model || model->imageLen <= 0 || model->flag != MODEL_FLAG_VIDEO || model->width <= 0
        || model->height <= 0 || model->pixelFormat <= 0 || !model->image) {
        LOGE("convertToARGB failed: invalid argument");
        return nullptr;
    }
    Yuv *yuv = new Yuv(model->width, model->height);
    ConvertToI420(model->image, (size_t) model->imageLen, yuv->bufY, yuv->strideY,
                  yuv->bufU, yuv->strideU, yuv->bufV, yuv->strideV,
                  0, 0, model->width, model->height, model->width, model->height,
                  kRotate0, getFourCC(model->pixelFormat));
    return yuv;
}
```

AVModel、Yuv 是我自定义的两个类，分别用于保存音视频数据及相关信息、YUV 数据及相关信息，源码可见 [GitHub](https://github.com/zouzhiheng/AVGraphics)。

YUV 转 RGB 的相关系数在 [音视频开发基础概述 - PCM、YUV、H264、常用软件介绍](https://blog.csdn.net/u011330638/article/details/81107312) 介绍过，可以在 fragment shader 完成：

```
#version 300 es

precision highp float;

uniform sampler2D yTexture;
uniform sampler2D uTexture;
uniform sampler2D vTexture;

in vec2 vTexCoord;

layout(location=0) out vec4 fragColor;

void main() {
    highp float y = texture(yTexture, vTexCoord).r;
    highp float u = texture(uTexture, vTexCoord).r - 0.5;
    highp float v = texture(vTexture, vTexCoord).r - 0.5;

    highp float r = y + 1.402 * v;
    highp float g = y - 0.344 * u - 0.714 * v;
    highp float b = y + 1.772 * u;
    fragColor = vec4(r, g, b, 1.0);
}
```

OpenGL 关键代码如下：

```cpp
bool YuvRenderer::doInit() {
    std::string *vShader = readShaderFromAsset(mAssetManager, "yuv_renderer.vert");
    std::string *fShader = readShaderFromAsset(mAssetManager, "yuv_renderer.frag");

    mProgram = loadProgram(vShader->c_str(), fShader->c_str());

    mMatrixLoc = glGetUniformLocation(mProgram, "mMatrix");
    mSamplerY = glGetUniformLocation(mProgram, "yTexture");
    mSamplerU = glGetUniformLocation(mProgram, "uTexture");
    mSamplerV = glGetUniformLocation(mProgram, "vTexture");

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // 生成三个纹理，分别用于装载 Y、U、V 数据
    glGenTextures(3, mTextures);
    glBindTexture(GL_TEXTURE_2D, mTextures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mTexWidth, mTexHeight, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, mTextures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mTexWidth / 2, mTexHeight / 2, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, mTextures[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mTexWidth / 2, mTexHeight / 2, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 缓存顶点坐标、纹理坐标、索引数据到 VBO 中
    glGenBuffers(3, mVboIds);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TEX_COORDS), TEX_COORDS, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);

    // 缓存 VBO 到 VAO 中
    glGenVertexArrays(1, &mVaoId);
    glBindVertexArray(mVaoId);

    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glEnableVertexAttribArray(ATTRIB_POSITION);
    glVertexAttribPointer(ATTRIB_POSITION, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE,
                          sizeof(GLfloat) * VERTEX_POS_SIZE, 0);

    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
    glEnableVertexAttribArray(ATTRIB_TEX_COORD);
    glVertexAttribPointer(ATTRIB_TEX_COORD, TEX_COORD_SIZE, GL_FLOAT, GL_FALSE,
                          sizeof(GLfloat) * TEX_COORD_SIZE, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[2]);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    delete vShader;
    delete fShader;

    return true;
}

void YuvRenderer::doDraw() {
    glViewport(0, 0, mWidth, mHeight);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(mProgram);

    glUniformMatrix4fv(mMatrixLoc, 1, GL_FALSE, mMatrix);

    if (!mYuv) {
        LOGW("YuvRenderer doDraw failed: yuv data have not assigned");
        return;
    }
    // 分别载入 Y、U、V 数据到对应的纹理中
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mTexWidth, mTexHeight, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, mYuv->bufY);
    glUniform1i(mSamplerY, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mTextures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mTexWidth / 2, mTexHeight / 2, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, mYuv->bufU);
    glUniform1i(mSamplerU, 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mTextures[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mTexWidth / 2, mTexHeight / 2, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, mYuv->bufV);
    glUniform1i(mSamplerV, 2);

    // 使用 VAO 缓存的数据绘制图像
    glBindVertexArray(mVaoId);
    glDrawElements(GL_TRIANGLES, INDEX_NUMBER, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
```

# OpenSL 播放 PCM 数据

初始化播放器：

```
bool BQAudioPlayer::init() {
    SLresult result;

    SLDataLocator_AndroidSimpleBufferQueue locBufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    // channelMask: 位数和 channel 相等，0 代表 SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT
    SLDataFormat_PCM formatPcm = {SL_DATAFORMAT_PCM, (SLuint32) mChannels, mSampleRate,
                                  (SLuint32) mSampleFormat, (SLuint32) mSampleFormat,
                                  mChannels == 2 ? 0 : SL_SPEAKER_FRONT_CENTER,
                                  SL_BYTEORDER_LITTLEENDIAN};

    if (mSampleRate) {
        formatPcm.samplesPerSec = mSampleRate;
    }
    SLDataSource audioSrc = {&locBufq, &formatPcm};

    SLDataLocator_OutputMix locOutpuMix = {SL_DATALOCATOR_OUTPUTMIX, mAudioEngine->outputMixObj};
    SLDataSink audioSink = {&locOutpuMix, nullptr};

    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_VOLUME, SL_IID_EFFECTSEND};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    result = (*mAudioEngine->engine)->CreateAudioPlayer(mAudioEngine->engine, &mPlayerObj,
                                                        &audioSrc, &audioSink,
                                                        mSampleRate ? 2 : 3, ids, req);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("CreateAudioPlayer failed: %d", result);
        return false;
    }

    result = (*mPlayerObj)->Realize(mPlayerObj, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("mPlayerObj Realize failed: %d", result);
        return false;
    }

    result = (*mPlayerObj)->GetInterface(mPlayerObj, SL_IID_PLAY, &mPlayer);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("mPlayerObj GetInterface failed: %d", result);
        return false;
    }

    result = (*mPlayerObj)->GetInterface(mPlayerObj, SL_IID_BUFFERQUEUE, &mBufferQueue);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("mPlayerObj GetInterface failed: %d", result);
        return false;
    }

    result = (*mBufferQueue)->RegisterCallback(mBufferQueue, playerCallback, this);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("mPlayerObj RegisterCallback failed: %d", result);
        return false;
    }

    mEffectSend = nullptr;
    if (mSampleRate == 0) {
        result = (*mPlayerObj)->GetInterface(mPlayerObj, SL_IID_EFFECTSEND, &mEffectSend);
        if (result != SL_RESULT_SUCCESS) {
            LOGE("mPlayerObj GetInterface failed: %d", result);
            return false;
        }
    }

    result = (*mPlayerObj)->GetInterface(mPlayerObj, SL_IID_VOLUME, &mVolume);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("mPlayerObj GetInterface failed: %d", result);
        return false;
    }

    result = (*mPlayer)->SetPlayState(mPlayer, SL_PLAYSTATE_PLAYING);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("mPlayerObj SetPlayState failed: %d", result);
        return false;
    }

    return true;
}
```

之后只需要把 PCM 入队即可：

```
// 一帧音频播放完毕后就会回调这个函数
void playerCallback(SLAndroidSimpleBufferQueueItf bq, void *context) {
    BQAudioPlayer *player = (BQAudioPlayer *) context;
    assert(bq == player->mBufferQueue);
    pthread_mutex_unlock(&player->mMutex);
}

void BQAudioPlayer::enqueueSample(void *data, size_t length) {
    // 必须等待一帧音频播放完毕后才可以 Enqueue 第二帧音频
    pthread_mutex_lock(&mMutex);
    if (mBufSize < length) {
        mBufSize = length;
        if (mBuffers[0]) {
            delete[] mBuffers[0];
        }
        if (mBuffers[1]) {
            delete[] mBuffers[1];
        }
        mBuffers[0] = new uint8_t[mBufSize];
        mBuffers[1] = new uint8_t[mBufSize];
    }
    memcpy(mBuffers[mIndex], data, length);
    (*mBufferQueue)->Enqueue(mBufferQueue, mBuffers[mIndex], length);
    mIndex = 1 - mIndex;
}
```

结束播放：

```cpp
void BQAudioPlayer::release() {
    pthread_mutex_lock(&mMutex);
    if (mPlayerObj) {
        (*mPlayerObj)->Destroy(mPlayerObj);
        mPlayerObj = nullptr;
        mPlayer = nullptr;
        mBufferQueue = nullptr;
        mEffectSend = nullptr;
        mVolume = nullptr;
    }

    if (mAudioEngine) {
        delete mAudioEngine;
        mAudioEngine = nullptr;
    }

    if (mBuffers[0]) {
        delete[] mBuffers[0];
        mBuffers[0] = nullptr;
    }

    if (mBuffers[1]) {
        delete[] mBuffers[1];
        mBuffers[1] = nullptr;
    }

    pthread_mutex_unlock(&mMutex);
    pthread_mutex_destroy(&mMutex);
}
```

# AudioTrack 播放 PCM 数据

相对 OpenSL，AudioTrack 代码量少很多，设置 AudioTrack：

```
private void setupAudioTrack() {
        int channelConfig = mChannels == 1 ? AudioFormat.CHANNEL_OUT_MONO : AudioFormat.CHANNEL_OUT_STEREO;
        // 获取 sample format 的 API 要求高，这里默认使用 ENCODING_PCM_16BIT
        int bufferSize = AudioTrack.getMinBufferSize(mSampleRate, channelConfig, AudioFormat.ENCODING_PCM_16BIT);
        mAudioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, mSampleRate, channelConfig,
                AudioFormat.ENCODING_PCM_16BIT, bufferSize, AudioTrack.MODE_STREAM);
    }
```

播放 PCM 数据：

```java
@Override
        public void onSampleDecoded(byte[] data) {
            if (mIsPlaying) {
                mAudioTrack.write(data, 0, data.length);
                mAudioTrack.play();
            }
        }
```

结束播放：

```
private void releaseAudioTrack() {
        if (mAudioTrack != null) {
            mAudioTrack.stop();
            mAudioTrack.release();
            mAudioTrack = null;
        }
    }
```

以上，一款简单的视频播放器就完成了，如果觉得哪些代码写得不够好，请留言交流一下，谢谢。

源码已上传到 [GitHub](https://github.com/zouzhiheng/AVGraphics)。





