# 【Linux学习】pthread_create主线程与创建的新线程之间退出关系 - 江南烟雨 - CSDN博客
2013年05月26日 16:21:06[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：19913标签：[多线程																[线程																[Linux](https://so.csdn.net/so/search/s.do?q=Linux&t=blog)](https://so.csdn.net/so/search/s.do?q=线程&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)
个人分类：[Linux](https://blog.csdn.net/xiajun07061225/article/category/856333)
我们在一个线程中经常会创建另外的新线程，如果主线程退出，会不会影响它所创建的新线程呢？下面就来讨论一下。
1、  主线程等待新线程先结束退出，主线程后退出。正常执行。
实例代码：
```cpp
#include "apue.h"
#include <pthread.h>
pthread_t ntid;//线程ID
void printids(const char *s)
{
        pid_t pid;
        pthread_t tid;
        pid = getpid();
        tid = pthread_self();
        printf("%s pid %u tid %u (0x%x)\n",s,(unsigned int)pid,
                        (unsigned int)tid,(unsigned int)tid);
}
void *thrfun(void *arg){
        //sleep(1);//使得主线程先退出
        printids("new thread");
        return ((void *)0);
}
int main(){
        int err;
        err = pthread_create(&ntid,NULL,thrfun,NULL);
        if(err != 0)
                err_quit("can't create thread: %s\n",strerror(err));
        printids("main thread");
        sleep(1);//等待新线程先结束
        exit(0);
}
```
运行结果：
![](https://img-blog.csdn.net/20130526161629163)
2、  进程先退出，新线程也会立即退出，系统清除所有资源。
实例代码：
```cpp
#include "apue.h"
#include <pthread.h>
pthread_t ntid;//线程ID
void printids(const char *s)
{
        pid_t pid;
        pthread_t tid;
        pid = getpid();
        tid = pthread_self();
        printf("%s pid %u tid %u (0x%x)\n",s,(unsigned int)pid,
                        (unsigned int)tid,(unsigned int)tid);
}
void *thrfun(void *arg){
        sleep(1);//使得主线程先退出
        printids("new thread");
        return ((void *)0);
}
int main(){
        int err;
        err = pthread_create(&ntid,NULL,thrfun,NULL);
        if(err != 0)
                err_quit("can't create thread: %s\n",strerror(err));
        printids("main thread");
        //sleep(1);
        exit(0);//注意是进程（不是线程）退出
}
```
运行结果：
![](https://img-blog.csdn.net/20130526161746828)
可以发现主线程退出后所创建的新线程也停止运行了。
3、如果主线程调用了pthread_exit，那么它退出了，子线程也不会退出。
实例代码：
```cpp
#include "apue.h"
#include <pthread.h>
pthread_t ntid;//线程ID
void printids(const char *s)
{
        pid_t pid;
        pthread_t tid;
        pid = getpid();
        tid = pthread_self();
        printf("%s pid %u tid %u (0x%x)\n",s,(unsigned int)pid,
                        (unsigned int)tid,(unsigned int)tid);
}
void *thrfun(void *arg){
        sleep(1);//使得主线程先退出
        printids("new thread");
        return ((void *)0);
}
int main(){
        int err;
        err = pthread_create(&ntid,NULL,thrfun,NULL);
        if(err != 0)
                err_quit("can't create thread: %s\n",strerror(err));
        printids("main thread");
        //sleep(1);
            pthread_exit(NULL);
        exit(0);
}
```
运行结果：
![](https://img-blog.csdn.net/20130526161838894)
POSIX标准定义：
When you program with POSIX Threads API,there is one thing about pthread_exit() that you may ignore for mistake. Insubroutines that complete normally, there is nothing special you have to dounless you want to pass a return code back using pthread_exit().
 The completionwon't affect the other threads which were created by the main thread of thissubroutine. However, in main(), when the code has been executed to the end,there could leave a choice for you. If you want to kill all the threads thatmain() created
 before, you can dispense with calling any functions. But if you want to keep the process and all the other threadsexcept for the main thread alive after the exit of main(), then you can call pthread_exit()to realize it. And any files
 opened inside the main thread will remain openafter its termination.
按照POSIX标准定义，当主线程在子线程终止之前调用pthread_exit()时，子线程是不会退出的。
注意：这里在main函数中调用pthread_exit()只会是主线程退出，而进程并未退出。因此新线程继续执行而没有退出。
我们可以在return 0;这条语句前面添加一条输出语句printf(“Mainthread has exited!\n”);来进行测试，输出结果不发生任何变化，说明这条语句没有被执行到。也就说明进程并未退出。
因此：
一个线程的退出不会影响另外一个线程。但是进程结束，所有线程也就结束了，所有资源会被回收。
我们可以再写一个程序来进行验证：
4、在创建的新线程B中再次创建新线程C，那么如果B先退出，那么C将会继续执行而不会退出。
实例代码：
```cpp
#include "apue.h"
#include<pthread.h>
pthread_t ntid;//线程ID
void printids(const char *s)
{
        pid_t pid;
        pthread_t tid;
        pid = getpid();
        tid = pthread_self();
        printf("%s pid %u tid %u (0x%x)\n",s,(unsigned int)pid,
                       (unsigned int)tid,(unsigned int)tid);
}
void *thrfun2(void *arg){
        sleep(1);//使得创建它的主线程先退出
        printids("new thread of the new thread");
        return ((void *)0);
}
void *thrfun(void *arg){
        sleep(1);//使得主线程先退出
        printids("new thread");
        int err;
        err = pthread_create(&ntid,NULL,thrfun2,NULL);
        if(err != 0)
                err_quit("can'tcreate thread: %s\n",strerror(err));
        return ((void *)0);
}
int main(){
        int err;
        err = pthread_create(&ntid,NULL,thrfun,NULL);
        if(err != 0)
                err_quit("can'tcreate thread: %s\n",strerror(err));
        printids("main thread");
        //sleep(1);
        pthread_exit(NULL);
        printf("main thread has exited!\n");
        exit(0);
}
```
运行结果：
![](https://img-blog.csdn.net/20130526161937377)
