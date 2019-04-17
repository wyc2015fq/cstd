# 《Linux程序设计》——进程和信号 - 一世豁然的专栏 - CSDN博客





2014年10月12日 18:36:41[一世豁然](https://me.csdn.net/Explorer_day)阅读数：740








一、进程

 1、进程的定义：一个其中运行着一个或多个线程的地址空间和这些线程所需要的系统资源。




 2、进程的结构

   1）、进程标识符及取值——每个进程都会被分配一个惟一的数字编号，称之为进程标识符或PID，它通常是一个取值范围从2到32768的正整数。

   2）、系统函数库和Linux进程可以被共享。

   3）、进程有自己的栈空间，用于保存函数中的局部变量和控制函数的调用与返回。

   4）、进程必须维护自己的程序计数器，这个计数器用来记录它执行到的位置，即在执行线程中的位置。




 3、进程表——Linux进程表就像一个数据结构，它把当前加载在内存中的所有进程的有关信息保存在一个表中，其中包括进程的PID、进程的状态、命令字符串和其他一些ps命

     令 输出的各类信息。




 4、查看进程

   1）、ps命令可以显示我们正在运行的进程、其他用户正在运行的进程或者目前在系统上运行的所有进程。默认情况下，ps程序只显示与终端、主控台、串行口或伪终端保持

          连接的进程的信息。

       I、ps -a（查看所有进程）

      II、ps -f（显示进程的完整信息）





 5、系统进程

   1）、ps输出的STAT一列用来表明进程的当前状态。（P391  表11-1）

   2）、一般而言，每个进程都是由另一个父进程的进程启动的，被父进程启动的进程叫做子进程。




 6、进程的调度

   1）、每个进程都有一个运行时间，称之为时间片。

   2）、可以用nice命令设置进程的nice值，使用renice调整它的值。







二、启动新进程

 1、启动新进程

   1）、system函数

** #include<stdlib.h>**

**         int  system(const  char  *string);**

        I、此函数的作用是：运行以字符串参数的形式传递给它的命令并等待该命令的完成。

       II、如果无法启动shell来运行这个命令，system函数将返回错误代码127；如果是其他错误，则返回-1.否则，system函数将返回该命令的退出码。

   2）、替换进程映像

**  #include<unistd.h>**

**         char  **environ;**

**         int  execl(const  char  *path,  const  char  *arg0,  .  .  .  ,  (char*)0);**

**         int  execlp(const  char  *file,  const  char  *arg0,  .  .  .  ,  (char*)0);**

**         int  execle(const  char  *path,  const  char  *arg0,  .  .  .  ,  (char*)0,  char  *const  envp[]);**

**         int  execv(const  char  *path,  char  *const  argv[]);**

**         int  execvp(const  char  *file,  char  *const  argv[]);**

**         int  execve(const  char  *path,  char  *const  argv[],  char  *const  envp[]);**

        I、exec函数可以把当前进程替换为一个新的进程，新的进程由path或file参数指定。

       II、execl、execlp和execle的参数是可变的，参数以一个空指针结束。execv和execvp的第二个参数是一个字符串数组。

      III、以字母p结尾的函数通过PATH环境变量来查找新程序的可执行文件的路径。如果可执行文件不再PATH定义的路径中，我们就需要把包括目录在内的使用绝对路径的文件

           名作为参数传递给函数。

      IV、全局变量environ可用来把一个值传递到新的程序环境中。此外，函数execle和execve可以通过参数envp传递字符串数组作为新程序的环境变量。

       V、一般情况下，exec函数是不会返回的，除非发生了错误。出现错误时，exec函数将返回-1，并且会设置错误变量errno。

   3）、复制进程映像

**   #inckude<sys/types.h>**
**         #include<unistd.h>**
**         pid_t  fork(void);**

        I、此函数可以用来创建新的进程。

        II、返回0，则为子进程，返回-1，则fork失败。返回其他，为父进程。

   4）、等待一个进程

**  #include<sys/types.h>**

**         #include<sys/wait.h>**

**         pid_t  wait(int  *stat_loc);**

       I、此函数可以让父进程等待子进程结束。它将暂停父进程知道它的子进程结束为止。

       II、此调用返回子进程PID，它通常是已经结束运行的子进程PID。

      II、利用sys/wait.h文件中定义的宏来解释状态信息。（P400  表11-2）  

   5）、僵尸进程

       I、僵尸进程例程（P402）


**    #include<sys/types.h>**

**       #include<sys/wait.h>**

**       pid_t   waitpid(pid_t  pid,  int  *stat_loc,  int  options);**

      I、此函数可以用来等待某个进程结束。

     II、pid参数指定需要等待的子进程的PID。如果它的值为-1，waitpid将返回任一子进程的信息。options参数可以改变waitpid的行为，其中一个最有用的选项是WNOHANG，

          他的作用是防止waitpid调用将调用者的执行挂起。

   6）、输入和输出重定向

   7）、线程







三、信号

 1、信号是UNIX和Linux系统响应某些条件而产生的一个事件。接受到该信号的进程会相应地采取一些行动。用术语生成（raise）表示一个信号的产生，使用术语捕捉

    （eatch）表示接受到一个信号。




 2、信号的名称在头文件signal.h中定义，它们以SIG开头。（P405  表11-3  表11-4）

   1）、如果shell和终端驱动程序是按通常情况配置的话，在键盘上敲入终端字符（通常是Ctrl+C组合键）就会向前台进程发送SIGINT信号，这将引起该程序的终止，除非它安

             排了捕捉这个信号。

    2）、如果想发送一个信号给进程，而该进程并不是当前的前台进程，就需要用**kill**命令。

    3）、signal函数

**          #include<signal.h>**

**          void  (*signal(int  sig,  void  (*func)(int)))(int);**

        I、此函数可以用来处理信号。

       II、此函数带有sig和func两个函数。准备捕捉或忽略的信号由参数sig给出，接收到指定信号后将要调用的函数由参数func给出。信号处理函数必须有一个int类型的参数（即

            接收到的信号代码）并且返回类型为viod。

       III、signal函数也返回一个同类的函数，即先前用来处理这个信号的函数，或者也可以用两个特殊值之一来代替信号处理函数。

      IV、signal函数赶回的是先前对制定信号处理的信号处理函数的函数指针，如果未定义信号处理函数，则返回SIG_ERR并设置errno为一个正数值。如果给出的是一个无效的

             信号，或者尝试处理的信号是不可捕捉或不可忽略的信号，errno将被设置为EINVAL。




 3、发送信号

   1）、kill函数

** #include<sys/types.h>**

**         #include<signal.h>**

**         int  kill(pid_t  pid,  int  sig)**

        I、进程可以调用此函数向包括它本身在内的其他进程发送一个信号。

       II、kill函数把参数sig给定的信号发送给由参数pid给出的进程号所指定的进程，成功时返回0.要想发送一个信号，发送进程必须拥有相应的权限。

      III、kill函数调用会在失败时返回-1并设置errno变量。失败的原因可能是：给定的信号无效（errno设置为EINVAL）；发送进程权限不够（errno设置为EPERM）；目标进程

            不存在（errno设置为ESRCH）。

   2）、alarm函数

**  #include<unistd.h>**

**         unsigned  int  alarm(unsigned  int  seconds);**

        I、进程可以调用此函数在经过预定时间后发送一个SIGALRM信号。此函数在second秒之后安排发送一个SIGARN信号，实际闹钟时间将比预想安排的要稍后拖后一点。把

             参数seconds设置为0将取消所有已设置的闹钟请求。

       II、如果在接收到SIGALRAM信号之前再次调用alarm函数，则闹钟重新定时。

      III、alarm函数的返回值是以前设置的闹钟时间的余留秒数，如果调用失败则返回-1.

   3）、paush函数

** #include<unistd.h>**

**         int  paush(void);**

        I、此函数把程序额度执行挂起知道有一个信号出现为止。当程序接收到一个信号时，预设好的信号处理函数将开始执行，程序也将恢复正常的执行。

       II、当它被一个信号中断时，将返回-1并吧errno设置为EINTR。

   4）、sigaction函数

**   #include<signal.h>**

**        int  sigaction(int  sig,  const  struct  sigaction  *act,  struct  sigaction  *oact);**

      I、sigaction结构定义在signalh中，它的作用是定义在接收到参数sig指定的信号后应该采的行动。

**  void  (*)  (int)  sa_handler**

**           sigset_t  sa_mask**

**           int  sa_flags**

           sa_handler是一个函数指针，它指向接收到信号sig时将被调用的信号处理函数。可以将sa_handler字段设置为特殊值SIG_ICN或SIG_DFL他们分别表示信号被忽略或把

           对该信号的处理方式恢复为默认。

           sa_mask指定了一个信号集，在调用sa_handler所指向的信号处理函数之前，该信号将被加入到进程的信号屏蔽字中。


     II、此函数设置与信号sig相关联的动作。如果oact不是空指针，此函数将把原先对信号的动作写到它指定的位置。如果oact是空指针，此函数就不需要再做其他设置了。否

          则，将在该参数中设置对指定信号的动作。

    III、此函数成功时返回1，失败是返回-1。如果给出的信号无效或者试图对一个不允许被捕捉或忽略的信号进行捕捉或忽略，错误变量errno将被设置为EINVAL。




 4、信号集

   1）、处理信号集的函数

**    #include<signal.h>**

**         int  sigaddset(sigset_t  *set,  int  signo);**

**         int  sigemptyset(sigset_t  *set);**

**         int  sigfillset(sigset_t  *set);**

**         int  sigdelset(sigset_t  *set,  int  signo);**

       I、sigemptyse函数将信号集初始化为空。

      II、sigfillset函数将信号集初始化为包含所有一顶一的信号。

     III、sigaddset和sigdelset分别从信号集中增加或删除给定的信号。

     IV、它们在成功时返回0，失败时返回-1并设置errno。只有一个错误代码被定义，即当给定的信号无效时，errno奖杯设置成EINVAL。

**    #include<signal.h>**

**        int  sigsmember(sigset_t  *set,  int  signo);**

       I、此函数判断一个给定的信号是否是一个信号集的成员。

      II、如果是就返回1：如果不是就返回0：如果给定的信号无效时，errno就设置为EINVAL。

**      #include<signal.h>**

**        int  sigprocmask(int  how,  const  sigset_t  *set,  sigset_t  *oset);**

       I、此函数可以进行进程的信号屏蔽字的设置或检查工作。信号屏蔽字是指当前被堵塞的一组信号，他们不能被当前进程接收到。

      II、此函数可以根据how参数（P412  表11-6）指定的方法修改进程的信号屏蔽字。

     III、新的信号屏蔽字由参数set指定，而原先的信号屏蔽字将保存到信号集oset中。如果参数set是空指针，how的值就没有意义了，此时这个调用的惟一目的就是把当前信号

           屏蔽字的值保存到oset中。

     IV、如果sigprocmask成功完成，它将返回0；如果参数how的值无效，它将返回-1并设置errno为EINVAL。

**    #include<signal.h>**

**         int  sigpending(sigset_t  *set);**

        I、此函数的作用是：将被堵塞的信号中停留在待处理状态的一组信号写到参数set指向的信号集中。

       II、成功时它将返回0，否则返回-1并设置errno以表明错误原因。

**  #include<signal.h>**

**         int  sigsuspend(const  sigset_t  *sigmask);**

        I、此函数将进程的屏蔽字替换为由参数sigmask给出的信号集，然后挂起程序运行。程序将在信号处理函数执行完毕后继续执行。

       II、如果接收到的信号终止了程序，sigsuspend就不会返回；如果接收到的信号没有终止程序，sigsuspend就返回-1并将errno设置为EINTR。

   2）、sigaction标志

        I、sa_flags字段的取值（P413  表11-7）

       II、安全调用的信号处理函数（P414  表11-8）

   3）、常用信号参考（P414  表11-9和P415  表11-10、表11-11、表11-12）




