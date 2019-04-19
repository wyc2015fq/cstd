# undefined reference to || C代码中实现Surface显示 - 三少GG - CSDN博客
2011年12月31日 19:04:46[三少GG](https://me.csdn.net/scut1135)阅读数：4199标签：[reference																[c																[android																[jni																[include																[exception](https://so.csdn.net/so/search/s.do?q=exception&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=reference&t=blog)
个人分类：[Android 多媒体](https://blog.csdn.net/scut1135/article/category/790993)
**共三处修改！！！！！！！**
**1.**
surface.c对应的Android.mk
LOCAL_PATH:= $(call my-dir) 
include $(CLEAR_VARS) 
# our source files 
# 
LOCAL_SRC_FILES:= \ 
    surface.cpp 
LOCAL_SHARED_LIBRARIES := \ 
    libskia \
**    libsurfaceflinger\    libgui\ **
        libutils \ 
    liblog 
LOCAL_C_INCLUDES += \ 
    $(JNI_H_INCLUDE) \ 
    external/skia/src/core \ 
    external/skia/include/core \ 
    frameworks/base/include \ 
    frameworks/base/native/include 
# Optional tag would mean it doesn't get installed by default 
LOCAL_MODULE_TAGS := optional 
LOCAL_PRELINK_MODULE := false 
LOCAL_ARM_MODE := arm 
LOCAL_MODULE:= libjnivideo 
include $(BUILD_SHARED_LIBRARY) 
+++++
**2.**
int AndroidSurface_updateSurface(bool autoscale) { 
    if(surface == NULL) { 
        return ANDROID_SURFACE_RESULT_JNI_EXCEPTION; 
    } 
** if (!surface->isValid()) { //Surface::isValid()**
        return ANDROID_SURFACE_RESULT_NOT_VALID; 
    }
+++++
**3.**
static 
Surface* getNativeSurface(JNIEnv* env, jobject jsurface) { 
    jclass clazz = env->FindClass("android/view/Surface"); 
    jfieldID field_surface = env->GetFieldID(clazz, **ANDROID_VIEW_SURFACE_JNI_ID**, "I");//"mSurface"
    if(field_surface == NULL) { 
        return NULL; 
    } 
    return (Surface *) env->GetIntField(jsurface, field_surface); 
}
因为 android2.2以上的版本，android.view.Surface里面没有“mSurface"了，而是用了一个常量 ANDROID_VIEW_SURFACE_JNI_ID, 区分下版本就行了。
++++++++**整体参考框架++**++
you must copy libjnivideo.so and libjniaudio.so into lib/armeabi of your project for eclipse, which pack them into apk. If you run this project on not froyo OS you will have errors because some libraries don't be there or have other name and dalvik can't find
 them
I tried several times and passed finally. Here is my configuration changed for elair: from the eclair source, copy your native directory to frameworks/base/, modify the file of native/video/jni/Android.mk to add libui to LOCAL_SHARED_LIBRARIES list and replace
 libsurfaceflinger_client with libsurfaceflinger, modify surface.cpp to replace #include with #include , add libjniaudio.so and libjnivideo.so to the end of myandroid/build/core/prelink-linux-arm.map, then I get the libjniaudio.so and libjnivideo.so in out/target/product/generic/obj/lib
 for eclair. I try them in my emulator, the result is exactly beyond my imagination. It will be better if your project can fit any android version. Anyway, Thank you for your porting. If possible, I will send the three .so for eclair to you for testing by other
 people.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
重要参考文献：
# [android之 JNI端获取并操作Surface](http://www.cnblogs.com/ljplum/archive/2011/09/30/2196873.html)
　　前一段时间研究android, 在jni端操作surface遇到麻烦，主要是C++基础太差，Surface.cpp读了n遍，也仿照网上查到的资料，试图从Java端传递Surface，然后jni端进行操作。却总是遇到各种各样的异常，前前后后卡住了7天，最后终于解决了，放出这个方法，总会对某些朋友有帮助。
　　其实不是原创的，只是，网上有1000篇帖子，有999篇都讲的同一种方法，但在我这里却偏偏成功不了。终于那天偶然看到一篇e文的帖子，是有人分析了 havlenapetr 的libjnivideo.so, 然后放出了ta使用的方法，原来想当简单：
static android::sp<android::Surface> native_surface;
static android::Surface* getNativeSurface(JNIEnv* env, jobject jsurface, jint version)
{
    jclass clazz = env->FindClass("android/view/Surface");
    jfieldID field_surface;
    if(version <=8)
    {
        field_surface = env->GetFieldID(clazz, "mSurface", "I");
    }
    else
        field_surface = env->GetFieldID(clazz, ANDROID_VIEW_SURFACE_JNI_ID, "I");
    if (field_surface == NULL)
    {
        return NULL;
    }
    return (android::Surface *) env->GetIntField(jsurface, field_surface);
}
int setSurface(JNIEnv *env, jobject jsurface, jint version)
{
    native_surface = getNativeSurface(env, jsurface, version);
    if(android::Surface::isValid(native_surface))
    {
        __android_log_print(ANDROID_LOG_INFO, "libjni", "native_surface is valid");
        return 1;
    }
    else
        __android_log_print(ANDROID_LOG_ERROR, "libjni", "native_surface is invalid");
    return 0;
}
　　jsurface就是从Java端传递过来的，然后这里的 native_surface，就是我们想要的native surface了。为什么要 传递个version？ 因为 android2.2以上的版本，android.view.Surface里面没有“mSurface"了，而是用了一个常量 ANDROID_VIEW_SURFACE_JNI_ID, 区分下版本就行了。
　　然后又发现jni端操作surface也是相当简单，至少显示图像之类的很容易：
static android::Surface::SurfaceInfo info;
static android::Region dirtyRegion;
做下初始化：
　　dirtyRegion.set(android::Rect(0x3FFF, 0x3FFF));
然后
　　native_surface->lock(&info, &dirtyRegion, true);
　　memcpy(info.bits, buf, bufSize);
　　native_surface->unlockAndPost();
就显示出来了。
**+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**
[**Android** 调试/测试]问题求助**，**通过ndk在本地C代码中实现Surface显示](http://www.eoeandroid.com/thread-49423-1-1.html)
[http://www.eoeandroid.com/thread-49423-1-1.html](http://www.eoeandroid.com/thread-49423-1-1.html)
我的目的是实现一个基于ffmpeg的播放器**，**主要参照和模仿的是havlenape的代码。大部分功能都实现了**，**但在图像输出时遇到了问题**，**无法输出。
  在java层建立了一个SUrfaceView**，**主要代码如下：
    public class FFMpegMovieViewAndroid extends SurfaceView {
    。。。
        public FFMpegMovieViewAndroid(Context context) {
            super(context);
            getHolder().addCallback(mSHCallback);
        }
    。。。
    SurfaceHolder.Callback mSHCallback = new SurfaceHolder.Callback() {
        public void surfaceChanged(SurfaceHolder holder**,** int format**,** int w**,**
 int h) {
            startVideo();
        }
        public void surfaceCreated(SurfaceHolder holder) {
            mSurfaceHolder = holder;
            openVideo();
        }
        public void surfaceDestroyed(SurfaceHolder holder) {
        }
    };
    。。。
    jni代码：
    static Surface* getNativeSurface(JNIEnv* env**,** jobject jsurface) {
        jclass clazz = env->FindClass("**android**/view/Surface");
        jfieldID field_surface = env->GetFieldID(clazz**,** "mSurface"**,** "I");
        if(field_surface == NULL) {
            return NULL;
        }
        return (Surface *) env->GetIntField(jsurface**,** field_surface);
    }
    通过这个得到Surface指针**，**而且java代码调用时把surface对象传递过来了。
    显示主要是通过SkBitmap来实现的**，**初始化后**，**通过函数得到指针：
    int AndroidSurface_getPixels(int width**,** int height**,** void** pixels) {
    __**android**_log_print(**ANDROID**_LOG_INFO**,**
 TAG**,** "getting surface's pixels %ix%i"**,** width**,**
 height);
    if(sSurface == NULL) {
        return **ANDROID**_SURFACE_RESULT_JNI_EXCEPTION;
    }
    if(initBitmap(&sBitmapClient**,** PIXEL_FORMAT_RGB_565**,** width**,**
 height**,** true) < 0) {
        return **ANDROID**_SURFACE_RESULT_COULDNT_INIT_BITMAP_CLIENT;
    }
    *pixels = sBitmapClient.getPixels();
    __**android**_log_print(**ANDROID**_LOG_INFO**,**
 TAG**,** "getted");
    return **ANDROID**_SURFACE_RESULT_SUCCESS;
}
    显示输出首先是通过下面的代码将frame和指针相联系。
            mFrame = avcodec_alloc_frame();
        if (mFrame == NULL) {
                return INVALID_OPERATION;
        }
        avpicture_fill((AVPicture *) mFrame**,** (uint8_t *) pixels**,** PIX_FMT_RGB565**,**
 stream->codec->width**,** stream->codec->height);
    在每一副图像输出是**，**直接将图像输出到指针mFrame->data**,**然后调用AndroidSurface_updateSurface()输出**，**代码如下：
static void doUpdateSurface() {
    SkCanvas canvas(sBitmapSurface);
    SkRect surface_sBitmapClient;
    SkRect surface_sBitmapSurface;
    SkMatrix matrix;
    surface_sBitmapSurface.set(0**,** 0**,** sBitmapSurface.width()**,**
 sBitmapSurface.height());
    surface_sBitmapClient.set(0**,** 0**,** sBitmapClient.width()**,**
 sBitmapClient.height());
    matrix.setRectToRect(surface_sBitmapClient**,** surface_sBitmapSurface**,** SkMatrix**::**kFill_ScaleToFit);
    canvas.drawBitmapMatrix(sBitmapClient**,** matrix);
}
static int prepareSurfaceBitmap(**Surface::**SurfaceInfo* info) {
    if(initBitmap(&sBitmapSurface**,** info->format**,** info->w**,**
 info->h**,** false) < 0) {
        return -1;
    }
    sBitmapSurface.setPixels(info->bits);
    return 0;
}
int AndroidSurface_updateSurface() {
    static **Surface::**SurfaceInfo surfaceInfo;
    if(sSurface == NULL) {
        return **ANDROID**_SURFACE_RESULT_JNI_EXCEPTION;
    }
    if (!sSurface->isValid()) {
        return **ANDROID**_SURFACE_RESULT_NOT_VALID;
    }
    if (sSurface->lock(&surfaceInfo) < 0) {
        return **ANDROID**_SURFACE_RESULT_COULDNT_LOCK;
    }
    if(prepareSurfaceBitmap(&surfaceInfo) < 0) {
        return **ANDROID**_SURFACE_RESULT_COULDNT_INIT_BITMAP_SURFACE;
    }
    doUpdateSurface();
    if (sSurface->unlockAndPost() < 0) {
        return **ANDROID**_SURFACE_RESULT_COULDNT_UNLOCK_AND_POST;
    }
    return **ANDROID**_SURFACE_RESULT_SUCCESS;
}
我现在的状态是**，**没有报错**，**但视频图像没有显示。
我希望得到帮助是：相关细节应用的阐述**，**或者类似的demo**，**或者能实现本地视频输出的其他方法。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
### [undefined reference to `__android_log_print'](http://yfxuchao.iteye.com/blog/949198)
**博客分类：**[android](http://yfxuchao.iteye.com/category/104173)
[Android](http://www.iteye.com/blogs/tag/Android)[JNI](http://www.iteye.com/blogs/tag/JNI)[C](http://www.iteye.com/blogs/tag/C)[C++](http://www.iteye.com/blogs/tag/C++)[C#](http://www.iteye.com/blogs/tag/C%23)
原因：没有加入支持的共享库
出错时：
Android.mk
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := testlog
LOCAL_SRC_FILES := testlog.c
include $(BUILD_SHARED_LIBRARY)
.c的头文件
#include <string.h>
#include <jni.h>
#include <android/log.h>
调试好的：
Android.mk
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := testlog
LOCAL_SRC_FILES := testlog.c
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)
.c的头文件
#include <string.h>
#include <jni.h>
#include <android/log.h>

