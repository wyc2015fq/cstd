# windows+msys2编译android版ijkplayer - tifentan的专栏 - CSDN博客

2018年06月19日 09:52:04[露蛇](https://me.csdn.net/tifentan)阅读数：235


近来想研究一下ijkplayer，发现在windows上编译有很多的坑。搞了一天终于能运行example，过程就像排雷一般，但也对整个项目结构了解了一番。记录下来！ 

可能有些坑没记得写下来，欢迎留言。 
[下载](https://download.csdn.net/download/tifentan/10485420)我改过的文件。

## 准备材料
- [msys2安装包](https://www.msys2.org)
- ndk（我用的14b）
- 安装android studio

## 环境配置

安装完msys2后从开始菜单启动msys2 minggw 64-bit,理论上其他两个也可以。 

可参考这个[https://segmentfault.com/n/1330000004236998](https://segmentfault.com/n/1330000004236998)进行基本环境配置， 

然后使用pacman安装git，make，yasm，python,例如：
`pacman -S git`
然后安装上ndk。

## 下载项目

先下载项目。

```
git clone https://github.com/Bilibili/ijkplayer.git ijkplayer-android
cd ijkplayer-android
git checkout -B latest k0.8.8
```

再下载依赖库ffmpeg，libyuv，soundtouch，这里脚本中为每个cpu架构都下载了一个对应的ffmpeg，但我对比过都没区别。
`./init-android.sh`
创建一个文件ndk-source.txt，内容为

```
export ANDROID_NDK=你自己的ndk目录（E:/ndk/android-ndk-r14b）
export ANDROID_SDK=你自己的sdk目录(C:/Users/Administrator/AppData/Local/Android/Sdk)
```

注意修改成你自己的ndk和sdk目录。

## 编译ffmpeg

先source 一下刚才的ndk-source.txt
`source ./ndk-source.txt`
往下都只编译armv7a架构，我这么做是为了方便排雷，其它的参考修改应该可行。 
**1.修改android/contrib/tools/do-compile-ffmpeg.sh**

我选择level16，其它估计也可行
`FF_ANDROID_PLATFORM=android-16`
修改armv7a分支下的（其它架构的可以同样修改）
`FF_CROSS_PREFIX=$FF_BUILD_ROOT/build/$FF_BUILD_NAME/toolchain/bin/arm-linux-androideabi`
还有以下

```
export COMMON_FF_CFG_FLAGS=
. $FF_BUILD_ROOT/../../config/module-lite.sh
```

```
#export PATH=$FF_TOOLCHAIN_PATH/bin/:$PATH
#export CC="ccache ${FF_CROSS_PREFIX}-gcc"
#export CC="${FF_CROSS_PREFIX}-gcc"
#export LD=${FF_CROSS_PREFIX}-ld
#export AR=${FF_CROSS_PREFIX}-ar
#export STRIP=${FF_CROSS_PREFIX}-strip
```

```bash
./configure $FF_CFG_FLAGS --sysroot=$FF_SYSROOT \
        --extra-cflags="$FF_CFLAGS $FF_EXTRA_CFLAGS" \
        --extra-ldflags="$FF_DEP_LIBS $FF_EXTRA_LDFLAGS"
```

```
$FF_CROSS_PREFIX-gcc -lm -lz -shared --sysroot=$FF_SYSROOT -Wl,--no-undefined -Wl,-z,noexecstack $FF_EXTRA_LDFLAGS \
    -Wl,-soname,libijkffmpeg.so \
    $FF_C_OBJ_FILES \
    $FF_ASM_OBJ_FILES \
    $FF_DEP_LIBS \
    -o $FF_PREFIX/libijkffmpeg.so
```

**2.修改你的ndk安装目录下的build/tools/make-standalone-toolchain.sh**

```
python `dirname $0`/make_standalone_toolchain.py \
    --arch $ARCH $PLATFORM_ARG --stl $STL $INSTALL_ARG $FORCE_ARG
```

**3.修改你ndk安装目录下的build/tools/make-standalone-toolchain.py**

```
THIS_DIR = os.path.dirname(__file__)
NDK_DIR = os.path.join(THIS_DIR, '../..')
```

```python
def get_host_tag_or_die():
    """Return the host tag for this platform. Die if not supported."""
    if platform.system() == 'Linux':
        return 'linux-x86_64'
    elif platform.system() == 'Darwin':
        return 'darwin-x86_64'
    #elif platform.system() == 'Windows':
    else:
        host_tag = 'windows-x86_64'
        if not os.path.exists(os.path.join(NDK_DIR, 'prebuilt', host_tag)):
            host_tag = 'windows'
        print(host_tag)
        return host_tag
    sys.exit('Unsupported platform: ' + platform.system())
```

**最后，执行**

```
cd android/contrib
./compile-ffmpeg.sh clean
./compile-ffmpeg.sh armv7a
```

## 编译ijk源码

**1.修改android/compile-ijk.sh**

```bash
do_sub_cmd () {
    SUB_CMD=$1
    echo param1 $1
    if [ -L "./android-ndk-prof" ]; then
        echo remove android-ndk-prof
        rm android-ndk-prof
    fi
    echo PARAM_SUB_CMD $PARAM_SUB_CMD
    echo SUB_CMD $SUB_CMD
    echo pwd `pwd`
    if [ "$PARAM_SUB_CMD" = 'prof' ]; then
        echo 'profiler build: YES';
        ln -s ../../../../../../ijkprof/android-ndk-profiler/jni android-ndk-prof
    else
        echo 'profiler build: NO';
        #ln -s ../../../../../../ijkprof/android-ndk-profiler-dummy/jni android-ndk-prof
        if [ ! -d "android-ndk-prof" ]; then
            mkdir -p android-ndk-prof
            cp -R -f ../../../../../../ijkprof/android-ndk-profiler-dummy/jni/* android-ndk-prof
        fi
    fi

    if [ ! -d "ijkmedia" ]; then
            mkdir -p ijkmedia
            cp -R -f ../../../../../../ijkmedia/* ijkmedia
    fi

    ./ijkmedia/ijkplayer/version.sh ./ijkmedia/ijkplayer/ ijkversion.h

    case $SUB_CMD in
        prof)
            $ANDROID_NDK/ndk-build.cmd $FF_MAKEFLAGS
        ;;
        clean)
            $ANDROID_NDK/ndk-build.cmd clean
        ;;
        rebuild)
            $ANDROID_NDK/ndk-build.cmd clean
            $ANDROID_NDK/ndk-build.cmd $FF_MAKEFLAGS
        ;;
        *)
            echo before ndk-build
            $ANDROID_NDK/ndk-build.cmd $FF_MAKEFLAGS
        ;;
    esac
}
```

然后先执行一下，目的是把ijkmedia项目拷贝进来，会看到在ndk-build出现错误。

```bash
cd ..
./compile-ijk.sh armv7a
```

然后按以下修改android/ijkplayer/ijkplayer-armv7a/src/main/jni/下的各种android.mk文件（其他架构的可参考修改） 
**2.修改\src\main\jni\Android.mk**

```
#MY_APP_JNI_ROOT := $(realpath $(LOCAL_PATH))
#MY_APP_PRJ_ROOT := $(realpath $(MY_APP_JNI_ROOT)/..)
#MY_APP_ANDROID_ROOT := $(realpath $(MY_APP_PRJ_ROOT)/../../../..)
MY_APP_JNI_ROOT := $(LOCAL_PATH)
MY_APP_PRJ_ROOT := $(MY_APP_JNI_ROOT)/..
MY_APP_ANDROID_ROOT := $(MY_APP_PRJ_ROOT)/../../../..

$(info MY_APP_JNI_ROOT $(MY_APP_JNI_ROOT)) 
#$(info MY_APP_PRJ_ROOT $(MY_APP_PRJ_ROOT)) 
#$(info MY_APP_ANDROID_ROOT $(MY_APP_ANDROID_ROOT)) 

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
#MY_APP_FFMPEG_OUTPUT_PATH := $(realpath $(MY_APP_ANDROID_ROOT)/contrib/build/ffmpeg-armv7a/output)
#MY_APP_FFMPEG_INCLUDE_PATH := $(realpath $(MY_APP_FFMPEG_OUTPUT_PATH)/include)
MY_APP_FFMPEG_OUTPUT_PATH := $(MY_APP_ANDROID_ROOT)/contrib/build/ffmpeg-armv7a/output
MY_APP_FFMPEG_INCLUDE_PATH := $(MY_APP_FFMPEG_OUTPUT_PATH)/include
```

**3.修改src\main\jni\ijkmedia\ijkplayer\Android.mk**

```
#
# Copyright (c) 2013 Bilibili
# Copyright (c) 2013 Zhang Rui <bbcallen@gmail.com>
#
# This file is part of ijkPlayer.
#
# ijkPlayer is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# ijkPlayer is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with ijkPlayer; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

$(info ijkmedia/ijkplayer/android.mk) 
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
# -mfloat-abi=soft is a workaround for FP register corruption on Exynos 4210
# http://www.spinics.net/lists/arm-kernel/msg368417.html
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_CFLAGS += -mfloat-abi=soft
endif
LOCAL_CFLAGS += -std=c99
LOCAL_LDLIBS += -llog -landroid

LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/..
LOCAL_C_INCLUDES += $(MY_APP_FFMPEG_INCLUDE_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../ijkj4a

LOCAL_SRC_FILES += ff_cmdutils.c
LOCAL_SRC_FILES += ff_ffplay.c
LOCAL_SRC_FILES += ff_ffpipeline.c
LOCAL_SRC_FILES += ff_ffpipenode.c
LOCAL_SRC_FILES += ijkmeta.c
LOCAL_SRC_FILES += ijkplayer.c

LOCAL_SRC_FILES += pipeline/ffpipeline_ffplay.c
LOCAL_SRC_FILES += pipeline/ffpipenode_ffplay_vdec.c

LOCAL_SRC_FILES += android/ffmpeg_api_jni.c
LOCAL_SRC_FILES += android/ijkplayer_android.c
LOCAL_SRC_FILES += android/ijkplayer_jni.c

LOCAL_SRC_FILES += android/pipeline/ffpipeline_android.c
LOCAL_SRC_FILES += android/pipeline/ffpipenode_android_mediacodec_vdec.c

LOCAL_SRC_FILES += ijkavformat/allformats.c
LOCAL_SRC_FILES += ijkavformat/ijklivehook.c
LOCAL_SRC_FILES += ijkavformat/ijkmediadatasource.c
LOCAL_SRC_FILES += ijkavformat/ijkio.c
LOCAL_SRC_FILES += ijkavformat/ijkiomanager.c
LOCAL_SRC_FILES += ijkavformat/ijkiocache.c
LOCAL_SRC_FILES += ijkavformat/ijkioffio.c
LOCAL_SRC_FILES += ijkavformat/ijkioandroidio.c
LOCAL_SRC_FILES += ijkavformat/ijkioprotocol.c
LOCAL_SRC_FILES += ijkavformat/ijkioapplication.c
LOCAL_SRC_FILES += ijkavformat/ijkiourlhook.c

LOCAL_SRC_FILES  += ijkavformat/ijkasync.c
LOCAL_SRC_FILES  += ijkavformat/ijkurlhook.c
LOCAL_SRC_FILES  += ijkavformat/ijklongurl.c
LOCAL_SRC_FILES  += ijkavformat/ijksegment.c

LOCAL_SRC_FILES += ijkavutil/ijkdict.c
LOCAL_SRC_FILES += ijkavutil/ijkutils.c
LOCAL_SRC_FILES += ijkavutil/ijkthreadpool.c
LOCAL_SRC_FILES += ijkavutil/ijktree.c
LOCAL_SRC_FILES += ijkavutil/ijkfifo.c
LOCAL_SRC_FILES += ijkavutil/ijkstl.cpp

LOCAL_SHARED_LIBRARIES := ijkffmpeg ijksdl
LOCAL_STATIC_LIBRARIES := android-ndk-profiler ijksoundtouch

LOCAL_MODULE := ijkplayer

#$(info VERSION_SH $(LOCAL_PATH)/version.sh) 

VERSION_SH  = $(LOCAL_PATH)/version.sh
VERSION_H   = ijkversion.h
#$(info $(shell ($(VERSION_SH) $(LOCAL_PATH) $(VERSION_H))))

include $(BUILD_SHARED_LIBRARY)
```

**4.修改src\main\jni\ijkmedia\ijksdl\Android.mk**

```
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/..
LOCAL_C_INCLUDES += $(MY_APP_FFMPEG_INCLUDE_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../ijkyuv/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../ijkj4a
```

**5修改src\main\jni\ijkmedia\ijksoundtouch\Android.mk**

```
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/..
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/
```

**最后执行**
`./compile-ijk.sh armv7a`
至此build成功了，在src\main\libs\armeabi-v7a下可看到build好的库了。

## 运行demo

可以直接用as打开android/ijkplayer下的项目进行调试。 
![这里写图片描述](https://img-blog.csdn.net/20180619091307451?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RpZmVudGFu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后可以开始看代码了哈。

## 后记

如果嫌麻烦，在linux下按github的流程走其实是个很好的选择，我选择这么干一个是：一开始在网上搜win下编译ijk无果后心有不甘，另一个是想着在排雷过程中能对整个项目结构进行了解。

