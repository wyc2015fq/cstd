# TS协议解析第一部分（PAT） - 建建的博客 - CSDN博客
2017年11月03日 12:18:09[纪建](https://me.csdn.net/u013898698)阅读数：179
1. 前言
1.1 工具准备：
|ts文件|[http://download.csdn.net/detail/u013354805/9540190](http://download.csdn.net/detail/u013354805/9540190)|
|----|----|
|UltraEdit|[http://download.csdn.net/detail/u013354805/9540205](http://download.csdn.net/detail/u013354805/9540205)|
|EStreamAnalyzer|[http://download.csdn.net/detail/u013354805/8866107](http://download.csdn.net/detail/u013354805/8866107)|
|Iso13818-1 规范|[http://download.csdn.net/detail/u013354805/8865315](http://download.csdn.net/detail/u013354805/8865315)|
1.2 界面：
1) UltraEdit打开ts文件（如果发现打开的是一堆乱请用ctrl +Ｈ以16进制打开）：
![](https://img-blog.csdn.net/20160603145214829)
2) EStreamAnalyzer打开TS文件如下：
![](https://img-blog.csdn.net/20160603145227829)
2. 分析TS包：
传输流包长度必须是188字节，sync_byte必须是0x47。任何有用的数据都不会是FF。
![](https://img-blog.csdn.net/20160603153931940)
2.1 第一个TS包：
        PAT是Program
 Association Table的简称,即”节目关联表”. PAT定义了当前TS流中所有的节目，其PID恒为0x00，它是PSI信息的根节点，要查找节目必须从PAT表开始.PAT表描述了当前TS流中包含什么样的PID,主要包括当前流的NIT表格的PID,当前流中有多少个不同类型PMT表,每个PMT表对应的频道号等信息.
![](https://img-blog.csdn.net/20160603150027046)
2.2 TS包头：
TS包头只有4个字节(47 60 00 10)，除掉第一个字节0x47，剩下就3个，重新分组如下：
0    1    1  0000000000000    00    01    0000
|sync_byte|同步字节|0x47：|
|----|----|----|
|transport_error_indicator|传输错误标识|0：|
|payload_unit_start_indicator|负载单元开始标识|1：该字段用来表示TS包的有效净荷带有PES包或者PSI数据的情况。当TS包带有PES包数据时，payload_unit_start_indicator具有以下特点：置为1，表示TS包的有效净荷以PES包的第一个字节开始；置为0，表示TS包的开始不是PES包。当TS包带有PSI数据时，payload_unit_start_indicator具有以下特点：置为1，表示TS包带有PSI部分的第一个字节，即第一个字节带有指针pointer_field；置为0，表示TS包不带有一个PSI部分的第一个字节，即在有效净荷中没有指针pointer_field。对于空包的包，payload_unit_start_indicator应该置为0。|
|transport_priority|传输优先级|1：|
|pid|PID|0x000：即为0，表示PAT|
|transport_scrambling_control|传输扰乱控制|00：|
|adaptation_field_control|自适应区域控制|00：是保留值。01：负载中只有有效载荷。10：负载中只有自适应字段。11：先有自适应字段，再有有效载荷。|
|continuity_counter|连续计数器|0x0：|
|同步字节|传输错误标识|负载单元开始标识|传输优先级|PID|传输扰乱控制|自适应区域控制|连续计数器|
|----|----|----|----|----|----|----|----|
|8|1|1|1|13|2|2|4|
|第1个字节|第2、3个字节|第4个字节| | | | | |
2.3 TS包：
1) 在payload_unit_start_indicator为1时，在前4个字节后会有一个调整字节。所以实际数据应该为去除第一个字节00后的数据。（~~47 60 00 10~~~~00~~00
 B0 0D 00 00 C1 00 00 00 01 E0 81 0C 8C BE 32）
2) B0 0D：1011 0000 0000 1101
section_length：13（~~47 60 00 10~~~~00~~~~00 B0 0D~~ 00 00 C1 00 00 00 01 E0 81 0C 8C BE 32）
3）
00 00：0000 0000 0000 0000
C1：1100 0001
00：0000 0000
00: 0000 0000
00 01：0000 0000 0000 0001
E0 81：1110 0000 1000 0001
0C 8C BE 32:0000 1100 1000 1100 1000 1110 0011 0010（CRC校验码，用来检验数据正确性的循环冗余校验码）
|table_id|0x00：对于PAT只能是0x00|
|----|----|
|section_syntax_indicator|1：应设置为‘1’|
|0|0：|
|reserved|11：|
|section_length|0000 0000 1101：为0xD,13该字段的头两个比特必为‘00’，剩余10比特指定该分段的字节数，紧随分段长度字段开始，并包括CRC。此字段中，该值应不超过1021（0x3FD）|
|transport_stream_id|0x00：该字节充当标签，标识网络内此传输流有别于任何其他路复用流。其值由用户规定。|
|reserved|11：|
|version_number|00000：一旦PAT有变化，版本号加1|
|current_next_indicator|1：表示传送的PAT当前可以使用，若为0表示下一个表有效|
|section_number|0x00：给出了该分段的数目。在PAT中的第一个分段的section_number为0x00，PAT中每一分段将加1。|
|last_section_number|0x00：该字段指出了最后一个分段号。在整个PAT中即分段的最大数目。|
|program_number|0x0001：这个为PMT。该字段指出了节目对于那个program_map_PID是可以使用的。如果是0x0000，那么后面的PID是网络PID，否则其他值由用户定义。|
|reserved|111：|
|program_map_PID|0 0000 1000 0001：0x81该字段指出TS包中的PID值。|
|table_id|8|第1个字节|
|----|----|----|
|section_syntax_indicator|1|第2、3个字节|
|0|1| |
|reserved|2| |
|section_length|12| |
|transport_stream_id|16|第4、5个字节|
|reserved|2|第6个字节|
|version_number|5| |
|current_next_indicator|1| |
|section_number|8|第7个字节|
|last_section_number|8|第8个字节|
|循环开始| | |
|program_number|16|2个字节|
|reserved|3|2个字节|
|network_id(节目号为0时)program_map_PID(节目号为其他时)|13| |
|循环结束| | |
|CRC_32|32|4个字节|
3. 实现：
3.1 PAT表格定义如下：
[cpp][view
 plain](http://blog.csdn.net/u013354805/article/details/51578457#)[copy](http://blog.csdn.net/u013354805/article/details/51578457#)
- typedefstruct TS_PAT_Program  
- {  
-  unsigned program_number    :16;   //节目号
-  unsigned program_map_PID   :13;   //节目映射表的PID，节目号大于0时对应的PID，每个节目对应一个
- }TS_PAT_Program;  
3.2 PAT表结构体如下:
[cpp][view
 plain](http://blog.csdn.net/u013354805/article/details/51578457#)[copy](http://blog.csdn.net/u013354805/article/details/51578457#)
- typedefstruct TS_PAT  
- {  
-     unsigned table_id                     : 8; //固定为0x00 ，标志是该表是PAT
-     unsigned section_syntax_indicator     : 1; //段语法标志位，固定为1
-     unsigned zero                         : 1; //0
-     unsigned reserved_1                   : 2; // 保留位
-      unsigned section_length               : 12; //表示这个字节后面有用的字节数，包括CRC32
-     unsigned transport_stream_id        : 16; //该传输流的ID，区别于一个网络中其它多路复用的流
-     unsigned reserved_2                   : 2;// 保留位
-     unsigned version_number               : 5; //范围0-31，表示PAT的版本号
-     unsigned current_next_indicator       : 1; //发送的PAT是当前有效还是下一个PAT有效
-     unsigned section_number               : 8; //分段的号码。PAT可能分为多段传输，第一段为00，以后每个分段加1，最多可能有256个分段
-     unsigned last_section_number          : 8;  //最后一个分段的号码
- 
-     std::vector<TS_PAT_Program> program;  
-     unsigned reserved_3                    : 3; // 保留位
-     unsigned network_PID                    : 13; //网络信息表（NIT）的PID,节目号为0时对应的PID为network_PID
-     unsigned CRC_32                        : 32;  //CRC32校验码
- } TS_PAT;   
3.3 解析代码如下：
[cpp][view
 plain](http://blog.csdn.net/u013354805/article/details/51578457#)[copy](http://blog.csdn.net/u013354805/article/details/51578457#)
- HRESULT CTS_Stream_Parse::adjust_PAT_table( TS_PAT * packet, unsigned char * buffer)  
- {  
-     packet->table_id                    = buffer[0];  
-     packet->section_syntax_indicator    = buffer[1] >> 7;  
-     packet->zero                        = buffer[1] >> 6 & 0x1;  
-     packet->reserved_1                  = buffer[1] >> 4 & 0x3;  
-     packet->section_length              = (buffer[1] & 0x0F) << 8 | buffer[2];   
- 
-     packet->transport_stream_id           = buffer[3] << 8 | buffer[4];  
- 
-     packet->reserved_2                    = buffer[5] >> 6;  
-     packet->version_number                = buffer[5] >> 1 &  0x1F;  
-     packet->current_next_indicator        = (buffer[5] << 7) >> 7;  
-     packet->section_number                = buffer[6];  
-     packet->last_section_number           = buffer[7];   
- 
- int len = 0;  
-     len = 3 + packet->section_length;  
-     packet->CRC_32                        = (buffer[len-4] & 0x000000FF) << 24  
-   | (buffer[len-3] & 0x000000FF) << 16  
-   | (buffer[len-2] & 0x000000FF) << 8   
-   | (buffer[len-1] & 0x000000FF);   
- 
- int n = 0;  
- for ( n = 0; n < packet->section_length - 12; n += 4 )  
-     {  
-         unsigned  program_num = buffer[8 + n ] << 8 | buffer[9 + n ];    
-         packet->reserved_3           = buffer[10 + n ] >> 5;   
- 
-         packet->network_PID = 0x00;  
- if ( program_num == 0x00)  
-        {    
-             packet->network_PID = (buffer[10 + n ] & 0x1F) << 8 | buffer[11 + n ];  
- 
-             TS_network_Pid = packet->network_PID; //记录该TS流的网络PID
- 
-              TRACE(" packet->network_PID %0x /n/n", packet->network_PID );  
-         }  
- else
-         {  
-            TS_PAT_Program PAT_program;  
-            PAT_program.program_map_PID = (buffer[10 + n] & 0x1F) << 8 | buffer[11 + n];  
-            PAT_program.program_number = program_num;  
-            packet->program.push_back( PAT_program );  
-            TS_program.push_back( PAT_program );//向全局PAT节目数组中添加PAT节目信息     
-         }           
-     }  
- return 0;  
- }  
- 
