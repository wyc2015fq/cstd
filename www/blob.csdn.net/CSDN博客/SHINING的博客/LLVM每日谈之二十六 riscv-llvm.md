# LLVM每日谈之二十六  riscv-llvm - SHINING的博客 - CSDN博客
2017年11月28日 09:58:20[snsn1984](https://me.csdn.net/snsn1984)阅读数：1352
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
       说起riscv-llvm，可能一下子让人摸不到头脑，因为有些人没有关注RISC-V。如果从RISC说起，RISC-V, riscv-llvm都会变得更容易理解。RISC就是经常说的精简指令集，这在之前计算机行业的发展过程中起到的作用，怎么形容都不为过了。真正做过编译器相关和CPU相关工作的人，可能对这些有更深的感触。RISC-V简单点来说,就是一个开源版本的RISC，并且由于设计团队包含了很多之前参与设计RISC的大牛，起点很高，团队力量很强，再加上借助开源社区的力量，至少从我的角度来说，发展前景一篇光明。再加上使用的是BSD许可，未来预计会有更多的商业公司加入。
       riscv-llvm是基于LLVM的一个针对RISC-V指令集的后端。众所周知，LLVM架构的前端、IR和后端的定位，后端要将IR转换为针对具体指令集的代码。而riscv-llvm可以视为LLVM的一个新的后端，这个后端所要做的就是将已经生成的IR代码，转换为支持RISC-V指令集的代码。riscv-llvm的出现，对RISC-V和LLVM都是一个好事。对于RISC来说，扩展了它的工具链，可以借助LLVM直接面对使用高级编程语言的开发者。对于LLVM来说，拓展了它所支持的指令集，可以让它支撑更多的硬件。
        因为本文的聚焦点是 riscv-llvm，所以本文不深入介绍RISC-V。收集资料的时候，整理出来一些资源，具体如下：
[https://zhuanlan.zhihu.com/p/20813811](https://zhuanlan.zhihu.com/p/20813811)
RISC-V登场，Intel和ARM会怕吗？
[https://cnrv.io/](https://cnrv.io/)
为RISC-V国内爱好者服务的网站，提供双周简报
[https://riscv.org/](https://riscv.org/)
RISC-V组织官方网站
[https://github.com/riscv/riscv-llvm](https://github.com/riscv/riscv-llvm)
RISCV-LLVM github 地址
