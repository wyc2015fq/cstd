# 蜕变成蝶：Linux设备驱动中的阻塞和非阻塞I/O - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [李辉](http://www.jobbole.com/members/harrylhmj) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
今天写的是Linux设备驱动中的阻塞和非阻塞I/0，何谓阻塞与非阻塞I/O？简单来说就是对I/O操作的两种不同的方式，驱动程序可以灵活的支持用户空间对设备的这两种访问方式。
**一、基本概念：**
- 阻塞操作     ： 是指在执行设备操作时，若不能获得资源，则挂起进程直到满足操作条件后再进行操作。被挂起的进程进入休眠， 被从调度器移走，直到条件满足。
- 非阻塞操作  ：在不能进行设备操作时，并不挂起，它或者放弃，或者不停地查询，直到可以进行操作。非阻塞应用程序通常使用select系统调用查询是否可以对设备进行无阻塞的访问最终会引发设备驱动中   poll函数执行。
**二、轮询操作**
阻塞的读取一个字符：

C
```
char buf;
fd = open("/dev/ttyS1",O_RDWR);
.....
res = read(fd,&buf,1); 
//当串口上有输入时才返回,没有输入则进程挂起睡眠
if(res == 1)
{
  printf("%c/n",buf);
}
```
非阻塞的读一个字符：

C
```
char buf;
fd = open("/dev/ttyS1",O_RDWR|O_NONBLOCK);
//O_NONBLOCK 非阻塞标识
.....
while(read(fd,&buf,1)!=1);
//串口上没有输入则返回，所以循环读取
printf("%c/n",buf);
```
阻塞操作常常用等待队列来实现，而非阻塞操作用轮询的方式来实现。非阻塞I/O的操作在应用层通常会用到select()和poll()系统调用查询是否可对设备进行无阻塞访问。select()和poll()系统调用最终会引发设备驱动中的poll()函数被调用。这里对队列就不多介绍了，大家可以看看数据结构里面的知识点。
应用层的select()原型为：

C
```
int select(int numfds,fd_set *readfds,fd_set *writefds,fd_set *exceptionfds,
struct timeval *timeout);  numfds 的值为需要检查的号码最高的文件描述符加1，若select()在等待timeout时间后，若没有文件描述符准备好则返回。
```
应用程序为：

C
```
#inlcude------
main()
{
  int fd,num;
  char rd_ch[BUFFER_LEN];
  fd_set rfds,wfds;  
//读写文件描述符集
//以非阻塞方式打开/dev/globalfifo设备文件
  fd=open("/dev/globalfifo",O_RDWR|O_NONBLOCK);
  if(fd != -1)
  {
//FIFO 清零
   if(ioctl(fd,FIFO_CLEAR,0) < 0)
   {
     printf("ioctl cmd failed /n");
   }
   while(1)
   {
     FD_ZERO(&rfds);
     FD_ZERO(&wfds);
     FD_SET(fd,&rfds);
     FD_SET(fd,&wfds);
      select(fd+1,&rfds,&wfds,null,null);
    }
  }
}
```
下面说说设备驱动中的poll()函数，函数原型如下：

C
```
static unsigned int poll(struct file *file, struct socket *sock,poll_table *wait) 
//第一个参数是file结构体指针，第三个参数是轮询表指针，这个函数应该进行两项工作
```
- 对可能引起设备文件状态变化的等待队列调用poll_wait()函数，将对应的等待队列头添加到poll_table
- 返回表示是否能对设备进行无阻塞读，写访问的掩码
这里还要提到poll_wait()函数，很多人会以为是和wait_event()一样的函数，会阻塞的等待某件事情的发生，其实这个函数并不会引起阻塞，它的工作是把当前的进程增添到wait参数指定的等待列表poll_table中去，poll_wait()函数原型如下：

C
```
static inline void poll_wait(struct file * filp, wait_queue_head_t * wait_address, poll_table *p)
从中可以看出是将等待队列头wait_address添加到p所指向的结构体中(poll_table)
```
驱动函数中的poll()函数典型模板如下：

C
```
static unsigned int xxx_poll(struct file *filp,struct socket *sock,
 poll_table *wait)
{
unsigned int mask = 0;
struct xxx_dev *dev = filp->private_data;
//获得设备结构体指针
...
poll_wait(filp,&dev->r_wait,wait);
//加读等待队列头到poll_table
poll_wait(filp,&dev->w_wait,wait);
//加写等待队列头到poll_table
...
if(...)
//可读
mask |= POLLIN | POLLRDNORM;
if(...)
//可写
mask |= POLLOUT | POLLRDNORM;
...
return mask;
}<strong>　　</strong>
```
**三、支持轮询操作的globalfifo驱动**
在globalfifo的poll()函数中，首先将设备结构体重的r_wait和w_wait等待队列头加到等待队列表，globalfifo设备驱动的poll()函数如下：

C
```
static unsigned int gloablfif0_poll(struct file *filp,poll_table *wait)
{
     unsigned int mask = 0;
    struct globalfifo_dev *dev = filp->private_data;
    down(&dev->sem);
    poll_wait(filp,&dev->r_wait , wait)  ;
    poll_wait(filp,&dev->r_wait , wait)  ;
    if(dev->current_len != 0)
    {
          mask |= POLLIN | POLLRDNORM; 
    }
    if(dev->current_len != GLOBALFIFO_SIZE)
    {
        mask |= POLLOUT | POLLWRNORM;
    }
    up(&dev->sem);
    return mask;
}
```
**四、总结**
阻塞与非阻塞操作：
- 定义并初始化等待对列头；
- 定义并初始化等待队列；
- 把等待队列添加到等待队列头
- 设置进程状态（TASK_INTERRUPTIBLE（可以被信号打断）和TASK_UNINTERRUPTIBLE（不能被信号打断））
- 调用其它进程
poll机制：
- 把等待队列头加到poll_table
- 返回表示是否能对设备进行无阻塞读，写访问的掩码
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/d4181232cde3a79b8c6d11e39fd06ad2.png)![](http://www.jobbole.com/wp-content/uploads/2016/04/0e2c29b501d9020b8ef4615fe1493fc0.jpg)
