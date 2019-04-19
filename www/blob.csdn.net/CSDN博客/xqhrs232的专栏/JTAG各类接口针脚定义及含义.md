# JTAG各类接口针脚定义及含义 - xqhrs232的专栏 - CSDN博客
2014年07月27日 10:24:36[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：642
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[http://www.cnblogs.com/jeakon/archive/2012/10/07/2813683.html](http://www.cnblogs.com/jeakon/archive/2012/10/07/2813683.html)
相关文章
1、[JTAG接口介绍](http://blog.chinaunix.net/uid-467164-id-2399013.html) ----[http://blog.chinaunix.net/uid-467164-id-2399013.html](http://blog.chinaunix.net/uid-467164-id-2399013.html)
2、JTAG接口_百度百科----[http://baike.baidu.com/view/1231341.htm?fr=aladdin](http://baike.baidu.com/view/1231341.htm?fr=aladdin)
 JTAG有10pin的、14pin的和20pin的，尽管引脚数和引脚的排列顺序不同，但是其中有一些引脚是一样的，各个引脚的定义如下。
一、引脚定义
### Test Clock Input (TCK) -----强制要求1
TCK在IEEE1149.1标准里是强制要求的。TCK为TAP的操作提供了一个独立的、基本的时钟信号，TAP的所有操作都是通过这个时钟信号来驱动的。
### Test Mode Selection Input (TMS) -----强制要求2
TMS信号在TCK的上升沿有效。TMS在IEEE1149.1标准里是强制要求的。TMS信号用来控制TAP状态机的转换。通过TMS信号，可以控制TAP在不同的状态间相互转换。
### Test Data Input (TDI) -----强制要求3
TDI在IEEE1149.1标准里是强制要求的。TDI是数据输入的接口。所有要输入到特定寄存器的数据都是通过TDI接口一位一位串行输入的（由TCK驱动）。
### Test Data Output (TDO) -----强制要求4
TDO在IEEE1149.1标准里是强制要求的。TDO是数据输出的接口。所有要从特定的寄存器中输出的数据都是通过TDO接口一位一位串行输出的（由TCK驱动）。
### Test Reset Input (TRST) ----可选项1
这个信号接口在IEEE 1149.1标准里是可选的，并不是强制要求的。TRST可以用来对TAPController进行复位（初始化）。因为通过TMS也可以对TAP Controll进行复位（初始化）。所以有四线JTAG与五线JTAG之分。
###  (VTREF) -----强制要求5
接口信号电平参考电压一般直接连接Vsupply。这个可以用来确定ARM的JTAG接口使用的逻辑电平（比如3.3V还是5.0V?）
### Return Test Clock ( RTCK) ----可选项2
可选项，由目标端反馈给仿真器的时钟信号,用来同步TCK信号的产生,不使用时直接接地。
### System Reset ( nSRST)----可选项3
可选项,与目标板上的系统复位信号相连,可以直接对目标系统复位。同时可以检测目标系统的复位情况，为了防止误触发应在目标端加上适当的上拉电阻。
### USER IN
用户自定义输入。可以接到一个IO上，用来接受上位机的控制。
### USER OUT
用户自定义输出。可以接到一个IO上，用来向上位机的反馈一个状态
由于JTAG经常使用排线连接，为了增强抗干扰能力，在每条信号线间加上地线就出现了这种20针的接口。但事实上，RTCK、USER IN、USER OUT一般都不使用，于是还有一种14针的接口。对于实际开发应用来说，由于实验室电源稳定，电磁环境较好，干扰不大。
二、20、14、10pin JTAG的引脚名称与序号对应关系
值得注意的是，不同的IC公司会自己定义自家产品专属的Jtag头，来下载或调试程序。嵌入式系统中常用的20、14、10pin JTAG的信号排列如下：![](https://img-my.csdn.net/uploads/201210/07/1349621802_2672.png)
![](https://img-my.csdn.net/uploads/201210/07/1349621552_5915.png)![](https://img-my.csdn.net/uploads/201210/07/1349621560_8016.png)
需要说明的是，上述Jtag头的管脚名称是对IC而言的。例如TDI脚，表示该脚应该与IC上的TDI脚相连，而不是表示数据从该脚进入download cable。
实际上10针的只需要接4根线，4号是自连回路，不需要接，1，2接的都是1管脚，而8，10接的是GND，也可以不接。
 附转接板电路：
![](https://img-my.csdn.net/uploads/201210/07/1349622089_2787.png)
