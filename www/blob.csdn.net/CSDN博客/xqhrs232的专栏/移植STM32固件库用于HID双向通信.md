# 移植STM32固件库用于HID双向通信 - xqhrs232的专栏 - CSDN博客
2018年02月08日 18:28:30[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：109
个人分类：[PS2/USB/HID技术](https://blog.csdn.net/xqhrs232/article/category/7431349)
原文地址::[http://blog.csdn.net/madrabbit1987/article/details/67690661](http://blog.csdn.net/madrabbit1987/article/details/67690661)
USB的应用中HID类是比较常见的方式。通过修改STM32 USB固件库V4.0的JOYSTICK应用，我们实现一个双向USB通信。
**一、移植**
使用STM32源程序为点亮LED灯程序。
首先将USB固件库中有用的函数复制到源函数中，建立LIB文件夹其中放入USB2.0协议函数
![](https://img-blog.csdn.net/20160623170054462?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
建立CFG文件夹放入USB应用函数
![](https://img-blog.csdn.net/20160623170134290?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
将两个文件夹都放到源工程目录下将文件添加进来，设置好，配置好KEIL软件设置。
**二、修改文件**
1、首先修改platform_config.h函数。
    该文件是对于多种芯片对于USB库的支持。我们使用STM32F103ZET6芯片，所以只保留与之相关的ID项，与USB_DISCONNECT线（PG11）的配置。修改之后的头文件如下所示
![](https://img-blog.csdn.net/20160623170221853?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2、然后修改hw_config.c中Set_System函数
    实际上STM32F103zet6不需要对USB端口进行单独的配置，删掉Set_System函数中没用的部分，只对USB_DISCONNECT线（PG11）进行初始化。修改后的函数如下图所示。
![](https://img-blog.csdn.net/20160623170257025?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3、接下来修改hw_config.c文件中USB_Cable_Config函数因为我使用的是比较老的神舟III开发板，当PG11为高电平时实现上拉，所以修改后的函数为
![](https://img-blog.csdn.net/20160623170331365?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4、修改USB_Interrupt_Config函数，配置USB_LP_CAN1_RX0_IRQn和USBWakeUp_IRQn中断修改后的函数如下图所示。
![](https://img-blog.csdn.net/20160623170407947?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
5、接下来删掉hw_config.c文件中GPIO_AINConfig函数，没有什么用，只会报错
6、删掉与按键相关的设置，因为我们并没有用到，主要是先清除JoyState和Joystick_Send函数中内容，不用管。
7、最后我们处理USB挂起相关的问题。在USB固件库中提供挂起相关的处理函数，主要有2个函数需要修改。它们是Suspend函数和Enter_LowPowerMode函数。其中，在Suspend函数中注释掉PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);语句。修改Suspend函数，如下图所示
![](https://img-blog.csdn.net/20160623170441994?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
OK接下来应该就没有什么需要修改的地方了（如果有的话请酌情修改）
**三、描述符修改**
使用USB HID类进行通信调试。所以要对USB的描述符要做部分修改（usb_desc.c）。下面我们贴程序说明（只对重要修改做说明）
设备描述符
修改idVendor和idProduct的值，为任意其他值，我里我用给的是0x0413和0x5724，修改的时候注意大小端
配置描述符集合
**[cpp]**[view plain](http://blog.csdn.net/a827415225/article/details/51745013#)[copy](http://blog.csdn.net/a827415225/article/details/51745013#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1727041/fork)
- <span style="font-family:SimSun;font-size:10px;">const uint8_t Joystick_ConfigDescriptor[JOYSTICK_SIZ_CONFIG_DESC] =  
-   {  
-     0x09, /* bLength: Configuration Descriptor size */
-     USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
-     JOYSTICK_SIZ_CONFIG_DESC,  
- /* wTotalLength: Bytes returned */
-     0x00,  
-     0x01,         /*bNumInterfaces: 1 interface*/
-     0x01,         /*bConfigurationValue: Configuration value*/
-     0x00,         /*iConfiguration: Index of string descriptor describing
-                                      the configuration*/
-     0xE0,         /*bmAttributes: Self powered */
-     0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/
- 
- /************** Descriptor of Joystick Mouse interface ****************/
- /* 09 */
-     0x09,         /*bLength: Interface Descriptor size*/
-     USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type*/
-     0x00,         /*bInterfaceNumber: Number of Interface*/
-     0x00,         /*bAlternateSetting: Alternate setting*/
-     0x02,         /*bNumEndpoints除端点0外，需要1输入1输出*/
-     0x03,         /*bInterfaceClass: HID*/
-     0x00,         /*bInterfaceSubClass : 1=BOOT, 0=no boot选择no boot*/
-     0x00,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse选择none*/
-     0,            /*iInterface: Index of string descriptor*/
- /******************** Descriptor of Joystick Mouse HID ********************/
- /* 18 */
-     0x09,         /*bLength: HID Descriptor size*/
-     HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
-     0x00,         /*bcdHID: HID Class Spec release number*/
-     0x01,  
-     0x00,         /*bCountryCode: Hardware target country*/
-     0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
-     0x22,         /*bDescriptorType*/
-     JOYSTICK_SIZ_REPORT_DESC,/*wItemLength: Total length of Report descriptor*/
-     0x00,  
- /******************** Descriptor of Joystick Mouse endpoint ********************/
- /* 27 */
-     0x07,          /*bLength: Endpoint Descriptor size*/
-     USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/
- 
-     0x81,          /*bEndpointAddress: Endpoint Address (IN)*/
-     0x03,          /*bmAttributes: Interrupt endpoint*/
-     0x40,          /*wMaxPacketSize: 64 Byte max修改最大包大小为64字节 */
-     0x00,  
-     0x0A,          /*bInterval: Polling Interval (10 ms)修改轮询间隔为10ms*/
- /******************** Descriptor of Joystick Mouse endpoint ********************/
- /* 34 */
-     0x07,          /*bLength: Endpoint Descriptor size*/
-     USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/
- 
-     0x01,          /*bEndpointAddress: Endpoint Address (OUT)*/
-     0x03,          /*bmAttributes: Interrupt endpoint*/
-     0x40,          /*wMaxPacketSize: 64 Byte max修改最大包大小为64字节 */
-     0x00,  
-     0x20,          /*bInterval: Polling Interval (10 ms) 修改轮询间隔为10ms */
- /*41 */
-   } ; </span><span style="font-family:SimSun;font-size:14px;">  
- </span>  
报告描述符
**[cpp]**[view plain](http://blog.csdn.net/a827415225/article/details/51745013#)[copy](http://blog.csdn.net/a827415225/article/details/51745013#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1727041/fork)
- <span style="font-family:SimSun;font-size:10px;">const uint8_t Joystick_ReportDescriptor[JOYSTICK_SIZ_REPORT_DESC] =  
-   {  
-     0x05, 0xFF,          /*Usage Page(User Define)*/
-     0x09, 0xFF,          /*Usage(User Define)*/
-     0xA1, 0x01,          /*Collection(application)*/
-     0x05, 0x01,          /*Usage Page(1)*/
-     0x19, 0x00,          /*Usage Minimum(0)*/
-     0x29, 0xFF,          /*Usage Maximum(255)*/
-     0x15, 0x00,          /*Logical Minimum(0)*/
-     0x25, 0xFF,          /*Logical Maximum(255)*/
-     0x95, 0x40,          /*Report Count(3)*/
-     0x75, 0x08,          /*Report Size(1)*/
- 0x81, 0x02,          /*Input(Data，Var，Abs)*/
- 0x05, 0x02          /*Usage Page(2)*/
- 0x19, 0x00,          /*Usage Minimum(0)*/
-     0x29, 0xFF,          /*Usage Maximum(255)*/
-     0x15, 0x00,          /*Logical Minimum(0)*/
-     0x25, 0xFF,          /*Logical Maximum(255)*/
-     0x95, 0x40,          /*Report Count(64)*/
-     0x75, 0x08,          /*Report Size(8)*/
- 0x91, 0x02,          /*Input(Data，Var，Abs)*/
- 0xc0               /*关集合*/
-   }; /* Joystick_ReportDescriptor */</span><span style="font-family:SimSun;font-size:14px;">  
- </span>  
上面我们修改了数组内容，导致数组大小发生变化，所以根据变化做响应修改，在usb_desc.c中
**[cpp]**[view plain](http://blog.csdn.net/a827415225/article/details/51745013#)[copy](http://blog.csdn.net/a827415225/article/details/51745013#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1727041/fork)
- <span style="font-family:SimSun;font-size:10px;">#define JOYSTICK_SIZ_CONFIG_DESC                41  
- #define JOYSTICK_SIZ_REPORT_DESC                39</span>
**四、修改函数**
    修改设备RESET函数Joystick_Reset，这个函数是在RESET中断中被调用，用于端口的初始化。因为我们增加了端点1的输出，修改了包大小，所以要做响应的修改。这个函数位于usb_prop.c中，修改后的函数为
**[cpp]**[view plain](http://blog.csdn.net/a827415225/article/details/51745013#)[copy](http://blog.csdn.net/a827415225/article/details/51745013#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1727041/fork)
- <span style="font-family:SimSun;font-size:10px;">void Joystick_Reset(void)  
- {  
- /* Set Joystick_DEVICE as not configured */
-   pInformation->Current_Configuration = 0;  
-   pInformation->Current_Interface = 0;/*the default Interface*/
- 
- /* Current Feature initialization */
-   pInformation->Current_Feature = Joystick_ConfigDescriptor[7];  
-   SetBTABLE(BTABLE_ADDRESS);  
- /* Initialize Endpoint 0 */
-   SetEPType(ENDP0, EP_CONTROL);  
-   SetEPTxStatus(ENDP0, EP_TX_STALL);  
-   SetEPRxAddr(ENDP0, ENDP0_RXADDR);  
-   SetEPTxAddr(ENDP0, ENDP0_TXADDR);  
-   Clear_Status_Out(ENDP0);  
-   SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);  
-   SetEPRxValid(ENDP0);  
- 
- /* Initialize Endpoint 1 */
-   SetEPType(ENDP1, EP_INTERRUPT);  
-   SetEPTxAddr(ENDP1, ENDP1_TXADDR);  
-   SetEPTxCount(ENDP1, 64);//修改大小为64
-   SetEPTxStatus(ENDP1, EP_TX_NAK);  
- 
- /* Initialize Endpoint 1 */
- //SetEPType(ENDP1, EP_INTERRUPT);
-   SetEPTxAddr(ENDP1, ENDP1_RXADDR);  
-   SetEPTxCount(ENDP1, 64); //修改大小为64
-   SetEPRxStatus(ENDP1, EP_RX_VALID);  
- /* Set this device to response on default address */
-   SetDeviceAddress(0);  
-   bDeviceState = ATTACHED;  
- }</span><span style="font-family:SimSun;font-size:14px;">  
- </span>  
在上面中ENDP1_RXADDR是我们新加的，需要我们在usb_conf.h中添加对于它的定义
**[cpp]**[view plain](http://blog.csdn.net/a827415225/article/details/51745013#)[copy](http://blog.csdn.net/a827415225/article/details/51745013#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1727041/fork)
- <span style="font-family:SimSun;font-size:10px;">/* EP1  */
- /* tx buffer base address */
- #define ENDP1_TXADDR        (0x100)
- #define ENDP1_RXADDR        (0x140)</span><span style="font-family:SimSun;font-size:14px;">
- </span>  
最后我们编写对于USB输出数据的处理函数，即
**[cpp]**[view plain](http://blog.csdn.net/a827415225/article/details/51745013#)[copy](http://blog.csdn.net/a827415225/article/details/51745013#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1727041/fork)
- <span style="font-family:SimSun;font-size:10px;">void EP1_OUT_Callback(void)  
- {  
-     DataLen = USB_SIL_Read(ENDP1,kk);//kk是个64字节的数组
- 
- if(kk[0]&&0x01 == 0x01)  
-         LED(LED1,1);  
- else
-         LED(LED1,0);  
- if(kk[0]&&0x02 == 0x02)  
-         LED(LED2,1);  
- else
-         LED(LED2,0);  
- if(kk[0]&&0x04 == 0x04)  
-         LED(LED3,1);  
- else
-         LED(LED3,0);  
- if(kk[0]&&0x08 == 0x08)  
-         LED(LED4,1);  
- else
-         LED(LED4,0);     
-     SetEPRxValid(ENDP1);  
- }</span><span style="font-family:SimSun;font-size:14px;">  
- </span>  
在主函数中我们添加USB IN的相关操作，因为设置的是interrupt端点，所以每过10ms主机就是问询设备是否有数据要发送，我们只需要在stm32的主函数中填装数据即可。具体的while循环中代码是
**[cpp]**[view plain](http://blog.csdn.net/a827415225/article/details/51745013#)[copy](http://blog.csdn.net/a827415225/article/details/51745013#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1727041/fork)
- <span style="font-family:SimSun;font-size:10px;">if(GetEPTxStatus(ENDP1)==EP_TX_NAK)  
- {  
- if(DataLen !=0)  
-     {  
-         USB_SIL_Write(ENDP1,kk,DataLen);                
-         SetEPTxValid(ENDP1);  
-         DataLen=0;  
-     }                 
- }</span><span style="font-family:SimSun;font-size:14px;">  
- </span>  
最后的最后不要忘了在stm32f10x_it.c中添加中断处理
**[cpp]**[view plain](http://blog.csdn.net/a827415225/article/details/51745013#)[copy](http://blog.csdn.net/a827415225/article/details/51745013#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1727041/fork)
- <span style="font-family:SimSun;font-size:10px;">void USB_LP_CAN1_RX0_IRQHandler(void)  
- {  
-   USB_Istr();  
- }    
- void USBWakeUp_IRQHandler(void)  
- {  
-    EXTI_ClearITPendingBit(EXTI_Line18);  
- }</span><span style="font-family:SimSun;font-size:14px;">  
- </span>  
至此整个移植过程就完成了，程序实现的功能是，通过电脑给USB设备发送数据，USB设备收到数据，然后再把数据发送回电脑，为了检验USB的发送与接收，使用Labview编写了一个简单的上位机。大概就是这个样子，发送哈哈，收到哈哈。
![](https://img-blog.csdn.net/20160623172224500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
关于上位机就在下一篇博文里写吧
