# PSI/SI深入学习2——PSI信息解析(PAT,PMT,CAT) - 建建的博客 - CSDN博客
2018年03月14日 12:17:34[纪建](https://me.csdn.net/u013898698)阅读数：47
个人分类：[多媒体封装格式详解](https://blog.csdn.net/u013898698/article/category/7105689)
***转自：http://blog.csdn.net/kkdestiny/article/details/12994085***
***PSI******/******SI*****深入学习**
BASIC KNOWLEDGE LEARNING OF PSI/SI 
***PSI*****信息**
INFORMATION OF PSI
*"**PSI**是对单一**TS**流的描述，是**TS**流中的引导信息**"*
         PSI信息由节目关联表PAT、条件接收表CAT、节目映射表PMT和网络信息表NIT组成。这些表会被插入到TS流中。**PSI****信息是对单一****TS****流的描述，它是****TS****流的引导信息**；PSI信息指定了如何从一个携带多个节目的传输流中找到指定的节目（见Q1）。
下面给出的是节目引导信息（或称节目特定信息，PSI）的四个表结构。
|结构名|中文|所定义标准|PID|描述|
|----|----|----|----|----|
|PAT|节目关联表|MPEG2标准|0x0000|将节目号码和节目映射表PID相关联，是获取数据的开始|
|PMT|节目映射表|MPEG2标准|在PAT中指出|指定一个或多个节目的PID|
|CAT|条件接收表|MPEG2标准|0x0001|将一个或多个专用EMM流分别与唯一的PID相关联|
|NIT|网络信息表|SI标准|PAT中指出|描述整个网络，如多少个TS流、频点和调制方式等信息|
虽然这四个结构可以视为简单的表，但事实上，它们被进一步划分为各个段，并插入到TS包中。
***PAT*****解析**
PARSINGOF PAT
*"**P**AT**是机顶盒接收的入口点，是它获取数据的开始**"*
节目关联表PAT的意义在于，它描述了当前TS流中包含了哪些PID；只有根据获得的PID，用户才可以以此作为凭据找出其他表（如PMT表）及其信息。所以**PAT****是机顶盒接收的入口点，是它获取数据的开始**；要保证一个TS流能被正常接收，则至少要有一个完整有效的PAT。
下面给出了**PID**字段的取值要求：
|值|描述|
|----|----|
|0x0000|PAT|
|0x0001|CAT|
|0x0002~0x000F|保留|
|0x0010~0x1FFE|可赋给network_PID、Program_map_PID、elementary_PID或作其他用途|
|0x1FFF|空的分组|
PAT的表结构分析
ANALYZEOF THE STRUCTUREOF PAT
首先，对PAT的表结构进行分析。下面给出了PAT表的基本结构：
节目关联段
Syntax（句法结构）         No. ofbits（所占位数）   Identifier(识别符)  Note(注释)
program_association_section(){
table_id                                            8                      uimsbf                                                  
Section_syntax_indicator               1                          bslbf                   通常设为“1”
"0"                                                     1                        bslbf
Reserved                                         2                        bslbf
Section_length                                12                      uimsbf                 见注释
transport_stream_id                        16                       uimsbf            见注释
Reserved                                         2                         bslbf
Version_number                              5                         uimsbf             见注释
Current_next_indicator                   1                          bslbf                 见注释
Section_number                              8                          uimsbf            见注释
last_section_number                       8                          uimsbf            见注释
for(i=0;i<N;i++){
  program_number                         16                         uimsbf        见注释
    reserved                                         3                          bslbf
    if(program_number == 0){
network_PID                             13                         uimsbf        见注释
    }
    else{
        program_map_PID                 13                         uimsbf        见注释
    }
}
CRC_32                                         32                  rpchof                  见注释
}
这里我们注意关注五个字段：
**table_id（赋值）：**PAT的table_id应为0x00
**transport_stream_id（传输流标志）：**用以标识来源于网络中任何其他复合流的TS流
**program_number（节目号）：**规定program_map_PID可适用的节目。当值为0x0000时，其后的PID参照将是网络PID。它可以作为一个指示符号，例如用于广播通道。
**network_PID（网络****PID****）：**仅当program_number为0x00时使用
**program_map_PID（节目映射****PID****）：**据此找出相应的PMT表
PAT的实例分析
ANALYZEOF REAL EXAMPLE OF PAT
借助码流分析仪，我们可以很清楚地以实例的方式更加清晰地了解PAT的结构。下面给出的就是一个TS流的PAT分析情况：
![](https://img-blog.csdn.net/20131024102836609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2tkZXN0aW55/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
根据上表可以看出，这个TS流里共有6套节目，其映射关系PID分别为0xc9、0xcb、0xce、0xcc、0xcd和0xca。（注：0xc9=201）
下面给出的是PMT的简单解析：
![](https://img-blog.csdn.net/20131024102831953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2tkZXN0aW55/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
两相比较，可以很清楚地知道，PAT是如何锁定各个PMT的（每个节目都有一个自己的PMT）。
Q1：如何找到一个特定的节目？
1.已知节目名称（或id等信息）
P248
首先要从节目关联表PAT中获取这个节目的节目映射表PMT的PID值，再从TS流中找出与此PID值相对应的节目映射表PMT；再从PMT中获取构成这个节目的基本码流的PID值，从而根据该PID过滤出相应的视频、音频和数据等基本码流。最后解码恢复为原始信号。
***PMT*****解析**
PARSINGOF PMT
*"**PMT**是连接节目号与节目元素的桥梁**"*
节目映射表PMT的意义在于，它给出了节目号与组成这个节目元素之间的映射；也就是说，**PMT****是连接节目号与节目元素的桥梁**。我们知道，一个电视节目至少包含了视频和音频数据，而每一个节目的视音频数据都是以包的形式在TS流中传输的；所以说，一个TS流包含了多个节目的视频和音频数据包。要想过滤出一个TS流中其中一个节目的视频和音频，则需要知道这个节目中视频和音频的标识号PID。**PMT****表的作用就在于，它提供了每个节目视频、音频（或其他）数据包的****P**ID。
PMT的表结构分析
ANALYZEOF THE STRUCTUREOF PMT
先给出PMT表的段结构：
##### Syntax （句法结构）         No. of bits（所占位数）   Identifier(识别符)  Note(注释)program_map_section(){table_id                                               8                       uimsbf                                                  Section_syntax_indicator                 1                          bslbf                   通常设为“1”"0"                                                        1                        bslbfReserved                                              2                        bslbfSection_length                                   12                      uimsbf                 见注释program_number                              16                       uimsbf            Reserved                                              2                         bslbfVersion_number                                5                         uimsbf             见注释Current_next_indicator                    1                          bslbf                 见注释Section_number                                8                          uimsbf            见注释last_section_number                        8                          uimsbf            见注释reserved                                              3                          bslbfPCR_PID                                             13                         uimsbf           见注释reserved                                              4                          bslbfprogram_info_length                       12                        uimsbf            头两位为"00"for(i=0;i<N;i++){    descriptor()}for(i=0;i<N1;i++){ stream_type                                8                          uimsbf        见注释    reserved                                       3                           bslbfelementary_PID                         13                        uimsbf        见注释    reserved                                       4                           bslbf   ES_info_length                          12                        uimsbf             头两位为"00"    for(j=0;j<N2;j++){        descriptor();    }}CRC_32                                         32                  rpchof                  见注释}
这里着重介绍两个字段：
elementary_PID：标识了该节目（program_number）携带的基本流的识别号PID
stream_type：流类型，指出了PID为elementary_PID的基本流的类型
附：流类型取值说明
|取值|描述|
|----|----|
|0x00|国际标准保留|
|0x01|视频|
|0x02|视频或受限参数视频流|
|0x03|音频|
|0x04|音频|
|0x05|private_sections|
|0x06|包含专用数据的PES分组|
|0x07|ISO/IEC 13533 MHEG|
|0x08||
|0x09|ITU-T Rec.H.222.1|
|0x0A~0x0D|GB/T类型|
|0x0E|GB/T辅助|
|0x0F~0x7F|GB/T保留|
|0x80~0xFF|用户专用|
PMT的实例分析
ANALYZEOF REAL EXAMPLE OF PMT
下面给出了码流分析仪分析一段TS流的PMT表：
![](https://img-blog.csdn.net/20131024103102625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2tkZXN0aW55/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
首先，注意到这张PMT表的progam_number=0xc9，就是我们在上面解析PAT表得到的。
从上面可以知道，节目号为0xc9（即201）的节目有两个单元流：
|流名称|标识号|类型|说明|
|----|----|----|----|
|MPEG2视频|ES PID=0x21|stream_type=0x02|ES PID即为elementary_PID|
|MPEG1音频|ES PID=0x28|stream_type=0x03|ES PID即为elementary_PID|
 elementary_PID和stream_type的意义在于，告知接收机基本流的类型和识别PID。要想获取这个节目的视频，只需要对PID为0x21的数据包进行过滤，并交由视频处理模块处理。下图所示（除了节目名称，还需要SDT表信息）即为PAT和PMT共同获取的信息。
![](https://img-blog.csdn.net/20131024103218328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2tkZXN0aW55/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
***CAT*****解析**
PARSINGOF CAT
*"**CAT**描述了节目的加密方式**"*
条件接收表CAT描述了节目的加密方式，它包含了节目的**EMM****注****1**识别PID。它给出了一个或多个CA系统、EMM流以及与CA相关的特定参数之间的关系。
**注****1**：EMM（Entitlement Management Message），即授权管理信息。描述了用户可看的节目或时间段、用户的付费等。
         CA描述符既用于规定像EMM这样的系统范围条件接收管理信息，也用于规定像ECM这样的基本流特定信息。
- 如果一个基本流（Elementary Stream）是加扰的，那么包含该基本流的节目信息PMT中需要一个CA描述符
- 如果一个TS流中有任何一个系统范围的条件接收管理信息，则条件接收表中应有CA描述符。
下图给出的PMT表即是基本流加扰的情况，因此PMT表需要有CA描述符：
![](https://img-blog.csdn.net/20131024103209171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2tkZXN0aW55/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
CAT的表结构分析
ANALYZEOF THE STRUCTUREOF CAT
条件存取段
### Syntax（句法结构）         No. ofbits（所占位数）   Identifier(识别符)  Note(注释)conditional_access_section(){table_id                                            8                      uimsbf                                                  Section_syntax_indicator               1                          bslbf                   通常设为“1”"0"                                                     1                        bslbfReserved                                         2                        bslbfSection_length                                12                      uimsbf                 见注释reserved                                          18                       bslbfVersion_number                              5                         uimsbf             见注释Current_next_indicator                   1                          bslbf                 见注释Section_number                              8                          uimsbf            见注释last_section_number                       8                          uimsbf            见注释for(i=0;i<N;i++){ descriptor()}CRC_32                                         32                  rpchof                  见注释}
条件存取段主要作用是指示本TS流中存在加扰节目，并且在描述符循环中必须插入CA描述符来作进一步指引。下面给出的是条件接收的描述符
条件接收描述符
##### Syntax（句法结构）    No. of bits（所占位数）Identifier(识别符)CA_identifier_descriptor(){descriptor_tag                         8                                          uimsbf                       descriptor_length                     8                                         uimsbf        CA_system_ID                          16                                       uimsbfreserved                                     3                                         bslbfCA_PID                                    13                                       uimsbf               for(i=0;i<N;i++){private_data_byte                    8}}
**CA_system_****ID**（CA系统ID）：指出用于相关ECM及（或）EMM流的CA系统类型。
**CA_PID**（CA PID）：指出包含用于相关CA_system_ID所规定的CA系统的ECM或EMM信息传输流分组的PID（即在TS流中过滤出相应PID的分组）。
下面给出的TS流中并没有CAT：
![](https://img-blog.csdn.net/20131024103454546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2tkZXN0aW55/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
但在PMT表中有CA描述符：
![](https://img-blog.csdn.net/20131024103522437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2tkZXN0aW55/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
***PSI*****小结**
SUMMARYOF PSI
![](https://img-blog.csdn.net/20131024103515140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2tkZXN0aW55/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
上图给出了在一个TS流中PSI各表之间的联系。首先从PAT表出发，获取当前有哪些节目号（program_map_PID），再根据这些节目号找出program_number相同的PMT；获取到PMT后，即可根据elementary_PID和stream_type来确定要过滤哪些含有基本流的TS包，以及这些包里面是什么类型的数据。这样，机顶盒就可以过滤出相应的节目（视频和音频数据）来收看了。
