
# MTK 8127平台使用busybox - 嵌入式Linux - CSDN博客

2015年10月08日 15:51:03[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1375


一、什么是BusyBox ？

BusyBox 是标准 Linux 工具的一个单个可执行实现。BusyBox 包含了一些简单的工具，例如 cat 和 echo，还包含了一些更大、更复杂的工具，例如 grep、find、mount 以及 telnet。有些人将 BusyBox 称为 Linux 工具里的瑞士军刀.简单的说BusyBox就好像是个大工具箱，它集成压缩了 Linux 的许多工具和命令。这些工具在标准Linux上都有，但Android系统却去掉了其中的大多数工具。
二、在Android上安装BusyBox

准备：

1. 先要把[手机](http://mobile.zol.com.cn/)给Root了，具体教程这里就不提供了，网上有很多。X战神已经是内核root，所以可以略过此步骤。

2. 下载BusyBox的binary，打开这个地址[http://www.busybox.net/downloads/binaries](http://www.busybox.net/downloads/binaries)，选择最新版本，然后下载对应你的设备架构的版本
联发科mt6592
核心数:物理八核，八线程。
核心架构: 32位**ARMv7指令集**Cortex - A7架构
工艺制程:台积电28纳米hpm工艺
带宽:最大支持32bit LPDDR2-1066[内存](http://memory.zol.com.cn/)（533MHz）
 带宽合计（位宽32bit*频率533mhz*4/32）=4.2Gbps
所以我下载了busybox-armv7l，下面将以这个文件名为示例。
![](http://i2.bbs.fd.zol-img.com.cn/t_s800x5000/g4/M05/07/0E/Cg-4WVPFVdyIWwhnAABN-SAfVzoAAPnHgGRS_4AAE4R308.png)
我下载到了D：  并且改名为：busybox
下载下来后，将push到sdcard中：（以下红色字体的是需要输入的命令）

![](http://i4.bbs.fd.zol-img.com.cn/t_s800x5000/g4/M07/07/0E/Cg-4WlPFamKICW5GAABoIJwzHFkAAPnLABRjxgAAGg4554.png)

--install 后面的./表示安装到当前目录，之所以要安装到当前目录，是因为默认安装到/system/bin目录下或/system/xbin目录下是不能成功的，因为没有足够的权限.

