# Posix多线程编程学习笔记（五）—条件变量（2） - ljx0305的专栏 - CSDN博客
2008年08月01日 13:47:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：664
下面的程序是经典的生产者/消费者的例证。
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX 5
<?xml:namespace prefix = o ns = "urn:schemas-microsoft-com:office:office" />
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER; /*初始化互斥锁*/
pthread_cond_t=PTHREAD_CODE_INITIALIZER; /*初始化条件变量*/
typedef struct{
char buffer[MAX];
int how_many;
}BUFFER;
BUFFER share={“”,0};
char ch=’A’;/*初始化ch*/
void *read_some(void *);
void *write_some(void *);
int main(void)
{
pthread_t t_read;
pthread_t t_write;
pthread_create(&t_read,NULL,read_some,(void *)NULL); /*创建进程t_a*/
pthread_create(&t_write,NULL,write_some,(void *)NULL); /*创建进程t_b*/
pthread_join(t_write,(void **)NULL);
pthread_mutex_destroy(&mutex);
pthread_cond_destroy(&cond);
exit(0);
}
void *read_some(void *junk)
{
int n=0;
printf(“R %2d: starting/n”,pthread_self());
while(ch!=’Z’)
{
pthread_mutex_lock(&lock_it);/*锁住互斥量*/
if(share.how_many!=MAX)
{
share.buffer[share.how_many++]=ch++;/*把字母读入缓存*/
printf(“R %2d:Got char[%c]/n”,pthread_self(),ch-1);/*打印读入字母*/
if(share.how_many==MAX)
{
printf(“R %2d:signaling full/n”,pthread_self());
pthread_cond_signal(&write_it);/*如果缓存中的字母到达了最大值就发送信号*/
}
pthread_mutex_unlock(&lock_it);/*解锁互斥量*/
} 
sleep(1);
printf(“R %2d:Exiting/n”,pthread_self());
return NULL;
}
void *write_some(void *junk)
{
int i;
int n=0;
printf(“w %2d: starting/n”,pthread_self());
while(ch!=’Z’)
{
pthread_mutex_lock(&lock_it);/*锁住互斥量*/
printf(“/nW %2d:Waiting/n”,pthread_self());
while(share.how_many!=MAX)/*如果缓存区字母不等于最大值就等待*/
pthread_cond_wait(&write_it,&lock_it);
printf(“W %2d:writing buffer/n”,pthread_self());
for(i=0;share.buffer[i]&&share.how_many;++i,share.how_many--)
putchar(share.buffer[i]); /*循环输出缓存区字母*/
pthread_mutex_unlock(&lock_it);/*解锁互斥量*/
}
printf(“W %2d:exiting/n”,pthread_self());
return NULL;
}
程序每读入5个字母，打印一遍，并清空缓存区，循环执行直到Y为止。
程序运行结果如下：
#cc –lpthread –o readandwrite readandwrite.c
#./readandwrire
R 1082330304: stareing
W 1090718784:string
W 1090718784:Waiting
R 1082330304:Got char[A]
R 1082330304:Got char[B]
R 1082330304:Got char[C]
R 1082330304:Got char[D]
R 1082330304:Got char[E]
R 1082330304:signaling full
W 1090718784:wring buffer
ABCDE
W 1090718784:Waiting
R 1082330304:Got char[F]
…………………………
**三、条件变量属性**
使用条件变量之前要先进行初始化。可以像我们前面那样可静态初始化pthread_cond_t my_condition=PTHREAD_COND_INITIALIZER;也可以利用函数pthread_cond_init动态初始化。条件变量属性类型为pthread_condattr_t，它们由以下函数初始化或摧毁。
11．
|名称:：|pthread_condattr_init/pthread_condattr_destroy|
|----|----|
|功能：|初始化/回收pthread_condattr_t结构|
|头文件：|#include <pthread.h>|
|函数原形：|int pthread_condattr_init(pthread_condattr_t *attr);int pthread_condattr_destroy(pthread_condattr_t *attr);|
|参数：||
|返回值：|若成功返回0，若失败返回错误编号。|

一旦某个条件变量对象被初始化了，我们就可以利用下面函数来查看或修改特定属性了。
12.
|名称:：|pthread_condattr_getpshared/pthread_condattr_setpshared|
|----|----|
|功能：|查看或修改条件变量属性|
|头文件：|#include <pthread.h>|
|函数原形：|int pthread_condattr_init(const pthread_condattr_t *restrict attr);int pthread_condattr_destroy(pthread_rwlockattr_t *attr,int pshared);|
|参数：||
|返回值：|若成功返回0，若失败返回错误编号。|

pthread_condattr_getpshared函数在由valptr指向的整数中返回这个属性的当前值，pthread_condattr_setpshared则根据value的值设置这个属性的当前值。value的值可以是PTHREAD_PROCESS_PRIVATE或PTHREAD_PROCESS_SHARED(进程间共享).
**四、条件变量与互斥锁、信号量的区别**
到这里，我们把posix的互斥锁、信号量、条件变量都接受完了，下面我们来比较一下他们。
1.互斥锁必须总是由给它上锁的线程解锁，信号量的挂出即不必由执行过它的等待操作的同一进程执行。一个线程可以等待某个给定信号灯，而另一个线程可以挂出该信号灯。
2.互斥锁要么锁住，要么被解开（二值状态，类型二值信号量）。
3.由于信号量有一个与之关联的状态（它的计数值），信号量挂出操作总是被记住。然而当向一个条件变量发送信号时，如果没有线程等待在该条件变量上，那么该信号将丢失。
4.互斥锁是为了上锁而优化的，条件变量是为了等待而优化的，信号灯即可用于上锁，也可用于等待，因而可能导致更多的开销和更高的复杂性。
 引用 ：[http://blog.chinaunix.net/u/22935/showart_340463.html](http://blog.chinaunix.net/u/22935/showart_340463.html)
