# Linux c 开发 - 进程 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年10月03日 19:27:48[initphp](https://me.csdn.net/initphp)阅读数：1096
个人分类：[Linux c 开发系列](https://blog.csdn.net/initphp/article/category/2609039)

所属专栏：[Linux c 开发系列](https://blog.csdn.net/column/details/mylinuxc.html)









Linux的四大要素：

1. 一段供进程执行的程序，该程序可以被多个进程执行。

2. 独立的内核堆栈。

3. 进程控制快（task_struct：有了这个数据结构，进程才能成为内核调度的一个基本单位接受内核的调度。同时，这个结构还记录着进程所占用的各项资源。

4. 独立的存储空间：即拥有专有的用户空间，除了前面的内核空间还有用户空间。

c语言中派生进程的创建需要include下面的包：



```cpp
#include <unistd.h>
```



### 1. 创建进程

#### 1. fork方法创建进程



```cpp
pid_t fork(void);
```


通常将原有的进程称为父进程，而通过fork生成的继承称为子进程。



该方法返回pid_t。如果返回-1，则创建子进程失败。

使用fork之前的代码都在父进程中执行，而fork之后的代码会在父进程和子进程之间执行。fork调用会执行两次返回（父进程和子进程），当pid的值为0的时候可以判断为子进程。

可以看一个fork的例子：



```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
int main() {
        printf("Test\n");
        pid_t pid; //定义一个进程ID
        pid = fork(); //fork.在fork之前是一个进程运行，而fork之后是两个进程在运行了。
        if (pid < 0) {
                printf("fork error!\n");
                exit(1);
        }
        if (pid == 0) {
                printf("child process! PID:%d\n", getpid()); //如果pid等于0,子进程,getpid()方法获取当前的进程ID
        } else {
                printf("parent process! PID:%d\n", getpid()); //父进程
        }
        return 0;
}
```




#### 2. vfork方法创建进程





```cpp
pid_t vfork(void)
```


vfork方法并不完全复制父进程的数据段，而是和父进程共享数据段。调用vfork时，父进程会被挂起，子进程运行完exec函数或者exit的时候才会接触父进程的挂起状态。


看一个vfork例子：



```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
int main() {
        printf("Test\n");
        pid_t pid; //定义一个进程ID
        pid = vfork(); //vfork 父进程会被挂起，而子进程结束后才会继续执行父进程
        if (pid < 0) {
                printf("fork error!\n");
                exit(1);
        }
        if (pid == 0) {
                printf("child process! PID:%d\n", getpid()); //如果pid等于0,子进程,getpid()方法获取当前的进程ID
                exit(1); //父进程会等待子进程结束，子进程调用exit或者exec函数的时候，父进程的挂起状态会解除
        } else {
                printf("parent process! PID:%d\n", getpid()); //父进程
        }
        return 0;
}
```


输出：





```cpp
Test
child process! PID:2636
parent process! PID:2635
```





### 2. 进程等待



#### 1. wait函数的进程等待



```cpp
#include <sys/types.h>
#inculde <sys/wait.h>
pid_t wait(int *status);
```


wait函数专门用于等待子进程。参数status是一个指针，会将等待的结果放置在这个指针中。当调用wait的时候，父进程就会被挂起，直到该进程的一个子进程结束为止。



wait函数调用成功则返回等待状态的进程PID；如果失败，则返回-1

当参数status不为NULL的时候，可以用下面的宏来解析状态：

**WIFEXITED(status) **如果子进程正常结束则为非 0 值。
**WEXITSTATUS(status) **取得子进程 exit()返回的结束代码,一般会先用 WIFEXITED 来判断是否正常结束才能使用此宏。
**WIFSIGNALED(status) **如果子进程是因为信号而结束则此宏值为真
**WTERMSIG(status) **取得子进程因信号而中止的信号代码,一般会先用 WIFSIGNALED 来判断后才使用此宏。
**WIFSTOPPED(status)** 如果子进程处于暂停执行情况则此宏值为真。一般只有使用 WUNTRACED 时才会有此情况。
**WSTOPSIG(status)** 取得引发子进程暂停的信号代码,一般会先用 WIFSTOPPED 来判断后才使用此宏。

一个例子：



```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
void check_exit(int status);
int main() {
        printf("Test\n");
        pid_t pid; //定义一个进程ID
        int status;
        pid = vfork(); //vfork 父进程会被挂起，而子进程结束后才会继续执行父进程
        if (pid < 0) {
                printf("fork error!\n");
                exit(1);
        }
        if (pid == 0) {
                printf("child process! PID:%d\n", getpid()); //如果pid等于0,子进程,getpid()方法获取当前的进程ID
                exit(1); //父进程会等待子进程结束，子进程调用exit或者exec函数的时候，父进程的挂起状态会解除
        } else {
                if (wait(&status) != pid) {//等待操作
                        printf("wait Error!\n");
                        exit(0);
                }
                printf("parent process! PID:%d\n", getpid()); //父进程
                check_exit(status); //检测返回的状态
        }
        return 0;
}
void check_exit(int status)  {
    if(WIFEXITED(status))
        printf("Wait Success eixt\n");
    else if(WIFSIGNALED(status))
        printf("killed by signal\n");
    else if(WIFSTOPPED(status))
        printf("stopped by signal\n");
    else if(WIFCONTINUED(status))
        printf("continued");
}
```

输出：

```cpp
Test
child process! PID:2705
parent process! PID:2704
Wait Success eixt
```


#### 2. waitpid等待进程结束的函数



```cpp
#include <sys/types.h>
#inculde <sys/wait.h>
pid_t waitpid(pid_t pid, int *status, int option);
```


参数说明：



pid：

pid<-1   等待进程组识别码为 pid 绝对值的任何子进程。

pid=-1   等待任何子进程,相当于 wait()。            

pid=0     等待进程组识别码与目前进程相同的任何子进程。       

pid>0     等待任何子进程识别码为 pid 的子进程。

status：状态

option：

0 进程挂起，等待其结束


WNOHANG 如果没有任何已经结束的子进程则马上返回, 不予以等待。

WUNTRACED 如果子进程进入暂停执行情况则马上返回,但结束状态不予以理会。

waitpid能指定具体的线程ID，以及可以选择不同的等待方式，所以比wait更加灵活一些

例子：



```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
void check_exit(int status);
int main() {
        printf("Test\n");
        pid_t pid; //定义一个进程ID
        int status;
        pid = vfork(); //vfork 父进程会被挂起，而子进程结束后才会继续执行父进程
        if (pid < 0) {
                printf("fork error!\n");
                exit(1);
        }
        if (pid == 0) {
                printf("child process! PID:%d\n", getpid()); //如果pid等于0,子进程,getpid()方法获取当前的进程ID
                exit(1); //父进程会等待子进程结束，子进程调用exit或者exec函数的时候，父进程的挂起状态会解除
        } else {
                if (waitpid(-1, &status, 0) != pid) {//等待所有子进程，然后主进程挂起状态等待返回
                        printf("wait Error!\n");
                        exit(0);
                }
                printf("parent process! PID:%d\n", getpid()); //父进程
                check_exit(status); //检测返回的状态
        }
        return 0;
}
void check_exit(int status)  {
    if(WIFEXITED(status))
        printf("Wait Success eixt\n");
    else if(WIFSIGNALED(status))
        printf("killed by signal\n");
    else if(WIFSTOPPED(status))
        printf("stopped by signal\n");
    else if(WIFCONTINUED(status))
        printf("continued");
}
```

输出：



```cpp
Test
child process! PID:2705
parent process! PID:2704
Wait Success eixt
```




### 3. 进程终止

1. 常用的c标准进程终止函数



```cpp
void exit(int status);
```


2. abort发送一个SIGABRT信号使当前的进程终止。





```cpp
void abort(void)
```


3. atexit 用于注册一个不带参数也没有返回值的函数以供程序正常退出时被调用





```cpp
int atexit(void (* function)(void))
```


4. on_exit函数和atexit类似，但是不同的是它注册的函数具有参数，退出状态。调用成功返回0，失败返回0-1





```cpp
int on_exit(void (* function)(int, void*), void *arg)
```






























