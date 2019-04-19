# Android NDK的C/C++代码中利用JNI回调实现字符编码转换的试验（中文UTF8与GBK） - xqhrs232的专栏 - CSDN博客
2011年06月23日 13:30:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2964
原文地址::[http://blog.csdn.net/huzgd/archive/2011/02/14/6183109.aspx](http://blog.csdn.net/huzgd/archive/2011/02/14/6183109.aspx)
CSDN相关网贴
1。Android ndk开发，cygwin环境iconv.h头文件问题
[http://topic.csdn.net/u/20101022/16/1b2e0cec-b9d2-42ea-8d9c-4f1bb8320a54.html?r=70149216](http://topic.csdn.net/u/20101022/16/1b2e0cec-b9d2-42ea-8d9c-4f1bb8320a54.html?r=70149216)
2。200分 求android中jni中文转码方法
[http://topic.csdn.net/u/20110418/15/1230e8b7-0b2b-4100-b44e-4eba6173fc79.html](http://topic.csdn.net/u/20110418/15/1230e8b7-0b2b-4100-b44e-4eba6173fc79.html)
3。android中使用mbstowcs报错und efined reference to `mbstowcs'
[http://topic.csdn.net/u/20110415/17/f5b72204-e94c-47bb-a6e1-b5a79e05a415.html](http://topic.csdn.net/u/20110415/17/f5b72204-e94c-47bb-a6e1-b5a79e05a415.html)
在NDK下做网络传输时，遇到一个编码转换的问题，就是对方传过来的文件名是中文GBK编码，需要转成UTF8来处理。
平常在C/C++环境下编程时，系统都会提供字符编码转换的API。如Windows下有MultiByteToWideChar之类的函数，Linux下有iconv库，纯C下用wcstombs、mbstowcs也可以做。然而我在我机上的Android NDK目录下，并没有找到iconv库，直接写iconv函数无法编译；而mbstowcs需要的本地库似乎也没有，即使能编译也会运行失败。这一来，似乎就没有办法，只能自己手工写码表库转换了。
后来还是想到一个办法：由于Android下NDK是由Java JNI调用的，而Java本身的字符编码转换功能是现成的，直接new一个String，传入字节和编码，就可以获取指定另一个编码的字符了。
通过网上查资料试验，终于编译运行通过，问题解决。Java这边调JNI的代码就不贴了，C程序大概内容及说明如下：
//首先是包含头文件，并定义全局变量
#include <string.h>
#include <jni.h>
#include <pthread.h>
//初始JNI虚拟机环境和线程
JavaVM*   gJavaVM;
JNIEnv*   gJniEnv;
pthread_t gJvmThread;
//Java字符串的类和获取字节的方法ID
jclass    gStringClass;
jmethodID gmidStringInit;
jmethodID gmidStringGetBytes;
//初始化JNI环境，此函数由JNI调用
jstring Java_com_huz_test_CharsetTest_InitJNIEnv(JNIEnv* env, jobject obj)
{
    (*env)->GetJavaVM(env, &gJavaVM);
    gJniEnv=env;
    gJvmThread=pthread_self();//记住当前JNI环境的线程
    //获取Java String类和回调方法ID信息，由于每次转换都需要，因此用全局变量记下来，免得浪费时间重复执行
    gStringClass= (*env)->FindClass(env,"java/lang/String");
    gmidStringGetBytes= (*env)->GetMethodID(env,gStringClass, "getBytes", "(Ljava/lang/String;)[B");
    gmidStringInit= (*env)->GetMethodID(env,gStringClass, "<init>", "([BLjava/lang/String;)V");
    ...
    return (*env)->NewStringUTF(env, "OK");
}
//由Java String转为指定编码的char
int  jstringToPchar(JNIEnv* env, jstring jstr, const char * encoding, char* outbuf, int outlen)
{
    char* rtn = NULL;
    jstring jencoding;
    if (encoding==HNULL)
        jencoding= (*env)->NewStringUTF(env,"utf-8");
    else
        jencoding=(*env)->NewStringUTF(env,encoding);
    jbyteArray barr= (jbyteArray)(*env)->CallObjectMethod(env,jstr, gmidStringGetBytes, jencoding);
    jsize alen = (*env)->GetArrayLength(env,barr);
    jbyte* ba = (*env)->GetByteArrayElements(env,barr, JNI_FALSE);
    if (alen > 0)
    {
        if(outlen==0)
            return alen;
        if(outlen<=alen)
            return -1;
        rtn=outbuf;
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    (*env)->ReleaseByteArrayElements(env,barr, ba, 0);
    return alen;
}
//由指定编码以零结束的char转为Java String
jstring pcharToJstring(JNIEnv* env, const char* pat, const char* encoding)
{
    jstring jencoding;
    jbyteArray bytes = (*env)->NewByteArray(env,strlen(pat));
    (*env)->SetByteArrayRegion(env,bytes, 0, strlen(pat), (jbyte*)pat);
    if (encoding==HNULL)
        jencoding= (*env)->NewStringUTF(env,"utf-8");
    else
        jencoding=(*env)->NewStringUTF(env,encoding);
    return (jstring)(*env)->NewObject(env,gStringClass, gmidStringInit, bytes, jencoding);
}
//在C代码中执行字符串编码转换
//参数分别为：原始字符，原始编码，目标字符缓冲区，目标编码，目标缓冲区大小，返加转换结果的长度
int changeCharset(char * src_buf, char * src_encoding, char * dst_buf, char * dst_encoding, int dst_size)
{
    JNIEnv *env;
    jstring jtemp;
    int res;
    //由于初始化只执行了一次，本函数与初始JNI调用可能不在同一线程，因此需要判断当前线程
    if(gJvmThread==pthread_self())
    {
        //如果是同一个线程，直接转
        env=gJniEnv;
        jtemp=pcharToJstring(env, src_buf, src_encoding);
        res=jstringToPchar(env, jtemp,dst_encoding, dst_buf, dst_size);
    }
    else
    {
        //如果不是同一个线程，先Attach再转
        env=gJniEnv;
        (*gJavaVM)->AttachCurrentThread(gJavaVM,&env,NULL);
        jtemp=pcharToJstring(env, src_buf, src_encoding);
        res=jstringToPchar(env, jtemp,dst_encoding, dst_buf, dst_size);
        (*gJavaVM)->DetachCurrentThread(gJavaVM);
    }
    return res;
}
虽然JNI回调能解决此问题，但用起来很麻烦，速度上估计也很慢，适合少量的数据处理。估计以后Android的NDK新版应该会解决此问题。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/huzgd/archive/2011/02/14/6183109.aspx](http://blog.csdn.net/huzgd/archive/2011/02/14/6183109.aspx)
