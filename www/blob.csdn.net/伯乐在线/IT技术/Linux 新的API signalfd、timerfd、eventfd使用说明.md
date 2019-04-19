# Linux 新的API signalfd、timerfd、eventfd使用说明 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [lvyilong316](http://www.jobbole.com/members/lvyilong316) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
**三种新的fd加入linux内核的的版本：**
signalfd：2.6.22
timerfd：2.6.25
eventfd：2.6.22
**三种fd的意义：**
l**s****ignalfd**
传统的处理信号的方式是注册信号处理函数；由于信号是异步发生的，要解决数据的并发访问，可重入问题。signalfd可以将信号抽象为一个文件描述符，当有信号发生时可以对其read，这样可以将信号的监听放到select、poll、epoll等监听队列中。
l**timerfd**
可以实现定时器的功能，将定时器抽象为文件描述符，当定时器到期时可以对其read，这样也可以放到监听队列的主循环中。
l**eventfd**
实现了线程之间事件通知的方式，**也可以用于用户态和内核通信**。eventfd的缓冲区大小是sizeof(uint64_t)；向其write可以递增这个计数器，read操作可以读取，并进行清零；eventfd也可以放到监听队列中，当计数器不是0时，有可读事件发生，可以进行读取。
三种新的fd都可以进行监听，当有事件触发时，有可读事件发生。
## **signalfd涉及API：**
点击(此处)折叠或打开


```
#include <sys/signalfd.h> 
int signalfd(int fd, const sigset_t *mask, int flags);
```
参数fd：如果是-1则表示新建一个，如果是一个已经存在的则表示修改signalfd所关联的信号；
参数mask：信号集合；
参数flag：内核版本2.6.27以后支持SFD_NONBLOCK、SFD_CLOEXEC；
成功返回文件描述符，返回的fd支持以下操作：read、select(poll、epoll)、close
l**例子**


```
#include <sys/signalfd.h> 
  #include <signal.h> 
  #include <unistd.h> 
  #include <stdlib.h> 
  #include <stdio.h> 
 
  #define handle_error(msg) \ 
  do { perror(msg); exit(EXIT_FAILURE); } while (0) 
 
  int main(int argc, char *argv[]) 
  { 
    sigset_t mask; 
    int sfd; 
    struct signalfd_siginfo fdsi; 
    ssize_t s; 
 
    sigemptyset(&mask); 
    sigaddset(&mask, SIGINT); 
    sigaddset(&mask, SIGQUIT); 
 
    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) 
        handle_error("sigprocmask"); 
 
    sfd = signalfd(-1, &mask, 0); 
    if (sfd == -1) 
        handle_error("signalfd"); 
 
    for (;;) { 
        s = read(sfd, &fdsi, sizeof(struct signalfd_siginfo)); 
        if (s != sizeof(struct signalfd_siginfo)) 
            handle_error("read"); 
 
        if (fdsi.ssi_signo == SIGINT) { 
           printf("Got SIGINT\n"); 
        } else if (fdsi.ssi_signo == SIGQUIT) { 
        printf("Got SIGQUIT\n"); 
        exit(EXIT_SUCCESS); 
        } else { 
        printf("Read unexpected signal\n"); 
        } 
     } 
  }
```
L17-L21：将感兴趣的信号加入到sigset_t中；
L24：调用signalfd，把信号集与fd关联起来，第一个参数为-1表示新建一个signalfd，不是-1并且是一个合法的signalfd表示向其添加新的信号。
L29：阻塞等待信号的发生并读取。根据读取的结果可以知道发生了什么信号。
## **timerfd涉及的API**

```
#include <sys/timerfd.h> 
int timerfd_create(int clockid, int flags); 
int timerfd_settime(int fd, int flags, const struct itimerspec *new_value,struct itimerspec *old_value); 
int timerfd_gettime(int fd, struct itimerspec *curr_value);
```
timerfd_create：创建一个timerfd；返回的fd可以进行如下操作：read、select(poll、epoll)、close
timerfd_settime：设置timer的周期，以及起始间隔
timerfd_gettime：获取到期时间。


```
//函数参数中数据结构如下： 
struct timespec 
{ 
    time_t tv_sec; /* Seconds */ 
    long tv_nsec; /* Nanoseconds */ 
}; 
  struct itimerspec 
{ 
    struct timespec it_interval; /* Interval for periodic timer */ 
    struct timespec it_value; /* Initial expiration */ 
};
```
l**例子**

```
#include <sys/timerfd.h> 
  #include <sys/time.h> 
  #include <time.h> 
  #include <unistd.h> 
  #include <stdlib.h> 
  #include <stdio.h> 
  #include <stdint.h> /* Definition of uint64_t */ 
 
  #define handle_error(msg) \ 
  do { perror(msg); exit(EXIT_FAILURE); } while (0) 
 
  void printTime() 
  { 
      struct timeval tv; 
      gettimeofday(&tv, NULL); 
      printf("printTime: current time:%ld.%ld ", tv.tv_sec, tv.tv_usec); 
  } 
 
  int main(int argc, char *argv[]) 
  { 
      struct timespec now; 
      if (clock_gettime(CLOCK_REALTIME, &now) == -1) 
          handle_error("clock_gettime"); 
 
      struct itimerspec new_value; 
      new_value.it_value.tv_sec = now.tv_sec + atoi(argv[1]); 
      new_value.it_value.tv_nsec = now.tv_nsec; 
      new_value.it_interval.tv_sec = atoi(argv[2]); 
      new_value.it_interval.tv_nsec = 0; 
 
      int fd = timerfd_create(CLOCK_REALTIME, 0); 
      if (fd == -1) 
      handle_error("timerfd_create"); 
 
      if (timerfd_settime(fd, TFD_TIMER_ABSTIME, &new_value, NULL) == -1) 
          handle_error("timerfd_settime"); 
 
      printTime(); 
      printf("timer started\n"); 
 
      for (uint64_t tot_exp = 0; tot_exp < atoi(argv[3]);) 
      { 
          uint64_t exp; 
          ssize_t s = read(fd, &exp, sizeof(uint64_t)); 
          if (s != sizeof(uint64_t)) 
              handle_error("read"); 
 
          tot_exp += exp; 
          printTime(); 
          printf("read: %llu; total=%llu\n",exp, tot_exp); 
  } 
 
  exit(EXIT_SUCCESS); 
 }
```
代码L25-L29：初始化定时器的参数，初始间隔与定时间隔。
L32：创建定时器fd，CLOCK_REALTIME：真实时间类型，修改时钟会影响定时器；CLOCK_MONOTONIC：相对时间类型，修改时钟不影响定时器。
L35：设置定时器的值。
L44：阻塞等待定时器到期。返回值是未处理的到期次数。比如定时间隔为2秒，但过了10秒才去读取，则读取的值是5。
编译运行：编译时要加rt库(g++ -lrt timerfd.cc -o timerfd)
[root@localhost appTest]# ./timerfd 5 2 10
printTime:  current time:1357391736.146196 timer started
printTime:  current time:1357391741.153430 read: 1; total=1
printTime:  current time:1357391743.146550 read: 1; total=2
printTime:  current time:1357391745.151483 read: 1; total=3
printTime:  current time:1357391747.161155 read: 1; total=4
printTime:  current time:1357391749.153934 read: 1; total=5
printTime:  current time:1357391751.157309 read: 1; total=6
printTime:  current time:1357391753.158384 read: 1; total=7
printTime:  current time:1357391755.150470 read: 1; total=8
printTime:  current time:1357391757.150253 read: 1; total=9
printTime:  current time:1357391759.149954 read: 1; total=10
[root@localhost appTest]#
第一个参数5为第一次定时器到期间隔，第二个参数2为定时器的间隔，第三个参数为定时器到期10次则退出。程序运行(5+2*10)S退出。
详细信息可以：man timerfd_create
## **eventfd涉及API：**

```
#include <sys/eventfd.h> 
int eventfd(unsigned int initval, int flags);
```
创建一个eventfd，这是一个计数器相关的fd，计数器不为零是有可读事件发生，read以后计数器清零，write递增计数器；返回的fd可以进行如下操作：read、write、select(poll、epoll)、close。
这个函数会创建一个事件对象 (eventfd object), 用来实现，进程(线程)间的等待/通知(wait/notify) 机制. 内核会为这个对象维护一个64位的计数器(uint64_t)。并且使用第一个参数(initval)初始化这个计数器。调用这个函数就会返回一个新的文件描述符(event object)。2.6.27版本开始可以按位设置第二个参数(flags)。有如下的一些宏可以使用：
l**EFD_NONBLOCK**
功能同open(2)的O_NONBLOCK，设置对象为非阻塞状态，如果没有设置这个状态的话，read(2)读eventfd,并且计数器的值为0 就一直堵塞在read调用当中，要是设置了这个标志， 就会返回一个 EAGAIN 错误(errno = EAGAIN)。效果也如同 额外调用select(2)达到的效果。
l**EFD_CLOEXEC**
这个标识被设置的话，调用exec后会自动关闭文件描述符，防止泄漏。如果是2.6.26或之前版本的内核，flags 必须设置为0。
创建这个对象后，可以对其做如下操作：
1) write： 将缓冲区写入的8字节整形值加到内核计数器上。
2) read： 读取8字节值， 并把计数器重设为0. 如果调用read的时候计数器为0， 要是eventfd是阻塞的， read就一直阻塞在这里，否则就得到 一个EAGAIN错误。如果buffer的长度小于8那么read会失败， 错误代码被设置成 EINVAL。
3) poll select epoll
4) close: 当不需要eventfd的时候可以调用close关闭， 当这个对象的所有句柄都被关闭的时候，内核会释放资源。 为什么不是close就直接释放呢， 如果调用fork 创建
进程的时候会复制这个句柄到新的进程，并继承所有的状态。
l**例子**


```
#include <sys/eventfd.h>
 #include <unistd.h>
 #include <stdio.h>
 #include <stdint.h>
 #include <stdlib.h>
 #include <errno.h>
 #define handle_error(msg) \
    do { perror(msg); exit(1); } while (0)
int main( int argc, char **argv ){
     uint64_t u;
     ssize_t s;5 int j;
     if ( argc < 2 ) {
        fprintf(stderr, "input in command argument");
         exit(1);
     }
 
     int efd;
     if ( (efd = eventfd(0, EFD_NONBLOCK)) == -1 )
             handle_error("eventfd failed");
 
 
     switch (fork()) {
         case 0:
             for( j = 1; j < argc; j ++ ) {
                 printf("Child writing %s to efd\n", argv[j] );
             
                 u = strtoull(argv[j], NULL, 0); /* analogesly atoi */
                 s = write(efd, &u, sizeof(uint64_t));/*append u to counter */
                 if ( s != sizeof(uint64_t) )
                     handle_error("write efd failed");
 
             }
             printf("child completed write loop\n");
 
             exit(0);
         default:
             sleep (2);
             
             printf("parent about to read\n");
             s = read(efd, &u, sizeof(uint64_t));
             if ( s != sizeof(uint64_t) ) {
                 if (errno = EAGAIN) {
                     printf("Parent read value %d\n", s);
                     return 1;
                 }
                 handle_error("parent read failed");
             }
             printf("parent read %d , %llu (0x%llx) from efd\n",
                     s, (unsigned long long)u, (unsigned long long) u);
             exit(0);
 
         case -1:
             handle_error("fork ");
     }
     return 0;
}
```
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/09/cf8e8b52a0e2fe66ce4100c5b2b99ed4.png)
