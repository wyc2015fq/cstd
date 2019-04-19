# 【转】MMX指令集简介 - 三少GG - CSDN博客
2010年09月19日 15:57:00[三少GG](https://me.csdn.net/scut1135)阅读数：1005

**MMX指令集在C++中的使用**
作者:孙原
上次在《关于内联汇编的几个技巧》一文中只是简单地介绍了如何在C++中使用内联汇编，这一次先对上一次的文章作一补充，然后介绍如何使用MMX指令。
 一、 内联汇编的一般原则：
1、 自由使用通用寄存器；（EAX,EBX,ECX和EDX）
2、 其它寄存器利用堆栈保留，使用，最后恢复；
一般的像下面这样：
__asm{  push ebppush esp……….//使用EBP和ESPpop esppop  ebp }
 二、 内联汇编__asm可以单独使用：
例如：
__asm mov eax,anyval1__asm mov ebx,anyval2
 三、 函数返回值可以直接放到eax中，可以不理会警告
例如：
int anyfun(……/*anyparm*/){int irtn;//函数返回值……        //函数语句__asm mov eax,irtn   //代替return irtn;但编译器会发出警告，可以不理它}
 四、 内联汇编不区分大小写，语法同普通汇编
例如：
__asm{mov eax,ebxMOV EAX,EBX//同上一句}
注意：C++的变量还是区分大小写的
 五、 内联汇编尽量使用__asm或_asm关键字，而不用标准C++的asm关键字(这个是微软说的)
以上只是一些关于内联汇编的补充，我将出一系列围绕内联汇编的文章，下面接着上一次的话题详细一点地讲一下MMX指令的调用
**1、 MMX指令集简介**：
[数据传输指令]
movq //传输64位整数
movd //传输32位整数
[数据打包转换指令]
PACKSSWB //Pack words into bytes with signed saturation.
PACKSSDW //Pack doublewords into words with signed saturation.
PACKUSWB //Pack words into bytes with unsigned saturation.
PUNPCKHBW //Unpack high-order bytes.
PUNPCKHWD //Unpack high-order words.
PUNPCKHDQ //Unpack high-order doublewords.
PUNPCKLBW //Unpack low-order bytes.
PUNPCKLWD //Unpack low-order words.
PUNPCKLDQ //Unpack low-order doublewords.
注：这一组指令我没有具体用过，不知道是干什么的，请高手赐教！小弟先谢了！
[ 算术指令]
PADDB
PADDW 
PADDD 
PADDSB
PADDSW
PADDUSB
PADDUSW 
PSUBB 
PSUBW 
PSUBD 
PSUBSB 
PSUBSW 
PSUBUSB 
PSUBUSW
PMULHW 
PMULLW 
PMADDWD 
[ 比较指令]
PCMPEQB Compare packed bytes for equal.
PCMPEQW Compare packed words for equal.
PCMPEQD Compare packed doublewords for equal.
PCMPGTB Compare packed signed byte integers for greater than.
PCMPGTW Compare packed signed word integers for greater than.
PCMPGTD Compare packed signed doubleword integers for greater than.
这组指令用于成组比较数据
[ 位逻辑指令]
PAND Bitwise logical AND.
PANDN Bitwise logical AND NOT.
POR Bitwise logical OR.
PXOR Bitwise logical exclusive OR.
这组指令与AND,XOR基本相同，都是按位进行逻辑运算。
[ 移位和循环移位指令]
PSLLW //Shift packed words left logical.
PSLLD //Shift packed doublewords left logical.
PSLLQ //Shift packed quadword left logical.
PSRLW //Shift packed words right logical.
PSRLD //Shift packed doublewords right logical.
PSRLQ //Shift packed quadword right logical.
PSRAW //Shift packed words right arithmetic.
PSRAD //Shift packed doublewords right arithmetic.
[ 状态管理指令]
EMMS //Empty MMX state.
在VC中要求所有的MMX指令调用完毕后都要调用这个指令清空
例如：
__asm{
	…..MMX 语句
	EMMS			//清空状态
}
以上是所有的MMX指令，你可以测试使用其中的指令，他的工作原理就是单指令，多数据
**2、 使用MMX指令集的注意事项**
由于在CPU内部，FPU寄存器和MMX寄存器是同一组寄存器，所以在同时引用上面寄存器时要注意正确的状态转换，具体做法以后在探讨。你只要先记住不能简单的混合以上两种指令集即可。
每次调用之前要先检测cpu是否支持MMX指令集，以免发生异常。具体做法看下列示例：
mov EAX, 1 ; request for feature flags
CPUID ; 0Fh, 0A2h CPUID instruction
test EDX, 00800000h ; Is IA MMX technology bit (Bit 23 of EDX)
; in feature flags set?
jnz MMX_Technology_Found
这段代码来自Intel的参考手册，所以你可以放心的使用。
另外指令参考： [http://blog.csdn.net/procedurecode/archive/2007/10/23/1839492.aspx](http://blog.csdn.net/procedurecode/archive/2007/10/23/1839492.aspx)
