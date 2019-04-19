# 【转】32位汇编 NASM 入门级 - 三少GG - CSDN博客
2010年09月18日 18:28:00[三少GG](https://me.csdn.net/scut1135)阅读数：1361
1. 
问题:VC.NET中如何调用汇编asm文件中定义的函数，请高手多多指点!!!?
我的普通VC工程中（没用到托管）要调用一个asm文件中定义的函数 
但是编译不能通过，请高手多多指教
提问时间：2004-3-15 20:51:24　回答次数：(4)　提问人：软界网友[我来回答](http://www.softwelt.com/Know/KnowDetail-2171112.html#huida)
回答（1）
**http://www.fruitfruit.com/vc/vc_asm.txt**
谢谢上面的仁兄，小弟用普通asm例子文件，用masm32编译成obj文件，然后将obj文件加入到工程中是可以的 
但是我的asm文件不能编译，不知道是不是用到MMX指令的缘故 
能不能帮助我看，我的文件如下： 
BITS 32 
%macro cglobal 1 
%ifdef PREFIX 
global _%1 
%define %1 _%1 
%else 
global %1 
%endif 
%endmacro 
%ifdef FORMAT_COFF 
SECTION .rodata data 
%else 
SECTION .rodata data align=16 
%endif 
ALIGN 16 
mmx_one: 
dw 1, 1, 1, 1 
SECTION .text 
cglobal transfer_8to16copy_mmx 
cglobal transfer_16to8copy_mmx 
cglobal transfer_8to16sub_mmx 
cglobal transfer_8to16subro_mmx 
cglobal transfer_8to16sub2_mmx 
cglobal transfer_8to16sub2_xmm 
cglobal transfer_16to8add_mmx 
cglobal transfer8x8_copy_mmx 
%macro COPY_8_TO_16 1 
movq mm0, [eax] 
movq mm1, [eax+edx] 
movq mm2, mm0 
movq mm3, mm1 
punpcklbw mm0, mm7 
movq [ecx+%1*32], mm0 
punpcklbw mm1, mm7 
movq [ecx+%1*32+16], mm1 
punpckhbw mm2, mm7 
punpckhbw mm3, mm7 
lea eax, [eax+2*edx] 
movq [ecx+%1*32+8], mm2 
movq [ecx+%1*32+24], mm3 
%endmacro 
ALIGN 16 
transfer_8to16copy_mmx: 
mov ecx, [esp+ 4] ; Dst 
mov eax, [esp+ 8] ; Src 
mov edx, [esp+12] ; Stride 
pxor mm7, mm7 
COPY_8_TO_16 0 
COPY_8_TO_16 1 
COPY_8_TO_16 2 
COPY_8_TO_16 3 
ret 
%macro COPY_16_TO_8 1 
movq mm0, [eax+%1*32] 
movq mm1, [eax+%1*32+8] 
packuswb mm0, mm
----------------------------------------------------------------------------------------------------------------------------------------
2.How to use ASM in Class method?
#include "stdafx.h"
class ClassWithAsm
{
private:
	int age;
	char data[123];
	char name[10];
public:
	void  SetName(char *name);
	void SetData(char* pdata);
	char* GetName()
	{
		return name;
	}
	char* GetData()
	{
		return data;
	}
};
void ClassWithAsm::SetData(char* pdata)
{//thanks to Onega(www.fruitfruit.com)
	int len = strlen(pdata);
	_asm
	{
		mov edi,this ;
		add edi, ClassWithAsm::data
		mov esi,pdata 
		mov ecx ,len
		rep movs byte ptr [edi], byte ptr [esi]
	}
	data[len]=0;
}--------------------------------------------------------------------------------------------------------------------------------------
3.
**PUNPCKLBW/PUNPCKLWD/PUNPCKLDQ/PUNPCKLQDQ **
扩展低位压缩数据
将目标操作数（第一个操作数）与源操作数（第二个操作数）的低位数据元素（字节、字或双字）展开并交叉组合，然后将结果放入目标操作数。（图 3-15 显示 64 位操作数中的字节扩展操作）。忽略高位数据元素。源操作数可以是 MMX™ 技术寄存器或 64 位内存位置，也可以是 XMM 寄存器或 128 位内存位置。目标操作数可以是 MMX 或 XMM 寄存器。如果源数据来自内存操作数，则从内存访问完整 64 位或 128 位操作数，但指令分别只使用高 32 位或 64 位。
#### 图 3-15. PUNPCKLBW 指令操作
PUNPCKLBW 指令交叉组合源操作数与目标操作数的低位字节，PUNPCKLWD 指令交叉组合源操作数与目标操作数的低位字，PUNPCKLDQ 指令交叉组合源操作数与目标操作数的低位双字，PUNPCKLQDQ 指令交叉组合源操作数与目标操作数的低位四字。
如果源操作数为全零，则结果（存储在目标操作数中）包含目标操作数中原始值的低位数据元素的零扩展。例如，使用 PUNPCKLBW 指令时，将对低位字节进行零扩展（即扩展成无符号字）；使用 PUNPCKLWD 指令时，则将对低位字进行零扩展（即扩展成无符号双字）。
## 操作
PUNPCKLBW instruction with 64-bit operands:
DEST[63..56]![](arrwleft.gif)
DEST[31..0];
3.
[http://topic.csdn.net/u/20100907/16/522c3cae-7c14-41bc-b202-e9d9836863d5.html](http://topic.csdn.net/u/20100907/16/522c3cae-7c14-41bc-b202-e9d9836863d5.html)
4.
[http://book.51cto.com/art/200902/111520.htm](http://book.51cto.com/art/200902/111520.htm)
### 14.2.2 MMX/SSE 2实现XviD CODEC（1）
[http://book.51cto.com](http://book.51cto.com/)  2009-02-25 10:15  路锦正/周冬梅  电子工业出版社  [我要评论(0)](http://www.51cto.com/php/feedbackt.php?id=111520)
- 摘要：《Visual C++音频/视频处理技术及工程实践》第14章XviD CODEC实现MPEG-4编/解码,本章首先介绍MPEG-4视频编码算法的原理，对MPEG-4编码算法的工程实现XviD CODEC的工作过程做详细的介绍。本节为大家讲述的是MMX/SSE 2实现XviD CODEC。
- 标签：[MMX](http://www.51cto.com/php/search.php?keyword=MMX)[SSE 2](http://www.51cto.com/php/search.php?keyword=SSE%202)[C++](http://www.51cto.com/php/search.php?keyword=C%2B%2B)[Visual
 C++](http://www.51cto.com/php/search.php?keyword=Visual%20C%2B%2B)[音频](http://www.51cto.com/php/search.php?keyword=%D2%F4%C6%B5)[视频](http://www.51cto.com/php/search.php?keyword=%CA%D3%C6%B5)
- 
[限时报名参加“甲骨文全球大会·2010·北京”及“JavaOne和甲骨文开发者大会2010”](http://www.51cto.com/exp/oow2010/baoming/)
**14.2.2  MMX/SSE 2实现XviD CODEC（1）**
上一节对XviD CODEC的系统框架做了功能剖析和技术说明，现在开始对最底层的模块如DCT/IDCT、Q/IQ、VLC/VLD、SAD等作汇编优化和设计。第12章已经对MMX/SSE 2汇编编程、汇编指令结构等作了详细介绍。基于通用CPU进行多媒体编程，汇编语言提供了数据打包、单指令多数据处理等特点的高性能支持。
由于算法系统是C语言编程，而在使用汇编优化核心模块的时候，就涉及了混合编程技术。汇编编写的函数要被C语言调用，则在编程时要遵守一定的规则。如在汇编函数名称前加下画线"_"。
**1．MMX汇编指令优化核心模块**
XviD CODEC的汇编程序支持NASM汇编器，它是一个为可移植性与模块化而设计的80 86的汇编器。它支持相当多的目标文件格式，包括Linux、NetBSD/FreeBSD、a.out、ELF、COFF、微软16位的OBJ和Win32。它还可以输出纯二进制文件。它的语法设计得相当地简洁易懂，和Intel语法相似但更简单。它支持Pentium、P6、MMX、3DNow!、SSE和SSE 2指令集。
**1）一般宏定义**
下面的宏定义中使用了宏汇编中表达式操作符%，它把后面的文本解释为表达式，功能是取表达式的最终的值。使用这个操作符可以把表达式的值作为实参引用，而不是引用表达式文本本身。
%macro表示定义多行宏，%endmacro表示多行宏的结束。在%macro一行上宏名后面的数字1定义了宏可以接收的参数的个数，宏定义里面的%1用来引用宏调用中的第一个参数。对于一个有多个参数的宏，参数序列可以这样写：%2、%3等。%define用来定义单行宏，NASM的详细开发技术请参考Nasm中文手册。
BITS 32                                       ; 表明32位指令
%macro cglobal 1                             ; cglobal宏定义
%ifdef PREFIX
%ifdef MARK_FUNCS
global _%1:function %1.endfunc-%1
%define %1 _%1:function %1.endfunc-%1
%else
global _%1                     ;有效定义，定义全局函数
%define %1 _%1                ;在汇编中，使用没有"_"的函数名称
%endif
%else
%ifdef MARK_FUNCS
global %1:function %1.endfunc-%1
%else
global %1
%endif
%endif
%endmacro
上面使用宏定义了函数的名称，宏名为cglobal，由于在汇编文件的汇编选项设置中定义了PREFIX，而没有定义MARK_FUNCS，所以宏的实际定义为上述代码中加黑显示的内容，例如：
global _myfunc
define myfunc _myfunc
**2）只读段.rodata存放常量**
;===========================================================================
; 只读数据段(Read Only Data)
;===========================================================================
%ifdef FORMAT_COFF
SECTION .rodata
%else
SECTION .rodata align=16   ;只读数据段，16字节（128位）对齐
%endif
ALIGN 16              ;16字节对齐
mmx_one:
dw 1, 1, 1, 1        ;定义的字
上述代码定义了16字节对齐的常量mmx_one，4个常量中每个常量长度为16比特，数据放置在只读段.rodata。
**3）C语言函数transfer_8to16copy_c的功能**
C语言函数transfer_8to16copy_c的功能是把像素值从8位扩展到16位。
void
transfer_8to16copy_c(int16_t * const dst,  
const uint8_t * const src,  uint32_t stride)
{
uint32_t i, j;
for (j = 0; j < 8; j++) {
for (i = 0; i < 8; i++) {
dst[j * 8 + i] = (int16_t) src[j * stride + i];
}
}
}
上述代码有两层循环，每次处理一个单元，即把8位的像素值扩展为16位。
MMX的寄存器是64位，这样MMX指令可以一次处理8个字节。该函数的汇编优化是transfer_8to16copy_mmx()，该函数优化的思路是把要复制的数据打包形成64位，每次存储两行，执行4次宏处理，下面是汇编优化的结果。
SECTION .text     ;.text 段
cglobal transfer_8to16copy_mmx ;函数声明：_ transfer_8to16copy_mmx
%macro COPY_8_TO_16 1   ;定义宏COPY_8_TO_16开始
movq mm0, [eax]    ;取64bit，第一行8个像素点
movq mm1, [eax+edx]   ;取64bit，第二行8个像素点
movq mm2, mm0     ;mm2=mm0
movq mm3, mm1     ;mm3=mm1
punpcklbw mm0, mm7    ;mm0的低4Byte扩展成word
movq [ecx+%1*32], mm0   ;mm0的内容存到[ecx+%1*32]
punpcklbw mm1, mm7    ;mm1的低4Byte扩展成word
movq [ecx+%1*32+16], mm1  ;mm1的内容存到[ecx+%1*32+16]
punpckhbw mm2, mm7    ;mm2的低4Byte扩展成word
punpckhbw mm3, mm7    ;mm3的低4Byte扩展成word
lea eax, [eax+2*edx]   ;修改eax值，指向当前行的第三行
movq [ecx+%1*32+8], mm2  ;mm2的内容存到[ecx+%1*32+8]
movq [ecx+%1*32+24], mm3  ;mm3的内容存到[ecx+%1*32+24]
%endmacro      ;定义宏COPY_8_TO_16结束
ALIGN 16       ;下面地址是16字节对齐
transfer_8to16copy_mmx:   ;实际是_transfer_8to16copy_mmx，前面有define定义。
mov ecx, [esp+ 4] ; Dst  ;取第一个参数 dst
mov eax, [esp+ 8] ; Src  ;取第二个参数 src
mov edx, [esp+12] ; Stride  ;取第三个参数 stride
pxor mm7, mm7                   ;mm7清零
COPY_8_TO_16 0                 ;第一次展开宏，处理0、1行
COPY_8_TO_16 1                 ;第二次展开宏，处理2、3行
COPY_8_TO_16 2                 ;第三次展开宏，处理4、5行
COPY_8_TO_16 3                 ;第四次展开宏，处理6、7行
ret                              ;函数返回
.endfunc
