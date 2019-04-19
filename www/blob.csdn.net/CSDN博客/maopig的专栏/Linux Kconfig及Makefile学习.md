# Linux Kconfig及Makefile学习 - maopig的专栏 - CSDN博客
2013年06月16日 15:54:07[maopig](https://me.csdn.net/maopig)阅读数：1115
个人分类：[Linux																[u-boot/驱动开发/linux](https://blog.csdn.net/maopig/article/category/930963)](https://blog.csdn.net/maopig/article/category/850267)
内核源码树的目录下都有两个文档 Kconfig （2.4版本是Config.in）和Makefile。分布到各目录的Kconfig构成了一个分布式的内核配置数据库，每个Kconfig分别描述了 所属目录源文档相关的内核配置菜单。在内核配置make menuconfig时，从Kconfig中读出菜单，用户选择后保存到.config的内核配置文档中。在内核编译时，主Makefile调用这 个.config，就知道了用户的选择。
上面的内容说明了，Kconfig就是对应着内核的配置菜单。假如要想添加新的驱动到内核的源码中，能够修改Kconfig,这样就能够选择这个驱动，假如想使这个驱动被编译，要修改Makefile。所以，添加新的驱动时需要修改的文档有两种（注意不只是两个）
*Kconfig
*Makefile
---------------------------------------------------------------------------------------------
Kconfig
1.先了解一下Kconfig的语法：
一个典型的内核配置菜单如下：
menu "Network device support"
config NETDEVICES
        bool "Enable Net Devices"
        depends on NET
        default y
       help
               This is help desciption。
...
endmenu
包含在menu/endmenu中的内容会成为Network device support的子菜单。每一个子菜单项都是由config来定义的。congfig下方的那些bool、depends on、default、help等为config的属性，用于定义该菜单项的类型、依赖项、默认值、帮助信息等。
2. 补充说明一下类型定义部分：
每个config菜单项都要有类型定义: bool布尔类型、 tristate三态(内建、模块、移除)、string字符串、 hex十六进制、 integer整型。
例如：
config HELLO_MODULE
bool "hello test module"
bool 类型的只能选中或不选中，显示为[ ]; tristate类型的菜单项多了编译成内核模块的选项，显示为< > , 假如选择编译成内核模块，则会在.config中生成一个 CONFIG_HELLO_MODULE=m的配置，假如选择内建，就是直接编译成内核影响，就会在.config中生成一个 CONFIG_HELLO_MODULE=y的配置. hex十六进制类型显示为（ ）。
3. 目录层次迭代
在Kconfig中有类似语句：source "drivers/usb/Kconfig"
用来包含（或嵌套）新的Kconfig文件，这样便可以使各个目录管理各自的配置内容，使不必把那些配置都写在同一个文件里，方便修改和管理。
----------------------------------------------------------------------------------------------
Makefile
2.6内核的Makefile分为5个组成部分：
       1. 最顶层的Makefile
       2. 内核的.config配置文件
       3.   在arch/$(ARCH) 目录下的体系结构相关的Makefile 
       4. 在s目录下的 Makefile.* 文件，是一些Makefile的通用规则 
       5. 各级目录下的大概约500个kbuild Makefile文件
顶层的Makefile文件读取 .config文件的内容，并总体上负责build内核和模块。Arch Makefile则提供补充体系结构相关的信息。 s目录下的Makefile文件包含了所有用来根据kbuild Makefile 构建内核所需的定义和规则。
Kbuild Makefile
对于Makefiles的不同组成部分，有一些不同的语法规则。针对的对象也不同，对于大部分内核模块或设备驱动的开发者和使用者来说，最常接触到的就是 各层目录下基于kbuild架构的kbuild Makefile文件。Kbuild Makefile核心内容主要包括：
1.目标定义
目标定义就是用来定义哪些内容要做为模块编译，哪些要编译链接进内核。如：
obj-y += foo.o
表示要由foo.c或者foo.s文件编译得到 foo.o并链接进内核，而obj-m则表示该文件要作为模块编译。 除了y，m以外的obj-x形式的目标都不会被编译。而更常见的做法是根据.config文件的CONFIG_ 变量来决定文件的编译方式（该变量如何起作用见文末另一篇文章的链接），如：
obj-$(CONFIG_EXT2) += ext2.o
除了obj-形式的目标以外，还有lib-y library库，hostprogs-y 主机程序等目标，但是基本都应用在特定的目录和场合下。
2.多文件模块的定义 
最简单的kbuild Makefile如上一节一句话的形式就够了，如果一个模块由多个文件组成，那么稍微复杂一些，采用模块名加 –objs后缀或者 –y后缀的形式来定义模块的组成文件。如以下例子：
obj-$(CONFIG_EXT2) += ext2.o
ext2-y := balloc.o bitmap.o
或者写成如-objs的形式：
obj-$(CONFIG_EXT2) += ext2.o
ext2-objs := balloc.o bitmap.o
模块的名字为ext2，如果CONFIG_EXT2 的值是m，由balloc.o和bitmap.o两个目标文件最终链接生成ext2.o 直至ext2.ko文件，如果CONFIG_EXT2的值是y，生成的 ext2.o将被链接进built-in.o最终链接进内核。
3.目录层次的迭代
如下例：
obj-$(CONFIG_EXT2) += ext2/
如果CONFIG_EXT2 的值为y或m，kbuild将会将ext2目录列入向下迭代的目标中。
----------------------------------------------------------------------------------------------
模块的编译
编译模块的时候，你可以将模块放在代码树中，用Make modules的方式来编译你的模块，
此时Makefile内容很简单，例如：obj-$(CONFIG_EXT2) += ext2.o 即可。
你也可以将模块相关文件目录放在代码树以外的位置，用如下命令来编译模块：
make -C <path to kernel src> M=$PWD modules
‘-C’指定代码树的位置，M=$PWD 或 M=`PWD` 告诉kbuild回到当前目录来执行build操作。
当然，我们也可以为其写一个Makefile，这里介绍一个教通用的Makefile（2.6版本）：
# Makefile2.6 
ifneq ($(KERNELRELEASE),) 
#kbuild syntax. dependency relationshsip of files and target modules are listed here. 
obj-m := hello.o 
hello-objs := hello.o   
else 
PWD := $(shell pwd) 
KDIR := /lib/modules/$(shell uname -r)/build 
all:
$(MAKE) -C $(KDIR) M=$(PWD) modules
clean: 
rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions 
endif 
此例子驱动目录叫做hello，实际中需要将下面的hello换成自己的目录名称。其中代码树路径是自动获取的。之后在目录下直接执行make命令即可，不再用敲上面一大长串命令。
----------------------------------------------------------------------------------------------
在添加新驱动时，需要创建Kconfig、Makefile文件，且需要修改父目录Kconfig、Makefile这两个文件以便将自己的驱动包含进去。例子见：[为Android内核添加新驱动，并添加到menuconfig菜单【实践简记】](http://hi.baidu.com/donghaozheng/blog/item/d48e2ea4028d51fd9152ee19.html)
在linux2.6.x/Documentation/kbuild目录下有周详的介绍有关kconfig、makefile的知识。
例子：
## 为Android内核添加新驱动，并添加到menuconfig菜单【实践简记】
为Android内核添加新驱动，并提供menuconfig选项
为Android的Linux内核2.6.25添加驱动。
1. 在drives目录下添加hello目录，内含**hello.c Kconfig Makefile**
hello.c内容：
#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");
static int hello_init(void)
{
        printk(KERN_ALERT"Hello, world\n");
        return 0;
}
static void hello_exit(void)
{
        printk(KERN_ALERT"Goodbye, cruel world\n");
}
module_init(hello_init);
module_exit(hello_exit);
Kconfig内容：
**config HELLOtristate "Hello Driver added by Dong" default nhelp   test for adding driver to menuconfig.**
MakeFile内容：
**obj-$(CONFIG_HELLO) += hello.o**
2. 上面的Kconfig文件再加上下面的两个配置，可使hello项出现在配置菜单中。
**在arch/arm/Kconfig menu "Device Drivers" endmenu之间添加source "drivers/hello/Kconfig"**
**在drivers/Kconfig menu "Device Drivers" endmenu之间添加source "drivers/hello/Kconfig"**
（不知为什么arch/arm/Kconfig中竟然含有Drivers里Kconfig内容的一个复本，
实验证明只对drivers/Kconfig中修改内容无效。）
3.**修改Drivers目录下的Makefile文件，添加如下行，obj-$(CONFIG_HELLO) += hello/当CONFIG_HELLO为y或m时，使系统能找到hello驱动的makefile。**
linux-2.6.25目录下make menuconfig，在Device Drivers菜单下选中Hello Driver added by Dong项比如M，作为module。然后保存配置，执行make命令，就可以看到 CC [M] drivers/hello/hello.o 的log了，hello目录里生成了hello.o hello.ko的等文件。
**流程：假如在make menuconfig时配置Hello Driver added by Dong为M（即编为模块，而不是编进linux内核）则.config中就会多一行CONFIG_HELLO = m如此一来，drivers/Makefile中obj-$(CONFIG_HELLO) += hello/就变成了obj-m +=hello/于是执行make命令时，便会进入hello目录里找makefile，MakeFile内容obj-$(CONFIG_HELLO) += hello.o 变成了obj-m +=hello.o，所以hello.c就被编译成模块了**
