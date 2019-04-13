
# H264 的 RTP 格式封装 - 老雍的博客 - CSDN博客


2019年03月28日 16:35:27[老雍](https://me.csdn.net/yongkai0214)阅读数：51



### H264 的 RTP 格式封装
[RFC 6184/3984](#RFC_61843984_6)
[H264 简介](#H264__11)
[分包模式](#_66)
[打包类型](#_116)
[Single NAL Unit Packet](#Single_NAL_Unit_Packet_118)
[Agregation packet](#Agregation_packet_136)
[单时间聚合-STAP](#STAP_153)
[多时间聚合-MTAP](#MTAP_199)
[分片单元-FUs](#FUs_260)
[其他](#_309)

H264 的 RTP 打包方式说明在网上已经很多了，一般都是按照标准协议 RFC 3984 或者 RFC 6184 的顺序来解释，也有通过代码来说明的。最近有同事遇到有关的问题来咨询，所以干脆再写一篇，换个顺序来说明。
# RFC 6184/3984
RFC 3984 在 2005 年正式称为RFC，被非常大范围的采用，即使现在也是绝大多数的代码遵循的标准。RFC 6184 是在时隔六年以后于2011 年替代 RFC 3984，实际上这两篇 RFC 差别并不是很大，甚至很多人都没想过现在遵循的是谁，这也是因为后相兼容的好处。但是不管怎么说，修订版既然已经废弃了 RFC 3984，还是有必要了解新的协议。
和 RFC 3984 比较，全文有非常多的文字描述修改，比如were 改成 are 等之类的文字方面，也有一些技术细节的描述，这些技术细节主要集中在第 8 章。第 8 章是描述类似 profile-level-id，max-smbps 等之类的媒体字段。其他修改包括一些字段比如 packetization mode 在 RFC 3984 中 描述可以由带外信令携带，在 RFC 6184 中删除。所以纵观全文，改动并不大，但是描述将更准确。如果只是关注 H264 数据的打包，几乎可以不关注。
# H264 简介
H264是现在应用范围最广的视频编解码算法，其中比较引人注目的是 x264，openH264 两个优秀的开源实现，解码一般用 ffmpeg。视频图像以帧为单位输入到编码器中，输出编码后的有损压缩码流。
为什么能压缩？压缩就是将图像中重复的细节或者近似的部分用精简的方式表达出来。最主要的相似在于帧间图像之间，其次是单帧图像内部。在足够短的以内采集两张图片，可以认为，这两张图片几乎相同，这就是时间序列上的相似。一般每秒采集的图像在30帧左右，图像上的差别仍然可以帮助节约非常大的数据流量。至于空间相似性，单张图片上的足够小的纹理范围的变化是非常小的，或者渐变的。通过减少相同内容，融合相似内容就可以实现压缩。
为什么是有损？为了最大限度的减少数据容量，还要保持更多的细节，还得加快运算速度，就可能扩大比较的范围，或者将非常小的变化忽略。打个比方，本来图像上有 1280 * 720 个像素点，但是感觉点太多，就将像素点每4个点取均值或中值代表，这样图像的点就大大减少，计算量也大大缩减。视觉冗余是指人们对一些色彩或细节并不敏感，所以不要也罢。
因为时间序列相似性的地位，在编码时，经常取某一帧为一个基准参考帧，不参考别人，只参考自己。后续帧以前一帧作为参考，这样，每张图像相对前一张都可以最大限度进行压缩。但是如果中间某一帧数据被丢失，可能就导致这个参考链条断裂，后来的所有帧参考出来的图像都有问题。就出现 GOP（Group of picture，图像组）的概念，隔一段时间就重新出现一个参考帧刷新一下，使错误不致传播。这就是 I（Intra-coded picture, 帧内编码图像帧）帧和 P （Predictive-coded Picture，预测编码图像帧）帧的关系。B （Bidirectionally predicted picture，双向预测编码图像帧）帧则是更近一步，不仅可以参考前面的，还可以参考后面的，更进一步提高压缩比。
但过多的 P，B 帧会使编码效率降低。为什么会降低？前面说了足够短的时间内，图像可以视为不变，但是场景总是可能切换的。切换前后会变化非常大，这个时候的图像变化就会很大了。还这么做，就要考虑是不是划算了，做事情，总得考虑性价比。不过这是使用编码器的人需要考虑的，H264 本身只是提供一种方法而已。
编码后每帧长度可能并不相同，那如何区分呢？每一个帧从编码器出来都会携带开始码八进制 00 00 00 01 或者 00 00 01。如果刚好出现 0x00，那就在帧间加上 0x03，在 H264 的机制名叫作 emulation prevention（防止竞争）。因为已经约定好了，所以解码器可以自动识别，不会产生歧义。
在 H264 中将图像组，或者图像本身宏块的一些公共参数抽象出来，独立起来传输，这一点在 H265 也保持下来了。这些参数分为两种，一种是序列参数集 SPS, 另一种是图像参数据 PPS。SPS 从名字就可以看出与整个序列有关。这个序列就是所处的这个 GOP，包括图像格式信息，编码参数信息，与参考图像相关的信息，档次、层和级相关信息等等。而 PPS 紧随 SPS，包含一幅图像所用的公共参数，即一幅图像中所有片段引用同一个 PPS。还存在一种数据，SEI 是 H264 标准中一个重要的技术，主要起补充和增强的作用。SEI 没有图像数据信息，只是对图像数据信息或者视频流的补充，有些内容可能对解码有帮助。
每一帧编码后的数据可以分成多个单元输出，也可以整体输出，每一个数据单元负责格式化数据并提供头信息，以保证数据适合各种信道和存储介质上的传输，这就是 NALU，也就是 NAL 单元。这个数据块是由一个字节的头和一系列来自编码层的称为原始字节序列载荷（RBSP）的字节流。
`+---------------+
  |0|1|2|3|4|5|6|7|
  +-+-+-+-+-+-+-+-+
  |F|NRI|  Type   |
  +---------------+
     NAL unit header`F: 1 bit, forbidden_zero_bit.
> 一般为0，为 1 时表示此单元出现错误，解码器会丢弃该数据，按丢包处理。
NRI: 2 bits, nal_ref_idc.
> 由编码器决定该 NAL 单元的优先级，值越高，优先级越高。00 时表示该帧不用于在重建参考图像的内部预测，可以丢弃。

> Table 2.  Example of NRI values for coded slices and coded slice
           data partitions of primary coded reference pictures
  NAL Unit Type     Content of NAL Unit              NRI (binary)
  ----------------------------------------------------------------
   1              non-IDR coded slice                         10
   2              Coded slice data partition A                10
   3              Coded slice data partition B                01
   4              Coded slice data partition C                01
       Informative note: As mentioned before, the NRI value of non-
       reference pictures is 00 as mandated by H.264.
Type:5 bits, nal_unit_type.
在网络中如何传输这些 NAL 单元，有不同的方式，常见的就是通过 RFC3984/RFC6184 定义的通过 RTP 打包的传输方式。一般情况下，在传输之前，需要事先协商好如何传输，传输什么内容。这种协商的描述方式采用 SDP 来完成。所以在会话协议中如 SIP, RTSP 等都会携带 SDP 的文本描述，使得双方完成意见交换，达成一致。
在协商过程中，除了对双方的 IP 地址，端口，编解码类型，PT 值，采样率等进行协商外，还有几个重要的参数，如 packetization-mode， profile-level-id，sprop-parameter-sets 等，这些值也在 RFC3984/RFC6184 有介绍。 如一个常见的 SDP 例中如下所示：
`m=video 49170 RTP/AVP 98
  a=rtpmap:98 H264/90000
  a=fmtp:98 profile-level-id=42A01E;
            packetization-mode=1;
            sprop-parameter-sets=<parameter sets data>`
# 分包模式
本文主讲分包，所以其他的部分以后再说，在这里相关的是 packetization-mode 这个字段，不同的值代表支持不同的分包模式。如下：
单一NAL单元模式（ Single NAL unit mode）：packetization-mode = 0 或者无此字段时缺省
非交错模式（Non-interleaved mode）: packetization-mode = 1
交错模式（Interleaved mode）: packetization-mode = 2
这三种模式的概念是基于不同的应用场景提出来的，单一分包和非交错模式都是为通信系统，人与人对话交流的场景设计，所以必须考虑低时延，按照解码的顺序传递数据包，而交错模式的场景一般是对时延并不是特别敏感的场景，传递包的顺序也可以不按照解码顺序来。所以交错模式下的包中都需要携带一个 DON (Decoding order number)。
虽然从总体上分类 3 类，但是实际上打包的类型不止三种，所以每一种模式实际上可能包含好几种打包类型。从封包的内容上看，分为两类，第一类为 single NAL unit packet，第二类为 aggregation packets。从名字上就可以看出，第一类的包种包含一个 NALU，而第二类包可能聚合了不止一包，需要解包时在重组。
对于聚合包来说，又分为 STAP-A，STAP-B，MTAP16，MTAP24 四种包类型，从名字上可以看出分为 STAP (Single-time aggregation packet) 和 MTAP (Multi-time aggregation packet)，也就是包内包含的时间戳是否相同，对于不同时间戳的包，就需要在包头内携带与参照时间戳的差值，以及解码顺序号。
`Table 4.  Type field for STAPs and MTAPs
  Type   Packet    Timestamp offset   DON-related fields
                   field length       (DON, DONB, DOND)
                   (in bits)          present
  --------------------------------------------------------
  24     STAP-A       0                 no
  25     STAP-B       0                 yes
  26     MTAP16      16                 yes
  27     MTAP24      24                 yes`有些数据太长，需要分片成多个包时，就会按照一定规则分片。分片之间需要知道开始包是谁，结束包是谁，中间包怎么区分等待，这就是 FUs( Fragmentation Units)。按照上面例如 STAP-A 和 STAP-B 这样的区别，分为 FU-A 和 FU-B，具体的区别后面再描述。但是 FUs 定义的分片只能作用于单个 NALU，如果是聚合包则不能直接使用此分片。
说了这么多的打包类型，再回头说三种分包模式。
对于 Single NAL Unit Mode，仅含有 single NAL unit packet，所以一般都是较小的包，太大的包可能分片，也不能适用此场景。协议要求接收方必须支持，只能 say YES。
对于 Non-interleaved mode，包含 single NAL unit packets，STAP-As 和 FU-As，不能包含 STAP-Bs，MTAPs 和 FU-Bs。协议要求接收方应该支持，即在一般场景都应该支持。这也是现在应用最广的模式，在通信领域，在互联网通信基本上都会支持。
对于 Interleaved mode，包含 STAP-Bs，MTAPs，FU-As，and FU-Bs，不能存在 STAP-As 和 single NAL unit packets。协议要求接收方可以支持，即按照场景决定是否支持，可以选择 NO 。
下面是一个打包类型的汇总，也作为下一部分介绍的引子。
`Table.  Summary of allowed NAL unit types for each packetization
            mode (yes = allowed, no = disallowed, ig = ignore)`|NAL Unit Type|Packet Type|Packet Type Name|Single NAL Unit Mode|Non-Interleaved Mode|Interleaved Mode|
|---|---|---|---|---|---|
|0|reserved|ig|ig|ig|
|1-23|NAL unit|Single NAL unit packet|yes|yes|no|
|24|STAP-A|Single-time aggregation packet|no|yes|no|
|25|STAP-B|Single-time aggregation packet|no|no|yes|
|26|MTAP16|Multi-time aggregation packet|no|no|yes|
|27|MTAP24|Multi-time aggregation packet|no|no|yes|
|28|FU-A|Fragmentation unit|no|yes|yes|
|29|FU-B|Fragmentation unit|no|no|yes|
|30-31|reserved|ig|ig|ig|
# 打包类型
## Single NAL Unit Packet
单 NALU 包比较简单，除了 RTP 头外，后面直接放上去掉开始码得 NALU 即可。如果要多个小包一起，又不想采用聚合包，那么小包之前得加上开始码，否则解码器不认识。
`0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|F|NRI|  Type   |                                               |
+-+-+-+-+-+-+-+-+                                               |
|                                                               |
|               Bytes 2..n of a single NAL unit                 |
|                                                               |
|                               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                               :...OPTIONAL RTP padding        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
Figure 2.  RTP payload format for single NAL unit packet`
## Agregation packet
聚合包与单NALU包比起来，差别还是在内容上，格式上起始基本上一致。如下所示。
`0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|F|NRI|  Type   |                                               |
+-+-+-+-+-+-+-+-+                                               |
|                                                               |
|             one or more aggregation units                     |
|                                                               |
|                               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                               :...OPTIONAL RTP padding        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
Figure 3.  RTP payload format for aggregation packets`
### 单时间聚合-STAP
STAP-A 用于非交错模式。STAP-用于交错模式，它们结构上最大的区别是是否需要携带 DON，每个 NALU 都会有长度标识。DON 和 NALU Size 都是无符号整数，网络字节序形式。如下是两种包的 RTP 封装格式示意图。
`0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                          RTP Header                           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|STAP-A NAL HDR |         NALU 1 Size           | NALU 1 HDR    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                         NALU 1 Data                           |
:                                                               :
+               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|               | NALU 2 Size                   | NALU 2 HDR    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                         NALU 2 Data                           |
:                                                               :
|                               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                               :...OPTIONAL RTP padding        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
Figure 7.  An example of an RTP packet including an STAP-A
           containing two single-time aggregation units
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                          RTP Header                           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|STAP-B NAL HDR | DON                           | NALU 1 Size   |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| NALU 1 Size   | NALU 1 HDR    | NALU 1 Data                   |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               +
:                                                               :
+               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|               | NALU 2 Size                   | NALU 2 HDR    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                       NALU 2 Data                             |
:                                                               :
|                               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                               :...OPTIONAL RTP padding        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
Figure 8.  An example of an RTP packet including an STAP-B
           containing two single-time aggregation units`
### 多时间聚合-MTAP
MTAP 聚合包与STAP比起来，差别最大的在头部。因为可能聚合多个不同时间的包，所以必须携带一个基准的 DON, timestamp。DOND 和 TS offset 是与基准的差值，这个基准应该是取自时间最早的那个 NALU 的值。DONB 和 NALU Size 为 16 位无符号整数，DOND 为 8 位无符号整数，TS offset 为 16 或者 24 位 无符号整数，均为网络字节序形式。
MTAP16 和 MTAP24 在本质上并没有什么差别，只是用了更多的位数来表达时间差，如果这个位数越多，可以容纳的时间戳差值越大，也就代表 MTAP 具有更大的扩展性，但是也占用了更多的头部字节，还是需要综合考虑性价比。
`0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                          RTP Header                           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|MTAP16 NAL HDR |  decoding order number base   | NALU 1 Size   |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  NALU 1 Size  |  NALU 1 DOND  |       NALU 1 TS offset        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  NALU 1 HDR   |  NALU 1 DATA                                  |
+-+-+-+-+-+-+-+-+                                               +
:                                                               :
+               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|               | NALU 2 SIZE                   |  NALU 2 DOND  |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|       NALU 2 TS offset        |  NALU 2 HDR   |  NALU 2 DATA  |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+               |
:                                                               :
|                               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                               :...OPTIONAL RTP padding        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
Figure 12.  An RTP packet including a multi-time aggregation
            packet of type MTAP16 containing two multi-time
            aggregation units

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                          RTP Header                           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|MTAP24 NAL HDR |  decoding order number base   | NALU 1 Size   |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  NALU 1 Size  |  NALU 1 DOND  |       NALU 1 TS offs          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|NALU 1 TS offs |  NALU 1 HDR   |  NALU 1 DATA                  |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               +
:                                                               :
+               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|               | NALU 2 SIZE                   |  NALU 2 DOND  |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|       NALU 2 TS offset                        |  NALU 2 HDR   |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  NALU 2 DATA                                                  |
:                                                               :
|                               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                               :...OPTIONAL RTP padding        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
Figure 13.  An RTP packet including a multi-time aggregation
            packet of type MTAP24 containing two multi-time
            aggregation units`
### 分片单元-FUs
前面提到了 FUs 只能作用到一个 NALU 上，也就是对同一个 NALU 进行分片。在网络传输中，将大的数据单元改小是很有意义的，比如有利于包的传输，有利于纠错机制的使用等，在接收端必须按照分片方式重组。
`0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| FU indicator  |   FU header   |                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               |
|                                                               |
|                         FU payload                            |
|                                                               |
|                               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                               :...OPTIONAL RTP padding        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
           Figure 14.  RTP payload format for FU-A
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| FU indicator  |   FU header   |               DON             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-|
|                                                               |
|                         FU payload                            |
|                                                               |
|                               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                               :...OPTIONAL RTP padding        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
           Figure 15.  RTP payload format for FU-B`同样分为两种格式 FU-A 和 FU-B，结构差别只在于一个 DON，相同的是都有两个头 FU indicator 和 FU header，大小都是一个字节。
`+---------------+
  |0|1|2|3|4|5|6|7|
  +-+-+-+-+-+-+-+-+
  |F|NRI|  Type   |
  +---------------+
   FU indicator format
  +---------------+
  |0|1|2|3|4|5|6|7|
  +-+-+-+-+-+-+-+-+
  |S|E|R|  Type   |
  +---------------+
   FU header format`分片单元不能用于传输一个分片，因为不能同时在一个 FU 头里面设置开始和结束标志。
# 其他
这篇文章只写了针对 TS 流的打包，而且对于协议中很多参数没有说明，有需要以后再仔细解释，这里简单列举一下。
`Table 6.  Interpretation of parameters for different direction
             attributes
                                          sendonly --+
                                       recvonly --+  |
                                    sendrecv --+  |  |
                                               |  |  |
            profile-level-id                   C  C  P
            max-recv-level                     R  R  -
            packetization-mode                 C  C  P
            sprop-deint-buf-req                P  -  P
            sprop-interleaving-depth           P  -  P
            sprop-max-don-diff                 P  -  P
            sprop-init-buf-time                P  -  P
            max-mbps                           R  R  -
            max-smbps                          R  R  -
            max-fs                             R  R  -
            max-cpb                            R  R  -
            max-dpb                            R  R  -
            max-br                             R  R  -
            redundant-pic-cap                  R  R  -
            deint-buf-cap                      R  R  -
            max-rcmd-nalu-size                 R  R  -
            sar-understood                     R  R  -
            sar-supported                      R  R  -
            in-band-parameter-sets             R  R  -
            use-level-src-parameter-sets       R  R  -
            level-asymmetry-allowed            O  -  -
            sprop-parameter-sets               S  -  S
            sprop-level-parameter-sets         S  -  S
         Legend:
         C: configuration for sending and receiving streams
         O: offer/answer mode
         P: properties of the stream to be sent
         R: receiver capabilities
         S: out-of-band parameter sets
         -: not usable (when present, SHOULD be ignored)`

