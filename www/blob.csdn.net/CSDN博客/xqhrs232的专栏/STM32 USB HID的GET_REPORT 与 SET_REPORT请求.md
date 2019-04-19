# STM32 USB HID的GET_REPORT 与 SET_REPORT请求 - xqhrs232的专栏 - CSDN博客
2018年03月12日 13:48:30[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：953
原文地址::[http://blog.csdn.net/shench88/article/details/26719121](http://blog.csdn.net/shench88/article/details/26719121)
相关文章
1、usb通过getreport和setreport发送数据怎么设置----[https://zhidao.baidu.com/question/1431521827822950619.html](https://zhidao.baidu.com/question/1431521827822950619.html)
2、自定义hid设备set_report设置和get_report获取报告描述符控制8字节数据 ----[http://zhangzhian.blog.163.com/blog/static/23589602010426103338869/](http://zhangzhian.blog.163.com/blog/static/23589602010426103338869/)
3、USB之HID类Set_Report Request[调试手记1]----[http://www.cnblogs.com/libra13179/p/7365910.html](http://www.cnblogs.com/libra13179/p/7365910.html)
4、[使用STM32CubeMX生成USB_HOST_HID工程[添加对CAPS_LOCK指示灯的控制][SetReport]](http://www.cnblogs.com/libra13179/p/7373217.html)----[http://www.cnblogs.com/libra13179/p/7373217.html](http://www.cnblogs.com/libra13179/p/7373217.html)
5、在USB键盘中取得SET REPORT的作法----[http://bbs.21ic.com/icview-1076414-1-1.html](http://bbs.21ic.com/icview-1076414-1-1.html)
6、在USB KEYBOARD下，结合SET_REPORT及GET_REPORT的传输方式 ----[http://bbs.21ic.com/icview-955196-1-1.html](http://bbs.21ic.com/icview-955196-1-1.html)
7、头大, NUC1xx USB HID Set Report数据不对 [问题解决]----[http://bbs.ic37.com/bbsview-34051.htm](http://bbs.ic37.com/bbsview-34051.htm)
1）在usb_prop.c的函数CustomHID_Data_Setup()中，GET_PROTOCOL之后增加如下代码：
  /*** GET_PROTOCOL ***/
  else if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
           && RequestNo == GET_PROTOCOL)
  {
    CopyRoutine = CustomHID_GetProtocolValue;
  } /* 以上代码为原有例程中的代码 */
  /*** GET_REPORT ***/ /* 以下代码支持GET_REPORT */
  else if((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
        && RequestNo == GET_REPORT)
  {
    CopyRoutine = CustomHID_GetReport;
  }
2）再增加一个函数：
uint8_t *CustomHID_GetReport(uint16_t Length)
{
  if (Length == 0)
  {
    return 10;
  }
  return &Report_Buffer[pInformation->Ctrl_Info.Usb_wOffset];
}
3）保证在IN Token到来之前，Report_Buffer中包含有效的数据。
上述过程十分简单，但有时不能保证在IN Token到来之前，Report_Buffer中包含有效的数据，即要发送的数据还没有准备好；这时就希望先设置一个标志，让IN Token返回NAK，让CustomHID_Data_Setup()这个函数返回，待数据准备好之后再继续前述的数据拷贝过程。
这个要求使用STM32的USB开发库也是很容易实现的：
1）在usb_prop.c的函数CustomHID_Data_Setup()中，GET_PROTOCOL之后增加的代码变为这样：
  /*** GET_REPORT ***/ 
  else if((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)) 
        && RequestNo == GET_REPORT) 
  { 
    if (数据未准备好) 
      return USB_NOT_READY; 
    CopyRoutine = CustomHID_GetReport; 
  } 
这样，STM32将以NAK回应PC端的IN请求。
2）同样增加一个函数CustomHID_GetReport。
3）在数据准备好之后（注意准备数据的时间不能太长，否则PC端会做超时处理），执行下述操作： 
    pInformation->Ctrl_Info.CopyData = CustomHID_GetReport; 
    pInformation->Ctrl_Info.Usb_wOffset = 0; 
    CustomHID_GetReport(0); 
    pInformation->Ctrl_Info.PacketSize = pProperty->MaxPacketSize; 
    DataStageIn();
