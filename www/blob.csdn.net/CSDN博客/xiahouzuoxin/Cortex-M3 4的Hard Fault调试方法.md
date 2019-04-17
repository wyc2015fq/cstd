# Cortex-M3/4的Hard Fault调试方法 - xiahouzuoxin - CSDN博客





2013年08月19日 15:00:38[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：26815








# 1 Cortex-M3/4的Fault简介

        Cortex-M3/4的Fault异常是由于非法的存储器访问（比如访问0地址、写只读存储位置等）和非法的程序行为（比如除以0等）等造成的。常见的4种异常及产生异常的情况如下：

**Bus Fault：**在fetch指令、数据读写、fetch中断向量或中断时存储恢复寄存器栈情况下，检测到内存访问错误则产生Bus Fault。

**Memory Management Fault：**访问了内存管理单元（MPU）定义的不合法的内存区域，比如向只读区域写入数据。

**Usage Fault：**检测到未定义指令或在存取内存时有未对齐。还可以通过软件配置是否检测到除0和其它未对齐内存访问也产生该异常，默认关闭，需要在工程初始化时配置：



```cpp
SCB->CCR |= 0x18; // enable div-by-0 and unaligned fault
```



**Hard Fault：**在调试程序过程中，这种异常最常见。上面三种异常发生任何一种异常都会引起Hard Fault，在上面的三种异常未使能的情况下，默认发生异常时进入Hard Fault中断服务程序。使能前三种异常也要在初始化时配置：



```cpp
SCB->SHCSR |= 0x00007000;   // enable Usage Fault, Bus Fault, and MMU Fault
```




在默认复位初始化时，Hard Fault使能，其它三者不使能，因此当程序中出现不合法内存访问（一般是指针错误引起）或非法的程序行为（一般就是数学里面常见的除0）时都将产生Hard Fault中断。




# 2 Hard Fault调试方法

假设IDE环境为Keil，芯片为STM32F103。

在stm32f10x_it.c中，添加软件断点，一旦调试时出现Hard Fault则会在停在__breakpoint(0)处。



```cpp
/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  if (CoreDebug->DHCSR & 1) {  //check C_DEBUGEN == 1 -> Debugger Connected
      __breakpoint(0);  // halt program execution here	     
  }
  while (1)
  {
  }
}
```





当进入Hard Fault断点后，菜单栏Peripherals >Core Peripherals >Fault Reports打开异常发生的报告，查看发生异常的原因。

![](https://img-blog.csdn.net/20130819142641171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20130819145437359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
上面的报告发生了BUS FAULT，并将Fault的中断服务转向Hard Fault。




相对于检测发生了什么异常，定位异常发生位置显得更重要。

（1）打开Call Stack窗口（如下图左侧，断点停在Hard Fault服务程序中）

![](https://img-blog.csdn.net/20130819144530171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

（2）在Call Stack的HardFault_Handler上右键Show Caller Code（有的Keil版本也可以直接双击）
![](https://img-blog.csdn.net/20130819144905625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


这时将跳转到发生异常的源代码位置（如上图），异常发生在p->hour=0这一行。这里错误很明显：指针p尚未为成员变量分配内存空间，直接访问未分配的内粗空间肯定出错。


再说明2点：

[1] 在复杂的情况下，即使定位了异常发生位置也很难容易的改正错误，要学会使用Watch窗口对发生错误的指针变量进行跟踪；

[2] 在问题不明晰的情况下，尝试分析反汇编代码，就自己遇到的，部分情况下的异常发生在BL等跳转指令处，BL跳转到了不合法的内存地址产生异常




**Refrences：**

**[1] **Application Note 209. Using Cortex-M3 and Cortex-M4 Fault Exceptions. 


**[2]** Cortex-M3权威指南



