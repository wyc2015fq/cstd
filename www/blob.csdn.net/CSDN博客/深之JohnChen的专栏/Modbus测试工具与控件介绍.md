# Modbus测试工具与控件介绍 - 深之JohnChen的专栏 - CSDN博客

2015年09月22日 10:32:06[byxdaz](https://me.csdn.net/byxdaz)阅读数：10277


Modbus测试工具与控件

1，简介

网站地址：[http://www.modbustools.com/](http://www.modbustools.com/)

该网站提供了几个软件工具，可以运行于windows2000/XP/Vista/7环境下，用来测试和仿真Modebus设备。

- 
[ModbusPoll](http://www.modbustools.com/modbus_poll.asp) ：Modbus主机仿真器，用于测试和调试Modbus从设备。该软件支持ModbusRTU、ASCII、TCP/IP。

- 
[ModbusSlave](http://www.modbustools.com/modbus_slave.asp)：Modbus从设备仿真器，可以仿真32个从设备/地址域。每个接口都提供了对EXCEL报表的OLE自动化支持。

- 
[WSMBT](http://www.modbustools.com/modbus_wsmbt.asp) ：Modbus主设备的.NET控件，支持C#，VB，managedC++语言，支持Modbus
 TCP/IP。

- 
[WSMBS](http://www.modbustools.com/modbus_wsmbs.asp)：Modbus从设备的.NET控件，支持C#，VB，managedC++语言，支持ModbusRTU/ASCII。

- 
[MBAXP](http://www.modbustools.com/modbus_activex.asp)：一个易用的ActiveX控件，支持Excel，VisualBasic，Visual
 C++，Delphi等。支持Modbus RTU/ASCII和Modbus TCP/IP

2，功能

Modbus Poll是一个主机仿真器，用来帮助开发人员测试Modbus从设备，或者其它MOdbus协议的测试和仿真。它支持多文档接口，即，可以同时监视多个从设备/数据域。每个窗口简单地设定从设备ID，功能，地址，大小和轮询间隔。你可以从任意一个窗口读写寄存器和线圈。如果你想改变一个单独的寄存器，简单地双击这个值即可。或者你可以改变多个寄存器/线圈值。提供数据的多种格式方式，比如浮点、双精度、长整型（可以字节序列交换）。

状态条显示错误信息。

如果你是一个从设备开发者，你可以通过"testcenter" 菜单，组织并发送你自己的测试字符串，并以16进制形式检查从设备返回的结果。

为VB,EXCEL等提供了OLE自动化接口。可以用OLE接口解析和显示Modbus数据，然后送达你指定的设备，即，在EXCEL中编辑数据，然后发送到你的从设备！示例参看安装后的Excelexample.xls。

支持下列协议:

- Modbus RTU
- Modbus ASCII
- Modbus TCP/IP
- Modbus RTU Over TCP/IP
- Modbus ASCII OverTCP/IP
- Modbus UDP/IP
- Modbus RTU Over UDP/IP
- Modbus ASCII OverUDP/IP

**MODBUSPOLL功能:**
- OLE自动化可以简单地与Visual Basic接口，使用起来类似于ActiveX控件。参见VBExample.vbp
- 读/写多达125个寄存器
- 读/写多达2000个输入/线圈
- [TestCenter](http://www.modbustools.com/Test.asp)菜单 (组织你自己的测试字串)
- 打印和打印预览
- 监视串行数据流量[serial datatraffic](http://www.modbustools.com/images/traffic.jpg)
- [Datalogging](http://www.modbustools.com/log.asp) to a text
 file 
- Data logging direct to [Excel](http://www.modbustools.com/log_excel.asp)
- 上下文敏感的HLP文件
- [10 Displayformats](http://www.modbustools.com/images/displayformat.jpg) such as float, double etc.
- Adjustable Address [Base (0 or1).](http://www.modbustools.com/base.asp)
- 字体和颜色选项
- 广播功能（从设备ID=0)
- Easy control of RS-485 converters with RTStoggle.

**支持MODBUS功能:**
- 01: Read coil status      读线圈状态
- 02: Read input status    读输入状态
- 03: Read holdingregister读保持寄存器
- 04: Read input registers 读输入寄存器
- 05: Force single coil      强制单线圈
- 06: Preset single register预置单寄存器
- 15: Force multiple coils  强制多线圈
- 16: Preset multiple registers预置多寄存器
- 17: Report slave ID        报告从设备ID
- 22: Mask write register   屏蔽写寄存器
- 23: Read/Write registers 读/写寄存器

3，两台微机之间串口测试

测试环境：XP32系统，一台机器运行Poll，一台运行Slave，中间用串口线连接。线序：2－3，3－2，5－5。

操作：

- 两边分别用F3设定连接参数：串口，串口1，9600band，8数据位，Even校验，1停止位，RTU模式，其它缺省。
- 主机端未正确连接，会给出提示
- 通讯开始，主机端显示：TX=35，Err=0，ID=1，F=03，SR=1000ms。意思是，发送35次命令，错误次数，从机ID，功能号，轮询间隔。
- 如果通讯出错，会在窗口第二行显示错误信息
- 使用Display菜单的"Communication Traffic"命令，可以显示出当前发送命令和接受的数据。
- 主机改变通讯功能，快捷键F8，令功能为4，读输入寄存器，然后在从机的10个寄存器上点击鼠标，输入每个寄存器的新值，则，在主机上会读取到这个新的寄存器值。

4，微机与AVR单片机之间串口测试：读输入寄存器

测试环境：微机运行Poll，AVR MEGA88运行移植后的代码，串口连线同上。

AVR代码：串口用9600bps，even校验，1停止位，RTU模式，SlaveID=1，其它未改变。

Poll设置：

- F3呼出连接对话框，设定9600bps同单片机波特率。
- F8呼出读写对话框，设定SlaveID=1；功能=4，地址=999，数量=4，扫描频率=1000ms，其它未变。
- 界面显示出AVR寄存器内容，通讯无错误提示。
- 换用9600/11400/19200/38400/115200bps波特率，通讯正常。因为Poll没有57600/230400等设置，这些波特率未测。AVR在该时钟下最高可达230400bps，无差错。

数据分析：

TX: 01 04 03 E7 00 04 41 BA          01-从设备地址，04-功能号，0x03E7=999，0x0004-读出寄存器个数，CRC校验

RX: 01 04 06 00 00 00 00 00 00 60 93   01-从设备地址，04-功能号，06-返回字节数，CRC校验

出现的问题：
- 用不同AVR数据测试时，发现AVR代码有下列问题：设置的地址要比实际demo.c中设定的地址少一个。比如，demo.c中起始地址1000，但测试时Poll只能设定999，否则测试出错。
- 该问题的根源在mbfuncinput.c中，解析出变量地址usRegAddress之后有个++动作，人为将其增加1。其它函数如mbfuncholding.c也是如此。
- 参看Modbus协议的定义：从零开始寻址寄存器，比如寻址输入寄存器1-16，则输入为0-15。真是奇怪啊！
- 自此可以看出，DEMO.C中定义的输入寄存器起始地址最小为1，此时才能满足Master要求的输入寄存器=0。

协议里规定，如果通讯出错，则返回数据格式为：

01 84 xx CRC2字节

这里xx表示错误码：xx=01 02 03 04，其中02-无效的数据地址

Modbus Poll 4.3.4 破解版下载地址

[点击下载](http://www.cr173.com/soft/21405.html)


