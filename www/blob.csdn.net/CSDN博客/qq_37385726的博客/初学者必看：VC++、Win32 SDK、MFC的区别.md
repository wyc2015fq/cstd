# 初学者必看：VC++、Win32 SDK、MFC的区别~ - qq_37385726的博客 - CSDN博客





2017年02月05日 23:53:22[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：205标签：[windows																[编程](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[windows](https://blog.csdn.net/qq_37385726/article/category/6701637)









# 初学者必看：VC++、Win32 SDK、MFC的区别

[949649117](http://wenku.baidu.com/u/949649117?from=wenku)上传于2014-07-12|(14人评价)|2722人阅读|113次下载|暂无简介[|举报文档](http://wenku.baidu.com/link?url=zXKmV9mFv7u8akQmlwEHYeYWURSlDTX05ic4CbcTIFSLFJzFna-LBjuDpN-ql0606HPRHEaCvUx1-byEWXDVRo4FteCsU692nVO1uUquZbu#%23%23)

在手机打开

















这是一个初进行开发人员都可能遇到过的概念不清的问题，自己当年也同样有过误解，做

技术我感觉一定要专，但是，不代表毫不关心相关的知识，至少概念层次上要知道，所以，

这里还是再把这些内容纪录下来，好记性不如烂笔头：





    C/C++

都是一种编程语言，程序员用它来创作（我喜欢用创作这个词，让写代码有一些

艺术气息

...

）各种各样的程序，简单如计算闰年，复杂到一个系统地实现。当然，



编写标

准

C

程序的时候，我们还经常会调用各种库函数来辅助完成某些功能；初学者使用得最多

的

C

库函数就是

printf

了，这些库函数是由你所使用的编译器厂商提供的。在

Windows

平

台下，也有类似的函数可供调用；不同的是，这些函数是由

Windows

操作系统本身提供的。





    Windows

操作系统提供了各种各样的函数，以方便我们开发

Windows

应用程序。这些

函数是

Windows

操作系统提供给应用程序编程的接口（

Application Programming 

Interface

），简称为

API

函数。我们在编写

Windows

程序时所说的

API

函数，就是指系统

提供的函数，所有主要的

Windows

函数都在

Windows.h

头文件中进行了声明。使用

windows API

创建的能在

windows

上运行的程序统称为

windows

程序。







这些

API

随着系统的更新升级，他们也是在不断更新和扩充，但是，其行为基本保持

不变以保证不同平台对应用程序的兼容性，面对越来越多地

API

，我们怎么获取可靠的帮

助呢？最大的帮助就是

MSDN

，你可以把

MSDN

理解为微软向开发人员提供的一套帮助系

统，其中包含大量的开发文档、技术文章和示例代码。

MSDN

包含的信息非常全面，程序

员不但可以利用

MSDN

来辅助开发，还可以利用

MSDN

来进行学习，从而提高自己。对于

初学者来说，学会使用

MSDN

并从中汲取知识，是必须要掌握的技能。







我们还经常听人说

Win32 SDK

开发，那么什么是

SDK

呢。

SDK

的全称是

Software 

Development Kit

，中文译为软件开发包。假如现在我们要开发呼叫中心，在购买语音卡的

同时，厂商就会提供语音卡的

SDK

开发包，以方便我们对语音卡的编程操作。这个开发包

通常都会包含语音卡的

API

函数库、帮助文档、使用手册、辅助工具等资源。也就是说，

SDK

实际上就是开发所需资源的一个集合，再具体点说，你知道

CreatePorcess

这个

API,

那怎么使用，你需要有头文件，当然还需要提供功能的系统

DLL

库的引出库

lib

，这些都在

SDK

中。现在读者应该明白

Win32 SDK

的含义了吧，即

Windows 32

位平台下的软件开发

包，包括了

API

函数、帮助文档、微软提供的一些辅助开发工具。







提示：

API

和

SDK

是一种广泛使用的专业术语，并没有专指某一种特定的

API

和

SDK

，

例如，语音卡

API

、语音卡

SDK

、

Java API

、

Java SDK

等。自己公开的

DLL

函数也可以叫

API

！！！







一般来讲，狭义上的

API

指

 MS

公开的函数。比如

MSDN

中介绍的函数。



广义的

API

可以包括所有的函数，你自己的函数也算，未公开的也是。



指世界上一切函数。都可以

叫

API--pplication Programming Interface 

；

    SDK

也不仅仅指

MS

的开发包，你自己的

程序如果需要让别人作

2

次开发，你就会提供



一些函数接口让别人编程，你提供的材料也























叫

SDK.   





有了语言

(C/C++)

，有了开发资源

(SDK)

、有了帮助文档

(MSDN)

，

ok,

我们可以编写

windows

程序了，等等、工具有没有更好的工具呢，人类之所以进步就在于会使用工具，

所以，为了更加高效的开发程序，一些集成开发环境诞生了，其中，

 Visual   C++

就是一

个支持

C/C++

语言开发的集成开发环境（

IDE

）。记住，

Vc++

不是什么新的编程语言，他

只是

 IDE

，只是一个编程的辅助工具，具体来说，

Visual   C++

包含了源程序编辑器、

C/C++

编译器、

MFC

和

ATL

等，其中，

MFC

和

ATL

可以简单的理解为再次封装的

Windows

的系统接口，原生接口就是

API 

。







其中，

MFC,

微软基础类

(Microsoft Foundation Classes),

实际上是微软提供的

,

用于在

C++

环境下编写应用程序的一个框架和引擎

,

也可以说，

MFC

是

Win API

与

C++

的结合后

的再一次封装。





    OWL

（

borland

公司，其已经逐渐淡处）、

VCL

（

Borland

公司

--

现在已和

Inprise

合并专

--

为

DELPHI

设计的，其是由

OWL

演变的）和

MFC

（

ms

专为

vc++

设计的）是不同公司提

供的三大类库，更确切点说，他们都是应用框架。





    DELPHI

：也是一种集成开发环境，不过他支持的语言是源至于

pascal

的

Object Pascal.

他使用的框架就是

VCL. 



       BCB:

就是

Inprise

公司使用了

Delphi

的

IDE

和

VCL

，再配上

C++

语言推出的开发环境

C++Builder,

很多人很念旧，所以冠以

Borland C++ Builder

之名，简称为

BCB

。





    ATL

是全新的

COM 

开发工具



：

---- ATL 

是

ActiveX Template Library 

的缩写，它是一套

C++

模板库。



使用

ATL 

能



够



快速地开发出高效、简洁的代码，



同时对

COM 

组件的开发

提供最大限度



的代码自动生成以及可视化支



持。从

Microsoft Visual C++ 5.0 

版本开始，

Microsoft 

把

ATL 

集成



到

Visual C++ 

开发环境中。





     SDK + C 

完全可以进行所有的

windows

程序开发，当然，你还可以采用

MFC + C++

，

当然，你要用

SDK + C++ 

也是你的自由，但是

MFC + C

可是

impossible

的事情，因为，

MFC

就是

C++

写的，

C

可不支持类哦

...

















