# STM32的RTC - xqhrs232的专栏 - CSDN博客
2013年04月14日 11:16:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1306
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[http://www.360doc.com/content/11/0919/13/7736891_149467322.shtml](http://www.360doc.com/content/11/0919/13/7736891_149467322.shtml)
## STM32的RTC
![](http://image37.360doc.com/DownloadImg/2011/09/1913/17758726_1.gif)   要使用 RTC,一般来说也顺带地使用上后备寄存器—这是因为RTC 是一个简单的秒中断定时器,年月日时分秒的信息必须要找个地方能掉电保存才有意义.STM32
 的备份寄存器(BKP)是42个16位的寄存器，可用来存储84个字节的用户应用程序数据。他们处在备份域里，当VDD电源被切断，他们仍然由VBAT维持供电。当系统在待机模式下被唤醒，或系统复位或电源复位时，他们也不会被复位。此外，BKP控制寄存器用来管理侵入检测和RTC校准功能。复位后，对备份寄存器和RTC的访问被禁止，并且备份域被保护以防止可能存在的意外的写操作它是用后备电池供电的,只要配置好后,即使系统关电,32768 的晶振和后备寄存器的数据也会得以维持.而且RTC的计数器是会继续秒递增的。
![STM32的RTC](http://image37.360doc.com/DownloadImg/2011/09/1913/17758726_2)
这里引用手册里一段概述“**RTC由两个主要部分组成。第一部分（APB1接口）用来和 APB1总线相连。此单元还包含一组 16位寄存器，可通过 APB1总线对其进行读写操作。APB1接口以 APB1总线时钟为时钟，用来与 APB1总线接口。**
**另一部分（RTC核）由一系列可编程计数器组成，分成两个主要模块。第一个模块是 RTC的预分频模块，它可编程产生最长为 1秒的 RTC时间基准 TR_CLK。RTC的预分频模块包含了一个 20位的可编程分频器（RTC预分频器）。在每个TR_CLK周期中，如果在 RTC_CR 寄存器中设置了相应允许位，则 RTC产生一个中断（秒中断）。第 2个模块是一个 32位的可编程的计数器，它可被初始化为当前的系统时间。系统时间以 TR_CLK速度增长并与存储在 RTC_ALR寄存器中的可编程的时间相比较，如果 RTC_CR控制寄存器中设置了相应允许位，则比较匹配时将产生一个闹钟中断。”**
对于第一次实用RTC的时候我们要对它进行配置一番，现在大致说一下（代码是通过调用RTC_Config函数来实现的）：
1.打开电源管理和备份寄存器时钟，提到备份寄存器这里要说一下，引用手册--“**备份寄存器是 10个 16位的寄存器，可用来存储 20个字节的用户应用程序数据。他们处在备份域里，当 VDD电源被切断，他们仍然由 VBAT维持供电。当系统在待机模式下被唤醒，或系统复位或电源复位时，他们也不会被复位。**”我们正式通过在备份寄存器写固定的数据来判断芯片是否第一次实用RTC，从而在系统运行RTC时提示配置时钟的。
2.使能RTC和备份寄存器的访问(复位默认关闭)。引用手册--“**复位后，对备份寄存器和 RTC的访问被禁止，并且备份域被保护以防止可能存在的意外的写操作。电源控制寄存器(PWR_CR)的 DBP位必须被置 1，以允许访问备份寄存器和RTC.**”因为程序要对RTC和备份寄存器操作，所以必须使能。
3.选择外部低速晶体为RTC时钟，并使能时钟；
4.使能秒中断，程序里在秒中断里置位标志位来通知主程序显示时间数据，同时在32位计数器到23：59：59时清零；
5.设置RTC预分频器值产生1秒信号计算公式 fTR_CLK = fRTCCLK/(PRL+1)，我们设置32767来产生秒信号；
下面是RTC初始化程序
void RTC_configuration()
{
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP|RCC_APB1Periph_PWR,ENABLE);
 PWR_BackupAccessCmd(ENABLE);//使能RTC后备寄存器的写
 BKP_DeInit();//BKP寄存器全部设为缺省值
 RCC_LSEConfig(RCC_LSE_ON);  //RCC打开了LSE时钟
 while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);//等待LSE就绪，一般来说，如果谐振不对，就会死在这里，实际代码请慎重
 RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//RTC使用时钟源LSE
 RCC_RTCCLKCmd(ENABLE);//RTC的时钟开启
 RTC_WaitForSynchro();//RTC等待同步，
 RTC_WaitForLastTask();//这个代码在RTC中常常出现，类似于等待就绪的含义
 //RTC_ITConfig(RTC_IT_SEC, ENABLE);//RTC秒中断
 RTC_WaitForLastTask();//
 RTC_SetPrescaler(32767);//RTC预分频，32768HZ，分为一秒一个振荡，RTC period = RTCCLK/RTC_PR = (32.768KHz)/(32767+1)
 RTC_WaitForLastTask(); //等待同步
 PWR_BackupAccessCmd(DISABLE);//禁止RTC后备寄存器的写      
}
**为啥要不停的等呢？？**
在这里要注意一下，所有在对RTC寄存器操作之前都要判断读写操作是否完成，也就是说当前是否有读写操作。系统内核是通过RTC的APB1接口来访问RTC内部寄存器的，所以在上电复位，休眠唤醒的时候，我们要先对RTC时钟与 RTC APB1时钟进行重新同步，在同步完成后再对器进行操作，因为RTC的AP1接口使用的系统APB1的时钟。上述配置在初次使用RTC时进行配置，在以后使用过程中，只要RTC外部电池持续供电，无论系统掉电还复位我们都无需重复配置，使用使能秒中断就可以了这一点很是方便嘿嘿...
RTC核完全独立于RTC APB1接口。软件通过APB1接口访问RTC的预分频值、计数器值和闹钟值。但是，相关的可读寄存器只在与RTC APB1时钟进行重新同步的RTC时钟的上升沿被更新。RTC标志也是如此的。这意味着，如果APB1接口曾经被关闭，而读操作又是在刚刚重新开启APB1之后，则在第一次的内部寄存器更新之前，从APB1上读出的RTC寄存器数值可能被破坏了(通常读到0)。
因此，若在读取RTC寄存器时，RTC的APB1接口曾经处于禁止状态，则软件首先必须等待RTC_CRL寄存器中的RSF位(寄存器同步标志)被硬件置’1’。
** 注：** RTC的 APB1接口不受WFI和WFE等低功耗模式的影响。
**NVIC的配置**
void NVIC_Configuration()
{  
 NVIC_InitTypeDef NVIC_InitStructure;
 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);   // 抢占式优先级别 
 NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQChannel;//指定中断源
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;// 指定响应优先级别1
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStructure);
}
void RTC_IRQHandler(void)
{
 if(RTC_GetITStatus(RTC_IT_SEC) != RESET) //RTC发生了秒中断（也有可能是溢出或者闹钟中断)
 {
   //此处添加代码
    RTC_ClearITPendingBit(RTC_IT_SEC);
  }
}
**RTC初始化后碰到的问题**
只要设置时间，就停在了RTC_WaitForLastTask(); 
发现是由于RTC配置的问题。 
在RTC初始化程序中，我们一般要查看BKP寄存器中的数据，判断后备电源是否掉电过。 
如果掉电过，则初始化。没有在不执行。 
但要设置时钟的话，必需允许PWR和BKP时钟，并允许读取BKP。 
而如果仅电源掉电而后备电池不掉电的话，PWR和BKP时钟是没有开放的。 
这样就等不到RTC指令执行完成的标志，停在那了。 
解决方法就是在RTC初始化中判断判断掉电标志位的else分支中加入 
//启用PWR和BKP的时钟（from APB1） 
RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); 
//后备域解锁 
PWR_BackupAccessCmd(ENABLE); 
即 
if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5) 
{ 
    //重新配置RTC 
    RTC_Configuration(); 
    ... 
    ... 
} 
else 
{ 
    //启用PWR和BKP的时钟（from APB1） 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); 
    //后备域解锁 
    PWR_BackupAccessCmd(ENABLE); 
    ... 
    ... 
}
