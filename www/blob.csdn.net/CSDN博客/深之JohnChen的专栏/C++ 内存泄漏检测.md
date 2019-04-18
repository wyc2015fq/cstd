# C++ 内存泄漏检测 - 深之JohnChen的专栏 - CSDN博客

2011年04月27日 14:54:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：4102


C++ 内存泄漏检测

检测内存泄漏的方法多种多样，有使用内存泄漏检测工具（比如BoundsChecker）检测内存泄漏；有直接看代码检测代码逻辑，看那些地方是否没有释放内存。一般地静态内存泄漏通过工具与代码检查很容易找到泄漏点；动态的内存泄漏很难查，一般通过在代码中加断点跟踪和Run-Time内存检测工具来查找。

总的来说，要检查内存泄漏分几个步骤：

1、首先写代码时要控制内存的释放，比如new之后要delete，看析构函数是否真的执行（很多人编写释放内存的代码在析构函数中处理的），如果没有真正执行，就需要动态释放对象；前段时间在一个项目中使用了单例模式对象，将构造函数和析构函数设置成保护类型，在运行代码时退出时不执行到析构函数里面（具体也不知道什么原因），最后只有手动删除对象。

2、其次让程序长时间运行，看任务管理器对应程序内存是不是一直向上增加；

3、最后使用常用内存泄漏检测工具来检测内存泄漏点。

文本主要描述一些内存泄漏检测工具功能介绍与简单使用方法。

一、对于VS2005/VS2008编译器自带的内存检测工具/函数。
在 main() 函数开头加上：
#include"crtdbg.h"
_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);

二、用BoundsChecker之类的工具。

BoundsChecker 是一个Run-Time错误检测工具，它主要定位程序在运行时期发生的各种错误。BoundsChecker能检测的错误包括：

1、指针操作和内存、资源泄露错误，比如：

内存泄露；

资源泄露；

对指针变量的错误操作。

2、内存操作方面的错误，比如：

内存读、写溢出；

使用未初始化的内存。

3、API函数使用错误

具体详细信息参考：[http://www.cnitblog.com/qiuyangzh/archive/2005/07/14/975.html](http://www.cnitblog.com/qiuyangzh/archive/2005/07/14/975.html)

三、linux下可以用valgrind检测内存泄露错误。

具体详细信息参考：[http://www.sudu.cn/info/html/edu/20070101/291096.html](http://www.sudu.cn/info/html/edu/20070101/291096.html)

四、purify工具，这个是专门检测内存的，包括泄露、越界、指针跑飞等都可以检查，在VC上使用方便。

五、用Windbg，试过查句柄泄漏的，比较方便。

六、Visual Leak Detector

Visual Leak Detector是一款用于Visual C++的免费的内存泄露检测工具。可以在http://www.codeproject.com/tools/visualleakdetector.asp 下载到。相比较其它的内存泄露检测工具，它在检测到内存泄漏的同时，还具有如下特点：

1、可以得到内存泄漏点的调用堆栈，如果可以的话，还可以得到其所在文件及行号；

2、可以得到泄露内存的完整数据；

3、可以设置内存泄露报告的级别；

4、它是一个已经打包的lib，使用时无须编译它的源代码。而对于使用者自己的代码，也只需要做很小的改动；

5、他的源代码使用GNU许可发布，并有详尽的文档及注释。对于想深入了解堆内存管理的读者，是一个不错的选择。

具体详细信息参考：[http://www.codeproject.com/KB/applications/visualleakdetector.aspx](http://www.codeproject.com/KB/applications/visualleakdetector.aspx)

