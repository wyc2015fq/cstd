# eAccelerator、memcached、xcache、APC 等四个加速扩展的区别 - weixin_33985507的博客 - CSDN博客
2017年01月04日 16:02:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：12
折腾VPS的朋友，在安装好LNMP等Web运行环境后都会选择一些缓存扩展安装以提高PHP运行速度，常被人介绍的有eAccelerator、memcached、xcache、Alternative PHP Cache这几个缓存扩展，它们之间有什么区别？分别的作用又是什么？我们如何选择？这是本文给于大家的答案。
#### 1、eAccelerator
eAccelerator是一个自由开放源码PHP加速器，优化和动态内容缓存，提高了PHP脚本的缓存性能，使得PHP脚本在编译的状态下，对服务器的开销几乎完全消除。 它还有对脚本起优化作用，以加快其执行效率。使PHP程序代码执效率能提高1-10倍。从文字上可以理解为：eAccelerator是PHP加速器扩展。
#### 2、memcached
Memcached 是一个高性能的分布式内存对象缓存系统，用于动态Web应用以减轻数据库负载。它通过在内存中缓存数据和对象来减少读取数据库的次数，从而提供动态、数据库驱动网站的速度。从文字上可以理解为：内存缓存扩展，并且针对于集群服务器使用较多，主要用于分布式缓存，算是数据库缓存。
#### 3、Alternative PHP Cache(APC缓存)
Alternative PHP Cache (APC缓存)是一种对PHP有效的开放源高速缓冲储存器工具，他能够缓存Opcode(目标文件)的PHP中间码。 APC的缓存分两部分:系统缓存和用户数据缓存. 系统缓存 是自动使用的,是指APC把PHP文件源码的编译结果缓存起来,然后在再次调用时先对比时间标记。如果未过期,则使用缓存代码运行。默认缓存 3600s(一小时).但是这样仍会浪费大量CPU时间.因此可以在php.ini中设置system缓存为永不过期(apc.ttl=0).不过如果这样设置,改运PHP代码后需要restart一下您的web服务器(比如apache…).目前对APC的性能测试一般指的是这一层cache;从文字上理到：Alternative PHP Cache(APC缓存)也算是一种内存缓存扩展，算是数据库缓存扩展。
### 4、Xcache
是一个开源的opcode缓存器/优化器, 他能够提高服务器上的PHP性能，他通过把编译PHP后的数据缓冲到共享内存从而避免重复的编译过程, 能够直接使用缓冲区已编译的代码从而提高速度. 通常能够提高您的页面生成速率2到5倍, 降低服务器负载。 其实它与eAccelerator一样是PHP加速器扩展。
以上就是他们的作用与区别，简单一点就是：eAccelerator,xcache是PHP缓存扩展，memcached、APC缓存是数据库缓存扩展，一般两者只有要安装其中一个即可，不要贪多。另外，实际测试中发现当Cache数量超过一定阀值的时候，APC的性能不如Memcache。所以在扩展的选择上，本人一般会选择xcache+memcached这个组合。
以上部份内容收集于互联网，本人知识有限，如上面文字解释不当，请指出得于更正，谢谢。
