# Android NDK编程实现终端功能（调用system函数） - xqhrs232的专栏 - CSDN博客
2012年01月07日 17:41:30[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3638
原文地址::[http://leo108.com/pid-597.asp](http://leo108.com/pid-597.asp)
本程序能够实现在android中利用system函数执行命令，并将执行结果输出到指定的文件中。 
核心代码： 
hello-jni.c
[复制代码]()
- #include <jni.h> 
- #include <string.h> 
- JNIEXPORT jstring Java_com_example_hellojni_HelloJni_system(JNIEnv* env,jobject thiz, jstring str , jstring path)
- { 
- const jbyte* p1 = (*env)->GetStringUTFChars(env , str, NULL); 
- const jbyte* p2 = (*env)->GetStringUTFChars(env , path, NULL); 
- char* str2=(char *)p1; 
- char* path2=(char *)p2; 
- strcat(str2," > "); 
- strcat(str2,path2); 
- system(str2); 
- return(*env)->NewStringUTF(env,str2); 
- }
HelloJni.java 
[复制代码]()
- package com.example.hellojni; 
- import android.app.Activity; 
- import android.widget.TextView; 
- import android.os.Bundle; 
- 
- public class HelloJni extends Activity 
- { 
- /** Called when the activity is first created. */ 
- @Override 
- public void onCreate(Bundle savedInstanceState) 
- { 
- super.onCreate(savedInstanceState); 
- TextView tv = new TextView(this); 
- tv.setText(system("ls","\"/mnt/sdcard/hello.txt\"") );//这里的第一个参数是要执行的命令，第二个参数是要输出的文件路径
- setContentView(tv); 
- } 
- public native String system(String str ,String path); 
- static { 
- System.loadLibrary("hello-jni"); 
- } 
- }
