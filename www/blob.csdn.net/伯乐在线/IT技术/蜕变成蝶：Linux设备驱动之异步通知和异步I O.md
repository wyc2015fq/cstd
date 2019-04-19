# 蜕变成蝶：Linux设备驱动之异步通知和异步I/O - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [李辉](http://www.jobbole.com/members/harrylhmj) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
在设备驱动中使用异步通知可以使得对设备的访问可进行时，由驱动主动通知应用程序进行访问。因此，使用无阻塞I/O的应用程序无需轮询设备是否可访问，而阻塞访问也可以被类似“中断”的异步通知所取代。异步通知类似于硬件上的“中断”概念，比较准确的称谓是“信号驱动的异步I/O”。
1、异步通知的概念和作用
影响：阻塞–应用程序无需轮询设备是否可以访问
非阻塞–中断进行通知
即：由驱动发起，主动通知应用程序
2、linux异步通知编程
2.1 linux信号
作用：linux系统中，异步通知使用信号来实现
函数原型为：

C
```
void (*signal(int signum,void (*handler))(int)))(int)
```

原型比较难理解可以分解为

C
```
typedef void(*sighandler_t)(int);
 
sighandler_t signal(int signum,sighandler_t handler);
```

第一个参数是指定信号的值，第二个参数是指定针对前面信号的处理函数
2.2 信号的处理函数（在应用程序端捕获信号）
signal()函数
例子：

C
```
//启动信号机制
 
void sigterm_handler(int sigo)
{
 
char data[MAX_LEN];
int len;
len = read(STDIN_FILENO,&data,MAX_LEN);
data[len] = 0;
printf("Input available:%s\n",data);
exit(0);
 
}
 
int main(void)
{
 
int oflags;
//启动信号驱动机制
 
signal(SIGIO,sigterm_handler);
fcntl(STDIN_FILENO,F_SETOWN,getpid());
oflags = fcntl(STDIN_FILENO,F_GETFL);
fctcl(STDIN_FILENO,F_SETFL,oflags | FASYNC);
//建立一个死循环，防止程序结束
 
whlie(1);
 
return 0;
 
}
```

2.3 信号的释放 (在设备驱动端释放信号)
为了是设备支持异步通知机制，驱动程序中涉及以下3项工作
（1）、支持F_SETOWN命令，能在这个控制命令处理中设置filp->f_owner为对应的进程ID。不过此项工作已由内核完成，设备驱动无须处理。
（2）、支持F_SETFL命令处理，每当FASYNC标志改变时，驱动函数中的fasync()函数得以执行。因此，驱动中应该实现fasync()函数
（3）、在设备资源中可获得，调用kill_fasync（）函数激发相应的信号
设备驱动中异步通知编程：
（1）、fasync_struct加入设备结构体模板中
（2）、两个函数
处理FASYNC标志的两个函数： int fasync_helper(int fd,struct file *filp,int mode,struct fasync_struct **fa);
释放信号的函数： void kill_fasync(struct fasync_struct **fa,int sig,int band);
和其他结构体指针放到设备结构体中，模板如下

C
```
struct xxx_dev{
struct cdev cdev;
...
struct fasync_struct *async_queue;//异步结构体指针
 
};
```

2.4 在设备驱动中的fasync()函数中，只需简单地将该函数的3个参数以及fasync_struct结构体指针的指针作为第四个参数传入fasync_helper()函数就可以了，模板如下

C
```
static int xxx_fasync(int fd,struct file *filp, int mode)
{
　　struct xxx_dev *dev = filp->private_data;
　　return fasync_helper(fd, filp, mode, &dev->async_queue);
}
```

2.5 在设备资源可获得时应该调用kill_fasync()函数释放SIGIO信号，可读时第三个参数为POLL_IN，可写时第三个参数为POLL_OUT，模板如下

C
```
static ssize_t xxx_write(struct file *filp,const char __user *buf,size_t count,loff_t *ppos)
 
{
 
struct xxx_dev *dev = filp->private_data;
...
 
if(dev->async_queue)
 
kill_fasync(&dev->async_queue,GIGIO,POLL_IN);
 
...
 
}
```

2.6 最后在文件关闭时，要将文件从异步通知列表中删除

C
```
int xxx_release(struct inode *inode,struct file *filp)
 
{
xxx_fasync(-1,filp,0);
 
...
return 0;
 
}
```

3、linux2.6异步I/O
同步I/O：linux系统中最常用的输入输出（I/O）模型是同步I/O，在这个模型中，当请求发出后，应用程序就会阻塞，知道请求满足
异步I/O：I/O请求可能需要与其它进程产生交叠
Linux 系统中最常用的输入/输出（I/O）模型是同步 I/O。在这个模型中，当请求发出之后，应用程序就会阻塞，直到请求满足为止。这是很好的一种解决方案，因为调用应用程序在等待 I/O 请求完成时不需要使用任何中央处理单元（CPU）。但是在某
些情况下，I/O 请求可能需要与其他进程产生交叠。可移植操作系统接口（POSIX）异步 I/O（AIO）应用程序接口（API）就提供了这种功能
4.1、AIO系列API：
**aio_read–异步读**
aio_read 函数的原型如下：

C
```
int aio_read( struct aiocb *aiocbp );
```

aio_read()函数在请求进行排队之后会立即返回。如果执行成功，返回值就为 0；如果出现错误，返回值就为−1，并设置 errno 的值。
**aio_write–异步写**
aio_write()函数用来请求一个异步写操作，其函数原型如下：

C
```
int aio_write( struct aiocb *aiocbp );
```

aio_write()函数会立即返回，说明请求已经进行排队（成功时返回值为 0，失败时返回值为−1，并相应地设置 errno。
**aio_error–确定请求的状态**
aio_error 函数被用来确定请求的状态，其原型如下：

C
```
int aio_error( struct aiocb *aiocbp );
```

这个函数可以返回以下内容。
EINPROGRESS：说明请求尚未完成。
ECANCELLED：说明请求被应用程序取消了。
-1：说明发生了错误，具体错误原因由 errno 记录。
**aio_return–获得异步操作的返回值**
异步 I/O 和标准块 I/O 之间的另外一个区别是不能立即访问这个函数的返回状态，因为并没有阻塞在 read()调用上。在标准的 read()调用中，返回状态是在该函数返回时提供的。但是在异步 I/O 中，我们要使用 aio_return()函数。这个函数的原型如下：

C
```
ssize_t aio_return( struct aiocb *aiocbp );
```

只有在 aio_error()调用确定请求已经完成（可能成功，也可能发生了错误）之后，才会调用这个函数。aio_return()的返回值就等价于同步情况中 read 或 write 系统调用的返回值（所传输的字节数，如果发生错误，返回值就为−1）。
**aio_suspend–挂起异步操作，知道异步请求完成为止**
aio_suspend()函数来挂起（或阻塞）调用进程，直到异步请求完成为止，此时会产生一个信号，或者发生其他超时操作。调用者提供了一个 aiocb 引用列表，其中任何一个完成都会导致 aio_suspend()返回。aio_suspend 的函数原型如下：

C
```
int aio_suspend( const struct aiocb *const cblist[], int n, const struct timespec *timeout );
```

**aio_cancel–取消异步请求**
aio_cancel()函数允许用户取消对某个文件描述符执行的一个或所有 I/O 请求。其原型如下：

C
```
int aio_cancel( int fd, struct aiocb *aiocbp );
```
如果要取消一个请求，用户需提供文件描述符和 aiocb 引用。如果这个请求被成功取消了，那么这个函数就会返回 AIO_CANCELED。如果请求完成了，这个函数就会返回AIO_NOTCANCELED。 如果要取消对某个给定文件描述符的所有请求，用户需要提供这个文件的描述符以及一个对 aiocbp 的 NULL 引用。如果所有的请求都取消了，这个函数就会返回AIO_CANCELED ；如果至少有一个请求没有被取消，那么这个函数就会返回AIO_NOT_CANCELED；如果没有一个请求可以被取消，那么这个函数就会返回AIO_ALLDONE。然后，可以使用 aio_error()来验证每个 AIO 请求，如果某请求已经被取消了，那么 aio_error()就会返回−1，并且 errno 会被设置为 ECANCELED。
**lio_listio–同时发起多个传输（一次系统调用可以启动大量的I/O操作）**
lio_listio()函数可用于同时发起多个传输。这个函数非常重要，它使得用户可以在一个系统调用（一次内核上下文切换）中启动大量的 I/O 操作。lio_listio API 函数的原型如下：

C
```
int lio_listio( int mode, struct aiocb *list[], int nent, struct sigevent *sig );
```
mode 参数可以是 LIO_WAIT 或 LIO_NOWAIT。LIO_WAIT 会阻塞这个调用，直到所有的 I/O 都完成为止。在操作进行排队之后，LIO_NOWAIT 就会返回。list 是一个 aiocb 引用的列表，最大元素的个数是由 nent 定义的。如果 list 的元素为 NULL，lio_listio()会将其忽略。
3.2、使用信号作为AIO的通知
信号作为异步通知的机制在AIO中依然使用，为了使用信号，使用AIO的应用程序同样需要定义信号处理程序，在指定的信号被触发时，调用这个处理程序，作为信号上下文的一部分，特定的 aiocb 请求被提供给信号处理函数用来区分 AIO 请求。 下面代码清单给出了使用信号作为 AIO 异步 I/O 通知机制的例子。

C
```
/*设置异步 I/O 请求*/
void
int
struct
struct
/* 设置信号处理函数 */
/* 设置 AIO 请求 */
charsizeofstruct
malloc
/* 连接 AIO 请求和信号处理函数 */
/* 将信号与信号处理函数绑定 */
/*发出异步读请求*/
/*信号处理函数*/
void  int  void
struct
/* 确定是我们需要的信号*/
if
struct /*获得 aiocb*/
/* 请求的操作完成了吗? */
if
/* 请求的操作完成，获取返回值 */
return
```

3.3 使用回调函数作为AIO的通知
![](http://jbcdn2.b0.upaiyun.com/2016/04/5ae2f49a1c1ed0a6af3f90128f1a7fa4.png)
代码清单给出了使用回调函数作为 AIO 异步 I/O 请求完成的通知机制的例子

C
```
/*设置异步 I/O 请求*/
void
int
struct
/* 设置 AIO 请求 */
charsizeofstruct
malloc
/* 连接 AIO 请求和线程回调函数 */
/*设置回调函数*/
//发起 AIO 请求
/* 异步 I/O 完成回调函数 */
void
struct
struct
/* AIO 请求完成? */
if
/* 请求完成，获得返回值 */
return
```

3.4 AIO与设备驱动
在内核中，每个I/O请求都对应一个kiocb结构体，其ki_filp成员只想对应的file指针，通过is_sync_kiocb判断某kiocb是否为同步I/O请求，如果是返回真，表示为异步I/O请求。
块设备和网络设备：本身是异步的
字符设备：必须明确应支持AIO（极少数是异步I/O操作）
 字符设备驱动程序中file_operations 包含 3 个与 AIO 相关的成员函数，如下所示：

C
```
struct char size_t
 
struct const char size_t
 
int struct int
```
over~
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/d4181232cde3a79b8c6d11e39fd06ad2.png)![](http://www.jobbole.com/wp-content/uploads/2016/04/0e2c29b501d9020b8ef4615fe1493fc0.jpg)
