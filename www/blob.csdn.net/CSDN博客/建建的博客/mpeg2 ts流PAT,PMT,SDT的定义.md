# mpeg2 ts流PAT,PMT,SDT的定义 - 建建的博客 - CSDN博客
2018年03月12日 15:19:28[纪建](https://me.csdn.net/u013898698)阅读数：77
更具体准确的信息请参考iso13818-1,都在里面定义的
PAT的定义：
Table_id：为8bit字段，该字段标识节目关联分段，对于PAT，置为0x00。
Section_syntax_indicator：1bit字段，对于PAT，置为
0x01。
Reserved：2bit保留字段，用于将来扩展，置为11。
Section_length：12bit字段，指示当前section的长度，计数值
从分段长度下一个字节开始，包括CRC校验的4个字节，开头两
位置为00，因此其大小不超过1021。
Transport_stream_id：16bit字段，当前TS流的ID，与网络中其
他TS流相区别，由运营商指定。
Reserved：2bit保留字段，用于将来扩展，置为11。
Version_number：5bit字段，指出PAT表的版本号，一旦PAT表有变
化，其版本号增1，当增至31时，恢复至0。
Current_next_indicator：1bit，置为1时，表示传送的PAT当
前有效，置为0表示PAT下一次有效。
Section_number：8bit字段，表示section的数目，从0x00开
始。
Last_section_number：8bit字段，指出最后一个section号，
即PAT表section的最大数目。
Program_number：16bit字段，指出了节目对于哪一个PMT
PID是可用的，当为0x00时，后面的PID对应于NIT。
Reserved：3bit保留字段，用于将来扩展，置为111。
Network_id：13bit字段，NIT PID。
Program_map_PID：13bit字段，对应于program_number
所指定的节目的program_map_section的PID，从上面可看出：一个program用4字节来表示（包括16bit的program_number与13bit的PID）。
CRC：用来证实数据正确性的循环冗余校验码。
(section_number和last_section_number的功能是当PAT内容>184字节时，PAT表会分成多个段(sections),解复用程序必须在全部接收完成后再进行PAT的分析)
PMT定义如下: 各字段含义如下:
table_id:8bits的ID,应该是0x02
section_syntax_indicator:1bit的段语法标志,应该是''1''''0'':固定是''0'',如果不是说明数据有错.
reserved:2bits保留位,应该是''00''
section_length:16bits段长度,从program_number开始,到CRC_32(包含)的字节总数.
program_number:16bits的频道号码,表示当前的PMT关联到的频道.换句话就是说,当前描述的是program_number频道的信息.
reserved:2bits保留位,应该是''00''
version_number:版本号码,如果PMT内容有更新,则version_number会递增1通知解复用程序需要重新接收节目信息,否则version_number是固定不变的.
current_next_indicator:当前未来标志符,一般是0
section_number:当前段号码
last_section_number:最后段号码,含义和PAT中的对应字段相同,请参考PAT部分.
reserved:3bits保留位,一般是''000''.
PCR_PID:13bits的PCR PID,具体请参考ISO13818-1,解复用程序不使用该参数.
reserved:4bits保留位,一般是''0000''
program_info_length:节目信息长度(之后的是N个描述符结构,一般可以忽略掉,这个字段就代表描述符总的长度,单位是Bytes)紧接着就是频道内部包含的节目类型和对应的PID号码了.
stream_type:8bits流类型,标志是Video还是Audio还是其他数据.
reserved:3 bits保留位.
elementary_PID:13bits对应的数据PID号码(如果stream_type是Video,那么这个PID就是VideoPID,如果stream_type标志是Audio,那么这个PID就是Audio PID)
reserved:4 bits保留位.
ES_info_length:和program_info_length类似的信息长度(其后是N2个描述符号)
CRC_32:32bits段末尾是本段的CRC校验值
SDT的定义
DVB系统提出了一个SDT表格,该表格标志一个节目的名称,并且能和PMT中的PID联系起来,这样用户就可以通过直接选择节目名称来选择节目了. SDT, Service descriptionsection,服务描述段 SDT可以提供的信息包括: (1) 该节目是否在播放中 (2) 该节目是否被加密 (3)该节目的名称
SDT定义如下: 各字段定义如下:
table_id:8bits的ID,可以是0x42,表示描述的是当前流的信息,也可以是0x46,表示是其他流的信息(EPG使用此参数)
section_syntax_indicator:段语法标志,一般是''1''
reserved_future_used:2bits保留未来使用
reserved:1bit保留位,防止控制字冲突,一般是''0'',也有可能是''1''
section_length:12bits的段长度,单位是Bytes,从transport_stream_id开始,到CRC_32结束(包含)
transport_stream_id:16bits当前描述的流ID
reserved:2bits保留位
version_number:5bits的版本号码,如果数据更新则此字段递增1
current_next_indicator:当前未来标志,一般是''0'',表示当前马上使用.
original_netword_id:16bits的原始网络ID号
reserved_future_use:8bits保留未来使用位
接下来是N个节目信息的循环:
service_id:16 bits的服务器ID,实际上就是PMT段中的program_number.
reserved_future_used:6bits保留未来使用位
EIT_schedule_flag:1bit的EIT信息,1表示当前流实现了该节目的EIT传送
EIT_present_following_flag:1bits的EIT信息,1表示当前流实现了该节目的EIT传送
running_status:3bits的运行状态信息:1-还未播放2-几分钟后马上开始,3-被暂停播出,4-正在播放,其他---保留
free_CA_mode:1bits的加密信息,''1''表示该节目被加密. 紧 接着的是描述符,一般是Servicedescriptor,分析此描述符可以获取servive_id指定的节目的节目名称.具体格式请参考 EN300468中的Servicedescriptor部分.
分析完毕,则节目名称和节目号码已经联系起来了.机顶盒程序就可以用这些节目名称代替PID让用户选择,从而实现比较友好的用户界面!
H.222规范---PMT的stream_type字段
0x00 
   ITU-T | ISO/IEC Reserved
0x01    ISO/IEC 11172-2 Video
0x02    ITU-T Rec. H.262 | ISO/IEC 13818-2 Video or ISO/IEC 11172-2constrained parameter video stream
0x03    ISO/IEC 11172-3 Audio
0x04    ISO/IEC 13818-3 Audio
0x05    ITU-T Rec. H.222.0 | ISO/IEC 13818-1 private_sections
0x06    ITU-T Rec. H.222.0 | ISO/IEC 13818-1 PES packets containing privatedata
0x07    ISO/IEC 13522 MHEG
0x08    ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Annex A DSM-CC
0x09    ITU-T Rec. H.222.1
0x0A    ISO/IEC 13818-6 type A
0x0B    ISO/IEC 13818-6 type B
0x0C    ISO/IEC 13818-6 type C
0x0D    ISO/IEC 13818-6 type D
0x0E    ITU-T Rec. H.222.0 | ISO/IEC 13818-1 auxiliary
0x0F    ISO/IEC 13818-7 Audio with ADTS transport syntax
0x10    ISO/IEC 14496-2 Visual
0x11    ISO/IEC 14496-3 Audio with the LATM transport syntax as defined inISO/IEC 14496-3/Amd.1
0x12    ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried inPES packets
0x13    ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried inISO/IEC 14496_sections
0x14    ISO/IEC 13818-6 Synchronized Download Protocol
0x15    Metadata carried in PES packets
0x16    Metadata carried in metadata_sections
0x17    Metadata carried in ISO/IEC 13818-6 Data Carousel
0x18    Metadata carried in ISO/IEC 13818-6 Object Carousel
0x19    Metadata carried in ISO/IEC 13818-6 Synchronized DownloadProtocol
0x1A    IPMP stream (defined in ISO/IEC 13818-11, MPEG-2 IPMP)
0x1B    AVC video stream as defined in ITU-T Rec. H.264 | ISO/IEC 14496-10Video
0x1C    ISO/IEC 14496-3 Audio, without using any additional transportsyntax, such as DST, ALS and SLS
0x1D    ISO/IEC 14496-17 Text
0x1E    Auxiliary video stream as defined in ISO/IEC 23002-3
0x1F-0x7E ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved
0x7F    IPMP stream
0x80-0xFF User Private
