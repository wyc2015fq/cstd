# JNI学习方法 - xmdxcsj的专栏 - CSDN博客





2015年08月26日 19:14:27[xmucas](https://me.csdn.net/xmdxcsj)阅读数：443








一、概况

1.      Jni的范例参考：适合初学者的范例

[https://www3.ntu.edu.sg/home/ehchua/programming/java/JavaNativeInterface.html](https://www3.ntu.edu.sg/home/ehchua/programming/java/JavaNativeInterface.html)

2.      Jni的官方文档：适合于查询函数用法和细节

[http://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/jniTOC.html](http://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/jniTOC.html)

3.      Jni的tips：编程规范

[http://developer.android.com/training/articles/perf-jni.html](http://developer.android.com/training/articles/perf-jni.html)

二、Jni编程的大致流程

Javah问题参考：[http://blog.csdn.net/hejinjing_tom_com/article/details/8125648](http://blog.csdn.net/hejinjing_tom_com/article/details/8125648)

1. 编写java类文件NameJNIInterface.java(文件名和类名一致)


package com.example.voice.name;

import android.util.Log;

/**

 *JNI 接口类

 */

public class NameJNIInterface{

        public static final String TAG = "name";

        public native static long nameInit(Stringdir, int[] error, float rate);

        public native static StringnameProcess(long ptr, String input);

        public native static void nameDestroy( long ptr);



        static {

              Log. d(TAG, "beginto load name");

              System. loadLibrary("name");

              Log. d(TAG, "finishedload name");

       }

}


2. 根据包名确定路径

mkdir -p com/example/voice/name/

将java文件放到该目录

3. 编译该java文件

cd com/example/voice/name/ &&javac NameJNIInterface.java

会生成NameJNIInterface.class文件

4. 返回到当前目录

cd ../../../

5. 生成头文件

javah  com.example.voice.name.NameJNIInterface

会生成相应的NameJNIInterface.h文件


/* DO NOTEDIT THIS FILE - it is machine generated */

#include<jni.h>

/* Headerfor class com_example_voice_name_NameJNIInterface */

#ifndef_Included_com_example_voice_name_NameJNIInterface

#define_Included_com_example_voice_name_NameJNIInterface

#ifdef__cplusplus

extern"C" {

#endif

/*

 * Class:    com_example_voice_name_NameJNIInterface

 * Method:   nameInit

 * Signature: (Ljava/lang/String;[IF)J

 */

JNIEXPORTjlong JNICALL Java_com_example_voice_name_NameJNIInterface_nameInit

  (JNIEnv *, jclass, jstring, jintArray,jfloat);



/*

 * Class:    com_example_voice_name_NameJNIInterface

 * Method:   nameProcess

 * Signature:(JLjava/lang/String;)Ljava/lang/String;

 */

JNIEXPORTjstring JNICALL Java_com_example_voice_name_NameJNIInterface_nameProcess

  (JNIEnv *, jclass, jlong, jstring);



/*

 * Class:    com_example_voice_name_NameJNIInterface

 * Method:   nameDestroy

 * Signature: (J)V

 */

JNIEXPORTvoid JNICALL Java_com_example_voice_name_NameJNIInterface_nameDestroy

  (JNIEnv *, jclass, jlong);



#ifdef__cplusplus

}

#endif

#endif


6. 实现对应的native函数

三、Jni函数实现

重点在于java的数据类型和c数据类型之间的转化，重点参考：

[https://www3.ntu.edu.sg/home/ehchua/programming/java/JavaNativeInterface.html](https://www3.ntu.edu.sg/home/ehchua/programming/java/JavaNativeInterface.html)



