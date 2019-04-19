# JNI NDK开发Crash错误定位 调试 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月24日 16:37:46[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：305
总结：
搜索backtrace 
然后：
$ /d/android-ndk-r10c/toolchains/arm-linux-androideabi-4.9/prebuilt/windows-x86
_64/bin/arm-linux-androideabi-addr2line.exe -e /e/devel/tools/androidProjects/J
niDemux/app/src/main/obj/local/armeabi-v7a/libjnimedia.so 0006494f
e:\devel\tools\androidProjects\JniDemux\app\src\main/jni/avdemux.c:1207
