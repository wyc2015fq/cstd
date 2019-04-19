# 如何调查Windows Embedded CE上的Exception“凶手”(1) - xqhrs232的专栏 - CSDN博客
2012年07月24日 15:06:41[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1236标签：[exception																[windows																[2010](https://so.csdn.net/so/search/s.do?q=2010&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=exception&t=blog)
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://www.cnblogs.com/wangkewei/archive/2010/06/04/1751780.html](http://www.cnblogs.com/wangkewei/archive/2010/06/04/1751780.html)
## 傻了吧，又犯事了
不管是在Windows Desktop、Windows Embedded CE，还是Windows Mobile(Phone)上，不管你是开发人员、测试人员还是用户，相信对应用程序的Crash是相对熟悉了——那画面太美了。
![2](http://images.cnblogs.com/cnblogs_com/wangkewei/WindowsLiveWriter/WindowsEmbeddedCEException_FA8C/2_thumb.png)
如果你有Log输出窗口的话，跟这个画面一起你还会看到：
[?](http://www.cnblogs.com/wangkewei/archive/2010/06/04/1751780.html#)
```
```xml
654322
 PID:c8a002a TID:c8b002a Exception 'Raised Exception' (-1): Thread-Id=0c8b002a(pth=8207bbcc),
```
```xml
```
```xml
Proc-Id=0c8a002a(pprc=9903e6f0)
 'testos_5.exe', VM-active=0c8a002a(pprc=9903e6f0) 'testos_5.exe'
```
```xml
654322
 PID:c8a002a TID:c8b002a PC=40042c58(coredll.dll+0x00022c58) RA=88046608(kernel.dll+0x00008608)
```
```xml
```
```xml
SP=0002f770,
 BVA=ffffffff
```
```
以及：
[?](http://www.cnblogs.com/wangkewei/archive/2010/06/04/1751780.html#)
```
```xml
656644
 PID:400002 TID:2e80002   DwXfer!TransferDumpFile: Dump file transfered to local file system,
```
```xml
```
```xml
Size=0x000419B0,
 Name=\Windows\System\DumpFiles\Ce053110-01\Ce053110-01.kdmp
```
```
现在要捉拿凶手，可是我是“傻瓜警探”(菜鸟程序员)啊，我不懂高级技巧啊，咋办啊？ 操作系统技术已经有很久的积淀，事后分析(Postmortem Debug)机制已经非常完善。
在微软的大家庭里有关的工具更傻瓜化，而且Windows Embedded上面的一套机制与Windows Desktop很多是通用的。我们拿着工具配置好参数几行命令一输问题差不多就可能知道出来哪了。如果你只想知道How to问题，那么只需要关注“手把手教你断案”部分带颜色的字，当然这篇文章我尝试把问题扩展并深入一下，我知道很多不愿意挖掘内部机制的朋友对此是不感兴趣的。如果你跟我一样是比较痴迷深一点机制的，那么欢迎加我QQ：3423
 67 776。
### 关于Android的题外话
做Android深入一点的开发估计你得汗流浃背了： 
[How
 do I obtain crash-data from my Android application?](http://stackoverflow.com/questions/601503/how-do-i-obtain-crash-data-from-my-android-application)
[Capturing Android
 Exceptions remotely](http://nullwire.com/capturing_android_exceptions_remotely)
![4130521531_8672609469](http://images.cnblogs.com/cnblogs_com/wangkewei/WindowsLiveWriter/WindowsEmbeddedCEException_FA8C/4130521531_8672609469_thumb.jpg)![gfb2](http://images.cnblogs.com/cnblogs_com/wangkewei/WindowsLiveWriter/WindowsEmbeddedCEException_FA8C/gfb2_thumb.png)
不过应该很快能够完善起来，我们看到Android 2.2就有新的Error Reports机制： 
[Android Application Error Reports](http://android-developers.blogspot.com/2010/05/google-feedback-for-android.html)
![gfb](http://images.cnblogs.com/cnblogs_com/wangkewei/WindowsLiveWriter/WindowsEmbeddedCEException_FA8C/gfb_thumb.png)
![gfb3](http://images.cnblogs.com/cnblogs_com/wangkewei/WindowsLiveWriter/WindowsEmbeddedCEException_FA8C/gfb3_thumb.png)
## 手把手教你断案
1.不管通过上面提到的Error Report机制通过网络获得，还是直接如下图所示从本地直接取得，我们最后要得到Dump文件(案发现场)。
![1](http://images.cnblogs.com/cnblogs_com/wangkewei/WindowsLiveWriter/WindowsEmbeddedCEException_FA8C/1_thumb.png)
PS：Windows下(Desktop/Embedded)的Dump文件一般有3种，
- 
Complete memory dump
- 
Kernel memory dump
- 
Small memory dump
更多有关Desktop的请见：[Overview of memory dump
 file options for Windows Vista, Windows Server 2008 R2, Windows Server 2008, Windows Server 2003, Windows XP, and Windows 2000](http://support.microsoft.com/kb/254649)
在CE上我们根据实际的情况选择对应的Dump File Type (比如通过网络上传到服务器的Dump File就不能太大，而压力测试时就应该生成Complete Dumps)：
![Capture](http://images.cnblogs.com/cnblogs_com/wangkewei/WindowsLiveWriter/WindowsEmbeddedCEException_FA8C/Capture_thumb.png)
2.Dump文件分析工具还有很多，这里就下载并安装Debugging Tools for Windows(Windbg)：[http://www.microsoft.com/whdc/devtools/debugging/installx86.mspx#b](http://www.microsoft.com/whdc/devtools/debugging/installx86.mspx#b)
3.在Windbg的File选择中设置好Symblo Search Path、Excutable Image Search Path以及Source Search Path。
PS：PDB代表[Program Database](http://en.wikipedia.org/wiki/Program_database)，PDB文件由微软开发出来用于存储调试Windows
 Native Code的符号。
如下图，把模块(EXE、DLL)对应的PDB文件的路径设置好。
![4](http://images.cnblogs.com/cnblogs_com/wangkewei/WindowsLiveWriter/WindowsEmbeddedCEException_FA8C/4_thumb.png)
在Source Search Path中设置好所有可能相关的源码，比如直接告诉它“E:\WINCE600”。
4.Open Crash Dump。
![Capture](http://images.cnblogs.com/cnblogs_com/wangkewei/WindowsLiveWriter/WindowsEmbeddedCEException_FA8C/Capture_thumb_1.png)
5.走到这一步时如果符号和代码比较全的话我们发现Windbg已经帮我们找到抛出异常的代码了：
![windbg](http://images.cnblogs.com/cnblogs_com/wangkewei/WindowsLiveWriter/WindowsEmbeddedCEException_FA8C/windbg_thumb_1.png)
此时我们输入命令：
[?](http://www.cnblogs.com/wangkewei/archive/2010/06/04/1751780.html#)
```
```cpp
!anlyze
 –v
```
```
![3](http://images.cnblogs.com/cnblogs_com/wangkewei/WindowsLiveWriter/WindowsEmbeddedCEException_FA8C/3_thumb_1.png)
从上面我们看到STATUS_INVALID_PARAMETER，顾名思义“无效参数”，很多朋友到这里时因为没有更多可用的信息还是找不到Root Cause，到底哪里传入的参数是无效的，也就不了了之了。
在有Callstack等信息，甚至源码的情况下，这个问题就好办了，我们可用正推/逆推相结合找到问题代码(另外我们不要忘记了Platform Builder工具以及它的一些扩展插件，比如CeDebugX，非常之强大。有机会以后另起文章详细介绍。)，下面的Deep Dig就是我正推的过程，借此也偷窥一下CE里CRT的异常机制。
其实我在应用程序代码里这么干的：
[?](http://www.cnblogs.com/wangkewei/archive/2010/06/04/1751780.html#)
```
```cpp
fseek
```
```cpp
(0,
 0, 0);
```
```
很坏吧？其实我们的软件里面暗藏了很多“杀手”，只是更隐蔽。
STATUS_INVALID_PARAMETER的值其实为0xC000000D，Error_Code指示的就是错误的ID。
更多关于STATUS_xxx请见WINCE600\PUBLIC\COMMON\SDK\INC\ntstatus.h(没错，它就是复用Windows NT的，跟NT的是一个鸟样。而且你能看到这个文件的日期是1989年。)
## Deep Dig
从fseek函数入手。
WINCE600\private\winceos\coreos\core\corelibc\crtw32\stdio\fseek.c L99
[?](http://www.cnblogs.com/wangkewei/archive/2010/06/04/1751780.html#)
```
```cpp
int
```
```cpp
__cdecl
```
```cpp
fseek
```
```cpp
(
```
```cpp
```
```cpp
FILEX
 *stream,
```
```cpp
```
```cpp
long
```
```cpp
offset,
```
```cpp
```
```cpp
int
```
```cpp
whence
```
```cpp
```
```cpp
)
```
```cpp
{
```
```cpp
```
```cpp
int
```
```cpp
retval;
```
```cpp
```
```cpp
_VALIDATE_RETURN((stream
 != NULL), EINVAL, -1);
```
```cpp
//参数检验
```
```cpp
```
```cpp
_VALIDATE_RETURN(((whence
 == SEEK_SET) ||
```
```cpp
```
```cpp
(whence
 == SEEK_CUR) ||
```
```cpp
```
```cpp
(whence
 == SEEK_END)), EINVAL, -1);
```
```
这些烦人的宏就略过了，最后在WINCE600\private\winceos\coreos\core\corelibc\crtw32\misc\invarg.c L72的以下函数被调用：
[?](http://www.cnblogs.com/wangkewei/archive/2010/06/04/1751780.html#)
```
```cpp
_CRTIMP
```
```cpp
void
```
```cpp
__cdecl
 _invalid_parameter(
```
```cpp
```
```cpp
const
```
```cpp
wchar_t
```
```cpp
*pszExpression,
```
```cpp
```
```cpp
const
```
```cpp
wchar_t
```
```cpp
*pszFunction,
```
```cpp
```
```cpp
const
```
```cpp
wchar_t
```
```cpp
*pszFile,
```
```cpp
```
```cpp
unsigned
```
```cpp
int
```
```cpp
nLine,
```
```cpp
```
```cpp
uintptr_t
```
```cpp
pReserved
```
```cpp
```
```cpp
)
```
```cpp
{
```
```cpp
```
```cpp
_invalid_parameter_handler
 pHandler = __pInvalidArgHandler;
```
```cpp
```
```cpp
pszExpression;
```
```cpp
```
```cpp
pszFunction;
```
```cpp
```
```cpp
pszFile;
```
```cpp
```
```cpp
pHandler
 = (_invalid_parameter_handler) _decode_pointer(pHandler);
```
```cpp
```
```cpp
if
```
```cpp
(pHandler
 != NULL)
```
```cpp
```
```cpp
{
```
```cpp
```
```cpp
pHandler(pszExpression,
 pszFunction, pszFile, nLine, pReserved);
```
```cpp
```
```cpp
return
```
```cpp
;
```
```cpp
```
```cpp
}
```
```cpp
```
```cpp
__crt_unrecoverable_error(pszExpression,
 pszFunction, pszFile, nLine, pReserved);
```
```cpp
//执行这里
```
```cpp
}
```
```
最后就是你上面在Windbg中看到的抛出异常的函数(在WINCE600\PRIVATE\WINCEOS\COREOS\CORE\DLL\crtsupp.cpp L43)：
[View
 Code](http://www.cnblogs.com/wangkewei/archive/2010/06/04/1751780.html#)
如果你对_CRT_CAPTURE_CONTEXT的实现感兴趣(在WINCE600\private\winceos\COREOS\core\inc\corecrt.h)，它其实是通过造成一个访问违例(简称AV)异常来实现的：
[View
 Code](http://www.cnblogs.com/wangkewei/archive/2010/06/04/1751780.html#)
## 为什么？为什么？
1.Windows为什么要“蓝屏”(Unix的panic，Linux的opps)，应用程序为什么要“崩溃”？~~为什么我就不可以再长帅一点？~~
“这活我没法干下去了，或者干下去也没有意义了，只能歇菜了”，对第一个问题操作系统是这么说的。“蓝屏”大多是驱动程序干的，特别内核态的驱动。
对第二个问题我想这么比喻会比较好：“我们需要秩序来保证整个系统有条不紊的运行着，就跟社会需要法律一样，没有准则每个人的利益都很容易被损害，甚至整个社会一团糟。”这就是为什么应用程序Crash是这么影响用户体验的事，但是我们还是需要让它Crash的原因。
2.pdb文件干嘛用的？怎样工作的？Windbg等工具是如何工作的？
3.Dump文件到底是什么样的？
[Dump File Format](http://msdn.microsoft.com/en-us/library/ms939593.aspx)
4.哪些常见问题造成应用程序Crash？
如果你感兴趣，可以好好捣鼓捣鼓你的应用程序看看到底有多少种让它Crash的“阴招”，最常见的原因是内存问题，而内存问题中常见的有(但并不是全部都会造成Crash，比如我们释放一个内存之后再去访问一般不会出问题，但是OS为了保证系统的稳定性可能会不允许)：
- 
Stack Overflow
- 
Heap Corruption
- 
Double Free
- 
Buffer Overflow
- 
Use After Free
## 更多参考
[记一次Windows
 Embedded CE应用程序崩溃的摸索](http://www.cnblogs.com/wangkewei/archive/2010/05/17/1737608.html)
CeDebugX：[http://msdn.microsoft.com/en-us/library/ee480021.aspx](http://msdn.microsoft.com/en-us/library/ee480021.aspx)
[Finding
 Windows CE bugs with help from "Dr. Watson"](http://www.windowsfordevices.com/c/a/Windows-For-Devices-Articles/Finding-Windows-CE-bugs-with-help-from-Dr-Watson/)
[Windows CE
 Error Reporting](http://msdn.microsoft.com/en-us/library/ms939656(v=MSDN.10).aspx)
[Mark's Windows troubleshooting](http://blogs.technet.com/markrussinovich/)
《Windows Internals》 
Windows Error Reporting：[http://en.wikipedia.org/wiki/Windows_Error_Reporting](http://en.wikipedia.org/wiki/Windows_Error_Reporting)
Dr.Waston：[http://msdn.microsoft.com/en-us/library/ff542753(v=VS.85).aspx](http://msdn.microsoft.com/en-us/library/ff542753(v=VS.85).aspx)
[使用*Crash* 工具分析Linux
 dump 文件](http://www.ibm.com/developerworks/cn/linux/l-cn-dumpanalyse/index.html?ca=drs-cn-0329)
[redhat crash whitepaper](http://people.redhat.com/crash_whitepaper)
作者: 王克伟
出处: [http://wangkewei.cnblogs.com/](http://wangkewei.cnblogs.com/)
版权声明: 本文的版权归作者与博客园共有。转载时须注明本文的详细链接，否则作者将保留追究其法律责任的权利。
您可以从[这里](http://www.cnblogs.com/wangkewei/archive/2009/10/15/1583745.html)更方便的找到我的文章。
