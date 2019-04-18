# cortex A8处理器启动分析一引导代码BL0 - 一世豁然的专栏 - CSDN博客





2015年04月23日 13:46:15[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1376








本博客转载于：[http://blog.csdn.net/u010216127/article/details/9320965](http://blog.csdn.net/u010216127/article/details/9320965)







cortex A8是基于ARMv7架构的处理器，主频可以达到1GHz。基于cortexA8的处理器有三星的S5PC100、S5PV210，TI的OMAP3530，全志的A10等。我有个想法就是把u-boot的2个阶段的代码独立。第一阶段代码称为硬件相关BL1，第二阶段代码称为硬件无关BL2。手上刚好有块s5pc100的板子，就介绍s5pc100的启动过程并验证。我把启动过程分为3个阶段BL0、BL1、BL2。BL0是固化在内部ROM上电就执行的一小段程序，我把自己写的boot代码，用于引导u-boot称为BL2，把u-boot的第二阶段代码用于引导内核称为BL2。这是我的理解，在接下来我将依次分析BL0、BL1、BL2，并实现BL1代码引导u-boot,BL2移到内核。

一、什么是BL0

BL0(1st boot loader)是固化在内部ROM上电就执行的一小段程序。BL0从系统指定的外部存储器件(NAND、SD/MMC、OneNAND与USB)中加载第二个引导程序BL1到内部RAM，同时对BL1进行安全检验。启动过程如下：

![](https://img-blog.csdn.net/20130713214816093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIxNjEyNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


二、BL0做了些什么

s5pc100芯片手册见2.2FUNCTIONAL SEQUENCE,翻译成中文如下

1.初始化PLL和时钟，将其设定为固定值；

2.初始化栈和堆区域；

3.初始化指令Cache 控制器；

4.从外部起动设备中加载BL1；

5.如果起动安全机制开启，则检查BL1数据完整性；

6.如果校验通过，则跳转到0x34010地址处运行；

7.如果校验失败则停止。


三、S5PC100支持的启动方式

根据OM[4：0]、B0[5::0]引脚的配置，可以支持不同启动方式

OM[4]  0：正常模式1：测试模式

OM[3]  0：首先从内部ROM启动1：从Nor Flash启动

OM[2：1]00：第二启动从NAND Flash 01：第二启动从One NAND10：第二启动从MMC/SD卡 11：第二启动从NOR Flash

OM[0] APLL/MPLL输入选择

B0[5] 0：第二启动由OM[2：1]确定1：第二启动从USB(当OM[2：1]为00时从NAND Flash启动)

B0[4] 0：ECC加密1:：ECC不加密

B0[3] 0

B0[2] 地址周期选择

B0[1:0]块大小选择 00：Small Block(Page 512)10：Large Block(Page 2048) 11：Dlarge Block(Page 4096)

注如果从SD/MMC起动，BL0将SD/MMC最后的9KB数据加载到0x34000处运行；如果从OneNAND 和NAND起动，BL0将开始的16KB数据加载到0x34000处运行。



参考文章：http://www.tuxi.com.cn/6-132-1327758.html 





