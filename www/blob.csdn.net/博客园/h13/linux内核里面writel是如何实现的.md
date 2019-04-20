# linux内核里面writel是如何实现的 - h13 - 博客园
在邮件列表里讨论了一下writel是如何实现的，这个函数实现在操作系统层，有内存保护的情况下，往一个寄存器或者内存地址写一个数据。
在arch/alpha/kernel/io.c中有
```
188 void writel(u32 b, volatile void __iomem *addr)
189 {
190     __raw_writel(b, addr);
191     mb();
192 }
```
这样一个writel函数的作用应该是向一个地址上写一个值，我想知道这个函数底下具体实现的细节，于是往下继续跟踪代码：__raw_writel(b, addr);
```
129 void __raw_writel(u32 b, volatile void __iomem *addr)
130 {
131     IO_CONCAT(__IO_PREFIX,writel)(b, addr);
132 }
```
再往下跟踪 IO_CONCAT，在对应的io.h中的定义如下：
```
134 #define IO_CONCAT(a,b) _IO_CONCAT(a,b)
135 #define _IO_CONCAT(a,b) a ## _ ## b
```
这段代码前几天问过了，是标示将两边的字符串连接起来的意思。
跟踪__IO_PREFIX 定义如下
```
501 #undef __IO_PREFIX
502 #define __IO_PREFIX     apecs
```
到这里就结束了，再往下我就晕了，有问题如下：
1、到底是怎么将数据写入地址的？我把这些单独提取出来，进行预编译，宏展开后，发现是这样的：
```
void __raw_writel(u32 b, volatile void __iomem *addr)
{
    apecs_writel(b, addr);
}
```
但是在内核里根本就没找到apecs_writel函数，请帮忙解释下。
```
For the first question,
you should refer to the file "arch\alpha\kernle\Machvec_impl.h"
"~\Machve.h" "~\io.c" "~\io.h" "~\core_**.h".
as you have analysized before, in the file Machvec_impl.h and Machve.h,
DO_CIA_IO,IO,IO_LITE, these three macros implement the symbole
connection between ** arch and writel function, and the function
pointer initializations.
so, the details implementation to writel is to init the
alpha_machine_vector structure and the definition to the relevant
function pointer invoked to complete the low-level write operation.
.mv_writel =CAT(low,_writel),<---IO(CIA,cia)<-->cia_writel(b, addr); <---
|
writel(b, addr)-->__raw_writel(b, addr);--->cia_writel(b,addr)---------------
For the second quesiton,
mb()--->__asm__ __volatile__("mb": : :"memory");
so, it is a memory barrier for alpha architecture to ensure some
operations before some actions could be occured.
and, it is similiar with the barrier() in x86 platform/arm platform.
```
继续阅读代码，看看定义__IO_PREFIX之后紧接着包含了哪个头文件。在哪个头文
件里面寻找答案。对于你的apsec，看看以下代码段（linux-2.6.28-rc4）
```
arch/alpha/include/asm/core_apecs.h
------------------------------------------
#undef __IO_PREFIX
#define __IO_PREFIX apecs
#define apecs_trivial_io_bw 0
#define apecs_trivial_io_lq 0
#define apecs_trivial_rw_bw 2
#define apecs_trivial_rw_lq 1
#define apecs_trivial_iounmap 1
#include <asm/io_trivial.h>
------------------------------------------
arch/alpha/include/asm/io_trivial.h
------------------------------------------
__EXTERN_INLINE void
IO_CONCAT(__IO_PREFIX,writel)(u32 b, volatile void __iomem *a)
{
*(volatile u32 __force *)a = b;
}
```
就是最终通过*(volatile u32 __force *)a = b;
来写入数据的。
如果在没有os，没有mmu的情况下，当开发板裸跑的时候，我们只需要一句话就一切ok：
*(unsigned long *)addr = value;

