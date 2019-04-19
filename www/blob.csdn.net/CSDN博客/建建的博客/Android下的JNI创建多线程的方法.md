# Android下的JNI创建多线程的方法 - 建建的博客 - CSDN博客
2017年06月13日 09:03:15[纪建](https://me.csdn.net/u013898698)阅读数：1630
一、概述
      JNI编程和Linux上的C/C++编程还是挺相似的，每次java调用JNI中的函数时都会传入有关JVM的一些参数（如JNIEnv，jobject），每次JNI回调java中的方法时都要通过JVM的有关参数来实现，当在JNI中涉及到多线程的话还是有一些不一样的地方，就是要在子线程函数里使用AttachCurrentThread()和DetachCurrentThread()这两个函数，在这两个函数之间加入回调java方法所需要的代码。
二、要求
     掌握JNI多线程编程的方法。
三、实现
     新建工程MyThread，修改main.xml文件，在里面只有一个Button，如下：
```
1 <?xml version="1.0" encoding="utf-8"?>
 2 <LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
 3     android:layout_width="fill_parent"
 4     android:layout_height="fill_parent"
 5     android:orientation="vertical" >
 6 
 7     <Button 
 8         android:id="@+id/button"
 9         android:layout_width="fill_parent"
10         android:layout_height="wrap_content"
11         android:text="启动JNI线程"
12         />
13 
14 </LinearLayout>
```
修改MyThreadActivity.java文件，实现按钮的监听，在里面调用JNI中的函数来启动JNI中的线程，比较简单，如下：
```
1 package com.nan.thread;
 2 
 3 import android.app.Activity;
 4 import android.os.Bundle;
 5 import android.util.Log;
 6 import android.view.View;
 7 import android.widget.Button;
 8 
 9 public class MyThreadActivity extends Activity 
10 {
11     private Button mButton = null; 
12     
13     /** Called when the activity is first created. */
14     @Override
15     public void onCreate(Bundle savedInstanceState) 
16     {
17         super.onCreate(savedInstanceState);
18         setContentView(R.layout.main);
19         
20         mButton = (Button)this.findViewById(R.id.button);
21         //按钮监听
22         mButton.setOnClickListener(new View.OnClickListener() 
23         {
24             
25             @Override
26             public void onClick(View v) 
27             {
28                 // TODO Auto-generated method stub
29                 //调用JNI中的函数来启动JNI中的线程
30                 mainThread();
31                 
32             }
33         });
34         //初始化JNI环境
35         setJNIEnv();
36     }
37          
38     //由JNI中的线程回调
39     private static void fromJNI(int i)
40     {
41         Log.v("Java------>", ""+i);
42     }
43     
44     //本地方法
45     private native void mainThread();
46     private native void setJNIEnv(); 
47     
48     static
49     {
50         //加载动态库
51         System.loadLibrary("JNIThreads");
52     }
53     
54 }
```
编写JNI_Thread.c文件，在mainThread()函数里启动5个子线程，在子线程函数里回调java中的静态方法fromJNI()来输出当前子线程是第几个被启动的线程。完整的内容如下：
```
1 #include<stdio.h>
 2 #include<stdlib.h>
 3 #include<unistd.h>
 4 #include<pthread.h>
 5 
 6 #include<jni.h>
 7 #include<android/log.h>
 8 
 9 #define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
10 #define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
11 #define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "native-activity", __VA_ARGS__))
12 
13 //线程数
14 #define NUMTHREADS 5
15 
16 //全局变量
17 JavaVM *g_jvm = NULL;
18 jobject g_obj = NULL;
19 
20 
21 void *thread_fun(void* arg)
22 {
23     JNIEnv *env;
24     jclass cls;
25     jmethodID mid;
26 
27     //Attach主线程
28     if((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK)
29     {
30         LOGE("%s: AttachCurrentThread() failed", __FUNCTION__);
31         return NULL;
32     }
33     //找到对应的类
34     cls = (*env)->GetObjectClass(env,g_obj);
35     if(cls == NULL)
36     {
37         LOGE("FindClass() Error.....");
38         goto error; 
39     }
40     //再获得类中的方法
41     mid = (*env)->GetStaticMethodID(env, cls, "fromJNI", "(I)V");
42     if (mid == NULL) 
43     {
44         LOGE("GetMethodID() Error.....");
45         goto error;  
46     }
47     //最后调用java中的静态方法
48         (*env)->CallStaticVoidMethod(env, cls, mid ,(int)arg);
49     
50 
51 error:    
52     //Detach主线程
53     if((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK)
54     {
55         LOGE("%s: DetachCurrentThread() failed", __FUNCTION__);
56     }
57     
58 
59     pthread_exit(0);
60 }
61 
62 //由java调用以创建子线程
63 JNIEXPORT void Java_com_nan_thread_MyThreadActivity_mainThread( JNIEnv* env, jobject obj)
64 {
65     int i;
66     pthread_t pt[NUMTHREADS];
67     
68     for (i = 0; i < NUMTHREADS; i++)
69         //创建子线程
70         pthread_create(&pt[i], NULL, &thread_fun, (void *)i);
71 }
72 
73 
74 //由java调用来建立JNI环境
75 JNIEXPORT void Java_com_nan_thread_MyThreadActivity_setJNIEnv( JNIEnv* env, jobject obj)
76 {
77     //保存全局JVM以便在子线程中使用
78     (*env)->GetJavaVM(env,&g_jvm);
79     //不能直接赋值(g_obj = obj)
80     g_obj = (*env)->NewGlobalRef(env,obj);
81 }
82 
83 
84 //当动态库被加载时这个函数被系统调用
85 JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
86 {
87     JNIEnv* env = NULL;
88     jint result = -1;    
89 
90     //获取JNI版本
91     if ((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_4) != JNI_OK) 
92     {
93         LOGE("GetEnv failed!");
94             return result;
95     }
96 
97     return JNI_VERSION_1_4;
98 }
```
最后，编写Android.mk文件：
```
1 LOCAL_PATH := $(call my-dir)
 2 
 3 include $(CLEAR_VARS)
 4 
 5 LOCAL_MODULE    := JNIThreads
 6 LOCAL_SRC_FILES := JNI_Threads.c
 7 
 8 LOCAL_LDLIBS    := -llog
 9 
10 include $(BUILD_SHARED_LIBRARY)
```
使用ndk-build成功编译出动态库后，运行该程序：
![](http://pic002.cnblogs.com/images/2012/247269/2012031618404535.png)
并点击2下按钮，LogCat输出如下：
![](http://pic002.cnblogs.com/images/2012/247269/2012031618424761.png)
