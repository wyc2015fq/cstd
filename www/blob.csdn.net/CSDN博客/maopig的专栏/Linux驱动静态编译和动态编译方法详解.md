# Linux驱动静态编译和动态编译方法详解 - maopig的专栏 - CSDN博客
2017年04月05日 23:37:58[maopig](https://me.csdn.net/maopig)阅读数：2602
# [](http://blog.csdn.net/qq_21792169/article/details/50465363)
内核源码树的目录下都有两个文档Kconfig和Makefile。分布到各目录的Kconfig构成了一个分布式的内核配置[数据库](http://lib.csdn.net/base/mysql)，每个Kconfig分别描述了所属目录源文档相关的内核配置菜单。在内核配置make
 menuconfig时，从Kconfig中读出菜单，用户选择后保存到.config的内核配置文档中。在内核编译时，主Makefile调用这个.config，就知道了用户的选择。
上面的内容说明了，Kconfig就是对应着内核的配置菜单。假如要想添加新的驱动到内核的源码中，能够修改Kconfig,这样就能够选择这个驱动，假如想使这个驱动被编译，要修改Makefile。所以，添加新的驱动时需要修改的文档有两种（注意不只是两个）*Kconfig，*Makefile
.config是make menuconfig【配置后生成的一个配置文件，也就是说可以拷贝一个别人配置好的.config来使用，内核中不同开发板也自带了一些配置文件在arch/arm/config/目录下面有很多配置文件可以选择和自己单板相似。
第一种方法：
利用内核自身的Makefile特性来编译我们的驱动程序，下面由一个简单的hello.c驱动程序来说明。
在/drivers/char/目录下写hello.c  修改Makefile,Kconfig
hello.c内容：
#include <[Linux](http://lib.csdn.net/base/linux)/init.h>           /* 初始化模块所用到的头文件 */
#include <linux/module.h>   /* 
KERN_ALERT用到的 头文件*/
static int hello_init(void)      /* 入口函数，加载驱动程序所调用的函数 */
{
        printk(KERN_ALERT"Hello, world\n");
        return 0;
}
static void hello_exit(void)     /* 出口函数 ，卸载驱动程序所调用的函数 */
{
        printk(KERN_ALERT"Goodbye, cruel world\n");
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("Dual BSD/GPL");   /* 通用的协议 */
MODULE_AUTHOR("wangluoren");     /* 作者 */
MODULE_DESCRIPTION("A simple hello world module");   /* 程序描述 */
MODULE_ALIAS("A simplest module");             /* 别名 */
Kconfig内容：
config HELLO           /* Makefile中编译可变选项中宏的名字，作用就是定义一个可以选择的变量 */
tristate "Hello Driver added by Dong"        /* make menuconfig 菜单界面显示的信息，tristate 提供Y M N三种选项 */
default n           /* 菜单选项默认为N */
help
   test for adding driver to menuconfig.     /* 一些帮助信息，写不写没有关系 */
MakeFile内容： 
obj-$(CONFIG_HELLO) += hello.o     /*CONFIG_HELLO我们在Kconfig中区定义它，我们用
 HELLO ，hello是文件名字*/
到此为止我们就写好了这个hello的驱动程序，然后我们可以make menuconfig 在
Device Drivers  --->
 Character devices  --->
<> Hello Driver added by Dong   /* 可以按空格选择Y M N ，来分别 编译进内核，模块，不编译*/
如果是直接编译进内核就可以执行make uImage
,如果是编译成模块就执行make modules.
上面我们采用的是静态编译方法看起来是那么的麻烦，当我们写驱动程序的时候如果采用这种方法的话那就够折腾了，我估计你调试一个驱动程序你都得折腾大半天，这种方法是我们确定我们驱动程序没有问题后在编译进内核。
第二种方法：
一般我们采用动态编译来写驱动程序，具体方法如下：
还是拿hello.c程序为例：
hello.c内容：
#include <linux/init.h>           /* 初始化模块所用到的头文件 */
#include <linux/module.h>   /* KERN_ALERT用到的 头文件*/
static int hello_init(void)      /* 入口函数，加载驱动程序所调用的函数 */
{
        printk(KERN_ALERT"Hello, world\n");
        return 0;
}
static void hello_exit(void)     /* 出口函数 ，卸载驱动程序所调用的函数 */
{
        printk(KERN_ALERT"Goodbye, cruel world\n");
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("Dual BSD/GPL");   /* 通用的协议 */
MODULE_AUTHOR("wangluoren");     /* 作者 */
MODULE_DESCRIPTION("A simple hello world module");   /* 程序描述 */
MODULE_ALIAS("A simplest module");             /* 别名 */
Makefile文件编写如下：（和hello.c放在同一层目录就OK）
KERN_DIR = /home/book/workspace/kernel/linux-3.4.2_jz2440   /* 虚拟机kernel的路径,每个人不一样 */
all:
        make -C $(KERN_DIR) M=`pwd` modules   /* -C 表示进入后面KERN_DIR这个目录把当前路径的hello.c编译成模块*/
clean:
        make -C $(KERN_DIR) M=`pwd` modules clean  /* 一些清除工作，清除中间生成的中间文件 */
        rm -rf modules.order
obj-m   += hello.o                        /* 编译，这里要注意这个hello是你要编译的程序的文件名字 */ 
上面两种方法编译完成后都会生成一个hello.ko的文件，我们把这个文件复制到开发板的目录下面，执行如下命令      insmod hello.ko  就会打印：Hello, world
rmmod hello    就会打印  Goodbye, cruel world  (这个是一一对应的，加载一次只能卸载一次，不能重复加载)
补充一：
动态加载补充一点：如果我要用一个Makefile同时编译多个文件该怎么办呢？(这个文件的一个函数在另外一个文件当中定义，我们可以采取如下方法)方法如下：
有来个驱动文件：hello.c  hello1.c
我们可以把上面Makefile中obj-m +=hello.o改成如下命令就可以了
obj-m :=led.o
module-objs :=hello.o hello1.o   /* 这时候生成的 驱动文件就是led.ko */
补充二：
如果同时又两个驱动程序都需要编译（两者之间没有任何联系的，就是共同用kernel来编译自己的驱动）
比如有两个文件：hello1.c  hello2.c 
我们可以在同一个Makefile中这样来编写：
obj-m+=hello1.o
obj-m+= 
hello2.o    (注意这个：=已经变成+=,这两者的区别这里就不说明了)
补充三：
上面静态编译驱动我们采用的是在/drivers/char目录下面添加一个文件，我们也可以在/drivers目录下面添加一个文件夹，下面我们讲讲具体的怎么来实现。
我们在/drivers目录下面添加一个hello的文件夹，里面创建一个hello.c,Kconfig,Makefile，这个三个文件和静态编译添加一个文件是完全相同的，主要是更改的是/drivers目录下的Makefile和Kconfig文件。
Makefile中添加如下命令：
obj-$(CONFIG_HELLO)
 += hello/        /* 
CONFIG_HELLO这个宏定义在hello目录里面的Kconfig中，‘/’表示进入这个目录，调用子目录Makefile来编译子目录中的文件 */
Kconfig添加如下命令：
source
 "drivers/hello/Kconfig"     /* 把Kconfig导出到子菜单界面 */
然后执行make modules   就在drivers/hello目录下面生成了一个hello.ko的文件
现在这个驱动编译的全部用法基本讲解完毕，有了这些知识你自己都可以去分析内核的结构，从顶层Makefile开始分析，子目录下的Makefile想对简单的多，顶层Makefile是比较难的，顶层Makefile讲解在这篇文章中，[请点击这里！](http://blog.csdn.net/qq_21792169/article/details/50448639)
上面讲解了一些基础的知识，这些知识对一个驱动开发成员已经足够了，如果你是工作在内核空间，那你就应该深入分析这些结构的原理，比如怎么添加递归创建子目录，通配符，正则表达式等，你都需要去深入了解。希望我这篇文章能给驱动开发者带来帮助。
