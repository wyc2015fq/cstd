# STM32学习笔记（3）：系统时钟和SysTick定时器 - xqhrs232的专栏 - CSDN博客
2013年04月26日 15:37:38[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：830
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[http://blog.sina.com.cn/s/blog_49cb42490100s60d.html](http://blog.sina.com.cn/s/blog_49cb42490100s60d.html)
1.STM32的时钟系统
在STM32中，一共有5个时钟源，分别是HSI、HSE、LSI、LSE、PLL
（1）HSI是高速内部时钟，RC振荡器，频率为8MHz；
（2）HSE是高速外部时钟，可接石英/陶瓷谐振器，或者接外部时钟源，频率范围是4MHz
 – 16MHz；
（3）LSI是低速内部时钟，RC振荡器，频率为40KHz；
（4）LSE是低速外部时钟，接频率为32.768KHz的石英晶体；
（5）PLL为锁相环倍频输出，严格的来说并不算一个独立的时钟源，PLL的输入可以接HSI/2、HSE或者HSE/2。倍频可选择为2
 – 16倍，但是其输出频率最大不得超过72MHz。
其中，40kHz的LSI供独立看门狗IWDG使用，另外它还可以被选择为实时时钟RTC的时钟源。另外，实时时钟RTC的时钟源还可以选择LSE，或者是HSE的128分频。
STM32中有一个全速功能的USB模块，其串行接口引擎需要一个频率为48MHz的时钟源。该时钟源只能从PLL端获取，可以选择为1.5分频或者1分频，也就是，当需使用到USB模块时，PLL必须使能，并且时钟配置为48MHz或72MHz。
另外STM32还可以选择一个时钟信号输出到MCO脚(PA.8)上，可以选择为PLL输出的2分频、HSI、HSE或者系统时钟。
系统时钟SYSCLK，它是提供STM32中绝大部分部件工作的时钟源。系统时钟可以选择为PLL输出、HSI、HSE。系系统时钟最大频率为72MHz，它通过AHB分频器分频后送给各个模块使用，AHB分频器可以选择1、2、4、8、16、64、128、256、512分频，其分频器输出的时钟送给5大模块使用：
（1）送给AHB总线、内核、内存和DMA使用的HCLK时钟；
（2）通过8分频后送给Cortex的系统定时器时钟；
（3）直接送给Cortex的空闲运行时钟FCLK；
（4）送给APB1分频器。APB1分频器可以选择1、2、4、8、16分频，其输出一路供APB1外设使用（PCLK1，最大频率36MHz），另一路送给定时器(Timer)2、3、4倍频器使用。该倍频器可以选择1或者2倍频，时钟输出供定时器2、3、4使用。
（5）送给APB2分频器。APB2分频器可以选择1、2、4、8、16分频，其输出一路供APB2外设使用（PCLK2，最大频率72MHz），另外一路送给定时器(Timer)1倍频使用。该倍频器可以选择1或2倍频，时钟输出供定时器1使用。另外APB2分频器还有一路输出供ADC分频器使用，分频后送给ADC模块使用。ADC分频器可选择为2、4、6、8分频。
需要注意的是定时器的倍频器，当APB的分频为1时，它的倍频值为1，否则它的倍频值就为2。
连接在APB1(低速外设)上的设备有：电源接口、备份接口、CAN、USB、I2C1、I2C2、UART2、UART3、SPI2、窗口看门狗、Timer2、Timer3、Timer4。**注意****USB模块虽然需要一个单独的48MHz的时钟信号，但是它应该不是供USB模块工作的时钟，而只是提供给串行接口引擎(SIE)使用的时钟。USB模块的工作时钟应该是由APB1提供的。**
连接在APB2（高速外设）上的设备有：UART1、SPI1、Timer1、ADC1、ADC2、GPIOx(PA~PE)、第二功能IO口。
2.STM32时钟的初始化
由于我现在所用的开发板已经外接了一个8MHz的晶振，因此将采用HSE时钟，在MDK编译平台中，程序的时钟设置参数流程如下：
(1)将RCC寄存器重新设置为默认值：RCC_DeInit;
(2)打开外部高速时钟晶振HSE： RCC_HSEConfig(RCC_HSE_ON);
(3)等待外部高速时钟晶振工作： HSEStartUpStatus
 = RCC_WaitForHSEStartUp();
(4)设置AHB时钟(HCLK)： RCC_HCLKConfig;
(5)设置高速AHB时钟(APB2)： RCC_PCLK2Config;
(6)设置低速AHB时钟(APB1)： RCC_PCLK1Config;
(7)设置PLL： RCC_PLLConfig;
(8)打开PLL： RCC_PLLCmd(ENABLE);
(9)等待PLL工作： while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)
 == RESET);
(10)设置系统时钟： RCC_SYSCLKConfig;
(11)判断PLL是否是系统时钟： while(RCC_GetSYSCLKSource()
 != 0x08);
(12)打开要使用的外设时钟： RCC_APB2PerphClockCmd()….
某些函数的详细的使用方法，可以参考ST公司出版的《STM32F10xxx_Library_Manual》
3.SysTick定时器
NVIC中，捆绑着一个SysTick定时器，它是一个24位的倒数计数定时器，当计到0时，将从RELOAD寄存器中自动重装载定时初值并继续计数，同时内部的COUNTFLAG标志会置位，触发中断(如果中断使能情况下)。只要不把它在SysTick控制及状态寄存器中的使能位清除，就用不停息。Cortex-M3允许为SysTick提供2个时钟源以供选择，第一个是内核的“自由运行时钟”FCLK，“自由”表现在它不是来自系统时钟HCLK，因此在系统时钟停止时，FCLK也能继续运行。第2个是一个外部的参考时钟，但是使用外部时钟时，因为它在内部是通过FCLK来采样的，因此其周期必须至少是FCLK的两倍（采样定理）。
下面介绍一下STM32中的SysTick，它属于NVIC控制部分，一共有4个寄存器：
STK_CSR， 0xE000E010：控制寄存器
STK_LOAD， 0xE000E014：重载寄存器
STK_VAL， 0xE000E018：当前值寄存器
STK_CALRB， 0xE000E01C：校准值寄存器
首先看**STK_CSR控制寄存器**，有4个bit具有意义：
第0位：ENABLE，SysTick使能位（0：关闭SysTick功能，1：开启SysTick功能）；
第1位：TICKINT，SysTick中断使能位（0：关闭SysTick中断，1：开启SysTick中断）；
第2位：CLKSOURCE，SysTick时钟选择（0：使用HCLK/8作为时钟源，1：使用HCLK）；
第3为：COUNTFLAG，SysTick计数比较标志，如果在上次读取本寄存器后，SysTick已经数到0了，则该位为1，如果读取该位，该位自动清零。
**STK_LOAD重载寄存器：**
Systick是一个递减的定时器，当定时器递减至0时，重载寄存器中的值就会被重装载，继续开始递减。STK_LOAD重载寄存器是个24位的寄存器最大计数0xFFFFFF。
**STK_VAL当前值寄存器：**
也是个24位的寄存器，读取时返回当前倒计数的值，写它则使之清零，同时还会清除在SysTick控制及状态寄存器中的COUNTFLAG标志。
**STK_CALRB校准值寄存器：**
其中包含着一个TENMS位段，具体信息不详。暂时用不到。
在MDK开发环境中，我们不必要非得去操作每一个寄存器，可以通过调用ST函数库中的函数来进行相关的操作，其步骤如下：
（1）调用SysTick_CounterCmd()失能SysTick计数器
（2）调用SysTick_ITConfig()失能SysTick中断
（3）调用SysTick_CLKSourceConfig()设置SysTick时钟源
（4）调用SysTick_SetReload()设置SysTick重装载值
（5）调用NVIC_SystemHandlerPriorityConfig()设置SysTick定时器中断优先级
（6）调用SysTick_ITConfig()使能SysTick中断
（7）在stm32f10x_it.c中SysTickHandler()下写中断服务函数。
（8）在需要的时候调用SysTick_CounterCmd()开启SysTick计数器
4.工程实现
根据以上描述，准备利用开发板上的LED灯做一个小实验，将第一个跑马灯的实验稍微改进一下，以1s精确延时的状态来顺序点亮LED灯，采用的定时器就是SysTick。
设计思路是先配置好系统的各个参数，然后设置SysTick定时器每1ms就进入一次中断，再定义一个全局变量作为定时长短的参数，然后将从延时函数中得到的参数赋值给这个全局变量，每进入一次中断，这个全局变量就减一次，直到减为0，才跳出延时函数。
1.配置系统时钟
void RCC_cfg()
{
 //定义错误状态变量
 ErrorStatus HSEStartUpStatus;
 //将RCC寄存器重新设置为默认值
 RCC_DeInit();
 //打开外部高速时钟晶振
 RCC_HSEConfig(RCC_HSE_ON);
 //等待外部高速时钟晶振工作
 HSEStartUpStatus = RCC_WaitForHSEStartUp();
 if(HSEStartUpStatus == SUCCESS)
 {
 //设置AHB时钟(HCLK)为系统时钟
 RCC_HCLKConfig(RCC_SYSCLK_Div1);
 //设置高速AHB时钟(APB2)为HCLK时钟
 RCC_PCLK2Config(RCC_HCLK_Div1);
 //设置低速AHB时钟(APB1)为HCLK的2分频
 RCC_PCLK1Config(RCC_HCLK_Div2);
 //设置FLASH代码延时
 FLASH_SetLatency(FLASH_Latency_2);
 //使能预取指缓存
 FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
 //设置PLL时钟，为HSE的9倍频8MHz
 * 9 = 72MHz
 RCC_PLLConfig(RCC_PLLSource_HSE_Div1,
 RCC_PLLMul_9);
 //使能PLL
 RCC_PLLCmd(ENABLE);
 //等待PLL准备就绪
 while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)
 == RESET);
 //设置PLL为系统时钟源
 RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
 //判断PLL是否是系统时钟
 while(RCC_GetSYSCLKSource()
 != 0x08);
 }
 //打开PB和PD用于点亮LED灯
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB
 | RCC_APB2Periph_GPIOD, ENABLE);
}
**其中使用到了NVIC的函数，需要将stm32f10xR.lib加入到工程中。**
2.配置SysTick定时器
void SysTick_cfg()
{
 //设置失能SysTick定时器
 SysTick_CounterCmd(SysTick_Counter_Disable);
 //设置失能SysTick中断
 SysTick_ITConfig(DISABLE);
 //设置SysTick的时钟源为AHB时钟
 SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
 //设置重装载值，由于SysTick是AHB时钟，即72MHz，所以重装载值设置为72000，即每1ms重新装载一次
 SysTick_SetReload(72000);
 //设置SysTick定时器中断优先级
 NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick,
 1, 0);
 //设置使能SysTick中断
 SysTick_ITConfig(ENABLE);
}
3.编写延时函数
void Delay(u32 nTime)
{
 TimingDelay = nTime;
 //允许SysTick定时器
 SysTick_CounterCmd(SysTick_Counter_Enable);
 //循环等待定时时间到
 while(TimingDelay != 0);
 //禁止SysTick定时器
 SysTick_CounterCmd(SysTick_Counter_Disable);
 //清空SysTick定时器
 SysTick_CounterCmd(SysTick_Counter_Clear);
}
4.设置通用IO口
void GPIO_cfg()
{
 GPIO_InitTypeDef
 GPIO_InitStructure; //定义GPIO宏操作结构体
 GPIO_InitStructure.GPIO_Pin
 = GPIO_Pin_5;
 GPIO_InitStructure.GPIO_Mode
 = GPIO_Mode_Out_PP; //将B5口配置为通用推挽输出
 GPIO_InitStructure.GPIO_Speed
 = GPIO_Speed_50MHz; //口线翻转速度为50MHz
 GPIO_Init(GPIOB,
 &GPIO_InitStructure); //配置GPIOB口
 GPIO_InitStructure.GPIO_Pin
 = GPIO_Pin_6|GPIO_Pin_3; //将D3和D6口配置为推挽输出
 GPIO_Init(GPIOD,
 &GPIO_InitStructure); //配置GPIOD口
}
5.中断服务函数
void SysTickHandler(void)
{
 TimingDelay--;
}
**其中TimingDelay这个全局变量在使用的时候需要在stm32f10x_it.c中进行一下声明，要不然不能使用，其声明语句是：**
**extern vu32 TimingDelay;**
在延时程序中一直检测TimingDelay这个全局变量，只有当其减为0的时候，才跳出延时函数。
6.主程序
#include "stm32f10x_lib.h"
vu32 TimingDelay;
void RCC_cfg();
void SysTick_cfg();
void Delay(u32 nTime);
void GPIO_cfg();
int main()
{
 RCC_cfg(); //配置RCC时钟
 SysTick_cfg(); //配置SysTick定时器
 GPIO_cfg(); //配置通用IO口
 while(1)
 {
 GPIO_ResetBits(GPIOB,
 GPIO_Pin_5);
 GPIO_ResetBits(GPIOD,
 GPIO_Pin_6);
 GPIO_SetBits(GPIOD,
 GPIO_Pin_3);
 Delay(1000);  //延时1s
 GPIO_ResetBits(GPIOB,
 GPIO_Pin_5);
 GPIO_ResetBits(GPIOD,
 GPIO_Pin_3);
 GPIO_SetBits(GPIOD,
 GPIO_Pin_6);
 Delay(1000);
 GPIO_SetBits(GPIOB,
 GPIO_Pin_5); 
 GPIO_ResetBits(GPIOD,
 GPIO_Pin_6);
 GPIO_ResetBits(GPIOD,
 GPIO_Pin_3); 
 Delay(1000);
 }
}
在延时函数Delay的参数中，可以填入任意的32bits的整数，延时单位是ms。至此，SysTick定时器和系统时钟的设置就到此基本上全部讲完了。
