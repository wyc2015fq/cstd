# 《精通Linux设备驱动程序开发》——内核组件 - 一世豁然的专栏 - CSDN博客





2014年10月31日 14:56:14[一世豁然](https://me.csdn.net/Explorer_day)阅读数：807








一、内核线程

 1、内核线程是一种在内核空间实现后台任务的方式。该任务可以进行频繁的异步事务处理，也可以睡眠等待某事件的发生。




 2、创建内核线程

   1）、内建的内核线程（略）

   2）、使用kernel_thread()可以创造线程

          ret = kernel_thread(mythread,  NULL,  CLONE_FS | CLONE_FILES | CLONE_SIGHAND | SIGCHID);

         I、标记参数定义了父进程与子进程之间要共享的资源。CLONE_FILES意味着打开的文件要被共享，CLONE_SIGHAND意味着信号处理函程序被共享。

   3）、实现内核线程（代码清单3-1）

         I、调用daemonize()，它会执行初始的家务工作，之后将本线程的父线程改为kthreadd。

         II、由于daemonize()在默认情况下会阻止所有的信号，因此，线程如果想处理某个信号，应该调用allow_signal()来使能它。

         III、在内核中没有信号处理函数，因此可调用signal_pending()来检查信号的存在并采取适当的行动。

         IV、注意只有在编译时配置了CONFIG_PREEMPT的情况下，内核才是可抢占的。

   4）、进程状态和等待队列

         I、等待队列

           A、等待队列用于存放需要等待时间和系统资源的线程。

           B、在被负责侦测事件的中断服务程序或另一个线程被唤醒之前，位于等待队列的线程会处于睡眠状态。

           C、入列和出列的操作分别通过调用add_wait_queue()和remove_wait_queue()完成。

           D、唤醒队列中的人物则通过wake_up_interruptible()完成。

         II、内核线程的状态

           A、处于运行状态（TASK_RUNNING）的进程位于调度器的运行队列（run  queue）中，等待调度器将CPU时间分给它执行。

           B、处于可被打断的睡眠状态（TASK_INTERRUPTIBLE）的进程正在等待一个事件的发生，不再调度器的运行队列之中。当它等待的事件发生后，或者如果它被信号打

                  断，它将重新进入运行队列。

           C、处于不可被打断的睡眠状态（TASK_INTERRUPTIBLE）的进程与处于可被打断的睡眠状态（TASK_INTERRUPTIBLE）的进程的行为相似，惟一区别是信号的发生

                  不会导致该进程被重新放入运行队列。

           D、处于停止状态（TASK_STOPPED）的进程由于收到了某些信号已经停止执行。

           E、如果一个应用程序正在使用内核的ptrace支持已拦截一个进程，该进程将处于追踪状态（TASK_TRACED）。

           F、处于僵死状态（EXIT_ZOMBIE）的进程已经被终止，但是其父进程并未等待它完成。一个推出后的进程要么处于EXIT_ZOMBIE状态，要么处于EXIT_DEAD状态。

   5）、用户模式辅助进程

         I、代码清单3-1中的mykthread会通过调用run_umode_handler()向用户空间通告被侦测到的时间。（P42代码）

         II、内核支持向用户模式的程序发出请求，让其执行某些程序这种机制。run_umode_handler()通过调用call_usermodehelper()使用了这种机制。

         III、必须通过/proc/sys/目录中的一个节点来注册run_umode_handler()要激活的用户模式程序。

         IV、对call_usermodehelper()的条用必须发生在进程上下文，而且以root权限运行。







二、辅助接口

 1、内核辅助接口：链表（list），散列链表（hlist），工作队列（work  queue）、完成函数（completion funcition）、通知块（notifier block）和kthreads。




 2、链表

   1）、为了组成数据结构的双向列表，使用include/linux/list.h文件中提供的函数。首先，需要在数据结构中嵌套一个struct_list_head:

          #include<linux/list.h>

          struct  list_head{

             struct  list_head  *nest,  *prev;

           };

           struct  mydatastructure{

              struct  list_head  mylist;

           };

           I、mylist是用于链接mydatastructure多个实例的链表。如果mydatastructure数据结构内嵌入了多个链表头，mydatastructure就可以连接到多个链表中，每个list_head用

                于其中的一个链表。

           II、可以使用链表库函数在链表中增加和删除函数。

   2）、链表库函数操作接口（P44  表3-1）

   3）、相关代码

          I、初始化数据结构（代码清单3-2）

          II、提交延后执行的工作（代码清单3-3）

          III、工作者线程（代码清单3-4）




 3、散列链表

   1）、散列链表——链表的变体。链表对链表头和结点使用了相同的结构体，但是散列链表对链表头和链表结点有不同的定义：

         struct  hlist_head{

            struct  hlist_node  *first;

         };

         struct  hlist_node{

            struct  hlist_node  *next,  **pprev;

         };

        I、为了适应单一的散列链表头指针策略散列链表结点会维护其前一个结点额地址，而不是它本身的指针。

        II、散列表的实现利用了hlist_head数组，每个hlist_head牵引着一个双向的hlist_node链表。而一个散列函数则被用来定位目标结点在hlist_head数组中的索引，此后，便

              可以使用hlist辅助函数操作与选择的索引对应的hlist_node链表。




 4、工作队列

   1）、工作队列是内核中用于进行延后工作的一种方式。

   2）、工作队列辅助库向用户呈现了两个接口结构：workqueue_struct和work_struct，使用工作队列的步骤如下。

         I、创建一个工作队列（或一个workqueue_struct），该工作队列与一个或多个内核线程关联。

           A、可以使用create_singlethread_workqueue()创建一个服务于workqueue_struct的内核线程。为了在系统中的每个CPU上创建一个工作者线程，可以使用


                create_workqueue()变体。

           B、内核中也存在默认的针对每个CPU的工作者线程（events/n，n是CPU序号），可以分时共享这个线程，而不是创建一个单独的工作者线程。

         II、创建一个工作元素（或者一个work_struct）。使用INIT_WORK可以初始化一个work_struct，填充它的工作函数的地址和函数。

         III、将工作元素提交给工作序列。

            A、可以通过queue_work()将work_struct提交给一个专用的work_struct。

            B、通过schedule_work（）提交给默认的内核工作者线程。

   3）、使用工作队列进行延后工作（代码清单3-5）。




 5、通知链

   1）、通知链可用于将状态改变信息发送给请求这些改变的代码段。

   2）、内核已经为主要事件预先定义了通知：

       I、死亡通知。当内核触发了陷阱和错误（由oops，缺页或断点命中引发）时发送死亡通知。

       II、网络设备通知。当一个网络接口卡启动或关闭的时候发出该通知。

       III、CPU频率通知。当处理器的频率发生跳变的时候，会分发这一通知。

       IV、因特网地址通知。当侦测到网络接口卡的IP地址发送改变的时候，会发送此通知。

   3）、为了将代码与某通知链关联，必须注册一个相关链的时间处理函数。当相应的事件发生时，事件ID和与通知相关的参数会传递给该处理函数。

   4）、为了实现一个自定义idea通知链，必须另外实现底层结构，以便事件被侦测到时i，链会被激活。

   5）、通知事件处理函数（代码清单3-6），通知链和它们传送的事件（表3-2）。




 6、完成接口

   1）、内核中的许多地方会激发某些单独的执行线程，之后等待它们完成。

   2）、使用完成接口进行同步（代码清单3-7）




 7、kthread辅助接口

   1）、使用kthread辅助接口辅助接口完成同步。




 8、错误处理助手

   1）、简单的示例（代码清单3-9）




