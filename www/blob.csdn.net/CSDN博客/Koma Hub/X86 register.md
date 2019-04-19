# X86 register - Koma Hub - CSDN博客
2019年03月16日 14:12:18[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：197
个人分类：[架构																[汇编																[体系结构																[Assembly](https://blog.csdn.net/Rong_Toa/article/category/8750816)](https://blog.csdn.net/Rong_Toa/article/category/7496482)](https://blog.csdn.net/Rong_Toa/article/category/8750815)](https://blog.csdn.net/Rong_Toa/article/category/8742415)
**Table of Contents**
[首先介绍x86，x64， x86_64, amd64。](#%E9%A6%96%E5%85%88%E4%BB%8B%E7%BB%8Dx86%EF%BC%8Cx64%EF%BC%8C%C2%A0x86_64%2C%20amd64%E3%80%82)
[X86 64 Register and Instruction Quick Start](#firstHeading)
[Contents](#Contents)
[Registers](#Registers)
[General-Purpose Registers](#General-Purpose%20Registers)
[Floating-Point and SIMD Registers](#Floating-Point%20and%20SIMD%20Registers)
[Instructions](#Instructions)
[Starter Kit](#Starter%20Kit)
[Resources](#Resources)
[x86 Registers](#x86%20Registers)
[General registers](#General%20registers)
[Segment registers](#Segment%20registers)
[Indexes and pointers](#Indexes%20and%20pointers)
[The EFLAGS register](#The%20EFLAGS%20register)
[Undocumented registers](#Undocumented%20registers)
[CPU Registers x86-64](#firstHeading)
[Contents](#Contents)
[General Purpose Registers](#General%20Purpose%20Registers)
[Pointer Registers](#Pointer%20Registers)
[Segment Registers](#Segment%20Registers)
[RFLAGS Register](#RFLAGS%20Register)
[Control Registers](#Control%20Registers)
[CR0](#CR0)
[CR2](#CR2)
[CR3](#CR3)
[CR4](#CR4)
[CR8](#CR8)
[CR1, CR5-7, CR9-15](#CR1%2C%20CR5-7%2C%20CR9-15)
[MSRs](#MSRs)
[IA32_EFER](#IA32_EFER)
[FS.base, GS.base](#FS.base%2C%20GS.base)
[KernelGSBase](#KernelGSBase)
[Debug Registers](#Debug%20Registers)
[DR0 - DR3](#DR0%20-%20DR3)
[DR6](#DR6)
[DR7](#DR7)
[Test Registers](#Test%20Registers)
[Protected Mode Registers](#Protected%20Mode%20Registers)
[GDTR](#GDTR)
[LDTR](#LDTR)
[TR](#TR)
[IDTR](#IDTR)
# 首先介绍x86，x64， x86_64, amd64。
- x86：386，586， 686等等的32位处理器；
- amd64：amd公司推出的领先于Intel的cpu，64位；
- x64：传统意义上的64位系统；
- x86_64：x86坐不住了，在原有x86基础上发展起来的64微系统；
然后，当然了，以我以往的作风，当然是直接粘贴英文文章喽！
# X86 64 Register and Instruction Quick Start
> 
原文地址：[https://wiki.cdot.senecacollege.ca/wiki/X86_64_Register_and_Instruction_Quick_Start](https://wiki.cdot.senecacollege.ca/wiki/X86_64_Register_and_Instruction_Quick_Start)
This page contains very basic information on the x86_64 architecture: the [Register|register] layout and naming and the some basic instructions.
## Contents
- [1 Registers](https://wiki.cdot.senecacollege.ca/wiki/X86_64_Register_and_Instruction_Quick_Start#Registers)- [1.1 General-Purpose Registers](https://wiki.cdot.senecacollege.ca/wiki/X86_64_Register_and_Instruction_Quick_Start#General-Purpose_Registers)
- [1.2 Floating-Point and SIMD Registers](https://wiki.cdot.senecacollege.ca/wiki/X86_64_Register_and_Instruction_Quick_Start#Floating-Point_and_SIMD_Registers)
- [2 Instructions](https://wiki.cdot.senecacollege.ca/wiki/X86_64_Register_and_Instruction_Quick_Start#Instructions)- [2.1 Starter Kit](https://wiki.cdot.senecacollege.ca/wiki/X86_64_Register_and_Instruction_Quick_Start#Starter_Kit)
- [3 Resources](https://wiki.cdot.senecacollege.ca/wiki/X86_64_Register_and_Instruction_Quick_Start#Resources)
## Registers
### General-Purpose Registers
The 64-bit versions of the 'original' x86 registers are named:
- rax - register a extended
- rbx - register b extended
- rcx - register c extended
- rdx - register d extended
- rbp - register base pointer (start of stack)
- rsp - register stack pointer (current location in stack, growing downwards)
- rsi - register source index (source for data copies)
- rdi - register destination index (destination for data copies)
The registers added for 64-bit mode are named:
- r8 - register 8
- r9 - register 9
- r10 - register 10
- r11 - register 11
- r12 - register 12
- r13 - register 13
- r14 - register 14
- r15 - register 15
These may be accessed as:
- 64-bit registers using the 'r' prefix: rax, r15
- 32-bit registers using the 'e' prefix (original registers: e_x) or 'd' suffix (added registers: r__d): eax, r15d
- 16-bit registers using no prefix (original registers: _x) or a 'w' suffix (added registers: r__w): ax, r15w
- 8-bit registers using 'h' ("high byte" of 16 bits) suffix (original registers - bits 8-15: _h): ah, bh
- 8-bit registers using 'l' ("low byte" of 16 bits) suffix (original registers - bits 0-7: _l) or 'b' suffix (added registers: r__b): al, bl, r15b
Usage during [syscall](https://wiki.cdot.senecacollege.ca/wiki/Syscalls)/function call:
- First six arguments are in rdi, rsi, rdx, rcx, r8d, r9d; remaining arguments are on the stack.
- For syscalls, the syscall number is in rax.
- Return value is in rax.
- The called routine is expected to preserve rsp,rbp, rbx, r12, r13, r14, and r15 but may trample any other registers.
### Floating-Point and SIMD Registers
x86_64 also defines a set of large registers for floating-point and single-instruction/multiple-data (SIMD) operations. For details, refer to the Intel or AMD documentation.
## Instructions
### Starter Kit
These instructions are sufficient to complete the [SPO600 Assembler Lab](https://wiki.cdot.senecacollege.ca/wiki/SPO600_Assembler_Lab) (GAS syntax):
```
add %r10,%r11    // add r10 and r11, put result in r11
cmp %r10,%r11    // compare register r10 with register r11.  The comparison sets flags in the processor status register which affect conditional jumps.
cmp $99,%r11     // compare the number 99 with register r11.  The comparison sets flags in the processor status register which affect conditional jumps.
div %r10         // divide rax by the given register (r10), places quotient into rax and remainder into rdx (rdx must be zero before this instruction)
inc %r10         // increment r10
jmp label        // jump to label
je  label        // jump to label if equal
jne label        // jump to label if not equal
jl  label        // jump to label if less
jg  label        // jump to label if greater
mov %r10,%r11    // move data from r10 to r11
mov $99,%r10     // put the immediate value 99 into r10
mov %r10,(%r11)  // move data from r10 to address pointed to by r11
mov (%r10),%r11  // move data from address pointed to by r10 to r10
mul %r10         // multiplies rax by r10, places result in rax and overflow in rdx
push %r10        // push r10 onto the stack
pop %r10         // pop r10 off the stack
syscall          // invoke a syscall (in 32-bit mode, use "int $0x80" instead)
```
Note the syntax:
- [Register](https://wiki.cdot.senecacollege.ca/wiki/Register) names are prefixed by %
- [Immediate values](https://wiki.cdot.senecacollege.ca/wiki/Immediate_Value) are prefixed by $
- Indirect memory access is indicated by (parenthesis).
- Hexadecimal values are indicated by a 0x prefix.
- Character values are indicated by quotation marks. Escapes (such as '\n') are permitted.
- Data sources are given as the first argument (mov %r10,%r11 moves FROM r10 INTO r11).
For the MOV instruction:
- You can append a suffix indicating the amount of data to be moved -- e.g., q for quadword (64 bits), d for doubleword (32 bits), w for word (16 bits), or b for byte (8 bits).
## Resources
- CPU Instruction Set and Software Developer Manuals
	- AMD: [http://developer.amd.com/resources/documentation-articles/developer-guides-manuals/](http://developer.amd.com/resources/documentation-articles/developer-guides-manuals/)
- Intel: [http://www.intel.com/content/www/us/en/processors/architectures-software-developer-manuals.html](http://www.intel.com/content/www/us/en/processors/architectures-software-developer-manuals.html)
- Web sites
	- [http://ref.x86asm.net/](http://ref.x86asm.net/)
- [http://sandpile.org/](http://sandpile.org/)
- GAS Manual - Using as, The GNU Assembler: [https://sourceware.org/binutils/docs/as/](https://sourceware.org/binutils/docs/as/)
# x86 Registers
> 
原文地址：[http://www.eecg.toronto.edu/~amza/www.mindsec.com/files/x86regs.html](http://www.eecg.toronto.edu/~amza/www.mindsec.com/files/x86regs.html)
The main tools to write programs in x86 assembly are the processor registers. The registers are like variables built in the processor. Using registers instead of memory to store values makes the process faster and cleaner. The problem with the x86 serie of processors is that there are few registers to use. This section describes the main use of each register and ways to use them. That in note that the rules described here are more suggestions than strict rules. Some operations need absolutely some kind of registers but most of the you can use any of the freely.
Here is a list of the available registers on the 386 and higher processors. This list shows the 32 bit registers. Most of the can be broken down to 16 or even 8 bits register.
```
General registers
EAX EBX ECX EDX
Segment registers
CS DS ES FS GS SS
Index and pointers
ESI EDI EBP EIP ESP
Indicator
EFLAGS
```
## **General registers**
As the title says, general register are the one we use most of the time Most of the instructions perform on these registers. They all can be broken down into 16 and 8 bit registers.
```
32 bits :  EAX EBX ECX EDX
16 bits : AX BX CX DX
 8 bits : AH AL BH BL CH CL DH DL
```
The "H" and "L" suffix on the 8 bit registers stand for high byte and low byte. With this out of the way, let's see their individual main use
```
EAX,AX,AH,AL : Called the Accumulator register. 
               It is used for I/O port access, arithmetic, interrupt calls,
               etc...
EBX,BX,BH,BL : Called the Base register
               It is used as a base pointer for memory access
               Gets some interrupt return values
ECX,CX,CH,CL : Called the Counter register
               It is used as a loop counter and for shifts
               Gets some interrupt values
EDX,DX,DH,DL : Called the Data register
               It is used for I/O port access, arithmetic, some interrupt 
               calls.
```
## **Segment registers**
Segment registers hold the segment address of various items. They are only available in 16 values. They can only be set by a general register or special instructions. Some of them are critical for the good execution of the program and you might want to consider playing with them when you'll be ready for multi-segment programming
```
CS         : Holds the Code segment in which your program runs.
             Changing its value might make the computer hang.
DS         : Holds the Data segment that your program accesses.
             Changing its value might give erronous data.
ES,FS,GS   : These are extra segment registers available for
             far pointer addressing like video memory and such.
SS         : Holds the Stack segment your program uses.
             Sometimes has the same value as DS.
             Changing its value can give unpredictable results,
             mostly data related.
```
## **Indexes and pointers**
Indexes and pointer and the offset part of and address. They have various uses but each register has a specific function. They some time used with a segment register to point to far address (in a 1Mb range). The register with an "E" prefix can only be used in protected mode.
```
ES:EDI EDI DI : Destination index register
                Used for string, memory array copying and setting and
                for far pointer addressing with ES
DS:ESI EDI SI : Source index register
                Used for string and memory array copying
SS:EBP EBP BP : Stack Base pointer register
                Holds the base address of the stack
                
SS:ESP ESP SP : Stack pointer register
                Holds the top address of the stack
CS:EIP EIP IP : Index Pointer
                Holds the offset of the next instruction
                It can only be read
```
## **The EFLAGS register**
The EFLAGS register hold the state of the processor. It is modified by many intructions and is used for comparing some parameters, conditional loops and conditionnal jumps. Each bit holds the state of specific parameter of the last instruction. Here is a listing :
```
Bit   Label    Desciption
---------------------------
0      CF      Carry flag
2      PF      Parity flag
4      AF      Auxiliary carry flag
6      ZF      Zero flag
7      SF      Sign flag
8      TF      Trap flag
9      IF      Interrupt enable flag
10     DF      Direction flag
11     OF      Overflow flag
12-13  IOPL    I/O Priviledge level
14     NT      Nested task flag
16     RF      Resume flag
17     VM      Virtual 8086 mode flag
18     AC      Alignment check flag (486+)
19     VIF     Virutal interrupt flag
20     VIP     Virtual interrupt pending flag
21     ID      ID flag
Those that are not listed are reserved by Intel.
```
## **Undocumented registers**
There are registers on the 80386 and higher processors that are not well documented by Intel. These are divided in control registers, debug registers, test registers and protected mode segmentation registers. As far as I know, the control registers, along with the segmentation registers, are used in protected mode programming, all of these registers are available on 80386 and higher processors except the test registers that have been removed on the pentium. Control registers are CR0 to CR4, Debug registers are DR0 to DR7, test registers are TR3 to TR7 and the protected mode segmentation registers are GDTR (Global Descriptor Table Register), IDTR (Interrupt Descriptor Table Register), LDTR (Local DTR), and TR.
# CPU Registers x86-64
> 
原文地址：[https://wiki.osdev.org/CPU_Registers_x86-64](https://wiki.osdev.org/CPU_Registers_x86-64)
## Contents
			 [[hide](https://wiki.osdev.org/CPU_Registers_x86-64#)] 
			- [1 General Purpose Registers](#General_Purpose_Registers)
- [2 Pointer Registers](#Pointer_Registers)
- [3 Segment Registers](#Segment_Registers)
- [4 RFLAGS Register](#RFLAGS_Register)
- [5 Control Registers](#Control_Registers)- [5.1 CR0](#CR0)
- [5.2 CR2](#CR2)
- [5.3 CR3](#CR3)
- [5.4 CR4](#CR4)
- [5.5 CR8](#CR8)
- [5.6 CR1, CR5-7, CR9-15](#CR1.2C_CR5-7.2C_CR9-15)
- [6 MSRs](#MSRs)- [6.1 IA32_EFER](#IA32_EFER)
- [6.2 FS.base, GS.base](#FS.base.2C_GS.base)
- [6.3 KernelGSBase](#KernelGSBase)
- [7 Debug Registers](#Debug_Registers)- [7.1 DR0 - DR3](#DR0_-_DR3)
- [7.2 DR6](#DR6)
- [7.3 DR7](#DR7)
- [8 Test Registers](#Test_Registers)
- [9 Protected Mode Registers](#Protected_Mode_Registers)- [9.1 GDTR](#GDTR)
- [9.2 LDTR](#LDTR)
- [9.3 TR](#TR)
- [9.4 IDTR](#IDTR)
## General Purpose Registers
|Monikers|Description| | | | |
|----|----|----|----|----|----|
| | | | | | |
|RAX|EAX|AX|AH|AL|Accumulator|
|RBX|EBX|BX|BH|BL|Base|
|RCX|ECX|CX|CH|CL|Counter|
|RDX|EDX|DX|DH|DL|Data (commonly extends the A register)|
|RSI|ESI|SI|N/A|SIL|Source index for string operations|
|RDI|EDI|DI|N/A|DIL|Destination index for string operations|
|RSP|ESP|SP|N/A|SPL|Stack Pointer|
|RBP|EBP|BP|N/A|BPL|Base Pointer (meant for stack frames)|
|R8|R8D|R8W|N/A|R8B|General purpose|
|R9|R9D|R9W|N/A|R9B|General purpose|
|R10|R10D|R10W|N/A|R10B|General purpose|
|R11|R11D|R11W|N/A|R11B|General purpose|
|R12|R12D|R12W|N/A|R12B|General purpose|
|R13|R13D|R13W|N/A|R13B|General purpose|
|R14|R14D|R14W|N/A|R14B|General purpose|
|R15|R15D|R15W|N/A|R15B|General purpose|
Note: you cannot access *AH*, *BH*, *CH* and *DH* when using the REX.W instruction prefix. This prefix is added (automatically by assemblers) when an operand contains a 64-bit register.
## Pointer Registers
|Monikers|Description| | |
|----|----|----|----|
| | | | |
|RIP|EIP|IP|Instruction Pointer|
Note: The instruction pointer can only be used in RIP-relative addressing, which was introduced with long mode.
## Segment Registers
All these are 16 bits long.
|Moniker|Description|
|----|----|
|CS|Code Segment|
|DS|Data Segment|
|SS|Stack Segment|
|ES|Extra Segment (used for string operations)|
|FS|General-purpose Segment|
|GS|General-purpose Segment|
Segments of *CS*, *DS*, *ES*, and *SS* are treated as if their base was 0 no matter what the segment descriptors in the GDT say. Exceptions are *FS* and *GS* which have MSRs to change their base.
Limit checks are disabled for all segments.
## RFLAGS Register
|Bit(s)|Label|Description|
|----|----|----|
|0|CF|Carry Flag|
|1|1|Reserved|
|2|PF|Parity Flag|
|3|0|Reserved|
|4|AF|Auxiliary Carry Flag|
|5|0|Reserved|
|6|ZF|Zero Flag|
|7|SF|Sign Flag|
|8|TF|Trap Flag|
|9|IF|Interrupt Enable Flag|
|10|DF|Direction Flag|
|11|OF|Overflow Flag|
|12-13|IOPL|I/O Privilege Level|
|14|NT|Nested Task|
|15|0|Reserved|
|16|RF|Resume Flag|
|17|VM|Virtual-8086 Mode|
|18|AC|Alignment Check / Access Control|
|19|VIF|Virtual Interrupt Flag|
|20|VIP|Virtual Interrupt Pending|
|21|ID|ID Flag|
|22-63|0|Reserved|
## Control Registers
### CR0
|Bit(s)|Label|Description|
|----|----|----|
|0|PE|Protected Mode Enable|
|1|MP|Monitor Co-Processor|
|2|EM|Emulation|
|3|TS|Task Switched|
|4|ET|Extension Type|
|5|NE|Numeric Error|
|6-15|0|Reserved|
|16|WP|Write Protect|
|17|0|Reserved|
|18|AM|Alignment Mask|
|19-28|0|Reserved|
|29|NW|Not-Write Through|
|30|CD|Cache Disable|
|31|PG|Paging|
|32-63|0|Reserved|
NOTE that this register is the only control register that can be written and read via 2 ways unlike the other that can be accessed only via the MOV instruction
```
;way 1:
;write:
mov cr0,reg32(64)
;read:
mov reg32(64),cr0 
;----------------------
;way 2:
;write:
lmsw reg16(32/64) ; the 'w' in lms(w) stands for word size (16 bit) but the instruction itself can modify the upper 48 bit of cr0 using instruction overrides.
 
;read:
smsw reg16(32/64) ; SAME as above
```
### CR2
This control register contains the linear (virtual) address which triggered a page fault, available in the page fault's interrupt handler.
### CR3
|Bit(s)|Label|Description|Condition| |
|----|----|----|----|----|
|0-11|0-2|0|Reserved|CR4.PCIDE = 0|
|3|PWT|Page-Level Write Through| | |
|5|PCD|Page-Level Cache Disable| | |
|5-11|0|Reserved| | |
|0-11|PCID|CR4.PCIDE = 1| | |
|12-63|Physical Base Address of the PML4| | | |
Note that this must be page aligned
### CR4
|Bit(s)|Label|Description|
|----|----|----|
|0|VME|Virtual-8086 Mode Extensions|
|1|PVI|Protected Mode Virtual Interrupts|
|2|TSD|Time Stamp enabled only in ring 0|
|3|DE|Debugging Extensions|
|4|PSE|Page Size Extension|
|5|PAE|Physical Address Extension|
|6|MCE|Machine Check Exception|
|7|PGE|Page Global Enable|
|8|PCE|Performance Monitoring Counter Enable|
|9|OSFXSR|OS support for fxsave and fxrstor instructions|
|10|OSXMMEXCPT|OS Support for unmasked simd floating point exceptions|
|11|UMIP|User-Mode Instruction Prevention (SGDT, SIDT, SLDT, SMSW, and STR are disabled in user mode)|
|12|0|Reserved|
|13|VMXE|Virtual Machine Extensions Enable|
|14|SMXE|Safer Mode Extensions Enable|
|15|0|Reserved|
|17|PCIDE|PCID Enable|
|18|OSXSAVE|XSAVE And Processor Extended States Enable|
|19|0|Reserved|
|20|SMEP|Supervisor Mode Executions Protection Enable|
|21|SMAP|Supervisor Mode Access Protection Enable|
|22-63|0|Reserved|
### CR8
CR8 is a new register accessible in 64-bit mode using the REX prefix. CR8 is used to prioritize external [interrupts](/Interrupts) and is referred to as the task-priority register (TPR).
The AMD64 architecture allows software to define up to 15 external interrupt-priority classes. Priority classes are numbered from 1 to 15, with priority-class 1 being the lowest and priority-class 15 the highest. CR8 uses the four low-order bits for specifying a task priority and the remaining 60 bits are reserved and must be written with zeros.
System software can use the TPR register to temporarily block low-priority interrupts from interrupting a high-priority task. This is accomplished by loading TPR with a value corresponding to the highest-priority interrupt that is to be blocked. For example, loading TPR with a value of 9 (1001b) blocks all interrupts with a priority class of 9 or less, while allowing all interrupts with a priority class of 10 or more to be recognized. Loading TPR with 0 enables all external interrupts. Loading TPR with 15 (1111b) disables all external interrupts.
The TPR is cleared to 0 on reset.
|Bit|Purpose|
|----|----|
|0-3|Priority|
|4-63|Reserved|
### CR1, CR5-7, CR9-15
Reserved, the cpu will throw a #ud exeption when trying to access them.
## MSRs
### IA32_EFER
Extended Feature Enable Register (EFER) is a [model-specific register](/MSR) added in the AMD K6 processor, to allow enabling the SYSCALL/SYSRET instruction, and later for entering and exiting [long mode](/X86-64). This register becomes architectural in [AMD64](/AMD64) and has been adopted by Intel. Its MSR number is 0xC0000080.
|Bit(s)|Label|Description|
|----|----|----|
|0|SCE|System Call Extensions|
|1-7|0|Reserved|
|8|LME|Long Mode Enable|
|10|LMA|Long Mode Active|
|11|NXE|No-Execute Enable|
|12|SVME|Secure Virtual Machine Enable|
|13|LMSLE|Long Mode Segment Limit Enable|
|14|FFXSR|Fast FXSAVE/FXRSTOR|
|15|TCE|Translation Cache Extension|
|16-63|0|Reserved|
### FS.base, GS.base
MSRs with the addresses 0xC0000100 (for FS) and 0xC0000101 (for GS) contain the base addresses of the FS and GS segment registers. These are commonly used for thread-pointers in user code and CPU-local pointers in kernel code. Safe to contain anything, since use of a segment does not confer additional privileges to user code.
In newer CPUs, these can also be written with WRFSBASE and WRGSBASE instructions at any privilege level.
### KernelGSBase
MSR with the address 0xC0000102. Is basically a buffer that gets exchanged with GS.base after a swapgs instruction. Usually used to seperate kernel and user use of the GS register.
## Debug Registers
### DR0 - DR3
Contain linear addresses of up to 4 breakpoints. If paging is enabled, they are translated to physical addresses.
### DR6
It permits the debugger to determine which debug conditions have occured. When an enabled debug exception is enabled, low order bits 0-3 are set before entering debug exception handler.
### DR7
|Bit|Description|
|----|----|
|0|Local DR0 Breakpoint|
|1|Global DR0 Breakpoint|
|2|Local DR1 Breakpoint|
|3|Global DR1 Breakpoint|
|4|Local DR2 Breakpoint|
|5|Global DR2 Breakpoint|
|6|Local DR3 Breakpoint|
|7|Global DR3 Breakpoint|
|16-17|Conditions for DR0|
|18-19|Size of DR0 Breakpoint|
|20-21|Conditions for DR1|
|22-23|Size of DR1 Breakpoint|
|24-25|Conditions for DR2|
|26-27|Size of DR2 Breakpoint|
|28-29|Conditions for DR3|
|30-31|Size of DR3 Breakpoint|
A local breakpoint bit deactivates on hardware task switches, while a global does not.
00b condition means execution break, 01b means a write watchpoint, and 11b means an R/W watchpoint. 10b is reserved for I/O R/W (unsupported).
## Test Registers
|Name|Description|
|----|----|
|TR3 - TR5|Undocumented|
|TR6|Test Command Register|
|TR7|Test Data Register|
## Protected Mode Registers
### GDTR
|Operand Size|Label|Description| |
|----|----|----|----|
| | | | |
|Bits 0-15|Limit|Size of [GDT](/GDT)| |
|Bits 16-79|Bits 16-47|Base|Starting Address of GDT|
### LDTR
Stores the segment selector of the [LDT](/LDT).
### TR
Stores the segment selector of the [TSS](/TSS).
### IDTR
|Operand Size|Label|Description| |
|----|----|----|----|
| | | | |
|Bits 0-15|Limit|Size of [IDT](/IDT)| |
|Bits 16-79|Bits 16-47|Base|Starting Address of IDT|
Retrieved from "[https://wiki.osdev.org/index.php?title=CPU_Registers_x86-64&oldid=22538](https://wiki.osdev.org/index.php?title=CPU_Registers_x86-64&oldid=22538)"
[Categories](/Special:Categories):
- [CPU Registers](/Category:CPU_Registers)
- [Disputed Pages](/Category:Disputed_Pages)
