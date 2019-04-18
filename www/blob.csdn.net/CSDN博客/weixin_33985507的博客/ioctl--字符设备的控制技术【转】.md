# ioctl--字符设备的控制技术【转】 - weixin_33985507的博客 - CSDN博客
2017年01月06日 09:40:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
本文转载自：[http://blog.csdn.net/coding__madman/article/details/51356313](http://blog.csdn.net/coding__madman/article/details/51356313)
字符设备的控制
1. 字符设备控制理论
    1.1 作用
          大部分驱动程序除了需要提供读写设备的能力外，还需要具备控制设备的能力。比如：改变波特率
    1.2 应用程序接口
          在用户空间，使用ioctl系统调用来控制设备，原型如下：
          int  ioctl(int fd, unsigned long cmd, ...)
          fd: 要控制的设备文件描述符
          cmd: 发送给设备的控制命令
          ...: 第三个参数是可选的参数，存在与否依赖于控制命令（第二个参数）
    1.3 设备驱动方法
![](https://img-blog.csdn.net/20160509212424414?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2. 字符设备控制实现
    2.1 定义命令 
          命令其实质而言就是一个整数，但为了让这个整数具备更好的可读性，我们通常会把这个整数分为几个段，类             型（8位），序号， 参数传送方向，参数长度
          type(类型/幻数)：表明这是属于哪个设备的命令
          number(序号)：用来区分同一设备的不同命令
          direction: 参数传送的方向，可能的值是_IOC_NONE(没有数据传输)，_IOC_READ, _IOC_WRITE(向设备读写           参数)
          size : 参数长度
[Linux](http://lib.csdn.net/base/linux)系统提供了下面的宏来帮助定义命令：
          * _IO(type, nr) : 不带参数的命令
          *_IOR(type, nr, datatype) : 从设备中读取参数的命令
          *_IOW(type, nr, datatype) : 向设备写入参数的命令
          例如：
                  #define MEM_MAGIC  'm' //定义幻数
                  #define MEM_SET  _IOW(MEM_MAGIC, 0, int)
    2.2 实现设备方法
          unlocked_ioctl函数的实现通常是根据命令执行的一个switch语句。但是，当命令号不能匹配任何一个设备所支           持的命令时， 返回 -EINVAL
          编程模型：
                 switch cmd:
                     case  命令A：
                        //执行A所对应的操作
                case  命令B：
                        //执行B对应的操作
                defaule:
                       return -EINVAL;
实例代码分析来实现ioctl:
这里的memdev.c还是接着上篇的基础上改的
**[cpp]**[view plain](http://blog.csdn.net/coding__madman/article/details/51356313#)[copy](http://blog.csdn.net/coding__madman/article/details/51356313#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1676916/fork)
- #include <linux/module.h>  
- #include <linux/types.h>  
- #include <linux/fs.h>  
- #include <linux/errno.h>  
- #include <linux/init.h>  
- #include <linux/cdev.h>  
- #include <asm/uaccess.h>  
- #include <linux/slab.h>  
- #include "memdev.h"  
- 
- 
- int dev1_registers[5];  
- int dev2_registers[5];  
- 
- struct cdev cdev;   
- dev_t devno;  
- 
- /*文件打开函数*/  
- int mem_open(struct inode *inode, struct file *filp)  
- {  
- 
- /*获取次设备号*/  
- int num = MINOR(inode->i_rdev);  
- 
- if (num==0)  
-         filp->private_data = dev1_registers;  
- else if(num == 1)  
-         filp->private_data = dev2_registers;  
- else  
- return -ENODEV;  //无效的次设备号  
- 
- return 0;   
- }  
- 
- /*文件释放函数*/  
- int mem_release(struct inode *inode, struct file *filp)  
- {  
- return 0;  
- }  
- 
- /*读函数*/  
- static ssize_t mem_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos)  
- {  
-   unsigned long p =  *ppos;  
-   unsigned int count = size;  
- int ret = 0;  
- int *register_addr = filp->private_data; /*获取设备的寄存器基地址*/  
- 
- /*判断读位置是否有效*/  
- if (p >= 5*sizeof(int))  
- return 0;  
- if (count > 5*sizeof(int) - p)  
-     count = 5*sizeof(int) - p;  
- 
- /*读数据到用户空间*/  
- if (copy_to_user(buf, register_addr+p, count))  
-   {  
-     ret = -EFAULT;  
-   }  
- else  
-   {  
-     *ppos += count;  
-     ret = count;  
-   }  
- 
- return ret;  
- }  
- 
- /*写函数*/  
- static ssize_t mem_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos)  
- {  
-   unsigned long p =  *ppos;  
-   unsigned int count = size;  
- int ret = 0;  
- int *register_addr = filp->private_data; /*获取设备的寄存器地址*/  
- 
- /*分析和获取有效的写长度*/  
- if (p >= 5*sizeof(int))  
- return 0;  
- if (count > 5*sizeof(int) - p)  
-     count = 5*sizeof(int) - p;  
- 
- /*从用户空间写入数据*/  
- if (copy_from_user(register_addr + p, buf, count))  
-     ret = -EFAULT;  
- else  
-   {  
-     *ppos += count;  
-     ret = count;  
-   }  
- 
- return ret;  
- }  
- 
- /* seek文件定位函数 */  
- static loff_t mem_llseek(struct file *filp, loff_t offset, int whence)  
- {   
-     loff_t newpos;  
- 
- switch(whence) {  
- case SEEK_SET:   
-         newpos = offset;  
- break;  
- 
- case SEEK_CUR:   
-         newpos = filp->f_pos + offset;  
- break;  
- 
- case SEEK_END:   
-         newpos = 5*sizeof(int)-1 + offset;  
- break;  
- 
- default:   
- return -EINVAL;  
-     }  
- if ((newpos<0) || (newpos>5*sizeof(int)))  
- return -EINVAL;  
- 
-     filp->f_pos = newpos;  
- return newpos;  
- 
- }  
- <span style="color:#ff0000;">//设备控制函数  
- long mem_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)  
- {  
- 
- switch(cmd)  
-     {  
- case MEM_RESTART: //假如这里传入的是重启的命令  
-             printk(KERN_WARNING"restart device!\n");//这里通过虚拟设备来模拟  
- return 0;  
- case MEM_SET: //如果传入的是设置参数命令  
-             printk(KERN_WARNING"arg is : %d\n", arg);//打印出传入的参数  
- return 0;  
- default:  
- return -EINVAL;//其他的打印错误  
- 
-     }  
- 
- return 0;  
- }</span>  
- 
- 
- /*文件操作结构体*/  
- static const struct file_operations mem_fops =  
- {  
-   .llseek = mem_llseek,  
-   .read = mem_read,  
-   .write = mem_write,  
-   .open = mem_open,  
-   .release = mem_release,  
-   <span style="color:#ff0000;">.unlocked_ioctl = mem_ioctl,</span>  
- };  
- 
- /*设备驱动模块加载函数*/  
- static int memdev_init(void)  
- {  
- /*初始化cdev结构*/  
-   cdev_init(&cdev, &mem_fops);  
- 
- /* 注册字符设备 */  
-   alloc_chrdev_region(&devno, 0, 2, "memdev");  
-   cdev_add(&cdev, devno, 2);  
- //printk(KERN_WARNING"Hello Memdev!\n");  
-   printk("Hello Memdev!\n");  
- }  
- 
- /*模块卸载函数*/  
- static void memdev_exit(void)  
- {  
-   cdev_del(&cdev);   /*注销设备*/  
-   unregister_chrdev_region(devno, 2); /*释放设备号*/  
-   printk(KERN_WARNING"exit Memdev!\n");  
- }  
- 
- MODULE_LICENSE("GPL");  
- 
- module_init(memdev_init);  
- module_exit(memdev_exit);  
memdev.h
**[cpp]**[view plain](http://blog.csdn.net/coding__madman/article/details/51356313#)[copy](http://blog.csdn.net/coding__madman/article/details/51356313#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1676916/fork)
- #define MEM_MAGIC 'm' //定义一个幻数，而长度正好和ASC码长度一样为8位，所以这里定义个字符  
- #define MEM_RESTART _IO(MEM_MAGIC, 0)   //第一个命令是重启的命令,重启的命令不带参数 第一命令这里序号定义成0  
- #define MEM_SET  _IOW(MEM_MAGIC, 1, int) //设置参数命令 序号为1， 类型为int  
测试应用程序mem_ctl.c
**[cpp]**[view plain](http://blog.csdn.net/coding__madman/article/details/51356313#)[copy](http://blog.csdn.net/coding__madman/article/details/51356313#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1676916/fork)
- #include<sys/types.h>  
- #include<sys/stat.h>  
- #include<fcntl.h>  
- #include<sys/ioctl.h>  
- #include "memdev.h"  
- 
- int main()  
- {  
- int fd;  
- 
-     fd = open("/dev/memdev0", O_RDWR);//可读可写打开文件  
- 
-     ioctl(fd, MEM_SET, 115200);//第一个参数fd，第二个是我们要发送的命令，第三个是要传入的参数  
- 
-     ioctl(fd, MEM_RESTART);//重启  
- 
-     close(fd);  
- }  
Makefile
**[cpp]**[view plain](http://blog.csdn.net/coding__madman/article/details/51356313#)[copy](http://blog.csdn.net/coding__madman/article/details/51356313#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1676916/fork)
- obj-m := memdev.o  
- KDIR := /home/kernel/linux-ok6410  
- all:  
-     make -C $(KDIR) M=$(PWD) modules CROSS_COMPILE=arm-linux- ARCH=arm  
- clean:  
-     rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.bak *.order  
这里ioctl函数原型可以查看内核源码Fs.h(/include/linux目录下)文件
![](https://img-blog.csdn.net/20160509224300475?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160509224505978?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后这次采用静态编译mem_ctl.c文件 arm-linux-gcc -static -o memctl (这里采用静态编译上面博文有说道)
由于挂载了整个rootfs目录，这里编译产生的文件会同步到我的开发板
![](https://img-blog.csdn.net/20160509224913749?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
安装设备驱动模块，然后cat /proc/device 查看设备驱动程序的主设备号
![](https://img-blog.csdn.net/20160509225054325?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后创建设备文件，这样我们的应用程序就可以通过这个设备文件来访问设备了（当然这里我们的设备是虚拟出来的字符设备，不过麻雀虽小五脏俱全）
mknod memdev0 c 252 0 (执行该命令)
memdevo是我们给字符设备取的名字， 252是该字符设备的设备号，
![](https://img-blog.csdn.net/20160509225512648?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这里可以看到/dev目录下就会产生memdev0这个设备文件了，运行mem_clt应用程序
![](https://img-blog.csdn.net/20160509225657827?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
至此ioctl字符设备的控制就OVER了！
