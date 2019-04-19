# librtmp编译for android and ios 不要openssl - starRTC免费im直播会议一对一视频 - CSDN博客
2018年08月15日 17:28:35[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：175
git clone git://git.ffmpeg.org/rtmpdump
不想要openssl
在rtmp.h里面
#undef CRYPTO
编译动态库与静态库只需要修改下面的
#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)
ffplay rtmp://101.200.44.42/live/starRTC
iPhoneOS.platform/Developer/usr/bin/gcc: No such file or directory
不要openssl
#CRYPTO=OPENSSL
CRYPTO=
[https://blog.csdn.net/cjj198561/article/details/28955461](https://blog.csdn.net/cjj198561/article/details/28955461)
