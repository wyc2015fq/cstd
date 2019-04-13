
# UDT 最新源码分析（一） -- 编译与源码初探 - 老雍的博客 - CSDN博客


2019年01月28日 22:49:23[老雍](https://me.csdn.net/yongkai0214)阅读数：144



### UDT 最新源码分析 -- 编译与源码初探
[UDT 编译](#UDT__1)
[UDT 模块](#UDT__25)
[部分重要模块描述](#_38)

# UDT 编译
UDT的编译以及测试用例运行比较简单，基本上是可以直接运行的，但是也有几点需要注意的地方:
VC工程较老，需要重定向解决方案到当前系统;
![重定向方案示意图](https://img-blog.csdnimg.cn/20190109114717557.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lvbmdrYWkwMjE0,size_16,color_FFFFFF,t_70#pic_center)
修改测试项目的最小类型检查为否;
![修改最小类型检查](https://img-blog.csdnimg.cn/20190108161735566.png#pic_center)
如果使用静态库，需要修改部分代码；
将udt.h代码中
\#ifdefUDT_EXPORTS\#defineUDT_API __declspec(dllexport)\#else\#defineUDT_API __declspec(dllimport)\#endif修改为
\#defineUDT_APILinux下make以后，需要设置环境变量，然后才可以运行。
UDT 模块
在udt.h中定义了对外的接口，放置在namespace UDT中，相关定义在头文件中也已经覆盖。
从接口上可以看出，除了简单的系统启动与清理以外，几乎都是网络socket与数据收发相关的操作，这也符合一个网络库的特征。至于UDT协议中提到的拥塞控制，各种数据包等定义属于系统内部结构与算法。
如果需要修改成c语言的结构，需要添加“udt_”前缀，并且对于sendfile之类的接口，调用对应的sendfile2。
namespaceUDT{...}// namespace UDT部分重要模块描述
CUDT是对应与UDT 命名空间的，建立一个UDT socket 对应一个CUDT。
CUDTUnited是整个系统的管理者，在 m_Sockets map中存储当前所有socket，资源回收，状态更新等，许多数据结构都是全局唯一的。
CUDTSocket记录每个socket在整个生命周期的所有状态。
粗略类图如下图所示。
![UML 图](https://img-blog.csdnimg.cn/201901091149505.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lvbmdrYWkwMjE0,size_16,color_FFFFFF,t_70#pic_center)
源码分析从下一篇开始。

