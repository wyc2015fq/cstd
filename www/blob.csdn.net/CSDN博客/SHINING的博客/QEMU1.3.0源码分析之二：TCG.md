# QEMU1.3.0源码分析之二：TCG - SHINING的博客 - CSDN博客
2012年12月07日 10:34:42[snsn1984](https://me.csdn.net/snsn1984)阅读数：3916
TCG是Tiny Code Generator的简称，它之前是一个后端编译器，现在是作为一个动态翻译器来使用。在QEMU中，它主要用来将虚拟出来的系统的指令转化成真正硬件支持的指令中的从中间代码到硬件支持的机器代码的过程。前端的将指令翻译成中间代码的过程，是一个反汇编的过程。
反汇编的过程的源码的主要地址：qemu source code/target-XXX。此处的XXX指的是模拟出来的系统的架构。
TCG的源码的位置是：qemu source code/tcg。这个目录下有很多文件夹，每个文件夹都代表一个目标架构。这里的目标架构指的是真正的硬件架构，也就是说运行QEMU的架构。
在qemu source code/tcg目录下，有一个README文件，介绍了tcg的主要内容。
在qemu source code/tcg/arm目录下，只有两个文件，实现了生成arm架构的内容。tcg-target.c和tcg-target.h两个文件。和arm同级目录的ia64,hppa,ppc,s390,i386,mips,ppc64,sparc,tci等目录下也是同样的名字的两个文件，当然文件的内容并不相同。关于这两个文件的内容，README是这么描述的：tcg-target.h contains the target specific definitions. tcg-target.c
 contains the target specific code.
动态翻译只是在必要的时候才进行翻译，而尽可能的将时间花费在执行host code上。TB(Translation Block)翻译之后得到的host code会存放在code cache中，因为有很多TB会被重复执行，所以这样会达到更加的效果。
