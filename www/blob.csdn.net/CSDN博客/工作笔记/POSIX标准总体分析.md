# POSIX标准总体分析 - 工作笔记 - CSDN博客





2016年03月26日 12:12:28[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：8771








## POSIX标准总体分析

转自：[http://blog.csdn.net/novagx/article/details/2077561](http://blog.csdn.net/novagx/article/details/2077561)

# [POSIX](http://blog.csdn.net/novagx/article/details/2077561)标准总体分析

    POSIX，全称为可移植性操作系统接口，是一种关于信息技术的IEEE标准。它包括了系统应用程序接口（简称API），以及实时扩展[C语言]。

该标准的目的是定义了标准的基于UNIX操作系统的系统接口和环境来支持源代码级的可移植性。现在，标准主要提供了依赖C语言的一系列标准服务，再将来的版本中，标准将致力于提供基于不同语言的规范。

该标准对核心需求部分定义了一系列任何编程语言都通用的服务，这一部分服务主要从其功能需求方面阐述，而非定义依赖于编程语言的接口。语言规范主要有两部分组成。一部分包括了访问核心服务的编程语言的标准接口，这些核心服务为标准中基于编程语言的核心需求部分所定义；另一部分包含了一个特殊语言服务的标准接口。基于任何语言，与该标准一致的执行都必须遵循语言规范的任何章节。

该标准一共被分为四个部分：

（1）陈述的范围和一系列标准参考（第一章）；

（2）定义和总概念；（第二章）

（3）各种接口设备；（第三章到第九章，第十一章到第十五章）

（4）数据交换格式；（第十章）

该标准的主要目的有：

（1）面向应用

（2）定义接口，而不是它的具体实现；

（3）涉及资源和可移植性，而非对象；

（4）基于c语言；

（5）无超级用户，无系统管理；

（6）最小限度的接口，最小限度的定义；

（7）应用领域广泛；

（8）对以前的实现进行最小限度改变；

（9）对原有程序代码做最小的修改；

（10）实时扩展；

以下就对各个章节做简要分析。

第一章概述

1．1范围

定义范围的关键要素有：

（1）定义足够的一套功能适用于实时应用程序领域的重要部分；

（2）定义足够的实现规范和性能相关的函数，以便允许实时应用程序完成系统的确定性的响应；

1．2一致性

系统须支持标准中定义的接口，系统能够提供标准中没有要求到的函数和工具。在遵循于该标准的实现中，一种一致性文档是需要用到的，它必须具有与该标准相同的结构，包含有全名，数字，和标准所指示的日期，以及头文件<limits.h>和<unistd.h>中的界限值等等。该一致性文档详细说明了标准中定义的执行行为。该标准要求了应用程序的一致性，所有遵循标准的应用程序都使用基于c语言的服务。

第二章术语和基本需求

2．1定义

（1）术语

该标准中定义了一系列术语，如一致性文档，被定义的实现，可能性，无用的特性等，还定义了一些通用名词，如绝对路径，存取模式，地址空间，适当权限，定时器，异步I/O操作，后台进程，后台进程组，块文件，阻塞进程，等等。

（2）基本概念

   扩展安全控制；文件存取允许；文件级别；文件名可移植性；路径名的决定；

（3）错误号

大部分函数都在外部变量errno中提供了错误号，定义如下：

         extern int errno;

 (4) 简单系统的数据类型

这些数据类型在头文件<sys/types.h>中定义，它包含了至少以下类型：

        dev_t:   用于设备号；

        gid_t:   用于进程标志符；

        ino_t:   用于文件序列号；

        inode_t: 用于一些文件参数；

nlink_t:    用于连接内容；

off_t:      用于文件大小；

pid_t:     用于进程或进程组标志符；

size_t:     在c标准（2）中定义；

ssize_t:    用于返回字节数和错误标志的函数；

uid_t:      用于用户标志符；

（4）环境描述

当一个进程开始执行时，将调用一个表示环境的字符串数组，这个数组为外部变量environ所指向，其定义如下：

extern char **environ;

 (5) 其他

   在该章中，标准还总体介绍了c语言定义的一些标志符，数字方面的限制，以及一些符号常量，这些在以后的章节中都会一一出现。

       以下是OSE服务的简要设计描述的一个清单。这些要求详细叙述在POSIX规范书中。

l进程源语

进程源语所涉及的函数完成了大多数简单的操作系统服务，如进程处理，进程信号，以及定时器。一般情况下，该标准所确定的进程的所有属性都不能被一个进程源语改变，除非描述符特别指明。



l进程环境

l目录与文件

目录与文件所涉及的函数执行了一系列操作系统服务，例如文件的创建和删除，检测，修改特性。它们提供了主要的方法，进程因为一系列 I/O操作使用他们来存取文件和目录。



l输入输出原语

    这些子文件和管道函数处理输入和输出。功能被指定为进行文件描述符管理和I/O活动之间得平衡调节。

l设备驱动和分类函数

            该章节描述了一个基本的终端接口和一些基本的终端控制函数，如果执行时被提供，所有的异步通信端口都应该支持它，接口是支持网络连接还是异步端口或者两者都支持由完成者定义，一致性文档应该描述那些设备类型被这些接口所支持。本章的一些函数主要应用与进程的控制终端

l基于c语言得服务



l系统数据库





l数据交换格式



l异步





l内存管理



l执行调度





l时钟和定时器



l消息传递





                 第三章进程原语



3．1进程的创建和执行

1．进程创建

   函数原型：pid_t   fork (void)

  函数功能：调用时需要引用的头文件是<sys/types.h>,fork()创建了一个新的进程。

2．执行一个文件

函数原型：int exec1(const char *path, const char *arg, …);

          int execv(const char *path, const *char argv[] );

          int execle(const char *path, const char *arg, …);

          int execve(const char *path, const *char argv[], char *const envp[])

          int execlp(const char *file, const char *arg. …);

          int execvp(const char *file, char *const argv[]);

函数功能：exec系列的函数用一个新的进程映像替代了当前的进

程映像，这个新的进程映像创建于一个规则的，可执行文件，叫做新进程映像文件。执行成功无返回值因为调用进程映像以为新的进程映像所覆盖。

3.2 进程的终止

进程的终止有两种情况：

（1）从main()函数返回时或者执行exit()或_exit()函数时正常的终止；

（2）被abort()函数请求或者接受到一些信号时不正常的终止；

1．等待进程终止

   函数原型：#include<sys/types.h>

#include<sys/wait.h>

pid_t wait(int *stat_loc);

pid_t waitpid(pid_t pid,int *stat_loc.int options);

   函数功能：wait（）和waitpid（）允许调用进程获得它的一个子进程的状态信息。Wait()函数将挂起调用进程直到获得了它的子进程的状态信息，或者是直到获得一个终止进程信号；如果pid＝－1并且options=0，waitpid()函数功能将和waitpid()相同，否则它的功能将依据pid和options的值而改变。

2. 终止一个进程

   函数原型：void_exit(int status);

  函数功能：_exit()函数将终止一个调用进程，该函数不能返回给其调用者

3.3信号

在头文件<signal.h>终声明了sigset_t类型和sigaction结构。完成所定义的信号分三类：必需的信号；任务控制信号；内存保护信号，分别如下表：

必需信号

|符号常量|描述|
|----|----|
|SIGABRT|非正常终止信号|
|SIGALRM|超时信号|
|SIGFPE|错误运算操作|
|SIGHUP|为控制中断所检测到的挂断|
|SIGILL|无效硬件信号的检测|
|SIGINT|交互式信号|
|SIGKILL|终止信号|
|SIGPIPE|写信号|
|SIGQUIT|交互式终止信号|
|SIGSEGV|无效内存引用检测信号|
|SIGTERM|终止信号|
|SIGUSR1|保留信号|
|SIGUSR2|保留信号|


作业控制信号

|符号常量|描述|
|----|----|
|SIGCHLD|子进程终止或停止|
|SIGCONT|停止后继续|
|SIGSTOP|停止信号|
|SIGTSTP|交互式的停止信号|
|SIGTTIN|从控制终端读|
|SIGTTOU|写到控制终端|


内存保护信号

|符号常量|描述|
|----|----|
|SIGBUS|获取内存中不确定的部分|


每一个进程有一个进程标记（process mask），它定义了一组产生但被阻塞传递的信号集。Sigaction（），sigpromask（），sigsuspend（）

函数控制这个进程标记的行为。

1.送一个信号到进程

函数原型：#include <sys/types.h>

          #include<signal.h>

          int kill(pid_t pid, int sig)

函数功能：该函数发送一个信号到一个由pid指明的进程或者进程组，sig标志了信号类型，其值是0或者上表中的值。如果发送成功，返回‘0’，否则返回‘1’。

2. 操纵信号集

 函数原型：#include<signal.h>

int sigemptyset(sigset_t *set);

int sigfillset(sigset_t *set);

int sigaddset(sigset_t *set, int signo);

int sigdelset(sigset_t *set, int signo);

int sigisemeber(const sigset_t *set,int signo);

 函数功能：sigsetops源语操纵信号集。他们操作以数字为对象，这些数据由应用程序的地址所指向，而不是以系统所知的信号集为对象。

3. 检测和更改信号的行为

 函数原型：#include<signal.h>

            int sigaction(int sig,const struct sigaction *act,struct sigaction *oact);

             函数功能：该函数允许调用函数检查与确定的信号相联系的行为，

                        参数sig确定了信号，sigaction结构在头文件<signal.h>中被定义，描述了所采取的行为。如果参数act不为null,它指向一个结构，它指定了与信号相联系的行为。如果参数oact不为null，先前与信号相联系的行为将被存储到由oact指向的地方。

4. 检查和改变阻塞信号

 函数原型：#include<signal.h>

int sigprocmask(int how,xonst sigset_t *set,sigset_t *oset);

 函数功能：该函数用来检查和改变调用进程的信号标记（signal mask），如果参数set不为null，它指向一个信号集的用于改变当前的阻塞集。参数how指明了改变方式，参数oset不为null时，先前的信号标记被存储在它所指向的地方，如果参数set为null，则参数how就没有意义了，而且进程的信号标记不能随函数调用而改变。

5.检查未定的信号

 函数原型：#include<signal.h>

            int sigpending(sigset_t *set);

 函数功能：该函数存储一个信号集，这些信号是在被调用进程传输和未定的情况下阻塞的，由参数set所指向。

6.等待一个信号

 函数原型：#include<signal.h>

             int sigsuspend(const sigset_t *sigmask);

  函数功能：该函数用参数sigmask所指向的信号集取代了进程信号标记（signal mask），然后挂起该进程直到接受到一个信号，其行为是执行信号跟踪功能或者是终止该进程。

7.同步接受一个信号

 函数原型： #include<signal.h>

             int sigwaitinfo(const sigset_t *set, siginfo_t *info);

             int sigtimedwait(const sigset_t *set,siginfo_ *info, const struct timespec *timeout );

 函数功能：该函数从参数set所确定的信号集中选择一个未定的信号出来。如果该函数成功，返回一个信号数；否则返回－1。

8.排队一个信号到进程

 函数原型：#include<signal.h>

           int sigqueue(pid_t pid,int signo, const union sigval value);

 函数功能：该函数功能是使由signo确定的信号将参数value所确定的值发送到由pid指明的进程中去。

3.4 定时器操作

1．调度警报

   函数原型：unsigned int alarm(unsigned int seconds);

  函数功能：当参数seconds指定的一段实时时间过去后，该函数将发送一个SIGALRM信号到调用进程。

2.挂起进程的执行

   函数原型：int pause(void);

  函数功能：该函数挂起一个调用进程直到得到一个信号，这个信号或者执行信号跟踪功能或者是终止该进程。如果是终止进程，该函数不返回；如果是执行信号跟踪功能，则该函数在信号跟踪函数返回后也要返回。

3.延迟进程的执行

 函数原型：unsigned int sleep(unsigned int seconds);

 函数功能：该函数使当前进程从执行状态转化为挂起状态，直到参数seconds所指定的一段实时时间过去后，或者是一个唤醒信号跟踪功能或终止进程功能的信号到来。该挂起时间由于系统的其他调度活动可能会比要求的时间长。



第三章进程环境

4.1 进程标志符

1.获得进程和父进程的ID

 函数原型：#include<sys/types.h>

pid_t getpid(void);

pid_t getppid(void);

 函数功能：getpid（）返回调用进程的进程ID,

getppid（）返回调用进程的父进程ID.

4.2 用户ID

1．获得真实用户，有效用户，真是组，有效组的ID

   函数原型：#include<sys/types.h>

uid_t getuid(void);

uid_t geteuid(void);

gid_t getgid(void);

gid_t getegid(void);

  函数功能：getuid（）返回调用进程的真实用户ID, geteuid()返回调用进程的有效用户ID，getgid（）返回调用进程的真实组ID，getegid（）返回调用进程的有效组的ID。

2.设置用户和组的ID

 函数原型：#include<sys/types.h>

            int setuid(uid_t uid);

            int setgid(gid_t gid);

 函数功能：这两个函数分别根据进程的权限设置真实用户ID,有效用户ID，真实组ID，有效组ID。

3.获得辅助组ID

 函数原型：# include<sys/types.h>

            int getgroups(int gidsetsize.gid_t grouplist[]);

 函数功能：该函数在队列的组列表中填入调用进程的辅助组ID。参数grouplist确定了组列表中的元素个数。

4.获得用户名

 函数原型：char *getlogin(void)

  函数功能：该函数返回一个指针，指向与调用进程相关的用户名。

4.3进程组

1.获得进程组ID

 函数原型：#include<sys/types.h>

pid_t getpgrp(void);

 函数功能：该函数返回调用进程的进程组ID。

2. 创建会议并且设置进程组ID

 函数原型：#include<sys/types.h>

pid_t setsid(void)

 函数功能：如果调用进程不是进程组的引导者，则该函数将创建一个新的会议。该调用进程应该为新会议的会议引导者，应该为新进程组的引导，应该没有控制终端。进程组的ID应该等于该调用进程的ID。

3.为作业控制设置进程组ID

 函数原型：#include<sys/types.h>

            int setpgid(pid_t pid,pid_t pgid);

 函数功能：如{_POSIX_JOB_CONTROL}被定义，则该函数用来加入已经存在的进程组或者创建一个新的进程组。

4.4系统标志

1.获得系统名

 函数原型：#include<sys/utaname.h>

int uname(struct utsname *name);

 函数功能：该函数存储了鉴别当前操作系统的信息。

4.5 时间

 1得到系统时间

函数原型：#include<time.h>

          time_t time(time_t *tloc);

函数功能：该函数返回自从一时间点以来的某个时间值，以秒为单位。参数tloc指向一个时间值所存储的地方。

2. 获得进程时间

 函数原型：#include<sys/times.h>

clock_t times(struct time *buffer);

 函数功能：参数buffer指向一个结构，该函数向该结构填写有关时间的信息。Clock_t和tms结构在<sys/times.h>中定义。

4．6环境变量

1.获取环境

 函数原型：#include<stdlib.h>

char *getenv(const char *name);

 函数功能：该函数在环境列表查找字符串name=value,返回指向value的指针。如果没有找到，则返回null。

4.7 终端标志

1．产生终端路径

 函数原型：#include<stdio.h>

             char Ictermid(char *s);

 函数功能：该函数产生一个字符串，作为路径名，提交到当前进程的当前控制终端。

2．确定终端设备名

 函数原型：char *ttyname(int fildes);

            int isatty(int fildes);

 函数功能：ttyname（）返回一个指针指向一个字符串，它包含了与文件描述符fildes相关的终端名；如果fildes是一个有效的与终端联系的文件描述符，isatty（）返回“1”，否则返回“0”。

4．8可配置的系统变量

1．获得可配置的系统变量

 函数原型：#include<unistd.h>

long sysconf(int name);

 函数功能：该函数提供了一个应用方法来决定可配置系统变量的当前值。参数name代表了所查询的系统变量。



第五章文件和目录

5．1目录

1．目录的入口形式

     头文件<dirent.h>定义了一个结构和目录程序用到的类型，没有确定的文件内部格式。Readdir()返回了一个指针指向一个类型对象struct dirent。

2．目录操作

 函数原型：#include<sys/types.h>

#include<dirent.h>

dir *opendir(const char *dirname);

struct dirent *readdir(dir *dirp);

void rewinddir(dir *dirp);

int closedir(dir *dirp);

 函数功能：opendir()根据参数dirname打开一个目录流；readdir（）返回一个指针，它指向一个目录流中当前位置的目录入口，目录流由参数dirp指向；rewinddir（）重置目录流的位置到目录的起点；closedir（）关闭目录流，如成功，则返回“0”值。

5．2工作目录

 1改变当前的工作目录

   函数原型：int chdir(const char *path);

   函数功能：path指向目录的路径名。该函数使命名的目录成为当前的工作目录。

 2获得工作目录路径名

   函数原型：char *getcwd(char *buf,size_t size);

   函数功能：该函数复制当前工作目录的绝对路径名到buf所指向的队列中。

5．3基本文件的创建

 1打开一个文件

   函数原型：#include<sys/types.h>

              #include<sys/stat.h>

              #include<fcnt1.h>

              int open(const char *path, int oflag,…);

   函数功能：open（）在文件和文件描述符之间建立了一个连接，它创建了一个指向一个文件的打开文件描述，参数path指向文件的路径名。

 2创建一个新文件或者重写一个已经存在的文件

   函数原型：#include<sys/types.h>

              #include<sys/stat.h>

              #include<fcnt1.h>

              int creat(const char *path, mode_t mode);

   函数功能：该函数调用creat（path，mode）相当于open（path，o_wronly/o_creat/o_trunc,mode）;

 3设置文件的创建标记

   函数原型：#include<sys/types.h>

              #include<sys/stat.h>

              mode_t umask(mode_t cmask);

   函数原型：umask()设置进程的文件模式创建标记到cmask，并且返回原来的标记值。

 4连接到一个文件

   函数原型：int link(const char *existing,const char *new);

   函数功能：参数existing指向路径名来命名存在文件，参数new指向一个路径名，它命名了一个创建的新的目录入口。该函数为已存在的文件自动的创建一个新的连接，并且将文件连接数加1。

5．4特殊文件创建

 1生成一个目录

   函数原型：#include<sys/types.h>

              #include<sys/stat.h>

              int mkdir(const char *path,mode_t mode);

   函数功能：该函数依据参数path创建一个新的目录。新目录的允许位根据mode初始化。

 2创建一个FIFO类型的文件

   函数原型：#include<sys/types.h>

              #includesys/stat.h>

              int mkfifo(const char *path,mode_t mode);

   函数功能：mkfifo（）创建一个新的fifo类型文件，它由path指向的路径名命名。

5．5文件的移动

 1移动目录入口

   函数原型：int unlink(const char *path);

   函数功能：该函数移去由参数path指向得路径名所命名得连接并将连接数减去1。

 2移去一个目录

   函数原型：int rmdir(const char *path)

   函数功能：

 3重命名一个文件

   函数原型：int rename(const char *old,const char *new);

   函数功能：该函数改变一个文件的名字，参数old指向被重命名文件的路径名，参数new指向文件的新路径名。

5．6文件特征

5．7可配置路径名变量



第六章输入与输出源语

6．1管道

  1创建内进程通道

   函数原型：int pipe(int filedw[2]);

   函数功能：该函数创建一个管道并且创建两个文件描述符，一个是fildes[0],一个是fildes[1],它们分别指的是‘读’与‘写’的管道端。

6．2文件描述符控制

   1复制一个打开文件描述符

    函数原型：int dup(int fildes);

               int dup2(int fileds,int fileds2);

    函数功能：这两个函数为fcntl（）服务提供了两个选择的接口，用到了F_DUPFD命令。

6．3撤除文件描述符

 1关闭一个文件

   函数原型：int close (int fildes);

   函数功能

6．4输入和输出

 1文件读

   函数原型：ssize_t read(int fildes,void *buf,size_t nbyte);

   函数功能：

 2文件写

函数原型：ssize_t write(int fildes,const void *buf,size_t nbyte);

函数功能：

6．5一个文件的控制操作

 1 文件控制操作的数据定义

       头文件<fcnt1.h>为fcntl（）和open（）定义了一下的要求和参数：

                     fcntl（）的_cmd值

|常量|描述|
|----|----|
|F_DUPFD||
|F_GETFD||
|F_GETLK||
|F_SETFD||
|F_GETTFL||
|F_SETFL||
|F_SETLK||
|F_SETLKW||


 2文件控制

   函数原型：#clude <sys/types.h>

              #clued<unistd.h>

              #include<fcntl.h>

              int fcntl(int fildes,int cnd,…);

   函数功能：fcntl()为打开的文件提供了一系列控制，参数fildes是一个文件描述符。Cmd决定了控制的内容。

 3读/写文件偏移量的重新定位

   函数原型：#include<sys/types.h>

              #include<unistd.h>

              off_t lseek(int fildes,off_t offset,int whence);

  函数功能：lseek（）为fildes所指定的文件重新设置偏移量。

6．6文件同步

 1文件的状态同步

   函数原型：#include<unistd.h>

              int fsync(int fildes);

   函数功能：该函数用来暗示所有的文件描述数据被传输到了存储设备上。

 2 文件数据的同步

函数原型：#include<unistd.h>

            int fdatasync(int fildes);

 函数功能：该函数迫使当前所有排队的I/O操作进入同步 I/O状态。

6．7异步输入与输出

 1●异步输入与输出的数据定义

       头文件<aio.h>能使<sys/types.h>，<signal.h>，<time.h>

和<fcntl.h>中的符号可见。

● 异步I/O控制块

异步I/O控制块结构aiocb在许多异步I/O接口中使用，它在<aio.h>中定义。

        ●主要常量

      2 异步读

        函数原型：#include<aio.h>

                  int aio_read (struct aiocb *aiocbp);

       函数功能：aiocbp->io_nbytes,表示读的字节数；aiocbp->aio_fildes，表示读的文件；aiocbp->aio_buf,表示读到的缓冲区。

     3异步写

      函数原型：#include<aio.h>

                 int aio_write(struct aiocb *aiocbp);

       函数功能：参数表示同上。

     4列出直接I/O

      函数原型：#include<aio.h>

                 int lio_listio(int mode,struct aiocb *const list[],int nent,struct sigevent *sig);

      函数功能：该函数允许用一个函数调用初始化一个I/O请求

列表。

 5 得到异步I/O操作的错误状态

函数原型：#include<aio.h>

          int aio_error(const struct aiocb *aiocbp);

函数功能：该函数返回aiocbp指向的结构所表示的错误状态。

 6 得到异步I/O操作的返回状态

函数原型：#include<aio.h>

          ssize_t aio_return(struct aiocb *aiocbp);

函数功能：

 7 删除异步I/O请求

函数原型：#include<aio.h>

          int aio_cancel (int fildes,struct aiocb *aiocbp);

函数功能：参数fildes是文件描述符，参数aiocbp指向异步I/O控制块上的请求删除部分。

 8 等待异步I/O请求

函数原型：#include<aio.h>

          int aio_suspend(const struct aiocb *const list[],int nent,const struct timespec *timeout);

函数功能：标准定义该函数挂起调用进程直到至少一个list指向

的异步I/O操作完成，或者一个信号中断了一个函数，

或者超时了（timeout指定）。

 9 异步文件同步化

函数原型：#include<aio.h>

          int aio_fsync(int op,struct aiocb *aiocbp);

函数功能：该函数迫使所有与（参数aiocbp指向的）结构aiocb中aio_fildes所指定的文件相关异步I/O操作进入同步状态。



      第七章设备和面向类的函数

7．1基本的终端接口

 1 接口特性

           ●当一个终端文件被打开，通常它将引起进程等待直到连接被建立。

            ●进程组

                 一个终端可以具有与它相关的前台进程组，它发挥特定的角色，后面会讲到。

●控制终端

●终端存取控制

●输入操作和读进程

●规范的输入操作

●非规范模式的输入操作

●写数据和输出处理

●特殊的符号(INTR,QUIT,ERASE,KILL…)

●modem断掉连接

●关闭终端设备文件

 2 可设置的参数

    ●termios机构

     该结构在<termios.h>中定义，在控制特定的终端I/O特性中要用到。

   ●输入模式                                                                                            

                    termios c_iflap值域

|标记名|描述|
|----|----|
|BRKINT|信号中断|
|ICRNL|输入时将CR映射到NL|
|IGNBRK|忽略中断状态|
|IGNCR|忽略CR|
|IGNPAR|忽略奇偶错误|
|INLCR|输入时将NL映射到CR|
|INPCK|输入奇偶校验使能|
|ISTRIP|Strip字符|
|IXOFF|开始/停止输入控制使能|
|IXON|开始/停止输出控制使能|
|PARMRK|产生奇偶错误|


    ●输出模式

    ●控制模式



|标记名|描述|
|----|----|
|CLOCAL|忽略modem状态行|
|CREAD|接受使能|
|CSIZE|每个字节的位数|
|CS5|5位|
|CS6|6位|
|CS7|7位|
|CS8|8位|
|CSTOPB|发送一个或两个停止位|
|HUPCL|在最后的关闭中挂起|
|PARENB|奇校验使能|
|PARODD|奇校验或偶校验|


●本地模式

                  termios c_lflag值

|标记名|描述|
|----|----|
|ECHO|响应使能|
|ECHOE|响应ETASE|
|ECHOK|响应KILL|
|ECHONL|响应’/n’|
|ICANON|规范输入|
|IEXTEN|扩展函数使能|
|ISIG|信号使能|
|NOFLSH|中断，停止或挂起后关掉flush|
|TOSTOP|为后台输出发送SIGTTOU|


●特殊的控制字符

 这些特殊的控制字符值在队列c_cc中定义，分为规范和非规范两种模式。

●波特率值

3 波特率函数

 函数原型：#include<termios.h>

speed_t cfgetospeed(const struct termios *termios_p);

int cfsetospeed (struct termios *termios_p,speed_t speed);

speed_t cfgetispeed(const struct termios *termios_p);

int cfsetispeed(struct termios *termios_p,speed_t speed);

 函数功能：以上这些接口被用来在termios结构获得和设定输入与输出的波特率值。

7．2基本的终端接口控制函数

1 获得并设定状态

 函数原型：#include<termios.h>

int tcgetattr(int fildes,struct termios *termios_p);

int tcsetattr(int fildes,int optional_actions,const struct termios * termios_p);

   函数功能：tcgetattr（）获得fildes所确定的文件的参数并将其存储在t’erops_p所指向的结构中；tcsetattr（）将设置参数。

 2行控制函数

  函数原型：#include<termios.h>

             int tcsendbreak(int fildes,int duration);

             int tcdrain(int fildes);

             int tcflush(int fildes,int queue_selector);

             int tcflow(int fildes,int action)’

  函数功能：如果终端使用异步连续数据传输，tcsendbreak（）引起在一段时间内连续的‘0’位传输；tcdrain（）等待直到输出传输完毕；tcflush（）和tcflow（）是溢出的相关处理。（参考第212页）

 3取得前台进程组的ID

  函数原型：#include<sys/types.h>

            pid_t tgetpgrp(int fildes);

    功能：

 4设置前台进程组ID

  函数原型：#include<sys/types.h>

             int tcsetpgrp(int fildes,pid_t pgrp_id);

  功能：如果进程支持控制终端，该函数设置与终端相关的前台进程组ID到pgrp_id。



第八章基于C语言的服务

8.1 参考的C语言规范

 1时间函数的扩展

 2 setlocale（）函数的扩展

   函数原型：#include<locale.h>

char *setlocale(int category,const char *locale);

  函数功能：该函数根据参数category和locale设置，更改或者查询进程现场。

8．2 C语言输入/输出函数

 1映射一个流指针到一个文件描述符

  函数原型：#include<stdio.h>

  函数功能：该函数返回一个与流相关的整型文件描述符。

 2根据一个文件描述符打开一个流

  函数原型：#include<stdio.h>

             FILE *fdopen(int fildes,const char *type);

  函数功能：该函数联合一个流和一个文件描述符。

 3其他文件类型函数之间的相互作用

      一个单一的文件描述说明可以通过流和文件描述符访问，流

或者是文件描述符被称作位打开文件的句柄，一个打开文件说明

可以有多个句柄。句柄可以在不影响重要的打开文件说明的情况

下被创建和删除，创建如fcntl().dup(),fdopen(),fileno(),fork();删除

如fclose(),close()。

（1）fopen（）随着open（）的调用分配描述符，基础函数为

open（）。

（2）fclose（）：该函数完成在与FILE流相关的，对文件描述符的close（）功能。

（3）freopen（）：具有fclose（）和fopen（）的特点。

（4）fflush（）：如果流可写或者缓存数据还没有写入文件时，

该函数标记下基础文件st_ctime和st_mtime的

值。

（5）fgetc（），fgets（），fread（），getc（），getchar（），gets

（），scanf（），fscanf（）：这些函数标记更新的st_atime值。

 基础函数是read（）和lseek（）。

（6）fputc(),fputs(),fwrite(),putc(),putchar(),puts(),printf(),fprintf():

从以上任一个函数的成功执行到下一个调用（在同一个流中的fflush（）或fclose（）或exit（）或abort（）），记下 更新的st_ctime和st_mtime值。基础函数是write（）和

 lseek（）。

（7） fseek（），rewind（）：如果流可写或者缓存数据还没有写

入文件时，该函数标记下文件更新的st_ctime和st_mtime值。基础函数是

lseek（）和write（）。

（8） perror（）：记下与标准错误流相关的文件。

（9） tmpfile（）：fopen（）执行后为文件分配文件描述符。

（10）ftell():基础文件是lseek（）。执行fflush（）后执行该

函数的结果与执行fflush前执行该函数的结果相同。

（11）报错

（12）exit（），abort（）：exit（）终止进程的时候要关闭流，

 abort（）只终止进程对流没有影响。

 4 文件操作-----remove（）函数

该函数和unlink（）函数的功能一样。

8．3其他的C函数

 1 非局部跳转

函数原型：#include<setjmp.h>

          int sigsetjmp(sigjmp_buf env,int savemask);

          void siglongjmp(sigjmp_buf env,int val);

函数功能：sigsetjmp（）宏要与标准中setjmp（）宏的定义一致，如果参数savemask不为“0”，sigsetjmp（）就要保存当前的信号标记作为调用环境的一部分。Siglongjmp（）同理。

 2 设置时间域

函数原型：#include<time.h>

          void tzset(void);

函数功能：该函数用环境变量TZ的值来设定时间变化信息。





第九章系统数据库

9．1系统数据库

 本章描述了两个数据库：组数据库和用户数据库。

 组数据库包括的信息有：组名，组的数字ID,组中的用户列表；

 用户数据库包含的信息有：用户名，用户的数字ID，组的数字ID,初始化的工作目录，和初始化的用户程序。

9．2数据库的访问

 1组数据库的访问

   函数原型：#include<sys/type.h>

              #include<grp.h>

              struct group *getgrgid(gid_t gid);

              struct group *getgrnam(const char *name);

   函数功能：getgrid（）和getgrnam（）返回指针，它指向一个struct

 group类型的对象，包含了组数据库的入口。

 2 用户数据库的访问

函数原型：#include<sys/types.h>

          #include <pwd.h>

          struct paswd *getpwuid(uid_t uid);

          struct passwd *getpwnam(const char *name);

函数功能：getpwuid（）和getpwnam返回一个指针，指向struct

 passwd类型的一个对象，它包含了用户数据库的入

口。





第十章数据交换形式

10．1归档/交换文件格式

   1 扩展的tar格式

   2扩展的cpio格式：面向字节的归档格式，包括了文件头，文件名和文件内容。

   3 大流量：该格式被看作是字节流。









第十一章同步

11．1信号量特征

    头文件<semaphore.h>定义了sem_t类型，它用于信号量操作中。sem_t代表了信号量，用文件描述符能够实现这些信号量，应用程序能够打开至少{OPEN_MAX}这么多的文件和信号量。标准中，头文件<semaphore.h>能使头文件<sys/types.h>和<fcntl.h>中的符号可见。

11．2信号量函数

 1初始化一个未命名的信号量

   函数原型：#inxlude<semaphore.h>

              int sem_init (sem_t *sem,int pshared,unsigned int value);

   函数功能：如果{_POSIX_SEMAPHORES}定义了：该函数被用

来初始化sem引用的未命名信号量。在成功调用该函

数以后，在接下来的sem_wait()，sem_trywait(),sem_post(),sem_destroy()的调用中，该信号量将被用到。如果参数pshared不为零，信号量将在进程中被共享，任何可以访问信号量sem的进程都可以使用sem。只有sem能被用来进行同步。如果参数pshared为零，则结构不确定。如果标准中未定义，可以由执行者来支持该函数。

2删除一个未命名信号量

 函数原型：#include <semaphore.h>

int sem_destroy(sem_t *sem);

 函数功能：如果{_POSIX_SEMAPHORES}定义了，该函数用来删除sem引用的未命名信号量，只有在sem_init（）中创建的信号量才能被该函数删除。

 3初始化/打开一个命名信号量

  函数原型：#include<semaphore.h>

             sem_t *sem_open(const char *name,int oflag,…);

  函数功能：如果{_POSIX_SEMAPHORES}定义了，该函数在进程和命名信号量之间创建一个链接。接着调用带有信号量名name的该函数，进程引用与name相关的信号量。该信号量在一些函数的调用中用到，如sem_wait(),sem_trywait(),sem_post,和sem_close()。信号量一直可用知道调用函数sem_close(),_exit,exec()关闭它。参数oflag控制是否信号量由sem_open（）创建或者仅被它访问。

 4关闭一个命名信号量

  函数原型：#include<semphore.h>

             int sem_close(sem_t *sem);

  函数功能：如果{_POSIX_SEMAPHORES}定义了，该函数用来提示调用进程已经完成使用sem所指明的信号量。该函数释放系统资源，这些资源被拥有该信号量的进程占有。

 5移走一个命名信号量

  函数原型：#include<semaphore.h>

             int sem_unlink(const char *name);

  函数功能：该函数将移走被字符串name命名的信号量。如果该信号量当前被其他进程引用，则该函数对信号量状态没有影响。如果一个和更多的进程打开了该信号量，则销毁该信号量被延迟知道所有的信号量被sem_close()，_exit(),exec关闭。

 6锁定一个信号量

  函数原型：#include<semaphore.h>

             int sem_wait (sem_t*sem);

             int sem_trywait(sem_t *sem);

  函数功能：sem_wait()锁定sem引用的一个信号量，对该信号进行锁定操作。如果信号量为“0”，调用进程将不会返回直到锁定了这个信号量或者被一个信号中断。Sem_trywait()只能在信号量当前没被锁定的情况下锁定它，否则将不会锁定信号量。成功返回，信号量的状态将被锁定直到sem_post()被调用并且成功返回。

 7解锁一个信号量

  函数原型：#include<semaphore.h>

             int sem_post(sem_t *sem);

  函数功能：该函数通过对一个信号量的解锁操作解锁一个被sem引用的信号量。如果该操作的结果，信号量值为正数，则没有进程被锁定来等待一个信号量解锁，信号量值是单一的增加。如果信号量值为“0”，则进程被锁定来等待一个信号量被允许从sem_wait()成功返回。

 8取得一个信号量值

  函数原型：#include<semaphore.h>

             int sem_getvalue(sem_t *sem,int *sval);

  函数功能：该函数更新参数sval所引用的位置，在不改变信号量状态的情况下得到信号量值。更新的值代表了一个确切的值，它产生在调用中的一个不定时刻，但它返回给调用进程是不一定需要信号量的确定值。如果sem被锁定，则sem_getvalue（）返回的值为0，或者负数，它代表了等待信号量的进程数。





                第十二章 内存管理

该章节描述了进程内存锁定，内存映象文件，和共享内存工具。

内存锁定和内存映象文件以页的方式定义。执行者可以据据页大小的规范限制和排列锁定和映象范围。页的大小，以字节为单位，是一个可配置系统变量。默认为1B。

内存锁定保证了一部分地址空间的驻留。

12．1内存锁定函数

 1 锁定/解锁进程地址空间

   函数原型：#include<sys/mman.h>

int mlockall(int flags);

int munlockall(void);

      函数功能：mlockall（）使进程地址空间所映射的所有页面成为内存驻留区，直到解锁或者进程退出或者execs另一个进程映象。参数flags决定了是否被锁定的页面是由当前，（将来，或者两者都是）的进程地址空间映射的。Munlockall（）解锁当前所有的进程地址空间映射页面。所有映射到进程地址空间的页面，调用了该函数，将不会被锁定，除非有中断调用mlockall()确定MCL_CURRENT，或者并发调用mlockall()确定MCL_CURRENT。

 2锁定/解锁一连续的地址空间

  函数原型：#include<sys/mman.h>

             int mlock(const void *addr,size_t len);

             int munlock(const void *addr,size_t len);

  函数功能：mlock（）使一个范围的进程地址空间成为内存驻留

区（addr决定了空间的起始地址，len决定了空间长

度）直到解锁或者进程退出或者execs另一个进程映

象；munlock（）解锁一个范围的进程地址空间（addr

决定了空间的起始地址，len决定了空间长度);

12.2 内存映象函数

 1 映象进程地址到一个内存目标

   函数映象：#include<sys/mman.h>

             void *mmap(void *addr,size_t len,int prot,int flags,int fildes,off_t off);

  函数功：该函数在一个进程的地址空间和内存对象间创建一个映

射，调用格式如：pa=mmap(addr,len,prot,flags,fildes,off);

pa代表进程进程地址空间的地址，由执行者定义（addr

和flags），len表示空间长度，fildes代表内存对象，off

表示偏移量。函数成功调用返回pa，空间地址范开始

与pa，长度为连续的len个字节；内存对象开始与off，

长度为len字节。参数prot决定了读，写，执行，或一

些映射数据的访问活动的集合。

 2 取消以前映射地址的映射关系

   函数原型：#include<sys/mman.h>

int munmap(void *addr,size_t len);

   函数功能：该函数移去任何包含有进程地址空间的页面的映射关

系，该进程地址空间起始于addr，长度为len字节。

 3 改变内存保护

   函数原型：#include <sys/mman.h>

int mprotet(const void *addr,size_t len,int prot);

  函数功能：该函数更改访问的保护活动，由参数prot确定。访问对象是一部分进程地址空间，起始于addr，长度为len字节。

 4内存对象同步

  函数原型：#inxlude<sys/mman.h>

             int msync(void *addr,size_t len,int flags);

  函数功能：该函数将所有更改了的数据写到拥有存储的地方，它包含了进程地址空间，起始于addr，长度为len字节。如果没有这样的存储区域存在，则给函数没有作用。该函数须保证写操作的完成符合所定义的同步I/O数据的一致完成性。参数flags确定了写的同步/异步性。

12．3共享内存函数

 1打开一个共享内存对象

  函数原型：include<sys/mman.h>

             int shm_open(const *name,int oflag,mode_t mode);

  函数功能：该函数在共享内存对象和文件描述符之间创建了一个连接。它创建了一个打开文件描述涉及到共享内存对象，并且创建了一个文件描述符指向这个打开文件描述。这个文件描述符可悲其他函数用来指向共享内存对象，参数name指向共享内存对象名。如函数调用成功，则返回一个共享内存的文件。文件状态和文件访问模式根据oflag的值来设定。

 2移去一个共享内存对象

  函数原型：int shm_unlink(const char *name);

  函数功能：该函数移去由name命名的共享内存对象。如果一个或者多个对共内存的引用在对象断开连接的同时退出的话，这个名字应在shim_unlink()返回之前移走，但是对内存对象的移走须延迟到所有对共享内存对打开和映射引用被移去后。





                 十三章 执行调度

13.1调度参数

  一个调度参数结构sched_param包括了调度策略所支持的执行者

所需要的调度参数，它在头文件<sched.h>中定义。执行者可根据规

对该结构进行扩展。

13.2 调度策略

 该章所阐述的调度术语是从概念模型上定义的，它包含了一组进程列表。这个模型只讨论了可运行进程的处理器调度，但是它注重了在其他资源考虑到处理器调度策略的情况下，增强了实时操作的可预见性。在这里，概念上讲是一个进程列表一个策略。调度策略的目的就是对这一组列表定义允许的操作（例如，在进程列表之中和之间移动进程）。每一个进程应由相关的调度策略和优先级，于每一个策略相关的是一个优先级范围。

 1 SCHED_FIFO

    该策略是一种先进先出的调度策略。如果正在执行的进程是被抢占的进程，则他应该在进程列表头；如果一个锁定的进程变成可执行进程，它就进入进程列表尾；当执行进程调用sched_setscheduler()，则函数中确定的进程被改为指定的策略；当执行进程调用sced_setparam()，进程的优先级根据参数param被修改，如果改进程是正在执行和可执行，则它将进入进程列表尾；当正在执行的进程调用seced_yield()，进程进入列表尾。

 2 SCHED_RR

    该策略与上面的策略不同的是，如果执行中检测到进程的执行时间已经到达或超过了sched_rr_get_interval()所返回的时间片，进程就进入列表尾并且进程列表头移动一个进程进入执行状态。

 3 SCHED_OTHER

 13.3进程调度函数

   1设置调度参数

    函数原型：#include<sched.h>

               int sched_setparam(pid_t pid,const struct sched_param *param);

    函数功能：pid指明了进程，param指向了sched_param结构，该结构设定了进程的调度参数。如果pid＝0，调度参数为调用进程设定。如果pid指定的进程的优先级高于正在指向的进程并且该进程可执行，则抢占现在正在运行的进程。如果当前的策略不是前面将的三种方式，则由执行者定义。

 2取得调度参数

  函数原型：#include<sched.h>

             int sched_getparam(pid_t,struct sched_param *param);

  函数功能：该函数返回调度参数，如果一个pid进程退出并且调用进程允许，则ID等于pid的进程返回其调用参数。如果pid＝0，则返回调用进程的调度参数。

 3设置调度策略和调度参数

  函数原型：#include<sched.h>

             int sched_setscheduler(pid_t pid,int policy,const struct sched_param *param);

  函数功能：该函数设置进程的调度策略和调度参数，pid表示进程，policy指明策略，参数param指向的sched_param结构指明了调度参数。执行者可要求请求进程能允许设定自己或其他进程的调度参数。

 4取得调度策略

  函数原型：#include<sched.h>

             int sched_getscheduler(pid_t pid);

  函数功能：该函数返回pid进程的调度策略，其返回值在头文件<sched.h>中定义。

 5放弃处理器

  函数原型：#include<sched.h>

             int sched_yield(void);

  函数功能：该函数迫使正在执行进程放弃处理器直到重新进入进程列表头。

 6获得调度参数范围

  函数原型：#incude<sched.h>

             int sched_get_priority_max(int policy);

             int sched get_priority min(int policy);

             int sched_rr_get_interval(pid_t pid,struct timespec *interval);

  函数功能：sched_get_priority_max()和sched_get_priority_min()返

回policy调度策略相应的最大最小值。Sched_rr_get

 _interval()更新interval参数引用的timespec结构，包

含了当前进程执行的时间限制。





第十四章时钟和定时器

14．1时钟和定时器的数据定义

头文件<time.h>定义了时间设备用到的类型和主要常量。

 1 时间值的规格结构

   一个时间值结构timespec确定了单一时间值并且包括了以下值：



|成员类型|成员名|描述|
|----|----|----|
|time_t|tv_sec|seconds|
|long|tv_nsec|nanosenconds|


执行者可对他做扩展，tv_nsed成员要大于等于零时才可用。

时间值结构itimerspec确定了一个初始化的定时器和各进程定时器函数用到的重复间隔。结构包括：

|成员类型|成员名|描述|
|----|----|----|
|Struct timespec|It_interval|Timer period|
|Struct timespec|It_value|Timer expiration|


执行者也可以对其做扩展。

2 定时器活动唤醒控制块

 为了使完成支持实时信号扩展功能，各进程定时器被创建通过排列实时扩展信号来通知定时器超时的进程。Sigevent结构在头文件<signal.h>中定义，用来创建一个这样的定时器。

3 类型定义

 在<sys/types.h>中定义。

4 主要常量

   在<time.h>中定义：

        CLOCK_REALTIME 系统范围的实时时钟标志

        TIMER_ABSTIME    flag指定的时间对于时钟是“完全”的

14．2时钟和定时器函数

 1 时钟

   函数原型：#include<time.h>

             int clock_settime(clockid_t clock_id,const struct timespec *tp);

             int coock_gettime(clockid_t clock_id,struct timespec*tp);

             int clock_getres(clockid_t clock_id,struct timespec *res);

  函数功能：clock_settime()设置特定时钟，clock_id,到tp指定的值。

            Clock_gettime()返回当前的tp值，即时钟，clock_id；

           调用clock_getres()可以得到时钟的决定值，该值由执行

           者定义而不由进程设定。如果res不为空，则该值被存

           储在res指向的地方，如果res为空，则时钟决定值不

被返回。一个时钟可以是系统范围的，对于所以的进程可见，所有的执行者要支持CLOCK_REALTIME的clock_id。对于这个时钟。Clock_gettime()返回的和clock_settime()确定的值代表了从一个时间点开始的时间长度，执行者也可以支持附加的时钟。借助clock_settime()设定时钟的影响由执行者定义。设置特殊时钟的权限也有执行者定义。

 2 创建一个总进程定时器

   函数原型：#include<signal>

#include<time.h>

int timer_create(clockid_t clock_id,struct sigevent

*evp,timer_t *timerid);

  函数功能：timer_create()创建一个总进程定时器，用来指明时钟，clock_id,作为计时基础。该函数在timerid指向的地方返回一个timer_t类型的定时器ID，该ID在调用进程中必须是唯一的直到定时器被删除。参数evp如果不为空，则指向一个sigevent结构，定义了定时器超时时出现的异步通知。结构中sigev_notify为SIGEV_SIGNAL时，结构就包括了超时时送往进程的信号数和应用数据；如果sigev_notify为SIGEV_NONE,则不发送任何通知；sigev_notify其他的值由完成者自行定义。总进程的定时器不被其子进程通过fork（）继承，它应该被exec撤销。

 3删除一个总进程定时器

   函数原型：#include<time.h>

int timer_delete(time_t timerid);

  函数功能：该函数删除一个指定的定时器，timerid，该定时器是在以前的timer_create（）中创建的。

 4总进程定时器

  函数原型： #include<time.h>

              int timer_settime(timer_t timerid,int flags,const struct itimerspec *value,struct itimerspec *ovalue);

             int timer_gettime(timer_t timerid,struct itimerspec *value);

int timer_getoverrun(timer_t timerid);

  函数功能：timer_settime()设置时间直到下一个timerid指定的定时器终止。如果该函数调用时定时器已经被装备，该调用则重置时间直到value确定的下一次终止。如果value的it_value成员为零，定时器被解除。如果flag没有设定为TIMER_ABSTIME,则定时器从调用开始在it_value十亿分之一秒内超时；如果设定为TIMER_ABSTIME,该函数表现为时间直到下一次超时被设定为it_value指定的绝对时间和与timerid相联的时钟值的差值。定时器的再装由value的it_interval成员值来设定。如果参数ovalue不为空，该函数被存储在ovalue引用的地方。该函数要存储时间累计值直到timerid指定的定时器终止或重装定时器值到value参数。在任一时刻，仅有一个单一信号被派了在指定的定时器的进程中。如果支持实时信号扩展，timer_getoverrun()返回定时器终止的溢出值。

 5高度睡眠

  函数原型：#include<time.h>

             int nanosleep(const struct timespec *rqtp,struct timespec *rmtp);

  函数功能：该函数使当前执行的进程挂起直到参数rptp指定的时

间间隔到达或者信号被送到调用进程并且其行为就

是唤醒信号跟踪功能或者使终止进程。挂起时间也许

要比要求时间长是因为参数值向上舍入成一个整数或者是因为系统对其他活动的调度。但是除非被信号打断，挂起时间不会少于tqtp值，这是系统时钟，CLOCK_REALTIME测量的。





第十五章消息传递

15．1消息队列的数据定义

 1 数据结构

头文件< mqueue.h>定义了以下执行者定义的类型：

    mqd_t ：用于消息队列描述符

头文件< mqueue.h>定义了以下执行者定义的结构：

    struct sigevent： 3.3中定义。mq_attr用来设定消息队列参数。

struct sigevent结构包含了至少以下的成员：

|类型|成员名|描述|
|----|----|----|
|long|mq_flags|消息队列标志|
|long|mq_maxmsg|最大消息数|
|long|mq_msgsize|消息的最大长度|
|long|mq_curmsgs|当前排列的消息数|


执行者可作扩展。

15．2消息传递函数

 1 打开消息队列

函数原型：#include<mqueue.h>

          mqd_t mq_open(const char *name,int oflag,…);

函数功能：该函数在进程和消息队列之间建立连接。它创建了一个消息队列描述符指向消息队列。参数oflag请求对消息队列发送或接收所需信息。如果调用进程承认对相应保护文件的读写请求，则对接收或发送消息的请求允许被通过。

 2 关闭一个消息队列

   函数原型：#include<mqueue.h>

int mq_close(mqd_t mqdes);

  函数功能：该函数撤销消息队列描述符（mqdes）和消息队列之间的关系。如果进程成功的配属了mqdes表示的消息队列通知请求，则这个配属关系被撤销，该消息队列可用于其他进程来配属通知。

 3 移去一个消息队列

   函数原型：#include<mqueue.h>

int mq_unlink(const char *name);

  函数功能：该函数移去路径名name指向的消息队列。如果该调用成功，并且flag没有设为O_CREATE，则mq_open()对于同一个name将会失败。所有对该队列的引用撤销后，该队列才能被关闭。该函数调用不会被阻塞指定所有的引用被关闭。

 4 发送一个消息到消息队列

   函数原型：#include<mqueue.h>

             int mq_send(mqd_t mqdes,const char *msg_ptr,size_t mag_len,unsigned int msg_prio);

  函数功能：该函数添加参数msg_ptr指向的消息到mqdes指定的

消息队列中去。参数msg_len表示消息长度，以字节

为单位，该参数应小于等于消息队列的mq_msgsize

参数，否则调用失败。如果消息队列没有满，则该函

数的功能就是插入消息到消息队列的指定位置，这个

位置邮msg_prio参数指定。msg_prio大者先插入队

列，msg_prio的值应小于等于{MQ_PRIO_MAX}。

如果消息已满并且O_NONBLOCK没有设定，该函

数阻塞一直到空间可用或者mq_send（）被信号中断。

如果空间可用时，多于一个进程在等待发送则按优先

级，等待最久的进程先发送它的信息。如果O_NON

BLOCK被设定，并且队列已满，则函数返回error。

 5从消息队列接受一条消息

  函数原型：#include<mqueue.h>

             ssize_t mq_receive(mqd_t mqdes,char *msg_ptr,size_t msg_len,unsigned int *msg_prio);

  函数功能：该函数接受mqdes确定的消息队列中最久优先级最高

的消息，对参数值的限制同上。所选消息从队列移出，

复制到msa_ptr指向的缓冲区。如果参数msg_prio不

为空，则指定消息被存储在msa_prio所引用的地方。

如果消息队列为空，并且O_NONBLODK没有设定，

该函数阻塞直到消息排列入队列中或者该函数被信

号中断。当有多个进程在消息到达队列后请求接受，

则优先级原则与上相同。

 6通知一个进程队列中的一条信息可用

  函数原型：#include<mqueue.h>

             int mq_notify(mqd_t mqdes,const struct sigevent *notification);

  函数功能：如果参数notification不为空，函数记录下调用进程被

通知空的消息队列（由消息队列描述符mqdes相联）

中有一条消息到来。当消息队列从空到非空时，一条

通知消息将发送到进程中，在任一时刻，只有一个通

知消息被一个消息队列记录。如果notifiction为空并

且进程当前被指定的队列记录，则已存在的记录被移

去。

 7设置消息队列参数

  函数原型：#include <mqueue.h>

             int mq_setattr(mqd_t mqdes,const struct mq_attr *mqstat,struct mq_attr *omqstat);

  函数功能：该函数用来设置与mqdes指定的消息队列相关的参数。

mq_attr结构中的mq_maxmsg,mq_magsize,mq_curmsgs

成员被mq_setattr()忽略。如果omqstat为非空，则该函

数被存储，由omqstat指明存储位置。

 8得到消息队列参数

  函数原型：#include<mqueue.h>

             int mq_getattr(mqd_t mqdes,struct mq_attr *mqstat);

  函数功能：该函数用来取得状态信息和与mqdes消息队列相关的

参数。﻿﻿


﻿﻿


﻿﻿



