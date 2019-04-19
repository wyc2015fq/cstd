# stm32 HardFault_Handler调试及问题查找方法 - xqhrs232的专栏 - CSDN博客
2015年12月03日 00:03:15[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：5175
原文地址::[http://www.51hei.com/bbs/dpj-39846-1.html](http://www.51hei.com/bbs/dpj-39846-1.html)
相关文章
1、[STM32 出现 hardfault_handler 处理方法](http://blog.csdn.net/sunjiajiang/article/details/7582752)----[http://blog.csdn.net/sunjiajiang/article/details/7582752](http://blog.csdn.net/sunjiajiang/article/details/7582752)
2、**深入Cortex-M3内核的问题，程序跑飞了HardFault_Handler----[http://bbs.21ic.com/icview-671360-1-1.html](http://bbs.21ic.com/icview-671360-1-1.html)**
3、解决stm32进入HardFault_Handler的定位办法----[http://bbs.ednchina.com/BLOG_ARTICLE_3024685.HTM](http://bbs.ednchina.com/BLOG_ARTICLE_3024685.HTM)
  STM32出现HardFault_Handler故障的原因主要有两个方面：
1、内存溢出或者访问越界。这个需要自己写程序的时候规范代码，遇到了需要慢慢排查。
2、堆栈溢出。增加堆栈的大小。
出现问题时排查的方法：
发生异常之后可首先查看LR寄存器中的值，确定当前使用堆栈为MSP或PSP，然后找到相应堆栈的指针，并在内存中查看相应堆栈里的内容。由于异常发生时，内核将R0~R3、R12、Returnaddress、PSR、LR寄存器依次入栈，其中Return address即为发生异常前PC将要执行的下一条指令地址。
注意：[寄存器](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=18&is_app=0&jk=70001d0e7379d299&k=%BC%C4%B4%E6%C6%F7&k0=%BC%C4%B4%E6%C6%F7&kdi0=0&luki=10&mcpm=0&n=10&p=baidu&q=98059059_cpr&rb=0&rs=1&seller_id=1&sid=99d27973e1d0070&ssp2=1&stid=9&t=tpclicked3_hc&td=1831118&tu=u1831118&u=http%3A%2F%2Fwww.51hei.com%2Fbbs%2Fdpj-39846-1.html&urlid=0)均是32位，且STM32是小端模式。(参考Cortex-M3权威)
![](http://c.51hei.com/a/a/c/510299381183059.jpg)
编写问题代码如下：
- 
void StackFlow(void)
- 
- 
{
- 
- 
int a[3],i;
- 
- 
for(i=0; i<10000; i++)
- 
- 
{
- 
- 
a[i]=1;
- 
- 
}
- 
- 
}
- 
- 
void SystemInit(void)
- 
- 
{
- 
- 
- 
- 
- 
- 
RCC->CR |= (uint32_t)0x00000001;
- 
- 
- 
- 
RCC->CFGR = 0x00000000;
- 
- 
- 
- 
RCC->CR &= (uint32_t)0xFEF6FFFF;
- 
- 
- 
- 
RCC->PLLCFGR = 0x24003010;
- 
- 
StackFlow();
- 
- 
- 
- 
RCC->CR &= (uint32_t)0xFFFBFFFF;
- 
- 
。。。。。。。。。。。。。。
- 
- 
}
复制代码
DEBUG如下图
SP值为0x20008560，查看堆栈里面的值依次为R0~R3、R12、Return address、PSR、LR， 例如R0(1027 00 00), 显然堆栈后第21个字节到24字节即为Returnaddress，该地址0x08001FFD即为异常前PC将要执行的下一条指令地址(即StackFlow()后面的语句处RCC->CR &= (uint32_t)0xFFFBFFFF)
![](http://c.51hei.com/a/a/c/510299381122306.jpg)
另一种方法：
默认的HardFault_Handler处理方法不是B .这样的死循环么?楼主将它改成BXLR直接返回的形式。然后在这条语句打个断点，一旦在断点中停下来，说明出错了，然后再返回，就可以返回到出错的位置的下一条语句那儿
      Cortex-M3/4的Fault异常是由于非法的存储器访问（比如访问0地址、写只读存储位置等）和非法的程序行为（比如除以0等）等造成的。常见的4种异常及产生异常的情况如下：
BusFault：在fetch指令、数据读写、fetch中断向量或中断时存储恢复[寄存器](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=18&is_app=0&jk=70001d0e7379d299&k=%BC%C4%B4%E6%C6%F7&k0=%BC%C4%B4%E6%C6%F7&kdi0=0&luki=10&mcpm=0&n=10&p=baidu&q=98059059_cpr&rb=0&rs=1&seller_id=1&sid=99d27973e1d0070&ssp2=1&stid=9&t=tpclicked3_hc&td=1831118&tu=u1831118&u=http%3A%2F%2Fwww.51hei.com%2Fbbs%2Fdpj-39846-1.html&urlid=0)栈情况下，[检测](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=18&is_app=0&jk=70001d0e7379d299&k=%BC%EC%B2%E2&k0=%BC%EC%B2%E2&kdi0=0&luki=3&mcpm=0&n=10&p=baidu&q=98059059_cpr&rb=0&rs=1&seller_id=1&sid=99d27973e1d0070&ssp2=1&stid=9&t=tpclicked3_hc&td=1831118&tu=u1831118&u=http%3A%2F%2Fwww.51hei.com%2Fbbs%2Fdpj-39846-1.html&urlid=0)到内存访问错误则产生BusFault。
Memory ManagementFault：访问了内存管理单元（MPU）定义的不合法的内存区域，比如向只读区域写入数据。
UsageFault：检测到未定义指令或在存取内存时有未对齐。还可以通过软件配置是否检测到除0和其它未对齐内存访问也产生该异常，默认关闭，需要在工程初始化时配置：
[cpp] viewplaincopyprint?
- SCB->CCR |= 0x18; // enable div-by-0 and unaligned fault  
HardFault：在调试程序过程中，这种异常最常见。上面三种异常发生任何一种异常都会引起HardFault，在上面的三种异常未使能的情况下，默认发生异常时进入HardFault中断服务程序。使能前三种异常也要在初始化时配置：
[cpp] viewplaincopyprint?
- SCB->SHCSR |= 0x00007000;   // enable Usage Fault, Bus Fault, and MMU Fault  
在默认复位初始化时，HardFault使能，其它三者不使能，因此当程序中出现不合法内存访问（一般是指针错误引起）或非法的程序行为（一般就是数学里面常见的除0）时都将产生HardFault中断。
[url=]2
 HardFault调试方法[/url]假设IDE环境为Keil，芯片为STM32F103。
在[stm32](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=18&is_app=0&jk=70001d0e7379d299&k=stm32&k0=stm32&kdi0=0&luki=9&mcpm=0&n=10&p=baidu&q=98059059_cpr&rb=0&rs=1&seller_id=1&sid=99d27973e1d0070&ssp2=1&stid=9&t=tpclicked3_hc&td=1831118&tu=u1831118&u=http%3A%2F%2Fwww.51hei.com%2Fbbs%2Fdpj-39846-1.html&urlid=0)f10x_it.c中，添加软件断点，一旦调试时出现Hard
 Fault则会在停在__breakpoint(0)处。
- 
- void HardFault_Handler(void)  
- {  
- 
-   if (CoreDebug->DHCSR & 1) {  //check C_DEBUGEN == 1 -> Debugger Connected  
-       __breakpoint(0);  // halt program execution here         
-   }  
-   while (1)  
-   {  
-   }  
- }  
当进入HardFault断点后，菜单栏Peripherals >Core Peripherals >FaultReports打开异常发生的报告，查看发生异常的原因。
![](http://c.51hei.com/a/a/c/510299111340007.jpg)
![](http://c.51hei.com/a/a/c/510299111356649.jpg)
上面的报告发生了BUS FAULT，并将Fault的中断服务转向Hard Fault。
相对于[检测](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=18&is_app=0&jk=70001d0e7379d299&k=%BC%EC%B2%E2&k0=%BC%EC%B2%E2&kdi0=0&luki=3&mcpm=0&n=10&p=baidu&q=98059059_cpr&rb=0&rs=1&seller_id=1&sid=99d27973e1d0070&ssp2=1&stid=9&t=tpclicked3_hc&td=1831118&tu=u1831118&u=http%3A%2F%2Fwww.51hei.com%2Fbbs%2Fdpj-39846-1.html&urlid=0)发生了什么异常，定位异常发生位置显得更重要。
（1）打开Call Stack窗口（如下图左侧，断点停在Hard Fault服务程序中）
![](http://c.51hei.com/a/a/c/510299111327828.jpg)
（2）在Call Stack的HardFault_Handler上右键Show CallerCode（有的Keil版本也可以直接双击）
![](http://c.51hei.com/a/a/c/510299111392073.jpg)
这时将跳转到发生异常的源代码位置（如上图），异常发生在p->hour=0这一行。这里错误很明显：指针p尚未为成员变量分配内存空间，直接访问未分配的内粗空间肯定出错。
再说明2点：
[1] 在复杂的情况下，即使[定位](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=18&is_app=0&jk=70001d0e7379d299&k=%B6%A8%CE%BB&k0=%B6%A8%CE%BB&kdi0=0&luki=1&mcpm=0&n=10&p=baidu&q=98059059_cpr&rb=0&rs=1&seller_id=1&sid=99d27973e1d0070&ssp2=1&stid=9&t=tpclicked3_hc&td=1831118&tu=u1831118&u=http%3A%2F%2Fwww.51hei.com%2Fbbs%2Fdpj-39846-1.html&urlid=0)了异常发生位置也很难容易的改正错误，要学会使用Watch窗口对发生错误的指针变量进行跟踪；
[2]在问题不明晰的情况下，尝试分析反汇编代码，就自己遇到的，部分情况下的异常发生在BL等跳转指令处，BL跳转到了不合法的内存地址产生异常
Refrences：
[1] Application
 Note209. Using Cortex-M3 and Cortex-M4 FaultExceptions. 
[2] Cortex-M3权威指南
