# iphone ipad等苹果设备30针数据线接口定义，大家可能有用 - xqhrs232的专栏 - CSDN博客
2013年05月13日 23:50:09[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2858
原文地址::[http://wenku.baidu.com/view/0afc1cfb910ef12d2af9e781.html](http://wenku.baidu.com/view/0afc1cfb910ef12d2af9e781.html)
相关网帖
1、iphone数据线接口定义----[http://wenku.baidu.com/view/0c31aa28ed630b1c59eeb5e5.html](http://wenku.baidu.com/view/0c31aa28ed630b1c59eeb5e5.html)
[iphone](http://ham.hellocq.com/tag.php?name=iphone),[数据线](http://ham.hellocq.com/tag.php?name=%CA%FD%BE%DD%CF%DF),[ipad](http://ham.hellocq.com/tag.php?name=ipad),[接口](http://ham.hellocq.com/tag.php?name=%BD%D3%BF%DA)
找到iphone ipad等苹果设备30针数据线接口定义，大家可能有用
[**下载**](http://ham.hellocq.com/attachment.php?aid=ODczODUyfDRlNzhhZmY1fDEzMjc1Mzk3MDR8OTA2YkdtYmErT3d1YklKWEhyT01maDl2RitIaFc4a0NrNmxiNWpXUzg1ckt0QW8%3D&nothumb=yes)
 (41.15 KB)
接口排列方式为
2012-1-26 00:15
**iPhone数据线接口定义**
[**下载**](http://ham.hellocq.com/attachment.php?aid=ODczODUzfDIxODRiNjhmfDEzMjc1Mzk3MDR8OTA2YkdtYmErT3d1YklKWEhyT01maDl2RitIaFc4a0NrNmxiNWpXUzg1ckt0QW8%3D&nothumb=yes)
 (50.13 KB)
接口排列方式为
2012-1-26 00:15
接口排列方式为:
1.2.3.4.5.6.7.8.9...........29.30
内部焊接点排列为:
1 3 5 7 9 11 13 15 17 19 21 23 25 27 29
2 4 6 8 10 12 14 16 18 20 22 24 26 28 30
接口定义为:
1 Ground (-) 地 *
2 Line Out - Common Ground (-) 线路输出 地*
3 Line Out - R (+) R声道线路输出
4 Line Out - L (+)L声道线路输出
5 Line In - R (+) R声道线路输入
6 Line In - L (+) L声道线路输入
7
8 复合视频输出 (只有彩屏的ipod有效)******
9 
S-Video 色度输出 (只有彩屏的ipod有效)  
10 S-Video亮度输出 (只有彩屏的ipod有效)  
11 Serial GNDRS-232串口 地
12 Serial TxD RS-232串口 TxD
13 Serial RxD RS-232串口 RxD
14
15 Ground (-) 地 **
16 USB GND (-) USB电源负极 **
17
18 3.3V Power (+) 3.3V电源正极
19 Firewire Power 12 VDC (+) 1394火线12V电源 正极 ***
20 Firewire Power 12 VDC (+)1394火线12V电源 正极***
21 Accessory Indicator 附件识别接口 ****
22 FireWire Data TPA (-) 1394火线数据 TPA(-)
23 USB Power 5 VDC (+)USB 5V 电源 正极
24 FireWire Data TPA (+) 1394火线数据 TPA(+)
25 USB Data (-)USB 数据(-)
26 FireWire Data TPB (-)1394火线数据 TPA(-)
27 USB Data (+) USB 数据(+)
28 FireWire Data TPB (+) 1394火线数据 TPB(+)
29 FireWire Ground (-) 1394火线12V电源 负极*****
30 FireWire Ground (-) 1394火线12V电源 负极*****
iphone接口定义英文版的：
30 pin connector used on the dock station for iPod and iPhone.
Available in all Apple iPod MP3 player (iPod 1G, 2G, 3G, 4G, 5G and Nano).
This connector is used on iPod (starting from 3rd generation) and iPhone. It is used to connect the iPod or iPhone to various devices: PC (via USB or FireWire IEEE1394), audio amplifier, serial device (controlled via the Apple Accessory Protocol).
Pin  Signal  Description  
1  GND  Ground (-), internaly connected with Pin 2 on iPod motherboard  
2  GND  Audio and Video ground (-), internaly connected with Pin 2 on iPod motherboard  
3  Right  Line Out - R (+) (Audio output, right channel)  
4  Left  Line Out - L(+) (Audio output, left channel)  
5  Right In  Line In - R (+)  
6  Left In  Line In - L (+)  
8  Video Out  Composite video output (only when the slideshow mode is active on iPod Photo)  
9  S-Video Chrominance output  for iPod Color, Photo only  
10  S-Video Luminance output  for iPod Color, Photo only  
11  GND  Serial GND  
12  Tx  ipod sending line, Serial TxD  
13  Rx  ipod receiving line, Serial RxD  
14  NC  
15  GND  Ground (-), internaly connected with pin 16 on iPod motherboard  
16  GND  USB GND (-), internaly connected with pin 15 on iPod motherboard  
17  NC  
18  3.3V  3.3V Power (+)
Stepped up to provide +5 VDC to USB on iPod Camera Connector.
If iPod is put to sleep while Camera Connector is present, +5 VDC at this pin slowly drains back to 0 VDC.  
19,20  +12V  Firewire Power 12 VDC (+)  
21  Accessory Indicator/Serial enable  Different resistances indicate accessory type:
1KOhm - iPod docking station, beeps when connected
10KOhm - Takes some iPods into photo import mode
500KOhm - related to serial communication / used to enable serial communications Used in Dension Ice Link Plus car interface
1MOhm - Belkin auto adaptor, iPod shuts down automatically when power disconnected Connecting pin 21 to ground with a 1MOhm resistor does stop the ipod when power (i.e. Firewire-12V) is cut. Looks to be that when this pin is grounded it closes a switch so that
 on loss of power the Ipod shuts off. Dock has the same Resistor.
22  TPA (-)  FireWire Data TPA (-)  
23  5 VDC (+)  USB Power 5 VDC (+)  
24  TPA (+)  FireWire Data TPA (+)  
25  Data (-)  USB Data (-)  
26  TPB (-)  FireWire Data TPB (-)  
27  Data (+)  USB Data (+)
Pins 25 and 27 may be used in different manner. To force the iPod 5G to charge in any case, when "USB Power 5 VDC'' (pin 23) is fed, 25 must be connected to 5V through a 10KOhm resistor, and 27 must be connected to the Ground (for example: pin 1) with a 10KOhm
 resistor.  
28  TPB (+)  FireWire Data TPB (+)  
29,30  GND  FireWire Ground (-)  
Notes
Signals with grey background may not exist on some docking stations.
Apple serial devices (i.e. iTalk and remote control) communicate with iPod (3rd generation or more) using the Apple Accessory Protocol.
The serial connection uses a standard 19200 baud 8N1 protocol (the speed can be increased up to 57600 but tends to become unstable).
