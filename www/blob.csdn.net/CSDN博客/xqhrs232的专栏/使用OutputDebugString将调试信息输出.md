# 使用OutputDebugString将调试信息输出 - xqhrs232的专栏 - CSDN博客
2013年03月27日 22:19:21[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：940
原文地址::[http://mazhibin.blog.51cto.com/4842922/894427](http://mazhibin.blog.51cto.com/4842922/894427)
在编写控制台程序的时候我们经常会使用printf输出调试信息，使我们了解程序的状态，方便调试，但是当编写非控制台程序的时候这种方法就行不通了，那我们应该怎么办？上网查了一些方法，大致就如下几种
- 
使用Log机制
- 
用TRACE宏
- 
其他
    首先，使用Log机制的话要先写一个Log系统，麻烦。而关于TRACE宏，查了资料后才发现原来是MFC里的东西，那对于非MFC程序，就用不了了。    后来发现了OutputDebugString这玩意儿，发现不错。他是属于windows API的，所以只要是包含了window.h这个头文件后就可以使用了，很方便。他可以把调试信息输出到编译器的输出窗口，如下：还可以用DbgView这样的工具查看，这样就可以脱离编译器了。    下面说说如何使用OutputDebugString，他的函数原型是:
void WINAPI OutputDebugString( __in_opt  LPCTSTR lpOutputString);
就一个参数，是LPCTSTR 类型的。
    这里我对这个API做了一些包装，当然，参考了一些文章：
- //木杉的博客  http://blog.csdn.net/mazhibinit 
- //2012年6月10日         ------转载请注明出处 
- 
- #ifndef _DEBUGPRINTF_H_
- #define _DEBUGPRINTF_H_
- 
- #include<Windows.h>
- #include <tchar.h>
- 
- //用于输出信息到编译器输出窗口的宏定义
- //使用win API，DEBUG版本会执行，RELEASE版本则不会
- //还可以使用DebugView，WinDbg等工具查看输出
- 
- #ifdef _DEBUG
- 
- #define DP0(fmt) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt));OutputDebugString(sOut);}
- #define DP1(fmt,var) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt),var);OutputDebugString(sOut);}
- #define DP2(fmt,var1,var2) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt),var1,var2);OutputDebugString(sOut);}
- #define DP3(fmt,var1,var2,var3) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt),var1,var2,var3);OutputDebugString(sOut);}
- 
- #endif
- 
- #ifndef _DEBUG
- 
- #define DP0(fmt) ;
- #define DP1(fmt, var) ;
- #define DP2(fmt,var1,var2) ;
- #define DP3(fmt,var1,var2,var3) ;
- 
- #endif
- 
- #endif
其中的DP就是表示BebugPrint。而且这些调试输出只会在BEBUG版本中有效，在Release版本中就不会有效。
这样使用：
- //木杉的博客  http://blog.csdn.net/mazhibinit 
- //2012年6月10日         ------转载请注明出处 
- 
- #include<stdio.h>
- #include"debugPrint.h"
- void main() 
- { 
- for(int i=0;i<10;i++) 
-     { 
-         printf("hello!\n"); 
-         DP0("这是调试信息！\n"); 
-         DP1("这是调试信息%d\n",i); 
-         DP2("这是调试信息%d--%d\n",i,i+1); 
-         DP3("这是调试信息%d--%d--%d\n",i,i+1,i+2); 
-     } 
-     getc(stdin); 
- } 
    不过，输出这些信息对程序还是有拖慢作用的，像我在写游戏是使用了这个输出调试信息就使帧率下降了不少，不过对于非游戏程序应该还是没有什么影响的。
//====================================================================
备注::
1、试了一下上面的方法还真的是可行！！！不错，整成一个头文件然后就可以调用对应的函数了！
