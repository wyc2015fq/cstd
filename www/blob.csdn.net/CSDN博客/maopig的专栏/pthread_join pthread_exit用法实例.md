# pthread_join/pthread_exit用法实例 - maopig的专栏 - CSDN博客
2011年09月11日 09:50:29[maopig](https://me.csdn.net/maopig)阅读数：1229标签：[thread																[join																[returning																[null																[gcc																[存储](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=gcc&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=returning&t=blog)](https://so.csdn.net/so/search/s.do?q=join&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)
个人分类：[Linux																[c/C++](https://blog.csdn.net/maopig/article/category/847675)](https://blog.csdn.net/maopig/article/category/850267)

函数pthread_join用来等待一个线程的结束。函数原型为：
　　extern int pthread_join __P ((pthread_t __th, void **__thread_return));
第一个参数为被等待的线程标识符，第二个参数为一个用户定义的指针，它可以用来存储被等待线程的返回值。这个函数是一个线程阻塞的函数，调用它的线程将一直等待到被等待的线程结束为止，当函数返回时，被等待线程的资源被收回。一个线程的结束有两种途径，一种是象我们上面的例子一样，函数结束了，调用它的线程也就结束了；另一种方式是通过函数pthread_exit来实现。它的函数原型为：
　　extern void pthread_exit __P ((void *__retval)) __attribute__ ((__noreturn__));
唯一的参数是函数的返回代码，只要pthread_exit中的参数retval不是NULL，这个值将被传递给 thread_return。最后要说明的是，一个线程不能被多个线程等待，否则第一个接收到信号的线程成功返回，其余调用pthread_join的线程则返回错误代码ESRCH。
实例： 
/*myfile11-3.c*/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
pthread_t       tid1, tid2; 
void            *tret;
void *
thr_fn1(void *arg)
{
        sleep(1);//睡眠一秒，等待TID2结束。
        pthread_join(tid2, &tret);//tid1一直阻赛，等到tid2的退出，获得TID2的退出码
         printf("thread 2 exit code %d\n", (int)tret);
    printf("thread 1 returning\n");
    return((void *)2);
}
void *
thr_fn2(void *arg)
{      
    printf("thread 2 exiting\n");
     pthread_exit((void *)3);
}
int
main(void)
{
    int            err;
err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    if (err != 0)
        printf("can't create thread 1\n");
  err = pthread_create(&tid2, NULL, thr_fn2, NULL);
    if (err != 0)
        printf("can't create thread 2\n");
err = pthread_join(tid1, &tret);                 //祝线程一直阻赛，等待TID1的返回。
    if (err != 0)
        printf("can't join with thread 1\n");
    printf("thread 1 exit code %d\n", (int)tret);
      //err = pthread_join(tid2, &tret);
    //if (err != 0)
    //    printf("can't join with thread 2\n");
//    printf("thread 2 exit code %d\n", (int)tret);
    exit(0);
} 
命令：＃gcc -lthread myfile11-3.c 
        ：＃./a.out 
运行结果： 
thread 2 exiting
thread 2 exit code 3
thread 1 returning
thread 1 exit code 2
pthread_t pthread_self();获取线程自身ID
int pthread_equal(pthread_t threadid1, pthread_t thread2)判断两个线程ID是否相等，返回0 不相等，非零相等

