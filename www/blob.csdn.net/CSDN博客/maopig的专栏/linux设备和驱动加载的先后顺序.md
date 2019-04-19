# linux设备和驱动加载的先后顺序 - maopig的专栏 - CSDN博客
2012年03月20日 22:50:04[maopig](https://me.csdn.net/maopig)阅读数：18903
**Linux驱动先注册总线，总线上可以先挂device，也可以先挂driver，那么究竟怎么控制先后的顺序呢。**
**Linux系统使用两种方式去加载系统中的模块：动态和静态。**
**静态加载：将所有模块的程序编译到Linux内核中，由do_initcall函数加载**
**核心进程（/init/main.c）kernel_inità do_basic_setup()àdo_initcalls（）该函数中会将在__initcall_start和__initcall_end之间定义的各个模块依次加载。那么在__initcall_start 和 __initcall_end之间都有些什么呢？**
**找到/arch/powerpc/kernel/vmlinux.lds文件，找到.initcall.init段：**
**.initcall.init : {**
**  __initcall_start = .;**
**  *(.initcall0.init) *(.initcall0s.init) *(.initcall1.init) *(.initcall1s.init) *(.initcall2.init) *(.initcall2s.init) *(.initcall3.init) *(.initcall3s.init) *(.initcall4.init) *(.initcall4s.init) *(.initcall5.init) *(.initcall5s.init) *(.initcallrootfs.init) *(.initcall6.init) *(.initcall6s.init) *(.initcall7.init) *(.initcall7s.init)**
**  __initcall_end = .;**
**  }**
**可以看出在这两个宏之间依次排列了14个等级的宏，由于这其中的宏是按先后顺序链接的，所以也就表示，这14个宏有优先级：0>1>1s>2>2s………>7>7s。**
**那么这些宏有什么具体的意义呢，这就要看/include/linux/init.h文件：**
**#define pure_initcall(fn)              __define_initcall("0",fn,0)**
**#define core_initcall(fn)              __define_initcall("1",fn,1)**
**#define core_initcall_sync(fn)           __define_initcall("1s",fn,1s)**
**#define postcore_initcall(fn)              __define_initcall("2",fn,2)**
**#define postcore_initcall_sync(fn)    __define_initcall("2s",fn,2s)**
**#define arch_initcall(fn)        __define_initcall("3",fn,3)**
**#define arch_initcall_sync(fn)            __define_initcall("3s",fn,3s)**
**#define subsys_initcall(fn)          __define_initcall("4",fn,4)**
**#define subsys_initcall_sync(fn)       __define_initcall("4s",fn,4s)**
**#define fs_initcall(fn)                   __define_initcall("5",fn,5)**
**#define fs_initcall_sync(fn)         __define_initcall("5s",fn,5s)**
**#define rootfs_initcall(fn)            __define_initcall("rootfs",fn,rootfs)**
**#define device_initcall(fn)          __define_initcall("6",fn,6)**
**#define device_initcall_sync(fn)       __define_initcall("6s",fn,6s)**
**#define late_initcall(fn)         __define_initcall("7",fn,7)**
**#define late_initcall_sync(fn)             __define_initcall("7s",fn,7s)**
**    这里就定义了具体的宏，我们平时用的module_init在静态编译时就相当于device_initcall。举个例子，在2.6.24的内核中：gianfar_device使用的是arch_initcall，而gianfar_driver使用的是module_init，因为arch_initcall的优先级大于module_init，所以gianfar设备驱动的device先于driver在总线上添加。**
系统初始化函数集(subsys_initcall)和初始化段应用
前言：前段时间做一个项目需要设计一个动态库，并希望在加载库的同时自动执行一些初始化动作，于是联想到了linux内核众子系统的初始化，于是研究之，并在过这程中发现了初始化段的存在，利用初始化段实现了该功能。工作一年，笔记积累多了，慢慢变得杂乱无章，于是开博，一方面整理笔记，梳理知识，另一方面和大家交流，共同进步。
keyword：subsys_initcall, init, init_call
1 系统初始化调用函数集分析(静态)
1.1 函数定义
在linux内核代码里，运用了subsys_initcall来进行各种子系统的初始化，具体怎么初始化的呢？其实并不复杂。以2.6.29内核作为例子。在<include/linux/init.h>下就能找到subsys_initcall的定义：
#define pure_initcall(fn)              __define_initcall("0",fn,0)
#define core_initcall(fn)              __define_initcall("1",fn,1)
#define core_initcall_sync(fn)    __define_initcall("1s",fn,1s)
#define postcore_initcall(fn)      __define_initcall("2",fn,2)
#define postcore_initcall_sync(fn) __define_initcall("2s",fn,2s)
#define arch_initcall(fn)              __define_initcall("3",fn,3)
#define arch_initcall_sync(fn)    __define_initcall("3s",fn,3s)
#define subsys_initcall(fn)          __define_initcall("4",fn,4)
#define subsys_initcall_sync(fn) __define_initcall("4s",fn,4s)
#define fs_initcall(fn)                  __define_initcall("5",fn,5)
#define fs_initcall_sync(fn)        __define_initcall("5s",fn,5s)
#define rootfs_initcall(fn)           __define_initcall("rootfs",fn,rootfs)
#define device_initcall(fn)         __define_initcall("6",fn,6)
#define device_initcall_sync(fn) __define_initcall("6s",fn,6s)
#define late_initcall(fn)               __define_initcall("7",fn,7)
#define late_initcall_sync(fn)      __define_initcall("7s",fn,7s)
而__define_initcall又被定义为
#define __define_initcall(level,fn,id) \
 static initcall_t  __initcall_##fn##id   __used \
 __attribute__((__section__(".initcall" level ".init"))) = fn
所以 subsys_initcall(fn) == __initcall_fn4 它将被链接器放于section  .initcall4.init 中。（attribute将会在另一篇文章中介绍）
1.2 初始化函数集的调用过程执行过程：
start_kernel->rest_init
系统在启动后在rest_init中会创建init内核线程
init->do_basic_setup->do_initcalls
do_initcalls中会把.initcall.init.中的函数依次执行一遍:
for (call = __initcall_start; call < __initcall_end; call++) {
.    .....
result = (*call)();
.    ........
}
这个__initcall_start是在文件<arch/xxx/kernel/vmlinux.lds.S>定义的:
 .initcall.init : AT(ADDR(.initcall.init) - LOAD_OFFSET) {
   __initcall_start = .;
   INITCALLS
   __initcall_end = .;
  }
INITCALLS被定义于<asm-generic/vmlinux.lds.h>:
#define INITCALLS       \
   *(.initcall0.init)      \
   *(.initcall0s.init)      \
   *(.initcall1.init)      \
   *(.initcall1s.init)      \
   *(.initcall2.init)      \
   *(.initcall2s.init)      \
   *(.initcall3.init)      \
   *(.initcall3s.init)      \
   *(.initcall4.init)      \
   *(.initcall4s.init)      \
   *(.initcall5.init)      \
   *(.initcall5s.init)      \
   *(.initcallrootfs.init)      \
   *(.initcall6.init)      \
   *(.initcall6s.init)      \
   *(.initcall7.init)      \
   *(.initcall7s.init)
2 基于模块方式的初始化函数(动态)
2.1函数定义subsys_initcall的静态调用方式应该讲清楚来龙去脉了,现在看看动态方式的初始化函数调用(模块方式)。在<include/linux/init.h>里，如果MODULE宏被定义，说明该函数将被编译进模块里，在系统启动后以模块方式被调用。
#define core_initcall(fn)         module_init(fn)
#define postcore_initcall(fn)  module_init(fn)
#define arch_initcall(fn)        module_init(fn)
#define subsys_initcall(fn)    module_init(fn)
#define fs_initcall(fn)             module_init(fn)
#define device_initcall(fn)     module_init(fn)
#define late_initcall(fn)         module_init(fn)
这是在定义MODULE的情况下对subsys_initcall的定义,就是说对于驱动模块,使用subsys_initcall等价于使用module_init
2.2 module_init 分析下面先看看module_init宏究竟做了什么
#define module_init(initfn)     \
 static inline initcall_t __inittest(void)  \ /*定义此函数用来检测传入函数的类型,并在编译时提供警告信息*/
 { return initfn; }     \
 int init_module(void) __attribute__((alias(#initfn))); /*声明init_modlue为 initfn的别名,insmod只查找名字为init_module函数并调用*/
typedef int (*initcall_t)(void); /*函数类型定义*/
在以模块方式编译一个模块的时候，会自动生成一个xxx.mod.c文件， 在该文件里面定义一个struct module变量，并把init函数设置为上面的init_module() 而上面的这个init_module，被alias成模块的初始化函数(参考<gcc关键字:__attribute__, alias, visibility, hidden>)。
也就是说，模块装载的时候(insmod，modprobe)，sys_init_module()系统调用会调用module_init指定的函数（对于编译成>模块的情况）。
2.3 module的自动加载内核在启动时已经检测到了系统的硬件设备，并把硬件设备信息通过sysfs内核虚拟文件系统导出。sysfs文件系统由系统初始化脚本挂载到/sys上。udev扫描sysfs文件系统，根据硬件设备信息生成热插拔（hotplug）事件，udev再读取这些事件，生成对应的硬件设备文件。由于没有实际的硬件插拔动作，所以这一过程被称为coldplug。
udev完成coldplug操作，需要下面三个程序： 
udevtrigger——扫描sysfs文件系统，生成相应的硬件设备hotplug事件。 
udevd——作为deamon，记录hotplug事件，然后排队后再发送给udev，避免事件冲突（race conditions）。 
udevsettle——查看udev事件队列，等队列内事件全部处理完毕才退出。
要规定事件怎样处理就要编写规则文件了.规则文件是udev的灵魂，没有规则文件，udev无法自动加载硬件设备的驱动模块。它一般位于<etc/udev/rules.d>
3初始化段的应用这里给出一个简单的初始化段的使用例子，将a.c编译成一个动态库，其中，函数a（）和函数c（）放在两个不同的初始化段里，函数b（）默认放置；编译main.c，链接到由a.c编译成的动态库，观察各函数的执行顺序。
# cat a.c #include <stdio.h>
typedef int (*fn) (void);
int a(void)
{
    printf("a\n");
    return 0;
}
__attribute__((__section__(".init_array.2"))) static fn init_a = a;
int c(void)
{
    printf("c\n");
    return 0;
}
__attribute__((__section__(".init_array.1"))) static fn init_c = c;
int b()
{
    printf("b\n");
    return 0;
} 
# cat main.c
#include<stdio.h>
int b();
int main()
{
    printf("main\n");
    b();
} 
# cat mk.sh
gcc -fPIC -g -c a.c
gcc -shared -g -o liba.so a.o
cp liba.so /lib/ -fr
gcc main.c liba.so 
ldconfig 
./a.out  
# gcc -fPIC -g -c a.c
# gcc -shared -g -o liba.so a.o
# cp liba.so /lib/
# gcc main.c liba.so 
# ldconfig 
# ./a.out 
a
c
main
b
在类unix操作系统中，驱动加载方式一般分为：动态加载和静态加载，下面分别对其详细论述。
一、动态加载
动态加载是将驱动模块加载到内 核中，而不能放入/lib/modules/下。
    在2.4内核中，加载驱动命令为：insmod ,删除模块为：rmmod；
    在2.6以上内核中，除了insmod与rmmod外，加载命令还有modprobe；
    insmod与modprobe不同之处：
    insmod 绝对路径/××.o，而modprobe ××即可，不用加.ko或.o后缀，也不用加路径；最重要的一点是：modprobe同时会加载当前模块所依赖的其它模块；
    lsmod查看当前加载到内核中的所有驱动模块，同时提供其它一些信息，比如其它模块是否在使用另一个模块。
二、静态加载
（一）概念
    在执行make menuconfig命令进行内核配置裁剪时，在窗口中可以选择是否编译入内核，还是放入/lib/modules/下相应内核版本目录中，还是不选。
（二） 操作步骤
    linux设备一般分为：字符设备、块设备和网络设备，每种设备在内核源代码目录树drivers/下都有对应的目录，其加载方法类似，以下以字符设备静 态加载为例，假设驱动程序源代码名为ledc.c，具体操作步骤如下：
第一步：将ledc.c源程序放入内核源码drivers/char/下；
第二步：修改drivers/char/Config.in文件，具体修改如下：
         按照打开文件中的格式添加即可；
         在文件的适当位置（这个位置随便都可以，但这个位置决定其在make menuconfig窗口中所在位置）加入以下任一段代码：
         tristate 'LedDriver' CONFIG_LEDC
         if [ "$CONFIG_LEDC" = "y" ];then
         bool '   Support for led on h9200 board' CONFIG_LEDC_CONSOLE
         fi
         说明:以上代码使用tristate来定义一个宏,表示此驱动可以直接编译至内核（用*选择）,也可以编制至/lib/modules/下（用M选择）, 或者不编译（不选）。
         bool 'LedDriver' CONFIG_LEDC
         if [ "$CONFIG_LEDC" = "y" ];then
         bool '   Support for led on h9200 board' CONFIG_LEDC_CONSOLE
         fi
         说明:以上代码使用tristate来定义一个宏,表示此驱动只能直接编译至内核（用*选择）或者不编译（不选），不能编制至/lib/modules/ 下（用M选择）。
 第三步：修改drivers/char/Makefile文件
         在适当位置加入下面一行代码：
         obj-$(CONFIG_LEDC)   +=   ledc.o
         或者在obj-y一行中加入ledc.o,如：
         obj-y += ledc.o mem.o 后面不变；
    OK，经过以上的设置就可以在执行make menuconfig命令后的窗口中的character devices---> 中进行选择配置了。选择后重新编译就ok了。
