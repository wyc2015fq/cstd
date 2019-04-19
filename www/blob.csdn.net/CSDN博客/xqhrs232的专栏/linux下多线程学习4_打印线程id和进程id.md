# linux下多线程学习4_打印线程id和进程id - xqhrs232的专栏 - CSDN博客
2012年03月16日 17:19:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2126
原文地址::[http://hi.baidu.com/neu_stroller/blog/item/4213d8c52ddf0aa18226accb.html](http://hi.baidu.com/neu_stroller/blog/item/4213d8c52ddf0aa18226accb.html)
例程8
    程序目的：实现在新建立的线程中打印该线程的id和进程id
    程序名称：pthread_id.c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h> /*getpid()*/
void *create(void *arg)
{
    printf("New thread .... \n");
    printf("This thread's id is %u \n", (unsigned int)pthread_self());
    printf("The process pid is %d \n",getpid());
    return (void *)0;
}
int main(int argc,char *argv[])
{
    pthread_t tid;
    int error;
    printf("Main thread is starting ... \n");
    error = pthread_create(&tid, NULL, create, NULL);
    if(error)
    {
        printf("thread is not created ... \n");
        return -1;
    }
    printf("The main process's pid is %d \n",getpid());
    sleep(1);
    return 0;
}
    编译方法：
gcc -Wall -lpthread pthread_id.c
    执行结果：
Main thread is starting ...
The main process's pid is 3307 
New thread ....
This thread's id is 3086347152 
The process pid is 3307 
