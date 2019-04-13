
# 2012年tcmalloc学习笔记之一 - 2019跟着小虎玩着去软考 - CSDN博客

2012年07月12日 08:41:34[littletigerat](https://me.csdn.net/littletigerat)阅读数：5864


**2012年tcmalloc****学习笔记之一**

# 一、
# tcmalloc
# 是什么？
Thread-CachingMalloc线程缓存的Malloc
# 二、
# google
# 开源工具
# google-perftools
# 究竟有哪些工具？
1.TCMalloc
2.Heap-checker
3.Heap-profiler
4.CPU-profiler
# 三、为什么要选择
# tcmalloc
1.内存分配效率高;
2.运行速度快;
3.稳定性强;
4.能够有效降低系统负载;
# 四、应用场景
1.多核；
2.高并发；
3.多线程；
# 五、如何进行内存泄露检测
使用方法：
exportHEAPCHECK=TYPE
TYPE可以为：minimal、normal、strict、draconian
# 六、还有哪些内存管理库
# (
# 或者说内存分配器
# )
1.标准的glibc
2.开源ptmalloc
3.jemalloc

# 七、
# 64
# 位操作系统与
# 32
# 位操作系统的区别
64位操作系统请先安装libunwind库，32位操作系统不要安装。
## 1.
## 如何检测是多少位的操作系统
使用命令：uname-a

# 八、
# libunwind
# 库的作用
libunwind库为基于64位CPU和操作系统的程序提供了基本的堆栈辗转开解功能，其中包括用于输出堆栈跟踪的API、用于以编程方式辗转开解堆栈的API以及支持C++异常处理机制的API。

# 九、代码的下载地址
[http://code.google.com/p/gperftools/](http://code.google.com/p/gperftools/)

# 十、最新版本
gperftools-2.0
时间：Feb3, 2012
下载地址：[http://code.google.com/p/gperftools/downloads/detail?name=gperftools-2.0.tar.gz](http://code.google.com/p/gperftools/downloads/detail?name=gperftools-2.0.tar.gz)
# 十一、如何进行编译
./configure  --disable-cpu-profiler --disable-heap-profiler --disable-heap-checker --disable-debugall

# 十二、编译出错怎么处理
configure:error: C++ compiler cannot create executables
在使用./configure编译时出现:configure:error:
 C++ compiler cannot create executables错误,是因为系统中缺少以下两个包:
gcc-c++
libstdc++-devel

请执行如下命令：
apt-getinstall gcc-c++
apt-getinstall g++
apt-getinstall libstdc++-devel
笔者的机器：
运行apt-getinstall g++即可编译通过。
# 十二、如何使用
make
makeinstall
注意：64位操作系统一定要先装libunwind这个库。

# 十三、
# libunwind
# 下载地址
[http://mirror.yongbok.net/nongnu/libunwind/](http://mirror.yongbok.net/nongnu/libunwind/)

# 十四、
# libunwind
# 最新版本
1.0.1
libunwind-1.0.1.tar.gz
时间：16-Aug-2011 13:33     287
# 十五、编译
# libunwind
# 解决办法
### 1.
### 错误提示
/usr/include/x86_64-linux-gnu/bits/setjmp2.h:26:13:错误： ‘longjmp’是未定义符号‘_longjmp’的别名/usr/include/bits/setjmp2.h:26: error: ‘longjmp’ aliased to undefined symbol ‘_longjmp’
## 2.
## 分析编译指令
gcc-U_FORTIFY_SOURCE-DHAVE_CONFIG_H -I. -I../include -I../include -I../include/tdep-x86_64 -I. -D_GNU_SOURCE -DNDEBUG -g -O2 -fexceptions -Wall -Wsign-compare -MT setjmp/longjmp.lo -MD -MP -MF setjmp/.deps/longjmp.Tpo -c setjmp/longjmp.c  -fPIC -DPIC -o setjmp/.libs/longjmp.o
## 3.
## 错误原因
缺少U_FORTIFY_SOURCE编译开关。
## 4.
## 解决办法
修改在libunwind-1.0.1/src目录下的Makefile文件
在680行针对CPPFLAGS编译选项，新增一个编译开关U_FORTIFY_SOURCE即可。






