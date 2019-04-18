# Linux操作系统线程同步：互斥量（mutex） - ljx0305的专栏 - CSDN博客
2008年07月30日 17:01:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：542
互斥量从本质上说就是一把锁, 提供对共享资源的保护访问。 
1. 初始化: 
在Linux下, 线程的互斥量数据类型是pthread_mutex_t. 在使用前, 要对它进行初始化: 
对于静态分配的互斥量, 可以把它设置为PTHREAD_MUTEX_INITIALIZER, 或者调用pthread_mutex_init. 
对于动态分配的互斥量, 在申请内存(malloc)之后, 通过pthread_mutex_init进行初始化, 并且在释放内存(free)前需要调用pthread_mutex_destroy. 
原型: 
int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restric attr); 
int pthread_mutex_destroy(pthread_mutex_t *mutex); 
头文件: 
返回值: 成功则返回0, 出错则返回错误编号. 
说明: 如果使用默认的属性初始化互斥量, 只需把attr设为NULL. 其他值在以后讲解。 
2. 互斥操作: 
对共享资源的访问, 要对互斥量进行加锁, 如果互斥量已经上了锁, 调用线程会阻塞, 直到互斥量被解锁. 在完成了对共享资源的访问后, 要对互斥量进行解锁。 
首先说一下加锁函数: 
头文件: 
原型: 
int pthread_mutex_lock(pthread_mutex_t *mutex); 
int pthread_mutex_trylock(pthread_mutex_t *mutex); 
返回值: 成功则返回0, 出错则返回错误编号. 
说明: 具体说一下trylock函数, 这个函数是非阻塞调用模式, 也就是说, 如果互斥量没被锁住, trylock函数将把互斥量加锁, 并获得对共享资源的访问权限; 如果互斥量被锁住了, trylock函数将不会阻塞等待而直接返回EBUSY, 表示共享资源处于忙状态。 
再说一下解所函数: 
头文件: 
原型: int pthread_mutex_unlock(pthread_mutex_t *mutex); 
返回值: 成功则返回0, 出错则返回错误编号. 
3. 死锁: 
死锁主要发生在有多个依赖锁存在时, 会在一个线程试图以与另一个线程相反顺序锁住互斥量时发生. 如何避免死锁是使用互斥量应该格外注意的东西。 
总体来讲, 有几个不成文的基本原则: 
对共享资源操作前一定要获得锁。 
完成操作以后一定要释放锁。 
尽量短时间地占用锁。 
如果有多锁, 如获得顺序是ABC连环扣, 释放顺序也应该是ABC。 
线程错误返回时应该释放它所获得的锁。 
引用:[http://www.chinalinuxpub.com/read.php?wid=1940](http://www.chinalinuxpub.com/read.php?wid=1940)
