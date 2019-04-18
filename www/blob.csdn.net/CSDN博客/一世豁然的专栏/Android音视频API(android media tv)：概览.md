# Android音视频API(android.media.tv)：概览 - 一世豁然的专栏 - CSDN博客





2017年07月07日 17:25:14[一世豁然](https://me.csdn.net/Explorer_day)阅读数：874








一、接口

[TvContract.BaseTvColumns](https://developer.android.google.cn/reference/android/media/tv/TvContract.BaseTvColumns.html)


电视频道/节目表的共同基础。





[TvView.OnUnhandledInputEventListener](https://developer.android.google.cn/reference/android/media/tv/TvView.OnUnhandledInputEventListener.html)


当接收到未处理的输入事件时，将调用回调的接口定义。








二、类

[TvContentRating](https://developer.android.google.cn/reference/android/media/tv/TvContentRating.html)


代表电视内容分级的类。





[TvContract](https://developer.android.google.cn/reference/android/media/tv/TvContract.html)


电视提供商和应用程序之间的合同。





[TvContract.Channels](https://developer.android.google.cn/reference/android/media/tv/TvContract.Channels.html)


电视频道表的列定义。





[TvContract.Channels.Logo](https://developer.android.google.cn/reference/android/media/tv/TvContract.Channels.Logo.html)


单个电视频道的子目录，代表其主要标志。





[TvContract.PreviewPrograms](https://developer.android.google.cn/reference/android/media/tv/TvContract.PreviewPrograms.html)


预览电视节目表的列定义。





[TvContract.Programs](https://developer.android.google.cn/reference/android/media/tv/TvContract.Programs.html)


电视节目表的列定义。





[TvContract.Programs.Genres](https://developer.android.google.cn/reference/android/media/tv/TvContract.Programs.Genres.html)


电视节目的规范流派。





[TvContract.RecordedPrograms](https://developer.android.google.cn/reference/android/media/tv/TvContract.RecordedPrograms.html)


记录的电视节目表的列定义。





[TvContract.WatchNextPrograms](https://developer.android.google.cn/reference/android/media/tv/TvContract.WatchNextPrograms.html)


“观看下一个”电视节目表的列定义。





[TvInputInfo](https://developer.android.google.cn/reference/android/media/tv/TvInputInfo.html)


该类用于指定电视输入的元信息。





[TvInputInfo.Builder](https://developer.android.google.cn/reference/android/media/tv/TvInputInfo.Builder.html)


用于创建TvInputInfo对象的便利构建器。





[TvInputManager](https://developer.android.google.cn/reference/android/media/tv/TvInputManager.html)


中央系统API到总体电视输入框架（TIF）架构，其仲裁应用程序和所选择的电视输入之间的交互。





[TvInputManager.TvInputCallback](https://developer.android.google.cn/reference/android/media/tv/TvInputManager.TvInputCallback.html)


回叫用于监视电视输入的状态。





[TvInputService](https://developer.android.google.cn/reference/android/media/tv/TvInputService.html)


TvInputService类代表电视输入或源，如HDMI或内置调谐器，可提供直通视频或广播电视节目。





[TvInputService.HardwareSession](https://developer.android.google.cn/reference/android/media/tv/TvInputService.HardwareSession.html)


电视输入会话的基类，代表连接到硬件电视输入的外部设备。





[TvInputService.RecordingSession](https://developer.android.google.cn/reference/android/media/tv/TvInputService.RecordingSession.html)


派生类的基类实现提供电视输入录音会话。





[TvInputService.Session](https://developer.android.google.cn/reference/android/media/tv/TvInputService.Session.html)


用于派生类的基类，用于实现提供电视输入会话。





[TvRecordingClient](https://developer.android.google.cn/reference/android/media/tv/TvRecordingClient.html)


用于与特定电视输入服务进行电视节目录制的公共接口对象。





[TvRecordingClient.RecordingCallback](https://developer.android.google.cn/reference/android/media/tv/TvRecordingClient.RecordingCallback.html)


回调用于在TvInputService.RecordingSession上接收各种状态更新





[TvTrackInfo](https://developer.android.google.cn/reference/android/media/tv/TvTrackInfo.html)


封装在TvInputService中播放的曲目格式。





[TvTrackInfo.Builder](https://developer.android.google.cn/reference/android/media/tv/TvTrackInfo.Builder.html)


用于创建TvTrackInfo对象的构建器类。





[TvView](https://developer.android.google.cn/reference/android/media/tv/TvView.html)


显示电视内容。





[TvView.TimeShiftPositionCallback](https://developer.android.google.cn/reference/android/media/tv/TvView.TimeShiftPositionCallback.html)


回调用于接收时间位置变化。





[TvView.TvInputCallback](https://developer.android.google.cn/reference/android/media/tv/TvView.TvInputCallback.html)


回调用于在TvView上接收各种状态更新。




