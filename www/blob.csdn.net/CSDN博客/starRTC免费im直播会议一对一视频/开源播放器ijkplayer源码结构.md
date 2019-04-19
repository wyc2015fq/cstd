# 开源播放器ijkplayer源码结构 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年11月07日 09:42:03[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：375
ijkplayer核心源码主要在ijkmedia文件夹下ijkplayer、ijksdl及ijkutils。
注：tag k0.3.1
player: remove ijkutil
android相关源码结构
- ijkmediademo:播放器实例demo
- ijkmediawidget:播放器组件封装,类似于系统播放器、vitamio结构，如mediacontroller、videoView。
- ijkmediaplayer:cpu armv7库。播放器核心jni层及相关上层调用接口开放(包括系统播放器封装切换)。
- ijkmediaplayer--* 其他cpu兼容库
jni API接口及主要核心流程
- ijkmedia/ijkplayer/android/ijkplayer-android.c
- ijkmedia/ijkplayer/android/ijkplayer-jni.c
- ijkmedia/ijkplayer/ff_ffplay.c
- ijkmedia/ijkplayer/ijkplayer.c
ijkplayer在解码含多路视频流的时候，默认先解码H264，其他播放器如VLC等解码第一路视频流(ff_ffplay.c文件中2293行)
## 艺搜参考
[http://blog.csdn.net/hpb21/article/details/43410449](http://blog.csdn.net/hpb21/article/details/43410449)
