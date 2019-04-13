
# 具有IOctl的简单字符驱动 - 嵌入式Linux - CSDN博客

2013年09月24日 11:03:51[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1332


[http://www.cnblogs.com/geneil/archive/2011/12/04/2275372.html](http://www.cnblogs.com/geneil/archive/2011/12/04/2275372.html)
**驱动层**
\#include <linux/init.h>
\#include <linux/module.h>
\#include <linux/cdev.h>
\#include <linux/fs.h>
\#include <linux/device.h>
\#include <linux/module.h>
\#include <linux/types.h>
\#include <linux/errno.h>
\#include <linux/mm.h>
\#include <linux/sched.h>
\#include <asm/io.h>
\#include <asm/system.h>
\#include <asm/uaccess.h>

/* 定义幻数 */
\#define MEMDEV_IOC_MAGIC  'k'
/* 定义命令 */
\#define MEMDEV_IOCPRINT   _IO(MEMDEV_IOC_MAGIC, 1)
\#define MEMDEV_IOCGETDATA _IOR(MEMDEV_IOC_MAGIC, 2, int)  /*获取数据*/
\#define MEMDEV_IOCSETDATA _IOW(MEMDEV_IOC_MAGIC, 3, int)  /*设置数据*/
\#define MEMDEV_IOC_MAXNR 3   /*一共定义了三条命令*/

MODULE_LICENSE("WQFDual BSD/GPL");
MODULE_AUTHOR("WQFDriver Monkey");
struct mmap_dev_t
{
	dev_t dev_num;
	char *dev_name;
	
	struct class* cls;
	char * cls_name;
	struct cdev cdev;
};
 
struct mmap_dev_t mmap_dev = 
{
	.dev_name = "test_dev",
	.cls_name = "test_class",
};
int my_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO"WQFmy_open()++\n");
	printk(KERN_INFO"WQFmy_open()--\n");
	printk("wQF===============================\n");
	return 0;
}
ssize_t my_read(struct file *filp, char *buff, size_t count, loff_t *f_pos)
{
	printk(KERN_INFO"my_read()++\n");
	printk(KERN_INFO"my_read()--\n");
	return 0;
}
ssize_t my_write(struct file *filp, const char *buff, size_t count, loff_t *f_pos)
{
		
	printk(KERN_INFO"buff is :\n %s", buff);
	printk(KERN_INFO"my_write()--\n");
	return count;
}
int my_ioctl(struct inode *inode, struct file *filp,  \
                 unsigned int cmd, unsigned long arg)   /*IOCTL的驱动函数*/
{
	    int err = 0;
    int ret = 0;
    int ioarg = 0;
    
    /* 检测命令的有效性 */
    if (_IOC_TYPE(cmd) != MEMDEV_IOC_MAGIC)   /*检测是不是我们声明的幻数*/
        return -EINVAL;
    if (_IOC_NR(cmd) > MEMDEV_IOC_MAXNR)    /*检测是不是超出了我们的命令个数*/ 
        return -EINVAL;
    /* 根据命令类型，检测参数空间是否可以访问 */
    if (_IOC_DIR(cmd) & _IOC_READ)
        err = !access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd));
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
        err = !access_ok(VERIFY_READ, (void *)arg, _IOC_SIZE(cmd));
    if (err) 
        return -EFAULT;
    /* 根据命令，执行相应的操作 */
    switch(cmd) {
      /* 打印当前设备信息 */
      case MEMDEV_IOCPRINT:
          printk("<--- CMD MEMDEV_IOCPRINT Done--->\n\n");
        break;
      
      /* 获取参数 */
      case MEMDEV_IOCGETDATA: 
        ioarg = 1101;
        ret = __put_user(ioarg, (int *)arg);
        break;
      
      /* 设置参数 */
      case MEMDEV_IOCSETDATA: 
        ret = __get_user(ioarg, (int *)arg);
        printk("<--- In Kernel MEMDEV_IOCSETDATA ioarg = %d --->\n\n",ioarg);
        break;
      default:  
        return -EINVAL;
    }
    return ret;	
}

struct file_operations mmap_opt = 
{
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.write = my_write,
	.unlocked_ioctl  = my_ioctl,
	/* linux2.6.29和linux2.6.38的内核在file_operations结构发生了变化，
	否则在linux2.6.38内核中，继续使用.ioctl成员，编译时就会报错：
	error: unknown field 'ioctl' specified in initializer，struct file_operations结构体定义在include/linux/fs.h文件中。*/
};
static int wqfmmap_init(void)
{
	int err = 0;
	printk("mmap_init()++\n");
	
	//dynamic alloc device number
	if(0 != (err = alloc_chrdev_region
		    (&mmap_dev.dev_num, 0,3, mmap_dev.dev_name)))  /*自动分配主次设备号*/
		goto alloc_chrdev_region_err;
	printk("Wmajor = %d, Wminor = %d\n",
		   MAJOR(mmap_dev.dev_num),MINOR(mmap_dev.dev_num));/*从设备编号里抽取出主次设备号*/
	//create class
	mmap_dev.cls = class_create(THIS_MODULE,mmap_dev.cls_name);
	if(IS_ERR(mmap_dev.cls))
		goto class_create_err;
	//create character device
	device_create(mmap_dev.cls,NULL,mmap_dev.dev_num,&mmap_dev,"wfdevice");/*创建字符设备 在dev/下面会生成这个设备文件*/
	//activate character driver
	cdev_init(&mmap_dev.cdev, &mmap_opt);/*file_operations 字符设备操作指针*/
	mmap_dev.cdev.owner = THIS_MODULE;
	err = cdev_add(&mmap_dev.cdev,mmap_dev.dev_num,1);
	if(err)
		goto cdev_add_err;
	
	printk("mmap_init()--\n");
	
	return 0;
	
cdev_add_err:	
	device_destroy(mmap_dev.cls, mmap_dev.dev_num);
	class_destroy(mmap_dev.cls);
class_create_err:
	unregister_chrdev_region(mmap_dev.dev_num,3);
alloc_chrdev_region_err:
	return (-1);
}
static void wqfmmap_exit(void)
{
	printk(KERN_INFO"mmap_exit()++\n");
	
	cdev_del(&mmap_dev.cdev);
	device_destroy(mmap_dev.cls, mmap_dev.dev_num);
	class_destroy(mmap_dev.cls);
	unregister_chrdev_region(mmap_dev.dev_num,3);
	
	printk(KERN_INFO"mmap_exit()--\n");
}
module_init(wqfmmap_init);
module_exit(wqfmmap_exit);**应用层**
**wqftest.c**

```python
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h> /*加入这个头 close才不会出错*/
#include "memdev.h"  /* 包含命令定义 */
int main()
{
    int fd = 0;
    int cmd;
    int arg = 0;
    char Buf[4096];
    
    
    /*打开设备文件*/
    fd = open("/dev/wfdevice",O_RDWR);
    if (fd < 0)
    {
        printf("Open Dev Mem0 Error!\n");
        return -1;
    }
    
    /* 调用命令MEMDEV_IOCPRINT */
    printf("<--- Call MEMDEV_IOCPRINT --->\n");
    cmd = MEMDEV_IOCPRINT;
    if (ioctl(fd, cmd, &arg) < 0)
        {
            printf("Call cmd MEMDEV_IOCPRINT fail\n");
            return -1;
    }
    
    
    /* 调用命令MEMDEV_IOCSETDATA */
    printf("<--- Call MEMDEV_IOCSETDATA --->\n");
    cmd = MEMDEV_IOCSETDATA;
    arg = 2007;
    if (ioctl(fd, cmd, &arg) < 0)
        {
            printf("Call cmd MEMDEV_IOCSETDATA fail\n");
            return -1;
    }
    
    /* 调用命令MEMDEV_IOCGETDATA */
    printf("<--- Call MEMDEV_IOCGETDATA --->\n");
    cmd = MEMDEV_IOCGETDATA;
    if (ioctl(fd, cmd, &arg) < 0)
        {
            printf("Call cmd MEMDEV_IOCGETDATA fail\n");
            return -1;
    }
    printf("<--- In User Space MEMDEV_IOCGETDATA Get Data is %d --->\n\n",arg);    
    
    close(fd);
    return 0;    
}
```
**memdev.h**\#ifndef _MEMDEV_H_
\#define _MEMDEV_H_
\#include "linux/ioctl.h"
\#ifndef MEMDEV_MAJOR
\#define MEMDEV_MAJOR 0   /*预设的mem的主设备号*/
\#endif
\#ifndef MEMDEV_NR_DEVS
\#define MEMDEV_NR_DEVS 2    /*设备数*/
\#endif
\#ifndef MEMDEV_SIZE
\#define MEMDEV_SIZE 4096
\#endif
/*mem设备描述结构体*/
struct mem_dev                                     
{                                                        
  char *data;                      
  unsigned long size;       
};
/* 定义幻数 */
\#define MEMDEV_IOC_MAGIC  'k'
/* 定义命令 */
\#define MEMDEV_IOCPRINT   _IO(MEMDEV_IOC_MAGIC, 1)
\#define MEMDEV_IOCGETDATA _IOR(MEMDEV_IOC_MAGIC, 2, int)
\#define MEMDEV_IOCSETDATA _IOW(MEMDEV_IOC_MAGIC, 3, int)
\#define MEMDEV_IOC_MAXNR 3
\#endif /* _MEMDEV_H_ */Makefile
```python
wqftest.o : wqftest.c memdev.h
	g++ -c wqftest.c -o wqftest.o
```



