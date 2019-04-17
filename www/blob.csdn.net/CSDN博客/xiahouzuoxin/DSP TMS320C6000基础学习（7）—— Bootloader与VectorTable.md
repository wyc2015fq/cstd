# DSP TMS320C6000基础学习（7）—— Bootloader与VectorTable - xiahouzuoxin - CSDN博客





2013年08月02日 12:42:17[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：9295








本文主要简单记录C6000在启动装载过程和中断向量表的配置。




***1. Bootloader***

![](https://img-blog.csdn.net/20130802115459296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


如上图，

（1）在Device Reset阶段：

设备初始化为默认状态，大部分三态输出都配置为高阻态。

（2）在CPU Reset阶段：

从RS上升沿处开始（这个时候，根据HD[4:3]配置启动模式，HD8配置大小端模式，CLKMODE配置输入时钟源，根据HPI_EN配置外设功能），处理器检查启动模式HD[4:3]，启动bootloader程序。

EDMA自动将CE1开始位置的1KB代码拷贝到内部程序存储器的0地址。

![](https://img-blog.csdn.net/20130802104433906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（3）RESET信号恢复高电平，CPU从内存0地址处开始运行程序




***2. Interrupt Vector Table（Interrupt Service Table, IST）***


        处理器在RESET为低电平时复位，在RESET的上升沿，启动配置被锁定，开始从0地址处执行程序。一旦ROM启动完成（将外部ROM的1KB程序拷贝到内存0地址完成），寄存器初始化为默认值，程序计数器PC装载复位中断向量（复位中断向量需要在0地址处），CPU开始从0地址开始执行，这个地址称为**复位向量**。

        默认的，**中断服务表（IST）**也在0地址处。IST是中断向量的一个集合，当CPU中断发生时，PC自动通过中断向量跳转到对应的中断服务程序。每个中断向量都是8个字（WORD）对齐。IST可以重映射到任意的以0x400-byte对齐的位置，但需要修改**中断服务表指针（ISTP）**，在复位时，ISTP初始化为0值，IST首地址与复位向量对齐。

**（1）创建中断向量表**

IST由32个服务向量组成，每个向量对应一个CPU中断，特殊的：中断号0对应复位向量，中断号1对应不可屏蔽中断（NMI）。中断4~15对应各种信号事件的中断。中断号为n的中断向量的地址为

![](http://www.forkosh.com/mathtex.cgi?formdata=%5CSmall+Address+%3D+ISTP+%2B+32+*+n)


比如，ISTP重定位到0x1000，中断号为4的中断向量地址为Addr=0x1000+0x20*4=0x1080。

每个中断向量必须在8条指令内完成，若无法在8条指令内完成，必须将一些工作放到**中断服务程序（Interrupt Service Routine, ISR）**中完成。



```cpp
.sect vectors
RESET:   MVK .S2 Start, B0 ; Load Start address
         MVKH .S2 Start, B0 ; Load Start address
         B .S2 B0 ; Branch to start
         NOP
         NOP
         NOP
         NOP
         NOP
NMI_ISR: MVK .S2 Nmi_isr, B0
         MVKH .S2 Nmi_isr, B0
         B .S2 B0
         NOP
         NOP
         NOP
         NOP
         NOP
```
上面是包含了复位和不可屏蔽中断的中断向量表的一个例子，主要是完成一个跳转到中断服务程序的功能。



**（2）中断服务程序（ISR）**


       当使用-c或-cr链接器选项时，DSP的C编译器自动的创建了函数_c_int00，这个函数对应C程序的入口地址，复位向量必须跳转到_c_int00地址处。当C程序遇到一个CPU中断时，在中断向量表中或中断服务程序中要使用的CPU寄存器都将先被压入堆栈，一旦中断服务程序完成，堆栈中的值弹出到对应的寄存器，继续执行原C程序。

        中断服务程序需要使用关键字interrupt声明，



```cpp
interrupt void myISR(void)
{
	/* Code for myISR */
	…
}
```

中断服务程序没有返回值，也没有参数，interrupt的功能就是在执行该中断服务程序前自动将寄存器压栈，执行完后自动的弹栈。







```cpp
*********************************************************************************
* vecs.asm
* Copyright	2003 by SEED Electronic Technology Ltd.
* All rights reserved. Property of SEED Electronic Technology Ltd.			                *
* Designed by:	Hongshuai.Li								                                *
*********************************************************************************

*------------------------------------------------------------------------------
* Global symbols defined here and exported out of this file
*------------------------------------------------------------------------------
   .global _vectors
   .global _c_int00
   .global _vector1
   .global _vector2
   .global _vector3
   .global _vector4
   .global _vector5
   .global _vector6
   .global _vector7
   .global _c_int08  ; Hookup the c_int08 ISR in main()
   .global _vector9	  
   .global _vector10 
   .global _vector11   
   .global _vector12  
   .global _vector13   
   .global _vector14   
   .global _vector15   

*------------------------------------------------------------------------------
* Global symbols referenced in this file but defined somewhere else. 
* Remember that your interrupt service routines need to be referenced here.
*------------------------------------------------------------------------------
   .ref _c_int00

*------------------------------------------------------------------------------
* This is a macro that instantiates one entry in the interrupt service table.
*------------------------------------------------------------------------------
VEC_ENTRY .macro addr
    STW   B0,*--B15
    MVKL  addr,B0
    MVKH  addr,B0
    B     B0
    LDW   *B15++,B0
    NOP   2
    NOP   
    NOP   
   .endm


*------------------------------------------------------------------------------
* This is a dummy interrupt service routine used to initialize the IST.
*------------------------------------------------------------------------------
_vec_dummy:
  B    B3
  NOP  5

*------------------------------------------------------------------------------
* This is the actual interrupt service table (IST). It is properly aligned and
* is located in the subsection .text:vecs. This means if you don't explicitly
* specify this section in your linker command file, it will default and link
* into the .text section. Remember to set the ISTP register to point to this
* table.
*------------------------------------------------------------------------------
 .sect ".text:vecs"
 .align 1024

_vectors:
_vector0:   VEC_ENTRY _c_int00    ;RESET
_vector1:   VEC_ENTRY _vec_dummy  ;NMI
_vector2:   VEC_ENTRY _vec_dummy  ;RSVD
_vector3:   VEC_ENTRY _vec_dummy
_vector4:   VEC_ENTRY _vec_dummy
_vector5:   VEC_ENTRY _vec_dummy
_vector6:   VEC_ENTRY _vec_dummy
_vector7:   VEC_ENTRY _vec_dummy
_vector8:   VEC_ENTRY _c_int08    ; Hookup the c_int08 ISR in main() 
_vector9:   VEC_ENTRY _vec_dummy
_vector10:  VEC_ENTRY _vec_dummy
_vector11:  VEC_ENTRY _vec_dummy
_vector12:  VEC_ENTRY _vec_dummy
_vector13:  VEC_ENTRY _vec_dummy
_vector14:  VEC_ENTRY _vec_dummy
_vector15:  VEC_ENTRY _vec_dummy

*------------------------------------------------------------------------------


********************************************************************************
* End of vecs.asm
********************************************************************************
```
上面是一个实际使用的C6713的中断向量表的汇编文件（vecs.asm），其中定义了全部15个中断向量，复位向量跳转到_c_int00地址，8号中断向量跳转到_c_int08函数地址，在C主程序中应该做如下定义，




```cpp
/*
 * interrupt function
 */
interrupt void c_int08(void)
{
	
}
```
另外，由于在C程序中的PC跳转到中断向量地址是远跳转，因此有时需要在C程序中声明中断向量地址为远地址，如下，





```cpp
extern far void vectors();
```
其中的vectors()表示中断向量表的起始地址（汇编中使用了_vectors）。



注意：C中的变量和函数对应到汇编中都多了一个下划线（_），编译器在将C编译成汇编时都会自动添加下划线。





