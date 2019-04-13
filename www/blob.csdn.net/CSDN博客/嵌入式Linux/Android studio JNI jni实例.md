
# Android studio JNI jni实例 - 嵌入式Linux - CSDN博客

2019年02月26日 17:06:59[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：73


**1.Jni的作用**
![](https://pic.newrank.cn/mmbiz_png/Qof5hj3zMPcNibzqVG8q8Gibv1zkd76uxPUHCnk2ufncME124CW8GDkudQiaeYr0NrtBdqJ2onE7AyF7Vfo0XJUiag/640?wx_fmt=png)
# 1.Jni的作用
Java是一种比较高级的语言，Java调用c库，调用c++库是必不可少的，所以Jni就应运而生了。看了这个文章的同学，应该能够自己写个APK装在自己的手机里面吧，以前刚开始做android的时候，写了一个“地瓜手电筒”和一个音乐播放器。
**NDK：**NDK是Native Development Kit的缩写，是Google提供的一套工具集，可以让你其他语言（C、C++或汇编）开发 Android的 JNI。NDK可以编译多平台的so，开发人员只需要简单修改 mk 文件说明需要的平台，不需要改动任何代码，NDK就可以帮你编译出所需的so库。
**JNI：**JNI是Java本机接口（Java Native Interface），是一个本机编程接口，它是Java软件开发工具箱（Java Software Development Kit，SDK）的一部分。JNI允许Java代码使用以其他语言编写的代码和代码库。Invocation API（JNI的一部分）可以用来将Java虚拟机（JVM）嵌入到本机应用程序中，从而允许程序员从本机代码内部调用Java代码。不过，对Java外部的调用通常不能移植到其他平台，在applet中还可能引发安全异常。实现本地代码将使您的Java应用程序无法通过100%纯Java测试。

# 2.生成一个JNI 的libjni.so库
我这个例子主要是在有android源码的编译环境下测试的，我有Android 7.1的开发编译环境，如果大家做Android 系统开发或者应用开发，有一个Android的开发环境非常有必要，或者需要自己下载Android NDK。

## 2.1.在源码位置下新建一个源码路径
![](https://pic.newrank.cn/mmbiz_png/Qof5hj3zMPcNibzqVG8q8Gibv1zkd76uxPFtoXWSrVkX65Tn6HgZW5tibZFKJMNGEyPga4Ccn1G9rcGu4yUOqQqiaw/640?wx_fmt=png)
HelloWorld.java源码如下
`package com.wqf.hellojni;
public class HelloWorld{
    public static native String helloWorld();
    public static native int add(int a,int b);
}`
## 2.2 用命令编译生成头文件
1、用命令
javac com/wqf/hellojni/HelloWorld.java
生成HelloWorld.class
2、用命令
**javah -classpath . -jni com.wqf.hellojni.HelloWorld**
生成头文件
![](https://pic.newrank.cn/mmbiz_png/Qof5hj3zMPcNibzqVG8q8Gibv1zkd76uxP46NMl85HGNibm7Mxx05Fj01QwDXpDQZReic2aYVx0FAwicibhKWic0pJJwg/640?wx_fmt=png)
**注意：**
网上很多说明用
javah -jni com.wqf.hellojni.HelloWorld
生成头文件，我试过很多次，都是失败，还是用我上面那个命令，可以看看javah的命令帮助
![](https://pic.newrank.cn/mmbiz_png/Qof5hj3zMPcNibzqVG8q8Gibv1zkd76uxPkG99LcsRunib3AcDGFeedXPCqiaVcnS2YQiaVIrBmA1N8s8bd5j3UaGFQ/640?wx_fmt=png)

## 2.3 编译生成libjni.so库步骤
### 2.3.1 在jni目录里面建立几个文件如下
1、把上面生成的
com_wqf_hellojni_HelloWorld.h
放到这个目录来
2、新建一个jni.c文件用来实现上面头文件里面的方法
3、新建一个Android.mk文件用来编译
### Android.mk的内容如下
`#########################################
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= jni.c
LOCAL_MODULE := libjni 
include $(BUILD_SHARED_LIBRARY)`
### 2.3.2 jni.c的内容如下
关于JNI的语法大家可以自行百度看看，说明的很多，这里不再详细说明，下面也比较简单。
`//自己创建一个c文件，实现自己定义的native方法，也就是.h文件中的方法
//引入自己生成的.h头文件
#include "com_wqf_hellojni_HelloWorld.h"
//返回一个字符串
JNIEXPORT jstring JNICALL Java_com_wqf_hellojni_HelloWorld_helloWorld(JNIEnv *env, jclass jobj) {
      return (*env)->NewStringUTF(env,"HelloWorld 我是用jni调用出来的字符串");
}
//返回 a+b的结果
JNIEXPORT jint JNICALL Java_com_wqf_hellojni_HelloWorld_add(JNIEnv *env, jclass jobj, jint a, jint b){
     return a+b;
}`
### 2.3.3 编译
使用**mm -B**命令编译生成so文件编译结果如下图
![](https://pic.newrank.cn/mmbiz_png/Qof5hj3zMPcNibzqVG8q8Gibv1zkd76uxPr2iaANZQGFxRfTib5eiaH9NOIb2pXAo7UNB50UMzwaUkhu3WFGomdILzQ/640?wx_fmt=png)说个小细节，我们编译生成的libjni.so，我们要会看他的几个参数
1、文件类型，使用file命令查看是32位的库还是64位的库
![](https://pic.newrank.cn/mmbiz_png/Qof5hj3zMPcNibzqVG8q8Gibv1zkd76uxPK16qQck7BtWurVd3ib97abb7j7d1eO5O410IgFrNS87EPzdBDOCfzQQ/640?wx_fmt=png)2、使用md5sum命令查看md5码，这个可以查看文件的唯一md5码，以后遇到一个文件名字一样的可以用这个方式知道是不是同一个文件
![](https://pic.newrank.cn/mmbiz_png/Qof5hj3zMPcNibzqVG8q8Gibv1zkd76uxPgv8Yy1l3giaLq7YjCAzCJNibFVbhczGbA0hTiasmicibtDXzDluCvJkibWaQ/640?wx_fmt=png)

# 3. 写个简单的apk测试一下
我们的APK编写用的是android studio来编写的，现在android studio是非常主流的android开发工具，如果大家想自己做一个小的DEMO的话，建议装这个工具，网上安装流程非常多了，下载SDK后，最好不要放在C盘，要不然重装系统后又没有了。
## 3.1先来一个整体的运行效果图片
**注意：**HelloWorld.java的包名要跟com_wqf_hellojni_HelloWorld .h上面生产库头文件对应起来，要不然运行会出现问题，要特别注意。
![](https://pic.newrank.cn/mmbiz_png/Qof5hj3zMPcNibzqVG8q8Gibv1zkd76uxPnV8l5BhzxkfR4N7KrfeAu378B2BmjMcAPsmMjIzrZlcIclX0Vc7LXQ/640?wx_fmt=png)我们在图片上可以看到add = 9,这个是我们在JNI里面的输出结果
![](https://pic.newrank.cn/mmbiz_png/Qof5hj3zMPcNibzqVG8q8Gibv1zkd76uxPUy7Bp7Gzyb4iayAmxsxI0WcUxLkrbRicRMgYTObRic5icHX3aADLYPM6Xg/640?wx_fmt=png)

## 3.2 APK编译步骤和几个注意的细节
我是在Android 7.1上编写APK进行测试的，相比其他Android版本，Android 7.1的问题更多，我说下操作步骤和细节。
### 3.2.1、我们先编写一个可以运行的APK
我们需要编写一个helloworld的简单apk，这个apk不需要引用库文件，直接输出一个helloworld。

### 3.2.2、然后我们引用我们的库文件
这里注意的是HelloWorld.java 里面的内容
加载库的代码
`package com.wqf.hellojni;
public class HelloWorld{
    static {
        //名字必须和libjni.so 名字对应起来
        System.loadLibrary("jni");
    }
    public static native String helloWorld();
    public static native int add(int a,int b);
}`在MainActivity.java里面导入包并调用add函数
![](https://pic.newrank.cn/mmbiz_png/Qof5hj3zMPcNibzqVG8q8Gibv1zkd76uxPBkuHTSwH4ic8OsOx6tU2HC0VTTX1wD6yxC6DbeELjJianiborv0bDLSBQ/640?wx_fmt=png)

# 4. 源码连接
源码我放在github上面，方面查看，有源码的例程大家研究起来更加得心应手
https://github.com/weiqifa0/androidjni
参考：
https://blog.csdn.net/a_zhon/article/details/53097512\#
![](https://pic.newrank.cn/mmbiz_png/Qof5hj3zMPdjctJM7xp9rYwERRBVjaK9m2FjZLQb1HE2dict0Bdm9STXoISgI1TUiavT4hZe89ZPtVR3PFAianvXQ/640?wx_fmt=png)
**欢迎加我微信拉大家进微信技术讨论群**
![](https://pic.newrank.cn/mmbiz_jpg/Qof5hj3zMPcTsskI3Umpnow3XlDvQmjgQksXNSEdpM7MsVZuTSia5SEw4ibo0R8LxSkj58RVsiaoGibumLElMbG9QQ/640?wx_fmt=jpeg)
**欢迎关注微信公众号-嵌入式Linux**
**觉得不错，请帮忙转发，您的每一次支持，我都将铭记于心**

![](https://pic.newrank.cn/mmbiz_jpg/Qof5hj3zMPcTsskI3Umpnow3XlDvQmjgP5yHl9ibRXBws3Yx7aroicU56T9oCn9WcYzI9I1gK7ZVWT3lh1WNLdAQ/640?wx_fmt=jpeg)


