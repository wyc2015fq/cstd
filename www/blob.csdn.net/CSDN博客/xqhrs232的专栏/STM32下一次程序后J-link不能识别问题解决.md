# STM32下一次程序后J-link不能识别问题解决 - xqhrs232的专栏 - CSDN博客
2019年01月09日 11:48:03[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：114
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[https://blog.csdn.net/weifengdq/article/details/78720845](https://blog.csdn.net/weifengdq/article/details/78720845)
相关文章
1、ST-Link不能下载程序的几种解决办法----[https://blog.csdn.net/lingsuifenfei123/article/details/62447343/](https://blog.csdn.net/lingsuifenfei123/article/details/62447343/)
先说下问题: 
1. 我用的是STM32的HAL库, 同事用的是STM32的标准库, 每次新板子在我电脑上下完程序, 同事电脑就下不进去了. 这种情况持续了好久, 还以为是库的问题… 
2. 我自己用STM32CubeMX生成的工程, 某次程序下载到MCU中后, J-link就不识别了, STM32变砖了…
变砖后点击下载会出现: 
并且SW口不识别: 
解决办法的分割线…
查了下, 发现是STM32CubeMX没有设置Debug口为Serial Wire, 只要忘记设置, 就极易出现变砖情况. 还好找到了两种解决办法:
第一种方法: 
配置STM32CubeMX SYS->Debug为Serial Wire: 
重新生成工程, 编译, 仍然设置为SW口下载调试, 然后按下STM32的复位按键, 没有复位的用镊子之类的短接复位电容. 保持按住复位按键, 点击Keil的下载按钮, 此时弹出窗口: 
点YES, 约1s后松开复位按键, 此时应该可以下进去程序了. 如果失败, 可以多尝试几次.
第二种方法: 
如果有串口ISP的话, 这就简单了, 可以用串口ISP下载程序文件, 这样之后再用J-link就没有问题了, 当然, STM32CubeMX不要忘记设置SYS->Debug为Serial Wire.

