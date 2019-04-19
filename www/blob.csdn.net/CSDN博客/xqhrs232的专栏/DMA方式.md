# DMA方式 - xqhrs232的专栏 - CSDN博客
2014年03月21日 17:18:28[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1246
原文地址::[http://baike.baidu.com/link?url=9lTFKE1BzRYE0FYvMgpMGpT8BLlBnc7D6jWXDlULu11i7sl6jBSwV_7ATYpLXrC0SP-W_H2pgTyTc79TxjeVYK](http://baike.baidu.com/link?url=9lTFKE1BzRYE0FYvMgpMGpT8BLlBnc7D6jWXDlULu11i7sl6jBSwV_7ATYpLXrC0SP-W_H2pgTyTc79TxjeVYK)

DMA方式，Direct Memory Access，也称为成组[数据传送](http://baike.baidu.com/view/5593592.htm)方式，有时也称为直接内存操作。
## 目录
1[工作原理](http://baike.baidu.com/link?url=9lTFKE1BzRYE0FYvMgpMGpT8BLlBnc7D6jWXDlULu11i7sl6jBSwV_7ATYpLXrC0SP-W_H2pgTyTc79TxjeVYK#1)
▪[原理](http://baike.baidu.com/link?url=9lTFKE1BzRYE0FYvMgpMGpT8BLlBnc7D6jWXDlULu11i7sl6jBSwV_7ATYpLXrC0SP-W_H2pgTyTc79TxjeVYK#1_1)▪[基本操作](http://baike.baidu.com/link?url=9lTFKE1BzRYE0FYvMgpMGpT8BLlBnc7D6jWXDlULu11i7sl6jBSwV_7ATYpLXrC0SP-W_H2pgTyTc79TxjeVYK#1_2)▪[用途](http://baike.baidu.com/link?url=9lTFKE1BzRYE0FYvMgpMGpT8BLlBnc7D6jWXDlULu11i7sl6jBSwV_7ATYpLXrC0SP-W_H2pgTyTc79TxjeVYK#1_3)
2[方式特点](http://baike.baidu.com/link?url=9lTFKE1BzRYE0FYvMgpMGpT8BLlBnc7D6jWXDlULu11i7sl6jBSwV_7ATYpLXrC0SP-W_H2pgTyTc79TxjeVYK#2)
3[传送方式](http://baike.baidu.com/link?url=9lTFKE1BzRYE0FYvMgpMGpT8BLlBnc7D6jWXDlULu11i7sl6jBSwV_7ATYpLXrC0SP-W_H2pgTyTc79TxjeVYK#3)
4[工作过程](http://baike.baidu.com/link?url=9lTFKE1BzRYE0FYvMgpMGpT8BLlBnc7D6jWXDlULu11i7sl6jBSwV_7ATYpLXrC0SP-W_H2pgTyTc79TxjeVYK#4)
## 1工作原理[编辑](http://baike.baidu.com/link?url=9lTFKE1BzRYE0FYvMgpMGpT8BLlBnc7D6jWXDlULu11i7sl6jBSwV_7ATYpLXrC0SP-W_H2pgTyTc79TxjeVYK#)
### 原理
一个[设备](http://baike.baidu.com/view/209665.htm)接口试图通过[总线](http://baike.baidu.com/view/1389.htm)直接向另一个设备发送数据(一般是大批量的数据)，
![](http://g.hiphotos.baidu.com/baike/s%3D220/sign=57a637f1a9014c081d3b2fa73a7a025b/8644ebf81a4c510f677b58426059252dd42aa52f.jpg)
它会先向[CPU](http://baike.baidu.com/view/2089.htm)发送DMA请求[信号](http://baike.baidu.com/view/54338.htm)。外设通过DMA的一种专门接口电路――DMA[控制器](http://baike.baidu.com/view/122229.htm)（DMAC），向CPU提出接管[总线](http://baike.baidu.com/view/1389.htm)控制权的总线请求，CPU收到该[信号](http://baike.baidu.com/view/54338.htm)后，在当前的[总线周期](http://baike.baidu.com/view/579623.htm)结束后，会按DMA[信号](http://baike.baidu.com/view/54338.htm)的优先级和提出DMA请求的先后顺序响应DMA信号。CPU对某个[设备](http://baike.baidu.com/view/209665.htm)接口响应DMA请求时，会让出[总线](http://baike.baidu.com/view/1389.htm)控制权。于是在DMA[控制器](http://baike.baidu.com/view/122229.htm)的管理下，外设和[存储器](http://baike.baidu.com/view/87697.htm)直接进行数据交换，而不需CPU干预。[数据传送](http://baike.baidu.com/view/5593592.htm)完毕后，[设备](http://baike.baidu.com/view/209665.htm)接口会向CPU发送DMA结束[信号](http://baike.baidu.com/view/54338.htm)，交还[总线](http://baike.baidu.com/view/1389.htm)控制权。
**DMA方式的主要优点是速度快。**
由 于CPU根本不参加传送操作，因此就省去了CPU取指令、取数、送数等操作。在[数据传送](http://baike.baidu.com/view/5593592.htm)过程中，没有保存现场、[恢复现场](http://baike.baidu.com/view/5131357.htm)之类的工作。[内存地址](http://baike.baidu.com/view/404417.htm)修改、传送字
 个数的计数等等，也不是由[软件](http://baike.baidu.com/view/37.htm)实现，而是用硬件线路直接实现的。所以DMA方式能满足高速I/O[设备](http://baike.baidu.com/view/209665.htm)的要求，也有利于CPU效率的发挥。
### 基本操作
实现DMA传送的基本操作如下：
1、外设可通过DMA[控制器](http://baike.baidu.com/view/122229.htm)向CPU发出DMA请求；
2、CPU响应DMA请求，系统转变为DMA工作方式，并把[总线](http://baike.baidu.com/view/1389.htm)控制权交给DMA[控制器](http://baike.baidu.com/view/122229.htm)；
3、由DMA[控制器](http://baike.baidu.com/view/122229.htm)发送[存储器地址](http://baike.baidu.com/view/5977785.htm)，并决定传送[数据块](http://baike.baidu.com/view/702806.htm)的长度；
4、执行DMA传送；
5、DMA操作结束，并把[总线](http://baike.baidu.com/view/1389.htm)控制权交还CPU。
### 用途
DMA方式主要适用于一些高速的[I/O设备](http://baike.baidu.com/view/1293973.htm)。这些[设备](http://baike.baidu.com/view/209665.htm)传输字节或字的速度非常快。对于这类高速I/O[设备](http://baike.baidu.com/view/209665.htm)，如果用输入输出指令或采用中断的方法来传输字节信息，会大量占用CPU的时间，同时也容易造成数据的丢失。而DMA方式能使I/O[设备](http://baike.baidu.com/view/209665.htm)直接和[存储器](http://baike.baidu.com/view/87697.htm)进行成批数据的快速传送。
DMA[控制器](http://baike.baidu.com/view/122229.htm)或接口一般包括四个[寄存器](http://baike.baidu.com/view/6159.htm)：
1：状态控制寄存器、
2：[数据寄存器](http://baike.baidu.com/view/1547752.htm)、
3：[地址寄存器](http://baike.baidu.com/view/178150.htm)、
4：字节计数器。
这些[寄存器](http://baike.baidu.com/view/6159.htm)在信息传送之前需要进行初始化设置。即在输入输出程序中用[汇编语言](http://baike.baidu.com/view/49.htm)指令对各个[寄存器](http://baike.baidu.com/view/6159.htm)写入初始化控制字。
## 2方式特点[编辑](http://baike.baidu.com/link?url=9lTFKE1BzRYE0FYvMgpMGpT8BLlBnc7D6jWXDlULu11i7sl6jBSwV_7ATYpLXrC0SP-W_H2pgTyTc79TxjeVYK#)
DMA 是所有现代电脑的重要特色，他允许不同速度的硬件装置来沟通，而不需要依于 CPU 的大量 中断 负载。否则，CPU 需要从 来源 把每一片段的资料复制到[暂存器](http://baike.baidu.com/view/238434.htm)，然后把他们再次写回到新的地方。在这个时间中，CPU 对于其他的工作来说就无法使用。
DMA 传输将一个内存区从一个装置复制到另外一个， CPU 初始化这个传输动作，传输动作本身是由 DMA[控制器](http://baike.baidu.com/view/122229.htm)来实行和完成。典型的例子就是移动一个外部内存的区块到芯片内部更快的内存去。像是这样的操作并没有让处理器工作拖延，反而可以被重新排程去处理其他的工作。DMA
 传输对于高效能[嵌入式系统](http://baike.baidu.com/view/6115.htm)算法和网络是很重要的。
举个例子，PC ISA DMA[控制器](http://baike.baidu.com/view/122229.htm)拥有 8 个 DMA 通道，其中的 7 个通道是可以让 PC 的 CPU 所利用。每一个 DMA 通道有一个 16位元 位址[暂存器](http://baike.baidu.com/view/238434.htm)和一个
 16 位元计数暂存器。要初始化资料传输时，装置驱动程式一起设定 DMA 通道的位址和计数[暂存器](http://baike.baidu.com/view/238434.htm)，以及资料传输的方向，读取或写入。然后指示 DMA 硬件开始这个传输动作。当传输结束的时候，装置就会以中断的方式通知 CPU。
但是，DMA传输方式只是减轻了CPU的工作负担；[系统总线](http://baike.baidu.com/view/65714.htm)仍然被占用。特别是在传输大容量文件时，CPU的占用率可能不到10%，但是用户会觉得运行部分程序时系统变得相当的缓慢。主要原因就是在运行这些[应用程序](http://baike.baidu.com/view/330120.htm)（特别是一些大型[软件](http://baike.baidu.com/view/37.htm)），[操作系统](http://baike.baidu.com/view/880.htm)也需要从[系统总线](http://baike.baidu.com/view/65714.htm)传输大量数据；故造成过长的等待时间。
## 3传送方式[编辑](http://baike.baidu.com/link?url=9lTFKE1BzRYE0FYvMgpMGpT8BLlBnc7D6jWXDlULu11i7sl6jBSwV_7ATYpLXrC0SP-W_H2pgTyTc79TxjeVYK#)
DMA传送方式有3种：单元传送方式、块传送方式和on－the－fly传送方式。与外部DMA请求/应答协议不同的是，DMA传送方式定义了每次传送读/写的单元数，如表所示。
![DMA传送方式表](http://c.hiphotos.baidu.com/baike/s%3D250/sign=d3faa8d5b13533faf1b6942b98d2fdca/b7fd5266d0160924613a83c0d40735fae7cd34c9.jpg)
DMA传送方式表
（1）单元传送方式（单字节传送方式）
单元传送方式意味着每个DMA请求对应一对DMA读/写周期，即1个单元读，然后1个单元写。
（2）块传送方式（连续传送方式）
块传送方式意味着在连续4个字的DMA写周期前有连续的4个字的DMA读周期，即4个字突发读，然后4个字突发写，因此传输的数据个数应该是16字节的倍数。
如果传送大小或者DMA计数值不是16的倍数，则DMA将不能完整地传送完数据。假设要传送的数据为50个字节，则3*16=48字节，会导致2个字节不能被传送，DMA在传送48个字节后停止。所以，选择DMA块传送方式时，一定要注意这一点。
（3）on－the－fly传送方式（请求传送方式）
在on－the－fly传送方式下DMA读/写可以同时进行。DMA应答信号通知外部设备去读或者写。同时，存储控制器将产生与读/写相关的控制信号给外部存储器。如果外部设备能够支持on－the－fly传送方式，将会使得外设的数据传输速率大大地增加。
**停止CPU访问内存**
当外围[设备](http://baike.baidu.com/view/209665.htm)要求传送一批数据时，由DMA[控制器](http://baike.baidu.com/view/122229.htm)发一个停止[信号](http://baike.baidu.com/view/54338.htm)给CPU，要求CPU放弃对[地址总线](http://baike.baidu.com/view/238412.htm)、[数据总线](http://baike.baidu.com/view/712987.htm)和有关[控制总线](http://baike.baidu.com/view/712982.htm)的使用权。DMA[控制器](http://baike.baidu.com/view/122229.htm)获得[总线](http://baike.baidu.com/view/1389.htm)控制权以后，开始进行[数据传送](http://baike.baidu.com/view/5593592.htm)。在一批[数据传送](http://baike.baidu.com/view/5593592.htm)完毕后，DMA[控制器](http://baike.baidu.com/view/122229.htm)通知CPU可以使用内存，并把[总线](http://baike.baidu.com/view/1389.htm)控制权交还给CPU。在这种DMA传送过程
 中，CPU基本处于不工作状态或者说保持状态。
优点：控制简单，它适用于数据传输率很高的[设备](http://baike.baidu.com/view/209665.htm)进行[成组传送](http://baike.baidu.com/view/421000.htm)。
缺点：在DMA[控制器](http://baike.baidu.com/view/122229.htm)访内阶段，内存的效能没有充分发挥，相当一部分内存工作周期是空闲的。这是因为，外围[设备](http://baike.baidu.com/view/209665.htm)传送两个数据之间的间隔一般总是大于内存[存储周期](http://baike.baidu.com/view/178487.htm)，即使高速I/O设备也是如此。
**周期挪用**
当I/O[设备](http://baike.baidu.com/view/209665.htm)没有DMA请求时，CPU按程序要求访问内存；一旦I/O[设备](http://baike.baidu.com/view/209665.htm)有DMA请求，则由I/O设备挪用一个或几个内存周期。
I/O[设备](http://baike.baidu.com/view/209665.htm)要求DMA传送时可能遇到两种情况：
（1）此时CPU不需要访内，如CPU正在执行乘法指令。由于乘法指令执行时间较长，此时I/O访内与CPU访内没有冲突，即I/O[设备](http://baike.baidu.com/view/209665.htm)挪用一二个内存周期对CPU执行程序没有任何影响。
（2）I/O[设备](http://baike.baidu.com/view/209665.htm)要求访内时CPU也要求访内，这就产生了访内冲突，在这种情况下I/O设备访内优先，因为I/O访内有时间要求，前一个I/O数据必须在下一个访内请求到来之前存取完毕。显然，在这种情况下I/O[设备](http://baike.baidu.com/view/209665.htm)挪用一二个内存周期，意味着CPU延缓了对指令的执行，或者更明确地说，在CPU执行访内指令的过程中插入DMA请求，挪用了一二个内存周期。
与停止CPU访内的DMA方法比较，[周期挪用](http://baike.baidu.com/view/1881708.htm)的方法既实现了I/O传送，又较好地发挥了内存和CPU的效率，是一种广泛采用的方法。但是I/O设备每一次[周期挪用](http://baike.baidu.com/view/1881708.htm)都有申请[总线](http://baike.baidu.com/view/1389.htm)控制权、建立总线控制权和归还总线控制权的过程，所以传送一个字对内存来说要占用一个周期，但对DMA[控制器](http://baike.baidu.com/view/122229.htm)来说一般要2—5个
 内存周期（视逻辑线路的延迟而定）。因此，[周期挪用](http://baike.baidu.com/view/1881708.htm)的方法适用于I/O[设备](http://baike.baidu.com/view/209665.htm)读写周期大于内存[存储周期](http://baike.baidu.com/view/178487.htm)的情况。
**DMA与CPU交替访内**
如果CPU的工作周期比内存[存取周期](http://baike.baidu.com/view/368187.htm)长很多，此时采用交替访内的方法可以使DMA传送和CPU同时发挥最高的效率。假设CPU工作周期为 1.2μs，内存[存取周期](http://baike.baidu.com/view/368187.htm)小于0.6μs，那么一个CPU周期可分为C1和C2两个分周期，其中C1供DMA[控制器](http://baike.baidu.com/view/122229.htm)访内，C2专供CPU访内。
这种方式不需要[总线](http://baike.baidu.com/view/1389.htm)使用权的申请、建立和归还过程，总线使用权是通过C1和C2分时进行的。CPU和DMA[控制器](http://baike.baidu.com/view/122229.htm)各自有自己的访内[地址寄存器](http://baike.baidu.com/view/178150.htm)、数据寄存
 器和读/写[信号](http://baike.baidu.com/view/54338.htm)等[控制寄存器](http://baike.baidu.com/view/4092816.htm)。在C1周期中，如果DMA[控制器](http://baike.baidu.com/view/122229.htm)有访内请求，可将地址、数据等[信号](http://baike.baidu.com/view/54338.htm)送到[总线](http://baike.baidu.com/view/1389.htm)上。在C2周期中，如CPU有访内请求，同样传送
 地址、数据等[信号](http://baike.baidu.com/view/54338.htm)。事实上，对于[总线](http://baike.baidu.com/view/1389.htm)，这是用C1，C2控制的一个多路转换器，这种总线控制权的转移几乎不需要什么时间，所以对DMA传送来讲效率是很高的。
这种传送方式又称为“透明的DMA”方式，其来由是这种DMA传送对CPU来说，如同透明的玻璃一般，没有任何感觉或影响。在透明的DMA方式下工作，CPU既不停止主程序的运行，也不进入等待状态，是一种高效率的工作方式。当然，相应的硬件逻辑也就更加复杂。
## 4工作过程[编辑](http://baike.baidu.com/link?url=9lTFKE1BzRYE0FYvMgpMGpT8BLlBnc7D6jWXDlULu11i7sl6jBSwV_7ATYpLXrC0SP-W_H2pgTyTc79TxjeVYK#)
**预处理阶段**
测试[设备](http://baike.baidu.com/view/209665.htm)状态；向DMA[控制器](http://baike.baidu.com/view/122229.htm)的设备[地址寄存器](http://baike.baidu.com/view/178150.htm)中送入设备号，并启动设备；向主存地址计数器中送入欲交换数据的主存起始地址；向字计数器中送入欲交换的数据个数
 。
[外部设备](http://baike.baidu.com/view/206732.htm)准备好发送的数据（输入）或上次接收的数据已处理完毕（输出）时，将通知DMA[控制器](http://baike.baidu.com/view/122229.htm)发出DMA请求，申请主存[总线](http://baike.baidu.com/view/1389.htm)。
**数据传送**
**输入操作　**
①.首先从[外部设备](http://baike.baidu.com/view/206732.htm)读入一个字（设每字16位）到DMA[数据缓冲寄存器](http://baike.baidu.com/view/178140.htm)IODR中（如果设备是面向字节的，一次读入一个字节，需要将两个字节装配成一个字）。
②.[外部设备](http://baike.baidu.com/view/206732.htm)发[选通脉冲](http://baike.baidu.com/view/2248996.htm)，使DMA[控制器](http://baike.baidu.com/view/122229.htm)中的DMA请求标志[触发器](http://baike.baidu.com/view/71792.htm)置“1”。
③.DMA[控制器](http://baike.baidu.com/view/122229.htm)向CPU发出[总线](http://baike.baidu.com/view/1389.htm)请求[信号](http://baike.baidu.com/view/54338.htm)（HOLD）。
④.CPU在完成了现行[机器周期](http://baike.baidu.com/view/713240.htm)后，即响应DMA请求，发出[总线](http://baike.baidu.com/view/1389.htm)允许[信号](http://baike.baidu.com/view/54338.htm)（[HLDA](http://baike.baidu.com/view/2947773.htm)），并由DMA[控制器](http://baike.baidu.com/view/122229.htm)发出DMA响应[信号](http://baike.baidu.com/view/54338.htm)，使DMA请求标记[触发器](http://baike.baidu.com/view/71792.htm)复位。此时，由DMA[控制器](http://baike.baidu.com/view/122229.htm)接管[系统总线](http://baike.baidu.com/view/65714.htm)。
⑤.将DMA[控制器](http://baike.baidu.com/view/122229.htm)中主存[地址寄存器](http://baike.baidu.com/view/178150.htm)中的主存地址送[地址总线](http://baike.baidu.com/view/238412.htm)，
⑥.将DMA[数据缓冲寄存器](http://baike.baidu.com/view/178140.htm)中的内容送[数据总线](http://baike.baidu.com/view/712987.htm)。
⑦.在读/写[控制信号](http://baike.baidu.com/view/8407048.htm)线上发出写命令。
⑧.将DMA[地址寄存器](http://baike.baidu.com/view/178150.htm)的内容加1，从而得到下一个地址，字计数器减1。
⑨.判断字计数器的值是否为“0”。若不为“0”，说明[数据块](http://baike.baidu.com/view/702806.htm)没有传送完毕，返回⑤，传送下一个数据；若为“0”，说明[数据块](http://baike.baidu.com/view/702806.htm)已经传送完毕，则向CPU申请[中断处理](http://baike.baidu.com/view/4499229.htm)。
**输出操作**
①.当DMA[数据缓冲寄存器](http://baike.baidu.com/view/178140.htm)已将输出数据送至I/O[设备](http://baike.baidu.com/view/209665.htm)后，表示数据缓冲寄存器为“空”；
②.外部设备发选通脉冲，使DMA控制器中的DMA请求标志触发器置“1”；
③.DMA控制器向CPU发出总线请求信号（HOLD）；
④.CPU在完成了现行机器周期后，即响应DMA请求，发出总线允许信号（HLDA），并由DMA控制器发出DMA响应信号，使DMA请求标记触发器复位。此时，由DMA控制器接管系统总线；
⑤.将DMA控制器中主存地址寄存器中的主存地址送地址总线，在读/写控制信号线上发出读命令；
⑥.主存将相应地址单元的内容通过[数据总线](http://baike.baidu.com/view/712987.htm)读入到DMA[数据缓冲寄存器](http://baike.baidu.com/view/178140.htm)中；
⑦.将DMA数据缓冲寄存器的内容送到输出[设备](http://baike.baidu.com/view/209665.htm)；
⑧.将DMA地址寄存器的内容加1，从而得到下一个地址，字计数器减1；
⑨.判断字计数器的值是否为“0”。若不为“0”，说明数据块没有传送完毕，返回到⑤，传送下一个数据；若为“0”，说明数据块已经传送完毕，则向CPU申请中断处理。
**传送后处理**
校验送入主存的数据是否正确。
决定是否继续用DMA传送其他[数据块](http://baike.baidu.com/view/702806.htm)。
测试在传送过程中是否发生错误。
