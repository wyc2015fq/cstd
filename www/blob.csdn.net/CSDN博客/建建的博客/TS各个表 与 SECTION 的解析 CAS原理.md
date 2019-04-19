# TS各个表 与 SECTION 的解析 CAS原理 - 建建的博客 - CSDN博客
2018年03月12日 16:20:04[纪建](https://me.csdn.net/u013898698)阅读数：57
个人分类：[多媒体封装格式详解](https://blog.csdn.net/u013898698/article/category/7105689)

TS流，通过一个个的TS包来传送；TS包可以是传送PSI SI等各表的数据包，也可以是传送节目音视频数据(携带的PES包：音视频基本流包)的包；TS携带 PSISI等表的数据时，各个表以各表对应的Section语法格式做为传输单元存放到TS包中 以便传输；
TS包，有一个TS包的PID，系统就是根据这个PID来找对应的TS包；对于包含音视频数据（PES包）的TS包，系统通过TS的PID找到对应TS数据包，提取其中的数据组合成节目的音视频；对于携带PSISI等数据的TS包，系统通过TS的PID找到对应TS数据包，提取各个PSI SI数据表格，用来指导系统；
   因此其中部分PID用来固定传输某些数据内容；如下
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s11.sinaimg.cn/bmiddle/6b94d568gx6DdPN48VI6a&690)
    有了TS的PID后， 如果TS包携带的是PSISI等表格的Section数据时，有时还不能确定该PID的TS包中携带的数据是什么，SDT BAT ST等表传送时，都用的是PID为0X0011的TS数据包，对于这种携带PSI SISection单元的TS包，对应的数据(表的Section语法中)还有一个TABLE_ID字段，用来可以确定是具体的什么表，如下图
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s12.sinaimg.cn/bmiddle/6b94d568gx6DdQEYiMPfb&690)
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s6.sinaimg.cn/bmiddle/6b94d568gx6DdQFnNNrd5&690)
每不同的表中，我们看表的SECTION语法，都会看到descriptor()字样，descriptor()表示的也是一个语法结构，他具体对应的语法结构，由他内部的descriptortag字段决定，各个表的具体的定义，可以参见13818对应的定义，其中这里面还可以有用户的自定义描述符，可见下表：
the descriptors declared or defined within the present document,giving the descriptors-tag values andthe intendedplacement within the SI tables. This does not imply that their usein other tables is restricted. PSI SI表中包含什么descriptor，就用对应的语法去解！当然用户自定义的由用户自定义去解！！！
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s3.sinaimg.cn/mw690/6b94d568gx6Dh0jL7oe72&690)
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s12.sinaimg.cn/mw690/6b94d568gx6Dh0phSvV5b&690)
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s16.sinaimg.cn/mw690/6b94d568gx6Dh0qZ2ZF6f&690)
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s3.sinaimg.cn/mw690/6b94d568gx6Dh0sgidk82&690)
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s15.sinaimg.cn/mw690/6b94d568gx6Dh0sZnLU8e&690)
•PSI(ProgramSpecific Information)
–PAT (Program AssociationTable)节目关联表
–PMT(Program MapTable)节目映射表
–CAT(Conditional AccessTable)条件接收表
•SI(ServiceInformation) 
–NIT(Network InformationTable)网络信息表
–SDT(ServiceDiscriptionTable)业务描述信息表
–BAT(Bouquet AssociationTable)业务群信息表
–EIT(Event InformationTable)节目事件信息表
–TDT(Time and Data Table)日期时间表
•事件 event
–一组给定了起始时间和结束时间、属于同一业务的基本广播数据流。例如：一场足球比赛的半场、新闻快报或娱乐表演的第一部分
•节目 programme
–由广播者提供的一个或多个连续的事件。例如：新闻广播，娱乐广播。
•网络 network
一个传输系统，可以传输一组MPEG-2传输流（TS）。例如：某个有线电视系统中的所有数字频道
•业务 service
–在广播者的控制下，可以按照时间表分步广播的一系列节目,我们也称之个频道，口语中也称之为节目
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s16.sinaimg.cn/mw690/6b94d568tx6CRoYq8ndef&690)
节目关联表PAT
•PAT定义了一个TS流中所有的节目，PAT的PID是0x0000，他是PSI信息的根节点。要查找节目播放信息必须从PAT开始。
•PAT中包含了TS中所有节目的完整列表，每个表项包括ServiceID和PMT(用于播放)的PID。
•**PAT中ServiceID 为0的表项表示NIT。**
节目映射表PMT
• PMT提供了一个节目的ServiceID和用于播放的所有资源信息，例如音视频PID，PCR，字幕PID等。
• 简单的说，PMT完整的描述了一路节目是由哪些PES组成，给播放提供相应的资源。
•  PAT与PMT关系
条件访问表CAT
•CAT提供了在一个或多个CA系统及其授权管理信息，用于节目的解扰工作。
•如果在一个TS中任何原始流进行了加密处理，那么在TS中一定要插入CAT。
•在CAT中，最重要的字段就是CA_descriptor()段。下面将简单讲述一下该描述段
•CA_descriptor用来表示含有ECM或者是EMM信息的TS的PID，即CA_PID。
–当CA_descriptor出现在PMT中时，CA_PID指向含有与访问控制信息（ECM）相关的节目包。
–当CA_descriptor出现在CAT中时，CA_PID指向含有与授权管理信息（EMM）相关的节目包。
•CA_descriptor中几个重要的字段如下：
–CA_system_ID：该字段表示适用于相关ECM和/或EMM流的CA系统类型。其值是用户定义的。
–CA_PID：该字段表示传送流包的PID，此包中含有由相应的CA_system_ID指明的CA系统的ECM或EMM信息，由CA_PID指明的包的内容（ECM或EMM）所在的上下文决定，既由TS中的PMT、CAT或节目流中的stream_id字段决定。
NIT
•NIT描述了一个DVB传输通道的所有物理参数，包括下列信息：
–传输路径（卫星、电缆、地面）
–接收频率
–调制类型
–误码保护
–传输参数
•机顶盒在扫描或变换信道时，可以存储一个物理信道的所有参数，便于以后很快跳回该信道。
•信道中也可以传送相邻或其他信道的传输参数，使得信道转换灵活快捷。
•如果NIT中的传输参数与实际不符，会对许多接收设备，如机顶盒，产生不可预知的影响。
–如果NIT中的传输频率与实际接收频率不同，许多接收设备在没有任何原因提示的情况下，不产生任何图像和声音。
SDT
•SDT包含对TS流中节目（服务）的更多详细描述：
–节目名称，如CNN,CBS,Eurosport,ARD,ZDF,BBC,ACB,SBS等等
–在提供节目PID的同时，SDT对用户提供了文本信息。
–通过提供文本列表，使得接收设备操作灵活。
•BAT，与SDT密切相关：
–BAT与SDT的PID相同，只是tableID不同。
–SDT描述一个物理信道的节目结构。
–BAT描述几个或大量物理信道的节目结构。
BAT
•BAT表是由DVB定义的，是一个SI表，因此它是一个全局表，一个数字电视系统只对应一个BAT表，其table_id=0X4A。
•一个节目类别对应一个段。为了让受众能更方便地在众多的节目中寻找出自己喜欢的节目，往往需要提供一种把众多的节目频道进行分类的方法（一个类相当一个节目组）。
•例如把电影频道归为“家庭影院”的类别，把电视连续剧归为“电视剧场”的类别等等，BAT表就提供了这一功能，每一个类别都用一个bouquet_id来标识。它包括了节目业务名称（类别）及节目组所包含的节目清单（节目列表）。BAT表在SI信息中属于可选表
EIT
•EIT(eventinformation table)
–即DVB中的EPG(electronicprogram guide)表
–包含一天或一周内所有广播的计划开始和结束时间。
–结构非常灵活，允许传送大量附加信息
–不是所有机顶盒都支持这一特性
– 事件信息表EIT按时间顺序提供每一个业务所包含的事件的信息。按照不同table_id有四类EIT：
–1） 现行传输流，当前/后续事件信息=table_id="0x4E"；
–2） 其它传输流，当前/后续事件信息=table_id="0x4F"；
–3） 现行传输流，事件时间表信息=table_id="0x50"至"0x5F"；
–4） 其它传输流，事件时间表信息=table_id="0x60"至"0x6F"。
–现行传输流的所有EIT子表都有相同的transport_stream_id和original_network_id。
TDT/TOT
•机顶盒操作还需要传输当前时钟和当前日期，分两步：
–TDT(time&datetable)
•传送GMT或UTC
•即零度子午线的当前时刻
–TOT(timeoffset table)
•传送不同时区各自适当的时间偏移量
–TDT和TOT中的信息如何计算以及计算到什么程度，取决于机顶盒的软件
–对广播时间信息的完全支持还需要机顶盒得到当前的位置信息：
•对拥有多个时区的国家，如澳大利亚和美国，这个问题尤其重要。
其它表
•运行状态表(RST)：
–运行状态表给出了事件的状态（运行/非运行）。运行状态表更新这些信息，允许自动适时切换事件。
•填充表(ST)：
–填充表用于使现有的段无效，例如在一个传输系统的边界。
•选择信息表(SIT)：
–选择信息表仅用于码流片段（例如，记录的一段码流）中，它包含了描述该码流片段的业务信息的概要数据。
•间断信息表(DIT)：
–间断信息表仅用于码流片段（例如，记录的一段码流）中，它将插入到码流片段业务信息间断的地方。
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s8.sinaimg.cn/mw690/6b94d568tx6CRqgBVmD37&690)
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s11.sinaimg.cn/mw690/6b94d568tx6CRqi9ULM4a&690)
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s10.sinaimg.cn/mw690/6b94d568tx6CRql6RWV39&690)
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s4.sinaimg.cn/mw690/6b94d568tx6CRqFkql573&690)
================================
TS流的形成：前段音视频数据经过音视频编码器后音视频数据流ES，ES经过分组器（打包器）形成一个个的分组，即PES（音视频数据流ES的分组包，PacketES,PES最长一般为188个字节）；音视频PES再经过复合器，从而形成传输流TS，传输流以传输流分组（TS Packet）；TSPacket中的有效数据既可以是PES（音视频ES分组包），也可以是PSI等信息数据，这个由TSPacket中的PID来指定负荷数据的类型；SI各表格是以SECTION为单位放到TSPacket中，因此不同的表格就要按标准遵循对应SECTION的语法；比如PMT表，PMT可能包含多个节目的描述，因此它可能会被划分为多个SECTION放到TSPacket中，遵循的语法就是TS_program_map_section() ;
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s10.sinaimg.cn/mw690/6b94d568tx6CJpzn52N59&690)
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s6.sinaimg.cn/mw690/6b94d568tx6CJqJDe9D25&690)
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s7.sinaimg.cn/mw690/6b94d568tx6CJsauN2Se6&690)
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s5.sinaimg.cn/mw690/6b94d568tx6CJsS0mEs44&690)
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s1.sinaimg.cn/mw690/6b94d568tx6CJra58pqa0&690)
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s6.sinaimg.cn/mw690/6b94d568tx6CJrL5sABf5&690)
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s6.sinaimg.cn/mw690/6b94d568tx6CJt0FBxr05&690)
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s4.sinaimg.cn/mw690/6b94d568tx6CJrFItV153&690)
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s14.sinaimg.cn/mw690/6b94d568tx6CJtIoVJ36d&690)
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s12.sinaimg.cn/mw690/6b94d568tx6CJtU55lV5b&690)
【通过码流分析工具的查看，800来个 PID为0X191的401节目的PMT SECTION TS包都是一样的】
根据TS Packet 的语法，可以简要分析下上面的数据包：
同步字节段：0X47（8bit）
传输错误指示字符段：0（1bit）
有效负载数据单元起始指示符字段：1（1bit）
传输优先级字段：0（1bit）
PID：0x191（13bit）(PAT表中指定该PID的TS包为包含PMT SECTION数据的TS包)
传输加扰控制字段：（0x0）（2bit）
调整字段控制字段：0x01(无调整字段，只有有效负载)（2bit）
连续性计数器字段：（4bit）
数据字节字段：这些数据 有可能是PES包，有可能是PSISECTION (由PID决定)，如果没有调整字段，从这开始就是负载数据了；
  若是PES数据，则这些负载数据通过PES分组语法来解析；
  若是PSISECTION数据，则通过SECTION语法来解释，可通过第一个table_id来确定是哪一个表的SECTION；如下表：【要知道PMT表须先知道PAT表】
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s8.sinaimg.cn/mw690/6b94d568tx6CFIN5Fht87&690)
**Programassociation Table**
The Program Association Table provides the correspondencebetween a program_number and the **PID valueof the Transport Stream packets** which carry the programdefinition. The program_number is the numeric label associated witha program. 
**The overall table is contained in oneor more sections** with the following syntax.**It may be segmented to occupy multiplesections **
**Programassociation section[PAT section syntax]**
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s8.sinaimg.cn/mw690/6b94d568tx6CFsfWqp157&690)
**table_id :**
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s6.sinaimg.cn/mw690/6b94d568tx6CFK2yM3X45&690)
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s7.sinaimg.cn/mw690/6b94d568tx6CRkyLsO2f6&690)
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s2.sinaimg.cn/mw690/6b94d568tx6CRkzjHuV81&690)
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s9.sinaimg.cn/mw690/6b94d568tx6CRkLRVZmc8&690)
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s3.sinaimg.cn/mw690/6b94d568tx6CRkMbwWe02&690)
**CAS原理：**
  条件接收（CA）系统（CAS）是数字电视广播（DVB）实行收费所必须采用的系统，也是数字电视平台不可缺少的部分，CAS负责完成用户授权控制与管理信息的获取、生成、加密、发送以及节目调度控制等工作，保证只有已被授权的用户才能收看节目，从而保护节目制作商和广播运营商的利益。
  在DVB前端，CAS将通过加扰器节目级复用器复用后的节目内容，即MPEG-2/DVB视频、音频和辅助数据传输流（TS），**与一个加扰伪随机序列做XOR运算**，这个**伪随机加扰序列**就是控制字发生器提供的**控制字CW**。
**CW**被业务密钥（**SK**）**加密处理后**在**授权控制信息（ECM）数据流中传送**，**SK**被用户个人分配密钥（**PDK**）**加密**处理后，在**授权管理信息EMM**数据流中传送，**PDK存放在用户智能卡（Smard Card）中**。
   已加扰的MPEG-2/DVB视频、音频和辅助数据传输流（TS）、ECM、EMM数据流、节目说明信息（PSI）和业务信息（SI）等数据流，经复用后，从发送端经传输信道传送给接收端机顶盒（STB）。
   对于已经缴费的用户，其智能卡会被授权，STB从接收到的已加扰传输流中，**解复用出ECM和EMM数据流**后送给智能卡，**智能卡首先读取PDK**，用**PDK对EMM解密得到SK**，再用**SK对ECM解密得到CW**，利用**CW由解扰器对已加扰传输流（TS）进行解扰后，再进行节目级解复用**。
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s4.sinaimg.cn/mw690/6b94d568tx6CRqQubTle3&690)
![TS各个表 <wbr>与 <wbr>SECTION <wbr>的解析 <wbr>CAS原理](http://s12.sinaimg.cn/mw690/6b94d568tx6CRrj9FID3b&690)
