# LDD3源码分析之异步通知 - 一世豁然的专栏 - CSDN博客





2014年12月18日 16:22:16[一世豁然](https://me.csdn.net/Explorer_day)阅读数：672








本博客转载于：[http://blog.csdn.net/liuhaoyutz/article/details/7401418](http://blog.csdn.net/liuhaoyutz/article/details/7401418) [作者：刘昊昱]






一、异步通知机制的实现

本文分析LDD3第6章中的异步通知机制。

通过使用异步通知机制，应用程序可以在指定的I/O操作可执行时，收到一个信号，而不需要不停的使用轮询来查询设备。

要使用异步通知机制，对于用户空间程序来说，需要执行如下步骤：

首先，指定一个进程作为文件的“属主”。这是通过使用fcntl系统调用执行F_SETOWN命令完成的，该命令会把进程ID号保存在filp->f_owner中。这一步的目的是让内核知道应该通知哪个进程。

其次，在设备中设置FASYNC标志，这是通过fcntl的F_SETFL命令完成的。

执行完这两个步骤后，当指定的I/O操作可执行时，就会给相应进程发送一个SIGIO信号。

从内核的角度看，要实现异步通知机制，需要经过如下三个步骤：

首先，F_SETOWN被调用时，对filp->f_owner赋值，此外什么也不做。

其次，执行F_SETFL设置FASYNC时，调用驱动程序的fasync函数。只要filp->f_flags中的FASYNC标志发生了变化，就应该调用这个函数，以便把这个变化通知驱动程序，使其能做出正确响应。文件打开时，FASYNC标志默认是被清除的。

第三，当指定的I/O操作可执行时，所有注册为异步通知的进程都会被发送一个SIGIO信号。

第一步的实现很简单，在驱动程序部分没有什么可做的。第二步和第三部则要涉及维护一个动态数据结构，以跟踪不同的异步读取进程，这种进程可能有好几个。不过，这个动态数据结构并不依赖于特定设备，内核已经提供了一套通用的实现方法，没有必要为每个驱动程序重写这部分代码。

Linux的这种通用方法基于一个数据结构和两个函数，这个数据结构是struct fasync_struct，该结构体在linux/fs.h中定义如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7401418#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7401418#)

- struct fasync_struct {  
- int magic;  
- int fa_fd;  
- struct  fasync_struct   *fa_next; /* singly linked list */
- struct  file        *fa_file;  
- };  

和处理等待队列的方式类似，我们需要把一个该类型的指针插入设备特定的数据结构中去。回忆一下scullpipe设备结构体的定义：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7401418#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7401418#)

- 33struct scull_pipe {  
- 34        wait_queue_head_t inq, outq;       /* read and write queues */
- 35        char *buffer, *end;                /* begin of buf, end of buf */
- 36        int buffersize;                    /* used in pointer arithmetic */
- 37        char *rp, *wp;                     /* where to read, where to write */
- 38        int nreaders, nwriters;            /* number of openings for r/w */
- 39        struct fasync_struct *async_queue; /* asynchronous readers */
- 40        struct semaphore sem;              /* mutual exclusion semaphore */
- 41        struct cdev cdev;                  /* Char device structure */
- 42};  

第39行定义了fasync_struct结构体变量。两个相关函数分别是：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7401418#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7401418#)

- int fasync_helper(int fd, struct file * filp,   
- int on, struct fasync_struct **fapp);  
- void kill_fasync(struct fasync_struct **fp, int sig, int band)  

当一个打开的文件的FASYNC标志被修改时，调用fasync_helper以便从相关的进程列表中增加或删除文件。除了最后一个参数外，fasync_helper的其它参数与驱动程序的fasync函数相同，可以直接传递。scullpipe设备的fasync函数实现如下：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7401418#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7401418#)

- 253static int scull_p_fasync(int fd, struct file *filp, int mode)  
- 254{  
- 255    struct scull_pipe *dev = filp->private_data;  
- 256  
- 257    return fasync_helper(fd, filp, mode, &dev->async_queue);  
- 258}  

当指定的I/O操作可执行时，应使用kill_fasync通知所有的相关进程，该函数的第二个参数是要发送的信号(通常是SIGIO)，第三个参数是带宽(通常是POLL_IN)。由于提供给scullpipe的读取进程的新数据是由某个进程调用write产生的，所以kill_fasync函数在scullpipe的write函数中调用，代码片段如下所示：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7401418#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7401418#)

- 221    /* and signal asynchronous readers, explained late in chapter 5 */
- 222    if (dev->async_queue)  
- 223        kill_fasync(&dev->async_queue, SIGIO, POLL_IN);  

如果是针对写入的异步通知，kill_fasync的第三个参数必须为POLL_OUT。最后要做的是，当文件关闭时，必须调用fasync方法，以便从活动的异步通知进程列表中删除该文件。在scullpipe的close函数中，有如下代码：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7401418#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7401418#)

- 98    /* remove this filp from the asynchronously notified filp's */
- 99    scull_p_fasync(-1, filp, 0);  

二、测试scullpipe的异步通知机制LDD3提供了一个异步通知机制的测试程序examples/misc-progs/asynctest.c，其代码如下：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7401418#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7401418#)

- /*
-  * asynctest.c: use async notification to read stdin
-  *
-  * Copyright (C) 2001 Alessandro Rubini and Jonathan Corbet
-  * Copyright (C) 2001 O'Reilly & Associates
-  *
-  * The source code in this file can be freely used, adapted,
-  * and redistributed in source or binary form, so long as an
-  * acknowledgment appears in derived source files.  The citation
-  * should list that the code comes from the book "Linux Device
-  * Drivers" by Alessandro Rubini and Jonathan Corbet, published
-  * by O'Reilly & Associates.   No warranty is attached;
-  * we cannot take responsibility for errors or fitness for use.
-  */
- 
- #include <stdio.h>
- #include <stdlib.h>
- #include <string.h>
- #include <unistd.h>
- #include <signal.h>
- #include <fcntl.h>
- 
- int gotdata=0;  
- void sighandler(int signo)  
- {  
- if (signo==SIGIO)  
-         gotdata++;  
- return;  
- }  
- 
- char buffer[4096];  
- 
- int main(int argc, char **argv)  
- {  
- int count;  
- struct sigaction action;  
- 
-     memset(&action, 0, sizeof(action));  
-     action.sa_handler = sighandler;  
-     action.sa_flags = 0;  
- 
-     sigaction(SIGIO, &action, NULL);  
- 
-     fcntl(STDIN_FILENO, F_SETOWN, getpid());  
-     fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | FASYNC);  
- 
- while(1) {  
- /* this only returns if a signal arrives */
-         sleep(86400); /* one day */
- if (!gotdata)  
- continue;  
-         count=read(0, buffer, 4096);  
- /* buggy: if avail data is more than 4kbytes... */
-         write(1,buffer,count);  
-         gotdata=0;  
-     }  
- }  

第38 - 42行，设置信号处理函数sighandler。第24 - 29行，是信号处理函数sighandler的实现。第44行，指定当前进程作为标准输入设备的“属主”。第45行，在标准输入设备中设置FASYNC标志。第47 - 56行，循环等待，当输入设备有数据可读时，会发信号唤醒进程，并读取和打印信息。使用该程序测试scullpipe的过程如下图所示：我们把asynctest的标准输入重定向到/dev/scullpipe1，所以当/dev/scullpipe1有数据可读，时会向asynctest发信号SIGIO，唤醒asynctest，执行信号处理函数，然后读取并打印信息，再进入下次循环。如下图所示：![](https://img-my.csdn.net/uploads/201203/28/1332897422_2367.png)



