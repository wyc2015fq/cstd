# 浅谈STM32的DMA模块的使用 - xqhrs232的专栏 - CSDN博客
2013年04月27日 15:37:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：918
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[http://szvcn.blog.163.com/blog/static/1867963200862922147743/](http://szvcn.blog.163.com/blog/static/1867963200862922147743/)
浅谈STM32的DMA模块的使用
              -----转贴
前言：接触MCU的编程已经有几年的历史，刚开始是从PLC（C语言的）学习与应用，后来有学习了8bits的AVR单片机，对MCU有一定的了解；最近接触了STM32的Chip，发现其优点不只是单单宣传的32bits的MCU的速度，而且在编程的风格上也有了较大的变化（与我原有的编程习惯）；特别是有一个与CPU并行运行的模块：DMA，对它印象深刻。现在就和大家一起讨论学习这个Module,谈谈我在学习中一些感悟和大家一起分享；如有对DMA理解和应用上的错误和偏差，欢迎“拍砖”和提出更正，虾米在此感谢大家了。哈哈！
什么是STM32的DMA？其全称是：Direct Memory Access；根据ST公司提供的相关信息，DMA是STM32中一个独立与Cortex-M3内核的模块，有点类似与ADC、PWM、TIMER等模块；主要功能是通信“桥梁”的作用，可以将所有外设映射的寄存器“连接”起来，这样就可以高速问各寄存器，其传输不受CPU的支配，传输还是双向的；例如，从“表面”上看，它可以将flash中的数据与储存器中变量建立通讯，还可以将一外设的积存器或缓冲器与另外设的寄存器或缓冲器建立双向通讯，有点像把外设硬件之间用“导线”连接在一起了。其间的通讯不占CPU资源，访问速度高，对于实时性强的应用将是一个很好的选择；当然，对于实时性非常强的，建议还是采用专用的DSP芯片。
过程：怎样启用DMA？首先，众所周知的是初始化，任何设备启用前都要对其进行初始化，要对模块初始化，还要先了解该模块相应的结构及其函数，以便正确的设置；由于DMA较为复杂，我就只谈谈DMA的基本结构和和常用函数，这些都是ST公司提供在库函数中的。
1、  下面代码是一个标准DMA设置，当然实际应用中可根据实际情况进行裁减：
  DMA_DeInit(DMA_Channel1);
  上面这句是给DMA配置通道，根据ST提供的资料，STM3210Fx中DMA包含7个通道（CH1~CH7），也就是说可以为外设或memory提供7座“桥梁”（请允许我使用桥梁一词，我觉得更容易理解，哈哈，别“拍砖”呀！）；
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  上面语句中的DMA_InitStructure是一个DMA结构体，在库中有声明了，当然使用时就要先定义了；DMA_PeripheralBaseAddr是该结构体中一个数据成员，给DMA一个起始地址，好比是一个buffer起始地址，数据流程是：外设寄存器à DMA_PeripheralBaseAddàmemory中变量空间（或flash中数据空间等），ADC1_DR_Address是我定义的一个地址变量；
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;
  上面这句很显然是DMA要连接在Memory中变量的地址，ADC_ConvertedValue是我自己在memory中定义的一个变量；
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  上面的这句是设置DMA的传输方向，就如前面我所说的，DMA可以双向传输，也可以单向传输，这里设置的是单向传输，如果需要双向传输：把DMA_DIR_PeripheralSRC改成DMA_DIR_PeripheralDST即可。
 DMA_InitStructure.DMA_BufferSize = 2;
  上面的这句是设置DMA在传输时缓冲区的长度，前面有定义过了buffer的起始地址：ADC1_DR_Address ，为了安全性和可靠性，一般需要给buffer定义一个储存片区，这个参数的单位有三种类型：Byte、HalfWord、word，我设置的2个half-word(见下面的设置)；32位的MCU中1个half-word占16 bits。
 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  上面的这句是设置DMA的外设递增模式，如果DMA选用的通道（CHx）有多个外设连接，需要使用外设递增模式：DMA_PeripheralInc_Enable;我的例子里DMA只与ADC1建立了联系，所以选用DMA_PeripheralInc_Disable
DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
上面的这句是设置DMA的内存递增模式，DMA访问多个内存参数时，需要使用DMA_MemoryInc_Enable，当DMA只访问一个内存参数时，可设置成：DMA_MemoryInc_Disable。
 DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
上面的这句是设置DMA在访问时每次操作的数据长度。有三种数据长度类型，前面已经讲过了，这里不在叙述。
DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
与上面雷同。在此不再说明。
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  上面的这句是设置DMA的传输模式：连续不断的循环模式，若只想访问一次后就不要访问了（或按指令操作来反问，也就是想要它访问的时候就访问，不要它访问的时候就停止），可以设置成通用模式：DMA_Mode_Normal
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  上面的这句是设置DMA的优先级别：可以分为4级：VeryHigh，High,Medium,Low.
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  上面的这句是设置DMA的2个memory中的变量互相访问的
  DMA_Init(DMA_Channel1,&DMA_InitStructure);
  前面那些都是对DMA结构体成员的设置，在次再统一对DMA整个模块做一次初始化，使得DMA各成员与上面的参数一致。
  /*DMA Enable*/
  DMA_Cmd(DMA_Channel1,ENABLE);
  哈哈哈！这一句我想我就不罗嗦了，大家一看就明白。
至此，整个DMA总算设置好了，但是，DMA通道又是怎样与外设联系在一起的呢？哈哈，这也是我当初最想知道的一个事情，别急！容我想喝口茶~~~~~~哈哈哈！
要使DMA与外设建立有效连接，这不是DMA自身的事情，是各个外设的事情，每个外设都有 一个xxx_DMACmd(XXXx,Enable )函数，如果使DMA与ADC建立有效联系，就使用ADC_DMACmd(ADC1,Enable); （这里我启用了ADC中的ADC1模块）。
下面就以我的一个实例来和大家一起学习。这个实例仿真是成功的，我使用的是IAR Embedded Workbench IDE(v4.42) 和Manley Mini-kit评估板+ST-Link II来调试，是一个USART+ADC+DMA的例子。
首先，按照相关的资料提示，建立好项目路径、拷贝库文件和相关必要的文件，并建立一个新工程；还有就是设置好IAR IDE的相关设置。一切准备就绪后就开始修改相关的文档：
 conf.h文档的修改是根据应用中有启用的相关功能Module。如下图：
ADC模块的启用设置：
![浅谈STM32的DMA模块的使用 - 雪舞九天 -   雪舞九天](http://space.ednchina.com/Upload/Blog/2008/6/5/477ef397-65d9-487b-b557-b325a39deee6.JPG)
DMA模块的启用设置：
![浅谈STM32的DMA模块的使用 - 雪舞九天 -   雪舞九天](http://space.ednchina.com/Upload/Blog/2008/6/5/7393b185-38f0-4c81-9c08-88d5397599a4.JPG)
USART模块启用的设置：
![浅谈STM32的DMA模块的使用 - 雪舞九天 -   雪舞九天](http://space.ednchina.com/Upload/Blog/2008/6/5/c1807ec5-a0ee-4eee-866a-901442e0fd11.JPG)
其他的如flash、GPIO、RCC、NVIC、HSE等就不一一举例，因为这些几乎每个工程都要用到。
再回头看看我的main.c文档，
下图是相关的私有定义
![浅谈STM32的DMA模块的使用 - 雪舞九天 -   雪舞九天](http://space.ednchina.com/Upload/Blog/2008/6/5/973c63e4-2e39-42a3-8934-579486bca721.JPG)
USART的配制如下（9600，8， N，1）：
![浅谈STM32的DMA模块的使用 - 雪舞九天 -   雪舞九天](http://space.ednchina.com/Upload/Blog/2008/6/5/06dc1f9f-885e-4935-ae06-26c3580893f3.JPG)
DMA的配置如下：
![浅谈STM32的DMA模块的使用 - 雪舞九天 -   雪舞九天](http://space.ednchina.com/Upload/Blog/2008/6/5/957a564a-808b-4192-a732-8d81cf63852f.JPG)
ADC的配置如下图（采用了2路A/D多通道的ADC模式）：
![浅谈STM32的DMA模块的使用 - 雪舞九天 -   雪舞九天](http://space.ednchina.com/Upload/Blog/2008/6/5/8d5cfff1-393f-418c-ba1f-fb196aa7b59c.JPG)
给各个模块配置的时钟使能如下图；在此想罗嗦一句就是在这个位置的设置我没有给DMA配置时钟，造成DMA功能无法正常使用，也就是前面所有说的与我原先的编程风格有较大的变化的原因，想说一句：在项目中所使用的所有功能模块都要在此配置时钟才能正常工作。
![浅谈STM32的DMA模块的使用 - 雪舞九天 -   雪舞九天](http://space.ednchina.com/Upload/Blog/2008/6/5/6d7c1b65-fb68-49aa-b32f-1f612707fc15.JPG)
GPIO的配置
![浅谈STM32的DMA模块的使用 - 雪舞九天 -   雪舞九天](http://space.ednchina.com/Upload/Blog/2008/6/5/b06416d4-2216-4dfd-9957-f7347caa6f26.JPG)
如果有开中断子程序，请在在NVIC中配置，还可以定义每个中断子程序的优先级别，由于我的工程没有用到，在此就不罗嗦了。
