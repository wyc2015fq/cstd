# 用户空间访问I2C设备驱动 - maopig的专栏 - CSDN博客
2012年06月14日 23:18:26[maopig](https://me.csdn.net/maopig)阅读数：866标签：[c																[linux																[semaphore																[cmd																[user																[makefile](https://so.csdn.net/so/search/s.do?q=makefile&t=blog)](https://so.csdn.net/so/search/s.do?q=user&t=blog)](https://so.csdn.net/so/search/s.do?q=cmd&t=blog)](https://so.csdn.net/so/search/s.do?q=semaphore&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[u-boot/驱动开发/linux](https://blog.csdn.net/maopig/article/category/930963)
2012-01-11 15:33:43
标签：[Linux](http://blog.51cto.com/tagindex.php?keyword=Linux)[I2C](http://blog.51cto.com/tagindex.php?keyword=I2C)[字符设备](http://blog.51cto.com/tagindex.php?keyword=%D7%D6%B7%FB%C9%E8%B1%B8)[设备驱动](http://blog.51cto.com/tagindex.php?keyword=%C9%E8%B1%B8%C7%FD%B6%AF)[用户空间](http://blog.51cto.com/tagindex.php?keyword=%D3%C3%BB%A7%BF%D5%BC%E4)
原创作品，允许转载，转载时请务必以超链接形式标明文章 [原始出处](http://ticktick.blog.51cto.com/823160/761830) 、作者信息和本声明。否则将追究法律责任。[http://ticktick.blog.51cto.com/823160/761830](http://ticktick.blog.51cto.com/823160/761830)
    关于Linux下如何编写芯片的I2C驱动，本系列第一篇文章[《手把手教你写Linux I2C设备驱动》](http://ticktick.blog.51cto.com/823160/760020)对编写I2C Client 设备驱动的关键代码给出了初步的讲述和示例，第二篇文章[《Linux下读写芯片的I2C寄存器》](http://ticktick.blog.51cto.com/823160/760965)对于具体如何在驱动层封装读写芯片I2C寄存器也进行了详细的描述，这两篇文章的代码整合到一起，就构成了I2C设备驱动的主要部分，本文则致力于将该驱动进一步封装，为用户空间提供访问的接口。
    如果希望在用户空间访问我们写的I2C设备驱动，最常用的做法就是为该I2C驱动编写一套字符设备驱动，这样，用户空间则可以通过对字符设备驱动的访问，间接地实现对I2C芯片寄存器的读写控制。下面，我们在前两篇文章的代码的基础上，封装一层字符设备驱动，并给出在用户空间的使用示例。
**1.  编写字符设备驱动**
    关于字符设备驱动的编写，我依然从实例应用的角度来展开描述，关于原理性的东西，网上有许多文章，可以搜索参考。
**（1）首先，创建一个包含有cdev对象的结构体及对象，代表着本实例的字符设备对象。**
- struct tvp5158_dev{        
- struct cdev cdev;  
-     int    major;      
- struct semaphore semLock;        
- };  
- // global dev object 
- struct tvp5158_dev g_tvp5158_dev; 
    cdev即字符设备对象，major为分配的字符设备主设备号，semaphore用于互斥，保护i2c读写过程。
**（2）第二步，创建文件操作结构体对象**
- struct file_operations tvp5158_dev_FileOps = {  
-         .owner   = THIS_MODULE,  
-         .open    = tvp5158_devOpen,  
-         .release = tvp5158_devRelease,  
-         .ioctl   = tvp5158_devIoctl,  
- }; 
    我们把对I2C寄存器的读写操作放到 ioctl 命令中执行，不需要实现 read 和 write 函数，故这里只实现文件的打开、释放、以及 ioctl 操作。
**（3） 实现设备打开和关闭函数**
- staticint tvp5158_devOpen(struct inode *inode, struct file *filp)  
- {  
- printk(KERN_INFO "I2C: tvp5158_devOpen, %4d, %2d \n", major, minor);  
- 
-     filp->private_data = NULL;  
- 
- return 0;  
- }  
- 
- staticint tvp5158_devRelease(struct inode *inode, struct file *filp)  
- {  
-     printk(KERN_INFO "I2C: tvp5158_devRelease");  
- 
- return 0;  
- } 
**（4）实现 IOCTL 函数**
    这里的ioctl 函数的实现很关键，是驱动层与用户层交互的核心部分，这里将会定义相关的I2C读写命令枚举，并且调用前面文章中封装好的I2C读写代码。
- #define I2C_CMD_READ       (0x01) 
- #define I2C_CMD_WRITE      (0x02) 
- 
- struct I2C_Param{  
- uint8_t *reg;  
- uint8_t *value;  
- };  
- 
- staticint tvp5158_devIoctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)  
- {  
- uint8_t reg,data;
- structI2C_Param param;  
- 
-     down_interruptible(&g_tvp5158_dev.semLock);  
- 
- // get i2c param from userspace 
-     copy_from_user(¶m, (void *)arg, sizeof(param));  
- 
- switch(cmd){  
- case I2C_CMD_WRITE:  
-         {  
-             copy_from_user(&reg, param.reg,sizeof(uint8_t));
-             copy_from_user(&data,param.value,sizeof(uint8_t));
-             tvp5158_i2c_write(&g_tvp5158_obj->client, reg, data);  
- break;  
-         }  
- case I2C_CMD_READ:  
-         {  
-             copy_from_user(&reg, param.reg,sizeof(uint8_t));
-             tvp5158_i2c_read(&g_tvp5158_obj->client, reg, &data);  
-             copy_to_user(param.value,&data,sizeof(uint8_t));  
- break;  
-         }  
- default:  
- break;  
-     }  
-     up(&g_tvp5158_dev.semLock);  
- 
- return 0;  
- } 
    其中，I2C_Param是与用户空间交互用的参数结构体，用户空间必须定义相同的结构体以保证交互的正确性。g_tvp5158_obj 和 tvp5158_i2c_read/write 均为前面文章中定义的变量和函数。
**（5）在__init 代码中注册本字符设备驱动**
- staticint __init tvp5158_i2c_init(void)  
- {  
- int result;  
-     dev_t dev = 0;  
-     result = alloc_chrdev_region(&dev, 0, 1, “tvp5158_dev”);  
- if (result < 0) {  
-         printk(KERN_WARNING "I2C: can't get device major num \n");  
- return result;  
-     }  
-     g_tvp5158_dev.major = MAJOR(dev);      
- 
-     sema_init(&g_tvp5158_dev.semLock, 1);  
-     cdev_init(&g_tvp5158_dev.cdev, &tvp5158_dev_FileOps);  
- 
-     g_tvp5158_dev.cdev.owner = THIS_MODULE;  
-     g_tvp5158_dev.cdev.ops   = &tvp5158_dev_FileOps;  
- 
-     cdev_add(&g_tvp5158_dev.cdev, dev, 1);     
- 
- return i2c_add_driver(&tvp5158_i2c_driver);;  
- }  
**（6）在 __exit 代码中注销本字符设备驱动**
- staticvoid __exit tvp5158_i2c_exit(void)  
- {  
-     dev_t devno = MKDEV(g_tvp5158_dev.major, 0);  
- 
-     i2c_del_driver(&tvp5158_i2c_driver);  
- 
-     cdev_del(&g_tvp5158_dev.cdev);  
- 
-     unregister_chrdev_region(devno, 1);  
- } 
    注意，本初始化代码和逆初始化在第一篇文章中已经出现过，这里补充完整了，将字符设备驱动的代码添加进来了。
**2.  用户空间的使用方法**
    首先，编写Makefile将驱动编译成模块，然后在用户空间对生成的模块(*.ko)进行加载（insmod），然后再 /dev 目录下创建设备节点 /dev/tvp5158_dev ，最后，在用户空间即可编写测试代码，打开该设备文件，通过 ioctl 命令进行访问。
    上面这个过程示例如下：
- // 假设生成的模块.ko名称为 tvp5158.ko
- 第一步：insmod tvp5158.ko 
- 
- // 假设上面tvp5158_i2c_init函数中 g_tvp5158_dev.major 的值为 74
- 第二步：mknod /dev/tvp5158_dev c 74 0 
    下面给出最后在用户空间的测试代码示例。
- #include <stdio.h> 
- 
- int main()  
- {  
- int status;  
- struct I2C_Param param;  
- 
-     unit8_t reg   = 0x08;
-     unit8_t value = 0;
- 
- int fd = open("/dev/tvp5158_dev", O_RDWR);  
- if( fd < 0)  
-     {  
- return -1;  
-     }  
- 
-     param.reg    = ®  
-     param.value  = &value;  
-     status = ioctl(fd,I2C_CMD_READ,¶m);  
- if( status < 0)  
-     {  
-         printf("read fail!\n");  
- return -1;  
-     }  
-     printf("the 0x80 reg 's value = %d\n",value);  
- 
-     close(fd);  
- 
- return 0;  
- } 
** 3.   总结**
    到此为止，Linux下的I2C设备驱动基本编写过程已经讲述完毕，以后凡是拿到新的芯片，需要在Linux下读写I2C寄存器，均可参考本系列的代码进行编写。当然，这里只是讲述了I2C设备驱动编写的一些最基本的方法，关于I2C设备驱动的原理部分并没有涉及，希望自己以后更加深入地了解了Linux设备驱动原理后再进一步阐述。本文希望对初学者有所帮助，文中有什么讲述不正确的地方，欢迎留言或者来信[lujun.hust@gmail.com](mailto:lujun.hust@gmail.com)交流。
