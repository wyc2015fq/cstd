# NDK在自己的SO中调用第三方SO - 建建的博客 - CSDN博客
2017年06月07日 17:17:18[纪建](https://me.csdn.net/u013898698)阅读数：376
- 拷贝第三方的SO及其头文件
- 在Android.mk中申明第三方编译模块
- 在Android.mk的自己编译模块中包含第三方编译模块
- 在自己的代码中调用第三方头文件
### Android.mk配置说明
```
LOCAL_PATH := $(call my-dir)
#第三方的编译模块
include $(CLEAR_VARS)
LOCAL_MODULE    := _3rdsdk
LOCAL_SRC_FILES := _3rdsdk.so
#下面是申明第三方头文件路径
LOCAL_EXPORT_C_INCLUDES := _3rdsdk/include
include $(PREBUILT_SHARED_LIBRARY)
#自己的编译模块
include $(CLEAR_VARS)
LOCAL_MODULE    := test
LOCAL_SRC_FILES := test.c
LOCAL_LDLIBS    += -L$(SYSROOT)/usr/lib -llog
LOCAL_ARM_MODE  := arm
LOCAL_CFLAGS    := -g
#这里引入第三方编译模块
LOCAL_SHARED_LIBRARIES := _3rdsdk
include $(BUILD_SHARED_LIBRARY)
```
