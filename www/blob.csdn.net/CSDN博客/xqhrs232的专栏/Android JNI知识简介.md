# Android JNI知识简介 - xqhrs232的专栏 - CSDN博客
2010年12月22日 10:59:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：650标签：[android																[jni																[java																[ubuntu																[class																[permissions](https://so.csdn.net/so/search/s.do?q=permissions&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/linweig/archive/2010/03/26/5417319.aspx](http://blog.csdn.net/linweig/archive/2010/03/26/5417319.aspx)
Java Native Interface (JNI)标准是java平台的一部分，它允许Java代码和其他语言写的代码进行交互。JNI 是本地编程接口，它使得在 Java 虚拟机 (VM) 内部运行的 Java 代码能够与用其它编程语言(如 C、C++ 和汇编语言)编写的应用程序和库进行交互操作。
1.从如何载入.so档案谈起
    由于Android的应用层的类都是以Java写的，这些Java类编译为Dex型式的Bytecode之后，必须靠Dalvik虚拟机(VM: Virtual Machine)来执行。VM在Android平台里，扮演很重要的角色。
    此外，在执行Java类的过程中，如果Java类需要与C组件沟通时，VM就会去载入C组件，然后让Java的函数顺利地调用到C组件的函数。此时，VM扮演着桥梁的角色，让Java与C组件能通过标准的JNI介面而相互沟通。
    应用层的Java类是在虚拟机(VM: Vitual Machine)上执行的，而C件不是在VM上执行，那么Java程式又如何要求VM去载入(Load)所指定的C组件呢? 可使用下述指令：
     System.loadLibrary(*.so的档案名);
    例如，Android框架里所提供的MediaPlayer.java类，含指令：
     public class MediaPlayer{   
         static {
                   System.loadLibrary("media_jni");
              }
     }
     这要求VM去载入Android的/system/lib/libmedia_jni.so档案。载入*.so之后，Java类与*.so档案就汇合起来，一起执行了。
2.如何撰写*.so的入口函数
    ---- JNI_OnLoad()与JNI_OnUnload()函数的用途
 当Android的VM(Virtual Machine)执行到System.loadLibrary()函数时，首先会去执行C组件里的JNI_OnLoad()函数。它的用途有二：
(1)告诉VM此C组件使用那一个JNI版本。如果你的*.so档没有提供JNI_OnLoad()函数，VM会默认该*.so档是使用最老的JNI 1.1版本。由于新版的JNI做了许多扩充，如果需要使用JNI的新版功能，例如JNI 1.4的java.nio.ByteBuffer,就必须藉由JNI_OnLoad()函数来告知VM。
(2)由于VM执行到System.loadLibrary()函数时，就会立即先呼叫JNI_OnLoad()，所以C组件的开发者可以藉由JNI_OnLoad()来进行C组件内的初期值之设定(Initialization) 。
例如，在Android的/system/lib/libmedia_jni.so档案里，就提供了JNI_OnLoad()函数，其程式码片段为：
//#define LOG_NDEBUG 0
#define LOG_TAG "MediaPlayer-JNI"
jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("ERROR: GetEnv failed/n");
        goto bail;
    }
assert(env != NULL);
    if (register_android_media_MediaPlayer(env) < 0) {
        LOGE("ERROR: MediaPlayer native registration failed/n");
        goto bail;
    }
    if (register_android_media_MediaRecorder(env) < 0) {
        LOGE("ERROR: MediaRecorder native registration failed/n");
        goto bail;
    }
    if (register_android_media_MediaScanner(env) < 0) {
        LOGE("ERROR: MediaScanner native registration failed/n");
        goto bail;
}
    if (register_android_media_MediaMetadataRetriever(env) < 0) {
        LOGE("ERROR: MediaMetadataRetriever native registration failed/n");
        goto bail;
    }
    /* success -- return valid version number */
    result = JNI_VERSION_1_4;
bail:
    return result;
}
 此函数回传JNI_VERSION_1_4值给VM，于是VM知道了其所使用的JNI版本了。此外，它也做了一些初期的动作(可呼叫任何本地函数)，例如指令：
   if (register_android_media_MediaPlayer(env) < 0) {
        LOGE("ERROR: MediaPlayer native registration failed/n");
        goto bail;
    }
就将此组件提供的各个本地函数(Native Function)登记到VM里，以便能加快后续呼叫本地函数的效率。
JNI_OnUnload()函数与JNI_OnLoad()相对应的。在载入C组件时会立即呼叫JNI_OnLoad()来进行组件内的初期动作；而当VM释放该C组件时，则会呼叫JNI_OnUnload()函数来进行善后清除动作。当VM呼叫JNI_OnLoad()或JNI_Unload()函数时，都会将VM的指针(Pointer)传递给它们，其参数如下：
jint JNI_OnLoad(JavaVM* vm, void* reserved) {     }
jint JNI_OnUnload(JavaVM* vm, void* reserved){     }
在JNI_OnLoad()函数里，就透过VM之指标而取得JNIEnv之指标值，并存入env指标变数里，如下述指令：
jint JNI_OnLoad(JavaVM* vm, void* reserved){
     JNIEnv* env = NULL;
     jint result = -1;
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
          LOGE("ERROR: GetEnv failed/n");
          goto bail;
    }
}
由于VM通常是多执行绪(Multi-threading)的执行环境。每一个执行绪在呼叫JNI_OnLoad()时，所传递进来的JNIEnv指标值都是不同的。为了配合这种多执行绪的环境，C组件开发者在撰写本地函数时，可藉由JNIEnv指标值之不同而避免执行绪的资料冲突问题，才能确保所写的本地函数能安全地在Android的多执行绪VM里安全地执行。基于这个理由，当在呼叫C组件的函数时，都会将JNIEnv指标值传递给它，如下：
jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
JNIEnv* env = NULL;
    if (register_android_media_MediaPlayer(env) < 0) {
   }
}
这JNI_OnLoad()呼叫register_android_media_MediaPlayer(env)函数时，就将env指标值传递过去。如此，在register_android_media_MediaPlayer()函数就能藉由该指标值而区别不同的执行绪，以便化解资料冲突的问题。
　例如，在register_android_media_MediaPlayer()函数里，可撰写下述指令：
       if ((*env)->MonitorEnter(env, obj) != JNI_OK) {
       }
查看是否已经有其他执行绪进入此物件，如果没有，此执行绪就进入该物件里执行了。还有，也可撰写下述指令：
       if ((*env)->MonitorExit(env, obj) != JNI_OK) {
        }
查看是否此执行绪正在此物件内执行，如果是，此执行绪就会立即离开。
3.registerNativeMethods()函数的用途
应用层级的Java类别透过VM而呼叫到本地函数。一般是仰赖VM去寻找*.so里的本地函数。如果需要连续呼叫很多次，每次都需要寻找一遍，会多花许多时间。此时，组件开发者可以自行将本地函数向VM进行登记。例如，在Android的/system/lib/libmedia_jni.so档案里的代码段如下：
//#define LOG_NDEBUG 0
#define LOG_TAG "MediaPlayer-JNI"
static JNINativeMethod gMethods[] = {
    {"setDataSource",   "(Ljava/lang/String;)V",
     (void *)android_media_MediaPlayer_setDataSource},
    {"setDataSource",    "(Ljava/io/FileDescriptor;JJ)V",
(void *)android_media_MediaPlayer_setDataSourceFD},
    {"prepare",   "()V",     (void *)android_media_MediaPlayer_prepare},
    {"prepareAsync",   "()V",   (void *)android_media_MediaPlayer_prepareAsync},
    {"_start",   "()V",   (void *)android_media_MediaPlayer_start},
    {"_stop",   "()V",    (void *)android_media_MediaPlayer_stop},
    {"getVideoWidth",   "()I",      (void *)android_media_MediaPlayer_getVideoWidth},
    {"getVideoHeight",   "()I",      (void *)android_media_MediaPlayer_getVideoHeight},
    {"seekTo",          "(I)V",      (void *)android_media_MediaPlayer_seekTo},
    {"_pause",          "()V",       (void *)android_media_MediaPlayer_pause},
    {"isPlaying",        "()Z",       (void *)android_media_MediaPlayer_isPlaying},
    {"getCurrentPosition", "()I",  (void *)android_media_MediaPlayer_getCurrentPosition},
    {"getDuration",      "()I",     (void *)android_media_MediaPlayer_getDuration},
    {"_release",         "()V",      (void *)android_media_MediaPlayer_release},
    {"_reset",           "()V",        (void *)android_media_MediaPlayer_reset},
    {"setAudioStreamType","(I)V", (void *)android_media_MediaPlayer_setAudioStreamType},
    {"setLooping",      "(Z)V",      (void *)android_media_MediaPlayer_setLooping},
    {"setVolume",      "(FF)V",     (void *)android_media_MediaPlayer_setVolume},
    {"getFrameAt",     "(I)Landroid/graphics/Bitmap;",
         (void *)android_media_MediaPlayer_getFrameAt},
    {"native_setup",    "(Ljava/lang/Object;)V",
           (void *)android_media_MediaPlayer_native_setup},
    {"native_finalize",     "()V",    (void *)android_media_MediaPlayer_native_finalize},
};
static int register_android_media_MediaPlayer(JNIEnv *env){
    return AndroidRuntime::registerNativeMethods(env,
                "android/media/MediaPlayer", gMethods, NELEM(gMethods));
}
jint JNI_OnLoad(JavaVM* vm, void* reserved){
    if (register_android_media_MediaPlayer(env) < 0) {
        LOGE("ERROR: MediaPlayer native registration failed/n");
        goto bail;
    }
}
当VM载入libmedia_jni.so档案时，就呼叫JNI_OnLoad()函数。接着，JNI_OnLoad()呼叫register_android_media_MediaPlayer()函数。此时，就呼叫到AndroidRuntime::registerNativeMethods()函数，向VM(即AndroidRuntime)登记gMethods[]表格所含的本地函数了。简而言之，registerNativeMethods()函数的用途有二：
(1)更有效率去找到函数。
(2)可在执行期间进行抽换。由于gMethods[]是一个<名称，函数指针>对照表，在程序执行时，可多次呼叫registerNativeMethods()函数来更换本地函数之指针，而达到弹性抽换本地函数之目的。
4.Andoird 中使用了一种不同传统Java JNI的方式来定义其native的函数。其中很重要的区别是Andorid使用了一种Java 和 C 函数的映射表数组，并在其中描述了函数的参数和返回值。这个数组的类型是JNINativeMethod，定义如下：
typedef struct {
const char* name;            /*Java中函数的名字*/         
const char* signature;      /*描述了函数的参数和返回值*/
void* fnPtr;               /*函数指针，指向C函数*/
} JNINativeMethod;
其中比较难以理解的是第二个参数，例如
"()V"
"(II)V"
"(Ljava/lang/String;Ljava/lang/String;)V"
实际上这些字符是与函数的参数类型一一对应的。
"()" 中的字符表示参数，后面的则代表返回值。例如"()V" 就表示void Func();
"(II)V" 表示 void Func(int, int);
具体的每一个字符的对应关系如下
字符   Java类型     C类型
V      void         void
Z      jboolean     boolean
I       jint         int
J       jlong        long
D      jdouble       double
F      jfloat            float
B      jbyte            byte
C      jchar           char
S      jshort          short
数组则以"["开始，用两个字符表示
[I     jintArray       int[]
[F     jfloatArray     float[]
[B     jbyteArray     byte[]
[C    jcharArray      char[]
[S    jshortArray      short[]
[D    jdoubleArray    double[]
[J     jlongArray      long[]
[Z    jbooleanArray    boolean[]
上面的都是基本类型。如果Java函数的参数是class，则以"L"开头，以";"结尾，中间是用"/" 隔开的包及类名。而其对应的C函数名的参数则为jobject. 一个例外是String类，其对应的类为jstring
Ljava/lang/String; String jstring
Ljava/net/Socket; Socket jobject
如果JAVA函数位于一个嵌入类，则用$作为类名间的分隔符。
例如 "(Ljava/lang/String;Landroid/os/FileUtils$FileStatus;)Z"
Android JNI编程实践
一、直接使用java本身jni接口（windows/ubuntu）
1.在Eclipsh中新建一个android应用程序。两个类：一个继承于Activity，UI显示用。另一个包含native方法。编译生成所有类。
jnitest.java文件：
package com.hello.jnitest;
import android.app.Activity;
import android.os.Bundle;
public class jnitest extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        Nadd cal = new Nadd();
        setTitle("The Native Add Result is " + String.valueOf(cal.nadd(10, 19)));
    }
}
Nadd.java文件：
package com.hello.jnitest;
public class Nadd {
    static {
        System.loadLibrary("Nadd");
    }
    public native int nadd(int a, int b);
}
以上在windows中完成。
2.使用javah命令生成C/C++的.h文件。注意类要包含包名，路径文件夹下要包含所有包中的类，否则会报找不到类的错误。classpath参数指定到包名前一级文件夹，文件夹层次结构要符合java类的组织层次结构。
javah -classpath ../jnitest/bin com.hello.jnitest.Nadd
com_hello_jnitest_Nadd .h文件：
/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_hello_jnitest_Nadd */
#ifndef _Included_com_hello_jnitest_Nadd
#define _Included_com_hello_jnitest_Nadd
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_hello_jnitest_Nadd
 * Method:    nadd
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_com_hello_jnitest_Nadd_nadd
  (JNIEnv *, jobject, jint, jint);
#ifdef __cplusplus
}
#endif
#endif
3.编辑.c文件实现native方法。
com_hello_jnitest_Nadd.c文件：
#include <stdlib.h>
#include "com_hello_jnitest_Nadd.h"
JNIEXPORT jint JNICALL Java_com_hello_jnitest_Nadd_nadd(JNIEnv * env, jobject c, jint a, jint b)
{
   return (a+b);
}
4.编译.c文件生存动态库。
arm-none-linux-gnueabi-gcc -I/home/a/work/android/jdk1.6.0_17/include -I/home/a/work/android/jdk1.6.0_17/include/linux -fpic -c com_hello_jnitest_Nadd.c
arm-none-linux-gnueabi-ld -T/home/a/CodeSourcery/Sourcery_G++_Lite/arm-none-linux-gnueabi/lib/ldscripts/armelf_linux_eabi.xsc -share -o libNadd.so com_hello_jnitest_Nadd.o
得到libNadd.so文件。
以上在ubuntu中完成。
5.将相应的动态库文件push到avd的system/lib中:adb push libNadd.so /system/lib。若提示Read-only file system错误，运行adb remount命令，即可。
Adb push libNadd.so /system/lib
6.在eclipsh中运行原应用程序即可。
以上在windows中完成。
对于一中生成的so文件也可采用二中的方法编译进apk包中。只需在工程文件夹中建libs/armeabi文件夹（其他文件夹名无效，只建立libs文件夹也无效），然后将so文件拷入，编译工程即可。
二.使用NDK生成本地方法（ubuntu and windows）
1.安装NDK：解压，然后进入NDK解压后的目录，运行build/host-setup.sh（需要Make 3.81和awk）。若有错，修改host-setup.sh文件：将#!/bin/sh修改为#!/bin/bash，再次运行即可。
2.在apps文件夹下建立自己的工程文件夹，然后在该文件夹下建一文件Application.mk和项project文件夹。
Application.mk文件：
APP_PROJECT_PATH := $(call my-dir)/project
APP_MODULES      := myjni
3.在project文件夹下建一jni文件夹，然后新建Android.mk和myjni.c。这里不需要用javah生成相应的.h文件，但函数名要包含相应的完整的包、类名。
4.编辑相应文件内容。
Android.mk文件：
# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := myjni
LOCAL_SRC_FILES := myjni.c
include $(BUILD_SHARED_LIBRARY)
myjni.c文件：
#include <string.h>
#include <jni.h>
jstring
Java_com_hello_NdkTest_NdkTest_stringFromJNI( JNIEnv* env,
                                                  jobject thiz )
{
    return (*env)->NewStringUTF(env, "Hello from My-JNI !");
}
myjni文件组织：
[a@ubuntu:~/work/android/ndk-1.6_r1/apps$](mailto:a@ubuntu:~/work/android/ndk-1.6_r1/apps$) tree myjni
myjni
|-- Application.mk
`-- project
    |-- jni
    |   |-- Android.mk
    |   `-- myjni.c
    `-- libs
        `-- armeabi
            `-- libmyjni.so
4 directories, 4 files
5.编译：make APP=myjni.
以上内容在ubuntu完成。以下内容在windows中完成。当然也可以在ubuntu中完成。
6.在eclipsh中创建android application。将myjni中自动生成的libs文件夹拷贝到当前工程文件夹中，编译运行即可。
NdkTest.java文件：
package com.hello.NdkTest;
import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
public class NdkTest extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        TextView  tv = new TextView(this);
        tv.setText( stringFromJNI() );
        setContentView(tv);
    }
    public native String  stringFromJNI();
    static {
        System.loadLibrary("myjni");
    }
}
对于二中生成的so文件也可采用一中的方法push到avd中运行。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/linweig/archive/2010/03/26/5417319.aspx](http://blog.csdn.net/linweig/archive/2010/03/26/5417319.aspx)
