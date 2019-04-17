# Linux下GPIO驱动 - DoubleLi - 博客园







编写驱动程序，首先要了解是什么类型的设备。linux下的设备分为三类，分别为：字符设备，块设备和网络设备。字符设备类型是根据是否以字符流为数据的交换方式，大部分设备都是字符设备，如键盘，串口等，块设备则是以块为单位进行管理的设备，如，磁盘。网络设备就是网卡等。

其次要了解应用程序和驱动程序的区别，两者的主要区别分为以下三点：

1入口函数的任务不相同，应用程序完成一个任务，驱动只完成初始化工作，比如中断

      申请，寄存器设置，定时器设置。

2运行时的cpu模式不相同，驱动具有很高的权限，应用程序是在用户态下运行，而驱

  动程序是在内核态下执行。

3 驱动程序不能调用C库函数，内核为驱动程序提供一些函数。如printk(KERN_NOTICE fmt, ##arg)，第一个参数为打印级别，有如下的打印级别：

KERN_EMERG 用于紧急事件,一般是系统崩溃前的提示信息

KERN_ALERT 用于需要立即采取动作的场合

KERN_CRIT 临界状态,通常设计验证的硬件或软件操作失败

KERN_ERR 用于报告错误状态.设备驱动程序通常会用它报告来自硬件的问题

KERN_WARNING 就可能出现的问题提出警告.这些问题通常不会对系统造成严重破坏

KERN_NOTICE 有必要提示的正常情况.许多安全相关的情况用这个级别汇报

KERN_INFO 提示性信息.有很多驱动程序在启动时用这个级别打印相关信息

KERN_DEBUG 用于调试的信息

u_long copy_from_user(void *to, const void *from, u_long len)，由用户态拷贝到内核态；

u_long copy_to_user(void * to, const void *from, u_long len)，由内核态拷贝到用户态。

鉴于以上区别，驱动程序需要完成以下三点基本功能：

1：要对设备进行初始化和释放功能模块，就如上面的寄存器设置，中断的申请，向内核注 

   册驱动程序（register_chrdev（）），卸载驱动程序（unregister_chrdev（））。

2：能进行数据传输，在read()，write()函数里具体实现，数据传输工作。

3：能进行控制操作，给用户提供的ioctl（）函数里可实现一些用户的选择性设置功能。

确定一个设备的执行函数集（结构体）

static struct file_operations myGPIO_fops = {

owner: THIS_MODULE,

write: myGPIO_write,

read: myGPIO_read,

ioctl: myGPIO_ioctl,

open: myGPIO_open,

release: myGPIO_release,

};

接下来是初始化工作，需要写在一个init（）函数中，这个函数是独立的也是自动执行的，在这之中主要是对一些寄存器进行初始化操作。同样需要完成卸载驱动模块。

myGPIO_Major = register_chrdev(0, DRIVER_NAME, &myDriver_fops);

上面的程序完成设备号的注册，第一个参数为主设备号，一般为0，由系统来分配。

第二个参数为设备名，这需要在/dev/（/dev目录下设备名由命令 <mknod  设备名 C 主设备号  从设备号>来生成）目录下出现的设备名相符合。相反的在卸载中就取消注册

unregister_chrdev(myGPIO_Major, DRIVER_NAME);

最后将这两个模块加入到内核中，由程序段的最后两行完成。

static int __init myGPIO_init(void)

{

PRINTK("GPIO init/n");

myGPIO_Major = register_chrdev(0, DRIVER_NAME, &myGPIO_fops);

if(myGPIO_Major < 0)

{

PRINTK("register char device fail!/n");

return myGPIO_Major;

}

PRINTK("register myGPIO OK! Major = %d/n", myGPIO_Major);

#ifdef CONFIG_DEVFS_FS

devfs_myDriver_dir = devfs_mk_dir(NULL, "GPIO", NULL);

devfs_myDriver_raw = devfs_register(devfs_myDriver_dir, "raw0", DEVFS_FL_DEFAULT, myGPIO_Major, 0, S_IFCHR | S_IRUSR | S_IWUSR, &myGPIO_fops, NULL);

PRINTK("add dev file to devfs OK!/n");

#endif

return 0;

}

static void __exit myGPIO_exit(void)

{

/* Module exit code */

PRINTK("GPIO exit/n");

/* Driver unregister */

if(myGPIO_Major > 0)

{

#ifdef CONFIG_DEVFS_FS

devfs_unregister(devfs_myDriver_raw);

devfs_unregister(devfs_myDriver_dir);

#endif

unregister_chrdev(myGPIO_Major, DRIVER_NAME);

}

return;

}

MODULE_AUTHOR("LiuFan");

MODULE_LICENSE("Dual BSD/GPL");

module_init(myGPIO_init);

module_exit(myGPIO_exit);

设备执行函数功能的实现将在下面完成。如结构体的函数，但并不是全都需要实现。open()函数中是执行一些设备工作前的初始化工作。rlease()则是将设备的相关寄存器恢复到原来的值。read()函数是将设备中的数据拷贝到内核，write（）函数是将内核数据拷贝到对应的设备中。MOD_INC_USE_COUNT和MOD_DEC_USE_COUNT两个宏是提供给系统对硬件资源进行控制访问的。在open（）和rlease()两个函数中最基本的操作应是实现以上两个宏的操作。

static unsigned char myGPIO_Buffer[1024*1024];

/* Driver Operation Functions */

static int myGPIO_open(struct inode *inode, struct file *filp)

{

// int Minor = MINOR(inode->i_rdev);

// filp->private_data = 0;

MOD_INC_USE_COUNT;

PRINTK("myDriver open called!/n");

return 0;

}

static int myGPIO_release(struct inode *inode, struct file *filp)

{

// int Minor = MINOR(inode->i_rdev);

MOD_DEC_USE_COUNT;

PRINTK("myDriver release called!/n");

return 0;

}

static ssize_t myGPIO_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)

{

char dat;

size_t read_size = count;

PRINTK("GPIO read called!/n");

PRINTK("/tcount=%d, pos=%d/n", count, (int)*f_pos);

/* if(*f_pos >= sizeof(myGPIO_Buffer))

{

PRINTK("[GPIO read]Buffer Overlap/n");

*f_pos = sizeof(myGPIO_Buffer);

return 0;

}

if((count + *f_pos) > sizeof(myGPIO_Buffer))

{

PRINTK("count + f_pos > sizeof buffer/n");

read_size = sizeof(myGPIO_Buffer) - *f_pos;

}*/

dat= GPFDAT;

copy_to_user(buf,&dat,1);

// *f_pos += read_size;

return read_size;

}

static ssize_t myGPIO_write(struct file *filp,const char *buf, size_t count, loff_t *f_pos)

{

char dat;

size_t fill_size = count;

PRINTK("myDriver write called!/n");

PRINTK("/tcount=%d, pos=%d/n", count, (int)*f_pos);

if(*f_pos >= sizeof(myGPIO_Buffer))

{

PRINTK("[myDriver write]Buffer Overlap/n");

*f_pos = sizeof(myGPIO_Buffer);

return 0;

}

if((count + *f_pos) > sizeof(myGPIO_Buffer))

{

PRINTK("count + f_pos > sizeof buffer/n");

fill_size = sizeof(myGPIO_Buffer) - *f_pos;

}

copy_from_user(&dat,buf,fill_size);

GPFDAT = dat;

// *f_pos += fill_size;

return fill_size;

}

控制ioctl() 函数则是提供给应用层的接口函数，功能并不是固定的，由开发者定义，一般都是对硬件的一些除过上述功能的其他操作。

static int myGPIO_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)

{

int i;

unsigned int mask=0x01;

GPFUP = 0x00;

PRINTK("myGPIO ioctl called(%d)!/n", cmd);

    switch(cmd)

{

case MOD_IN:

for(i=0;i<8;i++)

{

if((mask & arg)!=0x0)

{

GPFCON &=~(3<<i*2); 

}

mask =mask << 1;

}

break;

case MOD_OUT:

 PRINTK("IOCTRL 0 called(0x%lx)!/n", arg);

 for(i=0;i<8;i++)

 {

 if((mask & arg)!=0x00)

 {

        GPFCON &= ~(3 <<( i*2));

GPFCON |=(0x01<<(i*2));

 }

mask=mask<<1;

 }

 break;

case MOD_EXIT_INT:

PRINTK("IOCTRL 1 called(0x%lx)!/n", arg);

GPFDAT = 0xFFFFFF00;

break;

default:

break;

}

return 0;

}












