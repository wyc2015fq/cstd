# 当JNI遇到多线程－－java对象如何被C++中的多个线程访问? - xqhrs232的专栏 - CSDN博客
2012年01月11日 11:32:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1204
原文地址::[http://blog.csdn.net/hust_liuX/article/details/1460486](http://blog.csdn.net/hust_liuX/article/details/1460486)
 CSDN相关帖子::signal 11 (SIGSEGV), fault addr 0000015c是什么错误？怎么解决？
[http://topic.csdn.net/u/20110421/14/96405313-eab8-4069-93c6-a0bbdd8223db.html?seed=1167112573&r=77265101#r_77265101](http://topic.csdn.net/u/20110421/14/96405313-eab8-4069-93c6-a0bbdd8223db.html?seed=1167112573&r=77265101#r_77265101)
java中要访问C++代码时, 使用JNI是唯一选择. 然而,在多线程的情况下, 可能出现以下问题:
问题描述:
一个java对象通过JNI调用DLL中一个send()函数向服务器发送消息,不等服务器消息到来就立即返回.同时
把JNI接口的指针JNIEnv *env,和jobject obj保存在DLL中的变量里.
一段时间后,DLL中的消息接收线程接收到服务器发来的消息,
并试图通过保存过的env和obj来调用先前的java对象的方法来处理此消息.
然而,JNI文档上说,JNI接口的指针JNIEnv*不能在c++的线程间共享,
在我的程序中,如果接收线程试图调用java对象的方法,程序会突然退出.
不知道有没有方法突破JNI接口的指针不能在多个c++线程中共享的限制?
解决办法:
在 [http://java.sun.com/docs/books/jni/html/pitfalls.html#29161](http://java.sun.com/docs/books/jni/html/pitfalls.html#29161) 提到,
JNI接口指针不可为多个线程共用,但是java虚拟机的JavaVM指针是整个jvm公用的. 于是,在DLL中可以调用:
static JavaVM* gs_jvm;
env->GetJavaVM(&gs_jvm); 来获取JavaVM指针.获取了这个指针后,在DLL中的另一个线程里,可以调用:
JNIEnv *env;
gs_jvm->AttachCurrentThread((void **)&env, NULL);
来将DLL中的线程 "attached to the virtual machine"(不知如何翻译...),同时获得了这个线程在jvm中的  JNIEnv指针.
    由于我需要做的是在DLL中的一个线程里改变某个java对象的值,所以,还必须获取那个java对象的jobject指针.同 JNIEnv 指针一样,jobject指针也不能在多个线程中共享. 就是说,不能直接在保存一个线程中的jobject指针到全局变量中,然后在另外一个线程中使用它.幸运的是,可以用
gs_object=env->NewGlobalRef(obj);
来将传入的obj保存到gs_object中,从而其他线程可以使用这个gs_object来操纵那个java对象了.
示例代码如下:
(1)java代码:Test.java:
import java.io.*;
class Test implements Runnable
{
 public int value  = 0;
 private Thread tx=null;
 public Test()
 {
  tx=new Thread(this,"tx");
 }
 static 
 {
  System.loadLibrary("Test");
 }
 public native void setEnev();
 public static void main(String args[])
 {
  Test t = new Test();
  t.setEnev(); 
  System.out.println("ok in java main");
  t.tx.start();
  try
  {
   Thread.sleep(10000000);
  }catch(Exception e)
  {
   System.out.println("error in main");
  }
 }
 public void run()
 {
  try
  {
    while(true)
    { 
      Thread.sleep(1000);
      System.out.println(value);
    }
  }catch(Exception e)
  {
    System.out.println("error in run");
  } 
 }
}
(2) DLL代码:Test.cpp:
#include "test.h"
#include<windows.h>
#include<stdio.h>
static JavaVM *gs_jvm=NULL;
static jobject gs_object=NULL;
static int gs_i=10;
void WINAPI ThreadFun(PVOID argv)
{ 
 JNIEnv *env;
 gs_jvm->AttachCurrentThread((void **)&env, NULL);
 jclass cls = env->GetObjectClass(gs_object);
 jfieldID fieldPtr = env->GetFieldID(cls,"value","I");
 while(1)
 {
 Sleep(100);
 //在DLL中改变外面的java对象的value变量的值.
 env->SetIntField(gs_object,fieldPtr,(jint)gs_i++);
 }
}
JNIEXPORT void JNICALL Java_Test_setEnev(JNIEnv *env, jobject obj)
{
 printf("come into test.dll/n");
 //Returns “0” on success; returns a negative value on failure. 
 int retGvm=env->GetJavaVM(&gs_jvm);
 //直接保存obj到DLL中的全局变量是不行的,应该调用以下函数:
 gs_object=env->NewGlobalRef(obj);
 HANDLE ht=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadFun,0,NULL,NULL);
 printf("the Handle ht is:%d/n",ht);
}
//====================================================================
备注:
 1>JNI接口的指针JNIEnv*不能在c++的线程间共享
2>同 JNIEnv 指针一样,jobject指针也不能在多个线程中共享
3>每个native 函数传的jobject obj类其实就是相当于那个类的this指针
  //获取数组   
   mid = (*env)->GetMethodID(env,cls,"getCanvasBmpByte","()[B");   //这里的最后一个参数是用javap -s -p 命令拿到的，很多文章有讲过
   msg = (jbyteArray)(*env)->CallObjectMethod(env,job, mid); //注意这里的第二个参数就是对象，意义就是你调用的是当前类的当前对象的这个方法，如果当前类没有这个函数那就直接出错了，如果你要在其他类里面调用这个方法，请把这个类的对象的引用做参数传递进来：
Java_com_example_hellojni_HelloJni_InitGraphics( JNIEnv* env,jobject thiz ,jobject job )//这个就是函数所在的对象
