# Linux c 开发 - 进程间通信 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年10月05日 17:19:37[initphp](https://me.csdn.net/initphp)阅读数：1126
个人分类：[Linux c 开发系列](https://blog.csdn.net/initphp/article/category/2609039)

所属专栏：[Linux c 开发系列](https://blog.csdn.net/column/details/mylinuxc.html)









进程间通信主要有以下几种方法：

1. 共享内存

2. 信号量

3. 管道

4. 命名管道

5. 消息队列

6. 套接口

7. 全双工管道

其中共享内存、信号量、管道、命名管道和消息队列可以实现本地进程间通信，而套接口和全双工管道属于网络编程部分。

这里只介绍管道、命名管道的使用。




### 1. 管道 

管道pipe通信。管道通信有一个显著的特点就是当一个管道建立后，将获得两个文件描述符，分别用于对管道的读取和写入。

管道pipe只能用于两个进程间的通信，而且两个进程需要有同源性，必须由同一个进程所派生出来的。

创建pipe：



```cpp
#include<unistd.h>
int pipe(int pipe[2]);         //其中pipe[0]是读取数据的描述字，pipe[1]是写数据的描述字
```

对管道的读写可以通过read和write方法进行读写操作，类似于文件操作。

看一个详细的例子：



```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
void check_exit(int status);
int main() {
        printf("Test\n");
        pid_t pid; //定义一个进程ID
        char buf[1024]; //容器
        int fd[2]; //其中fd[0]是读取数据的描述字，fd[1]是写数据的描述字
        if (pipe(fd) < 0) {
                printf("pipe erroe!");
                exit(1);
        }
        pid = fork(); //创建一个进程
        if (pid < 0) {
                printf("fork error!\n");
                exit(1);
        }
        if (pid == 0) {
                printf("child process! PID:%d\n", getpid()); //如果pid等于0,子进程,getpid()方法获取当前的进程ID
                close(fd[0]);
                char* x = "pipe hello!\n";
                write(fd[1], x, 12);//写入一个字符串    
        } else {
                printf("parent process! PID:%d\n", getpid()); //父进程
                close(fd[1]);
                int c;
                int rc;
                while( (rc = read(fd[0], &c, 1)) > 0 ){ //等待读取pipe管道中的数据  
                        putchar(c);
                }
        }
        return 0;
}
```

输出：



```cpp
Test
parent process! PID:3303
child process! PID:3304
pipe hello!
```






### 2. 命名管道



命名管道又称先入先出队列，是特殊的管道，存在于文件系统中。

命名管道和管道的最大区别就是命名管道可以用于任何两个进程间的通信，并不限制于这两个进程的同源。

创建命名管道的函数：



```cpp
#include <sys/types.h>  
    #include <sys/stat.h>  
    int mkfifo(const char *filename, mode_t mode);  
    int mknod(const char *filename, mode_t mode | S_IFIFO, (dev_t)0);
```
mkfifo函数更加简单和规范



filename：是存放管道的名称

mode：用于指定创建文件的权限。

如果调用成功，返回0；否则返回-1

打开FIFO文件通常有四种方式：



```cpp
open(const char *path, O_RDONLY);//1  
open(const char *path, O_RDONLY | O_NONBLOCK);//2  
open(const char *path, O_WRONLY);//3  
open(const char *path, O_WRONLY | O_NONBLOCK);//4
```


看一个例子：



```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
int main() {
        printf("Test\n");
        pid_t pid; //定义一个进程ID
        pid = fork(); //创建一个进程
        if (pid < 0) {
                printf("fork error!\n");
                exit(1);
        }
        if (pid == 0) {
                printf("child process! PID:%d\n", getpid()); //如果pid等于0,子进程,getpid()方法获取当前的进程ID
                int res = mkfifo("hello", 0777); //创建一个FIFO管道
                if (res != 0) {
                        printf("mkfifo error!");
                        exit(1);
                }
                int fd = -1;
                fd = open("hello", O_WRONLY); //打开文件，可写模式
                if (fd < 0) {
                        printf("openfile error!");
                        exit(1);
                }
                write(fd, "fifo\n", 5); //写入文件 
        } else {
                sleep(5); //等待5秒
                printf("parent process! PID:%d\n", getpid()); //父进程
                //读取fifo管道数据
                char x[1024];
                int len;
                int fd = open("hello", O_RDONLY); //读取fifo管道
                if (fd < 0) {
                        printf("open faile error!");
                        exit(1);
                }
                while(len = read(fd, x, 5) > 0) {
                        printf("%s", x);
                }
        }
        return 0;
}
```


输出：



```cpp
Test
child process! PID:2739
parent process! PID:2738
fifo
```






















