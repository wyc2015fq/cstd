# MPEG2-TS音视频同步原理 - 建建的博客 - CSDN博客
2018年04月23日 16:27:31[纪建](https://me.csdn.net/u013898698)阅读数：104
一、引言
MPEG2系统用于视音频同步以及系统时钟恢复的时间标签分别在ES，PES和TS这3个层次中。
在TS 层, TS头信息包含了节目时钟参考PCR(Program Clock Reference),
               用于恢复出与编码端一致的系统时序时钟STC(System Time Clock)。     
在PES层, 在PES头信息里包含有表示时间戳PTS(Presentation Time Stamp)和
               解码时间戳DTS(Decoding Time Stamp);
 在ES 层, 与同步有关的主要是视频缓冲验证VBV(Video Buffer Verifier),
              用以防止解码器的缓冲器出现上溢或者下溢;
标准规定在原始音频和视频流中,
  PTS的间隔不能超过0.7s，
  出现在TS包头的PCR间隔不能超过0.1s。
![](http://blog.chinaunix.net/attachment/201301/20/26000296_1358690288IZ0P.jpg)
图1 从ES到PES的示意图
MPEG-2对视频的压缩产生I帧、P帧、B帧.
将上图所示的帧顺序 "I1-P4-B2-B3-P7-B5-B6" 表示的ES帧,
通过打包并在每个帧中插入PTS/DTS标志，组成PES.
在插入PTS/DTS标志时，
对于B帧,        由于在B帧PTS和DTS是相等的，所以无须在B帧插入DTS(参见图1).
对于I帧和P帧, 由于经过复用后, 数据包的顺序会发生变化,
                   显示前一定要存储于视频解码器的排序缓存器中，经过从新排序后再显示，
                   所以一定要同时插入PTS和DTS作为从新排序的依据.
二、同步机制
编码器
系统时钟STC:
  编码器中有一个系统时钟(其频率是27MHz),
  此时钟用来产生指示音视频的正确显示和解码的时间戳,
  同时可用来指示在采样过程中系统时钟本身的瞬时值。
PCR(Program Clock Reference):
  指示系统时钟本身的瞬时值的时间标签称为节目参考时钟标签(PCR)。
  PCR的插入必须在PCR字段的最后离开复用器的那一时刻,
  同时把27MHz系统时钟的采样瞬时值作为PCR字段插入到相应的PCR域。
  它是放在TS包头的自适应区中传送.
  27MHz的系统时钟STC经波形整理后分成两路:
    PCR_ext (9bits ),   由27MHz脉冲直接触发计数器生成扩展域.
    PCR_base(33bits), 经300分频器分频成90kHz脉冲送入一个33位计数器生成90kHz基值,
                              用于和PTS/DTS比较，产生解码和显示所需要的同步信号.
  这两部分被置入PCR域，共同组成42位的PCR.
Table 2-2 Transport packet of the Recommendation|International Standard
**[cpp]**[view plain](https://blog.csdn.net/charleslei/article/details/50575834#)[copy](https://blog.csdn.net/charleslei/article/details/50575834#)
- ==============================================================================  
-      syntax                                  No.of bits       Mnemonic  
- ==============================================================================  
- transport_packet(){  
-   sync_byte  
-     ...  
-   adaptation_field_control                     2               bslbf  
-   continuity_counter                           4               uimsbf  
- if(adaptation_field_control == '10' ||  
-      adaptation_field_control == '11'){  
-     adaptation_field()  
-   }  
-   ...  
- }  
- ==============================================================================  
Table 2-6 Transport Stream adaptation field
**[cpp]**[view plain](https://blog.csdn.net/charleslei/article/details/50575834#)[copy](https://blog.csdn.net/charleslei/article/details/50575834#)
- ==============================================================================  
-      syntax                                  No.of bits       Mnemonic  
- ==============================================================================  
- adaptation_field(){  
-   adaptation_field_length                      8               uimsbf  
- if(adaptation_field_length>0){  
-     ...  
-     PCR_flag                                   1               bslbf  
-     ...  
- if(PCR_flag == '1'){  
-       program_clock_reference_base             33              uimsbf  
-       Reserved                                 6               bslbf  
-       program_clock_reference_extension        9               uimsbf  
-       ...  
-     }  
-   }  
- }  
- ==============================================================================  
输入到T-STD解码器的第i个字节的PCR值:
  PCR(i) = PCR_base(i)*300 + PCR_ext(i) 
i: 包含program_clock_reference_base域的最后一个比特的字节号.
  PCR_base(i) = ((system_clock_frequency * t(i)) / 300) % 2^33
  PCR_ext(i)   = ((system_clock_frequency * t(i)) / 1   ) % 300
t(i): 字节i的编码时间.
例如:
  时间"03:02:29.012"的PCR计算如下:
  03:02:29.012 = ((3 * 60) + 2) * 60 + 29.012 = 10949.012s
  PCR_base = ((27 000 000 * 10949.012) / 300) % 2^33 = 98 541 080
  PCR_ext   = ((27 000 000 * 10949.012) / 1  ) % 300  = 0 
  PCR = 98 541 080 * 300 + 0 = 295 623 324 000
PCR-base的作用:
  a. 与PTS和DTS作比较, 当二者相同时, 相应的单元被显示或者解码.
  b. 在解码器切换节目时,提供对解码器PCR计数器的初始值,
     以让该PCR值与PTS、DTS最大可能地达到相同的时间起点.
PCR-ext的作用:
  通过解码器端的锁相环路修正解码器的系统时钟, 使其达到和编码器一致的27MHz.
PTS(Presentation Time Stamp):
  指示音视频显示时间的时间戳称为显示时间戳(PTS);
  PTS域为33bits, 是对系统时钟的300分频的时钟的计数值.
  它被编码成为3个独立的字段:
     PTS[32..30][29..15][14..0]
  表示此分组中第一个访问单元在系统目标解码器中的预定显示时间.
PTS值为:
  PTS(k) = ((system_clock_frequency * TPn(k)) / 300) % 2^33
TPn(k): 表示单元Pn(k)的表示时间.
DTS(Decoding Time Stamp):
  指示音视频的解码时间戳称为解码时间戳(DTS),
  DTS域为33bits,编码成为3个独立的字段:
     DTS[32..30][29..15][14..0]
  表示此分组中第一个访问单元在系统目标解码器中的预定解码时间.
DTS值为:
  DTS(j) = ((system_clock_frequency * TDn(j)) / 300) % 2^33
TDn(j): 第n个ES流的第j个存取单元An(j)的解码时间.
  DTS就视频来说,因为视频编码的时候用到了双向预测,
  一个图像单元被解出,并非马上就被显示,可能在存储器中留一段时间,作为其余图像单元的解码参考,
  在被参考完毕后,才被显示.
音频PTS:
  针对音频和视频的同步显示，MPEG提出了一个音频PTS.
  由于声音没有用到双向预测，它的解码次序就是它的显示次序，故它只有PTS.
Table 2-21 PES packet
**[cpp]**[view plain](https://blog.csdn.net/charleslei/article/details/50575834#)[copy](https://blog.csdn.net/charleslei/article/details/50575834#)
- ==============================================================================  
-      syntax                                  No.of bits       Mnemonic  
- ==============================================================================  
- PES_packet(){  
-   packet_start_code_prefix                       24             bslbf  
-   stream_id                                      8              uimsbf  
-   PES_packet_length                              16             uimsbf  
- if(stream_id != program_stream_map  
-   && stream_id != padding_stream  
-   && stream_id != private_stream_2  
-   && stream_id != ECM  
-   && stream_id != EMM  
-   && stream_id != program_stream_directory  
-   && stream_id != DSMCC_stream  
-   && stream_id != ITU-T REc.H.222.1 type E stream){  
- '10'                                         2              bslbf  
-     ...  
-     PTS_DTS_flags                                2              bslbf  
-     ...  
- if(PTS_DTS_flags == '10'){  
- '0010'                                     4              bslbf  
-       PTS[32..30]                                3              bslbf  
-       marker_bit                                 1              bslbf  
-       PTS[29..15]                                15             bslbf  
-       marker_bit                                 1              bslbf  
-       PTS[14..0]                                 15             bslbf  
-       marker_bit                                 1              bslbf  
-     }  
- if(PTS_DTS_flag == '11'){  
- '0011'
-       PTS[32..30]                                3              bslbf  
-       marker_bit                                 1              bslbf  
-       PTS[29..15]                                15             bslbf  
-       marker_bit                                 1              bslbf  
-       PTS[14..0]                                 15             bslbf  
-       marker_bit                                 1              bslbf  
- '0001'                                     4              bslbf  
-       DTS[32..30]                                3              bslbf  
-       marker_bit                                 1              bslbf  
-       DTS[29..15]                                15             bslbf  
-       marker_bit                                 1              bslbf  
-       DTS[14..0]                                 15             bslbf  
-       marker_bit                                 1              bslbf  
-     }  
-     ...  
-   }  
-   ...  
- }  
- ==============================================================================  
VBV_delay:
  视频流延时值,
  在解码时利用视频流缓冲区把视频流缓存到相应的vbv_delay时间后,
  再启动解码器解码、显示、实现音视频的同步.
  VBV_delay存在于视频ES的头部，长度为16bit.
解码器
首先,  解析PCR, 重建和编码器同步的27MHz系统时钟, 恢复27MHz系统时钟后;
再,     通过VBV_delay(视频流延时值)的数值来确定解码的开始;
之后,  利用PES流中解码时间戳(DTS)和显示时间戳(PTS)来确定解码和显示的次序.
         用PCR来对系统时钟进行修正.
解码器同步算法总结如下:
(1).  解码器从输入码流的包头中解出时间信息PCR送入到系统时间时钟恢复电路;
      系统时间时钟恢复电路在接收到每一个新的PCR时，进行本地系统时间时钟恢复和锁相。
(2).  解复用器后，从PES包头中解出显示时间标签PTS和解码时间标签DTS，并送入到基本流解码器中。
(3).  基本流解码器在接收到新的PTS/DTS后，存入对应的FIFO(先进先处存储器)中进行管理；
      对于没有PTS/DTS的显示单元，需要对其时间标签进行插值，并送入到FIFO中管理。
(4).  每一显示单元开始解码前，用其对应的DTS与STC进行比较，当STC与DTS相等时开始解码；
(5).  每一显示单元开始显示前，用其对应的PTS与STC进行比较，当STC与PTS相等时开始显示。
三、失同步处理
27 MHz系统时钟经过300分频后，得到本地的33 bits PCR_Base, 该时钟与寄存器中当前图像的PTS/DTS进行比较,
系统软件根据比较结果做出相应的处理:
(1).  若当前的PTS/DTS比PCR计数器的值小于半帧以上，即PTS_Base≤-ΔPTS/2,
      此时说明系统解码过慢，解码器处于失步状态，应根据该帧的结构做出相应的同步调整；
(2).  若当前的PTS/DTS比PCR计数器的值在半帧时间以内，
      我们认为此时系统解码正常，立即显示/解码当前帧；
(3).  若当前的PTS/DTS大于PCR计数器的值，则此时解码器稍快，
      在这种情况下，只需等到PCR与PTS/DTS相等时，就可显示/解码。
