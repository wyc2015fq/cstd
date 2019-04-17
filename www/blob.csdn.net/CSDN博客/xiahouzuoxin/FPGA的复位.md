# FPGA的复位 - xiahouzuoxin - CSDN博客





2014年04月06日 19:35:55[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：15887
个人分类：[FPGA](https://blog.csdn.net/xiahouzuoxin/article/category/1910479)









### 关于FGPA的复位


当初开始学FPGA的时候，总是疑惑：FPGA不是没有复位管教么，但总在always看到有复位信号。这个复位信号（我们暂且称为rst_n）从哪里来？


实际上是可以从两个方面获得的，这与我们的MCU一样。
- 上电自动复位
- 手动按键复位


考虑到系统的初始化可能需要一定的时间，需要写一段Verilog代码进行延时复位，这段代码综合后就是上电自动复位的过程，上电自动复位也要外部硬件提供一个低电平脉冲，第二种方法要求有按键复位的按键电路。作为一个正常的系统，上电自动复位和手动的按键复位都是必须的，且两者实际上是不可分割的。

### [](https://github.com/xiahouzuoxin/notes/blob/dev/essays/FPGA%E7%9A%84%E5%A4%8D%E4%BD%8D.md#%E4%B8%8A%E7%94%B5%E8%87%AA%E5%8A%A8%E5%A4%8D%E4%BD%8D)上电自动复位


原理上很简单，写一个复位模块，等待一段稳定时间，将复位信号拉低一段足够长的时间，再将复位信号拉高。


如下Verilog源码，外部按键复位也将作为模块的一个引脚输入，用于异步的全局复位操作，正常的复位操作要进行，必须要求外部有一个短暂的脉冲作用在rst_n信号上，这可以通过按键电路中的RC电路实现。

/***************************************  功能：上电复位模块*  输入参数：*         clk： 50M 时钟输入*         rst_n：外部按键全局复位信号*  输出参数：*         sys_rst_n:系统全局同步复位信号***************************************/modulereset(inputclk,inputrst_n,outputsys_rst_n);//------------------------------------------// Delay 100ms for steady statereg[22:0]cnt;always@(posedgeclkornegedgerst_n)beginif(!rst_n)cnt<=0;elsebeginif(cnt<23_00000)//100mscnt<=cnt+1'b1;elsecnt<=cnt;endend//------------------------------------------//rst_n synchronismregrst_nr0;regrst_nr1;always@(posedgeclkornegedgerst_n)beginif(!rst_n)beginrst_nr0<=0;rst_nr1<=0;endelseif(cnt==23_00000)beginrst_nr0<=1;rst_nr1<=rst_nr0;endelsebeginrst_nr0<=0;rst_nr1<=0;endendassignsys_rst_n=rst_nr1;endmodule

### [](https://github.com/xiahouzuoxin/notes/blob/dev/essays/FPGA%E7%9A%84%E5%A4%8D%E4%BD%8D.md#%E6%8C%89%E9%94%AE%E6%89%8B%E5%8A%A8%E5%A4%8D%E4%BD%8D%E7%94%B5%E8%B7%AF)按键手动复位电路


不使用专用芯片的参考低电平复位电路如下：

![reset1](https://github.com/xiahouzuoxin/notes/raw/dev/images/FPGA%E7%9A%84%E5%A4%8D%E4%BD%8D/reset1.png)


电路中的复位管脚一端连接到FPGA的某个普通通用管脚，这样电路中的RC电路将产生上面Verilog代码中的rst_n上电低脉冲，这就是本文开头说自动上电复位和硬件按键复位相辅相成。


请注意两个电阻的值，R21要是R22的两个数量级以上，这样才能保证按键按下后被识别为低电平。


手动复位过程中为保证按键复位的稳定性，还可以修改上面的Verilog代码进行按键消抖检测。下面是抓到的按键在闭合的时候的波形：

![key_bounce](https://github.com/xiahouzuoxin/notes/raw/dev/images/FPGA%E7%9A%84%E5%A4%8D%E4%BD%8D/key_bounce.png)


按键在几个us之内就能达到低电平，该期间触点抖动比较严重。

moduleRMV_BJ(BJ_CLK,//采集时钟，40HzRESET,//系统复位信号BUTTON_IN,//按键输入信号BUTTON_OUT//消抖后的输出信号);inputB_CLK;inputRESET;inputBUTTON_IN;outputBUTTON_OUT;regBUTTON_IN_Q,BUTTON_IN_2Q,BUTTON_IN_3Q;always@(posedgeBJ_CLKornegedgeRESET)beginif(~RESET)beginBUTTON_IN_Q<=1'b1;BUTTON_IN_2Q<=1'b1;BUTTON_IN_3Q<=1'b1;endelsebeginBUTTON_IN_Q<=BUTTON_IN;BUTTON_IN_2Q<=BUTTON_IN_Q;BUTTON_IN_3Q<=BUTTON_IN_2Q;endendwireBUTTON_OUT=BUTTON_IN_2Q|BUTTON_IN_3Q;endmodule


除了上面简单的复位电路，还可使用CAT811/TPS3823-33等专门的复位芯片，可以免去按键按键消抖的操作。



