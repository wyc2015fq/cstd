# 《Linux程序设计》——信号量、共享内存和消息队列 - 一世豁然的专栏 - CSDN博客





2014年10月19日 00:28:03[一世豁然](https://me.csdn.net/Explorer_day)阅读数：703标签：[信号量																[共享内存																[消息队列																[linux](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[liunx应用编程](https://blog.csdn.net/Explorer_day/article/category/2276817)





一、信号量

 1、信号量是一个特殊的变量，它只取正整数值，并且程序对其访问都是原子操作。只允许对信号量进行等待（wait）和发送信号量（signal）这两种操作。 Linux系统中，“等待”

       和“发送信号”都具有特殊意义，用定义的符号表示这两种操作：P（信号量变量）：用于等待。V（信号量变量）：用于发送信号。

   1）、二进制信号量：信号量的取值只能是0和1的变量。通用信号量：可以取多个正整数的值的信号量。

   2）、PV操作的定义（P489 表14-1）

         I、当临界区域可用时，信号量变量sv的值是true，然后P（sv）操作将它减1使它变为false以表示临界区域正在被使用；当进程离开临界区域时，使用V（sv）操作将它加一，

              使临界区域再次变为可用。




 2、理论实例（P489-P490）




 3、Linux信号量机制

   1）、semget函数

**  include<sys/sem.h>**

**         int  semget(key_t  key,  int  num_sems,  int  sem_flags);**

       I、此函数的·1作用是创建一个新的信号或取得一个已有信号量额键。

       II、参数key是整数值，不相关的进程可以通过它访问同一个信号量。只有semget函数才能直接使用信号量键，所有其他的信号量函数都是使用由semget返回的信号量标识

             符。

       III、num_sems参数指定需要的信号量数目。它几乎总是取值为1。sem_flags参数是一组标志，与open函数的标志非常相似。它低端的9个比特是该信号量的权限，起作


             用类似于文件的访问权限。此外，他们还可以和值IPC_CREAT做按位或操作，来创建一个新信号量。

       IV、此函数在成功时返回一个正数值，他就是其它信号量函数用到的信号量标识符。如果操作失败，则返回-1。

   2）、semop函数

**    #include<sys/sem.h>**

**         int  semop(int  sem_op,  struct  *sem_ops,  size_t  num_sem_ops);**

       I、此函数用于改变信号量额值。

       II、sem_id参数是由semget返回的信号量标识符。

       III、sem_ops参数是指向一个结构数组的指针，每个数组元素至少包含一下几个成员：

**            struct  sembuf {**

**             short  sem_num;**

**             short  sem_op;**

**             short  sem_flg;}**

             sem_num：此值是信号量编号，除非你需要使用一组信号量，否则它的取值一般为0。

             sem_op：此值是信号量在一次操作中需要改变的数值（可以用一个非1值来改变信号量的值）。通常只会用到两个值，一个是-1，也就是P操作，它等待信号量变为可

                                 用；一个是+1，也是就V操作，它发送信号表示信号量现在已可用。

             sem_flg：此成员通常被设置为SEM_UNDO。它将使得操作系统跟踪当前进程对这个信号量的修改情况，如果这个进程在没有释放该信号的情况下终止，操作系统将自

                                动释放该进程持有的信号量。除非对信号量额行为有特殊要求，否则应该改养成设置sem_flg为SEM_UNDO的习惯。

   3）、semctl函数

** #include<sys/sem.h>**

**         int  semtcl(int  sem_id,  int  sem_num,  int  command,  . . .);**

       I、sem_id是由semget返回的信号量标识符。

       II、sem_num参数是信号量编号，当需要用到成组的信号量时，就要用到此参数。它一般取值为0，表示这是一个也是唯一一个信号量。

       III、command参数是将要采取的动作，它的取值一般有两个：

              SETVAL：用来吧信号量初始化一个已知的值。

              IPC_RMID：用于删除一个已经无需继续的信号量标识符。


       IV、如果还有第四个参数，它将会是一个union_semun结构，根据X/OPEN规范的定义，它至少包含一下几个成员：

** union  semun{**

**              int  val;**

**              struct  semid_ds  *buf;**

**              unsigned  short  *arrag;}**



 4、使用信号量（代码P493-495）







二、共享内存

 1、共享内存是3个IPC机制中的第二个。它允许两个不相关的进程访问同一个逻辑内存。




 2、共享内存函数

   1）、shmget函数

** #include<sys/shm.h>**

**         int  shmget(key_y  key,  size_t  size,  int  shmflg);**

        I、此函数用来创建共享内存。

        II、key参数可以有效的为共享内存段命名。

        III、size参数以字节为单位指定需要共享的内存容量。

        IV、shmflg参数包含9比特的权限标志，它们的作用与创建文件时使用的mode标志一样。

        V、如果共享内存创建成功，shmget返回一个非负整数，即共享内存标识符；如果失败，就返回-1。

   2）、shmat函数

**         #include<sys/shm.h>**

**         void  *shmat(int  shm_id,  const  void  *shm_addr,  int  shmflg);**

       I、参数shm_id是由semget返回的共享内存标识符。

       II、参数shm_addr指定的是共享内存链接到当前进程中的地址位置。它通常是一个空指针，表示让系统来选择共享内存出现的地址。

       III、参数shmflg是一组位标志。

       IV、如果shat调用成功，它返回一个指向共享内存的第一个字节的指针；如果失败，它将返回-1。

       V、此函数用来连接到一个进程的地址空间中。

   3）、shmdt函数

**  #include<sys/shm.h>**

**         void  *shmdt(const  void  *shm_addr);**

       I、此函数额作用是将共享内存从当前中分离。

       II、参数shm_addr是shmat返回的地址指针。成功时它返回0，失败时返回-1。

   4）、shmctl函数

**         #include<sys/shm.h>**

**         int  shmctl(int  shm_id,  int  command,  struct  shmid_ds  *buf);**

       I、shmid_ds结构：

**            struct  shmid_ds{**

**            uid_t  shm_perm.uid;**

**            uid_t  shm_perm.gid;**

**            mode_t  shm_perm.mode;}**

       II、参数shm_id是shmget返回的共享内存标识符。

       III、参数command是要采取的动作，它可以去三个值。（P498表14-2）

       IV、buf参数是一个指针，它指向包含共享内存模式和访问权限的结构。

       V、此函数成功时返回0，失败时返回-1。








三、消息队列


 1、消息队列提供了一种在两个不相关的进程之间传递数据的相当简单且有效的方法。与命名管道相比，消息队列的优势在于，它独立于发送和接受进程而存在，这消除了在同

        步命名管道的打开和关闭时可能产生的一些困难。




 2、消息队列函数

   1）、msgget函数

**   #include<sys/msg.h>**

**         int  msgget(key_t  key,  int  msgflg);**

        I、此函数用来创建和访问一个消息队列。        

        II、程序必须提供key键值来命名某个特定的消息队列。

        III、msgflg参数由九个权限标志组成。由IPC_CREAT定义的一个特殊位必须和权限标志位或才能创建一个新的消息队列。在设置IPC_CREAT标志时，如果给出的是一个已

              有的消息队列也不会产生错误。如果消息队列已有，则IPC_CREAT标志就被悄悄忽略掉。

        IV、此函数成功时返回一个整数，即消息队列标识符，失败时返回-1。


   2）、msgsnd函数

**       #include<sys/msg.h>**

**         int msgsnd(int  msgid,  const  void  *msg_ptr,  size_t  msg_sz,  int  msgflg);**

       I、此函数用来把消息添加到消息队列中。

       II、消息结构受到两方面的约束。首先，他的长度必须小于系统规定的上限；其次，它必须以一个长整型成员变量开始，接受函数将这个成员变量来确定消息的类型。消息

             最好定义成 ：

**    struct  my_message{**

**             long  int  message_type;**

**  /*你希望发送的数据*/**

** }**

       III、.sqid参数是msgget函数返回的消息队列标识符。

       IV、msg_ptr参数是一个指向准备发送消息的指针，消息必须以长整型变量开始。

       V、msg_sz是msg_ptr指向的消息的长度。此长度不能包括长整型消息队列成员变量的长度。

      VI、msgflg参数控制在当前消息队列满或消息队列到达系统范围的限制时将要发生的事情。如果msgflg中设置了IPC_NOWAIT标志，函数立刻返回，不发送消息并且返回值

             为-1.如果msgflg中的IPC_NOWAIT标志被清除，则发送进程将挂起以等待队列中腾出可用空间。

     VII、此函数成功时返回0，失败时返回-1.如果成功，消息数据的一份副本将被放到消息队列中。

   3）、msgrcv函数

**#include<sys/msg.h>**

**         int  msgrcv(int  msgid,  void  *msg_ptr,  size_t  msg_sz,  long  int  msgtype,  int  msgflg);**

        I、msgid参数是msgget函数返回的消息队列标识符。

        II、msg_ptr参数是一个指向准备接收消息的指针，消息必须以长整型变量开始。

        III、msg_sz是msg_ptr指向的消息的长度。此长度不能包括长整型消息队列成员变量的长度。

       IV、参数msgtype是一个长整型，它可以实现一种简单形式的接受优先级。如果没事干type的值为0，就获取队列中第一个可用信息，如果它的值大于0，将获取具有相同信

             息类型的第一个信息。如果它的值小于0，将获取消息类型等于或小于msgtype的绝对值的第一个信息。

        V、参数msgflg用于控制当队列中没有相应类型的消息可以接收时将发生的事情。如果msgflg中设置了IPC_NOWAIT标志，函数立刻返回，返回值为-1。如果msgflg中的

              IPC_NOWAIT标志被清除，则发送进程将挂起以等待相应类型信息额到达。

      VI、此函数成功时返回放到接收区中的字节数，消息被复制到由msg_ptr指向的用户分配的缓存区中，然后删除消息队列中额1对应信息。失败返回-1。

   4）、msgctl函数

** #include<sys/msg.h>**

**         int  msgctl(int  msqid,  int  command,  struct  msqid_ds  *buf);**

        I、msqid_ds结构至少包含一下成员：

**      struct  msqid_ds{**

**             uid_t  msg_perm.uid;**

**             uid_t  msg_perm.gid;**

**             mode_t  msg_perm.mode;**

**             }**

        II、参数msqid是由msgget返回的消息队列标识符。参数command是将要采取的动作。它可以去3个值（P504  表14-3）

        III、成功时它返回哦，失败时返回1.如果删除消息队列，某个进程正在msgnd或msgrcv函数中等待，这两个函数将失败。](https://so.csdn.net/so/search/s.do?q=消息队列&t=blog)](https://so.csdn.net/so/search/s.do?q=共享内存&t=blog)](https://so.csdn.net/so/search/s.do?q=信号量&t=blog)




