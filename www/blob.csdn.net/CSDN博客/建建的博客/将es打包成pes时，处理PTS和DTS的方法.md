# 将es打包成pes时，处理PTS和DTS的方法 - 建建的博客 - CSDN博客
2018年07月03日 22:14:48[纪建](https://me.csdn.net/u013898698)阅读数：52
个人分类：[多媒体封装格式详解](https://blog.csdn.net/u013898698/article/category/7105689)
PES包格式：
![](https://img-blog.csdn.net/20140221173200796)
分组开始码前缀：0x00 0x00 0x01
流ID：视频为0x0E，音频为0x0C
PES分组长度：这个包中此后数据的长度，包括此后剩余的PES包头长度和帧数据长度。
PES首部数据长度：可选字段长度+填充字节长度。
其中PTS/DTS标志位的意义为：
0x01：表示pes包头长度后面跟一个DTS数据（一个DTS或PTS的长度为5个字节。）
0x02：表示pes包头长度后面跟一个PTS数据
0x03：表示pes包头长度后面跟一个PTS数据和一个DTS数据。
代码：
pes_head_buf[7] = (0x2<<(8-2));
另外，数据流识别那个字节，一般为
#define PES_AUD_STREAM_ID  0xc0
#define PES_VID_STREAM_ID  0xe0
其他参数值见下：
![](https://img-blog.csdn.net/20140214170712921)
pts共33bit，在PES中占5个字节，具体格式见下：
'0011'            4 bslbf             
PTS [32..30] 3 bslbf
marker_bit   1 bslbf
PTS [29..15] 15 bslbf
marker_bit   1 bslbf
PTS [14..0]   15 bslbf
marker_bit   1 bslbf
代码见下：
pes_head_buf[9]   = 0x21 | ((pts >> 29) & 0x0E);
pes_head_buf[10] =  (pts >>22 & 0xFF);
pes_head_buf[11] = 0x01 | ((pts >> 14 ) & 0xFE);
pes_head_buf[12] =  (pts >> 7 & 0xFF);
pes_head_buf[13] = 0x01 | ((pts << 1 ) & 0xFE);
