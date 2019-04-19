# ARM异常、中断以及他们的向量表分析 - xqhrs232的专栏 - CSDN博客
2014年04月08日 16:25:34[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1898
原文地址::[http://blog.csdn.net/gooogleman/article/details/3597175](http://blog.csdn.net/gooogleman/article/details/3597175)
作者：[wogoyixikexie@gliet](mailto:wogoyixikexie@gliet)
    以前，我一直很疑惑这个“ARM异常、中断以及他们的向量表”是怎么回事，他们到底是怎么实现的，没有想到今天偶然看到（ARM System Developer's Guide: Designing and Optimizing System Software的ARM异常、中断以及他们的向量表的章节，豁然开朗。——ARM嵌入式系统开发：软件设计与优化的英文原版——我个人感觉这是国内翻译ARM书籍最好的一本之一，比杜XX的ARM体系结构与编程好千倍。 本书虽然说软件设计与优化，但是讲的硬件也很多，比如MMU和cache等，讲的精彩纷呈：我刚才想写关于MMU和cache的博客，发现太庞大，看来这段时间要重新看看这本书才能写。 
下载地址：[http://download.csdn.net/source/904273](http://download.csdn.net/source/904273)
）
————————下面来看看
**2.4 Exceptions, Interrupts, and the Vector Table**
When an exception or interrupt occurs, the processor sets the pc to a speciﬁc memory
address. The address is within a special address range called the vector table. The entries
in the vector table are instructions that branch to speciﬁc routines designed to handle a
particular exception or interrupt.
——当异常或者中断发生的时候，处理器设置PC为一个特殊的内存地址。这个地址叫做中断向量表。中断向量表入口是中断、异常的分支入口（(*^__^*) 嘻嘻……，这个翻译的好烂，不过大家知道就可以了）。
The memory map address 0x00000000 is reserved for the vector table, a set of 32-bit
words. On some processors the vector table can be optionally located at a higher address
in memory (starting at the offset 0xffff0000). Operating systems such as Linux and
Microsoft’s embedded products can take advantage of this feature.
——内存映射地址0x00000000 是为中断向量表保留的。在某些处理器中断向量表地址为0xffff0000。某些操作系统如linux可以利用这个特征（其实wince就是采用0xffff0000作为中断向量表的地址，但是令人奇怪的是优龙的ADS bootloader的中断向量表地址是0x00000000 ，估计这是编译器决定了）。
When an exception or interrupt occurs, the processor suspends normal execution and
starts loading instructions fromthe exception vector table (see Table 2.6). Each vector table
entry contains a form of branch instruction pointing to the start of a speciﬁc routine:
——当异常或者中断发生的时候，处理器挂起正常执行的程序并开始加载中断向量表，每个中断入口包含一个指向 speciﬁc routine（这个不知道怎么翻译）的分支指令。
■ Reset vector is the location of the ﬁrst instruction executed by the processor when power
is applied. This instruction branches to the initialization code.
——复位向量是打开电源被处理器执行的第一条指令，这条指令branches to初始化代码
■ Undeﬁned instruction vector is used when the processor cannot decode an instruction.
——未定义指令向量，被用在处理器无法解码指令的时候
■ Software interrupt vector is called when you execute a SWI instruction. The SWI
instruction is frequently used as themechanismto invoke an operating systemroutine.
——软件中断向量........软件中断指令常用在操作系统条用。
■ Prefetch abort vector occurs when the processor attempts to fetch an instruction froman
address without the correct access permissions. The actual abort occurs in the decode
stage.
——预取终止向量发生处理器试图取一个指令地址，且没有正确访问许可的时候，实际终止发生在解码阶段
■ Data abort vector is similar to a prefetch abort but is raised when an instruction attempts
to access data memory without the correct access permissions.
——数据终止向量类似预取终止，但是他发生在指令试图访问数据，且内存没有正确访问许可。
■ Interrupt request vector is used by external hardware to interrupt the normal execution
ﬂow of the processor. It can only be raised if IRQs are not masked in the cpsr.
——中断申请向量被用在外部硬件中断正常执行的程序，它只能在IRQs 没有被cpsr屏蔽的情况下。
![](https://p-blog.csdn.net/images/p_blog_csdn_net/gooogleman/EntryImages/20081224/%E4%B8%AD%E6%96%AD%E5%90%91%E9%87%8F%E8%A1%A8.jpg)
====================================================================================
总结：这个中断向量的查找是由硬件实现的，硬件指定PC跳到0xffff0000/0x00000000（具体看编译器），以前我总是从软件角度去学习，大错特错。
转载请标明:作者[wogoyixikexie@gliet](mailto:wogoyixikexie@gliet).桂林电子科技大学一系科协，原文地址：[http://blog.csdn.net/gooogleman](http://blog.csdn.net/gooogleman)——如有错误，希望能够留言指出;如果你有更加好的方法，也请在博客后面留言，我会感激你的批评和分享。
