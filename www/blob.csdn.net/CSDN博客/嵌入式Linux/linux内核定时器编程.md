
# linux内核定时器编程 - 嵌入式Linux - CSDN博客

2014年04月17日 11:16:27[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：584个人分类：[Linux																](https://blog.csdn.net/weiqifa0/article/category/1388863)



## 1.linux内核定时器基本结构和函数
1）struct timer_list 一个struct timer_list对应了一个定时器。
\#include <linux/timer.h>
以下列出常用的接口：
```python
struct timer_list
  {
   /*....*/
   unsigned long expires;//定时器服务函数开始执行时间
   void (*function)(unsigned long);//定义一个指向定时器服务函数的指针function，服务函数有一个 unsigned long的参数，并且返回void
   unsigned long data；//定时时间到时，data参数会传入服务函数
  }
 
void init_timer(struct timer_list* timer)//初始化一个定时器
```

```python
-----------使用定时器的步骤--------------
struct timer_list  my_timer_list;//定义一个定时器，可以把它放在你的设备结构中
init_timer(&my_timer_list);//初始化一个定时器
my_timer_list.expire=jiffies+HZ;//定时器1s后运行服务程序
my_timer_list.function=timer_function;//定时器服务函数
add_timer(&my_timer_list);//添加定时器
void timer_function(unsigned long);//写定时器服务函数
del_timer(&my_timer_list);//当定时器不再需要时删除定时器
del_timer_sync(&my_timer_list);//基本和del_timer一样，比较适合在多核处理器使用，一般推荐使用del_timer_sync
```
-------------------------------------------------
2.以下是一个定时1s的驱动程序，直接上代码
```python
/*****************************************************************
原子操作，就是不能被更高等级中断抢夺优先的操作。你既然提这个问题，我就说深一点。
由于操作系统大部分时间处于开中断状态，所以，一个程序在执行的时候可能被优先级更高的线程中断。
而有些操作是不能被中断的，不然会出现无法还原的后果，这时候，这些操作就需要原子操作。就是不能被中断的操作。
1.atomic_read与atomic_set函数是原子变量的操作，就是原子读和原子设置的作用.
2.原子操作，就是执行操作的时候，其数值不会被其它线程或者中断所影响
3.原子操作是linux内核中一种同步的方式
typedef struct { volatile int counter; } atomic_t;
#define ATOMIC_INIT(i)        { (i) }
#define atomic_read(v)                ((v)->counter)
#define atomic_set(v,i)                (((v)->counter) = (i))
******************************************************************/  
#include <linux/miscdevice.h>  
#include <linux/delay.h>  
#include <asm/irq.h>  
//#include <mach/regs-gpio.h>  
//#include <mach/hardware.h>  
#include <linux/kernel.h>  
#include <linux/module.h>  
#include <linux/init.h>  
#include <linux/mm.h>  
#include <linux/fs.h>  
#include <linux/types.h>  
#include <linux/delay.h>  
#include <linux/moduleparam.h>  
#include <linux/slab.h>  
#include <linux/errno.h>  
#include <linux/ioctl.h>  
#include <linux/cdev.h>  
#include <linux/string.h>  
#include <linux/list.h>  
#include <linux/pci.h>  
#include <asm/uaccess.h>  
#include <asm/atomic.h>  
#include <asm/unistd.h>  
#include <asm/io.h>  
#include <asm/system.h>  
#include <asm/uaccess.h>  
#define TIMER_MAJOR 300  
#define TIMER_MINOR 0  
dev_t timer_dev_t;//设备号  
dev_t timer_dev_major=TIMER_MAJOR;  
dev_t timer_dev_minor=TIMER_MINOR;  
struct TIMER_DEV  
{  
  struct cdev cdev;  
  atomic_t count;  
  struct timer_list timer_list;  
  
};  
struct TIMER_DEV* timer_dev;  
//---------timer interrupt function----------------  
static void timer_function(unsigned long data)  
{  
	mod_timer(&(timer_dev->timer_list),jiffies+HZ);//重新设置时间  
	printk("current jiffies is %ld,count=%d\n",jiffies,timer_dev->count);  
	//(timer_dev->count)++;  
	atomic_inc(&(timer_dev->count));  
}  
//--------timer release function--------------------  
static int timer_release(struct inode* inode, struct file* filp)  
{  
	del_timer_sync(&(timer_dev->timer_list));  
	return 0;  
}  
  
//----------------file open function-----------------  
static int timer_open(struct inode* inode,struct file* filp)  
{  
	init_timer(&(timer_dev->timer_list));//初始化定时器  
	timer_dev->timer_list.function=timer_function;//设置定时器处理函数  
	timer_dev->timer_list.expires=jiffies+HZ;//处理函数1s后运行  
	add_timer(&timer_dev->timer_list);//添加定时器  
	atomic_set(&(timer_dev->count),0); //原子操作函数
	return 0;  
}  
//--------------------------------------  
//----------------timer_read function---------------  
static int timer_read(struct file* filp,char __user *buf,size_t count,loff_t* f_pos)  
{  
  
	 unsigned int counter=atomic_read(&(timer_dev->count));  
	if(copy_to_user(buf,(unsigned int*)&counter,sizeof(unsigned int)))  
	  {  
	   printk("copy to user error\n");  
	   goto out;  
	  }  
	return (sizeof(unsigned int));  
	out:  
	return (-EFAULT);  
  
}  
  
struct file_operations timer_ops={  
	.owner=THIS_MODULE,  
	.open=timer_open,  
	.read=timer_read,  
	.release=timer_release,  
};  
static int __init timer_init(void)  
{  
 int ret;  
   
 if(TIMER_MAJOR)//主设备号大于0，静态申请设备号  
    {  
    timer_dev_t=MKDEV(TIMER_MAJOR,TIMER_MINOR);  
    ret=register_chrdev_region(TIMER_MAJOR,1,"timer_dev");//first,count,name  
    }  
 else  
    {  
    ret=alloc_chrdev_region(&timer_dev_t,0,1,"time_dev");  
    timer_dev_major=MAJOR(timer_dev_t);  
    }  
 if(ret<0)  
    {  
    printk("can't get major %d\n",timer_dev_major);  
    return ret;  
    }  
//-----------------------------------------------------------  
 timer_dev=kmalloc(sizeof(struct TIMER_DEV),GFP_KERNEL);   
    memset(timer_dev,0,sizeof(struct TIMER_DEV));  
 cdev_init(&(timer_dev->cdev),&timer_ops); //linux字符设备的注册
 cdev_add(&(timer_dev->cdev),timer_dev_t,1);  //添加字符设备
 printk("init timer_dev success\n");  
return 0;  
  
  
}  
static void __exit timer_exit(void)  
{  
	kfree(timer_dev);  
	cdev_del(&(timer_dev->cdev));  
	unregister_chrdev_region(MKDEV(TIMER_MAJOR,0),1);  
}  
module_init(timer_init);  
module_exit(timer_exit);
```

测试程序
**[cpp]**[ view plain](http://blog.csdn.net/cbl709/article/details/7349538#)[copy](http://blog.csdn.net/cbl709/article/details/7349538#)
\#include <stdio.h>
\#include <stdlib.h>
\#include <unistd.h>
\#include <fcntl.h>
int main()
{
int fd;
int i;
int counter=0;
int old_counter=0;
fd=open("/dev/timer_dev",O_RDWR);
if(fd<0)
{
printf("open file error\n");
exit(1);
}
for(i=0;i<30;)//运行30s
{
read(fd,&counter,sizeof(int));
if(counter!=old_counter)
{
printf("second=%d\n",counter);
old_counter=counter;
i++;
}
}
close(fd);
exit (0);
}
测试结果 添加模块后使用dmesg查看内核信息
[ 1239.176994] current jiffies is 235468,count=123
[ 1240.174459] current jiffies is 235718,count=124
[ 1241.171920] current jiffies is 235968,count=125
[ 1242.169383] current jiffies is 236218,count=126
测试程序的结果
second=1
second=2
second=3
second=4
second=5
second=6
second=7
second=8
second=9
second=10
second=11


