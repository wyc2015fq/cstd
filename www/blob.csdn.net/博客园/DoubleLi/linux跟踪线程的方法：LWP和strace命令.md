# linux跟踪线程的方法：LWP和strace命令 - DoubleLi - 博客园






**摘要：**在使用[多线程](http://www.07net01.com/tags-%E5%A4%9A%E7%BA%BF%E7%A8%8B-0.html)[程序](http://www.07net01.com/tags-%E7%A8%8B%E5%BA%8F-0.html)时，有时会遇到程序功能异常的情况，而这种异常情况并不是每次都发生，很难模拟出来。这时就需要运用在程序运行时跟踪线程的手段，而[linux](http://www.07net01.com/linux/)系统的LWP和strace命令正是这种[技术](http://www.07net01.com/tags-%E6%8A%80%E6%9C%AF-0.html)手段。本文对LWP和strace命令做了简明扼要的介绍，并通过一个实例来说明如何运用。总而言之，LWP和strace的使用可以提高多线程程序的可维护性。

**问题描述：**

我们来看一个问题：程序tcp_client同时创建多个线程向同一个[服务器](http://www.07net01.com/tags-%E6%9C%8D%E5%8A%A1%E5%99%A8-0.html)发送数据，每个线程发送不同类型的数据，服务器接收数据后，可以通过界面查看到多个数据在刷新。程序运行一段时间后，突然出现只有某个类型的数据不刷新，其他数据刷新正常，但是[服务端](http://www.07net01.com/tags-%E6%9C%8D%E5%8A%A1%E7%AB%AF-0.html)和[客户端](http://www.07net01.com/tags-%E5%AE%A2%E6%88%B7%E7%AB%AF-0.html)程序都没有报错。这时我们该怎么办？

很明显，这时程序调试者需要知道不刷新线程的线程ID，然后通过某种手段查看该线程为何出现了异常。通过linux的ps -eLF|grep pid命令可以查看进程的线程ID（LWP号），但是问题在于如果我们没有在创建线程时记录其线程ID，我们仍然无法得知哪个LWP号是我们想要追踪的。

**我们来看一下LWP号是什么，以及如何在创建线程时记录每个线程的LWP号。**

pthread_create是基于clone实现的, 创建出来的其实是进程, 但这些进程与父进程共享很多东西, 

共享的东西都不用复制给子进程, 从而节省很多开销, 因此,这些子进程也叫轻量级进程(light-weight process)简称LWP. 每个LWP都会与一个内核线程绑定, 这样它就可以作为独立的调度实体参与cpu竞争. 

LWP被pthread封装后, 以线程面目示人, 它有自己的id, 这里要区分 phtread_create

给LWP分配的id, 和[操作系统](http://www.07net01.com/tags-%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F-0.html)给LWP分配的进程id. 这两者是不同的, 前者用于标志线程,可以暴露给

用户, 后者其实就是进程的id, 它没有暴露出来, 必须通过系统调用来得到.



获取LWP的方法是通过syscall系统调用（具体说明参加man手册），下面是一个例子程序：
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>

//线程处理函数，输出LWP号
void *func(void *argv)
{
    //获取LWP的方法1
    int lwp;
    lwp = syscall(__NR_gettid);
    printf("lwp[%d]\n", lwp);
    
    //获取LWP的方法2
    int lwp2;
    lwp2 = syscall(SYS_gettid);
    printf("lwp2[%d]\n", lwp2);
    sleep(50);
}

int main(void)
{
    pthread_t thd;
    pthread_create(&thd, NULL, func, NULL); //创建线程
    sleep(60);
    return 0;
}
**在获取LWP号以后，接下来就是通过strace命令对线程进行追踪了。**

strace常用来跟踪进程执行时的系统调用和所接收的信号。 在Linux世界，进程不能直接访问硬件设备，当进程需要访问硬件设备(比如读取磁盘文件，接收[网络](http://www.wredian.com/tags-%E7%BD%91%E7%BB%9C-0.html)数据等等)时，必须由用户态模式切换至内核态模式，通 过系统调用访问硬件设备。strace可以跟踪到一个进程产生的系统调用,包括参数，返回值，执行消耗的时间。



在理想世界里，每当一个程序不能正常执行一个功能时，它就会给出一个有用的错误提示，告诉你在足够的改正错误的线索。但遗憾的是，我们不是[生活](http://www.wredian.com/shenghuo/)在理想世界 里，起码不总是生活在理想世界里。有时候一个程序出现了问题，你无法找到原因。这就是调试程序出现的原因。strace是一个必不可少的 调试工具，strace用来监视系统调用。你不仅可以调试一个新开始的程序，也可以调试一个已经在运行的程序（把strace绑定到一个已有的PID上 面）。

strace的参数有很多，常见的参数介绍如下：

-o filename  将strace的输出写入文件filename 

-p pid      跟踪指定的进程pid. 

-t   在输出中的每一行前加上时间信息. 

-tt  在输出中的每一行前加上时间信息,微秒级.  

-T   显示每一调用所耗的时间. 

-f   跟踪由fork调用所产生的子进程. 

-ff  如果提供-o filename,则所有进程的跟踪结果输出到相应的filename.pid中,pid是各进程的进程     号. 

**举个例子**：我们对tcp_client程序做一个小小的改动，在通过pthread_create创建线程后，每个线程里都通过syscall函数记录其LWP号，同时记录该LWP号对应的数据类型。如果程序再次出现[文章](http://www.07net01.com/2015/07/860262.html)开头时提到的异常状况，这时我们可以通过“strace -o tcp_client.txt -tt -p 欲追踪的LWP号”，这样就很快就能知道发生异常的线程在做哪些系统调用了。

**总结：**在设计程序的时候，一定要考虑将来的维护问题。对于多线程程序，其调试会更加困难，通过在创建线程中记录LWP号和对应的线程功能，可以为以后的调试提供很大帮助。

参考文章：

1、关于Linux的进程和线程

http://kenby.iteye.com/blog/1014039

2、linux strace命令：

http://www.cnblogs.com/ggjucheng/archive/2012/01/08/2316692.html

本文出自 “篮彩大神” [博客](http://www.07net01.com/tags-%E5%8D%9A%E5%AE%A2-0.html)，请务必保留此出处http://lancaidashen.blog.51cto.com/11086793/1734196









