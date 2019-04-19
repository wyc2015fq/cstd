# [经验分享]如何将LPCXpresso的仿真器烧录成CMSIS-DAP或JLINK - xqhrs232的专栏 - CSDN博客
2017年03月26日 00:10:21[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1371
原文地址::[http://www.yiboard.com/thread-595-1-1.html](http://www.yiboard.com/thread-595-1-1.html)
相关文章
1、OM13054: LPC-Link2----[http://www.nxp.com/cn/products/reference-designs/lpc-link2:OM13054](http://www.nxp.com/cn/products/reference-designs/lpc-link2:OM13054)
2、[https://www.lpcware.com/content/forum/lpcxpresso-segger-j-link](https://www.lpcware.com/content/forum/lpcxpresso-segger-j-link)
3、NXP LPCXpresso Pro Edition----[https://www.segger.com/nxp-lpcxpresso.html](https://www.segger.com/nxp-lpcxpresso.html)

1.  前段时间入手了一块LPCXpresso11U37H EVB开发板，最近总算有时间来玩一玩，学习怎样烧写程序。原以为，应该会和STM32Fx探索板的烧写步骤，安装好仿真器的驱动就可以进入DEBUG模式来进行调试了。结果发现并没有那么容易呢。下面是测试的流程：
2.  在拿到开发板后，首先是要安装开发板的驱动程序LPCXpresso Link2 USB driver package，该程序可以在lpcware来下载。下载地址 -> [https://www.lpcware.com/content/
 ... -usb-driver-package](https://www.lpcware.com/content/nxpfile/lpcxpresso-link2-usb-driver-package)。
3.  安装完驱动文件后，将开发板连接到计算机，提示安装驱动，按照要求完成驱动程序的安装。
4.通常情况下，这时就可以使用IAR或者KEIL进行开发板的调试了。但有些时候这些IDE不能识别LPC
 Link。这时就可以将其烧写成JLINK或者CMSIS-DAP。NXP的开发板支持的调试方式比较特殊，同一个仿真器可以烧录不同的固件。这里使用的烧录工具是官方的LPCScypt。
5.首先到NXP的官网下载LPCScrypt。下载完成后，安装该工具。下载地址：[LPCScrypt
 Download Link](http://www.nxp.com/products/software-and-tools/software-development-tools/software-tools/lpc-microcontroller-utilities/lpcscrypt)
6.安装完成后，必须将LPC11U37H EVB开发板的JP3(Link
 DFU)短接后，再连接到计算机。JP3的位置如下图所示：
7.  开发板上有两个USB接口，一个是用于USB设备，另一个是Debug接口。使用USB将开发板的Debug接口连接到计算机。
8.打开LPCScrypt，选择要使用的是JLINK或者CMSIS-DAP，这里我选择的是Program LPC-Link2 with CMSIS-DAP。
9.  该工具非常简单，自动烧录固件到开发板上。
10. 移除JP3的短接帽。接着我们就可以使用IDE来进行验证了。打开Keil，选择Option选项。
11. 点击Debug选项卡，将Debug仿真器选择CMSIS-DAP
 Debugger。
12.  回到主窗口。单击工具栏的调试按钮，就可以进入到Debug模式了。
