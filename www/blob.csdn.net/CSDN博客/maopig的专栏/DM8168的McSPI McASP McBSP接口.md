# DM8168的McSPI/McASP/McBSP接口 - maopig的专栏 - CSDN博客
2013年02月19日 14:59:01[maopig](https://me.csdn.net/maopig)阅读数：2219
## McSPI接口
**SPI管脚：**
|管脚|类型|描述|
|----|----|----|
|SPI_SCLK|I/O|SPI串行时钟（MASTER时：输出；SLAVE：输入）|
|SPI_D0|I/O|能被配置为输入或输出（MOSI：master out，slave in或MISO：master in slave out）|
|SPI_D1|I/O|能被配置为输入或输出（MOSI：master out，slave in或MISO：master in slave out）|
|/SPI_CS0|I/O|SPI片选0，MASTER时：输出；SLAVE：输入，低有效|
|/SPI_CS1|I/O|SPI片选1，MASTER时：输出；SLAVE：输入，低有效|
|/SPI_CS2|I/O|SPI片选2，MASTER时：输出；SLAVE：输入，低有效|
|/SPI_CS3|I/O|SPI片选3，MASTER时：输出；SLAVE：输入，低有效|
**时序：**
![](https://img-my.csdn.net/uploads/201204/07/1333780264_2372.JPG)
![](https://img-my.csdn.net/uploads/201204/07/1333780292_9080.jpg)
![](https://img-my.csdn.net/uploads/201204/07/1333780306_2034.jpg)
![](https://img-my.csdn.net/uploads/201204/07/1333780313_5224.JPG)
**SPI接口可以进行以下配置：**
- SPI字长
- SPI使能产生可编程
- SPI使能断言
- SPI使能极性
- SPI时钟频率
- SPI时钟相位（PHA）
- SPI时钟极性（POL）
## McASP接口
**McASP0管脚：**
|管脚|类型|描述|
|----|----|----|
|MCA[0]_ACLKR|I/O|McASP0接收位时钟|
|MCA[0]_AHCLKR|I/O|McASP0接收高频主时钟I/O|
|MCA[0]_AFSR|I/O|McASP0接收帧同步IO|
|MCA[0]_AMUTEIN|I/O|McASP0禁音输入|
|MCA[0]_ACLKX|I/O|McASP0发送位时钟|
|MCA[0]_AHCLKX|I/O|McASP0发送高频主时钟I/O|
|MCA[0]_AFSX|I/O|McASP0发送帧同步|
|MCA[0]_AMUTE|O|McASP0禁音输出|
|MCA[0]_AXR[5]|I/O|McASP0发送/接收数据I/Os|
|MCA[0]_AXR[4]|I/O| |
|MCA[0]_AXR[3]|I/O| |
|MCA[0]_AXR[2]|I/O| |
|MCA[0]_AXR[1]|I/O| |
|MCA[0]_AXR[0]|I/O| |
**McASP1管脚：**
|管脚|类型|描述|
|----|----|----|
|MCA[1]_ACLKR|I/O|McASP1接收位时钟|
|MCA[1]_AHCLKR|I/O|McASP1接收高频主时钟I/O|
|MCA[1]_AFSR|I/O|McASP1接收帧同步IO|
|MCA[1]_AMUTEIN|I/O|McASP1禁音输入|
|MCA[1]_ACLKX|I/O|McASP1发送位时钟|
|MCA[1]_AHCLKX|I/O|McASP1发送高频主时钟I/O|
|MCA[1]_AFSX|I/O|McASP1发送帧同步|
|MCA[1]_AMUTE|O|McASP1禁音输出|
|MCA[1]_AXR[1]|I/O|McASP1发送/接收数据I/Os|
|MCA[1]_AXR[0]|I/O| |
**McASP2管脚：**
|管脚|类型|描述|
|----|----|----|
|MCA[2]_ACLKR|I/O|McASP2接收位时钟|
|MCA[2]_AHCLKR|I/O|McASP2接收高频主时钟I/O|
|MCA[2]_AFSR|I/O|McASP2接收帧同步IO|
|MCA[2]_AMUTEIN|I/O|McASP2禁音输入|
|MCA[2]_ACLKX|I/O|McASP2发送位时钟|
|MCA[2]_AHCLKX|I/O|McASP2发送高频主时钟I/O|
|MCA[2]_AFSX|I/O|McASP2发送帧同步|
|MCA[2]_AMUTE|O|McASP2禁音输出|
|MCA[2]_AXR[1]|I/O|McASP2发送/接收数据I/Os|
|MCA[2]_AXR[0]|I/O| |
**几个概念：**
- 位、字、slot、帧
![](https://img-my.csdn.net/uploads/201204/07/1333780319_6036.JPG)
![](https://img-my.csdn.net/uploads/201204/07/1333780325_4800.JPG)
图中，位（bit）b0-b7构成字（word），这个字加4个附着位（pad）构成slot，在这个slot中的对齐方式是左对齐。帧（Frame）由两个slot（slot0、slot1）构成。
- McAsp支持的TDM格式
![](https://img-my.csdn.net/uploads/201204/07/1333780333_8512.JPG)
（A)FS的宽度可以是1bit
FS还可以设置延迟时间：
![](https://img-my.csdn.net/uploads/201204/07/1333780342_9939.JPG)
（A）
FS的宽度可以是1bit
（B）
是上一帧的最后一位，这个最后一位与slot0之间不允许有间隙
- McAsp支持的I2S格式：
![](https://img-my.csdn.net/uploads/201204/07/1333780348_6195.JPG)
（A）
支持AXR0-AXR5共6个数据脚
在I2S方式，我们通常用通道（channel）来代替Slot。一般两个通道：左通道和右通道
l
McASP支持的S/PDIF
McASP的发送器支持S/PDIF格式。S/PDIF格式是将时钟（CLOCK）、帧（FRAME）和数据（Data）编码成一个信号，通过AXRx脚来发送。所以需要了解编码方式、子帧格式、帧格式。
（1）
编码方式（BMC：Biphase-Mark Code）
![](https://img-my.csdn.net/uploads/201204/07/1333780355_7482.JPG)
（2）
子帧格式
![](https://img-my.csdn.net/uploads/201204/07/1333780364_4003.JPG)
|Preamble code|Previous logical state|Logical state on pin AXRn|Description|
|----|----|----|----|
|B（或Z）|0/1|1110 1000/00010111|块的开始和子帧1|
|M（或X）|0/1|1110 0010/00011101|子帧1|
|W（或Y）|0/1|1110 0100/00011011|子帧2|
但是DM8168的DIT仅产生Previous logical state为0的Preamble，因为McASP在发送DIT模式的时候是采用偶极性的编码方案。
（3）
帧格式
![](https://img-my.csdn.net/uploads/201204/07/1333780370_5854.JPG)
## McBSP接口
**管脚定义：**
|管脚|类型|描述|
|----|----|----|
|MCB_CLKR|I/O|McBSP的接收时钟|
|MCB_FSR|I/O|McBSP的接收帧同步|
|MCB_DR|I|McBSP的接收输入输入|
|MCB_CLKX|I/O|McBSP的发送时钟|
|MCB_FSX|I/O|McBSP的发送帧同步|
|MCB_DX|O|McBSP的发送数据输出|
|MCB_CLKS|I|McBSP的源时钟|
**时序：**
![](https://img-my.csdn.net/uploads/201204/07/1333780377_5184.JPG)
![](https://img-my.csdn.net/uploads/201204/07/1333780384_4481.JPG)
我们可以设置每帧的相位数（单相位或双相位）；每帧的字数（对于单相位，每帧字数可以达128，但是对双相位，只能是2个字，对应，每个相位1个字）；每个相位中的字的长度。
- 单相位帧
![](https://img-my.csdn.net/uploads/201204/07/1333780392_1678.JPG)
在这种方式，可以设置每帧的字数（word个数）、每个字（word）的位数（bit数）等
- 双相位帧
![](https://img-my.csdn.net/uploads/201204/07/1333780401_3476.JPG)
这种方式，每个相位，只能设置1个字。在这里第一个相位的字是16bit，第二个相位的字是8bit
## 总结
- **McSPI与McASP和McBSP的区别是非常明显的，不多说**
- **McBSP能实现部分McASP的功能，比如都支持TDM和I2S，因此二者在功能上存在交叉**
