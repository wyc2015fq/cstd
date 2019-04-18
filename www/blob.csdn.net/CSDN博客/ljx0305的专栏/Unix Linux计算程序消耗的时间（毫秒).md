# Unix/Linux计算程序消耗的时间（毫秒) - ljx0305的专栏 - CSDN博客
2012年07月31日 11:33:47[ljx0305](https://me.csdn.net/ljx0305)阅读数：1554标签：[struct																[buffer																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)
使用`time(NULL)`得到的是从1970年1月1日到目前的秒，这种精度很多时候是不够用的。为了得到毫秒级的精度，需要使用`gettimeofday`:
直接上代码:
```
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
// Return 1 if the difference is negative, otherwise 0.  
int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;
    return (diff<0);
}
void timeval_print(struct timeval *tv)
{
    char buffer[30];
    time_t curtime;
    printf("%ld.%06ld", tv->tv_sec, tv->tv_usec);
    curtime = tv->tv_sec;
    strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime;));
    printf(" = %s.%06ld\n", buffer, tv->tv_usec);
}
int main()
{
    struct timeval tvBegin, tvEnd, tvDiff;
    // begin
    gettimeofday(&tvBegin;, NULL);
    timeval_print(&tvBegin;);
    // lengthy operation
    int i,j;
    for(i=0;i<999999L;++i) {
        j=sqrt(i);
    }
    //end
    gettimeofday(&tvEnd;, NULL);
    timeval_print(&tvEnd;);
    // diff
    timeval_subtract(&tvDiff;, &tvEnd;, &tvBegin;);
    printf("%ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);
    return 0;
}
```
