# 驱动程序调试方法之printk——自制proc文件（二） - DoubleLi - 博客园






上一节的程序很振奋人心，我们自己实现了一个myprintk打印函数。但是这个函数存在一个致命的缺陷，那就是只能使用一次cat /proc/mymsg命令来读取mylog_buf的值。这是因为读到最后会出现：mylog_r == mylog_w，表示缓冲区为空，下一次就不能在读到数据了。在本节里面我们就着手来解决这个问题，我们要实现的就是每次使用 cat /proc/mymsg 时，都会从头打印。那么我们就需要将入口做一个拷贝，一个保存起来，一个进行变换。这样的话，当下一次读的时候，我们可以将保存的入口重新做个拷贝，然后让拷贝进行变化。具体程序如下：




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

static int mylog_r = 0;

static int mylog_r_for_read = 0;//这个用来拷贝mylog_r ，它将改变，但是mylog_r 不变

static int mylog_w = 0;



static DECLARE_WAIT_QUEUE_HEAD(mymsg_waitq);



static int is_mylog_empty(void)

{

 return (mylog_r == mylog_w);

}



static int is_mylog_empty_for_read(void)

{

 return (mylog_r_for_read == mylog_w);

}



static int is_mylog_full(void)

{

 return ((mylog_w + 1)% MYLOG_BUF_LEN == mylog_r);

}



//这个函数是被myprintk函数调用的

static void mylog_putc(char c)

{

 if (is_mylog_full())

 {

 mylog_r = (mylog_r + 1) % MYLOG_BUF_LEN;

               /*加上下面三行的原因是：如果读的时候，也一直在调用printk写的话，

                *当写的速度比较快的时候，可能会导致mylog_w超过mylog_r_for_read，

                *这时就需要更新mylog_r_for_read，使mylog_r_for_read 指向新的入口

                *当mylog_w超过入口mylog_r时，mylog_r会一直跟着更新的！        

               */

 if ((mylog_r_for_read + 1) % MYLOG_BUF_LEN == mylog_r)

 {

 mylog_r_for_read = mylog_r;

 }

 }



 mylog_buf[mylog_w] = c;

 mylog_w = (mylog_w + 1) % MYLOG_BUF_LEN;



    wake_up_interruptible(&mymsg_waitq); 

}



static int mylog_getc_for_read(char *p)

{

 if (is_mylog_empty_for_read())

 {

 return 0;

 }

 *p = mylog_buf[mylog_r_for_read];

mylog_r_for_read = (mylog_r_for_read + 1) % MYLOG_BUF_LEN;

 return 1;

}





int myprintk(const char *fmt, ...)

{

 va_list args;

 int i;

 int j;



 va_start(args, fmt);

 i = vsnprintf(tmp_buf, INT_MAX, fmt, args);

 va_end(args);



 for (j = 0; j < i; j++)

mylog_putc(tmp_buf[j]);



 return i;

}



static ssize_t mymsg_read(struct file *file, char __user *buf,size_t count, loff_t *ppos)

{

 int error = 0;

 int i = 0;

 char c;



 if ((file->f_flags & O_NONBLOCK) && is_mylog_empty_for_read())

 return -EAGAIN;



 error = wait_event_interruptible(mymsg_waitq, !is_mylog_empty_for_read());



 /* copy_to_user */

 while (!error && (mylog_getc_for_read(&c)) && i < count) {

 error = __put_user(c, buf);

 buf++;

 i++;

 }



 if (!error)

 error = i;



 return error;

}



static int mymsg_open(struct inode *inode, struct file *file)

{

mylog_r_for_read = mylog_r;

 return 0;

}



const struct file_operations proc_mymsg_operations = {

 .open = mymsg_open,

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



EXPORT_SYMBOL(myprintk);



MODULE_LICENSE("GPL");



总结，关于这个函数，当我们在用户空间，使用命令：cat /proc/mymsg时，首先会调用open函数，在open函数里面会将入口做一个拷贝，然后拿出一份来作为变化量，另外一个作为入口不改变。这样，每次cat /proc/mymsg时，都会从入口处开始打印！










