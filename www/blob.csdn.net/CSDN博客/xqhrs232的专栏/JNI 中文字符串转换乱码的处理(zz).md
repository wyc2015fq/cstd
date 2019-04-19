# JNI 中文字符串转换乱码的处理(zz) - xqhrs232的专栏 - CSDN博客
2011年06月22日 18:13:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1760
原文地址::[http://hi.baidu.com/fountainblog/blog/item/b327fffee775d4265d60085e.html](http://hi.baidu.com/fountainblog/blog/item/b327fffee775d4265d60085e.html)
这两天在学习使用jni，在java程序中，调用海量词典的dll。利用jni的GetStringChars函数和NewString函数时，遇到了中文乱码的问题，折腾了一个晚上。查阅了一些资料，总结如下：
一.相关概念
java内部是使用16bit的unicode编码（UTF-16）来表示字符串的，无论中文英文都是2字节； jni内部是使用UTF-8编码来表示字符串的，UTF-8是变长编码的unicode，一般ascii字符是1字节，中文是3字节； c/c++使用的是原始数据，ascii就是一个字节了，中文一般是GB2312编码，用两个字节来表示一个汉字。
明确了概念，操作就比较清楚了。下面根据字符流的方向来分别说明一下
1、java --> c/c++
这种情况中，java调用的时候使用的是UTF-16编码的字符串，jvm把这个字符串传给jni，c/c++得到的输入是jstring，这个时候，可以利用jni提供的两种函数，一个是GetStringUTFChars，这个函数将得到一个UTF-8编码的字符串；另一个是 GetStringChars这个将得到UTF-16编码的字符串。无论那个函数，得到的字符串如果含有中文，都需要进一步转化成GB2312的编码。示意图如下：
String 
      (UTF-16)
          |
[java]    |
--------------------  JNI 调用
[cpp]     |
          v
       jstring 
       (UTF-16)
          |  
 +--------+---------+
 |GetStringChars    |GetStringUTFChars
 |                  |
 v                  v
wchar_t*           char*
(UTF_16)           (UTF-8)
2、c/c++ --> java
jni返回给java的字符串，c/c++首先应该负责把这个字符串变成UTF-8或者UTF-16格式，然后通过NewStringUTF或者NewString来把它封装成jstring，返回给java就可以了。
       String 
      (UTF-16)
          ^
          |
[java]    |
--------------------  JNI 返回
[cpp]     |
       jstring 
       (UTF-16)
          ^
          |  
 +--------+---------+
 ^                  ^
 |                  |
 |NewString         |NewStringUTF
wchar_t*          char*
(UTF_16)          (UTF-8)
    如果字符串中不含中文字符，只是标准的ascii码，那么使用GetStringUTFChars/NewStringUTF就可以搞定了，因为这种情况下，UTF-8编码和ascii编码是一致的，不需要转换。
    但是如果字符串中有中文字符，那么在c/c++部分进行编码转换就是一个必须了。我们需要两个转换函数，一个是把UTF8/16的编码转成GB2312；一个是把GB2312转成UTF8/16。
    这里要说明一下：linux和win32都支持wchar，这个事实上就是宽度为16bit的unicode编码UTF16，所以，如果我们的 c/c++程序中完全使用wchar类型，那么理论上是不需要这种转换的。但是实际上，我们不可能完全用wchar来取代char的，所以就目前大多数应用而言，转换仍然是必须的。
二。一种转换方法
使用wide char类型来转换。
char* jstringToWindows( JNIEnv *env, jstring jstr )
{ //UTF8/16转换成gb2312
  int length = (env)->GetStringLength(jstr );
  const jchar* jcstr = (env)->GetStringChars(jstr, 0 );
  char* rtn = (char*)malloc( length*2+1 );
  int size = 0;
  size = WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)jcstr, length, rtn,(length*2+1), NULL, NULL );
  if( size <= 0 )
    return NULL;
  (env)->ReleaseStringChars(jstr, jcstr );
  rtn[size] = 0;
  return rtn;
}
jstring WindowsTojstring( JNIEnv* env, const char* str )
{//gb2312转换成utf8/16
    jstring rtn = 0;
    int slen = strlen(str);
    unsigned short * buffer = 0;
    if( slen == 0 )
        rtn = (env)->NewStringUTF(str );
    else
    {
        int length = MultiByteToWideChar( CP_ACP, 0, (LPCSTR)str, slen, NULL, 0 );
        buffer = (unsigned short *)malloc( length*2 + 1 );
        if( MultiByteToWideChar( CP_ACP, 0, (LPCSTR)str, slen, (LPWSTR)buffer, length ) >0 )
            rtn = (env)->NewString(  (jchar*)buffer, length );
    }
    if( buffer )
        free( buffer );
    return rtn;
}
补充（需要包含的头文件）：
/*包含*/
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <Windows.h>
