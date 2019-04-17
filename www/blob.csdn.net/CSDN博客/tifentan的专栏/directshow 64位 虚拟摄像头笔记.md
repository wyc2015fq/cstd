# directshow 64位 虚拟摄像头笔记 - tifentan的专栏 - CSDN博客





2018年02月05日 14:03:10[露蛇](https://me.csdn.net/tifentan)阅读数：422








# directshow 64位 虚拟摄像头笔记

近来基于directshow source filter做了一个虚拟摄像头。32位编译情况下，在64位的obs下无法识别到，32位的obs倒是能识别；64位编译的source filter可以被64位obs识别，相反32位obs识别不了。小小研究一番，记下来。 

先写结论：**64位的directshow框架只能调用64位的filter，32位的directshow框架只能调用32位的filter**。

## 一个非常好用的directshow工具

一般使用graphedt.exe来调试directshow代码，实际上还有一个开源的工具跟这个类似，但功能多的多，叫[graphstudionext.exe](https://github.com/cplussharp/graph-studio-next)

这是有人在github上继续维护的graphstudio升级版。非常好用，可以直观看到filter详细的信息，枚举其所支持的接口。 

给出我编好的exe[下载](http://download.csdn.net/download/tifentan/10237192)（没脸没皮搞点分哈哈），包括32，64两个版本。
## 32和64位filter注册后注册表的情况

一. 32位的注册后会在3个地方有记录
- HKEY_CLASSES_ROOT\Wow6432Node\CLSID\你的id
- HKEY_LOCAL_MACHINE\SOFTWARE\Classes\Wow6432Node\CLSID\你的id
- HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Classes\CLSID\你的id

二. 64位的注册后会在2个地方有记录
- HKEY_CLASSES_ROOT\CLSID\你的id
- HKEY_LOCAL_MACHINE\SOFTWARE\Classes\CLSID\你的id

可以看出在64位系统上，32位跟64位filter的注册信息是分开的，互不影响。

## 关于虚拟摄像头的内容分发
- 必须要用到windows平台上的共享内存，这个效率是最高的了。
- 
由于是一对多或者是多对多的分发，要考虑跨进程同步的问题。使用一般的锁也可以用，但考虑到效率问题使用读写锁更好 

读写锁的实现可以参考我前面的博客，注意的是用到的状态变量放在共享内存上

- 
跨进程ipc模块封装，内容共享的时候，需要选择必要的模型。例如简单的单副本覆盖，就是说只存放一个包，新包来了覆盖旧的，没拿到旧的读进程就错过了；也可以是队列式的，可存放多个包，读进程有一定的抖动空间；也可以是保证必达的，每一包都保证读进程拿到，可以考虑用event做到，个人不考虑，因为容易卡住写进程，写进程是优先的。 

我实现的是队列式的，有人有兴趣的话可以再写个博客分享一下完整代码。





