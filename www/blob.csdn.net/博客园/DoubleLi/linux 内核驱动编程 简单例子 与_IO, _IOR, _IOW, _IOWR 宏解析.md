# linux 内核驱动编程 简单例子 与_IO, _IOR, _IOW, _IOWR 宏解析 - DoubleLi - 博客园






一、_IO, _IOR, _IOW, _IOWR 宏的用法与解析

在驱动程序里， ioctl() 函数上传送的变量 cmd 是应用程序用于区别设备驱动程序请求处理内容的值。cmd除了可区别数字外，还包含有助于处理的几种相应信息。 cmd的大小为 32位，共分 4 个域：
bit31~bit30 2位为 “区别读写” 区，作用是区分是读取命令还是写入命令。
bit29~bit15 14位为 "数据大小" 区，表示 ioctl() 中的 arg 变量传送的内存大小。
bit20~bit08  8位为 “魔数"(也称为"幻数")区，这个值用以与其它设备驱动程序的 ioctl 命令进行区别。
bit07~bit00   8位为 "区别序号" 区，是区分命令的命令顺序序号。
像命令码中的 “区分读写区” 里的值可能是 _IOC_NONE （0值）表示无数据传输，_IOC_READ (读)， _IOC_WRITE (写) ， _IOC_READ|_IOC_WRITE (双向)。
内核定义了 _IO() , _IOR() , IOW() 和 _IOWR() 这 4 个宏来辅助生成上面的 cmd 。下面分析 _IO() 的实现，其它的类似。

在 asm-generic/ioctl.h 里可以看到 _IO() 的定义：
#define _IO(type,nr)        _IOC(_IOC_NONE,(type),(nr),0)
再看 _IOC() 的定义：
#define _IOC(dir,type,nr,size) \
    (((dir)  << _IOC_DIRSHIFT) | \
     ((type) << _IOC_TYPESHIFT) | \
     ((nr)   << _IOC_NRSHIFT) | \
     ((size) << _IOC_SIZESHIFT))
可见，_IO() 的最后结果由 _IOC() 中的 4 个参数移位组合而成。

再看 _IOC_DIRSHIT 的定义：
#define _IOC_DIRSHIFT    (_IOC_SIZESHIFT+_IOC_SIZEBITS)_IOC_SIZESHIFT 的定义：
#define _IOC_SIZESHIFT    (_IOC_TYPESHIFT+_IOC_TYPEBITS)_IOC_TYPESHIF 的定义：
#define _IOC_TYPESHIFT    (_IOC_NRSHIFT+_IOC_NRBITS)_IOC_NRSHIFT 的定义：
#define _IOC_NRSHIFT    0_IOC_NRBITS 的定义：
#define _IOC_NRBITS    8_IOC_TYPEBITS 的定义：
#define _IOC_TYPEBITS    8由上面的定义，往上推得到：
_IOC_TYPESHIFT = 8
_IOC_SIZESHIFT = 16
_IOC_DIRSHIFT = 30
所以，(dir)  << _IOC_DIRSHIFT) 表是 dir 往左移 30 位，即移到 bit31~bit30 两位上，得到方向(读写)的属性；
            (size) << _IOC_SIZESHIFT) 位左移 16 位得到“数据大小”区；
            (type) << _IOC_TYPESHIFT) 左移 8位得到"魔数区" ；
           (nr)   << _IOC_NRSHIFT)      左移 0 位( bit7~bit0) 。

这样，就得到了 _IO() 的宏值。



**这几个宏的使用格式为**：
- _IO (魔数， 基数);
- _IOR (魔数， 基数， 变量型)
- _IOW  (魔数， 基数， 变量型)
- _IOWR (魔数， 基数，变量型 )


魔数 (magic number)
魔数范围为 0~255 。通常，用英文字符 "A" ~ "Z" 或者 "a" ~ "z" 来表示。设备驱动程序从传递进来的命令获取魔数，然后与自身处理的魔数想比较，如果相同则处理，不同则不处理。魔数是拒绝误使用的初步辅助状态。设备驱动程序可以通过 _IOC_TYPE (cmd) 来获取魔数。不同的设备驱动程序最好设置不同的魔数，但并不是要求绝对，也是可以使用其他设备驱动程序已用过的魔数。

基(序列号)数
基数用于区别各种命令。通常，从 0开始递增，相同设备驱动程序上可以重复使用该值。例如，读取和写入命令中使用了相同的基数，设备驱动程序也能分辨出来，原因在于设备驱动程序区分命令时使用 switch ，且直接使用命令变量 cmd值。创建命令的宏生成的值由多个域组合而成，所以即使是相同的基数，也会判断为不同的命令。设备驱动程序想要从命令中获取该基数，就使用下面的宏：
_IOC_NR (cmd)
通常，switch 中的 case 值使用的是命令的本身。

变量型
变量型使用 arg 变量指定传送的数据大小，但是不直接代入输入，而是代入变量或者是变量的类型，原因是在使用宏创建命令，已经包含了 sizeof() 编译命令。比如 _IOR() 宏的定义是：
#define _IOR(type,nr,size)    _IOC(_IOC_READ,(type),(nr),(_IOC_TYPECHECK(size)))
而 _IOC_TYPECHECK() 的定义正是：
#define _IOC_TYPECHECK(t) (sizeof(t))

设备驱动程序想要从传送的命令获取相应的值，就要使用下列宏函数：
_IOC_SIZE(cmd)




_IO 宏
该宏函数没有可传送的变量，只是用于传送命令。例如如下约定：
#define TEST_DRV_RESET _IO ('Q', 0)
此时，省略由应用程序传送的 arg 变量或者代入 0 。在应用程序中使用该宏时，比如：

ioctl (dev, TEST_DEV_RESET, 0)   或者  ioctl (dev, TEST_DRV_RESET) 。

这是因为变量的有效因素是可变因素。只作为命令使用时，没有必要判断出设备上数据的输出或输入。因此，设备驱动程序没有必要执行设备文件大开选项的相关处理。

_IOR 宏
该函数用于创建从设备读取数据的命令，例如可如下约定：
#define TEST_DEV_READ  _IRQ('Q', 1, int)
这说明应用程序从设备读取数据的大小为 int 。下面宏用于判断传送到设备驱动程序的 cmd 命令的读写状态：
_IOC_DIR (cmd)

运行该宏时，返回值的类型如下：
- _IOC_NONE                             :  无属性
- _IOC_READ                             :  可读属性
- _IOC_WRITE                           : 可写属性
- _IOC_READ | _IOC_WRITE : 可读，可写属性


使用该命令时，应用程序的 ioctl() 的 arg 变量值指定设备驱动程序上读取数据时的缓存(结构体)地址。

_IOW 宏
用于创建设备上写入数据的命令，其余内容与 _IOR 相同。通常，使用该命令时，ioctl() 的 arg 变量值指定设备驱动程序上写入数据时的缓存(结构体)地址。

_IOWR 宏
用于创建设备上读写数据的命令。其余内容与 _IOR 相同。通常，使用该命令时，ioctl() 的 arg 变量值指定设备驱动程序上写入或读取数据时的缓存 (结构体) 地址。

_IOR() , _IOW(), IORW() 的定义：
#define _IOR(type,nr,size)    _IOC(_IOC_READ,(type),(nr),(_IOC_TYPECHECK(size)))
#define _IOW(type,nr,size)    _IOC(_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))
#define _IOWR(type,nr,size)    _IOC(_IOC_READ|_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))



二、下面来实际动手写个测试例子

1.server.c





**[cpp]**[view plain](http://blog.csdn.net/hepeng597/article/details/7721885)[copy](http://blog.csdn.net/hepeng597/article/details/7721885)



- #include <linux/module.h>  
- #include <linux/highmem.h>  
- #include <asm/uaccess.h>  //copy_to_user  copy_from_user  
- #define MODULE_NAME "hepeng"  
- #define HEPENG_TEST 0x0  //request code  
- 
- #define HEPENG_TEST_OK 0    //reply code  
- #define HEPENG_TEST_UNKNOWN_OPTION -1 //reply code  
- #define HEPENG_TEST_UNKNOWN_DEV -2   //reply code  
- 
- #define TEST_IOC_MAGIC 'k'    //device type  
- 
- struct COMMAND_STRU_T  
- {  
- short op;   
- short error;  
- };//Params transfers between server and client  
- 
- int hepeng_cmd(struct COMMAND_STRU_T *cmd)  
- {  
- switch (cmd->op)  
-     {     
- case HEPENG_TEST:  
-             printk("hepeng_cmd: this is a test\n");  
-             cmd->error = HEPENG_TEST_OK;  
- break;  
- default:  
-             printk("hepeng_cmd: error option\n");  
-             cmd->error = HEPENG_TEST_UNKNOWN_OPTION;  
- return -1;   
-     }     
- return 0;  
- }  
- 
- int hepeng_ioctl(struct inode *inode, struct file *file, unsigned int opt, unsigned long arg)  
- {  
- int ret_val;  
- struct COMMAND_STRU_T *cmd = (struct COMMAND_STRU_T*)  
-                  kmalloc(sizeof(struct COMMAND_STRU_T), GFP_KERNEL);  
- if (!cmd)  
-     {     
-         printk("hepeng_ioctl: failed to allocate spcmd\n");  
- goto ioctl_error1;  
-     }     
- if (copy_from_user (cmd, (long *)arg, sizeof(struct COMMAND_STRU_T)))  
-     {  
-         printk("hepeng_ioctl: failed to copy from user\n");  
- goto ioctl_error2;  
-     }  
- if(_IOC_TYPE(opt) != TEST_IOC_MAGIC)  
-     {  
-         cmd->error = HEPENG_TEST_UNKNOWN_DEV;  
-         printk("hepeng_ioctl:unknown device type\n");  
- if(copy_to_user ((long *)arg, cmd, sizeof(struct COMMAND_STRU_T)))  
-             printk("hepeng_ioctl: failed to copy to user\n");  
- goto ioctl_error2;  
-     }  
-     ret_val = hepeng_cmd(cmd);  
- if (copy_to_user ((long *)arg, cmd, sizeof(struct COMMAND_STRU_T)))  
-     {  
-         printk("hepeng_ioctl: failed to copy to user\n");  
- goto ioctl_error2;  
-     }  
-     kfree(cmd);  
- return  0;  
- 
- ioctl_error2:  
-     kfree(cmd);  
- ioctl_error1:  
- return -1;  
- }  
- int sp_major; //global variable  
- 
- int test_init(void)  
- {  
- struct file_operations module_params =  
-     {  
-         owner:THIS_MODULE,  
-         ioctl:hepeng_ioctl,  
-     };//disorder assignment  
-     sp_major = register_chrdev(0, MODULE_NAME, &module_params);//register a character device  
- if (sp_major < 0)  
-     {  
-         printk(MODULE_NAME "failed to initialize\n");  
- return -1;  
-     }  
-     printk(MODULE_NAME ":initialize ok\n");  
- return 0;  
- }  
- void test_clean(void)  
- {  
-     printk(MODULE_NAME ":quit module\n");  
-     unregister_chrdev(sp_major, MODULE_NAME);//unregister a character device  
- }  
- 
- module_init(test_init);//load module  
- module_exit(test_clean);//quit module  
- 
- MODULE_DESCRIPTION("Only A Test");  
- MODULE_AUTHOR("He Peng");  
- MODULE_LICENSE("GPL v2");  



该模块由test_init函数开始，设置了file_operations的成员owner,ioctl，而ioctl指向hepeng_ioctl函数，即注册ioctl函数，如果该模块加载成功，收到客户端的ioctl函数，那么就会由hepeng_ioctl来处理。test_init调用了register_chrdev函数来注册字符型设备，注册字符设备是为了能和/dev/XXX的设备联系的前提。模块退出会调用test_clean函数。TEST_IOC_MAGIC即魔数，客户端需要用该魔数来ioctl
2.Makefile







**[cpp]**[view plain](http://blog.csdn.net/hepeng597/article/details/7721885)[copy](http://blog.csdn.net/hepeng597/article/details/7721885)



- MODULE_NAME:=hepeng  
- 
- $(MODULE_NAME)-objs := server.o  
- obj-m:=$(MODULE_NAME).o  
- KDIR := /usr/src/kernels/2.6.18-164.el5-x86_64  #usual path: /lib/modules/(kernel name)/build  
- PWD  := $(shell pwd)  
- default:  
-     $(MAKE) -C $(KDIR) M=$(PWD) modules  
- clean:  
-     rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module.symvers  
- distclean:  
-     rm -rf *.o *~ core .depend .*.cmd *.mod.c .tmp_versions Module.symvers  






3.make 编译

编译后，当前目录下生成一些文件，如hepeng.ko等，用/sbin/modinfo hepeng.ko查看结果如下：



filename:       hepeng.ko

license:        GPL v2

author:         He peng

description:    Only A Test

srcversion:     A66C3C701909D1C32559F94

depends:        

vermagic:       2.6.18-164.el5 SMP mod_unload gcc-4.1

4.手工加载模块

insmod hepeng.ko

tail -f /var/log/messages 可以看见如下信息：

Jun 14 23:52:41 localhost kernel: hepeng:initialize ok

如果不是，则检查linux系统运行的kernel版本与server.c编译用的kernel版本是否一致

5.生成字符设备文件

只有生成字符设备文件才能给客户端调用

cat /proc/devices | grep hepeng，可以看到如下信息：

253 hepeng

生成字符设置文件：mknod /dev/hepeng c 253 0

成功后生成/dev/hepeng文件，即字符设备，可以被客户端open来打开。这个文件可以用于应用程序与kernel程序交互



6.client.c



**[cpp]**[view plain](http://blog.csdn.net/hepeng597/article/details/7721885)[copy](http://blog.csdn.net/hepeng597/article/details/7721885)



- #include <iostream>  
- #include <unistd.h>  
- #include <cstdio>  
- #include <sys/ioctl.h>  
- #include <linux/types.h>  
- #include <fcntl.h>  
- using namespace std;  
- #define HEPENG_TEST 0  
- #define TEST_IOC_MAGIC 'k'  //device command type  
- #define IOCTL_HEPENGCMD     _IOWR ( TEST_IOC_MAGIC, 2, struct COMMAND_STRU_T)  
- struct COMMAND_STRU_T  
- {  
- short op;   
- short error;  
- };  
- int main()  
- {  
- int fd = open("/dev/hepeng", O_RDWR);  
- if(fd < 0)  
-     {     
-         cout << "error open" << endl;  
- return -1;   
-     }     
-     COMMAND_STRU_T cmd;  
-     cmd.op = HEPENG_TEST;  
- if( ioctl(fd, IOCTL_HEPENGCMD, (void*)&cmd) < 0 )   
-     {     
-         cout << "error ioctl" << endl;  
-         cout << "cmd.error:" << cmd.error << endl;  
- return -1;   
-     }     
-     cout << "cmd.error:" << cmd.error << endl;  
-     close(fd);  
- return 0;  
- }  



g++ client.c -o client



7.使用client测试驱动是否正常

sudo ./client

输入：cmd.error:0

查看/var/log/messages

hepeng_cmd: this is a test









