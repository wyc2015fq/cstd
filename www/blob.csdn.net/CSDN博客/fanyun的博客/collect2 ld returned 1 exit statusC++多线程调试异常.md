# collect2: ld returned 1 exit statusC++多线程调试异常 - fanyun的博客 - CSDN博客
2017年05月31日 21:26:55[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1230
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
在Linux下创建线程时,编译时会出现下面的错误,
[root@linuxserver 807]# gcc -o 22 22.c
/tmp/cc21HcoW.o(.text+0x4c): In function `main':
: undefined reference to `pthread_create'
collect2: ld returned 1 exit status
程序为:
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
void testthread(void)
{
printf("I
 am working.\n");
printf("I
 am stopping.\n");
pthread_exit(0);
}
int main(int argc,char *argv[])
{
int
 i=0;
pthread_t
 pid;
char
 *szP=NULL;
while(1)
{
 i++;
 pthread_create(&pid,NULL,(void
 *)testthread,(void *)&i);
 printf("ok%d,pid=%d\n",i,pid);
 sleep(5);
}
}
此时,只需改变编译方式
将gcc -o 22 22.c 改变为 gcc -O2 -Wall -o 22 22.c -lpthread
最关键的是-lpthread
根据错误
/tmp/cc21HcoW.o(.text+0x4c): In function `main':
: undefined reference to `pthread_create'
collect2: ld returned 1 exit status
可以看出是在ld的时候系统无法找到pthread_create函数。也就是说编译器在link得时候找不到其中的一个使用库的函数。
如果差pthread_create的话可以发现其在pthread.so中，所以需要增加 -lpthread编译参数，告诉linker在link的时候使用pthread模块
