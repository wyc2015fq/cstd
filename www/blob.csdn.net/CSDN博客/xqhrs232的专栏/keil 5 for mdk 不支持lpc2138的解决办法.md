# keil 5 for mdk 不支持lpc2138的解决办法 - xqhrs232的专栏 - CSDN博客
2017年05月20日 22:41:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1452
原文地址::[http://blog.csdn.net/moluoyu/article/details/47013347](http://blog.csdn.net/moluoyu/article/details/47013347)
相关文章
1、
## 使用keil5工具开发一个基于LPC17XX系列芯片的工程文件
 ----[http://blog.sina.com.cn/s/blog_762cf5f80102why2.html](http://blog.sina.com.cn/s/blog_762cf5f80102why2.html)
1,由于keil 5 采用了 pack install 管理，在4中的很多Devices并没有直接安装，需要自己下载，其中在使用过程中，发现在芯片库中没有发现lpc2000系列，在包管里中也没发现，在官网上逛了一下，发现了这个： 
![这里写图片描述](https://img-blog.csdn.net/20150723003404085)
于是乎安装说明，打开了如下网站，下载了相应安装包，成功解决了问题 
![这里写图片描述](https://img-blog.csdn.net/20150723003554695)
[http://www2.keil.com/mdk5/legacy](http://www2.keil.com/mdk5/legacy)
此时再次打开keil5，你会发现多了个器件包，如图所示 
![这里写图片描述](https://img-blog.csdn.net/20151121223546879)
选择NXP就能看见lpc系列
//==================================================================================================================
备注::
1>针对Keil MDK UV5要去支持LPC17XX系列芯片需要去下载专门的设备安装包----Keil.LPC1700_DFP.2.3.0.pack
2>针对Keil MDK UV5要去支持STM32F1系列芯片需要去下载专门的设备安装包----Keil.STM32F1xx_DFP.2.2.0.pack
3>Keil MDK UV5下专门的设备安装包下载地址----
# MDK5 Software Packs----[http://www.keil.com/dd2/pack/](http://www.keil.com/dd2/pack/)

