
# Android 接入 OpenCV库的三种方式 - 小小情意 - 博客园






# [Android 接入 OpenCV库的三种方式](https://www.cnblogs.com/xiaoxiaoqingyi/p/6676096.html)
OpenCV是一个基于BSD许可（开源）发行的跨平台计算机视觉库，可以运行在Linux、Windows、Android和Mac OS操作系统上。它轻量级而且高效——由一系列 C 函数和少量 C++ 类[构成](http://baike.baidu.com/item/%E6%9E%84%E6%88%90/103686)，同时提供了Python、Ruby、MATLAB等语言的接口，实现了[图像处理](http://baike.baidu.com/item/%E5%9B%BE%E5%83%8F%E5%A4%84%E7%90%86)和计算机视觉方面的很多通用算法。
我们利用它来做一些图片的处理，能大大的优化内存的处理。下面我来说说接下OpenCV 的三种方式：
**一、接入OpenCV 的Java SDK 包，这样你可以直接在Java调用OpenCV 的大部分方法。第一种方式适用于对Opencv c++ 不熟悉的童鞋，不需要直接调用C++方案， 因为SDK 已经用JNI 全部封装好了。假设你已经安装好JDK、AndroidStudio与NDK环境。**
先到官网[http://opencv.org/releases.html](http://opencv.org/releases.html)，下载Android 包，如：opencv-3.2.0-android-sdk.zip。
![](https://images2015.cnblogs.com/blog/804587/201704/804587-20170406232859878-1928460285.png)
sdk 目录提供了Android的API与Java 库
sdk/java 目录包含了一个 Eclipse 项目，该项目提供 OpenCV 的Java API，且可以导入到开发环境里。
sdk/native 目录包含了OpenCV C++ 头文件(用于JNI)，与Android的 .so动态库 .a静态库。
sdk/etc 目录包含了Haar 与 LBP cascades 级联。
apk  目录包含了用户安装在指定的Android设备的安装文件，该文件使opencv 库可以管理opencv API
首先： File > New > New Module
![](https://images2015.cnblogs.com/blog/804587/201704/804587-20170406233047238-1707772694.png)
然后：选择Import Eclipse ADT Project
![](https://images2015.cnblogs.com/blog/804587/201704/804587-20170406233213832-1124395592.png)
把sdk/java 下的项目导入到项目里，然后把这个modules 添加到 app modules里，
直接在 app 目录下build.gradle 文件里dependencies 大括号下添加：
compile project(':openCVLibrary')
接着在 app/src/main 目录下 创建一个jniLibs 目录，然后把sdk/native/libs 下所有文件 拷贝到jniLibs下，编译，运行。
如果导入后，出现一些android 自带的类识别不了，那就是因为编译的SDK版本出错了。
打开 刚导入 的模块下 build.gradle 文件，把 compileSdkVersion 与 targetSdkVersion修改成你最新的SDK版本，如：
apply plugin:'com.android.library'android {
    compileSdkVersion25buildToolsVersion"25.0.0"defaultConfig {
        minSdkVersion15targetSdkVersion25}
    buildTypes {
        release {
            minifyEnabledfalseproguardFiles getDefaultProguardFile('proguard-android.txt'),'proguard-rules.txt'}
    }
}
运行，则成功了，我发现似乎不需要再安装额外的 opencv 的apk 了。
下面链接是我添加成功的例子：
[https://github.com/xiaoxiaoqingyi/NDKDemos](https://github.com/xiaoxiaoqingyi/NDKDemos)OpenCV_java 项目
**二、使用opencv sdk 提供的 C++ 头文件与 .so动态库 与 .a静态库，自己封装jni，这样使用的效率会比第一种方法高一些， 且可以100%使用opencv 的接口。下面看一下安装的方式：**
用AndroidStudio 创建一个项目，然后在 Java 类里创建一个native 方法，
再在app/src/main 下创建 jni，然后创建 cpp 文件对于 native 方法。
以上是一个简单的jni 方法，你可以查看我以前的 文章：[http://www.cnblogs.com/xiaoxiaoqingyi/p/6524165.html](http://www.cnblogs.com/xiaoxiaoqingyi/p/6524165.html)
在基本的jni 跑通下，我们把 opencv 库加入项目里，首先：




1、把 sdk/native 目录 拷贝到 项目的 jni 目录下，
![](https://images2015.cnblogs.com/blog/804587/201704/804587-20170406233534644-652375041.png)
然后就是配置 Android.mk文件：
LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)
OpenCV_INSTALL_MODULES :=on
OpenCV_CAMERA_MODULES :=off
OPENCV_LIB_TYPE :=STATIC
ifeq ("$(wildcard $(OPENCV_MK_PATH))","")
include $(LOCAL_PATH)/native/jni/OpenCV.mkelseinclude $(OPENCV_MK_PATH)
endif
LOCAL_MODULE :=OpenCV
LOCAL_SRC_FILES :=com_magicing_eigenndk_NDKUtils.cpp
LOCAL_LDLIBS+=  -lm -llog
include $(BUILD_SHARED_LIBRARY)
接着配置 Application.mk 文件：
APP_STL :=gnustl_static
APP_CPPFLAGS := -frtti -fexceptions
APP_PLATFORM := android-9
最后在 cpp 文件调用 opencv 的方法， 首先导入 \#include <opencv2/opencv.hpp> 如：
\#include <jni.h>\#include<string>\#include<iostream>\#include<stdio.h>\#include<stdlib.h>\#include<opencv2/opencv.hpp>usingnamespacecv;extern"C"JNIEXPORT jintArray JNICALL Java_com_magicing_eigenndk_NDKUtils_gray(
          JNIEnv*env, jclass obj, jintArray buf,intw,inth) {
      jint*cbuf;
      cbuf= env->GetIntArrayElements(buf, JNI_FALSE );if(cbuf ==NULL) {return0;
      }
      Mat imgData(h, w, CV_8UC4, (unsignedchar*) cbuf);
      uchar* ptr = imgData.ptr(0);for(inti =0; i < w*h; i ++){//计算公式：Y(亮度) = 0.299*R + 0.587*G + 0.114*B//对于一个int四字节，其彩色值存储方式为：BGRAintgrayScale = (int)(ptr[4*i+2]*0.299+ ptr[4*i+1]*0.587+ ptr[4*i+0]*0.114);
          ptr[4*i+1] =grayScale;
          ptr[4*i+2] =grayScale;
          ptr[4*i+0] =grayScale;
      }intsize = w *h;
      jintArray result= env->NewIntArray(size);
      env->SetIntArrayRegion(result,0, size, cbuf);
      env->ReleaseIntArrayElements(buf, cbuf,0);returnresult;
  }
然后在 activity 页面里显示处理过的图片，如下：
publicclassMainActivity extends AppCompatActivity {
    @OverrideprotectedvoidonCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        NDKUtils ndk=newNDKUtils();
        Bitmap bitmap=((BitmapDrawable) getResources().getDrawable(
                R.mipmap.pic_test)).getBitmap();intw = bitmap.getWidth(), h =bitmap.getHeight();int[] pix =newint[w *h];
        bitmap.getPixels(pix,0, w,0,0, w, h);int[] resultPixes=ndk.gray(pix,w,h);
        Bitmap result=Bitmap.createBitmap(w,h, Bitmap.Config.RGB_565);
        result.setPixels(resultPixes,0, w,0,0,w, h);
        ImageView img=(ImageView)findViewById(R.id.img2);
        img.setImageBitmap(result);
    }
}
运行成功后：
![](https://images2015.cnblogs.com/blog/804587/201704/804587-20170406233938628-1507466963.jpg)
我编译成功的项目链接：
[https://github.com/xiaoxiaoqingyi/NDKDemos](https://github.com/xiaoxiaoqingyi/NDKDemos)（OpenCV_native项目）
官网参考资料：
[http://docs.opencv.org/2.4/doc/tutorials/introduction/android_binary_package/dev_with_OCV_on_Android.html\#native-c](http://docs.opencv.org/2.4/doc/tutorials/introduction/android_binary_package/dev_with_OCV_on_Android.html#native-c)
**三、通过opencv 的源码，重新编译成 Android sdk 库，这样的好处是能获取到最新的功能，缺点是编译有点困难（对于不懂C++/C 的童鞋），且新的代码或许会存在不兼容与错误。**
[http://code.opencv.org/projects/opencv/wiki/Building_OpenCV4Android_from_trunk](http://code.opencv.org/projects/opencv/wiki/Building_OpenCV4Android_from_trunk)
以上的链接是官网推荐的编译过程，我也尝试过编译成功，如果在windows 下安装Cygwin，然后来编译是通过不了的，需要重新安装cmake、shell 以及其它的软件，编译的过程确实复杂很多。
我是在MAC下编译的，只需安装 cmake软件，当然也要有NDK的环境。大概的步骤：
[https://github.com/opencv/opencv](https://github.com/opencv/opencv)下载opencv 源码包
1、在mac 下安装好 cmake 软件，
2、配置NDK环境变量， 参考百度：[http://jingyan.baidu.com/article/d2b1d1029c1ea65c7e37d4c9.html](http://jingyan.baidu.com/article/d2b1d1029c1ea65c7e37d4c9.html)
3、build_android_arm/install 目录下 得到编译好的jni 目录。
如果你想编译 opencv_contrib 也就是 opencv extra库的话，你需要把[https://github.com/Itseez/opencv_contrib](https://github.com/Itseez/opencv_contrib)额外库包也下载下来。我曾经把opencv_contrib包的tracking模块 加入到opencv 核心库，编译到PC的CodeBlocks开发环境就成功了，但是编译成Android的环境就出错了，这个问题我一直没有解决，最后我换用了其它的方法，我把相应的一些资料提供给大家：



[https://github.com/alexkarargyris/Caffe_OpenCV_Android_App](https://github.com/alexkarargyris/Caffe_OpenCV_Android_App)
[https://zami0xzami.wordpress.com/2016/03/17/building-opencv-for-android-from-source/](https://zami0xzami.wordpress.com/2016/03/17/building-opencv-for-android-from-source/)
我参考了上述的资料，在加载opencv 额外库的时候，编译了几次也没能成功，会报cmake 编译出错。我也让C++ 的同事尝试用cmake 帮我编译，也没能成功，也许我们对cmake 了解不够深，据说opencv extra 库是需要收费的，所以Android端不能直接编译进来，但是PC端是成功编译了的。如有编译成功的大神，请多多指教！









