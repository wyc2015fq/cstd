# 2012年tcmalloc学习笔记之二 - 2019跟着小虎玩着去软考 - CSDN博客
2012年07月12日 10:15:56[littletigerat](https://me.csdn.net/littletigerat)阅读数：2959
**2012****年****tcmalloc****学习笔记之二**
# 一、tcmalloc两种方式对malloc等内存分配和释放函数进行截获并提供服务。
## A.环境变量的方式，意即通过环境变量LD_PRELOAD
环境变量LD_PRELOAD指定程序运行时优先加载的动态连接库，这个动态链接库中的符号优先级是最高的。
标准C的各种函数都是存放在libc.so的文件中，在程序运行时自动链接。使用LD_PRELOAD后，自己编写的malloc的加载顺序高于glibc中的malloc，这样就实现了替换。
**1.如何使用，意即指定动态库的路径即可**
exportLD_PRELOAD="/usr/local/lib/libtcmalloc.so"
## B.直接调用动态链接库
### 1.在程序中如何使用tcmalloc
#if defined(USE_TCMALLOC)#define malloc(size) tc_malloc(size)#define calloc(count,size) tc_calloc(count,size)#define realloc(ptr,size) tc_realloc(ptr,size)#define free(ptr) tc_free(ptr)#endif
### 2.程序编译说明
编译的时候在Makefile增加编译开关USE_TCMALLOC即可
### 二、编译出来安装的libtcmalloc.so动态库放在什么地方
/usr/local/lib/libtcmalloc.so
# 三、如何检测tcmalloc是否正在工作
使用命令：
lsof-n | grep tcmalloc
