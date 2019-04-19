# linux select 学习 - Arthur的随笔 - CSDN博客
2011年08月23日 17:47:50[largetalk](https://me.csdn.net/largetalk)阅读数：677标签：[linux																[include																[null																[struct																[up](https://so.csdn.net/so/search/s.do?q=up&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[linux](https://blog.csdn.net/largetalk/article/category/727912)
```cpp
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
int
main(void)
{
    fd_set rfds;
    struct timeval tv;
    int retval;
    int rof = open("aa.tt", O_RDONLY);
    printf("open aa.tt with fd is %d\n", rof);
    /* Watch stdin (fd 0) to see when it has input. */
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    FD_SET(rof, &rfds);
    printf("FD_ISET: %d\n", FD_ISSET(0, &rfds));
    /* Wait up to five seconds. */
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    retval = select(rof + 1, &rfds, NULL, NULL, &tv);
    /* Don't rely on the value of tv now! */
    if (retval == -1)
        perror("select()");
    else if (retval)
    {
        printf("retval is %d\n", retval);
        printf("Data is available now.\n");
        char cc[20]={0};
        int rreaded = read(0, cc, 19);
        printf("rreaded is :%d\n", rreaded);
        read(rof, cc+rreaded, 19-rreaded);
        printf("data is %s\n", cc);
        /* FD_ISSET(0, &rfds) will be true. */
    }
    else
        printf("No data within five seconds.\n");
    close(rof);
    exit(EXIT_SUCCESS);
}
```
