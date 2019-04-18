# 在安卓JNI/NDK中使用C++11 - wishchinYang的专栏 - CSDN博客
2015年10月12日 10:55:13[wishchin](https://me.csdn.net/wishchin)阅读数：3121
在VS下编写的程序移植到Eclipse下，出现了问题：
 this file requires compiler and library support for iso C++ 2011 standard。
原因：是Eclipse使用的CDT默认未开启C++11 的支持。
解决方法：
      Add "-std=c++11" to your `CFLAGS` in your `Android.mk`:
```
LOCAL_CFLAGS += -std=c++11
```
编译通过.....
**注意事项**：
      把 语句添加在Android.mk的倒数第二行，不能添加在靠前位置，否则不产生任何作用。
