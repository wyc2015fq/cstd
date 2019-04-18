# [Android] createTrack_l - TaigaComplex求职中 - 博客园







# [[Android] createTrack_l](https://www.cnblogs.com/TaigaCon/p/4772066.html)





在分析AudioTrack的时候，第一步会new AudioTrack，并调用他的set方法。在set方法的最后调用了createTrack_l创建音轨。我们现在来分析createTrack_l的流程。

在分析createTrack_l之前，我们先来了解Android音频流的从PCM到输出的路线。首先，我们的PCM音频数据一般会在用户端，而混音会在AudioFlinger端，因此需要把PCM数据传送给AudioFlinger，因此需要开辟出一块内存用于数据传送；数据到了AudioFlinger之后，可以给PCM数据调节音量，增加音效等（即混音），因此还需要一块内存用于音效处理，这块buffer在[getOutput](http://http//www.cnblogs.com/TaigaCon/p/4753570.html)内已经开辟；混音完成后即可把PCM数据输出给音频设备进行播放。

![Audio_outputPath](https://images2015.cnblogs.com/blog/421096/201508/421096-20150831004237531-2107734462.jpg)



    creatTrack_l的任务主要是创建音轨，即开辟出数据传送的内存。具体实现是创建出一块share buffer，这块buffer既可以被AudioTrack写入，又可以被AudioFlinger读取进行混音。

    createTrack总体可以分为三个步骤：
- 从AudioFlinger获取创建sharebuffer所需的参数，如latency，framecount，sampleRate；然后与传入的参数（framecount,sampleRate）做对比，目的是计算出正确的framecount
- 从AudioFlinger创建buffer，并创建对sharebuffer进行控制的对象AudioTrackServerProxy
- 创建可以对sharebuffer进行控制的对象AudioTrackClientProxy



# 1. 获取正确framecount

AudioTrack按照如下方式获取framecount
status_t AudioTrack::createTrack_l(

    status = AudioSystem::getLatency(output, streamType, &afLatency);

    status = AudioSystem::getFrameCount(output, streamType, &afFrameCount);

    status = AudioSystem::getSamplingRate(output, streamType, &afSampleRate);


    if (!audio_is_linear_pcm(format)) {
        if (sharedBuffer != 0) {
            // Same comment as below about ignoring frameCount parameter for set()
            frameCount = sharedBuffer->size();
        } else if (frameCount == 0) {
            frameCount = afFrameCount;
        }
        if (mNotificationFramesAct != frameCount) {
            mNotificationFramesAct = frameCount;
        }
    } else if (sharedBuffer != 0) {
        // user share buffer,we donot neet to allocate
        // Ensure that buffer alignment matches channel count
        // 8-bit data in shared memory is not currently supported by AudioFlinger
        size_t alignment = /* format == AUDIO_FORMAT_PCM_8_BIT ? 1 : */ 2;
        if (mChannelCount > 1) {
            alignment <<= 1;
        }
        if (((size_t)sharedBuffer->pointer() & (alignment - 1)) != 0) {
            return BAD_VALUE;
        }
        frameCount = sharedBuffer->size()/mChannelCount/sizeof(int16_t);

    } else if (!(flags & AUDIO_OUTPUT_FLAG_FAST)) {
        // non-fast 
        uint32_t minBufCount = 2;

        if (minBufCount <= nBuffering) {
            minBufCount = nBuffering;
        }

        // calculate buffer size by param from AudioFlinger
        size_t minFrameCount = (afFrameCount*sampleRate*minBufCount)/afSampleRate;

        if (frameCount == 0) {
            frameCount = minFrameCount;
        } else if (frameCount < minFrameCount) {
            frameCount = minFrameCount;
        }
    } else {
        // For fast tracks, the frame count calculations and checks are done by server
    }


先看一下AudioTrack计算framecount时的式子：
minFrameCount = (afFrameCount*sampleRate*minBufCount)/afSampleRate;
afFrameCount与afSampleRate都是从AudioFlinger得到的两个参数。
- afFrameCount代表MixerBuffer的大小，单位为Frame。Frame的定义为PCM音频数据的一个“采样 * 音轨个数”。
- afSampleRate代表MixerBuffer的默认采样率，即一秒内包含的Frame数目。

因此有如下公式：

$BufferSeconds = \frac{afFrameCount}{afSampleRate} = \frac{frameCount}{sampleRate}$

计算出buffer中包含多少秒音频数据。

下面是一个buffer实例，虽然sample rate一般都会是44100，但是为了方便画图，下面以5代替



![Audio_createTrack](https://images2015.cnblogs.com/blog/421096/201508/421096-20150831004239094-1616744338.jpg)



AudioFlinger获取AfFrameCount的过程如下：
//AudioFlinger.cpp
size_t AudioFlinger::frameCount(audio_io_handle_t output) const
{
    return thread->frameCount();
}

//Thread.h
virtual     size_t      frameCount() const { return mNormalFrameCount; }

//Thread.cpp
void AudioFlinger::PlaybackThread::readOutputParameters()
{
    mFrameCount = mOutput->stream->common.get_buffer_size(&mOutput->stream->common) / mFrameSize;
    mNormalFrameCount = multiplier * mFrameCount;
}

//Audio_hw.c
#define SHORT_PERIOD_SIZE 512

static size_t out_get_buffer_size_low_latency(const struct audio_stream *stream)
{
    struct tuna_stream_out *out = (struct tuna_stream_out *)stream;

    /* take resampling into account and return the closest majoring
    multiple of 16 frames, as audioflinger expects audio buffers to
    be a multiple of 16 frames. Note: we use the default rate here
    from pcm_config_tones.rate. */
    size_t size = (SHORT_PERIOD_SIZE * DEFAULT_OUT_SAMPLING_RATE) / pcm_config_tones.rate;
    size = ((size + 15) / 16) * 16;
    return size * audio_stream_frame_size((struct audio_stream *)stream);
}


获取与AfSampleRate的过程如下:
//AudioFlinger.cpp
uint32_t AudioFlinger::sampleRate(audio_io_handle_t output) const
{
    return thread->sampleRate();
}

//Thread.h
uint32_t    sampleRate() const { return mSampleRate; }

//Thread.cpp  where sample rate be initialized
void AudioFlinger::PlaybackThread::readOutputParameters()
{
    mSampleRate = mOutput->stream->common.get_sample_rate(&mOutput->stream->common);
}

//Audio_hw.c
#define DEFAULT_OUT_SAMPLING_RATE 44100 // 48000 is possible but interacts poorly with HDMI

static uint32_t out_get_sample_rate(const struct audio_stream *stream)
{
    return DEFAULT_OUT_SAMPLING_RATE;
}


而minFrameCount则包含了minBufferCount，即share buffer有多少个Mixer Buffer的大小
    // The client's AudioTrack buffer is divided into n parts for purpose of wakeup by server, where
    //  n = 1   fast track; nBuffering is ignored
    //  n = 2   normal track, no sample rate conversion
    //  n = 3   normal track, with sample rate conversion
    //          (pessimistic; some non-1:1 conversion ratios don't actually need triple-buffering)
    //  n > 3   very high latency or very small notification interval; nBuffering is ignored

- 如果在调用set方法的的时候，指定了flag为fast track，则表明希望Audio Buffer内的数据被尽快处理，因此Buffer会被创建得比较小，期采用单buffer
- 一般情况下，即输入PCM音频数据的采样率与输出音频数据的采样率一样的话，则不用进行采样率转换，采用双buffer
- 在需要采样率转换的情况，则采用三buffer
- 在碰到高延迟的情况，（如硬件不能及时输出PCM音频），则需要更大的buffer对数据进行缓存





# 2. AudioFlinger创建share buffer

AudioTrack是通过调用AudioFlinger的createTrack的方法来实现创建share buffer。createTrack的步骤如下：
- 获取输出线程PlaybackThread
- 调用获取到的PlaybackThread的createTrack_l函数来创建Track对象，在Track对象内部会创建share buffer
- 创建Track的binder对象TrackHandle，Track由于需要通过binder返回给AudioTrack，因此是个binder对象，该对象会包含share buffer的信息
sp<IAudioTrack> AudioFlinger::createTrack(...)
{
    PlaybackThread *thread = checkPlaybackThread_l(output);

    track = thread->createTrack_l(client, streamType, sampleRate, format,
            channelMask, frameCount, sharedBuffer, lSessionId, flags, tid, clientUid, &lStatus);

    trackHandle = new TrackHandle(track);
    return trackHandle;
}


### ①. 获取输出线程PlaybackThread

还记得[getOutput](http://http//www.cnblogs.com/TaigaCon/p/4753570.html)时所创建的PlaybackThread吗？PlaybackThread会在创建MixerThread时一同被创建。在getOutput内，我们把该thread放进了mPlaybackThreads进行维护。现在我们有需要把它取出来。
AudioFlinger::PlaybackThread *AudioFlinger::checkPlaybackThread_l(audio_io_handle_t output) const
{
    return mPlaybackThreads.valueFor(output).get();
}


### ②. 调用PlaybackThread的createTrack_l

在createTrack_l内调用了new Track来实现创建share buffer
sp<AudioFlinger::PlaybackThread::Track> AudioFlinger::PlaybackThread::createTrack_l(...)
{
            track = new Track(this, client, streamType, sampleRate, format,
                    channelMask, frameCount, sharedBuffer, sessionId, uid, *flags);
}


Track的父类是TrackBase，因此会先构建TrackBase对象
// TrackBase constructor must be called with AudioFlinger::mLock held
AudioFlinger::ThreadBase::TrackBase::TrackBase(...)
{
    // buffer header
    size_t size = sizeof(audio_track_cblk_t);

    // buffer content size
    size_t bufferSize = (sharedBuffer == 0 ? roundup(frameCount) : frameCount) * mFrameSize;
    if (sharedBuffer == 0) {
        size += bufferSize;
    }

    if (client != 0) {
        //allocate share buffer
        mCblkMemory = client->heap()->allocate(size);
        if (mCblkMemory != 0) {
            mCblk = static_cast<audio_track_cblk_t *>(mCblkMemory->pointer());
            // can't assume mCblk != NULL
        } else {
            ALOGE("not enough memory for AudioTrack size=%u", size);
            client->heap()->dump("AudioTrack");
            return;
        }
    } else {
        // this syntax avoids calling the audio_track_cblk_t constructor twice
        mCblk = (audio_track_cblk_t *) new uint8_t[size];
        // assume mCblk != NULL
    }

    // construct the shared structure in-place.
    if (mCblk != NULL) {
        // this is header above buffer content
        new(mCblk) audio_track_cblk_t();
        // clear all buffers
        mCblk->frameCount_ = frameCount;
        if (sharedBuffer == 0) {
            mBuffer = (char*)mCblk + sizeof(audio_track_cblk_t);
            memset(mBuffer, 0, bufferSize);
        } else {
            mBuffer = sharedBuffer->pointer();
        }

    }
}
其中，创建出来的buffer需要包含存放Audio PCM data的share buffer，还需要包含audio_track_cblk_t这个buffer头。调用heap->allocate这个函数来创建share buffer，buffer头部调用new(mCblk) audio_track_cblk_t;这种定位new的方式来创建。buffer的结构如下：



![Audio_buffer](https://images2015.cnblogs.com/blog/421096/201508/421096-20150831004243656-532923561.jpg)





new Track在构造函数体内，会创建AudioTrackServerProxy，这个对象会被用作AudioFlinger这边的buffer操作，由于share buffer是跨线程，甚至是跨进程的，而Proxy可以保证buffer访问的线程安全。
AudioFlinger::PlaybackThread::Track::Track(
{
    mAudioTrackServerProxy = new AudioTrackServerProxy(mCblk, mBuffer, frameCount,mFrameSize);
    mServerProxy = mAudioTrackServerProxy;
}




### ③. 创建TrackHandle

由于share buffer不止会在AudioFlinger这端被读取，还会在AudioTrack这端被写入，因此创建出来的Track需要被传送回AudioTrack。而在binder间传送对象只有binder对象，因此需要构建binder对象TrackHandle，返回给AudioTrack。
sp<IAudioTrack> AudioFlinger::createTrack(...)
{
    trackHandle = new TrackHandle(track);
}

// TrackHandle is a BnBinder object
class TrackHandle : public android::BnAudioTrack {
...
}



至此，createTrack_l在AudioFlinger这端的工作基本完成了。





# 3. 创建ClientProxy

有ServerProxy，相应地也会有ClientProxy，AudioTrackClientProxy就是在AudioTrack端可以对Track（share buffer）进行操作的类。

从AudioFlinger的createTrack返回TrackHandle后，就能通过TrackHandle的相关函数获得Track的信息，如buffer的起始地址等。用这些信息构造AudioTrackClientProxy.
status_t AudioTrack::createTrack_l(...)
{
    sp<IAudioTrack> track = audioFlinger->createTrack(...);
    sp<IMemory> iMem = track->getCblk();
    audio_track_cblk_t* cblk = static_cast<audio_track_cblk_t*>(iMem->pointer());

    mProxy = new AudioTrackClientProxy(cblk, buffers, frameCount, mFrameSizeAF);
}




## 4. 总结

最后，总结一下各个对象间的关系。

AudioFlinger:
-     首先会在AudioFlinger端创建Track，Track内包含buffer的创建及buffer指针的维护
-     Track内部有一个AudioTrackServerProxy的成员对象，用于进行buffer的相关操作
-     TrackHandle是Track对象的Binder实例，用于通过Binder返回给AudioTrack

AudioTrack:
-     IAudioTrack是TrackHandle在AudioTrack端相对应的类，该类用于提供buffer的相关信息给AudioTrackClientProxy
-     AudioTrackClientProxy获得buffer的信息后，即可以对buffer进行相关操作



![Audio_createTrack11](https://images2015.cnblogs.com/blog/421096/201508/421096-20150831004246406-1932658863.jpg)





createTrack_l的总体流程如下：



![Audio_createTrack12](https://images2015.cnblogs.com/blog/421096/201508/421096-20150831004252969-1168148184.jpg)












