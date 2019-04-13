
# Linux 设备驱动模型中的class（类） - 嵌入式Linux - CSDN博客

2013年09月13日 16:45:04[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：621


首先，想说明一下，促使我研究class（类）的是因为它能够自动创建/dev下的设备节点。当然class还有其另外的作用，且自动创建设备节点的还有udev系统，udev是处于用户空间的，其自动创建设备节点也是依赖于sysfs文件系统中提供的class类，我有个问题，如果我的内核没有移植好udev系统，只是利用class（类），能不能够自动创建设备节点呢？？？针对这样一个问题，想写这个文章理清理清一下思路。
一个类是一个设备的高级视图, 它抽象出低级的实现细节. 驱动可以见到一个SCSI 磁盘或者一个 ATA 磁盘, 在类的级别, 它们都是磁盘. 类允许用户空间基于它们做什么来使用设备, 而不是它们如何被连接或者它们如何工作.
几乎所有的类都在 sysfs 中在 /sys/class 下出现. 因此, 例如, 所有的网络接口可在 /sys/class/net 下发现, 不管接口类型. 输入设备可在 /sys/class/input 下, 以及串行设备在 /sys/class/tty. 一个例外是块设备, 由于历史的原因在 /sys/block.
早期的Linux内核（版本2.4之前）并没有实现一个统一的设备模型，设备节点的创建一般是mknod命令手动创建或利用devfs文件系统创建。早期的Linux发行版一般会采用手动创建的方式预先把通常用到的节点都创建出来，而嵌入式系统则会采用devfs的方式。起初Linux2.6 内核还支持devfs，但从2.6.18开始，内核完全移除了devfs系统而采用的udev的方式动态的创建设备节点。因此，新的Linux发行版都采用udev的方式管理设备节点文件。
udev 依靠所有通过 sysfs 输出给用户空间的设备信息, 并且依靠被 /sbin/hotplug 通知有设备添加或去除. 策略决策, 例如给一个设备什么名子, 可在用户空间指定, 内核之外. 这保证了命名策略被从内核中去除并且允许大量每个设备名子的灵活性.现在来验证一下，class类是怎样自动创建设备节点的。代码如下：
注意，首先申明一下，在看ldd3的时候，书上说的class的相关接口是class_simple，比如：class_simple_create（）、class_simple_destory（）、class_device_create（）、class_device_destory（）等，经我查看，我这是Linux2.6.31版本的内核，里面没有这些接口，这些接口都被修改成了class_create（）、class_destroy（）、device_create（）、device_destory（）等。相关接口可以自己去查看内核源代码。

```python
#include <linux/device.h>  
#include <linux/module.h>  
#include <linux/kernel.h>  
#include <linux/init.h>  
#include <linux/cdev.h>  
#include <linux/fs.h>  
  
#define DEVNAME "hello"   
static dev_t dev;   
static struct class *hello_class;   
static struct cdev *hello_cdev;  
  
  
static int hello_open(struct inode *inode,struct file *flp)  
{  
    return 0;  
}  
  
static int hello_close(struct inode *inode,struct file *flp)  
{  
        return 0;  
}  
static struct file_operations hello_fops={  
        .owner=THIS_MODULE,  
        .open   =hello_open,  
        .release=hello_close,  
    };  
  
static int __init hello_init(void)  
{  
    int error;  
  
    error = alloc_chrdev_region(&dev, 0, 2, "hello");  
    if (error)  
    {  
        printk("hello: alloc_chardev_region failed! ");  
        goto out;  
    }  
    hello_cdev = cdev_alloc();  
    if (hello_cdev == NULL)  
    {  
        printk("hello: alloc cdev failed! ");  
        error = -ENOMEM;  
        goto out_chrdev;  
    }  
    hello_cdev->ops = &hello_fops;  
    hello_cdev->owner = THIS_MODULE;  
    error = cdev_add(hello_cdev, dev, 1);  
    if (error)  
    {  
        printk("hello: cdev_add failed! ");  
        goto out_cdev;  
    }  
    hello_class = class_create(THIS_MODULE, DEVNAME);
    if (IS_ERR(hello_class))   
    {   
        error = PTR_ERR(hello_class);   
        goto out_chrdev;   
    }   
    device_create(hello_class, NULL, dev, NULL, DEVNAME);  
    //memset (hello_buf, 0, sizeof(hello_buf));  
    //memcpy(hello_buf, DEFAULT_MSG, sizeof(DEFAULT_MSG));  
    printk("hello: Hello World! ");  
    return 0;  
  
out_cdev:  
    cdev_del(hello_cdev);  
out_chrdev:  
    unregister_chrdev_region(hello_cdev->dev, 2);  
out:  
    return error;  
}  
static void __exit hello_exit(void)  
{  
    device_destroy(hello_class, dev);  
    class_destroy(hello_class); 
    unregister_chrdev_region(hello_cdev->dev, 2);  
    cdev_del(hello_cdev);  
    printk("hello: Goodbye World ");  
}  
  
  
module_init(hello_init);  
module_exit(hello_exit);  
  
MODULE_LICENSE("GPL");  
MODULE_AUTHOR("YWW");  
MODULE_DESCRIPTION("HELLO_CLASS_AUTO_DEV");
```

上述代码中标注红色的为class类自动创建设备节点的代码。下载到开发板上运行，我们可以查看/dev目录、/sys/class/目录，看是否有/dev/hello、/sys/class/hello_class这样的目录。
经过哥的验证，在dev、sys/class目录下都会有相应的节点。可以说明一下，我的文件系统里并没有移植好udev系统。那就是说，class可以自动创建设备节点，不需要udev，而udev自动创建节点需要用到class。
为了证明我一开始的疑问，需要去深入class_create（）、device_create（）的源码探索了。
可以肯定，sys/class中节点的创建是在struct class的注册函数里，/dev/节点的创建也应该在device的注册函数里
日后分析。

