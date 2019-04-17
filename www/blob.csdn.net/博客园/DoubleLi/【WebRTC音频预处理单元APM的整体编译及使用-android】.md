# 【WebRTC音频预处理单元APM的整体编译及使用 - android】 - DoubleLi - 博客园






**前言**

在写[【单独编译使用WebRTC的音频处理模块 - android】](http://billhoo.blog.51cto.com/2337751/1213801)一文之前，就一直想直接把WebRTC的音频处理引擎VoE整个儿编译出来供自己的项目使用，但限于技术拙劣、时间紧迫，所以没能成功。只得挨个挨个把引擎中的Aecm、Agc、Ns以及Vad模块单独编译出来凑合着用。虽能达到一定效果，但始终不甚理想。5个月后，bill需要优化之前的项目，于是就下了狠心，定要将整个音频处理模块用上 ...



**正文**

然而本次优化仍然没能用上整套VoE，因为VoE不仅仅包含音频预处理，它将音频编码模块、传输模块一并融入了引擎，而bill的项目需要使用既有的编码、传输层，因此使用整个VoE对我来说显得冗余且不可操作。天无绝人之路，抛开VoE不谈，bill找到了仅次于VoE层级的模块 —— APM（Audio Preprocessing Module） —— 一个整合了前文所有模块且纯粹的音频预处理单元。



**Step 1 - 下载Google WebRTC源码**

Google WebRTC的开发进度还是可观的，本文将以WebRTC的最新trunk revision 5125为例进行讲解。请自行使用SVN同步以下目录（至于同步的方法，请自行google）：

[http://webrtc.googlecode.com/svn/trunk/](http://webrtc.googlecode.com/svn/trunk/)



**Step 2 - 提取编译APM所需资源**

APM的整体编译需要WebRTC源码目录下的如下资源：

1）common_audio 整个目录

2）modules 目录（不包含 video 部分）

3）system_wrappers 整个目录

4）位于 WebRTC 源码根目录下的 common_types.h | common.h | typedefs.h 三个头文件。

5）位于 WebRTC 主目录下的 android-webrtc.mk 文件。



****Step 3 - 在Eclipse中编译APM基础步骤及部分要点****

对于Eclipse中jni的编译及使用请参见[上篇文章](http://billhoo.blog.51cto.com/2337751/1213801)所述，在此不再赘述。

此节仅按照自己的jni目录组织结构进行讲解，读者可根据自己需要进行调整。

在Eclipse中的jni组织结构如下：

![102738855.png](http://img1.51cto.com/attachment/201311/102738855.png)

Step-2中的所有文件夹及头文件均位于 webrtc 子目录下。android-webrtc.mk 位于 jni 根目录下。

下面我们逐步进行分解：



step 3.1

首先我们需要对整个 android 工程进行描述和设定，打开 jni 根目录下的 Application.mk 文件，编辑如下：







```
```bash
# Copyright (c) 2013 BillHoo. All Rights Reserved.
```



```bash
# Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
```



```bash
#
```



```bash
# Use of this source code is governed by a BSD-style license
```



```bash
# that can be found in the LICENSE file in the root of the source
```



```bash
# tree. An additional intellectual property rights grant can be found
```



```bash
# in the file PATENTS.  All contributing project authors may
```



```bash
# be found in the AUTHORS file in the root of the source tree.
```



```bash
APP_STL := gnustl_static
```



```bash
APP_CPPFLAGS := -frtti -fexceptions
```



```bash
APP_ABI := armeabi armeabi-v7a
```



```bash
APP_PLATFORM := android-9
```
```



其中 APP_STL 的官方说明如下：





```
`APP_STL`

`    ``By default, the NDK build system provides C++ headers for the minimal`

`    ``C++ runtime library (/system/lib/libstdc++.so) provided by the Android`

`    ``system.`

`    ``However, the NDK comes with alternative C++ implementations that you can`

`    ``use or link to in your own applications. Define APP_STL to select one of`

`    ``them. Examples are:`

`       ``APP_STL := stlport_static    --> static STLport library`

`       ``APP_STL := stlport_shared    --> shared STLport library`

`       ``APP_STL := system            --> default C++ runtime library`

`    ``For more information on the subject, please read docs/CPLUSPLUS-SUPPORT.html`
```



由于 NDK 默认使用最小 C++ 运行时库进行项目的编译，导致无法编译 WebRTC 中使用诸如 std::map 等 STL 容器的源码。因此我们需要自行设定适合本项目的 C++ 运行时库 gnustl_static。



step 3.2

打开并编辑 jni 根目录下的 Android.mk 文件如下，本文件只需告诉 NDK 去调用所有子目录下的 Android.mk 文件即可：





```
```bash
# Copyright (c) 2013 BillHoo. All Rights Reserved.
```



```bash
# Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
```



```bash
#
```



```bash
# Use of this source code is governed by a BSD-style license
```



```bash
# that can be found in the LICENSE file in the root of the source
```



```bash
# tree. An additional intellectual property rights grant can be found
```



```bash
# in the file PATENTS.  All contributing project authors may
```



```bash
# be found in the AUTHORS file in the root of the source tree.
```



```bash
include $(call all-subdir-makefiles)
```
```







step 3.3

准备工作就绪，下面就可以开始着手编译整个 APM 单元了，首先打开 jni/webrtc 目录，新建 Android.mk 文件如下：





```
```bash
# Copyright (c) 2013 BillHoo. All Rights Reserved.
```



```bash
# Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
```



```bash
#
```



```bash
# Use of this source code is governed by a BSD-style license
```



```bash
# that can be found in the LICENSE file in the root of the source
```



```bash
# tree. An additional intellectual property rights grant can be found
```



```bash
# in the file PATENTS.  All contributing project authors may
```



```bash
# be found in the AUTHORS file in the root of the source tree.
```



```bash
#
```



```bash
MY_WEBRTC_ROOT_PATH := $(call my-
```

```bash
dir
```

```bash
)
```



```bash
#
```



```bash
# voice
```



```bash
include $(MY_WEBRTC_ROOT_PATH)
```

```bash
/common_audio/signal_processing/Android
```

```bash
.mk
```



```bash
include $(MY_WEBRTC_ROOT_PATH)
```

```bash
/common_audio/vad/Android
```

```bash
.mk
```



```bash
include $(MY_WEBRTC_ROOT_PATH)
```

```bash
/modules/audio_processing/aec/Android
```

```bash
.mk
```



```bash
include $(MY_WEBRTC_ROOT_PATH)
```

```bash
/modules/audio_processing/aecm/Android
```

```bash
.mk
```



```bash
include $(MY_WEBRTC_ROOT_PATH)
```

```bash
/modules/audio_processing/agc/Android
```

```bash
.mk
```



```bash
include $(MY_WEBRTC_ROOT_PATH)
```

```bash
/modules/audio_processing/Android
```

```bash
.mk
```



```bash
include $(MY_WEBRTC_ROOT_PATH)
```

```bash
/modules/audio_processing/ns/Android
```

```bash
.mk
```



```bash
include $(MY_WEBRTC_ROOT_PATH)
```

```bash
/modules/audio_processing/utility/Android
```

```bash
.mk
```



```bash
include $(MY_WEBRTC_ROOT_PATH)
```

```bash
/modules/utility/source/Android
```

```bash
.mk
```



```bash
include $(MY_WEBRTC_ROOT_PATH)
```

```bash
/modules/audio_device/Android
```

```bash
.mk
```



```bash
include $(MY_WEBRTC_ROOT_PATH)
```

```bash
/system_wrappers/source/Android
```

```bash
.mk
```



```bash
#
```



```bash
# build .so
```



```bash
LOCAL_PATH := $(call my-
```

```bash
dir
```

```bash
)
```



```bash
include $(CLEAR_VARS)
```



```bash
LOCAL_ARM_MODE := arm
```



```bash
LOCAL_MODULE := liblu_audio_preprocessing
```



```bash
LOCAL_MODULE_TAGS := optional
```



```bash
LOCAL_WHOLE_STATIC_LIBRARIES := \
```



```bash

```

```bash
libwebrtc_spl \
```



```bash

```

```bash
libwebrtc_apm \
```



```bash

```

```bash
libwebrtc_apm_utility \
```



```bash

```

```bash
libwebrtc_vad \
```



```bash

```

```bash
libwebrtc_ns \
```



```bash

```

```bash
libwebrtc_agc \
```



```bash

```

```bash
libwebrtc_aec \
```



```bash

```

```bash
libwebrtc_aecm \
```



```bash

```

```bash
libwebrtc_system_wrappers \
```



```bash

```

```bash
libwebrtc_audio_device \
```



```bash

```

```bash
libwebrtc_utility
```



```bash
#
```



```bash
# Add Neon libraries.
```



```bash
ifeq ($(WEBRTC_BUILD_NEON_LIBS),
```

```bash
true
```

```bash
)
```



```bash
LOCAL_WHOLE_STATIC_LIBRARIES += \
```



```bash

```

```bash
libwebrtc_aecm_neon \
```



```bash

```

```bash
libwebrtc_ns_neon \
```



```bash

```

```bash
libwebrtc_spl_neon
```



```bash
endif
```



```bash
LOCAL_STATIC_LIBRARIES := \
```



```bash

```

```bash
libprotobuf-cpp-2.3.0-lite
```



```bash
LOCAL_SHARED_LIBRARIES := \
```



```bash

```

```bash
libcutils \
```



```bash

```

```bash
libdl \
```



```bash

```

```bash
libstlport
```



```bash
LOCAL_PRELINK_MODULE :=
```

```bash
false
```



```bash
#
```



```bash
#TODO(billhoo) find a properway to do this.
```



```bash
LOCAL_LDLIBS += $(NDK_ROOT)
```

```bash
/sources/cxx-stl/gnu-libstdc
```

```bash
++
```

```bash
/4
```

```bash
.6
```

```bash
/libs/armeabi/libgnustl_static
```

```bash
.a
```



```bash
LOCAL_LDLIBS += -lOpenSLES
```



```bash
ifndef NDK_ROOT
```



```bash
include external
```

```bash
/stlport/libstlport
```

```bash
.mk
```



```bash
endif
```



```bash
include $(BUILD_SHARED_LIBRARY)
```
```



需要注意的几点：

1）在编译时如提示找不到 ../../../Android.mk 文件等错误，请检查并修正你的相对路径。

2）位于第60行的gnu静态库链接路径是针对NDK版本 r8d 的，如读者版本不匹配，请自行找到 libgnustl_static.a 静态库的路径进行替换。

3）本示例并不打算编译 WebRTC 的测试工程，请使用 Eclipse 搜索文件功能，找到 Android.mk 文件中的 -DWEBRTC_AUDIOPROC_DEBUG_DUMP 并注释掉。



step 3.4

万事俱备，我们可以开始编译 APM 了，不过在编译过程中肯定还会有很多小问题出现（比如相对路径不正确、找不到某某函数的符号等等），这些问题就留给读者自行google、SO解决了，bill就不再赘述。



******Step 4 - 在android应用中使用APM的注意事项******

经过上述步骤，读者便能够得到 libwebrtc_audio_preprocessing.so 这个动态链接库。我们需要做的仅仅是编写自己的 jni 包装函数向 android 应用层提供 APM 的接口。具体做法bill[之前的文章](http://billhoo.blog.51cto.com/2337751/1125039)已经详细介绍过。这里需要注意的是，如果读者打算在自己的动态库中引用已经编译好的 APM 库，那么在 android 类加载这两个库时的顺序是敏感的。

假设读者将自己的 JNI 接口封装成单独的库 libmy_jni_wrapper.so，而该库引用了 libwebrtc_audio_preprocessing.so，那么在加载这两个库时应该参照如下顺序：





```
```java
static
```

```java
{
```



```java

```

```java
// Ordering of loading these shared libraries is significant.
```



```java

```

```java
System.loadLibrary(
```

```java
"webrtc_audio_preprocessing"
```

```java
);
```



```java

```

```java
System.loadLibrary(
```

```java
"my_jni_wrapper"
```

```java
);
```



```java
}
```
```



若顺序写反，在运行时将得到找不到 webrtc_audio_preprocessing 库中符号的异常。



**总结**

整个编译工作在现在看来非常简单，但需要很多的耐心和搜索，不过结果还是令人比较满意的，APM出来的效果比之前自己单独使用各个音频模块要好很多。不过对于抖动等因素的影响，APM就力不从心了。也许bill接下来该花时间去看看NetEq、Jitter Buffer等模块了。如何使用他们，如何融进自己的项目，到时候就知道了。









