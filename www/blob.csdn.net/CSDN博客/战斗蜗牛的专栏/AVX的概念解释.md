# AVX的概念解释 - 战斗蜗牛的专栏 - CSDN博客





2014年08月06日 23:02:08[vbskj](https://me.csdn.net/vbskj)阅读数：3998









**Advanced Vector Extensions** (**AVX**) are extensions to the [x86](http://en.wikipedia.org/wiki/X86)[instruction
 set architecture](http://en.wikipedia.org/wiki/Instruction_set_architecture) for [microprocessors](http://en.wikipedia.org/wiki/Microprocessor) from [Intel](http://en.wikipedia.org/wiki/Intel_Corporation) and [AMD](http://en.wikipedia.org/wiki/Advanced_Micro_Devices) proposed
 by Intel in March 2008 and first supported by Intel with the [Sandy Bridge](http://en.wikipedia.org/wiki/Sandy_Bridge) processor shipping in
 Q1 2011 and later on by AMD with the [Bulldozer](http://en.wikipedia.org/wiki/Bulldozer_(microarchitecture)) processor shipping
 in Q3 2011. AVX provides new features, new instructions and a new coding scheme.

**AVX2** expands most integer commands to 256 bits and introduces [FMA](http://en.wikipedia.org/wiki/Fused_multiply-accumulate). [AVX-512](http://en.wikipedia.org/wiki/AVX-512) expands
 AVX to 512-bit support utilizing a new [EVEX prefix](http://en.wikipedia.org/wiki/EVEX_prefix) encoding proposed by Intel in July 2013 and first
 supported by Intel with the [Knights
 Landing](http://en.wikipedia.org/wiki/Knights_Landing_(microarchitecture)) processor scheduled to ship in 2015.[[1]](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_note-reinders512-1)





## Contents
 [[hide](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#)] 
- [1Advanced
 Vector Extensions 1](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#Advanced_Vector_Extensions_1)- [1.1Applications](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#Applications)
- [1.2New
 instructions](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#New_instructions)
- [1.3CPUs
 with AVX](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#CPUs_with_AVX)
- [1.4Compiler
 and assembler support](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#Compiler_and_assembler_support)
- [1.5Operating
 system support](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#Operating_system_support)

- [2Advanced
 Vector Extensions 2](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#Advanced_Vector_Extensions_2)- [2.1New
 instructions](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#New_instructions_2)
- [2.2CPUs
 with AVX2](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#CPUs_with_AVX2)

- [3AVX-512](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#AVX-512)
- [4See
 also](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#See_also)
- [5References](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#References)




## Advanced Vector Extensions 1[[edit](http://en.wikipedia.org/w/index.php?title=Advanced_Vector_Extensions&action=edit&section=1)]

The width of the [SIMD](http://en.wikipedia.org/wiki/SIMD) register file is increased
 from 128 bits to 256 bits, and renamed from [XMM](http://en.wikipedia.org/wiki/Streaming_SIMD_Extensions#Registers)0–XMM7 to YMM0–YMM7
 (in [x86-64](http://en.wikipedia.org/wiki/X86-64) mode, YMM0–YMM15). In processors with AVX support, the legacy [SSE](http://en.wikipedia.org/wiki/Streaming_SIMD_Extensions) instructions
 (which previously operated on 128-bit XMM registers) can be extended using the [VEX prefix](http://en.wikipedia.org/wiki/VEX_prefix) to operate
 on the lower 128 bits of the YMM registers.
|511256|255128|1270|
|----|----|----|
||||
|ZMM0|YMM0|XMM0|
|ZMM1|YMM1|XMM1|
|ZMM2|YMM2|XMM2|
|ZMM3|YMM3|XMM3|
|ZMM4|YMM4|XMM4|
|ZMM5|YMM5|XMM5|
|ZMM6|YMM6|XMM6|
|ZMM7|YMM7|XMM7|
|ZMM8|YMM8|XMM8|
|ZMM9|YMM9|XMM9|
|ZMM10|YMM10|XMM10|
|ZMM11|YMM11|XMM11|
|ZMM12|YMM12|XMM12|
|ZMM13|YMM13|XMM13|
|ZMM14|YMM14|XMM14|
|ZMM15|YMM15|XMM15|
|ZMM16|YMM16|XMM16|
|ZMM17|YMM17|XMM17|
|ZMM18|YMM18|XMM18|
|ZMM19|YMM19|XMM19|
|ZMM20|YMM20|XMM20|
|ZMM21|YMM21|XMM21|
|ZMM22|YMM22|XMM22|
|ZMM23|YMM23|XMM23|
|ZMM24|YMM24|XMM24|
|ZMM25|YMM25|XMM25|
|ZMM26|YMM26|XMM26|
|ZMM27|YMM27|XMM27|
|ZMM28|YMM28|XMM28|
|ZMM29|YMM29|XMM29|
|ZMM30|YMM30|XMM30|
|ZMM31|YMM31|XMM31|

AVX introduces a three-operand SIMD instruction format, where the destination register is distinct from the two source operands. For example, an [SSE](http://en.wikipedia.org/wiki/Streaming_SIMD_Extensions)instruction
 using the conventional two-operand form *a = a + b* can now use a non-destructive three-operand form *c = a + b*, preserving both source operands. AVX's three-operand format is limited to the instructions with [SIMD](http://en.wikipedia.org/wiki/SIMD) operands
 (YMM), and does not include instructions with general purpose registers (e.g. EAX). Such support will first appear in AVX2.[[2]](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_note-avx2-2)

The [alignment](http://en.wikipedia.org/wiki/Data_structure_alignment) requirement
 of [SIMD](http://en.wikipedia.org/wiki/SIMD) memory operands is relaxed.[*[citation needed](http://en.wikipedia.org/wiki/Wikipedia:Citation_needed)*]

The new [VEX
 coding scheme](http://en.wikipedia.org/wiki/VEX_coding_scheme) introduces a new set of code prefixes that extends the [opcode](http://en.wikipedia.org/wiki/Opcode) space, allows instructions to
 have more than two operands, and allows [SIMD](http://en.wikipedia.org/wiki/SIMD) vector registers to be longer than 128 bits. The VEX prefix can also
 be used on the legacy SSE instructions giving them a three-operand form, and making them interact more efficiently with AVX instructions without the need for VZEROUPPER and ZEROALL.

The AVX instructions support both 128-bit and 256-bit [SIMD](http://en.wikipedia.org/wiki/SIMD).
 The 128-bit versions can be useful to improve old code without needing to widen the vectorization, but can also be used if an AVX-capable processor is detected while the operating system is not AVX 256-bit capable. In that case using the 256-bit registers
 is unsafe, but using AVX on 128-bit registers is still safe; this mode is sometimes known as AVX128.[[3]](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_note-3)

### Applications[[edit](http://en.wikipedia.org/w/index.php?title=Advanced_Vector_Extensions&action=edit&section=2)]
- Suitable for [floating point](http://en.wikipedia.org/wiki/Floating_point)-intensive calculations in multimedia,
 scientific and financial applications ([integer](http://en.wikipedia.org/wiki/Integer) operations are expected in later extensions).
- Increases parallelism and throughput in floating point [SIMD](http://en.wikipedia.org/wiki/SIMD) calculations.
- Reduces register load due to the non-destructive instructions.
- Improves Linux RAID software performance.[[4]](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_note-4)

[Prime95](http://en.wikipedia.org/wiki/Prime95)/MPrime, the software used for [GIMPS](http://en.wikipedia.org/wiki/GIMPS),
 started using the AVX instructions since version 27.x.

### New instructions[[edit](http://en.wikipedia.org/w/index.php?title=Advanced_Vector_Extensions&action=edit&section=3)]

These AVX instructions are in addition to the ones that are 256-bit extensions of the legacy 128-bit SSE instructions; most are usable on both 128-bit and 256-bit operands.
|Instruction|Description|
|----|----|
|`VBROADCASTSS`,`VBROADCASTSD`,`VBROADCASTF128`|Copy a 32-bit, 64-bit or 128-bit memory operand to all elements of a XMM or YMM vector register.|
|`VINSERTF128`|Replaces either the lower half or the upper half of a 256-bit YMM register with the value of a 128-bit source operand. The other half of the destination is unchanged.|
|`VEXTRACTF128`|Extracts either the lower half or the upper half of a 256-bit YMM register and copies the value to a 128-bit destination operand.|
|`VMASKMOVPS`,`VMASKMOVPD`|Conditionally reads any number of elements from a SIMD vector memory operand into a destination register, leaving the remaining vector elements unread and setting the corresponding elements in the destination register to zero. Alternatively, conditionally writes any number of elements from a SIMD vector register operand to a vector memory operand, leaving the remaining elements of the memory operand unchanged. On the AMD Jaguar processor architecture, this instruction with a memory source operand takes more than 300 clock cycles when the mask is zero, in which case the instruction should do nothing. This appears to be a design flaw.[[5]](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_note-5)|
|`VPERMILPS`,`VPERMILPD`|Permute In-Lane. Shuffle the 32-bit or 64-bit vector elements of one input operand. These are in-line 256-bit instructions, meaning that they operate on all 256 bits with two separate 128-bit shuffles, so they can not shuffle across the 128-bit lanes.[[6]](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_note-6)|
|`VPERM2F128`|Shuffle the four 128-bit vector elements of two 256-bit source operands into a 256-bit destination operand, with an immediate constant as selector.|
|`VZEROALL`|Set all YMM registers to zero and tag them as unused. Used when switching between 128-bit use and 256-bit use.|
|`VZEROUPPER`|Set the upper half of all YMM registers to zero. Used when switching between 128-bit use and 256-bit use.|

### CPUs with AVX[[edit](http://en.wikipedia.org/w/index.php?title=Advanced_Vector_Extensions&action=edit&section=4)]
- [Intel](http://en.wikipedia.org/wiki/Intel_Corporation)- [Sandy Bridge](http://en.wikipedia.org/wiki/Sandy_Bridge_(microarchitecture)) processor,
 Q1 2011[[7]](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_note-7)
- [Sandy Bridge
 E](http://en.wikipedia.org/wiki/Sandy_Bridge_(microarchitecture)) processor, Q4 2011[[8]](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_note-8)
- [Ivy Bridge](http://en.wikipedia.org/wiki/Ivy_Bridge_(microarchitecture)) processor, Q1 2012
- [Ivy Bridge E](http://en.wikipedia.org/wiki/Ivy_Bridge_(microarchitecture)) processor, Q3 2013
- [Haswell](http://en.wikipedia.org/wiki/Haswell_(microarchitecture)) processor, Q2 2013
- [Haswell E](http://en.wikipedia.org/wiki/Haswell_(microarchitecture)) processor, Q3 2014
- [Broadwell](http://en.wikipedia.org/wiki/Broadwell_(microarchitecture)) processor, expected in
 2014
- [Broadwell E](http://en.wikipedia.org/wiki/Broadwell_(microarchitecture)) processor, expected
 in 2015

- [AMD](http://en.wikipedia.org/wiki/Advanced_Micro_Devices):
- [Bulldozer](http://en.wikipedia.org/wiki/Bulldozer_(processor))-based processor,
 Q4 2011[[9]](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_note-9)
- [Piledriver](http://en.wikipedia.org/wiki/Piledriver_(microarchitecture))-based processor, Q4
 2012[[10]](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_note-10)
- [Steamroller](http://en.wikipedia.org/wiki/Steamroller_(microarchitecture))-based processor,
 Q1 2014
- [Excavator](http://en.wikipedia.org/wiki/Excavator_(microarchitecture))-based processor, expected
 in 2015
- [Jaguar-based processor](http://en.wikipedia.org/wiki/Jaguar_(microarchitecture))


Issues regarding compatibility between future Intel and AMD processors are discussed under [XOP
 instruction set](http://en.wikipedia.org/wiki/XOP_instruction_set#Compatibility_issues).

### Compiler and assembler support[[edit](http://en.wikipedia.org/w/index.php?title=Advanced_Vector_Extensions&action=edit&section=5)]

Recent releases of [GCC](http://en.wikipedia.org/wiki/GNU_Compiler_Collection) starting
 with version 4.6 (although there was a 4.3 branch with certain support) and the Intel Compiler Suite starting with version 11.1 support AVX. The [Visual
 Studio 2010](http://en.wikipedia.org/wiki/Visual_Studio_2010)/[2012](http://en.wikipedia.org/wiki/Visual_Studio_2010) compiler supports AVX via intrinsic and /arch:AVX
 switch. The [Open64](http://en.wikipedia.org/wiki/Open64) compiler version 4.5.1 supports AVX with -mavx flag. [Absoft](http://en.wikipedia.org/wiki/Absoft_Fortran_Compilers) supports
 with -mavx flag. [PathScale](http://en.wikipedia.org/wiki/PathScale) supports via the -mavx flag. The [Free
 Pascal](http://en.wikipedia.org/wiki/Free_Pascal) compiler supports AVX and AVX2 with the -CfAVX and -CfAVX2 switches from version 2.7.1. The [Vector
 Pascal](http://en.wikipedia.org/wiki/Vector_Pascal) compiler supports AVX via the -cpuAVX32 flag. The [GNU Assembler](http://en.wikipedia.org/wiki/GNU_Assembler) (GAS) inline assembly
 functions support these instructions (accessible via GCC), as do Intel primitives and the Intel inline assembler (closely compatible to GAS, although more general in its handling of local references within inline code). Other assemblers such as [MASM](http://en.wikipedia.org/wiki/MASM) VS2010
 version, [YASM](http://en.wikipedia.org/wiki/YASM),[[11]](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_note-11)[FASM](http://en.wikipedia.org/wiki/FASM), [NASM](http://en.wikipedia.org/wiki/Netwide_Assembler) and [JWASM](http://en.wikipedia.org/wiki/JWASM).

### Operating system support[[edit](http://en.wikipedia.org/w/index.php?title=Advanced_Vector_Extensions&action=edit&section=6)]

AVX adds new register-state through the 256-bit wide YMM register file, so explicit [operating
 system](http://en.wikipedia.org/wiki/Operating_system) support is required to properly save and restore AVX's expanded registers between[context
 switches](http://en.wikipedia.org/wiki/Context_switch); without this, only AVX 128-bit is supported[*[citation needed](http://en.wikipedia.org/wiki/Wikipedia:Citation_needed)*]. The following operating system versions will support AVX 256-bit:
- Apple OS X: Support for AVX added in 10.6.8 (Snow Leopard) update[[12]](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_note-12) released
 on June 23, 2011.
- Linux: supported since kernel version 2.6.30,[[13]](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_note-13) released
 on June 9, 2009.[[14]](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_note-14)
- Windows: supported in [Windows 7](http://en.wikipedia.org/wiki/Windows_7) SP1 and [Windows
 Server 2008 R2](http://en.wikipedia.org/wiki/Windows_Server_2008_R2) SP1,[[15]](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_note-15)[Windows
 8](http://en.wikipedia.org/wiki/Windows_8)
- Windows Server 2008 R2 SP1 with Hyper-V requires a hotfix to support AMD AVX (Opteron 6200 and 4200 series) processors, [KB2568088](http://support.microsoft.com/kb/2568088)
- FreeBSD in a patch submitted on 21 January 2012,[[16]](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_note-16) which
 was included in the 9.1 stable release[[17]](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_note-17)
- DragonFly BSD added support in early 2013.
- Solaris 10 Update 10 and Solaris 11

## Advanced Vector Extensions 2[[edit](http://en.wikipedia.org/w/index.php?title=Advanced_Vector_Extensions&action=edit&section=7)]

Advanced Vector Extensions 2 (AVX2), also known as **Haswell New Instructions**,[[2]](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_note-avx2-2) is
 an expansion of the AVX instruction set introduced in Intel's [Haswell
 microarchitecture](http://en.wikipedia.org/wiki/Haswell_(microarchitecture)). AVX2 makes the following additions:
- expansion of most vector integer SSE and AVX instructions to 256 bits
- three-operand general-purpose bit manipulation and multiply
- three-operand [fused multiply-accumulate](http://en.wikipedia.org/wiki/FMA_instruction_set) support (FMA3)
- [Gather](http://en.wikipedia.org/wiki/Gather-scatter_(vector_addressing)) support, enabling
 vector elements to be loaded from non-contiguous memory locations
- DWORD- and QWORD-granularity any-to-any permutes
- vector shifts.

### New instructions[[edit](http://en.wikipedia.org/w/index.php?title=Advanced_Vector_Extensions&action=edit&section=8)]
|Instruction|Description|
|----|----|
|`VBROADCASTSS`,`VBROADCASTSD`|Copy a 32-bit or 64-bit register operand to all elements of a XMM or YMM vector register. These are register versions of the same instructions in AVX1. There is no 128-bit version however, but the same effect can be simply achieved using VINSERTF128.|
|`VPBROADCASTB`,`VPBROADCASTW`,`VPBROADCASTD`,`VPBROADCASTQ`|Copy an 8, 16, 32 or 64-bit integer register of memory operand to all elements of a XMM or YMM vector register.|
|`VGATHERDPD`,`VGATHERQPD`,`VGATHERDPS`,`VGATHERQPS`|[Gathers](http://en.wikipedia.org/wiki/Gather-scatter_(vector_addressing)) single or double precision floating point values using either 32 or 64-bit indices and scale.|
|`VPGATHERDD`,`VPGATHERDQ`,`VPGATHERQD`,`VPGATHERQQ`|Gathers 32 or 64-bit integer values values using either 32 or 64-bit indices and scale.|
|`VINSERTI128`|Replaces either the lower half or the upper half of a 256-bit YMM register with the value of a 128-bit source operand. The other half of the destination is unchanged.|
|`VEXTRACTI128`|Extracts either the lower half or the upper half of a 256-bit YMM register and copies the value to a 128-bit destination operand.|
|`VPMASKMOVD`,`VPMASKMOVQ`|Conditionally reads any number of elements from a SIMD vector memory operand into a destination register, leaving the remaining vector elements unread and setting the corresponding elements in the destination register to zero. Alternatively, conditionally writes any number of elements from a SIMD vector register operand to a vector memory operand, leaving the remaining elements of the memory operand unchanged.|
|`VPERMPS`, `VPERMD`|Shuffle the eight 32-bit vector elements of one 256-bit source operand into a 256-bit destination operand, with a register or memory operand as selector.|
|`VPERMPD`, `VPERMQ`|Shuffle the four 64-bit vector elements of one 256-bit source operand into a 256-bit destination operand, with a register or memory operand as selector.|
|`VPERM2I128`|Shuffle the four 128-bit vector elements of two 256-bit source operands into a 256-bit destination operand, with an immediate constant as selector.|
|`VPBLENDD`|Doubleword immediate version of the PBLEND instructions from [SSE4](http://en.wikipedia.org/wiki/SSE4).|
|`VPSLLVD`, `VPSLLVQ`|Shift left logical. Allows variable shifts where each element is shifted according to the packed input.|
|`VPSRLVD`, `VPSRLVQ`|Shift right logical. Allows variable shifts where each element is shifted according to the packed input.|
|`VPSRAVD`|Shift right arithmetically. Allows variable shifts where each element is shifted according to the packed input.|

### CPUs with AVX2[[edit](http://en.wikipedia.org/w/index.php?title=Advanced_Vector_Extensions&action=edit&section=9)]
- [Intel](http://en.wikipedia.org/wiki/Intel_Corporation)- [Haswell](http://en.wikipedia.org/wiki/Haswell_(microarchitecture)) processor, Q2 2013
- [Haswell E](http://en.wikipedia.org/wiki/Haswell_(microarchitecture)) processor, Q3 2014
- [Broadwell](http://en.wikipedia.org/wiki/Broadwell_(microarchitecture)) processor, expected in
 2014
- [Broadwell E](http://en.wikipedia.org/wiki/Broadwell_(microarchitecture)) processor, expected
 in 2015
- [Skylake](http://en.wikipedia.org/wiki/Skylake_(microarchitecture)) processor, expected in 2015
- [Cannonlake](http://en.wikipedia.org/wiki/Cannonlake) processor, expected in 2016

- [AMD](http://en.wikipedia.org/wiki/Advanced_Micro_Devices)- [Excavator](http://en.wikipedia.org/wiki/Excavator_(microarchitecture))-based processor, expected
 in 2015


## AVX-512[[edit](http://en.wikipedia.org/w/index.php?title=Advanced_Vector_Extensions&action=edit&section=10)]


Main article: [AVX-512](http://en.wikipedia.org/wiki/AVX-512)

*AVX-512* are 512-bit extensions to the 256-bit Advanced Vector Extensions SIMD instructions for x86 instruction set architecture proposed by [Intel](http://en.wikipedia.org/wiki/Intel_Corporation) in
 July 2013, and scheduled to be supported in 2015 with Intel's [Knights
 Landing](http://en.wikipedia.org/wiki/Knights_Landing_(microarchitecture)) processor.[[1]](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_note-reinders512-1)

AVX-512 consists of multiple extensions not all meant to be supported by all processors implementing them. Only the core extension AVX-512F (AVX-512 Foundation) is required by all implementations.

The instruction set consists of the following:
- AVX-512 Foundation – expands most 32-bit and 64-bit based AVX instructions with EVEX coding scheme to support 512-bit registers, operation masks, parameter broadcasting, and embedded rounding and exception control
- AVX-512 Conflict Detection Instructions (CDI) – efficient conflict detection to allow more loops to be vectorized, supported by Knights Landing[[1]](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_note-reinders512-1)
- AVX-512 Exponential and Reciprocal Instructions (ERI) – exponential and reciprocal operations designed to help implement transcendental operations, supported by Knights Landing[[1]](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_note-reinders512-1)
- AVX-512 Prefetch Instructions (PFI) – new prefetch capabilities, supported by Knights Landing[[1]](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_note-reinders512-1)

## See also[[edit](http://en.wikipedia.org/w/index.php?title=Advanced_Vector_Extensions&action=edit&section=11)]
- [Memory Protection
 Extensions](http://en.wikipedia.org/wiki/Memory_Protection_Extensions)

## References[[edit](http://en.wikipedia.org/w/index.php?title=Advanced_Vector_Extensions&action=edit&section=12)]

- ^ [Jump
 up to:***a***](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_ref-reinders512_1-0)[***b***](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_ref-reinders512_1-1)[***c***](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_ref-reinders512_1-2)[***d***](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_ref-reinders512_1-3)[***e***](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_ref-reinders512_1-4)James
 Reinders (23 July 2013), [*AVX-512 Instructions*](http://software.intel.com/en-us/blogs/2013/avx-512-instructions), [Intel](http://en.wikipedia.org/wiki/Intel),
 retrieved 20 August 2013
- ^ [Jump
 up to:***a***](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_ref-avx2_2-0)[***b***](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_ref-avx2_2-1)[*Haswell New Instruction Descriptions Now Available*](http://software.intel.com/en-us/blogs/2011/06/13/haswell-new-instruction-descriptions-now-available/), Software.intel.com, retrieved 2012-01-17
- **[Jump up^](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_ref-3)**["i386
 and x86-64 Options - Using the GNU Compiler Collection (GCC)"](http://gcc.gnu.org/onlinedocs/gcc-4.8.2/gcc/i386-and-x86-64-Options.html#i386-and-x86-64-Options). Retrieved 2014-02-09.
- **[Jump up^](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_ref-4)**["Linux
 RAID"](https://git.kernel.org/?p=linux/kernel/git/torvalds/linux-2.6.git;a=commitdiff;h=2c935842bdb46f5f557426feb4d2bdfdad1aa5f9). LWN. 2013-02-17.
- **[Jump up^](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_ref-5)**["The
 microarchitecture of Intel, AMD and VIA CPUs - An optimization guide for assembly programmers and compiler makers"](http://www.agner.org/optimize/microarchitecture.pdf). Retrieved May 2014.
- **[Jump up^](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_ref-6)**["Checssprograming
 AVX2"](https://chessprogramming.wikispaces.com/AVX2). Retrieved March 2014.
- **[Jump up^](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_ref-7)**["Intel
 Offers Peek at Nehalem and Larrabee"](http://www.extremetech.com/computing/80772-intel-offers-peek-at-nehalem-and-larrabee). ExtremeTech. 2008-03-17.
- **[Jump up^](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_ref-8)**["Intel
 Core i7-3960X Processor Extreme Edition"](http://ark.intel.com/products/63696/Intel-Core-i7-3960X-Processor-Extreme-Edition-%2815M-Cache-3_30-GHz%29). Retrieved 2012-01-17.
- **[Jump up^](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_ref-9)**Dave Christie (2009-05-07), [*Striking a balance*](http://developer.amd.com/2009/05/06/striking-a-balance/), AMD Developer blogs, retrieved 2012-01-17
- **[Jump up^](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_ref-10)**[*New "Bulldozer" and "Piledriver" Instructions*](http://developer.amd.com/wordpress/media/2012/10/New-Bulldozer-and-Piledriver-Instructions.pdf), AMD, October 2012
- **[Jump up^](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_ref-11)**YASM 0.7.0 Release Notes [http://yasm.tortall.net/releases/Release0.7.0.html](http://yasm.tortall.net/releases/Release0.7.0.html)
- **[Jump up^](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_ref-12)**[*Twitter*](https://twitter.com/#!/comex/status/85401002349576192),
 retrieved 2010-06-23[*[unreliable source?](http://en.wikipedia.org/wiki/Wikipedia:Identifying_reliable_sources)*]
- **[Jump up^](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_ref-13)**[*x86: add linux kernel support for YMM state*](http://git.kernel.org/linus/a30469e7921a6dd2067e9e836d7787cfa0105627), retrieved 2009-07-13
- **[Jump up^](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_ref-14)**[*Linux 2.6.30 - Linux Kernel Newbies*](http://kernelnewbies.org/Linux_2_6_30), retrieved 2009-07-13
- **[Jump up^](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_ref-15)**[*Floating-Point Support for 64-Bit Drivers*](http://msdn.microsoft.com/en-us/library/ff545910.aspx), retrieved 2009-12-06
- **[Jump up^](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_ref-16)**[*Add support for the extended FPU states on amd64, both for native 64bit and 32bit ABIs*](http://svnweb.freebsd.org/base?view=revision&revision=230426), svnweb.freebsd.org, 2012-01-21, retrieved 2012-01-22
- **[Jump up^](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#cite_ref-17)**["FreeBSD
 9.1-RELEASE Announcement"](http://www.freebsd.org/releases/9.1R/announce.html). Retrieved 2013-05-20.

|[[hide](http://en.wikipedia.org/wiki/Advanced_Vector_Extensions#)]- [v](http://en.wikipedia.org/wiki/Template:Multimedia_extensions)- [t](http://en.wikipedia.org/wiki/Template_talk:Multimedia_extensions)- [e](http://en.wikipedia.org/w/index.php?title=Template:Multimedia_extensions&action=edit)Multimedia extensions|[RISC](http://en.wikipedia.org/wiki/Reduced_instruction_set_computing)|[x86](http://en.wikipedia.org/wiki/X86) (current)|[x86](http://en.wikipedia.org/wiki/X86) (planned)|
|----|----|----|----|
| | | | |
|| | | |
|- [PA-RISC](http://en.wikipedia.org/wiki/PA-RISC)[Multimedia Acceleration eXtensions (MAX)](http://en.wikipedia.org/wiki/Multimedia_Acceleration_eXtensions)- [SPARC](http://en.wikipedia.org/wiki/SPARC)[Visual Instruction Set (VIS)](http://en.wikipedia.org/wiki/Visual_Instruction_Set)- [MIPS](http://en.wikipedia.org/wiki/MIPS_architecture)[MDMX](http://en.wikipedia.org/wiki/MDMX)[MIPS-3D](http://en.wikipedia.org/wiki/MIPS-3D)- [Alpha](http://en.wikipedia.org/wiki/DEC_Alpha)[Motion Video Instructions (MVI)](http://en.wikipedia.org/wiki/DEC_Alpha#Motion_Video_Instructions_.28MVI.29)- [Power Architecture](http://en.wikipedia.org/wiki/Power_Architecture)[AltiVec](http://en.wikipedia.org/wiki/AltiVec)- [ARM](http://en.wikipedia.org/wiki/ARM_architecture)[NEON](http://en.wikipedia.org/wiki/ARM_architecture#Advanced_SIMD_.28NEON.29)| | | |
|| | | |
|- [MMX](http://en.wikipedia.org/wiki/MMX_(instruction_set)) (1996)- *[3DNow!](http://en.wikipedia.org/wiki/3DNow!)* (1998)- [Streaming SIMD Extensions (SSE)](http://en.wikipedia.org/wiki/Streaming_SIMD_Extensions) (1999)- [SSE2](http://en.wikipedia.org/wiki/SSE2) (2001)- [SSE3](http://en.wikipedia.org/wiki/SSE3) (2004)- [Supplemental SSE3 (SSSE3)](http://en.wikipedia.org/wiki/SSSE3) (2006)- [SSE4](http://en.wikipedia.org/wiki/SSE4) (2006)- [SSE5](http://en.wikipedia.org/wiki/SSE5)~~(2007)~~- [Advanced Encryption Standard (AES)](http://en.wikipedia.org/wiki/AES_instruction_set) (2008)- Advanced Vector Extensions (AVX) (2008)- [F16C](http://en.wikipedia.org/wiki/F16C) (2009 (AMD), 2011 (Intel))- *[XOP](http://en.wikipedia.org/wiki/XOP_instruction_set)* (2009)- [FMA instructions](http://en.wikipedia.org/wiki/FMA_instruction_set) (*FMA4*: 2011, *FMA3*: 2012 (AMD), 2013 (Intel))- [Bit manipulation instructions](http://en.wikipedia.org/wiki/Bit_Manipulation_Instruction_Sets) (*ABM*: 2007, *BMI1*: 2012, *BMI2*: 2013, *TBM*: 2012)| | | |
|| | | |
|- [AVX-512](http://en.wikipedia.org/wiki/AVX-512) (2015)| | | |
|| | | |
|x86 : Instructions (Year Introduced); *Italics* = AMD exclusive; ~~Year~~ = Superseded| | | |



[Categories](http://en.wikipedia.org/wiki/Help:Category): 
- [X86 instructions](http://en.wikipedia.org/wiki/Category:X86_instructions)
- [SIMD computing](http://en.wikipedia.org/wiki/Category:SIMD_computing)
- [Advanced Micro Devices technologies](http://en.wikipedia.org/wiki/Category:Advanced_Micro_Devices_technologies)





