# Linux C语言编程学习笔记 (1)进程控制入门 - 在思索中前行！ - CSDN博客





2014年08月16日 21:25:15[_Tham](https://me.csdn.net/txl16211)阅读数：973标签：[Linux C](https://so.csdn.net/so/search/s.do?q=Linux C&t=blog)
个人分类：[linux学习](https://blog.csdn.net/txl16211/article/category/2231071)









    想进行Linux系统开发已经很久了，一直没有付诸实践。今日终于开始学习Linux下的C语言编程，研究一天，终于大概弄明白了Linux系统进程管理的一些基本概念和编程方法，总结下来以方便大家学习和自己实践。

## 进程系统

Linux是个多任务多用户的操作系统，系统直接管理的每个任务的最小单位，就是**进程(process)**。每个进程都有一个惟一的标识符pid，不同的进程pid不相同，在Shell下输入ps -A，可以显示当前的所有进程。一个进程不代表一个**应用程序(application)**，因为一个应用程序可能对应多个进程，也不代表一个**可执行文件(executable file)**，因为一些可执行文件可以被同时运行多个，它们是互不相干的。

在Linux中，进程不是相互独立的，每个进程（除了init进程）都有一个**父进程(parent process)**，同时每个进程可以有0个1个或多个**子进程(child process)**。换句话说，Linux的进程是一个树形结构，在Shell下输入pstree可以查看这个树的形状。下图为pstree返回结果的一部分。

```matlab
init─┬─NetworkManager─┬─dhclient
│                └─{NetworkManager}
├─SystemToolsBack
├─avahi-daemon───avahi-daemon
├─bonobo-activati───{bonobo-activati}
├─console-kit-dae───63*[{console-kit-dae}]
├─hald───hald-runner─┬─hald-addon-acpi
│                    ├─hald-addon-cpuf
├─pulseaudio─┬─gconf-helper
│            └─2*[{pulseaudio}]
├─rsyslogd───2*[{rsyslogd}]
├─seahorse-daemon
├─telepathy-gabbl
├─telepathy-haze─┬─telepathy-haze
│                └─{telepathy-haze}
├─trashapplet
└─wpa_supplicant
```

在C语言中，获得当前进程的pid的函数是pid_t getpid(void);，获得当前进程的父进程的pid的函数是pid_t getppid(void);，两者都在unistd.h中声明。

## 用户和权限

因为Linux是多用户的系统，所以内核中有着强大的用户控制，因此每个进程还有一个所有者，即**实际用户ID(uid)**。系统uid是一个整数，不同于用户名。默认情况下进程的uid继承于父进程。例如我用所有者为byvoid(uid为1000)的bash终端启动了一个进程，那么这个进程的uid也是1000。用户uid可以通过uid_t getuid(void);函数获得。如果权限满足，程序在运行时可以修改uid，C语言函数为int
 setuid(uid_t uid);，如果成功执行返回0，否则返回-1。只有具有root用户权限的进程可以设置uid。

除此以外，进程还有一个**有效用户ID(euid)**。euid是决定进程文件系统权限的身份，一般情况下进程euid和uid是相同的。在C语言中可以通过uid_t geteuid(void);函数获得进程euid。同样euid也可以修改，函数为int seteuid(uid_t uid);仅当当前uid和euid中至少有一个为0(root)时，才可以设置euid。有一种特殊情况，就是一个二进制可执行文件所有者为root，并且被chmod
 +s后，在一般用户身份下执行，这时产生的进程uid为一般用户，而euid为0(root)，这种情况下该进程具有和root一样高的权限。

## 进程生成

### fork函数

Linux允许用户创建用户进程的子进程，在C语言中通过pid_t fork(void);函数实现。fork函数的基本功能是生成一个子进程，并复制当前进程的数据段和堆栈段，子进程和父进程共用代码段。因为复制了堆栈段，所以父进程和子进程都停留在fork函数的栈帧中，fork函数要返回两次，一次在父进程中返回，一次在子进程中返回，但是两次的返回值是不一样的。在父进程中，fork函数返回值为子进程的pid（如果成功调用的话），在子进程中，fork函数的返回值为0。因此可以根据返回值的不同确定程序的运行流程。父进程和子进程默认情况下是同步执行的，由系统内核调度，哪个先执行是未知的。因为父子进程的数据段和堆栈段都是独立的，所以两者互不干涉，各行其是，内存不能直接共享。

### 执行程序

Linux中要执行一个外部程序，必须生成一个子进程，因为内核执行程序的命令exec会替换掉当前进程的地址空间的所有内容并继续执行，执行另一个程序意味着当前程序不再执行。在C语言中，并没有exec这样的一个函数，而是有下列一组函数。

```
int execl (const char * file,const char * arg,...);
int execlp(const char * file,const char * arg,...);
int execle(const char * file,const char * arg,...,NULL,char * const envp[ ]);
int execv (const char * file, char * const argv[ ]);
int execvp(const char *file ,char * const argv []);
int execve(const char * filename,char * const argv[ ],char * const envp[ ]);
```

其中以execl开头的函数，第一个参数file为可执行文件名，接下来有若干个参数，分别为传入的argv[0],argv[1],argv[2],...，最有以NULL结束。如果file参数为路径名(其中包含'/')，execl函数会直接定位到文件并执行，否则仅在当前目录下寻找文件，而execlp函数遇到文件名则会按照环境变量PATH的顺序寻找。execle最后一个参数为二维字符数组，表示传递给程序新的环境变量列表。execv，execvp，execve和前三者用法相似，只不过不以可变参数列表的方式传递参数，换以二维字符数组。上述函数执行失败后会返回-1，如果执行成功的话将会不返回，因为代码段已经被新的可执行程序替换。

## 进程阻塞

### wait函数

在实际的应用中，有时候需要让父进程停下来等待子进程的执行完毕，这时候就需要进行**进程阻塞(process blocking)**。C语言中使用pid_t wait(int *statloc)函数可以得到子进程的结束信息。调用wait函数的进程会阻塞，直到该进程的任意一个子进程结束，wait函数会返回结束的子进程的pid，结束信息保存在statloc指针指向的内存区域。如果该进程没有活动的子进程，那么立即出错并返回-1。如果statloc指针为NULL，那么表示不关心进程结束的状态。如果有多个子进程，wait函数返回哪个数不确定的，需要通过pid来判断。

如果我们需要等待特定的一个进程，可以使用pid_t waitpid(pid_t pid,int *statloc,int options)函数。waitpid函数的第一个参数指定了要等待的进程pid，并且有更多的选项。

### 僵尸进程

当一个子进程退出时，如果没有被父进程通过wait取得状态信息，这些信息会一直保留在内核内存中，子进程的pid也不会被消除，直到父进程退出，这时候这些子进程被称为**僵尸进程（zombie process）**。虽然僵尸进程只占用很少的一点内存，但如果是长期运行的服务器，积累大量的僵尸进程会导致系统进程表被塞满，以至于无法创建新的进程。产生一个僵尸进程很容易，只需要让子进程先于父进程退出即可，在父进程退出之前，子进程将会成为僵尸进程。

### 孤儿进程

与僵尸进程相反，如果父进程没有阻塞并先于子进程退出，那么子进程将会成为**孤儿进程(orphan process)**。Linux系统中init进程负责领养所有孤儿进程，也就是说，孤儿进程的父进程会被设为init进程。init进程作为系统**守护进程(daemon process)**，会不断调用wait函数等待领养的孤儿进程退出，不会产生僵尸进程。

### 利用孤儿进程避免僵尸进程

许多时候我们不能让父进程阻塞下来等待子进程处理完以后再继续，例如在多用户的服务器程序上。这时如果让子进程处理事务，就会产生大量僵尸进程。避免僵尸进程出现的一个经典方法就是利用孤儿进程，具体方法为首先用父进程产生一个子进程，然后让子进程立刻产生一个孙子进程，用孙子进程来处理事务。同时父进程阻塞等待，并让子进程则立刻退出。这时候由于子进程已经退出，孙子进程就变成了孤儿进程，被init领养。而子进程立刻退出后，父进程收到信号并正确销毁了子进程，相比之下父进程只阻塞了可以忽略不计的一瞬间。下面程序是一个例子避免僵尸进程。

```php
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    if(fork() == 0) //启动一个子进程
    {
        printf("the child\n");
        if(fork() == 0) //启动一个孙子进程
        {
            printf("do something you want\n");
            sleep(5);
            printf("done\n");
            exit(0);
        }
        else //子进程立刻退出
            exit(0);
    }
    else
    { //父进程立即阻塞
        wait(NULL);
        printf("the parent\n");
        sleep(10);
    }
    return 0;
}
```



