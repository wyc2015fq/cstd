# AudioSpecificConfig - starRTC免费im直播会议一对一视频 - CSDN博客
2016年12月05日 16:50:11[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1100
2个字节。
下面蓝色的为**AudioSpecificConfig**部分：
000001e0h: 00 00 00 00 00 AF 0011 90 00
 00 00 0F 09 00 02; .....?.?......
根据**AudioSpecificConfig**结构的定义：
- audioObjectType = (0x11 & 0xF8) >> 3 <- 取前5bit，结果为2
- samplingFrequencyIndex = ((0x11 & 0x7) << 1) | (0x90 >> 7)<- 取后4bit，结果为3
- channelConfiguration = (0x90 >> 3) & 0x0F <- 取后4bit，结果为2
- frameLengthFlag = (0x90 >> 2) & 0x01<- 取后1bit，结果为0
- dependsOnCoreCoder = (0x90 >> 1) & 0x01 <- 取后1bit，结果为0
- extensionFlag = 0x90 & 0x01 <- 最后1bit，结果始终为0
参考：
**AudioSpecificConfig部分参数含义见：**
[https://wiki.multimedia.cx/index.php?title=MPEG-4_Audio](https://wiki.multimedia.cx/index.php?title=MPEG-4_Audio)
[http://blog.csdn.net/jwybobo2007/article/details/9221657](http://blog.csdn.net/jwybobo2007/article/details/9221657)
