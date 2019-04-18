# [Android] AudioTrack::start - TaigaComplex求职中 - 博客园







# [[Android] AudioTrack::start](https://www.cnblogs.com/TaigaCon/p/4803952.html)





AudioTrack的start方法用于实现Android的音频输出，start究竟做了什么？回顾一下上一小节createTrack_l的最后部分，通过binder返回了一个Track的句柄，并以被保存了下来
status_t AudioTrack::createTrack_l(...)
{
    sp<IAudioTrack> track = audioFlinger->createTrack(...);
    mAudioTrack = track;
}
start主要就是调用这个track的start方法实现音频输出功能的
// -------------------------------------------------------------------------

status_t AudioTrack::start()
{
    AutoMutex lock(mLock);
    //如果该AudioTrack已经是start状态，直接返回
    if (mState == STATE_ACTIVE) {
        return INVALID_OPERATION;
    }

    mInUnderrun = true;
    //保存上一次的状态
    State previousState = mState;
    //设置当前状态
    if (previousState == STATE_PAUSED_STOPPING) {
        mState = STATE_STOPPING;
    } else {
        mState = STATE_ACTIVE;
    }
    //如果上一状态是停止状态，表明需要重新把position设置为0，从头播放
    if (previousState == STATE_STOPPED || previousState == STATE_FLUSHED) {
        // reset current position as seen by client to 0
        mProxy->setEpoch(mProxy->getEpoch() - mProxy->getPosition());
        // force refresh of remaining frames by processAudioBuffer() as last
        // write before stop could be partial.
        mRefreshRemaining = true;
    }
    //当前位置
    mNewPosition = mProxy->getPosition() + mUpdatePeriod;
    //获取share buffer的flag，原子操作
    int32_t flags = android_atomic_and(~CBLK_DISABLED, &mCblk->mFlags);

    //是否有回调线程，一般如果我们在apk端独立调用AudioTrack，是不会设置回调线程的，但是AudioPlayer这种系统播放器则会设置回调线程
    //这样做是为了设置优先级，否则Audio可能会由于得不到时间片，而卡顿
    //如果是AudioPlayer，会有自己定义的优先级，AudioTrack后面新创建的线程则会继承它的优先级
    //如果是Apk调用，优先级一般都是固定的，那么我们需要在这里设置一个ANDROID_PRIORITY_AUDIO的优先级来保证Audio的流畅输出
    sp<AudioTrackThread> t = mAudioTrackThread;
    if (t != 0) {
        if (previousState == STATE_STOPPING) {
            //中断
            mProxy->interrupt();
        } else {
            //恢复播放
            t->resume();
        }
    } else {
        //保存当前线程优先级，在后面停止的时候设置回来
        mPreviousPriority = getpriority(PRIO_PROCESS, 0);
        get_sched_policy(0, &mPreviousSchedulingGroup);
        //设置线程优先级为ANDROID_PRIORITY_AUDIO
        androidSetThreadPriority(0, ANDROID_PRIORITY_AUDIO);
    }

    status_t status = NO_ERROR;
    if (!(flags & CBLK_INVALID)) {
        //如果share buffer可用，则调用track的start方法
        status = mAudioTrack->start();
        if (status == DEAD_OBJECT) {
            flags |= CBLK_INVALID;
        }
    }
    if (flags & CBLK_INVALID) {
        status = restoreTrack_l("start");
    }

    if (status != NO_ERROR) {
        //start出错后的处理
        ALOGE("start() status %d", status);
        mState = previousState;
        if (t != 0) {
            if (previousState != STATE_STOPPING) {
                t->pause();
            }
        } else {
            setpriority(PRIO_PROCESS, 0, mPreviousPriority);
            set_sched_policy(0, mPreviousSchedulingGroup);
        }
    }

    return status;
}


由于mAudioTrack是binder的proxy对象，因此start会调用到BBinder对象的start方法，即
status_t AudioFlinger::TrackHandle::start() {
    return mTrack->start();
}


由于我们是在PlaybackThread下进行音频输出的，因此会进一步调用到PlaybackThread::Track:: start方法，其中最主要的是下面两个步骤：
status_t AudioFlinger::PlaybackThread::Track::start(
        PlaybackThread *playbackThread = (PlaybackThread *)thread.get();
        status = playbackThread->addTrack_l(this);
}

还记得我们在getOutput的时候创建了一个MixerThread吗，而且在createTrack_l的时候把这个Thread加入了mPlaybackThreads进行管理，现在我们要把它取出来，调用它的addTrack_l方法了
audio_io_handle_t AudioFlinger::openOutput(audio_module_handle_t module,...)
{     thread = new MixerThread(this, output, id, *pDevices);     return id;
}

AudioFlinger::PlaybackThread *AudioFlinger::checkPlaybackThread_l(audio_io_handle_t output) const
{     return mPlaybackThreads.valueFor(output).get();
}


在addTrack_l方法内，主要步骤有三个：
- 如果该track（share buffer）是新增track，则需要调用startOutput进行初始化
- 把该track加入mActiveTracks
- 发送广播，通知MixerThread开始工作
// addTrack_l() must be called with ThreadBase::mLock held
status_t AudioFlinger::PlaybackThread::addTrack_l(const sp<Track>& track)
{
    if (mActiveTracks.indexOf(track) < 0) {
        status = AudioSystem::startOutput(mId, track->streamType(), track->sessionId());
    }
    mActiveTracks.add(track);

    broadcast_l();
}



### 1. track初始化

在分析getOutput的时候，我们已经知道Audio接口的调用流程，即AudioSystem->AudioPolicyService->Audio_policy_hal->AudioPolicyManagerBase，现在我们来看一下AudioPolicyManagerBase:: startOutput做了什么
status_t AudioPolicyManagerBase::startOutput(audio_io_handle_t output,
                                             AudioSystem::stream_type stream,
                                             int session)
{
        checkAndSetVolume(stream,
                          mStreams[stream].getVolumeIndex(newDevice),
                          output,
                          newDevice);
}

checkAndSetVolume其实只是设置了stream volume.



### 2.  track加入mAudioTrack

mAudioTrack即当前MixerThread所包含的Track集合，在后面就是对这些Track集合进行混音





### 3. broadcast_l
void AudioFlinger::PlaybackThread::broadcast_l()
{
    // Thread could be blocked waiting for async
    // so signal it to handle state changes immediately
    // If threadLoop is currently unlocked a signal of mWaitWorkCV will
    // be lost so we also flag to prevent it blocking on mWaitWorkCV
    mSignalPending = true;
    mWaitWorkCV.broadcast();
}
我们已经有了MixerThread，由于MixerThread继承与PlaybackThread，因此跑的是PlaybackThread::threadLoop，在threadLoop内，如果mActiveTrack为空的话，表明没有音频数据等待输出，那么threadLoop会进入睡眠，等待唤醒，这里的broadcast就是做了这个唤醒的工作
bool AudioFlinger::PlaybackThread::threadLoop()
{
    if ((!mActiveTracks.size() && systemTime() > standbyTime) ||
                                   isSuspended())

        mWaitWorkCV.wait(mLock);

    }
    ...
}


下面是start的总体流程

![Audio_start](https://images2015.cnblogs.com/blog/421096/201509/421096-20150913005429137-753777536.jpg)












