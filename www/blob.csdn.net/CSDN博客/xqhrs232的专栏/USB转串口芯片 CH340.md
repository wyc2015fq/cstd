# USB转串口芯片:CH340 - xqhrs232的专栏 - CSDN博客
2017年06月26日 10:01:52[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：8999
原文地址::[http://www.wch.cn/product/CH340.html](http://www.wch.cn/product/CH340.html)
最新推荐：**免晶振（CH340E/C/B）**和**超小体积（CH340E）**
概 述
          CH340是一个USB 总线的转接芯片，实现USB转串口或者USB转打印口。
在串口方式下，CH340提供常用的MODEM联络信号，用于为计算机扩展异步串口，或者将普通的串口设备直接升级到USB总线。有关USB转打印口的说明请参考手册（二）CH340DS2。
![](http://www.wch.cn/img/ch340_1.gif)
特 点
>  全速USB 设备接口，兼容USB V2.0。
> 仿真标准串口，用于升级原串口外围设备，或者通过USB增加额外串口。
>  计算机端Windows 操作系统下的串口应用程序完全兼容，无需修改。
> 硬件全双工串口，内置收发缓冲区，支持通讯波特率50bps～2Mbps。
> 支持常用的MODEM 联络信号RTS、DTR、DCD、RI、DSR、CTS。
> 通过外加电平转换器件，提供RS232、RS485、RS422 等接口。
>  CH340R 芯片支持IrDA 规范SIR红外线通讯，支持波特率2400bps到115200bps。
>  软件兼容CH341，可以直接使用CH341的驱动程序。
>  支持5V 电源电压和3.3V电源电压甚至3V 电源电压。
>  CH340C、CH340E 和CH340B 内置时钟，**无需外部晶振**，CH340B
 还内置EEPROM 用于配置序列号等。
>  提供SOP-16 和SSOP-20以及**MSOP-10**无铅封装，兼容RoHS。
封 装
![](http://www.wch.cn/img/ch340_2.gif)
型号区别：
**CH340C和CH340E 以及CH340B内置时钟，无需外部晶振****。**
**CH340B 还内置EEPROM 用于配置序列号，以及部分功能可定制等。**
CH340R 内置简易RS232，无需外部RS232转换芯片，暂无现货。
开发资料：
技术手册：[CH340DS1.PDF](http://www.wch.cn/download/CH340DS1_PDF.html)；
驱动程序：
1、[CH341SER.EXE](http://www.wch.cn/download/CH341SER_EXE.html)（Windows系统，包含WIN10
 -32/64位）；
2、 [CH341SER_ANDROID.ZIP](http://www.wch.cn/download/CH341SER_ANDROID_ZIP.html)（安卓免驱应用库API）；
3、 [CH341SER_LINUX.ZIP](http://www.wch.cn/download/CH341SER_LINUX_ZIP.html)（Liunx系统）
4、 [CH341SER_MAC.ZIP](http://www.wch.cn/download/CH341SER_MAC_ZIP.html)（苹果MAC
 OS）
原理图/PCB：[CH340PCB.ZIP](http://www.wch.cn/download/CH340PCB_ZIP.html)
[详细资料下载](http://www.wch.cn/download/CH340DS1_PDF.html)[样品索取](http://www.wch.cn/sample.html)[联系我们](http://www.wch.cn/contact_us.html)
相关资料：[CH340DS2.PDF](http://www.wch.cn/download/CH340DS2_PDF.html)CH340技术手册2，USB总线转接芯片，用于USB转串口，打印口，I...[CH340PCB.ZIP](http://www.wch.cn/download/CH340PCB_ZIP.html)CH340的USB转串口、USB转打印口、USB转IrDA的原理图和P...[USB_SER.PDF](http://www.wch.cn/download/USB_SER_PDF.html)USB转串口Serial模块的说明，用于USB转RS232串口，USB...[CH341SER.EXE](http://www.wch.cn/download/CH341SER_EXE.html)CH340/CH341的USB转串口WINDOWS驱动程序的安装包，支...[CH340IR.EXE](http://www.wch.cn/download/CH340IR_EXE.html)CH340的USB转IrDA红外WINDOWS
 2000/XP驱动程序...[CH341DS1.PDF](http://www.wch.cn/download/CH341DS1_PDF.html)CH341技术手册，USB总线转接芯片，接口丰富，平台驱动齐全，用于U...[CH341EVT.ZIP](http://www.wch.cn/download/CH341EVT_ZIP.html)CH341评估板的说明及参考应用例程，用于USB转异步串口、转打印口或...[CH352DS1.PDF](http://www.wch.cn/download/CH352DS1_PDF.html)CH352技术手册，
 PCI总线双串口和打印口芯片，用于PCI转双串口...[CH432DS1.PDF](http://www.wch.cn/download/CH432DS1_PDF.html)CH432技术手册，2串口扩展芯片，兼容16C550，8位并口或者SP...[USBMonitor.RAR](http://www.wch.cn/download/USBMonitor_RAR.html)USB总线分析仪(USB
 Monitor)的说明及配套软件安装程序，分...[CH438DS1.PDF](http://www.wch.cn/download/CH438DS1_PDF.html)CH438技术手册，8串口扩展芯片，各串口收发独立128字节FIFO，...[CH341SER_ANDROID.ZIP](http://www.wch.cn/download/CH341SER_ANDROID_ZIP.html)CH340/CH341的USB转串口安卓免驱应用库，用于Android...
