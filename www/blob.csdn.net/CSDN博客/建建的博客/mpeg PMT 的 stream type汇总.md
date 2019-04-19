# mpeg PMT 的 stream type汇总 - 建建的博客 - CSDN博客
2017年11月14日 14:22:56[纪建](https://me.csdn.net/u013898698)阅读数：185
                
Value    Description
0x00     ITU-T | ISO/IEC Reserved
0x01     ISO/IEC 11172-2 Video
0x02     ITU-T Rec. H.262 | ISO/IEC 13818-2 Video or ISO/IEC 11172-2 constrained parameter video stream
0x03     ISO/IEC 11172-3 Audio
0x04     ISO/IEC 13818-3 Audio
0x05     ITU-T Rec. H.222.0 | ISO/IEC 13818-1 private_sections
0x06     ITU-T Rec. H.222.0 | ISO/IEC 13818-1 PES packets containing private data
0x07     ISO/IEC 13522 MHEG
0x08     ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Annex A DSM-CC
0x09     ITU-T Rec. H.222.1
0x0A     ISO/IEC 13818-6 type A
0x0B     ISO/IEC 13818-6 type B
0x0C     ISO/IEC 13818-6 type C
0x0D     ISO/IEC 13818-6 type D
0x0E     ITU-T Rec. H.222.0 | ISO/IEC 13818-1 auxiliary
0x0F     ISO/IEC 13818-7 Audio with ADTS transport syntax
0x10     ISO/IEC 14496-2 Visual
0x11     ISO/IEC 14496-3 Audio with the LATM transport syntax as defined in ISO/IEC 14496-3/Amd.1
0x12     ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in PES packets
0x13     ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in ISO/IEC 14496_sections
0x14     ISO/IEC 13818-6 Synchronized Download Protocol
0x15     Metadata carried in PES packets
0x16     Metadata carried in metadata_sections
0x17     Metadata carried in ISO/IEC 13818-6 Data Carousel
0x18     Metadata carried in ISO/IEC 13818-6 Object Carousel
0x19     Metadata carried in ISO/IEC 13818-6 Synchronized Download Protocol
0x1A     IPMP stream (defined in ISO/IEC 13818-11, MPEG-2 IPMP)
0x1B     AVC video stream as defined in ITU-T Rec. H.264 | ISO/IEC 14496-10 Video
0x1C     ISO/IEC 14496-3 Audio, without using any additional transport syntax, such as DST, ALS and SLS
0x1D     ISO/IEC 14496-17 Text
0x1E     Auxiliary video stream as defined in ISO/IEC 23002-3
0x1F-0x7E ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved
0x7F     IPMP stream
0x80-0xFF User Private
如上就是PMT表的stream_type的汇总，具体也可以查看13818-1标准的Table 2-36，第64页，有相关的详细说明。
我们以coretv为例子来做说明：
#define   STREAMTYPE_11172_VIDEO                  0x01
#define   STREAMTYPE_13818_VIDEO                  0x02
#define   STREAMTYPE_11172_AUDIO                  0x03
#define   STREAMTYPE_13818_AUDIO                  0x04
#define   STREAMTYPE_13818_PRIVATE                0x05
#define   STREAMTYPE_13818_PES_PRIVATE            0x06
#define   STREAMTYPE_13522_MHPEG                  0x07
#define   STREAMTYPE_13818_DSMCC                  0x08
#define   STREAMTYPE_ITU_222_1                    0x09
#define   STREAMTYPE_13818_A                      0x0a
#define   STREAMTYPE_13818_B                      0x0b
#define   STREAMTYPE_13818_C                      0x0c
#define   STREAMTYPE_13818_D                      0x0d
#define   STREAMTYPE_13818_AUX                    0x0e
#define   STREAMTYPE_AAC_AUDIO     0x0f
#define   STREAMTYPE_MPEG4_AUDIO     0x11
#define   STREAMTYPE_H264_VIDEO     0x1b
#define   STREAMTYPE_AVS_VIDEO     0x42
#define   STREAMTYPE_AC3_AUDIO                    0x81
#define   STREAMTYPE_DTS_AUDIO                    0x82
 switch(streamType)
 {
      case STREAMTYPE_11172_AUDIO:
      case STREAMTYPE_13818_AUDIO:
      case STREAMTYPE_AC3_AUDIO:
      case STREAMTYPE_AAC_AUDIO:
      case STREAMTYPE_MPEG4_AUDIO:
              break;
      case STREAMTYPE_11172_VIDEO:
      case STREAMTYPE_13818_VIDEO:
      case STREAMTYPE_H264_VIDEO:
      case STREAMTYPE_AVS_VIDEO:
             break;
      case STREAMTYPE_13818_PES_PRIVATE:
             break;
      case STREAMTYPE_13818_B:
             break;
      default:
             break;
}
            
