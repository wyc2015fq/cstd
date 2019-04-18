# Modbus测试工具ModbusPoll与Modbus Slave使用方法 - 深之JohnChen的专栏 - CSDN博客

2017年09月14日 13:15:39[byxdaz](https://me.csdn.net/byxdaz)阅读数：38828


一、介绍

Modbus Poll ：Modbus主机仿真器，用于测试和调试Modbus从设备。该软件支持ModbusRTU、ASCII、TCP/IP。用来帮助开发人员测试Modbus从设备，或者其它Modbus协议的测试和仿真。它支持多文档接口，即，可以同时监视多个从设备/数据域。每个窗口简单地设定从设备ID，功能，地址，大小和轮询间隔。你可以从任意一个窗口读写寄存器和线圈。如果你想改变一个单独的寄存器，简单地双击这个值即可。或者你可以改变多个寄存器/线圈值。提供数据的多种格式方式，比如浮点、双精度、长整型（可以字节序列交换）。

Modbus Slave： Modbus从设备仿真器，可以仿真32个从设备/地址域。每个接口都提供了对EXCEL报表的OLE自动化支持。主要用来模拟Modbus从站设备,接收主站的命令包,回送数据包。帮助Modbus通讯设备开发人员进行Modbus通讯协议的模拟和测试，用于模拟、测试、调试Modbus通讯设备。可以32个窗口中模拟多达32个Modbus子设备。与Modbus
 Poll的用户界面相同，支持功能01, 02, 03, 04, 05, 06, 15, 16, 22和23，监视串口数据。

二、Modbus Poll使用

1)、安装注册：

安装完毕，在桌面能够看到ModbusPoll的快捷方式，双击该快捷方式，打开软件，打开后界面如下图所示。

![](https://img-blog.csdn.net/20170914125832149?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

单击Connection->Connect,弹出注册窗口；打开压缩包解压后的readme文件，复制ModbusPoll的序列号，粘贴到注册窗口的注册栏，如下图所示，点击OK，破解完毕。

![](https://img-blog.csdn.net/20170914125846556?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

2)、程序主窗口：

按照上面破解流程，按“OK”按钮即可完成注册。然后直接进入主窗口，如下图所示：

![](https://img-blog.csdn.net/20170914125832149?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**modbus poll 4.3.4程序主窗口**

其中：Tx = 4表示向主站发送数据帧次数，图中为4次； Error = 0表示通讯错误次数，图中为0次；
 ID = 1表示模拟的Modbus子设备的设备地址，图中地址为1；F = 03表示所使用的Modbus功能码，图中为03功能码；
 SR = 1000ms表示扫描周期。红字部分，表示当前的错误状态，“No Connection”表示未连接状态。

3)、参数设置：

单击菜单【Setup】中【Read/Write Definition.. F8】进行参数设置，会弹出参数设置对话框。

![](https://img-blog.csdn.net/20170914125902309?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20170914125924868?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中：A. Slave为要访问的Modbus从站（从机）的地址，对应主窗口（主画面）中的ID值，默认为1。  B. Function 为寄存器功能码的选择，共四种,分别对应关系如下：
|代码|中文名称|寄存器PLC地址|位操作/字操作|操作数量|
|----|----|----|----|----|
|01|读线圈状态|00001-09999|位操作|单个或多个|
|02|读离散输入状态|10001-19999|位操作|单个或多个|
|03|读保持寄存器|40001-49999|字操作|单个或多个|
|04|读输入寄存器|30001-39999|字操作|单个或多个|
|05|写单个线圈|00001-09999|位操作|单个|
|06|写单个保持寄存器|40001-49999|字操作|单个|
|15|写多个线圈|00001-09999|位操作|多个|
|16|写多个保持寄存器|40001-49999|字操作|多个|

 C. Address 为寄存器起始地址，默认从0开始。

D. Length为寄存器连续个数，默认为10个，对应组态软件的通道数量。

E. Scan Rate为读取数据周期，单位毫秒，默认为1000ms。

设置完成单击OK按钮，模拟窗口将显示定义的寄存器列表。

4)、显示设置：

数据显示方式设置：

默认情况下，寄存器数据的显示方式为Signed方式(16进制无符号二进制)，数据范围为-32768~32767。如果用户要以其他数值方式显示，可以通过菜单【Display】进行选择设置，如下图所示：

![](https://img-blog.csdn.net/20170914125936631?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

地址格式设置： Modbus Slave默认使用PLC地址，【Display】菜单中【PLC
 Addresses(Base 1)】为默认选中状态，默认寄存器的起始地址为1开始，此点与组态软件的Modbus串口和TCP数据转发驱动是相同的。如果测试时需要设置起始地址为0开始，可选择【Protocol
 Addresses(Base 0)】。一般情况下使用默认PLC地址即可。

使用工具栏的”Communication Traffic”按钮，可以显示出当前发送命令和接受的数据。

5)、串口连接：

单击菜单【Connection】中【Connect.. F3】进行串口连接，如下图：

![](https://img-blog.csdn.net/20170914130621081?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中：Port3，表示使用的串口(COM3)，根据实际情况选择不同的串口或者TCP/IP进行连接。Mode，表示Modbus协议模式，使用串口时有效；使用TCP/IP时自动为TCP模式。Response
 Timeout，表示读取超时时间，从站在超时时间内没有返回数据，则认为通讯失败。Delay Between Polls，每次扫描的最小间隔时间，默认为10ms。Remote
 Server，表示TCP/IP模式时的终端从站设备的网络设置。IP Address，表示TCP/IP模式时从站IP地址。 
 Port，表示TCP模式时从站网络端口。用户可根据需要对参数进行设置，默认为：串口1、波特率9600、数据位8位、无校验位、1位停止位。确认设置后点击OK按钮即可。如果连接并读取正确，此时主窗口将显示读取寄存器的相关信息。

6)、寄存器值改变：

在主窗口寄存器地址上双击鼠标，弹出修改对话框，如下图：

![](https://img-blog.csdn.net/20170914125946798?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在Value输入框中输入值确认即可。范围为-32768——32767。

其中：Slave为要访问的Modbus从站的地址，对应主画面中的ID值，默认为1。  Address 为当前操作的寄存器的地址。图中为对寄存器40001操作。  Use Function为所使用的功能码，可以选择06或16功能码进行写入。

7)、查看通讯数据帧：

单击【Display】菜单中的【Communication…】或者单击工具栏上【101】按钮，可以调出串口收发数据帧监视信息对话框“CommunicationTraffic”，用来查看分析收发的数据帧。如下图所示：

![](https://img-blog.csdn.net/20170914125955477?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中：前6位为数据帧的序号。  Rx表示接收的数据帧。  Tx表示发送的数据帧。

8)、断开连接：

点击【Disconnect  F4】即可断开连接结束测试，此时主窗口中出现红色的“No Connection”表示未连接状态。

三、Modbus Slave使用

1)、安装注册：
安装软件，安装完成之后，双击执行ModbusSlave快捷方式即可。

注册方法：单击Connection->Connect,弹出注册窗口；打开压缩包解压后的readme文件，复制ModbusPoll的序列号，粘贴到注册窗口的注册栏，点击OK，破解完毕。ModbusSlave的安装破解与ModbusPoll类似，在此不再赘述。

2)、主窗口：
软件主窗口如下图所示：

![](https://img-blog.csdn.net/20170914130700453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

此时处于“Noconnection”未连接状态，如果只测试“04输出保持寄存器”，则可直接点击跳到第3.步，直接进行连接即可。

3)、参数设置：
点击菜单“Setup”中“Slave Definition.. F2”进行参数设置，会弹出如下图对话框。

![](https://img-blog.csdn.net/20170914130711016?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中：A.Slave为Modbus从站地址，对应主画面中的ID值，默认为1。

B. Function 为寄存器功能码的选择，共四种,分别对应关系如下：
|代码|中文名称|寄存器PLC地址|位操作/字操作|操作数量|
|----|----|----|----|----|
|01|读线圈状态|00001-09999|位操作|单个或多个|
|02|读离散输入状态|10001-19999|位操作|单个或多个|
|03|读保持寄存器|40001-49999|字操作|单个或多个|
|04|读输入寄存器|30001-39999|字操作|单个或多个|
|05|写单个线圈|00001-09999|位操作|单个|
|06|写单个保持寄存器|40001-49999|字操作|单个|
|15|写多个线圈|00001-09999|位操作|多个|
|16|写多个保持寄存器|40001-49999|字操作|多个|

C. Address 为寄存器起始地址。默认从1开始。

D. Length为寄存器连续个数。默认为10个。

设置OK按钮，模拟窗口将显示定义的寄存器列表：

![](https://img-blog.csdn.net/20170914130700453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中：ID，表示模拟的Modbus子设备的设备地址；F，表示功能码
点击对应的寄存器，即可修改对应寄存器的值或者状态。比如2寄存器值修改成9，9寄存器值修改成100。

4)、显示设置：
数据显示方式设置：
默认情况下，寄存器数据的显示方式为Signed方式(16进制无符号二进制)，数据范围为范围为-32768——32767。如果用户要以其他数值方式显示，可以通过菜单“Display”进行选择设置，如下图所示：

![](https://img-blog.csdn.net/20170914130721765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

地址格式设置：

Modbus Slave默认使用PLC地址，“Display”菜单中“PLC
 Addresses(Base 1)”为默认选中状态，默认寄存器的起始地址为1开始，此点与组态软件的Modbus串口和TCP数据转发驱动是相同的。如果测试时需要设置起始地址为0开始，可选择“Protocol
 Addresses(Base 0)”。一般情况下使用默认PLC地址即可。

5)、连接：
点击菜单“Connection”中“Connect.. F3”进行连接。弹出连接对话框：

![](https://img-blog.csdn.net/20170914130731171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中：Port2，表示使用的串口(COM2)，根据实际情况选择不同的串口或者TCP/IP进行连接

Mode，表示Modbus协议模式，使用串口时有效；使用TCP/IP时自动为TCP模式

Flow Control，表示流控制，串口模式时有效；

Ignore Unit ID，表示TCP模式时忽略无效的组编号
用户可根据需要对串口参数进行设置，默认为:串口1,波特率9600,数据位8位，无校验位，1位停止位。确认设置后点击“OK”按钮即可。此时主窗口中红色的“No
 connection”表示未连接状态的信息消失，表明从站处于正常连接状态。

6)、寄存器值改变：
在主窗口寄存器地址上双击鼠标，弹出修改对话框，如下图：

![](https://img-blog.csdn.net/20170914130739939?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在输入框中输入值确认即可。范围为-32768——32767。
其中：Auto increment选项钩选上后，对应寄存器的值可以每1秒种增加1。

7)、查看通讯数据帧：
点击“Display”菜单中的“Communication…”,可以调出串口收发数据帧监视信息对话框，用来查看分析收发的数据帧。如下图所示：

![](https://img-blog.csdn.net/20170914130748747?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中：前6位为数据帧的序号。

Rx：表示接收数据帧。

Tx: 表示发送数据帧。

8)、断开连接：
点击“Disconnect F4”即可断开连接结束测试，此时主窗口中出现红色的“No connection”表示未连接状态。

四、Modbus主机/从机模拟程序

1）、安装vspd虚拟串口工具。通过工具添加COM2和COM3两个端口。

![](https://img-blog.csdn.net/20170914131316613)

2）、接下来打开已经安装的modbuspoll和modbus
 slave软件，分别按“F8”配置主从端的相关配置，如图：

![](https://img-blog.csdn.net/20170914131404006)
以上配置的是10个计数器从机设备ID为1但只读取前6个计数器的数据，可根据自己的情况设置。

2、接下来按分别按“F3”进行连接，注意修改连接界面的端口，主从机分别选择我们虚拟出来的com2和com3端口，因为我只用到了RTU模式，所以其他选项如波特率9600,比特位8，校验位1，无等价位等可以保持不变，然后点击确定进行连接，未连接成功窗口第二行会给出提示信息。

通讯开始，主机端显示：TX=232，Err=0，ID=1，F=03，SR=1000ms。意思是，发送232次命令，错误次数，从机ID，功能号，轮询间隔。

使用工具栏的”Communication Traffic”按钮，可以显示出当前发送命令和接受的数据。

Modbus Poll 4.3.4 破解版下载：[http://www.cr173.com/soft/21405.html](http://www.cr173.com/soft/21405.html)

Modbus Slave调试工具下载：[http://www.cr173.com/soft/21410.html](http://www.cr173.com/soft/21410.html)

Modbus调试精灵 7.04绿色版下载：[http://www.cr173.com/soft/5480.html](http://www.cr173.com/soft/5480.html)


