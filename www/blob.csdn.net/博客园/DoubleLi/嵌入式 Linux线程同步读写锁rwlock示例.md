# 嵌入式 Linux线程同步读写锁rwlock示例 - DoubleLi - 博客园






读写锁比mutex有更高的适用性，可以多个线程同时占用读模式的读写锁，但是只能一个线程占用写模式的读写锁。
1. 当读写锁是**写加锁状态**时，在这个锁被解锁之前，所有试图对这个锁加锁的线程都会被阻塞；
2. 当读写锁在**读加锁状态**时，所有试图以读模式对它进行加锁的线程都可以得到访问权，但是以写模式对它进行枷锁的线程将阻塞；
3. 当读写锁在**读模式锁状态**时，如果有另外线程试图以**写模式加锁**，**读写锁通常会阻塞随后的读模式锁请求**，这样可以避免读模式锁长期占用，而等待的写模式锁请求长期阻塞；
这种锁适用对数据结构进行读的次数比写的次数多的情况下，因为可以进行读锁共享。

**API接口说明：**1) 初始化和销毁

```
#include <pthread.h>
int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
成功则返回0, 出错则返回错误编号.
```

2) 读加锁和写加锁
获取锁的两个函数是阻塞操作

```
#include <pthread.h>
int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock); 
成功则返回0, 出错则返回错误编号.
```

3) 非阻塞获得读锁和写锁
非阻塞的获取锁操作, 如果可以获取则返回0, 否则返回错误的EBUSY.

```
#include <pthread.h>
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
成功则返回0, 出错则返回错误编号.
```









