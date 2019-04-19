# Android 基于NDK的JNI开发 C调用java和java调用C的进阶教程 - xqhrs232的专栏 - CSDN博客
2012年01月13日 18:39:21[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：757标签：[java																[jni																[c																[android																[module																[虚拟机](https://so.csdn.net/so/search/s.do?q=虚拟机&t=blog)](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/loseleo/article/details/5858862](http://blog.csdn.net/loseleo/article/details/5858862)
首先，这里所要讲的主要是C调用java的，因为这里网上也没有一个详细的资料，其次，我开发android应用没多久，其中java错误敬请谅解！
   关于配置NDK环境问题应该不用再赘述了，这个网上有很多，只要配置好能编译JNI的本地示例程序就可以了。
   （NDK-1.6版本）
  一、 这里讲一下如何自己写JNI程序，网上很多都是讲用javah自动生成，其实并没有太多必要。首先进入android-ndk-1.6_r1/apps 复制hello-jni文件夹到app目录下，更改名字为你想要的，这里就叫 test-jni，然后进入test-jni目录 ，用写字板打开Application.mk
内容改为
APP_PROJECT_PATH := $(call my-dir)/project
APP_MODULES      := test-jni
保存关闭。再进入project/jni目录打开 Android.mk 
LOCAL_MODULE    := hello-jni
LOCAL_SRC_FILES := hello-jni.c
这两个更改为
LOCAL_MODULE    := test-jni
LOCAL_SRC_FILES := test-jni.c
同时把hello-jni.c改为test-jni.c 这样，你可以使用make APP=test-jni -B命令去编译你的jni了。
二、这里讲一下如何手动写jni代码
 接着上面创建出来的jni，编译时可以通过，但无法使用，因为jni函数有一个函数名规范。打开test-jni.c 会看的
Java_com_example_hellojni_HelloJni_stringFromJNI 这个命名是有详细规范的，Java_ 开头 下面是你在java哪个类里面要使用，比如我要在com/example/hellojni下面的testjni类中使用，那就要命名为Java_com_example_hellojni_testjni_stringFromJNI。
然后是参数JNIEnv* env,jobject thiz ,这两个是默认的参数，第一个是java虚拟机，必须有，第二个可以不用，相当于当前类的对象的指针，也就是this。
    里面内容的写法也有很多，无非是 一些标准C的处理了。这里要注意的是java那边的 数组类型在这边对应的都是一个类的指针，比如byte[]，在c这边是JbyteArray，
要获取数组的值要用
int len =(*env)->GetArrayLength(env,msg);   
     jbyte* elems =(*env)-> GetByteArrayElements(env,msg, 0);  
(*env)->ReleaseByteArrayElements(env,msg, elems, 0); 
三、重点：C调用java。
   先讲一下我理解的调用过程，c要先找到这个类，然后要有一个对象，然后在这个类的对象基础上调用这个函数。
因此调用java函数首先要有类，然后函数ID,然后对象
cls = (*env)->FindClass(env,"com/example/hellojni/Ext_Graphics");
  //获取数组   
   mid = (*env)->GetMethodID(env,cls,"getCanvasBmpByte","()[B");   //这里的最后一个参数是用javap -s -p 命令拿到的，很多文章有讲过
   msg = (jbyteArray)(*env)->CallObjectMethod(env,job, mid); //注意这里的第二个参数就是对象，意义就是你调用的是当前类的当前对象的这个方法，如果当前类没有这个函数那就直接出错了，如果你要在其他类里面调用这个方法，请把这个类的对象的引用做参数传递进来：
Java_com_example_hellojni_HelloJni_InitGraphics( JNIEnv* env,jobject thiz ,jobject job )//这个就是函数所在的对象
暂时就只有这些，具体不清楚请回我消息吧~我会解答的
