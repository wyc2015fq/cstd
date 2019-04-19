# FPGA结构 - Koma Hub - CSDN博客
2019年02月25日 21:27:48[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：53
个人分类：[FPGA](https://blog.csdn.net/Rong_Toa/article/category/8694195)
对PROM、EPROM、E2PROM 熟悉的人都知道这些可编程器件的可编程原理是通过加高压或紫外线导致三极管或MOS 管内部的载流子密度发生变化，实现所谓的可编程，但是这些器件或只能实现单次可编程或编程状态难以稳定。FPGA 则不同，它采用了逻辑单元阵列LCA(Logic Cell Array) 这样一个新概念，内部包括可配置逻辑模块CLB(Configurable Logic Block)、输出输入模块IOB(Input Output Block) 和内部连线(Interconnect)三个部分。
FPGA 的可编程实际上是改变了CLB 和IOB 的触发器状态，这样，可以实现多次重复的编程由于FPGA 需要被反复烧写，它实现组合逻辑的基本结构不可能像ASIC 那样通过固定的与非门来完成，而只能采用一种易于反复配置的结构。查找表可以很好地满足这一要求，目前主流FPGA 都采用了基于SRAM 工艺的查找表结构，也有一些军品和宇航级FPGA 采用Flash 或者熔丝与反熔丝工艺的查找表结构。通过烧写文件改变查找表内容的方法来实现对FPGA 的重复配置。
根据数字电路的基本知识可以知道，对于一个n 输入的逻辑运算，不管是与或非运算还是异或运算等等，最多只可能存在2n 种结果。所以如果事先将相应的结果存放于一个存贮单元，就相当于实现了与非门电路的功能。FPGA 的原理也是如此，它通过烧写文件去配置查找表的内容，从而在相同的电路情况下实现了不同的逻辑功能。
查找表(Look-Up-Table) 简称为LUT，LUT 本质上就是一个RAM。目前FPGA 中多使用4 输入的LUT，所以每一个LUT 可以看成一个有4 位地址线的 的RAM。 当用户通过原理图或HDL 语言描述了一个逻辑电路以后，PLD/FPGA 开发软件会自动计算逻辑电路的所有可能结果，并把真值表( 即结果) 事先写入RAM，这样，每输入一个信号进行逻辑运算就等于输入一个地址进行查表，找出地址对应的内容，然后输出即可。
**输入与门的真值表**
![输入与门的真值表](https://img-blog.csdnimg.cn/20190225212336860.png)
从表中可以看到，LUT 具有和逻辑电路相同的功能。实际上，LUT 具有更快的执行速度和更大的规模。由于基于LUT 的FPGA 具有很高的集成度，其器件密度从数万门到数千万门不等，可以完成极其复杂的时序与逻辑组合逻辑电路功能，所以适用于高速、高密度的高端数字逻辑电路设计领域。其组成部分主要有可编程输入/ 输出单元、基本可编程逻辑单元、内嵌SRAM、丰富的布线资源、底层嵌入功能单元、内嵌专用单元等，主要设计和生产厂家有赛灵思、Altera、Lattice、Actel、Atmel 和QuickLogic 等公司，其中最大的是美国赛灵思公司，占有可编程市场50% 以上的市场份额，比其他所有竞争对手市场份额的总和还多。
　　FPGA 是由存放在片内RAM 中的程序来设置其工作状态的，因此，工作时需要对片内的RAM 进行编程。用户可以根据不同的配置模式，采用不同的编程方式。
加电时，FPGA 芯片将EPROM 中数据读入片内编程RAM 中，配置完成后，FPGA 进入工作状态。掉电后，FPGA 恢复成白片，内部逻辑关系消失，因此，FPGA 能够反复使用。FPGA 的编程无须专用的FPGA 编程器，只须用通用的EPROM、PROM 编程器即可。这样，同一片FPGA，不同的编程数据，可以产生不同的电路功能。因此，FPGA 的使用非常灵活。
**被广泛应用的Xilinx Spartan－3系列FPGA**
![](https://img-blog.csdnimg.cn/20190225212525246.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
如前所述，FPGA 是由存放在片内的RAM 来设置其工作状态的，因此工作时需要对片内RAM 进行编程。用户可根据不同的配置模式，采用不同的编程方式。Xilinx FPGA 的常用配置模式有5 类：主串模式、从串模式、Select MAP 模式、Desktop 配置和直接SPI 配置。
目前，FPGA 市场占有率最高的两大公司赛灵思公司和Altera 生产的FPGA 都是基于SRAM 工艺的，需要在使用时外接一个片外存储器以保存程序。上电时，FPGA 将外部存储器中的数据读入片内RAM，完成配置后，进入工作状态；掉电后FPGA 恢复为白片，内部逻辑消失。这样FPGA 不仅能反复使用，还无需专门的FPGA编程器，只需通用的EPROM、PROM 编程器即可。Actel、QuickLogic 等公司还提供反熔丝技术的FPGA，具有抗辐射、耐高低温、低功耗和速度快等优点，在军品和航空航天领域中应用较多，但这种FPGA 不能重复擦写，开发初期比较麻烦，费用也比较昂贵。Lattice 是ISP 技术的发明者，在小规模PLD 应用上有一定的特色。早期的赛灵思公司产品一般不涉及军品和宇航级市场，但目前已经有多款产品进入该类领域。
**FPGA芯片内部结构**
![](https://img-blog.csdnimg.cn/20190225212638912.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
FPGA 芯片结构目前主流的FPGA 仍是基于查找表技术的，已经远远超出了先前版本的基本性能，并且整合了常用功能( 如RAM、时钟管理和DSP) 的硬核(ASIC 型) 模块。如上图所示( 注：图只是一个示意图，实际上每一个系列的FPGA 都有其相应的内部结构)，FPGA 芯片主要由6 部分完成，分别为：可编程输入输出单元、基本可编程逻辑单元、完整的时钟管理、嵌入块式RAM、丰富的布线资源、内嵌的底层功能单元和内嵌专用硬件模块。
