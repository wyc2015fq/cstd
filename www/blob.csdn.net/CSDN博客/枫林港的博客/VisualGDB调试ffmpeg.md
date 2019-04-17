# VisualGDB调试ffmpeg - 枫林港的博客 - CSDN博客





2018年07月24日 23:27:17[枫林港](https://me.csdn.net/u013601132)阅读数：74








# VisualGDB调试ffmpeg

VisualGDB是vs的插件，基于gdb的调试器，用的是河蟹版，官网[http://visualgdb.com/](http://visualgdb.com/)，只试用过。还有个vs+gdb的插件：WinGDB，官网[http://www.wingdb.com/wgHome.htm](http://www.wingdb.com/wgHome.htm)，也是收费的。
- **开始调试**

Debug->Quick Debug With GDB，在弹出的对话框中填入相关的参数就可以调试了，这个界面和其他集成了gdb的IDE如codeblock类似。勾选“I would like to enter additional GDB commands (e.g. set breakpoints) before debugging starts”，则在启动被调试程序前会弹出对话框，可以输入GDB命令。 
![这里写图片描述](https://img-blog.csdn.net/20180724232017395?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## SIFTRAP

用VisualGDB调试ffmpeg-3.0的时候，能在main函数断住，按F5后总是报错： 
![这里写图片描述](https://img-blog.csdn.net/20180724232030472?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
bt看其堆栈没有有用的信息，只知道在ntdll.dll出了问题。

> 
“bt\n”

~”#0  0x7c92120f in ntdll!DbgBreakPoint () from C:\WINDOWS\system32\ntdll.dll\n”

~”#1  0x7c98ee31 in ntdll!RtlpNtMakeTemporaryKey () from C:\WINDOWS\system32\ntdll.dll\n”


后来直接在MinGW下用gdb调试：

```bash
gdb.exe --interpreter mi --args "./ffplay_g.exe" -debug pict -x 720 -y 576 -af volume=0.01 "F:\stream\DDPlus.ts"
```

也是一样报错：

> 
&”warning: HEAP[ffplay_g.exe]: \n”

&”warning: Invalid Address specified to RtlFreeHeap( 0BCA0000, 0BCA4358 )\n”

&”\n”

~”\nProgram received signal “

~”SIGTRAP, Trace/breakpoint trap.\n”

~”[Switching to Thread 11564.0x2290]\n”

~”0x7c92120f in ntdll!DbgBreakPoint () from C:\WINDOWS\system32\ntdll.dll\n”

*stopped,frame={addr=”0x7c92120f”,func=”ntdll!DbgBreakPoint”,args=[],from=”C:\WINDOWS\system32\ntdll.dll”},thread-id=

“4”,stopped-threads=”all”

(gdb)


在ffmpeg-2.2、ffmpeg2.8.5下也是一样的问题。但直接运行ffplay_g.exe则没有问题。解决的办法请参考：

[http://mingw.5.n7.nabble.com/Invalid-Address-specified-to-RtlFreeHeap-and-libstdc-problem-td18045.html](http://mingw.5.n7.nabble.com/Invalid-Address-specified-to-RtlFreeHeap-and-libstdc-problem-td18045.html)

[http://stackoverflow.com/questions/1621059/breakpoints-out-of-nowhere-when-debugging-with-gdb-inside-ntdll](http://stackoverflow.com/questions/1621059/breakpoints-out-of-nowhere-when-debugging-with-gdb-inside-ntdll)

这是个可以忽略的错误，VisualGDB下在main函数断住后，执行`handle SIGTRAP noprint`命令就可以忽略，会有警告的打印但gdb不会停下来。





