# PC下串口IO空间及其寄存器详解 - xqhrs232的专栏 - CSDN博客
2013年04月28日 11:00:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：887
个人分类：[串口技术/无线串口技术](https://blog.csdn.net/xqhrs232/article/category/1347902)
原文地址::[http://blog.csdn.net/huangkangying/article/details/8070945](http://blog.csdn.net/huangkangying/article/details/8070945)
**串口（Serial UART),** 全称是通用异步收发口**universal asynchronous receiver/transmitter.**
主要用于计算机之间的串行通信任务。UART主要完成时序和奇偶校验等功能。而在通信时
需要附加的设备是TTL电平与RS232电平的转换设备。
对于TTL电平，规定**+5V**为逻辑“1”， **0V**为逻辑“0”
对于RS232电平，规定**+3V到+12V**为逻辑“1”， **-12V到-3V**为逻辑"0"
当要使用UART进行通信之前，通常要对UART的相关寄存器进行正确的配置，才能进行正常的通信。 
可能大家都有过在单片机或者ARM,DSP上使用串口的经历。串口配置起来的确简单，好用。但我这里要写的主要是针对PC机上的串口寄存器配置。如果是要在ARM或者是单片机上使用串口，可能下面要讲到的寄存器会不适用。不过大致的原理却是一致的。 
我们知道，串口都有相应的接收寄存器和发送寄存器，当有数据到来的时候，我们可以从接收寄存器中去读取，当要发送数据的时候，我们会向发送寄存器写数据，然后剩下来的一切就交给硬件去做吧。当然，在我们能进行正常的读写之前，有一点很重要，就是要配置好波特率，也就是说发送方和接收方的波特率必须要保持一致，不然就算是你能收到数据，到时候也只是一堆乱码而已。 
对于编程而言，串口的工作模式有两种：
第一种是**查询模式**，也就是CPU不停地检测接收寄存器，看有没有数据到达
第二种是**中断模式**，CPU可以去做别的事，有数据到的时候会有相应的中断，然后CPU再跳转过去处理数据。
这两种模式可以根据实际的情况来使用，在此不细说。 
还是回到讲PC的串口的寄存器的主题上来：
在PC上，串口的寄存器是要占用相应的IO空间的，也就是说我们可以通过向相应的IO口写值来改变寄存器的配置。如表1-1所示：
COM4
0x2E8 - 0x2EF
3
上面这张表是默认的串口默认使用的I/O地址和中断向量号，在一些特定的系统上，这些值有可能会变动。 
接下来要讲的是串口所使用的寄存器了。
对于每一个UART来说，有8个字节的I/O用于其寄存器的访问。如表1-2所示：表中的DLAB是LCR寄存器的Bit7，位于base+3处。
base + 7
**SCR** scratch
下面将详细地介绍每一个寄存器。
### **RBR : 接收缓冲寄存器Receiver buffer register (RO)**
RBR在FIFO未开启的时候，只包含当前接收到的一个Byte。当FIFO被启用的时候，每一次的读操作都会返回FIFO头部的一个数据，直到FIFO空为止。当FIFO为空时LSR的Bit 0会由1变为0。
### THR : 发送保持寄存器Transmitter holding register (WO)
THR主要用于缓冲输出数据，当FIFO未被启用的时候，只能缓冲一个Byte的数据，当FIFO启用时，可以缓冲多个Byte的数据，当LSR的Bit 1为“1”时表示缓冲器为空，此时可以写入数据，但是没有一个寄存器可以表示当前FIFO中的有效数据个数。
### IER : 中断使能寄存器Interrupt enable register (R/W)
IER是中断使能寄存器，在使用串口通信时使用中断方式无疑是高效的。使用中断可以不必定时地去查询寄存器的状态，UART会自动地向处理器发起中断信号。除非UART被配置成中断模式，否则中断是不会自动产生的。
**7**
### IIR : 中断识别寄存器Interrupt identification register (RO)
当UART的状态改变的时候会产生中断，但具体是哪种状态的改变产生的中断呢？如果软件在中断产生时想要识别当前的状态改变信息，如何办呢？这个时候，IIR就派上用场了。
1
1
FIFO enabled
–
### FCR : FIFO控制寄存器FIFO control register (WO)
FCR主要用于控制UART的FIFO动作。
1
1
14 bytes
### LCR : Line control register (R/W)
LCR，线控寄存器，主要用于配置stop bit的位数，奇偶校验以及DLL和DLM寄存器的可访问性。DLL和DLM这些寄存器与RBR, THR和IER寄存器一样被映射同样的IO口上。它们只有在串口初始化的时候才会被配置。
常用的串口配置有：
l  8个数据位，1个停止位，无奇偶校验
l  7个数据位，1个停止位，偶校验
1
### MCR : Modem control register(R/W)
MCR主要用于控制Modem。这个不多说。
**7**
Reserved
### LSR : Line status register (RO)
LSR这个寄存器很有用，它作为状态寄存器反映了当前通信的状态。
**7**
Errornous data in FIFO
其中bit 0用于接收数据状态，bit 5用于发送数据状态。
### MSR : Modem status register (RO)
MSR反映了Modem的状态信息，这个也不多说。
**7**
Carrier detect
### SCR : Scratch register (R/W)
这个寄存器也用得很少，在此不多讲。
### DLL and DLM : Divisor latchregisters (R/W)
这两个寄存器合起来用于配置分频器，以得到合适的波特率。一般而言，在pc上用于串口通信的晶体的频率为：1.8432MHz。用这个频率除以16后用作串口的最大频率。所以一般串的最大传输速率为：1.8432 * 10^6 / 16= 115200bps。但有的modem使用24MHz的晶体作为初始频率，因此最大的传输速率可以达到1.5Mbps。
**115,200**
1
0x01
0x00
