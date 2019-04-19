# AMR format - 三少GG - CSDN博客
2012年04月02日 23:17:07[三少GG](https://me.csdn.net/scut1135)阅读数：2112标签：[header																[audio																[codec																[insert																[compression																[byte](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=compression&t=blog)](https://so.csdn.net/so/search/s.do?q=insert&t=blog)](https://so.csdn.net/so/search/s.do?q=codec&t=blog)](https://so.csdn.net/so/search/s.do?q=audio&t=blog)](https://so.csdn.net/so/search/s.do?q=header&t=blog)
个人分类：[流媒体基础](https://blog.csdn.net/scut1135/article/category/934830)
[AMR音频编码器概述及文件格式分析](http://www.bairuitech.com/html/xingyezixun/technology/20080223/82.html)
全称Adaptive Multi-Rate，自适应多速率编码，主要用于移动设备的音频，压缩比比较大，但相对其他的压缩格式质量比较差，由于多用于人声，通话，效果还是很不错的。
全称Adaptive Multi-Rate，自适应多速率编码，主要用于移动设备的音频，压缩比比较大，但相对其他的压缩格式质量比较差，由于多用于人声，通话，效果还是很不错的。
一、分类
1. AMR: 又称为AMR-NB，相对于下面的WB而言，
语音带宽范围：300－3400Hz，
8KHz抽样
2. AMR-WB:AMR WideBand，
语音带宽范围： 50－7000Hz
16KHz抽样
“AMR-WB”全称为“Adaptive Multi-rate - Wideband”，即“自适应多速率宽带编码”，采样频率为16kHz，是一种同时被国际标准化组织ITU-T和3GPP采用的宽带语音编码标准，也称 为G722.2标准。AMR-WB提供语音带宽范围达到50～7000Hz，用户可主观感受到话音比以前更加自然、舒适和易于分辨。
.
.
.
五、 AMR 文件的存储格式（RFC 3267）：
AMR IF1, IF2定义了 AMR的帧格式， 用于无线传输用。 RFC 3267定义了把AMR数据存成文件的文件格式。
AMR的文件格式如下图1所示：
它包含一个文件头，然后就是一帧一帧的AMR数据了。
<!--[if !supportLists]-->1.       <!--[endif]-->文件头格式：
 AMR 文件支持单声道和多声道。单声道和多声道的文件头是不同的。
 单声道：
 AMR-NB文件头： "#!AMR\n" (or 0x2321414d520a in hexadecimal)(引号内的部分)
 AMR-WB 文件头："#!AMR-WB\n" (or 0x2321414d522d57420a in hexadecimal).（引号内）
多声道：
多声道的文件头包含一个magic number和32bit channle description域。
![](http://www.bairuitech.com/upimg/allimg/080223/0359222.jpg)
AMR-NB 的magic number："#!AMR_MC1.0\n"
(or 0x2321414d525F4D43312E300a in hexadecimal).
AMR-WB的magic number："#!AMR-WB_MC1.0\n"
                         (or 0x2321414d522d57425F4D43312E300a in hexadecimal).
![](http://www.bairuitech.com/upimg/allimg/080223/0359223.jpg)
32bit的channel description域的定义如下：
![](http://www.bairuitech.com/upimg/allimg/080223/0359224.jpg)
其中 reserved bits必须为0， CHAN:表示当前文件中含有几个声道。
帧头的格式：
帧头的格式如图2 所示， 它占1个字节（8个bit）
P为填充为设置为0
FT为编码模式， 即上面提到的16中编码模式。
Q为帧质量指示器，如果为0表明帧被损坏。
图3 列举了AMR-NB 5.9Kbit的一个帧的格式，
对于5.9kbit一帧的有118bit的数据，15*8=120=118+2, 所以在最后有2个bit的填充位。
参考文献：
RFC3267 RTP Payload Format for AMR and AMR-WB
3GPP TS 26.201 V6.0.0
3GPP TS 26.101 V6.0.0
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[http://tools.ietf.org/html/rfc3267](http://tools.ietf.org/html/rfc3267)
# Real-Time Transport Protocol (RTP) Payload Format and File Storage
# Format for the Adaptive Multi-Rate (AMR) and Adaptive Multi-Rate
# Wideband (AMR-WB) Audio Codecs
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[AMR format](http://www.developer.nokia.com.sixxs.org/Community/Wiki/AMR_format)
Each frame can be encoded using one of 8 varying levels of compression using various bitrates(AMR modes 0-7). Following are the various AMR modes and their corresponding bitrates:
MODE               BIT RATES
0           - AMR 4.75 - Encodes at 4.75kbit/s
1           - AMR 5.15 - Encodes at 5.15kbit/s
2           - AMR 5.9  - Encodes at 5.9kbit/s
3           - AMR 6.7  - Encodes at 6.7kbit/s
4           - AMR 7.4  - Encodes at 7.4kbit/s
5           - AMR 7.95 - Encodes at 7.95kbit/s
6           - AMR 10.2 - Encodes at 10.2kbit/s
7           - AMR 12.2 - Encodes at 12.2kbit/s
Each frame consists of a 1-byte header, then the rest of the frame is audio data. The entire frame is fed into the AMR decoder (header too). The frame size can be deduced from the frame header.
The 2nd bit through 5th bit (counting the most significant bit as the first bit) comprise the CMR (Codec Mode Request), values 0-7 being valid for AMR. The top bit of the CMR can actually be ignored, though it is used when AMR forms RTP payloads. The lower
 3-bits of the header are reserved and are not used. Viewing the header from most significant bit to least significant bit, the encoding is XCCCCXXX, where Xs are reserved (typically 0) and the Cs are the CMR.
Frame size of AMR modes in bytes (including the header byte) are shown below:
CMR      MODE       ** FRAME SIZE( in bytes )****0        AMR 4.75            13**
1        AMR 5.15            14
2        AMR 5.9             16
3        AMR 6.7             18
4        AMR 7.4             20
5        AMR 7.95            21
6        AMR 10.2            27
7        AMR 12.2            32
The above frame specifications and header information applies only for AMR-NB and the frame format might be different for AMR-WB.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
**[AMR over RTP:](http://sundararajana.blogspot.com.sixxs.org/2008/04/amr-over-rtp-rtp-packet-size-1389-rtp.html)**
RTP Packet Size 1389
RTP header size 12
--------
1377
-----------
(1377 / Frame Size) - 1 = Number of ToC Entries; each frame has ToC entry in RTP
Or just count the number of bytes (ToC entries) to identify the frames available in a RTP packet;
How can we extract the AMR mode information from the RTP packet?
First check the CMR (Codec Mode Request) .
It is having bandwidth. For Each bandwidth, the frame size is fixed;
**CMR Mode Frame size (bytes)**
0 AMR 4.75 13
1 AMR 5.15 14
2 AMR 5.9 16
3 AMR 6.7 18
4 AMR 7.4 20
5 AMR 7.95 21
6 AMR 10.2 27
7 AMR 12.2 32
Every AMR frame is having 20 ms of audio data;
So if 40 frames are available means
40 * 20 = 800 milli seconds (play time of that frames);
1000 milliseconds = 1 second
From Source Filter, Based on Number of Frames we need to set the Start and Stop timestamps.
AMR over RTP is as follows:
+----------------+-------------------+----------------
| payload header | table of contents | speech data ...
+----------------+-------------------+----------------
Payload header is 4 bits;
First 4 bits are CMR (Codec Mode Request); From Codec Mode Request value, we can identify the frame size;
ToC (Table of contents) -
Each and every frame has an entry for the ToC;
If RTP packet is having 43 audio frames means that much of Toc Bytes must be available.
After F0, Remove the same types of bytes; (this byte indicates the audio bit rate information)
But How can we know the Frame Type?
1011 1100 (BC) 12.2 kbps
1011 0100 (B4) 10.2 kbps
1010 1100 (AC) 7.95 kbps
RTP packet:
---------------
If a terminal has no preference in which mode to receive, it SHOULD set CMR=15 in all its outbound payloads
Each RTP AMR data begins with F0 and then ToC entries like 0xac, 0xbc, 0xb4 as repetitive.
If the RTP packet has N frames, RTP packet is having N number of TOC Entries.
From the TOC Entry we can define the frame size of the audio frame;
TOC Entry will be in the following form:
---------------------------------------------
F (1 bit) | **FT** (4 bits) | 1 (1 bit)
---------------------------------------------
1 0111 1 00 -12.2 kbps ( 0x BC)
1 0110 1 00 -10.2 kbps ( 0x B4)
1 0101 1 00 - 7.95 kbps ( 0x AC)
------------------------------------------------
FO BC BC BC BC BC BC BC BC BC 3C
After the TOC contents, the First start code acts as a frame header;
3C is the frame header available in an audio frame and every audio frame must begins with 3C;
From the bit rate, we can determine the Frame Size;
CMR Mode Frame size (bytes)
0 AMR 4.75 13
1 AMR 5.15 14
2 AMR 5.9 16
3 AMR 6.7 18
4 AMR 7.4 20
5 AMR 7.95 21
6 AMR 10.2 27
7 AMR 12.2 32
Frame Size is including a frame header;
But RTP packet is having the frames as follows:
First Frame alone has the 1 byte audio frame header; rest of the frames will not have header; we need to add it manually;
Ac 12 20 39 40 29 20 39 33
Ac is a frame header and from the header onwards we can identify the number of bytes per frame; assume that if the frame header info as 4.75kbps having frame size 13 means from the frame header, count the 13 bytes;
 then next insert the First frame’s frame header and count the 13 bytes from the header and then insert the frame header for 3rd frame;
1st Frame Header bytes After 13 bytes insert the First Frame’s header, then next insert the frame header after the 26 bytes and next insert the frame header after the 39 bytes. Do it repeatedly.
If we have not inserted the frame header at the every frame start, then it will be decoded by the AMR decoder but u will not have any hearable audio.
POSTED BY SUNDARAT [THURSDAY,
 APRIL 03, 2008](http://sundararajana.blogspot.com.sixxs.org/2008/04/amr-over-rtp-rtp-packet-size-1389-rtp.html)
#### 1 COMMENTS:
- 
![](http://3.bp.blogspot.com.sixxs.org/_dVS_rwQ57-4/SZdJl6N6pcI/AAAAAAAAA-Y/hSt5GxeOiQc/S45/picture%25252000150%25252050%25252050.jpg)
[Am not a Donkey](http://www.blogger.com.sixxs.org/profile/17326728686862125126) said...
Excellent post. Helped me a lot. Keep up the good work!
[SEPTEMBER
 21, 2009 9:59 PM](http://sundararajana.blogspot.com.sixxs.org/2008/04/amr-over-rtp-rtp-packet-size-1389-rtp.html?showComment=1253595544231#c5957633599039242604)

