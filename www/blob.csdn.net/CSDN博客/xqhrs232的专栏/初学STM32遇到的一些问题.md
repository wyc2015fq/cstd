# 初学STM32遇到的一些问题 - xqhrs232的专栏 - CSDN博客
2016年02月23日 12:35:05[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4323
原文地址::[http://blog.csdn.net/sy_lixiang/article/details/8586909](http://blog.csdn.net/sy_lixiang/article/details/8586909)
相关文章
1、**STM8S单片机EEPROM烧录错误：Verify failed at address 0xXXXX----[http://www.51hei.com/bbs/dpj-25357-1.html](http://www.51hei.com/bbs/dpj-25357-1.html)**
1.设置工程时，如果忘记了在C/C++选项卡上设置STM32F10X_HD,USE_STDPEFIPH_DRIVER编译时会报如下的错误
**[cpp]**[view
 plain](http://blog.csdn.net/sy_lixiang/article/details/8586909#)[copy](http://blog.csdn.net/sy_lixiang/article/details/8586909#)
- Build target 'USART'
- compiling stm32f10x_it.c...  
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h(96): error:  #35: #error directive: "Please select first the target STM32F10x device used in your application (in stm32f10x.h file)"
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h:    #error "Please select first the target STM32F10x device used in your application (in stm32f10x.h file)"
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h:     ^  
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h: ..\User\stm32f10x_it.c: 0 warnings, 1 error  
- compiling main.c...  
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h(96): error:  #35: #error directive: "Please select first the target STM32F10x device used in your application (in stm32f10x.h file)"
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h:    #error "Please select first the target STM32F10x device used in your application (in stm32f10x.h file)"
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h:     ^  
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h: ..\User\main.c: 0 warnings, 1 error  
- compiling core_cm3.c...  
- compiling system_stm32f10x.c...  
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h(96): error:  #35: #error directive: "Please select first the target STM32F10x device used in your application (in stm32f10x.h file)"
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h:    #error "Please select first the target STM32F10x device used in your application (in stm32f10x.h file)"
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h:     ^  
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h: ..\User\system_stm32f10x.c: 0 warnings, 1 error  
- compiling stm32f10x_usart.c...  
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h(96): error:  #35: #error directive: "Please select first the target STM32F10x device used in your application (in stm32f10x.h file)"
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h:    #error "Please select first the target STM32F10x device used in your application (in stm32f10x.h file)"
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h:     ^  
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h: ..\Libraries\STM32F10x_StdPeriph_Driver\src\stm32f10x_usart.c: 0 warnings, 1 error  
- compiling stm32f10x_gpio.c...  
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h(96): error:  #35: #error directive: "Please select first the target STM32F10x device used in your application (in stm32f10x.h file)"
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h:    #error "Please select first the target STM32F10x device used in your application (in stm32f10x.h file)"
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h:     ^  
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h: ..\Libraries\STM32F10x_StdPeriph_Driver\src\stm32f10x_gpio.c: 0 warnings, 1 error  
- compiling stm32f10x_rcc.c...  
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h(96): error:  #35: #error directive: "Please select first the target STM32F10x device used in your application (in stm32f10x.h file)"
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h:    #error "Please select first the target STM32F10x device used in your application (in stm32f10x.h file)"
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h:     ^  
- ..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.h: ..\Libraries\STM32F10x_StdPeriph_Driver\src\stm32f10x_rcc.c: 0 warnings, 1 error  
- Target not created  
2.要注意新建工程包含库文件的路径是
\Libraries\STM32F10x_StdPeriph_Driver\inc
而不是
\Libraries\STM32F10x_StdPeriph_Driver\src
3.Overlapping of Algorithms at Address 08000000H错误
![](https://img-my.csdn.net/uploads/201302/20/1361329164_2892.jpg)
工程设置里，误多添加了下载算法，删除后即可。
![](https://img-my.csdn.net/uploads/201302/20/1361329168_9926.jpg)
//============================================================================================
备注::
1>今天烧写STM32遇到一个新问题----出现data not matching at address 0x80000000，最后把问题找到是烧写的串口线出了问题 ，导致烧完程序再读回验证出了问题。
![](https://img-blog.csdn.net/20160223123439906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
