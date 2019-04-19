# NXP PN7150高性能全兼容NFC控制器解决方案 - xqhrs232的专栏 - CSDN博客
2017年02月21日 15:37:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：904
原文地址::[http://solution.eccn.com/solution_2016072110524831.htm](http://solution.eccn.com/solution_2016072110524831.htm)
相关文章
1、NXP-PN7150高性能全兼容NFC控制器解决方案----[http://wenku.baidu.com/link?url=gyF4Qupcci-u0U78Phn6-9gd32pg3Ofv_qHXOGwLwpYplH2oPqi85aTzVfvWQ7QWrpwt6VxI8CO0K3ns_npLPudXm5_wvoSP2evYEfXfHoG](http://wenku.baidu.com/link?url=gyF4Qupcci-u0U78Phn6-9gd32pg3Ofv_qHXOGwLwpYplH2oPqi85aTzVfvWQ7QWrpwt6VxI8CO0K3ns_npLPudXm5_wvoSP2evYEfXfHoG)
NXP公司的PN7150是最高性能即插即用NFC[解决方案](http://solution.eccn.com/),基于ARM
 Cortex-M0 MCU核,和NCI论坛规范V1.3兼容,支持NFC论坛类型1/2/3/4/5标签,提供高达4.75V/180mA的RF输出功率以及用于手持设备的低功耗模式,同时还在I2C主接口上提供标准NFC控制器接口(NCI).目标应用在需要NFC功能的所有设备,TV,STB,蓝光记录仪和音频设备,[家庭自动化](http://www.eccn.com/technology/index_c.html?id=10000052),网关和无线路由器,家用电器,[可穿戴](http://theme.eccn.com/theme/2015/wearable/index.html)设备,遥控,健身和健康设备,打印机,IP电话,游戏控制台等.本文介绍了PN7150主要特性,传输模式和框图,PN7150
 NFC控制器单板计算机(SBC)套件主要特性和电路图,PCB设计图.
PN7150 is the highest performing plug-and-play solution for NFC. It complies to NFC Forum device requirements V1.3 for Read/Write, Peer-to-Peer and Card Emulation modes and supports NFC Forum Type 1/2/3/4/5
 Tags. It provides high RF output power with up to 4.75 V, 180 mA, as well as low power modes for portable devices. It offers the standardized NFC Controller Interface (NCI) over I²C host interface.
Three different demokits make integration of NFC easy to different platforms such as RaspberryPi, BeagleBone Black and any boards featuring Arduino compatible header, including many LPCXpresso, Kinetis and
 i.MX boards. SW drivers enable easy integration into Linux, Android, Windows IoT and RTOS.
PN7150主要特性:
Includes NXP ISO/IEC14443-A, Innovatron ISO/IEC14443-B and NXP MIFARE crypto1 intellectual property licensing rights
ARM Cortex-M0 microcontroller core
Highly integrated demodulator and decoder
Buffered output drivers to connect an antenna with minimum number of external components
Integrated RF level detector
Integrated Polling Loop for automatic device discovery
RF [proto](http://www.eccn.com/topic/tag_list.html?id=40508)cols
 supported
NFCIP-1, NFCIP-2 protocol
ISO/IEC 14443A, ISO/IEC 14443B PICC mode via host interface
ISO/IEC 14443A, ISO/IEC 14443B PCD designed according to NFC Forum digital protocol T4T platform and ISO-DEP
FeliCa PCD mode
MIFARE PCD encryption mechanism (MIFARE 1K/4K)
NFC Forum tag 1 to 4 (MIFARE Ultralight, Jewel, Open FeliCa tag, DESFire)
ISO/IEC 15693/ICODE VCD mode
Supported host interfaces
NCI protocol interface according to NFC Forum standardization
I²C-bus High-speed mode
Integrated power management unit
Direct connection to a battery (2.3 V to 5.5 V voltage supply range)
Support different Hard Power-Down/Standby states activated by firmware
Autonomous mode when host is shut down
Automatic wake-up via RF field, internal timer and I²C-bus interface
Integrated non-volatile memory to store data and executable code for customization
PN7150目标应用:
All devices requiring NFC functionality especially those running in an Android or Linux environment
TVs, set-top boxes, Blu-ray decoders, audio devices
Home automation, gateways, wireless routers
Home appliances
Wearables, remote controls, healthcare, fitness
Printers, IP phones, gaming consoles, accessories
![](http://solution.eccn.com/uploads/solution/201607/20160721104657869.png)
图1.PN7150传输模式
![](http://solution.eccn.com/uploads/solution/201607/20160721104658674.png)
图2.PN7150框图
PN7150 NFC控制器单板计算机(SBC)套件
The present document describes the OM5578/PN7150S demonstration kit, a flexible and easy-to-use Single Board Computer (SBC) Kit for the PN7150 NFC Controller. 
It enables the development of an NFC solution based on PN7150 in a Linux, Android or Windows for IoT environment or even in system based on RTOS or without OS. 
It exists in different configurations:
- OM5578/PN7150RPI to be used with Raspberry Pi platform (see [1]) 
- OM5578/PN7150BBB to be used with BeagleBone Interface Board (see [2]) 
- OM5578/PN7150ARD to be used with platforms offering Arduino compatible interface (see [3]), like LPCXpresso (see [5]) or Kinetis Freedom (see [4]). 
All 3 OM5578 kits contains a PN7150 NFC Controller Board, a dedicated interface board, as well as an NFC Sample Card in form of an NFC Forum Type 2 Tag.
![](http://solution.eccn.com/uploads/solution/201607/2016072110470120.png)
图3.OM5578/PN7150RPI套件外形图
![](http://solution.eccn.com/uploads/solution/201607/20160721104702893.png)
图4.OM5578/PN7150BBB套件外形图
![](http://solution.eccn.com/uploads/solution/201607/20160721104703491.png)
图5.OM5578/PN7150ARD套件外形图
The OM5578 PN7150 NFC Controller Board is high performance fully NFC compliant expansion board, meeting compliance with Reader mode, P2P mode and Card emulation mode standards. 
The board features an integrated high performance RF antenna to insure high interoperability level with NFC devices. 
It has to be used in association with one interface board according to the targeted user environment. For this purpose it integrates the NFC generic interface allowing assembly with OM29110 Interface boards.
![](http://solution.eccn.com/uploads/solution/201607/20160721104707127.png)
图6.OM5578/PN7150S PN7150 NFC控制器板外形图
The OM29110 Interface Boards offer support for connection to Raspberry Pi, BeagleBone or Arduino Compatible Interface platforms.
![](http://solution.eccn.com/uploads/solution/201607/2016072110470822.png)
图7.OM29110RPI Raspberry Pi接口板外形图
![](http://solution.eccn.com/uploads/solution/201607/20160721104709369.png)
图8.OM29110BBB BeagleBone接口板外形图
![](http://solution.eccn.com/uploads/solution/201607/2016072110471255.png)
图9.OM29110ARD Arduino接口板外形图
OM5578/PN7150S kit includes a NFC Sample Card, based on NTAG216 tag IC, allowing to demonstrate NFC reader capabilities of PN7150 NFC Controller. 
For the current purpose of PN7150 NFC Controller demonstration, the card has been pre-configured with NDEF URI type message.
![](http://solution.eccn.com/uploads/solution/201607/20160721104714225.png)
图10.NFC卡样品图
![](http://solution.eccn.com/uploads/solution/201607/20160721104716101.png)
图11.PN7150 NFC控制器板电路图
![](http://solution.eccn.com/uploads/solution/201607/20160721104717346.png)
图12.PN7150 NFC控制器板PCB顶层元件布局图
![](http://solution.eccn.com/uploads/solution/201607/20160721104718671.png)
图13.PN7150 NFC控制器板PCB底层元件布局图
![](http://solution.eccn.com/uploads/solution/201607/20160721104718988.png)
图14.PN7150 NFC控制器板PCB设计图(层1)
![](http://solution.eccn.com/uploads/solution/201607/20160721104718431.png)
图15.PN7150 NFC控制器板PCB设计图(层2)
![](http://solution.eccn.com/uploads/solution/201607/20160721104719108.png)
图16.PN7150 NFC控制器板PCB设计图(层3)
![](http://solution.eccn.com/uploads/solution/201607/20160721104719340.png)
图17.PN7150 NFC控制器板PCB设计图(层4)
![](http://solution.eccn.com/uploads/solution/201607/20160721104719118.png)
图18.PN7150 NFC控制器板PCB顶层丝印图
The Generic NFC interface exposes Single-Board-Computer physical interfaces required by the NFC’s boards. Those are: 
- Usual power supplies (3.3V, 5.0V) 
- Usual IC interfaces (I²C, SPI, UART) 
- Generic GPIOs (can be used for different purposes depending on IC feature (field detect, interrupt, reset...) 
The BeagleBone Interface Board offers support for connection to BeagleBone board. 
As such it integrate the NFC generic interface connectors allowing NFC’s board to be plugged on it, as well as connectors to be assembled on top of the BeagleBone board.
![](http://solution.eccn.com/uploads/solution/201607/20160721104723931.png)
图19.OM29110 BeagleBone接口板概述图
![](http://solution.eccn.com/uploads/solution/201607/20160721104724538.png)
图20.OM29110 BeagleBone接口板电路图
The Raspberry Pi Interface board offers support for connection to Raspberry Pi board. As such it integrate the NFC generic interface connectors allowing NFC’s board to be plugged on it, as well as connectors
 to be assembled on top of the BeagleBone board.
![](http://solution.eccn.com/uploads/solution/201607/20160721104727682.png)
图21.OM29110 Raspberry Pi接口板概述图
![](http://solution.eccn.com/uploads/solution/201607/20160721104727574.png)
图22.OM29110 Raspberry Pi接口板电路图
The Arduino Interface board offers support for connection to any SBC board implementing ArduinoTM connectors compatible with the ’Arduino UNO’ platform. 
As such it integrate the NFC generic interface connectors allowing NFC’s board to be plugged on it, as well as connectors to be assembled on top of an Arduino compatible Controller board like for instance
 LPC824 MAX LPCXpresso board. 
The board include a R1 footprint between the VOUT pin of the NFC Generic connector and the VIN pin of the Arduino connector. The purpose of it is to allow powering the Arduino compatible platform from the
 NFC Board in case both support it.
![](http://solution.eccn.com/uploads/solution/201607/20160721104729389.png)
图23.OM29110 Arduino接口板概述图
![](http://solution.eccn.com/uploads/solution/201607/20160721104730306.png)
图24.OM29110 Arduino接口板电路图
详情请见:
<a href="http://cache.nxp.com/documents/data_sheet/PN7150.pdf?ps%3Ca%20target=" _blank"="" style="padding: 0px; margin: 0px auto; text-decoration: none; color: red; font-family: Arial, 微软雅黑;">pll=1">http://cache.nxp.com/documents/data_sheet/PN7150.pdf?pspll=1
和[http://cache.nxp.com/documents/user_manual/UM10935.pdf?fpsp=1&WT_TYPE=Users%20Guides&WT_VENDOR=FREESCALE&WT_FILE_FORMAT=pdf&WT_ASSET=Documentation&fileExt=.pdf](http://cache.nxp.com/documents/user_manual/UM10935.pdf?fpsp=1&WT_TYPE=Users%20Guides&WT_VENDOR=FREESCALE&WT_FILE_FORMAT=pdf&WT_ASSET=Documentation&fileExt=.pdf)
以及[http://cache.nxp.com/documents/user_manual/UM10956.pdf?fpsp=1&WT_TYPE=Users%20Guides&WT_VENDOR=FREESCALE&WT_FILE_FORMAT=pdf&WT_ASSET=Documentation&fileExt=.pdf](http://cache.nxp.com/documents/user_manual/UM10956.pdf?fpsp=1&WT_TYPE=Users%20Guides&WT_VENDOR=FREESCALE&WT_FILE_FORMAT=pdf&WT_ASSET=Documentation&fileExt=.pdf)
![](http://solution.eccn.com/webedit2/sysimage/icon16/pdf.gif)[PN7150.pdf](http://solution.eccn.com/uploads/solution/201607/20160721105428760.pdf)
![](http://solution.eccn.com/webedit2/sysimage/icon16/pdf.gif)[UM10935.pdf](http://solution.eccn.com/uploads/solution/201607/20160721105440603.pdf)
![](http://solution.eccn.com/webedit2/sysimage/icon16/pdf.gif)[UM10956.pdf](http://solution.eccn.com/uploads/solution/201607/20160721105456547.pdf)
