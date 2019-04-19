# AAC终结者Opus音频编码器的瑞士军刀，编译android ios - starRTC免费im直播会议一对一视频 - CSDN博客
2017年10月27日 15:23:56[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：584
AAC-LD/ELD it is either 480 or 512 PCM samples per frame and channel.
[http://opus-codec.org/downloads/](http://opus-codec.org/downloads/)
[https://archive.mozilla.org/pub/opus/opus-1.2.1.tar.gz](https://archive.mozilla.org/pub/opus/opus-1.2.1.tar.gz)
build for android:
[https://stackoverflow.com/questions/17869333/makefile-needed-for-compiling-the-opus-codec-for-android](https://stackoverflow.com/questions/17869333/makefile-needed-for-compiling-the-opus-codec-for-android)
ERROR: opus not found using pkg-config ，
由于Android的NDK编译链并没有提供pkg-config，而导致命令不存在的问题
1，把pc文件的路径PKG_CONFIG_PATHI放在/etc/profile里
2,编译时指定--pkg-config=/usr/bin/pkg-config \
3，新建opus.pc
4,将opus的库和头文件放到pc文件指定的目录里面。其中头文件不要放在opus文件夹里。直接放在include文件夹里。
[http://blog.itbiu.com/2017/07/26/201707261/](http://blog.itbiu.com/2017/07/26/201707261/)
build for ios
把tar包放在build/src
编译出来的库在dependencies目录
ffmpeg libopus解码器解码出来的帧格式是s16的。而不是aac解码出来的fltp.另外发现aac解码出来的的帧采样数跟编码时一致，都 是1024，但是opus编码时为320，解码出来却有960，查看源码发现。其内部使用了48k的采样率。
集成 opus 
需要安装 gas-processor.pl,
brew
brew install yasm
brew install pkg-config
