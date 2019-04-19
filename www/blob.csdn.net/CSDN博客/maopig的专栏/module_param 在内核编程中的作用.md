# module_param 在内核编程中的作用 - maopig的专栏 - CSDN博客
2012年01月02日 18:14:12[maopig](https://me.csdn.net/maopig)阅读数：3065
                
 module_param
　　在用户态下编程可以通过main()的来传递命令行参数，而编写一个内核模块则通过module_param()!
module_param的作用
一.module_param
1.为什么引入 
 在用户态下编程可以通过main()来传递命令行参数，而编写一个内核模块则可通过module_param()来传递命令行参数
内核允许对驱动程序在加载的时候传递参数，在编写内核程序的时候。要在代码中用宏module_param来指定接受的参数名，而这个宏在<linux/moduleparam.h>中的定义如下
/* Helper functions: type is byte, short, ushort, int, uint, long,
   ulong, charp, bool or invbool, or XXX if you define param_get_XXX,
   param_set_XXX and param_check_XXX. */
#define module_param_named(name, value, type, perm)               \
    param_check_##type(name, &(value));                   \
    module_param_call(name, param_set_##type, param_get_##type, &value, perm); \
    __MODULE_PARM_TYPE(name, #type)
#define module_param(name, type, perm)                \
    module_param_named(name, name, type, perm)
由此可知module_param的实现是通过module_param_named(name,
 name, type, perm)的。
其中使用了 3 个参数:要传递的参数变量名,变量的数据类型,以及访问参数的权限。
- **测试模块，源程序hello.c内容如下：**
- 
- #include <linux/init.h>
- #include <linux/module.h>
#include <linux/moduleparam.h>        
- 
- MODULE_LICENSE("Dual BSD/GPL");    
static char *who= "world";             
static int times = 1;       
module_param(times,int,S_IRUSR);     
module_param(who,charp,S_IRUSR);   
static int hello_init(void)       
{
int i;
for(i=0;i<times;i++)
printk(KERN_ALERT "(%d) hello, %s!\n",i,who);
return 0;
}
static void hello_exit(void) 
{
printk(KERN_ALERT"Goodbye, %s!\n",who);
}
module_init(hello_init);
module_exit(hello_exit);
编译生成可执行文件hello
插入:
- # insmod hellowho="world"times=5
- 出现5次"hello,world!"：
- #（1）hello,world!
- #（2）hello,world!
- #（3）hello,world!
- # （4）hello,world!
- #（5）hello,world!
- 
- 卸载：
- # rmmodhello
- 出现：
- #Goodbye,world!
驱动程序module的工作流程主要分为四个部分：
1、 insmod module
2、 驱动module的初始化（初始化结束后即进入“潜伏”状态，直到有系统调用）
3、 当操作设备时，即有系统调用时，调用驱动module提供的各个服务函数
4、 rmmod module
一、 驱动程序的加载
Linux驱动程序分为两种形式：一种是直接编译进内核，另一种是编译成module，然后在需要该驱动module时手动加载。
在用insmod加载module时，还可以给提供模块参数，如：
static char *whom=”world”；
static int howmany=10;
module_param(howmany,int,S_IRUGO);
module_param(whom,charp,S_IRUGO);
这样，当使用insmod scull.ko whom=”string” howmany=20这样的命令加载驱动时，whom和howmay的值就会传入scull驱动模块了。
二、 驱动module的初始化
scull_init_module函数中主要做了以下几件事情：
a) 分配并注册主设备号和次设备号
int register_chrdev_region(dev_t first, unsigned int count, char *name)
int alloc_chrdev_region(dev_t *dev, unsigned int firstminor, unsigned int count, char *name)
b) 初始化代表设备的struct结构体：scull_dev
c) 初始化互斥体init_MUTEX
d) 初始化在内核中代表设备的cdev结构体，最主要是将该设备与file_operations结构体联系起来。在Linux内核中，cdev结构体才是真正代表了某个设备。在内核调用设备的open，read等操作之前，必须先分配并注册一个或者多个cdev结构。
三、设备操作
涉及open ,close ioclt,release等函数
四、卸载
scull_cleanup_module
