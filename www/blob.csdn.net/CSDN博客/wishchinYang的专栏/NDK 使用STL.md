# NDK 使用STL - wishchinYang的专栏 - CSDN博客
2015年07月22日 15:22:35[wishchin](https://me.csdn.net/wishchin)阅读数：612
参考链接：[Android中使用STL](http://zhidao.baidu.com/link?url=UOzzq2rucDcfaaHjzcNKU3saoNsRR_WPJBC45F6MZ2mCBkfo3J3U4OWA28udjwTySsExlPMLy0MRetHyR5JSntOYe_2IsRNPXqefsQw6Eja)
1. 在 Application.mk 中添加代码“APP_STL:= gnustl_static”后，
文件中的内容如下：
APP_ABI := x86
**APP_STL:= gnustl_static**
编译后发现ndk-build可以编译通过了；
但是却发现 eclipse 上面却报出错误。
~~2. 在 Android.mk 中增加 “LOCAL_LDLIBS:= -landroid”，~~
~~后文件的内容如下：~~
~~LOCAL_PATH := $(call my-dir)~~
~~include $(CLEAR_VARS)~~
~~LOCAL_MODULE := hello-jni~~
~~LOCAL_SRC_FILES := hello-jni.c  HelloJni.cpp~~
~~**LOCAL_LDLIBS:= -landroid**~~
~~include $(BUILD_SHARED_LIBRARY)~~
~~Clean Object，编译可通过！~~
3. 出现Log print 未定义的问题，把上一句的 
~~**LOCAL_LDLIBS:= -landroid**~~
    注销掉
4. 在 工程——属性——C++——PathandSystem里面，添加
NDK root ——source/c**-stl/gnu-libstdc++/4.6/libs/include 等几个文件夹
