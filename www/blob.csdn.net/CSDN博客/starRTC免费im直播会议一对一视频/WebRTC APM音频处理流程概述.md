# WebRTC APM音频处理流程概述 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年12月03日 13:56:07[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1159
本文主要介绍WebRTC的APM。
现在主要介绍一下audio_processing.h。
首先插入了几个类，这些都是audio_processing的核心模块。
**class** AudioFrame;  
**class** EchoCancellation;  
**class** EchoControlMobile;  
**class** GainControl;  
**class** HighPassFilter;  
**class**LevelEstimator;
  
**class** NoiseSuppression;  
**class** VoiceDetection;  
AudioFrame：主要记录了通道基本信息，数据，VAD标志时间戳，采样频率，信道数等。
EchoCancellation：回声消除模块（AEC），在使用外置扬声器的时候应该使用，有些使用耳麦通讯的情况也会存在回声（因为麦克风与扬声器有空间或者电的弱耦合），如果影响了通话也应该开启。
EchoControlMobile：回声抑制模块（AES），这个模块和回声消除模块功能相似，但是实现方法不一样。运算量远远小于回声消除模块。非常适合移动平台使用。但是对语音损伤大。
GainControl：增益控制模块（AGC），这个模块使用了语音的特征对系统硬件音量和输出的信号大小进行调节。硬件上可以控制输入音量。软件上只能调节原来信号的幅度，如果对原来就已经破音的信号，或者本来输入就比较小的信号就无能为力了。
HighPassFilter：高通滤波器，抑制不需要的低频信号。可以根据需要修改参数选择相应的截止频率。对于某些有工频干扰的设备需要使用高通滤波器。
LevelEstimator：估计信号的能量值。
NoiseSuppression：噪声抑制模块（NS/SE），该模块一般应用在有环境噪声的情况，或者是麦克风采集到的数据有明显噪声的情况。
VoiceDetection：语音激活检测模块（VAD），该模块用于检测语音是否出现。用于编解码以及后续相关处理。
APM分为两个流，一个近端流，一个远端流。近端（Near-end）流是指从麦克风进入的数据；远端（Far-end）流是指接收到的数据。现在分别介绍一下，这部分代码在audio_processing_impl.cc里。
far_end流代码：
int AudioProcessingImpl::AnalyzeReverseStreamLocked() {
  AudioBuffer* ra = render_audio_.get();  // For brevity.
  if (rev_proc_format_.rate() == kSampleRate32kHz) {
    for (int i = 0; i < rev_proc_format_.num_channels(); i++) {
      // Split into low and high band.
      WebRtcSpl_AnalysisQMF(ra->data(i),
                            ra->samples_per_channel(),
                            ra->low_pass_split_data(i),
                            ra->high_pass_split_data(i),
                            ra->filter_states(i)->analysis_filter_state1,
                            ra->filter_states(i)->analysis_filter_state2);
    }
  }
  RETURN_ON_ERR(echo_cancellation_->ProcessRenderAudio(ra));
  RETURN_ON_ERR(echo_control_mobile_->ProcessRenderAudio(ra));
  RETURN_ON_ERR(gain_control_->ProcessRenderAudio(ra));
  return kNoError;
}
上述代码可以看出far-end获得数据后主要有4个步骤的处理。
1、判断是否是32k信号，采取相应的分频策略；
2、AEC流程，记录AEC中的far-end及其相关运算；
3、AES流程，记录AES中的far-end及其相关运算；
4、AGC流程，计算far-end及其相关特征。
near-end流代码：
int AudioProcessingImpl::ProcessStreamLocked() {
#ifdef WEBRTC_AUDIOPROC_DEBUG_DUMP
  if (debug_file_->Open()) {
    audioproc::Stream* msg = event_msg_->mutable_stream();
    msg->set_delay(stream_delay_ms_);
    msg->set_drift(echo_cancellation_->stream_drift_samples());
    msg->set_level(gain_control_->stream_analog_level());
    msg->set_keypress(key_pressed_);
  }
#endif
  AudioBuffer* ca = capture_audio_.get();  // For brevity.
  bool data_processed = is_data_processed();
  if (analysis_needed(data_processed)) {
    for (int i = 0; i < fwd_proc_format_.num_channels(); i++) {
      // Split into a low and high band.
      WebRtcSpl_AnalysisQMF(ca->data(i),
                            ca->samples_per_channel(),
                            ca->low_pass_split_data(i),
                            ca->high_pass_split_data(i),
                            ca->filter_states(i)->analysis_filter_state1,
                            ca->filter_states(i)->analysis_filter_state2);
    }
  }
  RETURN_ON_ERR(high_pass_filter_->ProcessCaptureAudio(ca));
  RETURN_ON_ERR(gain_control_->AnalyzeCaptureAudio(ca));
  RETURN_ON_ERR(echo_cancellation_->ProcessCaptureAudio(ca));
  if (echo_control_mobile_->is_enabled() && noise_suppression_->is_enabled()) {
    ca->CopyLowPassToReference();
  }
  RETURN_ON_ERR(noise_suppression_->ProcessCaptureAudio(ca));
  RETURN_ON_ERR(echo_control_mobile_->ProcessCaptureAudio(ca));
  RETURN_ON_ERR(voice_detection_->ProcessCaptureAudio(ca));
  RETURN_ON_ERR(gain_control_->ProcessCaptureAudio(ca));
  if (synthesis_needed(data_processed)) {
    for (int i = 0; i < fwd_proc_format_.num_channels(); i++) {
      // Recombine low and high bands.
      WebRtcSpl_SynthesisQMF(ca->low_pass_split_data(i),
                             ca->high_pass_split_data(i),
                             ca->samples_per_split_channel(),
                             ca->data(i),
                             ca->filter_states(i)->synthesis_filter_state1,
                             ca->filter_states(i)->synthesis_filter_state2);
    }
  }
  // The level estimator operates on the recombined data.
  RETURN_ON_ERR(level_estimator_->ProcessStream(ca));
  was_stream_delay_set_ = false;
  return kNoError;
}
其中包括七个步骤：1、分频；2、高通滤波；3、硬件音量控制；4、AEC；5、NS；6、AES；7、VAD；8、AGC；9、综合。
可见near-end的处理全面，流程清晰。可以根据实际需要打开不同的模块，适应不同场景的需要，对于一般通讯系统来说具有正面的改善效果。但是在实际工作中也发现了一些流程上隐患。另外就是该结构的各个模块处理相对独立耦合低，本来应该是一个优良的特性，然而在复杂情况的信号处理难以到达目标效果。由于低耦合造成的运算量浪费更加是无法避免的。
[http://blog.csdn.net/ssdzdk/article/details/39577335](http://blog.csdn.net/ssdzdk/article/details/39577335)
