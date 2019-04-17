# IAR FOR STM8 学习笔记 IAR工程的建立 - DoubleLi - 博客园






STM8是ST意法半导体针对工业应用和消费电子开发而推出的8位单片机。

每种MCU都有自身的优点与缺点，与其它8-bit MCU相比，STM8 8-bit MCU最大的特点是：

· 内核：

o 最高fcpu频率，可达24MHz，当fcpu≤16MHz时0等待的存储器访问

o 高级STM8内核，基于哈佛结构并带有3级流水线

o 扩展指令集

o 最高20 MIPS @ 24 MHz

· 时钟、复位和电源管理：

o 2.95V到5.5V工作电压

o 灵活的时钟控制，4个主时钟源

o 带有时钟监控的时钟安全保障系统

· 电源管理：

o 低功耗模式（等待、活跃停机、停机）

o 外设的时钟可单独关闭

o 永远打开的低功耗上电和掉电复位

· 通信接口：

o 高速1Mbit/s CAN 2.0B接口

o 带有同步时钟输出的UART — LIN主模式

o UART兼容LIN2.1协议，主/从模式和自动重新同步

o SPI接口最高到10Mbit/s

o I2C接口最高到400Kbit/s

· I/O端口:

o 带有高吸收电流输出的GPIO脚

o 非常强健的GPIO设计，对倒灌电流有非常强的承受能力

· 开发支持:

o 单线接口模块（SWIM）和调试模块（DM），可以方便地进行在线编程和非侵入式调试

· 性价比:

o 相对于现在众多的8位单片机，STM8的价格与性能比例是比较高。



这么多优点，虽然好多都不懂还是慢慢学吧。。首先建立开发平台：

    用的是IAR FOR STM8，感觉还不错吧，编译器的效率很高。

IAR下载也很简单，直接上官网上有完全版和代码限制版的。。



从建立一个新的工程开始：



1.先创建一个workplace.选择File>New>Workplace

2.(创建一个新的工程，选择Project>Creat New Project,如果用的是IAR FOR STM8  就默认无需更改。保存并输入Project名字，在workplace窗口中显示如下



![](http://upload.semidata.info/www.eefocus.com/blog/media/201111/187859.png)



默认配置是Debug

 3.在添加文件到工程之前，先保存Workplace。并输入保存的名字

Workplace的扩展名是eww。

4.添加文件到工程，先File>New>File,也可以直接使用快捷键![](file:///C:/Users/ERIC-ki/AppData/Local/Temp/ksohtml/wps_clip_image-23807.png)新建一个文件。之后保存。





![](http://upload.semidata.info/www.eefocus.com/blog/media/201111/187862.png)

保存的时候可以新建一个C文件夹，以便区分工程中的其他文件，方便查找和再调用。

再将文件添加到工程中就可以开始了。

右击工程名>ADD>ADD FILES,  就可以添加文件。

首先添加头文件，配置寄存器方便写程序的时候调用。

注意：记得设置路径，这样IAR才能找到你的头文件。

打开工程Option设置，在C/＋＋compiler ――――PreProcessor中加入你自己的路径
如
$PROJ_DIR$\H // 加入工程文件所在目录下的\H子目录![](http://upload.semidata.info/www.eefocus.com/blog/media/201111/187872.png)







测试程序：

#include <iostm8s207sb.h>



void main()

{

  while(1);

 }





编译连接下：![](http://upload.semidata.info/www.eefocus.com/blog/media/201111/187875.png)



![](http://upload.semidata.info/www.eefocus.com/blog/media/201111/187878.png)



用的是ST-LINK，下载前先设置下

![](http://upload.semidata.info/www.eefocus.com/blog/media/201111/187866.png)

下载





![](http://upload.semidata.info/www.eefocus.com/blog/media/201111/187877.png)

没有错误，可以开始了。。。









