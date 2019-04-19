# webrtc中APM（AudioProcessing module）的使用2 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年12月05日 10:45:58[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：762

这个其实就是从Audio_processing.h中拿出来的。
APM should be placed in the signal chain as close to the audio hardware abstraction layer
 (HAL) as possible.
APM accepts only 16-bit linear PCM audio data in frames of 10 ms.Multiple channels should
 be interleaved.
Audio Processing instantiation and configuration:    
AudioProcessing * apm = AudioProcessing :: Create (0);
apm-> level_estimator () -> Enable (true); // Enable retries estimation component
apm-> echo_cancellation () -> Enable (true); // Enable echo cancellation module
apm-> echo_cancellation () -> enable_metrics (true); //
apm-> echo_cancellation () -> enable_drift_compensation (true); // Enable clock compensation module (sound capture
 device clock frequency clock frequency and playback devices may be different)
apm-> gain_control () -> Enable (true); // Enable gain control module,
client must enable
Oh!
apm-> high_pass_filter () -> Enable (true); // high-pass filter components, DC offset and low frequency noise filtering,
client must be enabled
apm-> noise_suppression () -> Enable (true); // noise suppression components,
client must be enabled
apm-> voice_detection () -> Enable (true); // enable voice detection component, to detect whether there voices
apm-> voice_detection () -> set_likelihood (VoiceDetection :: kModerateLikelihood); // Set the voice detection threshold,
 the threshold bigger voice less likely to be ignored, some noise may be treated the same voice.
apm-> Initialize (); // Reserved internal state set by the user in all cases of re-initialization apm, to start processing
 a new audio stream. After creating the first stream does not necessarily need to call this method.
2.AudioProcessing workflow:
AudioProcessing is event-driven, event into the Initialize event, capturing audio event, rendering the audio event.
Initialize event:
apm-> set_sample_rate_hz (sample_rate_hz); // set the sample rate of local and remote audio stream
apm-> echo_cancellation () -> set_device_sample_rate_hz (); // set the sample rate audio equipment, we assume that
 the audio capture and playback device using the same sampling rate. (Must be called when the drift component is enabled)
apm-> set_num_channels (num_capture_input_channels, num_capture_output_channels);
// set the local and remote audio stream of the number of channels
Play event:
apm-> AnalyzeReverseStream (& far_frame)); // analysis of 10ms frame data far end of the audio stream, these data
 provide a reference for echo suppression. (Enable echo suppression when calling needs)
Capture events:
apm-> gain_control () -> set_stream_analog_level (capture_level);
apm-> set_stream_delay_ms (delay_ms + extra_delay_ms); // set the delay in milliseconds between local and remote audio
 streams of. This delay is the time difference and the distal end of the audio stream between the local audio streams, calculated as:
delay = (t_render – t_analyze) + (t_process – t_capture) ;
Among them
t_analyze end audio stream is time to AnalyzeReverseStream () method;
t_render is just similar to the distal end of the audio frame playback time;
t_capture local audio capture time frame;
t_process is the same time frame was given to local audio ProcessStream () method.
 apm-> echo_cancellation () -> set_stream_drift_samples (drift_samples); // Set the difference between the audio device
 to capture and playback sampling rate. (Must be called when the drift component is enabled)
int err = apm->
ProcessStream
(& near_frame); // processing audio streams, including all aspects of the deal. (Such as gain adjustment, echo cancellation, noise suppression, voice activity detection, high
 throughput rate without decoding Oh! Do for pcm data processing)
capture_level = apm-> gain_control () -> stream_analog_level (); // under emulation mode, you must call this method
 after ProcessStream, get the recommended analog value of new audio HAL.
stream_has_voice = apm-> voice_detection () -> stream_has_voice (); // detect whether there is a voice, you must call
 this method after ProcessStream
ns_speech_prob = apm-> noise_suppression () -> speech_probability (); // returns the internal voice priority calculated
 the probability of the current frame.
3.AudioProcessing release
AudioProcessing :: Destroy (apm);
apm = NULL;
另一个示例
AudioProcessing* apm = AudioProcessing::Create(0);
apm->set_sample_rate_hz(32000);
Super-wideband processing.
// 
Mono capture and stereo render.
apm->set_num_channels(1, 1);
apm->set_num_reverse_channels(2);
apm->high_pass_filter()->Enable(true);
apm->echo_cancellation()->enable_drift_compensation(false);
apm->echo_cancellation()->Enable(true);
apm->noise_reduction()->set_level(kHighSuppression);
apm->noise_reduction()->Enable(true);
apm->gain_control()->set_analog_level_limits(0, 255);
apm->gain_control()->set_mode(kAdaptiveAnalog);
apm->gain_control()->Enable(true);
apm->voice_detection()->Enable(true);
// Start a voice call...
// ... Render frame arrives bound for the audio HAL ...
apm->AnalyzeReverseStream(render_frame);
// ... Capture frame arrives from the audio HAL ...
// Call required set_stream_ functions.
apm->set_stream_delay_ms(delay_ms);
apm->gain_control()->set_stream_analog_level(analog_level);
apm->ProcessStream(capture_frame);
// Call required stream_ functions.
analog_level = apm->gain_control()->stream_analog_level();
has_voice = apm->stream_has_voice();
// Repeate render and capture processing for the duration of the call...
// Start a new call...
apm->Initialize();
// Close the application...
AudioProcessing::Destroy(apm);
apm = NULL;
参考：
[http://rg4.net/archives/738.html](http://rg4.net/archives/738.html)
[http://www.rosoo.net/a/201504/17270.html](http://www.rosoo.net/a/201504/17270.html)**webrtc 的回声抵消(aec、aecm)算法简介**
[http://blog.csdn.net/liulina603/article/details/21019915?locationNum=5&fps=1](http://blog.csdn.net/liulina603/article/details/21019915?locationNum=5&fps=1)DemuxAndMix() 这个函数，主要负责AudioProcess的所有过程，包括Aec,Aecm,AGC
[http://www.voidcn.com/blog/moming_2013/article/p-4959759.html](http://www.voidcn.com/blog/moming_2013/article/p-4959759.html)回音消除算法的delay值说明
[http://www.voidcn.com/blog/moming_2013/article/p-3435016.html](http://www.voidcn.com/blog/moming_2013/article/p-3435016.html)Android回音噪音处理Demo
[https://github.com/xiaomo/AudioProcess](https://github.com/xiaomo/AudioProcess)
