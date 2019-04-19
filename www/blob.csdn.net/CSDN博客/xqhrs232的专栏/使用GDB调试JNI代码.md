# 使用GDB调试JNI代码 - xqhrs232的专栏 - CSDN博客
2012年04月24日 10:23:50[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：671标签：[jni																[android																[library																[button																[path																[module](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/ba_jie/article/details/6651215](http://blog.csdn.net/ba_jie/article/details/6651215)
调试JNI的c/c++代码，我一直使用打印log的方式，费时费力。在网上搜索使用GDB调试JNI的文章也不多。后来在stackoverflow偶然看到ndk-gdb才知道自己把NDK的重要文档忽略了。于是赶快试验，没想到确实不难。现把使用方法记录下来。
NDK文档我总结了一下，有几方面需要了解：
● 使用方式
cd $PROJECT
$NDK/ndk-gdb
● 前提条件
- 必须使用 ndk-build 编译native代码；使用 make APP=<name> 是不行的
- AndroidManifest.xml 的 android:debuggable
- 使用Android 2.2+的设备或模拟器
- 使用Eclipse的话，ADT 0.9.7+
- 使用ant的话，Android 2.1 r2+ / 2.2 r1+
● ndk-gdb参数
--verbose        显示详细的会话启动信息。当不能启动GDB时，打开它可以得到更多帮助。
--force            如果存在了一个调试会话，先终止它，然后再启动自己。
--start            启动程序默认的activity，然后连接。默认是寻找系统中已经运行的程序。
--launch=<name>    指定要启动的activity，其它的和上面一样。
--launch-list    打印出可启动的所有activity。
--project=<path>    如果未进入项目目录而运行 ndk-gdb，必须使用该参数指明路径。
--port=<port>        默认使用TCP端口5039和程序通信。指定别的端口可以同时调试多个设备。
--adb=<file>    指明ADB的路径。最好把adb放到PATH环境变量里。
-d, -e, -s <serial>    调试设备？模拟器？或者指定号码（adb devices）
--exec=<file>
-x <file>        自定义一个初始化脚本，GDB每次启动都会调用。
● 只有Android 2.3的平台才能调试多线程。低版本的设备只能调试主线程。
下面是我调试JNI的步骤。
整个例子脱胎于NDK自带的hello-jni。
1. 先看java代码：
**[java]**[view plain](http://blog.csdn.net/ba_jie/article/details/6651215#)[copy](http://blog.csdn.net/ba_jie/article/details/6651215#)[print](http://blog.csdn.net/ba_jie/article/details/6651215#)[?](http://blog.csdn.net/ba_jie/article/details/6651215#)
- publicclass HelloAndroid extends Activity  
- {  
- private Button mButton;  
- private TextView mText;  
- 
- @Override
- publicvoid onCreate(Bundle savedInstanceState)  
-     {  
- super.onCreate(savedInstanceState);  
-         setContentView(R.layout.main);  
- 
-         mText = (TextView) findViewById(R.id.TVHelloAndroid);  
-         mButton = (Button) findViewById(R.id.button1);  
- 
-         mButton.setOnClickListener(new View.OnClickListener()  
-         {  
- @Override
- publicvoid onClick(View v)  
-             {  
-                 mText.setText(stringFromJNI());  
-             }  
-         });  
-     }  
- 
- publicnative String stringFromJNI();  
- 
- static {  
-         System.loadLibrary("HelloAndroid-jni");  
-     }  
- }  
public class HelloAndroid extends Activity
{
	private Button mButton;
	private TextView mText;
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		mText = (TextView) findViewById(R.id.TVHelloAndroid);
		mButton = (Button) findViewById(R.id.button1);
        mButton.setOnClickListener(new View.OnClickListener()
        {
			@Override
			public void onClick(View v)
			{
				mText.setText(stringFromJNI());
			}
		});
    }
	public native String stringFromJNI();
	static {
		System.loadLibrary("HelloAndroid-jni");
	}
}
2. 再看JNI代码-HelloAndroid-jni.cpp：
**[cpp]**[view plain](http://blog.csdn.net/ba_jie/article/details/6651215#)[copy](http://blog.csdn.net/ba_jie/article/details/6651215#)[print](http://blog.csdn.net/ba_jie/article/details/6651215#)[?](http://blog.csdn.net/ba_jie/article/details/6651215#)
- #include <string> 
- #include <jni.h> 
- #include <android/log.h> 
- #include "util.h"       // 引用另外一个自定义模块的函数
- 
- extern"C" {  
- 
- JNIEXPORT jstring JNICALL  
- Java_com_android_helloandroid_HelloAndroid_stringFromJNI( JNIEnv* env, jobject thiz )  
- {  
- int i = GetRandomInt(2, 10);  
-     std::string str(i, 'c');  
-     str.append(", Hello Android from JNI");  
- return env->NewStringUTF(str.c_str());  
- }  
- 
- }  
#include <string>
#include <jni.h>
#include <android/log.h>
#include "util.h"       // 引用另外一个自定义模块的函数
extern "C" {
JNIEXPORT jstring JNICALL
Java_com_android_helloandroid_HelloAndroid_stringFromJNI( JNIEnv* env, jobject thiz )
{
	int i = GetRandomInt(2, 10);
	std::string str(i, 'c');
	str.append(", Hello Android from JNI");
	return env->NewStringUTF(str.c_str());
}
}

3. 上面的JNI代码中使用了另外一个JNI模块定义的函数GetRandomInt，其代码util.c为：
**[cpp]**[view plain](http://blog.csdn.net/ba_jie/article/details/6651215#)[copy](http://blog.csdn.net/ba_jie/article/details/6651215#)[print](http://blog.csdn.net/ba_jie/article/details/6651215#)[?](http://blog.csdn.net/ba_jie/article/details/6651215#)
- #include <stdlib.h> 
- 
- /**
-  * @return a random integer in [min, max].
-  */
- int GetRandomInt(int min, int max)  
- {  
- return rand() % (max-min + 1) + min;  
- }  
#include <stdlib.h>
/**
 * @return a random integer in [min, max].
 */
int GetRandomInt(int min, int max)
{
	return rand() % (max-min + 1) + min;
}
4. 然后，把util.c这个模块编译成libutil.a，使用到的Android.mk和Application.mk文件如下示意：
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := util
LOCAL_SRC_FILES := util.c
include $(BUILD_STATIC_LIBRARY)
APP_ABI := armeabi
APP_STL := gnustl_static
5. 然后把HelloAndroid-jni.cpp编译成最后的.so文件：
LOCAL_PATH := $(call my-dir)
TARGET_ARCH_ABI := armeabi
###################
#   prebuilt lib
###################
include $(CLEAR_VARS)
LOCAL_MODULE := util-prebuilt
LOCAL_SRC_FILES := ../../archive/util/obj/local/$(TARGET_ARCH_ABI)/libutil.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../archive/util/jni
include $(PREBUILT_STATIC_LIBRARY)
###################
#   final target
###################
include $(CLEAR_VARS)
LOCAL_MODULE := HelloAndroid-jni
LOCAL_SRC_FILES := HelloAndroid-jni.cpp
LOCAL_CFLAGS := -fsigned-char
# prebuilt
LOCAL_STATIC_LIBRARIES := util-prebuilt
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)
APP_STL := gnustl_static
6. 接下来就是编译、链接、安装apk文件了。注意，要安装到**模拟器**里。
7. 打开Cygwin的bash shell窗口，键入下面的命令：
export ANDROID_NDK_ROOT=/cygdrive/y/android_ndk_r5c
8. 最后键入下面的命令：
ndk-gdb --force --start -e
--force 表示关掉正在运行的实例
--start 表示启动新的实例
-e 表示使用模拟器
9. 等待十几秒钟，gdb会装入所有能找到的调试信息，最后出现gdb的提示符。成功！
使用ndk-gdb调试确实很简单。无需-g；自动寻找源文件；速度也可以。不足之处：若想在设备上使用，必须取得root权限才可。

