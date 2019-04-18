# zigbee学习:示例程序SampleApp中按键工作流程 - weixin_33985507的博客 - CSDN博客
2013年06月29日 21:53:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
**zigbee学习:示例程序SampleApp中按键工作流程**
**本文博客链接:[http://blog.csdn.net/jdh99](http://blog.csdn.net/jdh99),作者:jdh,转载请注明.**
**环境：**
主机:WIN7
开发环境:IAR8.10.3
MCU:CC2530
示例程序:SampleApp
**按键电路:**
![](https://img-blog.csdn.net/20130629151324828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamRoOTk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
电路原理为利用ADC采集JOY_LEVEL脚电压,根据电压判断按键.
不同按键的电压值:
UP:0.1VCC
DN:0.38VCC
LT:0.54VCC
RT:0.60VCC
PUSH:0.7VCC
程序中利用8位分辨率,第一位为符号位,即7位分辨率分辨
程序中AD值表电压值
UP:2-380.02VCC-0.30VCC
DN:39-590.30VCC-0.46VCC
LT:60-730.47VCC-0.57VCC
RT:74-880.58VCC-0.69VCC
PUSH:89-1000.70VCC-0.78VCC
可见,程序检测与按键是能够匹配.
**代码结构:**
程序中按键检测机制为轮询机制,周期为100ms.
工作步骤:
1.main()函数中调用InitBoard()函数
2.此函数调用函数HalKeyConfig(HAL_KEY_INTERRUPT_DISABLE, OnBoard_KeyCallback)
3.此函数配置按键服务,如果是轮询机制,则调用函数osal_set_event(Hal_TaskID, HAL_KEY_EVENT),此函数启动按键事件,任务为Hal_TaskID
4.main()函数中调用硬件驱动初始化函数HalDriverInit()
5.此函数调用按键初始化函数HalKeyInit(),完成按键初始化
6.因为步骤3启动按键事件,所以进入Hal_TaskID处理函数Hal_ProcessEvent()
  此函数中判断是否按键事件,如果是则处理:
```cpp
HalKeyPoll();
    /* if interrupt disabled, do next polling */
    if (!Hal_KeyIntEnable)
    {
      osal_start_timerEx( Hal_TaskID, HAL_KEY_EVENT, 100);
    }
```
  首先轮询按键,然后启动定时器,100ms后再度轮询
7.轮询函数中调用halGetJoyKeyInput()函数,此函数调用读取ADC函数adc = HalAdcRead (HAL_KEY_JOY_CHN, HAL_ADC_RESOLUTION_8),然后根据ADC判断按键

