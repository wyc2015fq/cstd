# Linux中断处理体系结构分析（一） - h13 - 博客园
    异常，就是可以打断CPU正常运行流程的一些事情，比如外部中断、未定义指令、试图修改只读的数据、执行swi指令（Software Interrupt Instruction ，软件中断指令）等。当这些事情发生时，CPU暂停当前的程序，先处理异常事件，然后再继续执行被中断的程序。操作系统中经常通过异常来完成一些特定的功能。其中的中断也占有很大的一部分。例如下面的这几种情况：
- 当CPU执行未定义的机器指令时将触发“未定义指令异常”，操作系统可以利用这个特点使用一些自定义的机器指令，它们在异常处理函数中实现。 
- 当用户程序试图读写的数据或执行的指令不在内存中时，也会触发一个“数据访问中止异常”或“指令预取中止异常”，在异常处理函数中将这些数据或指令读入内存，然后重新执行被中断的程序，这样可以节省内存，还使得操作系统可以运行这类程序，它们使用的内存远大于实际的物理内存。
  在原先的内核版本中，内核在start_kernel函数（源码在init/main.c中）中调用trap_init、init_IRQ两个函数来设置异常和处理函数。在Linux2.6.32.2的内核版本中（也许在之前的版本就有变化），trap_init函数的内容发生了变化，在trap.c中， 
```
void __init trap_init(void)
{
    return;
}
```
在这个文件中还有一个函数，
```
void __init early_trap_init(void)
{
    unsigned long vectors = CONFIG_VECTORS_BASE;
    extern char __stubs_start[], __stubs_end[];
    extern char __vectors_start[], __vectors_end[];
    extern char __kuser_helper_start[], __kuser_helper_end[];
    int kuser_sz = __kuser_helper_end - __kuser_helper_start;
```
```
memcpy((void *)vectors, __vectors_start, __vectors_end - __vectors_start);
    memcpy((void *)vectors + 0x200, __stubs_start, __stubs_end - __stubs_start);
    memcpy((void *)vectors + 0x1000 - kuser_sz, __kuser_helper_start, kuser_sz);
     memcpy((void *)KERN_SIGRETURN_CODE, sigreturn_codes,
     sizeof(sigreturn_codes));
    memcpy((void *)KERN_RESTART_CODE, syscall_restart_code,
     sizeof(syscall_restart_code));
    flush_icache_range(vectors, vectors + PAGE_SIZE);
    modify_domain(DOMAIN_USER, DOMAIN_CLIENT);
}
```
这个函数才是真正要用到的，在init/mian.c中可以找到，调用了trap_init()，而early_trap_init（）函数在setup_arch(&command_line)函数中调用。在[Linux](http://lxr.free-electrons.com/source/arch/arm/kernel/source/?v=2.6.32)/[arch](http://lxr.free-electrons.com/source/arch/arm/kernel/source/arch/?v=2.6.32)/[arm](http://lxr.free-electrons.com/source/arch/arm/kernel/source/arch/arm/?v=2.6.32)/[kernel](http://lxr.free-electrons.com/source/arch/arm/kernel/source/arch/arm/kernel/?v=2.6.32)/[setup.c](http://lxr.free-electrons.com/source/arch/arm/kernel/source/arch/arm/kernel/setup.c?v=2.6.32)
```
698 void __init setup_arch(char **cmdline_p)
699 {
700 struct tag *tags = (struct tag *)&init_tags;
701 struct machine_desc *mdesc;
702 char *from = default_command_line;
```
```
...........
769 early_trap_init();
770 }
```
   这样我们就明白了trap_init()函数的具体调用过程了。下面我们具体来看一下这个trap_init()函数，确切的说是earl_trap_init()函数。earl_tarp_init函数（代码在arch/arm/kernel/traps.c中）被用来设置各种异常的处理向量，包括中断向量。所谓“向量”，就是一些被安放在固定位置的代码，当发生异常时，CPU会自动执行这些固定位置上的指令。ARM架构的CPU的异常向量基址可以是0x00000000,也可以是0xffff0000,Linux内核使用后者。earl_trap_init函数将异常向量复制到0xffff0000处，我们可以在该函数中看到下面的两行代码。
```
memcpy((void *)vectors, __vectors_start, __vectors_end - __vectors_start);
    memcpy((void *)vectors + 0x200, __stubs_start, __stubs_end - __stubs_start);
```
vectors等于0xffff0000。地址__vectors_start ~ __vectors_end之间的代码就是异常向量，在arch/arm/kernel/entry-armv.S中定义，它们复制到地址0xffff0000处。异常向量的代码很简单，它们只是一些跳转指令。发生异常时，CPU自动执行这些指令，跳转去执行更复杂的代码，比如保存被中断程序的执行环境，调用异常处理函数，恢复被中断程序的执行环境并重新运行。这些“更复杂的代码”在地址__stubs_start ~__stubs_end之间，它们在arch/arm/kernel/entry-armv.S中定义。将它们复制到地址0xffff0000+0x200处。 异常向量、异常向量跳去执行的代码都是使用汇编写的，它们在arch/arm/kernel/entry-armv.S中。异常向量的代码如下，其中的“stubs_offset”用来重新定位跳转的位置（向量被复制到地址0xffff0000处，跳转的目的代码被复制到地址0xffff0000+0x200处）。
```
.equ    stubs_offset, __vectors_start + 0x200 - __stubs_start
    .globl    __vectors_start
__vectors_start:
 ARM(    swi    SYS_ERROR0    ) //复位是，CPU将执行这条指令
 THUMB(    svc    #0        )
 THUMB(    nop            )
    W(b)    vector_und + stubs_offset//未定义异常时，CPU将执行这条指令
    W(ldr)    pc, .LCvswi + stubs_offset//swi异常
    W(b)    vector_pabt + stubs_offset//指令预取中止
    W(b)    vector_dabt + stubs_offset//数据访问中止
    W(b)    vector_addrexcptn + stubs_offset//没有用到
    W(b)    vector_irq + stubs_offset//irq异常
    W(b)    vector_fiq + stubs_offset// fiq 异常
    .globl    __vectors_end
__vectors_end:
```
    其中，vector_und、vector_pabt等表示要跳转去执行的代码。以vector_und为例,它仍在arch/arm/kernel/entry-armv.S中，通过vector_stub宏来定义，
```
vector_stub    und, UND_MODE
    .long    __und_usr      @ 0 (USR_26 / USR_32) 在用户模式执行了未定
义指令
    .long    __und_invalid   @ 1 (FIQ_26 / FIQ_32)在FIQ模式执行了未定义指令
    .long    __und_invalid   @ 2 (IRQ_26 / IRQ_32)在IRQ模式下执行了未定义指令
    .long    __und_svc       @ 3 (SVC_26 / SVC_32)在管理模式下执行了未定义指令
    .long    __und_invalid            @ 4
    .long    __und_invalid            @ 5
    .long    __und_invalid            @ 6
    .long    __und_invalid            @ 7
    .long    __und_invalid            @ 8
    .long    __und_invalid            @ 9
    .long    __und_invalid            @ a
    .long    __und_invalid            @ b
    .long    __und_invalid            @ c
    .long    __und_invalid            @ d
    .long    __und_invalid            @ e
    .long    __und_invalid            @ f
```
    vector_stub是一个宏，它根据后面的参数"und,UND_MODE"定义了以“vector_und”为标号的一段代码。vector_stub宏的功能为：计算处理完异常后的返回地址、保存一引起寄存器（比如r0、lr、spsr），然后进行管理模式，最后根据被中断的工作模式调用下面的某个跳转分支。当发生异常时，CPU会根据异常的类型进入某个工作模式，但是很快vector_stub宏又会强制CPU进行管理模式，在管理模式下进行后续处理，这种方法简化了程序的设计，使得异常发生前的工作模式根毛是用户模式，要么是管理模式。
    代码表示在各个工作模式下执行未定义指令时，发生异常的处理分支。比如__und_usr表示在用户模式下执行未定义指令时，所发生的未定义异常将由它来处理;__und_svc表示在管理模式下执行未定义指令时，所发生的未定义异常将由它来处理。在其他工作模式下不可能发生未定义指令异常，否则使用“__und_invalid”来处理错误。ARM架构CPU中使用4位数据来表示工作模式，所以共有16个跳转分支，目前只有7个工作模式。
     不同的跳转分支只是在它们的入口下（比如保存被中断程序的寄存器）稍有差别，后续的处理大体相同，都在调用相应的C函数，比如未定义指令异常发生时，最终会调用C函数do_undefinstr来进行处理。各种异常C处理函数可以分为5类，它们分布在不同的文件中
(1)在arch/arm/kernel/traps.c中
 未定义指令异常的C处理函数在这个文件中定义，总入口函数为do_undefinstr
(2)在arch/arm/mm/fault.c中
 与内存访问相关的异常C处理函数在这个文件中定义，比如数据访问中止异常、指令预取中止异常。总入口函数为do_DataAbort、do_prefetchAbort。
(3)在arch/arm/mm/irq.c中
 中断处理函数的在这个文件中定义，总入口函数为asm_do_IRQ，它调用其他文件注册的中断处理函数
(4)在arch/arm/kernel/calls.S
在这个文件中，swi异常的处理函数指针被组织成一个表格；swi指令机器码的位[23:0]被用来作为索引。这样，通过不同的swi index指令就可以调用不同的swi异常处理函数，它们被称为系统调用,比如sys_open、sys_read等。
(5)没有使用的异常
没有使用FIQ异常
trap_init函数搭建了各类异常的处理框架。当发生异常时，各种C处理函数会被调用。这些C函数还要进
一步细分异常发生的情况，分别调用更具体的处理函数。
2.init_IRQ函数分析
    中断也是一种异常，之所以把它单独的列出来，是因为中断的处理与具体的开发板密切相关，除一些必须、共用的中断（比如系统时钟中断、片内外设UART中断）外，必须由驱动开发者提供处理函数。内核提炼出中断处理的共性，搭建一个非常容易扩充的中断处理体系。
     init_IRQ函数（代码在arch/arm/kernel/irq.c中）被用来初始化中断和处理框架，设置各种中断的默认处理函数。当发生中断时，中断总入口函数asm_do_IRQ就可以调用这些函数进行下一步处理。
转自：[http://blogold.chinaunix.net/u3/104447/showart_2251010.html](http://blogold.chinaunix.net/u3/104447/showart_2251010.html)

