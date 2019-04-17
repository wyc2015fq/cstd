# webrtc的音频处理模块apm( audio processing)下载与编译出libwebrtc_audio_preprocessing.so - DoubleLi - 博客园






2017-05-22更新！

http://pan.baidu.com/s/1nvbTcRf



jni修改后的代码，可以直接ndk-build出so文件！！！！！！！！



-------------------------------------------------------



webrtc代码在android和chromium项目中都有。但是android中的那个带有Android.mk，稍微修改下就能用ndk-build编译出libwebrtc_audio_preprocessing.so




git clone [https://android.googlesource.com/platform/external/webrtc](https://android.googlesource.com/platform/external/webrtc)



 更新

cd webrtc

![](file:///D:/My%20Documents/youdaonote/elesos@163.com/c12d8f59ed4b4277ae96fb82ae86b4b8/clipboard.png)







apm几个模块：

NS（Noise Suppression 噪声抑制）、

VAD（Voice Activity Detection 静音检测）、

AECM（Acoustic Echo Canceller for Mobile 声学回声消除）

AGC（Auto Gain Control 自动增益控制）





刚开始目录：

![](file:///D:/My%20Documents/youdaonote/elesos@163.com/90210e24d7d84457bd23fd3fe57f9635/clipboard.png)

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

![](file:///D:/My%20Documents/youdaonote/elesos@163.com/c50b5995fdb242e0839805816e484d45/clipboard.png)

test目录删除，.git目录删除







最终效果：

![](file:///D:/My%20Documents/youdaonote/elesos@163.com/49cac9b00c1a4b66a5deb7b64f0c44dc/clipboard.png)


发现在modules\audio_processing\test目录下有unit_test.cc



ndk-build编译,几乎很少改动。即可生成libwebrtc_audio_preprocessing.so



===================================


![](file:///D:/My%20Documents/youdaonote/elesos@163.com/682a3719d8dc43c6ad7db5e5d91895d9/clipboard.png)

上面错误解决：需要将源代码放在一个叫jni的目录中。







![](file:///D:/My%20Documents/youdaonote/elesos@163.com/2bdeab8686584598b7ef2324fbf48f1d/clipboard.png)

解决：



![](file:///D:/My%20Documents/youdaonote/elesos@163.com/02136bcc926543339a322fa4f4b7a256/clipboard.png)








#include <map>报错



由于 NDK 默认使用最小 C++ 运行时库进行项目的编译，导致无法编译 WebRTC 中使用诸如std::map 等 STL 容器的源码。因此我们需要自行设定适合本项目的 C++ 运行时库 gnustl_static。

解决：

Application.mk

APP_ABI := armeabi-v7a

APP_STL := gnustl_static

APP_PLATFORM :=21







![](file:///D:/My%20Documents/youdaonote/elesos@163.com/2d0ea5aee24845b0ac5c7d13693e4668/clipboard.png)

版本问题，


好像webrtc对[Android](http://lib.csdn.net/base/android)的版本要求过高，至少要求2.3.1，所以从原来的2.2换成2.3.1即可

 解决办法：

#APP_PLATFORM := [android](http://lib.csdn.net/base/android)-8     //对应2.2

APP_PLATFORM := android-9     //对应2.3.1
#APP_STL := stlport_static 
APP_STL := gnustl_static





2.


![](file:///D:/My%20Documents/youdaonote/elesos@163.com/620ea2561325438fbcd5906922542af1/clipboard.png)



undefined reference to 'WebRtcSpl_SqrtFloor'





这个函数在spl_sqrt_floor.c，在


webrtc\common_audio\signal_processing\Android.mk中的LOCAL_SRC_FILES加上

spl_sqrt_floor.c \






生成的so

![](file:///D:/My%20Documents/youdaonote/elesos@163.com/320cd3c74751406e88b8a160acd3b698/clipboard.png)

附:



![Application.mk](file:///D:/My%20Documents/youdaonote/elesos@163.com/e731169871f14fc7aed2dd027028cd07/attachment.png?1480747466948)![Android.mk](file:///D:/My%20Documents/youdaonote/elesos@163.com/35325556f10f4131a4d411085aed09a4/attachment.png?1480747466950)






参考：

[https://github.com/webrtc/samples](https://github.com/webrtc/samples)












