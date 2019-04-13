
# 串口UART串行总线协议 - 嵌入式Linux - CSDN博客

2018年09月14日 20:13:30[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：418



## 串口UART
串行端口是异步的（不传输时钟相关数据），两个设备在使用串口通信时，必须先约定一个数据传输速率，并且这两个设备各自的时钟频率必须与这个速率保持相近，某一方的时钟频率相差很大都会导致数据传输混乱。
\#\#\#1 硬件连接
UART可以理解为可以用一线总线就可以完成数据的传输，连接图如下
VCC 用来给设备提供电源的，如果设备有电源，可以省略
TX 顾名思义 就是CPU发送数据给设备-对应接设备的RX
RX 顾名思义 就是CPU接收设备的数据-对应接设备的TX
GND 地线的作用，做嵌入式的应该必须要了解，没有GND怎么有参考信号
![这里写图片描述](https://img-blog.csdn.net/20180914194917189?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
\#\#\#2 软件通信协议
UART作为异步串口通信协议的一种，工作原理是将传输数据的每个字符一位接一位地传输。
串口通信协议还有很多其他的，详细可以看这个[串口通信协议](https://baike.baidu.com/item/%E4%B8%B2%E5%8F%A3%E9%80%9A%E4%BF%A1%E5%8D%8F%E8%AE%AE)
**UART协议定义，如下图**
![这里写图片描述](https://img-blog.csdn.net/20180914155528621?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**其中各位的意义如下：**
起始位：先发出一个逻辑”0”的信号，表示传输字符的开始。
资料位：紧接着起始位之后。资料位的个数可以是4、5、6、7、8等，构成一个字符。通常采用ASCII码。从最低位开始传送，靠时钟定位。
奇偶校验位：资料位加上这一位后，使得“1”的位数应为偶数(偶校验)或奇数(奇校验)，以此来校验资料传送的正确性。
停止位：它是一个字符数据的结束标志。可以是1位、1.5位、2位的高电平。 由于数据是在传输线上定时的，并且每一个设备有其自己的时钟，很可能在通信中两台设备间出现了小小的不同步。因此停止位不仅仅是表示传输的结束，并且提供计算机校正时钟同步的机会。适用于停止位的位数越多，不同时钟同步的容忍程度越大，但是数据传输率同时也越慢。
空闲位：处于逻辑“1”状态，表示当前线路上没有资料传送。
波特率：是衡量资料传送速率的指标。表示每秒钟传送的符号数（symbol）。一个符号代表的信息量（比特数）与符号的阶数有关。例如资料传送速率为120字符/秒，传输使用256阶符号，每个符号代表8bit，则波特率就是120baud，比特率是120*8=960bit/s。这两者的概念很容易搞错。
波特率计算示例
如图：
![这里写图片描述](https://img-blog.csdn.net/20180914193240806?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
第一个字节的10位(1位起始位，8位数据位和1位停止位)共占约1.05ms，这样可计算出其波特率约为：10bit / 1.05ms X 1000 ≈ 9600 bit/s
一般UART设置窗口如下图
![这里写图片描述](https://img-blog.csdn.net/20180914122232655?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
\#\#3 示波器逻辑分析仪测量UART波形
如果你是一个做嵌入式软件的，但是你不会量波形，会很丢脸，至少我可以知道示波器有一个AutoSet的按钮，当然，你也可以用逻辑分析仪，这个对软件工程师来说更加方便。
下面列举协议分析的图片
连续的数据波形：0xFF，0x00,…，0x80的波形。可以看出，如果要发送0b0000 0001, 按时间顺序则是，低(起始位)，高(1)，低(0)，低(0)，低(0)，低(0)，低(0)，低(0)，低(0)。但是后面的7个连续的低电平连接在一起，就形成了一个较长低电平。
![这里写图片描述](https://img-blog.csdn.net/20180914193130996?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
更具体的可以查看这个网站
[http://www.51hei.com/bbs/dpj-39824-1.html](http://www.51hei.com/bbs/dpj-39824-1.html)
喜欢可以关注微信公众号：嵌入式Linux

