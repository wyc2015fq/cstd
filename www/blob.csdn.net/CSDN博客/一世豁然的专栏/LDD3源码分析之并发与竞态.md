# LDD3源码分析之并发与竞态 - 一世豁然的专栏 - CSDN博客





2014年12月16日 20:59:34[一世豁然](https://me.csdn.net/Explorer_day)阅读数：576








本博客转载于：[http://blog.csdn.net/liuhaoyutz/article/details/7383653](http://blog.csdn.net/liuhaoyutz/article/details/7383653)  [作者：刘昊昱]






本文分析LDD3第五章并发与竞态相关代码，本章代码涉及两个内容，一个是信号量，另一个是completion。



一、scull中信号量的使用

在scull_write函数中有如下代码片断：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383653#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383653#)

- 364    if (!dptr->data[s_pos]) {  
- 365        dptr->data[s_pos] = kmalloc(quantum, GFP_KERNEL);  
- 366        if (!dptr->data[s_pos])  
- 367            goto out;  
- 368    }  


假设有两个进程A和B同时在尝试向同一个scull设备的相同偏移量写入数据，而且在同一时刻达到364行的if判断。如果dptr->data[s_pos]的值为NULL，两个进程都决定分配内存，则A、B进程都会把kmalloc的返回值赋值给dptr->data[s_pos]。显然，后赋值的那个进程会覆盖先赋值的进程的所赋的值。并且造成先赋值进程所分配的内存无法再找回来。

上述情况就是一种竞态。为了避免竞态的发生，scull使用了信号量。

scull设备用scull_dev结构体表示，该结构体在scull.h中定义如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383653#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383653#)

- 87struct scull_dev {  
- 88    struct scull_qset *data;      /* Pointer to first quantum set */
- 89    int quantum;              /* the current quantum size */
- 90    int qset;                  /* the current array size */
- 91    unsigned long size;         /* amount of data stored here */
- 92    unsigned int access_key;    /* used by sculluid and scullpriv */
- 93    struct semaphore sem;     /* mutual exclusion semaphore     */
- 94    struct cdev cdev;          /* Char device structure      */
- 95};  


在scull_dev结构体中，93行定义的sem成员，就是信号量，因为每个scull_dev结构体代表一个scull设备，所以每个scull设备都有一个专用的信号量。

如果要对使用的scull设备使用一个全局的信号量也是可以的，但是，不同的scull设备并不共享资源，没有理由让一个进程在其他进程访问不同的scull设备时等待。为每个scull设备提供专用的信号量，允许不同设备上的操作可以并行处理，从而提高性能。

信号量在使用之前必须先初始化，scull在模块初始化函数scull_init_module中执行下面的循环完成对所有scull设备专用信号量的初始化：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383653#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383653#)

- 648    /* Initialize each device. */
- 649    for (i = 0; i < scull_nr_devs; i++) {  
- 650        scull_devices[i].quantum = scull_quantum;  
- 651        scull_devices[i].qset = scull_qset;  
- 652        init_MUTEX(&scull_devices[i].sem);  
- 653        scull_setup_cdev(&scull_devices[i], i);  
- 654    }  


这个for循环每循环一次，完成对一个scull设备的初始化，其中652行，调用init_MUTEX每个设备专用的信号量(互斥体)进行初始化。要注意，信号量必须在设备被注册到系统中之前完成初始化，否则会出现竞态。scull设备的注册是在653行的scull_setup_cdev函数中完成的，所以在这个函数调用之前，我们完成了对信号量的初始化。

在使用信号量之前，首先要明确什么是需要用信号量保护的资源，然后，我们才能用信号量保证对这些资源的互斥访问。对于scull设备来说，所有的信息都保存在scull_dev结构体中，因此，scull_dev就是我们要保护的资源。

在main.c文件在有很多地方使用了信号量来保证对scull_dev的互斥访问。或者说，凡是要改变scull_dev结构休内容的地方，都必须加锁，防止竞态。

例如，在scull_write函数中，有如下语句：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383653#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383653#)

- 346    if (down_interruptible(&dev->sem))  
- 347        return -ERESTARTSYS;  


346行，调用down_interruptible(&dev->sem)进行加锁，注意，要对down_interruptible的返回值进行检查，如果返回0，说明说明加锁成功了，可以开始操作受保护的资源scull_dev，反之，如果down_interruptible返回非0值，说明是在等待过程中被中断了，这时要退出并返回-ERESTARTSYS，交给系统处理。

给信号量加锁后，不管scull_write能否完成其工作，都必须释放信号量，代码如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383653#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383653#)

- 384  out:  
- 385    up(&dev->sem);  
- 386    return retval;  


385行，释放信号量。

至此，信号量相关的代码就分析完了。



二、completion的使用

驱动开发中，有时我们需要在当前线程(A)之外创建另外一个线程(B)执行某个活动，然后线程(A)等待该活动结束，待活动结束后，线程(A)再继续向下执行。例如，这个活动可以是某种硬件操作。这种情况下，要实现新老线程的同步，可以使用completion接口。

注意，上面所说的情况，使用信号量也能实现同步，但信号量并不适合。因为在通常的使用中，如果试图锁定某个信号量，一般来说，都能加锁成功。如果存在对信号量的严重竞争，性能将受很大影响。这时，我们就需要检查一下我们的加锁操作设计是不是有问题了。信号量对“可用”情况已经做了大量优化。对于上面所说的情况，如果用信号量实现同步，则加锁的线程几乎总是要等待，造成系统性能下降。

completion是一种轻量级的机制，它允许一个线程告诉另外一个线程某个工作已经完成。

等待completion使用如下函数：

void wait_for_completion(struct completion *c);

相应的，completion事件可以通过如下函数触发：

void complete(struct completion *c);

void complete_all(struct completion *c);

如果有多个线程在等待同一个completion事件，complete函数只唤醒一个等待线程，而complete_all函数将唤醒所有等待线程。

LDD3提供了一个complete模块演示completion机制的用法.这个模块代码不多,下面列出其源码:



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383653#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383653#)

-  1/*
-  2 * complete.c -- the writers awake the readers
-  3 *
-  4 * Copyright (C) 2003 Alessandro Rubini and Jonathan Corbet
-  5 * Copyright (C) 2003 O'Reilly & Associates
-  6 *
-  7 * The source code in this file can be freely used, adapted,
-  8 * and redistributed in source or binary form, so long as an
-  9 * acknowledgment appears in derived source files.  The citation
- 10 * should list that the code comes from the book "Linux Device
- 11 * Drivers" by Alessandro Rubini and Jonathan Corbet, published
- 12 * by O'Reilly & Associates.   No warranty is attached;
- 13 * we cannot take responsibility for errors or fitness for use.
- 14 *
- 15 * $Id: complete.c,v 1.2 2004/09/26 07:02:43 gregkh Exp $
- 16 */
- 17  
- 18#include <linux/module.h>  
- 19#include <linux/init.h>  
- 20  
- 21#include <linux/sched.h>  /* current and everything */
- 22#include <linux/kernel.h> /* printk() */
- 23#include <linux/fs.h>     /* everything... */
- 24#include <linux/types.h>  /* size_t */
- 25#include <linux/completion.h>  
- 26  
- 27MODULE_LICENSE("Dual BSD/GPL");  
- 28  
- 29static int complete_major = 0;  
- 30  
- 31DECLARE_COMPLETION(comp);  
- 32  
- 33ssize_t complete_read (struct file *filp, char __user *buf, size_t count, loff_t *pos)  
- 34{  
- 35    printk(KERN_DEBUG "process %i (%s) going to sleep\n",  
- 36            current->pid, current->comm);  
- 37    wait_for_completion(&comp);  
- 38    printk(KERN_DEBUG "awoken %i (%s)\n", current->pid, current->comm);  
- 39    return 0; /* EOF */
- 40}  
- 41  
- 42ssize_t complete_write (struct file *filp, constchar __user *buf, size_t count,  
- 43        loff_t *pos)  
- 44{  
- 45    printk(KERN_DEBUG "process %i (%s) awakening the readers...\n",  
- 46            current->pid, current->comm);  
- 47    complete(&comp);  
- 48    return count; /* succeed, to avoid retrial */
- 49}  
- 50  
- 51  
- 52struct file_operations complete_fops = {  
- 53    .owner = THIS_MODULE,  
- 54    .read =  complete_read,  
- 55    .write = complete_write,  
- 56};  
- 57  
- 58  
- 59int complete_init(void)  
- 60{  
- 61    int result;  
- 62  
- 63    /*
- 64     * Register your major, and accept a dynamic number
- 65     */
- 66    result = register_chrdev(complete_major, "complete", &complete_fops);  
- 67    if (result < 0)  
- 68        return result;  
- 69    if (complete_major == 0)  
- 70        complete_major = result; /* dynamic */
- 71    return 0;  
- 72}  
- 73  
- 74void complete_cleanup(void)  
- 75{  
- 76    unregister_chrdev(complete_major, "complete");  
- 77}  
- 78  
- 79module_init(complete_init);  
- 80module_exit(complete_cleanup);  


79行，指定模块初始化函数为complete_init。

80行，指定模块清理函数是complete_cleanup。

我们先看模块初始化函数complete_init的实现：

66行，使用老的字符设备注册函数register_chrdev注册字符设备，因为在29行设置complete_major为0，所以是由系统动态分配主设备号；模块名称为”complete”；模块对应的文件操作函数集是complete_fops。

52 - 56行，定义了complete_fops，指定读写操作分别是complete_read和complete_write。

下面看complete_read的实现：

在打印即将进入睡眠的信息后，complete_read在37行调用wait_for_completion(&comp)，进入睡眠，即等待completion
 “comp”。”comp”是在31行用DECLARE_COMPLETION(comp)创建的。如果等待的completion发生了，complete_read函数将再次打印已被唤醒相关信息。

也就是说，任何进程读取模块设备文件，都会进入睡眠等待。

再来看complete_write的实现：

首先打印提示信息，然后在47行调用complete(&comp)触发completion事件，相应会唤醒一个在等待”comp”的进程。

可以有多个进程进行读操作，这些读进程都会进入睡眠等待，当有执行写操作的进程时，只有一个等待进程会被唤醒，但是哪个进程，不能确定。

为测试complete模块，我改写了LDD3提供的scull_load和scull_unload脚本，命名为complete_load和complete_unload。

complete_load脚本的内容如下所示：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383653#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383653#)

- #!/bin/sh
- # $Id: complete_load,v 1.4 2004/11/03 06:19:49 rubini Exp $
- module="complete"
- device="complete"
- mode="666"
- 
- # Group: since distributions do it differently, look for wheel or use staff
- if grep -q '^staff:' /etc/group; then  
-     group="staff"
- else
-     group="wheel"
- fi  
- 
- # invoke insmod with all arguments we got
- # and use a pathname, as insmod doesn't look in . by default
- /sbin/insmod ./$module.ko $* || exit 1  
- 
- # retrieve major number
- major=$(awk "\$2==\"$module\" {print \$1}" /proc/devices)  
- 
- # Remove stale nodes and replace them, then give gid and perms
- # Usually the script is shorter, it's scull that has several devices in it.
- 
- rm -f /dev/${device}  
- mknod /dev/${device} c $major 0  
- 
- chgrp $group /dev/${device}   
- chmod $mode  /dev/${device}  


complete_unload脚本的内容如下所示：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383653#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383653#)

- #!/bin/sh
- module="complete"
- device="complete"
- 
- # invoke rmmod with all arguments we got
- /sbin/rmmod $module $* || exit 1  
- 
- # Remove stale nodes
- 
- rm -f /dev/${device}    


complete模块的测试过程如下：

![](http://hi.csdn.net/attachment/201203/22/0_13324045024tke.gif)




