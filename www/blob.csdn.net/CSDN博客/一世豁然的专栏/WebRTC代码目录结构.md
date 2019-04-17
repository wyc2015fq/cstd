# WebRTC代码目录结构 - 一世豁然的专栏 - CSDN博客





2017年11月07日 12:07:42[一世豁然](https://me.csdn.net/Explorer_day)阅读数：327标签：[webrtc](https://so.csdn.net/so/search/s.do?q=webrtc&t=blog)
个人分类：[WebRTC](https://blog.csdn.net/Explorer_day/article/category/6289311)











本博客转载于：[http://blog.csdn.net/wanghorse/article/details/46387997](http://blog.csdn.net/wanghorse/article/details/46387997)







├── ./base//基础平台库，包括线程、锁、socket等

├── ./build//编译脚本，gyp

├── ./common_audio//基础公共的音频处理

│├── ./common_audio/include//就一个类型转换头文件

│├── ./common_audio/resampler//音频重采样代码

│ ├── ./common_audio/signal_processing//音频信号处理代码，和硬件平台有关，有汇编代码

│ └── ./common_audio/vad//vad代码

├── ./common_video//基础的公共视频处理，如I420桢处理、scaler、plane

├── ./examples//例子

├── ./libjingle//libjingle

├── ./modules

│ ├── ./modules/audio_coding

│ │ ├── ./modules/audio_coding/codecs//音频codec处理，统一封装公共接口和各类不同的codec的具体处理，cng,g711,g722等

│ │ ├── ./modules/audio_coding/main//音频codec处理模块代码

│ │ │ ├── ./modules/audio_coding/main/acm2//音频处理模块的主要代码

│ │ └── ./modules/audio_coding/neteq//neteq代码

│ │ ├── ./modules/audio_coding/neteq/interface

│ │ ├── ./modules/audio_coding/neteq/mock

│ │ ├── ./modules/audio_coding/neteq/test

│ │ └── ./modules/audio_coding/neteq/tools

│ ├── ./modules/audio_conference_mixer//音频合成代码

│ │ ├── ./modules/audio_conference_mixer/interface

│ │ └── ./modules/audio_conference_mixer/source

│ ├── ./modules/audio_device//audio设备处理代码，采集和放音，android，ios，linux，mac，win

│ │ ├── ./modules/audio_device/main//AudioDeviceModule处理代码

│ ├── ./modules/audio_processing//音频前后端处理，aec，aecm,agc,beamformer,ns,transient

│ ├── ./modules/bitrate_controller//码率模块控制代码

│ ├── ./modules/desktop_capture//桌面抓拍处理代码和各平台处理代码,mac,win,x11

│ ├── ./modules/interface│
 ├── ./modules/media_file //播放录制文件模块代码，支持avi

│ ├── ./modules/pacing//码率探测代码

│ ├── ./modules/remote_bitrate_estimator//远端码率计算

│ ├── ./modules/rtp_rtcp//rtp、rtcp的处理代码，封装解封装，各种codec的不同处理、fec

│ ├── ./modules/utility

│ ├── ./modules/video_capture//视频摄像头采集代码，android、ios、linux、mac、win

│ ├── ./modules/video_coding//视频codec处理代码，i420、vp8、vp9

│ │ ├── ./modules/video_coding/codecs

│ │ ├── ./modules/video_coding/main//VideoCodingModule处理代码

│ ├── ./modules/video_processing//视频前后处理，brighten，color
 enhancement，deflickering，spatial resampler等

│ │ └── ./modules/video_processing/main//VideoProcessingModule

│ └── ./modules/video_render//视频渲染代码，android，ios、linux、mac、windows、opengles

├── ./p2p//nat穿越代码，turn/stun等，服务器和客户端

│ ├── ./p2p/base

│ └── ./p2p/client

├── ./sound//未知

├── ./system_wrappers//系统api封装

├── ./test├── ./tools//音视频测试工具代码

├── ./video//未知

├── ./video_engine//视频引擎代码，视频的处理流程

└── ./voice_engine//音频引擎代码，音频处理流程




