# 仿照printk编写自己的打印函数到缓冲区 - TaigaComplex求职中 - 博客园







# [仿照printk编写自己的打印函数到缓冲区](https://www.cnblogs.com/TaigaCon/archive/2012/12/30/2839525.html)





我们知道printk分为两步，一是把要输出的信息放到缓冲区log_buf，另外一个是通过控制台输出信息。

这里构造myprintk函数，仿照printk的输出信息到缓冲区，并且把缓冲区对应/proc/mymsg文件节点，当cat    /proc/msg的时候会输出缓冲区信息

关于log_buf缓冲区的重要一点就是，该缓冲区是一个环形缓冲区（当然，这里用一个数组当作环形缓冲区），



该环形缓冲区有两个指针，读指针，写指针，

当写入一个数据的时候，写指针+1，　　写入表示存储数据，等待读出

当读出一个数据的时候，读指针+1　　　读出表示该读出数据已经没用了

另外，环形缓冲区还有两个状态：空（empty），满（full）

当读指针等于写指针的时候，表示为缓冲区空

　　　　　　w==r

![](https://images0.cnblogs.com/blog/421096/201212/30024147-6294fe98c38748d59ec2fe8368bad927.jpg)



当写指针在读指针后面的时候表示满，当然，一般情况w是不可能在r后面的，因为必须先写进去才能读，

出现这种情况是因为写数据写满了缓冲区，还有一个位置就到了r，也就是

　　(w+1)%size==read

![](https://images0.cnblogs.com/blog/421096/201212/30024623-7bcf6bac014f44f6bd004df1a58118b1.jpg)





如果缓冲区已经满了之后，还要写入数据，那么读指针跟写指针都要+1

　　w=(w+1)%size

　　r= (r+1)%size

![](https://images0.cnblogs.com/blog/421096/201212/30024901-a302989d61774bd6b3fe3229eaed572a.jpg)



代码如下：

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/irq.h>
#include <linux/gpio.h>

#include <linux/poll.h>

#include <linux/sched.h>

#include <linux/interrupt.h>

#include <asm/uaccess.h>
#include <mach/irqs.h>
#include <asm/io.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/proc_fs.h>
//#include <asm-generic/Uaccess.h>
#define MYLOG_BUF_LEN 1024

struct proc_dir_entry *myentry;
static char mylog_buf[MYLOG_BUF_LEN];
static char tmp_buf[MYLOG_BUF_LEN];
static int mylog_w = 0;
static int mylog_r = 0;

static DECLARE_WAIT_QUEUE_HEAD(mylog_waitq);

static int is_mylog_empty(void)
{
    return (mylog_w==mylog_r);
}

static int is_mylog_full(void)
{
    return (((mylog_w+1)%MYLOG_BUF_LEN)==mylog_r);
}

static void mylog_putc(char c)
{
    if(is_mylog_full())
        {
            /*如果环形缓冲区满，丢弃一个数据*/
            mylog_r = (mylog_r+1)%MYLOG_BUF_LEN;
        }
        mylog_buf[mylog_w]=c;
        mylog_w = (mylog_w+1)%MYLOG_BUF_LEN;
        
        /*唤醒等待数据的进程*/
        wake_up_interruptible(&mylog_waitq);
}

static int mylog_getc(char *p)
{
    if(is_mylog_empty())
        {
            return 0;
        }
        *p = mylog_buf[mylog_r];
        mylog_r = (mylog_r+1)%MYLOG_BUF_LEN;
        return 1;
}

int myprintk(const char *fmt, ...)
{
    va_list args;
    int i,j;
    
    va_start(args,fmt);                /*具体可了解可变参数的函数处理*/
    i=vsnprintf(tmp_buf,INT_MAX,fmt,args);    /*解析参数，把所得的字符串放到tmp_buf缓存区*/
    va_end(args);
    
    for(j=0;j<i;j++)
    mylog_putc(tmp_buf[j]);
    
    return i;
}

static ssize_t mymsg_read(struct file *file, char __user *buf,
             size_t count, loff_t *ppos)
{
    int error=0;
    int i=0;
    char c;
    /*把mylog_buf的数据copy_to_user,return，
     *对于cat命令，如果碰到\n则会停止，否则循环输出
     */

    if((file->f_flags&O_NONBLOCK)&&is_mylog_empty())    /*如果以非阻塞方式打开，并且mylog_buf是空的*/
        return -EAGAIN;
        
    error=wait_event_interruptible(mylog_waitq,!is_mylog_empty());    /*否则等待数据输入（mylog_buf非空）*/
    
    while (!error && (mylog_getc(&c)) && i < count) {    /*如果没错误，并且能从mylog_buf取出一个字符，并且请求长度大于i*/
            error = __put_user(c,buf);        /*返回取出的字符给用户空间*/
            buf++;
            i++;
        }
        if (!error)
            error = i;
                
    return error;
}

static const struct file_operations proc_mymsg_operations = {
    .read = mymsg_read,
};

static int mymsg_init(void)
{
    //proc_create("kmsg", S_IRUSR, NULL, &proc_kmsg_operations);
    
    myentry = create_proc_entry("mymsg",S_IRUSR,NULL);    /*在proc目录下创建条目*/
    if(myentry)
        myentry->proc_fops = &proc_mymsg_operations;    /*设置该条目的fops为proc_mymsg_operations*/
    return 0;
}

static void mymsg_exit(void)
{
    remove_proc_entry("mymsg",NULL);
}

EXPORT_SYMBOL(myprintk);

module_init(mymsg_init);
module_exit(mymsg_exit);

MODULE_LICENSE("GPL");
```



测试代码如下：

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


#include <linux/module.h>


extern int myprintk(const char *fmt, ...);

static int test_init(void)
{
    static int i = 0;
    myprintk("test_init\n");
    myprintk("count = %d\n",i);
    
    return 0;
}

static void test_exit(void)
{
    myprintk("test_exit\n");
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
```



上面的代码是，当缓冲区内的数据被读出来后就处于无用状态了，并不能重复读取，

如果要实现重复读取，我们可以设置多一个指针r_head（“头指针”），让每次cat 　　/proc/mymsg时都从“头指针”所指的位置重新读取数据

也就是在open函数内 　　r = r_head



透着针遵循的移动规则如下：

在缓冲区没满的时候，头指针一直指向缓冲区头部

当缓冲区满的时候，头指针如前面所说的读指针 r 一样向后移动

![](https://images0.cnblogs.com/blog/421096/201212/30033324-852a55fb828444b4a569e9e63e74e6bd.jpg)



改进后代码如下：

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/irq.h>
#include <linux/gpio.h>

#include <linux/poll.h>

#include <linux/sched.h>

#include <linux/interrupt.h>

#include <asm/uaccess.h>
#include <mach/irqs.h>
#include <asm/io.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/proc_fs.h>
//#include <asm-generic/Uaccess.h>
#define MYLOG_BUF_LEN 1024

struct proc_dir_entry *myentry;
static char mylog_buf[MYLOG_BUF_LEN];
static char tmp_buf[MYLOG_BUF_LEN];
static int mylog_w = 0;
static int mylog_r = 0;
static int mylog_r_head = 0;

static DECLARE_WAIT_QUEUE_HEAD(mylog_waitq);

static int is_mylog_empty(void)
{
    return (mylog_w==mylog_r);
}

static int is_mylog_full(void)
{
    return (((mylog_w+1)%MYLOG_BUF_LEN)==mylog_r_head);
}

static void mylog_putc(char c)
{
    if(is_mylog_full())
        {
            /*如果环形缓冲区满，丢弃一个数据*/
            mylog_r_head = (mylog_r_head+1)%MYLOG_BUF_LEN;
        }
        mylog_buf[mylog_w]=c;
        mylog_w = (mylog_w+1)%MYLOG_BUF_LEN;
        
        /*唤醒等待数据的进程*/
        wake_up_interruptible(&mylog_waitq);
}

static int mylog_getc(char *p)
{
    if(is_mylog_empty())
        {
            return 0;
        }
        *p = mylog_buf[mylog_r];
        mylog_r = (mylog_r+1)%MYLOG_BUF_LEN;
        return 1;
}

int myprintk(const char *fmt, ...)
{
    va_list args;
    int i,j;
    
    va_start(args,fmt);                /*具体可了解可变参数的函数处理*/
    i=vsnprintf(tmp_buf,INT_MAX,fmt,args);    /*解析参数，把所得的字符串放到tmp_buf缓存区*/
    va_end(args);
    
    for(j=0;j<i;j++)
    mylog_putc(tmp_buf[j]);
    
    return i;
}

static int mymsg_open(struct inode *inode,struct file *file)
{
     /*每次cat /proc/mymsg都会open
        *这里mylog_r = mylog_r_head 表示每次都把r设置为“头指针”，从头开始读数据
        */
        mylog_r = mylog_r_head;        
        return 0;
}

static ssize_t mymsg_read(struct file *file, char __user *buf,
             size_t count, loff_t *ppos)
{
    int error=0;
    int i=0;
    char c;
    
    /*把mylog_buf的数据copy_to_user,return，
     *对于cat命令，如果碰到\n则会停止，否则循环输出
     */

    if((file->f_flags&O_NONBLOCK)&&is_mylog_empty())    /*如果以非阻塞方式打开，并且mylog_buf是空的*/
        return -EAGAIN;
        
    error=wait_event_interruptible(mylog_waitq,!is_mylog_empty());    /*否则等待数据输入（mylog_buf非空）*/
    
    while (!error && (mylog_getc(&c)) && i < count) {    /*如果没错误，并且能从mylog_buf取出一个字符，并且请求长度大于i*/
            error = __put_user(c,buf);        /*返回取出的字符给用户空间*/
            buf++;
            i++;
        }
        if (!error)
            error = i;
                
    return error;
}

static const struct file_operations proc_mymsg_operations = {
    .open = mymsg_open,
    .read = mymsg_read,
};

static int mymsg_init(void)
{
    //proc_create("kmsg", S_IRUSR, NULL, &proc_kmsg_operations);
    
    myentry = create_proc_entry("mymsg",S_IRUSR,NULL);    /*在proc目录下创建条目*/
    if(myentry)
        myentry->proc_fops = &proc_mymsg_operations;    /*设置该条目的fops为proc_mymsg_operations*/
    return 0;
}

static void mymsg_exit(void)
{
    remove_proc_entry("mymsg",NULL);
}

EXPORT_SYMBOL(myprintk);

module_init(mymsg_init);
module_exit(mymsg_exit);

MODULE_LICENSE("GPL");
```
















