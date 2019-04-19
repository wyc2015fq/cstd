# select的用法 - Koma Hub - CSDN博客
2019年02月23日 20:29:45[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：68
个人分类：[unpv13e																[Linux																[Linux kernel](https://blog.csdn.net/Rong_Toa/article/category/7348890)](https://blog.csdn.net/Rong_Toa/article/category/7156254)](https://blog.csdn.net/Rong_Toa/article/category/8579444)
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
```
$ ./a.out 
main:41
No data within five seconds.
$ ./a.out 
this is main:41
No data within five seconds.
$ ./a.out 
hello
main:35
Data is available now.
$ hello
bash: hello: 未找到命令...
```
