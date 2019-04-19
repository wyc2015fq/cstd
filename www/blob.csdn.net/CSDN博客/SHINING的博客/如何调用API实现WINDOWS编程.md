# 如何调用API实现WINDOWS编程 - SHINING的博客 - CSDN博客
2009年12月28日 22:03:00[snsn1984](https://me.csdn.net/snsn1984)阅读数：6572

我们目前经常说的调用API编程中的API，全称是Application Programming Interface . Windows给我们提供的API可以分为两种，16位的API和32位的API，经常写为Win16 API和Win32 API。主要是针对CPU的位数才有的16位和32位，现在的系统基本上全是32位的了。所以现在主要是Win32 API。
API其实是系统留给我们调用的函数接口，当然系统本身也调用这些函数去做一些事情。有些我们需要操作系统去做的事情，可以直接通过调用API函数来实现，而不用自己去实现。
在Windows下面编程经常会需要调用Win32 API来实现一些功能，特别是涉及到底层的一些操作。调用Win32 API的方法主要有以下几种：
1、使用汇编语言调用Win32 API。该方法比较复杂，难以理解，应用不是太广。
2、使用C直接调用。这里的C语言跟标准C语言是有区别的，加入了一些跟操作系统有关的一些限制，可以直接调用系统的API实现自己功能。
3、通过MFC调用。MFC为Microsoft Foundation Class Library的缩写，即为微软基本类库。该类库封装了大部分的Win32 API函数，使其表现为类库中类的成员函数，这样用户就可以通过使用Visual C++来调用MFC中的类的成员函数，也就是Win32 API了。MFC在Visual Studio6.0中就存在，后来微软推出.NET平台后，MFC依然被包含在平台内。最新的.NET平台 VS2010.NET测试版中，MFC依然存在。
所以如果想编写Windows程序，都要调用Windows的API，如果想使用C语言，可以直接调用，如果想使用C++可以通过使用MFC来间接的调用系统API。当然很多编程语言也可以实现对WINDOWS系统的API调用，并能实现WINDOWS系统的编程，这里只是对C/C++的使用进行了简单的介绍。
