# 安卓JNI使用OpenCV - wishchinYang的专栏 - CSDN博客
2015年07月23日 14:20:28[wishchin](https://me.csdn.net/wishchin)阅读数：953
         OpenCV也有Java数据结构的包，不过计算速度还是很慢，非不得已不使用此种方式调用OpenCV。使用NDK编写底层OpenCv的调用代码，使用JNI对代码进行封装，可以稍微提高一点效率。
        参考链接：[http://www.tuicool.com/articles/yaeeuu](http://www.tuicool.com/articles/yaeeuu)
        安卓使用OpenCV注意事项：
        注意事项： 包含OpenCV的工程最好放置在 OpenCV-2.4.10-android-sdk\samples 目录下，省却很多 莫名其妙配置的麻烦。
**一、 添加头文件包含：**
       对自己建立的SO包里面的JNI源文件，添加即将使用到的头文件包含：
```cpp
#include <jni.h>
#include <string.h>
#include <stdio.h>
#include <android/log.h>
<u>#include "rec3D.h"</u>
#ifndef _Included_org_opencv_samples_fd_DetectionBasedTracker
#define _Included_org_opencv_samples_fd_DetectionBasedTracker
#ifdef __cplusplus
extern "C" {
#endif
//JNIEnv* jniEnv;
extern "C" jstring Java_com_example_feeljulygpsmap_FeelJulyActivity_careJuly(JNIEnv *env, jobject thiz) ;
extern "C" jint Java_com_example_feeljulygpsmap_FeelJulyActivity_careJulyIn(JNIEnv *env, jobject thiz,jint showCode) ;
extern "C" jintArray Java_com_example_feeljulygpsmap_FeelJulyActivity_careJulyArray(JNIEnv *env, jobject thiz,jintArray InCode) ;
#ifdef __cplusplus
}
#endif
#endif
```
        使用到OpenCV的源文件：
```cpp
#pragma once
#ifndef _REC3D_H
#define _REC3D_H
<u>#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/calib3d/calib3d.hpp></u>
#include <iostream>
#include <vector>
#include <stdlib.h>
using namespace  cv;
using namespace std;
class MyCamera{
public:
	cv::Mat intrinsic_Mat;
	cv::Mat disCoeff;
```
**二、在工程中添加OpenCV的目录包含和库引用**
文件：Android.mk
```
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_C_INCLUDES := C:\QT\OpenCV-3.0-android-sdk\sdk\native\jni\include
OPENCV_LIB_TYPE   := static
ifeq ("$(wildcard $(OPENCV_MK_PATH))","")  
#try to load OpenCV.mk from default install location  
include C:\QT\OpenCV-3.0-android-sdk\sdk\native\jni\OpenCV.mk 
else  
include $(OPENCV_MK_PATH)  
endif  
#include $(CLEAR_VARS)
LOCAL_MODULE    := feelJulyGpsMap
LOCAL_SRC_FILES := feelJulyGpsMap.cpp \
                   rec3D.cpp
#LOCAL_LDLIBS    := -landroid
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog
include $(BUILD_SHARED_LIBRARY)
```
在此注意事项：
`LOCAL_C_INCLUDES := C:\QT\OpenCV-3.0-android-sdk\sdk\native\jni\include`为OpenCV的C++ 头文件包含目录
`include C:\QT\OpenCV-3.0-android-sdk\sdk\native\jni\OpenCV.mk `为OpenCV的库文件包含目录
**三、使用OpenCV编译程序**
snip..............................................
**四、使用Android Native模式调试C++代码部分**
snip.............................
![](https://img-blog.csdn.net/20150723141908100?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
备注：cv::solvePnP使用OpenCv解决多点透视问题！

