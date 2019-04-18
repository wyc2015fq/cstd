# 安卓使用JNI-NDK - wishchinYang的专栏 - CSDN博客
2015年07月01日 15:35:43[wishchin](https://me.csdn.net/wishchin)阅读数：553
个人分类：[人形机器人](https://blog.csdn.net/wishchin/article/category/5597059)
详细配置，参考链接：[http://www.jb51.net/softjc/115204.html](http://www.jb51.net/softjc/115204.html)
**一  、为什么使用NDK**
1.代码的保护。由于apk的java层代码很容易被反编译，而C/C++库反汇难度较大。
2.可以方便地使用现存的开源库。大部分现存的开源库都是用C/C++代码编写的。
3.提高程序的执行效率。将要求高性能的应用逻辑使用C开发，从而提高应用程序的执行效率。
4.便于移植。用C/C++写得库可以方便在其他的嵌入式平台上再次使用。
**二、NDK简介**
1.NDK是一系列工具的集合
    NDK提供了一系列的工具，帮助开发者快速开发C（或C++）的动态库，并能自动将so和java应用一起打包成apk。这些工具对开发者的帮助是巨大的。
    NDK集成了交叉编译器，并提供了相应的mk文件隔离CPU、平台、ABI等差异，开发人员只需要简单修改mk文件（指出“哪些文件需要编译”、“编译特性要求”等），就可以创建出so。
    NDK可以自动地将so和Java应用一起打包，极大地减轻了开发人员的打包工作。
2.NDK提供了一份稳定、功能有限的API头文件声明
    Google明确声明该API是稳定的，在后续所有版本中都稳定支持当前发布的API。从该版本的NDK中看出，这些API支持的功能非常有限，包含有：C标准库（libc）、标准数学库（libm）、压缩库（libz）、Log库（liblog）。
**三、在Eclipse中使用NDK**
1.在eclips中配置NDK路径
在eclipse中点击菜单栏window-Perferences，配置NDK的路径。
![](https://img-blog.csdn.net/20150701151309645?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**2.在工程中添加native lib**新建一个空白的Android工程，在根目录上点击右键Adnroid Tools - Add Native Support...
在弹出的对话框中输入要新建的lib的名称，点击Finish，在工程目录下会多出一个jni文件夹，里面有一个空白的cpp文件和一个Android.mk文件
![](https://img-blog.csdn.net/20150701151529777?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3. Android.mk文件的内容是
```java
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := feelJulyGpsMap
LOCAL_SRC_FILES := feelJulyGpsMap.cpp
include $(BUILD_SHARED_LIBRARY)
```
feelJulyGpsMapJni就是要加载的lib的名称，LOCAL_SRC_FILES 是资源文件的路径。
feelJulyGpsMap.cpp的内容是：
```java
#include <jni.h>
extern "C" jstring Java_com_example_feeljulygpsmap_FeelJulyActivity_careJuly(JNIEnv *env, jobject thiz) {
	return env->NewStringUTF( "Hello, July!" );
}
```
FeelJulyActivity.java的内容是：
```java
public native String careJuly();
    
	static {
		System.loadLibrary("feelJulyGpsMap");
		}
```
直接运行程序，可以在Console窗口中看到NDK build的输出：
![](https://img-blog.csdn.net/20150701153352406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**四、使用NDK**
**............................**
