# 几个简单的Linux驱动程序 - Koma Hub - CSDN博客
2019年02月27日 21:32:11[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：15
个人分类：[Linux driver](https://blog.csdn.net/Rong_Toa/article/category/8645170)
**Table of Contents**
[hello.c](#hello.c)
[hellop.c带参数输入](#hellop.c%E5%B8%A6%E5%8F%82%E6%95%B0%E8%BE%93%E5%85%A5)
[complete.c](#complete.c)
# hello.c
```cpp
/*                                                     
 * $Id: hello.c,v 1.5 2004/10/26 03:32:21 corbet Exp $ 
 */                                                    
#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");
static int hello_init(void)
{
	printk(KERN_ALERT "Hello, world\n");
	return 0;
}
static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\n");
}
module_init(hello_init);
module_exit(hello_exit);
```
# hellop.c带参数输入
```cpp
/*                                                     
 * $Id: hellop.c,v 1.4 2004/09/26 07:02:43 gregkh Exp $ 
 */                                                    
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
MODULE_LICENSE("Dual BSD/GPL");
/*                                                        
 * These lines, although not shown in the book,           
 * are needed to make hello.c run properly even when      
 * your kernel has version support enabled                
 */                                                       
                                                          
/*
 * A couple of parameters that can be passed in: how many times we say
 * hello, and to whom.
 */
static char *whom = "world";
static int howmany = 1;
module_param(howmany, int, S_IRUGO);
module_param(whom, charp, S_IRUGO);
static int hello_init(void)
{
	int i;
	for (i = 0; i < howmany; i++)
		printk(KERN_ALERT "(%d) Hello, %s\n", i, whom);
	return 0;
}
static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\n");
}
module_init(hello_init);
module_exit(hello_exit);
```
# complete.c
```cpp
/*
 * complete.c -- the writers awake the readers
 *
 * Copyright (C) 2003 Alessandro Rubini and Jonathan Corbet
 * Copyright (C) 2003 O'Reilly & Associates
 *
 * The source code in this file can be freely used, adapted,
 * and redistributed in source or binary form, so long as an
 * acknowledgment appears in derived source files.  The citation
 * should list that the code comes from the book "Linux Device
 * Drivers" by Alessandro Rubini and Jonathan Corbet, published
 * by O'Reilly & Associates.   No warranty is attached;
 * we cannot take responsibility for errors or fitness for use.
 *
 * $Id: complete.c,v 1.2 2004/09/26 07:02:43 gregkh Exp $
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>  /* current and everything */
#include <linux/kernel.h> /* printk() */
#include <linux/fs.h>     /* everything... */
#include <linux/types.h>  /* size_t */
#include <linux/completion.h>
MODULE_LICENSE("Dual BSD/GPL");
static int complete_major = 0;
DECLARE_COMPLETION(comp);
ssize_t complete_read (struct file *filp, char __user *buf, size_t count, loff_t *pos)
{
	printk(KERN_DEBUG "process %i (%s) going to sleep\n",
			current->pid, current->comm);
	wait_for_completion(&comp);
	printk(KERN_DEBUG "awoken %i (%s)\n", current->pid, current->comm);
	return 0; /* EOF */
}
ssize_t complete_write (struct file *filp, const char __user *buf, size_t count,
		loff_t *pos)
{
	printk(KERN_DEBUG "process %i (%s) awakening the readers...\n",
			current->pid, current->comm);
	complete(&comp);
	return count; /* succeed, to avoid retrial */
}
struct file_operations complete_fops = {
	.owner = THIS_MODULE,
	.read =  complete_read,
	.write = complete_write,
};
int complete_init(void)
{
	int result;
	/*
	 * Register your major, and accept a dynamic number
	 */
	result = register_chrdev(complete_major, "complete", &complete_fops);
	if (result < 0)
		return result;
	if (complete_major == 0)
		complete_major = result; /* dynamic */
	return 0;
}
void complete_cleanup(void)
{
	unregister_chrdev(complete_major, "complete");
}
module_init(complete_init);
module_exit(complete_cleanup);
```
