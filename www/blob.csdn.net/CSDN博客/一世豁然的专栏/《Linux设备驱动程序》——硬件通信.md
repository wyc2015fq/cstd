# 《Linux设备驱动程序》——硬件通信 - 一世豁然的专栏 - CSDN博客





2014年11月18日 20:09:11[一世豁然](https://me.csdn.net/Explorer_day)阅读数：935








一、I/O端口和I/O内存

 一）、相关介绍

  1、每种外设都通过读写寄存器进行控制。大部分外设都有几个寄存器，不管是在内存地址空间还是在I/O地址空间，这些寄存器的访问都是连续的。




  2、在硬件层，内存区域和I/O区域没有概念上的区别：它们都是通过向地址总线和控制总线发送电平信号进行访问，再通过数据总线读写数据。




  3、Linux在所有的计算机上都实现了I/O端口，包括使用单一的地址空间的CPU在内。端口操作的具体实现有时依赖于宿主计算机的特定型号和构造。







 二）、I/O寄存器和常规内存

  1、尽管硬件寄存器和内存非常相似，但程序员在访问I/O寄存器的时候必须注意避免由于CPU或编译器不恰当的优化而改变预期的I/O动作。




  2、I/O寄存器和RAM的最主要区别就是I/O操作具有边际效应，而内存操作则没有。




  3、在对常规内存进行优化的时候，优化过程是透明的，而且效果良好。但对I/O操作来说这些优化很可能造成致命错误，这是因为它们受到边际效应的干扰，而这却是驱动程

        序访问I/O寄存器的主要目的。




  4、由硬件自身缓存引起的问题很好解决：只要把底层硬件配置成在访问I/O区域时禁止硬件缓存即可。




  5、由编译器优化和硬件重新排序引起的解决方法是，对硬件必须以特定顺序执行的操作之间设置内存屏障。Linux提供了四个宏：

       I、 #include<linux/kernel.h>

             void  barrier(void);

      II、  #include<sam/system.h>

              void  rmb(void);

              void  read_barrier_depends(void);

              void  wmb(void);

              void  mb(void);

      III、void  smp_rmb(void);

             void  smp_read_barrier_depends(void);

              void  smp_wmb(void);

              void  smp_mb(void);




  6、内存屏障会影响系统性能，所以应该只用于真正需要用到的地方。不同类型的内存屏障对性能的影响也不尽相同，所以最好尽可能使用符合需要的特定类型。值得注意的

        是，大多数处理同步的内核原语，也能作为内核屏障使用。




  7、在某些架构上，允许把赋值语句和内存屏障进行合并以提高效率。内核提供了几个执行这种合并的宏，在默认情况下，这些宏定义如下：

        #define  set_mb(var,  value)    do  {var  =  value;  mb()}   while  0


        #define  set_wmb(var,  value)    do  {var  =  value;  wmb()}   while  0


        #define  set_rmb(var,  value)    do  {var  =  value;  rmb()}   while  0











二、使用I/O端口

 一）、I/O端口分配

  1、内核为我们提供了一个注册yonder接口，它允许驱动程序声明自己需要操作的端口。该接口的核心函数是：

        #include<linux/ioport.h>

        struct  resource  *request_region(unsigned  long  first,  unsigned  long  n,  const  char  *name);




  2、所有的端口分配可从/proc/ioports中得到。如果我们无法分配到需要的端口集合，则可以通过这个/proc文件得知哪个驱动程序已经分配了这些端口。




  3、如果不再需要某组I/O端口，则应该使用下面的函数将这些端口返回给系统：

        void  release_region(unsigned  long  start,  unsigned  long  n);




  4、下面函数允许驱动程序检查给定的I/O端口集是否可用：

         int  check_reion(unsigned  long  first,  unsigned  long  n);







 二）、操作I/O端口

  1、当驱动程序请求了需要使用的I/O端口范围后，必须读取和/或写入这些端口。为此，大多数硬件会把8位，16位，32位的端口区分开来。




  2、Linux内核头文件中定义了如下一些访问I/O端口的内嵌函数

     I、unsigned  inb(unsigned  port);

          void  outb(unsigned  char  byte,  unsigned  port);

     II、unsigned  inw(unsigned  port);

           void  outw(unsigned  short  worf,  unsigned  port);

     III、unsigned  inl(unsigned  port);

            void  outl(unsigned  longword,  unsigned  port);







 三）、在用户空间访问I/O端口

  1、如果要在用户空间代码中使用inb及其相关函数，则必须满足下列条件：

    1）、编译该程序时必须带-O选项来强制内联函数的展开。

    2）、必须使用ioperm或iopl系统调用来获取对端口进行I/O操作的权限。

    3）、必须以root身份运行该程序才能调用ioperm或iopl系统调用。或者，进程的祖先进程之一已经以root身份获取对端口的访问。




  2、如果宿主平台没有ioperm或iopl系统调用，则用户空间程序仍然可以使用/dev/port设备文件访问I/O端口。







 四）、串操作

  1、某些处理器上实现了一次传输一个数据序列的特殊指令，序列中的数据单位可以是字节、子、双字。这些指令称为串操作指令。




  2、串I/O函数的原型如下：

     I、void  insb(unsigned  port,  void  *addr,  unsigned  .long  count);

          void  outsb(unsigned  port,  void  *addr,  unsigned  .long  count);

     II、void  insw(unsigned  port,  void  *addr,  unsigned  .long  count);

          void  outsw(unsigned  port,  void  *addr,  unsigned  .long  count);

     III、void  insl(unsigned  port,  void  *addr,  unsigned  .long  count);

          void  outsl(unsigned  port,  void  *addr,  unsigned  .long  count);







 五）、暂停式I/O

  1、如果有设备丢失数据的情况，或为了防止出现丢失数据的情况，可以使用暂停式的I/O函数来取代通常的I/O函数。暂停式I/O函数名字以_p结尾。




  2、在LInux支持的大多数平台上都定义了这些函数，不过他们常常扩展为何非暂停式I/O同样的代码。







 六）、平台相关性

  1、内核支持的体系结构可以使用的函数总结。（P243-P244）







三、I/O端口示例（略）










四、使用I/O内存

 一）、相关简介

  1、除了X86上普遍使用的I/O端口之外，和设备通信的另一种主要机制是通过使用映射到内存的寄存器或设备内存。这两种都称为I/O内存，因为寄存器和内存的差别对软件是

        透明的。




  2、访问I/O内存的方法和计算机体系架构、总线及正在使用的设备有关，不过原理都是一样的。




  3、根据计算机平台和所使用总线的不同，I/O内存可能是、也可能不是通过页表访问的。

    1）、如果访问是通过页表进行的，内核必须首先安排物理地址使其对设备驱动程序可见。

    2）、如果访问无需列表，那么I/O内存区域就非常类似于I/O端口，可以使用适当形式的函数读写它们。




  4、不管访问I/O内存时是否需要调用ioremap，都不鼓励直接使用指向I/O内存的指针。相反，使用包装函数访问I/O内存，这一方面在所有平台上都是安全的。另一方面，在可

        以直接对指针指向的内存区域执行操作的时候，这些函数是经过优化。







 二）、I/O内存分配和映射

  1、分配I/O内存区域

        struct  resource  *request_mem_region(unsigned  long  start,  unsigned  long  len,  char  *name);




  2、释放已分配的内存区域

        void  release_mem_reion(unsigned  long  start,  unsigned  long  len);




  3、检查给定的I/O内存区域是否可用的老函数：

        int  check_mem_region(unsigned  long  start,  unsigned  long  len);




  4、建立映射。映射的建立由ioremap函数完成。

    1）、一旦调用ioremap（以及iounmap）之后，设备驱动程序即可访问任意的I/O内存地址，而无论I/O内存地址是否直接映射到虚拟地址空间。

    2）、可以使用以下的定义来调用ioremap函数：

        #include<asm/io.h>


        void  *ioremap(unsigned  long  phys_addr,  unsigned  long  size);

        void  *ioremap_nocache(unsigned  long  phys_addr,  unsigned  long  size);

        void  iounmap(void  * addr);







 三）、访问I/O内存

  1、从I/O内存中读取，可使用下面函数之一：

       unsigned  int  ioread8(void  *addr);


       unsigned  int  ioread16(void  *addr);


       unsigned  int  ioread32(void  *addr);




  2、写入I/O内存函数集：

      void  iowrite8(u8  value,  void  *addr);

      void  iowrite16(u16  value,  void  *addr);

      void  iowrite32(u32  value,  void  *addr);




  3、如果必须从给定的I/O内存处读/写一系列的值，则可使用上述函数的重复版本：

       void  ioread8_rep(void  *addr,  void  buf,  unsigned  long  count);

       void  ioread16_rep(void  *addr,  void  buf,  unsigned  long  count);

       void  ioread32_rep(void  *addr,  void  buf,  unsigned  long  count);

       void  iowrite8_rep(void  *addr,  const  void  *buf,  unsigned  long  count);

       void  iowrite16_rep(void  *addr,  const  void  *buf,  unsigned  long  count);

       void  iowrite32_rep(void  *addr,  const  void  *buf,  unsigned  long  count);




  4、如果我们要在一块I/O内存上执行操作，则可以使用下面的函数之一：

       void  memset_io(void  *addr,  u8  value,  unsigned  int  count);

       void  memcpy_fromio(void  *dest,  void  *source,  unsigned  int  count);

       void  memcpy_toio(void  *dest,  void  *source,  unsigned  int  count);




  5、旧的I/O内存函数（不推荐使用）

       unsigned  readb(address);

       unsigned  readw(address);

       unsigned  readl(address);

       这些宏用来从I/O内存检索8位、16位、32位的数据。

       void  writeb(unsigned  value,  address);

       void  writew(unsigned  value,  address);

       void  writel(unsigned  value,  address);







 四）、像I/O内存一样使用端口

  1、某些硬件具有一种有趣的特性：某些版本使用I/O端口，而其他版本则使用I/O内存。为了处理这类硬件的驱动程序更加容易编写，也为了最小化I/O端口和内存访问之间的表

        面区别，可调用下面函数：

       void  *ioport_map(unsigned  long  port,  unsigned  int  count);



  2、当不再需要这种映射时，需要调用下面的函数来撤销：

       void  ioport_unmap(void  *addr);







 五）、为I/O内存重用short




 六）、1MB地址空间之下的ISA内存




 七）、isa_readb及相关函数




