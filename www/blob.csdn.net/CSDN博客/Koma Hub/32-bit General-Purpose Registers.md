# 32-bit General-Purpose Registers - Koma Hub - CSDN博客
2019年03月18日 21:52:13[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：8
个人分类：[Assembly																[汇编](https://blog.csdn.net/Rong_Toa/article/category/8750815)](https://blog.csdn.net/Rong_Toa/article/category/8750816)
**Table of Contents**
[Specialized Uses](#Specialized%20Uses)
[Segment Registers](#Segment%20Registers)
[Instruction Pointer](#Instruction%20Pointer)
[EFLAGS Register](#EFLAGS%20Register)
# Specialized Uses
Some general-purpose registers have specialized uses:
> - EAX is automatically used by multiplication and division instructions. It is often called the extended accumulator register.
- The CPU automatically uses ECX as a loop counter.
- ESP addresses data on the stack (a system memory structure). It is rarely used for ordinary arithmetic or data transfer. It is often called the extended stack pointer register.
- ESI and EDI are used by high-speed memory transfer instructions. They are sometimes called the extended source index and extended destination index registers.
- EBP is used by high-level languages to reference function parameters and local variables on the stack. It should not be used for ordinary arithmetic or data transfer except at an advanced level of programming. It is often called the extended frame pointer register.
- EAX由乘法和除法指令自动使用。它通常被称为扩展累加器寄存器。
- CPU自动使用ECX作为循环计数器。
- esp处理堆栈（系统内存结构）上的数据。它很少用于普通的算术或数据传输。它通常被称为扩展堆栈指针寄存器。
- ESI和EDI由高速内存传输指令使用。它们有时被称为扩展源索引和扩展目标索引寄存器。
- 高级语言使用EBP来引用堆栈上的函数参数和局部变量。它不应该用于普通的算术或数据传输，除非在高级编程级别。它通常被称为扩展帧指针寄存器。
![](https://img-blog.csdnimg.cn/20190318214607853.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# Segment Registers
> 
In real-address mode, 16-bit segment registers indicate base addresses of preassigned memory areas named segments. In protected mode, segment registers hold pointers to segment descriptor tables. Some segments hold program instructions (code), others hold variables (data), and another segment named the stack segment holds local function variables and function parameters.
在实地址模式下，16位段寄存器指示预先分配的内存区名为段的基址。在保护模式下，段寄存器保存指向段描述符表的指针。一些段保存程序指令（代码），另一些段保存变量（数据），另一个名为堆栈段的段保存局部函数变量和函数参数。
# Instruction Pointer
> 
The EIP, or instruction pointer, register contains the address of the next instruction to be executed. Certain machine instructions manipulate EIP, causing the program to branch to a new location.
EIP或指令指针寄存器包含要执行的下一条指令的地址。某些机器指令操作EIP，使程序分支到新位置。
# EFLAGS Register
> 
The EFLAGS (or just Flags) register consists of individual binary bits that control the operation of the CPU or reflect the outcome of some CPU operation. Some instructions test and manipulate individual processor flags.
eflags（或flags）寄存器由控制CPU操作或反映某些CPU操作结果的单个二进制位组成。一些指令测试和操作单个处理器标志。
![](https://img-blog.csdnimg.cn/20190318214614895.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190318214703482.PNG)
![](https://img-blog.csdnimg.cn/20190318214710823.PNG)
未完待续：《assembly_language_for_x86_processors》
