# 关于ST-Link的internal command error问题的解决方法 - xqhrs232的专栏 - CSDN博客
2018年12月30日 23:06:02[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：70
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[https://blog.csdn.net/u014563989/article/details/53925836/](https://blog.csdn.net/u014563989/article/details/53925836/)
相关文章
1、ST-Link的internal command error问题的原因及解决方法----[https://blog.csdn.net/vca821/article/details/80722017](https://blog.csdn.net/vca821/article/details/80722017)
调试stm32的停止模式的时候老遇到这个问题，据说是芯片进入休眠自锁状态，在网上查资料大概总结为2个方法：
方法一：
Options for target--->Debug--->右边的settings--->Debug分栏的Connect & Reset option, Nornal改为With pre-reset.
效果: 有时候可以有时候不行.
方法二：
按住开发板的复位按钮不放开，然后再进行烧写程序，即按Downing或Debug，过大约0.5到1秒后松开复位按钮即可成功烧写。
效果：很不错，基本100%成功。
--------------------- 
