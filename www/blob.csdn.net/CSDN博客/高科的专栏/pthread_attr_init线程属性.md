
# pthread_attr_init线程属性 - 高科的专栏 - CSDN博客

2011年08月26日 10:41:04[高科](https://me.csdn.net/pbymw8iwm)阅读数：32515标签：[thread																](https://so.csdn.net/so/search/s.do?q=thread&t=blog)[pthreads																](https://so.csdn.net/so/search/s.do?q=pthreads&t=blog)[struct																](https://so.csdn.net/so/search/s.do?q=struct&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[join																](https://so.csdn.net/so/search/s.do?q=join&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=null&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=struct&t=blog)个人分类：[linux																](https://blog.csdn.net/pbymw8iwm/article/category/863753)
[
																								](https://so.csdn.net/so/search/s.do?q=struct&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=pthreads&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=pthreads&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=thread&t=blog)


**1．线程属性**
线程具有属性，用pthread_attr_t表示，在对该结构进行处理之前必须进行初始化，在使用后需要对其去除初始化。我们用pthread_attr_init函数对其初始化，用pthread_attr_destroy对其去除初始化。
1．
|名称|:|：
|pthread_attr_init/pthread_attr_destroy
|
|功能：
|对线程属性初始化|/|去除初始化
|
|头文件：
|\#include<pthread.h>
|
|函数原形：
|int pthread_attr_init(pthread_attr_t*attr);
|int pthread_attr_destroy(pthread_attr_t*attr);
|
|参数：
|Attr|线程属性变量
|
|返回值：
|若成功返回|0|，若失败返回|-1|。
|






调用pthread_attr_init之后，pthread_t结构所包含的内容就是操作系统实现支持的线程所有属性的默认值。
如果要去除对pthread_attr_t结构的初始化，可以调用pthread_attr_destroy函数。如果pthread_attr_init实现时为属性对象分配了动态内存空间，pthread_attr_destroy还会用无效的值初始化属性对象，因此如果经pthread_attr_destroy去除初始化之后的pthread_attr_t结构被pthread_create函数调用，将会导致其返回错误。
线程属性结构如下：
typedef struct
{
intdetachstate;线程的分离状态
intschedpolicy;线程调度策略
structsched_paramschedparam;线程的调度参数
intinheritsched;线程的继承性
intscope;线程的作用域
size_tguardsize;线程栈末尾的警戒缓冲区大小
intstackaddr_set;
void*stackaddr;线程栈的位置
size_tstacksize;线程栈的大小
}pthread_attr_t;
每个个属性都对应一些函数对其查看或修改。下面我们分别介绍。
**二、线程的分离状态**
线程的分离状态决定一个线程以什么样的方式来终止自己。在默认情况下线程是非分离状态的，这种情况下，原有的线程等待创建的线程结束。只有当pthread_join（）函数返回时，创建的线程才算终止，才能释放自己占用的系统资源。
而分离线程不是这样子的，它没有被其他的线程所等待，自己运行结束了，线程也就终止了，马上释放系统资源。程序员应该根据自己的需要，选择适当的分离状态。所以如果我们在创建线程时就知道不需要了解线程的终止状态，则可以pthread_attr_t结构中的detachstate线程属性，让线程以分离状态启动。
2．
|名称|:|：
|pthread_attr_getdetachstate/pthread_attr_setdetachstate
|
|功能：
|获取|/|修改线程的分离状态属性
|
|头文件：
|\#include<pthread.h>
|
|函数原形：
|int pthread_attr_getdetachstate(const pthread_attr_t *attr,int *detachstate);
|int pthread_attr_setdetachstate(pthread_attr_t *attr,intdetachstate);
|
|参数：
|Attr|线程属性变量
|Detachstate|线程的分离状态属性
|
|返回值：
|若成功返回|0|，若失败返回|-1|。
|







可以使用pthread_attr_setdetachstate函数把线程属性detachstate设置为下面的两个合法值之一：设置为PTHREAD_CREATE_DETACHED,以分离状态启动线程；或者设置为PTHREAD_CREATE_JOINABLE,正常启动线程。可以使用pthread_attr_getdetachstate函数获取当前的datachstate线程属性。
以分离状态创建线程
|\#iinclude<pthread.h>
|void *child_thread(void *arg)
|{
|printf(“child thread run!\n”);
|}
|int main(int argc,char *argv[ ])
|{
|pthread_ttid;
|pthread_attr_tattr;
|pthread_attr_init(&attr);
|pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
|pthread_create(&tid,&attr,fn,arg);
|pthread_attr_destroy(&attr);
|sleep(1);
|}
|
**三、线程的继承性**
函数pthread_attr_setinheritsched和pthread_attr_getinheritsched分别用来设置和得到线程的继承性，这两个函数的定义如下：
3.
|名称|:|：
|pthread_attr_getinheritsched
|pthread_attr_setinheritsched
|
|功能：
|获得|/|设置线程的继承性
|
|头文件：
|\#include<pthread.h>
|
|函数原形：
|int pthread_attr_getinheritsched(const pthread_attr_t*attr,int *inheritsched);
|int pthread_attr_setinheritsched(pthread_attr_t *attr,intinheritsched);
|
|参数：
|attr|线程属性变量
|inheritsched|线程的继承性
|
|返回值：
|若成功返回|0|，若失败返回|-1|。
|




这两个函数具有两个参数，第1个是指向属性对象的指针，第2个是继承性或指向继承性的指针。继承性决定调度的参数是从创建的进程中继承还是使用在schedpolicy和schedparam属性中显式设置的调度信息。Pthreads不为inheritsched指定默认值，因此如果你关心线程的调度策略和参数，必须先设置该属性。
继承性的可能值是PTHREAD_INHERIT_SCHED（表示新现成将继承创建线程的调度策略和参数）和PTHREAD_EXPLICIT_SCHED（表示使用在schedpolicy和schedparam属性中显式设置的调度策略和参数）。
如果你需要显式的设置一个线程的调度策略或参数，那么你必须在设置之前将inheritsched属性设置为PTHREAD_EXPLICIT_SCHED.
下面我来讲进程的调度策略和调度参数。我会结合下面的函数给出本函数的程序例子。

**四、线程的调度策略**
函数pthread_attr_setschedpolicy和pthread_attr_getschedpolicy分别用来设置和得到线程的调度策略。
4.
|名称|:|：
|pthread_attr_getschedpolicy
|pthread_attr_setschedpolicy
|
|功能：
|获得|/|设置线程的调度策略
|
|头文件：
|\#include<pthread.h>
|
|函数原形：
|int pthread_attr_getschedpolicy(const pthread_attr_t*attr,int *policy);
|int pthread_attr_setschedpolicy(pthread_attr_t *attr,intpolicy);
|
|参数：
|attr|线程属性变量
|policy|调度策略
|
|返回值：
|若成功返回|0|，若失败返回|-1|。
|




这两个函数具有两个参数，第1个参数是指向属性对象的指针，第2个参数是调度策略或指向调度策略的指针。调度策略可能的值是先进先出（SCHED_FIFO）、轮转法（SCHED_RR）,或其它（SCHED_OTHER）。
SCHED_FIFO策略允许一个线程运行直到有更高优先级的线程准备好，或者直到它自愿阻塞自己。在SCHED_FIFO调度策略下，当有一个线程准备好时，除非有平等或更高优先级的线程已经在运行，否则它会很快开始执行。
SCHED_RR(轮循)策略是基本相同的，不同之处在于：如果有一个SCHED_RR
策略的线程执行了超过一个固定的时期(时间片间隔)没有阻塞，而另外的SCHED_RR或SCHBD_FIPO策略的相同优先级的线程准备好时，运行的线程将被抢占以便准备好的线程可以执行。
当有SCHED_FIFO或SCHED_RR策赂的线程在一个条件变量上等持或等持加锁同一个互斥量时，它们将以优先级顺序被唤醒。即，如果一个低优先级的SCHED_FIFO线程和一个高优先织的SCHED_FIFO线程都在等待锁相同的互斥且，则当互斥量被解锁时，高优先级线程将总是被首先解除阻塞。
**五、线程的调度参数**
函数pthread_attr_getschedparam和pthread_attr_setschedparam分别用来设置和得到线程的调度参数。
5.
|名称|:|：
|pthread_attr_getschedparam
|pthread_attr_setschedparam
|
|功能：
|获得|/|设置线程的调度参数
|
|头文件：
|\#include<pthread.h>
|
|函数原形：
|int pthread_attr_getschedparam(const pthread_attr_t*attr,struct sched_param *param);
|int pthread_attr_setschedparam(pthread_attr_t *attr,conststruct sched_param *param);
|
|参数：
|attr|线程属性变量
|param|sched_param|结构
|
|返回值：
|若成功返回|0|，若失败返回|-1|。
|




这两个函数具有两个参数，第1个参数是指向属性对象的指针，第2个参数是sched_param结构或指向该结构的指针。结构sched_param在文件/usr/include/bits/sched.h中定义如下：
struct sched_param
{
intsched_priority;
};
结构sched_param的子成员sched_priority控制一个优先权值，大的优先权值对应高的优先权。系统支持的最大和最小优先权值可以用sched_get_priority_max函数和sched_get_priority_min函数分别得到。
注意：如果不是编写实时程序，不建议修改线程的优先级。因为，调度策略是一件非常复杂的事情，如果不正确使用会导致程序错误，从而导致死锁等问题。如：在多线程应用程序中为线程设置不同的优先级别，有可能因为共享资源而导致优先级倒置。
6.
[http://linux.die.net/man/3/sched_get_priority_min](http://linux.die.net/man/3/sched_get_priority_min)

## Synopsis

**\#include <****[sched.h](http://linux.die.net/include/sched.h)****>**
int sched_get_priority_max(int*policy***);**
int sched_get_priority_min(int*policy***);**

## Description
The*sched_get_priority_max*() and*sched_get_priority_min*() functions shall return the appropriate maximum or minimum, respectively, for the scheduling policy specified by*policy*.
The value of*policy*shall be one of the scheduling policy values defined in*<**[sched.h](http://linux.die.net/include/sched.h)**>*.
## Return Value
If successful, the*sched_get_priority_max*() and*sched_get_priority_min*() functions shall return the appropriate maximum or minimum values, respectively. If unsuccessful, they shall return
 a value of -1 and set*errno*to indicate the error.
## Errors
The*sched_get_priority_max*() and*sched_get_priority_min*() functions shall fail if:
**EINVAL**The value of the*policy*parameter does not represent a defined scheduling policy.下面是上面几个函数的程序例子：

\#include <string.h>
\#include<pthread.h>
\#include<sched.h>
void *child_thread(void *arg)
{
int policy = 0;
int max_priority = 0,min_priority = 0;
struct sched_param param;
pthread_attr_t attr;
pthread_attr_init(&attr);
pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
pthread_attr_getinheritsched(&attr,&policy);
if(policy == PTHREAD_EXPLICIT_SCHED){
printf("Inheritsched:PTHREAD_EXPLICIT_SCHED\n");
}
if(policy == PTHREAD_INHERIT_SCHED){
printf("Inheritsched:PTHREAD_INHERIT_SCHED\n");
}
pthread_attr_setschedpolicy(&attr,SCHED_RR);
pthread_attr_getschedpolicy(&attr,&policy);
if(policy == SCHED_FIFO){
printf("Schedpolicy:SCHED_FIFO\n");
}
if(policy == SCHED_RR){
printf("Schedpolicy:SCHED_RR\n");
}
if(policy == SCHED_OTHER){
printf("Schedpolicy:SCHED_OTHER\n");
}
max_priority = sched_get_priority_max(policy);
min_priority = sched_get_priority_min(policy);
printf("Maxpriority:%u\n",max_priority);
printf("Minpriority:%u\n",min_priority);
param.sched_priority = max_priority;
pthread_attr_setschedparam(&attr,&param);
printf("sched_priority:%u\n",param.sched_priority);
pthread_attr_destroy(&attr);
}
int main(int argc,char *argv[ ])
{
pthread_t child_thread_id;
pthread_create(&child_thread_id,NULL,child_thread,NULL);
pthread_join(child_thread_id,NULL);
}
==[23]==gaoke@dev64_23:~/code$g++ -o test gaoke.cpp -lpthread
==[23]==gaoke@dev64_23:~/code$./test
Inheritsched:PTHREAD_EXPLICIT_SCHED
Schedpolicy:SCHED_RR
Maxpriority:99
Minpriority:1
sched_priority:99







