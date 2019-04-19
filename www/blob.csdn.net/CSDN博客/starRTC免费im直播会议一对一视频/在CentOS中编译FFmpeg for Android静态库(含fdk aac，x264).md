# 在CentOS中编译FFmpeg for Android静态库(含fdk aac，x264) - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月11日 20:49:10[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1373
所属专栏：[](https://blog.csdn.net/column/details/.html)[编解码](https://blog.csdn.net/column/details/12721.html)

本文可以编译出集成了x264和fdk_aac的库，而且支持neon
下载源码：
[https://github.com/mstorsjo/fdk-aac](https://github.com/mstorsjo/fdk-aac)
[http://sourceforge.net/projects/opencore-amr/files/fdk-aac/](http://sourceforge.net/projects/opencore-amr/files/fdk-aac/)
fdk_aac目前最新版是0.1.4，详见其ChangeLog，此处用的是0.1.1版
[http://www.videolan.org/developers/x264.html](http://www.videolan.org/developers/x264.html)
ffmpeg-2.7.2
解压后重命名文件夹：
mv fdk-aac-0.1.1 fdk-aac
mv ffmpeg-2.7.2 ffmpeg
mv x264-snapshot-20150813-22455 x264 
cp  cmdl_parser.cpp fdk-aac/libSYS/src/
cp genericStds.h fdk-aac/libSYS/include/
需要修改脚本里面ndk的路径。
chmod 777 build_android_static.sh 
./build_android.sh > result.log
结果：
本文所用脚本下载：
[https://yunpan.cn/c6SrYusQUPWqm](https://yunpan.cn/c6SrYusQUPWqm)  访问密码 5b38

附录：
问题1：
编译libSYS/src/cmdl_parser.cpp出错
stdio.h:57:9: error: 'off_t' does not name a type  
解决：注释掉cmdl_parser.cpp中的下面语句：
//#define _SYS_TYPES_H_  
问题2：
libSYS/src/genericStds.cpp:108 ： stdio.h:57:9: error: 'off_t' does not name a type  
解决：修改libSYS\include\genericStds.h
注释掉：
//#define _SYS_TYPES_H_  
问题3：
libavcodec/libavcodec.a(fft_fixed_init_arm.o): In function `ff_fft_fixed_init_arm':
fft_fixed_init_arm.c:(.text.unlikely+0x2c): undefined reference to `ff_fft_fixed_calc_neon'
libavcodec/libavcodec.a(fft_init_arm.o): In function `ff_fft_init_arm':
fft_init_arm.c:(.text.unlikely+0x30): undefined reference to `ff_fft_permute_neon'
fft_init_arm.c:(.text.unlikely+0x34): undefined reference to `ff_fft_calc_neon'
但好像编译出了.a,只是最后Building one...时报错了。
解决：使用最新的ffmpeg版本,并注释掉build_one那一行。
## 艺搜参考
[https://github.com/nxtreaming/FFmpegAndroid/blob/master/FFmpegLibrary/jni/build_android.sh](https://github.com/nxtreaming/FFmpegAndroid/blob/master/FFmpegLibrary/jni/build_android.sh)

[http://blog.csdn.net/iamshaofa/article/details/17551185](http://blog.csdn.net/iamshaofa/article/details/17551185)
[http://www.51testing.com/html/88/377588-834412.html](http://www.51testing.com/html/88/377588-834412.html)
