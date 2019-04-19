# 以HID的SET REPORT为例说明如何使用STM32的USB库支持控制端点0 - xqhrs232的专栏 - CSDN博客
2018年03月13日 11:24:12[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：518
原文地址::[http://longer.spaces.eepw.com.cn/articles/article/item/58628](http://longer.spaces.eepw.com.cn/articles/article/item/58628)
相关文章
1、Set_Report请求展示一个PS2 转 USB键盘范例----[http://share.eepw.com.cn/share/download/id/70429#share_fb_down](http://share.eepw.com.cn/share/download/id/70429#share_fb_down)
2、STM32 USB 问题汇总----[http://www.eeworld.com.cn/mcu/article_2016072727882.html](http://www.eeworld.com.cn/mcu/article_2016072727882.html)
3、USB之HID类Set_Report Request[调试手记1]----[http://www.bubuko.com/infodetail-2262262.html](http://www.bubuko.com/infodetail-2262262.html)
4、关于usb键盘
 set_report的问题求助！！----[http://archive.ednchina.com/group.ednchina.com/GROUP_MES_14600_93_37123.HTM](http://archive.ednchina.com/group.ednchina.com/GROUP_MES_14600_93_37123.HTM)
以HID的SET REPORT为例说明如何使用STM32的USB库支持控制端点0
首先我们先回顾一下控制端点的传输方式：
控制端点的传输有三个阶段，SETUP阶段、数据阶段和状态阶段；数据阶段又分为数据入(DATA IN)和数据出(DATA OUT)，控制端点传输可以没有数据阶段；状态阶段有状态入(STATUS IN)和状态出(STATUS OUT)。
总结起来，控制端点有如下三种可能的传输过程（以下括号中的0或1表示DATA0或DATA1传输）：
一、 SETUP  DATA_IN(0)  DATA_IN(1)  DATA_IN(0)  ......  STATUS_OUT(1)
二、 SETUP  DATA_OUT(0)  DATA_OUT(1)  DATA_OUT(0)  ...... STATUS_IN(1)
三、 SETUP  STATUS_IN(1)
这里做一个约定，把上述过程一定义为“数据入过程”，过程二定义为“数据出过程”，过程三定义为“无数据过程”。所有的USB控制端点的数据传输都可以而且只用这三种传输过程表示。HID的SET_REPORT是数据出过程，HID的GET_REPORT是数据入过程，USB的GET DEVICE DESCRIPTOR是数据入过程，USB的SET CONFIGURATION是无数据过程，等等。
接下来，我们看看STM32的USB库是如何处理控制端点0的传输。
根据USB协议，每个SETUP包都由8个字节构成，用户程序可以通过结构体Device_Info(类型DEVICE_INFO)访问SETUP包的数据，因为在整个的USB处理中都要用到结构体Device_Info的内容，库中定义了一个全局的指针pInformation指向这个结构体，用户可以通过这个指针访问结构体的内容。
对应SETUP包的8个字节，用户可以用下述方式访问：
  pInformation->USBbmRequestType (字节类型)
  pInformation->USBbRequest  (字节类型)
  pInformation->USBwValue  (双字节类型)
  pInformation->USBwIndex  (双字节类型)
  pInformation->USBwLength (双字节类型)
使用pInformation->USBwValue0访问wValue的低字节，pInformation->USBwValue1访问wValue的高字节。
使用pInformation->USBwIndex0访问USBwIndex的低字节，pInformation->USBwIndex1访问USBwIndex的高字节。
使用pInformation->USBwLength0访问USBwLength的低字节，pInformation->USBwLength1访问USBwLength的高字节。
通过分析SETUP包的8个字节，可以判断出一个SETUP的传输过程是属于数据入过程、数据出过程还是无数据过程。STM32的USB库中处理了所有的USB协议文本中定义的标准SETUP命令，对于USB协议文本中未定义的命令，USB库按照数据入过程、数据出过程或无数据过程通过回调函数交给用户程序处理。
全局变量Device_Property(DEVICE_PROP类型)封装了所有的回调函数，DEVICE_PROP定义如下：
typedef struct _DEVICE_PROP
{
  void (*Init)(void);        // 设备初始化回调函数
  void (*Reset)(void);       // USB复位回调函数
  void (*Process_Status_IN)(void);  // STATUS_IN阶段处理回调函数
  void (*Process_Status_OUT)(void); // STATUS_OUT阶段处理回调函数
  RESULT (*Class_Data_Setup)(u8 RequestNo);   // 数据入/出过程处理回调函数
  RESULT (*Class_NoData_Setup)(u8 RequestNo); // 无数据过程处理回调函数
  RESULT  (*Class_Get_Interface_Setting)(u8 Interface, u8 AlternateSetting); // GET_INTERFACE 回调函数
  u8* (*GetDeviceDescriptor)(u16 Length); // GET_DEVICE_DESCRIPTION回调函数
  u8* (*GetConfigDescriptor)(u16 Length); // GET_CONFIGURATION_DESCRIPTION回调函数
  u8* (*GetStringDescriptor)(u16 Length); // GET_STRING_DESCRIPTION回调函数
  u8 MaxPacketSize; // 最大包长度
} DEVICE_PROP;
结合SETUP的三种传输过程，用户通过实现不同的回调函数即可完成对各种USB类命令的处理，下面以HID的SET REPORT为例说明。
在介绍具体实现之前，先介绍一下另一个回调函数CopyRoutine的概念，这个函数的原型是：
   u8 *CopyRoutine(u16 length);    // 返回一个缓冲区指针
USB库通过这个函数获得用户的数据缓冲区地址，从而可以在数据出过程中把收到的数据拷贝到用户缓冲区，或在数据入过程中把用户缓冲区的数据拷贝到USB发送缓冲区。每个数据出过程可能有若干次DATA_OUT传输，USB库每完成一次这样的传输都会调用一次回调函数CopyRoutine，参数length是本次传输所收到的数据字节数目，CopyRoutine必须返回一个缓冲区指针，这个缓冲区必须能够容纳length字节的数据，CopyRoutine返回到USB库之后，USB库将把收到的数据拷贝到用户指定的缓冲区。同样每个数据入过程也可能有若干次DATA_IN传输，每次需要向主机传输数据时，USB库都会调用一次回调函数CopyRoutine，参数length是本次传输所要发送的数据字节数目，CopyRoutine必须返回一个缓冲区指针，这个缓冲区中必须包含要求的数据字节，USB库将把用户缓冲区的数据拷贝到USB缓冲区并择机发送出去。
当以length=0调用CopyRoutine时，CopyRoutine需要返回用户缓冲区的长度，因为CopyRoutine的返回类型是一个指针，所以需要通过类型的强制转换返回缓冲区长度。这个功能是为了处理用户缓冲区的长度与主机SETUP数据请求长度不符的情况，而不至于造成用户缓冲区的溢出。
介绍完上述若干概念和回调函数，再看SET_REPORT的实现就很容易了。
SET_REPORT是一个数据出过程，因此需要实现一个Class_Data_Setup回调函数，示例如下：
RESULT HID_Data_Setup(u8 RequestNo)
{
    u8 *(*CopyRoutine)(u16 length);
    CopyRoutine = NULL;
    if (pInformation->USBbmRequestType == CLASS_REQUEST|INTERFACE_RECIPIENT
            && RequestNo == SET_REPORT)
        CopyRoutine = My_Data_Request;
    if (CopyRoutine == NULL)
        return USB_UNSUPPORT;
    pInformation->Ctrl_Info.CopyData = CopyRoutine;
    pInformation->Ctrl_Info.Usb_wOffset = 0;
    pInformation->Usb_wLength = (*CopyRoutine)(0);
    return USB_SUCCESS;
} // End of HID_Data_Setup()
u8 My_Buffer[10];
u8 *My_Data_Request(u16 length)
{
    if (length == 0)
        return (u8*)10;    // 假定你的REPORT长度和Buffer长度为10
    return My_Buffer;
}
上面介绍的CopyRoutine用于把多次传输的数据包合并到一个完整的缓冲区中，因此只有到STATUS阶段才能够指导一次SETUP传输是否结束，所以用户程序需要在回调函数Process_Status_IN中处理从SET_REPORT接收到的数据。因为所有的回调函数都是USB中断处理的一部分，所以更好的办法是在Process_Status_IN中设置一个标记，然后在用户主程序中判断这个标记并做处理。
注意，STM32的USB库设计成以回调函数处理用户命令请求，包含类命令请求，是为了能够清晰地区分库程序和用户程序，使这两者不会混在一起，这样的好处是非常明显的，当USB库需要更新升级时，只需替换掉相应的程序模块，而不必修改用户已经完成的程序。
以上的介绍都可以在STM32 USB库的说明手册中找到。
上述示意代码是以My_Buffer长度为10字节为例，而USB库的默认包长度为16字节，因此My_Data_Request并没有多包的处理。
关于多包的缓冲区处理的示意代码可以是这样的：
u8 *My_Data_Request(u16 length)
{
    if (length == 0)
        return (u8*)100;    // 假定你的REPORT长度和Buffer长度为100
    return &My_Buffer[pInformation->Ctrl_Info.Usb_wOffset];
}
这里有一个库中使用的变量pInformation->Ctrl_Info.Usb_wOffset，这个变量回在传输每个数据包时候由库中的程序按数据包长度增加，如最大包长为16字节时，第一次调用My_Data_Request时Usb_wOffset=0，第二次调用My_Data_Request时Usb_wOffset=16，第三次调用My_Data_Request时Usb_wOffset=32，依此类推。这样就可以使用Usb_wOffset作为My_Buffer的下标从My_Data_Request返回。
对于提问“如何传递length？在上面没有看到这个参数的传递过程”的回答：
参数length是用于检测缓冲区长度是否足够，如果你有足够长的缓冲区，可以不必检测，上述示例中使用了一个固定的缓冲区，所以不必使用参数length检测缓冲区长度。

