# 蜕变成蝶：Linux设备驱动之CPU与内存和I/O - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [李辉](http://www.jobbole.com/members/harrylhmj) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
由于Linux系统提供了复杂的内存管理功能，本节将讲解的是内存和I/O的访问编程。
在X86中，I/O空间是相对于内存空间而言的，通过特定的in、out来访问，in、out指令格式如下：

C
```
IN 累加器，{端口号|DX}
OUT {端口号|DX}，累加器
```
下面说说MMU（内存管理单元），操作系统借助MMU可以让用户感觉到好像程序可以使用非常大的内核空间，实际上就是我们平时了解的虚拟地址一样的。为了好好了解一下MMU，先看两个概念
TLB：MMU的核心部件，缓存少量的虚拟地址与物理地址的转换关系，是转换表的Cache
TTW：当TLB中没有缓冲对应的地址转换关系时候，需要通过对内存中转换表的访问来获取虚拟地址和物理地址的对应关系，TTW成功后，结果应该写入TLB。
为了说明MMU在内存中使用的关系，下图可以说明如下关系
![](http://jbcdn2.b0.upaiyun.com/2016/04/9324cb3bc56ac61937316153dc8682e9.jpg)
对于提供了 MMU（存储管理器，辅助操作系统进行内存管理，提供虚实地址转换等硬件支持）的处理器而言，Linux 提供了复杂的存储管理系统，使得进程所能访问的内存达到4GB。进程的 4GB 内存空间被人为的分为两个部分——用户空间与内核空间。用户空间地址分布从0 到3GB(PAGE_OFFSET，在0x86 中它等于0xC0000000)，3GB 到4GB 为内核空间，
内核空间中，从3G 到vmalloc_start 这段地址是物理内存映射区域（该区域中包含了内核镜像、物理页框表mem_map 等等）kmalloc 和get_free_page 申请的内存位于物理内存映射区域，而且在物理上也是连续的，它们与真实的物理地址只有一个固定的偏移，因此存在较简单的转换关系，virt_to_phys()可以实现内核虚拟地址转化为物理地址：

C
```
#define __pa(x) ((unsigned long)(x)-PAGE_OFFSET)
extern inline unsigned long virt_to_phys(volatile void * address)
{
　return __pa(address);
}
```
上面转换过程是将虚拟地址减去3G（PAGE_OFFSET=0XC000000）。与之对应的函数为phys_to_virt()，将内核物理地址转化为虚拟地址：

C
```
#define __va(x) ((void *)((unsigned long)(x)+PAGE_OFFSET))
extern inline void * phys_to_virt(unsigned long address)
{
　return __va(address);
}
```
virt_to_phys()和phys_to_virt()都定义在includeasm-i386io.h中。而vmalloc申请的内存则位于vmalloc_start～vmalloc_end之间，与物理地址没有简单的转换关系，虽然在逻辑上它们也是连续的，但是在物理上它们不要求连续。我们用下面的程序来演示kmalloc、get_free_page和vmalloc的区别：

C
```
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
MODULE_LICENSE("GPL");
unsigned char *pagemem;
unsigned char *kmallocmem;
unsigned char *vmallocmem;
 
int __init mem_module_init(void)
{
　//最好每次内存申请都检查申请是否成功
　//下面这段仅仅作为演示的代码没有检查
　pagemem = (unsigned char*)get_free_page(0);
　printk("<1>pagemem addr=%x", pagemem);
 
　kmallocmem = (unsigned char*)kmalloc(100, 0);
　printk("<1>kmallocmem addr=%x", kmallocmem);
 
　vmallocmem = (unsigned char*)vmalloc(1000000);
　printk("<1>vmallocmem addr=%x", vmallocmem);
 
　return 0;
}
 
void __exit mem_module_exit(void)
{
　free_page(pagemem);
　kfree(kmallocmem);
　vfree(vmallocmem);
}
 
module_init(mem_module_init);
module_exit(mem_module_exit);
```
设备通常会提供一组寄存器来用于控制设备、读写设备、获取设备状态，那么Linux设备驱动究竟怎样访问设备的I/O端口（寄存器）和I/O内存的访问的呢
1.操作I/O口
（1）申请I/O 端口：
在驱动还没独占设备之前，不应对端口进行操作。内核提供了一个注册接口，以允许驱动声明其需要的端口：


```
/* request_region告诉内核：要使用first开始的n个端口。参数name为设备名。
如果分配成功返回值是非NULL；否则无法使用需要的端口(/proc/ioports包含了系统当前所有端口的分配信息，
若request_region分配失败时，可以查看该文件，看谁先用了你要的端口) */
struct resource *request_region(unsigned long first, unsigned long n, const char *name);
```
（2）访问IO端口：
在驱动成功请求到I/O 端口后，就可以读写这些端口了。大部分硬件会将8位、16位和32位端口区分开，无法像访问内存那样混淆使用。驱动程序必须调用不同的函数来访问不同大小的端口。
Linux 内核头文件(体系依赖的头文件<asm/io.h>) 定义了下列内联函数来存取I/O端口:


```
/* inb/outb:读/写字节端口(8位宽)。有些体系将port参数定义为unsigned long；
而有些平台则将它定义为unsigned short。inb的返回类型也是依赖体系的 */
unsigned inb(unsigned port);
void outb(unsigned char byte, unsigned port);
 
/* inw/outw:读/写字端口(16位宽） */
unsigned inw(unsigned port);
void outw(unsigned short word, unsigned port);
 
/* inl/outl:读/写32位端口。longword也是依赖体系的，有的体系为unsigned long；而有的为unsigned int */
unsigned inl(unsigned port);
void outl(unsigned longword, unsigned port);
```
（3）释放IO端口：

C
```
/* 用完I/O端口后(可能在模块卸载时)，应当调用release_region将I/O端口返还给系统。参数start和n应与之前传递给request_region一致 */
void release_region(unsigned long start, unsigned long n);
```
2 操作IO内存
（1）申请I/O 内存：
I/O 内存区在使用前必须先分配。分配内存区的函数接口在<linux/ioport.h>定义中：


```
/* request_mem_region分配一个开始于start,len字节的I/O内存区。分配成功，返回一个非NULL指针；否则返回NULL。
系统当前所有I/O内存分配信息都在/proc/iomem文件中列出，你分配失败时，可以看看该文件，看谁先占用了该内存区 */
struct resource *request_mem_region(unsigned long start, unsigned long len, char *name);
```
（2）映射：
在访问I/O内存之前，分配I/O内存并不是唯一要求的步骤，你还必须保证内核可存取该I/O内存。访问I/O内存并不只是简单解引用指针，在许多体系中，I/O 内存无法以这种方式直接存取。因此，还必须通过ioremap 函数设置一个映射。


```
/* ioremap用于将I/O内存区映射到虚拟地址。
参数phys_addr为要映射的I/O内存起始地址，
参数size为要映射的I/O内存的大小，返回值为被映射到的虚拟地址 */
void *ioremap(unsigned long phys_addr, unsigned long size);
```
（3）访问IO内存：
经过 ioremap之后，设备驱动就可以存取任何I/O内存地址。注意，ioremap返回的地址不可以直接解引用；相反，应当使用内核提供的访问函数。访问I/O内存的正确方式是通过一系列专门用于实现此目的的函数：

C
```
#include <asm/io.h>
/* I/O内存读函数。参数addr应当是从ioremap获得的地址(可能包含一个整型偏移); 返回值是从给定I/O内存读取到的值 */
unsigned int ioread8(void *addr);
unsigned int ioread16(void *addr);
unsigned int ioread32(void *addr);
 
/* I/O内存写函数。参数addr同I/O内存读函数，参数value为要写的值 */
void iowrite8(u8 value, void *addr);
void iowrite16(u16 value, void *addr);
void iowrite32(u32 value, void *addr);
 
/* 以下这些函数读和写一系列值到一个给定的 I/O 内存地址,从给定的buf读或写count个值到给定的addr。参数count表示要读写的数据个数，而不是字节大小 */
void ioread8_rep(void *addr, void *buf, unsigned long count);
void ioread16_rep(void *addr, void *buf, unsigned long count);
void ioread32_rep(void *addr, void *buf, unsigned long count);
void iowrite8_rep(void *addr, const void *buf, unsigned long count);
void iowrite16_rep(void *addr, const void *buf, unsigned long count);
void iowrite32_rep(void *addr,，onst void *buf,，nsigned long count);
 
/* 需要操作一块I/O 地址时，使用下列函数(这些函数的行为类似于它们的C库类似函数): */
void memset_io(void *addr, u8 value, unsigned int count);
void memcpy_fromio(void *dest, void *source, unsigned int count);
void memcpy_toio(void *dest, void *source, unsigned int count);
 
/* 旧的I/O内存读写函数，不推荐使用 */
unsigned readb(address);
unsigned readw(address);
unsigned readl(address);
void writeb(unsigned value, address);
void writew(unsigned value, address);
void writel(unsigned value, address);
```
（4）释放IO内存步骤：

C
```
void iounmap(void * addr); /* iounmap用于释放不再需要的映射 */
void release_mem_region(unsigned long start, unsigned long len); /* iounmap用于释放不再需要的映射 */
```
几乎每一种外设都是通过读写设备上的寄存器来进行的，通常包括控制寄存器、状态寄存器和数据寄存器三大类，外设的寄存器通常被连续地编址。根据CPU体系结构的不同，CPU对IO端口的编址方式有两种：
（1）I/O映射方式（I/O-mapped）
典型地，如X86处理器为外设专门实现了一个单独的地址空间，称为”I/O地址空间”或者”I/O端口空间”，CPU通过专门的I/O指令（如X86的IN和OUT指令）来访问这一空间中的地址单元。
（2）内存映射方式（Memory-mapped）
RISC指令系统的CPU（如ARM、PowerPC等）通常只实现一个物理地址空间，外设I/O端口成为内存的一部分。此时，CPU可以象访问一个内存单元那样访问外设I/O端口，而不需要设立专门的外设I/O指令。　但是，这两者在硬件实现上的差异对于软件来说是完全透明的，驱动程序开发人员可以将内存映射方式的I/O端口和外设内存统一看作是”I/O内存”资源。
一般来说，在系统运行时，外设的I/O内存资源的物理地址是已知的，由硬件的设计决定。但是CPU通常并没有为这些已知的外设I/O内存资源的物理地址预定义虚拟地址范围，驱动程序并不能直接通过物理地址访问I/O内存资源，而必须将它们映射到核心虚地址空间内（通过页表），然后才能根据映射所得到的核心虚地址范围，通过访内指令访问这些I/O内存资源。Linux在io.h头文件中声明了函数ioremap（），用来将I/O内存资源的物理地址映射到核心虚地址空间（3GB－4GB）中，原型如下：

C
```
void * ioremap(unsigned long phys_addr, unsigned long size, unsigned long flags);
```
iounmap函数用于取消ioremap（）所做的映射，原型如下：

C
```
void iounmap(void * addr);
```
在将I/O内存资源的物理地址映射成核心虚地址后，理论上讲我们就可以象读写RAM那样直接读写I/O内存资源了。为了保证驱动程序的跨平台的可移植性，我们应该使用Linux中特定的函数来访问I/O内存资源，而不应该通过指向核心虚地址的指针来访问。
最后，我们要特别强调驱动程序中mmap函数的实现方法。用mmap映射一个设备，意味着使用户空间的一段地址关联到设备内存上，这使得只要程序在分配的地址范围内进行读取或者写入，实际上就是对设备的访问。≠
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/d4181232cde3a79b8c6d11e39fd06ad2.png)![](http://www.jobbole.com/wp-content/uploads/2016/04/0e2c29b501d9020b8ef4615fe1493fc0.jpg)
