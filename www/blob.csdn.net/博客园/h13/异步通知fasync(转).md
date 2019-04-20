# 异步通知fasync(转) - h13 - 博客园
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
异步通知fasync是应用于系统调用signal和sigaction函数，下面我会使用signal函数。简单的说，signal函数就是让一个信号与一个函数对应，每当接收到这个信号就会调用相应的函数。
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
一、什么是异步通知
个人认为，异步通知类似于中断的机制，如下面的将要举例的程序，当设备可写时，设备驱动函数发送一个信号给内核，告知内核有数据可读,在条件不满足之前，并不会造成阻塞。而不像之前学的阻塞型IO和poll，它们是调用函数进去检查，条件不满足时还会造成阻塞。
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
二、应用层中启用异步通知机制
其实就三个步骤：
1）signal(SIGIO, sig_handler);
调用signal函数，让指定的信号SIGIO与处理函数sig_handler对应。
2）fcntl(fd, F_SET_OWNER, getpid());
指定一个进程作为文件的“属主(filp->owner)”，这样内核才知道信号要发给哪个进程。
3）f_flags = fcntl(fd, F_GETFL);
fcntl(fd, F_SETFL, f_flags | FASYNC);
在设备文件中添加FASYNC标志，驱动中就会调用将要实现的test_fasync函数。
三个步骤执行后，一旦有信号产生，相应的进程就会收到。
来个应用程序：
```
/*3rd_char_7/1st/app/monitor.c*/
1 #include <stdio.h>
2 #include <sys/types.h>
3 #include <sys/stat.h>
4 #include <fcntl.h>
5 #include <sys/select.h>
6 #include <unistd.h>
7 #include <signal.h>
8
9 unsigned int flag;
10
11 void sig_handler(int sig)
12 {
13 printf("<app>%s\n", __FUNCTION__);
14 flag = 1;
15 }
16
17 int main(void)
18 {
19 char buf[20];
20 int fd;
21 int f_flags;
22 flag = 0;
23
24 fd = open("/dev/test", O_RDWR);
25 if(fd < 0)
26 {
27 perror("open");
28 return -1;
29 }
30 /*三个步骤*/
31 signal(SIGIO, sig_handler);
32 fcntl(fd, F_SETOWN, getpid());
33 f_flags = fcntl(fd, F_GETFL);
34 fcntl(fd, F_SETFL, FASYNC | f_flags);
35
36 while(1)
37 {
38 printf("waiting \n"); //在还没收到信号前，程序还在不停的打印
39 sleep(4);
40 if(flag)
41 break;
42 }
43
44 read(fd, buf, 10);
45 printf("finish: read[%s]\n", buf);
46
47 close(fd);
48 return 0;
49 }
```
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
三、驱动中需要实现的异步通知
上面说的三个步骤，内核已经帮忙实现了前两个步骤，只需要我们稍稍实现第三个步骤的一个简单的传参。
实现异步通知，内核需要知道几个东西：哪个文件(filp)，什么信号(SIGIIO)，发给哪个进程(pid)，收到信号后做什么(sig_handler)。这些都由前两个步骤完成了。
回想一下，在实现等待队列中，我们需要将一个等待队列wait_queue_t添加到指定的等待队列头wait_queue_head_t中。
在这里，同样需要把一个结构体struct fasync_struct添加到内核的异步队列头(名字是我自己取的)中。这个结构体用来存放对应设备文件的信息(如fd, filp)并交给内核来管理。一但收到信号，内核就会在这个所谓的异步队列头找到相应的文件(fd)，并在filp->owner中找到对应的进程PID，并且调用对应的sig_handler了。
看一下fasync_struct
1097 struct fasync_struct {
1098 int magic;
1099 int fa_fd;
1100 struct fasync_struct *fa_next; /* singly linked list */ //一看就觉得他是链表
1101 struct file *fa_file;
1102 };
上面红色标记说所的步骤都是由内核来完成，我们只要做两件事情：
1）定义结构体fasync_struct。
struct fasync_struct *async_queue;
2）实现test_fasync，把函数fasync_helper将fd,filp和定义的结构体传给内核。
108 int test_fasync (int fd, struct file *filp, int mode)
109 {
110 struct _test_t *dev = filp->private_data;
111
112 return fasync_helper(fd, filp, mode, &dev->async_queue);
113 }
讲一下函数fasync_helper:
int fasync_helper(int fd, struct file * filp, int on, struct fasync_struct **fapp)
一看就知道，前面的三个参数其实就是teat_fasync的三个参数，只要我们定义号的fasync_struct结构体也传进去就可以了。内核会完成我上面红色自己所说的事情。
另外还有两件事：
3）当设备可写时，调用函数kill_fasync发送信号SIGIO给内核。
83 if (dev->async_queue){
84 kill_fasync(&dev->async_queue, SIGIO, POLL_IN);
85 }
讲解一下这个函数：
void kill_fasync(struct fasync_struct **fp, int sig, int band)
sig就是我们要发送的信号。
band(带宽)，一般都是使用POLL_IN，表示设备可读，如果设备可写，使用POLL_OUT
4）当设备关闭时，需要将fasync_struct从异步队列中删除：
117 test_fasync(-1, filp, 0);
删除也是调用test_fasync，不过改了一下参数而已。
既然说完了就上程序：上面的函数需要包含<linux/fs.h>
/*3rd_char_7/1st/test.c*/
23 struct _test_t{
24 char kbuf[DEV_SIZE];
25 unsigned int major;
26 unsigned int minor;
27 unsigned int cur_size;
28 dev_t devno;
29 struct cdev test_cdev;
30 wait_queue_head_t test_queue;
31 wait_queue_head_t read_queue;
32 wait_queue_head_t write_queue;
33 struct fasync_struct *async_queue; //1.定义结构体
34 };
。。。。省略。。。。
68 ssize_t test_write(struct file *filp, const char __user *buf, size_t count, loff_t *offset)
69 {
70 int ret;
71 struct _test_t *dev = filp->private_data;
72
73 if(copy_from_user(dev->kbuf, buf, count)){
74 ret = - EFAULT;
75 }else{
76 ret = count;
77 dev->cur_size += count;
78 P_DEBUG("write %d bytes, cur_size:[%d]\n", count, dev->cur_size);
79 P_DEBUG("kbuf is [%s]\n", dev->kbuf);
80 wake_up_interruptible(&dev->test_queue);
81 wake_up_interruptible(&dev->read_queue);
82
83 if (dev->async_queue){
84 kill_fasync(&dev->async_queue, SIGIO, POLL_IN); //3.可写时发送信号
85 }
86 }
87
88 return ret; //返回实际写入的字节数或错误号
89 }
。。。。省略。。。。
108 int test_fasync (int fd, struct file *filp, int mode) //2.实现test_fasync
109 {
110 struct _test_t *dev = filp->private_data;
111
112 return fasync_helper(fd, filp, mode, &dev->async_queue);
113 }
114
115 int test_close(struct inode *node, struct file *filp)
116 {
117 test_fasync(-1, filp, 0); //4文件关闭时将结构体从伊部队列中删除
118 return 0;
119 }
120
121 struct file_operations test_fops = {
122 .open = test_open,
123 .release = test_close,
124 .write = test_write,
125 .read = test_read,
126 .poll = test_poll,
127 .fasync = test_fasync, //此步骤切记
128 };
.。。。。。。
程序写完了就得验证一下：
[root: app]# insmod ../test.ko
major[253] minor[0]
hello kernel
[root: app]# mknod /dev/test c 253 0
[root: app]# ./monitor& //后台运行monitor
waiting
[root: app]# waiting //不停的打印，没有休眠
waiting
waiting
waiting
waiting
waiting
[root: app]# ./app_write //调用函数写数据，
<kernel>[test_write]write 10 bytes, cur_size:[10]
<kernel>[test_write]kbuf is [xiao bai]
<app>s<kernel>[test_read]read data..... //写完后minoter接收到信号，跳出循环读数据
<kernel>[test_read]read 10 bytes, cur_size:[0]
ig_handler //这是在sig_hanler里面打印的，本应出现在读函数之前，因为各个函数抢着打印，所以，出现了乱序，不过不影响验证。
finish: read[xiao bai]
[1] + Done ./monitor
贴张图总结一下：
![](http://blog168.chinaunix.net/attachment/201012/29/25014876_1293609596uR26.png)
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
四、阻塞型IO、poll和异步通知的区别：
宋宝华书上的图，描述的挺好的：图片不态清晰，将就一下。
![](http://blog168.chinaunix.net/attachment/201012/29/25014876_1293609617f2kD.png)
一个最重要的区别：
1）异步通知是不会造成阻塞的。
2）调用阻塞IO时如果条件不满足，会在驱动函数中的test_read或test_write中阻塞。
3）如果条件不满足，selcet会在系统调用中阻塞。
所谓的异步，就是进程可以在信号没到前干别的事情，等到信号到来了，进程就会被内核通知去做相应的信号操作。进程是不知道信号什么时候来的。
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
五，总结
今天只是讲了异步通知在内核中的实现，并且对应的应用函数和驱动函数需要做什么事情。最后总结了一下阻塞IO、poll和异步通知的区别。
转自：[http://blog.csdn.net/billowszpt/article/details/7184302](http://blog.csdn.net/billowszpt/article/details/7184302)
