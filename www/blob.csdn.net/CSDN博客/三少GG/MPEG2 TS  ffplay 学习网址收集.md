# MPEG2 TS || ffplay 学习网址收集 - 三少GG - CSDN博客
2011年12月26日 17:09:18[三少GG](https://me.csdn.net/scut1135)阅读数：2465标签：[stream																[header																[url																[file																[filter																[protocols](https://so.csdn.net/so/search/s.do?q=protocols&t=blog)](https://so.csdn.net/so/search/s.do?q=filter&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=url&t=blog)](https://so.csdn.net/so/search/s.do?q=header&t=blog)](https://so.csdn.net/so/search/s.do?q=stream&t=blog)
个人分类：[Android 多媒体](https://blog.csdn.net/scut1135/article/category/790993)
1.**[MPEG2 TS小结](http://blog.csdn.net/baymoon/article/details/1516202)**
2.**[ffmpeg for MPEG2 TS](http://blog.csdn.net/hjksfw900/article/details/3784821)**
系列好文 [http://blog.csdn.net/hjksfw900/article/month/2009/01](http://blog.csdn.net/hjksfw900/article/month/2009/01)
3.   **[FFMpeg对MPEG2 TS流解码的流程分析](http://keyewan.blog.163.com/blog/static/189827233201161641648907/)**
4.[ffmpeg分析系列之七(打开输入的流)](http://blog.csdn.net/menuconfig/article/details/6616283)
5.[**深入浅出FFMPEG**](http://3xin2yi.info.sixxs.org/wwwroot/tech/doku.php/tech:multimedia:ffmpeg)
# 6.[ffmpeg/ffplay vc6 源码剖析](http://www.cnblogs.com/mcodec/articles/1933754.html)
系列好博客：  [最新 ffmpeg-0.8 移植到 windows 开源代码](http://www.cnblogs.com/mcodec/articles/2114674.html)
7.    配置cavs相关：
# [在 Ubuntu 下使用 Android NDK r4b 编译 FFmpeg 0.6.3](http://www.linuxidc.com/Linux/2011-09/42691.htm)  估计我使用的也是0.6.3版本！
AVCODEC_C_FILES-$(CONFIG_CAVS_DECODER) += cavs.c cavsdec.c cavsdsp.c \
  mpeg12data.c mpegvideo.c
AVCODEC_C_FILES-$(CONFIG_CAVSVIDEO_PARSER)
 += cavs_parser.c
AVFORMAT_C_FILES-$(CONFIG_CAVSVIDEO_DEMUXER)
 += raw.c
**[在 Mac OS X 下使用 Android NDK r5b 编译 FFmpeg 0.8](http://www.linuxidc.com/Linux/2011-09/42692.htm)**[](http://www.linuxidc.com/Linux/2011-09/42692.htm)
 AVCODEC_C_FILES-$(CONFIG_CAVS_DECODER) += cavs.c cavsdec.c cavsdsp.c \
  mpeg12data.c mpegvideo.c
AVCODEC_C_FILES-$(CONFIG_CAVSVIDEO_PARSER) += cavs_parser.c
 AVFORMAT_C_FILES-$(CONFIG_CAVSVIDEO_DEMUXER)
 += cavsvideodec.c rawdec.c
830: AVFORMAT_C_FILES-$(CONFIG_CAVSVIDEO_MUXER) += rawenc.c
8.  [**http://blog.csdn.net/naonaodog/article/details/6370006**](http://blog.csdn.net/naonaodog/article/details/6370006)
Ffmpeg的mpeg-2视频解码（mpeg_decode_frame）过程如下：
1、读取前4个字节的头信息，一般为00 00 01 xx，其中xx 表示：
00    —— picture_start_code //表示一帧图像的开始
01~AF —— slice_start_code//表示一个slice的开始
B0、B1—— reserved
B2    —— user_data_start_code
B3    —— sequence_header_code//表示一个sequence的开始
B4    —— sequence_error_code
B5    —— extention_start_code
B6    —— reserved
B7    —— sequence_end_code//表示一个sequence的结束
B8    —— group_start_code
B9~FF —— system_start_code
2、判断以上头信息，如果xx= B7，表示前一个sequence结束，如果解码器中的next_picture_ptr还保存了一帧图像时，则输出该帧，且结束该次解码过程。
3、如果xx！= B7时，判断当前解码器是否支持内部组帧操作，如果支持，则组帧开始。组帧时用到两个函数：
²  int next = ff_mpeg1_find_frame_end(&s2->parse_context, buf, buf_size, NULL, avctx->parserRtStart, avctx);
²  ff_combine_frame(&s2->parse_context, next, (const uint8_t **)&buf, &buf_size)
以下为这两个函数的说明：
函数ff_mpeg1_find_frame_end：
输入参数：
码流解析上下文指针parse_context
传入的码流指针buf
传入的码流大小buf_size
音视频解码的统一上下文指针
返回值：
如果找到头的位置，则返回头位置+4（即00 00 01 xx之后的字节）；否则，返回错误信息
函数功能：
查找一个帧（xx==00）头。
说明：
根据状态机所处的不同状态进行转换。如果没有找到下一个帧开始符，则只将当前的数据拷贝入解析器的buffer中；如果找到了下一个帧的开始符，则返回；
函数ff_combine_frame：
输入参数：
码流解析上下文指针parse_context
由函数ff_mpeg1_find_frame_end返回的下一个帧头的起始位置信息
输出参数：
组帧之后的buf
组帧之后的数据大小
返回值：
0 —— 组帧完成；
-1—— 没有得到组完的一帧数据；
4、主体解码过程（decode_chunks）（状态机的转换）
4.1  寻找起始码，根据起始码的不同，进入不同的解码流程：
SEQ_START_CODE：
表示一个序列的开始，进入mpeg1_decode_sequence过程。在改过程中，可以获取视频图像的大小、帧率索引、图像宽高比、比特率信息等，对解码用到的参数进行进一步赋值等。
PICTURE_START_CODE：
首先进行mpeg_decode_postinit，即进行该帧解码前的初始化工作；
之后进行mpeg1_decode_picture，即解码当前帧信息，获取视频的图像类型(注：图像类型的获取方法为：去掉00 00 01 00之后的第一、二个字节，取第11、12、13位，该三位即为图像类型信息，1——I帧，2——P帧，3——B帧)、vbv_delay、是否关键帧等信息；
EXT_START_CODE：
解码一些扩展信息，如：
mpeg_decode_sequence_extension
mpeg_decode_sequence_display_extension
mpeg_decode_quant_matrix_extension
mpeg_decode_picture_display_extension
mpeg_decode_picture_coding_extension等
USER_START_CODE：
    用户数据信息解码，mpeg_decode_user_data。
GOP_START_CODE：
图像组信息解码，mpeg_decode_gop。
SLICE_MIN_START_CODE：
Slice信息解码，主函数为mpeg_decode_slice，即对每一个slice进行解码操作，之后会分解为一个宏块，再到一个子块。
5、当解码完一帧信息后，需要根据当前解码的图像类型判断是否需要输出
如果当前解码的视频序列中没有B帧，则每编完一帧即可进行输出；
如果当前解码的视频序列中有B帧，则需要进行判断：
如果当前解码完成的帧为I或P帧，并且该帧之前还有一个I或P帧没有输出，则输出其前一个I或P帧（若当前帧为第一帧则不用输出）；如果当前解码完成的帧为B帧，则输出该帧。参考如下：
I B B P B B P B B P B B P……（显示顺序）
I P B B P B B P B B P B B……（编码顺序）
同样，
I P B B P B B P B B P B B……（解码顺序）
I B B P B B P B B P B B P……（还原显示顺序）
[10 .  Android开源项目源码下载（不断更新中）](http://www.cnblogs.com/salam/archive/2010/10/26/1861779.html)
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
### [MPEG2 TS小结](http://blog.csdn.net/baymoon/article/details/1516202)
2007-02-27 18:16 3414人阅读 [评论](http://blog.csdn.net/baymoon/article/details/1516202#comments)(1) 收藏[举报](http://blog.csdn.net/baymoon/article/details/1516202#report)
应该说真正了解TS，还是看了朋友推荐的《数字电视业务信息及其编码》一书之后，MPEG2 TS和数字电视是紧密不可分割的，值得总结一下其中的一些关系。
ISO/IEC－13818－1：系统部分；ISO/IEC－13818－2：视频；ISO/IEC－13818－3：音频；ISO/IEC－ 13818－4：一致性测试；ISO/IEC－13818－5：软件部分；ISO/IEC－13818－6：数字存储媒体命令与控制；ISO/IEC－ 13818－7：高级音频编码；ISO/IEC－13818－8：系统解码实时接口；
MPEG2系统任务包括：1. 规定以包传输数据的协议；2. 规定收发两端数据流同步的协议；3. 提供多个数据流的复用和解复用协议；3. 提供数据流加密的协议。以包形式存储和传送数据流是MPEG2系统之要点。
ES是直接从编码器出来的数据流，可以是编码过的视频数据流，音频数据流，或其他编码数据流的统称。ES流经过PES打包器之后，被转换成PES包。PES包由包头和payload组成，具体格式摘录如下：
![](http://blog.lmtw.com/UploadFiles/2006-9/927993844.jpg)
可以看到PTS/DTS是打在PES包里面的，这两个parameters是解决视音频同步显示，防止解码器输入缓存上溢或下溢的关键。PTS表示 显示单元出现在系统目标解码器(STD: system target decoder)的时间，DTS表示将存取单元全部字节从STD的ES解码缓存器移走的时刻。每个I、P、B帧的包头都有一个PTS和DTS，但PTS与DTS对B帧都是一样的，无须标出B帧的DTS。对I帧和P帧，显示前一定要存储于视频解码器的重新排序缓存器中，经过延迟（重新排序）后再显示，一定要分别标明PTS和DTS。
上节介绍过，ES首先需打包成PES流包，然后PES根据需要打包成PS或TS包进行存储或传输。其每路ES只包含一路信源的编码数据流，所以每路PES也只包含相对应信源的数据流。
对PS流而言，每个PES包头含有PTS和DTS，流识别码，用于区别不同性质ES。然后通过PS复用器将PES包复用成PS包。实际上是将PES 包分解为更细小的PS包。在解码的时候，解复用器将PS分解成一个个PES包，拆包器然后将PES包拆成视频和音频的ES，最后输入至各自解码器进行解 码。一个问题是：各个ES在解码时，如何保证视音频的同步呢？除了PTS和DTS的配合工作外，还有一个重要的参数是SCR(system clock reference)。在编码的时候，PTS，DTS和SCR都是由STC(system
 time clock)生成的，在解码时，STC会再生，并通过锁相环路（PLL－phase lock loop），用本地SCR相位与输入的瞬时SCR相位锁相比较，以确定解码过程是否同步，若不同步，则用这个瞬时SCR调整27MHz的本地时钟频率。最 后，PTS，DTS和SCR一起配合，解决视音频同步播放的问题。PS格式摘录如下：
![](http://blog.lmtw.com/UploadFiles/2006-9/928142321.jpg)
PS包的长度比较长且可变，主要用于无误码环境里，因为越长的话，同步越困难，且在丢包的情况下，重组也越困难。所以，PS适合于节目信息的编辑和本地内容应用的application。
TS流也是由一个或多个PES组合而来的，他们可以具有相同的时间基准，也可以不同。其基本的复用思想是，对具有相同时间基准的多个PES现进行节目复用，然后再对相互有独立时间基准的各个PS进行传输复用，最终产生出TS。
TS包由包头和包数据2部分组成，其中包头还可以包括扩展的自适用区。包头长度占4bytes，自使用区和包数据共占184bytes，整个TS包长度相当于4个ATM包长。TS包的包头由如下图摘录所示的同步字节、传输误码指示符、有效载荷单元起始指示符、传输优先、包识别（PID-Packet Identification）、传输加扰控制、自适应区控制和连续计数器8个部分组成。
![](http://blog.lmtw.com/UploadFiles/2006-10/107720957.jpg)
其中，可用同步字节位串的自动相关特性，检测数据流中的包限制，建立包同步；传输误码指示符，是指有不能消除误码时，采用误码校正解码器可表示1bit 的误码，但无法校正；有效载荷单元起始指示符，表示该数据包是否存在确定的起始信息；传输优先，是给TS包分配优先权；PID值是由用户确定的，解码器根据PID将TS上从不同ES来的TS包区别出来，以重建原来的ES；传输加扰控制，可指示数据包内容是否加扰，但包头和自适应区永远不加扰；自适应区控制，用2
 bit表示有否自适应区，即（01）表示有有用信息无自适应区，（10）表示无有用信息有自适应区，（11）表示有有用信息有自适应区，（00）无定义；连续计数器可对PID包传送顺序计数，据计数器读数，接收端可判断是否有包丢失及包传送顺序错误。显然，包头对TS包具有同步、识别、检错及加密功能。
    TS包自适应区由自适应区长、各种标志指示符、与插入标志有关的信息和填充数据4部分组成。其中标志部分由间断指示符、随机存取指示符、ES优化指示符、PCR标志、接点标志、传输专用数据标志、原始PCR标志、自适应区扩展标志8个部分组成。重要的是标志部分的PCR字段，可给编解码器的27MHz时钟提供同步资料，进行同步。其过程是，通过PLL，用解码时本地用PCR相位与输入的瞬时PCR相位锁相比较，确定解码过程是否同步，若不同步，则用这个瞬时PCR调整时钟频率。因为，数字图像采用了复杂而不同的压缩编码算法，造成每幅图像的数据各不相同，使直接从压缩编码图像数据的开始部分获取时钟信息成为不可能。为此，选择了某些（而非全部）TS包的自适应区来传送定时信息。于是，被选中的TS包的自适应区，可用于测定包信息的控制bit和重要的控制信息。自适应区无须伴随每个包都发送，发送多少主要由选中的TS包的传输专用时标参数决定。标志中的随机存取指示符和接点标志，在节目变动时，为随机进入I帧压缩的数据流提供随机进入点，也为插入当地节目提供方便。自适应区中的填充数据是由于PES包长不可能正好转为TS包的整数倍，最后的TS包保留一小部分有用容量，通过填充字节加以填补，这样可以防止缓存器下溢，保持总码率恒定不变。
前面3节总结了MPEG2 TS的基本格式，其中包括PES，PS和TS，以及相关字段的介绍。那么作为一种传输流，TS将内容进行打包/复用，让其媒体内容变成TS传输，并最终在 解码端解码。简单来看，TS是一个传输层的协议栈，它可以承载各种内容的传输，比如MPEG，WMV，H264，甚至是IP，那么其中的传输规范是如何定 义的呢？这个即是PSI（节目特定信息）要做的事情。
PSI由四张表构成：PAT，PMT，CAT和NIT，这四张表分别描述了一个TS所 包括的所有ES流的传输结构。首先的一个概念是，TS是以包形式传播，在编解码端都需要以一定的包ID来标识TS流里承载的内容，比如，PAT表会存在于 一个或多个TS包里，所以要用一个特别的包ID来表示，另外，不同的ES流也需要不同的包ID来标识。我们有了PAT和PMT这两种表，解码器就可以根据 PID，将TS上从不同ES来的TS包区分出来进行解码。
TS的解码分两步进行，其一，是从PID为0 的TS包里，解析出PAT表，然后从PAT表里找到各个节目源的PID，一般此类节目源都由若干个ES流组成，并描述在PMT表里面，然后通过节目源的 PID，就可以在PMT表里检索到各个ES的PID。其二，解码器根据PMT表里的ES流的PID，将TS流上的包进行区分，并按不同的ES流进行解码。 所以，TS是经过节目复用和传输复用两层完成的，即在节目复用时，加入了PMT，在传输复用时，加入了PAT。同样在节目解复用时，可以得到PMT，在传
 输解复用时，可以得到PAT。下图很好地概述了其思想。
![](http://blog.lmtw.com/UploadFiles/2006-10/1015748066.jpg)
TS是支持多路复用的，所以它可用来传输经复用后的多层节目。在复用过程中，要注意的是，解码过程中所需要面对的时间参考和同步问题，因为解复用是需要各种信息同步进行的，所以在复用过程中，就需要插入相关的时间信息：PTS，DTS，PCR。
在TS形成过程中，PTS和DTS是在ES打包成PES时，根据STC的参考，将其时钟信息注入PES包中的，而之后在PES切成TS时，再将 PID和PCR畔⒆⑷肫浒械摹Ｔ诙嘟谀扛从檬保扛鼋谀縏S的PCR将会被提取出来，再进行分析，然后再根据统一的STC参考，将新的PCR生成并注入 到TS中去，最后，因为原来PAT表信息不在适用，所以新的PAT表需要再生成，并附加到新的TS流中去。经过这多层的复用之后，新的TS流即可以进入调 制，传输阶段。过程可参见下图：
![](http://blog.lmtw.com/UploadFiles/2006-10/1031369848.jpg)
解码过程要面对的问题是：解复用，视音频的同步，解码缓存器无上下溢。解复用即是将TS在同一信道里不同时序进行传输的节目分离出来；视音频同步由 DTS, PTS和PCR三者协调完成，并且PCR是重建系统时间基准的绝对时标，而DTS和PTS是解码和重现时刻的相对时标；对解码缓存器无上下溢的问题，必须 借助于系统目标解码器（STD）模型来对其进行实现，基本思想如下：
- TS流进入解码器后，首先由换向器，按照一定的时序关系，将各种ES流分解出来（其中也包括PSI信息流）。
- 分解过后的ES流会进入各自的传输缓存器，通过之后，其PES流进入各自的主存储器，注意的是：PSI信息流会进入系统缓存器，最后也到达主存储器。
- 最后，解码器根据DTS信息，从各个主存储器分别提取媒体或系统信息，进行解码，并根据PTS信息，将媒体内容进行显示处理。
其过程可参见下图：
![](http://blog.lmtw.com/UploadFiles/2006-10/1031942689.jpg)
from:
http://www.chinaavs.com/htmldata/6/2006_09/MPEG2-TS%D0%A1%BD%E1742_1.html 
+++++++++++++++++++++++++++++++++++++++++++++++++++
### [ffmpeg for MPEG2 TS](http://blog.csdn.net/hjksfw900/article/details/3784821)
2009-01-15 09:23
1368人阅读[评论](http://blog.csdn.net/hjksfw900/article/details/3784821#comments)(1)收藏[举报](http://blog.csdn.net/hjksfw900/article/details/3784821#report)
如果RTSP传输的是RTP/MPEG2 TS数据，那么ffmpeg的数据流过程如下：
rtsp_read_packet----->rtp_parse_packet--->rtp_valid_packet_in_sequence-------------------------------------------->
-------------------------------->如果是MPEG2 TS------>mpegts_parse_packet
--------------------------------->else s->parse_packet
---------------------------------->else  copy buf to packet and return;
static void handle_packet(MpegTSContext *ts, const uint8_t *packet)
该函数解析MPEG2 TS PACKET
[view plain](http://blog.csdn.net/hjksfw900/article/details/3784821#)[print](http://blog.csdn.net/hjksfw900/article/details/3784821#)[?](http://blog.csdn.net/hjksfw900/article/details/3784821#)
- /* handle one TS packet */
- staticvoid handle_packet(MpegTSContext *ts, const uint8_t *packet)  
- {  
-     AVFormatContext *s = ts->stream;  
-     MpegTSFilter *tss;  
- int len, pid, cc, cc_ok, afc, is_start;  
- const uint8_t *p, *p_end;  
- 
- //根据iso13818-1种ts packet的格式描述，可以从ts packet中获取PID
-     pid = AV_RB16(packet + 1) & 0x1fff;  
- if(pid && discard_pid(ts, pid))  
- return;  
- //如果start_indicator为1表示该ts packet中是PES packet的起始数据
-     is_start = packet[1] & 0x40;  
- 
-     tss = ts->pids[pid];  
- //tss为null，表示pid对应的program的filter还没有创建，所以创建一个pes stream以及pes filter
- //一般会创建两种ts filer：PES filter，用来过滤一个节目
- //--------------------：section filer，用来过滤PSI信息
- if (ts->auto_guess && tss == NULL && is_start) {  
- //参考add_pes_stream, add_pes_stream函数返回的PESContext变量没有在这里保存起来，但是在add_pes_stream函数中将其作为PESFilter的opaque变量传给了PESFilter，而PESFilter又可以通过ts->pids[pid]得到
-         add_pes_stream(ts, pid, -1, 0);  
-         tss = ts->pids[pid];  
-     }  
- if (!tss)  
- return;  
- 
- /* continuity check (currently not used) */
-     cc = (packet[3] & 0xf);  
- //在add_pes_stream中将tss的last_cc设为-1
-     cc_ok = (tss->last_cc < 0) || ((((tss->last_cc + 1) & 0x0f) == cc));  
-     tss->last_cc = cc;  
- 
- /* skip adaptation field */
-     afc = (packet[3] >> 4) & 3;  
-     p = packet + 4;  
- if (afc == 0) /* reserved value */
- return;  
- if (afc == 2) /* adaptation field only */
- return;  
- if (afc == 3) {  
- /* skip adapation field */
-         p += p[0] + 1;---------------〉获取TS PACKET中的data byte
-     }  
- /* if past the end of packet, ignore */
-     p_end = packet + TS_PACKET_SIZE;  
- if (p >= p_end)  
- return;  
- 
-     ts->pos47= url_ftell(ts->stream->pb) % ts->raw_packet_size;  
- 
- 
- //SECTION FILTER
- if (tss->type == MPEGTS_SECTION) {  
- if (is_start) {  
- /* pointer field present */
-             len = *p++;  
- if (p + len > p_end)  
- return;  
- if (len && cc_ok) {  
- /* write remaining section bytes */
-                 write_section_data(s, tss,  
-                                    p, len, 0);  
- /* check whether filter has been closed */
- if (!ts->pids[pid])  
- return;  
-             }  
-             p += len;  
- if (p < p_end) {  
-                 write_section_data(s, tss,  
-                                    p, p_end - p, 1);  
-             }  
-         } else {  
- if (cc_ok) {  
-                 write_section_data(s, tss,  
-                                    p, p_end - p, 0);  
-             }  
-         }  
-     } else {  
- //PES filter， pes_cb为mpegts_push_data
-         tss->u.pes_filter.pes_cb(tss,  
-                                  p, p_end - p, is_start);  
-     }  
- }  
- 
/*
 handle one TS packet */
static void handle_packet(MpegTSContext *ts, const uint8_t *packet)
{
    AVFormatContext *s = ts->stream;
    MpegTSFilter *tss;
    int len, pid, cc, cc_ok, afc, is_start;
    const uint8_t *p, *p_end;
//根据iso13818-1种ts packet的格式描述，可以从ts packet中获取PID
    pid = AV_RB16(packet + 1) & 0x1fff;
    if(pid && discard_pid(ts, pid))
        return;
//如果start_indicator为1表示该ts packet中是PES packet的起始数据
    is_start = packet[1] & 0x40;
    tss = ts->pids[pid];
//tss为null，表示pid对应的program的filter还没有创建，所以创建一个pes stream以及pes filter
//一般会创建两种ts filer：PES filter，用来过滤一个节目
//--------------------：section filer，用来过滤PSI信息
    if (ts->auto_guess && tss == NULL && is_start) {
        //参考add_pes_stream, 
add_pes_stream函数返回的PESContext变量没有在这里保存起来，但是在add_pes_stream函数中将其作为
PESFilter的opaque变量传给了PESFilter，而PESFilter又可以通过ts->pids[pid]得到
        add_pes_stream(ts, pid, -1, 0);
        tss = ts->pids[pid];
    }
    if (!tss)
        return;
    /* continuity check (currently not used) */
    cc = (packet[3] & 0xf);
//在add_pes_stream中将tss的last_cc设为-1
    cc_ok = (tss->last_cc < 0) || ((((tss->last_cc + 1) & 
0x0f) == cc));
    tss->last_cc = cc;
    /* skip adaptation field */
    afc = (packet[3] >> 4) & 3;
    p = packet + 4;
    if (afc == 0) /* reserved value */
        return;
    if (afc == 2) /* adaptation field only */
        return;
    if (afc == 3) {
        /* skip adapation field */
        p += p[0] + 1;---------------〉获取TS PACKET中的data byte
    }
  /* if past the end of packet, ignore */
    p_end = packet + TS_PACKET_SIZE;
    if (p >= p_end)
        return;
    ts->pos47= url_ftell(ts->stream->pb) % 
ts->raw_packet_size;
    //SECTION FILTER
    if (tss->type == MPEGTS_SECTION) {
        if (is_start) {
            /* pointer field present */
            len = *p++;
            if (p + len > p_end)
                return;
            if (len && cc_ok) {
                /* write remaining section bytes */
                write_section_data(s, tss,
                                   p, len, 0);
                /* check whether filter has been closed */
                if (!ts->pids[pid])
                    return;
            }
            p += len;
            if (p < p_end) {
                write_section_data(s, tss,
                                   p, p_end - p, 1);
            }
        } else {
            if (cc_ok) {
                write_section_data(s, tss,
                                   p, p_end - p, 0);
            }
        }
    } else {
       //PES filter， pes_cb为mpegts_push_data
        tss->u.pes_filter.pes_cb(tss,
                                 p, p_end - p, is_start);
    }
}
            
mpegts_push_data在构造一个PES packet：
[view plain](http://blog.csdn.net/hjksfw900/article/details/3784821#)[print](http://blog.csdn.net/hjksfw900/article/details/3784821#)[?](http://blog.csdn.net/hjksfw900/article/details/3784821#)
- /* return non zero if a packet could be constructed */
- staticvoid mpegts_push_data(MpegTSFilter *filter,  
- const uint8_t *buf, int buf_size, int is_start)  
- {  
-     PESContext *pes = filter->u.pes_filter.opaque;  
-     MpegTSContext *ts = pes->ts;  
- const uint8_t *p;  
- int len, code;  
- 
- if(!ts->pkt)  
- return;  
- 
- //开始一个PES packet
- if (is_start) {  
- //pes->data_index表示pes packet的数据的位置
-         pes->state = MPEGTS_HEADER;  
-         pes->data_index = 0;  
-     }  
-     p = buf;  
- while (buf_size > 0) {  
- switch(pes->state) {  
- case MPEGTS_HEADER:  
- //PES_START_SIZE=9，表示PES PACKET HEADER的长度，参考iso13818-1
-             len = PES_START_SIZE - pes->data_index;  
- if (len > buf_size)  
-                 len = buf_size;  
- //将PES packet header部分复制到pes->header中，pes->header根据不同的pes类型，或有不同的结构
-             memcpy(pes->header + pes->data_index, p, len);  
-             pes->data_index += len;  
-             p += len;  
-             buf_size -= len;  
- if (pes->data_index == PES_START_SIZE) {  
- //获取到PES PACKET的长度
- /* we got all the PES or section header. We can now
-                    decide */
- #if 0
-                 av_hex_dump_log(pes->stream, AV_LOG_DEBUG, pes->header, pes->data_index);  
- #endif
- 
- //packet_start_code_prefix为0x000001
- if (pes->header[0] == 0x00 && pes->header[1] == 0x00 &&  
-                     pes->header[2] == 0x01) {  
- /* it must be an mpeg2 PES stream */
- //pes->header[3]为stream id
-                     code = pes->header[3] | 0x100;  
- //0xc0---0xdf-------->audio stream
- //0xe0---0xef-------->video stream
- //0xbd----->private stream1
- //0xfd----->???
- if (!((code >= 0x1c0 && code <= 0x1df) ||  
-                           (code >= 0x1e0 && code <= 0x1ef) ||  
-                           (code == 0x1bd) || (code == 0x1fd)))  
- goto skip;  
- if (!pes->st) {  
- /* allocate stream */
- //为该PES分配AVStream
-                         new_pes_av_stream(pes, code);  
-                     }  
- 
- //下面进行pes packet分析
-                     pes->state = MPEGTS_PESHEADER_FILL;  
- //对于video/audio stream等，
- //total_size指的是下面的packet length
-                     pes->total_size = AV_RB16(pes->header + 4);  
- /* NOTE: a zero total size means the PES size is
-                        unbounded */
- if (pes->total_size)  
-                         pes->total_size += 6;  
- //pes_header_size 
-                    pes->pes_header_size = pes->header[8] + 9;  
-                 } else {  
- /* otherwise, it should be a table */
- /* skip packet */
-                 skip:  
-                     pes->state = MPEGTS_SKIP;  
- continue;  
-                 }  
-             }  
- break;  
- /**********************************************/
- /* PES packing parsing */
- case MPEGTS_PESHEADER_FILL:  
- //pes_header_size指的是pes packet header中optional fields的长度， optional fields由header_length前面的一个byte指定
-             len = pes->pes_header_size - pes->data_index;  
- if (len > buf_size)  
-                 len = buf_size;  
-             memcpy(pes->header + pes->data_index, p, len);  
-             pes->data_index += len;  
-             p += len;  
-             buf_size -= len;  
- if (pes->data_index == pes->pes_header_size) {  
- const uint8_t *r;  
-                 unsigned int flags;  
- 
-                 flags = pes->header[7];  
-                 r = pes->header + 9;  
-                 pes->pts = AV_NOPTS_VALUE;  
-                 pes->dts = AV_NOPTS_VALUE;  
- //PTS_DTS_FLAG=10, ONLY PTS
- if ((flags & 0xc0) == 0x80) {  
-                     pes->pts = get_pts(r);  
-                     r += 5;  
- //PTS_DTS_FLAG=11, PTS & DTS
-                 } elseif ((flags & 0xc0) == 0xc0) {  
-                     pes->pts = get_pts(r);  
-                     r += 5;  
-                     pes->dts = get_pts(r);  
-                     r += 5;  
-                 }  
- /* we got the full header. We parse it and get the payload */
-                 pes->state = MPEGTS_PAYLOAD;  
-             }  
- break;  
- case MPEGTS_PAYLOAD:  
- //将PES packet的data 部分copy到 AVPacket中
- if (pes->total_size) {  
-                 len = pes->total_size - pes->data_index;  
- if (len > buf_size)  
-                     len = buf_size;  
-             } else {  
-                 len = buf_size;  
-             }  
- if (len > 0) {  
-                 AVPacket *pkt = ts->pkt;  
- if (pes->st && av_new_packet(pkt, len) == 0) {  
-                     memcpy(pkt->data, p, len);  
-                     pkt->stream_index = pes->st->index;  
-                     pkt->pts = pes->pts;  
-                     pkt->dts = pes->dts;  
- /* reset pts values */
-                     pes->pts = AV_NOPTS_VALUE;  
-                     pes->dts = AV_NOPTS_VALUE;  
- //stop_parse指示mpegts_parse_packet可以停止parse packet
-                     ts->stop_parse = 1;  
- return;  
-                 }  
-             }  
-             buf_size = 0;  
- break;  
- case MPEGTS_SKIP:  
-             buf_size = 0;  
- break;  
-         }  
-     }  
- }  
/* return non zero if a packet could be constructed */
static void mpegts_push_data(MpegTSFilter *filter,
                             const uint8_t *buf, int buf_size, int is_start)
{
    PESContext *pes = filter->u.pes_filter.opaque;
    MpegTSContext *ts = pes->ts;
    const uint8_t *p;
    int len, code;
    if(!ts->pkt)
        return;
   //开始一个PES packet
    if (is_start) {
      //pes->data_index表示pes packet的数据的位置
        pes->state = MPEGTS_HEADER;
        pes->data_index = 0;
    }
    p = buf;
    while (buf_size > 0) {
        switch(pes->state) {
        case MPEGTS_HEADER:
        //PES_START_SIZE=9，表示PES PACKET HEADER的长度，参考iso13818-1
            len = PES_START_SIZE - pes->data_index;
            if (len > buf_size)
                len = buf_size;
          //将PES packet header部分复制到pes->header中，pes->header根据不同的pes类型，或有不同的结构
            memcpy(pes->header + pes->data_index, p, len);
            pes->data_index += len;
            p += len;
            buf_size -= len;
            if (pes->data_index == PES_START_SIZE) {
                //获取到PES PACKET的长度
                /* we got all the PES or section header. We can now
                   decide */
#if 0
                av_hex_dump_log(pes->stream, AV_LOG_DEBUG, pes->header, pes->data_index);
#endif
        //packet_start_code_prefix为0x000001
                if (pes->header[0] == 0x00 && pes->header[1] == 0x00 &&
                    pes->header[2] == 0x01) {
                    /* it must be an mpeg2 PES stream */
                   //pes->header[3]为stream id
                    code = pes->header[3] | 0x100;
                 //0xc0---0xdf-------->audio stream
                 //0xe0---0xef-------->video stream
                 //0xbd----->private stream1
                 //0xfd----->???
                    if (!((code >= 0x1c0 && code <= 0x1df) ||
                          (code >= 0x1e0 && code <= 0x1ef) ||
                          (code == 0x1bd) || (code == 0x1fd)))
                    goto skip;
                    if (!pes->st) {
                        /* allocate stream */
                        //为该PES分配AVStream
                        new_pes_av_stream(pes, code);
                    }
                   //下面进行pes packet分析
                    pes->state = MPEGTS_PESHEADER_FILL;
                   //对于video/audio stream等，
                     //total_size指的是下面的packet length
                    pes->total_size = AV_RB16(pes->header + 4);
                    /* NOTE: a zero total size means the PES size is
                       unbounded */
                    if (pes->total_size)
                        pes->total_size += 6;
                   //pes_header_size 
                   pes->pes_header_size = pes->header[8] + 9;
                } else {
                    /* otherwise, it should be a table */
                    /* skip packet */
                skip:
                    pes->state = MPEGTS_SKIP;
                    continue;
                }
            }
            break;
     /**********************************************/
            /* PES packing parsing */
        case MPEGTS_PESHEADER_FILL:
              //pes_header_size指的是pes packet header中optional fields的长度， optional fields由header_length前面的一个byte指定
            len = pes->pes_header_size - pes->data_index;
            if (len > buf_size)
                len = buf_size;
            memcpy(pes->header + pes->data_index, p, len);
            pes->data_index += len;
            p += len;
            buf_size -= len;
            if (pes->data_index == pes->pes_header_size) {
                const uint8_t *r;
                unsigned int flags;
                flags = pes->header[7];
                r = pes->header + 9;
                pes->pts = AV_NOPTS_VALUE;
                pes->dts = AV_NOPTS_VALUE;
                //PTS_DTS_FLAG=10, ONLY PTS
                if ((flags & 0xc0) == 0x80) {
                    pes->pts = get_pts(r);
                    r += 5;
                 //PTS_DTS_FLAG=11, PTS & DTS
                } else if ((flags & 0xc0) == 0xc0) {
                    pes->pts = get_pts(r);
                    r += 5;
                    pes->dts = get_pts(r);
                    r += 5;
                }
                /* we got the full header. We parse it and get the payload */
                pes->state = MPEGTS_PAYLOAD;
            }
            break;
      case MPEGTS_PAYLOAD:
         //将PES packet的data 部分copy到 AVPacket中
            if (pes->total_size) {
                len = pes->total_size - pes->data_index;
                if (len > buf_size)
                    len = buf_size;
            } else {
                len = buf_size;
            }
            if (len > 0) {
                AVPacket *pkt = ts->pkt;
                if (pes->st && av_new_packet(pkt, len) == 0) {
                    memcpy(pkt->data, p, len);
                    pkt->stream_index = pes->st->index;
                    pkt->pts = pes->pts;
                    pkt->dts = pes->dts;
                    /* reset pts values */
                    pes->pts = AV_NOPTS_VALUE;
                    pes->dts = AV_NOPTS_VALUE;
                    //stop_parse指示mpegts_parse_packet可以停止parse packet
                    ts->stop_parse = 1;
                    return;
                }
            }
            buf_size = 0;
            break;
        case MPEGTS_SKIP:
            buf_size = 0;
            break;
        }
    }
}
由于mpeg2ts里面指定了stream id来去分video/audio/psi，对于video/audio的具体的格式还要进一步的分析。
+++++++
FFMpeg对MPEG2 TS流解码的流程分析
1.引子
gnxzzz广告都打出去了，不能没有反应.现在写东西很少了，一是年纪大了，好奇心少了
许多，；二则是这几天又犯了扁桃体炎，每天只要是快睡觉或刚起床，头晕脑涨，不过功
课还是的做的，是吧:)
2. 从简单说起
说道具体的音频或者视频格式，一上来就是理论，那是国内混资历的所谓教授的做为，对
于我们，不合适，还是用自己的方式理解这些晦涩不已的理论吧。
其实MPEG2是一族协议，至少已经成为ISO标准的就有以下几部分:
ISO/IEC－13818－1：系统部分；
ISO/IEC－13818－2：视频编码格式；
ISO/IEC－13818－3：音频编码格式；
ISO/IEC－13818－4： 一致性测试；
ISO/IEC－13818－5：软件部分；
ISO/IEC－13818－6：数字存储媒体命令与控制；
ISO/IEC－13818－7： 高级音频编码；
ISO/IEC－13818－8：系统解码实时接口；
我不是很想说实际的音视频编码格式，毕竟协议已经很清楚了，我主要想说说这些部分怎
么组合起来在实际应用中工作的。
第一部分(系统部分)很重要，是构成以MPEG2为基础的应用的基础. 很绕口，是吧，我简
单解释一下:比如DVD实际上是以系统部分定义的PS流为基础，加上版权管理等其他技术构
成的。而我们的故事主角，则是另外一种流格式，TS流，它在现阶段最大的应用是在数字
电视节目的传输与存储上，因此，你可以理解TS实际上是一种传输协议，与实际传输的负
载关系不大，只是在TS中传输了音频，视频或者其他数据。
先说一下为什么会有这两种格式的出现，PS适用于没有损耗的环境下面存储，而TS 则适用
于可能出现损耗或者错误的各种物理网络环境，比如你在公交上看到的电视，很有可能就
是基于TS的DVB-T的应用:)
我们再来看MPEG2协议中的一些概念，为理解代码做好功课:
ES(Elementary Stream):
wiki上说“An elementary stream (ES) is defined by MPEG communication protocol
is usually the output of an audio or video encoder”
恩，很简单吧，就是编码器编出的一组数据，可能是音频的，视频的，或者其他数据
说到着，其实可以对编码器的流程思考一下，无非是执行：采样，量化，编码这3个步骤
中的编码而已(有些设备可能会包含前面的采样和量化)。关于视频编码的基本理论，还是
请参考其它的资料。
PES(Packetized Elementary Stream):
wiki上说“allows an Elementary stream to be divided into packets”
其实可以理解成，把一个源源不断的数据(音频，视频或者其他)流，打断成一段一段，以
便处理.
TS(Transport Stream):
PS(Program Stream):
这两个上面已经有所提及，后面会详细分析TS,我对PS格式兴趣不大.
3. 步入正题
才进入正题，恩，看来闲话太多了:(,直接看Code.
前面说过，TS是一种传输协议，因此，对应到FFmpeg,可以认为他是一种封装格式。因此
，对应的代码应该先去libavformat里面找，很容易找到，就是mpegts.c:)
还是逐步看过来:
[libavformat/utils.c]
int av_open_input_file(AVFormatContext **ic_ptr, const char *filename,
AVInputFormat *fmt,
int buf_size,
AVFormatParameters *ap)
{
int err, probe_size;
AVProbeData probe_data, *pd = &probe_data;
ByteIOContext *pb = NULL;
pd->filename = "";
if (filename)
pd->filename = filename;
pd->buf = NULL;
pd->buf_size = 0;
#########################################################################
【1】这段代码其实是为了针对不需要Open文件的容器Format的探测，其实就是使用
AVFMT_NOFILE标记的容器格式单独处理，现在只有使用了该标记的Demuxer很少，
只有image2_demuxer，rtsp_demuxer，因此我们分析TS时候可以不考虑这部分
#########################################################################
if (!fmt) {
/* guess format if no file can be opened */
fmt = av_probe_input_format(pd, 0);
}
/* Do not open file if the format does not need it. XXX: specific
hack needed to handle RTSP/TCP */
if (!fmt || !(fmt->flags & AVFMT_NOFILE)) {
/* if no file needed do not try to open one */
#####################################################################
【2】这个函数似乎很好理解，无非是带缓冲的IO的封装，不过我们既然到此了
，不妨跟踪下去，看看别人对带缓冲的IO操作封装的实现:)
#####################################################################
if ((err=url_fopen(&pb, filename, URL_RDONLY)) < 0) {
goto fail;
}
if (buf_size > 0) {
url_setbufsize(pb, buf_size);
}
for(probe_size= PROBE_BUF_MIN; probe_size<=PROBE_BUF_MAX && !fmt; probe_size<<=1){
int score= probe_size < PROBE_BUF_MAX ? AVPROBE_SCORE_MAX/4 : 0;
/* read probe data */
pd->buf= av_realloc(pd->buf, probe_size + AVPROBE_PADDING_SIZE);
##################################################################
【3】真正将文件读入到pd的buffer的地方，实际上最终调用FILE protocol
的file_read(),将内容读入到pd的buf，具体代码如果有兴趣可以自己跟踪
##################################################################
pd->buf_size = get_buffer(pb, pd->buf, probe_size);
memset(pd->buf+pd->buf_size, 0, AVPROBE_PADDING_SIZE);
if (url_fseek(pb, 0, SEEK_SET) < 0) {
url_fclose(pb);
if (url_fopen(&pb, filename, URL_RDONLY) < 0) {
pb = NULL;
err = AVERROR(EIO);
goto fail;
}
}
##################################################################
【4】此时的pd已经有了需要分析的原始文件，只需要查找相应容器format
的Tag比较，以判断读入的究竟为什么容器格式，这里
##################################################################
/* guess file format */
fmt = av_probe_input_format2(pd, 1, &score);
}
av_freep(&pd->buf);
}
/* if still no format found, error */
if (!fmt) {
err = AVERROR_NOFMT;
goto fail;
}
/* check filename in case an image number is expected */
if (fmt->flags & AVFMT_NEEDNUMBER) {
if (!av_filename_number_test(filename)) {
err = AVERROR_NUMEXPECTED;
goto fail;
}
}
err = av_open_input_stream(ic_ptr, pb, filename, fmt, ap);
if (err)
goto fail;
return 0;
fail:
av_freep(&pd->buf);
if (pb)
url_fclose(pb);
*ic_ptr = NULL;
return err;
}
【2】带缓冲IO的封装的实现
[liavformat/aviobuf.c]
int url_fopen(ByteIOContext **s, const char *filename, int flags)
{
URLContext *h;
int err;
err = url_open(&h, filename, flags);
if (err < 0)
return err;
err = url_fdopen(s, h);
if (err < 0) {
url_close(h);
return err;
}
return 0;
}
可以看到，下面的这个函数，先查找是否是FFmpeg支持的protocol的格式，如果文件名不
符合，则默认是FILE protocol格式，很显然，这里protocol判断是以URL的方式判读的，
因此基本上所有的IO接口函数都是url_xxx的形式。
在这也可以看到，FFmpeg支持的protocol有：
/* protocols */
REGISTER_PROTOCOL (FILE, file);
REGISTER_PROTOCOL (HTTP, http);
REGISTER_PROTOCOL (PIPE, pipe);
REGISTER_PROTOCOL (RTP, rtp);
REGISTER_PROTOCOL (TCP, tcp);
REGISTER_PROTOCOL (UDP, udp);
而大部分情况下，如果你不指明类似file://xxx,http://xxx格式，它都以FILE protocol
来处理。
[liavformat/avio.c]
int url_open(URLContext **puc, const char *filename, int flags)
{
URLProtocol *up;
const char *p;
char proto_str[128], *q;
p = filename;
q = proto_str;
while (*p != '\0' && *p != ':') {
/* protocols can only contain alphabetic chars */
if (!isalpha(*p))
goto file_proto;
if ((q - proto_str) < sizeof(proto_str) - 1)
*q++ = *p;
p++;
}
/* if the protocol has length 1, we consider it is a dos drive */
if (*p == '\0' || (q - proto_str) <= 1) {
file_proto:
strcpy(proto_str, "file");
} else {
*q = '\0';
}
up = first_protocol;
while (up != NULL) {
if (!strcmp(proto_str, up->name))
#################################################################
很显然，此时已经知道up，filename，flags
#################################################################
return url_open_protocol (puc, up, filename, flags);
up = up->next;
}
*puc = NULL;
return AVERROR(ENOENT);
}
[libavformat/avio.c]
int url_open_protocol (URLContext **puc, struct URLProtocol *up,
const char *filename, int flags)
{
URLContext *uc;
int err;
##########################################################################
【a】? 为什么这样分配空间
##########################################################################
uc = av_malloc(sizeof(URLContext) + strlen(filename) + 1);
if (!uc) {
err = AVERROR(ENOMEM);
goto fail;
}
#if LIBAVFORMAT_VERSION_MAJOR >= 53
uc->av_class = &urlcontext_class;
#endif
##########################################################################
【b】? 这样的用意又是为什么
##########################################################################
uc->filename = (char *) &uc[1];
strcpy(uc->filename, filename);
uc->prot = up;
uc->flags = flags;
uc->is_streamed = 0; /* default = not streamed */
uc->max_packet_size = 0; /* default: stream file */
err = up->url_open(uc, filename, flags);
if (err < 0) {
av_free(uc);
*puc = NULL;
return err;
}
//We must be carefull here as url_seek() could be slow, for example for
//http
if( (flags & (URL_WRONLY | URL_RDWR))
|| !strcmp(up->name, "file"))
if(!uc->is_streamed && url_seek(uc, 0, SEEK_SET) < 0)
uc->is_streamed= 1;
*puc = uc;
return 0;
fail:
*puc = NULL;
return err;
}
上面这个函数不难理解，但有些地方颇值得玩味，比如，上面给出问号的地方，你明白为
什么这样Coding么:)
很显然，此时up->url_open()实际上调用的是 file_open()[libavformat/file.c]，看完
这个函数，对上面的内存分配，是否恍然大悟:)
上面只是分析了url_open()，还没有分析url_fdopen(s, h);这部分代码，也留给有好奇
心的你了:)
恩，为了追踪这个流程，走得有些远，但不是全然无用:)
终于来到了【4】，我们来看MPEG TS格式的侦测过程，这其实才是我们今天的主角
4. MPEG TS格式的探测过程
[liavformat/mpegts.c]
static int mpegts_probe(AVProbeData *p)
{
#if 1
const int size= p->buf_size;
int score, fec_score, dvhs_score;
#define CHECK_COUNT 10
if (size < (TS_FEC_PACKET_SIZE * CHECK_COUNT))
return -1;
score = analyze(p->buf, TS_PACKET_SIZE *CHECK_COUNT, TS_PACKET_SIZE, NULL);
dvhs_score = analyze(p->buf, TS_DVHS_PACKET_SIZE *CHECK_COUNT, TS_DVHS_PACKET_SIZE, NULL);
fec_score= analyze(p->buf, TS_FEC_PACKET_SIZE*CHECK_COUNT, TS_FEC_PACKET_SIZE, NULL);
// av_log(NULL, AV_LOG_DEBUG, "score: %d, dvhs_score: %d, fec_score: %d \n", score, dvhs_score, fec_score);
// we need a clear definition for the returned score otherwise things will become messy sooner or later
if (score > fec_score && score > dvhs_score && score > 6) return AVPROBE_SCORE_MAX + score - CHECK_COUNT;
else if(dvhs_score > score && dvhs_score > fec_score && dvhs_score > 6) return AVPROBE_SCORE_MAX + dvhs_score - CHECK_COUNT;
else if( fec_score > 6) return AVPROBE_SCORE_MAX + fec_score - CHECK_COUNT;
else return -1;
#else
/* only use the extension for safer guess */
if (match_ext(p->filename, "ts"))
return AVPROBE_SCORE_MAX;
else
return 0;
#endif
}
之所以会出现3种格式，主要原因是：
TS标准是188Bytes,而小日本自己又弄了一个192Bytes的DVH-S格式，第三种的 204Bytes则
是在188Bytes的基础上，加上16Bytes的FEC(前向纠错).
static int analyze(const uint8_t *buf, int size, int packet_size, int *index)
{
int stat[packet_size];
int i;
int x=0;
int best_score=0;
memset(stat, 0, packet_size*sizeof(int));
##########################################################################
由于查找的特定格式至少3个Bytes，因此，至少最后3个Bytes不用查找
##########################################################################
for(x=i=0; i<size-3; i++){
######################################################################
参看后面的协议说明
######################################################################
if(buf[i] == 0x47 && !(buf[i+1] & 0x80) && (buf[i+3] & 0x30)){
stat[x]++;
if(stat[x] > best_score){
best_score= stat[x];
if(index) *index= x;
}
}
x++;
if(x == packet_size) x= 0;
}
return best_score;
}
这个函数简单说来，是在size大小的buf中，寻找满足特定格式，长度为packet_size的
packet的个数，显然，返回的值越大越可能是相应的格式(188/192/204)
其中的这个特定格式，其实就是协议的规定格式：
Syntax
No. of bits
Mnemonic
transport_packet(){
sync_byte
8
bslbf
transport_error_indicator
1
bslbf
payload_unit_start_indicator
1
bslbf
transport_priority
1
bslbf
PID
13
uimsbf
transport_scrambling_control
2
bslbf
adaptation_field_control
2
bslbf
continuity_counter
4
uimsbf
if(adaptation_field_control=='10' || adaptation_field_control=='11'){
adaptation_field()
}
if(adaptation_field_control=='01' || adaptation_field_control=='11') {
for (i=0;i<N;i++){
data_byte
8
bslbf
}
}
}
其中的sync_byte固定为0x47,即上面的: buf[i] == 0x47
由于 transport_error_indicator为1的TS Packet实际有错误，表示携带的数据无意义，
这样的Packet显然没什么意义，因此: !(buf[i+1] & 0x80)
对于adaptation_field_control，如果为取值为 0x00,则表示为未来保留，现在不用，因
此： buf[i+3] & 0x30
这就是MPEG TS的侦测过程，很简单吧:)
后面我们分析如何从mpegts文件中获取stream的过程，待续......
参考：
1.http://en.wikipedia.org/wiki/MPEG_transport_stream
2.ISO/IEC－13818－1
