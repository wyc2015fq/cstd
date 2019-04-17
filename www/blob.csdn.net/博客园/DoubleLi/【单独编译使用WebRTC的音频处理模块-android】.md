# 【单独编译使用WebRTC的音频处理模块 - android】 - DoubleLi - 博客园






**更新**

【2015年2月15日】

    Bill 这段时间没有再关注 WebRTC 以及音频处理的相关信息，且我个人早已不再推荐单独编译 WebRTC 中的各个模块出来使用。实际上本文的参考价值已经很小了，甚至可能会产生误导。不删这篇文章的原因在于文后有很多读者的讨论，其中的一些仍具备一定的价值，请大家务必以批判和审慎的态度阅读文章。



【2014年5月14日】

   昨天有幸在 Google 论坛里询问到 AECM 模块的延迟计算一事，Project member 说捣腾这个延迟实际上对 AECM 的效果没有帮助，这个延迟值仅在 AECM 启动时加快内置延迟估算器的收敛，如果更新的延迟有误，甚至会使 AECM 内置的延迟估算器出现错误的偏移，他建议我使用一个理论上的定值，Chrome 中他们用了 100ms。我之前也在 AECM 里看到了它对内置的 delay_estimator 二进制延迟估算器有很大的依赖，近端与远端数据匹配与否，几乎全部仰仗这个延迟估算器的结果。因此，对于 AECM 的使用，是否还需要花时间去计算这个系统延迟，bill 不再置评，个中效果大家自己实践和把握。

   其次，AECM 产生的唧唧声 Project member 澄清这不是 bug，而是 AECM 算法本来就有此表现，属正常现象。

   在这里仅希望大家知道此事，以免被我一家之言误导。



   【2014年5月8日】

   本文已有一年之久，随着自己在学习过程中认识的加深，以及期间和各位在文后的讨论，担心后来者照着这一年前的文章走弯路，bill 觉得有必要对文章做一个更新，点出自己走的弯路，以免误导后来者。

   1. 自己最开始是把 AECM、NS、VAD、AGC 各个模块单独提取出来使用，现在看来实属麻烦，且效果也不甚理想。如果大家的项目没有特殊的要求，大可将整个语音引擎 VoiceEngine 编译出来使用。就我个人而言，目前的解决方案是独立编译使用音频处理单元 AudioProcessingModule，因为 APM 是一个纯净的音频处理单元，其接口仅与音频处理有关，APM编译请见[此文](http://billhoo.blog.51cto.com/2337751/1325273)。APM的使用加上上层代码的优化，可以保证基本的通话效果（离完美还很远），回声基本是没有的。主要会存在两个问题，一是AECM出来的效果会有唧唧声，这个声音可以通过对延迟计算的不断优化而得到改善，最终可以做到说几句话之后有1~2次唧唧声。二是通话过程中声音会忽大忽小，目前我是怀疑由AECM的double talk处理引起的，具体的还要自己去倒腾。

   2. 关于回声消除滤波器延迟的计算，之前自己一直认为只要这个延迟计算准确，就能得到理想的回声消除效果，现在发现这个想法太幼稚，一是AECM算法本身有一定局限性，二是Android上的采集延迟没有系统API支持，很难计算准确，而播放端的API又不能保证其准确性。目前我的能力只能做到尽量优化上层的延迟计算，尽量减少由Android音频API对延迟造成的影响。

   3. 在 Android 上层优化计算系统音频延迟的代码达到一定瓶颈后，可以将优化目标转向 1）AECM 算法。 2）优化AEC（PC）（使其能在手机上正常运行，目前AECPC默认滤波器长度为12块，每块64个点，即AECPC仅能处理48ms的单声道16kHz延迟的数据，而Android的音频系统延迟大多在100ms以上，因此既要增加AECPC滤波器长度又要保证其运行效率是优化的重点） 3）其他模块的优化（比如抖动缓冲区等）。

   4. 文后的源码列表已经过时，由于我目前不再支持单独编译这些模块，恕我不再更新该列表，如确有独立编译需求的，可自行在WebRTC项目对应目录中找到需要的文件。



**附言**

   WebRTC是时下比较热门的新技术，由于bill接触时间尚短，对该项目的理解和认知定存在不足甚或偏差，文中有描述不当之处还望各位悉心指出，感激不尽。



**前言**

   最近一直在捣腾如何在android和iOS上使用Google的[WebRTC](http://www.webrtc.org/)——一个无疑大力推动了互联网即时通信以及VoIP发展的开源项目。

   虽然WebRTC主要目标是为互联网提供高质量的富媒体即时通信，但其源码为C/C++所写，且其开发版中也包含对 android 和 iOS 等移动设备的支持，因此对于如今飞速发展的移动互联网，WebRTC也能推波助澜大显神通。

   WebRTC提供一套音频处理引擎VOE（本文不涉及视频处理引擎VIE），但VOE在 android 和 iOS 上的整体编译一直是一个比较繁琐且恼火的问题，于是单独提取了VOE中的NS（Noise Suppression 噪声抑制）、VAD（Voice Activity Detection 静音检测）、AECM（Acoustic Echo Canceller for Mobile 声学回声消除）以及 AGC（Auto Gain Control 自动增益控制）等模块进行编译并捣鼓其使用方法。

   经过自己两月有余的捣腾和测试，终于在 android 和 iOS 上成功编译出各模块并在项目中使用了NS/VAD/AECM三大模块，效果比较不错。

   回过头来看看，这几大模块的编译其实非常简单，不过两月前的自己也着实为这个花了一番力气。



**正****文**

   由于几大模块的编译方式相同，故本文仅以 NS 模块为例，其余模块请读者自行摸索和实验。



**Step 1 - 下载 g****oo****gle WebRTC 源码**

   WebRTC目前的开发版主线版本已经到了 r4152 - 3.32，但这几大模块并未有大的修改，故本文依旧按bill当时的版本 3.31 进行讲解，请自行使用SVN同步以下目录（至于同步的方法，请自行google）：

[http://webrtc.googlecode.com/svn/branches/3.31/](http://webrtc.googlecode.com/svn/branches/3.31/)





**Step 2 - 提取WebRTC - NS模块代码**

   同步源码后，进入目录 \webrtc\modules\audio_processing\ns ，将NS模块的源码拷贝出来，下面是单独编译NS时的参考源码列表（部分头文件在WebRTC项目其他目录下，请自行搜索提取）：

                                       defines.h

                                       signal_procession_library.h

                                       spl_inl.h

                                       typdefs.h

                                       windows_private.h



                                       fft4g.h / fft4g.c

                                       noise_suppression.h / noise_suppression/c

                                       ns_core.h / ns_core.c

   除了上述WebRTC源码外，如果要在android的Java代码中使用，还需自行编写JNI包装文件：

ns_jni_wrapper.c（此为自定义的 jni 包装文件，详情请见 [此文](http://billhoo.blog.51cto.com/2337751/1125039)）



ADDED(billhoo - 2013-6-14) 鉴于有朋友询问JNI Wrapper的编写，下面提供NS模块create以及initialize函数（这两个函数足以说明问题）的wrapper源码及注释，希望对大家有所帮助。更详细的编写步骤请参考 Oracle官方文档 或 [此文](http://www3.ntu.edu.sg/home/ehchua/programming/java/JavaNativeInterface.html) 或 [此文](http://billhoo.blog.51cto.com/2337751/1125039)。



WebRtcNs_Create 包装函数及注释



```
```cpp
/***
```



```cpp

```

```cpp
* Summary
```



```cpp

```

```cpp
* types：
```



```cpp

```

```cpp
*   NSinst_t : the type of noise suppression instance structure.
```



```cpp

```

```cpp
*   NsHandle : actually the same type of NSinst_t, defined in
```



```cpp

```

```cpp
*              "noise_suppression.h" as a empty struct type named
```



```cpp

```

```cpp
*              "NsHandleT".
```



```cpp

```

```cpp
*
```



```cpp

```

```cpp
*   Note:
```



```cpp

```

```cpp
*    1.You have no need to pass env and jclazz to these functions,
```



```cpp

```

```cpp
*      cus' JVM will does it for you.
```



```cpp

```

```cpp
*    2.We only support 10ms frames, that means you can only input 320
```



```cpp

```

```cpp
*      Bytes a time.
```



```cpp

```

```cpp
**/
```



```cpp
/**
```



```cpp

```

```cpp
* This function wraps the "WebRtcNs_Create" function in "noise_suppression.c".
```



```cpp

```

```cpp
* Input:
```



```cpp

```

```cpp
*        none.
```



```cpp

```

```cpp
* Output:
```



```cpp

```

```cpp
*        the handler of created noise suppression instance.
```



```cpp

```

```cpp
* Return value:
```



```cpp

```

```cpp
*        -1 : error occurs.
```



```cpp

```

```cpp
*        other value : available handler of created NS instance.
```



```cpp

```

```cpp
*
```



```cpp

```

```cpp
* @author billhoo
```



```cpp

```

```cpp
* @version 1.0 2013-1-29
```



```cpp

```

```cpp
*/
```



```cpp
JNIEXPORT jint JNICALL
```



```cpp
Java_你的类限定名_createNSInstance(JNIEnv *env,
```



```cpp

```

```cpp
jclass jclazz) {
```



```cpp

```

```cpp
NsHandle *hNS = NULL;
```

```cpp
//create a pointer to NsHandle on native stack.
```



```cpp

```

```cpp
if
```

```cpp
(WebRtcNs_Create(&hNS) == -1) {
```

```cpp
//allocate dynamic memory on native heap for NS instance pointed by hNS.
```



```cpp

```

```cpp
return
```

```cpp
-1;
```

```cpp
//error occurs
```



```cpp

```

```cpp
}
```

```cpp
else
```

```cpp
{
```



```cpp

```

```cpp
return
```

```cpp
((
```

```cpp
int
```

```cpp
) (NSinst_t *) hNS);
```

```cpp
//returns the address of NS instance on native heap.
```



```cpp

```

```cpp
}
```



```cpp
}
```
```





WebRtcNs_Initiate 包装函数及注释



```
```cpp
/**
```



```cpp

```

```cpp
* This function wraps the "WebRtcNs_Init" function in
```



```cpp

```

```cpp
* "noise_suppression.c".
```



```cpp

```

```cpp
* Initializes a NS instance and has to be called before any other
```



```cpp

```

```cpp
* processing is made.
```



```cpp

```

```cpp
*
```



```cpp

```

```cpp
* Input:
```



```cpp

```

```cpp
*        - nsHandler   - Handler of NS instance that should be
```



```cpp

```

```cpp
*                        initialized.
```



```cpp

```

```cpp
*        - sf          - sampling frequency, only 8000, 16000, 32000
```



```cpp

```

```cpp
*                        are available.
```



```cpp

```

```cpp
* Output:
```



```cpp

```

```cpp
*         nsHandler  - the handler of initialized instance.
```



```cpp

```

```cpp
* Return value:
```



```cpp

```

```cpp
*         0                - OK
```



```cpp

```

```cpp
*         -1               - Error
```



```cpp

```

```cpp
*
```



```cpp

```

```cpp
* @author billhoo
```



```cpp

```

```cpp
* @version 1.0 2013-1-29
```



```cpp

```

```cpp
*/
```



```cpp
JNIEXPORT jint JNICALL
```



```cpp
Java_你的类限定名_initiateNSInstance(JNIEnv *env,
```



```cpp

```

```cpp
jclass jclazz, jint nsHandler, jlong sf) {
```



```cpp

```

```cpp
NsHandle *hNS = (NsHandle*) nsHandler;
```



```cpp

```

```cpp
return
```

```cpp
WebRtcNs_Init(hNS, sf);
```



```cpp
}
```
```





[END OF ADDED]





**Step 3 - 编译WebRTC - NS模块**

   此步请参照 [bill之前的文章](http://billhoo.blog.51cto.com/2337751/1125039)将刚才提取的NS代码添加进eclipse工程进行编译即可。以下为NS模块的Android.mk文件：







```
```xml
LOCAL_PATH := $(call my-dir)
```



```xml
include $(CLEAR_VARS)
```



```xml
LOCAL_MODULE    := webrtc_ns
```



```xml
LOCAL_SRC_FILES := \
```



```xml

```

```xml
noise_suppression.c \
```



```xml

```

```xml
ns_core.c \
```



```xml

```

```xml
fft4g.c \
```



```xml

```

```xml
ns_jni_wrapper.c
```



```xml
include $(BUILD_SHARED_LIBRARY)
```
```





   编译完成后，将项目中的 webrtc_ns.so 动态库拷贝出来以备后续使用。



**Step 4 - 加载编译好的NS模块动态库**

   接下来只需要按照 [此文](http://billhoo.blog.51cto.com/2337751/1125559) 的描述在 android 的JAVA代码中使用刚才编译好的 webrtc_ns.so 动态库便大功告成。



**Step 5 - 几大模块的使用及注意事项**

   前四步已经完成了几大音频处理模块在android上的单独编译过程，并分别生成了 webrtc_ns.so、webrtc_vad.so、webrtc_aecm.so 以及 webrtc_agc.so 四个动态库，下面bill简要介绍对NS、VAD以及AECM三个库的函数使用方法及注意事项：



**5.1 - NS库函数的使用及注意事项**

   这个很简单，参照 noise_suppression.h 头文件中对各API的描述即可，首先使用 WebRtcNs_Create 创建NS实体，然后 WebRtcNs_Init 初始化该实体，WebRtcNs_set_policy 设置噪声抑制的级别（bill使用的是最高级别 2，效果比较理想），设置完成后便可调用 WebRtcNs_Process 循环对10ms（8000Hz、16000Hz）音频帧进行NS处理，注意最后别忘了调用 WebRtcNs_Free 将NS实体销毁。





**5.2 - VAD库函数的使用及注意事项**

VAD的使用和NS区别不大，唯一需要注意的是VAD仅仅只是检测，返回结果1表示VAD检测此帧为活动帧，0表示此帧为静音帧，至于判断为静音后该进行何种处理，就和你自己的项目相关了。





**5.3 - AECM库函数的使用及注意事项**

AECM实体的创建、初始化和销毁工作与上述相同，之后需要在远端和近端分别调用 WebRtcAecm_BufferFarend 以及 WebRtcAecm_Process，对于AECM的使用，需要注意的重点在于Process函数的参数msInSndCardBuf，该参数在audio_procession.h头文件中以名为delay的变量呈现，该延迟的计算确为一难点（对于单独使用AECM模块来说），不过只要**严格**按照delay的描述进行操作即可。



**附：**

   其他几大模块单独编译时需要的源文件列表（所有依赖头文件略，请自行根据报错添加）：

**WebRTC - VAD 模块源文件列表**

       注意：VAD的编译需要宏 WEBRTC_POSIX 的支持，而该宏是否有实现，由 WEBRTC_ANDROID 等宏是否被定义决定，若你在编译时提示 once 函数未定义等错误， 请自行添加对 WEBRTC_ANDROID宏的定义。

       webrtc_vad.c

       vad_core.c

       vad_filterbank.c

       vad_gmm.c

       vad_sp.c

       real_fft.c

       division_operations.c

       complex_bit_reverse.c

       cross_correlation.c

       complex_fft.c

       downsample_fast.c

       vector_scaling_operations.c

       get_scaling_square.c

       energy.c

       min_max_operations.c

       spl_init.c



****WebRTC - AECM 模块源文件列表****

       randomization_functions.c

       spl_sqrt_floor.c

       division_operations.c

       min_max_operations.c

       ring_buffer.c

       delay_estimator.c

       delay_estimator_wrapper.c

       complex_bit_reverse.c

       complex_fft.c

       aecm_core.c

       echo_control_mobile.c



********WebRTC - AGC 模块源文件列表********







       spl_sqrt.c

       copy_set_operations.c

       division_operations.c

       dot_product_with_scale.c

       resample_by_2.c

       analog_agc.c

       digital_agc.c



from:http://billhoo.blog.51cto.com/2337751/1213801









