# 简述RTMPDump与编译移植 - DoubleLi - 博客园






[RTMPDump主页](http://rtmpdump.mplayerhq.hu/) ，RTMPDump库主要包含三部分：

1、一个基本的客户端程序

2、两个服务器程序（rtmpsrv、rtmpsuck）

3、一个支持rtmp协议的库—librtmp

下载RTMPDump最新源码，可以通过git拉取master分支上的最新代码：
git clone git://git.ffmpeg.org/rtmpdump

或者下载以前版本的代码— [Download old version](http://rtmpdump.mplayerhq.hu/download/)

下面主要介绍librtmp和librtmp的编译

### librtmp概述

librtmp库提供了大量客户端函数和少部分的服务器端的函数用来支持RTMP、RTMPT（RTMP使用Http通道），RTMPE（加密的RTMP），RTMPS（基于SSL/TLS的RTMP）和RTMPTE、RTMPTS（使用Http通道的加密和基于SSL/TLS的RTMP）协议

使用librtmp库进行的流媒体交互都是使用 FLV 封包格式进行传输的，当然编码不限定，不过为了拉流端同时也支持 HLS 协议，最好使用 H264 编码视频和 AAC 编码音频数据

使用librtmp通过 `RTMP[T][E|S]` 协议进行一次流媒体交互的基本过程为：

#### 推流端：
- RTMP_Alloc() -创建一个RTMP会话句柄
- RTMP_Init() -初始化RTMP句柄
- RTMP_SetupURL() -设置推流RTMP Url
- RTMP_EnableWrite() -配置该媒体流可进行写操作
- RTMP_Connect() -建立RTMP网络连接
- RTMP_ConnectStream() -建立RTMP流连接
- RTMP_Write() -对该媒体流进行写入流媒体信息
- RTMP_Close() -关闭RTMP连接
- RTMP_Free() -释放此次连接的会话句柄

#### 播放端：
- RTMP_Alloc() -创建一个RTMP会话句柄
- RTMP_Init() -初始化RTMP句柄
- RTMP_SetupURL() -设置拉流RTMP Url
- RTMP_Connect() -建立RTMP网络连接
- RTMP_ConnectStream() -建立RTMP流连接
- RTMP_Read() -读取媒体流数据，返回0时则媒体流读取完毕
- RTMP_Pause() -暂停读取媒体流数据或取消暂停（options）
- RTMP_Seek() -可通过它改变流播放的位置（options）
- RTMP_Close() -当媒体流读取完毕后进行RTMP连接的关闭
- RTMP_Free() -释放此次连接的会话句柄

上述方法的具体实现可以在librtmp包中的 `rtmp.c` 文件中找到，同时，它还提供了其它众多方法来处理RTMP的流媒体交互过程

### librtmp的RTMP URL格式

一个基本的RTMP URL格式为：
rtmp[t][e|s]://host[:port][/app[/playpath]]

和http类似，不过某些地方不一样，下面简单说说：

scheme为rtmp或者rtmp协议的诸多变种之一，这就不需要多解释了，像websocket协议scheme为ws或者wss，http协议scheme为http或https。

host为主机名，后面紧接着为端口号，如果不指定，rtmp协议默认端口号为1935.

app代表服务器上的一个Application应用程序，这个名称是由服务器定义的，如：live。

playpath则是表示该Application下的一个媒体流的路径，如：room100。

当然也可以追加额外的参数，只不过这和我们使用http协议追加参数的方式不同，librtmp是采取空格为分隔符，通过追加”key=value”的方式进行追加额外的参数，如：
"rtmp://localhost:1935/live/room100 app=happylive"

这就追加了额外的key为app，value为happylive的参数，表示使用happylive应用来连接而代替rtmp url中的live应用

关于librtmp的更多额外的参数配置，可以看 [librtmp文档](http://rtmpdump.mplayerhq.hu/librtmp.3.html)

### librtmp编译

由于librtmp默认依赖zlib和openssl这两个库，所以编译时需要链接这两个库，由于NDK自带了zlib库，所以不需要单独去下载和编译，在编译librtmp时配置 `Android.mk` 文件的 `LOCAL_LDLIBS` 属性为 `-lz` 链接即可，编译时会在指定的platforms的arch-xxx/usr/lib/目录下搜索

编译librtmp，在librtmp目录下新建Android.mk文件，内容如下：
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

# 编译模块输出的名称
LOCAL_MODULE := rtmp

# 编译所需的头文件路径
LOCAL_C_INCLUDES += \
$(LOCAL_PATH)/
#$(LOCAL_PATH)/../openssl/include/ #链接openssl库

# 编译所需的源文件路径
LOCAL_SRC_FILES := \
amf.c       \
log.c       \
parseurl.c  \
rtmp.c      \
hashswf.c

# 编译时候需要链接的动态库库存在静态库形式，将会链接到对应的静态库而不是动态库
# LOCAL_FORCE_STATIC_EXECUTABLE := true

# 添加编译配置选项，一些常用编译配置：
# -fPIC:编译位置无关的代码，一般用于编译动态库
# -02:编译优化程度
# -lxxx:链接名为libxxx库
# -I添加编译器搜索-l指定的库文件的搜索
# -D:加入宏定义，如：NO_CRYPTO，不使用加密
LOCAL_CFLAGS += -DRTMPDUMP_VERSION=v2.4 -DNO_CRYPTO -Wall -fPIC -O2 
#LOCAL_CFLAGS += -I$(LOCAL_PATH)/../openssl -lssl -lcrypto #可选添加openssl支持，需去除-DNO_CRYPTO

# 添加编译时需要链接器链接的那些存在于NDK目录下的其它库，所链接库的以“-lxxx”格式命名，如：-lz 表示编译时会加载libz.so库
LOCAL_LDLIBS += -lz

# include $(BUILD_STATIC_LIBRARY)
include $(BUILD_SHARED_LIBRARY)

关于mk文件的编写和参数配置，可以阅读 [mk官网文档](https://developer.android.com/ndk/guides/android_mk.html) ，上面也给出了全部编译时参数配置的注释，由于上面配置加入了 `NO_CRYPTO` 定义，没有链接openssl库，需要的可以自己加或者使用 [PolarSSL](https://tls.mbed.org/) 、[GnuTLS](http://www.gnu.org/software/gnutls/) 来替代 [OpenSSL](https://www.openssl.org/)

然后在rtmpdump目录下也建立个Android.mk文件，内容为：
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
subdirs := $(addprefix $(LOCAL_PATH)/,$(addsuffix /Android.mk, \
librtmp \
))
include $(subdirs)

其实不需要这个也行，不过加入该文件目的是为了能够统一rtmpdump文件夹下和子目录中的mk的编译运行入口，这样只需调用这一个mk文件即可，上面配置的只会运行librtmp目录下的mk文件，如果需要加入其它的，则在 `subdirs` 中加入即可，如：
subdirs := $(addprefix $(LOCAL_PATH)/,$(addsuffix /Android.mk, \
librtmp \
otherdir \
))

最后在rtmpdump中新建jni目录，新建Application.mk文件，内容为：
NDK_TOOLCHAIN_VERSION := 4.9
APP_PLATFORM := android-14
APP_ABI := all
APP_OPTIM := release
APP_PROJECT_PATH := $(shell pwd)
APP_BUILD_SCRIPT := $(APP_PROJECT_PATH)/Android.mk

主要是对编译时的编译配置进行配置， `APP_ABI := all` 表示支持当前平台支持的的全部CPU架构

然后新建一个开始执行编译的脚本文件，内容为：
#!/bin/bash
NDK=$HOME/Library/Android/sdk/ndk-bundle
$NDK/ndk-build NDK_APPLICATION_MK=./jni/Application.mk NDK_PROJECT_PATH=./

其中NDK路径替换为自己的即可，开始编译：
cd rtmpdump
./build_librtmp.sh

成功后，可以在rtmpdump目录下看到obj和libs两个文件夹，libs存放着编译出来的动态库，而obj则是存放静态库，这取决于所配置的是 `BUILD_SHARED_LIBRARY` 还是 `BUILD_STATIC_LIBRARY`

当然其它库也可以用这种方式进行编译

## 编译支持librtmp的FFmpeg动态链接库

由于FFmpeg本身默认自带就支持rtmp、rtmpt协议，不过也可以扩展以便支持rtmp协议的多种变种协议（rtmps、rtmpt、rtmpts、rtmpte和rtmpe协议），在FFmpeg加入openssl库的支持，可以支持rtmps、rtmpts协议，加入libgcrypt库的支持，可以支持rtmpe、rtmpte协议，这样支持相对麻烦，可以直接使用librtmp来支持rtmp以及它的多种变种协议，可通过链接librtmp库来支持









