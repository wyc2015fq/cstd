# 如何利用IDA反汇编逆向bin文件 - xqhrs232的专栏 - CSDN博客
2018年04月24日 23:34:04[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2759
原文地址::[https://blog.csdn.net/sinat_23338865/article/details/72930132](https://blog.csdn.net/sinat_23338865/article/details/72930132)
相关文章
1、ida逆 stm32 bootloader----[https://blog.csdn.net/dp__mcu/article/details/79686649](https://blog.csdn.net/dp__mcu/article/details/79686649)
2、用IDA反汇编个STM32小程序----[http://www.crystalradio.cn/thread-637028-1-1.html](http://www.crystalradio.cn/thread-637028-1-1.html)

由于最近学习单片机逆向工程，所以接触一些反汇编工作。接下来简单描述一下如何用IDA反汇编个一个STM32小程序
1、先利用开发板目写一个最简单的LED小程序
2、将工程文件夹的hex文件生成bin文件
（1）可以利用JLINK工具
（2）可以利用keil提供的软件生成
3、打开IDA，NEW新的工程，导入bin文件
   (1)   选择芯片类型为ARM，还有个高级选项选择Cortex-M。
  （2）根据KEIL工程Opitions 的设置，设置IDA工程ROM地址，RAM地址。
4、点击完成，就可以看到反汇编的汇编文件。
5、关于反汇编文件，根据对arm指令以及stm32的理解，就可以看到简单的汇编语句对应到的led工程C语言的语句了，深入的可以理解程序结构。

