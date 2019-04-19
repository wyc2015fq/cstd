# USB-HID鼠标、键盘通讯格式 - xqhrs232的专栏 - CSDN博客
2018年01月27日 23:49:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：476
原文地址::[https://www.amobbs.com/forum.php?mod=viewthread&tid=4823160&page=1&authorid=249202](https://www.amobbs.com/forum.php?mod=viewthread&tid=4823160&page=1&authorid=249202)
想用STM32做个无线键鼠遥控、找了好几天没有找到完整的USB鼠标键盘协议介绍。最后自己收集整理了一份，希望对需要的朋友有所帮助。
废话不多说，进入正题：
鼠标发送给PC的数据每次4个字节
BYTE1 BYTE2 BYTE3 BYTE4
定义分别是：
BYTE1 --
       |--bit7:   1   表示   Y   坐标的变化量超出－256   ~   255的范围,0表示没有溢出 
       |--bit6:   1   表示   X   坐标的变化量超出－256   ~   255的范围，0表示没有溢出 
       |--bit5:   Y   坐标变化的符号位，1表示负数，即鼠标向下移动 
       |--bit4:   X   坐标变化的符号位，1表示负数，即鼠标向左移动 
       |--bit3:     恒为1 
       |--bit2:     1表示中键按下 
       |--bit1:     1表示右键按下 
       |--bit0:     1表示左键按下 
BYTE2 -- X坐标变化量，与byte的bit4组成9位符号数,负数表示向左移，正数表右移。用补码表示变化量 
BYTE3 -- Y坐标变化量，与byte的bit5组成9位符号数，负数表示向下移，正数表上移。用补码表示变化量
BYTE4 -- 滚轮变化。
由于手上没有USB鼠标，对BYTE1的4-7位没有测试，对于BYTE2 BYTE3做个测试，BYTE1的4-7全为0的时候，BYTE2 BYTE3的正负表示鼠标移动方向
键盘发送给PC的数据每次8个字节
BYTE1 BYTE2 BYTE3 BYTE4 BYTE5 BYTE6 BYTE7 BYTE8
定义分别是：
BYTE1 --
       |--bit0:   Left Control是否按下，按下为1 
       |--bit1:   Left Shift  是否按下，按下为1 
       |--bit2:   Left Alt    是否按下，按下为1 
       |--bit3:   Left GUI    是否按下，按下为1 
       |--bit4:   Right Control是否按下，按下为1  
       |--bit5:   Right Shift 是否按下，按下为1 
       |--bit6:   Right Alt   是否按下，按下为1 
       |--bit7:   Right GUI   是否按下，按下为1 
BYTE2 -- 暂不清楚，有的地方说是保留位
BYTE3--BYTE8 -- 这六个为普通按键
键盘经过测试。
例如：键盘发送一帧数据   02 00 0x04 0x05 00 00 00 00
表示同时按下了Left Shift + ‘a’+‘b’三个键
对于键码和PS2的键盘有所不同，具体请看附件。
USB HID to PS2 Scan Code 对照表.pdf[ourdev_651088NZ5EKW.pdf(文件大小:133K)](http://cache.amobbs.com/bbs_upload782111/files_41/ourdev_651088NZ5EKW.pdf)(原文件名:USB
 HID to PS2 Scan Code Translation Table .pdf)
2011.6.23补充说明
补充对于键盘BYTE2的说明：
参照百合网（http://www.baiheee.com/OpenSource/Easy%20USB%2051%20Programer/Easy%20USB%2051%20Programer12.htm）键盘的报告描述符，如下
/*******************************************************************************************/
//这里只列出了第二个字节的说明
    //这样的数据段个数为1    
    0x95, 0x01,                    //   REPORT_COUNT (1)    
    //每个段长度为8bits    
    0x75, 0x08,                    //   REPORT_SIZE (8)    
    //输入用，常量，值，绝对值    
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)    
    //上面这8个bit是常量，设备必须返回0  
/*******************************************************************************************/
这里只是指明了这是一个以字节为单位的输入数据，并没有指明它的取值范围。
在HID1.11协议的Appendix B-->B.1 Protocol 1 (Keyboard) 的报告描述符中也有这样的说明（HID1.11协议的59页）
/*******************************************************************************************/
   Report Count (1),  
   Report Size (8), 
   Input (Constant),              ;Reserved byte 
   Report Count (5),  
   Report Size (1), 
/*******************************************************************************************/
在注释中说明，这个字节是保留字节。
这里我也把HID1.11传上来给大家研究。
[ourdev_651409JXHOAD.pdf(文件大小:661K)](http://cache.amobbs.com/bbs_upload782111/files_41/ourdev_651409JXHOAD.pdf)(原文件名:HID1.11协议.pdf)
[ourdev_651410WJINWH.pdf(文件大小:745K)](http://cache.amobbs.com/bbs_upload782111/files_41/ourdev_651410WJINWH.pdf)(原文件名:HID用途表1.12.pdf)
