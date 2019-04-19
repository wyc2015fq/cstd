# 如何在Android平台下编译带STL的C++程序 - xqhrs232的专栏 - CSDN博客
2011年05月27日 11:16:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1344
原文地址::[http://somenlp.info/?p=314](http://somenlp.info/?p=314)
Android操作系统由linux的内核和java的UI构成。但在已往的Android系统上，一般只能用java程序进行开发。若要使用C/C++，通常是将C/C++程序做成lib再由java来调用。理论上说，android上是可以直接用C/C++开发的，因为底层的系统就是linux内核。但以前发布的android NDK，最多是可以用C和最基本的C++进行开发，不能使用STL。所以也就有了Android NDK 的Crystax版本。
     所幸的是，最新发布的Android NDK R5版本已支持C++的STL开发了。使用NDK  R5编译带STL的C++程序过程比较简单，简要过程说明如下：
1、下载最新的Android SDK，下载Android NDK R5版本。
2、如是在windows平台下需要下载安装Cygwin，用来模拟linux的命令行环境。
3、打开Cygwin，进入到Android NDK R5的路径。声明NDK变量：
    export NDK=/path to android NDK R5
4、声明gcc和g++的路径。
5、为要编译的C++程序建一个文件夹，如myproject。在myproject下再建一个jini文件夹，将源代码放在这个文件夹下，myproject/jini。
mkdir myproject
mkdir myproject/jini
6、在jini文件夹下建两个分别名为：android.mk 和 application.mk文件。android.mk类以于C++程序的makefile，application.mk则指明当前程序依赖的库。
android.mk的示例为：
LOCAL_PATH := $(call my-dir) 
include $(CLEAR_VARS) 
LOCAL_MODULE := my_first_app #指明C++程序编译出的可执行程序的名称
LOCAL_SRC_FILES := my_first_app0.cpp / #指明要编译的源文件，可以有很多个
my_first_app1.cpp /
…
include $(BUILD_EXECUTABLE)#表明编译的是可执行程序
/**************************************************************************/
application.mk的示例为：（在application.mk中指明STL库）
APP_STL := stlport_static
这里选STL库时有三个选项：
system -> Use the default minimal C++ runtime library. 
stlport_static -> Use STLport built as a static library.#一般使用这个 
stlport_shared -> Use STLport built as a shared library.
7、在myproject目录用android的NDK build工具编译C++程序：
cd myproject
$NDK/ndk-build
如果程序没错的话，会编译出android的可执行程序，位置在myproject/libs/armeabi/my_first_app
8、将编译出来的my_first_app放到手机或是模拟器上运行。到Android SDK的tools目录下找到adb.exe，在windows的cmd上运行adb.exe。
用adb.exe将my_first_app程序push到手机或模拟器的/data/local目录上：
adb.exe push myproject/libs/armeabi/my_first_app  /data/local。
9、通过adb，在手机上运行my_frist_app：
在cmd上运行:
adb.exe shell
由此进入到手机的linux终端上，接下来再更改my_first_app的权限使其可以运行：
cd /data/shell
chmod 777 my_first_app
./my_first_app //如果没错的话，这一步即可运行my_first_app
至此在android上编译含STL的C++程序的过程结束。没想象中的复杂，不过也没那么简单。
