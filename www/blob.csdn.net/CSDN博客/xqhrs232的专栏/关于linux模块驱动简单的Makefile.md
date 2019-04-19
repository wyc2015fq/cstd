# 关于linux模块驱动简单的Makefile - xqhrs232的专栏 - CSDN博客
2016年04月06日 18:01:46[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：286
原文地址::[http://blog.csdn.net/lufeiop02/article/details/6446343](http://blog.csdn.net/lufeiop02/article/details/6446343)
相关文章
1、[一个简单的Linux驱动程序和Makefile](http://www.cppblog.com/edog/archive/2013/10/18/203810.html)----[http://www.cppblog.com/edog/archive/2013/10/18/203810.aspx](http://www.cppblog.com/edog/archive/2013/10/18/203810.aspx)
2、[linux设备驱动makefile入门解析](http://blog.csdn.net/shanzhizi/article/details/8626474)----[http://blog.csdn.net/shanzhizi/article/details/8626474](http://blog.csdn.net/shanzhizi/article/details/8626474)
( 转)嵌入式Linux驱动开发笔记
Linux驱动一般以模块module的形式来加载，首先需要把驱动编译成模块的形式。简单的例子，
Begin of hello.c file
**[cpp]**[view
 plain](http://blog.csdn.net/lufeiop02/article/details/6446343#)[copy](http://blog.csdn.net/lufeiop02/article/details/6446343#)
- #include <linux/init.h>
- #include <linux/kernel.h>
- #include <linux/module.h>
- 
- staticint __init test_init(void)  
- {  
-        printk("init module/n");  
- 
- return 0;  
- }  
- staticvoid __exit test_exit(void)  
- {  
-        printk("exit modules/n");  
- }  
- 
- module_init(test_init);  
- module_exit(test_exit);  
Over of hello.c file
Makefile为，
**[cpp]**[view
 plain](http://blog.csdn.net/lufeiop02/article/details/6446343#)[copy](http://blog.csdn.net/lufeiop02/article/details/6446343#)
- PWD = $(shell pwd)  
- KERNEL_SRC = /usr/src/linux-source-2.6.15/  
- 
- obj-m := test.o  
- module-objs := test.o  
- 
- all:  
-        $(MAKE) -C $(KERNEL_SRC) M=$(PWD) modules  
- 
- 
- clean:  
-        rm *.ko  
-        rm *.o  
在test.c和Makefile所在的目录下运行make，如果看到类似输出
**[cpp]**[view
 plain](http://blog.csdn.net/lufeiop02/article/details/6446343#)[copy](http://blog.csdn.net/lufeiop02/article/details/6446343#)
- make -C /usr/src/linux-source-2.6.15/ M=/home/vmeth modules  
- 
- make[1]: Entering directory `/usr/src/linux-source-2.6.15'  
-   CC [M]  /home/vmeth/hello.o  
-   Building modules, stage 2.  
-   MODPOST  
-   CC      /home/vmeth/hello.mod.o  
-   LD [M]  /home/vmeth/hello.ko  
- make[1]: Leaving directory `/usr/src/linux-source-2.6.15'  
一般用下面的Makefile，
**[cpp]**[view
 plain](http://blog.csdn.net/lufeiop02/article/details/6446343#)[copy](http://blog.csdn.net/lufeiop02/article/details/6446343#)
- # Makefile2.6
- ifneq ($(KERNELRELEASE),)  
- #kbuild syntax. dependency relationshsip of files and target modules are listed here.
- 
- mymodule-objs := hello.o  
- obj-m := hello.o     
- 
- else
- PWD  := $(shell pwd)  
- 
- KVER ?= $(shell uname -r)  
- KDIR := /lib/modules/$(KVER)/build #KDIR目录其实是链接到上面那个Makefile中的那个  
-                                          /usr/src/linux-source-2.6.15/*中  
- all:  
-        $(MAKE) -C $(KDIR) M=$(PWD)  
- 
- clean:  
-        rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions  
- 
- endif  
KERNELRELEASE 是在内核源码的顶层Makefile中定义的一个变量，在第一次读取执行此Makefile时，KERNELRELEASE没有被定义，所以make将读取执行else之后的内容。
当make的目标为all时，-C $(KDIR) 指明跳转到内核源码目录下读取那里的Makefile；M=$(PWD) 表明然后返回到当前目录继续读入、执行当前的Makefile。
当从内核源码目录返回时，KERNELRELEASE已被被定义，kbuild也被启动去解析kbuild语法的语句，make将继续读取else之前的内容。else之前的内容为kbuild语法的语句, 指明模块源码中各文件的依赖关系，以及要生成的目标模块名。
每个内核的名字都包含了它的版本号，这也是 uname -r 命令显示的值。
关于报以下错误的解决方法：
Makefile:1: *** 遗漏分隔符
这个有多种情况：
1，命令前要加tab键
2，可能标点符号输成中文形的了
3，比如上面那个文件：ifneq后面要加个空格
