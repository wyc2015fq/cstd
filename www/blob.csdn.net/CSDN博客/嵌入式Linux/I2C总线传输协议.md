
# I2C总线传输协议 - 嵌入式Linux - CSDN博客

2018年09月18日 22:54:44[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：387个人分类：[嵌入式学习																](https://blog.csdn.net/weiqifa0/article/category/1385985)



# 简介
I2C（Inter-integrated Circuit）总线支持设备之间的短距离通信，用于处理器和一些外围设备之间的接口，它只需要两根信号线来完成信息交换。I2C最早是飞利浦在1982年开发设计并用于自己的芯片上，一开始只允许100kHz、7-bit标准地址。1992年，I2C的第一个公共规范发行，增加了400kHz的快速模式以及10-bit扩展地址。在I2C的基础上，1995年Intel提出了“System Management Bus” (SMBus)，用于低速设备通信，SMBus 把时钟频率限制在10kHz~100kHz，但I2C可以支持0kHz~5MHz的设备：普通模式（100kHz即100kbps）、快速模式（400kHz）、快速模式+（1MHz）、高速模式（3.4MHz）和超高速模式（5MHz）。
# 与串行端口、SPI对比
## 串行端口
串行端口是异步的（不传输时钟相关数据），两个设备在使用串口通信时，必须先约定一个数据传输速率，并且这两个设备各自的时钟频率必须与这个速率保持相近，某一方的时钟频率相差很大都会导致数据传输混乱。
异步串行端口在每个数据帧中都要插入至少一个起始位和一个终止位，意味着每传输8bits的数据实际要花费10bits的传输时间，从而降低了数据传输速率。
另一个问题是异步串行端口的设计就是针对两个设备之间通信的，那么如果有多个设备连接到一个串口上，就必须解决信号碰撞的问题(bus contention)，通常要通过额外硬件来完成。
最后就是数据传输速率，异步串行通信并没有一个理论上的速率限制，大部分UART设备只支持一些特定的波特率，最高通常在230400bps左右。
## SPI
SPI最明显的缺点就是引脚数量，使用SPI总线相连的一个master和一个slave需要四根线（MISO/MOSI/SCK/CS），每增加一个slave，就需要在master上增加一个CS引脚。当一个master接多个slaves的时候，疯狂增长的引脚连接是难以忍受的，并且对紧凑的PCB layout是一个挑战。
SPI总线上只允许有一个master，但可以有任意多个slaves（只受限于总线上设备的驱动程序的能力，以及设备上最多能有多少个CS引脚）。
SPI可以很好的用于高速率全双工的连接，对一些设备可支持高达10MHz（10Mbps）的传输速率，因此SPI吞吐量大得多。SPI两端的设备通常是一个简单的移位寄存器，让软件的实现很简单。
## I2C
I2C最少只需要两根线，和异步串口类似，但可以支持多个slave设备。和SPI不同的是，I2C可以支持mul-master系统，允许有多个master并且每个master都可以与所有的slaves通信（master之间不可通过I2C通信，并且每个master只能轮流使用I2C总线）。master是指启动数据传输的设备并在总线上生成时钟信号以驱动该传输，而被寻址的设备都作为slaves。
![这里写图片描述](https://img-blog.csdn.net/20170820192543246?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFzb25jaGVuX2diZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
I2C的数据传输速率位于串口和SPI之间，大部分I2C设备支持100KHz和400KHz模式。使用I2C传输数据会有一些额外消耗：每发送8bits数据，就需要额外1bit的元数据（ACK或NACK）。I2C支持双向数据交换，由于仅有一根数据线，故通信是半双工的。
硬件复杂度也位于串口和SPI之间，而软件实现可以相当简单。
# I2C协议
I2C协议把传输的消息分为两种类型的帧：
一个**地址帧**—— 用于master指明消息发往哪个slave；
一个或多个**数据帧**—— 由master发往slave的数据（或由slave发往master），每一帧是8-bit的数据。
注：协议要求每次放到SDA上的字节长度必须为8位，并且每个字节后须跟一个ACK位，在下面会讲到。
数据在SCL处于低电平时放到SDA上，并在SCL变为高电平后进行采样。读写数据和SCL上升沿之间的时间间隔是由总线上的设备自己定义的，不同芯片可能有差异。
I2C数据传输的时序图如下：
![这里写图片描述](https://img-blog.csdn.net/20170820192701519?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFzb25jaGVuX2diZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**开始条件（start condition）：**
为了标识传输正式启动，master设备会将SCL置为高电平（当总线空闲时，SDA和SCL都处于高电平状态），然后将SDA拉低，这样，所有slave设备就会知道传输即将开始。如果两个master设备在同一时刻都希望获得总线的所有权，那么谁先将SDA拉低，谁就赢得了总线的控制权。在整个通信期间，可以存在多个start来开启每一次新的通信序列（communication sequence），而无需先放弃总线的控制权，后面会讲到这种机制。
**地址帧（address frame）：**
地址帧总是在一次通信的最开始出现。一个7-bit的地址是从最高位（MSB）开始发送的，这个地址后面会紧跟1-bit的操作符，1表示读操作，0表示写操作。
接下来的一个bit是NACK/ACK，当这个帧中前面8bits发送完后，接收端的设备获得SDA控制权，此时接收设备应该在第9个时钟脉冲之前回复一个ACK（将SDA拉低）以表示接收正常，如果接收设备没有将SDA拉低，则说明接收设备可能没有收到数据（如寻址的设备不存在或设备忙）或无法解析收到的消息，如果是这样，则由master来决定如何处理（stop或repeated start condition）。
**数据帧（data frames）：**
在地址帧发送之后，就可以开始传输数据了。Master继续产生时钟脉冲，而数据则由master（写操作）或slave（读操作）放到SDA上。每个数据帧8bits，数据帧的数量可以是任意的，直到产生停止条件。每一帧数据传输（即每8-bit）之后，接收方就需要回复一个ACK或NACK（写数据时由slave发送ACK，读数据时由master发送ACK。当master知道自己读完最后一个byte数据时，可发送NACK然后接stop condition）。
**停止条件（stop condition）：**
当所有数据都发送完成时，master将产生一个停止条件。停止条件定义为：在SDA置于低电平时，将SCL拉高并保持高电平，然后将SDA拉高。
注意，在正常传输数据过程中，当SCL处于高电平时，SDA上的值不应该变化，防止意外产生一个停止条件。
**重复开始条件（repeated start condition）：**
有时master需要在一次通信中进行多次消息交换（例如与不同的slave传输消息，或切换读写操作），并且期间不希望被其他master干扰，这时可以使用“重复开始条件” —— 在一次通信中，master可以产生多次start condition，来完成多次消息交换，最后再产生一个stop condition结束整个通信过程。由于期间没有stop condition，因此master一直占用总线，其他master无法切入。
为了产生一个重复的开始条件，SDA在SCL低电平时拉高，然后SCL拉高。接着master就可以产生一个开始条件继续新的消息传输（按照正常的7-bit/10-bit地址传输时序）。重复开始条件的传输时序如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20170820193918041?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFzb25jaGVuX2diZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**时钟拉伸（clock stretching）：**
有时候，低速slave可能由于上一个请求还没处理完，尚无法继续接收master的后续请求，即master的数据传输速率超过了slave的处理能力。这种情况下，slave可以进行时钟拉伸来要求master暂停传输数据 —— 通常时钟都是由master提供的，slave只是在SDA上放数据或读数据。而时钟拉伸则是slave在master释放SCL后，将SCL主动拉低并保持，此时要求master停止在SCL上产生脉冲以及在SDA上发送数据，直到slave释放SCL（SCL为高电平）。之后，master便可以继续正常的数据传输了。可见时钟拉伸实际上是利用了时钟同步的机制（见下文），只是时钟由slave产生。
如果系统中存在这种低速slave并且slave实现了clock stretching，则master必须实现为能够处理这种情况，实际上大部分slave设备中不包含SCL驱动器的，因此无法拉伸时钟。
所以更完整的I2C数据传输时序图为：
![这里写图片描述](https://img-blog.csdn.net/20170820193931065?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFzb25jaGVuX2diZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**10-bit地址空间：**
上面讲到I2C支持10-bit的设备地址，此时的时序如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20170820194108833?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFzb25jaGVuX2diZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在10-bit地址的I2C系统中，需要两个帧来传输slave的地址。第一个帧的前5个bit固定为b11110，后接slave地址的高2位，第8位仍然是R/W位，接着是一个ACK位，由于系统中可能有多个10-bit slave设备地址的高2bit相同，因此这个ACK可能由多有slave设备设置。第二个帧紧接着第一帧发送，包含slave地址的低8位（7:0），接着该地址的slave回复一个ACK（或NACK）。
注意，10-bit地址的设备和7-bit地址的设备在一个系统中是可以并存的，因为7-bit地址的高5位不可能是b11110。实际上对于7-bit的从设备地址，合法范围为b0001XXX-b1110XXX，’X’表示任意值，因此该类型地址最多有112个（其他为保留地址[1]）。
两个地址帧传输完成后，就开始数据帧的传输了，这和7-bit地址中的数据帧传输过程相同。
# 时钟同步和仲裁
如果两个master都想在同一条空闲总线上传输，此时必须能够使用某种机制来选择将总线控制权交给哪个master，这是通过时钟同步和仲裁来完成的，而被迫让出控制权的master则需要等待总线空闲后再继续传输。在单一master的系统上无需实现时钟同步和仲裁。
## 时钟同步
时钟同步是通过I2C接口和SCL之间的线“与”（wired-AND）来完成的，即如果有多个master同时产生时钟，那么只有所有master都发送高电平时，SCL上才表现为高电平，否则SCL都表现为低电平。
## 总线仲裁
总线仲裁和时钟同步类似，当所有master在SDA上都写1时，SDA的数据才是1，只要有一个master写0，那此时SDA上的数据就是0。一个master每发送一个bit数据，在SCL处于高电平时，就检查看SDA的电平是否和发送的数据一致，如果不一致，这个master便知道自己输掉仲裁，然后停止向SDA写数据。也就是说，如果master一直检查到总线上数据和自己发送的数据一致，则继续传输，这样在仲裁过程中就保证了赢得仲裁的master不会丢失数据。
输掉仲裁的master在检测到自己输了之后也不再产生时钟脉冲，并且要在总线空闲时才能重新传输。
仲裁的过程可能要经过多个bit的发送和检查，实际上两个master如果发送的时序和数据完全一样，则两个master都能正常完成整个的数据传输。
*注：本文是对两篇文档中协议部分的翻译整理，并做了调整和补充，见文末的附录链接。*
## I2C波形
设定I2C读的地址：01101101(0x6d)  10001000(0x88)，注意观察第9个时钟为低电平，表示从设备应答
Slave Read Address：0x6d，ID register value：0x88
![](https://img-blog.csdn.net/20131225144649437)
## I2C源码
收集了两个I2C源码，一个是GPIO口模拟I2C，一个是androitd i2c tools
下载链接如下
链接：https://pan.baidu.com/s/1KQQuATd5Lul_IzKavzoncA 密码：8cd2

---
[1]:[http://www.nxp.com/docs/en/user-guide/UM10204.pdf](http://www.nxp.com/docs/en/user-guide/UM10204.pdf)I2C总线规格书和用户手册Rev.6
[2]:[https://learn.sparkfun.com/tutorials/i2c](https://learn.sparkfun.com/tutorials/i2c)SparkFun I2C手册
喜欢可以关注微信公众号：嵌入式Linux
![](https://img-blog.csdn.net/20180918225826361?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


