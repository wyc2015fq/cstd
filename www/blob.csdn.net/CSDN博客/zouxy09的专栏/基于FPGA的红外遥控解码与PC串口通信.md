
# 基于FPGA的红外遥控解码与PC串口通信 - zouxy09的专栏 - CSDN博客


2013年06月20日 22:04:26[zouxy09](https://me.csdn.net/zouxy09)阅读数：11252


**基于FPGA的红外遥控解码与PC串口通信**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
这是我的《电子设计EDA》的课程设计作业（呵呵，这个月都拿来做大作业了，各种大作业，能发上来和大家分享的我会发上来，否则博客太冷清了）。之前没有学过FPGA，但要掌握基础的Verilog编程也不是很困难。不过altium公司的NanoBoard2开发板的确也不敢恭维啊，提供的资料不够详细。所以搞定这个东西也花了我一周的时间。需要整个FPGA端和PC端的工程的可以发邮件给我。
**一、设计目标**
本文设计实现一个基于FPGA的红外遥控解码和实现FPGA与PC机的串口通信。
**二、系统总体框架**
本系统综合FPGA和PC平台，总体功能框架如图2-1所示：
![](https://img-blog.csdn.net/20130620215311703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图2-1：系统功能组成图
系统主要包括四大模块：每个模块对应的作用分别如下：
1）**红外遥控器解码模块：**
主要是通过红外接收头接收红外遥控器发送的红外波形，通过FPGA对其进行解调和解码，得到遥控器每个按键对应的命令码。
2）**FPGA串口发送模块：**
FPGA解码获得遥控器每个按键对应的命令码后，需要将其发送到PC机上，以控制PC机上的多媒体播放器。
3）**PC串口接收模块：**
PC端需要控制串口，以获取串口传入的数据，也即是遥控器对应的命令码。得到该码后，需要将其映射到PC机系统的按键事件，以控制PC机上的软件（这里是用XBMC多媒体播放器来演示）。
4）**多媒体演示模块：**
采用现有的XBMC多媒体中心来实现。XBMC是一个跨平台的媒体娱乐中心软件（[http://xbmc.org/download/](http://xbmc.org/download/)），它可以播放音视频、浏览图片、查看天气、管理存储器上的媒体资源文件、系统设置、支持游戏手柄等功能。
下面对每一个模块的设计细节进行详细的介绍。
**三、关键模块设计细节**
**3.1、红外遥控器解码模块**
本模块的功能主要是通过红外接收头接收红外遥控器发送的红外波形，通过FPGA对其进行解调和解码，得到遥控器每个按键对应的命令码。
**（1）红外遥控基本知识**
通用红外遥控系统由发射和接收两大部分组成。应用编/解码专用集成电路芯片来进行控制操作，如图3-1所示。发射部分包括键盘矩阵、编码调制（增强发射效率）、LED红外发送器；接收部分包括光、电转换放大器、解调、解码电路。
![](https://img-blog.csdn.net/20130620215434156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图3-1：通用红外遥控系统
遥控发射器专用芯片很多，也存在很多编码协议。NanoBoard2的遥控器采用的是广泛的NEC协议，这里简述下该协议。
该协议采用脉宽调制的串行码，以脉宽为0.565ms、间隔0.56ms、周期为1.125ms的组合表示二进制的“0”；以脉宽为0.565ms、间隔1.685ms、周期为2.25ms的组合表示二进制的“1”，其波形如图3-2所示：
![](https://img-blog.csdn.net/20130620215528687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图3-2：0与1的波形
当我们按下遥控器的按键时，遥控器将发出一串二进制代码，我们称它为一帧数据。可将它们分为5 部分，分别为引导码、用户码、用户反码、数据码和数据反码，共32bit。如图3-3所示：
![](https://img-blog.csdn.net/20130620215557000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图3-3：一帧红外数据
遥控器发射代码时，均是低位在前。高位在后。其中引导码高电平为9ms，低电平为4.5ms，当接收到此码时，表示一帧数据的开始。解码的关键是如何识别"0"和"1"，从位的定义我们可以发现"0"和"1"均以0.565ms的低电平开始，不同的是高电平的宽度不同，"0"为0.56ms，"1"为1.68ms，所以必须根据高电平的宽度区别"0"和"1"。
**（2）本系统遥控解码**
NanoBoard2的遥控器采用的是广泛的NEC协议。在NanoBoard2的外设板PB03提供了红外收发器（infrared transceiver）。在Altium Designer的库中也提供了该红外收发器的编程控制接口（FPGA PB03 Port-Plugin.IntLib中）。如图3-4所示。
![](https://img-blog.csdn.net/20130620215633156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图3-4：NB2板上红外资源
但该收发器采用的是Vishay半导体公司的 TFDU6102收发芯片，其中并不像一般的红外接收模块一样，这款芯片不提供红外解调功能。所以我们通过Altium Designer提供的Wishbone的红外解调解码IP核来实现解调和解码功能。该IP核在Altium Designer的FPGA Peripherals (Wishbone).IntLib中，其引脚如图3-5所示：
![](https://img-blog.csdn.net/20130620215656000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图3-5：WB_IRDEC引脚图
各引脚的功能简述如下：
左边的部分是红外的控制信号。右边的部分是遵守Wishbone规范的从设备接口，具体说明如下：
CLK_I：外部输入时钟信号。
RST_I：同步复位信号，高电平有效。
DAT_O()/DAT_I()：主设备和从设备的之间的数据信号，数据可以由主设备传送给从设备，也可以由从设备传送给主设备。一对主设备和从设备之间最多存在两条数据总线，一条用于主设备向从设备传输数据，另外一条用于从设备向主设备传输数据。
ADR_I[3:0]：地址信号，主设备输出地址到从设备。
ACK_O：主从设备间的操作结束方式信号。ACK为高表示成功。操作总是在某一总线周期内完成的，因此操作结束方式也称为总线周期结束方式。
CYC_I：总线周期信号CYC_I有效代表一个主设备请求总线使用权或者正在占有总线，但是不一定正在进行总线操作（是否正在进行总线操作取决于选通信号STB_I是否有效）。只有该信号有效，Wishbone主设备和从设备接口的其它信号才有意义。
STB_I：选通信号。选通有效代表主设备发起一次总线操作。
WE_I：写使能信号，代表当前周期中进行的操作是写操作还是读操作。1代表写，0代表读。
这样我们就可以通过FPGA去控制WB_IRDEC来获得解码后的32bit遥控码。WB_IRDEC提供了四个寄存器：Clock Divider Register (CLK_DIV)、Control Register (CTRL)、Status Register (STATUS)和Data Register (DATA)。我们需要读写这四个寄存器来配置WB_IRDEC和读取红外码。
WB_IRDEC采用的是标准的Wishbone数据传输握手协议。我们将该协议总结如下：
**（****a****）单次读操作：**
在时钟上升沿i，主设备将地址信号ADR_O()放到总线上，将WE_O置为低表示读操作，将CYC_O置高表示操作正在进行，将STB_O置高表示操作开始。
在时钟上升沿i+1到达之前，从设备检测到主设备发起的操作，将适当的数据放到主设备的输入信号DAT_I()，将主设备的ACK_I置高作为对主设备STB_O的响应。
在时钟上升沿i+1，主设备发现ACK_I信号为高，将DAT_I()采样，并将STB_O和CYC_O置为低表示操作完成。从设备发现STB_O置低后，也将主设备的输入信号ACK_I置低。
**（****b****）单次写操作：**
在时钟上升沿i，主设备将地址信号ADR_O()放到总线上，将数据信号DAT_O()放到总线上，将WE_O置高表示写操作，将CYC_O置高表示操作正在进行，将STB_O置高表示操作开始。
在时钟上升沿i+1到达之前，从设备检测到主设备发起的操作，将主设备的ACK_I置高作为对主设备STB_O的响应。
在时钟上升沿i+1，从设备将DAT_I()采样；主设备发现ACK_I信号为高，将STB_O和CYC_O置为低表示操作完成；从设备发现STB_O置低后，也将主设备的ACK_I置低。
具体的寄存器配置和读写参考altium提供的wiki：WB_IRRC - Accessible Internal Registers。另外，该说明文档里面时钟分频寄存器Clock Divider Register (CLK_DIV)的设置值的公式存在错误。因为该寄存器的值提供解调红外波形的载波频率。所以这个值的设置必须准确，否则无法解调。所以这个错误使得调试过程中耗费了很长时间。后来通过对原理的理解，自己推导到了正确的公式。该IP核采用的时钟分频是通过24bit相位累加器来实现的。它在wiki上提供的确定分频该寄存器的值的公式是：
CLK_DIV = (fcarrier* 8000000h) / fCLK_I
这里的8000000h应该是1000000h（24bit也就是224=1000000h）才正确。
我们设计的原理图如图3-6所示：
![](https://img-blog.csdn.net/20130620215748531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图3-6：红外解码模块原理图
我们的FPGA控制模块在IrDA.v文件中实现。我们采用的是verilog硬件描述语言。具体的verilog代码见附录。
**3.2、FPGA串口发送模块**
该模块的功能是在FPGA解码获得遥控器每个按键对应的命令码后，将其发送到PC机上。
**（1）串口基础知识**
串口通信是指使用一条数据线（另外需要地线，可能还需要控制线），将数据一位一位地依次传输，每一位数据占据一个固定的时间长度。其只需要少数几条线就可以在系统间交换信息，特别使用于计算机与计算机、计算机与外设之间的远距离通信。使用串口通信时，发送和接收到的每一个字符实际上都是一次一位的传送的，每一位为1或者为0。如图3-7所示。
![](https://img-blog.csdn.net/20130620215816921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图3-7：串行通信原理
串行异步通信即RS232通信，是主机与外部硬件设备的常用通讯方式。可以双向传输。其数据格式如图3-8所示。
![](https://img-blog.csdn.net/20130620215845781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图3-8：异步通信的数据格式
**（2）本系统串行通信发送模块设计**
NanoBoard2的主板提供了RS232的串行接口。在Altium Designer的库中也提供了该接口的编程控制接口（FPGA NB2DSK01 Port-Plugin.IntLib中）。如图3-9所示：
![](https://img-blog.csdn.net/20130620215913781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图3-9：板上RS232的串行接口
在verilog代码中，我们需要根据我们选择的串口的波特率来对输入时钟进行分频得到串行通信的每一个数据位所需要的时间。然后按照图3-8所示的串行通信的数据格式将8bit的数据发送出去。闲时我们将串口发射的信号线UART_TX_O一直保持在高。当我们需要发送数据时，将其拉低并保持一个数据位的时间，再将8bit的数据从低位到高位一位一位的通过发送出去。然后再发送一个停止位。这里我们不添加校验位。这时候一个8bit的数据就成功通过串口发送出去了。
我们设计的原理图如图3-10所示：
![](https://img-blog.csdn.net/20130620215941859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图3-10：串口发送模块原理图
我们的FPGA控制模块在UART_TX.v文件中实现。然后在IrDA.v文件中实例化。这样就把串口发送模块嵌入到红外解码模块中。具体的verilog代码见附录。
**3.3、PC串口接收和键盘映射模块**
PC端需要控制串口，以获取串口传入的数据，也即是遥控器对应的命令码。得到该码后，需要将其映射到PC机系统的按键事件，以控制PC机上的软件（这里是用XBMC多媒体播放器来演示）。
**（1）串口接收**
在Windows系统中，串口和其它通信设备是作为文件处理的。串口的打开、关闭、读取和写入所用的函数与操作文件的函数完全一致。我们通过CreateFile()函数“打开”串口。
CreateFile函数创建或打开以下对象并返回一个句柄,可以用来访问对象:文件、管道、mailslots、通信资源、磁盘设备(Windows NT只有)、consoles等。
HANDLE CreateFile(
LPCTSTR lpFileName, //指向文件名的指针
DWORD dwDesiredAccess, //访问模式（写/读）
DWORD dwShareMode, //共享模式
LPSECURITY_ATTRIBUTES lpSecurityAttributes, //指向安全属性的指针
DWORD dwCreationDisposition, //如何创建
DWORD dwFlagsAndAttributes, //文件属性
HANDLE hTemplateFile //用于复制文件句柄
);
打开串口之后，我们再通过ReadFile()函数从文件指针指向的位置开始将数据读出到来。
BOOL ReadFile(
HANDLE hFile, //文件的句柄
LPVOID lpBuffer, //用于保存读入数据的一个缓冲区
DWORD nNumberOfBytesToRead, //要读入的字节数
LPDWORD lpNumberOfBytesRead, //指向实际读取字节数的指针
LPOVERLAPPED lpOverlapped //该结构定义了一次异步读取操作
);
**（2）键盘映射模块**
通过串口获得每个遥控按键对应的8bit，也就是一个字节的码后。我们需要将其与遥控器具体的按键功能对应。例如Altium的遥控器的“向左”这个按键的红外指令码是0x47。当我们接收到这个码后，我们就映射到PC机键盘上面的“向左”的键盘值。
我们用Windows提供了Keybd_event这个函数来模拟键盘。通过调用它来产生“向左”这个按键的键值。这个键值会发送到系统上，然后当前的活动程序将会捕捉到这个按键，然后达到控制软件的目的。
Keybd_event能触发一个按键事件，Keybd_event共有四个参数，第一个为按键的虚拟键值，如回车键为vk_return。第二个参数为扫描码，一般不用设置，用0代替就行。第三个参数为选项标志，如果为keydown则置0即可，如果为keyup则设成“KEYEVENTF_KEYUP”，第四个参数一般也是置0即可。下面两行代码就实现了模拟键盘“向左”的按键按下然后放开的效果：
keybd_event(VK_LEFT, 0, 0, 0);              // key down
keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP,0); // key up
本系统中，这两部分的代码采用C++来编写。具体源码见附录。
**3.4、多媒体演示模块**
该模块的作用是演示我们对遥控器解码的有效性。采用现有的XBMC多媒体中心来实现。XBMC是一个跨平台的媒体娱乐中心软件，它可以播放音视频、浏览图片、查看天气、管理存储器上的媒体资源文件、系统设置、支持游戏手柄等功能。其效果图见图3-11。其官网：http://xbmc.org/提供了下载和更多支持。
![](https://img-blog.csdn.net/20130620220037140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图3-11：XBMC多媒体软件
**四、附录**
**4.1、FPGA端**
**IrDA_v.v**
`//==============================================================================
// File Name   : IrDA_v.v
// Module Name : This is IrDA control module
// Description : Receive the IrDA code, then decode it and transmit the code to PC though COM
// Author      : Zou Xiaoyi
// HomePage    : http://blog.csdn.net/zouxy09
// Date        : 2013/06/08
// Rev.        : 0.1
//==============================================================================
module IrDA_WB(CLK_I, ACK_I, INT_I, DATA_I, reset, CYC_STB_O, WE_O, ADR_O, DATA_O, UART_TX_O, Ir_Commad);
    input CLK_I;
    input ACK_I;
    input INT_I;
    input [31:0] DATA_I;
    input reset;
    output CYC_STB_O;
    output WE_O;
    output [3:0] ADR_O;
    output [31:0] DATA_O;
    output UART_TX_O;
    output [7:0] Ir_Commad;
    reg ACK_I;
    reg INT_I;
    reg [31:0] DATA_I;
    reg CYC_STB_O;
    reg WE_O;
    reg [3:0] ADR_O;
    reg [31:0] DATA_O;
    reg [7:0] Ir_Commad;
    reg [7:0] data;
    reg [31:0] count;
    reg [31:0] count2;
    reg flag;
    // The whole process of IrDA decoder
    always @(posedge CLK_I)
        begin
             // count posedge of clk
            if (reset)  // initialize
               begin
                    count = 32'd0;
                    count2 = 32'd0;
                    flag = 1'b0;
                    CYC_STB_O = 1'b0;
                    START = 1'b0;
               end
            // 1) initialize
            // http://wiki.altium.com/display/ADOH/WB_IRRC+-+Accessible+Internal+Registers
            case (count)
            32'd0:
                begin
                    Ir_Commad = 8'b0000_1111;  // initialize state
                    count = 32'd1;
                end
            32'd1:
                begin
                    // write Clock Divider Register (CLK_DIV) Register
                    ADR_O = 4'h0;
                    DATA_O[31:0] = 32'h0000_7D3E;   //  32'h0000_3219 for 50MHz   32'h0000_7D3E for 20MHz
                    WE_O = 1'b1;
                    CYC_STB_O = 1'b1;
                    count = 32'd2;
                end
            32'd2:
                begin
                    if (ACK_I)  // waiting for ack
                       begin
                            CYC_STB_O = 1'b0;
                            count = 32'd3;
                       end
                    else
                         count = 32'd2;
                end
            32'd3:
                begin
                    // write Control Register
                    ADR_O = 4'h1;
                    DATA_O[31:0] = 32'h0000_0043;   // 43
                    WE_O = 1'b1;
                    CYC_STB_O = 1'b1;
                    count = 32'd4;
                end
            32'd4:
                begin
                    if (ACK_I)  // waiting for ack
                       begin
                            CYC_STB_O = 1'b0;
                            count = 32'd5;
                            count2 = 32'd0;
                       end
                    else
                        count = 32'd4;
                end
            default:
                begin
                     // make initialize strage run once
                     count = 32'd10;
                    // 2) if it has valid data, get it!
                    if (INT_I)  // haveData
                       begin
                            // 2.1) we need to read it from data register
                            case (count2)
                            32'd0:
                                begin
                                    // read Data Register
                                    ADR_O = 4'h3;
                                    WE_O = 1'b0;
                                    CYC_STB_O = 1'b1;
                                    count2 = 32'd1;
                                end
                            32'd1:
                                begin
                                    if (ACK_I)  // waiting for ack
                                        begin
                                             flag = 1'b1;
                                            data = DATA_I[23:16];
                                            CYC_STB_O = 1'b0;
                                            count2 = 32'd2;
                                            //START = 1'b1;   // start to transmit though COM
                                        end
                                    else
                                        count2 = 32'd1;
                                end
                            32'd2:
                                begin
                                    // 2.2) write Control Register to clear bit of data flag
                                    ADR_O = 4'h1;
                                    DATA_O[31:0] = 32'h0000_0043;  //43
                                    WE_O = 1'b1;
                                    CYC_STB_O = 1'b1;
                                    count2 = 32'd3;
                                end
                            32'd3:
                                begin
                                    if (ACK_I)  // waiting for ack
                                       begin
                                           CYC_STB_O = 1'b0;
                                           count2 = 32'd0;
                                           START = 1'b1;   // start to transmit though COM
                                       end
                                    else
                                        count2 = 32'd3;
                                end
                            default:
                                begin
                                    count2 = 32'd0;
                                end
                            endcase
                        end
                    else
                        begin
                             START = 1'b0;   // no data to transmit
                             if (flag)
                                Ir_Commad = data; //data;
                             else
                                 Ir_Commad = 8'b1111_0000;    // No data
                        end
                end
            endcase
        end

    // The whole process of COM transmit module
    wire RST_B;
    reg START;
    wire TX_BUSY;
    assign RST_B = ~reset;

    //Instance DUT
    UART_TX I_UART_TX
    (
        .SYSCLK     (CLK_I     ),
        .RST_B      (RST_B      ),
        .START      (START      ),
        .UART_TX_DATA   (data   ),
        .UART_TX_O  (UART_TX_O  ),
        .TX_BUSY    (TX_BUSY    )
    );
endmodule`
**UART_TX.v**
`//==============================================================================
// File Name   : UART_CTL.v
// Module Name : This is UART control module
// Description :
// Author      : Zou Xiaoyi
// HomePage    : http://blog.csdn.net/zouxy09
// Date        : 2013/06/08
// Rev.        : 0.1
//==============================================================================
`define UD #1
module UART_TX
        (
        SYSCLK,
        RST_B,
        START,
        UART_TX_DATA,
        UART_TX_O,
        TX_BUSY
        );
//==============================================================================
//  Input and output deceleration
//==============================================================================
input       SYSCLK;     //系统时钟50MHz
input       RST_B;      //全局复位信号
input       START;      //由主控层发来的启动发送的脉冲
input   [7:0]   UART_TX_DATA;   //主控层传来的需要发送的数据
output      UART_TX_O;  //串口的串行输出数据线，TX
output      TX_BUSY;    //控制层返回给主控层的"忙信号"，1 表示正在发送中
//==============================================================================
//  Wire and reg deceleration
//==============================================================================
wire        SYSCLK;
wire        RST_B;
wire        START;
wire    [7:0]   UART_TX_DATA;
reg     UART_TX_O;
reg     TX_BUSY;
//==============================================================================
//  Wire and reg in the module
//==============================================================================
reg [11:0]   TIME_CNT;   //系统时钟计数器，根据波特率计算每一位的时间
reg [11:0]   TIME_CNT_N; //TIME_CNT 的下一个状态
reg [3:0]   BIT_CNT;    //位计数器，在状态机中用来控制每个状态停留的时间
reg [3:0]   BIT_CNT_N;  //BIT_CNT 的下一个状态
reg [9:0]   SHIFT_DATA; //输出移位寄存器，加上起始、停止位共10位
reg [9:0]   SHIFT_DATA_N;   //SHIFT_DATA 的下一个状态
reg [2:0]   UART_TX_CS; //发送状态机的当前状态
reg [2:0]   UART_TX_NS; //发送状态机的下一下状态
reg     UART_TX_O_N;    //UART_TX_O 的下一个状态
reg     TX_BUSY_N;  //TX_BUSY 的下一个状态
reg [1:0]   START_REG;  //记录发送脉冲的边沿变化
//------------------------------------------------------------------------------
parameter   IDLE        = 3'h0; //状态机空闲状态
parameter   SEND_START  = 3'h1; //状态机发送开始码的状态
parameter   SEND_DATA   = 3'h2; //状态机发送8位数据的状态
parameter   SEND_STOP   = 3'h3; //状态机发送停止位的状态
parameter   FINISH      = 3'h4; //状态机的结束状态
//------------------------------------------------------------------------------
always @ (posedge SYSCLK or negedge RST_B)
    begin
     if(!RST_B)
      START_REG <= `UD 2'h0;
     else
      START_REG <= `UD {START_REG[0], START};
    end
//------------------------------------------------------------------------------
always @ (posedge SYSCLK or negedge RST_B)
    begin
     if(!RST_B)
      TX_BUSY <= `UD 1'h0;//0 -> IDLE ,1->BUSY
     else
      TX_BUSY <= `UD TX_BUSY_N;
    end
//BUSY 信号为忙时，是状态机不为 IDLE 的所有状态
always @ (*)
    begin
     if(UART_TX_CS == IDLE)
      TX_BUSY_N = 1'h0;
     else
      TX_BUSY_N = 1'h1;
    end
//------------------------------------------------------------------------------
always @ (posedge SYSCLK or negedge RST_B)
    begin
     if(!RST_B)
      TIME_CNT <= `UD 12'h0;
     else
      TIME_CNT <= `UD TIME_CNT_N;
    end

//parameter   COUNT   = 9'h1B2; // for 50MHz and 115200
//parameter   COUNT   = 9'h1b2; // for 50MHz and 9600
//parameter   COUNT   = 9'hAE; // for 20MHz and 115200
parameter   COUNT   = 12'h820; // for 20MHz and 9600
// 波特率率为115200 ,每一位的周期是8.68us, 计数值为，9'h1b2
// 这里计数范围为 0 ~ 9'h1b1
always @ (*)
    begin
     if(TIME_CNT == COUNT)
      TIME_CNT_N = 12'h0;
    //不为IDLE时才会发数据，也才需要计数器计数
     else if(UART_TX_CS != IDLE)
      TIME_CNT_N = TIME_CNT + 12'h1;
     else
      TIME_CNT_N = TIME_CNT;
    end
//------------------------------------------------------------------------------
always @ (posedge SYSCLK or negedge RST_B)
    begin
     if(!RST_B)
      SHIFT_DATA <= `UD 10'h0;
     else
      SHIFT_DATA <= `UD SHIFT_DATA_N;
    end
always @ (*)
    begin
    //在发数据状态的第一时刻把要发送的10位数据先加载进来。
     if((TIME_CNT == 12'h0) && (UART_TX_CS == SEND_START))
      SHIFT_DATA_N = {1'b1,UART_TX_DATA[7:0],1'b0};
    //TIME_CNT 每一次为0时，就需要移出一位数据到TX线上
     else if(TIME_CNT == 12'h0)
      SHIFT_DATA_N = {1'b1,SHIFT_DATA[9:1]};
     else
      SHIFT_DATA_N = SHIFT_DATA;
    end
//------------------------------------------------------------------------------
always @ (posedge SYSCLK or negedge RST_B)
    begin
     if(!RST_B)
      BIT_CNT <= `UD 4'h0;
     else
      BIT_CNT <= `UD BIT_CNT_N;
    end
always @ (*)
    begin
     //每次状态机状态发生变化时，计数器重新清0，为下一次从0开始计数做准备
     if(UART_TX_CS != UART_TX_NS)
      BIT_CNT_N = 4'h0;
     //BIT_CNT 是对TIME_CNT的计数周期进行计数
     else if(TIME_CNT == COUNT)
      BIT_CNT_N = BIT_CNT + 4'h1;
     else
      BIT_CNT_N = BIT_CNT;
    end
//------------------------------------------------------------------------------
always @ (posedge SYSCLK or negedge RST_B)
    begin
     if(!RST_B)
      UART_TX_O = 1'b1;
     else
      UART_TX_O = UART_TX_O_N;
    end
//TX信号在发送过程中始终等于移位寄存器的0位，TX是低位先发，其它状态时保持高电平
always @ (*)
    begin
     if((UART_TX_CS == IDLE) || (UART_TX_CS == FINISH))
      UART_TX_O_N = 1'b1;
     else
      UART_TX_O_N = SHIFT_DATA[0];
    end
//------------------------------------------------------------------------------
// 发送流程控制的核心状态机
always @ (posedge SYSCLK or negedge RST_B)
begin
 if(!RST_B)
  UART_TX_CS <= `UD IDLE;
 else
  UART_TX_CS <= `UD UART_TX_NS;
end
always @ (*)
begin
 case(UART_TX_CS)
 IDLE       :
  if(START_REG)
   UART_TX_NS = SEND_START;
  else
   UART_TX_NS = UART_TX_CS;
 SEND_START :
  //发送状态必须保持完整的计数周期，每一位的时间严格保证
  if((BIT_CNT == 4'h0) && (TIME_CNT == COUNT))
   UART_TX_NS = SEND_DATA;
  else
   UART_TX_NS = UART_TX_CS;
 SEND_DATA  :
  if((BIT_CNT == 4'h7) && (TIME_CNT == COUNT))
   UART_TX_NS = SEND_STOP;
  else
   UART_TX_NS = UART_TX_CS;
 SEND_STOP  :
  if((BIT_CNT == 4'h0) && (TIME_CNT == COUNT))
   UART_TX_NS = FINISH;
  else
   UART_TX_NS = UART_TX_CS;
 FINISH     :
  if((BIT_CNT == 4'h0) && (TIME_CNT == COUNT))
   UART_TX_NS = IDLE;
  else
   UART_TX_NS = UART_TX_CS;
 default    :
   UART_TX_NS = IDLE;
 endcase
end

endmodule
//==============================================================================
//  End of file
//==============================================================================`
**4.2、PC端**
**Main.cpp**
```cpp
// Description : Receive IR command code from UART
// Author      : Zou Xiaoyi
// HomePage    : http://blog.csdn.net/zouxy09
// Date        : 2013/06/08
// Rev.        : 0.1
#include "serialPort.h"
#include <iostream>
#include <windows.h>
#include <stdlib.h>
using namespace std;
#define NUM_OF_KEY 7
enum COMMAND
{
	STOP,
	ENTER,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	STANDBY
};
char command[NUM_OF_KEY] = {0x52, 0x07, 0x47, 0x40, 0x06, 0x44, 0x14 };
char *command_name[NUM_OF_KEY] = 
{
	"STOP",
	"ENTER",
	"LEFT",
	"RIGHT",
	"UP",
	"DOWN",
	"STANDBY"
};
char command2key[NUM_OF_KEY] = {VK_CANCEL, VK_RETURN, VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN, VK_ESCAPE};
int main()
{
	SerialPort serial(2, 9600);
	if (!serial.Port_Init())
	{
		cout<<"The COM is not open error!"<<endl;
		return -1;
	}
	HANDLE comHandle = serial.getHANDLE();
	cout<<"Waiting for IR remote controler to send command..."<<endl;
	while(1)
	{
		char buf[10];
		int length = serial.readData(buf[0], 10);
		if (length)
		{
			for (int i = 0; i < NUM_OF_KEY; i++)
			{
				if (command[i] == buf[0])
				{
					cout<<"The key you press is: "<<command_name[i]<<endl;
					keybd_event(command2key[i], 0, 0, 0);				// key down
					keybd_event(command2key[i], 0, KEYEVENTF_KEYUP,0);	// key up
					break;
				}
				else if (i == (NUM_OF_KEY - 1))
				{
					cout<<"This is a new key, you can add to our program for regconizing it next time"<<endl;
				}
			}
		}
	}
	serial.Port_Close();
	return 0;
}
```


