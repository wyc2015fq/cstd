# SmartRF04EB制作介绍 - xqhrs232的专栏 - CSDN博客
2015年05月07日 14:33:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3511
原文地址::[http://www.cnblogs.com/yulongchen/archive/2012/10/27/2742133.html](http://www.cnblogs.com/yulongchen/archive/2012/10/27/2742133.html)
相关文章
1、smartrf04EB原理图，zigbee调试工具，CCdebugger----[http://download.csdn.net/detail/zhaigf0441/4757234](http://download.csdn.net/detail/zhaigf0441/4757234)
2、smartrf05EB原理图----[http://download.csdn.net/detail/qgaoyuan/2226095](http://download.csdn.net/detail/qgaoyuan/2226095)
3、SK-SmartRF05EB评估板原理图----[http://wenku.baidu.com/link?url=H7-nmtXhxCYAAhGcpfYI1muVmIPEPU7gC2xnfIhpCqcRqjyMsGuy0oVa9Bt6HFZxAJ35syf9SrAOjl62cgXQSbc7ErFKNHbFXsBY8LMcmOu](http://wenku.baidu.com/link?url=H7-nmtXhxCYAAhGcpfYI1muVmIPEPU7gC2xnfIhpCqcRqjyMsGuy0oVa9Bt6HFZxAJ35syf9SrAOjl62cgXQSbc7ErFKNHbFXsBY8LMcmOu)
4、CC Debugger原理图----[http://wenku.baidu.com/view/fa5f490852ea551810a68742.html](http://wenku.baidu.com/view/fa5f490852ea551810a68742.html)
 前前后后花费大约两个星期.从一点点都不懂，到制作出来实物，乃至通过最后的调试。期间经历了颇多曲折，感悟良多，所以写下此文，愿与各位分享。希望这篇文章能够为大家提供指导，避免走弯路。
## （一）原理图
   原理图来源于网络，当然你也可以去查找。此原理图是根据Ti官方电路修改而来。支持内核为51的TI Zigbee芯片:CC111x/CC243x/CC253x/CC251x。
![](http://pic002.cnblogs.com/images/2012/461108/2012102709402655.png)
我特意将C8051F320芯片主板与底板分开。一来方便布局，二来可以减少制作失败的损失。
## （二）PCB图
![](http://pic002.cnblogs.com/images/2012/461108/2012102709405043.png)
线宽比较粗，最细的也有0.5mm，方便手工制作PCB。
## （三）制作实物
![](http://pic002.cnblogs.com/images/2012/461108/2012102709410798.jpg)
看着还行吧！！！
## （四）软件配置
   由于采用的是C8051F320芯片，所以需要手头有一个该单片机的下载器。下载方式采用的是C2接口。悲剧的是，我在这上面折腾了一天。后来，多亏在阿莫论坛的一位网友的发言中才注意到问题所在：都是电容惹得祸（看来我以后在这方面处理得小心点）。
1.下载bootloader
![](http://pic002.cnblogs.com/images/2012/461108/2012102709413096.png)
给C8051F320下载srf04dd_bootloader.hex这个文件。
2.安装驱动和下载程序
   使用USB接口连接上PC。如果前面步骤都正确的话会提示安装驱动。你可以手动安装驱动，驱动在IAR的安装目录下面，
![](http://pic002.cnblogs.com/images/2012/461108/2012102709414758.png)
选择文件srf04eb.inf。
![](http://pic002.cnblogs.com/images/2012/461108/2012102709422143.png)
然后打开SmartRF Flash Programmer软件，向芯片烧入fw0400.hex这个十六进制文件。
3.成功烧入了这个文件，那恭喜你，zigbee仿真器已经制作完成了。赶紧试试吧！！！
//========================================================================================
//备注::
TI的一些工具软件装了就有了调试器的固件程序！！！
1>C:\Program Files\Texas Instruments\SmartRF Studio 7\firmware
2>C:\Program Files\Texas Instruments\SmartRF Tools\Firmware
   a.CC Debugger; b.SmartRF04EB; c.SmartRF05EB ;d.TrxEB
