
# 如何调试MFC中的内存泄漏[转帖] - .NET博文收藏 - CSDN博客


2008年06月10日 12:47:00[hejishan](https://me.csdn.net/hejishan)阅读数：647


首先，应该是MFC报告我们发现内存泄漏。注意：要多运行几次，以确定输出的内容不变，特别是{}之间的数值，不能变，否则下面的方法就不好用了。![LeakOut.JPG](http://www.cnitblog.com/images/cnitblog_com/wangk/MFCLeak/LeakOut.JPG)
我们来看看：F:/CodeSample/Test/TestPipe/LeakTest/MainFrm.cpp(54) : {86} normal block at0x00422E80,10byteslong.
Data:<>1F 1F 1F 1F 1F CD CD CD CD CD

F:/CodeSample/Test/TestPipe/LeakTest/MainFrm.cpp(54) 告诉我们MFC认为是在该文件的54行，发生了内存泄漏。你双击改行就可以转到该文件的54行了。但是有时候这一信息并不能用来准确判断，比如：MFC可能报告Strcore.cpp文件的某行，实际上这是CString的实现函数，此时并不知道什么时候发生了内存泄漏。
此时我们需要更多的信息。那么我们看看紧接其后的：
{86} normal block at0x00422E80,10byteslong.
Data:<>1F 1F 1F 1F 1F CD CD CD CD CD

它告诉我们：在第86次分配的内存没有释放，一共有10字节，内容移16进制方式打印给我们看。
有了这些信息，我们可以开始调试内存泄漏了。
按下F10在程序的刚开始处，停下来，打开Watch窗口：
![LeakWatchMenu.JPG](http://www.cnitblog.com/images/cnitblog_com/wangk/MFCLeak/LeakWatchMenu.JPG)
在Watch窗口中输入：
{,,msvcrtd.dll}_crtBreakAlloc
![watch1.JPG](http://www.cnitblog.com/images/cnitblog_com/wangk/MFCLeak/watch1.JPG)
然后更改值为上文提到的分配次数：86
![watch2.JPG](http://www.cnitblog.com/images/cnitblog_com/wangk/MFCLeak/watch2.JPG)
接着按下F5继续，然后在第86次分配的时候会发生中断：
![MessageBox.JPG](http://www.cnitblog.com/images/cnitblog_com/wangk/MFCLeak/MessageBox.JPG)
然后我们打开堆栈窗口：
![LeakStackMenu.JPG](http://www.cnitblog.com/images/cnitblog_com/wangk/MFCLeak/LeakStackMenu.JPG)![ClickStack.JPG](http://www.cnitblog.com/images/cnitblog_com/wangk/MFCLeak/ClickStack.JPG)
往回查看最近我们自己的代码，双击堆栈我们自己的函数那一层，上图有绿色三角的那一层。就定位到泄漏时分配的内存了。
![FoundLeak.JPG](http://www.cnitblog.com/images/cnitblog_com/wangk/MFCLeak/FoundLeak.JPG)
之后，就是看你的编码功底了。



