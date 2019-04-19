# webrtc中APM（AudioProcessing module）的使用 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年12月03日 18:34:20[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：2122
一，实例化和配置
AudioProcessing* apm = AudioProcessing::Create(0); //这里的0指的是channelID，只是一个标注那个通道的表示
apm->level_estimator()->Enable(true)；//启用  重试次数估计   组件
apm->echo_cancellation()->Enable(true);//启用回声消除组件
apm->echo_cancellation()->enable_metrics(true);//
apm->echo_cancellation()->enable_drift_compensation(true);//启用时钟补偿模块（声音捕捉设备的时钟频率和
                                                                                                                    播放设备的时钟频率可能不一样）
apm->gain_control()->Enable(true);//启用增益控制组件，client必须启用哦！
apm->high_pass_filter()->Enable(true);//高通过滤器组件，过滤DC偏移和低频噪音，client必须启用
apm->noise_suppression()->Enable(true);//噪声抑制组件，client必须启用
apm->voice_detection()->Enable(true);//启用语音检测组件，检测是否有说话声
apm->voice_detection()->set_likelihood( VoiceDetection::kModerateLikelihood);//设置语音检测的阀值，阀值越大，
                                                语音越不容易被忽略，同样一些噪音可能被当成语音。
apm->Initialize();//保留所有用户设置的情况下重新初始化apm的内部状态，用于开始处理一个新的音频流。第一个流
                        创建之后不一定需要调用此方法。
二，工作流程
AudioProcessing也是事件驱动的，事件分为初始化事件、捕捉音频事件、渲染音频事件。
初始化事件：
apm->set_sample_rate_hz(sample_rate_hz)；//设置本地和远程音频流的采样率。
apm->echo_cancellation()->set_device_sample_rate_hz()；//设置音频设备的采样率，
我们假定音频采集和播放设备采用同样的采样率。（drift组件启用时必须调用）。
 apm->set_num_channels(num_capture_input_channels, num_capture_output_channels);//设置
本地和远程音频流的通道数。

播放事件：
apm->AnalyzeReverseStream(&far_frame));//分析远端音频流的10ms的frame数据，这些数据为回声抑制提供参考。
（启用回声抑制的时候需要调用）
捕捉事件：
apm->gain_control()->set_stream_analog_level(capture_level);
apm->set_stream_delay_ms(delay_ms + extra_delay_ms);//设置本地和远端音频流之间的延迟，单位毫秒。
这个延迟是远端音频流和本地音频流之间的时差，计算方法为：
delay = (t_render - t_analyze) + (t_process - t_capture)；在调用前记录即可。
其中
t_analyze是远端音频流交给AnalyzeReverseStream（）方法的时间；
t_render是与刚才同样的远端音频frame的播放时间；
elesos注：render-aanalyze感觉就是远端音频处理的时间，从其开始处理到最终渲染的这一段时间。而
process-capture感觉就是近端，从采集到开始处理的这段时间。时差就是双方的和。
t_capture是本地音频frame捕捉的时间；
t_process是同样的本地音频frame被交给ProcessStream（）方法的时间。
 apm->echo_cancellation()->set_stream_drift_samples(drift_samples);//
设置音频设备捕捉和播放的采样率的差值。（drift组件启用时必须调用）
int err = apm->ProcessStream(&near_frame);//处理音频流，包括各个环节的处理。（如增益调节、回声消除、噪声
抑制、语音检测、高通过率等，没有解码哦！是针对pcm数据做处理的）
capture_level = apm->gain_control()->stream_analog_level();//模拟模式下，必须在ProcessStream之后调用此方法，
获取新的音频HAL的推荐模拟值。
stream_has_voice =apm->voice_detection()->stream_has_voice();//检测是否有语音，必须在ProcessStream之后调用
此方法，elesos注：可参考Audio_processing.h
ns_speech_prob = apm->noise_suppression()->speech_probability();//返回内部计算出的当前frame的人声优先概率。
三，释放和资源回收
AudioProcessing::Destroy(apm);
apm = NULL;
四，webrtc中自带例子测试
webrtc工程项目组有个audioproc项目，是测试APM模块的，不需要指定测试文件和参数，详情可以看里面的--hlep选项。
elesos注：[https://code.google.com/p/webrtc-audioproc/](https://code.google.com/p/webrtc-audioproc/)  移到
[https://github.com/DoubangoTelecom/webrtc-audioproc](https://github.com/DoubangoTelecom/webrtc-audioproc)  （WebRTC AudioProc
 (AEC, VAD, NS...)）
参考：
[http://blog.csdn.net/tanningzhong/article/details/42639179](http://blog.csdn.net/tanningzhong/article/details/42639179)
