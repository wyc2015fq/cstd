# USB库STM32F0x2移植到STM32F070笔记 - xqhrs232的专栏 - CSDN博客
2017年09月13日 11:05:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：751
原文地址::[http://blog.csdn.net/flydream0/article/details/52074394](http://blog.csdn.net/flydream0/article/details/52074394)
相关文章
1、**STM32F0x2xx 全速USB设备库----[http://www.stmcu.org/document/detail/index/id-214961](http://www.stmcu.org/document/detail/index/id-214961)**
2、深入解析STM32_USB-FS-Device_Lib库----[https://wenku.baidu.com/view/96b9ab543b3567ec102d8afe.html](https://wenku.baidu.com/view/96b9ab543b3567ec102d8afe.html)
# 1. 前言
ST官方提供的USB库STM32F0x2_USB-FS-Device_LibV1.0.0 是基于标准库的，适用于STM32F0x2系列MCU，但是对于STM32F070来说，就需要稍作修改，本文就一直到STM32F070作一个笔记。
# 2. 移植
从STM中文官网上下载STM32F0x2 USB库，地址：[http://www.stmcu.org/document/detail/index/id-214961](http://www.stmcu.org/document/detail/index/id-214961)。用MDK打开，首先在Manager Project Items下的Project Targets下新增一项 “STM32F070”：
![](https://img-blog.csdn.net/20160730185806125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后切换到”STM32F070”这个Target： 。此后对所有工程属性的修改都会使用于“STM32F070”，而不再是原先的“USBD_HID-STM32072B-EVAL”了。
接下来修改device为STM32F070RB：
![](https://img-blog.csdn.net/20160730185847199?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
工程配置弄好了后，接下来我们来修改代码部分。
首先我们来编译一下工程，发现此时是可以编译通过的。但是烧录到STM32F070的板子里（这里使用ST的NUCLEO-F070RB板）去时却不能成功运行。
STM32F072与STM32F070这两个MCU都有USB，且此IP没有什么不同，那么差异是什么呢？
对比它俩的时钟树：
![](https://img-blog.csdn.net/20160730185921465?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如上图是STM32F072的时钟树，可知STM32F072是有一个内部48M的晶振，这个晶振是专门给USB提供时钟的。
![](https://img-blog.csdn.net/20160730190000611?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如上图是STM32F070的时钟树，对比STM32F072,发现STM32F070是没有那个48M内部晶振的，因此在给USB提供晶振时，需要使用到外部晶振，于是，在代码处找到设置晶振的代码进行修改：
usb_bsp.c 的USB_BSP_Init函数内:
**[cpp]**[view
 plain](http://blog.csdn.net/flydream0/article/details/52074394#)[copy](http://blog.csdn.net/flydream0/article/details/52074394#)
- RCC_HSEConfig(RCC_HSE_Bypass);  
- 
- /* Wait till HSE is ready */
- while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)  
-   {}  
- 
- /*Config the PREDIV for RCC_CFGR2*/
-   RCC_PREDIV1Config(RCC_PREDIV1_Div1);  
- /*HSE/PREDIV selected as PLL input clock*/
-   RCC_PLLConfig(RCC_PLLSource_PREDIV1,RCC_PLLMul_6);  
- /* Enable PLL */
-   RCC_PLLCmd(ENABLE);  
- 
- /* Wait till PLL is ready */
- while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)  
-   {}  
- /*use the PLLCLK as system input clock*/
-   RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);  
- /* Wait till PLL is used as system clock source */
- while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)  
-     {  
-     }  
-   RCC_HCLKConfig(RCC_SYSCLK_Div1);  
-   RCC_PCLKConfig(RCC_HCLK_Div1);  
- /* Configure USBCLK from PLL clock */
-   RCC_USBCLKConfig(RCC_USBCLK_PLLCLK);  
在usb_conf.h头文件中注释掉一些宏：
**[cpp]**[view
 plain](http://blog.csdn.net/flydream0/article/details/52074394#)[copy](http://blog.csdn.net/flydream0/article/details/52074394#)
- //#include "stm32072b_eval.h"
- ...  
- //#ifdef USE_STM32072B_EVAL
- /* When using STM32072B_EVAL board the internal pullup must be enabled */
-  #define INTERNAL_PULLUP
- //#endif
- ...  
- //#define USB_DEVICE_LOW_PWR_MGMT_SUPPORT   //关掉低功耗管理
- ...  
- //#define USB_CLOCK_SOURCE_CRS          //STM32F070下是没有CRS的
接下来整理一下systick:
**[cpp]**[view
 plain](http://blog.csdn.net/flydream0/article/details/52074394#)[copy](http://blog.csdn.net/flydream0/article/details/52074394#)
- void SysTick_Handler(void)  
- {  
- 
- #if 0
-     uint8_t buf[4] ={0,10,10,0};  
-     USBD_HID_SendReport (&USB_Device_dev,  
-                          buf,  
-                          4);  
- #endif
- //#if 0
- //  uint8_t *buf;
- //
- //  /* Get Joystick position */
- //  buf = USBD_HID_GetPos();
- //
- //  /* Update the cursor position */
- //  if((buf[1] != 0) ||(buf[2] != 0))
- //  {
- //    /* Send Report */
- //    USBD_HID_SendReport (&USB_Device_dev,
- //                         buf,
- //                         4);
- //  }
- //#endif
-   TimingDelay_Decrement();  
- }  
这个是延时函数：
**[cpp]**[view
 plain](http://blog.csdn.net/flydream0/article/details/52074394#)[copy](http://blog.csdn.net/flydream0/article/details/52074394#)
- void HAL_Delay(__IO uint32_t nTime)  
- {  
-   TimingDelay = nTime;  
- 
- while(TimingDelay != 0);  
- }  
- 
- /**
-   * @brief  Decrements the TimingDelay variable.
-   * @param  None
-   * @retval None
-   */
- void TimingDelay_Decrement(void)  
- {  
- if (TimingDelay != 0x00)  
-   {  
-     TimingDelay--;  
-   }  
- }  
修改下systick的间隔时间：
在usbd_usr.c文件中的：
**[cpp]**[view
 plain](http://blog.csdn.net/flydream0/article/details/52074394#)[copy](http://blog.csdn.net/flydream0/article/details/52074394#)
- void USBD_USR_Init(void)  
- {  
- /* SysTick used for periodic check mouse position */
-   SysTick_Config(SystemCoreClock /1000);  
- }  
最后在main函数内定时发送HID消息：
**[cpp]**[view
 plain](http://blog.csdn.net/flydream0/article/details/52074394#)[copy](http://blog.csdn.net/flydream0/article/details/52074394#)
- int main(void)  
- {  
-     uint8_t buf[4] ={0,10,10,0};  
- /*!< At this stage the microcontroller clock setting is already configured,
-        this is done through SystemInit() function which is called from startup
-        file (startup_stm32f072.s) before to branch to application main.
-        To reconfigure the default setting of SystemInit() function, refer to
-        system_stm32f0xx.c file
-       */
- 
- /* The Application layer has only to call USBD_Init to
-   initialize the USB low level driver, the USB device library, the USB clock
-   ,pins and interrupt service routine (BSP) to start the Library*/
- 
-   USBD_Init(&USB_Device_dev,  
-             &USR_desc,  
-             &USBD_HID_cb,  
-             &USR_cb);  
- 
- while (1)  
-   {  
- #if 1
-       USBD_HID_SendReport (&USB_Device_dev,  
-                          buf,  
-                          4);  
- //delay
-       HAL_Delay(1000);  
- #endif
-   }  
- }  
这样代码部分就完成了，通过以上main函数的代码可知，我们是每隔1S向PC端发送一次鼠标消息，鼠标会向右下角移动10个像素。
最后在NUCLEO板上测试OK！
最终移植后的工程下载地址：[http://download.csdn.net/detail/flydream0/9590631](http://download.csdn.net/detail/flydream0/9590631)

