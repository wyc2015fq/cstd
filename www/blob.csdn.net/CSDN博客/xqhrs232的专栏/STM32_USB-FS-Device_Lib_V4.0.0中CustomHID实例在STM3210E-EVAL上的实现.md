# STM32_USB-FS-Device_Lib_V4.0.0中CustomHID实例在STM3210E-EVAL上的实现 - xqhrs232的专栏 - CSDN博客
2017年09月13日 11:12:40[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1169
原文地址::[http://blog.csdn.net/phenixyf/article/details/9001951](http://blog.csdn.net/phenixyf/article/details/9001951)
相关文章
1、最新STM32_USB-FS-Device_Lib_V4.0.0.zip----[http://download.csdn.net/download/u012672024/6512625](http://download.csdn.net/download/u012672024/6512625)
1. 将STM32_USB-FS-Device_Lib_V4.0.0下的Libraries、Utilities文件夹和Projects\Custom_HID下的inc、src和MDK-ARM文件夹拷出。并保持原层叠关系：
![](https://img-blog.csdn.net/20130605155337906)
![](https://img-blog.csdn.net/20130605155252578)
2. 用MDK新建工程，Device选择STM32F103ZE
![](https://img-blog.csdn.net/20130531174848430)
3. 在工程中添加如下文件夹：
![](https://img-blog.csdn.net/20130531175046645)
4. 各文件夹下从添加的文件如下：
    4.1 User文件夹下的文件在Projects\Custom_HID\src下
![](https://img-blog.csdn.net/20130531175642899)
    4.2  CMSIS文件夹下文件在Libraries\CMSIS\Device\ST\STM32F10x\Source\Templates和Libraries\CMSIS\Device\ST\STM32F10x\Source\Templates\arm下
![](https://img-blog.csdn.net/20130531180211591)
    4.3  STM32_USB-FS-Device_Driver文件夹下文件路径Libraries\STM32_USB-FS-Device_Driver\src
![](https://img-blog.csdn.net/20130531180604742)
    4.4  STM32F10x_StdPeriph_Driver文件夹下文件路径：Libraries\STM32F10x_StdPeriph_Driver\src
![](https://img-blog.csdn.net/20130531180954624)
    4.5  STM3210E_EVAL文件夹下文件路径：Utilities\STM32_EVAL\STM3210E_EVAL
![](https://img-blog.csdn.net/20130531181245103)
5.  工程设置：Option->C/C++  （重点）
![](https://img-blog.csdn.net/20130531181648460)
![](https://img-blog.csdn.net/20130531182727256)
Include Paths下各文件夹路径：
Projects\Custom_HID\inc
Libraries\CMSIS\Device\ST\STM32F10x\Include
Libraries\STM32_USB-FS-Device_Driver\inc
Libraries\STM32F10x_StdPeriph_Driver\inc
Utilities\STM32_EVAL\Common
Utilities\STM32_EVAL\STM3210E_EVAL
以上各路径均为步骤1中文件路径
6.  Build Project
![](https://img-blog.csdn.net/20130531183242295)
[](http://blog.csdn.net/phenixyf/article/details/9001951#)[](http://blog.csdn.net/phenixyf/article/details/9001951#)[](http://blog.csdn.net/phenixyf/article/details/9001951#)[](http://blog.csdn.net/phenixyf/article/details/9001951#)[](http://blog.csdn.net/phenixyf/article/details/9001951#)[](http://blog.csdn.net/phenixyf/article/details/9001951#)

