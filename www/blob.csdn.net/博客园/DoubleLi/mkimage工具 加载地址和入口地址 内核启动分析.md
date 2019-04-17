# mkimage工具 加载地址和入口地址 内核启动分析 - DoubleLi - 博客园







第三章第二节 mkimage工具
制作Linux内核的压缩镜像文件，需要使用到mkimage工具。mkimage这个工具位于u-boot-2013. 04中的tools目录下，它可以用来制作不压缩或者压缩的多种可启动镜像文件。mkimage在制作镜像文件的时候，是在原来的可执行镜像文件的前面加上一个16个byte（0x40）的头，用来记录参数所指定的信息，这样u-boot才能识别出制作出来的这个镜像是针对哪一个CPU体系结构、哪一种OS、哪种类型、加载到内存中的哪个位置、入口点在内存的哪个位置以及镜像名是什么等信息。在/u-boot-2013.04/tools目录下执行./mkimage，输出信息如下所示：
zhuzhaoqi@zhuzhaoqi-desktop:~/u-boot/u-boot-2013.04/u-boot-2013.04/tools$./mkimage
Usage: ./mkimage -l image
          -l==> list image header information
       ./mkimage[-x] -A arch -O os -T type -C comp -a addr -e ep -n name -d data_file[:data_file...] image
          -A==> set architecture to 'arch'
          -O==> set operating system to 'os'
          -T==> set image type to 'type'
          -C==> set compression type 'comp'
          -a==> set load address to 'addr' (hex)
          -e==> set entry point to 'ep' (hex)
          -n==> set image name to 'name'
          -d==> use image data from 'datafile'
          -x==> set XIP (execute in place)
       ./mkimage[-D dtc_options] -f fit-image.its fit-image
       ./mkimage-V ==> print version information and exit


表3. 1  CPU体系结构



取值

表示的体系结构
取值
表示的体系结构
alpha
Alpha
arm
ARM
x86
Intel x86
ia64
IA64
mips
MIPS
mips64
MIPS 64 Bit
ppc
PowerPC
s390
IBM S390
sh
SuperH
sparc
SPARC
sparc64
SPARC 64 Bit
m68k
MC68000




   针对上面的输出信息，-A 指定CPU的体系结构，也就是说，arch的取值可以是如表3. 1所示。
-O 指定操作系统类型，os可以取：openbsd、netbsd、freebsd、4_4bsd、linux、svr4、esix、solaris、irix、sco、dell、ncr、lynxos、vxworks、psos、qnx、u-boot、rtems、artos。
-T 指定镜像类型，type可以是：standalone、kernel、ramdisk、multi、firmware、script、filesystem。
-C 指定镜像压缩方式，comp可以是：none（不压缩）、gzip（ 用gzip的压缩方式）、bzip2 （用bzip2的压缩方式）。
-a 指定镜像在内存中的加载地址，镜像下载到内存中时，要按照用mkimage制作镜像时，这个参数所指定的地址值来下载。
-e 指定镜像运行的入口点地址，这个地址就是-a参数指定的值加上0x40（因为前面有个mkimage添加的0x40个字节的头）。
-n 指定镜像名。
-d 指定制作镜像的源文件。
将u-boot-2013.04下的tools这个文件夹下中的mkimage工具复制到ubuntu系统的/user/bin下，这样可以直接当作操作命令使用。


第三章第三节    加载地址和入口地址

在上一节中，无法启动内核，导致的原因可能是加载地址、入口地址等导致的。执行./mkimage之后如下：
zhuzhaoqi@zhuzhaoqi-desktop:~/u-boot/u-boot-2013.04/u-boot-2013.04/tools$ ./mkimage
./mkimage [-x] -A arch -O os -T type -C comp -a addr -e ep -n name -d data_file[:data_file...] image
其中-a addr指的就是镜像在内存中的加载地址，镜像下载到内存中时，要按照用mkimage制作镜像时，这个参数所指定的地址值来下载。
而-e ep 是指定镜像运行的入口点地址。
还有两个概念需要明白，即是bootm address和kernel运行地址。bootm address：通过uboot的bootm命令，从address启动kernel。kernel运行地址：在具体mach目录中的Makefile.boot中指定，是kernel启动后实际运行的物理地址。
如果bootm address和Load Address相等，在这种情况下，bootm不会对uImage header后的zImage进行memory move的动作，而会直接go到Entry Point开始执行。因此此时的Entry Point必须设置为Load Address+ 0x40。如果kernel boot过程没有到uncompressing the kernel，就可能是这里设置不对。它们之间的关系为：boom address == Load Address == Entry Point - 0x40。
如果bootm address和Load Address不相等（但需要避免出现memory move时出现覆盖导致zImage被破坏的情况）。此种情况下，bootm会把uImage header后的zImage文件move到Load Address，然后go到entry point开始执行。这段代码在common/cmd_bootm.c中bootm_load_os函数中，如下程序所示。由此知道此时的Load Address必须等于Entry Point。它们之间的关系则为：boom address != Load Address == Entry Point。
    case IH_COMP_NONE:
        if (load == blob_start || load == image_start)
{
            printf("   XIP %s ... ", type_name);
            no_overlap = 1;
        }
else
{
            printf("   Loading %s ... ", type_name);
            memmove_wd((void *)load, (void *)image_start,
                    image_len, CHUNKSZ);
        }
        *load_end = load + image_len;
        puts("OK\n");
        break;
zImage的头部有地址无关的自解压程序，因此刚开始执行的时候，zImage所在的内存地址（Entry Point）不需要同编译kernel的地址相同。自解压程序会把kernel解压到编译时指定的物理地址，然后开始地址相关代码的执行。在开启MMU之前，kernel都是直接使用物理地址（可参看内核符号映射表System.map）。
通过上面的分析，大概找出了问题的根源，由于bootm address和Load Address都为50008000，属于相等情况，也就是说Entry Point:  50008000，这个地址需要修改，替换成50008040。
找到Load Address和Entry Point这两个地址的定义，存在于scripts/makefile.lib中，
zhuzhaoqi@zhuzhaoqi-desktop:~/Linux/linux-3.8.3/scripts$ gedit Makefile.lib
打开之后可以找到如下：
318    UIMAGE_LOADADDR ?= arch_must_set_this
319    UIMAGE_ENTRYADDR ?= $(UIMAGE_LOADADDR)
这里就是说Entry Point等于Load Address，那么应该修改成为Entry Point=Load Address+0x40，在GNU make中，有sed –e替换操作，如sed -e "s/..$$/40/"，就是把输出的字符串的最后两个字符删掉，并且用40来补充，也就是说把字符串最后两个字符用40来替换。
那么作如下修改：
318    UIMAGE_LOADADDR ?= arch_must_set_this
319    #UIMAGE_ENTRYADDR ?= $(UIMAGE_LOADADDR)
320    UIMAGE_ENTRYADDR  ?=$(shell echo $(UIMAGE_LOADADDR) |
sed -e "s/..$$/40/")
修改完成之后，回到linux-3.8.3根目录下进行编译，如下操作：
zhuzhaoqi@zhuzhaoqi-desktop:~/Linux/linux-3.8.3$ make uImage
如果有如下报错：
zhuzhaoqi@zhuzhaoqi-desktop:~/Linux/linux-3.8.3$ make uImage
scripts/kconfig/conf --silentoldconfig Kconfig

*** Error during update of the configuration.

make[2]: *** [silentoldconfig] 错误 1
make[1]: *** [silentoldconfig] 错误 2
make: *** 没有规则可以创建“include/config/kernel.release”需要的目标“include/config/auto.conf”。停止。
那就是权限的问题，要么修改文件权限，要么在root下编译。这样即可：
zhuzhaoqi@zhuzhaoqi-desktop:~/Linux/linux-3.8.3$ sudo make uImage
编译成功之后输出如下信息：
······
Image Name:   Linux-3.8.3
Created:      Sat Mar 16 10:38:47 2013
Image Type:   ARM Linux Kernel Image (uncompressed)
Data Size:    1664080 Bytes = 1625.08 kB = 1.59 MB
Load Address: 50008000
Entry Point:  50008040
  Image arch/arm/boot/uImage is ready
从串口输出可知，Entry Point=Load Address+0x40，依旧按照SD烧写方式进行测试，如果bootdelay延时过长，可以修改bootdelay时间，如下操作：
Hit any key to stop autoboot:  0
zzq6410 >>> set bootdelay 3
zzq6410 >>> sav
Saving Environment to NAND...
Erasing Nand...
Erasing at 0x80000 -- 100% complete.
Writing to Nand... done
zzq6410 >>>
重启OK6410开发平台，测试结果如下：
NAND read: device 0 offset 0x100000, size 0x500000
5242880 bytes read: OK
## Booting kernel from Legacy Image at 50008000 ...
   Image Name:   Linux-3.8.3
   Image Type:   ARM Linux Kernel Image (uncompressed)
   Data Size:    1664080 Bytes = 1.6 MiB
   Load Address: 50008000
   Entry Point:  50008040
   Verifying Checksum ... OK
   XIP Kernel Image ... OK
OK

Starting kernel ...

Starting kernel ...

Uncompressing Linux... done, booting the kernel.
Booting Linux on physical CPU 0x0
Linux version 3.8.3 (zhuzhaoqi@zhuzhaoqi-desktop) (gcc version 4.4.1 (Sourcery G++ Lite 2009q3-67) ) #1 Fri Mar 15 12:56:52 CST 2013
CPU: ARMv6-compatible processor [410fb766] revision 6 (ARMv7), cr=00c5387d
CPU: PIPT / VIPT nonaliasing data cache, VIPT nonaliasing instruction cache
Machine: OK6410
Memory policy: ECC disabled, Data cache writeback
CPU S3C6410 (id 0x36410101)
S3C24XX Clocks, Copyright 2004 Simtec Electronics
S3C64XX: PLL settings, A=533000000, M=533000000, E=24000000
S3C64XX: HCLK2=266500000, HCLK=133250000, PCLK=66625000
mout_apll: source is fout_apll (1), rate is 533000000
mout_epll: source is epll (1), rate is 24000000
mout_mpll: source is mpll (1), rate is 533000000
usb-bus-host: source is clk_48m (0), rate is 48000000
irda-bus: source is mout_epll (0), rate is 24000000
CPU: found DTCM0 8k @ 00000000, not enabled
CPU: moved DTCM0 8k to fffe8000, enabled
CPU: found DTCM1 8k @ 00000000, not enabled
CPU: moved DTCM1 8k to fffea000, enabled
CPU: found ITCM0 8k @ 00000000, not enabled
CPU: moved ITCM0 8k to fffe0000, enabled
CPU: found ITCM1 8k @ 00000000, not enabled
CPU: moved ITCM1 8k to fffe2000, enabled
Built 1 zonelists in Zone order, mobility grouping on.  Total pages: 65024
Kernel command line: root=/dev/mtdblock2 rootfstype=cramfs console=ttySAC0,115200
PID hash table entries: 1024 (order: 0, 4096 bytes)
Dentry cache hash table entries: 32768 (order: 5, 131072 bytes)
Inode-cache hash table entries: 16384 (order: 4, 65536 bytes)
__ex_table already sorted, skipping sort
Memory: 256MB = 256MB total
Memory: 256532k/256532k available, 5612k reserved, 0K highmem
Virtual kernel memory layout:
    vector  : 0xffff0000 - 0xffff1000   (   4 kB)
    DTCM    : 0xfffe8000 - 0xfffec000   (  16 kB)
    ITCM    : 0xfffe0000 - 0xfffe4000   (  16 kB)
    fixmap  : 0xfff00000 - 0xfffe0000   ( 896 kB)
    vmalloc : 0xd0800000 - 0xff000000   ( 744 MB)
    lowmem  : 0xc0000000 - 0xd0000000   ( 256 MB)
    modules : 0xbf000000 - 0xc0000000   (  16 MB)
      .text : 0xc0008000 - 0xc02bed88   (2780 kB)
      .init : 0xc02bf000 - 0xc02da7a4   ( 110 kB)
      .data : 0xc02dc000 - 0xc03076a0   ( 174 kB)
       .bss : 0xc0308000 - 0xc0338ef8   ( 196 kB)
SLUB: Genslabs=13, HWalign=32, Order=0-3, MinObjects=0, CPUs=1, Nodes=1
NR_IRQS:246
VIC @f6000000: id 0x00041192, vendor 0x41
VIC @f6010000: id 0x00041192, vendor 0x41
sched_clock: 32 bits at 100 Hz, resolution 10000000ns, wraps every 4294967286ms
Console: colour dummy device 80x30
Calibrating delay loop... 353.89 BogoMIPS (lpj=1769472)
pid_max: default: 32768 minimum: 301
Mount-cache hash table entries: 512
CPU: Testing write buffer coherency: ok
Setting up static identity map for 0x502149a8 - 0x50214a04
DMA: preallocated 256 KiB pool for atomic coherent allocations
OK6410: Option string ok6410=0
OK6410: selected LCD display is 480x272
s3c64xx_dma_init: Registering DMA channels
PL080: IRQ 73, at d0846000, channels 0..8
PL080: IRQ 74, at d0848000, channels 8..16
S3C6410: Initialising architecture
bio: create slab <bio-0> at 0
usbcore: registered new interface driver usbfs
usbcore: registered new interface driver hub
usbcore: registered new device driver usb
ROMFS MTD (C) 2007 Red Hat, Inc.
io scheduler noop registered
io scheduler deadline registered
io scheduler cfq registered (default)
s3c-fb s3c-fb: window 0: fb
Serial: 8250/16550 driver, 4 ports, IRQ sharing disabled
s3c6400-uart.0: ttySAC0 at MMIO 0x7f005000 (irq = 69) is a S3C6400/10
console [ttySAC0] enabled
s3c6400-uart.1: ttySAC1 at MMIO 0x7f005400 (irq = 70) is a S3C6400/10
s3c6400-uart.2: ttySAC2 at MMIO 0x7f005800 (irq = 71) is a S3C6400/10
s3c6400-uart.3: ttySAC3 at MMIO 0x7f005c00 (irq = 72) is a S3C6400/10
brd: module loaded
loop: module loaded
s3c24xx-nand s3c6400-nand: Tacls=4, 30ns Twrph0=8 60ns, Twrph1=6 45ns
s3c24xx-nand s3c6400-nand: System booted from NAND
s3c24xx-nand s3c6400-nand: NAND soft ECC
NAND device: Manufacturer ID: 0xec, Chip ID: 0xd5 (Samsung NAND 2GiB 3,3V 8-bit), 2048MiB, page size: 4096, OOB size: 218
No oob scheme defined for oobsize 218
……
Kernel panic - not syncing: Attempted to kill init! exitcode=0x0000000b
中间作者省去了很多信息，因为这些信息暂时对我们是没有太大关系，但是也给出了很多信息，因为可以很好地和接下来的每一步移植作对比。从串口的输出，可以得知内核是启动了。也就是说，此时u-boot已经成功将相关参数传递给linux3.8.3内核，完成了u-boot到内核的交接。并且内核已经识别了是OK6410开发平台，控制CPU是s3c6410等信息。
当然，读者不仅仅可以通过修改Entry Point使得内核启动，还可以修改启动内核的地址使得bootm address和Load Address不相等，也就是修改U-Boot源码中include/configs/目录下的s3c6410.h文件中：
#ifdef CONFIG_ENABLE_MMU
#define CONFIG_SYS_MAPPED_RAM_BASE  0xc0000000
#define CONFIG_BOOTCOMMAND"nand read 0xc0018000 0x600000x1c0000;\"bootm 0xc0018000"
#else

#define CONFIG_SYS_MAPPED_RAM_BASE  CONFIG_SYS_SDRAM_BASE
#define CONFIG_BOOTCOMMAND"nand read 0x50018000 0x100000 0x500000;"\"bootm 0x50018000"
#endif


第三章第四节    内核启动分析
    对于ARM处理器，内核启动大体上可以分为两个阶段：与处理器相关的汇编启动阶段和与处理器无关的C代码启动阶段。汇编启动阶段从head.S(arch/arm/kernel/head.S)文件开始，C代码启动阶段从start_kernel函数(init/main.c)开始。当然，经过压缩的内核镜像文件zImage，在进入汇编启动阶段前还要运行一段自解压代码(arch/arm/boot/compressed/head.S)。
    省略一些无关紧要的过程和编译后不运行的代码，该过程的启动流程如图3. 7所示。相对早期linux-2.6.38的版本，linux-3.8.3在汇编启动阶段并没有出现__lookup_machine_type，但这并不意味着内核不再检查bootloader传入的machine_arch_type参数(R1)，只是将检查机制推迟到了C代码阶段。

1)     __lookup_processor_type
__lookup_processor_type函数的具体实现如程序清单3. 1。
程序清单3. 1查找处理器类型函数
__lookup_processor_type:
    adr r3, __lookup_processor_type_data
    ldmia r3, {r4 - r6}
    sub r3, r3, r4          @ get offset between virt&phys
    add r5, r5, r3          @ convert virt addresses to
    add r6, r6, r3          @ physical address space
1:  ldmia   r5, {r3, r4}            @ value, mask
    and r4, r4, r9          @ mask wanted bits
    teq r3, r4
    beq 2f
    add r5, r5, #PROC_INFO_SZ       @ sizeof(proc_info_list)
    cmp r5, r6
    blo 1b
    mov r5, #0              @ unknown processor
2:  mov pc, lr
ENDPROC(__lookup_processor_type)
    .align  2
    .type   __lookup_processor_type_data, %object
__lookup_processor_type_data:
    .long   .
    .long   __proc_info_begin
    .long   __proc_info_end
    .size   __lookup_processor_type_data, . - __lookup_processor_type_data
__lookup_processor_type函数的主要功能是将内核支持的所有CPU类型与通过程序实际读取的cpu id进行查表匹配。如果匹配成功，将匹配到的proc_info_list的基地址存到r5,否则，r5为0，程序将会进入一个死循环。函数传入参数r9为程序实际读取的cpu id，传出参数r5为匹配到的proc_info_list指针的地址。同时为了使C语言能够调用这个函数，根据APCS（ARM 过程调用标准）规则，简单使用以下代码就能包装成一个C语言版本__lookup_processor_type的API函数，函数的原型为struct proc_info_list *lookup_processor_type(unsigned int)。
ENTRY(lookup_processor_type)
    stmfd   sp!, {r4 - r6, r9, lr}
    mov r9, r0
    bl  __lookup_processor_type
    mov r0, r5
    ldmfd   sp!, {r4 - r6, r9, pc}
ENDPROC(lookup_processor_type)
ENTRY和ENDPROC宏的定义如下：
#define ENTRY(name)             \
    .globl name;                \
    name:
#define ENDPROC(name)
内核利用一个结构体proc_info_list来记录处理器相关的信息，在文件arch/arm/include/asm/procinfo.h声明了该结构体的类型，如下所示。
struct proc_info_list {
    unsigned int        cpu_val;
    unsigned int        cpu_mask;
    unsigned long       __cpu_mm_mmu_flags; /* used by head.S */
    unsigned long       __cpu_io_mmu_flags; /* used by head.S */
    unsigned long       __cpu_flush;        /* used by head.S */
    const char      *arch_name;
    const char      *elf_name;
    unsigned int        elf_hwcap;
    const char      *cpu_name;
    struct processor    *proc;
    struct cpu_tlb_fns  *tlb;
    struct cpu_user_fns *user;
    struct cpu_cache_fns    *cache;
};
事实上，在arch/arm/mm/proc-*.S这类文件中，程序才真正给内核所支持的arm处理器的proc_info_list分配了内存空间，例如linux/arch/arm/mm/proc-v6.S文件用汇编语言定义的__v6_proc_info结构体。.section指示符来指定这些结构体编译到.proc.info段。.proc.info的起始地址为 __proc_info_begin，终止位置为__proc_info_end，把它们作为全局变量保存在内存中，链接脚本arch/arm/kernel/vmlinux.lds部分内容参考如下：
.init.proc.info : {
  . = ALIGN(4);
__proc_info_begin = .;
*(.proc.info.init)
__proc_info_end = .;
}
2)       __vet_atags
在启动内核时, bootloader会向内核传递一些参数。通常，bootloader 有两种方法传递参数给内核：一种是旧的参数结构方式（parameter_struct)——主要是2.6 之前的内核使用的方式；另外一种是现在的内核在用的参数列表(tagged list) 的方式。这些参数主要包括，系统的根设备标志、页面大小、内存的起始地址和大小、当前内核命令参数等。而这些参数是通过struct tag结构体组织，利用指针链接成一个按顺序排放的参数列表。bootloader引导内核启动时，就会把这个列表的首地址放入R2中，传给内核，内核通过这个地址就分析出传入的所有参数。
内核要求参数列表必须存放在RAM物理地址的头16k位置，并且ATAG_CORE类型的参数需要放置在参数的列表的首位。__vet_atags的功能就是初步分析传入的参数列表，判断的方法也很简单。如果这个列表起始参数是ATAG_CORE类型，则表示这是一个有效的参数列表。如果起始参数不是ATAG_CORE，就认为bootloader没有传递参数给内核或传入的参数不正确。
1)       __create_page_tables
图3. 8实际内存分布图

linux内核使用页式内存管理，应用程序给出的内存地址是虚拟地址，它需要经过若干级页表一级一级的变换，才变成真正的物理地址。32位CPU的虚拟地址大小从0x0000_0000到0xFFFF_FFFF共4G。以段(1 MB)的方式建立一级页表，可以将虚拟地址空间分割成4096个段条目(section entry)。条目也称为“描述符”(Descriptor)，每一个段描述符32位，因此一级页表占用16K(0x4000)内存空间。



s3c6410处理器DRAM的地址空间从0x5000_0000开始，上文提到bootloader传递给内核的参数列表存放在RAM物理地址的头16K位置，页表放置在内核的前16K，因此内核的偏移地址为32K(0x8000)，由此构成了如图3. 8所示的实际内存分布图。
图3. 9初步页表建立流程

__create_page_tables函数初始化了一个非常简单页表，仅映射了使内核能够正常启动的代码空间，更加细致的工作将会在后续阶段完善。流程如所示，获取页表物理地址、清空页表区和建立启动参数页表通过阅读源码很容易理解，不加分析。
__enable_mmu函数使能mmu后，CPU发出的地址是虚拟地址，程序正常运行需要映射得到物理地址，为了保障正常地配置mmu，需要对这段代码1:1的绝对映射，映射范围__turn_mmu_on至__turn_mmu_on_end。正常使能mmu后，不需要这段特定的映射了，在后续C代码启动阶段时被paging_init()函数删除。建立__enable_mmu函数区域的页表代码如程序清单3. 2所示。
程序清单3. 2  __enable_mmu页表的建立
//r4 =页表物理地址
//获取段描述符的默认配置flags
    ldr r7, [r10, #PROCINFO_MM_MMUFLAGS]
    adr r0, __turn_mmu_on_loc //得到__turn_mmu_on_loc的物理地址
    ldmia   r0, {r3, r5, r6}
    sub r0, r0, r3      //计算得到物理地址与虚拟地址的偏差
    add r5, r5, r0      //修正得到__turn_mmu_on的物理地址
    add r6, r6, r0      //修正得到__turn_mmu_on_end的物理地址
    mov r5, r5, lsr #SECTION_SHIFT //1M对齐
    mov r6, r6, lsr #SECTION_SHIFT //1M对齐
1:  orr r3, r7, r5, lsl #SECTION_SHIFT  //生成段描述符：flags + 段基址
    str r3, [r4, r5, lsl #PMD_ORDER]    //设置段描述绝对映射，物理地址等于虚拟地址。每个段描述符占4字节，PMD_ORDER = 2
    cmp r5, r6
    addlo   r5, r5, #1          //下一段，实际上__turn_mmu_on_end - __turn_mmu_on<  1M
    blo 1b
............................
__turn_mmu_on_loc:
    .long   .                   //__turn_mmu_on_loc当前位置的虚拟地址
    .long   __turn_mmu_on      //__turn_mmu_on的虚拟地址
    .long   __turn_mmu_on_end  //__turn_mmu_on_end的虚拟地址
建立内核的映射区页表，分析见程序清单3. 3。
程序清单3. 3内核的映射区页表的建立
//r4 =页表物理地址
mov r3, pc                  //r3 = 当前物理地址
    mov r3, r3, lsr #SECTION_SHIFT    //物理地址转化段基址
    orr r3, r7, r3, lsl #SECTION_SHIFT  //段基址 + flags = 段描述符
//KERNEL_START = 0xC000_8000  SECTION_SHIFT = 20  PMD_ORDER =  2
//由于arm 的立即数只能是8位表示，所有用两条指令实现了将r3存储到对应的页表项中
    add r0, r4,  #(KERNEL_START & 0xff000000) >> (SECTION_SHIFT - PMD_ORDER)
    str r3, [r0, #((KERNEL_START & 0x00f00000) >> SECTION_SHIFT) << PMD_ORDER]!
    ldr r6, =(KERNEL_END - 1)
    add r0, r0, #1 << PMD_ORDER
    add r6, r4, r6, lsr #(SECTION_SHIFT - PMD_ORDER)  //内核映射页表结束的段基址
1:  cmp r0, r6
    add r3, r3, #1 << SECTION_SHIFT   //得到段描述符
    strls   r3, [r0], #1 << PMD_ORDER    //设置段描述符
    bls 1b
1)       __v6_setup
__v6_setup 函数在 proc-v6.S 文件中，在页表建立起来之后，此函数进行一些使能 MMU 之前的初始化操作。
2)       __enable_mmu
__v6_setup已经为使能 MMU做好了必要的准备，为了保证MMU启动后程序顺利返回，在进入__enable_mmu函数之前，已经将__mmap_switched的虚拟地址(链接地址)存储在R13中。
3)       __mmap_switched
程序运行到这里，MMU已经启动，__mmap_switched函数为内核进入C代码阶段做了一些准备工作：复制数据段，清楚BSS段，设置堆栈指针，保存processor ID、machine type(bootloader中传入的)、atags pointer等。最后，终于跳转到start_kernel函数，进入C代码启动阶段。



from:http://bbs.eeworld.com.cn/thread-419923-1-1.html









