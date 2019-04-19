# ARM920T的MMU与Cache ——转载 - xqhrs232的专栏 - CSDN博客
2014年04月08日 16:21:34[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：639
原文地址::[http://blog.csdn.net/gooogleman/article/details/3593434](http://blog.csdn.net/gooogleman/article/details/3593434)
相关文章
1、[ARM MMU作用和cache](http://blog.csdn.net/winmenaruto/article/details/6698715)----[http://blog.csdn.net/winmenaruto/article/details/6698715](http://blog.csdn.net/winmenaruto/article/details/6698715)
2、[ARM920T及其MMU,Cache学习杂记(一)](http://blog.chinaunix.net/uid-7627289-id-2047807.html) ----[http://blog.chinaunix.net/uid-7627289-id-2047807.html](http://blog.chinaunix.net/uid-7627289-id-2047807.html)
3、[ARM920T及其MMU,Cache学习杂记(二)](http://blog.chinaunix.net/uid-7627289-id-2047808.html) ----[http://blog.chinaunix.net/uid-7627289-id-2047808.html](http://blog.chinaunix.net/uid-7627289-id-2047808.html)
4、[ARM协处理器CP15（设置MMU，cache等）学习](http://blog.csdn.net/gooogleman/article/details/3595294)----[http://blog.csdn.net/gooogleman/article/details/3595294](http://blog.csdn.net/gooogleman/article/details/3595294)
5、mmu和cache详解----[http://download.csdn.net/download/fla888/3046524](http://download.csdn.net/download/fla888/3046524)
Cache是高性能CPU解决总线访问速度瓶颈的方法，然而它的使用却是需要权衡的，因为缓存本身的动作，如块拷贝和替换等，也是很消耗CPU时间的。MMU的重要性勿庸置疑，ARM920T（和ARM720T）集成了MMU是其最大的卖点；有了MMU，高级的操作系统（虚拟地址空间，平面地址，进程保护等）才得以实现。**二者都挺复杂，并且在920T中又高度耦合，相互配合操作，所以需要结合起来研究。同时，二者的操作对象都是内存，内存的使用是使用MMU/Cache的关键。另外，MMU和Cache的控制寄存器不占用地址空间，CP15是操纵MMU/Cache的唯一途径。**
Cache/Write Buffer的功能
Cache通过预测CPU即将要访问的内存地址（一般都是顺序的），预先读取大块内存供CPU访问，来减少后续的内存总线上的读写操作，以提高速度。然而，如果程序中长跳转的次数很多，Cache的命中率就会显著降低，随之而来，大量的替换操作发生，于是，过多的内存操作反而降低了程序的性能。
ARM920T内部采用哈佛结构，将内部指令总线和数据总线分开，分别连接到ICache和DCache，再通过AMBA（高级微控制器总线结构）总线接口连接到ASB总线上去访问内存。Cache由Line组成，Line是Cache进行块读取和替换的单位。
Writer Buffer是和DCache相逆过程的一块硬件，目的也是通过减少memory bus的访问来提高性能。
MMU的功能
在内存中维护一张或几张表，就看你怎么给内存划分page和section了。通过CP15指定好转换表的位置，920T的硬件会自动将转换表的一部分读到TLB中。CPU每次进行内存读写时，发出虚拟地址，参照TLB中的转换表转换到物理地址，并读取相应entry中的信息，以决定是否可以有权限读写和缓存。
mmugen这个工具就是帮你构造这个表的，省的自己写程序了。
操作MMU，实际上就是如何分配和使用你的内存，并记录在translationtable里。
ARM920T中，MMU的每条entry包括Cachable和Buffable位来指定相应的内存是否可以用Cache缓存。此处就是MMU与Cache的交互作用处。
实际上，MMU和Cache的使用是操作系统设计者根据系统软硬件配置而考虑的事情。操作系统针对分配给应用程序的地址空间作内存保护和缓存优化。在没有操作系统的情况下，就需要我们自己来掌控它们了。其中，主要是合理分配内存。
我认为，以下几点需要着重考虑：
1) 安全第一！ -- 避免MMU和Cache的副作用。
当你在无OS的裸机上开发程序时，初始化运行环境的代码很重要，比如：各种模式堆栈指针的初始化；将代码和RW data从ROM拷贝到RAM；初始化.bss段（zero initialized）空间等。此时会有大量的内存操作，如果你enable了Cache，那么在拷贝完代码之后，一定要invalidate ICache和flush DCache。否则将会出现缓存中的代码或数据与内存中的不一致，程序跑飞。
另外，有时候我们需要自己作loader来直接运行ELF文件，情况也是一样，拷贝完代码后一定要刷新Cache，以免不测。
还有，对硬件的操作要小心。很多寄存器值都是被硬件改变的，读写时，要保证确实访问到它的地址。首先，在C语言代码中声明为volatile变量，以防止内存读写被编译器优化掉；另外，设置好TLB，使得寄存器映射的地址空间不被缓存。
总之，缓存和内存中代码的不一致，是一定要避免的。
2) 弄巧成拙！ -- 只对频繁访问的地址空间进行Cache优化。
我们很清楚自己的程序中，那里有大量的运算，哪里有无数的循环或递归，而这正是Cache的用武之地，我们将这些空间进行缓存将大大提高运行速度。但是，很多函数或子程序往往仅仅运行很少几次，若是对它们也缓存，只会捡了芝麻丢了西瓜，造成不必要的缓存和替换操作，反而增加了系统负担，降低了整体性能。
3) 断点哪儿去了？ -- 如何调试“加速”了的代码？
据我所知，一般，debugger都是通过扫描地址总线，在断点处暂停CPU。ARM9TDMI中集成的JTAG调试口，也是这样。
当我们调试使用Cache的代码时，将会出现问题。比如：CPU访问某断点所在地址之前的地址时，发生缓存操作，断点处代码被提前读入Cache，此时地址总线上出现了断点地址，CPU被debugger暂停，并且断点之后的指令也被Cache缓存。于是，当你从断点处step时，程序却停不了了，因为地址总线上不再出现断点之后的下一个地址了。
再举个例子：
int i,a;
for (i=0; i<100; i++) {
-> a++; /* set breakpoints */
}
当地址总线上第一次出现断点地址时，CPU暂停；之后，就再也不会停了。因为，之后CPU会从cache中直接去代码了。(当然，后来，Cache的代码有可能会被替换掉，断点又可到达。) 所幸的是，我用的debugger提供JTAG Monitor，允许断点跟踪使用cache的程序。
全部参考来源于ARM的手册，建议详细阅读1)的PartB及其在2)中的具体实现。
1) DDI0100E_ARM_ARM.pdf (ARM Architecture Reference Manual)
2) DDI0151C_920T_TRM.pdf (ARM920T Technical Reference Manual)
本文出自 “[交换机技术](http://21cnbao.blog.51cto.com/)” 博客，请务必保留此出处[http://21cnbao.blog.51cto.com/109393/120078](http://21cnbao.blog.51cto.com/109393/120078)
