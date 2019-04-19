# LINUX内核中的xx_initcall初始化标号 - maopig的专栏 - CSDN博客
2012年01月02日 22:29:03[maopig](https://me.csdn.net/maopig)阅读数：1983标签：[linux内核																[basic																[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=basic&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)
**LINUX内核中的xx_initcall初始化标号**
**[田海立@CSDN](http://blog.csdn.net/thl789/) 2011-07-02**
          LINUX内核中有很多的初始化指示标志postcore_initcall(), arch_initcall(), subsys_initcall(), device_initcall(), etc. 这些起什么作用呢？查阅源代码（android goldfish-2.6.29）并搜索网上相关文章，对此做一总结。
- **初始化标号**
先看这些宏的定义（定义在文件include/linux/init.h中）
[view plain](http://blog.csdn.net/thl789/article/details/6581146#)[copy to clipboard](http://blog.csdn.net/thl789/article/details/6581146#)[print](http://blog.csdn.net/thl789/article/details/6581146#)[?](http://blog.csdn.net/thl789/article/details/6581146#)
- #define pure_initcall(fn)               __define_initcall("0",fn,0)
- #define core_initcall(fn)               __define_initcall("1",fn,1)
- #define core_initcall_sync(fn)          __define_initcall("1s",fn,1s)
- #define postcore_initcall(fn)           __define_initcall("2",fn,2)
- #define postcore_initcall_sync(fn)      __define_initcall("2s",fn,2s)
- #define arch_initcall(fn)               __define_initcall("3",fn,3)
- #define arch_initcall_sync(fn)          __define_initcall("3s",fn,3s)
- #define subsys_initcall(fn)             __define_initcall("4",fn,4)
- #define subsys_initcall_sync(fn)        __define_initcall("4s",fn,4s)
- #define fs_initcall(fn)                 __define_initcall("5",fn,5)
- #define fs_initcall_sync(fn)            __define_initcall("5s",fn,5s)
- #define rootfs_initcall(fn)             __define_initcall("rootfs",fn,rootfs)
- #define device_initcall(fn)             __define_initcall("6",fn,6)
- #define device_initcall_sync(fn)        __define_initcall("6s",fn,6s)
- #define late_initcall(fn)               __define_initcall("7",fn,7)
- #define late_initcall_sync(fn)          __define_initcall("7s",fn,7s)
#define pure_initcall(fn)               __define_initcall("0",fn,0)
#define core_initcall(fn)               __define_initcall("1",fn,1)
#define core_initcall_sync(fn)          __define_initcall("1s",fn,1s)
#define postcore_initcall(fn)           __define_initcall("2",fn,2)
#define postcore_initcall_sync(fn)      __define_initcall("2s",fn,2s)
#define arch_initcall(fn)               __define_initcall("3",fn,3)
#define arch_initcall_sync(fn)          __define_initcall("3s",fn,3s)
#define subsys_initcall(fn)             __define_initcall("4",fn,4)
#define subsys_initcall_sync(fn)        __define_initcall("4s",fn,4s)
#define fs_initcall(fn)                 __define_initcall("5",fn,5)
#define fs_initcall_sync(fn)            __define_initcall("5s",fn,5s)
#define rootfs_initcall(fn)             __define_initcall("rootfs",fn,rootfs)
#define device_initcall(fn)             __define_initcall("6",fn,6)
#define device_initcall_sync(fn)        __define_initcall("6s",fn,6s)
#define late_initcall(fn)               __define_initcall("7",fn,7)
#define late_initcall_sync(fn)          __define_initcall("7s",fn,7s)
- **__define_initcall**
这些宏都用到了__define_initcall()，再看看它的定义（同样定义在文件include/linux/init.h中）
[view plain](http://blog.csdn.net/thl789/article/details/6581146#)[copy to clipboard](http://blog.csdn.net/thl789/article/details/6581146#)[print](http://blog.csdn.net/thl789/article/details/6581146#)[?](http://blog.csdn.net/thl789/article/details/6581146#)
- #define __define_initcall(level,fn,id) \
- static initcall_t __initcall_##fn##id __used \  
-         __attribute__((__section__(".initcall" level ".init"))) = fn  
#define __define_initcall(level,fn,id) \
        static initcall_t __initcall_##fn##id __used \
        __attribute__((__section__(".initcall" level ".init"))) = fn
这其中initcall_t是函数指针，原型如下，
[view plain](http://blog.csdn.net/thl789/article/details/6581146#)[copy to clipboard](http://blog.csdn.net/thl789/article/details/6581146#)[print](http://blog.csdn.net/thl789/article/details/6581146#)[?](http://blog.csdn.net/thl789/article/details/6581146#)
- typedefint (*initcall_t)(void);  
typedef int (*initcall_t)(void);
而属性 __attribute__((__section__())) 则表示把对象放在一个这个由括号中的名称所指代的section中。
所以__define_initcall的含义是：
1) 声明一个名称为__initcall_##fn的函数指针；
2) 将这个函数指针初始化为fn；
3) 编译的时候需要把这个函数指针变量放置到名称为 ".initcall" level ".init"的section中。
**3.  放置.initcall.init SECTION**
明确了__define_initcall的含义，就知道了是分别将这些初始化标号修饰的函数指针放到各自的section中的。
SECTION“.initcall”level”.init”被放入INITCALLS（include/asm-generic/vmlinux.lds.h）
[view plain](http://blog.csdn.net/thl789/article/details/6581146#)[copy to clipboard](http://blog.csdn.net/thl789/article/details/6581146#)[print](http://blog.csdn.net/thl789/article/details/6581146#)[?](http://blog.csdn.net/thl789/article/details/6581146#)
- #define INITCALLS                                                   \
-             *(.initcallearly.init)                                  \  
-             VMLINUX_SYMBOL(__early_initcall_end) = .;               \  
-             *(.initcall0.init)                                      \  
-             *(.initcall0s.init)                                     \  
-             *(.initcall1.init)                                      \  
-             *(.initcall1s.init)                                     \  
-             *(.initcall2.init)                                      \  
-             *(.initcall2s.init)                                     \  
-             *(.initcall3.init)                                      \  
-             *(.initcall3s.init)                                     \  
-             *(.initcall4.init)                                      \  
-             *(.initcall4s.init)                                     \  
-             *(.initcall5.init)                                      \  
-             *(.initcall5s.init)                                     \  
-             *(.initcallrootfs.init)                                 \  
-             *(.initcall6.init)                                      \  
-             *(.initcall6s.init)                                     \  
-             *(.initcall7.init)                                      \  
-             *(.initcall7s.init)  
#define INITCALLS                                                   \
            *(.initcallearly.init)                                  \
            VMLINUX_SYMBOL(__early_initcall_end) = .;               \
            *(.initcall0.init)                                      \
            *(.initcall0s.init)                                     \
            *(.initcall1.init)                                      \
            *(.initcall1s.init)                                     \
            *(.initcall2.init)                                      \
            *(.initcall2s.init)                                     \
            *(.initcall3.init)                                      \
            *(.initcall3s.init)                                     \
            *(.initcall4.init)                                      \
            *(.initcall4s.init)                                     \
            *(.initcall5.init)                                      \
            *(.initcall5s.init)                                     \
            *(.initcallrootfs.init)                                 \
            *(.initcall6.init)                                      \
            *(.initcall6s.init)                                     \
            *(.initcall7.init)                                      \
            *(.initcall7s.init)
__initcall_start和__initcall_end以及INITCALLS中定义的SECTION都是在arch/xxx/kernel/vmlinux.lds.S中放在.init段的。
[view plain](http://blog.csdn.net/thl789/article/details/6581146#)[copy to clipboard](http://blog.csdn.net/thl789/article/details/6581146#)[print](http://blog.csdn.net/thl789/article/details/6581146#)[?](http://blog.csdn.net/thl789/article/details/6581146#)
- SECTIONS  
- {  
-         .init : {  
-                 __initcall_start = .;  
-                         INITCALLS  
-                 __initcall_end = .;  
-         }  
- }  
SECTIONS
{
        .init : {
                __initcall_start = .;
                        INITCALLS
                __initcall_end = .;
        }
}
**4.   初始化.initcallxx.init里的函数**
而这些SECTION里的函数在初始化时被顺序执行（init内核线程->do_basic_setup()[main.c#778]->do_initcalls()）。
程序（init/main.c文件do_initcalls()函数）如下，do_initcalls()把.initcallXX.init中的函数按顺序都执行一遍。
[view plain](http://blog.csdn.net/thl789/article/details/6581146#)[copy to clipboard](http://blog.csdn.net/thl789/article/details/6581146#)[print](http://blog.csdn.net/thl789/article/details/6581146#)[?](http://blog.csdn.net/thl789/article/details/6581146#)
- for (call = __early_initcall_end; call < __initcall_end; call++)  
-         do_one_initcall(*call);  
        for (call = __early_initcall_end; call < __initcall_end; call++)
                do_one_initcall(*call);
*************************** **本文完** *****************************
