# USB之（四）HID设备类协议 - xqhrs232的专栏 - CSDN博客
2017年09月14日 16:14:01[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：572
原文地址::[http://blog.csdn.net/ivy_reny/article/details/47722757](http://blog.csdn.net/ivy_reny/article/details/47722757)
相关文章
1、[USB HID协议中几个关键概念的理解](http://blog.csdn.net/coroutines/article/details/45044077)----[http://blog.csdn.net/coroutines/article/details/45044077](http://blog.csdn.net/coroutines/article/details/45044077)
2、USBHID协议中文版——USB接口HID设备----[https://wenku.baidu.com/view/2c2ecd2fccbff121dd368396.html](https://wenku.baidu.com/view/2c2ecd2fccbff121dd368396.html)
3、HID设备如何收发数据？需不需要类似串口通信的什么通信协议----[http://bbs.csdn.net/topics/390906241](http://bbs.csdn.net/topics/390906241)
4、[USB HID 设备类协议入门](http://blog.csdn.net/victory08/article/details/52404180)----[http://blog.csdn.net/victory08/article/details/52404180?locationNum=9](http://blog.csdn.net/victory08/article/details/52404180?locationNum=9)
USB HID类设备属于人机交互操作的设备，是一个比较大的类，用于控制计算机操作的某些方面，如USB鼠标、USB键盘、USB游戏操作杆、USB触摸板、USB轨迹球、电话拨号设备、VCR遥控等等设备。另外，使用HID设备的一个好处就是操作系统自带了HID类的驱动程序，用户无需开发很麻烦的驱动程序，只有直接使用API调用即可完成通信，所有很多简单的USB设备喜欢枚举成HID设备，这样就可以不用安装驱动而直接使用。
# 1、HID设备描述符
     当插入USB设备后，主机会向设备请求各种描述符来识别设备。为了把一个设备识别为HID类，设备在定义描述符的时候必须遵守HID规范。HID设备的描述符除了5个USB的标准描述符（设备描述符、配置描述符、接口描述符、端点描述符、字符串描述符）外，还包括3个HID设备类特定描述符：HID描述符、报告描述符、物理描述符。HID描述符是关联于接口（而不是端点）的，所以设备不需要为每个端点都提供一个HID描述符。设备和主机的通信是通过报告的形式来实现的。而物理描述符不是必需的。
     5个标准描述符中与HID设备有关的部分有：
     设备描述符中bDeviceClass、bDeviceSubClass和bDeviceProtocol三个字段的值必须为0。
     接口描述符中bInterfaceClass的值必须为0x03，bInterfaceSubClass的值为0或1，为1表示HID设备是一个启动设备（Boot Device，一般对PC机而言才有意义，意思是BIOS启动时能识别并使用您的HID设备，且只有标准鼠标或键盘类设备才能成为Boot Device。如果为0则只有在操作系统启动后才能识别并使用您的HID设备）。
3~255
保留
## 1.1 HID描述符
     HID描述符关联于接口描述符，因而如果一个设备只有一个接口描述符，则无论它有几个端点描述符，HID设备只有一个HID描述符。HID设备描述符主要描述HID规范的版本号、HID设备所用的额外描述符、报告描述符的长度等。
10
wDescrioptorLength
2
描述符总长度，使用一个以上描述符的设备
## 1.2 报告描述符
     HID设备的报告描述符比较复杂也比较难理解。它以item形式排列组合而成，无固定长度，用户可以自定义长度以及每一bit的含义。HID报告描述符已经不是简简单单地描述某个值对应某个固定意义了，它已经能够组合出很多种情况，并且需要PC上的HID驱动程序提供parser解释器来对描述的设备情形进行重新解释，进而组合生成出本HID硬件设备独特的数据流格式，所以可以把它理解为“报告描述符脚本语言”更为贴切。我们使用“报告描述符”专用脚本语言，让用户来自己定义他们的HID设备都有什么数据以及这些数据各bit都有什么意义。USB协会提供了一个HID描述符编辑工具HIDDescrioptor
 Tool，用它可以方便生成报告描述符。
     item类型分为三种：main, global和local。其中main类型又可以分为5种tag：
- input item tag：指的是从设备的一个或多个类似控制管道得到的数据
- output item tag：指的是发送给一个或多个类似控制管道的数据
- feature item tag：表示设备的输入输出不面向最终用户
- collection item tag：一个有意义的input, output和feature的组合项目
- end collection item tag：指定一个collectionitem的终止
每一个mainitem tag（input，output，feature）都表明了来自一个特定管道的数据的大小，数据相对还是独立，以及其他相关信息。在此之前，global和local item定义了数据的最大值和最小值，等等。local item仅仅描述下一个main item定义的数据域，而global item是这一个报告描述符中所有后续数据段的默认属性。
一个报告描述符可能包含多个main item，为了准确描述来自一个控制管道的数据，一个报告描述符必须包括以下内容：
- input（output，feature）
- usage
- usage page
- Logical Minimum
- Logical Maximum
- Report Size
- Report Count
下面用一个三键鼠标举例说明：
UsagePage (Generic Desktop);    //global item
Usage(Mouse);    //global item
Collection(Application);    //Start Mousecollection
Usage(Pointer);    //
Collection(Physical);    //Start Pointer collection
UsagePage (Buttons)
UsageMinimum (1),
UsageMaximum (3),
LogicalMinimum (0),
LogicalMaximum (1) ;   //Fields return valuesfrom 0 to 1
ReportCount (3),
ReportSize (1);   //Create three 1 bit fields(button 1, 2, & 3)
Input(Data, Variable, Absolute);   //Addfields to the input report.
ReportCount (1),
ReportSize (5);   //Create 5 bit constant field
Input(Constant), ;Add field to the input report
UsagePage (Generic Desktop),
Usage(X),
Usage(Y),
LogicalMinimum (-127),
Logical Maximum(127);    //Fields return values from-127 to 127
ReportSize (8),
ReportCount (2);    //Create two 8 bit fields(X & Y position)
Input(Data, Variable, Relative);   //Addfields to the input report
EndCollection;   //Close Pointer collection
EndCollection;   //Close Mouse collection
     item的数据格式有两种，分别是短item和长item。
     短item格式
|bSize|0:0个字节  1:1个字节  2:2个字节  3:3个字节|
|----|----|
|bType|0:main  1:global  2:local  3:保留|
|bTag|item类型8:input  9:output  A:collection  B: feature  C:end collection|
     长item，其bType位值为3，bTag值为F
|bDataSize|0:0个字节  1:1个字节  2:2个字节  3:3个字节|
|----|----|
|bLongItemTag|0:main  1:global  2:local  3:保留|
|data|数据|
## 1.3 物理描述符
     物理描述符被用来描述设备的行为特性，物理描述符是可选的，HID设备可以根据其本体的设备特性选择是否包含物理描述符。下表是HID的物理描述符结构。
1
bFlags
1
位指定标志
位0~4：Effort
位5~7：Qualifier
0x14
无名指
0x03
同时
0x07
保留
# 2、HID类可采用的通信管道
     所有的HID设备通过USB的控制管道（默认管道，即端点0）和中断管道与主机通信。
中断输出
可选
传输从主机到设备的输出数据
     控制管道主要用于以下3个方面：
- 接收/响应USB主机的控制请示及相关的类数据
- 在USB主机查询时传输数据（如响应Get_Report请求等）
- 接收USB主机的数据
     中断管道主要用于以下两个方面：
- USB主机接收USB设备的异步传输数据
- USB主机发送有实时性要求的数据给USB设备
     从USB主机到USB设备的中断输出数据传输是可选的，当不支持中断输出数据传输时，USB主机通过控制管道将数据传输给USB设备。
# 3、HID设备类特定的命令（请求）
     HID设备类特定的命令（请求）有6个，它们分别是Get_Report, Get_Idle, Get_Protocol,Set_Report, Set_Idle和Set_Procotol。
     HID类请求（命令）数据包格式如下表所示：
6
wLength
2
该请求的数据段长度
0x0B
SET_PROTOCOL
仅仅适应于支持启动功能的HID设备（Boot Device）
## 3.1 GET_REPORT
     主机通过控制端点获取一个Report
|**域**|**值**|
|----|----|
|bmRequestType|0xA1|
|bRequest|0x01|
|wValue|高字节表示报告类型0x01: input0x02: output0x03: featureOther: reserved低字节表示ReportID，如不使用设为0|
|wIndex|HID的interface索引值|
|wLength|Report长度|
|Data|Report内容|
## 3.2 SET_REPORT
     主机发送一个Report给设备，用以设置input，output或者feature
|**域**|**值**|
|----|----|
|bmRequestType|0x21|
|bRequest|0x09|
|wValue|高字节表示报告类型0x01: input0x02: output0x03: featureOther: reserved低字节表示ReportID，如不使用设为0|
|wIndex|HID的interface索引值|
|wLength|Report长度|
|Data|Report内容|
## 3.3 GET_IDLE
|**域**|**值**|
|----|----|
|bmRequestType|0xA1|
|bRequest|0x02|
|wValue|高字节0低字节表示ReportID，如不使用设为0|
|wIndex|HID的interface索引值|
|wLength|1|
|Data|空闲速率|
## 3.4 SET_IDLE
|**域**|**值**|
|----|----|
|bmRequestType|0x21|
|bRequest|0x0A|
|wValue|高字节新的速率低字节表示ReportID，如不使用设为0|
|wIndex|HID的interface索引值|
|wLength|0|
|Data|无|
## 3.5 GET_PROTOCOL
|**域**|**值**|
|----|----|
|bmRequestType|0xA1|
|bRequest|0x03|
|wValue|0|
|wIndex|HID的interface索引值|
|wLength|1|
|Data|0=Boot Protocol1=Report Protocol|
## 3.6 SET_PROTOCOL
|**域**|**值**|
|----|----|
|bmRequestType|0x21|
|bRequest|0x0B|
|wValue|0=Boot Protocol1=Report Protocol|
|wIndex|HID的interface索引值|
|wLength|0|
|Data|无|

