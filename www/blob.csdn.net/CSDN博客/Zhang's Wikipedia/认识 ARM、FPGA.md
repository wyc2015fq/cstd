
# 认识 ARM、FPGA - Zhang's Wikipedia - CSDN博客


2018年04月05日 20:33:36[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：173个人分类：[硬件](https://blog.csdn.net/lanchunhui/article/category/6307127)



## 0. ARM
ARM：Advanced RISC machine，微处理行业的一家知名企业。适用于多种领域，如嵌入控制，消费、教育类多媒体、DSP和移动式应用。
优势：
功耗低，不容易发热、死机；
32 位指令集；
生态丰富；
## 1. FPGA
Field－Programmable Gate Array，即现场可编程门阵列，通过逻辑编程可以内部电路连接，从而达到智能化的控制效果；
其程序不是通过执行指令，而是通过改变电路，
arm的编程指的是c语言或者汇编的编程，arm**一条条的读取语句，顺序实现其功能。fpga的编程则完全不同。fpga编程本身指的就是编辑其内部的电路结构。fpga运行代码也不是一条条执行的，而是读入代码之后就生成了逻辑门结构。这些逻辑门结构是并行运作的。给输入就同时产生输出**。所以fpga的编程语言叫做hdl（硬件描述语言）。hdl中的语句执行是不分先后的。


