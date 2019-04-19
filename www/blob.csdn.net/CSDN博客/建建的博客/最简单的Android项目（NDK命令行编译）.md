# 最简单的Android项目（NDK命令行编译） - 建建的博客 - CSDN博客
2017年06月02日 10:04:44[纪建](https://me.csdn.net/u013898698)阅读数：795
Android的NDK编程需要下载NDK编译环境，可以从[官网](http://dl.google.com/android/repository/android-ndk-r11c-windows-x86_64.zip)下载window64位版，然后解压到任意目录即可。
NDK的实现其实是利用里Java的jni方法，所以前期的步骤可以参考jni的实现方法，只是编译时可以利用NDK工具来进行。
首先将Java源程序写好，例如：
```
1 package test.android;
 2 
 3 import android.app.Activity;
 4 import android.os.Bundle;
 5 import android.view.View;
 6 import android.widget.Toast;
 7 import android.widget.LinearLayout;
 8 import android.widget.Button;
 9 
10 public class Mini extends Activity {
11   
12   static {
13     System.loadLibrary("jt");
14   }
15   
16   public native String stringFromJNI();
17   
18   public void onCreate(Bundle savedInstanceState) {
19     super.onCreate(savedInstanceState);
20     
21     LinearLayout lla = new LinearLayout(this);
22     Button b = new Button(this);
23     b.setText("click me");
24     
25     lla.addView(b);
26     this.setContentView(lla);
27     
28     final Activity _this = this;
29     
30     b.setOnClickListener(new View.OnClickListener() {
31       @Override
32       public void onClick(View v) {
33         Toast.makeText(_this, stringFromJNI(), Toast.LENGTH_LONG).show();
34       }
35     });
36   }
37 }
```
利用javah工具生成.h头文件，然后在项目根目录新建jni目录，在jni目录新建jni.c文件，如下：
```
1 #include <string.h>
2 #include <jni.h>
3 #include "test_android_Mini.h"
4 
5 JNIEXPORT jstring JNICALL Java_test_android_Mini_stringFromJNI
6   (JNIEnv *env, jobject _this)
7 {
8   return (*env)->NewStringUTF(env, "return from c");
9 }
```
在jni目录里新建Android.mk文件（注意大小写），这个文件是说明如何编译动态链接库的，内容如下：
```
1 LOCAL_PATH := $(call my-dir)
2 
3 include $(CLEAR_VARS)
4 
5 LOCAL_MODULE := jt
6 LOCAL_SRC_FILES := jni.c
7 
8 include $(BUILD_SHARED_LIBRARY)
```
打开命令处理程序，切换到项目目录，输入以下命令：
```
D:\Android\android-ndk-r11c\ndk-build
```
ndk-build前面是你的ndk解压目录。
这时，项目下会产生libs目录，里面就是生成的动态链接库。
因为android有多重cpu平台，所以还可以用一个文件来配置生成的平台类型。
在jni目录下新建Application.mk（注意大小写），输入如下内容：
```
APP_ABI := armeabi armeabi-v7a x86
```
再次使用ndk-build命令编译，就会在libs下生成三种平台的动态链接库
接下来打包的过程和以前的打包步骤只有一步不同，用apkbuilder将所有文件打包成apk时，加上动态链接库的文件目录，用-nf 目录，例如：
```
D:\Android\sdk\tools\apkbuilder.bat \workspace\test\android\minimum\mini_unsigned.apk -v -u -z D:\workspace\test\android\minimum\bin\mini.ap_ -f D:\workspace\test\android\minimum\bin\classes.dex -rf D:\workspace\test\android\minimum\src -nf D:\workspace\test\android\minimum\libs
```
