# Posix线程编程指南(2) - ljx0305的专栏 - CSDN博客
2012年12月28日 17:54:38[ljx0305](https://me.csdn.net/ljx0305)阅读数：494
概念及作用
在单线程程序中，我们经常要用到"全局变量"以实现多个函数间共享数据。在多线程环境下，由于数据空间是共享的，因此全局变量也为所有线程所共有。但有时应用程序设计中有必要提供线程私有的全局变量，仅在某个线程中有效，但却可以跨多个函数访问，比如程序可能需要每个线程维护一个链表，而使用相同的函数操作，最简单的办法就是使用同名而不同变量地址的线程相关数据结构。这样的数据结构可以由Posix线程库维护，称为线程私有数据（Thread-specific Data，或TSD）。
[回页首](http://www.ibm.com/developerworks/cn/linux/thread/posix_threadapi/part2/#ibm-pcon)
创建和注销
Posix定义了两个API分别用来创建和注销TSD：
int pthread_key_create(pthread_key_t *key, void (*destr_function) (void *))
该函数从TSD池中分配一项，将其值赋给key供以后访问使用。如果destr_function不为空，在线程退出（pthread_exit()）时将以key所关联的数据为参数调用destr_function()，以释放分配的缓冲区。
不论哪个线程调用pthread_key_create()，所创建的key都是所有线程可访问的，但各个线程可根据自己的需要往key中填入不同的值，这就相当于提供了一个同名而不同值的全局变量。在LinuxThreads的实现中，TSD池用一个结构数组表示：
static struct pthread_key_struct pthread_keys[PTHREAD_KEYS_MAX] = { { 0, NULL } };
创建一个TSD就相当于将结构数组中的某一项设置为"in_use"，并将其索引返回给*key，然后设置destructor函数为destr_function。
注销一个TSD采用如下API：
int pthread_key_delete(pthread_key_t key)
这个函数并不检查当前是否有线程正使用该TSD，也不会调用清理函数（destr_function），而只是将TSD释放以供下一次调用pthread_key_create()使用。在LinuxThreads中，它还会将与之相关的线程数据项设为NULL（见"访问"）。
[回页首](http://www.ibm.com/developerworks/cn/linux/thread/posix_threadapi/part2/#ibm-pcon)
访问
TSD的读写都通过专门的Posix Thread函数进行，其API定义如下：
int  pthread_setspecific(pthread_key_t  key,  const   void  *pointer)
void * pthread_getspecific(pthread_key_t key)
写入（pthread_setspecific()）时，将pointer的值（不是所指的内容）与key相关联，而相应的读出函数则将与key相关联的数据读出来。数据类型都设为void *，因此可以指向任何类型的数据。
在LinuxThreads中，使用了一个位于线程描述结构（_pthread_descr_struct）中的二维void *指针数组来存放与key关联的数据，数组大小由以下几个宏来说明：
#define PTHREAD_KEY_2NDLEVEL_SIZE       32
#define PTHREAD_KEY_1STLEVEL_SIZE   \
((PTHREAD_KEYS_MAX + PTHREAD_KEY_2NDLEVEL_SIZE - 1)
/ PTHREAD_KEY_2NDLEVEL_SIZE)
    其中在/usr/include/bits/local_lim.h中定义了PTHREAD_KEYS_MAX为1024，
    因此一维数组大小为32。而具体存放的位置由key值经过以下计算得到：
idx1st = key / PTHREAD_KEY_2NDLEVEL_SIZE
idx2nd = key % PTHREAD_KEY_2NDLEVEL_SIZE
也就是说，数据存放与一个32×32的稀疏矩阵中。同样，访问的时候也由key值经过类似计算得到数据所在位置索引，再取出其中内容返回。
[回页首](http://www.ibm.com/developerworks/cn/linux/thread/posix_threadapi/part2/#ibm-pcon)
使用范例
以下这个例子没有什么实际意义，只是说明如何使用，以及能够使用这一机制达到存储线程私有数据的目的。
#include <stdio.h>
#include <pthread.h>
pthread_key_t   key;
void echomsg(int t)
{
        printf("destructor excuted in thread %d,param=%d\n",pthread_self(),t);
}
void * child1(void *arg)
{
        int tid=pthread_self();
        printf("thread %d enter\n",tid);
        pthread_setspecific(key,(void *)tid);
        sleep(2);
        printf("thread %d returns %d\n",tid,pthread_getspecific(key));
        sleep(5);
}
void * child2(void *arg)
{
        int tid=pthread_self();
        printf("thread %d enter\n",tid);
        pthread_setspecific(key,(void *)tid);
        sleep(1);
        printf("thread %d returns %d\n",tid,pthread_getspecific(key));
        sleep(5);
}
int main(void)
{
        int tid1,tid2;
        printf("hello\n");
        pthread_key_create(&key,echomsg);
        pthread_create(&tid1,NULL,child1,NULL);
        pthread_create(&tid2,NULL,child2,NULL);
        sleep(10);
        pthread_key_delete(key);
        printf("main thread exit\n");
        return 0;
}
给例程创建两个线程分别设置同一个线程私有数据为自己的线程ID，为了检验其私有性，程序错开了两个线程私有数据的写入和读出的时间，从程序运行结果可以看出，两个线程对TSD的修改互不干扰。同时，当线程退出时，清理函数会自动执行，参数为tid。
引用：http://www.ibm.com/developerworks/cn/linux/thread/posix_threadapi/part2/
