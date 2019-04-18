# 关于windows中一些系统dll的卸载套路 - tifentan的专栏 - CSDN博客

2017年04月05日 19:43:21[露蛇](https://me.csdn.net/tifentan)阅读数：607


今天又遇到坑。

情况是这样。在使用一些系统自带的com对象时，在程序退出时调用ptr->release出错。提示访问被保护的内存。

经过研究发现，在主线程调用CoCreateInstance后加载进来的dll，统一在程序退出时才卸载。

但注意的是如果在非主线程调用CoCreateInstance加载进来的dll，在这个线程退出时，这些dll也跟随卸载（但程序还没退出）。如下：

线程 0x2e8c 已退出，返回值为 0 (0x0)。

“xxxx.exe”(Win32): 已卸载“C:\Windows\SysWOW64\quartz.dll”

“xxxx.exe”(Win32): 已卸载“C:\Windows\SysWOW64\devenum.dll”

“xxxx.exe”(Win32): 已卸载“C:\Windows\SysWOW64\mfperfhelper.dll”

“xxxx.exe”(Win32): 已卸载“C:\Windows\SysWOW64\MFWMAAEC.DLL”

导致在退出程序时主线程去ptr->release出错。因为这些dll已经被卸载了，ptr指向的对象内存是不错在的。

解决方法如下：

1.统一在一个线程使用这些com对象（例如在主线程）。

2.在调用CoCreateInstance的线程退出前，其他线程主动release（其实不release也行，反正整个dll都卸载了，对象也不存在。但保证别在此线程退出后release就好），做好同步。


