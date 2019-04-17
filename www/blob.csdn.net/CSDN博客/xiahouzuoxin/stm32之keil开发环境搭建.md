# stm32之keil开发环境搭建 - xiahouzuoxin - CSDN博客





2013年08月13日 14:28:54[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：45135
个人分类：[Cortex-M3/M4](https://blog.csdn.net/xiahouzuoxin/article/category/1205536)








-     只要按照下面的一步步来，绝对能从0开始建立一个STM32工程。不仅包括工程建立过程，还有Jlink设置方法。本文使用芯片为STM32F103CB。
## 1 下载stm32F10x的官方库


[http://www.st.com/web/en/catalog/tools/PF257890](http://www.st.com/web/en/catalog/tools/PF257890)


- 
## 2 新建工程


工程名设为stm32_demo，选择芯片型号为STM32F103B，如图，

![](http://blog.chinaunix.net/attachment/201308/13/25984261_1376374859ggg3.png)

因为下载的stm32库中有启动代码，所以这里选择"否"，不拷贝启动代码。

![](http://blog.chinaunix.net/attachment/201308/13/25984261_1376374859NnWj.png)

在工程文件下，新建Startup 、Headers、User、Libraries、CMSIS、Lists、Output文件夹。

|文件夹|用途|
|----|----|
|Startup|启动文件，Flash在16~32Kb小容量，64~128Kb中容量，256~512Kb大容量|
|CMSIS|Cortex微控制器软件接口标准文件，该目录下文件适用所有Cortex系列|
|Libraries|存放stm32的驱动库文件|
|Headers|自定义的全局头文件|
|User|用户文件，我们把main.c放在该目录下|
|Lists|编译过程中产生的文件|
|Output|编译后输出文件，hex/bin等可执行属性的文件将保存在该目录下|


至此，stm32的工程文件结构如下

![](http://blog.chinaunix.net/attachment/201308/13/25984261_1376374860c89Y.png)


- 
## 3 库文件拷贝


把下载stm32库中文件拷贝到新建工程中

|stm32F10x的官方库|工程|
|----|----|
|Libraries\STM32F10x_StdPeriph_Driver\inc 库头文件Libraries\STM32F10x_StdPeriph_Driver\src 库源文件|Libraries|
|Project\STM32F10x_StdPeriph_Template\main.cProject\STM32F10x_StdPeriph_Template\stm32f10x_it.c 中断函数文件|User|
|Project\STM32F10x_StdPeriph_Template\stm32f10x_it.h 中断函数头文件Project\STM32F10x_StdPeriph_Template\stm32f10x_conf.h 配置文件|Headers|
|Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\startup\arm\* 启动文件|Startup|
|Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\stm32f10x.hLibraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\ system_stm32f10x.cLibraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\system_stm32f10x.h|CMSIS|
|Libraries\CMSIS\CM3\CoreSupport\core_cm3.c Cortex-M3系统文件Libraries\CMSIS\CM3\CoreSupport\core_cm3.h|CMSIS|


文件拷贝完成后的工程文件目录结构如下：

![](http://blog.chinaunix.net/attachment/201308/13/25984261_1376374861SZs3.png)


- 
## 4 将文件添加到工程


点击Keil右上角的工程分组按钮，在Group一列添加分组，分组和工程的文件名可以一一对应。

![](http://blog.chinaunix.net/attachment/201308/13/25984261_1376374862yCyf.png)



![](http://blog.chinaunix.net/attachment/201308/13/25984261_1376374862h8Y0.png)



![](http://blog.chinaunix.net/attachment/201308/13/25984261_1376374863IETA.png)



![](http://blog.chinaunix.net/attachment/201308/13/25984261_1376374863d11Z.png)

![](http://blog.chinaunix.net/attachment/201308/13/25984261_1376374864Kq4B.png)



- 
## 5 工程配置


点击右上角的工程配置按钮，弹出对话框，有多个选项卡，按照下面截图逐一配置。

![](http://blog.chinaunix.net/attachment/201308/13/25984261_13763748653dZ5.jpg)

![](http://blog.chinaunix.net/attachment/201308/13/25984261_1376374865ZhhI.jpg)

![](http://blog.chinaunix.net/attachment/201308/13/25984261_13763748661Vit.png)

![](http://blog.chinaunix.net/attachment/201308/13/25984261_1376374867WGMW.jpg)



- 
## 6 编译链接


原来的main.c从库文件中拷贝过来的，把其中的内容都删除，添加最简单的main函数：



```cpp
#include "stm32f10x.h"
int main(void)
{
    while(1) {
 
    }
}
```



修改配置文件stm32f10x_conf.h，通过注释添加或取消注释删除需要的功能模块，这里根据自己需要配置。

![](http://blog.chinaunix.net/attachment/201308/13/25984261_13763748683g8q.png)

编译

![](http://blog.chinaunix.net/attachment/201308/13/25984261_1376374869XhTB.jpg)


- 
## 7 Jlink调试配置


接上Jlink及开发板，

![](http://blog.chinaunix.net/attachment/201308/13/25984261_1376374869SSUi.jpg)

![](http://blog.chinaunix.net/attachment/201308/13/25984261_1376374870txqi.jpg)

![](http://blog.chinaunix.net/attachment/201308/13/25984261_1376374871slg7.png)

![](http://blog.chinaunix.net/attachment/201308/13/25984261_137637487282pK.jpg)

至此，STM32的工程搭建和Jlink调试配置都设置好了。为了方便，在工程成中添加了includes.h、types.h、gpio_bits.h等提供基本功能。
- 
## 8 测试程序——LED流水灯程序


在工程文件目录下新建Drivers与Devices文件夹，

|Drivers|存放stm32相关的驱动，比如：延时函数等|
|----|----|
|Devices|存放开发板上涉及的硬件设备相关代码|


工程中File->New，新建下面的一些文件。

|Drivers/delay.h|延时函数头文件|
|----|----|
|Drivers/delay.c|延时函数源文件|
|Devices/led.h|LED流水灯头文件|
|Devices/led.c|LED流水灯源文件|


注：作为一种好的习惯，每个.c文件都应该有一个对应的.h文件。

添加代码：

led.c



```cpp
/*
* LED example
* Author : xiahouzuoxin
* Date : 2013.08
*/
#include "LED.h"
 
GPIO_InitTypeDef LED_InitStructure;
 
/********************************************************************
Function Name: Init_LED
Author : 夏侯佐鑫
Date : 2011-09-28
Description :
Inputs : None
Outputs : None
Notes :
Revision :
********************************************************************/
void InitLED(void)
{
//使能PA端口时钟
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
 
    //端口配置推挽输出
    LED_InitStructure.GPIO_Pin = LED1 | LED2 | LED3 | LED4;
    LED_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    LED_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    
    GPIO_Init(GPIOA, &LED_InitStructure);
 
    //初始输出高电平
    GPIO_SetBits(GPIOA, LED1 | LED2 | LED3 | LED4);
}
 
/********************************************************************
Function Name: Flash_LED
Author : xiahouzuoxin
Date : 2011-09-28
Description :
Inputs : None
Outputs : None
Notes :
Revision :
********************************************************************/
void FlashLED(void)
{
GPIO_SetBits(GPIOA, LED2 | LED3 | LED4);
    GPIO_ResetBits(GPIOA, LED1);
    delay_ms(500);
GPIO_SetBits(GPIOA, LED1 | LED3 | LED4);
    GPIO_ResetBits(GPIOA, LED2);
    delay_ms(500);
GPIO_SetBits(GPIOA, LED1 | LED2 | LED4);
    GPIO_ResetBits(GPIOA, LED3);
    delay_ms(500);
GPIO_SetBits(GPIOA, LED1 | LED2 | LED3);
    GPIO_ResetBits(GPIOA, LED4);
    delay_ms(500);
}
```



led.h



```cpp
/*
* LED example
* Author : xiahouzuoxin
* Date : 2013.08
*/
#ifndef __LED_H__
#define __LED_H__
 
#include "../Headers/includes.h"
 
#define LED1                      GPIO_Pin_4
#define LED2                      GPIO_Pin_5
#define LED3                      GPIO_Pin_6
#define LED4                      GPIO_Pin_7
 
extern void InitLED(void);
extern void FlashLED(void);
 
#endif
```



delay.h



```cpp
#ifndef _DELAY_H
#define _DELAY_H
 
#include "includes.h"
 
extern void delay_ms(UINT32 ms);
 
#endif
```



delay.c



```cpp
#include "delay.h"
 
/********************************************************************* DELAY_MS
* Discription : delay for 1 ms if ms=1, not accurate
* Author : xiahouzuoxin
* data : 2012-08-01
* inputs :    ms -- ms number
* outputs :
* Modified :
********************************************************************/
void delay_ms(UINT32 ms)
{
int i = 0;
    int j = 0;
 
for(i = 0; i < ms; i++)
{
for(j = 0; j < 8040; j++)
{
// Delay n ms
}
}
}
```



includes.h



```cpp
#include "../Devices/LED.h"
```



main.c



```cpp
#include "includes.h"
 
int main(void)
{
InitLED();
while(1) {
     FlashLED();
}
}
```



编译下载，运行，OK！



