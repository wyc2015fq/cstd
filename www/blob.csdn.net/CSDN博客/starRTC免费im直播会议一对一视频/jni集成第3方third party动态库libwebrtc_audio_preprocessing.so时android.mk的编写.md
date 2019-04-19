# jni集成第3方third party动态库libwebrtc_audio_preprocessing.so时android.mk的编写 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年05月22日 18:42:23[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：791
现在要调用一个第3方库libwebrtc_audio_preprocessing.so
最终布局如下：
即创建2个文件夹，将第3方库放进去。
include目录放第3方库的头文件 
其中lib目录如下：
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := webrtc_audio_preprocessing
LOCAL_SRC_FILES := libwebrtc_audio_preprocessing.so
#prebuilt user libraries
include $(PREBUILT_SHARED_LIBRARY)
最上层的Android.mk如下：
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := apm
LOCAL_SRC_FILES := apm_jni_wrapper.cpp
LOCAL_LDLIBS += -llog -lz -landroid
LOCAL_SHARED_LIBRARIES := webrtc_audio_preprocessing
include $(BUILD_SHARED_LIBRARY)
#place it in the end
include $(LOCAL_PATH)/lib/Android.mk
运行ndk-build后，会将第3方库拷贝到armeabi-v7a目录下。

[http://stackoverflow.com/questions/10363021/multiple-my-and-3rd-party-native-libraries-in-android-ndk](http://stackoverflow.com/questions/10363021/multiple-my-and-3rd-party-native-libraries-in-android-ndk)
