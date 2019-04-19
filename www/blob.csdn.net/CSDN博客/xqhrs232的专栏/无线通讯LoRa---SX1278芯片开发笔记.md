# 无线通讯LoRa---SX1278芯片开发笔记 - xqhrs232的专栏 - CSDN博客
2017年06月20日 10:47:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：14781
原文地址::[http://blog.csdn.net/csdn_logo/article/details/51768050](http://blog.csdn.net/csdn_logo/article/details/51768050)
**1、资源搜集Datasheet 和驱动源码**
从Semtech官网下载最新驱动代码
[http://www.semtech.com/apps/filedown/down.php?file=sx12xxDrivers-V2.1.0.zip](http://www.semtech.com/apps/filedown/down.php?file=sx12xxDrivers-V2.1.0.zip)
Datasheet我上传了资源，中文版和英文版都有，还带了笔记的
[http://download.csdn.net/detail/csdn_logo/9560768](http://download.csdn.net/detail/csdn_logo/9560768)
**2、过一遍Datasheet，过之前必须对一些英文缩写有些了解，不然会要到处翻**
专业术语及其缩写：
FHSS 跳频扩频技术             FIFO 先进先出队列，这里代表队列寄存器  
PA 功率放大器                       LNA 低噪声放大器
SNR 信噪比                           SF 扩频因子                                                     
PLL 锁相环                            CAD 信道活动检测
CR 编码率                              BW 带宽                                                             
RS符号速率                           Preamble 序头
。。。
重要参数：
扩频因子 RegModulationCfg
![](https://img-blog.csdn.net/20160701170048554?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
因为不同扩频因子（SpreadingFactor）之间为正交关系，因此必须提前获知链路发
送端和接收端的扩频因子。另外，还必须获知接收机输入端的信噪比。在负信噪比条件下信
号也能正常接收，这改善了LoRa接收机的灵敏度、链路预算及覆盖范围。
注：SF=6 时必须用ImplicitHeader 模式
循环纠错编码cyclic error coding
![](https://img-blog.csdn.net/20160701170102742?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
信号带宽Bandwidth
![](https://img-blog.csdn.net/20160701170117461?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
较低频段（169 MHz）不支持250kHz和500kHz的带宽
数据包结构
     序头preamble                                     报头 header （可配）                  数据段 payload              校验码CRC
![](https://img-blog.csdn.net/20160701170131227?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
对于希望前导码是固定开销的情况，可以将前导码寄存器长度设置在6到65536之间来改变发送前导码长度，实际发送前导码的长度范围为6+4至65535+4个符号。这样几乎就可以发送任意长的前导码序列。接收机会定期执行前导码检测。因此，接收机的前导码长度应与发射机一致。如果前导码长度为未知或可能会发生变化，应将接收机的前导码长度设置为最大值。
报头分显示报头模式和隐式报头模式
低数据速率优化LowDataRateOptimize
当单个符号传输时间超过16毫秒时，必须使用LowDataRateOptimize位。注意：发射机和接收机的LowDataRateOptimize位设置必须一致。
有效负载 payload
其实就是数据段，即你要发或者要收的数据
数字寄存器和 数字I/O
寄存器113个，数字I/O也有6个，太多了，具体见Datasheet
操作模式 OpMode
![](https://img-blog.csdn.net/20160701170142474?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**3、分析驱动源码**
解压后打开doc下的README.txt
找到关于sx1276的相关说明如下：
2.1.3 SX1276 driver version V2.1.0
-------------------------------------------------------------------------------
The SX1276 driver is split in 4 parts  (驱动源码包含如下四个部分)
1. Generic SX1276 driver.    驱动调用接口
  ( src\radio\SX1276.c )
2. SX1272 FSK modem driver.    FSK调制模式用
  ( src\radio\SX1276-Fsk.c andsrc\radio\SX1276-FskMisc.c )
3. SX1272 LoRa modem driver.   LoRa调制模式用
  ( src\radio\SX1276-LoRa.cand src\radio\SX1276-LoRaMisc.c )
4. SX1276 HAL ( Hardware Abstraction Layer ).    硬件抽象层
  (src\platform\sx12xxEiger\SX1276-Hal.c )
1. The generic SX1276 driver implements atleast the functions required by
  the RadioDriver structure defined in src\radio\radio.h file. It offersalso
  the same interface for the FSK or the LoRa modem.
   Inorder to choose which modem to use one must modify the src\radio\radio.h
   file as follows:
   -For FSK modem
       #define LORA                                        0
   -For LoRa modem
       #define LORA                                        1
2. FSK调制的可以跳过不看
3. The LoRa modem driver handles the SX1276as a LoRa modem
   Inorder to change generic LoRa modem settings one must modify the following
  parameters in file src\radio\SX1276-LoRa.c （LoRa模式初始化参数配置的结构体）
  tLoRaSettings LoRaSettings =
   {
     870000000,  // RFFrequency   收发频率
     20,         // Power  发射功率
     8,          // SignalBw [0: 7.8kHz, 1: 10.4 kHz, 2: 15.6 kHz, 3: 20.8 kHz, 4: 31.2 kHz,    带宽
                 // 5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8:250 kHz, 9: 500 kHz, other: Reserved]
     7,          // SpreadingFactor [6:64, 7: 128, 8: 256, 9: 512,     扩频因子
                  // 10: 1024, 11: 2048, 12:4096  chips]
     2,          // ErrorCoding [1:4/5, 2: 4/6, 3: 4/7, 4: 4/8]      循环纠错编码
     true,        // CrcOn   CRC校验
     false,       // ImplicitHeaderOn   序头模式
     1,          // RxSingleOn    接收模式中的single模式开关  0代表continue模式
     0,          // FreqHopOn    跳频开关
     4,          // HopPeriod     跳频周期
     100,        // TxPacketTimeout   发送超时时间
     100,        // RxPacketTimeout   接收超时时间
     4           // PayloadLength    负载数据长度
   };
4. The HAL makes the SX1276 driver platformindependent.One must modify each function inside this file
    (src\platform\sx12xxEiger\SX1272-Hal.c ) according to the platform used. 
（根据硬件平台修改硬件抽象层）
3. How to use the driver:
-------------------------------------------------------------------------------
This driver has been tested for high speedtransmission (up to 100kbps in FSK)
and long payloads (up to 255 bytes in FSK or LoRa). To set a transmission /
reception, it is necessary to:
   -Change the payload length
The payload length for the system isdefined with the parameter BUFFER_SIZE
located in main.c
#define BUFFER_SIZE                                 128 // Definethe payload // size here
The payload length can be configured from 1up to 255
**4、用例**
自己定义一个函数用来动态的初始化芯片
**[cpp]**[view
 plain](http://blog.csdn.net/csdn_logo/article/details/51768050#)[copy](http://blog.csdn.net/csdn_logo/article/details/51768050#)
[print](http://blog.csdn.net/csdn_logo/article/details/51768050#)[?](http://blog.csdn.net/csdn_logo/article/details/51768050#)
- staticvoid RFInit()  
- {  
-     Radio->LoRaSetOpMode( RFLR_OPMODE_STANDBY );  
- // set the RF settings
-     Radio->LoRaSetPa20dBm( false );  
-     Radio->LoRaSetRFPower( 5 );  
-     Radio->LoRaSetSpreadingFactor( 7 ); // SF6 only operates in implicit header mode.
-     Radio->LoRaSetErrorCoding( 1 );  
-     Radio->LoRaSetPacketCrcOn( 0 );  
-     Radio->LoRaSetSignalBandwidth( 7 );  
-     Radio->LoRaSetImplicitHeaderOn( 0 );    
-     Radio->LoRaSetSymbTimeout( 0x3FF );  
-     Radio->LoRaSetPayloadLength( 128 );  
-     Radio->LoRaSetLowDatarateOptimize( true );  
-     Radio->LoRaSetFreqHopOn(false);      
-     Radio->LoRaSetRxSingleOn(true);  
-     Radio->LoRaSetPreambleLength( 6 );   
-     Radio->LoRaSetOpMode( RFLR_OPMODE_STANDBY );  
- }  
通过RF发送数据的发送函数，主要用于收发异频，收发异频能减少干扰
**[cpp]**[view
 plain](http://blog.csdn.net/csdn_logo/article/details/51768050#)[copy](http://blog.csdn.net/csdn_logo/article/details/51768050#)
[print](http://blog.csdn.net/csdn_logo/article/details/51768050#)[?](http://blog.csdn.net/csdn_logo/article/details/51768050#)
-  INT8U RFWrite(INT8U* buff, INT8U size, INT32U freq )  
- {  
-     Radio->LoRaSetRFFrequency( freq );// 478750000   DownChannel[10]
-     Radio->SetTxPacket( buff, size);  
- while( Radio->Process( ) != RF_TX_DONE );  
- 
- return size;  
- }  
通过RF接收数据的接收函数
**[cpp]**[view
 plain](http://blog.csdn.net/csdn_logo/article/details/51768050#)[copy](http://blog.csdn.net/csdn_logo/article/details/51768050#)
[print](http://blog.csdn.net/csdn_logo/article/details/51768050#)[?](http://blog.csdn.net/csdn_logo/article/details/51768050#)
- INT8U RFRead(INT8U* buff, INT32U freq, INT8U timeout )  
- {  
-     uint32_t result;  
-     INT16U RxLen;  
- 
-     Radio->LoRaSetRFFrequency( freq );   
-     Radio->LoRaSetRxPacketTimeout( timeout*1500 );   // 1s = 1500
-     Radio->StartRx( );  
- 
- while( 1 )  
-     {  
-         result = Radio->Process( );///SX1276LoRaProcess
- if( (result == RF_RX_DONE) || (result == RF_RX_TIMEOUT) )  
-         {  
- break;  
-         }  
-     }  
- 
- if( result == RF_RX_DONE )//RFBuffer
-     {  
-         Radio->GetRxPacket( buff, &RxLen );  
- return RxLen;  
-     }  
- else
-     {  
- return 0;  
-     }  
- 
- }  
