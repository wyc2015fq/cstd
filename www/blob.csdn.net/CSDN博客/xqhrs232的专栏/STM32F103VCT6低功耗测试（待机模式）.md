# STM32F103VCT6低功耗测试（待机模式） - xqhrs232的专栏 - CSDN博客
2016年07月01日 10:45:25[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1183
原文地址::[http://blog.csdn.net/wuhuijun165/article/details/49516893](http://blog.csdn.net/wuhuijun165/article/details/49516893)
相关文章
1、
## 终于我也可以对STM32低功耗进行总结
 ----[http://blog.sina.com.cn/s/blog_542bad910101sads.html](http://blog.sina.com.cn/s/blog_542bad910101sads.html)
只测试了待机模式，待机模式实现系统的最低功耗。 
原理图如下，一开始全部焊接了，其中S2用来进入待机，S1用来唤醒 
![这里写图片描述](https://img-blog.csdn.net/20151030203220328)
测试程序为：
<code class="hljs vala has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;"><span class="hljs-preprocessor" style="color: rgb(68, 68, 68); box-sizing: border-box;">#include "stm32f10x.h"</span>
<span class="hljs-preprocessor" style="color: rgb(68, 68, 68); box-sizing: border-box;">#include "system_stm32f10x.h"v</span>
oid Sys_Standby(<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">void</span>)
{  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">//使能PWR外设时钟</span>
    PWR_WakeUpPinCmd(ENABLE);  <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">//使能唤醒管脚功能</span>
    PWR_EnterSTANDBYMode();   <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">//进入待机（standby）模式          </span>
}
<span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">//系统进入待机模式</span>
<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">void</span> Sys_Enter_Standby(<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">void</span>)
{            
    RCC_APB2PeriphResetCmd(<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0X01FC</span>,DISABLE); <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">//复位所有IO口，屏蔽这条语句也没有看到什么影响</span>
    Sys_Standby();
}
<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">void</span> IO_Init(<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">void</span>)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">//PB6上拉输入，对应按键S2</span>
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
}
<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">int</span> main()
{
    IO_Init();
    <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">while</span>(<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>)
    {
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>)
        {
            Sys_Enter_Standby();
        }
    }
}</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li><li style="box-sizing: border-box; padding: 0px 5px;">11</li><li style="box-sizing: border-box; padding: 0px 5px;">12</li><li style="box-sizing: border-box; padding: 0px 5px;">13</li><li style="box-sizing: border-box; padding: 0px 5px;">14</li><li style="box-sizing: border-box; padding: 0px 5px;">15</li><li style="box-sizing: border-box; padding: 0px 5px;">16</li><li style="box-sizing: border-box; padding: 0px 5px;">17</li><li style="box-sizing: border-box; padding: 0px 5px;">18</li><li style="box-sizing: border-box; padding: 0px 5px;">19</li><li style="box-sizing: border-box; padding: 0px 5px;">20</li><li style="box-sizing: border-box; padding: 0px 5px;">21</li><li style="box-sizing: border-box; padding: 0px 5px;">22</li><li style="box-sizing: border-box; padding: 0px 5px;">23</li><li style="box-sizing: border-box; padding: 0px 5px;">24</li><li style="box-sizing: border-box; padding: 0px 5px;">25</li><li style="box-sizing: border-box; padding: 0px 5px;">26</li><li style="box-sizing: border-box; padding: 0px 5px;">27</li><li style="box-sizing: border-box; padding: 0px 5px;">28</li><li style="box-sizing: border-box; padding: 0px 5px;">29</li><li style="box-sizing: border-box; padding: 0px 5px;">30</li><li style="box-sizing: border-box; padding: 0px 5px;">31</li><li style="box-sizing: border-box; padding: 0px 5px;">32</li><li style="box-sizing: border-box; padding: 0px 5px;">33</li><li style="box-sizing: border-box; padding: 0px 5px;">34</li><li style="box-sizing: border-box; padding: 0px 5px;">35</li><li style="box-sizing: border-box; padding: 0px 5px;">36</li><li style="box-sizing: border-box; padding: 0px 5px;">37</li><li style="box-sizing: border-box; padding: 0px 5px;">38</li><li style="box-sizing: border-box; padding: 0px 5px;">39</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li><li style="box-sizing: border-box; padding: 0px 5px;">11</li><li style="box-sizing: border-box; padding: 0px 5px;">12</li><li style="box-sizing: border-box; padding: 0px 5px;">13</li><li style="box-sizing: border-box; padding: 0px 5px;">14</li><li style="box-sizing: border-box; padding: 0px 5px;">15</li><li style="box-sizing: border-box; padding: 0px 5px;">16</li><li style="box-sizing: border-box; padding: 0px 5px;">17</li><li style="box-sizing: border-box; padding: 0px 5px;">18</li><li style="box-sizing: border-box; padding: 0px 5px;">19</li><li style="box-sizing: border-box; padding: 0px 5px;">20</li><li style="box-sizing: border-box; padding: 0px 5px;">21</li><li style="box-sizing: border-box; padding: 0px 5px;">22</li><li style="box-sizing: border-box; padding: 0px 5px;">23</li><li style="box-sizing: border-box; padding: 0px 5px;">24</li><li style="box-sizing: border-box; padding: 0px 5px;">25</li><li style="box-sizing: border-box; padding: 0px 5px;">26</li><li style="box-sizing: border-box; padding: 0px 5px;">27</li><li style="box-sizing: border-box; padding: 0px 5px;">28</li><li style="box-sizing: border-box; padding: 0px 5px;">29</li><li style="box-sizing: border-box; padding: 0px 5px;">30</li><li style="box-sizing: border-box; padding: 0px 5px;">31</li><li style="box-sizing: border-box; padding: 0px 5px;">32</li><li style="box-sizing: border-box; padding: 0px 5px;">33</li><li style="box-sizing: border-box; padding: 0px 5px;">34</li><li style="box-sizing: border-box; padding: 0px 5px;">35</li><li style="box-sizing: border-box; padding: 0px 5px;">36</li><li style="box-sizing: border-box; padding: 0px 5px;">37</li><li style="box-sizing: border-box; padding: 0px 5px;">38</li><li style="box-sizing: border-box; padding: 0px 5px;">39</li></ul>
电流的测量用的是万用表，串联在电源的输入端，也就是说，实际测量的电流值为电路板消耗电流。电机、喇叭、OLED-0.9寸屏这些外部器件均未接入。 
系统时钟选择外部8M晶振，电源为电脑USB口取电，上电后按下S2，进入待机模式，按下S1唤醒。 
上电， 正常运行电流7.9mA，待机电流205uA，待机电流比较大； 
取下DS1302芯片，正常运行电流7.9mA，待机电流10.5uA； 
再取下DS1302芯片的三个上拉电阻，和上面一样，没变化；（看来即便有外部上拉，在待机模式时也是不用管的，只是不知道这上拉电阻接到了外围芯片上对外围电路的功耗有怎样的影响。） 
再取下AT24C02芯片模块，正常电流7.7mA，待机电流10.5uA; 
再取下L9910S电机驱动芯片，正常电流7.7mA，待机电流10.5uA，没有变化； 
再取下SK040G语音芯片，就剩电源和按键部分了，正常电流7.6mA，待机7.4uA。 
修改程序，开RTC后（选外部32.768k晶振），待机时电流为8.6uA。 
装上OLED 0.9寸小128x64液晶屏测试了下，待机时120uA，此时若取下液晶屏，电流由120uA变到正常待机的7.4uA。
待机模式可实现 STM32的最低功耗。该模式是在 CM3 深睡眠模式时关闭电压调节器，整个 1.8V 供电区域被断电，PLL、HSI和 HSE振荡器也被断电，SRAM和寄存器内容丢失，仅备份的寄存器和待机电路维持供电。
从待机模式唤醒后的代码执行等同于复位后的执行(采样启动模式引脚，读取复位向量等)，电源控制/状态寄存器(PWR_CSR)将会指示内核由待机状态退出。
待机模式下的输入/输出端口状态 
在待机模式下，所有的I/O引脚处于高阻态，除了以下的引脚： 
● 复位引脚(始终有效) 
● 当被设置为防侵入或校准输出时的TAMPER引脚 
● 被使能的唤醒引脚 
<既然进入待机模式后各IO处于高阻态，那么所谓的IO口进待机前需配置为AIN、或者弱上拉弱下拉模式的，其实都没必要了，但看其他的网络文章有说需配置的，我也是弱上拉、弱下拉、模拟输入、浮空输入都测试了下，对于最小系统，没看到待机电流有什么变化，也测试了下开串口、SPI口什么的，对待机电流都没有发现影响，文档上“进入待机模式后，只有备份的寄存器和待机电路维持供电，其他部分没有供电”，那自然不会产生功耗，进入待机模式前就没必要配置。对外围硬件电路进入待机前根据情况才看是否有设置外围芯片工作模式的必要。主芯片进入待机后，管脚都为高阻态，要看这种状态对外围芯片电路会带来怎样的影响，如果不合适就要考虑停止模式，在停止模式下，所有的I/O引脚都保持它们在运行模式时的状态。>
对于唤醒管脚PA0（WKUP），在寄存器PWR_CSR中的第8位EWUP位有说明： 
EWUP：使能WKUP引脚 
0： WKUP引脚为通用I/O。 WKUP引脚上的事件不能将CPU从待机模式唤醒 
1：WKUP引脚用于将CPU从待机模式唤醒，WKUP引脚被强置为输入下拉的配置(WKUP引脚上的上升沿将系统从待机模式唤醒) 
注：在系统复位时清除这一位。(即系统复位重启后该位为0) 
也就是说进待机模式后，WKUP自动被设置为下拉输入（下拉电阻典型值40K），无需额外配置端口A时钟及PA0管脚功能。
正常运行时IO口的损耗及响应配置： 
以下为转载[http://blog.csdn.net/beep_/article/details/47975227](http://blog.csdn.net/beep_/article/details/47975227)
I/O模块损耗： 
静态损耗： 
内部上下拉电阻损耗：这部分损耗主要取决于内部电阻的大小，一般为了降低内部电阻损耗常常需要降低电阻两端电压，若引脚为低电压则采用下拉电阻，若引脚为高电压则采用上拉电阻。 
I/O额外损耗：当引脚设为输入I/O时，用来区分电压高低的斯密特触发器电路会产生一部分消耗，为此可将引脚设为模拟输入模式。 
动态损耗：对于悬浮的引脚，由于其电压不稳定会产生外部电磁干扰和损耗，因此必须把悬浮引脚设为模拟模式或输出模式。 
引脚电压的切换会对外部和内部电容负载产生动态损耗，其损耗与电压切换频率和负载电容有关。具体损耗值如下： 
![这里写图片描述](https://img-blog.csdn.net/20151031125902789)
