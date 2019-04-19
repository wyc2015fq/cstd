# JNI WARNING: illegal class name 'Ljava/lang/String;' (FindClass) - 三少GG - CSDN博客
2012年08月02日 13:51:54[三少GG](https://me.csdn.net/scut1135)阅读数：5779
jclass objClass = env->FindClass("java/lang/String");//定义数组中元素类型
而不是FindClass("Ljava/lang/String;");
吭爹呀！！！又是一天的功夫～～～
/*****************推荐博客*关于JNI************/
# [duicky的cnblogs](http://www.cnblogs.com/luxiaofeng54/)
[NDK(12)](http://www.cnblogs.com/luxiaofeng54/category/315742.html)[Android(42)](http://www.cnblogs.com/luxiaofeng54/category/276200.html)
/***************************************************/
[http://www.cnblogs.com/luxiaofeng54/archive/2011/08/17/2142000.html](http://www.cnblogs.com/luxiaofeng54/archive/2011/08/17/2142000.html)
## [基于 Android NDK 的学习之旅----- C调用Java(附源码)](http://www.cnblogs.com/luxiaofeng54/archive/2011/08/17/2142000.html)
# 基于 Android NDK 的学习之旅----- C调用Java
许多成熟的C引擎要移植到Android 平台上使用 , 一般都会 提供 一些接口, 让Android sdk 和 jdk 实现。
下文将会介绍 C 如何 通过 JNI 层调用 Java 的静态和非静态方法。
### 1、主要流程
1、 新建一个测试类TestProvider.java
a)        该类提供了2个方法
b)       一个静态的方法,一个非静态的方法
2、 JNI中新建Provider.c
a)        该文件中需要把Java中的类TestProvider映射到C中
b)       把TestProvider的两个方法映射到C中
c)        新建TestProvider 对象
d)       调用两个方法
3、 Android 上层 调用 JNI层
4、 JNI层调用C层
5、 C 层调用 Java 方法
### 2、设计实现
1、界面设计如下：
![](http://pic002.cnblogs.com/images/2011/154043/2011081708464353.png)
老样子,很搓,不过实用,嘿嘿
代码不在这贴出了,有需要的兄弟直接到文章结束部分下载。
2、      关键代码说明
**C中定义映射的类、方法、对象**
jclassTestProvider;
jobjectmTestProvider;
jmethodIDgetTime;
jmethodIDsayHello;
**C 中映射 类**
       TestProvider= (*jniEnv)->FindClass(jniEnv,"com/duicky/TestProvider");
**C中新建对象 **
       jmethodIDconstruction_id = (*jniEnv)->GetMethodID(jniEnv,TestProvider,"<init>", "()V");
TestProvider mTestProvider= (*jniEnv)->NewObject(jniEnv, TestProvider,construction_id);
**C 中映射方法**
       静态：
getTime = (*jniEnv)->GetStaticMethodID(jniEnv,TestProvider, "getTime","()Ljava/lang/String;");
       非静态：
sayHello = (*jniEnv)->GetMethodID(jniEnv, TestProvider,"sayHello","(Ljava/lang/String;)V");
**C 中调用 Java的 方法**
       静态：
(*jniEnv)->CallStaticObjectMethod(jniEnv,TestProvider, getTime);
       非静态：
(*jniEnv)->CallVoidMethod(jniEnv,mTestProvider, sayHello,jstrMSG);
注意 GetXXXMethodID  和CallXXXMethod 。
第一个XXX 表示的是映射方法的类型，如： 静态跟非静态
第二个 XXX 表示 调用方法的返回值 ，如：Void,Object,等等。（调用静态方法的时候Call后面要加Static）
详细 映射方法和 调用方法 请参考 [JNI 文档](http://files.cnblogs.com/luxiaofeng54/JNI_Docs.rar) ，这个很重要 ！
3、      Java 上层 关键代码
TestProvider.Java的两个方法
[?](http://www.cnblogs.com/luxiaofeng54/archive/2011/08/17/2142000.html#)
`package``com.duicky;`
`/**`
` ``* `
` ``* `
` ``* @author luxiaofeng <454162034@qq.com>`
` ``*`
` ``*/`
`public``class``TestProvider {`
`    ``public``static``String getTime() {`
`        ``LogUtils.printWithSystemOut( ``"Call From C Java Static Method"``);`
`        ``LogUtils.toastMessage(MainActivity.mContext, ``"Call From C Java Static Method"``);`
`        ``return``String.valueOf(System.currentTimeMillis());`
`    ``}`
`    ``public``void``sayHello(String msg) {`
`        ``LogUtils.printWithSystemOut(``"Call From C Java Not Static Method ："``+ msg);`
`        ``LogUtils.toastMessage(MainActivity.mContext, ``"Call From C Java Not Static Method ："``+ msg);`
`    ``}`
`}`
4、      Android.mk 文件 关键代码
[?](http://www.cnblogs.com/luxiaofeng54/archive/2011/08/17/2142000.html#)
`LOCAL_PATH := $(call my-dir)`
`include $(CLEAR_VARS)`
`LOCAL_C_INCLUDES := $(LOCAL_PATH)/include`
`LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog`
`LOCAL_MODULE    := NDK_04`
`LOCAL_SRC_FILES := \`
`CToJava.c \`
`Provider.c`
`include $(BUILD_SHARED_LIBRARY)`
老样子,不说了,你懂的。如果不懂,嘎嘎,那就请点击[Android.mk文件](http://www.cnblogs.com/luxiaofeng54/archive/2011/08/13/2137577.html) 简介
5、      JNI文件夹下文件
**Provider.h**
[?](http://www.cnblogs.com/luxiaofeng54/archive/2011/08/17/2142000.html#)
`#include <string.h>`
`#include <jni.h>`
`void``GetTime() ;`
`void``SayHello();`
**Provider.c  **
[?](http://www.cnblogs.com/luxiaofeng54/archive/2011/08/17/2142000.html#)
`#include "Provider.h"`
`#include <android/log.h>`
`extern``JNIEnv* jniEnv;`
`jclass TestProvider;`
`jobject mTestProvider;`
`jmethodID getTime;`
`jmethodID sayHello;`
`int``GetProviderInstance(jclass obj_class);`
`/**`
` ``* 初始化 类、对象、方法`
` ``*/`
`int``InitProvider() {`
`    ``__android_log_print(ANDROID_LOG_INFO, ``"JNIMsg"`
```
,
```
`"InitProvider Begin  1"``);`
`    ``if``(jniEnv == NULL) {`
`        ``return``0;`
`    ``}`
`    ``if``(TestProvider == NULL) {`
`        ``TestProvider = (*jniEnv)->FindClass(jniEnv,``"com/duicky/TestProvider"``);`
`        ``if``(TestProvider == NULL){`
`            ``return``-1;`
`        ``}`
`        ``__android_log_print(ANDROID_LOG_INFO, ``"JNIMsg"`
```
,
```
`"InitProvider Begin  2 ok"``);`
`    ``}`
`    ``if``(mTestProvider == NULL) {`
`        ``if``(GetProviderInstance(TestProvider) != 1) {`
`            ``(*jniEnv)->DeleteLocalRef(jniEnv, TestProvider);`
`            ``return``-1;`
`        ``}`
`        ``__android_log_print(ANDROID_LOG_INFO, ``"JNIMsg"`
```
,
```
`"InitProvider Begin  3 ok"``);`
`    ``}`
`    ``if``(getTime == NULL) {`
`        `
```
getTime = (*jniEnv)->GetStaticMethodID(jniEnv, TestProvider,
```
`"getTime"``,``"()Ljava/lang/String;"``);`
`        ``if``(getTime == NULL) {`
`            ``(*jniEnv)->DeleteLocalRef(jniEnv, TestProvider);`
`            ``(*jniEnv)->DeleteLocalRef(jniEnv, mTestProvider);`
`            ``return``-2;`
`        ``}`
`        ``__android_log_print(ANDROID_LOG_INFO, ``"JNIMsg"`
```
,
```
`"InitProvider Begin  4 ok"``);`
`    ``}`
`    ``if``(sayHello == NULL) {`
`        `
```
sayHello = (*jniEnv)->GetMethodID(jniEnv, TestProvider,
```
`"sayHello"``,``"(Ljava/lang/String;)V"``);`
`        ``if``(sayHello == NULL) {`
`            ``(*jniEnv)->DeleteLocalRef(jniEnv, TestProvider);`
`            ``(*jniEnv)->DeleteLocalRef(jniEnv, mTestProvider);`
`            ``(*jniEnv)->DeleteLocalRef(jniEnv, getTime);`
`            ``return``-3;`
`        ``}`
`        ``__android_log_print(ANDROID_LOG_INFO, ``"JNIMsg"`
```
,
```
`"InitProvider Begin  5 ok"``);`
`    ``}`
`    ``__android_log_print(ANDROID_LOG_INFO, ``"JNIMsg"`
```
,
```
`"InitProvider Begin  6"``);`
`    ``return``1;`
`}`
`int``GetProviderInstance(jclass obj_class) {`
`    ``if``(obj_class == NULL) {`
`        ``return``0;`
`    ``}`
`    ``jmethodID construction_id = (*jniEnv)->GetMethodID(jniEnv, obj_class,`
`            ``"<init>"``, ``"()V"``);`
`    ``if``(construction_id == 0) {`
`        ``return``-1;`
`    ``}`
`    ``mTestProvider = (*jniEnv)->NewObject(jniEnv, obj_class,`
`            ``construction_id);`
`    ``if``(mTestProvider == NULL) {`
`        ``return``-2;`
`    ``}`
`    ``return``1;`
`}`
`/**`
` ``* 获取时间 ---- 调用 Java 方法`
` ``*/`
`void``GetTime() {`
`    ``if``(TestProvider == NULL || getTime == NULL) {`
`        ``int``result = InitProvider();`
`        ``if``(result != 1) {`
`            ``return``;`
`        ``}`
`    ``}`
`    ``jstring jstr = NULL;`
`    ``char``* cstr = NULL;`
`    ``__android_log_print(ANDROID_LOG_INFO, ``"JNIMsg"`
```
,
```
`"GetTime Begin"``);`
`    ``jstr = (*jniEnv)->CallStaticObjectMethod(jniEnv, TestProvider, getTime);`
`    ``cstr = (``char``*) (*jniEnv)->GetStringUTFChars(jniEnv,jstr, 0);`
`    ``__android_log_print(ANDROID_LOG_INFO, ``"JNIMsg"`
```
,
```
`"Success Get Time from Java , Value = %s"``,cstr );`
`    ``__android_log_print(ANDROID_LOG_INFO, ``"JNIMsg"`
```
,
```
`"GetTime End"``);`
`    ``(*jniEnv)->ReleaseStringUTFChars(jniEnv, jstr, cstr);`
`    ``(*jniEnv)->DeleteLocalRef(jniEnv, jstr);`
`}`
`/**`
` ``* SayHello ---- 调用 Java 方法`
` ``*/`
`void``SayHello() {`
`    ``if``(TestProvider == NULL || mTestProvider == NULL || sayHello == NULL) {`
`        ``int``result = InitProvider() ;`
`        ``if``(result != 1) {`
`            ``return``;`
`        ``}`
`    ``}`
`    ``jstring jstrMSG = NULL;`
`    ``jstrMSG =(*jniEnv)->NewStringUTF(jniEnv, ``"Hi,I'm From C"``);`
`    ``__android_log_print(ANDROID_LOG_INFO, ``"JNIMsg"`
```
,
```
`"SayHello Begin"``);`
`    ``(*jniEnv)->CallVoidMethod(jniEnv, mTestProvider, sayHello,jstrMSG);`
`    ``__android_log_print(ANDROID_LOG_INFO, ``"JNIMsg"`
```
,
```
`"SayHello End"``);`
`    ``(*jniEnv)->DeleteLocalRef(jniEnv, jstrMSG);`
`}`
**CToJava.c**
[?](http://www.cnblogs.com/luxiaofeng54/archive/2011/08/17/2142000.html#)
`#include <string.h>`
`#include <android/log.h>`
`#include <jni.h>`
`#include "Provider.h"`
`JNIEnv* jniEnv;`
`/**`
` ``*  Java 中 声明的native getTime 方法的实现`
` ``*/`
`void``Java_com_duicky_MainActivity_getTime(JNIEnv* env, jobject thiz)`
`{`
`    ``if``(jniEnv == NULL) {`
`        ``jniEnv = env;`
`    ``}`
`    ``GetTime();`
`}`
`/**`
` ``*  Java 中 声明的native sayHello 方法的实现`
` ``*/`
`void``Java_com_duicky_MainActivity_sayHello(JNIEnv* env, jobject thiz)`
`{`
`    ``if``(jniEnv == NULL) {`
`        ``jniEnv = env;`
`    ``}`
`    ``SayHello();`
`}`
### 3、运行效果
1、点击“C调用java静态方法”按钮
C成功调用了Java中的getTime 方法,通过C方法打印出上层调用得到的时间,并且上层成功吐司出调用信息出来。
![](http://pic002.cnblogs.com/images/2011/154043/2011081708495276.png)
![](http://pic002.cnblogs.com/images/2011/154043/2011081708500249.png)
![](http://pic002.cnblogs.com/images/2011/154043/2011081708501054.png)
2、点击 “C调用java非静态方法”按钮
C成功调用了sayHello 方法, 并成功接收到 C 传递的参数，和 吐司出相对应的信息
![](http://pic002.cnblogs.com/images/2011/154043/2011081708502022.png)
![](http://pic002.cnblogs.com/images/2011/154043/2011081708503657.png)
### 4、C调用Java注意点
       a)C 映射java 方法时 对应的签名
getTime = (*jniEnv)->GetStaticMethodID(jniEnv,TestProvider, "getTime","()Ljava/lang/String;");
       故事情节还没发展这么快,下一章才会专门介绍下这个签名的使用
       b)映射方法的时候需要区别静态和非静态GetStaticMethodID,GetMethodID
c)调用的时候也需要区分CallStaticObjectMethod,CallVoidMethod
而且还需要区分返回值类型
有不理解的兄弟请留言，个人技术有限，有讲错的地方请大牛们指出,讲的不够全面的请多多包涵，谢谢,
点击下载源码[C调用Java例子](http://files.cnblogs.com/luxiaofeng54/C%E8%B0%83%E7%94%A8Java%E4%BE%8B%E5%AD%90.rar)
本文出自 [duicky](http://www.cnblogs.com/luxiaofeng54/) 博客 ， 转载请注明出处 [http://www.cnblogs.com/luxiaofeng54/archive/2011/08/17/2142000.html](http://www.cnblogs.com/luxiaofeng54/archive/2011/08/17/2142000.html)

分类: [Android](http://www.cnblogs.com/luxiaofeng54/category/276200.html), [NDK](http://www.cnblogs.com/luxiaofeng54/category/315742.html)
