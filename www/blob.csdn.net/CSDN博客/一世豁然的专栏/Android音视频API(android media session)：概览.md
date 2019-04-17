# Android音视频API(android.media.session)：概览 - 一世豁然的专栏 - CSDN博客





2017年07月07日 10:33:30[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1576








一、接口

[MediaSessionManager.OnActiveSessionsChangedListener](https://developer.android.google.cn/reference/android/media/session/MediaSessionManager.OnActiveSessionsChangedListener.html)


监听活动会话列表的更改。








二、类

[MediaController](https://developer.android.google.cn/reference/android/media/session/MediaController.html)


允许应用与正在进行的媒体会话进行交互。





[MediaController.Callback](https://developer.android.google.cn/reference/android/media/session/MediaController.Callback.html)


回调用于从会话接收更新。





[MediaController.PlaybackInfo](https://developer.android.google.cn/reference/android/media/session/MediaController.PlaybackInfo.html)


保存有关当前播放的信息以及如何处理此会话的音频。





[MediaController.TransportControls](https://developer.android.google.cn/reference/android/media/session/MediaController.TransportControls.html)


接口，用于控制会话中的媒体播放。





[MediaSession](https://developer.android.google.cn/reference/android/media/session/MediaSession.html)


允许与媒体控制器，音量键，媒体按钮和传输控件进行交互。





[MediaSession.Callback](https://developer.android.google.cn/reference/android/media/session/MediaSession.Callback.html)


从控制器和系统接收媒体按钮，传输控制和命令。





[MediaSession.QueueItem](https://developer.android.google.cn/reference/android/media/session/MediaSession.QueueItem.html)


作为播放队列的一部分的单个项目。





[MediaSession.Token](https://developer.android.google.cn/reference/android/media/session/MediaSession.Token.html)


代表正在进行的会话。





[MediaSessionManager](https://developer.android.google.cn/reference/android/media/session/MediaSessionManager.html)


提供与应用程序已发布的媒体会话交互以表达其正在进行的媒体播放状态的支持。





[PlaybackState](https://developer.android.google.cn/reference/android/media/session/PlaybackState.html)


MediaSession的播放状态。





[PlaybackState.Builder](https://developer.android.google.cn/reference/android/media/session/PlaybackState.Builder.html)


PlaybackState对象的构建器。





[PlaybackState.CustomAction](https://developer.android.google.cn/reference/android/media/session/PlaybackState.CustomAction.html)


通过将应用程序特定的操作公开给MediaControllers，CustomActions可用于扩展标准传输控件的功能。





[PlaybackState.CustomAction.Builder](https://developer.android.google.cn/reference/android/media/session/PlaybackState.CustomAction.Builder.html)


PlaybackState.CustomAction对象的构建器。




