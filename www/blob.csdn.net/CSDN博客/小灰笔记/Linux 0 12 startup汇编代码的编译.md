# Linux 0.12 startup汇编代码的编译 - 小灰笔记 - CSDN博客





2017年08月27日 20:22:09[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：363








       在学习Linux内核的原理，看到了startup的代码。安装了相应的GNU汇编器之后，尝试了一下代码的编译。

       我使用的Win10安装了一个内置的Ubuntu子系统，在此系统下编译结果如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/15_linux/01_kernal/03_my_test_code/04_as_demo$ls

head.s  make.py

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/15_linux/01_kernal/03_my_test_code/04_as_demo$as -o head.o head.s

head.s: Assemblermessages:

head.s:43: Error:unsupported instruction `mov'

head.s:47: Error:unsupported instruction `mov'

head.s:59: Error:unsupported instruction `mov'

head.s:61: Error:unsupported instruction `mov'

head.s:136: Error:invalid instruction suffix for `push'

head.s:137: Error:invalid instruction suffix for `push'

head.s:138: Error:invalid instruction suffix for `push'

head.s:139: Error:invalid instruction suffix for `push'

head.s:140: Error:invalid instruction suffix for `push'

head.s:151: Error:invalid instruction suffix for `push'

head.s:152: Error:invalid instruction suffix for `push'

head.s:153: Error:invalid instruction suffix for `push'

head.s:154: Error:operand type mismatch for `push'

head.s:155: Error:operand type mismatch for `push'

head.s:161: Error:invalid instruction suffix for `push'

head.s:163: Error:invalid instruction suffix for `pop'

head.s:165: Error:operand type mismatch for `pop'

head.s:166: Error:operand type mismatch for `pop'

head.s:167: Error:invalid instruction suffix for `pop'

head.s:168: Error:invalid instruction suffix for `pop'

head.s:169: Error:invalid instruction suffix for `pop'

head.s:214: Error:unsupported instruction `mov'

head.s:215: Error:unsupported instruction `mov'

head.s:217: Error:unsupported instruction `mov'

head.s:231: Error:alignment not a power of 2

       我下载的代码是原版，也是Linus写的，编译的时候居然报错！束手无策之际，尝试使用Windows下的编译器进行编译。结果如下：

E:\01_workspace\15_linux\01_kernal\03_my_test_code\04_as_demo>as-o head.o head.s

head.s: Assemblermessages:

head.s:231: Error:alignment not a power of 2

       两个系统下编译都遇到了问题，但是Linux下面要多很多错误。尝试按照Windows下的提示进行了代码修复，修改代码如下：

/*

 * linux/boot/head.s

 *

 *  (C)1991  Linus Torvalds

 */



/*

 * head.s contains the 32-bit startup code.

 *

 * NOTE!!! Startup happens at absolute address0x00000000, which is also where

 * the page directory will exist. The startupcode will be overwritten by

 * the page directory.

 */

.text

.globl_idt,_gdt,_pg_dir,_tmp_floppy_area

_pg_dir:

startup_32:

       movl $0x10,%eax

       mov %ax,%ds

       mov %ax,%es

       mov %ax,%fs

       mov %ax,%gs

       lss _stack_start,%esp

       call setup_idt

       call setup_gdt

       movl $0x10,%eax           # reload all the segment registers

       mov %ax,%ds          # after changing gdt. CS was already

       mov %ax,%es          # reloaded in 'setup_gdt'

       mov %ax,%fs

       mov %ax,%gs

       lss _stack_start,%esp

       xorl %eax,%eax

1:     incl %eax        #check that A20 really IS enabled

       movl %eax,0x000000      # loop forever if it isn't

       cmpl %eax,0x100000

       je 1b

/*

 * NOTE! 486 should set bit 16, to check forwrite-protect in supervisor

 * mode. Then it would be unnecessary with the"verify_area()"-calls.

 * 486 users probably want to set the NE (#5)bit also, so as to use

 * int 16 for math errors.

 */

       movl %cr0,%eax             # check math chip

       andl $0x80000011,%eax        # Save PG,PE,ET

/* "orl$0x10020,%eax" here for 486 might be good */

       orl $2,%eax            # set MP

       movl %eax,%cr0

       call check_x87

       jmp after_page_tables



/*

 * We depend on ET to be correct. This checksfor 287/387.

 */

check_x87:

       fninit

       fstsw %ax

       cmpb $0,%al

       je 1f               /*no coprocessor: have to set bits */

       movl %cr0,%eax

       xorl $6,%eax           /* reset MP, set EM */

       movl %eax,%cr0

       ret

.align 2

1:     .byte 0xDB,0xE4            /* fsetpm for 287, ignored by 387 */

       ret



/*

 * setup_idt

 *

 *  setsup a idt with 256 entries pointing to

 * ignore_int, interrupt gates. It then loads

 *  idt.Everything that wants to install itself

 *  inthe idt-table may do so themselves. Interrupts

 *  areenabled elsewhere, when we can be relatively

 *  sureeverything is ok. This routine will be over-

 * written by the page tables.

 */

setup_idt:

       lea ignore_int,%edx

       movl $0x00080000,%eax

       movw %dx,%ax              /* selector = 0x0008 = cs */

       movw $0x8E00,%dx       /* interrupt gate - dpl=0, present */



       lea _idt,%edi

       mov $256,%ecx

rp_sidt:

       movl %eax,(%edi)

       movl %edx,4(%edi)

       addl $8,%edi

       dec %ecx

       jne rp_sidt

       lidt idt_descr

       ret



/*

 * setup_gdt

 *

 *  Thisroutines sets up a new gdt and loads it.

 *  Onlytwo entries are currently built, the same

 *  onesthat were built in init.s. The routine

 *  isVERY complicated at two whole lines, so this

 * rather long comment is certainly needed :-).

 *  Thisroutine will beoverwritten by the page tables.

 */

setup_gdt:

       lgdt gdt_descr

       ret



/*

 * I put the kernel page tables right after thepage directory,

 * using 4 of them to span 16 Mb of physicalmemory. People with

 * more than 16MB will have to expand this.

 */

.org 0x1000

pg0:



.org 0x2000

pg1:



.org 0x3000

pg2:



.org 0x4000

pg3:



.org 0x5000

/*

 * tmp_floppy_area is used by the floppy-driverwhen DMA cannot

 * reach to a buffer-block. It needs to bealigned, so that it isn't

 * on a 64kB border.

 */

_tmp_floppy_area:

       .fill 1024,1,0



after_page_tables:

       pushl $0         #These are the parameters to main :-)

       pushl $0

       pushl $0

       pushl $L6              #return address for main, if it decides to.

       pushl $_main

       jmp setup_paging

L6:

       jmp L6                  #main should never return here, but

                            # just in case, weknow what happens.



/* This is thedefault interrupt "handler" :-) */

int_msg:

       .asciz "Unknown interrupt\n\r"

.align 2

ignore_int:

       pushl %eax

       pushl %ecx

       pushl %edx

       push %ds

       push %es

       push %fs

       movl $0x10,%eax

       mov %ax,%ds

       mov %ax,%es

       mov %ax,%fs

       pushl $int_msg

       call _printk

       popl %eax

       pop %fs

       pop %es

       pop %ds

       popl %edx

       popl %ecx

       popl %eax

       iret





/*

 * Setup_paging

 *

 * This routine sets up paging by setting thepage bit

 * in cr0. The page tables are set up,identity-mapping

 * the first 16MB. The pager assumes that noillegal

 * addresses are produced (ie >4Mb on a 4Mbmachine).

 *

 * NOTE! Although all physical memory should beidentity

 * mapped by this routine, only the kernel pagefunctions

 * use the >1Mb addresses directly. All"normal" functions

 * use just the lower 1Mb, or the local dataspace, which

 * will be mapped to some other place - mmkeeps track of

 * that.

 *

 * For those with more memory than 16 Mb - toughluck. I've

 * not got it, why should you :-) The source ishere. Change

 * it. (Seriously - it shouldn't be toodifficult. Mostly

 * change some constants etc. I left it at16Mb, as my machine

 * even cannot be extended past that (ok, butit was cheap :-)

 * I've tried to show which constants to changeby having

 * some kind of marker at them (search for"16Mb"), but I

 * won't guarantee that's all :-( )

 */

.align 2

setup_paging:

       movl $1024*5,%ecx               /* 5 pages - pg_dir+4 page tables */

       xorl %eax,%eax

       xorl %edi,%edi               /* pg_dir is at 0x000 */

       cld;rep;stosl

       movl $pg0+7,_pg_dir           /* set present bit/user r/w */

       movl $pg1+7,_pg_dir+4              /* --------- " " --------- */

       movl $pg2+7,_pg_dir+8              /* --------- " " --------- */

       movl $pg3+7,_pg_dir+12            /* --------- " " --------- */

       movl $pg3+4092,%edi

       movl $0xfff007,%eax             /* 16Mb - 4096 + 7 (r/w user,p) */

       std

1:     stosl              /*fill pages backwards - more efficient :-) */

       subl $0x1000,%eax

       jge 1b

       xorl %eax,%eax       /* pg_dir is at 0x0000 */

       movl %eax,%cr3             /* cr3 - page directory start */

       movl %cr0,%eax

       orl $0x80000000,%eax

       movl %eax,%cr0             /* set paging (PG) bit */

       ret                 /*this also flushes prefetch-queue */



.align 2

.word 0

idt_descr:

       .word 256*8-1              # idt contains 256 entries

       .long _idt

.align 2

.word 0

gdt_descr:

       .word 256*8-1              # so does gdt (not that that's any

       .long _gdt             # magic number, but it works for me :^)



       .align 4 /* changed by me */

_idt: .fill 256,8,0             #idt is uninitialized



_gdt:       .quad 0x0000000000000000        /* NULL descriptor */

       .quad 0x00c09a0000000fff    /* 16Mb */

       .quad 0x00c0920000000fff    /* 16Mb */

       .quad 0x0000000000000000        /* TEMPORARY - don't use */

       .fill 252,8,0                    /* space for LDT's and TSS's etc */

       继续在Windows下进行编译，结果如下：

E:\01_workspace\15_linux\01_kernal\03_my_test_code\04_as_demo>as-o head.o head.s



E:\01_workspace\15_linux\01_kernal\03_my_test_code\04_as_demo>dir

 驱动器 E 中的卷没有标签。

 卷的序列号是 2013-5089



 E:\01_workspace\15_linux\01_kernal\03_my_test_code\04_as_demo的目录



2017/08/27  20:14   <DIR>          .

2017/08/27  20:14   <DIR>          ..

2017/08/27  20:14            26,811 head.o

2017/08/27  20:13             5,938 head.s

2017/08/27  20:05                75 make.py

               3 个文件         32,824 字节

               2 个目录 79,968,800,768可用字节

       修改居然顺利编译通过了！再次在Linux下进行编译：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/15_linux/01_kernal/03_my_test_code/04_as_demo$as -o head.o head.s

head.s: Assemblermessages:

head.s:43: Error:unsupported instruction `mov'

head.s:47: Error:unsupported instruction `mov'

head.s:59: Error:unsupported instruction `mov'

head.s:61: Error:unsupported instruction `mov'

head.s:136: Error:invalid instruction suffix for `push'

head.s:137: Error:invalid instruction suffix for `push'

head.s:138: Error:invalid instruction suffix for `push'

head.s:139: Error:invalid instruction suffix for `push'

head.s:140: Error:invalid instruction suffix for `push'

head.s:151: Error:invalid instruction suffix for `push'

head.s:152: Error:invalid instruction suffix for `push'

head.s:153: Error:invalid instruction suffix for `push'

head.s:154: Error:operand type mismatch for `push'

head.s:155: Error:operand type mismatch for `push'

head.s:161: Error:invalid instruction suffix for `push'

head.s:163: Error:invalid instruction suffix for `pop'

head.s:165: Error:operand type mismatch for `pop'

head.s:166: Error:operand type mismatch for `pop'

head.s:167: Error:invalid instruction suffix for `pop'

head.s:168: Error:invalid instruction suffix for `pop'

head.s:169: Error:invalid instruction suffix for `pop'

head.s:214: Error:unsupported instruction `mov'

head.s:215: Error:unsupported instruction `mov'

head.s:217: Error:unsupported instruction `mov'

       这样也仅仅是少了最后一条错误而已！查看一下两个系统上的软件区别如下：

Windows下：

E:\01_workspace\15_linux\01_kernal\03_my_test_code\04_as_demo>as--version

GNU assembler (GNUBinutils) 2.25

Copyright (C) 2014Free Software Foundation, Inc.

This program isfree software; you may redistribute it under the terms of

the GNU GeneralPublic License version 3 or later.

This program hasabsolutely no warranty.

This assembler wasconfigured for a target of `i686-w64-mingw32'.

       Linux下的版本：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/15_linux/01_kernal/03_my_test_code/04_as_demo$as --version

GNU assembler (GNUBinutils for Ubuntu) 2.24

Copyright 2013Free Software Foundation, Inc.

This program isfree software; you may redistribute it under the terms of

the GNU GeneralPublic License version 3 or later.

This program hasabsolutely no warranty.

This assembler wasconfigured for a target of `x86_64-linux-gnu'.

       两个版本确实不一样，但是有点让人费解。按理说，越老的编译器对旧代码的兼容性越好才对！疑惑之际发现其实我用的两个编译器还有另一点差异：Windows下使用的32位软件而Linux下则是64位。查看as的文档发现确实有一个32位代码兼容的选项，在Linux上增加之后效果如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/15_linux/01_kernal/03_my_test_code/04_as_demo$ls

head.s  make.py

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/15_linux/01_kernal/03_my_test_code/04_as_demo$as --32 -o head.o head.s

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/15_linux/01_kernal/03_my_test_code/04_as_demo$ls

head.o  head.s make.py

       通过调整，这次终于编译成功！



