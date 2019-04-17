# 非MFC工程中使用MFC库 - DoubleLi - 博客园







目录[(?)](http://blog.csdn.net/luoweifu/article/details/41527069#)[[-]](http://blog.csdn.net/luoweifu/article/details/41527069#)
- [需求说明](http://blog.csdn.net/luoweifu/article/details/41527069#t0)
- [常见问题](http://blog.csdn.net/luoweifu/article/details/41527069#t1)
- [问题分析](http://blog.csdn.net/luoweifu/article/details/41527069#t2)
- [参考解决方法](http://blog.csdn.net/luoweifu/article/details/41527069#t3)
- [我的解决方案](http://blog.csdn.net/luoweifu/article/details/41527069#t4)
- [Stdafxh的原理](http://blog.csdn.net/luoweifu/article/details/41527069#t5)







# 需求说明

C++工程的类型有很多，从VS(或VC)可以看到常见的有:Win32 Console Application、MFC Application、Win32 Project等。在创建MFC工程时，通过IDE的向导会自动帮我们创建相应的类文件和包含必需的头文件，但有时候我们需要在非MFC工程中包含MFC的库。至于为什么会有这个需要，为何不在一开始就创建MFC工程呢？可能有两种原因：1.在MFC工程会产生很多向导生成的代码以及资源文件，如基于单文档的工程会有View,Doc等类，很多时候我们并不需要这些东西，只需要一个空工程就可以了。2.使用第三方框架创建的工程，我们很难更改它的工程属性(如用Firebreath开发浏览器插件，通过脚本文件firebreath会自动帮我们生成VS下的工程)。



# 常见问题

在非MFC工程中使用MFC的库就需要包含相应的头文件，经常会遇到下面这个问题：

1. fatal error C1189: #error :  Building MFC application with /MD[d] (CRT dll version) requires MFC shared dll version. Please #define _AFXDLL or do not use /MD[d]

2. fatal error C1189: #error :  WINDOWS.H already included.  MFC apps must not #include <windows.h>





# 问题分析

对于第1个问题，很简单：

选中工程名右键属性(Project)，在Properties\Configuration Properties\General\Use of MFC中选择Use MFC in a Shared DLL 

出现上面第2个问题主要是因为包含头文件的顺序不对。为什么包含WINDOWS.H的时候会有顺序要求，网上有一段传播的非常广泛解释：

 如果在MFC工程中#include   <windows.h>，那么会有以下编译错误（因为afxwin.h文件中包含了afx.h，afx.h文件中包含了afxver_.h，afxver_.h中包含了afxv_w32.h，而afxv_w32.h中包含了windows.h，请看以下分析）：

  compile   error:   
  c:\program   files\microsoft   visual   studio\vc98\mfc\include\afxv_w32.h(14)   :   
  fatal   error   C1189:   #error   :     WINDOWS.H   already   included.     MFC   apps   must   not   #include   <windows.h>         
  如果编译器在编译afxv_w32.h文件之前编译了windows.h文件，编译器会报上面的错误，因为在afxv_w32.h文件中有下面的一句预编译报警：   
  #ifdef   _WINDOWS_   
  #error   WINDOWS.H   already   included.     MFC   apps   must   not   #include   <windows.h>   
  #endif   

  问题在于为什么afxv_w32.h中要有这么一句预编译处理。看了afxv_w32.h和windows.h文件就有点明白了。   
  在afxv_w32.h中有下面的预编译语句：   
  ...   ...   
  #undef   NOLOGERROR   
  #undef   NOPROFILER   
  #undef   NOMEMMGR   
  #undef   NOLFILEIO   
  #undef   NOOPENFILE   
  #undef   NORESOURCE   
  #undef   NOATOM   
  ...   ...   
  在afxv_w32.h中还有一句：   
  #include   "windows.h"   

  而在windows.h文件中有下面的预编译语句：   
  ...   ...   
  #define   NOATOM   
  #define   NOGDI   
  #define   NOGDICAPMASKS   
  #define   NOMETAFILE   
  #define   NOMINMAX   
  #define   NOMSG   
  #define   NOOPENFILE   
  ...   ...   

  注意到在windows.h的开头有防止windows.h被重复编译的预编译开关：   
  #ifndef   _WINDOWS_   
  #define   _WINDOWS_   

  这样问题就明白了，虽然我不知道微软为什么要这么做，但是我知道如果在afxv_w32.h没有那句预编译报警，那么如果在编译afxv_w32.h之前   
  编译了windows.h，那么在windows.h中#define的NOATOM等宏就会被#undef掉，可能会导致相应的错误发生。   

  猜想原因可能如上所述，我的解决方法是，将包含有#include   “windows.h"的头文件放在所有包含的头文件的最后面，这样使得对afxv_w32文件   
  的编译处理发生在先，这样，由于在afxv_w32.h中已经包含了windows.h，那么宏_WINDOWS_将被定义，后继的#include   "windows.h"语句将形同虚设，   
  上面的编译报警也不会发生了。我觉得这种处理要比将所有的#include   "windows.h”语句删掉要好一点。   

  一句话，编译器必须在编译windows.h之前编译afxv_w32.h,因为我不是十分清除什么时候afxv_w32.h会被编译，所以我将可能包含有#include   "windows.h"的头文件放在其他头文件之后#include。



# 参考解决方法

解决这个问题的总体思路是：把＃include <afxwin.h>的包含语句把到最前面。

sunshine1314 的博文《[非](http://blog.csdn.net/sunshine1314/article/details/459809)[MFC工程使用MFC](http://blog.csdn.net/sunshine1314/article/details/459809)[库时的问题及解决办法](http://blog.csdn.net/sunshine1314/article/details/459809)》给出了一序列的解决方案，大家可能参考一下，也许能解决你们的问题。但我当时通过这一系列方法还是没能解决我的问题。



# 我的解决方案

我的问题是：用Firebreath开发浏览器插件，通过fbgen.py和prep2010.cmd脚本帮我们生成了基于VS2010的工程(这个工程中没有stdaf.h)，我们要在这个工程中获得MFC中的HDC以及使用MessageBox，于是就碰到了上面提到的问题。

解决方案：

手动添加stdafx.h和stdafx.cpp文件使用预编译机制，在stdafx.h的最前面包含<afxwin.h>。于是问题就变成了stdafx.h的原理和手动添加stdafx.h文件及相应配置。下面我们以Win32 Console Application工程的TextProject为例，演示一下这过程。

1.在VS2010中创建Win32 Console Application工程的TextProject，创建向导会自动生成的stdafx.h和stdafx.cpp，省去了手动添加的过程。如果你的工程没有这两个文件可以手动创建。

2.stdafx.h和stdafx.cpp这两个文件已创建并添加到工程，下面讲讲相关的配制。

2.1选中工程名，右键属性(Properties)，在Precompiled Header/Precompiled Header中选择Use(/Yu)，Precompiled Header File中填stdafx.h。设置工程编译时使用预编译头文件stdafx.h(在VS中文件名的大小写不敏感，即StdAfx.h和stdafx.h是等价的)。

![](http://img.blog.csdn.net/20141126232430687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvd2VpZnU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



2.2选中stdafx.cpp文件，右键属性(Properties)，在Precompiled Header/Precompiled Header中选择Create(/Yc), Precompiled Header File中填stdafx.h。这样设置的作用是：每次编译stdafx.cpp文件时创建.pch文件(扩展名pch表示预编译头文件 )。

![](http://img.blog.csdn.net/20141126232502076?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvd2VpZnU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



3.在stdafx.h的开发包含<afxwin.h>文件：

#include <afxwin.h>

4.这时在我们的main函数中写入下面这句话，就可以弹出一个消息框：

AfxMessageBox(L"非MFC工程使用MFC库", MB_OK, 0 );

# Stdafx.h的原理

关于stdafx.h的原理请看下一篇文章《[预编译头文件](http://blog.csdn.net/luoweifu/article/details/41527289)[(stdafx.h)](http://blog.csdn.net/luoweifu/article/details/41527289)[的原理](http://blog.csdn.net/luoweifu/article/details/41527289)》。










