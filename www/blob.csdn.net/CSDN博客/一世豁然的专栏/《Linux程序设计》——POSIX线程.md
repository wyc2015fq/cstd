# 《Linux程序设计》——POSIX线程 - 一世豁然的专栏 - CSDN博客





2014年10月13日 22:54:02[一世豁然](https://me.csdn.net/Explorer_day)阅读数：684
个人分类：[liunx应用编程](https://blog.csdn.net/Explorer_day/article/category/2276817)









一、线程及线程相关介绍

 1、线程的定义：在一个程序中的多个执行路线叫作线程，线程是一个进程内部的一个控制序列。




 2、fork系统调用和创建新线程的区别

   I、当进程执行fork调用时，将创建出该进程的一份副本。这个新进程拥有自己的变量和PID，它的时间调度也是独立的，它的执行几乎完全独立于父进程。

  II、当在进程中创建一个新线程，新的执行线程将拥有自己的栈（因此也有自己的局部变量），但与它的创建者共享全局变量、文件标识符、信号处理函数和当前目录状态。




 3、线程的优点和缺点

   1）、优点

        I、可以让线程同时做两件事。

       II、一个混杂着输入、计算和输出的应用程序，可以将这个部分分离为3个线程来执行，从而改善程序执行的性能。

      III、使用线程之间的切换对资源的需求要比线程之间的切换要少的多。


  2）、缺点

       I、编写多线程程序需要非常仔细的设计。

      II、对多线程程序的程序要比对单线程程序的调试困难的多。

     III、使用线程在计算机的执行速度不一定快。







二、线程程序

 1、线程有一套完整的与其有关的函数调用，它们中的绝大部分函数都以pthread开头。在程序中我们必须包含头文件pthread.h，并且在编译程序时需要用选项-lpthread来链接 


       线程库。




 2、为了解决线程与线程之间的共用执行线程相互影响，使用可重入的例程。可重入代码可以被多次调用而任然正常工作，这些调用可以来自不同的线程，也可以是某种形式的

       嵌套调用。在编写多线程时，通过定义宏_REENTRANT来告诉编译器我们需要可重入功能，这个宏的定义必须位于程序中的任何#include语句之前。




 3、创建新线程

**     #include<pthread.h>**

**     int  pthread_create(pthrea_t  *thread,  pthread_attr_t  *attr,  void  *(*start_routime) (void *),  void  *arg);**

    I、thread参数指向pthrea_t类型数据的指针。线程被创建时，这个指针指向额变量将写入一个标识符，我们用该标识符引用新线程。

   II、attr参数设置线程额属性。我们一般不需要特殊的属性，所以将其参数设置为NULL。

   III、void  *(*start_routime) (void *)参数为线程将要启动的函数。

   IV、arg参数为传递给以上参数函数的的参数。

    V、此函数调用成功时返回0，失败则返回错误代码。

**     #include<pthread.h>**

**     void  pthread_exit(void  *retval);**

    I、此函数的作用是终止调用它的线程并返回一个指向某个对象的指针。

**  #include<pthread.h>**

**     int  pthread_join(pthread_t  th,  void  **thread_return);**

    I、th参数指定了将要等待的线程，线程通过pthread_create返回的标识符来指定。

   II、thread_return参数是一个指针，它指向另一个指针，而后者指向线程的返回值。

  III、此函数在成功时返回0，失败时返回错误代码。




 4、同时执行两个线程（P421-422代码）







三、同步

 1、线程同步执行额两种基本方法：信号量和互斥量。




 2、用信号量进行同步

   1）、有两组接口函数用于信号量。一组取自POSIX的实时扩展，用于线程/另一组被称为系统V信号量，常用于进程的同步。

   2）、信号量是一个特殊类型的变量，它可以被增加或减少，但对其的关键访问被保证是原子操作，即使在一个多线程程序中也是如此。

   3）、二进制信号量和计数信号量。

   4）、信号量的创建

**   #include<semaphore.h>**

**        int  sem_init(sem_t  *sem,  int  pshared,  unsigned  int  value);    **

       I、此函数初始化由sem指向的信号量对象，设置它额共享选项，并给它一个初始的整数值。pshared参数控制信号量的类型，如果其值为0，就表示这个信号量是当前进程

            到 的局部信号量，否则，这个信号量就可以在多个进程之间共享。

   5）、控制信号量函数值

**        #include<semaphore.h>**

**        int  sem_wait(sem_t  * sem);**

**        int  sem_post(sem_t  * sem);**

       I、这两个函数都以一个指针为参数，该指针指向的对象是由sem_init调用初始化的信号量。

       II、sem_post函数的作用是以原子操作的方式给信号量的值加1。

       III、sem_wait函数的作用是以原子操作的方式给信号量的值减1。

   6）、清理信号量

**     #include<semaphore.h>**

**         int  sem_destroy(sem_t  * sem);**

       I、此函数以一个信号量为参数，并清理该信号量拥有的所有资源。如果企图清理的信号量正被一些线程等待，就会收到一个错误。




 3、用互斥量进行同步

**     #include<pthread.h>**

**       int  pthread_mutex_init(pthread_mutex_t  *mutex,  const  pthread_mutexattr_t  *mutexattr);**

**       int  pthread_mutex_lock(pthread_mutex_t  *mutex);     **

**       int  pthread_mutex_unlock(pthread_mutex_t  *mutex);     **

**       int  pthread_mutex_destroy(pthread_mutex_t  *mutex);     **

     I、以上函数成功时返回0，失败时返回错误代码。







四、线程的属性

 1、初始化线程属性对象

     #include<pthread.h>

     int  pthread_attr_init(pthread_attr_t  *attr);

    I、此成功时返回0，失败时返回错误代码。




 2、属性对象的清理和回收

    #include<pthread.h>

    int  pthread_attr_destory(pthread_attr_t  *attr);




 3、属性设置函数

** #include<pthread.h>**

**    int  pthread_attr_setdetachstate(pthread_attr_t  *attr,  int  detachstate);**

**    int  pthread_attr_getdetachstate(const  pthread_attr_t  *attr,  int  *detachstate);**

**    int  pthread_attr_setschedpolicy(pthread_attr_t  *attr,  int  policy);**

**    int  pthread_attr_getschedpolicy(pthread_attr_t  *attr,  int  *policy);**

**    int  pthread_attr_setschedparam(pthread_attr_t  *attr,  const  struct  sched_param  *param);**

**    int  pthread_attr_getschedparam(const  pthread_attr_t  *attr,  struct  sched_param  *param);**

**    int  pthread_attr_setsinheritsched(pthread_attr_t  *attr,  int  inherit);**

**    int  pthread_attr_getsinheritsched(const  pthread_attr_t  *attr,  int  *inherit);**

**    int  pthread_attr_setscope(pthread_attr_t  *attr,  int  inherit);**

**    int  pthread_attr_getscope(pthread_attr_t  *attr,  int  *inherit);**

**    int  pthread_attr_setstacksize(pthread_attr_t  *attr,  int  scope);**

**    int  pthread_attr_getstacksize(const  pthread_attr_t  *attr,  int  *scope);  **

   I、detachedstate：此属性允许我们无需对线程进行重新合并。pthread_attr_setdetachstate函数可能用到的两个标志分别是PTNREAD_CREATE_JOINABLE和


        PTNREAD_CREATE_DETACHED。此属性的默认标志值是PTNREAD_CREATE_JOINABLE，所以可以允许两个线程重新合并。如果标准设置为  


        PTNREAD_CREATE_DETACHED，就不能调用pthread_join来获得另一个线程的退出状态。

   II、schedpolicy：此属性控制线程的调度方式。它的取值可以是SCHED_OTHER、SCHED_RP和SCHED_FIFO。这个属性的默认值为SCHED_OTHER。另外两种方式只能

         以超级用户权限运行的进程。SCHED_RP使用循环调用机制，而SCHED_FIFO使用先进先出策略。

   III、schedparam：此属性是和schedpolicy结合使用的，它可以对以SCHED_OTHER运行的线程的调度进行控制。

   IV、inheritsched：此属性可取两个值：PTHREAD_EXPLICIT_SCHED和PTHREAD_INHERIT_SCHED。它的默认值为是PTHREAD_EXPLICIT_SCHED，表示调度由属性

           明确的设置。如果把它设置为PTHREAD_INHERIT_SCHED，新线程将沿用其创建者所使用的参数。

   V、scope：此属性控制一个线程调度的方式。Linux只支持一种取值PTHREAD_SCOPE_SYSTEM。

  VI、stacksize：此属性控制线程创建的栈大小，单位为字节。







五、取消线程

 1、线程终止函数

**  #include<pthread.h>**

**    int  pthread_cancel(pthread_t  thread);**




 2、设置取消状态

**    #include<pthread.h>**

**    int  pthread_setcanselstate(int  state,  int  *oldstate);**

   I、state参数可以是PTHREAD_CANSEL_ENABLE，此致允许线程取消请求：或者是PTHREAD_CANSEL_DISABLE，它的作用是忽略请求。

   II、oldstate指针用于获取先前的取消状态




 3、设置取消类型

**#include<pthread.h>**

**    int  pthread_setcanseltype(int  type,  int  *oldtype);**

   I、type参数有两种取值：一个是PTHREAD_CANSEL_ASYNCHRONOUS，它将使得在接收到取消请求后立即采取行动：另一个PTHREAD_CANSEL_DEFERRED，它将使

        得在接收到取消请求后，一直等待知道线程执行了相关函数后才采取行动。

  II、oldtype参数可以保存先前的状态，如果不想知道先前额状态，可以传递NULL给它。默认情况下，线程在启动时的取消状态为PTHREAD_CANSEL_ENABLE，取消类型为

        PTHREAD_CANSEL_DEFERRED。







六、多线程（P438-442代码）




