# STM32F103C8T6使用MDK 4.12中Custom_HID的修改方法 - xqhrs232的专栏 - CSDN博客
2018年02月09日 23:05:25[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：693
原文地址::[http://bh7kqk.blog.163.com/blog/static/127412291201272312850693/](http://bh7kqk.blog.163.com/blog/static/127412291201272312850693/)
相关文章
1、[[讨论]](http://bbs.eeworld.com.cn/forum.php?mod=forumdisplay&fid=126&filter=typeid&typeid=171)stm32 USB-HID移植----[http://bbs.eeworld.com.cn/thread-427309-1-1.html](http://bbs.eeworld.com.cn/thread-427309-1-1.html)
2、**stm32f103 usb驱动 电脑无法识别的解决办法----[http://blog.csdn.net/wenyuexunyin/article/details/65631076](http://blog.csdn.net/wenyuexunyin/article/details/65631076)**
3、STM32 USB软件移植注意事项 及硬件接线注意----[http://blog.csdn.net/agave7/article/details/43524975](http://blog.csdn.net/agave7/article/details/43524975)
4、[STM32F103实现HID键盘](http://jamsan.blog.hexun.com/83079360_d.html) ----[http://jamsan.blog.hexun.com/83079360_d.html](http://jamsan.blog.hexun.com/83079360_d.html)
STM32F103C8T6使用MDK 4.12中Custom_HID的修改方法
目的，由上位机发送64字节，设备回应3个字节，功能自定。。
板子使用的是最小核心板，MCU为STM32F103C8T6。
USB的D-和D+分别接到PA11(USBDM)和PA12(USBDP)上，PA12(USBDP)即D+通过跳线使上拉1.5K电阻到VCC（3.3V）。
1：打开MDK4.12，打开
C:\Keil\ARM\Examples\ST\STM32F10xUSBLib\Demos\Custom_HID\project目录下的Custom_HID项目文件。
2：选择菜单 Project->Options for Target 'STM3210E-EVAL'
在Device选项卡里选择 STM32F103C8。
在Output选项卡里把Create HEX File选上，取消Browse Information。
在C/C++选项卡里的Preprocessor Symbols里的Define: USE_STM3210E_EVAL清除。
3：从Application files组中的文件由上而下逐个修改
第一个是usb_pwr.c
把PowerOn和PowerOff()面里的USB_Cable_Config(ENABLE);和USB_Cable_Config(DISABLE);这两句删除，因为我的板子上没有这个控制电路，
而是通过跳线把USBDP通过1.5K电阻接通到VCC（3.3V）的。
接下来修改hw_config.c
把
#define ADC1_DR_Address    ((u32)0x4001244C)
u32 ADC_ConvertedValueX = 0;
u32 ADC_ConvertedValueX_1 = 0;
这三句删除，因为我不用ADC,外部中断和DMA,只需要跟上位机通信点亮板上连接到PB0上的一个LED。
所以还要把Set_System函数里的
EXTI_Configuration();
ADC_Configuration();
这两句删除。
把USB_Interrupts_Config函数里的
EXTI9_5 Interrupt
EXTI15_10 Interrupt
DMA1 Channel1 Interrupt
这三个相关的语句都删除。
把USB_Cable_Config整个函数删除。
把GPIO_Configuration里面的
KEY
Tamper
Potentiometer
三个相关的配置语句都删除。
下面的LED配置只配置LED1，用来指示上位机的指令，其它都删除。
把
EXTI_Configuration
ADC_Configuration
这两个函数整个删除。
加上这句RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE); 别忘了开CRC电源。计算序列号要用到的，否则序列号就会变成42949-67295，是0xffffffff，是错误的。
把Get_SerialNum函数的功能改成如下
目的是把96位ID作CRC运算后来做产品的序列号。
可以不要这个函数，只在usb_desc.c里定义序列号。同时其它文件里用到这个函数的地方就要删除。
增加RBIT这个函数，32位位倒置程序，CRC用到的。因为STM32的CRC计算结果跟一般的CRC校验工具计算结果不同，所以要特殊处理。
如
原始序列号: 39006C065834313149670543
分成三个32位数。每一个32位按位倒置，然后给STM32计算CRC，结果跟0xffffffff异或运算后，再把32位的结果按位倒置。就对了。
39006C06=00111001000000000110110000000110 = 01100000001101100000000010011100 = 6036009C
58343131=01011000001101000011000100110001 = 10001100100011000010110000011010 = 8C8C2C1A
49670543=01001001011001110000010101000011 = 11000010101000001110011010010010 = C2A0E692
STM32的CRC结果跟0xffffffff后为十进制4289354847=FFAA5C5F=11111111101010100101110001011111=倒过来
11111010001110100101010111111111=FA3A55FF
__asm u32 RBIT(u32 value)
{
    rbit r0, r0
    bx lr
}
void Get_SerialNum(void)
{
  u32 Device_Serial, Device_Serial0, Device_Serial1, Device_Serial2;
  Device_Serial0 = *(u32*)(0x1FFFF7E8);
  Device_Serial0 = RBIT(Device_Serial0);
  Device_Serial1 = *(u32*)(0x1FFFF7EC);
  Device_Serial1 = RBIT(Device_Serial1);
  Device_Serial2 = *(u32*)(0x1FFFF7F0);
  Device_Serial2 = RBIT(Device_Serial2);
  CRC_ResetDR();
  Device_Serial = CRC_CalcCRC(Device_Serial0);
  Device_Serial = CRC_CalcCRC(Device_Serial1);
  Device_Serial = CRC_CalcCRC(Device_Serial2);
  Device_Serial ^= 0xFFFFFFFF;
  Device_Serial = RBIT(Device_Serial);
  if(Device_Serial != 0)
  {
  CustomHID_StringSerial[0] = CUSTOMHID_SIZ_STRING_SERIAL;
  CustomHID_StringSerial[1] = USB_STRING_DESCRIPTOR_TYPE;
  CustomHID_StringSerial[2] = (u8)(Device_Serial/(u32)1000000000)+0x30;
  CustomHID_StringSerial[3] = 0x00;
  CustomHID_StringSerial[4] = (u8)((Device_Serial%(u32)1000000000)/(u32)100000000)+0x30;
  CustomHID_StringSerial[5] = 0x00;
  CustomHID_StringSerial[6] = (u8)((Device_Serial%(u32)100000000)/(u32)10000000)+0x30;
  CustomHID_StringSerial[7] = 0x00;
  CustomHID_StringSerial[8] = (u8)((Device_Serial%(u32)10000000)/(u32)1000000)+0x30;
  CustomHID_StringSerial[9] = 0x00;
  CustomHID_StringSerial[10] = (u8)((Device_Serial%(u32)1000000)/(u32)100000)+0x30;
  CustomHID_StringSerial[11] = 0x00;
  CustomHID_StringSerial[12] = '-';
  CustomHID_StringSerial[13] = 0x00;
  CustomHID_StringSerial[14] = (u8)((Device_Serial%(u32)100000)/(u32)10000)+0x30;
  CustomHID_StringSerial[15] = 0x00;
  CustomHID_StringSerial[16] = (u8)((Device_Serial%(u32)10000)/(u32)1000)+0x30;
  CustomHID_StringSerial[17] = 0x00;
  CustomHID_StringSerial[18] = (u8)((Device_Serial%(u32)1000)/(u32)100)+0x30;
  CustomHID_StringSerial[19] = 0x00;
  CustomHID_StringSerial[20] = (u8)((Device_Serial%(u32)100)/(u32)10)+0x30;
  CustomHID_StringSerial[21] = 0x00;
  CustomHID_StringSerial[22] = (u8)((Device_Serial%(u32)10)/(u32)1)+0x30;
  CustomHID_StringSerial[23] = 0x00;
  }   
}
然后修改对应的 hw_config.h
void USB_Cable_Config (FunctionalState NewState);
void EXTI_Configuration(void);
void ADC_Configuration(void);
把这三条删除。
Get_SerialNum也视情况决定要不要删除。
main.c不动。
接下来修改 stm32f10x_it.c
把u8 Send_Buffer[2]; 这句删除。
把
extern u32 ADC_ConvertedValueX;
extern u32 ADC_ConvertedValueX_1;
删除。
把
DMA1_Channel1_IRQHandler
EXTI9_5_IRQHandler
EXTI15_10_IRQHandler
这三个函数清空，是清空里面的内容，保留一个空的函数。
接下来改 usb_desc.c
把报告描述符改成自已的：
/* CustomHID_ConfigDescriptor */
const u8 CustomHID_ReportDescriptor[CUSTOMHID_SIZ_REPORT_DESC] =
  {    
    0x05, 0x01,                    // USAGE_PAGE(User define)
    0x09, 0x00,                    // USAGE(User define)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x19, 0x00,                    //   USAGE_MINIMUM(0)
    0x29, 0xFF,                    //   USAGE_MAXIMUM(255)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0xFF,                    //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x03,                    //   REPORT_COUNT (3)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x05, 0x02,                    // USAGE_PAGE(2)
    0x19, 0x00,                    //   USAGE_MINIMUM (0)
    0x29, 0xFF,                    //   USAGE_MAXIMUM (255)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0xFF,                    //   LOGICAL_MAXIMUM (255)
    0x95, 0x40,                    //   REPORT_COUNT (8)
    0x75, 0x08,                    //   REPORT_SIZE (64)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0xc0            /*     END_COLLECTION              */
  }; /* CustomHID_ReportDescriptor */
然后更改配置描述符中的输入输出端点的最大包数。输入端点改成3个字节，输出端点改成64字节。
再来改下面三个字符串描述符，使用的是UNICODE格式的，用记事本写好保存为UNICODE格式文件，再用十六进制编辑器打开就看到了。
改完以上的数据，还要更改usb_desc.h里的定义的大小。(注意字节数还包括描述符长度及标识),如上面的报告描述符大小应改成
#define CUSTOMHID_SIZ_REPORT_DESC 37
接下来改usb_endp.c
把
u8 Receive_Buffer[2];
改成
u8 Receive_Buffer[64]; //因本例中要求上位机发来64字节。
然后是EP1_OUT_Callback这个函数的修改，根据上位机发来的数据，作出相应回应。
usb_istr.c不用改。
然后改usb_prop.c
把CustomHID_Reset函数里的
  SetEPTxCount(ENDP1, 2);
  SetEPRxCount(ENDP1, 2);
改成
  SetEPTxCount(ENDP1, 3);
  SetEPRxCount(ENDP1, 64);
CustomHID_init函数里调用了Get_SerialNum函数，如果使用usb_desc.c里定义的产品序列号，这里也不用了。。
还要修改platform_config.h
把修改LED引脚，USB引脚的定义。
终于改完了。。
以下是记录下来的发送程序段。
u8 Send_Buffer[3];
Send_Buffer[0] = 0xxx;
Send_Buffer[1] = 0xxx;
Send_Buffer[2] = 0xxx;
UserToPMABufferCopy(Send_Buffer, ENDP1_TXADDR, 3);
SetEPTxValid(ENDP1);
最后要还说一说跟上位机通信要注意的地方，上位机程序要限制只能用运行一个实例，可以用互斥量。运行多个程序会导致USB不知应该跟哪个
程序通信，数据会错误。
注意程序关闭再打开后要获得设备的状态。而设备复位后也要通知上位机，以保证两者状态一致。
上位机程序的读写缓冲都要比设备定义的大一个字节（开始位置，0x00)。这个字节由电脑发到设备中会被去掉，所以设备端不用管。
电脑接收到的数据也比设备发来的多一个字节，也要处理掉。。。
还有ANSI跟UNICODE的处理，要清楚。如字符串描述符中的字符串的显示等。
STM32 USB软件移植注意事项 及硬件接线注意
