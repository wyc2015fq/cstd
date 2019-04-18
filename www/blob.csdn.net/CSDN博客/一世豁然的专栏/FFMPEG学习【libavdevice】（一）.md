# FFMPEG学习【libavdevice】（一） - 一世豁然的专栏 - CSDN博客





2017年06月22日 10:36:38[一世豁然](https://me.csdn.net/Explorer_day)阅读数：543








指定设备muxing/demuxing库。




一、头文件

[avdevice.h](http://ffmpeg.org/doxygen/trunk/avdevice_8h.html)





[version.h](http://ffmpeg.org/doxygen/trunk/libavdevice_2version_8h.html)








二、数据结构


|struct|[AVDeviceRect](http://ffmpeg.org/doxygen/trunk/structAVDeviceRect.html){int x;//左上角的x坐标int y;// 左上角的y坐标int width;// 宽度int height;//高度}|
|----|----|




以下API允许用户探测设备功能（支持的编解码器，像素格式，采样格式，分辨率，通道数等）。


它是建立在顶级操作AVOption API上。 查询功能可以设置适合设备的视频或音频参数转换器。


可以查询的功能列表：


1、对于音频和视频设备都有效的功能：

 1）、编解码器：支持的音频/视频编解码器。类型：AV_OPT_TYPE_INT（AVCodecID值）

2、适用于音频设备的功能：

 1）、sample_format：支持的采样格式。 类型：AV_OPT_TYPE_INT（AVSampleFormat值）

 2）、sample_rate：支持的采样率。 键入：AV_OPT_TYPE_INT

 3）、频道：支持频道数量。 类型：AV_OPT_TYPE_INT

 4）、channel_layout：支持的频道布局。类型：AV_OPT_TYPE_INT64

3、对视频设备有效的功能：

 1）、pixel_format：支持的像素格式。 类型：AV_OPT_TYPE_INT（AVPixelFormat值）

 2）、window_size：支持的窗口大小（描述提供给用户的窗口大小的大小）。 类型：AV_OPT_TYPE_IMAGE_SIZE

 3）、frame_size：支持的帧大小（描述提供的视频帧的大小）。 类型：AV_OPT_TYPE_IMAGE_SIZE

 4）、fps：支持的fps值类型：AV_OPT_TYPE_RATIONAL

功能的值可以由用户使用av_opt_set（）函数和AVDeviceCapabilitiesQuery对象设置。 以下查询将结果限制为匹配已设置的功能的值。 例如，设置编解码器可能会影响下一次查询期间返回的格式或fps值。 设置无效值可能会将结果限制为零。


基于opengl输出设备的使用示例：




```cpp
AVFormatContext *oc = NULL;
 AVDeviceCapabilitiesQuery *caps = NULL;
 AVOptionRanges *ranges;
 int ret;
 if ((ret = avformat_alloc_output_context2(&oc, NULL, "opengl", NULL)) < 0)
     goto fail;
 if (avdevice_capabilities_create(&caps, oc, NULL) < 0)
     goto fail;
 //query codecs
 if (av_opt_query_ranges(&ranges, caps, "codec", AV_OPT_MULTI_COMPONENT_RANGE)) < 0)
     goto fail;
 //pick codec here and set it
 av_opt_set(caps, "codec", AV_CODEC_ID_RAWVIDEO, 0);
 //query format
 if (av_opt_query_ranges(&ranges, caps, "pixel_format", AV_OPT_MULTI_COMPONENT_RANGE)) < 0)
     goto fail;
 //pick format here and set it
 av_opt_set(caps, "pixel_format", AV_PIX_FMT_YUV420P, 0);
 //query and set more capabilities
fail:
 //clean up code
 avdevice_capabilities_free(&query, oc);
 avformat_free_context(oc);
```
结构描述设备功能。



它被设备与AV设备功能AVOption表结合使用，以实现基于AVOption API的能力探测API。 不应该直接使用




|struct|[AVDeviceCapabilitiesQuery](http://ffmpeg.org/doxygen/trunk/structAVDeviceCapabilitiesQuery.html){const AVClass * av_classAVFormatContext * device_contextenum AVCodecID codecenum AVSampleFormat sample_formatenum AVPixelFormat pixel_formatint sample_rateint channelsint64_t channel_layoutint window_widthint window_heightint frame_widthint frame_heightAVRational fps}|
|----|----|


结构描述了设备的基本参数。



|struct|[AVDeviceInfo](http://ffmpeg.org/doxygen/trunk/structAVDeviceInfo.html){char * device_name；//设备名称，格式取决于设备char * device_description；//  人性化的名字}|
|----|----|


设备列表


|struct|[AVDeviceInfoList](http://ffmpeg.org/doxygen/trunk/structAVDeviceInfoList.html){AVDeviceInfo ** devices；//  自动检测设备列表int nb_devices；//  自动检测的设备数量int default_device；// 默认设备的索引，如果没有默认设备则为-1}|
|----|----|





三、枚举

由avdevice_app_to_dev_control_message（）使用的消息类型。



|enum|[AVAppToDevMessageType](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ga16d549a25f11693a06a211f00864fed8) { [AV_APP_TO_DEV_NONE](http://ffmpeg.org/doxygen/trunk/group__lavd.html#gga16d549a25f11693a06a211f00864fed8a635d820c2098c4f58feff50b7d6c2811) = MKBETAG('N','O','N','E'), [AV_APP_TO_DEV_WINDOW_SIZE](http://ffmpeg.org/doxygen/trunk/group__lavd.html#gga16d549a25f11693a06a211f00864fed8ace5fb49b2638f933e758fed3dcb07249) = MKBETAG('G','E','O','M'), [AV_APP_TO_DEV_WINDOW_REPAINT](http://ffmpeg.org/doxygen/trunk/group__lavd.html#gga16d549a25f11693a06a211f00864fed8a9f4006d75c6e368d9d0224d973d57710) = MKBETAG('R','E','P','A'), [AV_APP_TO_DEV_PAUSE](http://ffmpeg.org/doxygen/trunk/group__lavd.html#gga16d549a25f11693a06a211f00864fed8a89a46eae01f8ff45ec1f200e6738d804) = MKBETAG('P', 'A', 'U', ' '), [AV_APP_TO_DEV_PLAY](http://ffmpeg.org/doxygen/trunk/group__lavd.html#gga16d549a25f11693a06a211f00864fed8a344983cb9068e56d3f847162081a9747) = MKBETAG('P', 'L', 'A', 'Y'), [AV_APP_TO_DEV_TOGGLE_PAUSE](http://ffmpeg.org/doxygen/trunk/group__lavd.html#gga16d549a25f11693a06a211f00864fed8a754ae9e6688b1de6f92140415e56b40c) = MKBETAG('P', 'A', 'U', 'T'), [AV_APP_TO_DEV_SET_VOLUME](http://ffmpeg.org/doxygen/trunk/group__lavd.html#gga16d549a25f11693a06a211f00864fed8abac3d409c36e4774717f9a8ecf24ada9) = MKBETAG('S', 'V', 'O', 'L'), [AV_APP_TO_DEV_MUTE](http://ffmpeg.org/doxygen/trunk/group__lavd.html#gga16d549a25f11693a06a211f00864fed8a93114a33bcd20f06d39fb9a64febf751) = MKBETAG(' ', 'M', 'U', 'T'), [AV_APP_TO_DEV_UNMUTE](http://ffmpeg.org/doxygen/trunk/group__lavd.html#gga16d549a25f11693a06a211f00864fed8ad8c2e96b8cf1ebd5e85c5fd171eecc3f) = MKBETAG('U', 'M', 'U', 'T'), [AV_APP_TO_DEV_TOGGLE_MUTE](http://ffmpeg.org/doxygen/trunk/group__lavd.html#gga16d549a25f11693a06a211f00864fed8ad83bcf41ae7e0d09e46bdc2b240afb4b) = MKBETAG('T', 'M', 'U', 'T'), [AV_APP_TO_DEV_GET_VOLUME](http://ffmpeg.org/doxygen/trunk/group__lavd.html#gga16d549a25f11693a06a211f00864fed8a5caaa1fda2910d345270d8ac1710c138) = MKBETAG('G', 'V', 'O', 'L'),[AV_APP_TO_DEV_GET_MUTE](http://ffmpeg.org/doxygen/trunk/group__lavd.html#gga16d549a25f11693a06a211f00864fed8a8219697bb3af980fa778ffb1e11606b3) = MKBETAG('G', 'M', 'U', 'T') }|
|----|----|



|*AV_APP_TO_DEV_NONE*|虚假信息。|
|----|----|
|*AV_APP_TO_DEV_WINDOW_SIZE*|窗口大小更改消息。每次应用程序更改窗口设备呈现的大小时，消息都会发送到设备。消息也应在窗口创建后立即发送。数据：AVDeviceRect：新窗口大小。|
|*AV_APP_TO_DEV_WINDOW_REPAINT*|重绘请求消息。当窗口必须重画时，消息被发送到设备。数据：AVDeviceRect：需要重新绘制的区域。 NULL：整个区域需要重新绘制。|
|*AV_APP_TO_DEV_PAUSE*|请求暂停/播放。应用程序请求暂停/取消暂停播放。 主要用于具有内部缓冲区的设备。 默认情况下，设备不会暂停。数据: NULL|
|*AV_APP_TO_DEV_PLAY*||
|*AV_APP_TO_DEV_TOGGLE_PAUSE*||
|*AV_APP_TO_DEV_SET_VOLUME*|音量控制信息。设置音量级别。 如果每个流或系统范围内更改音量，则可能与设备有关。 预计每个流量的变化可能。数据：double：新的音量范围0.0 - 1.0。|
|*AV_APP_TO_DEV_MUTE*|静音控制消息。改变静音状态。 如果每个流或系统范围内的静音状态发生变化，则可能与设备有关。 如果可能，预计每流静音状态更改。数据: NULL.|
|*AV_APP_TO_DEV_UNMUTE*||
|*AV_APP_TO_DEV_TOGGLE_MUTE*||
|*AV_APP_TO_DEV_GET_VOLUME*|获取音量/静音消息。强制设备分别发送AV_DEV_TO_APP_VOLUME_LEVEL_CHANGED或AV_DEV_TO_APP_MUTE_STATE_CHANGED命令。数据: NULL.|
|*AV_APP_TO_DEV_GET_MUTE*| |


由avdevice_dev_to_app_control_message（）使用的消息类型。



|enum|[AVDevToAppMessageType](http://ffmpeg.org/doxygen/trunk/group__lavd.html#gac379d3691ce091da6387a461a158a690) { [AV_DEV_TO_APP_NONE](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ggac379d3691ce091da6387a461a158a690a64ccc494050138379bd52b62159ea4ce) = MKBETAG('N','O','N','E'), [AV_DEV_TO_APP_CREATE_WINDOW_BUFFER](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ggac379d3691ce091da6387a461a158a690a0c4bc0ff29547bc0dc4bd5d0aba95020) = MKBETAG('B','C','R','E'), [AV_DEV_TO_APP_PREPARE_WINDOW_BUFFER](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ggac379d3691ce091da6387a461a158a690a7d665015d93c6f6f0bf6bdb3d86c619e) = MKBETAG('B','P','R','E'),[AV_DEV_TO_APP_DISPLAY_WINDOW_BUFFER](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ggac379d3691ce091da6387a461a158a690a1108397b3605ed9aff591de55a3c27bd) = MKBETAG('B','D','I','S'), [AV_DEV_TO_APP_DESTROY_WINDOW_BUFFER](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ggac379d3691ce091da6387a461a158a690abd200e116b23c39311db9661d9221b04) = MKBETAG('B','D','E','S'), [AV_DEV_TO_APP_BUFFER_OVERFLOW](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ggac379d3691ce091da6387a461a158a690a037a21369f35dd608f54fb34022de229) = MKBETAG('B','O','F','L'), [AV_DEV_TO_APP_BUFFER_UNDERFLOW](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ggac379d3691ce091da6387a461a158a690ac3652337ce29a2bb83a57ae9b76196c1) = MKBETAG('B','U','F','L'), [AV_DEV_TO_APP_BUFFER_READABLE](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ggac379d3691ce091da6387a461a158a690a1fe6ba0b4093fb06244e342105d9c1a2) = MKBETAG('B','R','D',' '), [AV_DEV_TO_APP_BUFFER_WRITABLE](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ggac379d3691ce091da6387a461a158a690a0cf06e453ce4b8618573bf9fbeca07f4) = MKBETAG('B','W','R',' '), [AV_DEV_TO_APP_MUTE_STATE_CHANGED](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ggac379d3691ce091da6387a461a158a690a3e4c4485f83fa9818ce1e9a559a7dc02) = MKBETAG('C','M','U','T'), [AV_DEV_TO_APP_VOLUME_LEVEL_CHANGED](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ggac379d3691ce091da6387a461a158a690a19abbd0fc9488541fd4df18660120e0c) = MKBETAG('C','V','O','L') }|
|----|----|




创建窗口缓冲区消息。

|*AV_DEV_TO_APP_NONE*|Dummy message.|
|----|----|
|*AV_DEV_TO_APP_CREATE_WINDOW_BUFFER*|设备请求创建窗口缓冲区。 具体的含义是依赖于设备和应用程序。 在渲染第一帧之前发送消息，并且应在此完成所有单次初始化。 允许应用程序忽略首选的窗口缓冲区大小。- 注意：: 应用程序有义务通过AV_APP_TO_DEV_WINDOW_SIZE消息通知窗口缓冲区大小。数据：AVDeviceRect：窗口缓冲区的首选大小。 NULL：没有窗口缓冲区的首选大小。|
|*AV_DEV_TO_APP_PREPARE_WINDOW_BUFFER*|准备窗口缓冲区消息。设备请求准备一个用于渲染的窗口缓冲区。 具体的含义是依赖于设备和应用程序。 消息在渲染每帧之前发送。数据: NULL.|
|*AV_DEV_TO_APP_DISPLAY_WINDOW_BUFFER*|显示窗口缓冲区消息。设备请求显示窗口缓冲区。 新帧准备好显示时发送消息。 通常，缓冲区需要在该消息的处理程序中进行交换。数据: NULL.|
|*AV_DEV_TO_APP_DESTROY_WINDOW_BUFFER*|销毁窗口缓冲区消息。设备要求销毁窗口缓冲区。 当设备即将被销毁时发送消息，并且不再需要窗口缓冲区。数据: NULL.|
|*AV_DEV_TO_APP_BUFFER_OVERFLOW*|缓冲区填满状态消息。设备信号缓冲区溢出/下溢。数据: NULL.|
|*AV_DEV_TO_APP_BUFFER_UNDERFLOW*||
|*AV_DEV_TO_APP_BUFFER_READABLE*|设备通知缓冲区是可读/可写的。 在可能的情况下，设备会通知可读/写的字节数。- 警告设备可能不会通知当字节数比可以读/写更改多时。data：int64_t：可用于读/写的字节数。 NULL：可用于读/写的字节数目未知。|
|*AV_DEV_TO_APP_BUFFER_WRITABLE*||
|*AV_DEV_TO_APP_MUTE_STATE_CHANGED*|静音状态更改消息。设备通知静音状态已更改。data：int：0表示非静音状态，非零表示静音状态。|
|*AV_DEV_TO_APP_VOLUME_LEVEL_CHANGED*|音量变化信息。设备通知音量级别已更改。数据：double：新音量范围0.0 - 1.0。|




