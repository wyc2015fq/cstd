# CPU指令集介绍 - 战斗蜗牛的专栏 - CSDN博客





2014年04月05日 01:12:51[vbskj](https://me.csdn.net/vbskj)阅读数：1118









（1）什么是指令集


参考：

[http://product.pconline.com.cn/itbk/bjbzj/notebook/1109/2522116.html](http://product.pconline.com.cn/itbk/bjbzj/notebook/1109/2522116.html)



所谓指令集，**就是CPU中用来计算和控制计算机系统的一套指令的集合**，而每一种新型的CPU在设计时就规定了一系列与其他硬件电路相配合的指令系统。而指令集的先进与否，也关系到CPU的性能发挥，它也是CPU性能体现的一个重要标志。



通俗的理解，指令集就是CPU能认识的语言，指令集运行于一定的微架构之上，不同的微架构可以支持相同的指令集，比如Intel和AMD的CPU的微架构是不同的，但是同样支持X86指令集，这很容易理解，指令集只是一套指令集合，一套指令规范，具体的实现，仍然依赖于CPU的翻译和执行。就像，同样是一段C语言代码，我们可以用不同的编译器去编译得到不同的可执行文件，当然，自然而言，效率也可能不一样。


（2）指令集分类：


从大类来分，一般将指令集分为精简指令集和复杂指令集。


精简指令集，即RISC指令集reduced instruction set computer：（[http://baike.baidu.com/view/981569.htm](http://baike.baidu.com/view/981569.htm)）


这种指令集的特点是指令数目少，每条指令都采用标准字长、执行时间短、中央处理器的实现细节对于机器级程序是可见的。



复杂指令集，即CISC指令集Complex Instruction Set Computer：（[http://baike.baidu.com/view/1177592.htm](http://baike.baidu.com/view/1177592.htm)）


在CISC微处理器中，程序的各条指令是按顺序串行执行的，每条指令中的各个操作也是按顺序串行执行的。顺序执行的优点是控制简单，但计算机各部分的利用率不高，执行速度慢。



通俗的理解，RICS指令集是针对CISC指令集中的一些常用指令进行优化设计，放弃了一些复杂的指令，对于复杂的功能，需要通过组合指令来完成。自然，两者的使用场合不一样，对于复杂的系统，CISC更合适，否则，RICS更合适，且低功耗。


注意：当初本没有RICS和CISC之分，最开始，Intel X86的第一个CPU定义了第一套指令集，这就是最开始的指令集，后来，一些公司发现很多指令并不常用，所以决定设计一套简洁高效的指令集，称之为RICS指令集，从而将原来的Intel X86指令集定义为CISC指令集。所以，并不是先有RICS后来才有CISC，而是反过来的。


典型的RICS指令集的CPU有：ARM、MIPS等


典型的CICS指令集的CPU有：Intel的x86指令集，以及现在的AMD的x86-64指令集。PS：AMD的兼容CPU也支持x86指令集，反之。


（3）指令集发展：


上面的分类是一个大致的分类，指令集是一直在发展的，在CISC指令集中，慢慢的发展了一系列的指令集：


1. X86指令集：


X86指令集是Intel为其第一块16位CPU(i8086)专门开发的，IBM1981年推出的世界第一台PC机中的CPU—i8088(i8086简化版)使用的也是X86指令，同时电脑中为提高浮点数据处理能力而增加的X87芯片系列数学协处理器则另外使用X87指令，以后就将X86指令集和X87指令集统称为X86指令集。


2. MMX指令集：


1997年Intel公司推出了多媒体扩展指令集MMX（MultiMedia eXtensions），它包括57条多媒体指令。MMX指令主要用于增强CPU对多媒体信息的处理能力，提高CPU处理3D图形、视频和音频信息的能力。


3. SSE指令集：Streaming SIMD Extensions


由于MMX指令并没有带来3D游戏性能的显著提升，所以，1999年Inter公司在Pentium III CPU产品中推出了数据流单指令序列扩展指令（SSE）。SSE兼容MMX指令，它可以通过SIMD（单指令多数据技术）和单时钟周期并行处理多个浮点来有效地提高浮点运算速度。



4. SSE2指令集：


在Pentium 4 CPU中，Inter公司开发了新指令集SSE2。这一次新开发的SSE2指令一共144条，包括浮点SIMD指令、整形SIMD指令、SIMD浮点和整形数据之间转换、数据在MMX寄存器中转换等几大部分。其中重要的改进包括引入新的数据格式，如：128位SIMD整数运算和64位双精度浮点运算等。



5. SSE3指令集：


相对于SSE2，SSE3又新增加了13条新指令，此前它们被统称为pni(prescott new instructions)。13条指令中，一条用于视频解码，两条用于线程同步，其余用于复杂的数学运算、浮点到整数转换和SIMD浮点运算。


6. SSE4指令集：


SSE4又增加了50条新的增加性能的指令，这些指令有助于编译、媒体、字符/文本处理和程序指向加速。



7. 3D Now!扩展指令集：


3D Now!指令集是AMD公司1998年开发的多媒体扩展指令集，共有21条指令。针对MMX指令集没有加强浮点处理能力的弱点，重点提高了AMD公司K6系列CPU对3D图形的处理能力。由于指令有限，3D Now!指令集主要用于3D游戏，而对其他商业图形应用处理支持不足。


8. EM64T指令集：


Intel公司的EM64T（Extended Memory 64 Technology）即64位内存扩展技术。该技术为服务器和工作站平台应用提供扩充的内存寻址能力，拥有更多的内存地址空间，可带来更大的应用灵活性，特别有利于提升音频视频编辑、CAD设计等复杂工程软件及游戏软件的应用。



9. 3DNow!+指令集：

在原有的指令集基础上，增加到52条指令，其中包含了部分SSE指令，该指令集主要用于新型的AMD CPU上。


10. AVX指令集：


Intel公司将为Sandy Bridge带来全新的指令扩展集Intel Advanced Vector Extensions (Intel AVX)。AVX是在之前的128bit扩展到和256bit的SIMD(Single Instruction, Multiple Data)。而Sandy Bridge的SIMD演算单元扩展到256bits的同时数据传输也获得了提升，所以从理论上看CPU内核浮点运算性能提升到了2倍。






总结：可以看到，CPU指令集是一只在不断发展的，随着需求的不断增加，指令集也在不断扩展，从而提高CPU的性能。RICS指令集一般用于嵌入式等场合，所以指令集并没有太多的扩展。





参考：

[http://baike.soso.com/v126959.htm](http://baike.soso.com/v126959.htm)


[http://baike.baidu.com/view/9174.htm](http://baike.baidu.com/view/9174.htm)


[http://product.pconline.com.cn/itbk/bjbzj/notebook/1109/2522116.html](http://product.pconline.com.cn/itbk/bjbzj/notebook/1109/2522116.html)



