# [Android] 混音线程MixerThread - TaigaComplex求职中 - 博客园







# [[Android] 混音线程MixerThread](https://www.cnblogs.com/TaigaCon/p/4827227.html)





MixerThread是Android音频输出的核心部分，所有Android的音频都需要经过MixerThread进行混音后再输出到音频设备。



MixerThread的继承关系如下：

MixerThread--->PlaybackThread--->ThreadBase--->Thread

在PlaybackThread中，重写了Thread的threadLoop,onFirstRef等方法，因此在调用MixerThread这些方法时，实际上就是调用了PlaybackThread的方法。







# 1. onFirstRef

在[getOutput](http://www.cnblogs.com/TaigaCon/p/4753570.html)的时候，我们创建了一个MixerThread对象，由于这个对象继承于Thread，因此在创建对象时，会调用它的onFirstRef函数。
void AudioFlinger::PlaybackThread::onFirstRef()
{
    run(mName, ANDROID_PRIORITY_URGENT_AUDIO);
}
在该方法内部，调用了run，即开始运行threadLoop。也就是说，其实在new MixerThread的时候就已经开始启动PlaybackThread::threadLoop。









# 2. threadLoop

在分析threadLoop之前，我们先来了解MixerThread中的几种Audio操作。

在Threads.cpp内有几个threadLoop_xxx方法，这些方法就分别代表不同的Audio操作：

|**操作**|**方法**|**功能**|
|----|----|----|
|standby|threadLoop_standby|待机|
|mix|threadLoop_mix|混音|
|write|threadLoop_write|音频输出|
|exit|threadLoop_exit|退出|
|drain|threadLoop_drain|只有offload用到，还不清楚作用|
|sleep|threadLoop_sleepTime|无音频需要处理，计算睡眠时间|




另外还有几个非常重要的变量：

|**变量**|**取值**|**含义**|
|----|----|----|
|tracksToRemove||需要被移除的Track，一旦所有的Track都被移除，则表明没有音频数据需要处理，那么线程会进入睡眠|
|sleepTime||睡眠时间|
|standbyTime||如果持续睡眠超出standbyTime，则会进入待机|
|mStandby||表明当前是否为待机状态|
|mActiveTracks||需要进行音频处理的Track，如果该Track已经播放完成或者被停止，则会被移入tracksToRemove|
|mMixerStatus|MIXER_IDLE|Mixer状态，no active tracks，表明不需要混音，而是进入睡眠|
|mMixerStatus|MIXER_TRACKS_ENABLED|Mixer状态，at least one active track, but no track has any data ready|
|mMixerStatus|MIXER_TRACKS_READY|Mixer状态，at least one active track, and at least one track has data，表明可以进行混音|








#### threadLoop循环

threadLoop内有一个循环，MixerThread是与output（输出设备）相关的（在openOutput的时候才会新建MixerThread），基本上都不会跑出循环之外。
bool AudioFlinger::PlaybackThread::threadLoop()
{
    while (!exitPending())  
    {
        ....
    }

}




#### MixerThread创建

在进入处理循环之前，首先会设置standbyTime、sleepTime。如果目前没有音频需要处理，进入睡眠，如果持续的睡眠时间超出了standbyTime，则会进入待机。不过由于standbyTime设置为当前时间，因此第一次肯定会执行待机动作。执行了待机操作后，MixerThread就会进入睡眠，等待被唤醒
bool AudioFlinger::PlaybackThread::threadLoop()
{
    //设置待机时间、睡眠时间
    standbyTime = systemTime();
    sleepTime = idleSleepTime;

    while (!exitPending())  
    {
        //创建MixerThread时，mActiveTracks肯定是空的，并且当前时间会超出standbyTime
        if ((!mActiveTracks.size() && systemTime() > standbyTime) || isSuspended()) {
               if (shouldStandby_l()) {                                                    //创建MixerThread时肯定会进入待机 
                       threadLoop_standby();                        
                       mStandby = true;                                                    }
               }
               if (!mActiveTracks.size() && mConfigEvents.isEmpty()) {
                   //然后MixerThread会在这里睡眠等待,知道AudioTrack:: start发送广播唤醒
                      mWaitWorkCV.wait(mLock);
                   standbyTime = systemTime() + standbyDelay;
                   sleepTime = idleSleepTime;
               }
    ...
    }
}






#### MixerThread处理音频

如上一篇所说，AudioTrack:: start被执行后，就会唤醒MixerThread线程，接下来就会对音频数据进行处理。处理流程如下图：

![MixerThread_mix](https://images2015.cnblogs.com/blog/421096/201509/421096-20150921203911319-1047213213.jpg)





正常的音频处理时，会在threadLoop循环内不断的进行混音与音频输出，其中分为三个步骤：
- **混音前的准备工作，prepareTracks_l **
- **混音，threadLoop_mix **
- **音频输出，threadLoop_write**






bool AudioFlinger::PlaybackThread::threadLoop()
{
    while (!exitPending())
    {
        mMixerStatus = prepareTracks_l(&tracksToRemove); 

        if(mMixerStatus == MIXER_TRACKS_READY)
            threadLoop_mix();
        }

        threadLoop_write();
    }
}






##### **① prepareTracks_l**

准备混音的过程中，主要的目的有三个：
- 设置混音所需要的参数，包括：音量，混音的源buffer，混音目的buffer，音频格式，是否重采样等。    
- 删除被加入tracksToRemove的track
- 返回当前状态mMixerStatus

由于在mActiveTracks中维护的track可能会有多个，因此需要对每个track都执行上述步骤，我们可以依据上述目的来对prepareTrack_l进行分析。
AudioFlinger::PlaybackThread::mixer_state AudioFlinger::MixerThread::prepareTracks_l(Vector< sp<Track> > *tracksToRemove) 
{
    //默认为空闲状态
    mixer_state mixerStatus = MIXER_IDLE;
    size_t count = mActiveTracks.size();

    //对于所有在mActiveTracks里面的Track，都需要进行设置
    for (size_t i=0 ; i<count ; i++) {
        const sp<Track> t = mActiveTracks[i].promote();
        Track* const track = t.get();

        //由于不是fastTrack,因此不会跑这里，而且fastTrack也不会在这里进行混音，我是没有发现有跑进过这个条件里面的
        if (track->isFastTrack()) {
            ...
        }

        //获取track的name，其实是个索引，AudioMixer会最多维护32个track，分别对饮int的32个bit，如果track的name还没定下来的话，会自行选择一个空位
        int name = track->name();

        //查看当前track是否stop，如果track被stop，那么这个track不需要设置AudioMixer参数，即frameReady = 0
        size_t framesReady;                                                     
        if (track->sharedBuffer() == 0) {                                           
            framesReady = track->framesReady();                                 
        } else if (track->isStopped()) {                                            
            framesReady = 0;                               
        } else {             
            framesReady = 1;                                  
        }

        //混音的情况下，frameReady = 1,那么会进入下面的条件，进行AudioMixer参数设置
        if ((framesReady >= minFrames) && track->isReady() && !track->isPaused() && !track->isTerminated())                                 {
            //音量参数
            ...

            //设置AudioMixer参数
            //源buffer
            mAudioMixer->setBufferProvider(name, track);
            //使能该track，即可以混音
            mAudioMixer->enable(name);
            //左音轨
            mAudioMixer->setParameter(name, param, AudioMixer::VOLUME0, (void *)vl);
            //右音轨
            mAudioMixer->setParameter(name, param, AudioMixer::VOLUME1, (void *)vr);    
            //aux                    
            mAudioMixer->setParameter(name, param, AudioMixer::AUXLEVEL, (void *)va); 
            //音频格式                      
            mAudioMixer->setParameter( 
                           name,                 
                           AudioMixer::TRACK,
                           AudioMixer::FORMAT, (void *)track->format());
            //音轨mask，哪个需要或者不需要混音
            mAudioMixer->setParameter(
                           name,
                           AudioMixer::TRACK,
                           AudioMixer::CHANNEL_MASK, (void *)track->channelMask());  
            //进行重采样
            mAudioMixer->setParameter(
                           name, 
                           AudioMixer::RESAMPLE,
                           AudioMixer::SAMPLE_RATE, 
                          (void *)reqSampleRate);
            //目的buffer
            mAudioMixer->setParameter(
                           name,
                           AudioMixer::TRACK,
                           AudioMixer::MAIN_BUFFER, (void *)track->mainBuffer());
            //aux
            mAudioMixer->setParameter(
                           name,
                           AudioMixer::TRACK,
                           AudioMixer::AUX_BUFFER, (void *)track->auxBuffer());
            
            //当前状态为ready，即可以混音
            mixerStatus = MIXER_TRACKS_READY;
        }else{
            //track stop时才会走这里
            ...
        }
  
    }
    //从mActiveTracks删除需要移除的track
    removeTracks_l(*tracksToRemove);

    //返回mMixerStatus, 正常混音准备时，这里返回的是MIXER_TRACK_READY
}


从上面的代码来看，有一个需要注意的地方：
            mAudioMixer->setParameter(
                           name, 
                           AudioMixer::RESAMPLE,
                           AudioMixer::SAMPLE_RATE, 
                          (void *)reqSampleRate);
即安卓的MixerThread会对所有的track进行重采样，那么在混音的时候会调用重采样的混音方法。







##### **②threadLoop_mix**

在prepareTrack_l返回了mMixerStatus = MIXER_TRACK_READY，那么就可以进入threadLoop_mix进行混音了。有了上面prepareTrack_l设置的参数，在threadLoop_mix所需要做的主要就是调用AudioMixer的process方法进行混音了。不过还需要对某些变量进行更新。


void AudioFlinger::MixerThread::threadLoop_mix()
{
    //首先需要获取timestamps，即输出时间戳，用于seek到源buffer的某个位置进行混音？
    if (mNormalSink != 0) {
        status = mNormalSink->getNextWriteTimestamp(&pts); 
    }else{
        status = mOutputSink->getNextWriteTimestamp(&pts);
    }

    //AudioMixer混音
    mAudioMixer->process(pts);

    //混音了多少音频数据
    mCurrentWriteLength = mixBufferSize;

    //等下不需要睡眠，直接输出音频
    sleepTime = 0;

    //待机时间更新
    standbyTime = systemTime() + standbyDelay;
}


在混音完成过后，混音目的buffer中的数据都会等待输出，mBytesRemaining就代表了又多少数据需要输出，混音完成后需要用mCurrentWriteLength对这个变量进行更新
bool AudioFlinger::PlaybackThread::threadLoop(){
    ...
    threadLoop_mix();
    mBytesRemaining = mCurrentWriteLength;
    ...
}








##### **③threadLoop_write**

threadLoop_write用于混音后的音频输出
ssize_t AudioFlinger::MixerThread::threadLoop_write(){
    //现在先不讨论fastMixer
    if (mFastMixer != NULL) {
        ...
    }
    return PlaybackThread::threadLoop_write();
}

ssize_t AudioFlinger::PlaybackThread::threadLoop_write()
{
    //调用write方法输出音频
    //如果用fastMixer的话其实会走mNormalSink分支的，现在不讨论
    if (mNormalSink != 0) {
        ssize_t framesWritten = mNormalSink->write(mMixBuffer + offset, count);
    }else{
        bytesWritten = mOutput->stream->write(mOutput->stream, mMixBuffer + offset, mBytesRemaining);
    }
    //最后返回输出了多少音频数据
    return bytesWritten;
}


每次音频输出后，都需要对混音目的buffer内剩余的数据量进行更新，并且记录一共输出了多少音频数据
bool AudioFlinger::PlaybackThread::threadLoop(){
    ...
    ssize_t ret = threadLoop_write();
    mBytesWritten += ret;
    mBytesRemaining -= ret;
    ...
}


这里也需要注意一点，如果在一次的输出后mBytesRemaining不为0，表明混音目的buffer内的数据并没有被完全输出，那么下一场循环就不能进行混音，而是直接继续输出音频。其实进入threadLoop_mix还有一个条件：
bool AudioFlinger::PlaybackThread::threadLoop()
{
    ...
    if(mBytesRemaining == 0){
        if(mMixerStatus == MIXER_TRACK_READY){
            threadLoop_mix();
        }
    }
    ...
}






#### MixerThread音频处理结束流程

音频处理结束分为两个阶段：
- sleep
- standby





##### sleep

在sleep阶段，还会在threadLoop内继续执行循环，但是不会再调用threadLoop_mix进行混音，而prepareTrack_l与threadLoop_write还会继续执行。

一般来说，在音频输出结束时，会执行AudioTrack:: stop，这会导致在prepareTrack_l返回状态MIXER_IDLE
 AudioFlinger::PlaybackThread::mixer_state AudioFlinger::MixerThread::prepareTracks_l(
        Vector< sp<Track> > *tracksToRemove)
{
    mixer_state mixerStatus = MIXER_IDLE;
    
    ...
    
    if (track->sharedBuffer() == 0) {
        framesReady = track->framesReady();
    }else if (track->isStopped()) {
        //在音频播放完成或者被停止的时候会走这个条件
        framesReady = 0;
    }else{
        framesReady = 1;
    }

    //并不会走设置混音参数的流程
    if ((framesReady >= minFrames) && track->isReady() &&
            !track->isPaused() && !track->isTerminated())
    {
        ...
    }else{
        ...

        //track 被停止就会把track加入tracksToRemove
        if ((track->sharedBuffer() != 0) || track->isTerminated() ||
                track->isStopped() || track->isPaused()) {

            if (mStandby || track->presentationComplete(framesWritten, audioHALFrames)) {
                 if (track->isStopped()) {
                     track->reset();
                 }
                 tracksToRemove->add(track); 
             }
        }
        //disable，通知AudioMixer不需要对这个track进行混音
        mAudioMixer->disable(name);
    }

    ...
    
    //从mActiveTracks删除该track
    removeTracks_l(*tracksToRemove);

    //返回开头的MIXER_IDLE
    return mixerStatus;
}




由于返回的mMixerStatus == MIXER_IDLE，因此并不会走threadLoop_mix进行混音，从而进入另一个分支threadLoop_sleepTime
bool AudioFlinger::PlaybackThread::threadLoop()
{
        if (mBytesRemaining == 0) {
             mCurrentWriteLength = 0;
             if (mMixerStatus == MIXER_TRACKS_READY) {
                 //在结束的时候不会跑这里
                 threadLoop_mix();
             } else if ((mMixerStatus != MIXER_DRAIN_TRACK)
                         && (mMixerStatus != MIXER_DRAIN_ALL)) {
                 //进入这个分支
                 threadLoop_sleepTime();
                 if (sleepTime == 0) {
                     mCurrentWriteLength = mixBufferSize;
                 }
             }
        }
}




在音频处理结束后的每个循环，threadLoop_sleepTime会取代threadLoop_mix，在threadLoop_sleepTime里面计算出来这次循环需要睡眠多久。threadLoop_sleepTime会交替计算出不同的sleepTime，如: 在音频处理结束后的第一个循环，会算出sleepTime = idleSleepTime;在第二个循环，会计算出sleepTime = 0；第三次又是sleepTime = idleSleepTime; 如此交替下去。（为什么需要这样？）
 void AudioFlinger::MixerThread::threadLoop_sleepTime()
 {
     // If no tracks are ready, sleep once for the duration of an output
     // buffer size, then write 0s to the output
     // 一开始进入这个函数前的前一个循环，是执行的threadLoop_mix，那时候的sleepTime == 0
     // 因此会进入下面这个条件
     if (sleepTime == 0) {
         if (mMixerStatus == MIXER_TRACKS_ENABLED) {
             sleepTime = activeSleepTime >> sleepTimeShift;
             if (sleepTime < kMinThreadSleepTimeUs) {
                 sleepTime = kMinThreadSleepTimeUs;
             }
             // reduce sleep time in case of consecutive application underruns to avoid
             // starving the audio HAL. As activeSleepTimeUs() is larger than a buffer
             // duration we would end up writing less data than needed by the audio HAL if
             // the condition persists.
             if (sleepTimeShift < kMaxThreadSleepTimeShift) {
                 sleepTimeShift++;
             }
         } else {
             // 由于我们现在的状态时MIXER_IDLE，因此会进入这个条件
             // idleSleepTime我们打印出来的是11500us
             sleepTime = idleSleepTime;
         }
     } else if (mBytesWritten != 0 || (mMixerStatus == MIXER_TRACKS_ENABLED)) {
         // 由于前一次循环赋值sleepTime = idleSpeelTime;第二个循环进来后会进入这个分支，重新设置sleepTime = 0;
         memset (mMixBuffer, 0, mixBufferSize);
         sleepTime = 0;
     }
     // TODO add standby time extension fct of effect tail
 }




获得sleepTime后，就可以通过sleepTime是否为0来执行write或者sleep了
void AudioFlinger::MixerThread::threadLoop_sleepTime()
{
    ...
            //可以看到只有sleepTime == 0的时候才会调用write，否则会睡眠
            if (sleepTime == 0) {
                if (mBytesRemaining) {
                    ssize_t ret = threadLoop_write();
                }
            } else {
                 usleep(sleepTime);
            }
    ...
}
也就是说在这个时间段还是会去输出音频数据的，虽然说这些数据都是0（没有声音）





sleep的流程可以参考下图

![MixerThread_sleep](https://images2015.cnblogs.com/blog/421096/201509/421096-20150921203913865-946625860.jpg)











##### standby

进入standby模式的时候，只需要执行两个步骤：
- 调用threadLoop_standby使音频设备进入待机模式
- 调用mWaitWorkCV.wait(mLock);使threadLoop进入睡眠，等待下一次播放音频数据的时候唤醒

那么如何才会进入standby模式呢？我们来回顾前面MixerThread创建的时候，已经进入过一次standby模式了。没错，在播放音频结束后还是从这里进入standby模式。

那么看一下进入standby模式的条件：
if ((!mActiveTracks.size() && systemTime() > standbyTime) ||
                       isSuspended())


正常情况会通过!mActiveTracks.size() && systemTime() > standbyTime这个条件进去。其中
- 在sleep模式的prepareTrack_l已经把mActiveTracks中需要删除的track去除，当mActiveTracks被完全清空，就代表没有track需要混音输出了，此时mActiveTracks.size() == 0
- systemTime取得当前时间，standbyTime最后一次被赋值是在threadLoop_mix的时候：standbyTime = systemTime() + standbyDelay; 这就表示在最后一次混音之后过了standbyDelay时间，即可以进入standby模式



![MixerThread_standby](https://images2015.cnblogs.com/blog/421096/201509/421096-20150921203916631-1595618758.jpg)












