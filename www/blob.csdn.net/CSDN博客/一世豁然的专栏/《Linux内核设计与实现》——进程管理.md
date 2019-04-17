# 《Linux内核设计与实现》——进程管理 - 一世豁然的专栏 - CSDN博客





2014年10月31日 16:23:47[一世豁然](https://me.csdn.net/Explorer_day)阅读数：597








一、进程

 1、进程是正在执行的程序代码的实时结果。进程包含可执行代码、打开的文件、挂起的信号、内核内部数据、处理器状态、一个或多个具有内核映射的内存地址空间及一个或



        多个执行线程、用来存放全局变量的的数据段等。




 2、执行线程，简称线程，是在进程中活动的对象。每个线程都拥有一个独立的程序计数器、进程栈和一组进程存储器。



 3、内核的对象是线程，而不是进程。




 4、在现在操作系统中，进程提供两种虚拟机制：虚拟处理器和虚拟内存。在线程之间可以共享虚拟内存，但每个都有各自的虚拟处理器。




 5、调用**fork()**创建新进程，接着调用**exec()**这组函数就可以创建新的地址空间，并把新的程序载入其中。最终、调用**exit()**退出执行。父进程可以调用**wait4()**查询子系统是否总

       结。进程退出后被设置为僵死状态，知道它额父进程调用wait()或waitpid()为止。







二、进程描述符及任务结构

 1、内核把进程的列表存放在叫做**任务队列**的双向循环链表中。链表中的每一项都是类型为**task_struct**、称为**进程描述符**（process  descriptor）的结构，该结构定义在

        <linux/sched.h>文件中。

   1）、进程描述符中包含的数据能完整地描述一个正在执行的程序：它打开的文件、进程的地址空间、挂起的信号，进程的状态，还有其他更多的信息。（P21  图3-1）




 2、分配进程描述符

   1）、Linux通过slab分配器分配task _struct结构，此就能达到对象复用和缓存着色的目的。

   2）、由于现在用slab分配器动态生成task_struct，所以只需在栈底（对于向下增长的栈来说）或栈顶（对于向下增长的栈来说）创建一个新的结构**struct  thread_info**。

            （P22  图3-2）

   3）、在X86上，struct  thread_info在文件<asm/thread_info.h>中定义如下：

          struct  thread_info{

                      struct  task_struct  *task;

                      struct  exec_domain  *exec_domain;

                      _ _u32  flags;

                      _ _u32  status;

                      _ _u32  cpu;

                      int  preempt_count;

                      mm_segment_t  addr_limit;

                      struct  restart_block  restart_block;

                      void  *sysenter_return;

                      int  uaccess_err;

            };

 3、进程描述符的存放

   1）、内核通过唯一的进程标识值（process  identification  value）或**PID**来标识每个进程。

          I、PID是一个数，表示为pid_t隐含类型，实际上就是int类型。

          II、PID的最大值默认设置为32768。此最大值也是系统中允许同时存在的进程的最大数目。系统管理员可修改**/proc/sys/kernel/pid_max**来提高上限。

   2）、在内核中，访问任务通常需要获得指向其task_struct的指针。实际上，内核中大部分处理进程的代码都是直接通过task_struct进行的。因此，通过current宏查找当前正

             在运行进程的进程描述符的熟读就非常重要。

          I、硬件体系结构不同，宏current的实现也不同，它必须针对专门的硬件体系结构做处理。

          II、有的体系结构用专门寄存器来存放指向当前进程task_struct的指针。但有些像X86的体系结构，只能在内核栈的尾端创建thread_info结构，通过计算偏移间接地查找


               task_struct结构。

          III、X86系统上，current把栈指针的后13位有效位屏蔽掉，用来计算thread_info的偏移。该操作是通过cyrrent_thread_info()函数来完成的。




 4、进程状态

   1）、进程描述符中的state域描述了进程的当前状态。系统中的每个进程都必然处于以下五种状态中的一种：

         I、**TSAK_RUNNING**（运行）——进程是可执行的；它或者正在执行，或者在运行队列中等待执行。

         II、**TASK_INTERRUPTIBLE**（可中断）——进程正在睡眠（也就是说它被堵塞），等待某些条件的达成。一旦条件达成，内核就会把进程状态设置为运行。

         III、**TASK_UNINTERRUPTIBLE**（不可中断）——除了就算是接收到信号也不会被唤醒或准备投入运行外，这个状态与可打断状态相同。

         IV、**_TASK_TRACED**——被其他进程跟踪的进程。

         V、**_TASK_STOPPED**（停止）——进程停止运行；进程没有投入运行也不能投入运行。通常，此状态发生在SIGSTOP、SIGTSTP、SIGTTIN、SIGTTOU等信号的时候。

              此外，在调试期间接收到任何信号，都会使进程进入该状态。

   2）、进程状态的转化（P24  图3-3）




 5、设置当前进程状态

   1）、内核经常需要调整某个进程的状态，此时最好使用**set_task_state(task_state)**函数。

         I、该函数将指定的进程设置为指定的状态。

         II、必要的时候，它会设置内存屏障来强制其他处理器作重新排序。否则，它等价于：

             task->state = state;

   2）、**set_task_state(task_state)**和**set_current_state(task_state)**含义是等同的。




 6、进程上下文

   1）、当一个程序调执行了系统调用或者触发了某个异常，它就陷入了内核空间。此时，称内核“代表进程执行”并处于进程上下文中。在此上下文中current宏是有效的。




 7、进程家族树

   1）、系统中的每个进程必有一个父进程，相应的，每个进程也可以拥有零个或多个子进程。拥有同一个父进程的所有进程被称为兄弟。

   2）、进程间的关系存放在进程描述符中。每个task_struct都包含一个指向其父进程task_struct、叫做parent的指针，还包含一个称为choldren的子进程链表。

   3）、相关代码（P25-P26）

        I、获得父进程的进程描述符

        struct  task_struct  *my_parent = current->parent;

        II、一次访问子进程

        struct  task_struct  *task;

        struct list_head  *list;

        list_for_each(list,  &curent->children){

                       task = list_entry(list,  struct  task_struct,  sibing);

                       /*task现在指向当前的某个子进程*/

       ];

       III、init进程描述符的静态分配

       struct  task_struct  *task;

       for（task = current;  task  !=  &init_task;  task  =  task->parent）; /*task现在指向init*/

       IV、对于指定的进程获取链表中的下一个进程：

       list_entry(task->tasks.next,  struct  task_struct,  tasks);

       V、对于指定的进程获取链表中的前一个进程：

       list_entry(task->tasks.prev,  struct  task_struct,  tasks);

       VI、以次访问整个任务队列


       struct  task_struct  *task;

       for_each_process(task){

                          /*打印每一个任务的名称和PID*/

                          printk("%s[%d]\n",  task->comm,  task->pid);

       }







三、进程创建

 1、UNIX的进程创建：

   1）、首先，fork()通过拷贝当前进程创建一个子程序。子进程和父进程的区别仅仅在于OID、PPID和某些资源和统计量。

   2）、然后exec()函数负责读取可执行文件并将其载入地址空间开始执行。




 2、写时拷贝

   1）、Linux的fork()使用写时拷贝页实现。写时拷贝是一种可以推迟甚至免除拷贝数据的技术。内核此时并不复制整个进程地址空间，而是让父进程和子进程共享同一个拷贝。

             只有在需要写入的时候，数据才会被复制，从而使各个进程拥有各自的拷贝。


   2）、fork()的实际开销就是复制父进程的页表以及给子进程创建惟一的进程描述符。




 3、fork()

   1）、Linux通过clone()系统调用实现fork()。

   2）、fork()、vfork()和_clone()库函数都根据各自需要的参数标志去调用clone()，然后由clone()去调用do_fork()。该函数调用copy_process()函数，然后让进程开始运行。




 4、vfork()

   1）、除了不考贝父进程的页表项外，vfork()系统调用和fork()的功能相同。







四、线程在Linux中的实现

 1、Linux把所有的线程都当作进程来实现。线程仅仅被视为一个与其它进程共享某些资源的进程。每个线程都拥有惟一隶属于自己的task_struct。




 2、创建线程

   1）、线程用clone()创建：

         clone(CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHEAD,  0);

        I、传递给clone()的参数标志决定了新创建进程的行为和父子进程之间共享资源的种类。（P29  表3-1）




 3、内核线程

   1）、内核经常需要在后台执行一些操作。次通过内核线程完成——独立运行在内核空间的标准进程。

         I、内核线程与普通的进程间区别在于内核线程没有独立的地址空间。

         II、内核线程只运行于内核空间，从来不切换到用户空间去。

         III、内核线程可以被调度，也可以被抢占。

   2）、内核线程在<linux/kthread.h>中声明有接口。从现有内核线程中创建一个新的内核线程的方法如下：

         struct  task_struct  *kthread_create(int  (*threadfn) (void  *data),

                                                                         void  *data,

                                                                          const  char  namefmt[],

                                                                          .  .  .)

          I、新的任务是由kthread内核进程通过clone()系统调用而创建的。

          II、新的进程将运行threadfn函数，给其传递的参数为data。

          III、进程会命名为namefmt，namrfmt接受可变参数列表类似于printf的格式化参数。

          IV、新创建的进程处于不可运行的状态，如果不通过wake_up_process()明确唤醒它，它不会主动执行。

   3）、创建一个进程并让它运行起来，可以通过调用kthread_run()来达到：

           struct  task_struct  *thread_run(int  (*threadfn) (void  *data),

                                                                         void  *data,

                                                                          const  char  namefmt[],

                                                                          .  .  .)；

   4）、内核线程启动后就一直运行直到调用do_exit()退出，或者内核额其他部分调用kthread_stop()退出，传递给kthread_stop()的参数为kthread_create函数返回的


             task_struct结构的地址：

             int  kthread_stop(struct  task_struct  *k);







五、进程终结


 1、一般来说，进程的析构是自身引起的。

    I、它发生在进程调用exit()系统调用时，即可能显式地调用这个系统调用，也可能隐式地从某个程序的主函数返回。

    II、不管是怎么终结的，大该任务部分都要靠do_exit()完成。




 2、删除进程描述符

   1）、调用do_exit()之后，尽管进程不再运行，但系统还保留了它的进程描述符。这样做可以让系统有办法在子进程结束后仍能获得它的信号。

   2）、进程终结时所需的清理工作和进程描述符的删除被分开执行。在父进程获得子进程的信息后，或者通知内核它并不关注哪些信息后，子进程的task_struct的结构才被释

             放。

   3）、释放进程描述符的过程。




 3、孤儿进程造成的进退维谷

   1）、如果父进程在子进程之前推出，必须有机制来保护子进程找到一个新的父亲，否则这些称为孤儿的进程就会在退出时永远处于僵死状态，白白耗费内存。

   2）、相关代码。







