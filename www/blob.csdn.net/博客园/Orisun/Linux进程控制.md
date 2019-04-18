# Linux进程控制 - Orisun - 博客园







# [Linux进程控制](https://www.cnblogs.com/zhangchaoyang/articles/1942453.html)





_exit()函数的作用是:直接使进程停止运行,清除其使用的内存空间, 并清除其在内核中的各种数据结构;exit()函数则在这些基础上作了一些包装, exit()函数在调用 exit 系统之前要检查文件的打开情况,把文件缓冲区中的内容写回文件。

wait 函数是用于使父进程(也就是调用 wait 的进程)阻塞,直到一个子进程结束或者该进程接到了一个指定的信号为止。如果该父进程没有子进程或者他的子进程已经结束, wait 则就会立即返回。

waitpid 的作用和 wait 一样, 但它并不一定要等待第一个终止的子进程, 它还有若干选项, 如可提供一个非阻塞版本的 wait 功能,也能支持作业控制。实际上 wait 函数只是 waitpid 函数的一个特例,在 Linux 内部实现 wait 函数时直接调用的就是 waitpid 函数。

#include <sys/types.h> 

#include <sys/wait.h> 

pid_t wait(int *status) 

status 是一个整型指针,是该子进程退出时的状态

• status 若为空,则代表任意状态结束的子进程

• status 若不为空,则代表指定状态结束的子进程

另外,子进程的结束状态可由 Linux 中一些特定的宏来测定



#include <sys/types.h> 

#include <sys/wait.h> 

pid_t waitpid(pid_t pid, int *status, int options) 

pid参数选项：

pid>0:只等待进程 ID 等于 pid 的子进程,不管已经有其他子进程运行结束退出了,只要指定的子进程还没有结束,waitpid 就会一直等下去

pid=−1:等待任何一个子进程退出,此时和 wait 作用一样

pid=0:等待同组内的任一进程退出

pid<−1:等待其组 ID 等于 pid 的绝对值的任一子进程

options参数选项：

WNOHANG:若由 pid 指定的子进程不立即可用,则 waitpid 不阻塞, 此时返回值为 0 

WUNTRACED:若实现某支持作业控制,则由 pid 指定的任一子进程状态已暂停,且其状态自暂停以来还未报告过,则返回其状态

0:同 wait,阻塞父进程,等待子进程退出



守护进程,也就是通常所说的 Daemon（许多进程名称末尾的d代表Daemon的意思） 进程，常常在系统引导装入时启动,在系统关闭时终止。

每个进程组都有一个组长进程,其组长进程的进程号等于进程组 ID。且该进程 ID 不会因组长进程的退出而受到影响。

一个会话开始于用户登录（即打开一个Shell）,终止于用户退出, 在此期间该用户运行的所有进程都属于这个会话期。

由于创建的进程都依赖于特定的控制台，控制台退出后进程也就终止了，为突破此界限，就要把该进程变为守护进程。

创建守护进程的一般步骤：

一、创建子进程，父进程退出

pid=fork();

　　if(pid>0){

 　　exit(0);

}

子进程变成孤儿进程后1号进程（即init进程）就会收养它。

父进程退出后在Shell中造成这一程序已退出的假象，Shell继续执行其他命令。

二、子进程中创建新会话

 setsid 函数用于创建一个新的会话,并担任该会话组的组长。调用 setsid 有下面的 3 个作用: 

• 让进程摆脱原会话的控制。

• 让进程摆脱原进程组的控制。

• 让进程摆脱原控制终端的控制。

由于在调用 fork 函数时, 子进程全盘拷贝了父进程的进会话期、进程组、控制终端等,虽然父进程退出了,但原先的会话期、进程组、控制终端等并没有改变,因此,还不是真正意义上独立开来,而 setsid 函数能够使进程完全独立出来,从而脱离所有其他进程的控制。

#include <sys/types.h> 

#include <unistd.h> 

pid_t setsid(void) 

成功:该进程组 ID 

出错:−1 

完成该步后守护进程已经与所属的控制终端失去了联系，stdin、stdout、stderr已不再起作用。

三、改变当前目录为根目录

使用 fork 创建的子进程继承了父进程的当前工作目录。由于在进程运行过程中,当前目录所在的文件系统(比如“/mnt/usb”等)是不能卸载的,这对以后的使用会造成诸多的麻烦(比如系统由于某种原因要进入单用户模式) 。因此,通常的做法是让“/”作为守护进程的当前工作目录,这样就可以避免上述的问题,当然,如有特殊需要, 也可以把当前工作目录换成其他的路径,如/tmp。改变工作目录的常见函数是 chdir。

四、重新设置文件掩码

由于使用 fork 函数新建的子进程继承了父进程的文件权限掩码,这就给该子进程使用文件带来了诸多的麻烦。因此,把文件权限掩码设置为 0,可以大大增强该守护进程的灵活性。设置文件权限掩码的函数是 umask。在这里,通常的使用方法为 umask(0)。

五、关闭文件描述符

用 fork 函数新建的子进程会从父进程那里继承一些已经打开了的文件。这些被打开的文件可能永远不会被守护进程读或写,但它们一样消耗系统资源,而且可能导致所在的文件系统无法卸下。

```
#include<stdio.h>
#include<sys/wait.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>

#define MAXFILE 65535

int main()
{
    pid_t pid;
    char *buff = "This is dameon process!\n";
    int len, i;
    int fd;
    pid = fork();        //创建子进程
    if (pid < 0) {
        perror("fork");
        exit(-1);
    } 
    else if (pid > 0)
        exit(0);        //子进程还未退出，父进程先退出
    setsid();        //子进程中创建新会话
    chdir("/tmp");        //切换目录
    umask(0);        //消除文件掩码
    for (i = 0; i <= MAXFILE; i++)        //关闭所有的文件描述符
        close(i);
    /*在守护进程中要完成的工作 */
    len = strlen(buff);
    while (1) {
        if ((fd =open("dameon.log", O_CREAT | O_WRONLY | O_APPEND,0600)) < 0) {
            perror("open");
            exit(1);
        }
        write(fd, buff, len + 1);
        close(fd);
        sleep(1);
    }
    return 0;
}
```

该程序在Shell中执行后立即返回。

ps -ef|grep defunc可以找出僵尸进程.

orisun 1228 1197 0 14:01 ? 00:00:00 [fcitx] <defunct>
orisun    3571  3552  0 17:34 pts/0    00:00:00 grep --color=auto defunc

$tail -f  /tmp/dameon.log

可以看到每隔一秒文件末尾就增加一行"This is dameon process!“



清除ZOMBIE（僵尸）进程可以使用如下方法： 
1） kill –18 PPID （PPID是其父进程） 
这个信号是告诉父进程，该子进程已经死亡了，请收回分配给他的资源。 
2）如果不行则看能否终止其父进程（如果其父进程不需要的话）。先看其父进程又无其他子进程，如果有，可能需要先kill其他子进程，也就是兄弟进程。方法是： 
kill –15 PID1 PID2(PID1,PID2是僵尸进程的父进程的其它子进程)。 
然后再kill父进程：kill –15 PPID



由于守护进程完全脱离了控制终端,那守护进程如何调试呢？一种通用的办法是使用 syslog服务,将程序中的出错信息输入到“/var/log/messages”系统日志文件中,从而可以直观地看到程序的问题所在。

“/var/log/message”系统日志文件只能由拥有 root 权限的超级用户查看。


Syslog 是 Linux 中的系统日志管理服务,通过守护进程 syslogd 来维护。该守护进程在启动时会读一个配置文件“/etc/syslog.conf”。该文件决定了不同种类的消息会发送向何处。例如,紧急消息可被送向系统管理员并在控制台上显示,而警告消息则可记录到一个文件中。



#include <syslog.h>

void openlog (char *ident,int option ,int facility)

ident要向每个消息加入的字符串,通常为程序的名称



facility:指定程序发送的消息类型



void syslog( int priority, char *format, ...)  向日志文件写入内容



void closelog( void )













