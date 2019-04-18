# Posix多线程编程学习笔记（一）—线程基础（2） - ljx0305的专栏 - CSDN博客
2008年08月01日 10:07:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：577

6．
|名称:：|pthread_detach|
|----|----|
|功能：|使线程进入分离状态。|
|头文件：|#include <pthread.h>|
|函数原形：|int pthread_detach(pthread_t tid);|
|参数：|<?xml:namespace prefix = o ns = "urn:schemas-microsoft-com:office:office" />|
|返回值：|若成功则返回0，否则返回错误编号。|
在默认情况下，线程的终止状态会保存到对该线程调用pthread_join,如果线程已经处于分离状态，线程的底层存储资源可以在线程终止时立即被收回。当线程被分离时，并不能用pthread_join函数等待它的终止状态。对分离状态的线程进行pthread_join的调用会产生失败，返回EINVAL.pthread_detach调用可以用于使线程进入分离状态。
7．
|名称:：|pthread_cancel|
|----|----|
|功能：|取消同一进程中的其他线程|
|头文件：|#include <pthread.h>|
|函数原形：|int pthread_cancel(pthread_t tid);|
|参数：|tid线程id|
|返回值：|若成功返回0，否则返回错误编号。|

在默认的情况下，pthread_cancel函数会使由tid标识的线程的行为表现为如同调用了参数为PTHEAD_CANCELED的pthread_exit函数，但是，线程可以选择忽略取消方式和控制取消方式。pthread_cancel并不等待线程终止，它仅仅提出请求。
8．
|名称:：|pthread_cancel_push/ pthread_cancel_push_pop|
|----|----|
|功能：|线程清理处理程序|
|头文件：|#include <pthread.h>|
|函数原形：|void pthread_cancel_push(void (*rtn)(void *)，void *arg);void pthread_cancel_pop(int execute);|
|参数：|rtn 处理程序入口地址arg 传递给处理函数的参数|
|返回值：|无|

线程可以安排它退出时需要调用的函数，这样的函数称为线程清理处理程序，线程可以建立多个清理处理程序。处理程序记录在栈中，也就是说它们的执行顺序与它们注册时的顺序相反。
要注意的是如果线程是通过从他的启动例程中返回而终止的，它的处理程序就不会调用。还要注意清理处理程序是按照与它们安装时相反的顺序调用的。
#include <pthread.h>
#include <stdio.h>
void cleanup(void *arg)
{
printf(“cleanup: %s/n”,(char *)arg);
}
void *thr_fn(void *arg) /*线程入口地址*/
{
printf(“thread start/n”);
pthread_cleanup_push(cleanup,”thread first handler”);/*设置第一个线程处理程序*/
pthread_cleanup_push(cleanup,”thread second handler”); /*设置第二个线程处理程序*/
printf(“thread push complete/n”);
pthread_cleanup_pop(0); /*取消第一个线程处理程序*/
pthread_cleanup_pop(0); /*取消第二个线程处理程序*/
}
int main()
{
pthread_t tid;
void *tret;
pthread_creat(&tid,NULL,thr_fn,(void *)1); /*创建一个线程*/
pthread_join(tid,&tret); /*获得线程终止状态*/
ptinrf(“thread exit code %d/n”,(int)tret);
}
**八、一次性初始化**
有时候我们需要对一些posix变量只进行一次初始化，如线程键（我下面会讲到）。如果我们进行多次初始化程序就会出现错误。
在传统的顺序编程中，一次性初始化经常通过使用布尔变量来管理。控制变量被静态初始化为0，而任何依赖于初始化的代码都能测试该变量。如果变量值仍然为0，则它能实行初始化，然后将变量置为1。以后检查的代码将跳过初始化。
但是在多线程程序设计中，事情就变的复杂的多。如果多个线程并发地执行初始化序列代码，2个线程可能发现控制变量为0，并且都实行初始话，而该过程本该仅仅执行一次。初始化的状态必须由互斥量保护。
如果我们需要对一个posix变量静态的初始化，可使用的方法是用一个互斥量对该变量的初始话进行控制。但有时候我们需要对该变量进行动态初始化，pthread_once就会方便的多。
9.
|名称:：|pthread_once|
|----|----|
|功能：|一次性初始化|
|头文件：|#include <pthread.h>|
|函数原形：|pthread_once_t once_control=PTHREAD_ONCE_INIT;int pthread_once(pthread_once_t *once_control,void(*init_routine)(void));|
|参数：|once_control控制变量init_routine初始化函数|
|返回值：|若成功返回0，若失败返回错误编号。|

类型为pthread_once_t的变量是一个控制变量。控制变量必须使用PTHREAD_ONCE_INIT宏静态地初始化。
pthread_once函数首先检查控制变量，判断是否已经完成初始化，如果完成就简单地返回；否则，pthread_once调用初始化函数，并且记录下初始化被完成。如果在一个线程初始时，另外的线程调用pthread_once，则调用线程等待，直到那个现成完成初始话返回。
下面就是该函数的程序例子：
#include <pthread.h>
pthread_once_t once=PTHREAD_ONCE_INIT;
pthread_mutex_t mutex;)　/*互斥量，我们后面会讲到*/
void once_init_routine(void)　/*一次初始化函数*/
{
int status;
status=pthread_mutex_init(&mutex,NULL);/*初始化互斥量*/
if(status==0)
printf(“Init success!,My id is %u”,pthread_self());
}
void *child_thread(void *arg)
{
printf(“I’m child ,My id is %u”,pthread_self());
pthread_once(&once,once_init_routine); /*子线程调用一次性初始化函数*/
}
int main(int argc,char *argv[ ])
{
pthread_t child_thread_id;
pthread_create(&child_thread_id,NULL,child_thread,NULL);/*创建子线程*/
printf(“I’m father,my id is %u”,pthread_self());
pthread_once(&once_block,once_init_routine);/*父线程调用一次性初始化函数*/
pthread_join(child_thread_id,NULL);
}
程序运行结果如下：
./once
I’m father,My id is 3086874304
Init success!,My id is 3086874304
I’m child, My id is 3086871472
从上面的结果可以看到当主函数初始化成功后，子函数初始化失败。
**九、线程的私有数据**
在进程内的所有线程共享相同的地址空间，任何声明为静态或外部的变量，或在进程堆声明的变量，都可以被进程所有的线程读写。那怎样才能使线程序拥有自己的私有数据呢。
posix提供了一种方法，创建线程键。
10.
|名称:：|pthread_key_create|
|----|----|
|功能：|建立线程私有数据键|
|头文件：|#include <pthread.h>|
|函数原形：|int pthread_key_create(pthread_key *key,void(*destructor)(void *));|
|参数：|key私有数据键destructor清理函数|
|返回值：|若成功返回0，若失败返回错误编号。|

第一个参数为指向一个键值的指针，第二个参数指明了一个destructor函数（清理函数），如果这个参数不为空，那么当每个线程结束时，系统将调用这个函数来释放绑定在这个键上的内存块。这个函数常和函数pthread_once一起使用，为了让这个键只被创建一次。函数pthread_once声明一个初始化函数，第一次调用pthread_once时它执行这个函数，以后的调用将被它忽略。
下面是程序例子：
#include <pthread.h>
pthread_key_t tsd_key;
pthread_once_t key_once=PTHREAD_ONCE_INIT;
void once_routine(void)
{
int status;
status=pthread_key_create(&tsd_key,NULL);/*初始化线程私有数据键*/
if(status=0)
printf(“Key create success! My id is %u/n”,pthread_self());
}
void *child_thread(void *arg)
{
printf(“I’m child,My id is %u/n”,pthread_self());
pthread_once(&key_once,once_routine);/* 调用一次性初始化函数*/
}
int main(int argc,char *argv[ ])
{
pthread_t child_thread_id;
pthread_create(&child_thread_id,NULL,child_thread,NULL);
printf(“I’m father,my id is%u/n”,pthread_self());
pthread_once(&key_once,once_routine);
}
程序运行结果如下：
I’m father,My id is 3086231232
Key create success! My id is 3086231232
I’m child,My id is 2086228400
本文转自 :[http://blog.chinaunix.net/u/22935/showart_310714.html](http://blog.chinaunix.net/u/22935/showart_310714.html)
