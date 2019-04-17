# 内存、cache和寄存器之间的关系及区别 - Machine Learning with Peppa - CSDN博客





2018年01月12日 22:51:27[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：1376
所属专栏：[C语言入门](https://blog.csdn.net/column/details/18792.html)








cache是一个高速小容量的临时存储器，可以用高速的[静态存储器](https://www.baidu.com/s?wd=%E9%9D%99%E6%80%81%E5%AD%98%E5%82%A8%E5%99%A8&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1dBrA7-P1P9uhD3n1NhnHfv0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHRsnj6drj0z)芯片实现，或者集成到CPU芯片内部，存储CPU最经常访问的指令或者操作数据。
而寄存器不同,寄存器是内存阶层中的最顶端，也是系统获得操作资料的最快速途径。
三者之间的顺序就是 ：内存-cahe-寄存器

此外，寄存器存放的是当前CPU环境以及任务环境的数据,而cahe则存放最近经常访问的指令和数据的.


把CPU比做大脑  寄存器就像你正在思考的问题,而cahe就是你的记忆(临时的)

大脑的其他部分存储了记忆,但是大脑直接处理比较慢则需要一个更加快的地方来临时存放你从大脑其他部分提取的记忆这个地方就是cache更加严格的区别定义如下：
**1. 寄存器**是中央处理器内的组成部份。寄存器是有限存贮容量的高速存贮部件，它们可用来暂存指令、数据和位址。在中央处理器的**控制部件**中，包含的寄存器有**指令寄存器(IR)**和**程序计数器(PC)。**在中央处理器的**算术及逻辑部件**中，包含的寄存器有**累加器(ACC)。2. 内存包含的范围非常广，一般分为只读存储器（ROM）、随机存储器（RAM）和高速缓存存储器（cache）。**

3. 寄存器是CPU内部的元件，寄存器拥有非常高的读写速度，所以在寄存器之间的数据传送非常快。
4. **Cache** ：**即高速缓冲存储器**，**是位于CPU与主内存间的一种容量较小但速度很高的存储器**。由于CPU的速度远高于主内存，CPU直接从内存中存取数据要等待一定时间周期，**Cache中保存着CPU刚用过或循环使用的一部分数据**，当CPU再次使用该部分数据时可从Cache中直接调用,这样就减少了CPU的等待时间,提高了系统的效率。**Cache又分为一级Cache(L1 Cache)和二级Cache(L2 Cache)，L1 Cache集成在CPU内部**，L2 Cache早期一般是焊在主板上,**现在也都集成在CPU内部**，常见的容量有256KB或512KB L2 Cache。

总结：**大致来说数据是通过内存-Cache-寄存器，Cache缓存则是为了弥补CPU与内存之间运算速度的差异而设置的的部件。**



