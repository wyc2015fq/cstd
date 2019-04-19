# 内核中_init，_exit中的作用 - maopig的专栏 - CSDN博客
2012年03月30日 09:29:54[maopig](https://me.csdn.net/maopig)阅读数：7395
__init， __initdata等属性标志，是要把这种属性的代码放入目标文件的.init.text节，数据放入.init.data节──这一过程是通过编译内核时为相关目标平台提供了xxx.lds链接脚本来指导ld完成的。
    对编译成module的代码和数据来说，当模块加载时，__init属性的函数就被执行;
   对静态编入内核的代码和数据来说，当内核引导时，do_basic_setup()函数调用do_initcalls()函数，后者负责所有.init节函数的执行。
    在初始化完成后，用这些关键字标识的函数或数据所占的内存会被释放掉。
1） 
所有标识为__init的函数在链接的时候都放在.init.text这个区段内， 
在这个区段中，函数的摆放顺序是和链接的顺序有关的，是不确定的。 
2） 
所有的__init函数在区段.initcall.init中还保存了一份函数指针， 
在初始化时内核会通过这些函数指针调用这些__init函数指针， 
并在整个初始化完成后，释放整个init区段（包括.init.text，.initcall.init等）， 
注意，这些函数在内核初始化过程中的调用顺序只和这里的函数指针的顺序有关， 
和1）中所述的这些函数本身在.init.text区段中的顺序无关。 
在2.4内核中，这些函数指针的顺序也是和链接的顺序有关的，是不确定的。 
在2.6内核中，initcall.init区段又分成7个子区段，分别是 
.initcall1.init 
.initcall2.init 
.initcall3.init 
.initcall4.init 
.initcall5.init 
.initcall6.init 
.initcall7.init 
（参见include/linux/init.h和vmlinux.lds ）
当需要把函数fn放到.initcall1.init区段时，只要声明 
core_initcall(fn); 
即可。 
其他的各个区段的定义方法分别是： 
core_initcall(fn) --->.initcall1.init 
postcore_initcall(fn) --->.initcall2.init 
arch_initcall(fn) --->.initcall3.init 
subsys_initcall(fn) --->.initcall4.init 
fs_initcall(fn) --->.initcall5.init 
device_initcall(fn) --->.initcall6.init 
late_initcall(fn) --->.initcall7.init 
而与2.4兼容的initcall(fn)则等价于device_initcall(fn)。 
各个子区段之间的顺序是确定的，即先调用.initcall1.init中的函数指针 
再调用.initcall2.init中的函数指针，等等。 
而在每个子区段中的函数指针的顺序是和链接顺序相关的，是不确定的。 
在内核中，不同的init函数被放在不同的子区段中，因此也就决定了它们的调用顺序。 
这样也就解决了一些init函数之间必须保证一定的调用顺序的问题。 
2. Linux Kernel源代码中与段有关的重要宏定义 
Ａ. 关于__init、__initdata、__exit、__exitdata及类似的宏 
打开Linux Kernel源代码树中的文件：include/init.h，可以看到有下面的宏定议： 
#define __init  __attribute__ ((__section__ (".init.text")))  __cold 
#define __initdata    __attribute__ (( __section__ (".init.data"))) 
#define __exitdata   __attribute__ (( __section__ (".exit.data"))) 
#define __exit_call  __attribute_used__ __attribute__ (( __section__ (".exitcall.exit")))
#define __init_refok  oninline __attribute__ ((__section__ (".text.init.refok")))
#define __initdata_refok __attribute__ ((__section__ (".data.init.refok"))) 
#define __exit_refok noinline __attribute__ ((__section__ (".exit.text.refok")))
......... 
#ifdef MODULE 
#define __exit  __attribute__ (( __section__ (".exit.text"))) __cold 
#else 
#define __exit __attribute_used__ __attribute__ ((__section__ (".exit.text"))) __cold
#endif 
对于经常写驱动模块或翻阅Kernel源代码的人，看到熟悉的宏了吧：__init, __initdata, __exit, __exitdata。 
__init 宏最常用的地方是驱动模块初始化函数的定义处，其目的是将驱动模块的初始化函数放入名叫.init.text的输入段。当内核启动完毕后，这个段中的内存会被释放掉供其他使用。
__initdata宏用于数据定义，目的是将数据放入名叫.init.data的输入段。其它几个宏也类似。
另外需要注意的是，在以上定意中，用__section__代替了section。还有其它一些类似的宏定义，这里不一一列出，其作用都是类似的。
模块加载分为动态加载和静态加载。 
所谓静态加载就是，开机加载系统时将模块加载上去，这就是编译进内核。 
而动态加载就是在开机以后将模块加载上去，这就是编译成模块!
init_module是默认的模块的入口,如果你想指定其他的函数作为模块的入口就需要 
module_init函数来指定,比如 
module_init   (your_func); 
其中your_func是你编写的一个函数的名称.
init_module()是真正的入口，module_init是宏，如果在模块中使用，最终还是要转换到init_module()上。
如果不是在模块中使用，module_init可以说没有什么作用。总之，使用module_init方便代码在模块和非模块间移植。
