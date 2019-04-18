# 【Linux】进程间通信-命名管道FIFO - 江南烟雨 - CSDN博客
2013年01月06日 15:41:07[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：18098
## **命名管道概述**
如果我们要在不相关的进程间交换数据，那么使用FIFO文件将会十分方便。
FIFO文件通常也称为命名管道（named pipe）。命名管道是一种特殊类型的文件，它在文件系统中以文件名的形式存在。
## **创建命名管道**
创建命名管道一般有两种方式：
### **命令行方式**
一个比较旧的方式是：
mknod filename p
这个命令并未出现在X/Open规范的命令列表中，所以可能并不是所有的类Unix系统都可以这样做。
推荐的做法是：
mkfifo filename
### **函数调用方式**
```cpp
#include <sys/types.h>
#include <sys/stat.h>
int mkfifo(const char *pathname, mode_t mode);
int mknod(const char *pathname, mode_t mode | S_FIFO, (dev_t)0);
```
函数说明
    mkfifo()会依参数pathname建立特殊的FIFO文件，该文件必须不存在，而参数mode为该文件的权限（mode%~umask），因此 umask值也会影响到FIFO文件的权限。Mkfifo()建立的FIFO文件其他进程都可以用读写一般文件的方式存取。当使用open()来打开 FIFO文件时，O_NONBLOCK旗标会有影响
    1、当使用O_NONBLOCK 旗标时，打开FIFO 文件来读取的操作会立刻返回，但是若还没有其他进程打开FIFO 文件来读取，则写入的操作会返回ENXIO 错误代码。
    2、没有使用O_NONBLOCK 旗标时，打开FIFO 来读取的操作会等到其他进程打开FIFO文件来写入才正常返回。同样地，打开FIFO文件来写入的操作会等到其他进程打开FIFO 文件来读取后才正常返回。
返回值
    若成功则返回0，否则返回-1，错误原因存于errno中。
错误代码
    EACCESS 参数pathname所指定的目录路径无可执行的权限
    EEXIST 参数pathname所指定的文件已存在。
    ENAMETOOLONG 参数pathname的路径名称太长。
    ENOENT 参数pathname包含的目录不存在
    ENOSPC 文件系统的剩余空间不足
    ENOTDIR 参数pathname路径中的目录存在但却非真正的目录。
    EROFS 参数pathname指定的文件存在于只读文件系统内。
### 代码演示
下面代码演示了mkfifo函数的用法：
```cpp
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
int main()
{
    int res = mkfifo("~/Test/PipeTest/my_fifo",0777);
    if(!res)
	printf("FIFO created\n");
    exit(EXIT_SUCCESS);
}
```
运行结果：
![](https://img-my.csdn.net/uploads/201301/06/1357439270_1960.png)
linux中ls命令的-F选项是列出文件的类型。
## 访问FIFO文件
同样有两种方式访问FIFO文件。
### 命令行方式
首先用cat命令读取刚才创建的FIFO文件：
cat < /tmp/my_fifo
这个时候，cat命令将一直挂起，直到终端或者有数据发送到FIFO中。
然后尝试向FIFO中写数据（在另外一个终端执行这个命令）
echo "FIFO test" > /tmp/my_fifo
这个时候cat将会输出内容。
### 函数调用方式
首先需要注意的是：
与通过pipe调用创建管道不同，FIFO是以命名文件的形式存在，而不是打开的文件描述符，所以在对它进行读写操作之前必须先打开它。
**1、使用open函数打开FIFO文件**
```cpp
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int open(const char *pathname, int flags);
```
打开FIFO文件和普通文件的区别有2点：
**第一个是不能以O_RDWR模式打开FIFO文件进行读写操作。**这样做的行为是未定义的。
因为我们通常使用FIFO只是为了单向传递数据，所以没有必要使用这个模式。
如果确实需要在程序之间双向传递数据，最好使用一对FIFO或管道，一个方向使用一个。或者采用先关闭在重新打开FIFO的方法来明确改变数据流的方向。
**第二是对标志位的O_NONBLOCK选项的用法。**
使用这个选项不仅改变open调用的处理方式，还会改变对这次open调用返回的文件描述符进行的读写请求的处理方式。
O_RDONLY、O_WRONLY和O_NONBLOCK标志共有四种合法的组合方式：
- flags=O_RDONLY：open将会调用阻塞，除非有另外一个进程以写的方式打开同一个FIFO，否则一直等待。
- flags=O_WRONLY：open将会调用阻塞，除非有另外一个进程以读的方式打开同一个FIFO，否则一直等待。
- flags=O_RDONLY|O_NONBLOCK：如果此时没有其他进程以写的方式打开FIFO，此时open也会成功返回，此时FIFO被读打开，而不会返回错误。
- flags=O_WRONLY|O_NONBLOCK：立即返回，如果此时没有其他进程以读的方式打开，open会失败打开，此时FIFO没有被打开，返回-1。
测试代码：
```cpp
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#define FIFO_NAME "/tmp/my_fifo"
int main(int argc,char *argv[])
{
        int res,i;
        int open_mode=0;
        if(argc < 2){
                fprintf(stderr,"Usage:%s<some combination of \
                        O_RDONLY,O_WRONLY,O_NONBLOCK\n",*argv);
                exit(EXIT_FAILURE);
        }
        argv++;
        if(strncmp(*argv,"O_RDONLY",8)==0)open_mode|=O_RDONLY;
        if(strncmp(*argv,"O_WRONLY",8)==0)open_mode|=O_WRONLY;
        if(strncmp(*argv,"O_NONBLOCK",10)==0)open_mode|=O_NONBLOCK;
       
	for(i = 1;i < argc;++i)
	{
       		argv++;
        	if(*argv){
                	if(strncmp(*argv,"O_RDONLY",8)==0)open_mode|=O_RDONLY;
                	if(strncmp(*argv,"O_WRONLY",8)==0)open_mode|=O_WRONLY;
                	if(strncmp(*argv,"O_NONBLOCK",10)==0)open_mode|=O_NONBLOCK;
        	}
	}
        if(access(FIFO_NAME,F_OK)==-1){
                res=mkfifo(FIFO_NAME,0777);
                if(res!=0){
                        fprintf(stderr,"Could not create fifo %s\n",FIFO_NAME);
                        exit(EXIT_FAILURE);
                }
        }
        printf("process %d open FIFO with %d\n",getpid(),open_mode);
        res=open(FIFO_NAME,open_mode);
        printf("process %d result %d\n",getpid(),res);
        sleep(5);
        if(res!=-1)close(res);
        printf("process %d finished\n",getpid());
        exit(EXIT_SUCCESS);
}
```
运行结果：
![](https://img-my.csdn.net/uploads/201301/06/1357442005_3145.png)
## 对FIFO文件进行读写操作
open函数调用中的参数标志O_NONBLOCK会影响FIFO的读写操作。
规则如下：
- 对一个空的阻塞的FIFO的read调用将等待，直到有数据可以读的时候才继续执行/
- 对一个空的非阻塞的FIFO的read调用立即返回0字节。
- 对一个完全阻塞的FIFO的write调用将等待，直到数据可以被写入时才开始执行。
- **系统规定：如果写入的数据长度小于等于PIPE_BUF字节，那么或者写入全部字节，要么一个字节都不写入。**
注意这个限制的作用：
当只使用一个FIF并允许多个不同的程序向一个FIFO读进程发送请求的时候，为了保证来自不同程序的数据块 不相互交错，即每个操作都原子化，这个限制就很重要了。如果能够包子所有的写请求是发往一个阻塞的FIFO的，并且每个写请求的数据长父小于等于PIPE_BUF字节，系统就可以确保数据绝不会交错在一起。通常将每次通过FIFO传递的数据长度限制为PIPE_BUF是一个好办法。
- 在非阻塞的write调用情况下，如果FIFO 不能接收所有写入的数据，将按照下面的规则进行：
- 请求写入的数据的长度小于PIPE_BUF字节，调用失败，数据不能被写入。
- 请求写入的数据的长度大于PIPE_BUF字节，将写入部分数据，返回实际写入的字节数，返回值也可能是0。
其中。PIPE_BUF是FIFO的长度，它在头文件limits.h中被定义。在linux或其他类UNIX系统中，它的值通常是4096字节。
在这里需要注意的是：
有两个进程去访问FIFO管道时，**Linux会安排好两个进程之间的调度**，使得两个进程在可以运行的时候运行，在不能运行的时候阻塞。
## 程序实例
下面的程序用命名管道在两个独立的进程之间通信，模拟了消费者和生产者程序。
生产者程序fork3.c：
```cpp
//生产者程序
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#define FIFO_NAME "/tmp/my_fifo"
#define BUFFER_SIZE PIPE_BUF
#define TEN_MSG (1024 * 1024 * 10)
int main()
{
    int pipe_fd;
    int res;
    int open_mode = O_WRONLY;
    int bytes_sent = 0;
    char buffer[BUFFER_SIZE + 1];
    printf("Productor Program beginning...\n");
    //检查FIFO文件是否存在
    if(access(FIFO_NAME, F_OK) == -1){
        res = mkfifo(FIFO_NAME, 0777);
        if(res != 0){
            fprintf(stderr, "Could not create fifo %s\n", FIFO_NAME);
            exit(EXIT_FAILURE);
        }
    }
    printf("Process %d opening FIFO O_WRONLY\n", getpid());
    //打开FIFO文件
    pipe_fd = open(FIFO_NAME, open_mode);
    printf("Process %d result %d\n", getpid(), pipe_fd);
    if(pipe_fd != -1){
        while(bytes_sent < TEN_MSG){
            res = write(pipe_fd, buffer, BUFFER_SIZE); //向FIFO写入数据
            if(res == -1){
                fprintf(stderr, "Write error on pipe\n");
                exit(EXIT_FAILURE);
            }
            bytes_sent += res;
        }
        (void)close(pipe_fd);
    }
    else{
        exit(EXIT_FAILURE);
    }
    
    printf("Process %d finished\n", getpid());
    exit(EXIT_SUCCESS);
}
```
消费者程序fork4.c：
```cpp
//消费者程序
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#define FIFO_NAME "/tmp/my_fifo"
#define BUFFER_SIZE PIPE_BUF
int main()
{
    int pipe_fd;
    int res;
    int open_mode = O_RDONLY;
    char buffer[BUFFER_SIZE + 1];
    int bytes = 0;
    printf("COnsumer Program beginning...");
    memset(buffer,'\0', sizeof(buffer));
    printf("Process %d opeining FIFO O_RDONLY\n", getpid());
    pipe_fd = open(FIFO_NAME, open_mode);
    printf("Process %d result %d\n", getpid(), pipe_fd);
    if (pipe_fd != -1)
    {
        do{
            res = read(pipe_fd, buffer, BUFFER_SIZE);
            bytes += res;
        }while(res > 0);
        close(pipe_fd);
    }
    else
    {
        exit(EXIT_FAILURE);
    }
    printf("Process %d finished, %d bytes read\n", getpid(), bytes);
    exit(EXIT_SUCCESS);
}
```
运行结果：

![](https://img-my.csdn.net/uploads/201301/06/1357456595_8764.png)
可以发现读进程只运行了不到0.1S的时间，却读取了10MB的数据。这说明管道在程序之间传递数据是很有效率的。
## 删除FIFO文件
FIFO文件使用完毕之后需删除，以免造成垃圾文件。
```cpp
#include <unistd.h>
int unlink(const char *pathname);
```
关于unlink的详细内容参考：
[unlink(2) - Linux man page](http://linux.die.net/man/2/unlink)
如需转载，请注明出处：[http://blog.csdn.net/xiajun07061225/article/details/8471777](http://blog.csdn.net/xiajun07061225/article/details/8471777)
