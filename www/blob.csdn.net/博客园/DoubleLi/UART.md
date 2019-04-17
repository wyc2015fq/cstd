# UART - DoubleLi - 博客园







**一、S3C2410内置的UART控制器**S3C2410内部具有3个独立的UART控制器，每个控制器都可以工作在**Interrupt（中断）模式**或**DMA（直接内存访问）模式**，也就是说**UART控制器可以在CPU与UART控制器传送数据的时候产生中断或DMA请求**。并且每个UART控制器均具有16字节的FIFO（先入先出寄存器），支持的最高波特率可达到115.2Kbps
图5-11是S3C2410内部UART控制器的结构图

![](http://www.ec66.com/article/pic/2006727050275365.gif)


图5-11 

通过上图我们可以看到，每个UART控制器中大抵分为发送器和接收器两部分，在发送器中如果当前UART处于FIFO模式，则有16B的发送缓冲寄存器，一般在发送数据时CPU会将数据先暂存到这16B中来，如果当前UART处于非FIFO模式，则这16B的缓冲寄存器是没用的，我们只用到了这16B中最低一个字节的缓冲寄存器来存放数据也叫做Transmit Holding Register，而关键的一点是在发送器中发送缓冲器中的数据并不是直接送到输出引脚上的，还必须要先送到发送移位寄存器也就是Transmit Shifter，然后再由Transmit Shifter送出到输出引脚。而接收端这边大概都是一样，发送移位寄存器和接收移位寄存器对数据的发送和接收都是在波特率发生器产生的波特率下来进行控制的。而波特率的产生也需要在时钟源的控制下才行



**UART的操作**UART的操作分为以下几个部分分别是**数据发送**、**数据接收**、**产生中断**、**产生波特率**、**Loopback模式**、**红外模式**以及**自动流控模式**。
**数据发送：**
发送的数据帧格式是可以编程设置的。它包含了**起始位**、**5~8个数据位**、**可选的奇偶校验位**以及**1~2位停止位**。这些都是通过UART的控制寄存器 ULCONn 来设置的。
**数据接收：**同发送一样，接收的数据帧格式也是可以进行编程设置的。此外，还具备了**检测溢出出错**、**奇偶校验出错**、**帧出错**等出错检测，并且每种错误都可以设置相应的错误标志。
**自动流控模式：**
S3C2410的UART0和UART1都可以通过各自的nRTS和nCTS信号来实现自动流控。**在自动流控（AFC）模式下nRTS取决于接收端的状态，而nCTS控制了发送端的操作**。具体地说：只有当nCTS有效时（**表明接收方的FIFO已经准备就绪来接收数据了因为接收端的nRTS是和发送端的nCTS连接的nCTS有效也就表示接收端的nRTS有效也就表示接收方的FIFO已经准备好接收数据啦**），UART才会将FIFO中的数据发送出去。**在UART接收资料之前，只要当接收FIFO有至少2-byte空余的时候，nRTS就会被置为有效**。图5-12是UART 自动流控模式的连接方式 
![](http://www.ec66.com/article/pic/20067270501724033.gif)
图5-12  
**中断/DMA请求产生**S3C2410的每个UART都有7种状态，分别是：**溢出覆盖（Overrun）错误**、**奇偶校验错误**、**帧出错**、**断线错误(暂停态)、接收就绪(接收缓冲区准备好)**、**发送缓冲空闲**、**发送移位器空闲**。它们在UART状态寄存器**UTRSTATn / UERSTATn** 中有相应的标志位。 
波特率发生器
**每个UART控制器都有各自的波特率发生器来产生发送和接收资料所用的序列时钟**，波特率发生器的时钟源可以由CPU内部的系统时钟(PCLK)，也可以从CPU的 UCLK (外部UART设备的时钟)管脚由外部取得时钟信号，并且可以通过 UCONn 选择各自的时钟源。
波特率产生的具体计算方法如下：
当选择CPU内部时钟(PCLK)时：
UBRDIVn=(int)(PCLK/(bps*16))-1,bps为所需要的波特率值，PCLK为CPU内部外设总线（APB）的工作时钟。
当需要得到更精确的波特率时，可以选择由 UCLK 引入的外部时钟来生成。
UBRDIVn=(int)(UCLK/(bps*16))-1
**LoopBack操作模式：**
S3C2410 CPU的UART提供了一种**测试模式**，也就是这里所说的LoopBack模式。在设计系统的具体应用时，为了判断通讯故障是由于外部的数据链路上的问题，还是CPU内驱动程序或CPU本身的问题，这就需要采用LoopBack模式来进行测试。在LoopBack模式中，资料发送端TXD在UART内部就从逻辑上与接收端RXD连在一起，并可以来验证资料的收发是否正常。
UART控制寄存器
下面将针对UART的各个控制寄存器逐一进行讲解，以期对UART的操作和设置能有更进一步的了解。

**ULCONn （UART Line Control Register）**见图5-13
![](http://www.ec66.com/article/pic/20067270503237451.gif)

图5-13  
Word Length ：决定每帧的数据位数
Number of Stop Bit ：停止位数
Parity Mode ：奇偶校验位类型
Infra-Red Mode ：UART/红外模式选择（当以UART模式工作时，需设为“0”） 
**UCONn （UART Control Register）**见图5-14
Receive Mode ：选择接收模式。如果是采用DMA模式的话，还需要指定说使用的DMA信道。
Transmit Mode ：同上。
Send Break Signal ：选择是否在传送一帧数据中途发送Break信号。
Loopback Mode ：选择是否将UART置于Loopback测试模式。
Rx Error Status Interrupt Enable ：选择是否使能当发生接收异常时，是否产生接收错误中断。
Rx Time Out Enable ：是否使能接收超时中断。
Rx Interrupt Type ：选择接收中断类型。
选择0：Pulse（脉冲式/边沿式中断。非FIFO模式时，一旦接收缓冲区中有资料，即产生一个中断；为FIFO模式时，一旦当FIFO中的资料达到一定的触发水平后，即产生一个中断）


选择1：Level（电平模式中断。非FIFO模式时，只要接收缓冲区中有资料，即产生中断；为FIFO模式时，只要FIFO中的资料达到触发水平后，即产生中断）
Tx Interrupt Type ：类同于Rx Interrupt Type
Clock Selection ：选择UART波特率发生器的时钟源。 
![](http://www.ec66.com/article/pic/20067270505261993.gif)
图5-14  
**UFCONn （UART FIFO Conrtol Register）用于对收发缓冲的管理，包括缓冲的触发字节数的设置，FIFO的使能**见图5-15
FIFO Enable ：FIFO使能选择。
Rx FIFO Reset ：**选择当复位接收FIFO时是否自动清除FIFO中的内容**。
Tx FIFO Reset ：**选择当复位发送FIFO时是否自动清除FIFO中的内容**。
Rx FIFO Trigger Level ：选择接收FIFO的触发水平。
Tx FIFO Trigger Level ：选择发送FIFO的触发水平。 
![](http://www.ec66.com/article/pic/2006727051811787.gif)
图5-15  
**UMCONn （UART Modem Control Register）**见图5-16
Request to Send ：如果在AFC模式下，该位将由UART控制器自动设置；否则的话就必须由用户的软件来控制。
Auto Flow Control ：选择是否使能自动流控（AFC）。 
![](http://www.ec66.com/article/pic/20067270512368317.gif)
图5-16  
**UTRSTATn （UART TX/RX Status Register）**见图5-17
Receive buffer data ready :**当接收缓冲寄存器从UART接收端口接收到有效资料时将自动置“1”。反之为“0”则表示缓冲器中没有资料。**Transmit buffer empty ：当发送缓冲寄存器中为空，自动置“1”；反之表明缓冲器中正有资料等待发送。
Transmitter empty ：当发送缓冲器中已经没有有效资料时，自动置“1”；反之表明尚有资料未发送。 
![](http://www.ec66.com/article/pic/20067270513773946.gif)
图5-17  
UERSTATn （UART Error Status Register）见图5-18
Overrun Error ：为“1”，表明发生Overrun错误。
Frame Error ：为“1”。表明发生Frame（帧）错误。 
![](http://www.ec66.com/article/pic/20067270514940468.gif)
图5-18  
UFSTATn　：（UART　FIFO Status Register）见图5-19
Rx FIFO Count :接收FIFO中当前存放的字节数。
　Tx FIFO Count :发送FIFO中当前存放的字节数。
　Rx FIFO Full :为“1“表明接收FIFO已满。
　Tx FIFO Full :为“1“表明发送FIFO已满。 
![](http://www.ec66.com/article/pic/2006727052233073.gif)

图5-19  
UMSTATn　：（UART　FIFO Status Register）见图5-20
Clear to Send ：为“0”表示CTS无效；为“1”表示CTS有效。
Delta CTS ：指示自从上次CPU访问该位后，nCTS的状态有无发生改变。
为“0”则说明不曾改变；反之表明nCTS信号已经变化了。 
![](http://www.ec66.com/article/pic/20067270521514532.gif)

图5-20  
**UTXHn 和 URXHn 分别是UART发送和接收资料寄存器**这两个寄存器存放着发送和接收的资料，当然只有一个字节８位资料。需要注意的是在发生溢出错误的时候，接收的资料必须要被读出来，否则会引发下次溢出错误

UBRDIVn ：（UART Baud Rate Divisor Register）见图5-21
![](http://www.ec66.com/article/pic/20067270522964203.gif)

图5-21 


接着我们通过结合代码来看看具体在程序中怎样来操作UART的流程：

先来看看在C代码中对用到的UART多个寄存器的定义：

#define UART_CTL_BASE 0x50000000  //UART0的寄存器的起始地址也就是ULCON0的地址
#define UART0_CTL_BASE UART_CTL_BASE
#define bUART(x, Nb) __REGl(UART_CTL_BASE + (x)*0x4000 + (Nb))  //因为考虑到UART1与UART0相应的寄存器地址相差0x4000所以这里也考虑到了对UART1和UART2的使用，而上面__REGl的定义：

#define __REG(x) (*(volatile unsigned long *)(x))
#define __REGl(x) __REG(x)
#define bUARTb(x, Nb) __REGb(UART_CTL_BASE + (x)*0x4000 + (Nb)) //#define __REGb(x) (*(volatile unsigned char *)(x))
/* offset */
#define oULCON  0x00
#define oUCON  0x04
#define oUFCON  0x08
#define oUMCON  0x0c
#define oUTRSTAT 0x10
#define oUERSTAT 0x14
#define oUFSTAT  0x18
#define oUMSTAT  0x1c
#define oUTXHL  0x20
#define oUTXHB  0x23
#define oURXHL  0x24
#define oURXHB  0x27
#define oUBRDIV  0x28
/* Registers */
#define ULCON0  bUART(0, oULCON)
#define UCON0  bUART(0, oUCON)
#define UFCON0  bUART(0, oUFCON)
#define UMCON0  bUART(0, oUMCON)
#define UTRSTAT0 bUART(0, oUTRSTAT)
#define UERSTAT0 bUART(0, oUERSTAT)
#define UFSTAT0  bUART(0, oUFSTAT)
#define UMSTAT0  bUART(0, oUMSTAT)
#define UTXH0  bUART(0, oUTXHL)
#define URXH0  bUART(0, oURXHL)
#define UBRDIV0  bUART(0, oUBRDIV)
/* state */
#define UTRSTAT_TX_EMPTY (1 << 2)
#define UTRSTAT_RX_READY (1 << 0)
#define UART_ERR_MASK  0x0f



再来看看真正对UART的操作：



void init_uart(void)
{
 ULCON0 = vULCON0; //#define vULCON0 0x03  表示每帧有8个数据位，1个停止位，不进行奇偶校验，正常模式
 UCON0 = vUCON0;  //#define vUCON0 0x245 表示接收模式和发送模式都是中断或轮询模式，当发生接收异常时，产生接收错误中断，发送中断类型为Level，接收中断类型为Pulse，波特率时钟源为PCLK
 UFCON0 = vUFCON0;
 UMCON0 = vUMCON0;
 UBRDIV0 = 12;
}

void putc(char c)
{
 char i;

 while (!(UTRSTAT0 & UTRSTAT_TX_EMPTY)) {  //不断的查询，直到发送缓冲寄存器和和移位寄存器都不为空，就可以发送数据
  ;
 }

 for (i=0; i<10; i++) {
  ;
 }

**UTXH0 = c; //直接将要发送的字符赋值给发送寄存器发送出去**
}

unsigned char getc(void)
{
 while (!(UTRSTAT0 & UTRSTAT_RX_READY)) {
  ;
 }

**return URXH0;  //直接从接收寄存器中返回接收到的1个字节的数据**
}














