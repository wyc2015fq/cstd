
# linux简单的字符设备驱动 - 嵌入式Linux - CSDN博客

2013年08月30日 20:46:47[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：593个人分类：[Linux																](https://blog.csdn.net/weiqifa0/article/category/1388863)



```python
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
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

struct file_operations mmap_opt = 
{
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.write = my_write,
};
static int mmap_init(void)
{
	int err = 0;
	printk("mmap_init()++\n");
	
	//dynamic alloc device number
	if(0 != (err = alloc_chrdev_region
		    (&mmap_dev.dev_num, 0,3, mmap_dev.dev_name)))  /*自动分配主次设备号*/
		goto alloc_chrdev_region_err;
	printk("major = %d, minor = %d\n",
		   MAJOR(mmap_dev.dev_num),MINOR(mmap_dev.dev_num));
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
static void mmap_exit(void)
{
	printk(KERN_INFO"mmap_exit()++\n");
	
	cdev_del(&mmap_dev.cdev);
	device_destroy(mmap_dev.cls, mmap_dev.dev_num);
	class_destroy(mmap_dev.cls);
	unregister_chrdev_region(mmap_dev.dev_num,3);
	
	printk(KERN_INFO"mmap_exit()--\n");
}
module_init(mmap_init);
module_exit(mmap_exit);
```


