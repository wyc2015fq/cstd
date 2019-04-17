# 《Linux内核设计与实现》——下半部和推后执行的工作 - 一世豁然的专栏 - CSDN博客





2014年11月12日 21:11:36[一世豁然](https://me.csdn.net/Explorer_day)阅读数：593








一、下半部

 一）、相关简介

  1、下半部的任务就是执行与中断处理密切相关但中断处理程序本身不执行的工作。




  2、对于在上半部和下半部之间划分工作，尽管不存在某种严格的规则，但还是有一些易事可供借鉴：

    1）、如果一个任务对时间非常敏感，将其放在中断处理程序中执行。

    2）、如果一个任务和硬件相关，将其放在中断处理程序中执行。

    3）、如果一个任务要保留不被其他中断打断，将其放在中断处理程序中执行。

    4）、其他所欲任务，考虑放置在下半部执行。







 二）、使用下半部的原因

  1、下半部并不需要指明一个确切的时间，只要把这些人物推迟一点，让它们在系统不太繁忙并且中断回复后执行就可以。




  2、通常下半部在中断处理程序一返回就会马上执行。下半部执行的关键在于当它们运行的时候，允许相应所有的中断。







三）、下半部的环境

  1、和上半部只能通过中断处理程序实现不同，下半部可以通过多种机制实现。这些用来实现下半部的机制分别由不同的接口和子系统组成。




  2、下半部的起源（BH）




  3、任务队列




  4、软中断和tasklet




  5、下半部状态（P110  表8-1）










二、软中断

 一）、软中断的实现

  1、软中断是在编译期间静态分配的。它不像tasklet那样能被动态地注册或注销。软中断由softirq_action结构表示：

      struct  softirq_action {

          void  (*action)(struct  softirq_action  *);

      };




  2、软中断处理程序

    1）、软中断处理程序原型：

        void  softirq_handler(struct  softirq_action  *);

       I、当内核运行一个软中断处理程序的时候，它就会执行这个action函数，其唯一的参数为指向相应softirq_action结构体的指针。

    2）、一个软中断不会抢占另外一个软中断。实际上，唯一可以抢占软中断的是中断处理程序。不过，其他的软中断可以在其他处理器上同时执行。




  3、执行软中断

    1）、一个注册的软中断必须在被标记之后才会执行。这被称作触发软中断。

    2）、在下列地方，待处理的软中断会被执行

       I、从一个软件中断代码处返回时。

       II、在ksoftirqd内核线程中。

       III、在那些显式检查和执行待处理的软中断的代码中。







 二）、使用软中断

  1、分配索引

    1）、在编译期间，通过在<linux/interrupt.h>中定义的一个枚举类型来静态声明软中断。内核用这些从0开始的索引来表示一种相对优先级。索引小的软中断在索引号大的软

              中断之前执行。


    2）、建立一个新的软中断必须在此举类型中加入新的项。而加入时，必须根据希望赋予它们的优先级来决定加入的位置。

    3）、tasklet类型表（P113  表8-2）




  2、注册处理函数

    1）、在运行通过调用open_softirq()注册软中断处理程序，该函数有两个参数：软中断的索引号和处理函数。

    2）、软中断处理程序的时候，允许相应中断，但它自己不能休眠。




  3、触发你的软中断

    1）、通过在枚举类型的列表中添加新项以及调用open_softirq进行注册以后，新的软中断处理程序就能够运行。

    2）、saise_softirq()函数可以将一个软中断设备为挂起状态，让它在下次调用do__softirq()函数以运行。










三、tasklet

 一）、tasklet的实现

  1、tasklet结构体

    1）、tasklet由tasklet_struct结构表示。每个结构体单独代表一个tasklet。

        struct  tasklet_struct {

           struct  tasklet_struct  *next;

           unisgned  long  state;

           atomic_t  count;

           void  (*func)(unsigned  long);

           unsigned  long  data;

        };




  2、调度tasklet

    1）、已调度的tasklet存放在两个单处理器数据结构：tasklet(普通tasklet)和tasklet_hi_vcc（高优先级tasklet）。这两个数据结构都是由tasklet_struct结构体构成的链表。联

              表中的每个tasklet_struct代表一个不同的tasklet。

    2）、tasklet由tasklet_schedule()和tasklet_hi_schedule()函数进行调度，它们接受一个指向tasklet_struct结构的指针作为参数。







 二）、使用tasklet

  1、声明tasklet

    1）、静态声明一个tasklet：

        DECLARE_TASKLET(name,  fhunc,  data);

        DECLARE_TASKLET_DISABLED(name,  fhunc,  data);

    2）、动态声明一个tasklet：

       tasklet_init(name,  fhunc,  data);




  2、编写自己的tasklet处理程序

    1）、tasklet处理程序必须符号规定的参数类型：

       void  tasklet_handler(unsigned  long  data);

    2）、因为靠软中断实现，所以tasklet不能睡眠。这意味着不能再tasklet中使用信号量或者其他什么堵塞式的函数。




  3、调度自己的tasklet

    1）、通过调用tasklet_chedule()函数并传递给它相应的tasklet_struct指针，该tasklet就会被调用：

        tasklet_schedule(&my_tasklet);

    2）、可以调用tasklet_disable()和tasklet_disable_nosync()函数禁止某个指定的tasklet。

    3）、可以调用tasklet_enable()函数来激活一个tasklet。

    4）、可以调用tasklet_kill()函数从挂起的队列中去掉一个tasklet。




  4、ksoftirqd




  5、老的BH机制










四、工作队列

 一）、相关简介

  1、工作队列是另外一种将工作推后执行的形式。它可以把工作推后，交由一个内核线程去执行——这个下半部分总是会在进程上下文中执行。




  2、通常，在工作队列或软中断/tasklet中做出选择非常容易。入伙推后执行的任务需要睡眠，就选择工作队列。如果推后执行的任务不需要睡眠，那么就选择软中断或

        tasklet。







 二）、工作队列的实现

  1、工作队列子系统是一个用于创建内核线程的接口，通过它创建的进程负责执行由内核其他部分排到队列里的任务。它创造的这些线程称作工作者线程。



  2、工作队列可以让你的驱动程序创建一个专门的工作者线程处理需要推后的工作。不过，工作队列子系统提供了一个缺省的工作者线程来处理这些工作。缺省的工作者线程叫

         做events/n，这里的n是处理器的编号；每个处理器对应一个线程。




  3、表示线程的数据结构

    1）、工作者线程用workqueue_struct结构表示：

       struct  workqueue_struct {

          struct  cpu_workqueue_struct  cpu_wq[NR_CPUS];

          struct  list_head  list;

          const  char  *name;

          int  sinqlethread;

          int  frreezeable;

          int  rt;

       };

    2）、cpu_workqueue_struct结构

      struct  cpu_workqueue_struct {

         spinlock_t  lock;

         struct  list_head  worklist;

         wait_queue_head_t  more_work;

         struct  work_struct*current_struct;

         struct  workqueue_struct  *wq;

          task_t  *thread;

       };




  4、表示工作的数据结构

    1）、所有的工作者线程都是用普通的内核线程实现的，它们都要执行worker_thread()函数。在它初始化完之后，这恶鬼函数执行一个死循环并开始休眠。当有操作被插入到

               队列里的时候，线程就会被唤醒，以便执行这些操作。当没有剩余操作的时候，它又会继续休眠。

    2）、工作用<linux/workqueue.h>中定义的work_struct结构体表示：

       struct  work_queue {

          atomic_long_t  data;

          struct  list_head  entry;

          work_func_t  func;

       };




  5、工作队列实现机制的总结（P123  图8-1）







 三）、使用工作队列

  1、创建推后的工作

    1）静态创建：

        DECLARE_WORK(name,  void(*func)(void  *),  void  *data);

    2）、动态创建

       INIT_WORK(struct  work_struct  *work,  void(*func)(void  *),  void  *data);




  2、工作队列处理函数

    1）、工作队列处理函数的原型是：

       void  work_handler(void  *data);




  3、对工作进行调度

    1）、想要把给定工作的处理函数提交给缺省的events工作线程，只需调用：

       schedule_work(&work);

    2）、不希望工作被马上执行，而是希望它经过一段延时之后才会执行，可调用：

       schedule_delayed_work(&work,  delay);




  4、刷新操作

    1）、刷新指定工作队列的函数

       void  flush_schedule_work(void);

    2）、取消延迟执行的工作：

       int  cancel_delayed_work(struct  work_struct  *work);




  5、创建新的工作队列

    1）、创建一个新的任务队列和之相关的工作者线程：

       struct  workqueue_struct  *create_workqueue(const  char  *name);

    2）、想要把给定工作的处理函数提交给针对的工作队列以及被延迟的特定工作队列调用，只需调用：

        int  queue_work(struct  workqueue_struct  *wq,  struct  wrok_struct  *work);

        int  queue_work(struct  workqueue_struct  *wq,  struct  wrok_struct  *work,  unisgned  long  delay);

    3）、刷新指定的工作队列：

       flush_workqueue(struct  workqueue_struct  *wq);




  5、老的任务队列机制










五、下半部机制的选择

  1、如果被考察的代码本身多线索化的工作就做的非常好，那么软中断就是非常好的选择。




  2、如果代码多线索化考虑的并不充分，那么选择tasklet意义更大。




  3、如果准备使用每一处理器上的变量或者类似的情形，以确保软中断能安全的在多个处理器上并发的运行，那么软中断就是非常好的选择。




  4、如果需要把任务推后到进程上下文中完成，那么在这三者中就只能选择工作队列。如果进程上下文并不是必须的条件，那么软中断和tasklet可能更适合。




  5、三种下半部接口的比较（P127  图8-3）










六、在下半部之间加锁










七、禁止下半部

  1、一般单纯禁止下半部的处理是不够的。为了保证共享数据的安全，更常见的做法是，先得到一个锁然后禁止下半部的处理，驱动程序中通常使用的都是这种方法。




  2、如果需要禁止所有的下半部，可以调用local_bh_disable(0函数。允许下半部进行处理，可以调用local_bh_enable()函数。




