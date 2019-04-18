# VS2010编译错误：fatal error C1189: #error : This file requires _WIN32_WINNT to be #defined at least to 0x - wishchinYang的专栏 - CSDN博客
2013年11月06日 20:10:56[wishchin](https://me.csdn.net/wishchin)阅读数：1432
编译VTK,MFC时，mfc对版本的要求问题：
解决方法原文链接：[http://www.cnblogs.com/madhenry/archive/2011/06/29/2093678.html](http://www.cnblogs.com/madhenry/archive/2011/06/29/2093678.html)
最近拿到一个别人的工程，是使用VS.net创建的，而我的机器上只有vs2010，于是用自带的转换工具将它转换成vs2010的工程，转换之前我就很担心，怕转换完后会出问题，但是没有办法，我实在是不想再安一个vs.net了。
  转完后果不其然真出了问题，在重新build工程时，报了一大堆错误，其中第一个就是“fatal error C1189: #error : This file requires _WIN32_WINNT to be #defined at least to 0x0403. Value 0x0501 or higher is recommended”，然后看错误的来源，竟然是atlcore.h，这我就无语了，这是mfc自带的文件，出错的可能性基本上为0，于是只好去请教谷大叔，发现很多人都遇到了这个问题，看了几篇博客和帖子后，大概明白了，应该是_WIN32_WINNT这个宏对应定义的系统的版本号，如果太低的话，编译器就会认为代码无法在当前的系统上编译。
  说了原因，下面是修改方法，就是在stdafx.h文件中修改相关的定义，修改完后的效果应该如下
（代码可参见原文）
```
#ifndef
 WINVER // Allow use of features specific to Windows 95 and Windows NT 4 or later.
```
```
#define
 WINVER 0x0501 // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
```
`#endif`
```
#ifndef
 _WIN32_WINNT // Allow use of features specific to Windows NT 4 or later.
```
```
#define
 _WIN32_WINNT 0x0501 // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
```
`#endif`
```
#ifndef
 _WIN32_WINDOWS // Allow use of features specific to Windows 98 or later.
```
```
#define
 _WIN32_WINDOWS 0x0501 // Change this to the appropriate value to target Windows Me or later.
```
`#endif`
```
#ifndef
 _WIN32_IE // Allow use of features specific to IE 4.0 or later.
```
```
#define
 _WIN32_IE 0x0601 // Change this to the appropriate value to target IE 5.0 or later.
```
```
#endif
```
这样就应该没有问题了
参考资料：
（1）[http://topic.csdn.net/u/20100801/22/2a271d13-b3e9-419b-906c-039c8f3a5e0e.html](http://topic.csdn.net/u/20100801/22/2a271d13-b3e9-419b-906c-039c8f3a5e0e.html)
（2）[http://blog.csdn.net/dongliqiang2006/archive/2010/08/13/5810055.aspx](http://blog.csdn.net/dongliqiang2006/archive/2010/08/13/5810055.aspx)
（3）[http://topic.csdn.net/u/20100401/14/c5b852d1-9934-4056-89f5-4a6041fe73de.html](http://topic.csdn.net/u/20100401/14/c5b852d1-9934-4056-89f5-4a6041fe73de.html)
后记：编译解决这个错误后，VTK完全编译成功！mark一下！
