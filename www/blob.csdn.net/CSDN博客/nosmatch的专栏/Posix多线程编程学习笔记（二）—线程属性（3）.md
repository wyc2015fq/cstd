# Posix多线程编程学习笔记（二）—线程属性（3） - nosmatch的专栏 - CSDN博客
2012年01月09日 15:07:36[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：793标签：[多线程																[编程																[扩展](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)
个人分类：[多线程](https://blog.csdn.net/HDUTigerkin/article/category/1061405)
**六、线程的作用域**
函数pthread_attr_setscope和pthread_attr_getscope分别用来设置和得到线程的作用域，这两个函数的定义如下：
7．
|名称:：|pthread_attr_setscopepthread_attr_getscope|
|----|----|
|功能：|获得/设置线程的作用域|
|头文件：|#include <pthread.h>|
|函数原形：|int pthread_attr_setscope(pthread_attr_t *attr,int scope);int pthread_attr_getscope(const pthread_attr_t *attr,int *scope);|
|参数：|attr          线程属性变量scope        线程的作用域|
|返回值：|若成功返回0，若失败返回-1。|

这两个函数具有两个参数，第1个是指向属性对象的指针，第2个是作用域或指向作用域的指针，作用域控制线程是否在进程内或在系统级上竞争资源，可能的值是PTHREAD_SCOPE_PROCESS（进程内竞争资源）PTHREAD_SCOPE_SYSTEM.（系统级上竞争资源）。
**七、线程堆栈的大小**
函数pthread_attr_setstacksize和pthread_attr_getstacksize分别用来设置和得到线程堆栈的大小，这两个函数的定义如下所示：
8．
|名称:：|pthread_attr_getdetstacksizepthread_attr_setstacksize|
|----|----|
|功能：|获得/修改线程栈的大小|
|头文件：|#include <pthread.h>|
|函数原形：|int pthread_attr_getstacksize(const pthread_attr_t *restrict attr,size_t *restrict stacksize);int pthread_attr_setstacksize(pthread_attr_t *attr ,size_t *stacksize);|
|参数：|attr          线程属性变量stacksize      堆栈大小|
|返回值：|若成功返回0，若失败返回-1。|


这两个参数具有两个参数，第1个是指向属性对象的指针，第2个是堆栈大小或指向堆栈大小的指针
如果希望改变栈的默认大小，但又不想自己处理线程栈的分配问题，这时使用pthread_attr_setstacksize函数就非常用用。
**八、线程堆栈的地址**
函数pthread_attr_setstackaddr和pthread_attr_getstackaddr分别用来设置和得到线程堆栈的位置，这两个函数的定义如下：
9.
|名称:：|pthread_attr_setstackaddrpthread_attr_getstackaddr|
|----|----|
|功能：|获得/修改线程栈的位置|
|头文件：|#include <pthread.h>|
|函数原形：|int pthread_attr_getstackaddr(const pthread_attr_t *attr,void **stackaddf);int pthread_attr_setstackaddr(pthread_attr_t *attr,void *stackaddr);|
|参数：|attr          线程属性变量stackaddr     堆栈地址|
|返回值：|若成功返回0，若失败返回-1。|

这两个函数具有两个参数，第1个是指向属性对象的指针，第2个是堆栈地址或指向堆栈地址的指针。
**九、线程栈末尾的警戒缓冲区大小**
函数pthread_attr_getguardsize和pthread_attr_setguardsize分别用来设置和得到线程栈末尾的警戒缓冲区大小，这两个函数的定义如下：
10.
|名称:：|pthread_attr_getguardsizepthread_attr_setguardsize|
|----|----|
|功能：|获得/修改线程栈末尾的警戒缓冲区大小|
|头文件：|#include <pthread.h>|
|函数原形：|int pthread_attr_getguardsize(const pthread_attr_t *restrict attr,size_t *restrict guardsize);int pthread_attr_setguardsize(pthread_attr_t *attr ,size_t *guardsize);|
|参数：||
|返回值：|若成功返回0，若失败返回-1。|

线程属性guardsize控制着线程栈末尾之后以避免栈溢出的扩展内存大小。这个属性默认设置为PAGESIZE个字节。可以把guardsize线程属性设为0，从而不允许属性的这种特征行为发生：在这种情况下不会提供警戒缓存区。同样地，如果对线程属性stackaddr作了修改，系统就会假设我们会自己管理栈，并使警戒栈缓冲区机制无效，等同于把guardsize线程属性设为0。
