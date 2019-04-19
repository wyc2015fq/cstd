# linux select 函数用法！ - xqhrs232的专栏 - CSDN博客
2010年12月29日 15:48:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：705
原文地址::[http://blog.csdn.net/xioahw/archive/2009/04/08/4056290.aspx](http://blog.csdn.net/xioahw/archive/2009/04/08/4056290.aspx)
表头文件
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
定义函数
    int select(int n,fd_set * readfds,fd_set * writefds,fd_set * exceptfds,struct timeval * timeout);
函数说明
    select()用来等待文件描述词状态的改变。参数n代表最大的文件描述词加1，参数readfds、writefds 和exceptfds 称为描述词组，是用来回传该描述词的读，写或例外的状况。底下的宏提供了处理这三种描述词组的方式:
FD_CLR(inr fd,fd_set* set)；用来清除描述词组set中相关fd 的位
FD_ISSET(int fd,fd_set *set)；用来测试描述词组set中相关fd 的位是否为真
FD_SET(int fd,fd_set*set)；用来设置描述词组set中相关fd的位
FD_ZERO(fd_set *set)；用来清除描述词组set的全部位
参数
    timeout为结构timeval，用来设置select()的等待时间，其结构定义如下
struct timeval
{
time_t tv_sec;
time_t tv_usec;
};
返回值
    如果参数timeout设为NULL则表示select()没有timeout。
错误代码
    执行成功则返回文件描述词状态已改变的个数，如果返回0代表在描述词状态改变前已超过timeout时间，当有错误发生时则返回-1，错误原因存于errno，此时参数readfds，writefds，exceptfds和timeout的值变成不可预测。
EBADF 文件描述词为无效的或该文件已关闭
EINTR 此调用被信号所中断
EINVAL 参数n 为负值。
ENOMEM 核心内存不足
范例
    常见的程序片段:fs_set readset；
FD_ZERO(&readset);
FD_SET(fd,&readset);
select(fd+1,&readset,NULL,NULL,NULL);
if(FD_ISSET(fd,readset){……}
下面是linux环境下select的一个简单用法
#include <sys/time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
int main()
{
    int keyboard;
    int ret, i;
    char c;
    fd_set readfd;
    struct timeval timeout;
    keyboard = open("/dev/tty", O_RDONLY | O_NONBLOCK);
    assert(keyboard > 0);
    while (1)
    {
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        FD_ZERO(&readfd);
        FD_SET(keyboard, &readfd);
        ret = select(keyboard + 1, &readfd, NULL, NULL, &timeout);
        if (FD_ISSET(keyboard, &readfd))
        {
            i = read(keyboard, &c, 1);
            if ('/n' == c)
                continue;
            printf("hehethe input is %c/n", c);
            if ('q' == c)
                break;
        }
    }
}
用来循环读取键盘输入
2007年9月17日，将例子程序作一修改，加上了time out,并且考虑了select得所有的情况：
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
struct timeval {
    long tv_sec;                /*   seconds   */
    long tv_usec;               /*   and   microseconds   */
};
int main()
{
    int keyboard;
    int ret, i;
    char c;
    fd_set readfd;
    struct timeval timeout;
    keyboard = open("/dev/tty", O_RDONLY | O_NONBLOCK);
    assert(keyboard > 0);
    while (1)
    {
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        FD_ZERO(&readfd);
        FD_SET(keyboard, &readfd);
        ret = select(keyboard + 1, &readfd, NULL, NULL, &timeout);
        //select error when ret = -1
        if (ret == -1)
            perror("select error");
        //data coming when ret>0
        else if (ret)
        {
            if (FD_ISSET(keyboard, &readfd))
            {
                i = read(keyboard, &c, 1);
                if ('/n' == c)
                    continue;
                printf("hehethe input is %c/n", c);
                if ('q' == c)
                    break;
            }
        }
        //time out when ret = 0
        else if (ret == 0)
            printf("time out/n");
    }
}
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/xioahw/archive/2009/04/08/4056290.aspx](http://blog.csdn.net/xioahw/archive/2009/04/08/4056290.aspx)
