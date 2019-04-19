# Linux select() - Koma Hub - CSDN博客
2018年10月13日 14:09:54[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：80
先看看linux的man select手册（节选）：
```
$man select
SELECT(2)                      Linux Programmer's Manual                      SELECT(2)
NAME
       select,  pselect, FD_CLR, FD_ISSET, FD_SET, FD_ZERO - synchronous I/O multiplex‐
       ing
SYNOPSIS
       /* According to POSIX.1-2001 */
       #include <sys/select.h>
       /* According to earlier standards */
       #include <sys/time.h>
       #include <sys/types.h>
       #include <unistd.h>
       int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);
       void FD_CLR(int fd, fd_set *set);
       int  FD_ISSET(int fd, fd_set *set);
       void FD_SET(int fd, fd_set *set);
       void FD_ZERO(fd_set *set);
       #include <sys/select.h>
       int pselect(int nfds, fd_set *readfds, fd_set *writefds,
                   fd_set *exceptfds, const struct timespec *timeout,
                   const sigset_t *sigmask);
RETURN VALUE
       On  success,  select()  and pselect() return the number of file descriptors con‐
       tained in the three returned descriptor sets (that is, the total number of  bits
       that  are  set in readfds, writefds, exceptfds) which may be zero if the timeout
       expires before anything interesting happens.  On  error,  -1  is  returned,  and
       errno  is  set  appropriately;  the sets and timeout become undefined, so do not
       rely on their contents after an error.
```
下面就我的理解简单介绍：
将文件描述符用FD_SET宏添加到一个长整型long的fd_set中，然后select会检测内核对哪个文件描述符添加到fd_set中的文件描述符感兴趣。
下面将man select中的例子做出完整的注释说明，并作出少许改动：
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#define debug() printf("\033[31m%s\033[m:%d\n", __func__, __LINE__)
int
main(void)
{
    fd_set rfds;         /*创建一个fd_set类型数据，实则为long，使用其中的位*/
    struct timeval tv;   /*声明一个时间变量，里面包括秒和毫秒两个字段*/
    int retval;          /*返回值*/
    /* Watch stdin (fd 0) to see when it has input. */
    FD_ZERO(&rfds);      /*将fd_set置零*/
    FD_SET(0, &rfds);    /*将stdin加入到这个rfds中，其等同于下面的注释行，即fileno(stdin)=0*/
    //FD_SET(fileno(stdin), &rfds);
    
    /* Wait up to two seconds. */
    tv.tv_sec = 2;      /*设置时间为2s，两秒内stdin无操作则select不再阻塞，继续运行*/
    tv.tv_usec = 0;
    retval = select(1, &rfds, NULL, NULL, &tv); /*调用select函数*/
    /* Don't rely on the value of tv now! */
    if (retval == -1)   /*如果select出错*/
    {
        debug();
        perror("select()");
    }
    else if (retval)    /*如果stdin有输入*/
    {
        debug();
        printf("Data is available now.\n");
        /* FD_ISSET(0, &rfds) will be true. */
    }
    else                /*否则*/
    {
        debug();
        printf("No data within five seconds.\n");
    }
    exit(EXIT_SUCCESS);
}
```
运行结果：
```
$ gcc select.c 
[Toa@toa ~]$ ./a.out 
main:41
No data within five seconds.
$ ./a.out 
123
main:35
Data is available now.
```
