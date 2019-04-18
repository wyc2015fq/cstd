# execvp - Orisun - 博客园







# [execvp](https://www.cnblogs.com/zhangchaoyang/articles/2298308.html)





**问题：一个程序如何运行另一个程序？**

**答：程序调用execvp**

```
#include<stdio.h>
main(){
    char *arglist[3];
    arglist[0]="ls";
    arglist[1]="-l";
    arglist[2]=0;   //参数字符串必须以0结束
    printf("********About to execute ls -l\n");
    execvp("ls",arglist);
    printf("********ls is done.bye\n");
}
```

execvp有两个参数：要运行的程序名和那个程序的命令行参数。当程序运行时命令行参数以argv[]传给程序。最后一个参数必须为NULL。

$ ./exec1
********About to execute ls -l
total 196
-rwxr-xr-x 1 orisun orisun 7568 2011-12-22 16:35 bounce
-rw-r--r-- 1 orisun orisun 1736 2011-12-22 16:35 bounce.c
-rw------- 1 orisun orisun 208896 2011-12-22 15:19 core
-rwxr-xr-x 1 orisun orisun 7169 2011-12-22 17:28 exec1

这里有一个问题：为什么printf("************ls is done.bye\n")没有执行？

因为调用execvp时，内核将新程序载入到当前进程，替代当前进程的代码和数据。












