# Posix多线程编程学习笔记（三）—信号灯（2） - nosmatch的专栏 - CSDN博客
2012年01月10日 09:33:34[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：636标签：[多线程																[编程																[thread																[semaphore																[function																[input](https://so.csdn.net/so/search/s.do?q=input&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=semaphore&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)
个人分类：[多线程](https://blog.csdn.net/HDUTigerkin/article/category/1061405)
5.
|名称:：|sem_wait/sem_trywait|
|----|----|
|功能：|等待共享资源|
|头文件：|#include <semaphore.h>|
|函数原形：|int sem_wait(sem_t *sem);int sem_trywait(sem_t *sem);|
|参数：|sem 指向信号灯的指针|
|返回值：|若成功则返回0，否则返回-1。|

我们可以用sem_wait来申请共享资源，sem_wait函数可以测试所指定信号灯的值，如果该值大于0，那就将它减1并立即返回。我们就可以使用申请来的共享资源了。如果该值等于0，调用线程就被进入睡眠状态，直到该值变为大于0，这时再将它减1，函数随后返回。sem_wait操作必须是原子的。sem_wait和sem_trywait的差别是：当所指定信号灯的值已经是0时，后者并不将调用线程投入睡眠。相反，它返回一个EAGAIN错误。
下面的程序我们先不去运行，稍后再运行。
/*semwait.c*/
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
int main(int argc,char **argv)
{
sem_t *sem;
int val;
if(argc!=2)
{
    printf(“please input a file name!\n”);
    exit(1);
}
sem=sem_open(argv[1],0);
sem_wait(sem);
sem_getvalue(sem,&val);
printf(“pid %ld has semaphore,value=%d\n”,(long)getpid(),val);
pause();
exit(0);
}
6.
|名称:：|sem_post|
|----|----|
|功能：|挂出共享资源|
|头文件：|#include <semaphore.h>|
|函数原形：|int sem_post(sem_t *sem);int sem_getvalue(sem_t *sem,int *valp);|
|参数：|sem 指向信号灯的指针|
|返回值：|若成功则返回0，否则返回-1。|

当一个线程使用完某个信号灯时，它应该调用sem_post来告诉系统申请的资源已经用完。本函数和sem_wait函数的功能正好相反，它把所指定的信号灯的值加1，然后唤醒正在等待该信号灯值变为正数的任意线程。
下面的程序我们先不去运行，稍后再运行。
/*sempost.c*/
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
int main(int argc,char **argv)
{
sem_t *sem;
int val;
if(argc!=2)
{
    printf(“please input a file name!\n”);
    exit(1);
}
sem=sem_open(argv[1],0);
sem_post(sem);
sem_getvalue(sem,&val);
printf(“value=%d\n”, val);
exit(0);
}
**2.关于posix有名信号灯使用的几点注意**
我们要注意以下几点：
1.Posix有名信号灯的值是随内核持续的。也就是说，一个进程创建了一个信号灯，这个进程结束后，这个信号灯还存在，并且信号灯的值也不会改变。
下面我们利用上面的几个程序来证明这点
#./semopen test
#./semgetvalue test
value=1 
信号的值仍然是1
2。当持有某个信号灯锁的进程没有释放它就终止时，内核并不给该信号灯解锁。
#./semopen test
#./semwait test&
pid 1834 has semaphore,value=0
#./semgetvalue test 
value=0  
信号量的值变为0了
**3.posix有名信号灯应用于多线程**
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <thread.h>
void *thread_function(void *arg); /*线程入口函数*/
void print(pid_t); /*共享资源函数*/
sem_t *sem; /*定义Posix有名信号灯*/
int val; /*定义信号灯当前值*/
int main(int argc,char *argv[])
{
int n=0;
if(argc!=2)
{
    printf(“please input a file name!\n”);
    exit(1);
}
sem=sem_open(argv[1],O_CREAT,0644,3); /*打开一个信号灯*/
while(n++<5) /*循环创建5个子线程，使它们同步运行*/
{
    if（（pthread_create(&a_thread,NULL,thread_function,NULL)）!=0）
        {
             perror(“Thread creation failed”);
             exit(1);
         }
}
pthread_join(a_thread,NULL);
sem_close(bin_sem);
sem_unlink(argv[1]);
}
void *thread_function(void *arg)
{
sem_wait(sem); /*申请信号灯*/
    print(); /*调用共享代码段*/
    sleep(1);
    sem_post(sem); /*释放信号灯*/
    printf(“I’m finished,my tid is %d\n”,pthread_self());
}
void print()
{
printf(“I get it,my tid is %d\n”,pthread_self());
sem_getvalue(sem,&val);
printf(“Now the value have %d\n”,val);
}
程序用循环的方法建立5个线程，然后让它们调用同一个线程处理函数thread_function，在函数里我们利用信号量来限制访问共享资源的线程数。共享资源我们用print函数来代表，在真正编程中它有可以是一个终端设备（如打印机）或是一段有实际意义的代码。
运行结果为：
#gcc –lpthread –o 8_1 8_1.c
#./8_1 test
I get it,my tid is 1082330304
Now the value have 2
Iget it,my pid is 1894
Now the value have 1
Iget it,my pid is 1895
Now the value have 0
I’m finished,my pid is 1893
I’m finished,my pid is 1894
I’m finished,my pid is 1895
I get it,my pid is 1896
Now the value have 2
I get it,mypid is 1897
Now the value have 1
I’m finished,my pid is 1896
I’m finished,my pid is 1897
