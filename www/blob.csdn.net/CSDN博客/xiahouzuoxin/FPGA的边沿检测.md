# FPGA的边沿检测 - xiahouzuoxin - CSDN博客





2014年04月07日 21:58:25[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：8167









以前我曾一度傻乎乎的使用

always@(posedgesignal)


这样的代码去检测signal的上升沿，闹出了很多问题。


当受实验室的一同学指教后，再也不会傻乎乎的这样干了。当然，你看完下文也不会这样干了。


检测上升沿的原理：使用高频的时钟对信号进行采样，因此要实现上升沿检测，时钟频率至少要在信号最高频率的2倍以上，否则就可能出现漏检测。具体请参见下面代码。

moduleedge_check(clk,rst_n,signal,pos_edge,neg_edge,both_edge);inputclk;inputrst_n;inputsignal;outputpos_edge;outputneg_edge;outputboth_edge;regsig_r0,sig_r1;// 状态寄存器always@(posedgeclkornegedgerst_n)if(!rst_n)beginsig_r0<=1'b0;sig_r1<=1'b0;endelsebeginsig_r0<=signal;sig_r1<=sig_r0;endassignpos_edge=(~sig_r1)&(sig_r0);assignneg_edge=sig_r1&(~sig_r0);assignboth_edge=sig_r1^sig_r0;// 双边沿检测，或pos_edge|neg_edgeendmodule


使用Quartus II综合布线之后的RTL视图如下：

![check_regs](https://github.com/xiahouzuoxin/notes/raw/dev/images/FPGA%E7%9A%84%E8%BE%B9%E6%B2%BF%E6%A3%80%E6%B5%8B/check_regs.png)


从RTL视图中可以看出，电路是通过一个异步复位的D触发器实现的。


ModelSim的仿真视图如下，从中可看出已检测出上升和下降沿，但存在一个延时，这是因为使用了时钟同步的检测。

![modelsim1](https://github.com/xiahouzuoxin/notes/raw/dev/images/FPGA%E7%9A%84%E8%BE%B9%E6%B2%BF%E6%A3%80%E6%B5%8B/modelsim1.png)


或者上面的Verilog代码还可以换一种写法，效率上差不了太多；

moduleedge_check(clk,rst_n,signal,pos_edge,neg_edge,both_edge);inputclk;inputrst_n;inputsignal;outputpos_edge;outputneg_edge;outputboth_edge;reg[1:0]sig_fifo;regsig_r0,sig_r1;// 状态寄存器always@(posedgeclkornegedgerst_n)if(!rst_n)beginsig_fifo<=2'b0;endelsebeginsig_fifo<={sig_fifo[0],signal};endassignpos_edge=(sig_fifo==2'b01);assignneg_edge=(sig_fifo==2'b10);assignboth_edge=sig_fifo[0]^sig_fifo[1];// 双边沿检测，或pos_edge|neg_edgeendmodule


生成的RTL视图为

![check_fifo](https://github.com/xiahouzuoxin/notes/raw/dev/images/FPGA%E7%9A%84%E8%BE%B9%E6%B2%BF%E6%A3%80%E6%B5%8B/check_fifo.png)



