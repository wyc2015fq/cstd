# Linux下hello.ko内核模块制作的全过程 - xqhrs232的专栏 - CSDN博客
2016年07月12日 23:04:55[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1081
原文地址::[http://blog.csdn.net/jiankangshiye/article/details/6665179](http://blog.csdn.net/jiankangshiye/article/details/6665179)
相关文章
1、[Linux内核模块自动加载机制](http://www.cnblogs.com/image-eye/archive/2011/08/19/2145858.html)----[http://www.cnblogs.com/image-eye/archive/2011/08/19/2145858.html](http://www.cnblogs.com/image-eye/archive/2011/08/19/2145858.html)
2、Linux驱动的两种加载方式过程分析----[http://www.linuxidc.com/Linux/2014-06/103569.htm](http://www.linuxidc.com/Linux/2014-06/103569.htm)
3、linux insmod error inserting ' ':-1file exists问题----[http://bbs.csdn.net/topics/350266534](http://bbs.csdn.net/topics/350266534)
Linux下hello.ko内核模块制作的全过程
1. linux系统用的是Redflag 6.0 SP1 下载地址：ftp://ftp.redflag-linux.com/pub/redflag/dt6sp1/SP1/redflag-6-sp1.iso， 系统安装很容易，安提示做就好。
所用的内核源码目录树下载地址：ftp://ftp.redflag-linux.com/pub/redflag/dt6sp1/SP1/redflag-6-tool-sp1-src1.iso，将此iso文件挂载到/mnt下，安装其中的内核rpm包。
挂载方法：mount -t iso9660 redflag-6-tool-sp1-src1.iso /mnt/ -o loop
内核目录树安装方法：cd /mnt/RedFlag/SRMPS/
** rpm -i kernel-2.6.23.1-4.src.rpm**
3. 编写hello模块代码，源码如下：
hello.c
**[cpp]**[view
 plain](http://blog.csdn.net/jiankangshiye/article/details/6665179#)[copy](http://blog.csdn.net/jiankangshiye/article/details/6665179#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/310303/fork)
- #include <linux/init.h>
- #include <linux/module.h>
- 
- MODULE_LICENSE("GPL");  
- staticint hello_init(void)  
- {  
-   printk(KERN_ALERT "Hello, world\n");  
- return 0;  
- }  
- staticvoid hello_exit(void)  
- {  
-   printk(KERN_ALERT "Goodbye, cruel world\n");  
- }  
- 
- module_init(hello_init);  
- module_exit(hello_exit);  
4. 编写hello模块的Makefile文件，Makefile内容如下：
Makefile
**[plain]**[view
 plain](http://blog.csdn.net/jiankangshiye/article/details/6665179#)[copy](http://blog.csdn.net/jiankangshiye/article/details/6665179#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/310303/fork)
- #Makefile 2.6  
- 
- obj-m :=hello.o  
- KERNEL :=/usr/src/kernels/2.6.23.1-4-i686/  
- PWD :=$(shell pwd)  
- modules :  
-     $(MAKE) -C $(KERNEL) M=$(PWD) modules  
- .PHONEY:clean  
- clean :  
-     rm -f *.o *.ko  
5. 编译模块
在命令行进入hello.c所在的文件夹下执行**make**命令即可完成hello模块的编译。用ls命令可以查看到hello.ko文件，此文件就是我们自定义的内核模块。
6. 安装hello模块
命令行下执行命令：**insmod hello.ko **。通过命令:cat /var/log/messages 
可以看到下面这样的信息：“Aug  6 13:37:59 localhost kernel: Hello, world”，说明模块加载成功了。
7. 另外一种模块Makefile的编写方法
Makefile
**[plain]**[view
 plain](http://blog.csdn.net/jiankangshiye/article/details/6665179#)[copy](http://blog.csdn.net/jiankangshiye/article/details/6665179#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/310303/fork)
- # If KERNELRELEASE is defined, we've been invoked from the  
- # kernel build system and can use its language.  
- ifneq ($(KERNELRELEASE),)  
- 
-  obj-m := hello.o   
- # Otherwise we were called directly from the command  
- # line; invoke the kernel build system.  
- else  
- 
-  KERNELDIR ?= /lib/modules/$(shell uname -r)/build  
-  PWD := $(shell pwd)   
- default:  
-     $(MAKE) -C $(KERNELDIR) M=$(PWD) modules  
- 
- endif  
7. 卸载hello模块
命令行下执行命令：**rmmod hello.ko**即可。通过命令：cat /var/log/messages.
可以看到下面这样的信息：“Aug  6 13:40:36 localhost kernel: Goodbye, cruel world”，说明模块卸载成功。
8. 查看模块信息
命令行下执行命令：**modinfo hello**
二 Debian 6下制作hello内核模块的过程
1. 软件
Debian 6.02 
linux-2.6.32.10.tar.bz2
2. 解压内核源码到一个目录下， 我解压到/home/kernel/下来
3. 查询安装内核头文件
**[plain]**[view
 plain](http://blog.csdn.net/jiankangshiye/article/details/6665179#)[copy](http://blog.csdn.net/jiankangshiye/article/details/6665179#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/310303/fork)
- aptitude search linux-headers-2.6.32*  
- aptitude install linux-headers-2.6.32-5-686  
注意：2.6.32-5-686来自命令**uname -r**的结果
上面这个命令比较麻烦，还可以选择下面的命令实现同样的目的
**[plain]**[view
 plain](http://blog.csdn.net/jiankangshiye/article/details/6665179#)[copy](http://blog.csdn.net/jiankangshiye/article/details/6665179#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/310303/fork)
- apt-get install linux-headers-`uname -r`  
注意这个命令里使用的**不是单引号**，而是反单引号，位于键盘的左上角， 一般和数字1是邻居。
4. 写个Hello模块测试
5. FAQ
<Q1> 内核代码下载后， 要简单的运行俩命令配置一下，我这里的命令如下[当然， 您也可以不尝试这一步， 当你make时， 系统会提示你该怎么做]
<A1>
**[plain]**[view
 plain](http://blog.csdn.net/jiankangshiye/article/details/6665179#)[copy](http://blog.csdn.net/jiankangshiye/article/details/6665179#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/310303/fork)
- cd /home/kernel/linux-2.6.32.10  
- make oldconfig && make prepare  
<Q2>WARNING: Symbol version dump /home/kernel/linux-2.6.32.10/Module.symvers
is missing; modules will have no dependencies and modversions.
<A2>
**[plain]**[view
 plain](http://blog.csdn.net/jiankangshiye/article/details/6665179#)[copy](http://blog.csdn.net/jiankangshiye/article/details/6665179#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/310303/fork)
- cd /home/kernel/linux-2.6.32.10  
- make modules  
[参考内容]
The Module.symvers is (re)generated when you (re)compile modules. Run make modules, and you should get a Module.symvers file at the root of the kernel tree.
Note that if you only ran make and not make modules, you haven't built any modules yet. The symbols from the kernel itself (vmlinux or one of the architecture-dependent image formats) are in System.map.
经测试问题得到很好的解决， **make modules**要花费好长编译时间段。
<Q3> type defaults to "int' in declaration of module_init
<A3> module_init(hello_init);这句中某个字符弄成汉字编码导致的
<Q4> XP与虚拟机里的debian通信我用俩办法一个samba传输数据,一个是ssh传输命令
<A4> debian中安装sshd的命令:  apt-get install openssh-server
启动sshd服务的命令: /etc/init.d/ssh start
参考：[http://bbs.chinaunix.net/thread-3570849-1-1.html](http://bbs.chinaunix.net/thread-3570849-1-1.html)
6. hello驱动涉及到linux驱动模型的方方面面
hello.h代码文件
**[cpp]**[view
 plain](http://blog.csdn.net/jiankangshiye/article/details/6665179#)[copy](http://blog.csdn.net/jiankangshiye/article/details/6665179#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/310303/fork)
- #ifndef _HELLO_ANDROID_H
- #define _HELLO_ANDROID_H
- 
- #include <linux/cdev.h>
- #include <linux/semaphore.h>
- 
- #define HELLO_DEVICE_NODE_NAME  "hello"
- #define HELLO_DEVICE_FILE_NAME  "hello"
- #define HELLO_DEVICE_PROC_NAME  "hello"
- #define HELLO_DEVICE_CLASS_NAME "hello"
- 
- struct hello_android_dev {  
- int val;  
- struct semaphore sem;  
- struct cdev dev;  
- };  
- 
- #define init_MUTEX(sem) sema_init(sem, 1)
- 
- #endif
hello.c代码文件
**[cpp]**[view
 plain](http://blog.csdn.net/jiankangshiye/article/details/6665179#)[copy](http://blog.csdn.net/jiankangshiye/article/details/6665179#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/310303/fork)
- #include <linux/init.h>
- #include <linux/module.h>
- #include <linux/types.h>
- #include <linux/fs.h>
- #include <linux/proc_fs.h>
- #include <linux/device.h>
- 
- #include <asm/uaccess.h>
- 
- #include "hello.h"
- 
- staticint hello_major = 0;  
- staticint hello_minor = 0;  
- 
- staticstructclass *hello_class = NULL;  
- staticstruct hello_android_dev *hello_dev = NULL;  
- 
- staticint hello_open(struct inode *inode, struct file *filp);  
- staticint hello_release(struct inode *inode, struct file *filp);  
- static ssize_t hello_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);  
- static ssize_t hello_write(struct file *filp, constchar __user *buf, size_t count, loff_t *f_pos);  
- 
- staticstruct file_operations hello_fops = {  
-     .owner = THIS_MODULE,  
-     .open  = hello_open,  
-     .release = hello_release,  
-     .read  = hello_read,  
-     .write = hello_write,  
- };  
- 
- static ssize_t hello_val_show(struct device *dev, struct device_attribute *attr, char *buf);  
- static ssize_t hello_val_store(struct device *dev, struct device_attribute *attr, constchar *buf, size_t count);  
- 
- static DEVICE_ATTR(val, S_IRUGO | S_IWUSR, hello_val_show, hello_val_store);  
- 
- staticint hello_open(struct inode *inode, struct file *filp)  
- {  
- struct hello_android_dev *dev;  
- 
-     printk(KERN_ALERT"hello_open 1\n");  
-     dev = container_of(inode->i_cdev, struct hello_android_dev, dev);  
-     printk(KERN_ALERT"hello_open 2\n");  
-     dev = container_of(inode->i_cdev, struct hello_android_dev, dev);  
-     printk(KERN_ALERT"hello_open 3\n");  
-     filp->private_data = dev;  
-     printk(KERN_ALERT"hello_open 4\n");  
- 
- return 0;  
- }  
- 
- staticint hello_release(struct inode *inode, struct file *filp)  
- {  
- return 0;  
- }  
- 
- static ssize_t hello_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)  
- {  
-     ssize_t err = 0;  
- struct hello_android_dev *dev = filp->private_data;  
- 
- if (down_interruptible(&(dev->sem))) {  
- return -ERESTARTSYS;  
-     }  
- 
- if (count < sizeof(dev->val)) {  
- goto out;  
-     }  
- 
-     printk(KERN_ALERT"hello_read\n");  
- if (copy_to_user(buf, &(dev->val), sizeof(dev->val))) {  
-         err = -EFAULT;  
- goto out;  
-     }  
- 
-     err = sizeof(dev->val);  
- 
- out:  
-     up(&(dev->sem));  
- return err;  
- }  
- 
- static ssize_t hello_write(struct file *filp, constchar __user *buf, size_t count, loff_t *f_pos)  
- {  
- struct hello_android_dev *dev = filp->private_data;  
-     ssize_t err = 0;  
- 
- if (down_interruptible(&(dev->sem))) {  
- return -ERESTARTSYS;  
-     }  
- 
- if (count != sizeof(dev->val)) {  
- goto out;  
-     }  
- 
- if (copy_from_user(&(dev->val), buf, count)) {  
-         err = -EFAULT;  
- goto out;  
-     }  
- 
-     err = sizeof(dev->val);  
- 
- out:  
-     up(&(dev->sem));  
- return err;  
- }  
- 
- /*
-  * dev fs operations 
-  */
- static ssize_t __hello_get_val(struct hello_android_dev *dev, char *buf)  
- {  
- int val = 0;  
- 
- if (down_interruptible(&(dev->sem))) {  
- return -ERESTARTSYS;  
-     }  
- 
-     val = dev->val;  
-     up(&(dev->sem));  
- 
- return snprintf(buf, PAGE_SIZE, "%d\n", val);  
- }  
- 
- static ssize_t __hello_set_val(struct hello_android_dev *dev, constchar *buf, size_t count)  
- {  
- int val = 0;  
- 
-     val = simple_strtol(buf, NULL, 10);  
- 
- if (down_interruptible(&(dev->sem))) {  
- return -ERESTARTSYS;  
-     }  
- 
-     dev->val = val;  
-     up(&(dev->sem));  
- 
- return count;  
- }  
- 
- static ssize_t hello_val_show(struct device *dev, struct device_attribute *attr, char *buf)  
- {  
- struct hello_android_dev *hdev = (struct hello_android_dev *)dev_get_drvdata(dev);  
- 
- return __hello_get_val(hdev, buf);  
- }  
- 
- static ssize_t hello_val_store(struct device *dev, struct device_attribute *attr, constchar *buf, size_t count)   
- {  
- struct hello_android_dev *hdev = (struct hello_android_dev *)dev_get_drvdata(dev);  
- 
- return __hello_set_val(hdev, buf, count);  
- }  
- 
- /*
-  * proc fs operations
-  */
- static ssize_t hello_proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)  
- {  
- if (off > 0) {  
-         *eof = 1;  
- return 0;  
-     }  
- 
- return __hello_get_val(hello_dev, page);  
- }  
- 
- static ssize_t hello_proc_write(struct file *filp, constchar __user *buf, unsigned long len, void *data)  
- {  
- int err = 0;  
- char *page = NULL;  
- 
- if (len > PAGE_SIZE) {  
-         printk(KERN_ALERT"The buff is too large: %lu.\n", len);  
- return -EFAULT;  
-     }  
- 
-     page = (char *)__get_free_page(GFP_KERNEL);  
- if (!page) {  
-         printk(KERN_ALERT"Failed to alloc page.\n");  
- return -ENOMEM;  
-     }  
- 
- if (copy_from_user(page, buf, len)) {  
-         printk(KERN_ALERT"Failed to copy buff from user.\n");  
-         err = -EFAULT;  
- goto out;  
-     }  
- 
-     err = __hello_set_val(hello_dev, page, len);  
- 
- out:  
-     free_page((unsigned long)page);  
- return err;  
- }  
- 
- /*
-  * /proc/hello 
-  */
- staticvoid hello_create_proc(void)  
- {  
- struct proc_dir_entry *entry;  
- 
-     entry = create_proc_entry(HELLO_DEVICE_PROC_NAME, 0, NULL);  
- if (entry) {  
- /* entry->owner = THIS_MODULE;*/
-         entry->read_proc  = hello_proc_read;  
-         entry->write_proc = hello_proc_write;  
-     }  
- }  
- 
- staticvoid hello_remove_proc(void)  
- {  
-     remove_proc_entry(HELLO_DEVICE_PROC_NAME, NULL);  
- }  
- 
- staticint __hello_setup_dev(struct hello_android_dev *dev)  
- {  
- int err;  
-     dev_t devno = MKDEV(hello_major, hello_major);  
- 
-     memset(dev, 0, sizeof(struct hello_android_dev));  
-     cdev_init(&(dev->dev), &hello_fops);  
-     dev->dev.owner = THIS_MODULE;  
-     dev->dev.ops   = &hello_fops;  
- 
-     err = cdev_add(&(dev->dev), devno, 1);  
- if (err) {  
- return err;  
-     }  
- 
-     init_MUTEX(&(dev->sem));  
-     dev->val = 0;  
- 
- return 0;  
- }  
- 
- staticint __init hello_init(void)  
- {  
- int err = -1;  
-     dev_t dev = 0;  
- struct device *temp = NULL;  
- 
-     printk(KERN_ALERT"Initializing hello device.\n");  
- 
-     err = alloc_chrdev_region(&dev, 0, 1, HELLO_DEVICE_NODE_NAME);  
- if (err < 0) {  
-         printk(KERN_ALERT"Failed to alloc char dev region.\n");  
- goto fail;  
-     }  
- 
-     hello_major = MAJOR(dev);  
-     hello_minor = MINOR(dev);  
- 
-     hello_dev = kmalloc(sizeof(struct hello_android_dev), GFP_KERNEL);  
- if (!hello_dev) {  
-         err = -ENOMEM;  
-         printk(KERN_ALERT"Failed to alloc hello_dev.\n");  
- goto unregister;  
-     }  
- 
-     err = __hello_setup_dev(hello_dev);  
- if (err) {  
-         printk(KERN_ALERT"Failed to setup dev: %d.\n", err);  
- goto cleanup;  
-     }  
- 
-     hello_class = class_create(THIS_MODULE, HELLO_DEVICE_CLASS_NAME);  
- if (IS_ERR(hello_class)) {  
-         err = PTR_ERR(hello_class);  
-         printk(KERN_ALERT"Failed to create hello class.\n");  
- goto destroy_cdev;  
-     }  
- 
- /*
-      * create /dev/hello
-      * create /sys/class/hello/hello
-      */
-     temp = device_create(hello_class, NULL, dev, "%s", HELLO_DEVICE_FILE_NAME);  
- if (IS_ERR(hello_class)) {  
-         err = PTR_ERR(hello_class);  
-         printk(KERN_ALERT"Failed to create hello device.\n");  
- goto destroy_class;  
-     }  
- 
- /* 
-      * create /sys/class/hello/hello/val 
-      */
-     err = device_create_file(temp, &dev_attr_val);  
- if (err < 0) {  
-         printk(KERN_ALERT"Failed to create attribute val.\n");  
- goto destroy_device;  
-     }  
- 
-     dev_set_drvdata(temp, hello_dev);  
- 
- /*
-      * create /proc/hello
-      */
-     hello_create_proc();  
- 
-     printk(KERN_ALERT"Succedded to initialize hello device.\n");  
- return 0;  
- 
- destroy_device:  
-     device_destroy(hello_class, dev);  
- 
- destroy_class:  
-     class_destroy(hello_class);  
- 
- destroy_cdev:  
-     cdev_del(&(hello_dev->dev));  
- 
- cleanup:  
-     kfree(hello_dev);  
- 
- unregister:  
-     unregister_chrdev_region(MKDEV(hello_major, hello_minor), 1);  
- 
- fail:  
- return err;  
- }  
- 
- staticvoid __exit hello_exit(void)  
- {  
-     dev_t devno = MKDEV(hello_major, hello_minor);  
- 
-     printk(KERN_ALERT"Remove hello device.\n");  
- 
- /*
-      * remove /proc/hello
-      */
-     hello_remove_proc();  
- 
- /*
-      * destroy device and class
-      */
- if (hello_class) {  
-         device_destroy(hello_class, MKDEV(hello_major, hello_minor));  
-         class_destroy(hello_class);  
-     }  
- 
- /*
-      * delete cdev and free malloced mem
-      */
- if (hello_dev) {  
-         cdev_del(&(hello_dev->dev));  
-         kfree(hello_dev);  
-     }  
- 
- /*
-      * free device ID
-      */
-     unregister_chrdev_region(devno, 1);  
- }  
- 
- MODULE_LICENSE("GPL");  
- MODULE_DESCRIPTION("First Android Driver /dev/hello");  
- 
- module_init(hello_init);  
- module_exit(hello_exit);  
![](http://static.blog.csdn.net/images/save_snippets.png)
讨论群：
Atmel技术交流讨论群
群号是305940105
Linux驱动开发调试群
群号是297141784
OS若水园
群号是113148289
[](http://blog.csdn.net/jiankangshiye/article/details/6665179#)[](http://blog.csdn.net/jiankangshiye/article/details/6665179#)[](http://blog.csdn.net/jiankangshiye/article/details/6665179#)[](http://blog.csdn.net/jiankangshiye/article/details/6665179#)[](http://blog.csdn.net/jiankangshiye/article/details/6665179#)[](http://blog.csdn.net/jiankangshiye/article/details/6665179#)
- 
顶
9- 
踩

