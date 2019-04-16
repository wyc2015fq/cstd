# 最简单的基于FFmpeg的移动端例子附件：SDL Android HelloWorld - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2015年07月25日 23:02:55[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：19175
所属专栏：[FFmpeg](https://blog.csdn.net/column/details/ffmpeg-devel.html)











=====================================================

最简单的基于FFmpeg的移动端例子系列文章列表：

[最简单的基于FFmpeg的移动端例子：Android HelloWorld](http://blog.csdn.net/leixiaohua1020/article/details/47008825)

[最简单的基于FFmpeg的移动端例子：Android 视频解码器](http://blog.csdn.net/leixiaohua1020/article/details/47010637)

[最简单的基于FFmpeg的移动端例子：Android 视频解码器-单个库版](http://blog.csdn.net/leixiaohua1020/article/details/47011021)

[最简单的基于FFmpeg的移动端例子：Android 推流器](http://blog.csdn.net/leixiaohua1020/article/details/47056051)

[最简单的基于FFmpeg的移动端例子：Android 视频转码器](http://blog.csdn.net/leixiaohua1020/article/details/47056365)

[最简单的基于FFmpeg的移动端例子附件：Android 自带播放器](http://blog.csdn.net/leixiaohua1020/article/details/47056733)

[最简单的基于FFmpeg的移动端例子附件：SDL Android HelloWorld](http://blog.csdn.net/leixiaohua1020/article/details/47059553)

[最简单的基于FFmpeg的移动端例子：IOS HelloWorld](http://blog.csdn.net/leixiaohua1020/article/details/47071547)

[最简单的基于FFmpeg的移动端例子：IOS 视频解码器](http://blog.csdn.net/leixiaohua1020/article/details/47072257)

[最简单的基于FFmpeg的移动端例子：IOS 推流器](http://blog.csdn.net/leixiaohua1020/article/details/47072519)

[最简单的基于FFmpeg的移动端例子：IOS 视频转码器](http://blog.csdn.net/leixiaohua1020/article/details/47072673)

[最简单的基于FFmpeg的移动端例子附件：IOS自带播放器](http://blog.csdn.net/leixiaohua1020/article/details/47072861)

[最简单的基于FFmpeg的移动端例子：Windows Phone HelloWorld](http://blog.csdn.net/leixiaohua1020/article/details/47191283)

=====================================================


本文记录一个安卓平台下SDL的例子。该例子读取并显示了一张BMP图片，通过该例子可以了解SDL在安卓平台下的使用。本文中使用的SDL版本为2.0。

![](https://img-blog.csdn.net/20150801182633054)





## Android程序SDL2类库使用说明

简单记录一下Android程序使用SDL2类库的方法。这部分的信息可以参考SDL2源代码目录中的README-android.txt文件。SDL2使用的步骤可以分为以下步骤：




### （1）配置Android工程
**a) 新建Android工程**

可以直接使用SDL2源代码目录中的android-project工程，也可以自己新建工程。如果自己新建工程，则需要拷贝android-project工程中src目录下的SDLActivity.java文件。该文件中的SDLActivity定义比较长，目前还没有研究过。
**b) 拷贝SDL2源代码**
将SDL2源代码目录中下列内容拷贝至Android工程的jni目录下：

> *src目录include目录Android.mk*

### （2）编译C语言代码
**a) 编写C语言代码**

在Android工程的jni目录下新建C语言文件并编写调用SDL2的代码。在这里需要注意C语言代码的主函数名称和命令行程序是一样的，依然是“main(int argc, char *argv[])”。
**b) 修改Android.mk文件**

在Android.mk文件后面追加一段代码，用来编译一个libSDL2main.so的库。该库可以调用自己的C语言程序。在这里需要注意，“libSDL2main”这个名称是可以随意起的，但是要和SDLActivity中LoadLibrary()函数中的库名称相对应（默认名称应该是libmain.so）。代码内容如下所示。



```
#libSDL2main=======================================
# Lei Xiaohua
include $(CLEAR_VARS)
LOCAL_MODULE := SDL2main
SDL_PATH := ./
LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include
# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
	$(SDL_PATH)/simplest_showbmp.c
LOCAL_SHARED_LIBRARIES := SDL2
LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog
include $(BUILD_SHARED_LIBRARY)
```


PS：上述代码修改自SDL2源代码目录中android-project工程的jni\src目录下的Android.mk文件。

**c) 编写Application.mk文件（可选）**
Application.mk中的APP_ABI设定了编译后库文件支持的指令集，默认使用“armeabi”，这里使用默认值就可以了。
**d) 运行ndk-build**
编写完C语言代码和Android.mk之后就可以运行ndk-build命令进行编译了。ndk-build命令运行成功后，会在根目录下的“libs/armeabi”目录中生成2个库文件：

> *libSDL2.solibSDL2main.so*
接下来就可以在Android手机或者虚拟机上对整个Android工程进行测试了。


## 源代码

项目的目录结构如图所示。Java源代码位于src目录，而C代码位于jni目录。

![](https://img-blog.csdn.net/20150725201556042)


Android程序Java端代码位于src\org\libsdl\app\SDLActivity.java。该Activity取自于SDL2源代码目录中的android-project工程，在这里不再记录。

jni目录中的src和include文件夹分别是SDL2的源代码目录中的src文件夹和include文件夹。

C语言端源代码位于jni/simplest_showbmp.c，如下所示。该源代码读取了存储卡中的一张test.bmp文件，并显示出来。

```cpp
/**
 * 移植SDL到安卓平台的HelloWorld程序
 * Simplest SDL Android Helloworld
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 *
 * 本程序是移植SDL到安卓平台的最简单程序。它可以读取并显示一张BMP图片。
 *
 * This software is the simplest program transplant SDL2 to Android platform.
 * It shows a BMP file on the screen.
 */

#ifdef __ANDROID__

#include <jni.h>
#include <android/log.h>
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO , "(^_^)", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , "(^_^)", __VA_ARGS__)
#else
#define LOGE(format, ...)  printf("(>_<) " format "\n", ##__VA_ARGS__)
#define LOGI(format, ...)  printf("(^_^) " format "\n", ##__VA_ARGS__)
#endif

#include "SDL.h"
#include "SDL_log.h"
#include "SDL_main.h"

int main(int argc, char *argv[]) {
	struct SDL_Window *window = NULL;
	struct SDL_Renderer *render = NULL;
	struct SDL_Surface *bmp = NULL;
	struct SDL_Texture *texture = NULL;

    char *filepath = "/storage/emulated/0/test.bmp";

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == -1) {
        LOGE("SDL_Init failed %s", SDL_GetError());
    }

    window = SDL_CreateWindow("SDL HelloWorld!", 100, 100, 640, 480,
            SDL_WINDOW_SHOWN);
    if (window == NULL) {
        LOGE("SDL_CreateWindow failed  %s", SDL_GetError());
    }

    render = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (render == NULL) {
        LOGE("SDL_CreateRenderer failed  %s", SDL_GetError());
    }

    bmp = SDL_LoadBMP(filepath);
    if (bmp == NULL) {
        LOGE("SDL_LoadBMP failed: %s", SDL_GetError());
    }

    texture = SDL_CreateTextureFromSurface(render, bmp);
    SDL_FreeSurface(bmp);

    SDL_RenderClear(render);
    SDL_RenderCopy(render, texture, NULL, NULL);
    SDL_RenderPresent(render);

    SDL_Delay(10000);

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);

    //Quit SDL
    SDL_Quit();
    return 0;
}
```

Android.mk文件位于jni/Android.mk，如下所示。

```
LOCAL_PATH := $(call my-dir)

###########################
#
# SDL shared library
#
###########################

include $(CLEAR_VARS)

LOCAL_MODULE := SDL2

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
	$(wildcard $(LOCAL_PATH)/src/*.c) \
	$(wildcard $(LOCAL_PATH)/src/audio/*.c) \
	$(wildcard $(LOCAL_PATH)/src/audio/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/audio/dummy/*.c) \
	$(LOCAL_PATH)/src/atomic/SDL_atomic.c \
	$(LOCAL_PATH)/src/atomic/SDL_spinlock.c.arm \
	$(wildcard $(LOCAL_PATH)/src/core/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/cpuinfo/*.c) \
	$(wildcard $(LOCAL_PATH)/src/dynapi/*.c) \
	$(wildcard $(LOCAL_PATH)/src/events/*.c) \
	$(wildcard $(LOCAL_PATH)/src/file/*.c) \
	$(wildcard $(LOCAL_PATH)/src/haptic/*.c) \
	$(wildcard $(LOCAL_PATH)/src/haptic/dummy/*.c) \
	$(wildcard $(LOCAL_PATH)/src/joystick/*.c) \
	$(wildcard $(LOCAL_PATH)/src/joystick/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/loadso/dlopen/*.c) \
	$(wildcard $(LOCAL_PATH)/src/power/*.c) \
	$(wildcard $(LOCAL_PATH)/src/power/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/filesystem/dummy/*.c) \
	$(wildcard $(LOCAL_PATH)/src/render/*.c) \
	$(wildcard $(LOCAL_PATH)/src/render/*/*.c) \
	$(wildcard $(LOCAL_PATH)/src/stdlib/*.c) \
	$(wildcard $(LOCAL_PATH)/src/thread/*.c) \
	$(wildcard $(LOCAL_PATH)/src/thread/pthread/*.c) \
	$(wildcard $(LOCAL_PATH)/src/timer/*.c) \
	$(wildcard $(LOCAL_PATH)/src/timer/unix/*.c) \
	$(wildcard $(LOCAL_PATH)/src/video/*.c) \
	$(wildcard $(LOCAL_PATH)/src/video/android/*.c) \
    $(wildcard $(LOCAL_PATH)/src/test/*.c))

LOCAL_CFLAGS += -DGL_GLEXT_PROTOTYPES
LOCAL_LDLIBS := -ldl -lGLESv1_CM -lGLESv2 -llog -landroid

include $(BUILD_SHARED_LIBRARY)

###########################
#
# SDL static library
#
###########################

LOCAL_MODULE := SDL2_static

LOCAL_MODULE_FILENAME := libSDL2

LOCAL_SRC_FILES += $(LOCAL_PATH)/src/main/android/SDL_android_main.c

LOCAL_LDLIBS := 
LOCAL_EXPORT_LDLIBS := -Wl,--undefined=Java_org_libsdl_app_SDLActivity_nativeInit -ldl -lGLESv1_CM -lGLESv2 -llog -landroid

include $(BUILD_STATIC_LIBRARY)


#libSDL2main=======================================
# Lei Xiaohua
include $(CLEAR_VARS)
LOCAL_MODULE := SDL2main
SDL_PATH := ./
LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include
# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
	$(SDL_PATH)/simplest_showbmp.c
LOCAL_SHARED_LIBRARIES := SDL2
LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog
include $(BUILD_SHARED_LIBRARY)
```


## 运行结果

App在手机上运行后的结果如下图所示。可以看出App读取了test.bmp文件并且显示出来。

注意需要把BMP文件拷贝至存储卡相应的目录中。程序默认情况下会读取根目录下的“test.bmp”文件。


![](https://img-blog.csdn.net/20150725201709877)



## 下载

**simplest ffmpeg mobile项目主页**

Github：[https://github.com/leixiaohua1020/simplest_ffmpeg_mobile](https://github.com/leixiaohua1020/simplest_ffmpeg_mobile)
开源中国：[https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_mobile](https://git.oschina.net/leixiaohua1020/simplest_ffmpeg_mobile)

SourceForge：[https://sourceforge.net/projects/simplestffmpegmobile/](https://sourceforge.net/projects/simplestffmpegmobile/)




CSDN工程下载地址：[http://download.csdn.net/detail/leixiaohua1020/8924391](http://download.csdn.net/detail/leixiaohua1020/8924391)


本解决方案包含了使用FFmpeg在移动端处理多媒体的各种例子：

> [Android]
simplest_android_player: 基于安卓接口的视频播放器
simplest_ffmpeg_android_helloworld: 安卓平台下基于FFmpeg的HelloWorld程序
simplest_ffmpeg_android_decoder: 安卓平台下最简单的基于FFmpeg的视频解码器
simplest_ffmpeg_android_decoder_onelib: 安卓平台下最简单的基于FFmpeg的视频解码器-单库版
simplest_ffmpeg_android_streamer: 安卓平台下最简单的基于FFmpeg的推流器
simplest_ffmpeg_android_transcoder: 安卓平台下移植的FFmpeg命令行工具
simplest_sdl_android_helloworld: 移植SDL到安卓平台的最简单程序
[IOS]
simplest_ios_player: 基于IOS接口的视频播放器
simplest_ffmpeg_ios_helloworld: IOS平台下基于FFmpeg的HelloWorld程序
simplest_ffmpeg_ios_decoder: IOS平台下最简单的基于FFmpeg的视频解码器
simplest_ffmpeg_ios_streamer: IOS平台下最简单的基于FFmpeg的推流器
simplest_ffmpeg_ios_transcoder: IOS平台下移植的ffmpeg.c命令行工具
simplest_sdl_ios_helloworld: 移植SDL到IOS平台的最简单程序








