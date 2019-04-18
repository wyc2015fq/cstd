# PC平台的SIMD支持检测 - TaigaComplex求职中 - 博客园







# [PC平台的SIMD支持检测](https://www.cnblogs.com/TaigaCon/p/7882216.html)





如果我们希望在用SIMD来提升程序处理的性能，首先需要做的就是检测程序所运行的平台是否支持相应的SIMD扩展。平台对SIMD扩展分为两部分的支持：
- CPU对SIMD扩展的支持。SIMD扩展是随着CPU的发展不断改进的，CPU为SIMD扩展提供了硬件上的最基本支持。
- 操作系统对SIMD扩展的支持。目前PC上运行的基本都是多任务操作系统，也就是会“同时”运行着多个程序，这些程序依靠操作系统进行任务调度以使得多个程序看起来是在同时运行，在进行任务调度时需要进行程序上下文的切换，其中就包括寄存器内容的保存与恢复。操作系统对SIMD扩展的支持总的来说主要是在任务上下文切换时，对该SIMD扩展所使用的寄存器的保存与恢复。





# CPU Identification

检查平台对SIMD扩展的支持，必不可少的就是指令CPUID。CPUID即CPU Identification，通过该指令能获取到CPU相关的各种信息，其中包括CPU制造商、CPU版本、CPU串号、对各种扩展的支持、Cache相关信息等等。

如果EFLAGS寄存器的ID flag（bit 21）能被置1或者置零的话，则表明能使用CPUID指令。CPUID没有任何操作数，不过CPUID却是一个功能性的指令，有输入以及输出。
- 输入用的寄存器为EAX（有时也会用到ECX作为扩展输入），用于指定CPUID的功能。在执行CPUID指令前需要往EAX寄存器写入相应的值。
- 输出用的寄存器为EAX、EBX、ECX、EDX共四个。在CPUID指令执行后可以从这四个寄存器中获取到所需要的信息。

如下所示为CPUID的一些基本功能
|**Initial EAX Value**|**Information Provided about the Processor**| | | | | | | | | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
|Basic CPUID Information| | | | | | | | | | | | | | | | |
|00H||EAX|Maximum Input Value for Basic CPUID information.||----|----||EBX|"Genu"||ECX|"ntel"||EDX|"inel"||EAX|Maximum Input Value for Basic CPUID information.|EBX|"Genu"|ECX|"ntel"|EDX|"inel"| | | | | | | |
|EAX|Maximum Input Value for Basic CPUID information.| | | | | | | | | | | | | | | |
|EBX|"Genu"| | | | | | | | | | | | | | | |
|ECX|"ntel"| | | | | | | | | | | | | | | |
|EDX|"inel"| | | | | | | | | | | | | | | |
|01H||EAX|Version Information: Type,Family,Model, and Stepping ID.| ||----|----|----||EBX|Bits 07-00:|Brand Index.||Bits 15-08:|CLFLUSH line size(Value*8 = cache line size in bytes; used also by CLFLUSHOPT).| ||Bits 23-16:|Maximum number of addressable IDs for logical processors in this physical package.| ||Bits 31-24:|Initial APIC ID.| ||ECX|Feature Information.| ||EDX|Feature Information.| ||EAX|Version Information: Type,Family,Model, and Stepping ID.|EBX|Bits 07-00:|Brand Index.|Bits 15-08:|CLFLUSH line size(Value*8 = cache line size in bytes; used also by CLFLUSHOPT).|Bits 23-16:|Maximum number of addressable IDs for logical processors in this physical package.|Bits 31-24:|Initial APIC ID.|ECX|Feature Information.|EDX|Feature Information.|
|EAX|Version Information: Type,Family,Model, and Stepping ID.| | | | | | | | | | | | | | | |
|EBX|Bits 07-00:|Brand Index.| | | | | | | | | | | | | | |
|Bits 15-08:|CLFLUSH line size(Value*8 = cache line size in bytes; used also by CLFLUSHOPT).| | | | | | | | | | | | | | | |
|Bits 23-16:|Maximum number of addressable IDs for logical processors in this physical package.| | | | | | | | | | | | | | | |
|Bits 31-24:|Initial APIC ID.| | | | | | | | | | | | | | | |
|ECX|Feature Information.| | | | | | | | | | | | | | | |
|EDX|Feature Information.| | | | | | | | | | | | | | | |
|02H||EAX|Cache and TLB Information.||----|----||EBX|Cache and TLB Information.||ECX|Cache and TLB Information.||EDX|Cache and TLB Information.||EAX|Cache and TLB Information.|EBX|Cache and TLB Information.|ECX|Cache and TLB Information.|EDX|Cache and TLB Information.| | | | | | | |
|EAX|Cache and TLB Information.| | | | | | | | | | | | | | | |
|EBX|Cache and TLB Information.| | | | | | | | | | | | | | | |
|ECX|Cache and TLB Information.| | | | | | | | | | | | | | | |
|EDX|Cache and TLB Information.| | | | | | | | | | | | | | | |
|03H||EAX|Reserved.||----|----||EBX|Reserved.||ECX|Bit 00-31 of 96 bit processor serial number.(Available in P3 only, otherwise reserved.)||EDX|Bit 32-64 of 96 bit processor serial number.(Available in P3 only, otherwise reserved.)||EAX|Reserved.|EBX|Reserved.|ECX|Bit 00-31 of 96 bit processor serial number.(Available in P3 only, otherwise reserved.)|EDX|Bit 32-64 of 96 bit processor serial number.(Available in P3 only, otherwise reserved.)| | | | | | | |
|EAX|Reserved.| | | | | | | | | | | | | | | |
|EBX|Reserved.| | | | | | | | | | | | | | | |
|ECX|Bit 00-31 of 96 bit processor serial number.(Available in P3 only, otherwise reserved.)| | | | | | | | | | | | | | | |
|EDX|Bit 32-64 of 96 bit processor serial number.(Available in P3 only, otherwise reserved.)| | | | | | | | | | | | | | | |
|...| | | | | | | | | | | | | | | | |





# 检查CPU对SIMD扩展的支持

通过指令CPUID可以检测CPU对SIMD扩展的支持。在输入为EAX=01H的情况下执行CPUID，可以使得ECX以及EDX返回如下信息：

![image](https://images2017.cnblogs.com/blog/421096/201711/421096-20171123005228602-400789613.png)

![image](https://images2017.cnblogs.com/blog/421096/201711/421096-20171123005229555-840539813.png)



其中与SIMD扩展硬件支持的相关bit如下，当相应的bit为1时表示支持该扩展：
|**Register**|**Bit**|**Feature**|
|----|----|----|
|ECX|0|SSE3|
|9|SSSE3| |
|12|FMA| |
|19|SSE4.1| |
|20|SSE4.2| |
|25|AES| |
|28|AVX| |
|EDX|23|MMX|
|25|SSE| |
|26|SSE2| |





# 检查操作系统对SIMD扩展的支持

程序通过访问寄存器XCR0（eXterned Control Register）可以得到操作系统对SIMD扩展的支持信息。该寄存器通过XSETBV进行设置，通过XGETBV进行读取。

回顾上一小节，可以看到EAX=01H CPUID.ECX的26、27bit分别为XSAVE以及OSXSAVE。其中XSAVE为1则表示存在XCR0寄存器，并且可以通过XSETBV以及XGETBV访问该寄存器。对操作系统来说，会先查看处理器是否支持XSAVE，如果支持，则会根据操作系统自身的实现情况去设置XCR0寄存器。不过，操作系统与一般的程序有不同的权限，操作系统可以通过设置CR4寄存器的bit8（CR4.OSXSAVE）来控制一般程序对XCR0的访问权限，CPUID得到的OSXSAVE（bit27）表示的就是操作系统是否设置了允许一般程序去访问XCR0寄存器，我们一般的程序只需要去判断这一个bit就知道是否能访问XCR0。
|**Register**|**Bit**|**Feature**|**Description**|
|----|----|----|----|
|ECX|26|XSAVE|为1则表明该处理器支持XSAVE/XRSTOR，支持扩展state，支持XSETBV/XGETBV之类，支持XCR0寄存器|
|27|OSXSAVE|为1则表明操作系统允许一般程序通过XSETBV/SGETBV访问XCR0寄存器，允许一般程序通过XSAVE/XRSTOR访问扩展state| |



如果OSXSAVE=1，则可以通过XGETBV指令访问XCR0寄存器，得到系统对SIMD扩展的支持信息。指令XGETBV同样也没有任何的操作数，不过也存在输入与输出。其中输入为ECX，用于指定XCR寄存器，一般只有XCR0，即ECX=0。输出有64bit，保存于EDX:EAX。

XGETBV的输出，即返回值的各个bit有如下指示：

![image](https://images2017.cnblogs.com/blog/421096/201711/421096-20171123004750649-372239041.png)
|**Register**|**Bit**|**Feature**|**Description**|
|----|----|----|----|
|EAX|0|MMX/FPU|这个bit必为1，表明操作系统支持MMX以及ST寄存器|
|1|SSE|为1则表明操作系统支持XMM寄存器，32位时为XMM0~XMM7，64位时为XMM0~XMM15| |
|2|AVX|为1则表明操作系统支持YMM寄存器，32位时为YMM0~YMM7，64位时为YMM0~YMM15| |
|6|ZMM_Hi265|为1则表明操作系统支持ZMM寄存器，32位时为ZMM0~ZMM7，64位时为ZMM0~ZMM15| |
|7|Hi16_ZMM|为1则表明如果CPU工作在64位模式，则操作系统支持ZMM16~ZMM31| |





# 总结

查看平台对SIMD扩展的支持需要分别检查处理器以及操作系统对SIMD扩展的支持。
- 处理器：调用EAX=1 CPUID，然后查看所返回的ECX或EDX寄存器相应的bit。
- 操作系统：调用EAX=1 CPUID，然后查看ECX.OSXSAVE（bit27），如果为1则调用XGETBV去获取XCR0，最后查看返回值EAX上相应的bit。 

GCC:
#include <stdio.h>

inline static void cpuid (unsigned int output[4], unsigned int EAX, unsigned int ECX) {	
    unsigned int a, b, c, d;
    __asm("cpuid"  : "=a"(a),"=b"(b),"=c"(c),"=d"(d) : "a"(EAX),"c"(ECX) : );
    output[0] = a;
    output[1] = b;
    output[2] = c;
    output[3] = d;
}

inline static unsigned int xgetbv (unsigned int ECX) {
    unsigned int ret = 0;
    __asm("xgetbv" : "=a"(ret) : "c"(ECX) : );
    return ret;
}

int main(){
    unsigned int CPUInfo[4] = {0}, ECX = 0, EDX = 0, XCR0_EAX = 0;
    cpuid(CPUInfo, 1, 0);
    ECX = CPUInfo[2];
    EDX = CPUInfo[3];

    if(EDX & 0x00800000)
        printf("CPU Support MMX\n");
    if(EDX & 0x02000000)
        printf("CPU Support SSE\n");
    if(EDX & 0x04000000)
        printf("CPU Support SSE2\n");
    if(ECX & 1)
        printf("CPU Support SSE3\n");
    if(ECX & 0x00000200)
        printf("CPU Support SSSE3\n");
    if(ECX & 0x00080000)
        printf("CPU Support SSE4.1\n");
    if(ECX & 0x00100000)
        printf("CPU Support SSE4.2\n");
    if(ECX & 0x02000000)
        printf("CPU Support AES\n");
    if(ECX & 0x10000000)
        printf("CPU Support AVX\n");
    if(ECX & 0x08000000)
        printf("OS Support XSAVE\n");
    else{
        printf("OS not Support XSAVE, OS not Support SIMD\n");
        return -1;
    }

    XCR0_EAX = xgetbv(0);
    if(XCR0_EAX & 0x00000002)
        printf("OS Support SSE/SSE2/SSE3/SSE4\n");
    if(XCR0_EAX & 0x00000004)
        printf("OS Support AVX\n");
    if(XCR0_EAX & 0x00000040)
        printf("OS Support AVX-512\n");

    printf("ECX=%x, EDX=%x, XCR0_EAX=%x\n", ECX, EDX, XCR0_EAX);
    return 0;
}

YASM:
global SIMD_Support

;this code just check SSE3 support
;bit0:SSE3, bit27:OSXSAVE
%define CPU_SUPPORT_CONST_ECX 0x08000001
;bit23:MMX, bit25:SSE, bit26:SSE2
%define CPU_SUPPORT_CONST_EDX 0x06800000

;bit2:XMM
%define OS_SUPPORT_CONST 2

SIMD_Support: 
    ;processor supports
    mov eax, 1
    cpuid
    and ecx, CPU_SUPPORT_CONST_ECX
    cmp ecx, CPU_SUPPORT_CONST_ECX
    jne not_supported 
    and edx, CPU_SUPPORT_CONST_EDX
    cmp edx, CPU_SUPPORT_CONST_EDX
    jne not_supported 

    ;OS supports
    mov ecx, 0
    XGETBV; result in EDX:EAX
    and eax, OS_SUPPORT_CONST
    cmp eax, OS_SUPPORT_CONST
    jne not_supported

    mov eax, 0 ; return 0
    ret

not_supported:
    mov eax, -1 ; return -1
    ret



**Reference:**

[Intel 64 and IA-32 Architectures Software Developer's Manual](https://www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-software-developer-manual-325462.html)












