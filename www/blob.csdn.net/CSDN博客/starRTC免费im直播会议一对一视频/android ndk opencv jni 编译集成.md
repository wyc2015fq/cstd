# android ndk opencv  jni 编译集成 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年07月25日 18:09:04[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：156
OpenCV (Open Source Computer Vision Library) 
[https://docs.opencv.org/2.4/doc/tutorials/introduction/android_binary_package/dev_with_OCV_on_Android.html#native-c](https://docs.opencv.org/2.4/doc/tutorials/introduction/android_binary_package/dev_with_OCV_on_Android.html#native-c)
1，下载opencv for android 的sdk
[https://opencv.org/releases.html](https://opencv.org/releases.html)
打开 sdk/native/jni，如无意外，里面肯定有个 文件叫做 OpenCV.mk，它就是我们在 android.mk 脚本文件中要引入 opencv C++库所要参照的文件。
2，写mk
Android.mk
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
OPENCV_INSTALL_MODULES := on #Copies necessary OpenCV dynamic libs to the project libs folder
OPENCV_CAMERA_MODULES := off #Skip native OpenCV camera related libs copying to the project libs folder.
OPENCV_LIB_TYPE:=STATIC
include E:\work\starRTC\opencv\jni\OpenCV-android-sdk\sdk\native\jni\OpenCV.mk
LOCAL_MODULE := starRTC
LOCAL_SRC_FILES := main.cpp
LOCAL_LDLIBS += -llog -lz -lm
include $(BUILD_SHARED_LIBRARY)
Application.mk
APP_OPTIM:=debug
APP_ABI := armeabi-v7a
APP_PLATFORM=android-18
APP_STL := gnustl_static
#APP_CPPFLAGS := -std=c++11
APP_CPPFLAGS := -frtti -fexceptions
3，写jni代码
main.h
#ifndef _android_starLiveVideo_
#define _android_starLiveVideo_
#include <jni.h>
#include <opencv2/opencv.hpp>
#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
IplImage * change4channelTo3InIplImage(IplImage * src);
#endif
main.cpp
#include <main.h>
#include <stdio.h>
#include <stdlib.h>
using namespace cv;
JNIEXPORT jintArray JNICALL interface_test(JNIEnv* env, jobject obj,
jintArray buf, int w, int h) {
jint *cbuf;
cbuf = env->GetIntArrayElements(buf, false);
if (cbuf == NULL) {
return 0;
}
Mat myimg(h, w, CV_8UC4, (unsigned char*) cbuf);
IplImage image=IplImage(myimg);
IplImage* image3channel = change4channelTo3InIplImage(&image);
IplImage* pCannyImage=cvCreateImage(cvGetSize(image3channel),IPL_DEPTH_8U,1);
cvCanny(image3channel,pCannyImage,50,150,3);
int* outImage=new int[w*h];
for(int i=0;i<w*h;i++)
{
outImage[i]=(int)pCannyImage->imageData[i];
}
int size = w * h;
jintArray result = env->NewIntArray(size);
env->SetIntArrayRegion(result, 0, size, outImage);
env->ReleaseIntArrayElements(buf, cbuf, 0);
return result;
}
IplImage * change4channelTo3InIplImage(IplImage * src) {
if (src->nChannels != 4) {
return NULL;
}
IplImage * destImg = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
for (int row = 0; row < src->height; row++) {
for (int col = 0; col < src->width; col++) {
CvScalar s = cvGet2D(src, row, col);
cvSet2D(destImg, row, col, s);
}
}
return destImg;
}
static JNINativeMethod gMethodsSrc[] = {
{"interface_test", "([III)[I", (void*)interface_test}
};
int register_native_mothods(JNIEnv *env, const char *class_name, JNINativeMethod *gMethods, int numMethods){
jclass clazz;
clazz = env->FindClass(class_name);
if(clazz == NULL){
//loge("err:FindClass failed,className=%s\n",class_name);
return 0;
}
if(env->RegisterNatives(clazz, gMethods, numMethods) < 0){
//loge("err:RegisterNatives failed, numMethods=%d\n", numMethods);
return 0;
}
return 1;
}
JNIEXPORT
jint JNI_OnLoad(JavaVM *jvm, void *reserved){
JNIEnv *env = NULL;
jint result = JNI_ERR;
int i;
if(jvm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK){
//loge("err:GetEnv failed");
return result;
}
register_native_mothods(env, "com/starrtc/starrtcsdk/socket/StarSocket", gMethodsSrc, NELEM(gMethodsSrc));
return JNI_VERSION_1_4;
}
4,ndk-build
