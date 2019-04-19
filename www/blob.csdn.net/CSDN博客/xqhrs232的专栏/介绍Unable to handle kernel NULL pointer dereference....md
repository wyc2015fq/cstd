# 介绍Unable to handle kernel NULL pointer dereference... - xqhrs232的专栏 - CSDN博客
2010年12月14日 09:43:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1367
原文地址::[http://blog.chinaunix.net/u/32588/showart_307195.html](http://blog.chinaunix.net/u/32588/showart_307195.html)
Oops错误  
我在at91rm9200下写了一个spi的驱动，加载后，运行测试程序时，蹦出这么个吓人的东西：
Unable to handle kernel paging request at virtual address 000e0000              
pgd = c1f9c000                                                                  
[000e0000] *pgd=20315801, *pmd = 20315801, *pte = 00000000, *ppte = 00000000    
Internal error: Oops: 7                                                         
CPU: 0                                                                          
pc : [<c014fd08>]    lr : [<c34a61f4>]    Tainted: P                            
sp : c1fa3f50  ip : 00000001  fp : c1fa3f78                                     
r10: 401421e4  r9 : c1fa2000  r8 : bffffe1c                                     
r7 : 00000000  r6 : ffffffea  r5 : c0282a20  r4 : 00000001                      
r3 : 00000000  r2 : 00000001  r1 : 000e0000  r0 : bffffe1c                      
Flags: Nzcv  IRQs on  FIQs on  Mode SVC_32  Segment user                        
Control: C000317F  Table: 21F9C000  DAC: 00000015                               
Process addrv_test (pid: 73, stack limit = 0xc1fa2374)                          
Stack: (0xc1fa3f50 to 0xc1fa4000)                                               
3f40:                                     00000001 00000001 c0282a20 ffffffea   
3f60: 00000000 c34a61f4 00000001 c1fa3fa4 c1fa3f7c c0044040 c34a6194 c1fa3f88   
3f80: c0043a18 4001d9cc bffffe54 00008330 00000003 c0017644 00000000 c1fa3fa8   
3fa0: c00174a0 c0043f74 4001d9cc c001d5bc 00000003 bffffe1c 00000001 bffffe1c   
3fc0: 4001d9cc bffffe54 00008330 4000c85c 00000001 000084d4 401421e4 bffffe34   
3fe0: 400e40d0 bffffe1c 0000856c 400e40d4 60000010 00000003 00000000 20000040   
Backtrace:                                                                      
Function entered at [<c34a6184>] from [<c0044040>]                              
 r4 = 00000001                                                                  
Function entered at [<c0043f64>] from [<c00174a0>]                              
 r8 = C0017644  r7 = 00000003  r6 = 00008330  r5 = BFFFFE54                     
 r4 = 4001D9CC                                                                  
Code: 0affffe0 e33c0000 0a000009 e35c0002 (e4d13001)                            
Segmentation fault  
网上搜了一把，原来这就是oops。经过我一番定位（printk），原来是我在spi的read函数中引用了一个错误的地址指针（就是本来应该有取地址符&的，我给漏了）。
附oops介绍（网上搜的，感谢作者）：
 Oops Messages 
 Oops 消息 
   Most bugs show themselves in NULL pointer dereferences or by the use of other incorrect pointer values. The usual outcome of such bugs is an oops message. 
   大多数bug通常是因为废弃了一个NULL指针或者使用了错误的指针值。这类bug导致的结果通常是一条oops消息。 
   Almost any address used by the processor is a virtual address and is mapped to physical addresses through a complex structure of page tables (the exceptions are physical addresses used with the memory management subsystem itself). When an invalid pointer is dereferenced, the paging mechanism fails to map the pointer to a physical address, and the processor signals a page fault to the operating system. If the address is not valid, the kernel is not able to "page in" the missing address; it (usually) generates an oops if this happens while the processor is in supervisor mode. 
    处理器使用的所有地址几乎都是通过一个复杂的页表结构对物理地址映射而得到的虚拟地址(除了内存管理子系统自己所使用的物理地址)。当一个非法的指针被废弃时，内存分页机制将不能为指针映射一个物理地址，处理器就会向操作系统发出一个页故障信号。如果地址不合法，那么内核将不能在该地址“布页”；；这时如果处理器处于超级用户模式，内核就会生成一条oops消息。
    An oops displays the processor status at the time of the fault, including the contents of the CPU registers and other seemingly incomprehensible information. The message is generated by printk statements in the fault handler (arch/*/kernel/traps.c) and is dispatched as described earlier in Section 4.2.1). 
    一条oops消息能够显示发生故障时处理器的状态，以及CPU寄存器的内容和其他从表面难以理解的信息。该消息是由容错处理中的printk语句产生的(arch/*kernel/traps.c)并按照4.2.1小节中描述的方式进行分派。
    Let's look at one such message. Here's what results from dereferencing a NULL pointer on a PC running Version 2.6 of the kernel. The most relevant information here is the instruction pointer (EIP), the address of the faulty instruction. 
    下面我们来看一条这样的消息。这是通过在一台运行着2.6内核的PC机上废弃一个NULL指针所引起的。其中最有关的信息是指令指针(EIP)，就是故障指令的地址。 
Unable to handle kernel NULL pointer dereference at virtual address 00000000 printing eip: d083a064 Oops: 0002 [#1] SMP CPU: 0 EIP: 0060:[] Not tainted EFLAGS: 00010246 (2.6.6) EIP is at faulty_write+0x4/0x10 [faulty] eax: 00000000 ebx: 00000000 ecx: 00000000 edx: 00000000 esi: cf8b2460 edi: cf8b2480 ebp: 00000005 esp: c31c5f74 ds: 007b es: 007b ss: 0068 Process bash (pid: 2086, threadinfo=c31c4000 task=cfa0a6c0) Stack: c0150558 cf8b2460 080e9408 00000005 cf8b2480 00000000 cf8b2460 cf8b2460 fffffff7 080e9408 c31c4000 c0150682 cf8b2460 080e9408 00000005 cf8b2480 00000000 00000001 00000005 c0103f8f 00000001 080e9408 00000005 00000005 Call Trace: [] vfs_write+0xb8/0x130 [] sys_write+0x42/0x70 [] syscall_call+0x7/0xb Code: 89 15 00 00 00 00 c3 90 8d 74 26 00 83 ec 0c b8 00 a6 83 d0 
    This message was generated by writing to a device owned by the faulty module, a module built deliberately to demonstrate failures. The implementation of the write method of faulty.c is trivial: 
    这条消息是由一个问题模块向其设备执行写操作时引起的，该模块是特意为示范故障而构建的。faulty.c中的write函数很普通：
ssize_t faulty_write (struct file *filp, const char _ _user *buf, size_t count, loff_t *pos) 
{ 
    /* make a simple fault by dereferencing a NULL pointer */ 
    *(int *)0 = 0; 
    return 0; 
} 
    As you can see, what we do here is dereference a NULL pointer. Since 0 is never a valid pointer value, a fault occurs, which the kernel turns into the oops message shown earlier. The calling process is then killed. 
    如你所见，我们在这里做的就是废弃一个NULL指针。因为0从来都不是一个可用的指针值，所以会引发一个故障，内核会简单地将其转换为oops消息并显示。然后其调用进程会被杀死。 
The faulty module has a different fault condition in its read implementation: faulty
该示例模块在其read函数中则有着不同的故障条件： 
ssize_t faulty_read(struct file *filp, char _ _user *buf, size_t count, loff_t *pos) 
{ 
    int ret; 
    char stack_buf[4]; 
    /* Let's try a buffer overflow */ 
    memset(stack_buf, 0xff, 20); 
    if (count > 4) 
        count = 4; 
    /* copy 4 bytes to the user */ 
    ret = copy_to_user(buf, stack_buf, count); 
    if (!ret) 
        return count; 
    return ret; 
} 
    This method copies a string into a local variable; unfortunately, the string is longer than the destination array. The resulting buffer overflow causes an oops when the function returns. Since the return instruction brings the instruction pointer to nowhere land, this kind of fault is much harder to trace, and you can get something such as the following: 
    该函数将一个字符串赋给一个局部变量；不幸的是，字符串的长度超出了目标数组的范围。当函数返回时就会导致缓冲区溢出而引起一条oops消息。由于返回指令会带来指向空地址的指针，因此这类故障更加难以跟踪，你将会看到下面这样的信息： 
EIP: 0010:[<00000000>] Unable to handle kernel paging request at virtual address ffffffff printing eip: ffffffff Oops: 0000 [#5] SMP CPU: 0 EIP: 0060:[] Not tainted EFLAGS: 00010296 (2.6.6) EIP is at 0xffffffff eax: 0000000c ebx: ffffffff ecx: 00000000 edx: bfffda7c esi: cf434f00 edi: ffffffff ebp: 00002000 esp: c27fff78 ds: 007b es: 007b ss: 0068 Process head (pid: 2331, threadinfo=c27fe000 task=c3226150) Stack: ffffffff bfffda70 00002000 cf434f20 00000001 00000286 cf434f00 fffffff7 bfffda70 c27fe000 c0150612 cf434f00 bfffda70 00002000 cf434f20 00000000 00000003 00002000 c0103f8f 00000003 bfffda70 00002000 00002000 bfffda70 Call Trace: [] sys_read+0x42/0x70 [] syscall_call+0x7/0xb Code: Bad EIP value. 
    In this case, we see only part of the call stack (vfs_read and faulty_read are missing), and the kernel complains about a "bad EIP value." That complaint, and the offending address (ffffffff) listed at the beginning are both hints that the kernel stack has been corrupted. 
    这种情况下，你只能看到部分函数调用的堆栈情况(vfs_read和faulty_read丢失了)，而且内核会为了一个“不可用的EIP值”而抱怨。这种抱怨以及开始部分列出的讨厌地址(ffffffff)都暗示了内核堆栈已经坍塌。 
    In general, when you are confronted with an oops, the first thing to do is to look at the location where the problem happened, which is usually listed separately from the call stack. In the first oops shown above, the relevant line is: 
    通常，当你面临一个oops时，首要问题就是查看故障的发生位置，它通常会与函数调用的堆栈信息分开列出。对于上面第一个oops，与此相关的信息是： 
EIP is at faulty_write+0x4/0x10 [faulty] 
    Here we see that we were in the function faulty_write , which is located in the faulty module (which is listed in square brackets). The hex numbers indicate that the instruction pointer was 4 bytes into the function, which appears to be 10 (hex) bytes long. Often that is enough to figure out what the problem is. 
     这里可以看出我们正位于faulty模块(方括号中的是模块名称)的faulty_write函数中。十六进制的数字指明了该函数中的指令指针长度为4字节，而现在看起来则有10(十六进制)字节长。通常这足以查明问题的所在了。 
     If you need more information, the call stack shows you how you got to where things fell apart. The stack itself is printed in hex form; with a bit of work, you can often determine the values of local variables and function parameters from the stack listing. Experienced kernel developers can benefit from a certain amount of pattern recognition here; for example, if we look at the stack listing from the faulty_read oops: 
    如果你需要更多信息，函数调用的堆栈信息将会告诉你怎样找到已崩溃的东西。堆栈信息会以十六进制列出；稍加分析，你就能从中辨别出局部变量以及函数参数。有经验的内核开发者会从中获得很大的帮助；例如，faulty_read函数的堆栈信息如下所示： 
Stack: ffffffff bfffda70 00002000 cf434f20 00000001 00000286 cf434f00 fffffff7 bfffda70 c27fe000 c0150612 cf434f00 bfffda70 00002000 cf434f20 00000000 00000003 00002000 c0103f8f 00000003 bfffda70 00002000 00002000 bfffda70 
   The ffffffff at the top of the stack is part of our string that broke things. On the x86 architecture, by default, the user-space stack starts just below 0xc0000000; thus, the recurring value 0xbfffda70 is probably a user-space stack address; it is, in fact, the address of the buffer passed to the read system call, replicated each time it is passed down the kernel call chain. On the x86 (again, by default), kernel space starts at 0xc0000000, so values above that are almost certainly kernel-space addresses, and so on. 
   位于堆栈顶部的ffffffff是引发故障的字符串的一部分。在x86体系中，默认用户空间中的堆栈地址是小于0xc00000000的；因此，其中0xbfffda70很有可能是一个用户空间的堆栈地址；实际上它就是传递给read系统调用的缓冲区的地址，它在内核调用链中每次被下传时都会被复制。在x86中(再次说明，缺省的)，内核空间地址起始自0xc00000000，所以可以基本确定凡是大于该值的地址都是属于内核空间的地址。 
   Finally, when looking at oops listings, always be on the lookout for the "slab poisoning" values discussed at the beginning of this chapter. Thus, for example, if you get a kernel oops where the offending address is 0xa5a5a5a5, you are almost certainly forgetting to initialize dynamic memory somewhere. 
   最后要注意的一点是，当你查看oops信息时，始终要留意本章开始时讨论的“slab poisoning”的值。因此，如果一条内核oops中出现了讨厌的地址值0xa5a5a5a5，那么你肯定是在什么地方忘记初始化动态分配的内存了。 
   Please note that you see a symbolic call stack (as shown above) only if your kernel is built with the CONFIG_KALLSYMS option turned on. Otherwise, you see a bare, hexadecimal listing, which is far less useful until you have decoded it in other ways. 
   请注意要想看到一条可读的调用堆栈信息(如上所示)，你必须要在构建内核时启用CONFIG_KALLSYMS选项。否则你将会看到一个原始的十六进制列表，在你使用其他方法译解它之前它几乎没什么用处。 
