# 如何将Android应用程序编译至image/rom中 - xqhrs232的专栏 - CSDN博客
2011年11月19日 12:30:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：553标签：[android																[jni																[include																[module																[build																[path](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=build&t=blog)](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/silvervi/article/details/6315606](http://blog.csdn.net/silvervi/article/details/6315606)
有时我们希望将自己的Android应用程序编译至image/rom中，这样程序将被安装至/system/app目录下，并且不能被卸载。下面我以自己的程序TestJniApp为例子，仿照源代码中自带的SimpleJNI，演示一下怎样将自己的应用程序编译至rom中。
# **1.    搭建编译环境**
编译环境： Ubuntu 10.10
Android版本：Android 2.2
编译过程中可能需要在Ubuntu上安装必要的一些软件，我安装过的包含如下软件，不同的系统可能会有差别：
jdk6(Android官方建议装jdk5，但是我在编译时会遇到Java override问题，改用6没有任何问题), bison, lib64z1-dev, libasound2-dev, flex, gperf, libncurses5-dev
# 2.    应用程序存放目录 
      SimpleJNI是采用Android NDK和Java编写的程序，包含apk和so库文件，它的源代码在source tree的development/samples/目录下。
      实际上package在编译时所放的目录并没有明确限定，编译后apk和so存放的位置是根据目录下Android.mk所指定的编译类型所决定的，例如：
- SimpleJNI根目录下的Android.mk中有一行include $(BUILD_PACKAGE)，表示将该目录下的模块编译成package，即apk文件，默认存放的位置为/system/app。
- SimpleJNI/jni目录下的Android.mk中有一行为include $(BUILD_SHARED_LIBRARY)，表示将该目录下的native.cpp编译为共享库文件，即so文件，默认存放的位置为/system/lib
      因此，如果我们想要将自己编写的程序编译至image中，只需要将Eclipse下完成的整个工程copy到source tree下的某个目录即可，我一般习惯放到packages/apps下。
# 3.    添加Android.mk 
完成了上一步，可以知道，Android.mk在编译中起着至关重要的作用，这其实就是Android编译环境中的make file。为了完成我们的工作，需要在源代码中添加Android.mk。添加自己的Android.mk可以仿照SimpleJNI中的Android.mk，稍微修改即可。我们首先看看SimpleJNI目录下的两个Android.mk的内容：
- **根 目录下的Android.mk **
> 
TOP_LOCAL_PATH:= $(call my-dir)
# Build activity
LOCAL_PATH:= $(TOP_LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := samples
LOCAL_SRC_FILES := $(call all-subdir-java-files)
LOCAL_PACKAGE_NAME := SimpleJNI
LOCAL_JNI_SHARED_LIBRARIES := libsimplejni
LOCAL_PROGUARD_ENABLED := disabled
include $(BUILD_PACKAGE)
# ============================================================
# Also build all of the sub-targets under this one: the shared library.
include $(call all-makefiles-under,$(LOCAL_PATH))
      根目录下的Android.mk决定了整个工程编译的配置，其中，
      LOCAL_PATH 定义了当前的目录
      LOCAL_MUDULE_TAGS 定义了当前模块的类型，编译器在编译时会根据类型的不同有些差别，有些tags的module甚至不会被编译至系统中。LOCAL_MUDULE_TAGS主要有如下几种：user debug eng tests optional samples shell_ash shell_mksh。optional表示在所有版本的编译条件下都被编译至image中，剩下的表示在该版本中才会被编译只image中，如user表示在user版本下才会被编译至image中。
对于包含LOCAL_PACKAGE_NAME的mk文件，该项默认为optinal，具体可以参看build/core/package.mk。SimpleJNI中定义为samples的具体作用我也不太清楚，为了保险起见，我自己的apk一般定义为optional。
      LOCAL_SRC_FILES 定义了编译apk所需要的java代码的目录
      LOCAL_PACKAGE_NAME 这里需要改成自己的package的名字
      LOCAL_JNI_SHARED_LIBRARIES 定义了要包含的so库文件的名字，如果你的程序没有采用JNI，这行不需要。
      LOCAL_PROGUARD_ENABLED 定义了Java开发中的ProGuard压缩方法，主要用来分析压缩程序的，在我自己的应用中我没有加这行。
      include $(BUILD_PACKAGE) 这行是build的关键，表示当前java代码build成apk
      include $(call all-makefiles-under,$(LOCAL_PATH)) 表示需要build该目录下的子目录的文件，这样编译系统就会在当前目录下的子目录寻找Android.mk来编译so等其它程序。
      根据上述所写，创建我自己的Android.mk如下：
> 
TOP_LOCAL_PATH:= $(call my-dir)
# Build activity
LOCAL_PATH:= $(TOP_LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := $(call all-subdir-java-files)
LOCAL_PACKAGE_NAME := TestJniApp
LOCAL_JNI_SHARED_LIBRARIES := libtestjniapp
include $(BUILD_PACKAGE)
# ============================================================
# Also build all of the sub-targets under this one: the shared library.
include $(call all-makefiles-under,$(LOCAL_PATH))
      看起来很简单吧，基本不需要改动即可。
- **Jni目录下的Android.mk **
      由于我们的TestJniApp是用JNI完成的，包含C源代码，因此也需要一个jni目录下的Android.mk。同样首先看看SimpleJNI中jni目录下的Android.mk的内容：
> 
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := samples
# This is the target being built.
LOCAL_MODULE:= libsimplejni
# All of the source files that we will compile.
LOCAL_SRC_FILES:= /
  native.cpp
# All of the shared libraries we link against.
LOCAL_SHARED_LIBRARIES := /
        libutils
# No static libraries.
LOCAL_STATIC_LIBRARIES :=
# Also need the JNI headers.
LOCAL_C_INCLUDES += /
        $(JNI_H_INCLUDE)
# No special compiler flags.
LOCAL_CFLAGS +=
# Don't prelink this library.  For more efficient code, you may want
# to add this library to the prelink map and set this to true. However,
# it's difficult to do this for applications that are not supplied as
# part of a system image.
LOCAL_PRELINK_MODULE := false
include $(BUILD_SHARED_LIBRARY)
      LOCAL_MODULE 当前模块的名字，即编译后的so文件的名字
      LOCAL_SRC_FILES 所要编译的文件
      LOCAL_SHARED_LIBRARIES, LOCAL_STATIC_LIBRARIES 该模块要链接的动态库和静态库。
      LOCAL_C_INCLUDES 要包含的头文件
      LOCAL_CFLAGS  C语言编译选项
      LOCAL_PRELINK_MODULE 定义是否使用prelink工具，它用事先链接代替运行时链接的方法来加速共享库的加载，不仅可以加快起动速度，还可以减少部分内存开销。
      经过修改后，我自己的TestJniApp中jni目录下的Android.mk如下：
> 
LOCAL_PATH := $(call my-dir) 
include $(CLEAR_VARS) 
LOCAL_MODULE    := libtestjniapp 
LOCAL_SRC_FILES := com_test_app_Jni.c 
LOCAL_C_INCLUDES += $(JNI_H_INCLUDE) 
LOCAL_PRELINK_MODULE := false 
include $(BUILD_SHARED_LIBRARY) 
  这里有一点需要注意，如果要将so文件编译入image，必须要修改LOCAL_MODULE_TAGS，将原有的值samples修改为user，或者可以直接删掉 。删掉是因为对于包含LOCAL_MODULE的mk文件，如果没有指定LOCAL_MODULE_TAGS，该项默认为user，而只有定义为user的情况下，才会将so文件编译入image，具体定义可以参看build/core/base_rule.mk。
# 4.    修改/bulid/target/product/generic.mk 把工程编译到系统中 
      至此，还有最后一部工作。为了将工程编译进入image，还需要在/bulid/target/product/generic.mk文件中将package name添加进去
> 
PRODUCT_PACKAGES := /
    AccountAndSyncSettings /
    CarHome /
    DeskClock /
    ……
    SyncProvider /
    TestJniApp 
       完成上面这些步骤后，在 source tree 根目录下编译 image 就可以了。 
