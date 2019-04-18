# [Android] AudioTrack实例 - TaigaComplex求职中 - 博客园







# [[Android] AudioTrack实例](https://www.cnblogs.com/TaigaCon/p/4752384.html)





AudioTrack在Android系统中是用于PCM数据的混音、播放，并不涉及到音频的解码。因此MP3这类经过编码的音频格式文件不能直接通过AudioTrack正确地播放，AudioTrack只能播放PCM格式的音频数据，如wav格式的音频。



AudioTrack播放音频的实例如下：
    AudioTrack audio = new AudioTrack(
                           AudioManager.STREAM_MUSIC, // stream mode
                           32000, // sample rate
                           AudioFormat.CHANNEL_OUT_STEREO, // single or stereo
                           AudioFormat.ENCODING_PCM_16BIT, // bit format
                           AudioTrack.MODE_STREAM
                       );
    audio.start();
    byte[] buffer = new buffer[4096];
    int count;
    while(true)
    {
        audio.write(buffer, 0, 4096);
    }




共有三个步骤：
- 构建AudioTrack对象，并且把PCM的参数传到对象里面
- 调用start
- 调用write







# 1. 构建AudioTrack对象

在AudioTrack构造方法内部仅调用了一个set函数
AudioTrack::AudioTrack(
        audio_stream_type_t streamType,
        uint32_t sampleRate,
        audio_format_t format,
        audio_channel_mask_t channelMask,
        const sp<IMemory>& sharedBuffer,
        audio_output_flags_t flags,
        callback_t cbf,
        void* user,
        int notificationFrames,
        int sessionId,
        transfer_type transferType,
        const audio_offload_info_t *offloadInfo,
        int uid)
    : mStatus(NO_INIT),
      mIsTimed(false),
      mPreviousPriority(ANDROID_PRIORITY_NORMAL),
      mPreviousSchedulingGroup(SP_DEFAULT)
{
    mStatus = set(streamType, sampleRate, format, channelMask,
            0 /*frameCount*/, flags, cbf, user, notificationFrames,
            sharedBuffer, false /*threadCanCallJava*/, sessionId, transferType, offloadInfo, uid);
}




set方法内部主要做了什么？
status_t AudioTrack::set(
        audio_stream_type_t streamType,
        uint32_t sampleRate,
        audio_format_t format,
        audio_channel_mask_t channelMask,
        int frameCountInt,
        audio_output_flags_t flags,
        callback_t cbf,
        void* user,
        int notificationFrames,
        const sp<IMemory>& sharedBuffer,
        bool threadCanCallJava,
        int sessionId,
        transfer_type transferType,
        const audio_offload_info_t *offloadInfo,
        int uid)
{

    audio_io_handle_t output = AudioSystem::getOutput(
                                    streamType,
                                    sampleRate, format, channelMask,
                                    flags,
                                    offloadInfo);


    if (cbf != NULL) {
        mAudioTrackThread = new AudioTrackThread(*this, threadCanCallJava);
        mAudioTrackThread->run("AudioTrack", ANDROID_PRIORITY_AUDIO, 0 /*stack*/);
    }


    // create the IAudioTrack
    status_t status = createTrack_l(streamType,
                                  sampleRate,
                                  format,
                                  frameCount,
                                  flags,
                                  sharedBuffer,
                                  output,
                                  0 /*epoch*/);



除了对参数的预处理以及保存之外，中间最主要的功能有三个：
- getOutput，获取输出设备并打开该设备
- 创建AudioTrackThread，并执行该线程
- 创建音轨，实际上是创建用户与Android系统的共享内存，用于传输音频数据





由于getOutput与createTrack_l的篇幅较大，我会在后面的章节在分析，这里分析一下AudioTrackThread。



AudioTrackThread主要用于反馈处理，会根据不同的buffer状态进行不同的操作，其中buffer状态包括下面几种：
- buffer的position有增长，说明buffer正在被填充；
- remainFrames减少说明buffer正在被消耗，即buffer内的Audio数据已被混音；
- loop_cycle说明buffer需要循环播放；
- loop_final说明buffer已经播放完毕；
- etc.



对于不同的状态，会调用到回调函数mCbf，该函数在set的时候被传入。回调函数mCbf的作用还是很大的，比如说能通知应用程序当前的播放状态，也能根据当前的状态继续进行下一步的操作。像AwesomePlayer中的音频处理模块AudioPlayer就能通过mCbf的回调自动填充buffer。
bool AudioTrack::AudioTrackThread::threadLoop()
{

    nsecs_t ns = mReceiver.processAudioBuffer(this);

}



// -------------------------------------------------------------------------

nsecs_t AudioTrack::processAudioBuffer(const sp<AudioTrackThread>& thread)
{

        mCbf(...);

}



# 2. 调用start

start方法是触发混音线程对当前音轨进行混音并输出，篇幅较大，会在后面章节分析





# 3. 调用write

write方法需要获得刚刚所创建的音轨，并且把PCM数据往音轨写入。由于音轨的大小有限，write也很有可能一次性不能写入全部数据，因此需要循环调用write方法


// -------------------------------------------------------------------------

ssize_t AudioTrack::write(const void* buffer, size_t userSize)
{

    while (userSize >= mFrameSize) {
        audioBuffer.frameCount = userSize / mFrameSize;

        status_t err = obtainBuffer(&audioBuffer, &ClientProxy::kForever);

        memcpy(audioBuffer.i8, buffer, toWrite);
    }
}





需要注意的就是AudioTrack除了初始化之外，write只是往音轨内写入PCM数据，这是Audio数据流动的第一步。









![AudioTrack](https://images0.cnblogs.com/blog/421096/201508/231415441289688.jpg)












