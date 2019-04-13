
# linux module_init - 嵌入式Linux - CSDN博客

2015年01月04日 16:32:40[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：416个人分类：[Linux																](https://blog.csdn.net/weiqifa0/article/category/1388863)


就像你写C程序需要包含C库的头文件那样，Linux内核编程也需要包含Kernel头文件，大多的Linux驱动程序需要包含下面三个头文件：
\#include <linux/init.h>
\#include <linux/module.h>
\#include <linux/kernel.h>
其中，init.h 定义了驱动的初始化和退出相关的函数，kernel.h 定义了经常用到的函数原型及宏定义，module.h 定义了内核模块相关的函数、变量及宏。
几乎每个linux驱动都有个module_init（与module_exit的定义在Init.h (/include/linux) 中）。没错，驱动的加载就靠它。为什么需要这样一个宏？原因是按照一般的编程想法，各部分的初始化函数会在一个固定的函数里调用比如：
void init(void)
{
init_a();
init_b();
}
如果再加入一个初始化函数呢，那么在init_b()后面再加一行：init_c();这样确实能完成我们的功能，但这样有一定的问题，就是不能独 立的添加初始化函数，每次添加一个新的函数都要修改init函数。可以采用另一种方式来处理这个问题，只要用一个宏来修饰一下：
void init_a(void)
{
}
**__initlist(init_a, 1);**
它是怎么样通过这个宏来实现初始化函数列表的呢？先来看__initlist的定义：
\#define __init __attribute__((unused,**__section__(".initlist")**))
\#define __initlist(fn, lvl) /
static initlist_t __init_\#\#fn __init = { /
magic:    INIT_MAGIC, /
callback: fn, /
level:   lvl }
请注意：**__section__(".initlist")，**这个属性起什么作用呢？它告诉连接器这个变量存放在**.initlist**区段，如果所有的初始化函数都是用这个宏，那么每个函数会有对应的一个initlist_t结构体变量存放在.initlist区段，也就是说我们可以在.initlist区段找到所有初始化函数的指针。怎么找到.initlist区段的地址呢？
extern u32 __initlist_start;
extern u32 __initlist_end;
这两个变量起作用了，__initlist_start是.initlist区段的开始，__initlist_end是结束，通过这两个变量我们就可以访问到所有的初始化函数了。这两个变量在那定义的呢？在一个连接器脚本文件里
. = ALIGN(4);
.initlist : {
__initlist_start = .;
*(.initlist)
__initlist_end = .;
}
这两个变量的值正好定义在.initlist区段的开始和结束地址，所以我们能通过这两个变量访问到所有的初始化函数。
与此类似，内核中也是用到这种方法，所以我们写驱动的时候比较独立，不用我们自己添加代码在一个固定的地方来调用我们自己的初始化函数和退出函数，连接器已经为我们做好了。先来分析一下module_init。定义如下：
\#define module_init(x)     __initcall(x);              //include/linux/init.h
\#define __initcall(fn) device_initcall(fn)
\#define device_initcall(fn)                 __define_initcall("6",fn,6)
\#define __define_initcall(level,fn,id) /
static initcall_t __initcall_\#\#fn\#\#id __used /
__attribute__((__section__(".initcall" level ".init"))) = fn
如果某驱动想以func作为该驱动的入口，则可以如下声明：module_init(func);被上面的宏处理过后，变成 __initcall_func6 __used加入到内核映像的".initcall"区。内核的加载的时候，会搜索".initcall"中的所有条目，并按优先级加载它们，普通驱动程 序的优先级是6。其它模块优先级列出如下：值越小，越先加载。
\#define pure_initcall(fn)           __define_initcall("0",fn,0)
\#define core_initcall(fn)            __define_initcall("1",fn,1)
\#define core_initcall_sync(fn)          __define_initcall("1s",fn,1s)
\#define postcore_initcall(fn)             __define_initcall("2",fn,2)
\#define postcore_initcall_sync(fn)  __define_initcall("2s",fn,2s)
\#define arch_initcall(fn)            __define_initcall("3",fn,3)
\#define arch_initcall_sync(fn)          __define_initcall("3s",fn,3s)
\#define subsys_initcall(fn)                 __define_initcall("4",fn,4)
\#define subsys_initcall_sync(fn)      __define_initcall("4s",fn,4s)
\#define fs_initcall(fn)                          __define_initcall("5",fn,5)
\#define fs_initcall_sync(fn)               __define_initcall("5s",fn,5s)
\#define rootfs_initcall(fn)                  __define_initcall("rootfs",fn,rootfs)
\#define device_initcall(fn)                 __define_initcall("6",fn,6)
\#define device_initcall_sync(fn)       __define_initcall("6s",fn,6s)
\#define late_initcall(fn)             __define_initcall("7",fn,7)
\#define late_initcall_sync(fn)           __define_initcall("7s",fn,7s)
可以看到，被声明为pure_initcall的最先加载。
module_init除了初始化加载之外，还有后期释放内存的作用。linux kernel中有很大一部分代码是设备驱动代码，这些驱动代码都有初始化和反初始化函数，这些代码一般都只执行一次，为了有更有效的利用内存，这些代码所 占用的内存可以释放出来。
linux就是这样做的，对只需要初始化运行一次的函数都加上__init属性，__init 宏告诉编译器如果这个模块被编译到内核则把这个函数放到（.init.text）段，module_exit的参数卸载时同__init类似，如果驱动被 编译进内核，则__exit宏会忽略清理函数，因为编译进内核的模块不需要做清理工作，显然__init和__exit对动态加载的模块是无效的，只支持 完全编译进内核。
在kernel初始化后期，释放所有这些函数代码所占的内存空间。连接器把带__init属性的函数放在同一个section里，在用完以后，把整个section释放掉。当函数初始化完成后这个区域可以被清除掉以节约系统内存。Kenrel启动时看到的消息“Freeing unused kernel memory: xxxk freed”同它有关。
我们看源码，init/main.c中start_kernel是进入kernel()的第一个c函数，在这个函数的最后一行是rest_init();
static void rest_init(void)
{
.....
kernel_thread(kernel_init, NULL, CLONE_FS | CLONE_SIGHAND);
unlock_kernel();
cpu_idle();
.....
}
创建了一个内核线程，主函数kernel_init末尾有个函数：
/*
* Ok, we have completed the initial bootup, and
* we're essentially up and running. Get rid of the
* initmem segments and start the user-mode stuff..
*/
init_post();
这个init_post中的第一句就是free_initmem();就是用来释放初始化代码和数据的。
void free_initmem(void)
{
if (!machine_is_integrator() && !machine_is_cintegrator()) {
free_area((unsigned long)(&__init_begin),
(unsigned long)(&__init_end),
"init");
}
}
接下来就是kernel内存管理的事了。
参考地址：[http://blog.csdn.net/citytramper/archive/2006/02/16/600708.aspx](http://blog.csdn.net/citytramper/archive/2006/02/16/600708.aspx)
[http://blog.csdn.net/citytramper/archive/2006/04/15/664930.aspx](http://blog.csdn.net/citytramper/archive/2006/04/15/664930.aspx)

