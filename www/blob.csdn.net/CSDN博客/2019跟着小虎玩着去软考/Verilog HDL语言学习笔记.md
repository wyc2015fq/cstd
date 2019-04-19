# Verilog HDL语言学习笔记 - 2019跟着小虎玩着去软考 - CSDN博客
2010年04月28日 18:28:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：6864
**Verilog HDL****语言**
## 一．Verilog是什么？　
Verilog HDL是一种硬件描述语言（HDL:Hardware Discription Language），是一种以文本形式来描述数字系统硬件的结构和行为的语言。
## 二．Verilog的用途是什么？　
用它可以表示逻辑电路图、逻辑表达式，还可以表示数字逻辑系统所完成的逻辑功能。
## 三．Verilog是哪个公司开发的产品？
是由Gateway Design Automation公司（该公司于1989年被Cadence公司收购）开发。
## 三．最流行的两种硬件描述语言是什么？
Verilog HDL和VHDL是目前世界上最流行的两种硬件描述语言，都是在20世纪80年代中期开发出来的。两种HDL均为IEEE标准。
## 三．Verilog HDL的简介：
Verilog HDL就是在用途最广泛的C语言的基础上发展起来的一种件描述语言，它是由GDA(Gateway Design Automation)公司的PhilMoorby在1983年末首创的，最初只设计了一个仿真与验证工具，之后又陆续开发了相关的故障模拟与时序分析工具。1985年Moorby推出它的第三个商用仿真器Verilog-XL,获得了巨大的成功，从而使得Verilog HDL迅速得到推广应用。1989年CADENCE公司收购了GDA公司，使得VerilogHDL成为了该公司的独家专利。1990年CADENCE公司公开发表了Verilog HDL,并成立LVI组织以促进Verilog HDL成为IEEE标准，即IEEE Standard 1364-1995. 
## 四．Verilog HDL的最大特点是什么？
Verilog HDL的最大特点就是易学易用，如果有C语言的编程经验，可以在一个较短的时间内很快的学习和掌握，因而可以把Verilog HDL内容安排在与ASIC设计等相关课程内部进行讲授，由于HDL语言本身是专门面向硬件与系统设计的，这样的安排可以使学习者同时获得设计实际电路的经验。与之相比，VHDL的学习要困难一些。但Verilog HDL较自由的语法，也容易造成初学者犯一些错误，这一点要注意。
## 五．选择VHDL还是verilog HDL？
　　这是一个初学者最常见的问题。其实两种语言的差别并不大，他们的描述能力也是类似的。掌握其中一种语言以后，可以通过短期的学习，较快的学会另一种语言。选择何种语言主要还是看周围人群的使用习惯，这样可以方便日后的学习交流。当然，如果您是集成电路（ASIC）设计人员，则必须首先掌握verilog，因为在IC设计领域，90％以上的公司都是采用verilog进行IC设计。对于PLD/FPGA设计者而言，两种语言可以自由选择。
　　设计人员通过计算机对HDL语言进行逻辑仿真和逻辑综合，方便高效地设计数字电路及其产品。
　　常用的Verilog HDL语言开发软件有Altera公司的MAX+PLUS II，Quartus II和Xilinx公司的Foundation ISE。
## 六．Verlog HDL的发展历史
　任何新生事物的产生都有它的历史沿革，早期的硬件描述语言是以一种高级语言为基础，加上一些特殊的约定而产生的，目的是为了实现RTL级仿真，用以验证设计的正确性，而不必像在传统的手工设计过程中那样，必须等到完成样机后才能进行实测和调试。　
1、1981年Gateway Automation(GDA)硬件描述语言公司成立。
2、1983年该公司的Philip Moorby首创了Verilog HDL，Moorby后来成为Verrlog HDL-XL的主要设计者和Cadence公司的第一合伙人。
3、1984-1985年Moorby设计出第一个关于Verilog HDL的仿真器。
4、1986年Moorby对Verilog HDL的发展又做出另一个巨大的贡献，提出了用于快速门级仿真的XL算法。
5、随着Verilog HDL-XL的成功，Verilog HDL语言得到迅速发展。
6、1987年Synonsys公司开始使用Verilog HDL行为语言作为综合工具的输入。
7、1989年Cadence公司收购了Gateway公司，Verilog HDL成为Cadence公司的私有财产。
8、1990年初Cadence公司把Verilong HDL和Verilong HDL-XL分开，并公开发布了Verilog HDL.随后成立的OVI（Open Verilog HDL International）组织负责Verilog HDL的发展，OVI由Verilog HDL的使用和CAE供应商组成，制定标准。
9、1993年，几乎所有ASIC厂商都开始支持Verilog HDL，并且认为Verilog HDL-XL是最好的仿真器。同时，OVI推出2.0版本的Verilong HDL规范，IEEE接收将OVI的Verilong HDL2.0作为IEEE标准的提案。
10、1995年12月，IEEE制定了Verilong HDL的标准IEEE1364-1995. 
## 七．一个简单的VerilogHDL的例子：(12位寄存器）
// Verilog Example
// User-Defined Macrofunction
module reg12 ( d, clk, q);
define size 11
input [`size:0]d;
input clk;
output [`size:0]q;
reg [`size:0]q;
always @(posedge clk)
q = d;
endmodule
