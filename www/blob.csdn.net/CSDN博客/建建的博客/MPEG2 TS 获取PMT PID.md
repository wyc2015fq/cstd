# MPEG2/TS 获取PMT PID - 建建的博客 - CSDN博客
2017年09月04日 11:57:43[纪建](https://me.csdn.net/u013898698)阅读数：146
一个简单的从指定MPEG2/TS文件中获取PMT PID的函数，
注意代码中的注释。
#define MAKE_PID(p) (((p[0]<<8) + p[1]) & 0x1fff)
**[html]**[view
 plain](http://blog.csdn.net/myaccella/article/details/6982596#)[copy](http://blog.csdn.net/myaccella/article/details/6982596#)
- ushort GetPmtPid(const char* filename)  
- {  
- #define TSPACKET_SIZE     188    
-     ushort pid;  
-     uchar buf[TSPACKET_SIZE];  
-     uchar* p = buf;  
-     FILE*  fp;  
- 
- fp = fopen(filename, "rb");  
-     if (fp == NULL) {  
-         fprintf(stderr, "Failed to open the file %s: %s\n", filename, strerror(errno));  
-         return 0;  
-     }  
- 
-     while (fread(buf, 1, TSPACKET_SIZE, fp) == TSPACKET_SIZE) {  
-         if (buf[0] != MPEG_TS_SYNC_BYTE) {  
-             fprintf(stderr, "No sync byte where one was expected!\n");  
-             break;  
-         }  
- 
- p = buf;  
- pid = MAKE_PID((p+1));  
-         if ( pid != 0 ) {  
-             // 不是PAT包  
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
-         // 8 为PAT表头长度，如果定义了表头，则用sizeof运算符  
-         p += 8;  
- 
-         // NIT Program exist??  
-         if( *((ushort*)p) == 0) {  
-             p += 4; // PAT的节目结构为4字节  
-         }  
- 
-         p += 2; // 节目号为两个字节  
- 
-         fclose(fp);  
-         return MAKE_PID(p);  
-     }  
- 
-     fclose(fp);  
-     return 0;  
- }  
