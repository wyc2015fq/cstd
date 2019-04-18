# Android音视频API(android.media)：概览 - 一世豁然的专栏 - CSDN博客





2017年07月04日 16:23:28[一世豁然](https://me.csdn.net/Explorer_day)阅读数：2042








提供管理音频和视频中各种媒体接口的类。

媒体API用于播放，在某些情况下，用于记录媒体文件。 这包括音频（例如，播放MP3或其他音乐文件，铃声，游戏声音效果或DTMF音调）和视频（例如，播放通过web或从本地存储器流式传输的视频）。





该包中的其他特殊类提供了检测位图（FaceDetector）中的人脸，控制音频路由（到设备或耳机）并控制诸如铃声和电话振动（AudioManager）之类的警报的能力。








一、接口

[AudioManager.OnAudioFocusChangeListener](https://developer.android.google.cn/reference/android/media/AudioManager.OnAudioFocusChangeListener.html)


当更新系统的音频焦点时要调用的回调接口定义。





[AudioRecord.OnRecordPositionUpdateListener](https://developer.android.google.cn/reference/android/media/AudioRecord.OnRecordPositionUpdateListener.html)


当AudioRecord达到setNotificationMarkerPosition（int）设置的通知标记或由setPositionNotificationPeriod（int）设置的记录头的进度定期更新时，将调用回调的接口定义。





[AudioRecord.OnRoutingChangedListener](https://developer.android.google.cn/reference/android/media/AudioRecord.OnRoutingChangedListener.html)


此接口在API级别24中已被弃用，用户应切换到通用AudioRouting.OnRoutingChangedListener类。





[AudioRouting](https://developer.android.google.cn/reference/android/media/AudioRouting.html)


AudioRouting定义了一个用于控制AudioTrack和AudioRecord对象中的路由和路由通知的接口。





[AudioRouting.OnRoutingChangedListener](https://developer.android.google.cn/reference/android/media/AudioRouting.OnRoutingChangedListener.html)


定义应用程序可以接收关联的AudioRouting的路由更改通知接口。





[AudioTrack.OnPlaybackPositionUpdateListener](https://developer.android.google.cn/reference/android/media/AudioTrack.OnPlaybackPositionUpdateListener.html)


当AudioTrack的播放头位置到达通知标记或已经增加一定时间段时，要调用的回调的接口定义。





[AudioTrack.OnRoutingChangedListener](https://developer.android.google.cn/reference/android/media/AudioTrack.OnRoutingChangedListener.html)


此接口在API级别24中已被弃用，用户应切换到通用AudioRouting.OnRoutingChangedListener类。





[ImageReader.OnImageAvailableListener](https://developer.android.google.cn/reference/android/media/ImageReader.OnImageAvailableListener.html)


回调接口被通知新图像可用。





[ImageWriter.OnImageReleasedListener](https://developer.android.google.cn/reference/android/media/ImageWriter.OnImageReleasedListener.html)


ImageWriter回调接口，用于异步通知各种ImageWriter事件的应用程序。





[JetPlayer.OnJetEventListener](https://developer.android.google.cn/reference/android/media/JetPlayer.OnJetEventListener.html)


当JET引擎生成事件时处理通知。





[MediaCas.EventListener](https://developer.android.google.cn/reference/android/media/MediaCas.EventListener.html)


由调用者向setEventListener（MediCas.Event Listener，Handler）注册的接口，以从MediCas实例接收方案特定的通知。





[MediaCodec.OnFrameRenderedListener](https://developer.android.google.cn/reference/android/media/MediaCodec.OnFrameRenderedListener.html)


当输出框在输出表面上呈现时，将被调用的监听器





[MediaDrm.OnEventListener](https://developer.android.google.cn/reference/android/media/MediaDrm.OnEventListener.html)


在发生drm事件时调用回调的接口定义





[MediaDrm.OnExpirationUpdateListener](https://developer.android.google.cn/reference/android/media/MediaDrm.OnExpirationUpdateListener.html)


在发生drm会话到期更新时调用回调的接口定义





[MediaDrm.OnKeyStatusChangeListener](https://developer.android.google.cn/reference/android/media/MediaDrm.OnKeyStatusChangeListener.html)


当drm会话中的键更改状态时，将调用回调的接口定义。





[MediaPlayer.OnBufferingUpdateListener](https://developer.android.google.cn/reference/android/media/MediaPlayer.OnBufferingUpdateListener.html)


要调用的回调的接口定义，指示通过网络流式传输的媒体资源的缓冲状态。





[MediaPlayer.OnCompletionListener](https://developer.android.google.cn/reference/android/media/MediaPlayer.OnCompletionListener.html)


在播放媒体源时调用的回调的接口定义已完成。





[MediaPlayer.OnDrmConfigHelper](https://developer.android.google.cn/reference/android/media/MediaPlayer.OnDrmConfigHelper.html)


应用程序可以在会话打开之前执行DRM配置（get / set属性）时调用的回调的接口定义。





[MediaPlayer.OnDrmInfoListener](https://developer.android.google.cn/reference/android/media/MediaPlayer.OnDrmInfoListener.html)


当DRM信息可用时要调用的回调的接口定义





[MediaPlayer.OnDrmPreparedListener](https://developer.android.google.cn/reference/android/media/MediaPlayer.OnDrmPreparedListener.html)


接口定义回调以在DRM准备好进行密钥请求/响应时通知应用程序





[MediaPlayer.OnErrorListener](https://developer.android.google.cn/reference/android/media/MediaPlayer.OnErrorListener.html)


在异步操作中发生错误时要调用的回调的接口定义（其他错误会在方法调用时抛出异常）。





[MediaPlayer.OnInfoListener](https://developer.android.google.cn/reference/android/media/MediaPlayer.OnInfoListener.html)


要调用的回调的接口定义来传达关于媒体或其播放的一些信息和/或警告。





[MediaPlayer.OnPreparedListener](https://developer.android.google.cn/reference/android/media/MediaPlayer.OnPreparedListener.html)


媒体源准备好播放时要调用的回调的接口定义。





[MediaPlayer.OnSeekCompleteListener](https://developer.android.google.cn/reference/android/media/MediaPlayer.OnSeekCompleteListener.html)


要调用的回调的接口定义，指示搜索操作的完成。





[MediaPlayer.OnTimedMetaDataAvailableListener](https://developer.android.google.cn/reference/android/media/MediaPlayer.OnTimedMetaDataAvailableListener.html)


当轨道定时元数据可用时，要调用的回调的接口定义。





[MediaPlayer.OnTimedTextListener](https://developer.android.google.cn/reference/android/media/MediaPlayer.OnTimedTextListener.html)


当定时文本可用于显示时要调用的回调的接口定义。





[MediaPlayer.OnVideoSizeChangedListener](https://developer.android.google.cn/reference/android/media/MediaPlayer.OnVideoSizeChangedListener.html)


在首次知道或更新视频大小时要调用的回调的接口定义





[MediaRecorder.OnErrorListener](https://developer.android.google.cn/reference/android/media/MediaRecorder.OnErrorListener.html)


在录制时发生错误时调用回调的接口定义。





[MediaRecorder.OnInfoListener](https://developer.android.google.cn/reference/android/media/MediaRecorder.OnInfoListener.html)


要调用的回调的接口定义来传达一些信息和/或关于录音的警告。





[MediaScannerConnection.MediaScannerConnectionClient](https://developer.android.google.cn/reference/android/media/MediaScannerConnection.MediaScannerConnectionClient.html)


当建立与MediaScanner服务的连接以及文件的扫描完成时，用于通知MediaScannerConnection的客户端的界面。





[MediaScannerConnection.OnScanCompletedListener](https://developer.android.google.cn/reference/android/media/MediaScannerConnection.OnScanCompletedListener.html)


用于通知客户扫描所请求的媒体文件的结果的接口。





[MediaSync.OnErrorListener](https://developer.android.google.cn/reference/android/media/MediaSync.OnErrorListener.html)


在异步操作中发生错误时要调用的回调的接口定义（其他错误会在方法调用时抛出异常）。





[RemoteControlClient.OnGetPlaybackPositionListener](https://developer.android.google.cn/reference/android/media/RemoteControlClient.OnGetPlaybackPositionListener.html)


查询媒体播放位置时要调用的回调的接口定义。





[RemoteControlClient.OnMetadataUpdateListener](https://developer.android.google.cn/reference/android/media/RemoteControlClient.OnMetadataUpdateListener.html)


当一个元数据值被更新时要调用的回调的接口定义。





[RemoteControlClient.OnPlaybackPositionUpdateListener](https://developer.android.google.cn/reference/android/media/RemoteControlClient.OnPlaybackPositionUpdateListener.html)


要求更新媒体播放位置时要调用的回调的接口定义。





[RemoteController.OnClientUpdateListener](https://developer.android.google.cn/reference/android/media/RemoteController.OnClientUpdateListener.html)


每当媒体事件，元数据和播放状态可用时，回调的接口定义被调用。





[SoundPool.OnLoadCompleteListener](https://developer.android.google.cn/reference/android/media/SoundPool.OnLoadCompleteListener.html)





[VolumeAutomation](https://developer.android.google.cn/reference/android/media/VolumeAutomation.html)


VolumeAutomation定义了一个用于AudioTrack和MediaPlayer对象的自动音量控制的界面。








二、类

[AsyncPlayer](https://developer.android.google.cn/reference/android/media/AsyncPlayer.html)


播放一系列音频URI，但是在另一个线程上做所有的繁重的工作，所以准备或加载的任何缓慢都不会阻塞调用线程。





[AudioAttributes](https://developer.android.google.cn/reference/android/media/AudioAttributes.html)


封装描述有关音频流的信息的属性集合的类。





[AudioAttributes.Builder](https://developer.android.google.cn/reference/android/media/AudioAttributes.Builder.html)


AudioAttributes对象的Builder类。





[AudioDeviceCallback](https://developer.android.google.cn/reference/android/media/AudioDeviceCallback.html)


音频设备回拨定义了应用程序可以接收音频设备连接和断开事件通知的机制。





[AudioDeviceInfo](https://developer.android.google.cn/reference/android/media/AudioDeviceInfo.html)


类提供有关音频设备的信息。





[AudioFocusRequest](https://developer.android.google.cn/reference/android/media/AudioFocusRequest.html)


封装有关音频焦点请求的信息的类。





[AudioFocusRequest.Builder](https://developer.android.google.cn/reference/android/media/AudioFocusRequest.Builder.html)


AudioFocusRequest对象的Builder类。





[AudioFormat](https://developer.android.google.cn/reference/android/media/AudioFormat.html)


AudioFormat类用于访问多个音频格式和通道配置常量。





[AudioFormat.Builder](https://developer.android.google.cn/reference/android/media/AudioFormat.Builder.html)


AudioFormat对象的Builder类。





[AudioManager](https://developer.android.google.cn/reference/android/media/AudioManager.html)


AudioManager提供对音量和振铃模式控制的访问。





[AudioManager.AudioPlaybackCallback](https://developer.android.google.cn/reference/android/media/AudioManager.AudioPlaybackCallback.html)


用于接收有关系统上播放活动的更新通知的界面。





[AudioManager.AudioRecordingCallback](https://developer.android.google.cn/reference/android/media/AudioManager.AudioRecordingCallback.html)


用于接收关于记录配置的更新通知的界面。





[AudioPlaybackConfiguration](https://developer.android.google.cn/reference/android/media/AudioPlaybackConfiguration.html)


AudioPlaybackConfiguration类收集描述音频回放会话的信息。





[AudioRecord](https://developer.android.google.cn/reference/android/media/AudioRecord.html)


AudioRecord类管理用于Java应用程序的音频资源，以从平台的音频输入硬件记录音频。





[AudioRecord.Builder](https://developer.android.google.cn/reference/android/media/AudioRecord.Builder.html)


AudioRecord对象的Builder类。





[AudioRecordingConfiguration](https://developer.android.google.cn/reference/android/media/AudioRecordingConfiguration.html)


AudioRecordingConfiguration类收集描述录音会话的信息。





[AudioTimestamp](https://developer.android.google.cn/reference/android/media/AudioTimestamp.html)


相对于假定的音频流以帧为单位分组位置的结构，以及该帧进入或离开该设备上的音频处理流水线的估计时间。





[AudioTrack](https://developer.android.google.cn/reference/android/media/AudioTrack.html)


AudioTrack类为Java应用程序管理和播放单个音频资源。





[AudioTrack.Builder](https://developer.android.google.cn/reference/android/media/AudioTrack.Builder.html)


AudioTrack对象的Builder类。





[CamcorderProfile](https://developer.android.google.cn/reference/android/media/CamcorderProfile.html)


检索摄像机应用程序的预定摄像机配置文件设置。





[CameraProfile](https://developer.android.google.cn/reference/android/media/CameraProfile.html)


CameraProfile类用于检索在相机应用程序中用于低，中和高质量设置的预定义静止图像捕获（jpeg）质量等级（0-100）。





[DrmInitData](https://developer.android.google.cn/reference/android/media/DrmInitData.html)


封装MediaDrm实例所需的初始化数据。





[DrmInitData.SchemeInitData](https://developer.android.google.cn/reference/android/media/DrmInitData.SchemeInitData.html)


方案初始化数据。





[ExifInterface](https://developer.android.google.cn/reference/android/media/ExifInterface.html)


这是用于在JPEG文件或RAW图像文件中读写Exif标签的类。





[FaceDetector](https://developer.android.google.cn/reference/android/media/FaceDetector.html)


识别位图图形对象中人物的脸部。





[FaceDetector.Face](https://developer.android.google.cn/reference/android/media/FaceDetector.Face.html)


脸部包含标识位图中脸部位置的所有信息。





[Image](https://developer.android.google.cn/reference/android/media/Image.html)


与媒体源（如MediaCodec或CameraDevice）一起使用的单个完整映像缓冲区。





[Image.Plane](https://developer.android.google.cn/reference/android/media/Image.Plane.html)


图像数据的单色平面。





[ImageReader](https://developer.android.google.cn/reference/android/media/ImageReader.html)


ImageReader类允许直接应用程序访问渲染到Surface中的图像数据


几个Android媒体API类接受Surface对象作为渲染目标，包括MediaPlayer，MediaCodec，CameraDevice，ImageWriter和RenderScript分配。





[ImageWriter](https://developer.android.google.cn/reference/android/media/ImageWriter.html)


ImageWriter类允许应用程序将图像数据生成到Surface中，并由CameraDevice等其他组件使用。





[JetPlayer](https://developer.android.google.cn/reference/android/media/JetPlayer.html)


JetPlayer提供对JET内容播放和控制的访问。





[MediaActionSound](https://developer.android.google.cn/reference/android/media/MediaActionSound.html)


用于产生与媒体和相机API采取的各种动作产生的声音相匹配的音乐类。





[MediaCas](https://developer.android.google.cn/reference/android/media/MediaCas.html)


MediaCas可用于与MediaDescrambler一起获取用于解扰受保护媒体流的密钥。





[MediaCas.PluginDescriptor](https://developer.android.google.cn/reference/android/media/MediaCas.PluginDescriptor.html)


用其CA_system_ID和字符串名称描述CAS插件。





[MediaCas.Session](https://developer.android.google.cn/reference/android/media/MediaCas.Session.html)


与CA系统打开会话的类。





[MediaCodec](https://developer.android.google.cn/reference/android/media/MediaCodec.html)


MediaCodec类可用于访问低级媒体编解码器，即





[MediaCodec.BufferInfo](https://developer.android.google.cn/reference/android/media/MediaCodec.BufferInfo.html)


每个缓冲器元数据包括指定相关联的编解码器（输出）缓冲器中的有效数据的范围的偏移量和大小。





[MediaCodec.Callback](https://developer.android.google.cn/reference/android/media/MediaCodec.Callback.html)


MediaCodec回调接口。





[MediaCodec.CryptoInfo](https://developer.android.google.cn/reference/android/media/MediaCodec.CryptoInfo.html)


描述（至少部分）加密输入样本的结构的元数据。





[MediaCodec.CryptoInfo.Pattern](https://developer.android.google.cn/reference/android/media/MediaCodec.CryptoInfo.Pattern.html)


描述子样本中受保护字节的加密模式的元数据。





[MediaCodec.MetricsConstants](https://developer.android.google.cn/reference/android/media/MediaCodec.MetricsConstants.html)





[MediaCodecInfo](https://developer.android.google.cn/reference/android/media/MediaCodecInfo.html)


提供有关设备上可用的给定媒体编解码器的信息。





[MediaCodecInfo.AudioCapabilities](https://developer.android.google.cn/reference/android/media/MediaCodecInfo.AudioCapabilities.html)


支持查询编解码器音频功能的类。





[MediaCodecInfo.CodecCapabilities](https://developer.android.google.cn/reference/android/media/MediaCodecInfo.CodecCapabilities.html)


封装给定编解码器组件的功能。





[MediaCodecInfo.CodecProfileLevel](https://developer.android.google.cn/reference/android/media/MediaCodecInfo.CodecProfileLevel.html)


封装可用于编解码器组件的配置文件。





[MediaCodecInfo.EncoderCapabilities](https://developer.android.google.cn/reference/android/media/MediaCodecInfo.EncoderCapabilities.html)


支持查询编解码器的编码功能的类。





[MediaCodecInfo.VideoCapabilities](https://developer.android.google.cn/reference/android/media/MediaCodecInfo.VideoCapabilities.html)


支持查询编解码器的视频功能的类。





[MediaCodecList](https://developer.android.google.cn/reference/android/media/MediaCodecList.html)


允许您枚举可用的编解码器，每个指定为MediaCodecInfo对象，找到支持给定格式的编解码器，并查询给定编解码器的功能。





[MediaCrypto](https://developer.android.google.cn/reference/android/media/MediaCrypto.html)


MediaCrypto类可以与MediaCodec一起使用，以解码加密的媒体数据。





[MediaDataSource](https://developer.android.google.cn/reference/android/media/MediaDataSource.html)


将媒体数据提供给框架。





[MediaDescrambler](https://developer.android.google.cn/reference/android/media/MediaDescrambler.html)


MediaDescrambler类可以与MediaCodec和MediaExtractor结合使用，以对通过ISO / IEC13818-1中的条件访问（CA）系统加扰的媒体数据进行解码。





[MediaDescription](https://developer.android.google.cn/reference/android/media/MediaDescription.html)


用于适合显示的媒体项目的简单的元数据集。





[MediaDescription.Builder](https://developer.android.google.cn/reference/android/media/MediaDescription.Builder.html)


MediaDescription对象的构建器。





[MediaDrm](https://developer.android.google.cn/reference/android/media/MediaDrm.html)


MediaDrm可以与MediaCrypto结合使用来获取解密受保护媒体流的密钥。





[MediaDrm.CryptoSession](https://developer.android.google.cn/reference/android/media/MediaDrm.CryptoSession.html)


除了支持DASH通用加密介质的解密外，MediaDrm API提供了根据工厂安装的信任根安全地将会话密钥从运营商的会话密钥服务器传递到客户端设备的能力，然后执行加密，解密， 使用会话密钥对任意用户数据进行签名和验证操作。





[MediaDrm.KeyRequest](https://developer.android.google.cn/reference/android/media/MediaDrm.KeyRequest.html)


包含应用程序用于从许可证服务器请求密钥的不透明数据





[MediaDrm.KeyStatus](https://developer.android.google.cn/reference/android/media/MediaDrm.KeyStatus.html)


定义键的状态。





[MediaDrm.ProvisionRequest](https://developer.android.google.cn/reference/android/media/MediaDrm.ProvisionRequest.html)


包含应用程序从配置服务器请求证书的不透明数据





[MediaExtractor](https://developer.android.google.cn/reference/android/media/MediaExtractor.html)


MediaExtractor有助于从数据源中提取解复用的，通常编码的媒体数据。





[MediaExtractor.CasInfo](https://developer.android.google.cn/reference/android/media/MediaExtractor.CasInfo.html)


描述用于争夺轨道的条件访问系统。





[MediaExtractor.MetricsConstants](https://developer.android.google.cn/reference/android/media/MediaExtractor.MetricsConstants.html)





[MediaFormat](https://developer.android.google.cn/reference/android/media/MediaFormat.html)


封装描述媒体数据格式的信息，无论是音频还是视频。





[MediaMetadata](https://developer.android.google.cn/reference/android/media/MediaMetadata.html)


包含有关项目的元数据，如标题，艺术家等。





[MediaMetadata.Builder](https://developer.android.google.cn/reference/android/media/MediaMetadata.Builder.html)


用于构建MediaMetadata对象。





[MediaMetadataEditor](https://developer.android.google.cn/reference/android/media/MediaMetadataEditor.html)


该类在API级别21中已被弃用。请使用MediaMetadata与MediaSession一起使用。





[MediaMetadataRetriever](https://developer.android.google.cn/reference/android/media/MediaMetadataRetriever.html)


MediaMetadataRetriever类提供了一个统一的接口，用于从输入媒体文件中检索帧和元数据。





[MediaMuxer](https://developer.android.google.cn/reference/android/media/MediaMuxer.html)


MediaMuxer便于多路复用基本流。





[MediaMuxer.OutputFormat](https://developer.android.google.cn/reference/android/media/MediaMuxer.OutputFormat.html)

定义输出格式。





[MediaPlayer](https://developer.android.google.cn/reference/android/media/MediaPlayer.html)


MediaPlayer类可用于控制音频/视频文件和流的播放。





[MediaPlayer.DrmInfo](https://developer.android.google.cn/reference/android/media/MediaPlayer.DrmInfo.html)


封装源的DRM属性。





[MediaPlayer.MetricsConstants](https://developer.android.google.cn/reference/android/media/MediaPlayer.MetricsConstants.html)





[MediaPlayer.TrackInfo](https://developer.android.google.cn/reference/android/media/MediaPlayer.TrackInfo.html)


媒体播放器的类返回每个音频/视频/字幕轨道元数据。





[MediaRecorder](https://developer.android.google.cn/reference/android/media/MediaRecorder.html)


用于录制音频和视频。





[MediaRecorder.AudioEncoder](https://developer.android.google.cn/reference/android/media/MediaRecorder.AudioEncoder.html)


定义音频编码。





[MediaRecorder.AudioSource](https://developer.android.google.cn/reference/android/media/MediaRecorder.AudioSource.html)


定义音频源。





[MediaRecorder.MetricsConstants](https://developer.android.google.cn/reference/android/media/MediaRecorder.MetricsConstants.html)





[MediaRecorder.OutputFormat](https://developer.android.google.cn/reference/android/media/MediaRecorder.OutputFormat.html)


定义输出格式。





[MediaRecorder.VideoEncoder](https://developer.android.google.cn/reference/android/media/MediaRecorder.VideoEncoder.html)


定义视频编码。





[MediaRecorder.VideoSource](https://developer.android.google.cn/reference/android/media/MediaRecorder.VideoSource.html)


定义视频源。





[MediaRouter](https://developer.android.google.cn/reference/android/media/MediaRouter.html)


MediaRouter允许应用程序控制从当前设备到外部扬声器和目标设备的媒体通道和流的路由。





[MediaRouter.Callback](https://developer.android.google.cn/reference/android/media/MediaRouter.Callback.html)


用于接收关于媒体路由更改的事件的接口。





[MediaRouter.RouteCategory](https://developer.android.google.cn/reference/android/media/MediaRouter.RouteCategory.html)


定义一类路由。





[MediaRouter.RouteGroup](https://developer.android.google.cn/reference/android/media/MediaRouter.RouteGroup.html)


关于由组中的多个其他路由组成的路由的信息。





[MediaRouter.RouteInfo](https://developer.android.google.cn/reference/android/media/MediaRouter.RouteInfo.html)


有关媒体路由的信息。





[MediaRouter.SimpleCallback](https://developer.android.google.cn/reference/android/media/MediaRouter.SimpleCallback.html)


Stub实现MediaRouter.Callback。





[MediaRouter.UserRouteInfo](https://developer.android.google.cn/reference/android/media/MediaRouter.UserRouteInfo.html)


有关应用程序可能定义和修改的路由的信息。





[MediaRouter.VolumeCallback](https://developer.android.google.cn/reference/android/media/MediaRouter.VolumeCallback.html)


用于接收有关卷更改的事件的界面。





[MediaScannerConnection](https://developer.android.google.cn/reference/android/media/MediaScannerConnection.html)


MediaScannerConnection提供了一种应用程序将新创建或下载的媒体文件传递到媒体扫描器服务的方法。





[MediaSync](https://developer.android.google.cn/reference/android/media/MediaSync.html)


MediaSync类可用于同步播放音频和视频流。





[MediaSync.Callback](https://developer.android.google.cn/reference/android/media/MediaSync.Callback.html)


MediaSync回调接口。





[MediaSyncEvent](https://developer.android.google.cn/reference/android/media/MediaSyncEvent.html)


MediaSyncEvent类定义可用于同步播放或捕获不同播放器和录像机之间的动作的事件。





[MediaTimestamp](https://developer.android.google.cn/reference/android/media/MediaTimestamp.html)


表示媒体时间与系统时间之间的线性相关性的不可变对象。





[PlaybackParams](https://developer.android.google.cn/reference/android/media/PlaybackParams.html)


普通播放参数的结构。





[Rating](https://developer.android.google.cn/reference/android/media/Rating.html)


封装用作内容元数据的评分信息的类。





[RemoteControlClient](https://developer.android.google.cn/reference/android/media/RemoteControlClient.html)


该类在API级别21中已被弃用。请改用MediaSession。





[RemoteControlClient.MetadataEditor](https://developer.android.google.cn/reference/android/media/RemoteControlClient.MetadataEditor.html)


此类已在API级别21中弃用。请改用MediaMetadata和MediaSession。





[RemoteController](https://developer.android.google.cn/reference/android/media/RemoteController.html)


此类在API级别21中已被弃用。请改用MediaController。





[RemoteController.MetadataEditor](https://developer.android.google.cn/reference/android/media/RemoteController.MetadataEditor.html)


用于读取RemoteControlClient发布的元数据的类，或者为可编辑的密钥发送一个RemoteControlClient新值。





[Ringtone](https://developer.android.google.cn/reference/android/media/Ringtone.html)


Ringtone提供了一种播放铃声，通知或其他类似类型声音的快速方法。





[RingtoneManager](https://developer.android.google.cn/reference/android/media/RingtoneManager.html)


RingtoneManager提供对铃声，通知和其他类型声音的访问。





[SoundPool](https://developer.android.google.cn/reference/android/media/SoundPool.html)


SoundPool类管理和播放应用程序的音频资源。





[SoundPool.Builder](https://developer.android.google.cn/reference/android/media/SoundPool.Builder.html)


SoundPool对象的Builder类。





[SyncParams](https://developer.android.google.cn/reference/android/media/SyncParams.html)


公共A / V同步参数的结构。





[ThumbnailUtils](https://developer.android.google.cn/reference/android/media/ThumbnailUtils.html)


媒体提供商的缩略图生成例程。





[TimedMetaData](https://developer.android.google.cn/reference/android/media/TimedMetaData.html)


包含一个定时元数据访问单元的类

1、时间戳，和

2、从容器直接提取原始的未解释的字节数组。





[TimedText](https://developer.android.google.cn/reference/android/media/TimedText.html)


持有时间文本元数据的类，包括：


1、渲染的角色

2、定时文本的渲染位置

为了呈现定时文本，应用程序需要执行以下操作：


1、实现MediaPlayer.OnTimedTextListener接口

2、在用于播放的MediaPlayer对象上注册MediaPlayer.OnTimedTextListener回调

3、收到onTimedText回调时，请执行以下操作：

1）、调用getText（）获取渲染的字符

2）、调用getBounds（）来获取文本渲染区域/区域




[ToneGenerator](https://developer.android.google.cn/reference/android/media/ToneGenerator.html)


该类提供播放DTMF音调（ITU-T建议Q.23），呼叫监督音调（3GPP TS 22.001，CEPT）和专有音调（3GPP TS 31.111）的方法。





[VolumeProvider](https://developer.android.google.cn/reference/android/media/VolumeProvider.html)


处理请求以调整或设置会话上的卷。





[VolumeShaper](https://developer.android.google.cn/reference/android/media/VolumeShaper.html)


VolumeShaper类用于在媒体播放期间自动控制音频音量，允许简单实现过渡效果和躲避。





[VolumeShaper.Configuration](https://developer.android.google.cn/reference/android/media/VolumeShaper.Configuration.html)


VolumeShaper.Configuration类包含曲线和持续时间信息。





[VolumeShaper.Configuration.Builder](https://developer.android.google.cn/reference/android/media/VolumeShaper.Configuration.Builder.html)


一个VolumeShaper.Configuration对象的Builder类。





[VolumeShaper.Operation](https://developer.android.google.cn/reference/android/media/VolumeShaper.Operation.html)


VolumeShaper.Operation类用于指定影响卷更改的VolumeShaper的操作。







三、Exceptions

[DeniedByServerException](https://developer.android.google.cn/reference/android/media/DeniedByServerException.html)


配置服务器或密钥服务器拒绝设备的证书或许可证时抛出异常。





[MediaCasException](https://developer.android.google.cn/reference/android/media/MediaCasException.html)


MediaCas异常的基类





[MediaCasException.DeniedByServerException](https://developer.android.google.cn/reference/android/media/MediaCasException.DeniedByServerException.html)


配置服务器或密钥服务器拒绝设备的许可证时抛出异常。





[MediaCasException.NotProvisionedException](https://developer.android.google.cn/reference/android/media/MediaCasException.NotProvisionedException.html)


在成功配置MediaCas对象之前尝试进行操作时抛出异常。





[MediaCasException.ResourceBusyException](https://developer.android.google.cn/reference/android/media/MediaCasException.ResourceBusyException.html)


尝试对MediaCas对象进行操作并且由于使用硬件资源不可用而抛出异常。





[MediaCasException.UnsupportedCasException](https://developer.android.google.cn/reference/android/media/MediaCasException.UnsupportedCasException.html)


尝试使用设备不支持的CA_system_id构建MediaCas对象时抛出异常





[MediaCasStateException](https://developer.android.google.cn/reference/android/media/MediaCasStateException.html)


MediaCas运行时异常的基类





[MediaCodec.CodecException](https://developer.android.google.cn/reference/android/media/MediaCodec.CodecException.html)


内部编解码器错误发生时抛出。





[MediaCodec.CryptoException](https://developer.android.google.cn/reference/android/media/MediaCodec.CryptoException.html)


在排队安全输入缓冲区时发生加密错误时抛出。





[MediaCryptoException](https://developer.android.google.cn/reference/android/media/MediaCryptoException.html)


如果MediaCrypto对象无法实例化，或者无法对MediaCrypto对象执行操作，则抛出异常。





[MediaDrm.MediaDrmStateException](https://developer.android.google.cn/reference/android/media/MediaDrm.MediaDrmStateException.html)


在MediaDrm操作期间发生不可恢复的故障时抛出





[MediaDrmException](https://developer.android.google.cn/reference/android/media/MediaDrmException.html)


MediaDrm异常的基类





[MediaDrmResetException](https://developer.android.google.cn/reference/android/media/MediaDrmResetException.html)


当MediaDrm实例由于重新启动mediaserver进程而变得不可用时，抛出此异常。





[MediaPlayer.NoDrmSchemeException](https://developer.android.google.cn/reference/android/media/MediaPlayer.NoDrmSchemeException.html)


在通过prepareDrm（）准备DRM方案之前调用DRM方法时抛出。





[MediaPlayer.ProvisioningNetworkErrorException](https://developer.android.google.cn/reference/android/media/MediaPlayer.ProvisioningNetworkErrorException.html)


当设备需要DRM配置时，由于网络错误（Internet可达性，超时等）而导致配置尝试失败）时抛出。





[MediaPlayer.ProvisioningServerErrorException](https://developer.android.google.cn/reference/android/media/MediaPlayer.ProvisioningServerErrorException.html)


当设备需要DRM配置时引发，但由于配置服务器拒绝该请求，配置尝试失败。





[NotProvisionedException](https://developer.android.google.cn/reference/android/media/NotProvisionedException.html)


尝试在MediaDrm对象上进行操作并且设备没有证书时抛出异常。





[ResourceBusyException](https://developer.android.google.cn/reference/android/media/ResourceBusyException.html)


尝试使用MediaDrm对象的操作并且由于使用硬件资源不可用时抛出异常。





[UnsupportedSchemeException](https://developer.android.google.cn/reference/android/media/UnsupportedSchemeException.html)


使用设备不支持的加密方案UUID尝试构建MediaDrm对象时抛出的异常




