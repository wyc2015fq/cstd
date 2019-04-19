# h.264 rtp打包 - maopig的专栏 - CSDN博客
2011年08月07日 18:37:50[maopig](https://me.csdn.net/maopig)阅读数：3047标签：[h.264																[header																[video																[网络](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=video&t=blog)](https://so.csdn.net/so/search/s.do?q=header&t=blog)](https://so.csdn.net/so/search/s.do?q=h.264&t=blog)
个人分类：[RTSP/RTP](https://blog.csdn.net/maopig/article/category/861027)
(2011-05-27 08:44:13)
[转载]()
|标签：### [杂谈](http://uni.sina.com.cn/c.php?t=blog&k=%D4%D3%CC%B8&ts=bpost&stype=tag)||
payload,H.264 RTP payload 格式
on 2011-2-18 in 博文摘选 | 0 Comment
1. 网络抽象层单元类型 (NALU)
NALU 头由一个字节组成, 它的语法如下:
+---------------+
|0|1|2|3|4|5|6|7|
+-+-+-+-+-+-+-+-+
|F|NRI| Type |
+---------------+
F: 1 个比特.
forbidden_zero_bit. 在 H.264 规范中规定了这一位必须为 0.
NRI: 2 个比特. nal_ref_idc. 取 00 ~ 11, 似乎指示这个 NALU 的重要性, 如 00 的 NALU 解码器可以丢弃它而不影响图像的回放. 不过一般情况下不太关心这个属性.
Type: 5 个比特.
nal_unit_type. 这个 NALU 单元的类型. 简述如下:
0 没有定义
1-23 NAL单元 单个 NAL 单元包.
24 STAP-A 单一时间的组合包
24 STAP-B 单一时间的组合包
26 MTAP16 多个时间的组合包
27 MTAP24 多个时间的组合包
28 FU-A 分片的单元
29 FU-B 分片的单元
30-31 没有定义
2. 打包模式
下面是 RFC 3550 中规定的 RTP 头的结构.
0 1 2 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|V=2|P|X| CC |M| PT | sequence number |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| timestamp |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| synchronization source (SSRC) identifier |
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
| contributing source (CSRC) identifiers |
| .... |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
负载类型 Payload type (PT): 7 bits
序列号 Sequence number (SN): 16 bits
时间戳 Timestamp: 32 bits
H.264 Payload 格式定义了三种不同的基本的负载(Payload)结构. 接收端可能通过 RTP Payload
的第一个字节来识别它们. 这一个字节类似 NALU 头的格式, 而这个头结构的 NAL 单元类型字段
则指出了代表的是哪一种结构,
这个字节的结构如下, 可以看出它和 H.264 的 NALU 头结构是一样的.
+---------------+
|0|1|2|3|4|5|6|7|
+-+-+-+-+-+-+-+-+
|F|NRI| Type |
+---------------+
字段 Type: 这个 RTP payload 中 NAL 单元的类型. 这个字段和 H.264 中类型字段的区别是, 当 type
的值为 24 ~ 31 表示这是一个特别格式的 NAL 单元, 而 H.264 中, 只取 1~23 是有效的值.
24 STAP-A 单一时间的组合包
24 STAP-B 单一时间的组合包
26 MTAP16 多个时间的组合包
27 MTAP24 多个时间的组合包
28 FU-A 分片的单元
29 FU-B 分片的单元
30-31 没有定义
可能的结构类型分别有:
1. 单一 NAL 单元模式
即一个 RTP 包仅由一个完整的 NALU 组成. 这种情况下 RTP NAL 头类型字段和原始的 H.264的
NALU 头类型字段是一样的.
2. 组合封包模式
即可能是由多个 NAL 单元组成一个 RTP 包. 分别有4种组合方式: STAP-A, STAP-B, MTAP16, MTAP24.
那么这里的类型值分别是 24, 25, 26 以及 27.
3. 分片封包模式
用于把一个 NALU 单元封装成多个 RTP 包. 存在两种类型 FU-A 和 FU-B. 类型值分别是 28 和 29.
2.1 单一 NAL 单元模式(Single NAL Unit Packet: 一个RTP Payload 仅包含一个NAL 单元, NALU 类型号为1~23, 和H264 标准码流非常接近.)
对于 NALU 的长度小于 MTU 大小的包, 一般采用单一 NAL 单元模式.对于一个原始的 H.264 NALU 单元常由 [Start Code] [NALU Header] [NALU Payload] 三部分组成, 其中 Start Code 用于标示这是一个NALU 单元的开始, 必须是 "00 00 00 01" 或 "00 00 01", NALU 头仅一个字节, 其后都是 NALU 单元内容.
打包时去除 "00 00 01" 或 "00 00 00 01" 的开始码, 把其他数据封包的 RTP 包即可.
0 1 2 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|F|NRI| type | |
+-+-+-+-+-+-+-+-+ |
| |
| Bytes 2..n of a Single NAL unit |
| |
| +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| :...OPTIONAL RTP padding |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
如有一个 H.264 的 NALU 是这样的:
[00 00 00 01 67 42 A0 1E 23 56 0E 2F ... ]
这是一个序列参数集 NAL 单元. [00 00 00 01] 是四个字节的开始码, 67 是 NALU 头, 42 开始的数据是 NALU 内容.
封装成 RTP 包将如下:
[ RTP Header ] [ 67 42 A0 1E 23 56 0E 2F ]
即只要去掉 4 个字节的开始码就可以了.
2.2 组合封包模式(Aggregation packet: 一个RTP Payload 包含多个NAL 单元. 有STAP-A, STAP-B, MTAP-A, MTAP-B 四种类型, 编号依次为 24, 25, 26, 27.)
其次, 当 NALU 的长度特别小时, 可以把几个 NALU 单元封在一个 RTP 包中.
0 1 2 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| RTP Header |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|STAP-A NAL HDR | NALU 1 Size | NALU 1 HDR |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| NALU 1 Data |
: :
+ +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| | NALU 2 Size | NALU 2 HDR |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| NALU 2 Data |
: :
| +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| :...OPTIONAL RTP padding |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
2.3 Fragmentation Units (FUs).(Fragmentation unit: 一个RTP Payload 包含一个NAL 单元的一部分. 有FU-A, FU-B 两种,分别标记为 28, 29.)
而当 NALU 的长度超过 MTU 时, 就必须对 NALU 单元进行分片封包. 也称为 Fragmentation Units (FUs).
0 1 2 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| FU indicator | FU header | |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ |
| |
| FU payload |
| |
| +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| :...OPTIONAL RTP padding |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
Figure 14. RTP payload format for FU-A
The FU indicator octet has the following format:
+---------------+
|0|1|2|3|4|5|6|7|
+-+-+-+-+-+-+-+-+
|F|NRI| Type |
+---------------+
The FU header has the following format:
+---------------+
|0|1|2|3|4|5|6|7|
+-+-+-+-+-+-+-+-+
|S|E|R| Type |
+---------------+
{
1，将FU-A包根据rtp的包序号和FU-A的header字节，组成完整264帧；
2，判断帧类型如果是idr帧，需要在帧头加上sps和pps，格式为：
起始码（0x00000001）+sps+起始码+pps+起始码+完整帧数据
3，帧末添加帧间分隔符“0x00, 0x00, 0x01, 0x09, 0x10”
4，使用网上精简过的"ff_264_dec_vc"进行解码。该项目不支持imgconvert，可从最新ff源码查找
并拷贝yuv420p_to_xxx函数进行合适转码
}
3. SDP 参数
下面描述了如何在 SDP 中表示一个 H.264 流:
. "m=" 行中的媒体名必须是 "video"
. "a=rtpmap" 行中的编码名称必须是 "H264".
. "a=rtpmap" 行中的时钟频率必须是 90000.
. 其他参数都包括在 "a=fmtp" 行中.
如:
m=video 49170 RTP/AVP 98
a=rtpmap:98 H264/90000
a=fmtp:98 profile-level-id=42A01E; sprop-parameter-sets=Z0IACpZTBYmI,aMljiA==
下面介绍一些常用的参数.
3.1 packetization-mode:
表示支持的封包模式.
当 packetization-mode 的值为 0 时或不存在时, 必须使用单一 NALU 单元模式.
当 packetization-mode 的值为 1 时必须使用非交错(non-interleaved)封包模式.
当 packetization-mode 的值为 2 时必须使用交错(interleaved)封包模式.
这个参数不可以取其他的值.
3.2 sprop-parameter-sets:
这个参数可以用于传输 H.264 的序列参数集和图像参数 NAL 单元. 这个参数的值采用 Base64 进行编码. 不同的参数集间用","号隔开.
3.3 profile-level-id:
这个参数用于指示 H.264 流的 profile 类型和级别. 由 Base16(十六进制) 表示的 3 个字节. 第一个字节表示 H.264 的 Profile 类型, 第
三个字节表示 H.264 的 Profile 级别:
3.4 max-mbps:
这个参数的值是一个整型, 指出了每一秒最大的宏块处理速度.
