# MPEG2/TS获取PCR PID方法 - 建建的博客 - CSDN博客
2017年09月04日 11:58:51[纪建](https://me.csdn.net/u013898698)阅读数：261
一个简单的从指定MPEG2/TS文件中获取PCR PID的函数，
PMT的头结构中有PCR PID,我们首先得到PMT PID，然后分析PMT包，就可以得到PCR PID了。
GetPmtPid函数的实现见：[http://blog.csdn.net/myaccella/article/details/6982596](http://blog.csdn.net/myaccella/article/details/6982596)
MAKE_PID:
#define MAKE_PID(p) (((p[0]<<8) + p[1]) & 0x1fff)
**[html]**[view
 plain](http://blog.csdn.net/myaccella/article/details/6982762#)[copy](http://blog.csdn.net/myaccella/article/details/6982762#)
- ushort GetPcrPid(const char* filename)  
- {  
- #define TSPACKET_SIZE     188   
-     ushort pid, pmtpid, pcrpid = 0;  
-     uchar buf[TSPACKET_SIZE];  
-     uchar* p = buf;  
-     FILE*  fp;  
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
-         // 8 为PCR PID在PMT表头中的偏移  
-         p += 8;  
- 
-         // 取到的PCR PID如果是0x1fff, 则需要继续取  
- pcrpid = MAKE_PID(p);  
-         if (pcrpid == 0x1fff) {  
- pcrpid = 0;  
-             continue;  
-         }  
- 
-         break;  
-     }  
-          fclose(fp);  
-     return pcrpid;  
- }  
