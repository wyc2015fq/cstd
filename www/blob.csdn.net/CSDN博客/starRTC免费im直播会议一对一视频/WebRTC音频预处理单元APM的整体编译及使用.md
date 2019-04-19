# WebRTC音频预处理单元APM的整体编译及使用 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年12月08日 14:27:57[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1774

正文
然而本次优化仍然没能用上整套VoE，因为VoE不仅仅包含音频预处理，它将音频编码模块、传输模块一并融入了引擎，而bill的项目需要使用既有的编码、传输层，因此使用整个VoE对我来说显得冗余且不可操作。天无绝人之路，抛开VoE不谈，bill找到了仅次于VoE层级的模块
 —— APM（Audio Preprocessing Module）—— 一个整合了前文所有模块且纯粹的音频预处理单元。
Step
 1 - 下载Google WebRTC源码
Google
 WebRTC的开发进度还是可观的，本文将以WebRTC的最新trunk
 revision 5125为例进行讲解。请自行使用SVN同步以下目录（至于同步的方法，请自行google）：
[http://webrtc.googlecode.com/svn/trunk/](http://webrtc.googlecode.com/svn/trunk/)
Step
 2 - 提取编译APM所需资源
APM的整体编译需要WebRTC源码目录下的如下资源：
1）common_audio 整个目录
2）modules 目录（不包含 video 部分）
3）system_wrappers 整个目录
4）位于 WebRTC 源码根目录下的 common_types.h
 | common.h | typedefs.h 三个头文件。
5）位于 WebRTC 主目录下的 android-webrtc.mk 文件。
Step
 3 - 在Eclipse中编译APM基础步骤及部分要点
对于Eclipse中jni的编译及使用请参见[上篇文章](http://billhoo.blog.51cto.com/2337751/1213801)所述，在此不再赘述。
此节仅按照自己的jni目录组织结构进行讲解，读者可根据自己需要进行调整。
在Eclipse中的jni组织结构如下：
![](file:///D:/My%20Documents/youdaonote/elesos@163.com/6563a4d1b7a84e30a44cc3ccdcc3d2bd/102738855.png.gif)
Step-2中的所有文件夹及头文件均位于 webrtc 子目录下。android-webrtc.mk 位于 jni 根目录下。
下面我们逐步进行分解：
step 3.1
首先我们需要对整个 android 工程进行描述和设定，打开 jni 根目录下的 Application.mk 文件，编辑如下：
|123456789101112|```# Copyright (c) 2013 BillHoo. All Rights Reserved.``````# Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.````#````# Use of this source code is governed by a BSD-style license``````# that can be found in the LICENSE file in the root of the source``````# tree. An additional intellectual property rights grant can be found``````# in the file PATENTS.  All contributing project authors may``````# be found in the AUTHORS file in the root of the source tree.``````APP_STL :=````gnustl_static````APP_CPPFLAGS := -frtti -fexceptions``````APP_ABI := armeabi armeabi-v7a``````APP_PLATFORM := android-9```|
其中 APP_STL 的官方说明如下：
|1234567891011|`APP_STL``    ````By default, the NDK build system provides C++ headers for the minimal````    ````C++ runtime library (/system/lib/libstdc++.so) provided by the Android````    ``system.``    ````However, the NDK comes with alternative C++ implementations that you can````    ````use or link to in your own applications. Define APP_STL to select one of````    ````them. Examples are:````       ````APP_STL := stlport_static    --> static STLport library````       ````APP_STL := stlport_shared    --> shared STLport library````       ````APP_STL := system            --> default C++ runtime library````    ````For more information on the subject, please read docs/CPLUSPLUS-SUPPORT.html```|
由于 NDK默认使用最小 C++ 运行时库进行项目的编译，导致无法编译 WebRTC 中使用诸如 std::map 等 STL 容器的源码。因此我们需要自行设定适合本项目的 C++ 运行时库gnustl_static。
step 3.2
打开并编辑 jni 根目录下的 Android.mk 文件如下，本文件只需告诉 NDK 去调用所有子目录下的 Android.mk 文件即可：
|123456789|```# Copyright (c) 2013 BillHoo. All Rights Reserved.``````# Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.````#````# Use of this source code is governed by a BSD-style license``````# that can be found in the LICENSE file in the root of the source``````# tree. An additional intellectual property rights grant can be found``````# in the file PATENTS.  All contributing project authors may``````# be found in the AUTHORS file in the root of the source tree.``````include $(call all-subdir-makefiles)```|
step 3.3
准备工作就绪，下面就可以开始着手编译整个 APM 单元了，首先打开 jni/webrtc 目录，新建 Android.mk 文件如下：
|1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859606162636465|```# Copyright (c) 2013 BillHoo. All Rights Reserved.``````# Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.````#````# Use of this source code is governed by a BSD-style license``````# that can be found in the LICENSE file in the root of the source``````# tree. An additional intellectual property rights grant can be found``````# in the file PATENTS.  All contributing project authors may``````# be found in the AUTHORS file in the root of the source tree.````#````MY_WEBRTC_ROOT_PATH := $(call my-````dir``)``#````# voice``````include $(MY_WEBRTC_ROOT_PATH)````/common_audio/signal_processing/Android``.mk````include $(MY_WEBRTC_ROOT_PATH)````/common_audio/vad/Android``.mk````include $(MY_WEBRTC_ROOT_PATH)````/modules/audio_processing/aec/Android``.mk````include $(MY_WEBRTC_ROOT_PATH)````/modules/audio_processing/aecm/Android``.mk````include $(MY_WEBRTC_ROOT_PATH)````/modules/audio_processing/agc/Android``.mk````include $(MY_WEBRTC_ROOT_PATH)````/modules/audio_processing/Android``.mk````include $(MY_WEBRTC_ROOT_PATH)````/modules/audio_processing/ns/Android``.mk````include $(MY_WEBRTC_ROOT_PATH)````/modules/audio_processing/utility/Android``.mk````include $(MY_WEBRTC_ROOT_PATH)````/modules/utility/source/Android``.mk````include $(MY_WEBRTC_ROOT_PATH)````/modules/audio_device/Android``.mk````include $(MY_WEBRTC_ROOT_PATH)````/system_wrappers/source/Android``.mk``#````# build .so``````LOCAL_PATH := $(call my-````dir``)````include $(CLEAR_VARS)``````LOCAL_ARM_MODE := arm``````LOCAL_MODULE := lib````lu``_audio_preprocessing````LOCAL_MODULE_TAGS := optional``````LOCAL_WHOLE_STATIC_LIBRARIES := \````    ````libwebrtc_spl \````    ````libwebrtc_apm \````    ````libwebrtc_apm_utility \````    ````libwebrtc_vad \````    ````libwebrtc_ns \````    ````libwebrtc_agc \````    ````libwebrtc_aec \````    ````libwebrtc_aecm \````    ````libwebrtc_system_wrappers \````    ````libwebrtc_audio_device \````    ``libwebrtc_utility``#````# Add Neon libraries.``````ifeq ($(WEBRTC_BUILD_NEON_LIBS),````true``)````LOCAL_WHOLE_STATIC_LIBRARIES += \````    ````libwebrtc_aecm_neon \````    ````libwebrtc_ns_neon \````    ``libwebrtc_spl_neon``endif````LOCAL_STATIC_LIBRARIES := \````    ``libprotobuf-cpp-2.3.0-lite````LOCAL_SHARED_LIBRARIES := \````    ````libcutils \````    ````libdl \````    ``libstlport````LOCAL_PRELINK_MODULE :=````false``#````#TODO(billhoo) find a properway to do this.``````LOCAL_LDLIBS += $(NDK_ROOT)````/sources/cxx-stl/gnu-libstdc``++``/4``.6``/libs/armeabi/libgnustl_static``.a````LOCAL_LDLIBS += -lOpenSLES``````ifndef NDK_ROOT``````include external````/stlport/libstlport``.mk``endif````include $(BUILD_SHARED_LIBRARY)```|
需要注意的几点：
1）在编译时如提示找不到 ../../../Android.mk 文件等错误，请检查并修正你的相对路径。
2）位于第60行的gnu静态库链接路径是针对NDK版本 r8d 的，如读者版本不匹配，请自行找到 libgnustl_static.a 静态库的路径进行替换。
3）本示例并不打算编译 WebRTC 的测试工程，请使用 Eclipse 搜索文件功能，找到 Android.mk 文件中的 -DWEBRTC_AUDIOPROC_DEBUG_DUMP 并注释掉。
step 3.4
万事俱备，我们可以开始编译 APM 了，不过在编译过程中肯定还会有很多小问题出现（比如相对路径不正确、找不到某某函数的符号等等），这些问题就留给读者自行google、SO解决了，bill就不再赘述。
Step
 4 - 在android应用中使用APM的注意事项
经过上述步骤，读者便能够得到 libwebrtc_audio_preprocessing.so这个动态链接库。我们需要做的仅仅是编写自己的 jni 包装函数向 android 应用层提供 APM 的接口。具体做法bill[之前的文章](http://billhoo.blog.51cto.com/2337751/1125039)已经详细介绍过。这里需要注意的是，如果读者打算在自己的动态库中引用已经编译好的 APM 库，那么在 android 类加载这两个库时的顺序是敏感的。
假设读者将自己的 JNI 接口封装成单独的库 libmy_jni_wrapper.so，而该库引用了 libwebrtc_audio_preprocessing.so，那么在加载这两个库时应该参照如下顺序：
|12345|`static``{``    ````// Ordering of loading these shared libraries is significant.````    ``System.loadLibrary(``"webrtc_audio_preprocessing"``);``    ``System.loadLibrary(``"my_jni_wrapper"``);``}`|
若顺序写反，在运行时将得到找不到 webrtc_audio_preprocessing 库中符号的异常。
总结
整个编译工作在现在看来非常简单，但需要很多的耐心和搜索，不过结果还是令人比较满意的，APM出来的效果比之前自己单独使用各个音频模块要好很多。不过对于抖动等因素的影响，APM就力不从心了。也许bill接下来该花时间去看看NetEq、Jitter
 Buffer等模块了。如何使用他们，如何融进自己的项目，到时候就知道了。
出处[http://billhoo.blog.51cto.com/2337751/1325273](http://billhoo.blog.51cto.com/2337751/1325273)

[3楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/e3ea83c141b849359703a1629614a502/reply.gif)[huangqizhen](http://8269104.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2013-12-09 17:59:42
博主您好，看您的文章很有启发。
参看了您的博文之后，关于回音处理部分我的项目也基本完成了，正像您所说的，在网络比较差的时候，由于延时与抖动，和丢包所引起的质量下降。而我的下一步也是想把NetEQ使用自己的项目中。如果您有什么进展的话，期望能和您一起讨论讨论。
[5楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/6119ee208ba5425dab1e299cba2677dd/reply.gif)[liusimon](http://8581350.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-02-13 15:20:53
编译连接出错求指点E:/cygwin/android-ndk-r8d/toolchains/arm-linux-androideabi-4.6
bin/../lib/gcc/arm-linux-androideabi/4.6/../../../../arm-linux
d.exe: E:/cygwin/home/simon/WebRTC/obj/local/armeabi/libwebrtc
dio_device_opensles_android.o): in function webrtc::AudioDevic
MicrophoneIsAvailable(bool&):E:/cygwin/home/simon/WebRTC/jni/w
o_device/android/audio_device_opensles_android.cc:167: error:
e to 'webrtc::OpenSlesInput::MicrophoneIsAvailable(bool&)'
[7楼][楼主](http://billhoo.blog.51cto.com/)![](file:///D:/My%20Documents/youdaonote/elesos@163.com/2ca74893786c4c3c972308f86703c8e9/reply.gif)[Bill_Hoo](http://billhoo.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-02-14 20:37:45
回复 it206:[[4楼]](http://billhoo.blog.51cto.com/#)
您好it206，
最近没有倒腾android上的回声消除了，效果和之前是一样的，好的时候很好，差的时候总能听到唧唧声，很不爽。现在效果很不完美。关于apk，那是团队的成果了，我没有权力拿出来分享，博客里我已经把自己能分享的都分享给大家了，忘海涵。
[8楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f492b281c1cb42c9ba6eeef810c0f588/reply.gif)[it206](http://2634871.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-02-17 16:01:38
碰到的情况和博主一样，偶尔会出现唧唧声，感觉在一些麦克和话筒距离比较远的手机上效果好，有些手机麦克和话筒紧靠着，一直有比较大唧唧声，效果很差
[9楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/ab6c8315933d476892d7df8040d640fc/reply.gif)[liusimon](http://8581350.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-02-26 16:20:21
你好，请教一下，APM具体怎么使用，可以分享一下吗，处理第三方的音频数据
[10楼][楼主](http://billhoo.blog.51cto.com/)![](file:///D:/My%20Documents/youdaonote/elesos@163.com/92d8ac8450144905bd7ea285a7471ea8/reply.gif)[Bill_Hoo](http://billhoo.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-03-20 16:18:53
回复 liusimon:[[9楼]](http://billhoo.blog.51cto.com/#)
您好，APM各个接口的使用方法在其头文件 audio_processing.h 中描述的很清楚。android上由于没有控制MIC音量的接口，故APM中的AGC不要使用
 kAdaptiveAnalog 模式。
[11楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/5351c1bb61154bd295fa06aaf9ac394e/reply.gif)[菜鸟阿陈](http://8947348.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-05-19 10:17:59
您好！Bill_Hoo.非常感谢您分享的这片博客。我也正在编译中，可是遇到一个错误提示：webrtc/system_wrappers/source/cpu_features_android.c:11:26: error: cpu-features.h: No such file or directory；我想问下您编译的时候有遇到？我在网上没找到相关错误解答，所以想在这里，向Bill_Hoo,提起，这问题应该如何解决呢。
[12楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/8cccb6142f17496da43a0ff7f5ae4db4/reply.gif)[菜鸟阿陈](http://8947348.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-05-19 11:47:27
您好！Bill_Hoo.关于上面编译遇到错误提示：一个错误提示：webrtc/system_wrappers/source/cpu_features_android.c:11:26: error: cpu-features.h: No such file or directory
，我查找了，在webrtc/system_wrappers/source/Android.mk 文件中添加了$(call import-module,android/cpufeatures)
，可以编译通过这个错误了，这是正确解决方法？另外编译system_wrappers/source/Android.mk，又遇到提示一个错误：system_wrappers/source/spreadsortlib/spreadsort.hpp:1034:139: error: macro "getchar" passed 2 arguments, but takes just 0；这个真心不知道什么错误，很想问，Bill_Hoo,你在编译过程，又遇到这些错误？这我应该如何解决呢，有点困惑。
[13楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/4481daa02bc249ffa80bea18c1a664d7/reply.gif)[techLong](http://4682583.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-05-20 15:09:22
请教下啊！为什么我这里通过ctrl点击能够找到相应的头文件，可是编译的时候就显示fatal error,no such file呢！！！
[14楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/502a916138d4407599bf654cab9b42de/reply.gif)[liuliu886](http://5553116.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-12-16 22:11:08
感谢分享 请问有在iso用过此模块吗 ？
[15楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/317256389733456885d61870294d0c74/reply.gif)[liuliu886](http://5553116.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-12-16 22:11:49
打错了 应该是 iOS
[16楼][楼主](http://billhoo.blog.51cto.com/)![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f98d5a9d9d3940e780ca1f3c1c900e4f/reply.gif)[Bill_Hoo](http://billhoo.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-12-17 09:11:05
回复 liuliu886:[[14楼]](http://billhoo.blog.51cto.com/#)
你好，IOS 上仅使用了 NS降噪 和 NetEQ 抖动缓冲，回声消除交由其自身的 API 实现更加合适。
[17楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f98d5a9d9d3940e780ca1f3c1c900e4f/reply.gif)[liuliu886](http://5553116.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-12-17 15:32:09
感谢回复 我发现博主以前在Stack Overflow上曾分享过一个示例代码
https://github.com/billhoo/webrtc-based-android-aecm
但是现在已经无法下载了 可否给我一份作为研究 不胜感激 71892967@qq.com
[18楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f98d5a9d9d3940e780ca1f3c1c900e4f/reply.gif)[liuliu886](http://5553116.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-12-17 15:45:15
另外关于安卓下录放音，除了两个Java的类以外，还可以用ndk层c＋＋的OpenSL，目前我的程序是用它（SLRecordItf）录音的。
它不是用AudioRecord.read这种阻塞式函数读取录到的数据，
而是和ios中AudioQueue类似的填满一个缓冲区即回调的方式，我想这样是不是更容易计算延迟一些呢？
[19楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f98d5a9d9d3940e780ca1f3c1c900e4f/reply.gif)[tanningzhong](http://3542172.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-12-27 14:22:37
楼主好，我编译的时候提示很多这一类的错误，找不到头文件的情况，但是在mk文件中路径是已经添加进去了的
[armeabi] Compile arm   : webrtc_spl <= auto_corr_to_refl_coef.c
jni/webrtc/common_audio/signal_processing/auto_corr_to_refl_coef.c:17:85: fatal error: webrtc/common_audio/signal_processing/include/signal_processing_library.h: No such file or directory
[20楼][楼主](http://billhoo.blog.51cto.com/)![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f98d5a9d9d3940e780ca1f3c1c900e4f/reply.gif)[Bill_Hoo](http://billhoo.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-12-30 09:04:18
回复 tanningzhong:[[19楼]](http://billhoo.blog.51cto.com/#)
您好，根据你的错误路径提示，编译 auto_corr_to_refl_coef.c 的 Android.mk 至少应该包含路径：
“$(LOCAL_PATH)/../../../..”，即目录“jni”。
也就是说，你需要让编译器能够找到根目录 “webrtc”
[21楼][楼主](http://billhoo.blog.51cto.com/)![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f98d5a9d9d3940e780ca1f3c1c900e4f/reply.gif)[Bill_Hoo](http://billhoo.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-12-30 09:06:15
回复 liuliu886:[[17楼]](http://billhoo.blog.51cto.com/#)
您好，GitHub上的示例我已经删除了，那是当时自己最开始自以为是的认为手机上的回声消除很简单，所以有了一个直接使用AECM对静态音频文件进行消除的例子。但实际上VoIP和静态文件是两码事。为避免误导他人，故已删除。
[22楼][楼主](http://billhoo.blog.51cto.com/)![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f98d5a9d9d3940e780ca1f3c1c900e4f/reply.gif)[Bill_Hoo](http://billhoo.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-12-30 09:09:55
回复 liuliu886:[[18楼]](http://billhoo.blog.51cto.com/#)
就个人经验而言，Java层的 AudioTrack 以及 AudioRecord 实在不适合（未考虑最新版本的Android）做所谓的“延迟估算”。OpenSLES 我没有实际测试过，不敢妄言。
[23楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f98d5a9d9d3940e780ca1f3c1c900e4f/reply.gif)[zhu4272](http://6068431.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2015-01-09 16:09:17
bill,你好，通过你这篇文章的帮助,我成功编译出了libwebrtc_audio_preprocessing.so，但是接下来不知道如何整体地区使用他，请问有相关的资料可以借鉴吗？
[24楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f98d5a9d9d3940e780ca1f3c1c900e4f/reply.gif)[zhu4272](http://6068431.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2015-05-27 14:25:19
回复 Bill_Hoo:[[16楼]](http://billhoo.blog.51cto.com/#)
回复 liuliu886:
[14楼]
你好，IOS 上仅使用了 NS降噪 和 NetEQ 抖动缓冲，回声消除交由其自身的 API 实现更加合适。
你好,bill。请问IOS具体哪个API可以用来消除回声呢？
[25楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f98d5a9d9d3940e780ca1f3c1c900e4f/reply.gif)[wwwlookycc](http://10443421.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2015-06-19 09:52:37
楼主 ，我们也在研究 webrtc， 能否求指教
[26楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f98d5a9d9d3940e780ca1f3c1c900e4f/reply.gif)[hnraysir](http://hnraysir.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2015-08-28 18:20:58
我以最少改动，编译出来了so文件，欢迎大家参考，现在就差apm整体模块的使用了，哪个有apm的使用方法的，欢迎通知我哈。目前只发现在modules\audio_processing\test目录下有unit_test.cc，等我研究好其使用了再详细记录下。
http://wiki.elesos.com/index.php?title=WebRTC%E7%B3%BB%E5%88%974:%E9%9F%B3%E9%A2%91%E5%A4%84%E7%90%86%E6%A8%A1%E5%9D%97apm%E4%B8%8B%E8%BD%BD%E4%B8%8E%E6%95%B4%E4%BD%93%E7%BC%96%E8%AF%91

**正文**
然而本次优化仍然没能用上整套VoE，因为VoE不仅仅包含音频预处理，它将音频编码模块、传输模块一并融入了引擎，而bill的项目需要使用既有的编码、传输层，因此使用整个VoE对我来说显得冗余且不可操作。天无绝人之路，抛开VoE不谈，bill找到了仅次于VoE层级的模块
 —— APM（Audio Preprocessing Module）—— 一个整合了前文所有模块且纯粹的音频预处理单元。
**Step 1 - 下载Google WebRTC源码**
Google
 WebRTC的开发进度还是可观的，本文将以WebRTC的最新trunk
 revision 5125为例进行讲解。请自行使用SVN同步以下目录（至于同步的方法，请自行google）：
[http://webrtc.googlecode.com/svn/trunk/](http://webrtc.googlecode.com/svn/trunk/)
**Step 2 - 提取编译APM所需资源**
APM的整体编译需要WebRTC源码目录下的如下资源：
1）common_audio 整个目录
2）modules 目录（不包含 video 部分）
3）system_wrappers 整个目录
4）位于 WebRTC 源码根目录下的 common_types.h
 | common.h | typedefs.h 三个头文件。
5）位于 WebRTC 主目录下的 android-webrtc.mk 文件。
**Step 3 - 在Eclipse中编译APM基础步骤及部分要点**
对于Eclipse中jni的编译及使用请参见[上篇文章](http://billhoo.blog.51cto.com/2337751/1213801)所述，在此不再赘述。
此节仅按照自己的jni目录组织结构进行讲解，读者可根据自己需要进行调整。
在Eclipse中的jni组织结构如下：
![](file:///D:/My%20Documents/youdaonote/elesos@163.com/6563a4d1b7a84e30a44cc3ccdcc3d2bd/102738855.png.gif)
Step-2中的所有文件夹及头文件均位于 webrtc 子目录下。android-webrtc.mk 位于 jni 根目录下。
下面我们逐步进行分解：
step 3.1
首先我们需要对整个 android 工程进行描述和设定，打开 jni 根目录下的 Application.mk 文件，编辑如下：
|123456789101112|```# Copyright (c) 2013 BillHoo. All Rights Reserved.``````# Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.````#````# Use of this source code is governed by a BSD-style license``````# that can be found in the LICENSE file in the root of the source``````# tree. An additional intellectual property rights grant can be found``````# in the file PATENTS.  All contributing project authors may``````# be found in the AUTHORS file in the root of the source tree.``````APP_STL :=````gnustl_static````APP_CPPFLAGS := -frtti -fexceptions``````APP_ABI := armeabi armeabi-v7a``````APP_PLATFORM := android-9```|
其中 APP_STL 的官方说明如下：
|1234567891011|`APP_STL``    ````By default, the NDK build system provides C++ headers for the minimal````    ````C++ runtime library (/system/lib/libstdc++.so) provided by the Android````    ``system.``    ````However, the NDK comes with alternative C++ implementations that you can````    ````use or link to in your own applications. Define APP_STL to select one of````    ````them. Examples are:````       ````APP_STL := stlport_static    --> static STLport library````       ````APP_STL := stlport_shared    --> shared STLport library````       ````APP_STL := system            --> default C++ runtime library````    ````For more information on the subject, please read docs/CPLUSPLUS-SUPPORT.html```|
由于 NDK默认使用最小 C++ 运行时库进行项目的编译，导致无法编译 WebRTC 中使用诸如 std::map 等 STL 容器的源码。因此我们需要自行设定适合本项目的 C++ 运行时库gnustl_static。
step 3.2
打开并编辑 jni 根目录下的 Android.mk 文件如下，本文件只需告诉 NDK 去调用所有子目录下的 Android.mk 文件即可：
|123456789|```# Copyright (c) 2013 BillHoo. All Rights Reserved.``````# Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.````#````# Use of this source code is governed by a BSD-style license``````# that can be found in the LICENSE file in the root of the source``````# tree. An additional intellectual property rights grant can be found``````# in the file PATENTS.  All contributing project authors may``````# be found in the AUTHORS file in the root of the source tree.``````include $(call all-subdir-makefiles)```|
step 3.3
准备工作就绪，下面就可以开始着手编译整个 APM 单元了，首先打开 jni/webrtc 目录，新建 Android.mk 文件如下：
|1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859606162636465|```# Copyright (c) 2013 BillHoo. All Rights Reserved.``````# Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.````#````# Use of this source code is governed by a BSD-style license``````# that can be found in the LICENSE file in the root of the source``````# tree. An additional intellectual property rights grant can be found``````# in the file PATENTS.  All contributing project authors may``````# be found in the AUTHORS file in the root of the source tree.````#````MY_WEBRTC_ROOT_PATH := $(call my-````dir``)``#````# voice``````include $(MY_WEBRTC_ROOT_PATH)````/common_audio/signal_processing/Android``.mk````include $(MY_WEBRTC_ROOT_PATH)````/common_audio/vad/Android``.mk````include $(MY_WEBRTC_ROOT_PATH)````/modules/audio_processing/aec/Android``.mk````include $(MY_WEBRTC_ROOT_PATH)````/modules/audio_processing/aecm/Android``.mk````include $(MY_WEBRTC_ROOT_PATH)````/modules/audio_processing/agc/Android``.mk````include $(MY_WEBRTC_ROOT_PATH)````/modules/audio_processing/Android``.mk````include $(MY_WEBRTC_ROOT_PATH)````/modules/audio_processing/ns/Android``.mk````include $(MY_WEBRTC_ROOT_PATH)````/modules/audio_processing/utility/Android``.mk````include $(MY_WEBRTC_ROOT_PATH)````/modules/utility/source/Android``.mk````include $(MY_WEBRTC_ROOT_PATH)````/modules/audio_device/Android``.mk````include $(MY_WEBRTC_ROOT_PATH)````/system_wrappers/source/Android``.mk``#````# build .so``````LOCAL_PATH := $(call my-````dir``)````include $(CLEAR_VARS)``````LOCAL_ARM_MODE := arm``````LOCAL_MODULE := lib````lu``_audio_preprocessing````LOCAL_MODULE_TAGS := optional``````LOCAL_WHOLE_STATIC_LIBRARIES := \````    ````libwebrtc_spl \````    ````libwebrtc_apm \````    ````libwebrtc_apm_utility \````    ````libwebrtc_vad \````    ````libwebrtc_ns \````    ````libwebrtc_agc \````    ````libwebrtc_aec \````    ````libwebrtc_aecm \````    ````libwebrtc_system_wrappers \````    ````libwebrtc_audio_device \````    ``libwebrtc_utility``#````# Add Neon libraries.``````ifeq ($(WEBRTC_BUILD_NEON_LIBS),````true``)````LOCAL_WHOLE_STATIC_LIBRARIES += \````    ````libwebrtc_aecm_neon \````    ````libwebrtc_ns_neon \````    ``libwebrtc_spl_neon``endif````LOCAL_STATIC_LIBRARIES := \````    ``libprotobuf-cpp-2.3.0-lite````LOCAL_SHARED_LIBRARIES := \````    ````libcutils \````    ````libdl \````    ``libstlport````LOCAL_PRELINK_MODULE :=````false``#````#TODO(billhoo) find a properway to do this.``````LOCAL_LDLIBS += $(NDK_ROOT)````/sources/cxx-stl/gnu-libstdc``++``/4``.6``/libs/armeabi/libgnustl_static``.a````LOCAL_LDLIBS += -lOpenSLES``````ifndef NDK_ROOT``````include external````/stlport/libstlport``.mk``endif````include $(BUILD_SHARED_LIBRARY)```|
需要注意的几点：
1）在编译时如提示找不到 ../../../Android.mk 文件等错误，请检查并修正你的相对路径。
2）位于第60行的gnu静态库链接路径是针对NDK版本 r8d 的，如读者版本不匹配，请自行找到 libgnustl_static.a 静态库的路径进行替换。
3）本示例并不打算编译 WebRTC 的测试工程，请使用 Eclipse 搜索文件功能，找到 Android.mk 文件中的 -DWEBRTC_AUDIOPROC_DEBUG_DUMP 并注释掉。
step 3.4
万事俱备，我们可以开始编译 APM 了，不过在编译过程中肯定还会有很多小问题出现（比如相对路径不正确、找不到某某函数的符号等等），这些问题就留给读者自行google、SO解决了，bill就不再赘述。
**Step 4 - 在android应用中使用APM的注意事项**
经过上述步骤，读者便能够得到 libwebrtc_audio_preprocessing.so这个动态链接库。我们需要做的仅仅是编写自己的 jni 包装函数向 android 应用层提供 APM 的接口。具体做法bill[之前的文章](http://billhoo.blog.51cto.com/2337751/1125039)已经详细介绍过。这里需要注意的是，如果读者打算在自己的动态库中引用已经编译好的 APM 库，那么在 android 类加载这两个库时的顺序是敏感的。
假设读者将自己的 JNI 接口封装成单独的库 libmy_jni_wrapper.so，而该库引用了 libwebrtc_audio_preprocessing.so，那么在加载这两个库时应该参照如下顺序：
|12345|`static``{``    ````// Ordering of loading these shared libraries is significant.````    ``System.loadLibrary(``"webrtc_audio_preprocessing"``);``    ``System.loadLibrary(``"my_jni_wrapper"``);``}`|
若顺序写反，在运行时将得到找不到 webrtc_audio_preprocessing 库中符号的异常。
**总结**
整个编译工作在现在看来非常简单，但需要很多的耐心和搜索，不过结果还是令人比较满意的，APM出来的效果比之前自己单独使用各个音频模块要好很多。不过对于抖动等因素的影响，APM就力不从心了。也许bill接下来该花时间去看看NetEq、Jitter
 Buffer等模块了。如何使用他们，如何融进自己的项目，到时候就知道了。
出处[http://billhoo.blog.51cto.com/2337751/1325273](http://billhoo.blog.51cto.com/2337751/1325273)

[3楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/e3ea83c141b849359703a1629614a502/reply.gif)[**huangqizhen**](http://8269104.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2013-12-09 17:59:42
博主您好，看您的文章很有启发。
参看了您的博文之后，关于回音处理部分我的项目也基本完成了，正像您所说的，在网络比较差的时候，由于延时与抖动，和丢包所引起的质量下降。而我的下一步也是想把NetEQ使用自己的项目中。如果您有什么进展的话，期望能和您一起讨论讨论。
[5楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/6119ee208ba5425dab1e299cba2677dd/reply.gif)[**liusimon**](http://8581350.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-02-13 15:20:53
编译连接出错求指点E:/cygwin/android-ndk-r8d/toolchains/arm-linux-androideabi-4.6
bin/../lib/gcc/arm-linux-androideabi/4.6/../../../../arm-linux
d.exe: E:/cygwin/home/simon/WebRTC/obj/local/armeabi/libwebrtc
dio_device_opensles_android.o): in function webrtc::AudioDevic
MicrophoneIsAvailable(bool&):E:/cygwin/home/simon/WebRTC/jni/w
o_device/android/audio_device_opensles_android.cc:167: error:
e to 'webrtc::OpenSlesInput::MicrophoneIsAvailable(bool&)'
[7楼][楼主](http://billhoo.blog.51cto.com/)![](file:///D:/My%20Documents/youdaonote/elesos@163.com/2ca74893786c4c3c972308f86703c8e9/reply.gif)[**Bill_Hoo**](http://billhoo.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-02-14 20:37:45
回复 it206:[[4楼]](http://billhoo.blog.51cto.com/#)
您好it206，
最近没有倒腾android上的回声消除了，效果和之前是一样的，好的时候很好，差的时候总能听到唧唧声，很不爽。现在效果很不完美。关于apk，那是团队的成果了，我没有权力拿出来分享，博客里我已经把自己能分享的都分享给大家了，忘海涵。
[8楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f492b281c1cb42c9ba6eeef810c0f588/reply.gif)[**it206**](http://2634871.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-02-17 16:01:38
碰到的情况和博主一样，偶尔会出现唧唧声，感觉在一些麦克和话筒距离比较远的手机上效果好，有些手机麦克和话筒紧靠着，一直有比较大唧唧声，效果很差
[9楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/ab6c8315933d476892d7df8040d640fc/reply.gif)[**liusimon**](http://8581350.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-02-26 16:20:21
你好，请教一下，APM具体怎么使用，可以分享一下吗，处理第三方的音频数据
[10楼][楼主](http://billhoo.blog.51cto.com/)![](file:///D:/My%20Documents/youdaonote/elesos@163.com/92d8ac8450144905bd7ea285a7471ea8/reply.gif)[**Bill_Hoo**](http://billhoo.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-03-20 16:18:53
回复 liusimon:[[9楼]](http://billhoo.blog.51cto.com/#)
您好，APM各个接口的使用方法在其头文件 audio_processing.h 中描述的很清楚。android上由于没有控制MIC音量的接口，故APM中的AGC不要使用
 kAdaptiveAnalog 模式。
[11楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/5351c1bb61154bd295fa06aaf9ac394e/reply.gif)[**菜鸟阿陈**](http://8947348.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-05-19 10:17:59
您好！Bill_Hoo.非常感谢您分享的这片博客。我也正在编译中，可是遇到一个错误提示：webrtc/system_wrappers/source/cpu_features_android.c:11:26: error: cpu-features.h: No such file or directory；我想问下您编译的时候有遇到？我在网上没找到相关错误解答，所以想在这里，向Bill_Hoo,提起，这问题应该如何解决呢。
[12楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/8cccb6142f17496da43a0ff7f5ae4db4/reply.gif)[**菜鸟阿陈**](http://8947348.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-05-19 11:47:27
您好！Bill_Hoo.关于上面编译遇到错误提示：一个错误提示：webrtc/system_wrappers/source/cpu_features_android.c:11:26: error: cpu-features.h: No such file or directory
，我查找了，在webrtc/system_wrappers/source/Android.mk 文件中添加了$(call import-module,android/cpufeatures)
，可以编译通过这个错误了，这是正确解决方法？另外编译system_wrappers/source/Android.mk，又遇到提示一个错误：system_wrappers/source/spreadsortlib/spreadsort.hpp:1034:139: error: macro "getchar" passed 2 arguments, but takes just 0；这个真心不知道什么错误，很想问，Bill_Hoo,你在编译过程，又遇到这些错误？这我应该如何解决呢，有点困惑。
[13楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/4481daa02bc249ffa80bea18c1a664d7/reply.gif)[**techLong**](http://4682583.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-05-20 15:09:22
请教下啊！为什么我这里通过ctrl点击能够找到相应的头文件，可是编译的时候就显示fatal error,no such file呢！！！
[14楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/502a916138d4407599bf654cab9b42de/reply.gif)[**liuliu886**](http://5553116.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-12-16 22:11:08
感谢分享 请问有在iso用过此模块吗 ？
[15楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/317256389733456885d61870294d0c74/reply.gif)[**liuliu886**](http://5553116.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-12-16 22:11:49
打错了 应该是 iOS
[16楼][楼主](http://billhoo.blog.51cto.com/)![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f98d5a9d9d3940e780ca1f3c1c900e4f/reply.gif)[**Bill_Hoo**](http://billhoo.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-12-17 09:11:05
回复 liuliu886:[[14楼]](http://billhoo.blog.51cto.com/#)
你好，IOS 上仅使用了 NS降噪 和 NetEQ 抖动缓冲，回声消除交由其自身的 API 实现更加合适。
[17楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f98d5a9d9d3940e780ca1f3c1c900e4f/reply.gif)[**liuliu886**](http://5553116.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-12-17 15:32:09
感谢回复 我发现博主以前在Stack Overflow上曾分享过一个示例代码
https://github.com/billhoo/webrtc-based-android-aecm
但是现在已经无法下载了 可否给我一份作为研究 不胜感激 71892967@qq.com
[18楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f98d5a9d9d3940e780ca1f3c1c900e4f/reply.gif)[**liuliu886**](http://5553116.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-12-17 15:45:15
另外关于安卓下录放音，除了两个Java的类以外，还可以用ndk层c＋＋的OpenSL，目前我的程序是用它（SLRecordItf）录音的。
它不是用AudioRecord.read这种阻塞式函数读取录到的数据，
而是和ios中AudioQueue类似的填满一个缓冲区即回调的方式，我想这样是不是更容易计算延迟一些呢？
[19楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f98d5a9d9d3940e780ca1f3c1c900e4f/reply.gif)[**tanningzhong**](http://3542172.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-12-27 14:22:37
楼主好，我编译的时候提示很多这一类的错误，找不到头文件的情况，但是在mk文件中路径是已经添加进去了的
[armeabi] Compile arm   : webrtc_spl <= auto_corr_to_refl_coef.c
jni/webrtc/common_audio/signal_processing/auto_corr_to_refl_coef.c:17:85: fatal error: webrtc/common_audio/signal_processing/include/signal_processing_library.h: No such file or directory
[20楼][楼主](http://billhoo.blog.51cto.com/)![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f98d5a9d9d3940e780ca1f3c1c900e4f/reply.gif)[**Bill_Hoo**](http://billhoo.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-12-30 09:04:18
回复 tanningzhong:[[19楼]](http://billhoo.blog.51cto.com/#)
您好，根据你的错误路径提示，编译 auto_corr_to_refl_coef.c 的 Android.mk 至少应该包含路径：
“$(LOCAL_PATH)/../../../..”，即目录“jni”。
也就是说，你需要让编译器能够找到根目录 “webrtc”
[21楼][楼主](http://billhoo.blog.51cto.com/)![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f98d5a9d9d3940e780ca1f3c1c900e4f/reply.gif)[**Bill_Hoo**](http://billhoo.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-12-30 09:06:15
回复 liuliu886:[[17楼]](http://billhoo.blog.51cto.com/#)
您好，GitHub上的示例我已经删除了，那是当时自己最开始自以为是的认为手机上的回声消除很简单，所以有了一个直接使用AECM对静态音频文件进行消除的例子。但实际上VoIP和静态文件是两码事。为避免误导他人，故已删除。
[22楼][楼主](http://billhoo.blog.51cto.com/)![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f98d5a9d9d3940e780ca1f3c1c900e4f/reply.gif)[**Bill_Hoo**](http://billhoo.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2014-12-30 09:09:55
回复 liuliu886:[[18楼]](http://billhoo.blog.51cto.com/#)
就个人经验而言，Java层的 AudioTrack 以及 AudioRecord 实在不适合（未考虑最新版本的Android）做所谓的“延迟估算”。OpenSLES 我没有实际测试过，不敢妄言。
[23楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f98d5a9d9d3940e780ca1f3c1c900e4f/reply.gif)[**zhu4272**](http://6068431.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2015-01-09 16:09:17
bill,你好，通过你这篇文章的帮助,我成功编译出了libwebrtc_audio_preprocessing.so，但是接下来不知道如何整体地区使用他，请问有相关的资料可以借鉴吗？
[24楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f98d5a9d9d3940e780ca1f3c1c900e4f/reply.gif)[**zhu4272**](http://6068431.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2015-05-27 14:25:19
回复 Bill_Hoo:[[16楼]](http://billhoo.blog.51cto.com/#)
回复 liuliu886:
[14楼]
你好，IOS 上仅使用了 NS降噪 和 NetEQ 抖动缓冲，回声消除交由其自身的 API 实现更加合适。
你好,bill。请问IOS具体哪个API可以用来消除回声呢？
[25楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f98d5a9d9d3940e780ca1f3c1c900e4f/reply.gif)[**wwwlookycc**](http://10443421.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2015-06-19 09:52:37
楼主 ，我们也在研究 webrtc， 能否求指教
[26楼]    ![](file:///D:/My%20Documents/youdaonote/elesos@163.com/f98d5a9d9d3940e780ca1f3c1c900e4f/reply.gif)[**hnraysir**](http://hnraysir.blog.51cto.com/)[回复](http://billhoo.blog.51cto.com/#)
2015-08-28 18:20:58
我以最少改动，编译出来了so文件，欢迎大家参考，现在就差apm整体模块的使用了，哪个有apm的使用方法的，欢迎通知我哈。目前只发现在modules\audio_processing\test目录下有unit_test.cc，等我研究好其使用了再详细记录下。
http://wiki.elesos.com/index.php?title=WebRTC%E7%B3%BB%E5%88%974:%E9%9F%B3%E9%A2%91%E5%A4%84%E7%90%86%E6%A8%A1%E5%9D%97apm%E4%B8%8B%E8%BD%BD%E4%B8%8E%E6%95%B4%E4%BD%93%E7%BC%96%E8%AF%91



