# linux 下多线程错误 undefined reference to `sem_init' - xqhrs232的专栏 - CSDN博客
2017年05月17日 10:37:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1762
原文地址::[http://www.cnblogs.com/stli/archive/2010/03/11/1683823.html](http://www.cnblogs.com/stli/archive/2010/03/11/1683823.html)
undefined reference to `sem_init'
undefined reference to `sem_post'
undefined reference to `sem_wait'
编译选项需要加入一个多线程
gcc -pthread -o outfile.out filename.c
[source code]
#include "sched.h"
#include "pthread.h"
#include "stdio.h"
#include "stdlib.h"
#include "semaphore.h"
int producer(void * args);
int consumer(void * args);
pthread_mutex_t mutex;
sem_t product;
sem_t warehouse;
char buffer[8][4];
int bp=0;
int main(int argc,char * * argv)
{
pthread_mutex_init(& mutex,NULL);
sem_init(& product,0,0);
    sem_init(& warehouse,0,8);
int clone_flag,arg,retval;
    char * stack;
    clone_flag=CLONE_VM | CLONE_SIGHAND | CLONE_FS | CLONE_FILES;
    int i;
for(i=0;i<2;i++)
{ //创建四个线程
        arg=i;
        stack=(char *)malloc(4096); 
        retval=clone((void *)producer,& (stack[4095]),clone_flag,(void *) & arg);
   stack=(char *)malloc(4096); 
        retval=clone((void *)consumer,& (stack[4095]),clone_flag,(void *) & arg);
    }
    exit(1);
return 0;
}
int producer(void * args)
{
    int id = *((int *)args);
    int i;
    for(i=0;i<10;i++)
    {
   sleep(i+1); //表现线程速度差别
        sem_wait(&warehouse);
        pthread_mutex_lock(&mutex);
        if(id==0)
            strcpy(buffer[bp],"aaa\0");
        else
            strcpy(buffer[bp],"bbb\0");
        bp++;
        printf("producer%d produce %s in %d\n",id,buffer[bp],bp-1);
        pthread_mutex_unlock(&mutex);
        sem_post(&product);
    }
    printf("producer%d is over!\n",id);
}
int consumer(void * args)
{
    int id = * ((int *)args);
    int i;
    for(i=0;i<10;i++)
    {
        sleep(10-i); //表现线程速度差别
        sem_wait(&product);
        pthread_mutex_lock(&mutex);
        bp--;
        printf("consumer%d get %s in%d\n",id,buffer[bp],bp+1);
        strcpy(buffer[bp],"zzz\0");
        pthread_mutex_unlock(&mutex);
        sem_post(&warehouse);
    }
    printf("consumer%d is over!\n",id);
}本文转自：http://hi.baidu.com/luosiyong/blog/item/e5e1980fffd1b6e5ab6457e5.html
