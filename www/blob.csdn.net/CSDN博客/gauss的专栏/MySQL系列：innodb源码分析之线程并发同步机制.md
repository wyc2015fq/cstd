# MySQL系列：innodb源码分析之线程并发同步机制 - gauss的专栏 - CSDN博客
2015年05月10日 09:46:24[gauss](https://me.csdn.net/mathlmx)阅读数：318
个人分类：[数据库](https://blog.csdn.net/mathlmx/article/category/2641909)
转自:http://blog.csdn.net/column/details/innodb-zerok.html
innodb是一个多线程并发的存储引擎，内部的读写都是用多线程来实现的，所以innodb内部实现了一个比较高效的并发同步机制。innodb并没有直接使用系统提供的锁(latch)同步结构，而是对其进行自己的封装和实现优化，但是也兼容系统的锁。我们先看一段innodb内部的注释（MySQL-3.23）:
Semaphore operations in operating systems are slow: Solaris on a 1993 Sparc takes 3 microseconds (us) for a lock-unlock pair and Windows NT on a 1995 Pentium takes 20 microseconds for a lock-unlock pair. Therefore, we have toimplement
 our own efficient spin lock mutex. Future operating systems mayprovide efficient spin locks, but we cannot count on that.
大概意思是说1995年的时候，一个Windows NT的 lock-unlock所需要耗费20us,即使是在Solaris 下也需要3us,这也就是他为什么要实现自定义latch的目的,在innodb中作者实现了系统latch的封装、自定义mutex和自定义rw_lock。下面我们来一一做分析。
# 1 系统的mutex和event
    在innodb引擎当中，封装了操作系统提供的基本mutex（互斥量）和event（信号量），在WINDOWS下的实现暂时不做记录，主要还是对支持POSIX系统来做介绍。在POSIX系统的实现是os_fast_mutex_t和os_event_t。os_fast_mutex_t相对简单，其实就是pthread_mutex。定义如下：
**[cpp]**[view plain](http://blog.csdn.net/yuanrxdu/article/details/41170381#)[copy](http://blog.csdn.net/yuanrxdu/article/details/41170381#)
- typedef pthread_mutex os_fast_mutex_t;  
而os_event_t相对复杂，它是通过os_fast_mutex_t和一个pthread_cond_t来实现的，定义如下：
**[cpp]**[view plain](http://blog.csdn.net/yuanrxdu/article/details/41170381#)[copy](http://blog.csdn.net/yuanrxdu/article/details/41170381#)
- typedefstruct os_event_struct  
-     {  
-         os_fast_mutex_t        os_mutex;  
-         ibool                  is_set;  
-         pthread_cond_t         cond_var;  
-     }os_event_t;  
以下是os_event_t的两线程信号控制的例子流程：
![](https://img-blog.csdn.net/20141116101151946?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveXVhbnJ4ZHU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
对于系统的封装，最主要的就是os_event_t接口的封装，而在os_event_t的封装中，os_event_set、os_event_reset、os_event_wait这三
个方法是最关键的。
# 2 CPU原子操作
在innodb的mutex(互斥量)的实现中，除了引用系统的os_mutex_t以外，还使用了原子操作来进行封装一个高效的mutex实现。在
系统支持原子操作的情况下，会采用自己封装的mutex来做互斥，如果不支持，就使用os_mutex_t。在gcc 4.1.2之前，编译器是
不提供原子操作的API的，所以在MySQL-.3.23的innodb中自己实现了一个类似__sync_lock_test_and_set的实现，代码是采用
了汇编实现：
**[cpp]**[view plain](http://blog.csdn.net/yuanrxdu/article/details/41170381#)[copy](http://blog.csdn.net/yuanrxdu/article/details/41170381#)
- asm volatile("movl $1, %%eax; xchgl (%%ecx), %%eax" :  
- "=eax" (res), "=m" (*lw) :  
- "ecx" (lw));  
这段代码是什么意思呢?其实就是将lw的值设置成1,并且返回设置lw之前的值(res),这个过程都是CPU需要回写内存的，也就是CPU和内存是完全一致的。除了上面设置1以外，还有一个复位的实现，如下：
**[cpp]**[view plain](http://blog.csdn.net/yuanrxdu/article/details/41170381#)[copy](http://blog.csdn.net/yuanrxdu/article/details/41170381#)
- asm volatile("movl $0, %%eax; xchgl (%%ecx), %%eax" :  
- "=m" (*lw) :   "ecx" (lw) :  "eax");   
这两个函数交叉起来使用，就是gcc-4.1.2以后的__sync_lock_test_and_set的基本实现了。在MySQL-5.6的Innodb引擎当中，将以上汇编代码采用了__sync_lock_test_and_set代替，我们可以采用原子操作实现一个简单的mutex.
**[cpp]**[view plain](http://blog.csdn.net/yuanrxdu/article/details/41170381#)[copy](http://blog.csdn.net/yuanrxdu/article/details/41170381#)
- #define LOCK() while(__sync_lock_test_and_set(&lock, 1)){}
- #define UNLOCK() __sync_lock_release(&lock)
以上就是一个基本的无锁结构的mutex，在linux下测试确实比pthread_mutex效率要高出不少。当然在innodb之中的mutex实现不会仅仅这么简单，需要考虑的因素还是比较多的，例如：同线程多次lock、lock自旋的周期、死锁检测等。
# 3 mutex的实现
在innodb中，带有原子操作的mutex自定义互斥量是基础的并发和同步的机制，目的是为了减少CPU的上下文切换和提供高效率，一般mutex等待的时间不超过100微秒的条件下，这种mutex效率是非常高的。如果等待的时间长，建议选择os_mutex方式。虽然自定义mutex在自旋时间超过自旋阈值会进入信号等待状态，但是整个过程相对os_mutex来说，效率太低，这不是自定义mutex的目的。自定义mutex的定义如下：
**[cpp]**[view plain](http://blog.csdn.net/yuanrxdu/article/details/41170381#)[copy](http://blog.csdn.net/yuanrxdu/article/details/41170381#)
- struct mutex_struct  
- {  
-  ulint   lock_word;                             /*mutex原子控制变量*/
-  os_fast_mutex_t     os_fast_mutex;     /*在编译器或者系统部支持原子操作的时候采用的系统os_mutex来替代mutex*/
-  ulint   waiters;                                  /*是否有线程在等待锁*/
-  UT_LIST_NODE_T(mutex_t)    list;     /*mutex list node*/
-  os_thread_id_t  thread_id;              /*获得mutex的线程ID*/
- char*   file_name;                            /*mutex lock操作的文件/
-  ulint   line;                                       /*mutex lock操作的文件的行数*/
-  ulint   level;                                     /*锁层ID*/
- char*   cfile_name;                          /*mute创建的文件*/
-  ulint   cline;                                     /*mutex创建的文件行数*/
-  ulint   magic_n;                              /*魔法字*/
- };  
在自定义mute_t的接口方法中，最核心的两个方法是:mutex_enter_func和mutex_exit方法
    mutex_enter_func                    获得mutex锁，如果mutex被其他线程占用，先会自旋SYNC_SPIN_ROUNDS,然后
                                                         再等待占用锁的线程的信号
    mutex_exit                                 释放mutex锁，并向等待线程发送可以抢占mutex的信号量
## 3.1 mutex_enter_func流程图：
![](https://img-blog.csdn.net/20141116102844234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveXVhbnJ4ZHU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
以上流程主要是在mutex_spin_wait这个函数中实现的，从其代码中可以看出，这个函数是尽力让线程在自旋周期内获得锁，因为一旦进入cell_wait状态，至少的耗费1 ~ 2次系统调用，在cell_add的时候有可能触发os_mutex_t的锁等待和一定会event_wait等待。这比系统os_mutex效率会低得多。如果在调试状态下，获得锁的同时会向thread_levels的添加一条正在使用锁的信息，以便死锁检查和调试。
## 3.2 mutex_exit流程图
![](https://img-blog.csdn.net/20141116102959424?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveXVhbnJ4ZHU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 3.4 mutex_t的内存结构关系图
![](https://img-blog.csdn.net/20141116103157922?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveXVhbnJ4ZHU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3.4mutex获得锁和释放锁的示意图
![](https://img-blog.csdn.net/20141116103343690?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveXVhbnJ4ZHU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 4 rw_lock的实现
innodb为了提高读的性能，自定义了read write lock，也就是读写锁。其设计原则是：
    1、同一时刻允许多个线程同时读取内存中的变量
    2、同一时刻只允许一个线程更改内存中的变量
    3、同一时刻当有线程在读取变量时不允许任何线程写存在
    4、同一时刻当有线程在更改变量时不允许任何线程读，也不允许出自己以外的线程写（线程内可以递归占有锁）。
    5、当有rw_lock处于线程读模式下是有线程写等待，这时候如果再有其他线程读请求锁的时，这个读请求将处于等待前面写完成。
从上面5点我们可以看出，rw_lock在被占用是会处于读状态和写状态，我们称之为S-latch(读共享)和X-latch（写独占），《MySQL技术内幕：innodb引擎》对S-latch和X_latch的描述如下：
||S-latch|X-latch|
|----|----|----|
|S-latch|兼容|不兼容|
|X-latch|不兼容|不兼容|
innodb中的rw_lock是在建立在自定义mutex_t之上的，所有的控制是基于mutex和thread_cell的。
以下是rw_lock_t的结构定义：
**[cpp]**[view plain](http://blog.csdn.net/yuanrxdu/article/details/41170381#)[copy](http://blog.csdn.net/yuanrxdu/article/details/41170381#)
- struct rw_lock_struct  
- {  
-  ulint   reader_count;                         /*获得S-LATCH的读者个数,一旦不为0，表示是S-LATCH锁*/
-  ulint   writer;                                     /*获得X-LATCH的状态，主要有RW_LOCK_EX、RW_LOCK_WAIT_EX、                               
-                                                             RW_LOCK_NOT_LOCKED， 处于RW_LOCK_EX表示是一个x-latch
-                                                             锁,RW_LOCK_WAIT_EX的状态表示是一个S-LATCH锁*/
-  os_thread_id_t  writer_thread;        /*获得X-LATCH的线程ID或者第一个等待成为x-latch的线程ID*/
-  ulint   writer_count;                         /*同一线程中X-latch lock次数*/
-  mutex_t     mutex;                             /*保护rw_lock结构中数据的互斥量*/
-  ulint   pass;                                      /*默认为0，如果是非0，表示线程可以将latch控制权转移给其他线程，
-                                                             在insert buffer有相关的调用*/
-  ulint   waiters;                                 /*有读或者写在等待获得latch*/
-  ibool   writer_is_wait_ex;  
- 
-  UT_LIST_NODE_T(rw_lock_t) list;  
-  UT_LIST_BASE_NODE_T(rw_lock_debug_t) debug_list;  
- 
-  ulint   level;                                     /*level标示，用于检测死锁*/
- 
- /*用于调试的信息*/
- char*   cfile_name;                          /*rw_lock创建时的文件*/
-  ulint   cline;                                     /*rw_lock创建是的文件行位置*/
- char*   last_s_file_name;                 /*最后获得S-latch时的文件*/
- char*   last_x_file_name;                 /*最后获得X-latch时的文件*/
-  ulint   last_s_line;                            /*最后获得S-latch时的文件行位置*/
-  ulint   last_x_line;                           /*最后获得X-latch时的文件行位置*/
-  ulint   magic_n;                              /*魔法字*/
- };  
 在rw_lock_t获得锁和释放锁的主要接口是:rw_lock_s_lock_func、rw_lock_x_lock_func、rw_lock_s_unlock_func、rw_lock_x_unlock_func四个关键函数。 其中rw_lock_s_lock_func和rw_lock_x_lock_func中定义了自旋函数，这两个自旋函数的流程和mutex_t中的自旋函数实现流程是相似的，其目的是要在自旋期间就完成锁的获得。具体细节可以查看sync0rw.c中的rw_lock_s_lock_spin/rw_lock_x_lock_func的代码实现。从上面结构的定义和函数的实现可以知道rw_lock有四种状态：
  RW_LOCK_NOT_LOCKED                    空闲状态
  RW_LOCK_SHARED                             处于多线程并发都状态
  RW_LOCK_WAIT_EX                            等待从S-latch成为X-latch状态
  RW_LOCK_EX                                       处于单线程写状态
以下是这四种状态迁移示意图：
![](https://img-blog.csdn.net/20141116104127562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveXVhbnJ4ZHU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
通过上面的迁徙示意图我们可以很清楚的了解rw_lock的运作机理，除了状态处理以外，rw_lock还为debug提供了接口，我们可以通过内存关系图来了解他们的关系：
![](https://img-blog.csdn.net/20141116104307787?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveXVhbnJ4ZHU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 5 死锁检测与调试
  innodb除了实现自定义mutex_t和rw_lock_t以外，还对这两个类型的latch做了调试性死锁检测，
这大大简化了innodb的latch调试，latch的状态和信息在可以实时查看到，但这仅仅是在innodb的调试
版本中才能看到。与死锁检测相关的模块主要是mutex level、rw_lock level和sync_cell。latch level相关的定义：
**[cpp]**[view plain](http://blog.csdn.net/yuanrxdu/article/details/41170381#)[copy](http://blog.csdn.net/yuanrxdu/article/details/41170381#)
- /*sync_thread_t*/
- struct sync_thread_struct  
-     {  
-          os_thread_id_t id;            /*占用latch的thread的id*/
-          sync_level_t*  levels;         /*latch的信息，sync_level_t结构内容*/
-      };  
- 
- /*sync_level_t*/
- struct sync_level_struct  
-     {  
- void*  latch;                    /*latch句柄，是mute_t或者rw_lock_t的结构指针*/
-          ulint  level;                     /*latch的level标识ID*/
-     };  
在latch获得的时候，innodb会调用mutex_set_debug_info函数向sync_thread_t中加入一个latch被获得的状态信息，其实就是包括获得latch的线程id、获得latch的文件位置和latch的层标识（具体的细节可以查看mutex_enter_func和mutex_spin_wait）。只有占用了latch才会体现在sync_thread_t中，如果只是在等待获得latch是不会加入到sync_thread_t当中的。innodb可以通过sync_thread_levels_empty_gen函数来输出所有latch等待依赖的cell_t序列，追踪线程等待的位置。
## 5.1sync_thread_t与sync_level_t的内存结构关系:
![](https://img-blog.csdn.net/20141116104540855?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveXVhbnJ4ZHU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
sync_thread_level_arrays的长度是OS_THREAD_MAX_N（linux下默认是10000），也就是和最大线程个数是一样的。
levels的长度是SYNC_THREAD_N_LEVELS（10000）。
## 5.2死锁与死锁检测
什么是死锁，通过以下的例子我们可以做个简单的描述：
    线程A                                        
线程B
    mutex1    enter                mutex2        enter
    mutex2    enter                mutex1        enter
    执行任务                           执行任务
    mutex2    release             mutex1          release
    mutex1    release            mutex2           release
   上面两个线程同时运行的时候，可能产生死锁的情况，就是A线程获得了mutex1正在等待mutex2的锁，同时线程2获得了mutex2正在等待mutex1的锁。在这种情况下，线程1在等线程2，线程2在等线程就造成了死锁。
  了解了死锁的概念后，我们就可以开始分析innodb中关于死锁检测的流程细节，innodb的检车死锁的实质就是判断
要进行锁的latch是否会产生所有线程的闭环，这个是通过sync_array_cell_t的内容来判断的。在开始等待cell信号的时候，
会判断将自己的状态信息放入sync_array_cell_t当中，在进入os event wait之前会调用sync_array_detect_deadlock来判
断是否死锁，如果死锁，会触发一个异常。死锁检测的关键在与sync_array_detect_deadlock函数。
以下是检测死锁的流程描述：
    1、将进入等待的latch对应的cell作为参数传入到sync_array_detect_deadlock当中，其中start的参数和依赖的cell参
数填写的都是这个cell自己。
    2、进入sync_array_detect_deadlock先判断依赖的cell是否正在等待latch,如果没有，表示没有死锁，直接返回.
如果有，先判断等待的锁被哪个线程占用，并获得占用线程的id,通过占用线程的id和全局的sync_array_t  等待cell数组状
态信息调用sync_array_deadlock_step来判断等待线程的锁依赖。
    3、进入sync_array_deadlock_step先找到占用线程的对应cell,如果cell和最初的需要event wait的cell是同一
个cell,表示是一个闭环，将产生死锁。如果没有，继续将查询到的cell作为参数递归调用
sync_array_detect_deadlock执行第2步。这是个两函数交叉递归判断的过程。
在检测死锁过程latch句柄、thread id、cell句柄三者之间环环相扣和递归，通过latch的本身的状态来判断闭环死锁。在上面的第2步会根据latch是mutex和rw_lock的区别做区分判断，这是由于mutex和rw_lock的运作机制不同造成的。因为关系数据库的latch使用非常频繁和复杂，检查死锁对于锁的调试是非常有效的，尤其是配合thread_levels状态信息输出来做调试，对死锁排查是非常有意义的。
死锁示意图:
![](https://img-blog.csdn.net/20141116105330343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveXVhbnJ4ZHU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 6.总结
通过上面的分析可以知道innodb除了实现对操作系统提供的latch结构封装意外，还提供了原子操作级别的自定义latch，那么它为什么要实现自定义latch呢?我个人理解主要是减少操作系统上下文的切换，提高并发的效率。innodb中实现的自定义latch只适合短时间的锁等待（最好不超过50us）,如果是长时间锁等待，最好还是使用操作系统提供的，虽然自定义锁在等待一个自旋周期会进入操作系统的event_wait，但这无疑比系统的mutex
 lock耗费的资源多。最后我们还是看作者在代码中的总结：
We conclude that the best choice is to set the spin time at 20 us. Then the system should work well on a multiprocessor. On a uniprocessor we have to make sure that thread swithches due to mutex collisions are not frequent, i.e.,
 they do not happen every 100 us or so, because that wastes too much resources. If the thread switches are not frequent, the 20 us wasted in spin loop is not too much.
