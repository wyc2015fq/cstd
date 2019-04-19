# 自动添加需要编译的源文件Android.mk模板 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年10月23日 15:14:09[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：208

自动添加需要编译的源文件列表
添加第三方静态库、动态库的依赖
  假设我们的项目依赖 libmath.a, libjson.a, libffmpeg.so 这几个第三方库文件，项目包含如下几个模块：algorithm，core，network，utils，tests，每个模块都有若干个.c/.h文件，我们要把整个工程编译为动态库，并通过
 native_sdk.c 中的函数向Java层提供调用接口。
LOCAL_PATH := $(call my-dir)
3RD_INC_DIR = $(LOCAL_PATH)/3rd/inc
3RD_LIB_DIR = $(LOCAL_PATH)/3rd/libs
# Prebuild the 3rd libraries 
include $(CLEAR_VARS)
LOCAL_MODULE := math
LOCAL_SRC_FILES := $(3RD_LIB_DIR)/libmath.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE := json
LOCAL_SRC_FILES := $(3RD_LIB_DIR)/libjson.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE := ffmpeg
LOCAL_SRC_FILES := $(3RD_LIB_DIR)/libffmpeg.so
include $(PREBUILT_SHARED_LIBRARY)
# Build native sdk 
include $(CLEAR_VARS)    
LOCAL_MODULE := native_sdk
LOCAL_SRC_FILES := \
$(subst $(LOCAL_PATH)/,,$(wildcard $(LOCAL_PATH)/src/algorithm/*.c))  \
$(subst $(LOCAL_PATH)/,,$(wildcard $(LOCAL_PATH)/src/core/*.c))  \
$(subst $(LOCAL_PATH)/,,$(wildcard $(LOCAL_PATH)/src/network/*.c)) \
$(subst $(LOCAL_PATH)/,,$(wildcard $(LOCAL_PATH)/src/utils/*.c)) \
$(subst $(LOCAL_PATH)/,,$(wildcard $(LOCAL_PATH)/src/*.c))  
LOCAL_C_INCLUDES := $(3RD_INC_DIR)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/algorithm
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/core
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/network
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/utils
LOCAL_CFLAGS := -DANDROID                                    
LOCAL_LDLIBS := -llog 
LOCAL_STATIC_LIBRARIES := math json
LOCAL_SHARED_LIBRARIES := ffmpeg
include $(BUILD_SHARED_LIBRARY)
# Build tests
include $(CLEAR_VARS)    
LOCAL_MODULE := test.out
LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,,$(wildcard $(LOCAL_PATH)/tests/*.c)) 
LOCAL_CFLAGS := -DANDROID 
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src
LOCAL_LDLIBS := -llog -fPIE -pie
LOCAL_SHARED_LIBRARIES:= native_sdk
include $(BUILD_EXECUTABLE)
 对于 Android.mk 而言，如果要依赖第三方库文件，则必须添加 PREBUILT_XXXX_LIBRARY 进行预编译，LOCAL_SRC_FILES 宏给出了需要编译的源文件列表，
这里我通过 subst和wildcard 这两个函数，实现了自动扫描指定目录下的源文件，最后，通过 BUILD_EXECUTABLE
 编译出可以运行于adb shell命令行的可执行程序，用于做代码的单元测试。
附：
:= 是覆盖之前的值
?= 是如果没有被赋值过就赋予等号后面的值
+= 是添加等号后面的值
 make会将整个makefile展开后，再决定变量的值。也就是说，变量的值将会是整个makefile中最后被指定的值。看例子：
x = foo
            y = $(x) bar
            x = xyz
在上例中，y的值将会是 xyz bar ，而不是 foo
 bar 。
 “:=”表示变量的值决定于它在makefile中的位置，而不是整个makefile展开后的最终值。
x := foo
            y := $(x) bar
            x := xyz
      在上例中，y的值将会是 foo bar ，而不是 xyz
 bar 了。
[参考：](http://ticktick.blog.51cto.com/823160/1733500/)
[http://ticktick.blog.51cto.com/823160/1733500/](http://ticktick.blog.51cto.com/823160/1733500/)
