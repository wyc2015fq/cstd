# webrtc的音频处理模块apm( audio processing)下载与编译出libwebrtc_audio_preprocessing.so - starRTC免费im直播会议一对一视频 - CSDN博客
2016年12月03日 15:25:59[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：3360
2017-05-22更新！
http://pan.baidu.com/s/1nvbTcRf
jni修改后的代码，可以直接ndk-build出so文件！！！！！！！！
-------------------------------------------------------
webrtc代码在android和chromium项目中都有。但是android中的那个带有Android.mk，稍微修改下就能用ndk-build编译出libwebrtc_audio_preprocessing.so
git clone [https://android.googlesource.com/platform/external/webrtc](https://android.googlesource.com/platform/external/webrtc)
 更新
cd webrtc

apm几个模块：
NS（Noise Suppression 噪声抑制）、
VAD（Voice Activity Detection 静音检测）、
AECM（Acoustic Echo Canceller for Mobile 声学回声消除）
AGC（Auto Gain Control 自动增益控制）
刚开始目录：
删除了modules下的audio_coding目录，并把src改名为webrtc了，当然同时得修改Android.mk中对audio_coding相关mk的引用，还要修改路径，原来是src，要改成webrtc.
最后会生成libwebrtc_audio_coding.so，这个需要在Android.mk中修改。其中有个
LOCAL_MODULE := libwebrtc_audio_coding
把相关的删除，从
include $(CLEAR_VARS)
。。。。
include $(BUILD_SHARED_LIBRARY)
这个也可以通过上面一样的办法删除
LOCAL_MODULE := libwebrtc_audio_coding_gnustl_static
其它文件删除：MODULE_LICENSE_BSD， NOTICE，TODO
test目录删除，.git目录删除

最终效果：
发现在modules\audio_processing\test目录下有unit_test.cc
ndk-build编译,几乎很少改动。即可生成libwebrtc_audio_preprocessing.so
===================================
上面错误解决：需要将源代码放在一个叫jni的目录中。

解决：

#include <map>报错
由于NDK默认使用最小C++运行时库进行项目的编译，导致无法编译WebRTC中使用诸如std::map等STL容器的源码。因此我们需要自行设定适合本项目的C++运行时库gnustl_static。
解决：
Application.mk
APP_ABI := armeabi-v7a
APP_STL := gnustl_static
APP_PLATFORM :=21
版本问题，
好像webrtc对android的版本要求过高，至少要求2.3.1，所以从原来的2.2换成2.3.1即可
解决办法：
#APP_PLATFORM := android-8     //对应2.2
APP_PLATFORM := android-9     //对应2.3.1
#APP_STL := stlport_static
APP_STL := gnustl_static
2.
undefined reference to 'WebRtcSpl_SqrtFloor'
这个函数在spl_sqrt_floor.c，在
webrtc\common_audio\signal_processing\Android.mk中的LOCAL_SRC_FILES加上
spl_sqrt_floor.c \
生成的so
附:
参考：
[https://github.com/webrtc/samples](https://github.com/webrtc/samples)
