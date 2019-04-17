# H.264 RTPpayload 格式------ H.264 视频 RTP 负载格式（包含AAC部分解析） - DoubleLi - 博客园






**[H.264 RTPpayload 格式](http://www.cppblog.com/czanyou/archive/2008/11/26/67940.html)------**

**H.264 视频 RTP 负载格式**

**1. 网络抽象层单元类型 (NALU)**

NALU 头由一个字节组成, 它的语法如下:

     +---------------+
      |0|1|2|3|4|5|6|7|
      +-+-+-+-+-+-+-+-+
      |F|NRI|  Type   |
      +---------------+

**F: 1 个比特(禁止位).**
  forbidden_zero_bit. 在 H.264 规范中规定了这一位必须为 0.

**NRI: 2 个比特.**
  nal_ref_idc. 取 00 ~ 11, 似乎指示这个NALU 的重要性,如00 的NALU 解码器可以丢弃它而不影响图像的回放.不过一般情况下不太关心这个属性.

**Type: 5 个比特.**  nal_unit_type. 这个 NALU 单元的类型.简述如下:

 0     没有定义
  1-23  NAL单元  单个NAL 单元包.
  24   STAP-A   单一时间的组合包
  25    STAP-B   单一时间的组合包
  26    MTAP16   多个时间的组合包
  27    MTAP24   多个时间的组合包
  28   FU-A     分片的单元
  29    FU-B     分片的单元
  30-31 没有定义



**2. 打包模式**

**  下面是 RFC 3550 中规定的 RTP 头的结构(12字节).**

    0                  1                  2                  3
    0 1 2 3 4 5 6 7 8 9 0 1 2 34 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |V=2|P|X|  CC   |M|    PT      |       sequencenumber         |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                          timestamp                          |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |          synchronization source (SSRC) identifier           |
   +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
   |           contributing source (CSRC)identifiers            |
   |                            ....                             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

  负载类型Payload type (PT): 7 bits
  序列号Sequence number (SN): 16 bits
  时间戳Timestamp: 32 bits

  H.264 Payload 格式定义了三种不同的基本的负载(Payload)结构.接收端可能通过RTP Payload 的第一个字节来识别它们.这一个字节类似NALU 头的格式,而这个头结构的NAL 单元类型字段则指出了代表的是哪一种结构,

  这个字节的结构如下,可以看出它和H.264 的NALU 头结构是一样的.
      +---------------+
      |0|1|2|3|4|5|6|7|
      +-+-+-+-+-+-+-+-+
      |F|NRI|  Type   |
      +---------------+
  字段Type: 这个RTP payload 中NAL 单元的类型.这个字段和H.264 中类型字段的区别是,当type的值为24 ~ 31 表示这是一个特别格式的NAL 单元,而H.264 中,只取1~23 是有效的值.

  24    STAP-A   单一时间的组合包
  25    STAP-B   单一时间的组合包
  26    MTAP16   多个时间的组合包
  27    MTAP24   多个时间的组合包
  28    FU-A     分片的单元
  29    FU-B     分片的单元
  30-31 没有定义

  可能的结构类型分别有:

  1. 单一 NAL 单元模式
     即一个 RTP 包仅由一个完整的NALU 组成.这种情况下RTP NAL 头类型字段和原始的H.264的NALU 头类型字段是一样的.

  2. 组合封包模式
    即可能是由多个 NAL 单元组成一个RTP 包.分别有4种组合方式:STAP-A, STAP-B, MTAP16, MTAP24.那么这里的类型值分别是 24, 25, 26以及27.

  3. 分片封包模式
    用于把一个 NALU 单元封装成多个RTP 包.存在两种类型 FU-A 和 FU-B. 类型值分别是28 和 29.

2.1 单一NAL 单元模式

  对于NALU 的长度小于MTU 大小的包,一般采用单一NAL 单元模式.
  对于一个原始的H.264 NALU 单元常由[Start Code] [NALU Header] [NALU Payload] 三部分组成, 其中Start Code 用于标示这是一个NALU 单元的开始,必须是"00 00 00 01" 或 "00 00 01", NALU 头仅一个字节,其后都是NALU 单元内容.
  打包时去除 "00 00 01" 或 "00 00 00 01" 的开始码, 把其他数据封包的 RTP 包即可.

      0                  1                  2                  3
       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 01 2 3 4 5 6 7 8 9 0 1
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |F|NRI|  type  |                                              |
     +-+-+-+-+-+-+-+-+                                              |
     |                                                              |
     |              Bytes 2..n of a Single NALunit                |
     |                                                              |
     |                              +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     |                              :...OPTIONAL RTP padding        |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


  如有一个H.264 的NALU 是这样的:

  [00 00 00 01 67 42 A0 1E 23 56 0E 2F ... ]

  这是一个序列参数集NAL 单元.[00 00 00 01] 是四个字节的开始码,67 是NALU 头,42 开始的数据是NALU 内容.封装成RTP 包将如下:

  [ RTP Header ] [ 67 42 A0 1E 23 56 0E 2F ]

  即只要去掉4 个字节的开始码就可以了.


2.2 组合封包模式

  其次,当NALU 的长度特别小时,可以把几个NALU 单元封在一个RTP 包中.


      0                  1                  2                  3
       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 01 2 3 4 5 6 7 8 9 0 1
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                         RTPHeader                          |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |STAP-A NAL HDR|         NALU 1Size           | NALU 1HDR    |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     |                        NALU 1 Data                          |
     :                                                              :
     +              +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     |              | NALU 2Size                  | NALU 2 HDR    |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     |                        NALU 2 Data                          |
     :                                                              :
     |                              +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     |                              :...OPTIONAL RTP padding        |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


2.3 Fragmentation Units (FUs).

  而当NALU 的长度超过MTU 时,就必须对NALU 单元进行分片封包.也称为Fragmentation Units (FUs).

      0                  1                  2                  3
       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 01 2 3 4 5 6 7 8 9 0 1
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      | FU indicator  |   FUheader  |                              |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                              |
     |                                                              |
     |                        FU payload                           |
     |                                                              |
     |                              +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                              :...OPTIONAL RTP padding        |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

     Figure 14.  RTP payload format for FU-A

   The FUindicator octet has the following format:

     +---------------+
      |0|1|2|3|4|5|6|7|
      +-+-+-+-+-+-+-+-+
      |F|NRI|  Type   |
      +---------------+

   The FU headerhas the following format:

     +---------------+
      |0|1|2|3|4|5|6|7|
      +-+-+-+-+-+-+-+-+
      |S|E|R|  Type   |
      +---------------+


3. SDP 参数

  下面描述了如何在SDP 中表示一个H.264 流:

  . "m=" 行中的媒体名必须是"video"
  . "a=rtpmap" 行中的编码名称必须是"H264".
  . "a=rtpmap" 行中的时钟频率必须是 90000.
  . 其他参数都包括在"a=fmtp" 行中.

  如:

  m=video 49170 RTP/AVP98
  a=rtpmap:98 H264/90000
  a=fmtp:98 profile-level-id=42A01E;sprop-parameter-sets=Z0IACpZTBYmI,aMljiA==

  下面介绍一些常用的参数.

3.1packetization-mode:
  表示支持的封包模式.
  当packetization-mode 的值为 0 时或不存在时,必须使用单一NALU 单元模式.
  当packetization-mode 的值为 1 时必须使用非交错(non-interleaved)封包模式.
  当packetization-mode 的值为 2 时必须使用交错(interleaved)封包模式.
  这个参数不可以取其他的值.

3.2sprop-parameter-sets:
  这个参数可以用于传输H.264 的序列参数集和图像参数NAL 单元.这个参数的值采用Base64 进行编码.不同的参数集间用","号隔开.

3.3 profile-level-id:
  这个参数用于指示H.264 流的profile 类型和级别.由Base16(十六进制)表示的3 个字节.第一个字节表示H.264 的Profile 类型,第

三个字节表示H.264 的Profile 级别:

3.4 max-mbps:
  这个参数的值是一个整型,指出了每一秒最大的宏块处理速度.



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



**一、NALU打包成RTP的方式有三种：**

1. 单一 NAL 单元模式
     即一个RTP 包仅由一个完整的 NALU 组成. 这种情况下 RTP NAL 头类型字段和原始的 H.264的
NALU 头类型字段是一样的.

2. 组合封包模式
    即可能是由多个NAL 单元组成一个 RTP 包. 分别有4种组合方式: STAP-A, STAP-B, MTAP16, MTAP24.
那么这里的类型值分别是 24, 25, 26 以及 27.

3. 分片封包模式
    用于把一个NALU 单元封装成多个 RTP 包. 存在两种类型 FU-A 和 FU-B. 类型值分别是 28 和 29.

还记得前面nal_unit_type的定义吧，0~23是给H264用的，24~31未使用，在rtp打包时，如果一个NALU放在一个RTP包里，可以使用NALU的nal_unit_type，但是当需要把多个NALU打包成一个RTP包，或者需要把一个NALU打包成多个RTP包时，就定义新的type来标识。

*Type  Packet      Typename                           ---------------------------------------------------------     0     undefined                                   -     1-23   NAL unit    Single NAL unit packet perH.264       24     STAP-A     Single-timeaggregation packet         25     STAP-B     Single-timeaggregation packet         26     MTAP16    Multi-time aggregationpacket          27     MTAP24    Multi-time aggregationpacket          28     FU-A      Fragmentationunit                    29     FU-B      Fragmentationunit                     30-31 undefined                   *



**二、三种打包方式的具体格式**

1 .单一 NAL 单元模式

对于 NALU 的长度小于 MTU 大小的包, 一般采用单一 NAL 单元模式.
对于一个原始的 H.264 NALU 单元常由 [Start Code] [NALU Header] [NALU Payload] 三部分组成, 其中 Start Code 用于标示这是一个

NALU 单元的开始, 必须是 "00 00 00 01" 或 "00 00 01", NALU 头仅一个字节, 其后都是 NALU 单元内容.
打包时去除 "00 00 01" 或 "00 00 00 01" 的开始码, 把其他数据封包的 RTP 包即可.

      0                  1                  2                  3
       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 01 2 3 4 5 6 7 8 9 0 1
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |F|NRI| type  |                                              |
     +-+-+-+-+-+-+-+-+                                              |
     |                                                              |
     |              Bytes 2..n of a Single NALunit                |
     |                                                              |
     |                              +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     |                              :...OPTIONAL RTP padding        |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


如有一个 H.264 的 NALU 是这样的:

[00 00 00 01 67 42 A0 1E 23 56 0E 2F... ]

这是一个序列参数集 NAL 单元. [00 00 00 01] 是四个字节的开始码,67 是 NALU 头, 42 开始的数据是 NALU 内容.

封装成 RTP 包将如下:

[ RTP Header ] [ 67 42 A0 1E 23 56 0E 2F]

即只要去掉 4 个字节的开始码就可以了.


2 组合封包模式

其次, 当 NALU 的长度特别小时, 可以把几个 NALU 单元封在一个 RTP 包中.


      0                  1                  2                  3
       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 01 2 3 4 5 6 7 8 9 0 1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     |                         RTP Header                          |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |STAP-A NAL HDR|         NALU 1Size           | NALU 1HDR    |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     |                        NALU 1 Data                          |
     :                                                              :
     +              +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     |              | NALU 2Size                  | NALU 2 HDR    |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     |                        NALU 2 Data                          |
      :                                                              :
     |                              +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     |                              :...OPTIONAL RTP padding        |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


3 FragmentationUnits (FUs).

而当 NALU 的长度超过 MTU 时, 就必须对 NALU 单元进行分片封包. 也称为 Fragmentation Units (FUs).

      0                  1                  2                  3
       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 01 2 3 4 5 6 7 8 9 0 1
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      | FU indicator |   FUheader  |                              |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                              |
     |                                                              |
     |                        FU payload                           |
     |                                                              |
     |                              +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     |                              :...OPTIONAL RTP padding        |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

      Figure 14. RTPpayload format for FU-A

**  FU indicator有以下格式：**     +---------------+
      |0|1|2|3|4|5|6|7|
      +-+-+-+-+-+-+-+-+
      |F|NRI|  Type   |
      +---------------+
   FU指示字节的类型域 Type=28表示FU-A。。NRI域的值必须根据分片NAL单元的NRI域的值设置。



**   FU header的格式如下：**     +---------------+
      |0|1|2|3|4|5|6|7|
      +-+-+-+-+-+-+-+-+
      |S|E|R|  Type   |
      +---------------+
   S: 1 bit
   当设置成1,开始位指示分片NAL单元的开始。当跟随的FU荷载不是分片NAL单元荷载的开始，开始位设为0。
   E: 1 bit
   当设置成1, 结束位指示分片NAL单元的结束，即, 荷载的最后字节也是分片NAL单元的最后一个字节。当跟随的FU荷载不是分片NAL单元的最后分片,结束位设置为0。
   R: 1 bit
   保留位必须设置为0，接收者必须忽略该位。
   Type: 5 bits



**1、单个NAL包单元**

12字节的RTP头后面的就是音视频数据，比较简单。一个封装单个NAL单元包到RTP的NAL单元流的RTP序号必须符合NAL单元的解码顺序。

**2、FU-A的分片格式**
数据比较大的H264视频包，被RTP分片发送。12字节的RTP头后面跟随的就是FU-A分片：
FU indicator有以下格式：
      +---------------+
      |0|1|2|3|4|5|6|7|
      +-+-+-+-+-+-+-+-+
      |F|NRI|  Type   |
      +---------------+
   FU指示字节的类型域 Type=28表示FU-A。。NRI域的值必须根据分片NAL单元的NRI域的值设置。

   FU header的格式如下：
      +---------------+
      |0|1|2|3|4|5|6|7|
      +-+-+-+-+-+-+-+-+
      |S|E|R|  Type   |
      +---------------+
   S: 1 bit
   当设置成1,开始位指示分片NAL单元的开始。当跟随的FU荷载不是分片NAL单元荷载的开始，开始位设为0。
   E: 1 bit
   当设置成1, 结束位指示分片NAL单元的结束，即, 荷载的最后字节也是分片NAL单元的最后一个字节。当跟随的FU荷载不是分片NAL单元的最后分片,结束位设置为0。
   R: 1 bit
   保留位必须设置为0，接收者必须忽略该位。
   Type: 5 bits
   NAL单元荷载类型定义见下表


表1.  单元类型以及荷载结构总结
      Type  Packet      Typename                      
     ---------------------------------------------------------
      0     undefined                                   -
      1-23   NALunit    Single NAL unit packet per H.264  
      24    STAP-A     Single-time aggregation packet   
      25    STAP-B     Single-time aggregation packet   
      26    MTAP16    Multi-time aggregation packet     
      27    MTAP24    Multi-time aggregation packet     
      28     FU-A     Fragmentation unit               
      29    FU-B      Fragmentationunit                
      30-31 undefined                                   -



**3、拆包和解包**

拆包：当编码器在编码时需要将原有一个NAL按照FU-A进行分片，原有的NAL的单元头与分片后的FU-A的单元头有如下关系：
原始的NAL头的前三位为FU indicator的前三位，原始的NAL头的后五位为FU header的后五位，FUindicator与FU header的剩余位数根据实际情况决定。

解包：当接收端收到FU-A的分片数据，需要将所有的分片包组合还原成原始的NAL包时，FU-A的单元头与还原后的NAL的关系如下：
还原后的NAL头的八位是由FU indicator的前三位加FU header的后五位组成，即：
nal_unit_type = (fu_indicator & 0xe0) | (fu_header & 0x1f)

4、代码实现

从RTP包里面得到H264视频数据的方法：


// 功能：解码RTP H.264视频
// 参数：1.RTP包缓冲地址 2.RTP包数据大小 3.H264输出地址 4.输出数据大小
// 返回：true:表示一帧结束  false:FU-A分片未结束或帧未结束 
#define  RTP_HEADLEN 12 
bool  UnpackRTPH264( void  *  bufIn,  int len,   void **  pBufOut,  int   *  pOutLen)
{
     * pOutLen  =   0 ;
     if  (len  <  RTP_HEADLEN)
    {
        return   false ;
    } 

    unsigned char *  src  =  (unsigned  char* )bufIn +  RTP_HEADLEN;
    unsigned char  head1 =   * src; // 获取第一个字节 
    unsigned  char  head2 =   * (src + 1 ); // 获取第二个字节 
    unsigned  char  nal =  head1 &   0x1f; // 获取FU indicator的类型域， 
    unsigned  char  flag =  head2 &   0xe0 ; // 获取FU header的前三位，判断当前是分包的开始、中间或结束 
    unsigned  char  nal_fua =  (head1 &   0xe0 )  |  (head2 &   0x1f); // FU_A nal 
    bool  bFinishFrame =   false ;
     if  (nal == 0x1c ) // 判断NAL的类型为0x1c=28，说明是FU-A分片 
    { // fu-a 
        if  (flag== 0x80 ) // 开始 
         {
            * pBufOut =  src - 3 ; // lostyears: 本人觉得，这里应该-4，就是给留给startcode的4个字节
            * (( int * )( * pBufOut))  =   0x01000000  ; // zyf:大模式会有问题, lostyears: 这里的value就是startcode
             * ((char * )( * pBufOut) + 4 )  =  nal_fua; // lostyears: 第五个字节即nal head
            *  pOutLen =  len -  RTP_HEADLEN +   3 ; // lostyears: 这里就应该+4了，因为startcode是4个字节（如果是3个字节，那中间两行代码就得改一下）
        }
        else   if (flag == 0x40 ) // 结束 
         {
            * pBufOut =  src + 2 ; // lostyears: 去掉FU indicator 和 FU header两个字节
            *  pOutLen =  len -  RTP_HEADLEN -   2 ;
        }
        else // 中间 
         {
            * pBufOut =  src + 2 ;
            *  pOutLen =  len -  RTP_HEADLEN -   2 ;
        }
    } 
     else // 单包数据 
    {
        * pBufOut =  src - 4 ;
        * (( int * )( * pBufOut))  =   0x01000000 ; // zyf:大模式会有问题 
         *  pOutLen =  len -  RTP_HEADLEN +   4 ;
    } 

    unsigned char *  bufTmp  = (unsigned  char* )bufIn;
     if  (bufTmp[ 1 ]  &   0x80 )
    {
        bFinishFrame =   true ; // rtp mark 
    }
     else 
    {
        bFinishFrame =   false ;
    } 
     return  bFinishFrame;
}   




从RTP包里面得到AAC音频数据的方法：


//功能：解RTP AAC音频包，声道和采样频率必须知道。
//参数：1.RTP包缓冲地址 2.RTP包数据大小 3.H264输出地址 4.输出数据大小
//返回：true:表示一帧结束  false:帧未结束 一般AAC音频包比较小，没有分片。
bool UnpackRTPAAC(void * bufIn, int recvLen, void** pBufOut,  int* pOutLen)
{
    unsigned char*  bufRecv = (unsigned char*)bufIn;
    //char strFileName[20];

    unsigned char ADTS[] = {0xFF, 0xF1, 0x00, 0x00, 0x00, 0x00, 0xFC}; 
    int audioSamprate = 32000;//音频采样率
    int audioChannel = 2;//音频声道 1或2
    int audioBit = 16;//16位 固定
    switch(audioSamprate)
    {
    case  16000:
        ADTS[2] = 0x60;
        break;
    case  32000:
        ADTS[2] = 0x54;
        break;
    case  44100:
        ADTS[2] = 0x50;
        break;
    case  48000:
        ADTS[2] = 0x4C;
        break;
    case  96000:
        ADTS[2] = 0x40;
        break;
    default:
        break;
    }
    ADTS[3] = (audioChannel==2)?0x80:0x40;

    int len = recvLen - 16 + 7; // lostyears: 12个字节是RTP head，还有4个字节是表示占两字节的AU_HEADER_LENGTH+占两字节的*AU_HEADER*

*//AAC封装RTP比较简单//将AAC的ADTS头去掉//12字节RTP头后紧跟着2个字节的AU_HEADER_LENGTH,*

*//然后是2字节的AU_HEADER(2 bytes: 13 bits = length of frame, 3 bits = AU-Index(-delta)))，之后就是AAC payload。*

*//所以要得到AACpayload*

*//payLen= （UINT16）usAuheader >> 3;（这里要注意usAuheader的值，RTP中是网络序的，要转成主机序）*


    len <<= 5;//8bit * 2 - 11 = 5(headerSize 11bit)
    len |= 0x1F;//5 bit    1            
    ADTS[4] = len>>8;
    ADTS[5] = len & 0xFF;
    *pBufOut = (char*)bufIn+16-7;
    memcpy(*pBufOut, ADTS, sizeof(ADTS));
    *pOutLen = recvLen - 16 + 7;

    unsigned char* bufTmp = (unsigned char*)bufIn;
    bool bFinishFrame = false;
    if (bufTmp[1] & 0x80)
    {
        //DebugTrace::D("Marker");
        bFinishFrame = true;
    }
    else
    {
        bFinishFrame = false;
    }
    return true;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



## 1.ADTS是个啥



ADTS全称是(Audio Data Transport Stream)，是AAC的一种十分常见的传输格式。

记得第一次做demux的时候，把AAC音频的ES流从FLV封装格式中抽出来送给硬件解码器时，不能播;保存到本地用pc的播放器播时，我靠也不 能播。当时崩溃了，后来通过查找资料才知道。一般的AAC解码器都需要把AAC的ES流打包成ADTS的格式，一般是在AAC ES流前添加7个字节的ADTS header。也就是说你可以吧ADTS这个头看作是AAC的frameheader。


|ADTS AAC| | | | | | |
|----|----|----|----|----|----|----|
|ADTS_header|AAC ES|ADTS_header|AAC ES|...|ADTS_header|AAC ES|



## 2.ADTS内容及结构

ADTS 头中相对有用的信息 采样率、声道数、帧长度。想想也是，我要是解码器的话，你给我一堆得AAC音频ES流我也解不出来。每一个带ADTS头信息的AAC流会清晰的告送解码器他需要的这些信息。

一般情况下ADTS的头信息都是7个字节，分为2部分：

adts_fixed_header();

adts_variable_header();

![](http://my.csdn.net/uploads/201203/31/1333164211_8586.jpg)



syncword ：同步头 总是0xFFF, all bits must be 1，代表着一个ADTS帧的开始

ID：MPEG Version: 0 for MPEG-4, 1 for MPEG-2

Layer：always: '00'

profile：表示使用哪个级别的AAC，有些芯片只支持AAC LC 。在MPEG-2 AAC中定义了3种：

![](http://my.csdn.net/uploads/201203/31/1333165759_1644.jpg)

sampling_frequency_index：表示使用的采样率下标，通过这个下标在 Sampling Frequencies[ ]数组中查找得知采样率的值。

There are 13 supported frequencies:
- 0: 96000 Hz
- 1: 88200 Hz
- 2: 64000 Hz
- 3: 48000 Hz
- 4: 44100 Hz
- 5: 32000 Hz
- 6: 24000 Hz
- 7: 22050 Hz
- 8: 16000 Hz
- 9: 12000 Hz
- 10: 11025 Hz
- 11: 8000 Hz
- 12: 7350 Hz
- 13: Reserved
- 14: Reserved
- 15: frequency is written explictly


channel_configuration: 表示声道数 
- 0: Defined in AOT Specifc Config
- 1: 1 channel: front-center
- 2: 2 channels: front-left, front-right
- 3: 3 channels: front-center, front-left, front-right
- 4: 4 channels: front-center, front-left, front-right, back-center
- 5: 5 channels: front-center, front-left, front-right, back-left, back-right
- 6: 6 channels: front-center, front-left, front-right, back-left, back-right, LFE-channel
- 7: 8 channels: front-center, front-left, front-right, side-left, side-right, back-left, back-right, LFE-channel
- 8-15: Reserved

![](http://my.csdn.net/uploads/201203/31/1333171714_8159.jpg)

frame_length : 一个ADTS帧的长度包括ADTS头和AAC原始流.

adts_buffer_fullness：0x7FF 说明是码率可变的码流

## 3.将AAC打包成ADTS格式

如果是通过[嵌入式](http://lib.csdn.net/base/embeddeddevelopment)高清解码芯片做产品的话，一般情况的解码工作都是由硬件来完成的。所以大部分的工作是把AAC原始流打包成ADTS的格式，然后丢给硬件就行了。

通过对ADTS格式的了解，很容易就能把AAC打包成ADTS。我们只需得到封装格式里面关于音频采样率、声道数、元数据长度、aac格式类型等信息。然后在每个AAC原始流前面加上个ADTS头就OK了。

贴上ffmpeg中添加ADTS头的代码，就可以很清晰的了解ADTS头的结构：



[html] [view plain](http://blog.csdn.net/tx3344/article/details/7414543#)[copy](http://blog.csdn.net/tx3344/article/details/7414543#)

- int ff_adts_write_frame_header(ADTSContext *ctx,  
-                                uint8_t *buf, int size, int pce_size)  
- {  
-     PutBitContext pb;  
- 
-     init_put_bits(&pb, buf, ADTS_HEADER_SIZE);  
- 
-     /* adts_fixed_header */  
-     put_bits(&pb, 12, 0xfff);   /* syncword */  
-     put_bits(&pb, 1, 0);        /* ID */  
-     put_bits(&pb, 2, 0);        /* layer */  
-     put_bits(&pb, 1, 1);        /* protection_absent */  
-     put_bits(&pb, 2, ctx->objecttype); /* profile_objecttype */  
-     put_bits(&pb, 4, ctx->sample_rate_index);  
-     put_bits(&pb, 1, 0);        /* private_bit */  
-     put_bits(&pb, 3, ctx->channel_conf); /* channel_configuration */  
-     put_bits(&pb, 1, 0);        /* original_copy */  
-     put_bits(&pb, 1, 0);        /* home */  
- 
-     /* adts_variable_header */  
-     put_bits(&pb, 1, 0);        /* copyright_identification_bit */  
-     put_bits(&pb, 1, 0);        /* copyright_identification_start */  
-     put_bits(&pb, 13, ADTS_HEADER_SIZE + size + pce_size); /* aac_frame_length */  
-     put_bits(&pb, 11, 0x7ff);   /* adts_buffer_fullness */  
-     put_bits(&pb, 2, 0);        /* number_of_raw_data_blocks_in_frame */  
- 
-     flush_put_bits(&pb);  
- 
-     return 0;  
- }  


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



二. ADIF：

Audio Data Interchange Format 音频数据交换格式。这种格式的特征是可以确定的找到这个音频数据的开始，不需进行在音频数据流中间开始的解码，即它的解码必须在明确定义的开始处进行。故这种格式常用在磁盘文件中。

AAC的ADIF格式见下图：

![](http://hi.csdn.net/attachment/201009/10/0_1284105267YFfk.gif)



三. faad解码aac

**iRet = pcmRender.init(2, 44100, 16, NULL);**

**static unsigned char frame[FRAME_MAX_LEN];**



**unsigned long samplerate;**

**unsigned char channels;**

**NeAACDecHandle decoder = 0;**



**size_t data_size = 0;**

**size_t size = 0;**



**NeAACDecFrameInfo frame_info;**

**unsigned char* input_data = buffer;**

**unsigned char* pcm_data = NULL;**

**static int iFlag = 0;**

**int iRead = 0;**



**while (m_iThreadFlag && (data_size = ReadData(NULL, buffer+iRead, BUFFER_MAX_LEN-iRead)))**

**{**

**#if 1**

**data_size += iRead;**

**if (0 == iFlag)**

**{**

**if(get_one_ADTS_frame(buffer, data_size, frame, &size, &iRead) < 0)**

**{**

**continue ;**

**}**



**decoder = NeAACDecOpen();    **

**//initialize decoder**

**NeAACDecInit(decoder, frame, size, &samplerate, &channels);**

**printf("samplerate %d, channels %d\n", samplerate, channels);**

**iFlag = 1;**



**}**

**input_data = buffer;**

**while(m_iThreadFlag && get_one_ADTS_frame(input_data, data_size, frame, &size, &iRead) == 0)**

**{**

**//decode ADTS frame**

**pcm_data = (unsigned char*)NeAACDecDecode(decoder, &frame_info, frame, size); **



**if(frame_info.error > 0)**

**{**

**printf("%s\n",NeAACDecGetErrorMessage(frame_info.error));            **



**}**

**else if(pcm_data && frame_info.samples > 0)**

**{**

**static FILE *fp1 = NULL;**

**if (NULL == fp1)**

**{**

**fp1 = fopen("F:\\6.pcm", "wb");**

**}**

**if (fp1)**

**{**

**fwrite(pcm_data, 1, frame_info.samples * frame_info.channels,fp1);**

**fflush(fp1);**

**}**

**Player((char*)pcm_data, frame_info.samples * frame_info.channels);**

**}        **

**data_size -= size;**

**input_data += size;**

**} **

**#endif**

**}**

**NeAACDecClose(decoder);**











