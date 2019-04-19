# 编写一个以C 语言为基础的DSP程序 - 三少GG - CSDN博客
2010年07月20日 13:45:00[三少GG](https://me.csdn.net/scut1135)阅读数：2052标签：[语言																[c																[cmd																[allocation																[存储																[汇编](https://so.csdn.net/so/search/s.do?q=汇编&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=allocation&t=blog)](https://so.csdn.net/so/search/s.do?q=cmd&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)
个人分类：[开源硬件Raspberry](https://blog.csdn.net/scut1135/article/category/712785)
编写一个以C 语言为基础的DSP程序
一．实验目的
1．学习用标准C语言编制程序；了
解常用的C语言程序设计方法和组成部分。
2．学习编制连接命令文件，并用来控制代码的连接。
3．学会建立和改变map文件，以及利用它观
察DSP内存使用情况的方法。
4．熟悉使用软件仿真方式调试程序。
二．实验原理
1． 标准C语言程序
CCS支持使用标准
C语言开发DSP应用程序。当使用标准C语言编制的程序时，其源
程序文件名的后缀应为.c(如：volume.c)。
CCS在编译标准C
语言程序时，首先将其编译成相应汇编语言程序，再进一步编译成目
标DSP的可执行代码。最后生成的是coff格式的可下载到DSP中运行的文件，
其文件名后
缀为.out。
由于使用C语言编制程序，其中调用的标准C的库函数由专门的库提供，在编译连接时
编译系统还负责构建C
运行环境。所以用户工程中需要注明使用C的支持库。
2． 命令文件的作用
命令文件(文件名后缀为cmd)为链接程序提供程序和数据在具体
DSP硬件中的位置分配
信息。通过编制命令文件，我们可以将某些特定的数据或程序按照我们的意图放置在DSP
所管理的内存中。命令文件也
为链接程序提供了DSP外扩存储器的描述。在程序中使用CMD
文件描述硬件存储区，可以只说明使用部分，但只要是说明的，必须和硬件匹配，也就是
只
要说明的存储区必须是存在的和可用的。
3． 内存映射(map)文件的作用
一般地，我们设计、开发的DSP程序在调试好后，要
固化到系统的ROM中。为了更
精确地使用ROM空间，我们就需要知道程序的大小和位置，通过建立目标程序的map文件
可以了解DSP代码
的确切信息。当需要更改程序和数据的大小和位置时，就要适当修改cmd
文件和源程序，再重新生成map文件来观察结果。另外，通过观察map文
件，可以掌握DSP
存储器的使用和利用情况，以便进行存储器方面的优化工作。
4．程序设计要求
程序框图（图2-1）：
三．
实验设备
PC兼容机一台，操作系统为Windows2000(或Windows98，WindowsXP，以下默认为
Windows2000)，
安装Code Composer Studio 2.0软件。
四．实验内容与步骤
1．实验准备
设置软件仿真模式，参看：本书第一
部分、四、1。
![](http://www.elecfans.com/article/UploadPic/2009-3/2009317012933635.jpg)
2．建立新的工程文件
(1)双击桌面上图标，
启动Code Composer 
Studio 2.21。
(2)进行以下设置(如图2-2)：
3．编辑输入源程序
(1)C语言程序
-先新建源程序窗口：
-
输入源程序：
int x,y,z;
main()
{
x=1; y=2;
while ( 1 )
{
z=x+y;
}
}
![](http://www.elecfans.com/article/UploadPic/2009-3/20093170124272700.jpg)
图2-3 新建源文件
-保存源程序为CProgram.c：
(2)连接命令文件
-
如同第(1)步操作，建立空的源程序窗口。
-输入连接命令文件内容：
-l rts2800.lib
-stack 400h
-heap
 100
MEMORY
{
PAGE 0 : PROG(R) : origin = 0x3E8000, length = 
0x10000
PAGE 0 : BOOT(R) : origin = 0x3FF000, length = 0xFC0
PAGE 0
 : RESET(R) : origin = 0x3FFFC0, length = 0x2
PAGE 0 : VECTORS(R) : 
origin = 0x3FFFC2, length = 0x3E
PAGE 1 : M0RAM(RW) : origin = 
0x000000, length = 0x400
PAGE 1 : M1RAM(RW) : origin = 0x000400, length =
 0x400
PAGE 1 : L0L1RAM(RW) : origin = 0x008000, length = 0x2000
图2-4 保存为c文件
PAGE 1 : H0RAM(RW) : origin = 
0x3F8000, length = 0x2000
}
SECTIONS
{
/* 22-bit program 
sections */
.reset : > RESET, PAGE = 0
vectors : > VECTORS, 
PAGE = 0
.pinit : > PROG, PAGE = 0
.cinit : > PROG, PAGE = 0
.text
 : > PROG, PAGE = 0
/* 16-Bit data sections */
.const : > 
L0L1RAM, PAGE = 1
.bss : > L0L1RAM, PAGE = 1
.stack : > 
M1RAM, PAGE = 1
.sysmem : > M0RAM, PAGE = 1
/* 32-bit data 
sections */
.ebss : > H0RAM, PAGE = 1
.econst : > H0RAM, 
PAGE = 1
.esysmem : > H0RAM, PAGE = 1
}-l 
rts2800.lib
-如同第(1)步操作，将文件存为：
C:/ICETEK-F2812-A-EDUlab/DSP281x_examples/Lab0102-CProgram
 /CProgram.cmd
(3) 将上述编译的源程序加入工程CProgram.pjt。
5．编译源文件、下载可执行程序
(1)
 单击菜单“Project”、“Rebuild All”。
(2) 执行FileàLoad Program 
，在随后打开的对话框中选择刚刚建立的
C:/ICETEK-F2812-EDULab/Lab0102-CProgram/debug/CProgram.out
 文件。完成后，系
统自动打开一个反汇编窗口“Disassembly”，并在其中指示程序的入口地址为
“_c_int00”。
6．
打开观察窗口
开启CPU寄存器观察窗口：单击菜单View->Registers->Core。
7．观察程序运行结果
这
时，在“Disassembly”代表程序运行位置的绿色箭头指向程序的入口地址，程序将从
此开始执行。
(1) 
选择菜单中Debug->Go Main，CCS自动打开CProgram.c，程序会停在用户主程序入
口main上，这从反汇编窗口和
CProgram.c窗口中的指示箭头位置可以看出。
(2) 在内存观察窗口中观察变量的值：
选择“View”菜单中“Memory…”
项，在“Memroy Window Options”窗口中的“Adress”
项中输入&x，单击“OK”完成设置；“Memory”
窗口中x 的当前取值显示在第1个地
址的后。
(3) 将变量x、y、z分别加入观察窗口：
在源程序中双击变量名，再单击鼠标右
键，选择“Add to Watch Window”。这时，这3
个变量还未作初始化。
(4) 
单步运行2次，在观察窗中观察到变量x、y被赋值。变化的值被显示成红色。同时
在“Memory”窗口中也能观察到x和y值的改变。
(5)
 再单步运行，可观察到z 的值被计算出来。双击观察窗口中变量x、y在“Value”栏
中的取值并修改成其他取值，单步运行后观察结果。
(6)
 双击观察窗口中变量x、y在“Value”栏中的取值，并修改成0；选择菜单
Debug->Restart，返回程序起点。
(7)
 重新单步运行程序
8．内存映像文件
(1) 选择菜单Project->Build Options…，启动“Build 
Options”工程设置对话框。
(2) 单击“Linker”属性页，在“Map Filename”项中观察生成的map文件名和路径。
(3)
 单击“取消”退出。
9．对照观察map文件和cmd文件的内容
(1) 选择菜单File->Open…，将找到
C:/ICETEK-F2812-A-EDUlab/DSP281x_examples/Lab0102-CProgram
 /Debug目录，将文
件类型改为“Memory Map Files”，选择CProgram.map文件、打开。
(2) 
打开CProgram.cmd 文件。
(3) 程序的入口地址：map文件中“ENTRY POINT SYMBOL”中说明了程序入口地址
(_c_int00)。
(4)
 内存使用情况：
-map文件中“MEMORY CONFIGURATION”标明了程序占用RAM 的使用情况，共占
用aaH 
个存储单元。
-观察map文件中的“SECTION ALLOCATION MAP”段，可以看出CProgram.obj的入
口地址为
0x3e801e，这也是main函数的入口地址。
-用户堆栈段从400H 开始，程序运行到main函数中后，变量x、y、z 均开设在栈中。
-
还能看出程序运行都需要调用rts2800.lib 中的哪些模块。
10．改变内存分配
修改cmd文件中的
PAGE 0 : 
PROG(R) : origin = 0x3E8000, length = 0x10000
改为
PAGE 0 : PROG(R) :
 origin = 0x3E9000, length = 0x10000
重新编译工程，观察map文件中有何变化。
11．退出CCS
五．
预习要求
1．熟悉C语言，了解常用的C语言程序设计方法。
2．了解DSP内存的分配情况。
六．注意事项
在重新单步运行程
序，注意观察在CPU寄存器窗口中，各寄存器使用情况，观察哪个
寄存器参与了运算。
七．思考题
修改程序完成计算
sin(2.3π)+cos(1.7π)的值。
八． 实验报告要求
1．通过实验可以发现，修改cmd文件可以安排程序和数据在DSP内存
资源中的分配和位置；
map文件中描述了程序和数据所占用的实际尺寸和地址。
2．C语言编制的程序，在经过编译器编译后，需要连接若干C
标准程序辅助运行。以下是
运行流程：
(1)程序入口为_c_int00，执行标准C库中的程序，负责初始化C环境、申请堆栈、初始化
有
初始值的变量等。
(2)程序最终转到用户编制的主函数运行。
(3)程序在主函数中的无限循环中持续运行。
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
