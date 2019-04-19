# 编译fdk-aac for ios - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月15日 09:56:39[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：439
所属专栏：[](https://blog.csdn.net/column/details/.html)[编解码](https://blog.csdn.net/column/details/12721.html)
                
Build all:
build-fdk-aac.sh
Build for some architectures:
build-fdk-aac.sh armv7s x86_64
Build universal library from separately built architectures:
build-fdk-aac.sh lipo
最后输出的fdk-aac-ios为fat库
如何集成到ffmpeg中。
将编译出来的fdk-aac-ios命名为fat-fdk-aac
build-fdk-aac.sh
艺搜参考
https://github.com/kewlbear/fdk-aac-build-script-for-iOS
            
