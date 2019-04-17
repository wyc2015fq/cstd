# 用实例分析H264 RTP payload - DoubleLi - 博客园






H264的RTP中有三种不同的基本负载（Single NAL,Non-interleaved,Interleaved)

应用程序可以使用第一个字节来识别。



在SDP中也说明了本次会话的属性

**SDP 参数**
下面描述了如何在 SDP 中表示一个 H.264 流:
. "m=" 行中的媒体名必须是 "video"
. "a=rtpmap" 行中的编码名称必须是 "H264".
. "a=rtpmap" 行中的时钟频率必须是 90000.
. 其他参数都包括在 "a=fmtp" 行中.
如:
m=video 49170 RTP/AVP 98
a=rtpmap:98 H264/90000
a=fmtp:98 profile-level-id=42A01E; packetization-mode=1; sprop-parameter-sets=Z0IACpZTBYmI,aMljiA==

下面介绍一些常用的参数.
**3.1 packetization-mode:**
表示支持的封包模式. 
当 packetization-mode 的值为 0 时或不存在时, 必须使用单一 NALU 单元模式.
当 packetization-mode 的值为 1 时必须使用非交错(non-interleaved)封包模式.

当 packetization-mode 的值为 2 时必须使用交错(interleaved)封包模式.





每个打包方式允许的NAL单元类型总结(yes = 允许, no = 不允许, ig = 忽略)
      Type   Packet    Single NAL    Non-Interleaved    Interleaved
                       Unit Mode           Mode             Mode
      -------------------------------------------------------------

      0      undefined     ig               ig               ig
      1-23   NAL unit     yes              yes               no
      24     STAP-A        no              yes               no
      25     STAP-B        no               no              yes
      26     MTAP16        no               no              yes
      27     MTAP24        no               no              yes
      28     FU-A          no              yes              yes
      29     FU-B          no               no              yes
      30-31  undefined     ig               ig               ig



这个参数不可以取其他的值.

**3.2 sprop-parameter-sets:** SPS,PPS
这个参数可以用于传输 H.264 的序列参数集和图像参数 NAL 单元. 这个参数的值采用 Base64 进行编码. 不同的参数集间用","号隔开.


**3.3 profile-level-id:**这个参数用于指示 H.264 流的 profile 类型和级别. 由 Base16(十六进制) 表示的 3 个字节. 第一个字节表示 H.264 的 Profile 类型, 第三个字节表示 H.264 的 Profile 级别:

**3.4 max-mbps:**
这个参数的值是一个整型, 指出了每一秒最大的宏块处理速度.



![](http://hi.csdn.net/attachment/201010/18/0_1287381614praR.gif)

![](http://hi.csdn.net/attachment/201010/18/0_1287381724V9FP.gif)

Rtp payload的第一个字节和264的NALU类似



+---------------+
|0|1|2|3|4|5|6|7|
+-+-+-+-+-+-+-+-+
|F|NRI| Type    |
+---------------+



F: 1 个比特.

forbidden_zero_bit. 在 H.264 规范中规定了这一位必须为 0.

NRI: 2 个比特.

nal_ref_idc. 取 00 ~ 11, 似乎指示这个 NALU 的重要性, 如 00 的 NALU 解码器可以丢弃它而不影响图像的回放. 不过一般情况下不太关心这个属性.

Type: 5 个比特.

**nal_unit_type. 这个 NALU 单元的类型. 简述如下:**0     没有定义
1-23 NAL单元 单个 NAL 单元包.
24    STAP-A   单一时间的组合包
24    STAP-B   单一时间的组合包
26    MTAP16   多个时间的组合包
27    MTAP24   多个时间的组合包
28    FU-A     分片的单元
29    FU-B     分片的单元
30-31 没有定义

例子：

0x5C=01011100 (F:0  NRI:10  Type:28) FU-A

0x41=01000001 (F:0  NRI:10  Type:01)Single NAL

0x68=01000100 (F:0  NRI:10  Type:08)Single NAL



**Single NAL Unit Mode****:Type[1-23] **packetization-mode=0



对于 NALU 的长度小于 MTU 大小的包, 一般采用单一 NAL 单元模式.
对于一个原始的 H.264 NALU 单元常由 [Start Code] [NALU Header] [NALU Payload] 三部分组成, 其中 Start Code 用于标示这是一个 NALU 单元的开始, 必须是 "00 00 00 01" 或 "00 00 01", NALU 头仅一个字节, 其后都是 NALU 单元内容.
打包时去除 "00 00 01" 或 "00 00 00 01" 的开始码, 把其他数据封包的 RTP 包即可.









**Non-interleaved Mode:Type[1-23,24,28] **packetization-mode=1

       Type=[1-23]的情况 参照 packetization-mode=0

Type=28 FU-A

+---------------+---------------+
|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|F|NRI| Type:28 |S|E|R| Type    |
+---------------+---------------+



S:开始标志

E:结束标志 (与 Mark相同）

R:必须为0



Type:h264的NALU Type



例：



0x7C85=01111100 10000101 (开始包)

0x7C05=01111100 00000101 (中间包)

0x7C45=01111100 01000101 (结束包)



Type=23  STAP-A

0               1             2                 3
|0 1 2 3 4 5 6 7|8 9 0 1 2 3 4|5 6 7 8 9 0 1 2 3|4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                          RTP Header                           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|STAP-A NAL HDR |         NALU 1 Size           | NALU 1 HDR    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                         NALU 1 Data                           |
:                                                               :
+               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

|               | NALU 2 Size                   | NALU 2 HDR    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                         NALU 2 Data                           |
:                                                               :
|                               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                               :...OPTIONAL RTP padding        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+







**[cpp]**[view plain](http://blog.csdn.net/zblue78/article/details/5948538#)[copy](http://blog.csdn.net/zblue78/article/details/5948538#)



- class H264NALUParser    
- {  
- public:  
-     H264NALUParser(int width , int height);  
-     H264NALUParser();  
- virtual ~H264NALUParser();  
- void SetBuffer(unsigned char * buffer,int len,int f,int nri,int type);  
- BOOL readOnePacket(unsigned char * buffer,int &len);  
- BOOL isPacketOutstanding();  
- private:  
-     unsigned char * m_pNaluBuffer;  // NALU数据指向的缓冲区的指针  
-     unsigned int m_nNaluSize;       // NALU数据缓冲区的大小  
-     unsigned char * m_pCurNaluPos;  //指向下一个数据包要读取的缓冲区指针  
- int m_nFrameWidth;  
- int m_nFrameHeight;  
- int m_nPacketCounts;  
- int m_nPacketSeqNum;  
- int m_nF;  
- int m_nNRI;  
- int m_nType;  
- enum {  
-         STAP_A = 24,  
-         STAP_B = 25,  
-         MTAP16 = 26,  
-         MTAP24 = 27,  
-         FU_A   = 28,  
-         FU_B   = 29  
-     };  
- };    
- 
- ////////////////// class H264NALUParser /////////////////////////////  
- H264NALUParser::H264NALUParser(int width , int height)  
- {  
-     m_nFrameWidth   = width;  
-     m_nFrameHeight  = height;  
-     m_pNaluBuffer   = NULL;  
-     m_nNaluSize     = 0;  
-     m_nPacketCounts = 0;  
-     m_nPacketSeqNum = 0;  
-     m_nF            = 0;  
-     m_nNRI          = 0;  
-     m_nType         = 0;  
- }  
- H264NALUParser::H264NALUParser()  
- {  
-     m_pNaluBuffer   = NULL;  
-     m_nNaluSize     = 0;  
-     m_nPacketCounts = 0;  
-     m_nPacketSeqNum = 0;  
-     m_nF            = 0;  
-     m_nNRI          = 0;  
-     m_nType         = 0;  
- }  
- H264NALUParser::~H264NALUParser()  
- {  
- }  
- void H264NALUParser::SetBuffer(unsigned char * buffer,int len,int f,int nri,int type)  
- {  
-     m_pNaluBuffer   = buffer;  
-     m_nNaluSize     = len;  
-     m_nF            = f;  
-     m_nNRI          = nri;  
-     m_nType         = type;  
-     m_pCurNaluPos   = m_pNaluBuffer;  
-     m_nPacketCounts = (m_nNaluSize + H264_MTU - 1) / H264_MTU;  
-     m_nPacketSeqNum = 0;  
- }  
- BOOL H264NALUParser::readOnePacket(unsigned char * buffer,int &len)  
- {  
- if(m_pCurNaluPos >= m_pNaluBuffer + m_nNaluSize)  
-     {  
- return FALSE;  
-     }  
- struct h264_rtp_hdr header;  
- int headersize;  
-     unsigned char * pCurBuf = buffer;  
- if(m_nNaluSize <= H264_MTU)// Single NALU  
-     {  
-         header.SingleNALU.f     = m_nF;  
-         header.SingleNALU.nri   = m_nNRI;  
-         header.SingleNALU.type  = m_nType;  
-         headersize = sizeof(header.SingleNALU);  
-         memcpy(pCurBuf,&(header.SingleNALU),headersize);  
-         pCurBuf += headersize;  
-     }  
- else// FU-A  
-     {  
-         header.FU_A.f           = m_nF;  
-         header.FU_A.nri         = m_nNRI;  
-         header.FU_A.type_indicator  = FU_A;  
- if(0 == m_nPacketSeqNum)  
-         {  
-             header.FU_A.s       = 1;  
-         }  
- else  
-         {  
-             header.FU_A.s       = 0;  
-         }  
- if(m_nPacketSeqNum == m_nPacketCounts - 1)  
-         {  
-             header.FU_A.e       = 1;  
-         }  
- else  
-         {  
-             header.FU_A.e       = 0;  
-         }  
-         header.FU_A.r           = 0;  
-         header.FU_A.type_header = m_nType;  
- //  
-         headersize = sizeof(header.FU_A);  
-         memcpy(pCurBuf,&(header.FU_A),headersize);  
-         pCurBuf += headersize;  
-     }  
- if(m_nPacketSeqNum < m_nPacketCounts - 1)  
-     {  
-         memcpy(pCurBuf,m_pCurNaluPos,H264_MTU);  
-         m_pCurNaluPos += H264_MTU;  
-         len = headersize + H264_MTU;  
-     }  
- else  
-     {  
- int remainLen = m_nNaluSize % H264_MTU;  
- if(0 == remainLen)  
-         {  
-             remainLen = H264_MTU;  
-         }  
-         memcpy(pCurBuf,m_pCurNaluPos,remainLen);  
-         m_pCurNaluPos += remainLen;  
-         len = headersize + remainLen;  
-     }  
-     m_nPacketSeqNum ++;  
- return TRUE;  
- }  
- BOOL H264NALUParser::isPacketOutstanding()  
- {  
- return (m_nPacketSeqNum < m_nPacketCounts);  
- }  




**Interleaved Mode:Type[26-29] **packetization-mode=2



待续



STAP-B

MTAP16

MTAP24

FU-B









