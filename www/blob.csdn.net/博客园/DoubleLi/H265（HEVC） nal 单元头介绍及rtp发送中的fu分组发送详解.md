# H265（HEVC） nal 单元头介绍及rtp发送中的fu分组发送详解 - DoubleLi - 博客园






首先来介绍下h265（HEVC）nal单元头，与h264的nal层相比，h265的nal unit header有两个字节构成，如下图所示：

![](http://img.blog.csdn.net/20150210190251434?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2F0ZXIxMjA5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

从图中可以看出hHEVC的nal包结构与h264有明显的不同，hevc加入了nal所在的时间层的ＩＤ，取去除了nal_ref_idc，此信息合并到了naltype中，通常情况下F为0，layerid为0,TID为1。

        nal单元的类型有如下几种：





**[cpp]**[view plain](http://blog.csdn.net/water1209/article/details/43706525#)[copy](http://blog.csdn.net/water1209/article/details/43706525#)

![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- enum NalUnitType  
- {  
-   NAL_UNIT_CODED_SLICE_TRAIL_N = 0,   // 0  
-   NAL_UNIT_CODED_SLICE_TRAIL_R,   // 1  
- 
-   NAL_UNIT_CODED_SLICE_TSA_N,     // 2  
-   NAL_UNIT_CODED_SLICE_TLA,       // 3   // Current name in the spec: TSA_R  
- 
-   NAL_UNIT_CODED_SLICE_STSA_N,    // 4  
-   NAL_UNIT_CODED_SLICE_STSA_R,    // 5  
- 
-   NAL_UNIT_CODED_SLICE_RADL_N,    // 6  
-   NAL_UNIT_CODED_SLICE_DLP,       // 7 // Current name in the spec: RADL_R  
- 
-   NAL_UNIT_CODED_SLICE_RASL_N,    // 8  
-   NAL_UNIT_CODED_SLICE_TFD,       // 9 // Current name in the spec: RASL_R  
- 
-   NAL_UNIT_RESERVED_10,  
-   NAL_UNIT_RESERVED_11,  
-   NAL_UNIT_RESERVED_12,  
-   NAL_UNIT_RESERVED_13,  
-   NAL_UNIT_RESERVED_14,  
-   NAL_UNIT_RESERVED_15, NAL_UNIT_CODED_SLICE_BLA,       // 16   // Current name in the spec: BLA_W_LP  
- NAL_UNIT_CODED_SLICE_BLA,       // 16   // Current name in the spec: BLA_W_LP  
-   NAL_UNIT_CODED_SLICE_BLANT,     // 17   // Current name in the spec: BLA_W_DLP  
-   NAL_UNIT_CODED_SLICE_BLA_N_LP,  // 18  
-   NAL_UNIT_CODED_SLICE_IDR,       // 19  // Current name in the spec: IDR_W_DLP  
-   NAL_UNIT_CODED_SLICE_IDR_N_LP,  // 20  
-   NAL_UNIT_CODED_SLICE_CRA,       // 21  
-   NAL_UNIT_RESERVED_22,  
-   NAL_UNIT_RESERVED_23,  
- 
-   NAL_UNIT_RESERVED_24,  
-   NAL_UNIT_RESERVED_25,  
-   NAL_UNIT_RESERVED_26,  
-   NAL_UNIT_RESERVED_27,  
-   NAL_UNIT_RESERVED_28,  
-   NAL_UNIT_RESERVED_29,  
-   NAL_UNIT_RESERVED_30,  
-   NAL_UNIT_RESERVED_31,  
- 
-   NAL_UNIT_VPS,                   // 32  
-   NAL_UNIT_SPS,                   // 33  
-   NAL_UNIT_PPS,                   // 34  
-   NAL_UNIT_ACCESS_UNIT_DELIMITER, // 35  
-   NAL_UNIT_EOS,                   // 36  
-   NAL_UNIT_EOB,                   // 37  
-   NAL_UNIT_FILLER_DATA,           // 38  
-   NAL_UNIT_SEI,                   // 39 Prefix SEI  
-   NAL_UNIT_SEI_SUFFIX,            // 40 Suffix SEI  
-   NAL_UNIT_RESERVED_41,  
-   NAL_UNIT_RESERVED_42,  
-   NAL_UNIT_RESERVED_43,  
-   NAL_UNIT_RESERVED_44,  
-   NAL_UNIT_RESERVED_45,  
-   NAL_UNIT_RESERVED_46,  
-   NAL_UNIT_RESERVED_47,  
-   NAL_UNIT_UNSPECIFIED_48,  
-   NAL_UNIT_UNSPECIFIED_49,  
-   NAL_UNIT_UNSPECIFIED_50,  
-   NAL_UNIT_UNSPECIFIED_51,  
-   NAL_UNIT_UNSPECIFIED_52,  
-   NAL_UNIT_UNSPECIFIED_53,  
-   NAL_UNIT_UNSPECIFIED_54,  
-   NAL_UNIT_UNSPECIFIED_55,  
-   NAL_UNIT_UNSPECIFIED_56,  
-   NAL_UNIT_UNSPECIFIED_57,  
-   NAL_UNIT_UNSPECIFIED_58,  
-   NAL_UNIT_UNSPECIFIED_59,  
-   NAL_UNIT_UNSPECIFIED_60,  
-   NAL_UNIT_UNSPECIFIED_61,  
-   NAL_UNIT_UNSPECIFIED_62,  
-   NAL_UNIT_UNSPECIFIED_63,  
-   NAL_UNIT_INVALID,  
- };  


下面接收下fu分组打包方式，fu分组包头格式如下：



![](http://img.blog.csdn.net/20150210192152283?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2F0ZXIxMjA5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

fus包头包含了两个字节的payloadhdr，一个字节的fu header，fu header与h264一样，结构如下图，包含开始位（1b）、停止位(1b)、futype(6b)

![](http://img.blog.csdn.net/20150210192118958?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2F0ZXIxMjA5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

paylodhdr两个自己的赋值，其实就是把hevc帧数据的nal unit header的naltype替换为49即可，下面是从ffmpeg源码中截取出来的fu打包方式代码片段：





**[cpp]**[view plain](http://blog.csdn.net/water1209/article/details/43706525#)[copy](http://blog.csdn.net/water1209/article/details/43706525#)

![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- static void nal_send(AVFormatContext *ctx, const uint8_t *buf, int len, int last_packet_of_frame)  
- {  
-      RTPMuxContext *rtp_ctx = ctx->priv_data;  
- int rtp_payload_size = rtp_ctx->max_payload_size - RTP_HEVC_HEADERS_SIZE;  
- int nal_type = (buf[0] >> 1) & 0x3F;  
- 
- /* send it as one single NAL unit? */  
- if (len <= rtp_ctx->max_payload_size) //小于对定的最大值时，直接发送（最大值一般小于mtu）  
-      {  
- /* use the original NAL unit buffer and transmit it as RTP payload */  
-         ff_rtp_send_data(ctx, buf, len, last_packet_of_frame);  
- 
-     }  
- else //大于最大值时进行fu分组发送  
-     {  
- /* 
-                     create the HEVC payload header and transmit the buffer as fragmentation units (FU) 
- 
-                     0                   1 
-                     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 
-                     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
-                     |F|   Type    |  LayerId  | TID | 
-                     +-------------+-----------------+ 
- 
-                         F       = 0 
-                         Type    = 49 (fragmentation unit (FU)) 
-                         LayerId = 0 
-                         TID     = 1 
-         */  
-         rtp_ctx->buf[0] = 49 << 1;  
-         rtp_ctx->buf[1] = 1;  
- //此处为paylaodhdr,规范赋值应该是替换hevc数据nal 的payloadhdr的type  
- //rtp_ctx->buf[0] = (buf[0] &0x81) | (49<<1);  
- //rtp_ctx->buf[1] = buf[1]  
- /* 
-             create the FU header 
- 
-             0 1 2 3 4 5 6 7 
-             +-+-+-+-+-+-+-+-+ 
-             |S|E|  FuType   | 
-             +---------------+ 
- 
-                 S       = variable 
-                 E       = variable 
-                 FuType  = NAL unit type 
-         */  
-         rtp_ctx->buf[2] = nal_type;  
- /* set the S bit: mark as start fragment */  
-         rtp_ctx->buf[2] |= 1 << 7;       
- /* pass the original NAL header */  
- //此处要注意，当是分组的第一报数据时，应该覆盖掉前两个字节的数据，h264要覆盖前一个字节的数据，即是第一包要去除hevc帧数据的paylaodhdr  
-         buf += 2;  
-         len -= 2;     
- while (len > rtp_payload_size)   
-         {  
- /* complete and send current RTP packet */  
-                 memcpy(&rtp_ctx->buf[RTP_HEVC_HEADERS_SIZE], buf, rtp_payload_size);  
-             ff_rtp_send_data(ctx, rtp_ctx->buf, rtp_ctx->max_payload_size, 0);  
- 
-                 buf += rtp_payload_size;  
-             len -= rtp_payload_size;  
- 
- /* reset the S bit */  
-                 rtp_ctx->buf[2] &= ~(1 << 7);  
- 
-         }  
- 
- /* set the E bit: mark as last fragment */  
-          rtp_ctx->buf[2] |= 1 << 6;  
- 
- /* complete and send last RTP packet */  
-          memcpy(&rtp_ctx->buf[RTP_HEVC_HEADERS_SIZE], buf, len);  
-         ff_rtp_send_data(ctx, rtp_ctx->buf, len + 2, last_packet_of_frame);  
- 
-     }  
- }  










通过rtp发送hevc视频数据，当hevc帧数据大于mtu时，应该进行fu分组发送，从上面代码流程就是对超过max_payload_size数据进行fu分组的流程，这个h264 fu-A很类似，很容易理解。



参考规范：

https://tools.ietf.org/html/draft-ietf-payload-rtp-h265-14

ffmpeg相关代码

https://www.ffmpeg.org/doxygen/2.5/rtpenc__hevc_8c_source.html









