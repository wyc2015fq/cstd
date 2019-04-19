# OpenH264编译ffmpeg android - starRTC免费im直播会议一对一视频 - CSDN博客
2018年11月14日 13:52:21[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：132

思科的
安装NASM
git clone [https://github.com/cisco/openh264.git](https://github.com/cisco/openh264.git)
Android Builds
install android sdk and ndk.
export PATH=**ANDROID_SDK**/tools:$PATH
make OS=android NDKROOT=**ANDROID_NDK** TARGET=**ANDROID_TARGET**
Valid **ANDROID_TARGET** can be found in **ANDROID_SDK**/platforms, such as android-12.
You can also set ARCH, NDKLEVELaccording to your device and NDK version. 
ARC:arm, arm64, x86 and x86_64
NDKLEVEL specifies android api level, the default is 12.Available possibilities can be found in **ANDROID_NDK**/platforms,
By default these commands build for the armeabi-v7a
- codec - encoder, decoder, console (test app), build (makefile, vcproj)
- build - scripts for Makefile build system
- test - GTest unittest files
- testbin - autobuild scripts, test app config files
- res - yuv and bitstream test files
# armeabi-v7a
make OS=android NDKROOT=$NDK_ROOT TARGET=android-21 ARCH=arm NDKLEVEL=21 clean
make OS=android NDKROOT=$NDK_ROOT TARGET=android-17 ARCH=arm NDKLEVEL=17
[https://blog.ibaoger.com/2017/07/10/compile-openh264-for-android/index.html](https://blog.ibaoger.com/2017/07/10/compile-openh264-for-android/index.html)
NDK 从 level21 开始支持 64位架构，因此 TARGET 低于 android-21 的将无法编译 arm64、x86_64、mips64 库
 mips、mips64 两种架构未做优化处理
生成的文件位于 ~/openh264 目录下，共计5个静态库，1个动态库
libcommon.a
libdecoder.a
libencoder.a
libopenh264.a
libprocessing.a
libopenh264.so
Invalid or unsupported command "update project -t android-17 -p ."
看makefile
"cd ./test/build/android && $(NDKROOT)/ndk-build -B APP_ABI=$(APP_ABI) && android update project -t $(TARGET) -p . && ant debug"
[https://github.com/cisco/openh264/issues/2994](https://github.com/cisco/openh264/issues/2994)
this error caused by using a higher SDK tool to build encoder & decoder demo program. The "android" tool does not support "update project" in the higher SDK version. Actually, the openh264 libraries have been built successfully.
实际已经编译成功了
ffmpeg
./configure --list-decoders和./configure --list-encoders都有
libopenh264
--enable-libopenh264
ffmpeg中
#include <wels/codec_api.h>
#include <wels/codec_ver.h>
vim meson.build
foreach t : ['', '-static']
pkgconf = configuration_data()
pkgconf.set('prefix', join_paths(get_option('prefix')))
pkgconf.set('VERSION', meson.project_version())
if t == '-static'
do_install = false
pkgconf.set('LIBS', '-lstdc++ -lpthread -lm')
pkgconf.set('LIBS_PRIVATE', '')
else
do_install = true
pkgconf.set('LIBS', '')
pkgconf.set('LIBS_PRIVATE', '-lstdc++ -lpthread -lm')
endif
undefined reference to '__dso_handle'
make install PREFIX=/some/path
make install-static PREFIX=/root/starRTC_ffmpeg/ffmpeg_compile/build_result/armeabi-v7a-neon
undefined reference to XXX_neonerrors at the stage of FFmpeg configuring.
In the end, I set USE_ASM to no and everything was okay.
Makefile
ifeq ($(BUILDTYPE), Release)
CFLAGS += $(CFLAGS_OPT)
USE_ASM = No
else
CFLAGS += $(CFLAGS_DEBUG)
USE_ASM = No
endif
arm-linux-androideabi/bin/ld: error: cannot find -lpthread
改为 -pthread
因为android的ndk虽然有pthread.h,但是没有libpthread.a,集成到libc.a里了-lc
[https://blog.csdn.net/u014006343/article/details/46545831](https://blog.csdn.net/u014006343/article/details/46545831)
