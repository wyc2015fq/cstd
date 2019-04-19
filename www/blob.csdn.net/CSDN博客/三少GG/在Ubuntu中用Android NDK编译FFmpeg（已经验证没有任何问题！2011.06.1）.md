# 在Ubuntu中用Android NDK编译FFmpeg（已经验证没有任何问题！2011.06.1） - 三少GG - CSDN博客
2011年06月02日 01:21:00[三少GG](https://me.csdn.net/scut1135)阅读数：1835标签：[android																[ubuntu																[include																[path																[library																[module](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android 多媒体](https://blog.csdn.net/scut1135/article/category/790993)
原文地址：  [http://www.cnblogs.com/scottwong/archive/2010/12/17/1909455.html](http://www.cnblogs.com/scottwong/archive/2010/12/17/1909455.html)
最近在做 Android 上的项目，我被恶心的一塌糊涂。本以为 Java 是 Android 上的一等公民，结果深入学习之后才发现，Java 在 Android 上 就是个做 UI 的，除此之外无论想干什都得用 C 语言去实现。Android 一个非常糟糕差劲的操作系统，甚至连 Windows Mobile 都不如。Android 能取得今天的市场占有率只是因为当年微软的 Window Phone 7 还在开发中，而 iOS 又只给 iPhone用，所以手机生产商没得选，只能被迫采用 Android 这个连
 Linux 内核开发团队都不承认的 Linux 操作系统。而基于 Linux 内核就是 Android 唯一的优点了，正是因为如此我们才想办法能把那些 Linux 上的伟大开源项目移植到 Android 上以弥补 Android 的不足。
Android 的多媒体功能是如此之弱，限制是如此之多，逼着我只能想办法去把 FFmpeg 移植到 Android 上。 感谢 [havlenapetr](http://github.com/havlenapetr/FFMpeg) 给出的示例代码，感谢 [ABitNo](http://abitno.me/compile-ffmpeg-android-ndk) 整理的说明文档，没有他们的贡献，我不可能把 FFmpeg 成功移植到 Android 上。下面我将说明将 FFmpeg 移植到 Android 上的详细步骤，希望能对正在进行同样工作的朋友有所帮助。
**一、下载必要软件**
[Oracle VM VirtualBox 3.2.12](http://www.virtualbox.org/wiki/Downloads)
[Ubuntu Desktop Edition 10.10 32-bit](http://www.ubuntu.org.cn/desktop/get-ubuntu/download/)
[Android NDK r4b](http://developer.android.com/sdk/ndk/index.html)（需要翻墙访问）
[Android NDK r5](http://developer.android.com/sdk/ndk/index.html)（需要翻墙访问）
[FFmpeg 0.6.1](http://www.ffmpeg.org/download.html)
**二、配置编译环境**
- 在 VirtualBox 中创建一个 Ubuntu 虚拟机 
- 在 Ubuntu 虚拟机中使用 sudo passwd root 命令启动 root 账户 
- 用 root 账户登录进入 Ubuntu 
- 将 android-ndk-r4b-linux-x86.zip 中的内容解压缩到 /root 目录下 
- 将 android-sdk_r07-linux_x86.tgz 中的内容解压缩到 /root 目录下 
- 将 ffmpeg-0.6.1.tar.bz2 中的内容解压缩到 /root/ffmpeg/jni 目录下 
**三、准备编译 FFmpeg**
- 编写 mk 文件
- 在 /root/ffmpeg/jni 目录中创建一个 Android.mk 文件，内容如下
include $(all-subdir-makefiles)
- 在 /root/ffmpeg/jni/ffmpeg-0.6.1 目录中创建一个 Android.mk 文件，内容如下
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_WHOLE_STATIC_LIBRARIES := libavformat libavcodec libavutil libpostproc libswscale
LOCAL_MODULE := ffmpeg
include $(BUILD_SHARED_LIBRARY)
include $(call all-makefiles-under,$(LOCAL_PATH))
- 在 /root/ffmpeg/jni/ffmpeg-0.6.1 目录中创建一个 av.mk 文件，内容如下
# LOCAL_PATH is one of libavutil, libavcodec, libavformat, or libswscale
#include
 $(LOCAL_PATH)/../config-$(TARGET_ARCH).mak
include $(LOCAL_PATH)/../config.mak
OBJS :=
OBJS-yes :=
MMX-OBJS-yes :=
include $(LOCAL_PATH)/Makefile
# collect objects
OBJS-$(HAVE_MMX) += $(MMX-OBJS-yes)
OBJS += $(OBJS-yes)
FFNAME := lib$(NAME)
FFLIBS := $(foreach,NAME,$(FFLIBS),lib$(NAME))
FFCFLAGS  = -DHAVE_AV_CONFIG_H -Wno-sign-compare -Wno-switch
 -Wno-pointer-sign
FFCFLAGS += -DTARGET_CONFIG=/"config-$(TARGET_ARCH).h/"
ALL_S_FILES := $(wildcard $(LOCAL_PATH)/$(TARGET_ARCH)/*.S)
ALL_S_FILES := $(addprefix $(TARGET_ARCH)/, $(notdir $(ALL_S_FILES)))
ifneq ($(ALL_S_FILES),)
ALL_S_OBJS := $(patsubst %.S,%.o,$(ALL_S_FILES))
C_OBJS := $(filter-out $(ALL_S_OBJS),$(OBJS))
S_OBJS := $(filter $(ALL_S_OBJS),$(OBJS))
else
C_OBJS := $(OBJS)
S_OBJS :=
endif
C_FILES := $(patsubst %.o,%.c,$(C_OBJS))
S_FILES := $(patsubst %.o,%.S,$(S_OBJS))
FFFILES := $(sort $(S_FILES)) $(sort $(C_FILES))
- 在 /root/ffmpeg/jni/ffmpeg-0.6.1/libavcodec 目录中创建一个 Android.mk 文件，内容如下
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
include $(LOCAL_PATH)/../av.mk
LOCAL_SRC_FILES := $(FFFILES)
LOCAL_C_INCLUDES :=        /
    $(LOCAL_PATH)        /
    $(LOCAL_PATH)/..
LOCAL_CFLAGS += $(FFCFLAGS)
LOCAL_LDLIBS := -lz
LOCAL_STATIC_LIBRARIES := $(FFLIBS)
LOCAL_MODULE := $(FFNAME)
include $(BUILD_STATIC_LIBRARY)
- 在 /root/ffmpeg/jni/ffmpeg-0.6.1/libavformat 目录中创建一个 Android.mk 文件，内容如下
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
include $(LOCAL_PATH)/../av.mk
LOCAL_SRC_FILES := $(FFFILES)
LOCAL_C_INCLUDES :=        /
    $(LOCAL_PATH)        /
    $(LOCAL_PATH)/..
LOCAL_CFLAGS += $(FFCFLAGS)
LOCAL_CFLAGS += -include "string.h"
 -Dipv6mr_interface=ipv6mr_ifindex
LOCAL_LDLIBS := -lz
LOCAL_STATIC_LIBRARIES := $(FFLIBS)
LOCAL_MODULE := $(FFNAME)
include $(BUILD_STATIC_LIBRARY)
- 在 libavfilter、libavutil、libpostproc 和 libswscale 目录中各创建一个 Android.mk 文件，内容如下
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
include $(LOCAL_PATH)/../av.mk
LOCAL_SRC_FILES := $(FFFILES)
LOCAL_C_INCLUDES :=        /
    $(LOCAL_PATH)        /
    $(LOCAL_PATH)/..
LOCAL_CFLAGS += $(FFCFLAGS)
LOCAL_STATIC_LIBRARIES := $(FFLIBS)
LOCAL_MODULE := $(FFNAME)
include $(BUILD_STATIC_LIBRARY)
- 修改 libm.h 文件和 Makefile 文件
- 编辑 /root/ffmpeg/jni/ffmpeg-0.6.1/libavutil 目录中的 libm.h 文件，删除以下 static 方法
#if
 !HAVE_LRINT
static
 av_always_inline av_const longint
 lrint(double
 x)
{
    return
 rint(x);
}
#endif/* HAVE_LRINT */#if
 !HAVE_LRINTF
static
 av_always_inline av_const longint
 lrintf(float
 x)
{
    return
 (int
)(rint(x));
}
#endif/* HAVE_LRINTF */#if
 !HAVE_ROUND
static
 av_always_inline av_const double
 round(double
 x)
{
    return
 (x > 0) ? floor(x + 0.5) : ceil(x - 0.5);
}
#endif/* HAVE_ROUND */#if
 !HAVE_ROUNDF
static
 av_always_inline av_const float
 roundf(float
 x)
{
    return
 (x > 0) ? floor(x + 0.5) : ceil(x - 0.5);
}
#endif/* HAVE_ROUNDF */#if
 !HAVE_TRUNCF
static
 av_always_inline av_const float
 truncf(float
 x)
{
    return
 (x > 0) ? floor(x) : ceil(x);
}
#endif/* HAVE_TRUNCF */
- 编辑 libavcodec、libavfilter、libavformat、libavutil、libpostproc 和 libswscale 目录中的 Makefile 文件，删除
include $(SUBDIR)../subdir.mak
和
include $(SUBDIR)../config.mak
- 生成 config.h 文件
- 在 /root/ffmpeg/jni/ffmpeg-0.6.1 目录中创建一个 config.sh 文件，使用 Android NDK r4b 编译时内容如下
PREBUILT=/root/android-ndk-r4b/build/prebuilt/linux-x86/arm-eabi-4.4.0
PLATFORM=/root/android-ndk-r4b/build/platforms/android-8/arch-arm
./configure --target-os=linux /
    --arch=arm /
    --enable-version3 /
    --enable-gpl /
    --enable-nonfree /
    --disable-stripping /
    --disable-ffmpeg /
    --disable-ffplay /
    --disable-ffserver /
    --disable-ffprobe /
    --disable-encoders /
    --disable-muxers /
    --disable-devices /
    --disable-protocols /
    --enable-protocol=file /
    --enable-avfilter /
    --disable-network /
    --disable-mpegaudio-hp /
    --disable-avdevice /
    --enable-cross-compile /
    --cc=$PREBUILT/bin/arm-eabi-gcc /
    --cross-prefix=$PREBUILT/bin/arm-eabi- /
    --nm=$PREBUILT/bin/arm-eabi-nm /
    --extra-cflags="-fPIC -DANDROID"
 /
    --disable-asm
 /
    --enable-neon /
    --enable-armv5te /
    --extra-ldflags="-Wl,-T,$PREBUILT/arm-eabi/lib/ldscripts/armelf.x -Wl,-rpath-link=$PLATFORM/usr/lib -L$PLATFORM/usr/lib -nostdlib $PREBUILT/lib/gcc/arm-eabi/4.4.0/crtbegin.o $PREBUILT/lib/gcc/arm-eabi/4.4.0/crtend.o -lc -lm -ldl"
使用 Android NDK r5 编译时内容如下
#!/bin/bash
PREBUILT=/root/android-ndk-r5/toolchains/arm-eabi-4.4.0/prebuilt/linux-x86
PLATFORM=/root/android-ndk-r5/platforms/android-8/arch-arm
./configure --target-os=linux /
    --arch=arm /
    --enable-version3 /
    --enable-gpl /
    --enable-nonfree /
    --disable-stripping /
    --disable-ffmpeg /
    --disable-ffplay /
    --disable-ffserver /
    --disable-ffprobe /
    --disable-encoders /
    --disable-muxers /
    --disable-devices /
    --disable-protocols /
    --enable-protocol=file /
    --enable-avfilter /
    --disable-network /
    --disable-mpegaudio-hp /
    --disable-avdevice /
    --enable-cross-compile /
    --cc=$PREBUILT/bin/arm-eabi-gcc /
    --cross-prefix=$PREBUILT/bin/arm-eabi- /
    --nm=$PREBUILT/bin/arm-eabi-nm /
    --extra-cflags="-fPIC -DANDROID"
 /
    --disable-asm
 /
    --enable-neon /
    --enable-armv5te /
    --extra-ldflags="-Wl,-T,$PREBUILT/arm-eabi/lib/ldscripts/armelf.x -Wl,-rpath-link=$PLATFORM/usr/lib -L$PLATFORM/usr/lib -nostdlib $PREBUILT/lib/gcc/arm-eabi/4.4.0/crtbegin.o $PREBUILT/lib/gcc/arm-eabi/4.4.0/crtend.o -lc -lm -ldl"
- 打开终端，进入 /root/ffmpeg/jni/ffmpeg-0.6.1 目录，运行下面的命令
chmod +x config.sh
./config.sh
![](http://images.cnblogs.com/cnblogs_com/scottwong/201012/201012171710464654.png)
- 编辑 /root/ffmpeg/jni/ffmpeg-0.6.1 目录中的 config.h 文件，将
#define
 restrict restrict
修改为
#define
 restrict
**+++++++++++++++++++++++++++++++++++++++++++++**
**（附录：复制即可用config.h内容，【20110908】究竟是/还是\??  前者貌似试验过，后者今天20110908可以）**
```java
**#!/bin/bashPREBUILT=/home/grant/ProgramFiles/android-ndk-r5/toolchains/arm-eabi-4.4.0/prebuilt/linux-x86PLATFORM=/home/grant/ProgramFiles/android-ndk-r5/platforms/android-9/arch-arm./configure --target-os=linux \--arch=arm \--enable-version3 \--enable-gpl \--enable-nonfree \--disable-stripping \--disable-ffmpeg \--disable-ffplay \--disable-ffserver \--disable-ffprobe \--disable-encoders \--disable-muxers \--disable-devices \--disable-protocols \--enable-protocol=file \--enable-avfilter \--disable-network \--disable-mpegaudio-hp \--disable-avdevice \--enable-cross-compile \--cc=$PREBUILT/bin/arm-eabi-gcc \--cross-prefix=$PREBUILT/bin/arm-eabi- \--nm=$PREBUILT/bin/arm-eabi-nm \--extra-cflags="-fPIC -DANDROID"\--disable-asm \--enable-neon \--enable-armv5te \--extra-ldflags="-Wl,-T,$PREBUILT/arm-eabi/lib/ldscripts/armelf.x -Wl,-rpath-link=$PLATFORM/usr/lib -L$PLATFORM/usr/lib -nostdlib $PREBUILT/lib/gcc/arm-eabi/4.4.0/crtbegin.o $PREBUILT/lib/gcc/arm-eabi/4.4.0/crtend.o -lc -lm -ldl"**
```
**四、开始编译 FFmpeg**
很多人在用 havlenapetr 的方法编译 FFmpeg 时只得到一个 1599 字节 1.6KB 大小的 libffmpeg.so 文件，无论是用 Android NDK r4b 编译还是用 Android NDK r5 编译结果都是如此，很让人抓狂。我也很郁闷，最后花时间研究了一下 NDK，终于发现了解决方法，而且 Android NDK r4b 和 Android NDK r5  的情况还是完全不同的，请继续往下读。
- 使用 Android NDK r4b 编译
打开 android-ndk-r4b/build/toolchains/arm-eabi-4.4.0 目录中的 setup.mk 文件，你会发现 Google 在里面定义了一个用于编译动态库的 cmd-build-shared-library 函数。在cmd-build-shared-library 函数中 Google 使用了 PRIVATE_WHOLE_STATIC_LIBRARIES 函数。但是你在 android-ndk-r4b/build/core 目录中的 build-binary.mk 文件里却找不到
 PRIVATE_WHOLE_STATIC_LIBRARIES 函数…… 外？WHY？终于搞清楚了，原来得不到正确的 libffmpeg.so 文件不是我的错，而是 Android NDK r4b 的 BUG！你妹啊！你大爷啊！坑爹呢这是！发布前不做测试吗！居然漏掉一个函数！！！（我敢说这是个 BUG 是因为 Google 在 Android NDK r5 中修复了这个 BUG）
木办法，只好手动替 Google 修补这个 BUG。好在修改方法很简单，只需要照 build-binary.mk 文件里的 PRIVATE_STATIC_LIBRARIES 增加一个 PRIVATE_WHOLE_STATIC_LIBRARIES 就行了。具体方法见下图
修改前的 build-binary.mk 文件 
![image](http://images.cnblogs.com/cnblogs_com/scottwong/201012/201012171710509307.png)
修改后的 build-binary.mk 文件 
![image](http://images.cnblogs.com/cnblogs_com/scottwong/201012/201012171710537308.png)
保存 build-binary.mk 文件之后，运行下面的命令编译
/root/android-ndk-r4b/ndk-build NDK_PROJECT_PATH=/root/ffmpeg
接着你会看到 warning 不停的出现在屏幕上，熬过这段心惊肉跳的时间之后，你会看到 libffmpeg.so 文件已经被编译生成了。
![image](http://images.cnblogs.com/cnblogs_com/scottwong/201012/201012171710571056.png)
看看 /root/ffmpeg/obj/local/armeabi 目录中的 libffmpeg.so 文件，文件大小是 12.2MB  
![image](http://images.cnblogs.com/cnblogs_com/scottwong/201012/201012171710584828.png)
再看看 /root/ffmpeg/libs/local/armeabi 目录中的 libffmpeg.so 文件，文件大小是 3.2MB
![image](http://images.cnblogs.com/cnblogs_com/scottwong/201012/201012171710594696.png)
- 使用 Android NDK r5 编译
打开 android-ndk-r5/build/core 目录中的 build-binary.mk 文件，发现 Google 这次没有忘记 PRIVATE_WHOLE_STATIC_LIBRARIES，但还最后编译得到的 libffmpeg.so 文件大小还是不正确。 这次的问题是，android-ndk-r5 默认是使用 arm-linux-androideabi-4.4.3 编译，而不是 arm-eabi-4.4.0。但 android-ndk-r5/toolchains/arm-linux-androideabi-4.4.3
 目录中的 setup.mk 文件里定义的  cmd-build-shared-library 函数并没有将静态库文件链接在一起生成动态库文件。所以解决的办法就是在执行 ndk-build 时加上 NDK_TOOLCHAIN 参数，指定使用 arm-eabi-4.4.0 来编译。完整命令如下
/root/android-ndk-r5/ndk-build NDK_PROJECT_PATH=/root/ffmpeg NDK_TOOLCHAIN=arm-eabi-4.4.0 NDK_PLATFORM=android-8
**五、结语**
关于如何编写在 Android 上运行的 FFmpeg 播放器，请看 [havlenapetr/FFMpeg](https://github.com/lhzhang/FFMpeg)、[tewilove/faplayer](https://github.com/tewilove/faplayer)、[NicoRo](http://sourceforge.jp/projects/nicoro) 和[android / ffmpeg dynamic
 module, JNI simple wrapper](http://demo860.blogspot.com/2010/07/android-ffmpeg-dynamic-module-jni.html)（需要翻墙访问）
希望我的这篇随笔能对读到这里你有帮助。
