# Linux 系统编程笔记 守护进程，进程通信 - xqhrs232的专栏 - CSDN博客
2011年03月15日 15:43:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：731
原文地址::[http://hi.baidu.com/vivo01/blog/item/9218a37b1418ebe72e73b3dd.html](http://hi.baidu.com/vivo01/blog/item/9218a37b1418ebe72e73b3dd.html)
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
------守护进程------
---PPID=1(init)
---tty=? 
---创建守护进程
--1.fork创建子进程
--2.杀死父进程，然后子进程由1号进程接收，由此完成脱壳
--3.创建会话期（使进程脱离控制终端）setsid().
-会话期:多个进程-->进程组 多个进程组-->会话期.一个进程组中的每个进程的组号是一样的，进程组有一个领头进程（其ID和进程组ID相同）。
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
#define FILE_YES
#ifdef FILE_YES
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>
int main()
{
pid_t pid;
pid = fork();
if(pid < 0)
{
perror("Fork() failed:");
exit(EXIT_FAILURE);
}
if(pid > 0)
{
puts("Killed Parent Process!");
exit(0);
}
else if(pid == 0)
{
if(setsid() < 0)
{
perror("Setsid() failure:");
exit(-1);
}
int i = 1;
while(i++)
{
puts("I am daemon!");
printf("My parent id: %d/n",getppid());
sleep(3);
if(i > 9)
exit(0);
}
}
return 0;
}
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
------创建守护进程后一般使用chdir("/");umask(0);for(i = 0;i < 3;i++)close(i);关闭标准输入输出和错误输出
------守护进程的日志
--syslogd系统守护进程,日志在/var/log
--/etc/syslogd.conf为syslogd配置文件
----openlog()打开链接
----syslog()写入日志文件
----closelog()
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
------杀死僵尸进程
---杀到僵尸进程的父进程
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
------进程间通信
-----目的：数据传输，资源共享，通知事件，进程控制
-----方法：管道（pipe）和有名（命名）管道（FIFO）、信号、消息队列、共享内存、信号量、协同进程、文件锁、套接字
---管道：单向的先进现出的字节流，把一个进程的输出和另一个进程的输入连接在一起。
--匿名管道：父进程与子进程间的通信、子孙进程的通信、兄弟进程间的通信
--命名管道：同一台主机任意两个进程间通信
--匿名管道：pipe(int filedes[2])建立，函数创建两个文件描述符，filedes[0]用于读，filedes[1]用于写
--命名管道：任意进程间通信(管道文件)
-mknod pipe1 p或mkfifo pipe2创建管道文件
-函数mkfifo(*pathname,mode)与open的参数基本一致
--协同进程：两个管道
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
例子：命名管道使用
//读管道进程源文件
#define FILE_YES
#ifdef FILE_YES
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>
#include <string.h>
#define PIPE_FIFO "name_pipe"
int main()
{
int p_fd;
int ret;
int w_read;
char buf[200] = {0};
unlink(PIPE_FIFO);
ret = mkfifo(PIPE_FIFO,O_CREAT);
if(ret < 0)
{
strerror(errno);
exit(-1);
}
p_fd = open(PIPE_FIFO,O_RDONLY);
if(p_fd < 0)
{
strerror(errno);
exit(-1);
}
while(1)
{
w_read = read(p_fd,buf,200);
if(w_read)
printf("PIPE_READ:%s/n",buf);
}
return 0;
}
//写管道进程源文件
#define FILE_YES
#ifdef FILE_YES
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>
#include <string.h>
#define PIPE_FIFO "name_pipe"
int main()
{
int p_fd;
int count = 0;
char buf[200] = {0};
p_fd = open(PIPE_FIFO,O_WRONLY);
if(p_fd < 0)
{
strerror(errno);
exit(-1);
}
while(count++ < 10)
{
sprintf(buf,"This is alpha %d,do you read?I repeat,do you read?",count);
write(p_fd,buf,200);
}
return 0;
}
