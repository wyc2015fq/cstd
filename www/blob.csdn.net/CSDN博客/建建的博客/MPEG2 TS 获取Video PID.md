# MPEG2/TS 获取Video PID - 建建的博客 - CSDN博客
2017年09月04日 11:59:47[纪建](https://me.csdn.net/u013898698)阅读数：211
个人分类：[多媒体封装格式详解](https://blog.csdn.net/u013898698/article/category/7105689)

从PMT表中获取Video PID的函数代码
GetPmtPid函数的实现见：[http://blog.csdn.net/myaccella/article/details/6982596](http://blog.csdn.net/myaccella/article/details/6982596)
#define MAKE_PID(p) (((p[0]<<8) + p[1]) & 0x1fff)
#define MAKE_WORD(h, l) (((h) << 8) | (l))
Little endian 的结构定义
typedef struct {
  unsigned stream_type  :8;
  unsigned elementary_PID_high :5;
  unsigned    :3;
  unsigned elementary_PID_low :8;
  unsigned ES_info_length_high :4;
  unsigned    :4;
  unsigned ES_info_length_low :8;
}MpegPmtStream_t;
typedef struct {
  unsigned table_id   :8;
  unsigned section_length_high  :4;
  unsigned     :2;
  unsigned    :1;
  unsigned section_syntax_indicator :1;
  unsigned section_length_low  :8;
  unsigned program_number_high  :8;
  unsigned program_number_low  :8;
  unsigned current_next_indicator :1;
  unsigned version_number  :5;
  unsigned     :2;
  unsigned section_number  :8;
  unsigned last_section_number  :8;
  unsigned PCR_PID_high   :5;
  unsigned    :3;
  unsigned PCR_PID_low   :8;
  unsigned program_info_length_high :4;
  unsigned    :4;
  unsigned program_info_length_low :8;
}MpegPmtHeader_t;
**[html]**[view
 plain](http://blog.csdn.net/myaccella/article/details/6983015#)[copy](http://blog.csdn.net/myaccella/article/details/6983015#)
- ushort GetVideoPid(const char* filename)  
- {  
- #define TSPACKET_SIZE     188   
- #define CRC_SIZE          4  
-     ushort pid, pmtpid, videopid = 0;  
-     uchar buf[TSPACKET_SIZE];  
-     uchar* p = buf;  
-     MpegPmtHeader_t* header;  
-     MpegPmtStream_t* stream;  
-     FILE*  fp;  
-     int pos = 0;  
-     ushort section_len;  
- 
- pmtpid = GetPmtPid(filename);  
-     if (pmtpid == 0) {  
-         fprintf(stderr, "Unable to get PMT PID!\n");  
-         return 0;  
-     }  
- 
- fp = fopen(filename, "rb");  
-     if (fp == NULL) {  
-         fprintf(stderr, "Failed to open the file %s: %s\n", filename, strerror(errno));  
-         return 0;  
-     }  
- 
- 
-     while (fread(buf, 1, TSPACKET_SIZE, fp) == TSPACKET_SIZE) {  
-         if (buf[0] != MPEG_TS_SYNC_BYTE) { // @MPEG_TS_SYNC_BYTE = 0x47
-             fprintf(stderr, "No sync byte!\n");  
-             break;  
-         }  
- 
- p = buf;  
- pid = MAKE_PID((p+1));  
-         if( pid != pmtpid ) {  
-             // Not a  PMT packet  
-             continue;  
-         }  
- 
-         // payload_unit_start_indicator 必须为1  
-         if (!(buf[1] >> 6 & 0x01)) {  
-             fprintf(stderr, "No payload_unit_start_indicator!\n");  
-             continue;  
-         }  
- 
-         // Skip the TS header  
-         p += MPEG_TS_HEADER_SIZE; //@MPEG_TS_HEADER_SIZE = 4
- 
-         // Adaptation field exist or not  
-         if ((buf[3] >> 4 & 0x03) & 0x2) {  
-             fprintf(stderr, "Adaptation field exist\n");  
-             p += 1 + p[0];  // p[0]是Adaptation field长度, 1为长度域  
-         }  
- 
-         // p[0] 为 Point field长度， 1为Point field  
-         p += 1 + p[0];  
- 
-         // Now p 指向 PMT表头  
- header = (MpegPmtHeader_t*)p;  
-         pos +=  sizeof(MpegPmtHeader_t);   
-         pos += MAKE_WORD(header->program_info_length_high, header->program_info_length_low);;  
- 
-         // Parse stream table  
- section_len = MAKE_WORD(header->section_length_high, header->section_length_low);  
-         while (pos < (section_len + 2) - CRC_SIZE) {  
- stream = (MpegPmtStream_t*)(p + pos);  
-             // @MPEG_TS_STREAM_TYPE_VIDEO_2 = 0x02
-             if (stream->stream_type == MPEG_TS_STREAM_TYPE_VIDEO_2) {  
- videopid = MAKE_WORD(stream->elementary_PID_high, stream->elementary_PID_low);  
-                 break;  
-             }  
-             // @MPEG_PMT_STREAM_HEADER_SIZE = 5
-             pos +=  MPEG_PMT_STREAM_HEADER_SIZE   
-                 + MAKE_WORD(stream->ES_info_length_high, stream->ES_info_length_low);  
-         }  
-     }  
- 
-     fclose(fp);  
-     return videopid;  
- }  
