# 驱动程序调试方法之printk——自制proc文件（一） - DoubleLi - 博客园






首先我们需要弄清楚proc机制，来看看fs/proc/proc_misc.c这个文件，从入口函数开始看：

proc_misc_init(void)

        #ifdef CONFIG_PRINTK
 {

 struct proc_dir_entry *entry;

 entry = create_proc_entry("kmsg", S_IRUSR, &proc_root);//这里创建了一个proc入口kmsg

 if (entry)

 entry->proc_fops = &proc_kmsg_operations;//设置操作函数，见注释1

 }



**注释1：**


const struct file_operations proc_kmsg_operations = {

 .read = kmsg_read,

 .poll = kmsg_poll,

 .open = kmsg_open,

 .release = kmsg_release,

};


这个操作函数就用于多kmsg来进行操作



于是我们可以仿照来设计一下！


#include <linux/module.h>

#include <linux/kernel.h>

#include <linux/fs.h>

#include <linux/init.h>

#include <linux/delay.h>

#include <asm/uaccess.h>

#include <asm/irq.h>

#include <asm/io.h>

#include <asm/arch/regs-gpio.h>

#include <asm/hardware.h>

#include <linux/proc_fs.h>



struct proc_dir_entry *myentry;



const struct file_operations proc_mymsg_operations = {

};



static int mymsg_init(void)

{

 myentry = create_proc_entry("mymsg", S_IRUSR, &proc_root);

 if (myentry)

     myentry->proc_fops = &proc_mymsg_operations ;



    return 0;

}



void  mymsg_eixt(void)

{

    remove_proc_entry("mymsg", &proc_root);

} 



module_init(mymsg_init);

module_exit(mymsg_eixt);




这个函数只是在proc目录下面创建了一个入口而已！我们加载后：

ls /proc/mymsg -l

打印出如下信息：

-r--------    1 0        0               0 Feb  4 13:37 /proc/mymsg



如果我们想查看/proc/mymsg内容的话：


# cat /proc/mymsg

打印出来

cat: read error: Invalid argument


这也是理所当然的，因为我们根本没有读函数嘛！



所以我们接下来要做的就是来完成这个读函数，而这这个读函数里面我们要做的就是将mylog_buf中的中的数据拷贝到用户空间。有一个很关键的地方就是，我们的 mylog_buf 应该是一个环形队列，关于环形队列的概念我们先来说一下：

front指向队列首部，rear指向队列尾部，size表示队列长度。读只能从首部读，写的话只能从尾部写！写到尾部的话，就回去重头开始写！

当front==rear的时候，可以判断队列是空的。

当(rear+1)/size==front的时候，可以判断队列是满的。程序如下：


#include <linux/module.h>

#include <linux/kernel.h>

#include <linux/fs.h>

#include <linux/init.h>

#include <linux/delay.h>

#include <asm/uaccess.h>

#include <asm/irq.h>

#include <asm/io.h>

#include <asm/arch/regs-gpio.h>

#include <asm/hardware.h>

#include <linux/proc_fs.h>



#define MYLOG_BUF_LEN 1024



struct proc_dir_entry *myentry;



static char mylog_buf[MYLOG_BUF_LEN];

static char tmp_buf[MYLOG_BUF_LEN];

static int mylog_r = 0;    //用来标识读

static int mylog_w = 0;  //用来标识写



static DECLARE_WAIT_QUEUE_HEAD(mymsg_waitq);



//判断环形缓冲区是否为空

static int is_mylog_empty(void)

{

 return (mylog_r == mylog_w);

}

//判断环形缓冲区是否已满

static int is_mylog_full(void)

{

 return ((mylog_w + 1)% MYLOG_BUF_LEN == mylog_r);

}



/*写缓冲区：如果缓冲区已满的话，就让覆盖掉下一个要读的数据

*否则就直接写入

此外在写缓冲区函数里面还需要做的一件事情就是唤醒等待队列，

这是因为当缓冲区为空的时候，如果调用读函数的话，就会使进程

进入等待队列，理当在写入数据的时候唤醒进程

*/

static void mylog_putc(char c)

{

 if (is_mylog_full())

 {

/* 丢弃一个数据 */

 mylog_r = (mylog_r + 1) % MYLOG_BUF_LEN;

 }



 mylog_buf[mylog_w] = c;

 mylog_w = (mylog_w + 1) % MYLOG_BUF_LEN;



/* 唤醒等待数据的进程 */ 

    wake_up_interruptible(&mymsg_waitq);   /* 唤醒休眠的进程 */

}



/*读缓冲区：如果缓冲区为空的话，就返回0

否则从首部读出一个数据，返回1

*/

static int mylog_getc(char *p)

{

 if (is_mylog_empty())

 {

 return 0;

 }

 *p = mylog_buf[mylog_r];

 mylog_r = (mylog_r + 1) % MYLOG_BUF_LEN;

 return 1;

}



/*打印函数：这个函数是参考sprintf函数得编写的

*它将传递进来的参数转换为固定的格式之后，放入到一个临时缓冲区里面

*然后将环形缓冲区的值写入到mylog_buf缓冲区里面，详见注释2

*/

int myprintk(const char *fmt, ...)

{

 va_list args;

 int i;

 int j;



 va_start(args, fmt);

 i = vsnprintf(tmp_buf, INT_MAX, fmt, args);//将传进来的参数转换后放入tmp_buf

 va_end(args);



 for (j = 0; j < i; j++)

 mylog_putc(tmp_buf[j]);//将tmp_buf里面的东东放入mylog_buf缓冲区里面



 return i;

}



/*读函数：当在应用空间调用命令：cat /proc/mymsg的时候，会调用这个函数

*

*/

static ssize_t mymsg_read(struct file *file, char __user *buf,

  size_t count, loff_t *ppos)

{

 int error = 0;

 int i = 0;

 char c;



/* 把mylog_buf的数据copy_to_user, return */



        //如果为非阻塞且mylog_buf为空，那么就出错返回

 if ((file->f_flags & O_NONBLOCK) && is_mylog_empty())

 return -EAGAIN;



//如果mylog_buf为空的话进程进入等待队列，还记得我们在写缓冲区

       //函数里面会唤醒进程这件事情吧！

 error = wait_event_interruptible(mymsg_waitq, !is_mylog_empty());



/* copy_to_user */

        //首先从缓冲区里面获得一个字符，然后拷贝到用户空间

        //如果缓冲区还有信息的话，就再次获得字符，拷贝到用户

       //空间，直到缓冲区为空

 while (!error && (mylog_getc(&c)) && i < count) {

 error = __put_user(c, buf);//将c的内容拷贝到用户空间

 buf++;

 i++;

 }



 if (!error)

 error = i;



 return error;

}



const struct file_operations proc_mymsg_operations = {

 .read = mymsg_read,

};



static int mymsg_init(void)

{ 

 myentry = create_proc_entry("mymsg", S_IRUSR, &proc_root);

 if (myentry)

 myentry->proc_fops = &proc_mymsg_operations;

 return 0;

}



static void mymsg_exit(void)

{

 remove_proc_entry("mymsg", &proc_root);

}



module_init(mymsg_init);

module_exit(mymsg_exit);



/*因为myprintk是我们自己写的打印语句

*所以需要导出才能被使用，使用的时候还需要声明一下：

extern int myprintk(const char *fmt,...);

*/

EXPORT_SYMBOL(myprintk);



MODULE_LICENSE("GPL");




我们在来总结一下：在本文件里面我们做了两件事情，一件事情是定义了一个写函数，当我们在用户空间使用命令：cat /proc/mymsg的时候，就会调用到这个读函数，这个读函数会将mylog_buf中的数据拷贝到用户空间，那么mylog_buf里面的数据哪里来的呢？这就是我们做的另外一件事情，我们定义了一个打印函数，这个打印函数会将要打印的数据写入一个临时缓冲区，然后又从临时缓冲区里面取出数据放入mylog_buf中。cat /proc/mymsg的候就会将mylog_buf中的数据拷贝到用户空间，就可以显示出来了！



**注释2：**


int sprintf(char * buf, const char *fmt, ...)

{

 va_list args;

 int i;



 va_start(args, fmt);

 i=vsnprintf(buf, INT_MAX, fmt, args);

 va_end(args);

 return i;

}

这个就是sprintf函数，它将传递进来的参数放入buf缓冲区，我们做的就是将这个缓冲区里面的值在放入mylog_buf缓冲区里面！相当的巧妙啊！




接下来我们不妨测试一下，测试函数如下：




#include <linux/module.h>

#include <linux/kernel.h>

#include <linux/fs.h>

#include <linux/init.h>

#include <linux/delay.h>

#include <asm/uaccess.h>

#include <asm/irq.h>

#include <asm/io.h>

#include <asm/arch/regs-gpio.h>

#include <asm/hardware.h>



xtern int myprintk(const char *fmt, ...);



static int first_drv_init(void)

{

 myprintk("first_drv_init\n");



 return 0;

}



static void first_drv_exit(void)

{

    myprintk("abcdefhg\n");

}



module_init(first_drv_init);

module_exit(first_drv_exit);





MODULE_LICENSE("GPL");




先加载proc.ko

在加载test.ko

cat /proc/mymsg，打印如下信息：

first_drv_init

这正是我们打印的信息，成功了！










