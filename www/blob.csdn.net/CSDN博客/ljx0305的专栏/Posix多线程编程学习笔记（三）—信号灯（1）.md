# Posix多线程编程学习笔记（三）—信号灯（1） - ljx0305的专栏 - CSDN博客
2008年08月01日 11:08:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：724
**Posix****有名信号灯<?xml:namespace prefix = o ns = "urn:schemas-microsoft-com:office:office" />**
函数sem_open创建一个新的有名信号灯或打开一个已存在的有名信号灯。有名信号灯总是既可用于线程间的同步，又可以用于进程间的同步。
**1.posix****有名信号灯函数**
1.
|名称:：|sem_open|
|----|----|
|功能：|创建并初始化有名信号灯|
|头文件：|#include <semaphore.h>|
|函数原形：|sem_t *sem_open(const char *name,int oflag,/*mode_t mode,unsigned int value*/);|
|参数：|name 信号灯的外部名字oflag选择创建或打开一个现有的信号灯mode 权限位value 信号灯初始值|
|返回值：|成功时返回指向信号灯的指针，出错时为SEM_FAILED|

oflag参数可以是0、O_CREAT（创建一个信号灯）或O_CREAT|O_EXCL（如果没有指定的信号灯就创建），如果指定了O_CREAT，那么第三个和第四个参数是需要的；其中mode参数指定权限位，value参数指定信号灯的初始值，通常用来指定共享资源的书面。该初始不能超过SEM_VALUE_MAX，这个常值必须低于为32767。二值信号灯的初始值通常为1，计数信号灯的初始值则往往大于1。
如果指定了O_CREAT（而没有指定O_EXCL），那么只有所需的信号灯尚未存在时才初始化它。所需信号灯已存在条件下指定O_CREAT不是一个错误。该标志的意思仅仅是“如果所需信号灯尚未存在，那就创建并初始化它”。但是所需信号灯等已存在条件下指定O_CREAT|O_EXCL却是一个错误。
sem_open返回指向sem_t信号灯的指针，该结构里记录着当前共享资源的数目。
/*semopen.c*/
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
int main(int argc,char **argv)
{
sem_t *sem;
if(argc!=2)
{
printf(“please input a file name!/n”);
exit(1);
}
sem=sem_open(argv[1],O_CREAT,0644,1);
exit(0);
}
#gcc –lpthread –o semopen semopen.c
#./semopen
2.
|名称:：|sem_close|
|----|----|
|功能：|关闭有名信号灯|
|头文件：|#include <semaphore.h>|
|函数原形：|int sem_close(sem_t *sem);|
|参数：|sem 指向信号灯的指针|
|返回值：|若成功则返回0，否则返回-1。|
一个进程终止时，内核还对其上仍然打开着的所有有名信号灯自动执行这样的信号灯关闭操作。不论该进程是自愿终止的还是非自愿终止的，这种自动关闭都会发生。
但应注意的是关闭一个信号灯并没有将它从系统中删除。这就是说，Posix有名信号灯至少是随内核持续的：即使当前没有进程打开着某个信号灯，它的值仍然保持。
3.
|名称:：|sem_unlink|
|----|----|
|功能：|从系统中删除信号灯|
|头文件：|#include <semaphore.h>|
|函数原形：|int sem_unlink(count char *name);|
|参数：|name信号灯的外部名字|
|返回值：|若成功则返回0，否则返回-1。|

有名信号灯使用sem_unlink从系统中删除。
每个信号灯有一个引用计数器记录当前的打开次数，sem_unlink必须等待这个数为0时才能把name所指的信号灯从文件系统中删除。也就是要等待最后一个sem_close发生。
/*semunlink.c*/
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
int main(int argc,char **argv)
{
sem_t *sem;
int val;
if(argc!=2)
{
printf(“please input a file name!/n”);
exit(1);
}
if((sem_unlink(argv[1]))!=0)
perror(“sem_unlink”);
else
printf(“success”);
exit(0);
}
4.
|名称:：|sem_getvalue|
|----|----|
|功能：|测试信号灯|
|头文件：|#include <semaphore.h>|
|函数原形：|int sem_getvalue(sem_t *sem,int *valp);|
|参数：|sem 指向信号灯的指针|
|返回值：|若成功则返回0，否则返回-1。|
sem_getvalue在由valp指向的正数中返回所指定信号灯的当前值。如果该信号灯当前已上锁，那么返回值或为0，或为某个负数，其绝对值就是等待该信号灯解锁的线程数。
/*semgetvalue.c*/
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
int main(int argc,char **argv)
{
sem_t *sem;
int val;
if(argc!=2)
{
printf(“please input a file name!/n”);
exit(1);
}
sem=sem_open(argv[1],0);
sem_getvalue(sem,&val);
printf(“getvalue:value=%d/n”,val);
exit(0);
}
引用：[http://blog.chinaunix.net/u/22935/showart_340395.html](http://blog.chinaunix.net/u/22935/showart_340395.html)
