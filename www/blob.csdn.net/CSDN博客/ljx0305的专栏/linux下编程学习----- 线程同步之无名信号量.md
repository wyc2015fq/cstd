# linux下编程学习----- 线程同步之无名信号量 - ljx0305的专栏 - CSDN博客
2010年06月21日 16:52:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：602标签：[linux																[编程																[buffer																[null																[join																[sun](https://so.csdn.net/so/search/s.do?q=sun&t=blog)](https://so.csdn.net/so/search/s.do?q=join&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux程序及使用](https://blog.csdn.net/ljx0305/article/category/394700)
### [linux下编程学习----- 线程同步之无名信号量](blog/181250)
这是一个书上的例子，逻辑是：一个线程生产一组数字1、2、3..... 等等，放入一个循环队列中。另一个线程去处理这些数字，达到的效果是把这些数字求和。还是看看代码吧。 
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#define BUFSIZE 4
#define NUMBER 8
int sum_of_number=0;
/* 可读 和 可写资源数*/
sem_t write_res_number;
sem_t read_res_number;
/* 循环队列 */
struct recycle_buffer{
    int buffer[BUFSIZE];
    int head,tail;
}re_buf;
/* 用于实现临界区的互斥锁，我们对其初始化*/
pthread_mutex_t buffer_mutex=PTHREAD_MUTEX_INITIALIZER;
static void *producer(void * arg)
{
    int i;
    for(i=0;i<=NUMBER;i++)
    {
        /* 减少可写的资源数 */
        sem_wait(&write_res_number);
        /* 进入互斥区 */
        pthread_mutex_lock(&buffer_mutex);
        /*将数据复制到缓冲区的尾部*/
        re_buf.buffer[re_buf.tail]=i;
        re_buf.tail=(re_buf.tail+1)%BUFSIZE;
        printf("procuder %d write %d./n",pthread_self(),i);
        /*离开互斥区*/
        pthread_mutex_unlock(&buffer_mutex);
        /*增加可读资源数*/
        sem_post(&read_res_number);
    }
    /* 线程终止，如果有线程等待它们结束，则把NULL作为等待其结果的返回值*/
    return NULL;
}
static void * consumer(void * arg)
{
    int i,num;
    for(i=0;i<=NUMBER;i++)
    {
        /* 减少可读资源数 */
        sem_wait(&read_res_number);
        /* 进入互斥区*/
        pthread_mutex_lock(&buffer_mutex);
        /* 从缓冲区的头部获取数据*/
        num = re_buf.buffer[re_buf.head];
        re_buf.head = (re_buf.head+1)%BUFSIZE;
        printf("consumer %d read %d./n",pthread_self(),num);
        /* 离开互斥区*/
        pthread_mutex_unlock(&buffer_mutex);
        sum_of_number+=num;
        /* 增加客写资源数*/
        sem_post(&write_res_number);
    }        
    /* 线程终止，如果有线程等待它们结束，则把NULL作为等待其结果的返回值*/
    return NULL;
}
int main(int argc,char ** argv)
{
    /* 用于保存线程的线程号 */
    pthread_t p_tid;
    pthread_t c_tid;
    int i;
    re_buf.head=0;
    re_buf.tail=0;
    for(i=0;i<BUFSIZE;i++)
        re_buf.buffer[i] =0;
    /* 初始化可写资源数为循环队列的单元数 */
    sem_init(&write_res_number,0,BUFSIZE);    //  这里限定了可写的bufsize，当写线程写满buf时，会阻塞，等待读线程读取
    /* 初始化可读资源数为0 */
    sem_init(&read_res_number,0,0);
    /* 创建两个线程，线程函数分别是 producer 和 consumer */
    /* 这两个线程将使用系统的缺省的线程设置，如线程的堆栈大小、线程调度策略和相应的优先级等等*/
    pthread_create(&p_tid,NULL,producer,NULL);
    pthread_create(&c_tid,NULL,consumer,NULL);
    /*等待两个线程完成退出*/
    pthread_join(p_tid,NULL);
    pthread_join(c_tid,NULL);
    printf("The sun of number is %d/n",sum_of_number);
}
编译命令：gcc -g -o threadtest threadtest.c -lpthread
引用：[http://shake863.javaeye.com/blog/181250](http://shake863.javaeye.com/blog/181250)
