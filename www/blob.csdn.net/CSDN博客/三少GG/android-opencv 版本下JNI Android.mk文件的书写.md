# android-opencv 版本下JNI Android.mk文件的书写 - 三少GG - CSDN博客
2012年07月26日 19:26:10[三少GG](https://me.csdn.net/scut1135)阅读数：2957标签：[jni																[wildcard																[include																[path																[module																[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=wildcard&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)
个人分类：[Android 多媒体](https://blog.csdn.net/scut1135/article/category/790993)
**不能利用 include $(all-subdir-makefiles)**
**会报重复引用的错误。**
`Android NDK: Trying to define local module 。。。opencv。`**故放弃采取分文件夹写.mk文件的策略。**
**参考以下工程：**
[http://code.google.com/p/android-opencv-panorama/source/browse/android/pano/jni/Android.mk](http://code.google.com/p/android-opencv-panorama/source/browse/android/pano/jni/Android.mk)
解决方案如下：
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
OPENCV_CAMERA_MODULES:= off
include ../includeOpenCV.mk
ifeq ("$(wildcard $(OPENCV_MK_PATH))","")
#try to load OpenCV.mk from default install location
include $(TOOLCHAIN_PREBUILT_ROOT)/user/share/OpenCV/OpenCV.mk
else
include $(OPENCV_MK_PATH)
endif
NEAR_C_INCLUDES := $(LOCAL_C_INCLUDES)
NEAR_STATIC_LIBRARIES := $(LOCAL_STATIC_LIBRARIES)
NEAR_CFLAGS := $(LOCAL_CFLAGS)
########################
第一个。。。
############第二个############
include $(CLEAR_VARS)
LOCAL_C_INCLUDES := $(NEAR_C_INCLUDES)
LOCAL_STATIC_LIBRARIES := $(NEAR_STATIC_LIBRARIES)
LOCAL_CFLAGS := $(NEAR_CFLAGS)
。。。
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
P:
`Java.lang.UnsatisfiedLinkError`.
`cannot load library:...`
Ans:
Best guess with what information you have provided, The library you are trying to load needs some dependencies to be loaded before it.
For example:
```
System.loadLibrary("bullet");
System.loadLibrary("irrlicht");
System.loadLibrary("gamescript");
```
`gamescript`**library needs other 2 library to be loaded before it.** Otherwise, it gives me the same error you have mentioned.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
若为动态库，  需要注意在java层代码里加载相关的每个动态库！！！
}
若为静态库， 注意必须是+=，要不然报错！
Be careful with the following line, it should be like this:
LOCAL_STATIC_LIBRARIES**+=**libfastcv
not like
LOCAL_STATIC_LIBRARIES := libfastcv
类似：
[http://stackoverflow.com/questions/7346580/problem-using-opencv2-3-1-with-android-native-activity](http://stackoverflow.com/questions/7346580/problem-using-opencv2-3-1-with-android-native-activity)
I solved the problem there. It was my fault (as usual xD) the problem was I was writting in my Android.mk this line: LOCAL_STATIC_LIBRARIES := android_native_app_glue, instead of this line: LOCAL_STATIC_LIBRARIES += android_native_app_glue.
I needed the "plus" symbol, in order to add the new library and not deleting the previously loaded. Thanks anyway!!
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
siox error:  
[expected unqualified-id before numeric constant](http://blog.csdn.net/needle2/article/details/7699534)**-Y _D 变量重新命名，用_YPoint _DPoint 代替！**
今天编译库的时候出现这个问题unqualified-id before numeric constant，是因为变量重复定义了，主要是变量与宏定义同名了，及以下模式：
一个头文件定义一个接口体，
class XXXClass｛
const static int MAX_PATTERN_NUM = 1024;
｝
然后在另外一个文件定义宏如下：
#define  MAX_PATTERN_NUM 100
这时就会出现以上的错误
Linux gcc 编译出错   “expected unqualified-id before numeric constant ”
原因很可能是所有大写的名字常常被用于预处理宏，真的需要采用常用的习惯的命名原则，不然真不好查。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
## [Installation error: INSTALL_FAILED_INSUFFICIENT_STORAGE](http://www.cnblogs.com/mumue/archive/2012/04/11/2442990.html)
解决办法：1. 把以前的早版本 apk文件卸载
进入菜单
settings->applications->mange
applications-> select theapplication->select "unistall".
2.
把以前的apk文件都给删除了。删除方法：命令 进入tools文件夹下，
adb shell 
 cd data
 cd app 
 ls,就可以看见之前的apk文件，用rm文件名，就可以把它删除。（模拟器要处在运行的状态下）
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
**[Android用ImageView显示本地和网上的图片](http://blog.3gstdy.com/?p=38)**
**/****
*** 加载本地图片**
*** http://bbs.3gstdy.com**
*** @param url**
*** @return**
***/**
**public static Bitmap getLoacalBitmap(String url) { **
**try {  **
**FileInputStream fis = new FileInputStream(url);  **
**return BitmapFactory.decodeStream(fis);  **
**} catch (FileNotFoundException e) {  **
**e.printStackTrace();  **
**return null;  **
**}**
**}**
