
# Android Jni 例程 - 嵌入式Linux - CSDN博客

2018年10月18日 10:44:47[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：44



# 1.Jni的作用
Java是一种比较高级的语言，Java调用c库，调用c++库是必不可少的，所以Jni就应运而生了。
NDK：NDK是Native Development Kit的缩写，是Google提供的一套工具集，可以让你其他语言（C、C++或汇编）开发 Android的 JNI。NDK可以编译多平台的so，开发人员只需要简单修改 mk 文件说明需要的平台，不需要改动任何代码，NDK就可以帮你编译出所需的so库。
JNI：JNI是Java本机接口（Java Native Interface），是一个本机编程接口，它是Java软件开发工具箱（Java Software Development Kit，SDK）的一部分。JNI允许Java代码使用以其他语言编写的代码和代码库。Invocation API（JNI的一部分）可以用来将Java虚拟机（JVM）嵌入到本机应用程序中，从而允许程序员从本机代码内部调用Java代码。不过，对Java外部的调用通常不能移植到其他平台，在applet中还可能引发安全异常。实现本地代码将使您的Java应用程序无法通过100%纯Java测试。但是，如果必须执行本地调用，则要考虑几个准则：
1、将您的所有本地方法都封装到一个类中，这个类调用单个的DLL。对每一种目标操作系统平台，都可以用特定于适当平台的版本的DLL。这样可以将本地代码的影响减少到最小，并有助于将以后所需要的移植问题考虑在内。
2、本地方法尽量简单。尽量使您的本地方法对第三方（包括Microsoft）运行时DLL的依赖减少到最小。使您的本地方法尽量独立，以将加载您的DLL和应用程序所需的开销减少到最小。如果需要运行时DLL，必须随应用程序一起提供.
2.生成一个JNI 的libjni.so库
我这个例子主要是在有android源码的编译环境下测试的，我有Android 7.1的开发编译环境，如果大家做Android 系统开发或者应用开发，有一个Android的开发环境非常有必要，或者需要自己下载Android NDK。
2.1.在源码位置下新建一个源码路径
![代码结构如图](https://img-blog.csdn.net/20181016153603736?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
HelloWorld.java源码如下
package com.wqf.hellojni;
public class HelloWorld{
    
    public static native String helloWorld();
    public static native int add(int a,int b);
    
}2.2 用命令编译生成头文件
1、用命令javac com/wqf/hellojni/HelloWorld.java，生成HelloWorld.class
2、用命令javah -classpath . -jni com.wqf.hellojni.HelloWorld生成头文件
![编译后如图](https://img-blog.csdn.net/20181016154338266?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
注意：
网上很多说明用javah -jni com.wqf.hellojni.HelloWorld 生成头文件，我试过很多次，都是失败，还是用我上面那个命令，可以看看javah的命令帮助
![在这里插入图片描述](https://img-blog.csdn.net/20181018092049923?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2.3 编译生成libjni.so库步骤
2.3.1 在jni目录里面建立几个文件如下
1、把上面生成的com_wqf_hellojni_HelloWorld.h放到这个目录来
2、新建一个jni.c文件用来实现上面头文件里面的方法
3、新建一个Android.mk文件用来编译
Android.mk的内容如下
\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= jni.c
LOCAL_MODULE := libjni 
include $(BUILD_SHARED_LIBRARY)2.3.2 jni.c的内容如下
//自己创建一个c文件，实现自己定义的native方法，也就是.h文件中的方法
//引入自己生成的.h头文件
\#include "com_wqf_hellojni_HelloWorld.h"
//返回一个字符串
JNIEXPORT jstring JNICALL Java_com_wqf_hellojni_HelloWorld_helloWorld(JNIEnv *env, jclass jobj) {
      return (*env)->NewStringUTF(env,"HelloWorld 我是用jni调用出来的字符串");
}
//返回 a+b的结果
JNIEXPORT jint JNICALL Java_com_wqf_hellojni_HelloWorld_add(JNIEnv *env, jclass jobj, jint a, jint b){
     return a+b;
}2.3.3 编译
使用mm -B命令编译生成so文件编译结果如下图
![编译结果生成的so在out/target/product/rk3399_mid/obj/libjin.so](https://img-blog.csdn.net/20181016171403395?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
说个小细节，[我们编译生成的libjni.so](http://xn--libjni-hz8iu02kha5989bihdf07ar01b.so)，我们要会看他的几个参数
1、文件类型，使用file命令查看是32位的库还是64位的库
![在这里插入图片描述](https://img-blog.csdn.net/20181018085737469?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2、使用md5sum命令查看md5码，这个可以查看文件的唯一md5码，以后遇到一个文件名字一样的可以用这个方式知道是不是同一个文件
![在这里插入图片描述](https://img-blog.csdn.net/20181018085933875?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3. 写个简单的apk测试一下
我们的APK编写用的是android studio来编写的，现在android studio是非常主流的android开发工具，如果大家想自己做一个小的DEMO的话，建议装这个工具，网上安装流程非常多了，下载SDK后，最好不要放在C盘，要不然重装系统后又没有了。
3.1先来一个整体的运行效果图片
![在这里插入图片描述](https://img-blog.csdn.net/20181018084250488?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
我们在图片上可以看到add = 9,这个是我们在JNI里面的输出结果
![在这里插入图片描述](https://img-blog.csdn.net/20181018084334637?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3.2 APK编译步骤和几个注意的细节
我是在Android 7.1上编写APK进行测试的，相比其他Android版本，Android 7.1的问题更多，我说下操作步骤和细节。
1、我们先编写一个可以运行的APK
我们需要编写一个helloworld的简单apk，这个apk不需要引用库文件，直接输出一个helloworld。
![在这里插入图片描述](https://img-blog.csdn.net/20181018095057461?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2、然后我们引用我们的库文件
这里注意的是HelloWorld.java 里面的内容
加载库的代码
package com.wqf.hellojni;
public class HelloWorld{
    static {
        //名字必须和libjni.so 名字对应起来
        System.loadLibrary("jni");
    }
    public static native String helloWorld();
    public static native int add(int a,int b);
}在MainActivity.java里面导入包并调用add函数
![在这里插入图片描述](https://img-blog.csdn.net/20181018100350225?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
4. 源码连接
源码我放在github上面，方面查看，有源码的例程大家研究起来更加得心应手
[https://github.com/weiqifa0/androidjni](https://github.com/weiqifa0/androidjni)
参考：
[https://blog.csdn.net/a_zhon/article/details/53097512\#](https://blog.csdn.net/a_zhon/article/details/53097512#)
5.鸣谢
觉得不错的可以关注我的微信公众号【嵌入式Linux】关注后可以获取二维码加入微信技术群

