# QEMU1.3.0的源码分析一 : 源码目录简介 - SHINING的博客 - CSDN博客
2012年12月06日 15:31:36[snsn1984](https://me.csdn.net/snsn1984)阅读数：3967
                
最近在研究QEMU，读了一些QEMU的源码，因为涉及的东西比较多，找到的资料又都比较破碎，不太完整。所以将最近的成果总结一下。
相比其他的开源软件来说，QEMU源码下面目录比较多，下面就先把这些目录的内容大致整理一下。
docs/ 包含了一些文档，说实话，对初学者来说，读这些文档压根没有头绪
hw/   包含了所有支持的硬件设备
include/  包含了一些头文件
linux-user/  包含了linux下的用户模式的代码
target-XXX/   包含了QEMU目前所支持guset端的处理器架构。包括：alpha,arm,cris,i386,lm32,m68k,microblaze,mips,openrisc,ppc,s390x,sh4,sparc,unicore32,xtensa. 此处的XXX就是指这其中的一种架构。包含的代码的主要功能是将该guest架构的指令翻译成TCG OP代码。也就是target-arm下的代码就是将arm架构的指令翻译成TCG OP。这些目录占了源码目录的很大一部分。
tcg/   包含了动态翻译工具tcg的源码部分，主要是将TCG OP转化为host binary的部分。这个目录下也包含了多个架构名字命名的目录，每个目录下存放着针对该架构的代码。后续会详细介绍。
test/ 从名字上可以看出，应该是存放测试部分的代码，但是目前这部分代码还没读。
            
