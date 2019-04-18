# Posix多线程编程学习笔记（四）—互斥量（2） - ljx0305的专栏 - CSDN博客
2008年08月01日 11:14:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：650
**四、互斥锁属性<?xml:namespace prefix = o ns = "urn:schemas-microsoft-com:office:office" />**
线程和线程的同步对象（互斥量，读写锁，条件变量）都具有属性。在修改属性前都需要对该结构进行初始化。使用后要把该结构回收。我们用pthread_ mutexattr_init函数对pthread_mutexattr结构进行初始化，用pthread_mutexattr_destroy函数对该结构进行回收。
4
|名称:：|pthread_mutexattr_init/pthread_mutexattr_destroy|
|----|----|
|功能：|初始化/回收pthread_mutexattr_t结构|
|头文件：|#include <pthread.h>|
|函数原形：|int pthread_mutexattrattr_init(pthread_mutexattr_t *attr);int pthread_mutexattrattr_destroy( pthread_mutexattr_t *attr );|
|参数：|attrpthread_mutexattr_t结构变量|
|返回值：|若成功返回0，若失败返回错误编号。|

pthread_mutexattr_init将属性对象的值初始化为缺省值。并分配属性对象占用的内存空间。
attr中pshared属性表示用这个属性对象创建的互斥锁的作用域，它的取值可以是PTHREAD_PROCESS_PRIVATE(缺省值，表示由这个属性对象创建的互斥锁只能在进程内使用)或PTHREAD_PROCESS_SHARED。
互斥量属性分为共享互斥量属性和类型互斥量属性。两种属性分别由不同的函数得到并由不同的函数进行修改。pthread_mutexattr_getpshared和pthread_mutexattr_setpshared函数可以获得和修改共享互斥量属性。pthread_mutexattr_gettype和pthread_mutexattr_settype函数可以获得和修改类型互斥量属性。下面我们分别介绍。
5
|名称:：|pthread_mutexattr_getpshared/pthread_mutexattr_setpshared|
|----|----|
|功能：|获得/修改共享互斥量属性|
|头文件：|#include <pthread.h>|
|函数原形：|int pthread_mutexattrattr_ getpshared ( const pthread_attr_t *restrict attr,int*restrict pshared);int pthread_mutexattrattr_ setpshared (const pthread_attr_t *restrict attr,int pshared);|
|参数：||
|返回值：|若成功返回0，若失败返回错误编号。|

共享互斥量属性用于规定互斥锁的作用域。互斥锁的域可以是进程内的也可以是进程间的。pthread_mutexattrattr_ getpshared可以返回属性对象的互斥锁作用域属性。可以是以下值：PTHREAD_PROCESS_SHARED，PTHREAD_PROCESS_PRIVATE。如果互斥锁属性对象的pshared属性被置PTHREAD_PROCESS_SHARED。那么由这个属性对象创建的互斥锁将被保存在共享内存中，可以被多个进程中的线程共享。如果pshared属性被置为PTHREAD_PROCESS_PRIVATE，那么只有和创建这个互斥锁的线程在同一个进程中的线程才能访问这个互斥锁。 
6
|名称:：|pthread_mutexattr_gettype/pthread_mutexattr_settype|
|----|----|
|功能：|获得/修改类型互斥量属性|
|头文件：|#include <pthread.h>|
|函数原形：|int pthread_mutexattrattr_ getpshared ( const pthread_attr_t *restrict attr,int*restrict pshared);int pthread_mutexattrattr_ setpshared (const pthread_attr_t *restrict attr,int pshared);|
|参数：||
|返回值：|若成功返回0，若失败返回错误编号。|

pthread_mutexattr_gettype函数可以获得互斥锁类型属性。缺省的互斥锁类型属性是PTHREAD_MUTEX_DEFAULT。
合法的类型属性值有：
PTHREAD_MUTEX_NORMAL；
PTHREAD_MUTEX_ERRORCHECK；
PTHREAD_MUTEX_RECURSIVE；
PTHREAD_MUTEX_DEFAULT。
类型说明：
PTHREAD_MUTEX_NORMAL
这种类型的互斥锁不会自动检测死锁。如果一个线程试图对一个互斥锁重复锁定，将会引起这个线程的死锁。如果试图解锁一个由别的线程锁定的互斥锁会引发不可预料的结果。如果一个线程试图解锁已经被解锁的互斥锁也会引发不可预料的结果。
PTHREAD_MUTEX_ERRORCHECK
这种类型的互斥锁会自动检测死锁。如果一个线程试图对一个互斥锁重复锁定，将会返回一个错误代码。如果试图解锁一个由别的线程锁定的互斥锁将会返回一个错误代码。如果一个线程试图解锁已经被解锁的互斥锁也将会返回一个错误代码。
PTHREAD_MUTEX_RECURSIVE
如果一个线程对这种类型的互斥锁重复上锁，不会引起死锁，一个线程对这类互斥锁的多次重复上锁必须由这个线程来重复相同数量的解锁，这样才能解开这个互斥锁，别的线程才能得到这个互斥锁。如果试图解锁一个由别的线程锁定的互斥锁将会返回一个错误代码。如果一个线程试图解锁已经被解锁的互斥锁也将会返回一个错误代码。这种类型的互斥锁只能是进程私有的（作用域属性为PTHREAD_PROCESS_PRIVATE）。
PTHREAD_MUTEX_DEFAULT
这种类型的互斥锁不会自动检测死锁。如果一个线程试图对一个互斥锁重复锁定，将会引起不可预料的结果。如果试图解锁一个由别的线程锁定的互斥锁会引发不可预料的结果。如果一个线程试图解锁已经被解锁的互斥锁也会引发不可预料的结果。POSIX标准规定，对于某一具体的实现，可以把这种类型的互斥锁定义为其他类型的互斥锁。
**五、应用互斥量需要注意的几点**
1、互斥量需要时间来加锁和解锁。锁住较少互斥量的程序通常运行得更快。所以，互斥量应该尽量少，够用即可，每个互斥量保护的区域应则尽量大。
2、互斥量的本质是串行执行。如果很多线程需要领繁地加锁同一个互斥量，
则线程的大部分时间就会在等待，这对性能是有害的。如果互斥量保护的数据(或代码)包含彼此无关的片段，则可以特大的互斥量分解为几个小的互斥量来提高性能。这样，任意时刻需要小互斥量的线程减少，线程等待时间就会减少。所以，互斥量应该足够多(到有意义的地步)，每个互斥量保护的区域则应尽量的少。
引用：[http://blog.chinaunix.net/u/22935/showart_340408.html](http://blog.chinaunix.net/u/22935/showart_340408.html)
