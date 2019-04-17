# VS2013 编译程序时报无法加载PDB文件错误解决方案 - zj360202的专栏 - CSDN博客





2014年08月19日 10:37:01[zj360202](https://me.csdn.net/zj360202)阅读数：12852









vs2013运行c++出现：无法查找或打开 PDB 文件。




```java
“ConsoleApplication1.exe”(Win32): 已加载“C:\Users\hp\Documents\Visual Studio 2013\Projects\ConsoleApplication1\Debug\ConsoleApplication1.exe”。已加载符号。
“ConsoleApplication1.exe”(Win32): 已加载“C:\Windows\SysWOW64\ntdll.dll”。无法查找或打开 PDB 文件。
“ConsoleApplication1.exe”(Win32): 已加载“C:\Windows\SysWOW64\kernel32.dll”。无法查找或打开 PDB 文件。
“ConsoleApplication1.exe”(Win32): 已加载“C:\Windows\SysWOW64\KernelBase.dll”。无法查找或打开 PDB 文件。
“ConsoleApplication1.exe”(Win32): 已加载“C:\Windows\SysWOW64\msvcp110d.dll”。已加载符号。
“ConsoleApplication1.exe”(Win32): 已加载“C:\Windows\SysWOW64\msvcr110d.dll”。已加载符号。
程序“[8124] ConsoleApplication1.exe”已退出，返回值为 0 (0x0)。
```





1、点 调试

2、然后 选项和设置

![](https://img-blog.csdn.net/20130909220431515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWloYWl5aXJlbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

3、左边点 符号

4、把微软符号服务器勾

![](https://img-blog.csdn.net/20140819102728727?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


5、运行的时候等一下 会从微软下载符号！！

6、问题解决



如下显示：



```java
“ConsoleApplication1.exe”(Win32): 已加载“D:\VS2013Job\Projects\ConsoleApplication1\Debug\ConsoleApplication1.exe”。已加载符号。
“ConsoleApplication1.exe”(Win32): 已加载“C:\Windows\SysWOW64\ntdll.dll”。已加载符号。
“ConsoleApplication1.exe”(Win32): 已加载“C:\Windows\SysWOW64\kernel32.dll”。已加载符号。
“ConsoleApplication1.exe”(Win32): 已加载“C:\Windows\SysWOW64\KernelBase.dll”。已加载符号。
“ConsoleApplication1.exe”(Win32): 已加载“C:\Windows\SysWOW64\apphelp.dll”。已加载符号。
SHIMVIEW: ShimInfo(Complete)
“ConsoleApplication1.exe”(Win32): 已加载“C:\Windows\SysWOW64\msvcp110d.dll”。已加载符号。
“ConsoleApplication1.exe”(Win32): 已加载“C:\Windows\SysWOW64\msvcr110d.dll”。已加载符号。
程序“[25512] ConsoleApplication1.exe”已退出，返回值为 0 (0x0)。
```


```java
--------------------------------------------------------------------------------------------------------------
```

无法启动此程序 因为计算机中丢失opencv_core249d.dll-我的解决方案





看下PATH中是否配置对了

%OPENCV%\x86\vc12\bin


VC一定要是vc12，

```java
vc10    vs2010

vc11    vs2012

vc12    vs2013
```









