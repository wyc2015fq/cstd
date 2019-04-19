# 蜕变成蝶：Linux设备驱动之中断与定时器 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [李辉](http://www.jobbole.com/members/harrylhmj) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
中断与定时器是我们再熟悉不过的问题了，我们在进行裸机开发学习的 时候，这几乎就是重难点，也是每个程序必要的模块信息，那么在Linux中，我们又怎么实现延时、计数，和中断呢？
一、中断
1.概述
所谓中断是指cpu在执行程序的过程中，出现了某些突发事件急待处理，cpu必需暂停执行当前执行的程序，转去处理突发事件，处理完之后cpu又返回原程序位置并继续执行，根据中断来源，中断分为内部中断和外部中断，软中断指令等属于内部中断，中断还可以分为可屏蔽中断和不可以屏蔽中断。Linux 的中断处理分为顶半部和底半部，顶半部完成尽可能少得的比较紧急的功能，往往只是简单的完成“登记中断”的工作，就是将底半部处理程序挂到该设备的底半部处理队列中去，中断处理机制如下图：
![](http://jbcdn2.b0.upaiyun.com/2016/04/7cfd2dcace4c60f8e4edbf3e453e6602.png)
2、中断编程
2.1 申请和释放中断
（1） 申请irq

C
```
int request_irq (unsigned int irq, irq_handler_t handler, unsigned long irqflags, const char *devname, void *dev_id)
```
irq 是要申请的中断号，handler是向系统登记的中断处理函数，irq_flags是中断处理的属性，可以指定中断的触发方式机处理方式，在处理方式方面，IRQF_DISABLED，表明中断处理程序是快速处理程序，快速处理程序被调用时屏蔽所有中断，IRQF_SHARED，表示多个设备共享中断（中断处理程序）。dev_id 在中断共享时会用到，一般设置为这个设备的结构体或者NULL.
（2） 释放irq

C
```
void free_irq (unsigned int irq, void *dev_id); 参数定义与request_irq()相同
```

2.2、使能屏蔽中断
 (1)　屏蔽(3个)
void disable_irq (int irq);
void disable_irq_nosync (ing irq);//立即返回
void enable_irq (int irq);
void disable_irq_nosync(int irq)与void disable_irq(int irg)的区别是前者立即返回，后者等待目前中断处理完。
(2) 屏蔽所有中断　　　　
#define local_irq_save (flags)//屏蔽本cpu所有
void local_irq_disable (void) //屏蔽本cpu所有中断
前者会保留中断状态保存在flags中（flags为unsigned long类型）。
(3） 恢复中断
#define local_irq_restore (flags)
void local_irq_enable (void);
以local开头的方法作用范围是本cpu内。
2.3 底半部机制–实现机制主要有tasklet, 工作队列和软中断
（1） tasklet

C
```
void my_tasklet_func (unsigned long);
 
DECLARE_TASKLET (my_tasklet, my_tasklet_func, data);
 
/*定义一个tasklet结构my_tasklet, 与my_tasklet_func(data)函数相关联*/
 
tasklet_schedule (&my_tasklet);
/*使系统在适当的时候调度tasklet注册的函数*/
```

（2）工作队列

C
```
struct work_struct my_wq;
 
void my_wq_func (unsigned long);
 
INIT_WORK (&my_wq, (void（＊）（ｖｏｉｄ　＊））ｍｙ＿ｗｑ＿ｆｕｎｃ，　ＮＵＬＬ）；
 
／＊初始化工作队列并将其与处理函数绑定＊／
 
schedule_work (&my_wq); /*调度工作队列执行*/
```
(3) 软中断（与通常说的软中断（软件指令引发的中断），比如arm的swi是完全不同的概念）
在linux内核中，用softirq_action结构体表征一个软中断，这个结构体包含软中断处理函数指针和传递给函数的参数。使用open_softirq()函数可以注册软中断对应的处理函数，而raise_softirq()函数可以触发一个软中断。软中断和tasklet 运行与软中断上下文，仍属于原子上下文的一种，而工作队列则运行与进程上下文。因此，软中断和tasklet处理函数中不能睡眠，而工作队列处理函数中允许睡眠。local_bh_disable() 和 local_bh_enable()是内核中用于禁止和使能软中断和tasklet底半部机制的函数。
2.4 中断共享
多个设备共享一根中断线的情况在硬件系统中广泛存在，共享中断的多个设备在申请中断时，都应该使用IRQF_SHARED标志，而且一个设备以IRQF_SHARED标志申请中断成功的前提是该中断未被申请或该中断虽然被申请了，但它之前申请该中断的设备都以IRQF_SHARED标志申请中断，尽管内核模块可以访问全局地址都可以作为request_irq(…,void *dev_id)的最后一个参数，但是社结构体被指针显然是可传入的最佳参数.
在中端到来时，会遍历共享此中断的所有中断处理程序，在中断处理程序顶半部中，应该根据硬件寄存器中的信息比照传入的dev_id参数判断是不是本设备的中断
共享中断模块

C
```
irqreturn_t xxx_interrupt(int irq,void *dev_id,struct pt_regs *regs)
 
{
 
...
 
int status = read_int_status();//获知中断源
 
if(!is_myint(dev_id,status))//判断是否为本设备
 
return IRQ_NONE;//不是本设备中断立即返回
 
//是本设备中断进行处理
 
...
 
return IRQ_HANDLED;//返回IRQ_HANDLED说明中断已被处理
 
}
 
...
```
二、定时器/时钟
1、概述
软件意义上的定时器最终依赖硬件定时器来实现，内核在时钟中断发生后检测个定时器释放到期，到期后的定时器处理函数将作为软中断底半部执行。驱动编程中，可以利用一组函数和数据结构来完成定时器触发工作或者某些周期性任务。
（1) 一个timer_list 结构体的实例对应一个定时器，其定义如下：

C
```
struct timer_list {
 
struct list_head entry, /*定时器列表*/
 
unsigned long expires, /*定时器到期时间*/
 
void (*function) (unsigned long), /*定时器处理函数*/
 
unsigned long data,/*作为参数被传入定时器处理函数*/
 
struct timer_base_s *base,
 
...
 
};
```

如定义一个名为my_timer 的定时器：

C
```
struct timer_list my_timer;
```

(2) 初始化定时器

C
```
void init_timer (struct timer_list *timer);
 
TIMER_INITIALIZER (_function, _expires, _data)
 
DEFINE_TIMER (_name, _function, _expires, _data)
 
setup_timer ();
```

(3) 增加定时器

C
```
void add_timer (struct timer_list *timer);
```

(4) 删除定时器

C
```
int del_timer (struct timer_list *timer);
```

(5) 修改定时器的expire

C
```
int mod_timer (struct timer_list *timer, unsigned long expires);
```

(6) 对于周期性的任务，linux内核还提供了一种delayed_work机制来完成，本质上用工作队列和定时器实现。
6.1,内核延时
linux内核中提供了如下3个函数分别进行纳秒，微妙和毫秒延时

C
```
void ndelay(unsigned long nsecs);
 
void udelay(unsigned long usecs);
 
void mdelay(unsigned long msecs);
```

上述延时实现的原理实质上是忙等待，毫秒延时比较cpu耗资源，对于毫秒级以上时延，内核提供了如下函数

C
```
void msleep(unsigned int millisecs);
 
unsigned long msleep_interruptible(unsigned int millisecs);
 
void ssleep(unsigned int seconds);
```

上述函数将使得调用它的进程，睡眠参数指定的时间，unsigned long msleep_interruptible（）可以被信号打断，另两个不行
6.2、睡着延迟
睡着延迟在等待的时间到来之间进程处于睡眠状态，schedule_timeout()可以使当前任务睡眠指定的jiffies之后重新被调度，msleep（）和msleep_interruptible()就包含了schedule_timeout()实质上schedule_timeout()的实现原理是向系统添加一个定时器，在定时器处理函数中唤醒参数对应的进程，其中结合了sleep_on()和__set_current_state(TASK_INTERRUPTIBLE)等函数。
2、内核定时器使用模板

C
```
//设备结构体
 
struct xxx_dev{
 
struct cdev cdev;
 
...
 
struct timer_list xxx_timer;//定义定时器
 
}
 
//驱动中某函数
 
xxx_funcl(...)
 
{
 
struct xxx_dev *dev = filp->private_data;
 
...
 
//初始化定时器
 
init_timer(&dev->xxx_time);
 
dev->xxx_timer.function = &xxx_do_timer;//定义定时器处理函数
 
dev->xxx_timer.data = (unsigned long)dev;//设备结构体指针作为定时器处理参数
 
dev->xxx_timer.expires = jiffies + delay;//定义到期时间
 
  
 
add_timer(&dev->xxx_timer);//注册定时器
 
...
 
  
 
}
 
//驱动中某函数
 
xxx_func2(...)
 
{
 
...
 
//删除中断
 
del_timer(&dev->xxx_timer);
 
...
 
}
 
//定时器处理函数
 
static void xxx_do_timer(unsigned long arg)
 
{
 
struct xxx_dev *dev = filp->private_data;
 
...
 
dev->xxx_timer.expires = jiffies + delay;//重新设置定时时间
 
add_timer(&dev->xxx_timer);
 
...
 
}
HZ表示延时1s
```
3、实例–秒字符设备second_drv.c ，它在被打开时将初始化的定时器加到内核定时器链表中，每秒输出一次当前的jiffes,代码如下：

C
```
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
 
#define SECOND_MAJOR 248
 
static int second_major = SECOND_MAJOR;
 
struct second_dev {
    struct cdev cdev;
    atomic_t counter;
    struct timer_list s_timer;
};
 
struct second_dev *second_devp;
static void second_timer_handle (unsigned long arg)
{
    mod_timer (&second_devp->s_timer, jiffies + HZ);
    atomic_inc (&second_devp->counter);
    printk (KERN_NOTICE "current jiffies is %ld\n", jiffies);
}
int second_open (struct inode *inode, struct file *filp)
{
    init_timer (&second_devp->s_timer);
    second_devp->s_timer.function = &second_timer_handle;
    second_devp->s_timer.expires = jiffies + HZ;
    add_timer (&second_devp->s_timer);
    atomic_set (&second_devp->counter, 0);
    return 0;
}
int second_release (struct inode *inode, struct file *filp)
{
    del_timer (&second_devp->s_timer);
    return 0;
}
static ssize_t second_read (struct file *filp, char __user *buf,
        size_t count, loff_t *ppos)
{
    int counter;
    counter = atomic_read (&second_devp->counter);
    if (put_user (counter, (int *)buf))
        return -EFAULT;
    else
        return sizeof (unsigned int);
}
static const struct file_operations second_fops = {
    .owner = THIS_MODULE,
    .open = second_open,
    .release = second_release,
    .read = second_read,
};
static void second_setup_cdev (struct second_dev *dev, int index)
{
    int err, devno = MKDEV (second_major, index);
    cdev_init (&dev->cdev, &second_fops);
    dev->cdev.owner = THIS_MODULE;
    err = cdev_add (&dev->cdev, devno, 1);
    if (err)
        printk (KERN_NOTICE "Error %d adding CDEV %d", err, index);
}
int second_init (void)
{
    int ret;
    dev_t devno = MKDEV (second_major, 0);
    if (second_major)
        ret = register_chrdev_region (devno, 1, "second");
    else {
        return alloc_chrdev_region (&devno, 0, 1, "second");
        second_major = MAJOR (devno);
    }
    if (ret < 0)
        return ret;
    second_devp = kmalloc (sizeof (struct second_dev), GFP_KERNEL);
    if (!second_devp) {
        ret = -ENOMEM;
        goto fail_malloc;
    }
    memset (second_devp, 0, sizeof (struct second_dev));
    second_setup_cdev (second_devp, 0);
    return 0;
fail_malloc:
    unregister_chrdev_region (devno, 1);
    return ret;
}
void second_exit (void)
{
    cdev_del (&second_devp->cdev);
    kfree (second_devp);
    unregister_chrdev_region (MKDEV (second_major, 0), 1);
}
MODULE_AUTHOR ("Ljia-----Ljia");
MODULE_LICENSE ("Dual BSD/GPL");
module_param (second_major, int, S_IRUGO);
module_init (second_init);
module_exit (second_exit);
```

在second的open()函数中，将启动定时器，此后每秒会再次运行定时器处理函数，且在release()函数中删除，编译驱动，加载并创建“/dev/second”设备文件节点之后，用以下程序打开，second_test会不断读取来自“/dev/second”设备文件以来经历的秒数。

C
```
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
 
int main (void)
{
    int fd;
    int counter = 0;
    int old_counter = 0;
 
    fd = open ("/dev/second", O_RDONLY);
    if (fd != -1) {
        while (1) {
            read (fd, &counter, sizeof (unsigned int));
            if (counter != old_counter) {
                printf ("seconds after open /dev/second: %d\n",
                        counter);
                old_counter = counter;
            }
        }
    } else {
        printf ("Device open failure\n");
    }
    return 0;
}
```

运行second_test后，不断输出jiffes的值，如下


```
current jiffes is 17216
current jiffes is 17316
current jiffes is 17416
current jiffes is 17516
current jiffes is 17616
current jiffes is 17716
current jiffes is 17816
current jiffes is 17916
current jiffes is 17016
current jiffes is 17116
current jiffes is 17216
current jiffes is 17316
```
而应用程序将不断输出来自打开的“/dev/second”如下：
```
```
seconds after open /dev/second :1
seconds after open /dev/second :2
seconds after open /dev/second :3
seconds after open /dev/second :4
seconds after open /dev/second :5
seconds after open /dev/second :6
seconds after open /dev/second :7
seconds after open /dev/second :8
seconds after open /dev/second :9
seconds after open /dev/second :10
```
```
三、总结
Linux中断处理分为两个半部，上述都讲得很清楚了，这里强调以下，为了充分利用CPU资源，在对延时使用不是很精确的情况下，睡眠等待值得推荐。对于上述的几个例子，需要大家自己在Linux的操作中敲出来，并且编译，看输出的结果才能完全理解~
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/d4181232cde3a79b8c6d11e39fd06ad2.png)![](http://www.jobbole.com/wp-content/uploads/2016/04/0e2c29b501d9020b8ef4615fe1493fc0.jpg)
