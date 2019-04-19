# 实例抓包分析USB鼠标枚举数据流 - xqhrs232的专栏 - CSDN博客
2017年08月31日 15:50:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：214
个人分类：[PS2/USB/HID技术](https://blog.csdn.net/xqhrs232/article/category/7431349)
原文地址::[http://blog.csdn.net/luohsh1/article/details/9374449](http://blog.csdn.net/luohsh1/article/details/9374449)
相关文章
1、[linux下的usb抓包方法](http://blog.csdn.net/xiaojsj111/article/details/14127607)----[http://blog.csdn.net/xiaojsj111/article/details/14127607](http://blog.csdn.net/xiaojsj111/article/details/14127607)
USB Monitor的高版本太复杂了，都不会用，这里还是使用2.5的老版本.
新建session，选择新插入的HID鼠标开始抓包，拔掉鼠标，清空记录，再次插入鼠标，现在就抓到了USB鼠标枚举数据流
要看懂这些数据流，需要
[http://hi.baidu.com/deep_pro/blog/item/3f97effa7048f814a9d31115.html](http://hi.baidu.com/deep_pro/blog/item/3f97effa7048f814a9d31115.html)
[http://hi.baidu.com/deep_pro/blog/item/0ca086af97ca15f1faed5016.html](http://hi.baidu.com/deep_pro/blog/item/0ca086af97ca15f1faed5016.html)
里给出的14个表
**要注意的是USB数据包通常是先发LSB，再发MSB，所以低字节在前，高字节在后。后面的讲解中2个字节长度以上的数据没有转换回高字节在前，低字节在后的顺序，保留了原始状态。**
首先是两个PnP事件
**000026: PnP Event: Query ID (UP), 18.10.2009 09:53:28.9218750 +4.2656250**Hardware IDs: USB/Vid_15ca&Pid_00c3&Rev_0512, USB/Vid_15ca&Pid_00c3
**000027: PnP Event: Query ID (UP), 18.10.2009 09:53:28.9218750 +0.0**Compatible IDs: USB/Class_03&SubClass_01&Prot_02, USB/Class_03&SubClass_01, USB/Class_03
接着主机发出了第一个包**000028**
**000028: Get Descriptor Request (DOWN), 18.10.2009 09:53:28.9375000 +0.0156250**Descriptor Type: Device
Descriptor Index: 0x0
Transfer Buffer Size: 0x12 bytes
在这里没有给出主机包里的具体内容，跟Bus Hound是不一样的。只能看出请求的是设备描述符，要求返回的长度是18个字节，偏移为0
对应的，USB鼠标回答了这个请求
**000029: Control Transfer (UP), 18.10.2009 09:53:28.9375000 +0.0**Pipe Handle: 0x89457768
|12 01 00 02 00 00 00 08 CA 15 C3 00 12 05 00 02 00 01|........Ê.Ã.......|
Setup Packet
|80 06 00 01 00 00 12 00|€.......|
Recipient: Device
Request Type: Standard
Direction: Device->Host
Request: 0x6 (GET_CONFIGURATION)
Value: 0x100
Index: 0x0
Length: 0x12
这里可以看到主机刚才发给来的数据包80 06 00 01 00 00 12 00，结构上面提到的参考表一
第一个字节80是位图，没什么大的作用，限制第二个字节的。
第二个字节06参照表二表三得知是GET_DESCRIPTOR命令
wValue：00 01 参照表5得知是设备描述符
wIndex：00 00 偏移为0
wLength：12 00 要求设备端返回18个字节
再看看设备端的回答12 01 00 02 00 00 00 08 CA 15 C3 00 12 05 00 02 00 01，正好18字节
设备描述符的结构参照表四，
bLength：12 ，此设备描述符的长度是18字节
bDecriptorType：01 ,参照表5得知是设备描述符 
bcdUSB:00 02 ,代表USB协议的版本号，此处2.0版
bDeviceClass:00
bDeviceSubClass:00,表述设备类码由接口文件给出，可能是为了每个接口独立实现不同的功能。
bDevicePortocol：00，跟前两个字节紧密联系，这里指还是等待在接口文件里再说明使用的设备协议
bMaxPacketSize0：08，指端点0最大可接受的包大小，真是够抠门的，刚刚够用。
idVendor：CA 15 ，VID其实是0x15ca，字节序问题不在赘述
idProduct：C3 00 ，PID 0x00c3，在[http://www.linux-usb.org/usb.ids](http://www.linux-usb.org/usb.ids)查询得知
                   15ca  Textech International Ltd. 00c3  Mini Optical Mouse，实物确实是一个mini鼠标
bcdDevice:12 05，这个似乎没什么通用的意义
iManufacturer：00，
iProduct：02，
iSerialNumber：00,
bNumConfigurations：01
这几个数字都是索引值，如果以后主机想向设备端索要这些字符串信息（包含在字符串描述符里），就用这些值填充wIndex
第一个回合终于结束，由人工翻译真得很累啊
主机可没歇着，又发送了一个包
**000030: Get Descriptor Request (DOWN), 18.10.2009 09:53:28.9375000 +0.0**Descriptor Type: Configuration
Descriptor Index: 0x0
Transfer Buffer Size: 0x9 bytes
这个包也看不出来什么，因为发送的数据信息在回复包里才有显示，直接看回复
**000031: Control Transfer (UP), 18.10.2009 09:53:28.9375000 +0.0**Pipe Handle: 0x89457768
|09 02 22 00 01 01 00 A0 31|..".... 1|
Setup Packet
|80 06 00 02 00 00 09 00|€.......|
Recipient: Device
Request Type: Standard
Direction: Device->Host
Request: 0x6 (GET_CONFIGURATION)
Value: 0x200
Index: 0x0
Length: 0x9
主机发送的数据
80 06 00 02 00 00 09 00
分析了刚才那个包，这个一眼就看出来了，还是GET_DESCRIPTOR命令，要求的是配置描述符（Configuration Descriptor），9个字节
设备回复的数据09 02 22 00 01 01 00 A0 31,配置描述符见表8
bLength：09 
bDescriptorType：02 这跟设备描述符都没什么区别
wTotalLength： 22 00 ，表示包括此配置描述符、接口描述符、端点描述符和设备类及厂商定义的描述符的总长为0x0022=34个字节。
bNumInterfaces： 01 ，支持的接口数为1
bCongfigurationValue:01,  SetConfiguration请求中用作参数来选定此配置
iConfiguration:00,描述此配置的字串描述表索引
bmAttributes:A0,10100000B,只看得出是D5： 远程唤醒 ，D7是保留位
MaxPower:31,总线耗电量为49x2=98mA.
第三个回合
主机发送
**000032: Get Descriptor Request (DOWN), 18.10.2009 09:53:28.9375000 +0.0**Descriptor Type: Configuration
Descriptor Index: 0x0
Transfer Buffer Size: 0x22 bytes
设备响应
**000033: Control Transfer (UP), 18.10.2009 09:53:28.9531250 +0.0156250**Pipe Handle: 0x89457768
|09 02 22 00 01 01 00 A0 31 09 04 00 00 01 03 01 02 00 09 21 10 01 00 01 22 48 00 07 05 81 03 04 00 0A|..".... 1..........!...."H.......|
Setup Packet
|80 06 00 02 00 00 22 00|€.....".|
Recipient: Device
Request Type: Standard
Direction: Device->Host
Request: 0x6 (GET_CONFIGURATION)
Value: 0x200
Index: 0x0
Length: 0x22
80 06 00 02 00 00 22 00，GET_DESCRIPTOR命令，要求还是要求02，配置描述符，但是长度变成了0x22=34个字节
还好，USB monitor帮我们分了一下，响应数据一共有4个部分,因为第一个字节总是长度嘛，还是好分的
09 02 22 00 01 01 00 A0 31 
09 04 00 00 01 03 01 02 00 
09 21 10 01 00 01 22 48 00 
07 05 81 03 04 00 0A
第一部分还是刚才第二个回合返回的配置描述符
第二部分09 04 00 00 01 03 01 02 00 是接口描述符（Interface Descriptor），见表10
bLength：09
bDescriptorType：04
bInterfaceNumber：00 ，当前配置的是0号接口（第一个接口）
bAlternateSetting：00，可选设置的索引值，还不清楚具体意义
bNumEndpoints:01,此接口用的端点数量为1
bInterfaceClass：03 ，表明是USB HID设备
bInterfaceSubClass：01，为1表示HID设备符是一个启动设备（Boot Device，一般对PC机而言才有意义，意思是BIOS启动时能识别并使用您的HID设备，且只有标准鼠标或键盘类设备才能成为Boot Device）
bInterfaceProtocol：02 ，表示是USB鼠标，感觉网上有的资料把鼠标和键盘的bInterfaceProtocol搞反了
iInterface：00，又是一个字符串索引
第三部分09 21 10 01 00 01 22 48 00 是人机接口类描述符（HID）
参见[http://www.baiheee.com/Documents/090522/090522165226.htm](http://www.baiheee.com/Documents/090522/090522165226.htm)
bLength：09
bDescriptorType：21
bcdHID：10 01 ，0x0110，USB1.1版本
bCountryCode：00，不区分国家地区
bNumDescritors：01，支持的附属描述符数目1
bDescriptorType：22，HID相关描述符的类型，0x22是报表描述符
wDescriptorLength：48 00  ，报告描述符总长度
第四部分07 05 81 03 04 00 0A是端点描述符（EndPont Descriptor）
有必要提一下，端点0是默认必备的控制管道，同时只有端点0是双向的
bLength：07
bDescriptorType：05
bEndpointAddress：81，端点地址0x1，方向从输入主机
bmAttributes：03 ，位图，中断传送
wMaxPacketSize：04 00 ，当前配置下此端点能够接收或发送的最[大数据](http://lib.csdn.net/base/hadoop)包的大小为4，那后面就好分析了
bInterval：0A，就是说10ms发生一次中断
三个回合结束，主机终于对设备有了深刻的理解
接下来主机开始设置设备，可恶的USB Monitor居然没有把数据显示出来
只能猜测是 00 09 01 00  00 00 00 00  
**000034: Select Configuration (DOWN), 18.10.2009 09:53:28.9531250 +0.0**Configuration Index: 1
**000035: Select Configuration (UP), 18.10.2009 09:53:28.9687500 +0.0156250**Configuration Index: 1
Configuration Handle: 0x88e3f880
第五个回合继续设置接口配置，很奇怪的包
**000036: Class-Specific Request (DOWN), 18.10.2009 09:53:28.9687500 +0.0**Destination: Inrterface, Index 0
Reserved Bits: 34
Request: 0xa
Value: 0x0
Send 0x0 bytes to the device
**000037: Control Transfer (UP), 18.10.2009 09:53:28.9687500 +0.0**Pipe Handle: 0x89457768
 Setup Packet
|21 0A 00 00 00 00 00 00|!.......|
Recipient: Inrterface
Request Type: Class
Direction: Host->Device
Request: 0xa (Unknown)
Value: 0x0
Index: 0x0
Length: 0x0
21 0A 00 00 00 00 00 00，请求0a，代表命令GET_INTERFACE，
wIndex：00，使用0号接口
第六个回合，居然又要**Get Descriptor ，这次还要0x88个这么多**
**000038: Get Descriptor Request (DOWN), 18.10.2009 09:53:28.9687500 +0.0**Descriptor Type: Unknown
Descriptor Index: 0x0
Transfer Buffer Size: 0x88 bytes
**000039: Control Transfer (UP), 18.10.2009 09:53:28.9843750 +0.0156250**Pipe Handle: 0x89457768
|05 01 09 02 A1 01 05 09 19 01 29 03 15 00 25 01 95 03 75 01 81 02 95 01 75 05 81 03 05 01 09 01 A1 00 09 30 09 31 15 81 25 7F 75 08 95 02 81 06 C0 09 38 95 01 81 06 09 3C 15 00 25 01 75 01 95 01 B1 22 95 07 B1 01 C0|....¡.....)...%.•.u..•.u......¡..0.1.%u.•..À.8•...<..%.u.•.±"•.±.À|
Setup Packet
|81 06 00 22 00 00 88 00|.."..ˆ.|
Recipient: Inrterface
Request Type: Standard
Direction: Device->Host
Request: 0x6 (GET_CONFIGURATION)
Value: 0x2200
Index: 0x0
Length: 0x88
81 06 00 22 00 00 88 00，完全不知道81 06 是不是合法组合，00 22 居然没有找到这样的描述符类型，
前面提到过一个HID相关描述符的类型是0x22，那么返回的就是HID设备描述符
[http://blog.csdn.net/xianfengdesign/archive/2007/11/12/1881211.aspx](http://blog.csdn.net/xianfengdesign/archive/2007/11/12/1881211.aspx)
拿电脑圈圈的一个HID设备描述符来说明下吧
下面这个报告描述符是USB鼠标报告描述符，比起键盘的来说要简单些。
它描述了4个字节，第一个字节表示按键，第二个字节表示x轴（即鼠标左右移动，
0表示不动，正值表示往右移，负值表示往左移），第三个字节表示y轴（即鼠标
上下移动，0表示不动，正值表示往下移动，负值表示往上移动），第四个字节
表示鼠标滚轮（正值为往上滚动，负值为往下滚动）。
code char MouseReportDescriptor[52] = {
    //通用桌面设备
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    //鼠标
    0x09, 0x02,                    // USAGE (Mouse)
    //集合
    0xa1, 0x01,                    // COLLECTION (Application)
    //指针设备
    0x09, 0x01,                    //   USAGE (Pointer)
    //集合
    0xa1, 0x00,                    //   COLLECTION (Physical)
    //按键
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    //使用最小值1
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    //使用最小值3。1表示左键，2表示右键，3表示中键
    0x29, 0x03,                    //     USAGE_MAXIMUM (Button 3)
    //逻辑最小值0
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    //逻辑最大值1
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    //数量为3
    0x95, 0x03,                    //     REPORT_COUNT (3)
    //大小为1bit
    0x75, 0x01,                    //     REPORT_SIZE (1)
    //输入，变量，数值，绝对值
    //以上3个bit分别表示鼠标的三个按键情况，最低位（bit-0）为左键
    //bit-1为右键，bit-2为中键，按下时对应的位值为1，释放时对应的值为0
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    //填充5个bit，补足一个字节
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x75, 0x05,                    //     REPORT_SIZE (5)
    0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
    //用途页为通用桌面
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    //用途为X
    0x09, 0x30,                    //     USAGE (X)
    //用途为Y
    0x09, 0x31,                    //     USAGE (Y)
    //用途为滚轮
    0x09, 0x38,                    //     USAGE (Wheel)
    //逻辑最小值为-127
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    //逻辑最大值为+127
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
    //大小为8个bits
    0x75, 0x08,                    //     REPORT_SIZE (8)
    //数量为3个，即分别代表x,y,滚轮
    0x95, 0x03,                    //     REPORT_COUNT (3)
    //输入，变量，值，相对值
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
    //关集合
    0xc0,                          //   END_COLLECTION
    0xc0                           // END_COLLECTION
};
接下来是两个pnp事件
**000288: PnP Event: Query ID (UP), 18.10.2009 12:45:53.3906250 +0.0**Device ID: USB/Vid_15ca&Pid_00c3
**000289: PnP Event: Query ID (UP), 18.10.2009 12:45:53.3906250 +0.0**Hardware IDs: USB/Vid_15ca&Pid_00c3&Rev_0512, USB/Vid_15ca&Pid_00c3
然后鼠标就可以用了
**000290: Bulk or Interrupt Transfer (UP), 18.10.2009 12:54:42.8750000 +529.4843750**Pipe Handle: 0x88d298bc (Endpoint Address: 0x81)
Get 0x4 bytes from the device:
|00 01 FE 00|..þ.|
每个鼠标中断发送的数据果然很少，只有4个字节，分别对应按键、x轴、y轴、滚轮。
至此USB鼠标数据流就分析完了，好累。

