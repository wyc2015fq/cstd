# ffmpeg对mpeg2-TS解析的最详细分析 ffmpeg - zp704393004的专栏 - CSDN博客





2018年06月11日 17:03:29[原来未知](https://me.csdn.net/zp704393004)阅读数：147








﻿﻿

MPEG的系统层编码为不同的应用场景设计了两种格式: 

TS(Transport Stream) 和PS(Program Stream),

它们两者之间不具有层级关系，

在逻辑上，它们两者都是由PES(Packetized Elementary Stream)包组成的，

所以可以很方便地实现相互转换.




TS(Transport Stream): 

  是将具有一个或多个独立时间基的一个或多个节目(包括音频和视频)组成一个流,

  组成同一个节目的基本流(如一个视频流，多个音频流)的PES包有一个共用的时间基。

  TS的包长标准为188bytes.



从上面的定义可以分成三层来看TS/PS。

ES层   : 由单独的音频(如mp3)，视频流(如h.264)组成基本的ES(Elementary Stream)。

PES层  : 将基本的ES按一定的规则(如H.264以AU)进行封装，并打上时间戳，组成PES。

TS/PS层: 将PES包进行切分后再封装成188bytes大小的TS包，

         同时还将一些节目信息也封装成TS包(称为section), 两者共同组成TS层。




从上面的总结，TS/PS总体上来说，是一种封装格式，用来承载数据。

所以FFmpeg

将TS/PS的解析文件定义在libavformat/mpegts.c文件中

将音频，视频的解码定义在libavcodec/mpeg12.c文件中




下面来看FFmpeg是如何进行TS的demuxer的。

**1. MPEG2-TS的demuxer函数**


- AVInputFormat ff_mpegts_demuxer ={

- "mpegts",

-     NULL_IF_CONFIG_SMALL("MPEG-2 transport stream format"),

-     sizeof(MpegTSContext),

-     mpegts_probe,

-     mpegts_read_header,

-     mpegts_read_packet,

-     mpegts_read_close,

-     read_seek,

-     mpegts_get_pcr,

- .flags = AVFMT_SHOW_IDS|AVFMT_TS_DISCONT,

- #ifdef USE_SYNCPOINT_SEARCH                                                            

- .read_seek2 = read_seek2,

- #endif  

- };






**2. 解析流中的TS格式**


- /*

-  * 出现3种格式，主要原因是：

-  * TS标准是  188Bytes;

-  * 日本标准是192Bytes的DVH-S格式;

-  * 第三种的  204Bytes则是在188Bytes的基础上，加上16Bytes的FEC(前向纠错).

-  */

- #define TS_PACKET_SIZE      188

- #define TS_DVHS_PACKET_SIZE 192

- #define TS_FEC_PACKET_SIZE  204

- 

- #define TS_MAX_PACKET_SIZE  204

- 

- //< maximum score, half of that is used for file-extension-based detection

- #define AVPROBE_SCORE_MAX   100







- /*

-  * 函数功能:

-  *   分析流中是三种TS格式的哪一种

-  */

- static int mpegts_probe(AVProbeData *p)

- {

- #define CHECK_COUNT 10

- 

- constint size= p->buf_size;

- int score, fec_score, dvhs_score;

- int check_count= size / TS_FEC_PACKET_SIZE;

- 

- if(check_count < CHECK_COUNT)

-       return -1;

- 

-   score     = analyze(p->buf, TS_PACKET_SIZE     *check_count, TS_PACKET_SIZE     ,NULL)
-               * CHECK_COUNT / check_count;

-   dvhs_score= analyze(p->buf, TS_DVHS_PACKET_SIZE*check_count, TS_DVHS_PACKET_SIZE,NULL)
-               * CHECK_COUNT / check_count;

-   fec_score = analyze(p->buf, TS_FEC_PACKET_SIZE *check_count, TS_FEC_PACKET_SIZE ,NULL)
-               * CHECK_COUNT / check_count;

- 

- /*

- * we need a clear definition for the returned score ,

- * otherwise things will become messy sooner or later

- */

- if(score > fec_score && score > dvhs_score && score > 6)

-     return AVPROBE_SCORE_MAX + score - CHECK_COUNT;

- elseif(dvhs_score > score && dvhs_score > fec_score && dvhs_score > 6)

-     return AVPROBE_SCORE_MAX + dvhs_score  - CHECK_COUNT;

- elseif(fec_score > 6)

-     return AVPROBE_SCORE_MAX + fec_score - CHECK_COUNT;

- else

-     return -1;

- }







- /*

-  * 函数功能:

-  *   在size大小的buf中，寻找满足特定格式，长度为packet_size的

-  *   packet的个数;

-  *   显然，返回的值越大越可能是相应的格式(188/192/204)

-  */

- static int analyze(const uint8_t *buf,int size,int packet_size,int*index){

- int stat[TS_MAX_PACKET_SIZE];

- int i;

- int x=0;

- int best_score=0;

- 

-   memset(stat, 0, packet_size*sizeof(int));

- 

- for(x=i=0; i < size-3; i++)

- {

- if((buf[i]== 0x47)&&!(buf[i+1]& 0x80)&&(buf[i+3]& 0x30))

- {

-       stat[x]++;

- 

- if(stat[x]> best_score)

- {

-         best_score= stat[x];

- if(index)

- *index= x;

- }

- }

- 

-     x++;

- if(x == packet_size)

-       x= 0;

- }

- 

-   return best_score;

- }






buf[i] == 0x47  

   其中的sync_byte固定为0x47,即上面的. 

!(buf[i+1] & 0x80)  

   由于transport_error_indicator为1的TS Packet实际有错误,

   表示携带的数据无意义, 这样的Packet显然没什么意义.

buf[i+3] & 0x30 

   对于adaptation_field_control, 如果取值为0x00,则表示为未来保留，现在不用.




这就是MPEG TS的侦测过程.







**3. MPEG2-TS头解析**


- #define NB_PID_MAX 8192

- #define MAX_SECTION_SIZE 4096

- 

- /* pids */

- #define PAT_PID                 0x0000

- #define SDT_PID                 0x0011

- 

- /* table ids */

- #define PAT_TID   0x00

- #define PMT_TID   0x02

- #define SDT_TID   0x42






- /*

- * 函数功能:

- *

- */

- int mpegts_read_header(AVFormatContext *s, AVFormatParameters *ap)

- {

-   /*

-    * MpegTSContext , 是为了解码不同容器格式所使用的私有数据，

-    * 只有在相应的诸如mpegts.c文件才可以使用的.

-    * 这样，增加了这个库的模块化.

-    */

-   MpegTSContext *ts = s->priv_data;

-   AVIOContext *pb = s->pb;

-   uint8_t buf[8*1024];

- intlen;

-   int64_t pos;

- 

- /* read the first 8*1024 bytes toget packet size */

-   pos = avio_tell(pb);// 获取buf的当前位置

- len= avio_read(pb, buf, sizeof(buf));// 从pb->opaque中读取sizeof(buf)个字节到buf

- if(len!= sizeof(buf))

-     goto fail;

- 

-   /* 

-    * 获得TS包的实际长度

-    */

-   ts->raw_packet_size = get_packet_size(buf, sizeof(buf));

- if(ts->raw_packet_size <= 0)

- {

-     av_log(s, AV_LOG_WARNING,"Could not detect TS packet size, defaulting to non-FEC/DVHS\n");

-     ts->raw_packet_size = TS_PACKET_SIZE;

- }

- 

-   ts->stream = s;

-   ts->auto_guess = 0;

- 

- if(s->iformat ==&ff_mpegts_demuxer)

- {

- /* normal demux */

- /* first do a scaning toget all the services */

- if(avio_seek(pb, pos, SEEK_SET)< 0)

- {

-       av_log(s, AV_LOG_ERROR,"Unable to seek back to the start\n");

- }

- 

-     /*

-      * 挂载了两个Section类型的过滤器,

-      * 其实在TS的两种负载中，section是PES的元数据，

-      * 只有先解析了section,才能进一步解析PES数据，因此先挂上section的过滤器。

-      */

-     mpegts_open_section_filter(ts, SDT_PID, sdt_cb, ts, 1);

-     mpegts_open_section_filter(ts, PAT_PID, pat_cb, ts, 1);

- 

- /*

- */

-     handle_packets(ts, s->probesize / ts->raw_packet_size);

- 

- /*if could not find service, enable auto_guess */

-     ts->auto_guess = 1;

-     av_dlog(ts->stream,"tuning done\n");

-     s->ctx_flags |= AVFMTCTX_NOHEADER;

- }

- else

- {

- ...

- }

- 

-   avio_seek(pb, pos, SEEK_SET);

-   return 0;

- 

- fail:

-   return -1;

- }





- MpegTSFilter *mpegts_open_section_filter(MpegTSContext*   ts,

-                                          unsigned int     pid,

-                                          SectionCallback* section_cb,

-                                          void*            opaque,

- int              check_crc)

- {

-   MpegTSFilter *filter;

-   MpegTSSectionFilter *sec;

- 

-   av_dlog(ts->stream,"Filter: pid=0x%x\n", pid);

- 

- if(pid >= NB_PID_MAX || ts->pids[pid])

-     return NULL;

- 

- filter= av_mallocz(sizeof(MpegTSFilter));

- if(!filter)

-     return NULL;

- 

-   ts->pids[pid]=filter;

- filter->type    = MPEGTS_SECTION;

- filter->pid     = pid;

- filter->last_cc =-1;

-   sec             =&filter->u.section_filter;

-   sec->section_cb = section_cb;

-   sec->opaque     = opaque;

-   sec->section_buf= av_malloc(MAX_SECTION_SIZE);

-   sec->check_crc  = check_crc;

- 

- if(!sec->section_buf)

- {

-     av_free(filter);

-     return NULL;

- }

- 

-   return filter;

- }



对于这部分代码，需要分析数据结构的定义：

依次为：




    struct MpegTSContext;

               |

               V

    struct MpegTSFilter;

               |

               V

+--------------+---------------+

|                              |

V                              V

MpegTSPESFilter        MpegTSSectionFilter




就是struct MpegTSContext；中有NB_PID_MAX(8192)个TS的Filter，

而每个struct MpegTSFilter

  可能是 PES    的Filter

  或者是 Section的Filter。




为什么NB_PID_MAX 是 8192,

需要看TS的语法结构(ISO/IEC 138138-1 page 19):


- Syntax                          No. of bits         Mnemonic

- transport_packet(){

-   sync_byte                        8                 bslbf

-   transport_error_indicator        1                 bslbf

-   payload_unit_start_indicator     1                 bslbf

-   transport_priority               1                 bslbf

-   PID                              13                uimsbf

-   transport_scrambling_control     2                 bslbf

-   adaptation_field_control         2                 bslbf

-   continuity_counter               4                 uimsbf

- if(adaptation_field_control=='10'||

-       adaptation_field_control=='11')

- {

-         adaptation_field()

- }

- 

- if(adaptation_field_control=='01'||

-       adaptation_field_control=='11')

- {

- for(i=0;i<N;i++)

- {

-       data_byte                     8                bslbf

- }

- }

- }


而8192,是2^13=8192(PID)的最大数目，

为什么会有PES和Section的区分，更详细的可以参考ISO/IEC-13818-1.







挂载上了两种section过滤器，如下：

=========================================================================

PID                |Section Name           |Callback

=========================================================================

SDT_PID(0x0011)    |ServiceDescriptionTable|sdt_cb

                   |                       |

PAT_PID(0x0000)    |ProgramAssociationTable|pat_cb

=========================================================================

设计成回调函数，是为了在后面使用。




**4. MPEG2-TS的包处理**






- int handle_packets(MpegTSContext *ts,int nb_packets)

- {

-   AVFormatContext *s = ts->stream;

-   uint8_t packet[TS_PACKET_SIZE];

- int packet_num, ret;

- 

-   ts->stop_parse = 0;

-   packet_num = 0;

- 

- for(;;)

- {

-     packet_num++;

- 

- if(nb_packets != 0 && packet_num >= nb_packets ||

-         ts->stop_parse > 1)

- {

-       ret = AVERROR(EAGAIN);

-       break;

- }

- 

- if(ts->stop_parse > 0)

-       break;

- 

-     ret = read_packet(s, packet, ts->raw_packet_size);

- if(ret != 0)

-       return ret;

- 

-     ret = handle_packet(ts, packet);

- if(ret != 0)

-       return ret;

- }

- 

-   return 0;

- }






它的代码结构很简单:

handle_packets()

    |

    +->read_packet()

    |

    +->handle_packet()

        |

        +->write_section_data()



read_packet(),  很简单, 就是去找sync_byte(0x47),

handle_packet(),是真正处理数据的地方.它的代码如下:


- /* 

-  * 功能: handle one TS packet 

-  */

- int handle_packet(MpegTSContext *ts,const uint8_t *packet)

- {

-   AVFormatContext *s = ts->stream;

-   MpegTSFilter *tss;

- intlen, pid, cc, expected_cc, cc_ok, afc, is_start;

- const uint8_t *p,*p_end;

-   int64_t pos;

- 

-   /* 获取该包的PID */

-   pid = AV_RB16(packet + 1)& 0x1fff;

- if(pid && discard_pid(ts, pid))

-      return 0;

- 

-   /* 

-    * 是否是PES或者Section的开头

-    * 即syntax element: payload_unit_start_indicator 

-    */

-   is_start = packet[1]& 0x40;

-   tss = ts->pids[pid];

- 

-   /* 

-    * ts->auto_guess此时为0，因此不考虑下面的代码

-    */

- if(ts->auto_guess && tss ==NULL&& is_start)

- {

-     add_pes_stream(ts, pid,-1);

-     tss = ts->pids[pid];

- }

- if(!tss)

-     return 0;

- 

-   /* 

-    * continuity check (currently not used) 

-    * 虽然检查，但不利用检查的结果

-    */

-   cc =(packet[3]& 0xf);

-   expected_cc =(packet[3]& 0x10)?(tss->last_cc + 1)& 0x0f : tss->last_cc;

-   cc_ok =(tss->last_cc < 0)||(expected_cc == cc);

-   tss->last_cc = cc;

- 

-   /* 

-    * 解析 adaptation_field_control 语法元素

-    * =======================================================

-    * 00  | Reserved for future use by ISO/IEC

-    * 01  | No adaptation_field, payload only

-    * 10  | Adaptation_field only, no payload

-    * 11  | Adaptation_field follwed by payload

-    * =======================================================

-    */

-   afc =(packet[3]>> 4)& 3;

-   p = packet + 4;

- if(afc == 0)/* reserved value */

-     return 0;

- if(afc == 2)/* adaptation field only */

-     return 0;

- if(afc == 3)

- {

-     /* 

-      * 跳过 adapation field 

-      * p[0]对应的语法元素为: adaptation_field_length

-      */

-     p += p[0]+ 1;

- }

- 

-   /* 

-    * if past the end of packet, ignore 

-    * p已近到达TS包中的有效负载的地方

-    */

-   p_end = packet + TS_PACKET_SIZE;

- if(p >= p_end)

-     return 0;

- 

-   pos = avio_tell(ts->stream->pb);

-   ts->pos47= pos % ts->raw_packet_size;

- 

- if(tss->type == MPEGTS_SECTION)

- {

-     /*

-      * 针对Section, 第一个字节对应的语法元素为:pointer_field(见2.4.4.1),

-      * 它表示在当前TS包中，从pointer_field开始到第一个section的第一个字节间的字节数。

-      * 当TS包中有至少一个section的起始时,

-      *    payload_unit_start_indicator = 1 且 TS负载的第一个字节为pointer_field;

-      *    pointer_field = 0x00时，表示section的起始就在这个字节之后;

-      * 当TS包中没有section的起始时,  

-      *    payload_unit_start_indicator = 0 且 TS负载中没有pointer_field;

-      */

- if(is_start)

- {

- /* pointer field present */

- len=*p++;

- if(p +len> p_end)

-         return 0;

- 

- if(len&& cc_ok)

- {

-         /* 

-          * write remaining section bytes 

-          * TS包的负载部分由Section A的End部分和Section B的Start组成，

-          * 先把Section A的End部分写入

-          */

-         write_section_data(s, tss, p,len, 0);

- 

- /* check whether filter has been closed */

- if(!ts->pids[pid])

-           return 0;

- }

-       p +=len;

- 

- if(p < p_end)

- {

-         /*

-          * 再将Section B的Start部分写入

-          */

-         write_section_data(s, tss, p, p_end - p, 1);

- }

- }

- else

- {

-       /* TS包负载仅是一个Section的中间部分部分，将其写入*/

- if(cc_ok)

- {

-         write_section_data(s, tss, p, p_end - p, 0);

- }

- }

- }

- else

- {

- int ret;

- 

-     /* 

-      * 如果是PES类型，直接调用其Callback，

-      * 但显然，只有Section部分解析完成后才可能解析PES

-      */

- // Note: The position here points actually behind the current packet.

- if((ret = tss->u.pes_filter.pes_cb(tss, p, p_end - p, is_start,

-         pos - ts->raw_packet_size))< 0)

-       return ret;

- }

- 

-   return 0;

- }



write_section_data()函数:

  反复收集buffer中的数据，指导完成相关Section的重组过程,

  然后调用之前注册的两个section_cb.







**5. 节目指定信息的解析**



- /*

-  * PAT(Program Association Table) 节目相关表

-  * 提供了节目号与PID值的对应关系

-  * 见ISO/IEC 13818-1 2.4.4.3 Table 2-30

-  */

- void pat_cb(MpegTSFilter *filter,const uint8_t *section,int section_len);

- 

- /*

-  * PMT(Program Map Table) 节目映射表

-  * 提供了节目号与组成节目的元素之间的映射关系--或者称为"节目定义"

-  * 见ISO/IEC 13818-1 2.4.4.8 Table 2-33

-  */

- void pmt_cb(MpegTSFilter *filter,const uint8_t *section,int section_len);

- 

- 

- /*

-  * SDT(Transport Stream Description Table) TS描述表

-  * 用于定义TS描述子的表

-  * 见ISO/IEC 13818-1 2.4.4.12 Table 2-36

-  */

- void sdt_cb(MpegTSFilter *filter,const uint8_t *section,int section_len)






**6. 解析PES包**


- /* 

-  * 见ISO/IEC 13818-1 2.4.3.6 Table 2-21

-  */

- int mpegts_push_data(MpegTSFilter*filter,

- const uint8_t* buf,

- int            buf_size,

- int            is_start,

-                      int64_t        pos);






至此，整个TS层的解析基本完成。

[http://blog.chinaunix.net/uid-26000296-id-3454725.html](http://blog.chinaunix.net/uid-26000296-id-3454725.html)





