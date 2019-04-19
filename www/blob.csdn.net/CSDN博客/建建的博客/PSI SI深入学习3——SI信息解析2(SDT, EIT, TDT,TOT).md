# PSI/SI深入学习3——SI信息解析2(SDT, EIT, TDT,TOT) - 建建的博客 - CSDN博客
2018年03月14日 10:00:54[纪建](https://me.csdn.net/u013898698)阅读数：106
***PSI******/******SI*****深入学习**
BASIC KNOWLEDGE LEARNING OF PSI/SI 
SDT**解析**
PARSINGOF SDT
"SDT描述了业务内容及信息，连接了NIT与EIT和PMT（PSI）"
        SDT即服务描述表（Service Description Table），它描述了一个业务中的内容以及信息，它承上启下，以transport_stream_ID连接了NIT和EIT；SDT的servicID必须与PMT中的Program_no一致，因此，SDT又与PSI的信息连接。
        对于一般的音视频业务，SDT表中不会有太多的信息需要添加。如果是其他业务（如NVOD、马赛克、数据广播等），在SDT中需要插入相应业务的描述符。
SDT的表结构分析
ANALYZEOF THE STRUCTUREOF SDT
         SDT表被切分成业务描述段（service_description_section），由PID为0x0011的TS包传输（BAT段也由PID为0x0011的TS包传输，但table_id不同）。
描述现行TS（即包含SDT表的TS）的SDT表的任何段的table_id都为0x42，且具有相同的table_id_extension（transport_stream_id）以及相同的original_network_id。
指向非现行TS的SDT表的任何段的table_id都应取0x46。
下面给出了SDT的业务描述段：
##### 业务描述段Syntax（句法结构）         No. ofbits（所占位数）   Identifier(识别符)  Note(注释)service_description_section(){table_id                                            8                      uimsbf                                                  Section_syntax_indicator               1                          bslbf                   通常设为“1”Reserved_future_use                      1                        bslbfReserved                                         2                        bslbfSection_length                                12                      uimsbf                 见注释transport_stream_id                       16                       uimsbf                给出TS识别号Reserved                                         2                         bslbfVersion_number                              5                         uimsbf             见注释Current_next_indicator                   1                          bslbf                 见注释Section_number                              8                          uimsbf            见注释last_section_number                       8                          uimsbf            见注释original_nerwork_id                        16                         uimsbf            见注释reserved_future_use                        8                          bslbffor(i=0;i<N;i++){    service_id                                     16                         uimsbf           见注释    reserved_future_use                     6                         bslbf    EIT_schedule_flag                         1                          bslbf            见注释    EIT_present_following_flag          1                          bslbf            见注释  running_status                                3                         uimsbf        见下面分析    freed_CA_mode                            1                         bslbf            见注释    descriptors_loop_length                12                      uimsbf    for(j=0;j<N;j++){  descriptor()    }}CRC_32                                               32                  rpchof                  见注释}
***running_status***（运行状态）：表示业务状态，下表给出了业务状态
|值|含义|
|----|----|
|0|未定义|
|1|未运行|
|2|几秒后开始（如录像）|
|3|暂停|
|4|运行|
|5~7|预留|
         SDT是描述一个TS流中所有业务信息的一张表，重要的字段包含transport_stream_id，明确这些业务是属于哪个TS流的；另一个重要字段时候service_id，这是作为频道索引信息存在的。
SDT的业务描述符分析
ANALYZEOFTHE STRUCTUREOFSERVICE_DESCRIPTORINSDT
业务描述符与业务类型一起，以文本形式给出业务提供者的名称和业务名称，如下表：
**业务描述符**
Syntax（句法结构）    No. of bits（所占位数）Identifier(识别符)
service_descriptor(){
descriptor_tag                       8                       uimsbf
descriptor_length                   8                       uimsbf
service_type                          8                      uimsbf
service_provider_name_length  8                    uimsbf
for(i=0;i<N;i++){
    Char                                 8                      uimsbf
}
service_name_length            8                      uimsbf
for(i=0;i<N;i++){
    Char                                8                      uimsbf
}
}
***service_type***（业务类型）：这里的业务类型和NIT中的业务列表描述符中的service_type编码是一致的：
|service_type|描述|
|----|----|
|0x00|预留使用|
|0x01|数字电视业务|
|0x02|数字音频广播业务|
|0x03|图文电视业务|
|0x04|NVOD参考业务|
|0x05|NVOD时移业务|
|0x06|马赛克业务|
|0x07|PAL制编码信号|
|0x08|SECAM制编码信号|
|0x09|D/D2-MAC|
|0x0A|调频广播|
|0x0B|NTSC制信号|
|0x0C|数据广播业务|
|0x0D|公共接口使用预留|
|0x0E|RCS映射（见EN 301 790）|
|0x0F|RCS FLS（见EN 301 790）|
|0x10|DVB  MHP业务|
|0x11~0x7F|预留使用|
|0x80~0xFE|用户定义|
|0xFF|预留使用|
业务描述符是SDT中最重要的描述符，也是运营商中必须的描述符。其中的service_type描述这个业务类型的域；service_name描述频道名。
SDT的实例分析
ANALYZEOF REAL EXAMPLE OF SDT
这里主要演示SDT如何连接PSI和NIT。
![](https://img-blog.csdn.net/20131024110858625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2tkZXN0aW55/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
首先，根据SDT描述符的service_id可以和PMT中的program_number建立连接。如图所示，SDT的service_id=201=0xc9，与PMT中的program_number一致。
![](https://img-blog.csdn.net/20131024110855031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2tkZXN0aW55/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
上图所示，根据SDT的transport_stream_id和NIT的transport_stream_id，可以将SDT和NIT联系起来。根据上图的信息，我们可以根据SDT获取该业务（频道）的业务列表信息和有线传输信息（位于NIT中）。
***EIT*****解析**
PARSINGOF EIT
"EIT按时间顺序提供每一个业务所包含的事件信息"
        EIT即事件信息表（Event Information Table），它是EPG中绝大部分信息的携带者。事实上，EPG主要就是通过SDT和EIT信息的获取和重组实现的。SDT只提供了频道信息，而EIT则提供各频道下的所有节目的信息。
         EIT的主要信息也是通过插入的描述符来实现的。EIT按照时间顺序提供每一个业务所包含的事件的相关信息（如节目名称、节目简介）。
|传输流|信息|table_id|
|----|----|----|
|当前TS流|当前/后续事件信息|0x4E|
|其他TS流|当前/后续事件信息|0x4F|
|当前TS流|事件时间表信息|0x50~0x5F|
|其他TS流|事件时间表信息|0x60~0x6F|
按照不同的table_id，一共有四类EIT。其中，当前TS流的所有EIT子表都有相同的transport_stream_id和original_stream_id。下图给出了一个实例的码流中的EIT信息，这里一共有三类EIT表，观察其table_id就可以知道它们代表什么类型的EIT。
![](https://img-blog.csdn.net/20131024110956375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2tkZXN0aW55/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
第一个EIT_PF_Actual应该是当前TS流的“当前/后续事件信息”；第二个EIT_PF_Other是其他TS流的“当前/后续事件信息”；第三个EIT_Schedule_Actual是当前TS流的“事件时间表信息”。
EIT的表结构分析
ANALYZEOF THE STRUCTUREOF EIT
         EIT表被切分成事件信息段。任何构成EIT的段都由PID=0x0012的TS包传输。下面给出的是事件信息段的结构：
事件信息段
Syntax（句法结构）         No. ofbits（所占位数）   Identifier(识别符)  Note(注释)
event_information_section(){
table_id                                            8                      uimsbf                                                  
Section_syntax_indicator               1                          bslbf                   通常设为“1”
Reserved_future_use                      1                        bslbf
Reserved                                         2                        bslbf
Section_length                                12                      uimsbf                 见注释
service_id                                        16                       uimsbf                与PAT中的program_number一致
Reserved                                         2                         bslbf
Version_number                              5                         uimsbf             见注释
Current_next_indicator                   1                          bslbf                 见注释
Section_number                              8                          uimsbf            见注释
last_section_number                       8                          uimsbf            见注释
transport_stream_id                       16                         uimsbf            见注释
original_nerwork_id                        16                         uimsbf            见注释
segment_last_section_number     8                           uimsbf            见注释
last_table_id                                     8                           uimsbf            见注释
for(i=0;i<N;i++){
 event_id                                        16                         uimsbf          事件（节目）id
 start_time                                      40                         bslbf           事件（节目）开始时间
 duration                                        24                         bslbf           事件（节目）持续始时间
 running_status                                3                         uimsbf         见注释
    freed_CA_mode                            1                         bslbf            见注释
    descriptors_loop_length                12                      uimsbf
    for(j=0;j<N;j++){
        descriptor()
    }
}
CRC_32                                               32                  rpchof                  见注释
}
***start_time***（开始时间）：40位字段，指的是事件（节目）的开始时间，它包含以UTC和MJD形式表示的事件（节目）的起始时间及日期。此字段前16位表示MJD日期码，其余24位按4位BCD编码，表示6个数字。例如：93/10/13 12:45:00被编码为：“0xc079124500”
HINT：如果事件起始时间未定，则所有位都置为“1”，例如对NVOD业务的一个事件。
***duration***（持续时间）：24位字段，表示事件的持续时间，以时、分、秒的格式表示，即由6个4位BCD码显示。如：01:45:30被编码为“0x14530”。
***running_status***（运行状态）：参看SDT表解析部分。
下面给出一个具体的例子：
![](https://img-blog.csdn.net/20131024111159312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2tkZXN0aW55/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
   EIT主要承载的信息就是Event的相关内容，即节目信息。其中，节目所在频道（service_id）、开始时间（start_time）、结束时间（start_time+duration）可在EIT子表获取；而节目描述、节目名称、节目分类等信息就要在插入EIT的描述符中获取了。
![](https://img-blog.csdn.net/20131024111206093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2tkZXN0aW55/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
EIT的短事件描述符分析
ANALYZEOFTHE STRUCTUREOFSHORT_EVENT_DESCRIPTORINEIT
短事件描述符（Short_event_descriptor）以文本方式提供了事件名称和该事件的简短描述。描述符结构如下：
短事件描述符
Syntax（句法结构）    No. of bits（所占位数）Identifier(识别符)
short_event_descriptor(){
descriptor_tag                       8                       uimsbf
descriptor_length                   8                       uimsbf
ISO_639_language_code      24                      bslbf
event_name_length               8                       uimsbf
for(i=0;i<event_name_length;i++){
   event_name_char             8                       uimsbf
}
text_length                          8                       uimsbf
for(i=0;i<text_length;i++){
 text_char                        8                       uimsbf
}
}
***ISO_639_language_code***（ISO 639-2语言代码）：24位字段，指明后续文本字段的语言。该字段包含一个由ISO 639-2定义的3字符代码。ISO 639-2/B和ISO 639-2/T均可使用。每个字符按照GB/T15273.1-1994编码为8位，并依次插入24位字段。如：法语的3字符代码“fre”，可编码为：“0110 01100111 0010 0110 0101”。
***event_name_char***（事件名称字符）：一个字符串给出事件的名字。
***text_char***（文本字符）：一个字符串给出事件的文本描述。
下面给出的是一个实际案例的短时间描述符，可见中文的3字符代码是“chi”。
![](https://img-blog.csdn.net/20131024111338484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2tkZXN0aW55/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
EIT的扩展事件描述符分析
ANALYZEOFTHE STRUCTUREOFEXTENDED_EVENT_DESCRIPTORINEIT
扩展事件描述符（Extended_Event_Descriptor）拓展事件描述符给出了一个事件的详细文本描述。如果一个事件的信息长度超过256字节，可以使用多于一个相关联的扩展事件描述符来描述。文本信息可以分为两个栏目，一栏为条目的描述，另一栏为条目的内容。
下面给出了扩展事件描述符的结构。
扩展事件描述符
Syntax（句法结构）    No. of bits（所占位数）Identifier(识别符)
extended_event_descriptor(){
descriptor_tag                         8           uimsbf        
descriptor_length                 8           uimsbf   
descriptor_number                      4             uimsbf
last_descriptor_number                4             uimsbf
ISO_639_language_code           24           bslbf 
length_of_items                            8             uimsbf
for(i=0;i<N;i++){
    item_description_length          8               uimsbf
    for(j=0;j<N;j++){
  item_description_char       8                uimsbf
    }
    item_length
    for(j=0;j<N;j++){
 item_char
    }
}   
text_length
for(i-0;i<N;i++){
 text_char
}             
}
下面给出的是一个实例截图：
![](https://img-blog.csdn.net/20131024111408625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2tkZXN0aW55/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
EIT的内容描述符分析
ANALYZEOFTHE STRUCTUREOFCONTENT_DESCRIPTORINEIT
内容描述符的目的是为事件提供清晰的信息描述符。根据这个描述符的信息，接收机可以清晰地知道事件的分类，并告知观众。下面给出了内容描述符的结构：
内容描述符
Syntax（句法结构）    No. of bits（所占位数）Identifier(识别符)
content_descriptor(){
descriptor_tag                         8           uimsbf     
descriptor_length                 8           uimsbf                        
for(i=0;i<N;i++){
 content_nibble_level1       4              uimsbf      
content_nibble_level2      4              uimsbf      
    user_nibble                         4             uimsbf      
    user_nibble                         4             uimsbf      
}
}
这里主要需要介绍的就是*** content_nibble_level1***和***content_nibble_level2***：根据EN 300 468V1.3.1(1998-02)中的Table 18可以确定该节目的具体分类。
下面给出了一个实例。
![](https://img-blog.csdn.net/20131024111510484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2tkZXN0aW55/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

***TDT*****解析**
PARSINGOF TDT
*"**TDT**仅传送**UTC**时间和日期信息，只有一个段**"*
TDT为时间和日期表（Time&Date Table），它仅传送UTC时间和日期信息。并且TDT仅包含一个段，其结构如下：
时间和日期段
Syntax（句法结构）         No. ofbits（所占位数）   Identifier(识别符)  Note(注释)
time_date_section(){
table_id                                            8                      uimsbf                                                  
Section_syntax_indicator               1                          bslbf             通常设为“1”
Reserved_future_use                      1                        bslbf
Reserved                                         2                        bslbf
Section_length                                12                      uimsbf                 见注释
UTC_time                                           40                      bslbf                     见注释
}
***UTC_time***（UTC时间）：40位字段，包含以UTC和MJD形式表示的当前时间和日期。此字段前16位为MJD日期码，后24位按4位BCD编码，表示6个数字。如：93/10/13 12:45:00被编码为“0xC079124500”。
下图给出了一个实例：
![](https://img-blog.csdn.net/20131024111558953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2tkZXN0aW55/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
***TOT*****解析**
PARSINGOF TOT
"TOT是TDT的一个扩展，增加了一个描述符"
        TOT为时间偏移表（Time Offset Table），它包含了UTC时间和日期信息及当地时间偏移。传输此表的TS包PID为0x0014，table_id=0x73。下面给出了时间偏移段的结构：
时间偏移段
Syntax（句法结构）         No. ofbits（所占位数）   Identifier(识别符)  Note(注释)
time_offset_section(){
table_id                                            8                      uimsbf                                                  
section_syntax_indicator               1                          bslbf         通常设为“1”
reserved_future_use                      1                        bslbf
reserved                                         2                        bslbf
section_length                                12                      uimsbf                 见注释
UTC_time                                           40                      bslbf                     见注释
reserved                                         4                         bslbf
descriptors_loop_length                12                  uimsbf              
for(i=0;i<N;i++){
 descriptor()
}
CRC_32                                         32                  rpchof                  见注释
}
这里的UTC_time和TDT表是一致的，都是以UTC和MJD形式表示当前时间和日期；其格式也与TDT的UTC_time相同，这里不再赘述。需要特别注意的是这里的描述符descriptor()。下面给出了本地时间偏移描述符的结构：
本地时间偏移描述符
Syntax（句法结构）    No. of bits（所占位数）Identifier(识别符)
local_time_offset_descriptor(){
descriptor_tag                         8           uimsbf        
descriptor_length                 8           uimsbf   
for(i=0;i<N;i++){
    coutry_code                           24             bslbf
    country_region_id                    6             bslbf
    reserved                                  1             bslbf
    local_time_offset_polarity       1             bslbf
    local_time_offset                    16            bslbf
    time_of_change                      40            bslbf
    next_time_offset                     16            bslbf
}
}
国家代码  country_code
24位字段，按照ISO 3166用3字符代码指明国家。每个字符根据GB/T 15273.1-1994编码为8位，并依次插入24位字段。假设3个字符代表了一个900至999的数字，那么country_code指定了一组ETSI定义的国家。其分配见ETR 162。国家组的国家代码应该被限制在同一时区内。
例如：英国由3字符代码“GBR”表示，编码为：“01000111 0100 0010 0101 0010”。
国家区域标识符  country_region_id
6位字段，表示country_code指明的国家所在的时区。若国家内部里没有时差，则置“000000”。
|country_region_id|描述|
|----|----|
|00 0000|未使用时区扩展|
|00 0001|时区1（最东部）|
|00 0010|时区2|
|……..|……..|
|11 1100|时区 60|
|11 1101 – 11 1111|预留|
本地时间偏移极性 local_time_offset_polarity
1位字段，用于指明随后的local_time_offset的极性。置“0”时，极性为正，说明本地时间早于UTC时间（通常在格林威治以东）；置“1”时，极性为负，说明本地时间晚于UTC时间。
本地时间偏移 local_time_offset
16位字段，指出由country_code和country_region_id确定的区域的相对于UTC的时间偏移，范围为-12小时至+13小时。16比特含有4个4位BCD码，顺序为小时的十位，小时的个位，分的十位，分的个位。
时间变化  time_of_change
40位字段，指明时间改变时当前的日期（MJD）与时间（UTC），见附录C。该字段分为两部分，前16位给出了LSB格式的日期（MJD），后24位给出了UTC时间（6个4位BCD码）。
下一时间偏移  next_time_offset
16位字段，指出由country_code和country_region_id确定的区域，当UTC时间变化时的下一个时间偏移，范围为-12小时至+13小时。此16比特域为4个4位BCD码，依次为时的十位，时的个位，分的十位，分的个位。
***SI*****小结**
SUMMARYOF SI
![](https://img-blog.csdn.net/20131024111700046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2tkZXN0aW55/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
