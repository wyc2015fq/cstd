# Android NDK--自己编写调用JNI - Orisun - 博客园







# [Android NDK--自己编写调用JNI](https://www.cnblogs.com/zhangchaoyang/articles/1956889.html)





![](https://pic002.cnblogs.com/images/2011/103496/2011021916285589.jpg)

其实ubuntu中android开发环境的搭建也很简单

(1)下载android-sdk　　(2)为Eclipse安装ADT，从help->Install New SoftWare进入，地址输入http://dl-ssl.google.com/android/eclipse/

要进入NDK开发再下一个android-ndk就可以了

以上开发包均可以从[http://androidappdocs.appspot.com/sdk/ndk/index.html](http://androidappdocs.appspot.com/sdk/ndk/index.html)上下载得到

然后vi .bashrc，在PATH中添加3个路径：

android-sdk/tools　　android-sdk/platform-tools　　android-ndk

source .bashrc使更改生效

PATH下的工具命令可以直接使用，不需要指定路径。今天我们要用到android-sdk/tools下的android命令、android-sdk/platform-tools下的adb命令、android-ndk下的ndk-build命令。

Eclipse中使用NDK编程

1.New->Android Project

Project Name:hello-jni

Applicatin Name:hello-jni

create activity:HelloJni

Package Name:com.example.hellojni

Min SDK Version:3

2.项目下新建一个文件夹jni，该文件夹下新建两个文件:hello-jni.c和Android.mk

hello-jni.c

#include <string.h>
#include <jni.h>

jstring
Java_com_example_hellojni_HelloJni_stringFromJNI( JNIEnv* env,  jobject thiz )
{
    return (*env)->NewStringUTF(env, "Hello from JNI !");
}

Java_com_example_hellojni_HelloJni_stringFromJNI这个函数名称是有讲究的Java_　　固定com_example_hellojni_　　java代码的包名HelloJni_　　java代码的类名stringFromJNI　　java代码中使用的方法名
Android.mk

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := hello-jni
LOCAL_SRC_FILES := hello-jni.c

include $(BUILD_SHARED_LIBRARY)


3.编写Android程序

com.example.hellojni.HelloJni.java

package com.example.hellojni;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;


public class HelloJni extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        TextView  tv = new TextView(this);
        tv.setText( stringFromJNI() );
        setContentView(tv);
    }
    public native String  stringFromJNI();
    static {
        System.loadLibrary("hello-jni");
    }
}


4.编译C代码，生成共享库

进入项目的根目录$cd hello-jni

hello-jni $ android update project -p . -s

hello-jni $ ndk-build

此时libhello-jni.so就生成了，在bin下也有了apk包

5.直接在Eclipse中点击运行，或者手动安装apk包

首先输入android命令，启动模拟器

然后安装apk包：adb install bin/hello-jni.apk

6.卸载apk包

使用adb提供的shell命令

adb shell

#cd /data/app

#rm com.example.hellojin.apk












