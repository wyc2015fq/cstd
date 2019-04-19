# 什么是DVB-SI?对PSI(PAT,PMT,CAT,NIT,SDT,EIT)的理解 - 建建的博客 - CSDN博客
2018年03月12日 14:53:20[纪建](https://me.csdn.net/u013898698)阅读数：117
个人分类：[多媒体封装格式详解](https://blog.csdn.net/u013898698/article/category/7105689)

数字电视与传统模拟电视节目选择的方式完全不同， 传统电视的每一个频道对应一个节目，只要调到相应的频率， 就可以看到节目。 而在数字电视信号中，一路码流对应多路节目，使用复用技术就可以做到了。一个物理的频道只能给出包含多路节目的一路传输流。要观看其中的某一路节目，还必须从该传输流中提取出该路节目的压缩包，然后再进行解码。 所以怎样从众多的传输流中，选中一路节目播放，就变得很复杂。在mpeg-2的传输流(Transport Stream)中，节目专用信息PSI（Program Specific Information），就是规定不同节目和节目中的不同成分如何复用成一个统一的码流。以PSI为基础可以提供一个码流的构成，从而帮助用户对节目进行选择。DVB中的服务信息SI(Service Information)则对此进行了进一步的扩展，加入了一些对用户有用的信息，标示节目的类型，服务商，节目的相互关系等。正确的了解mpeg-2的PSI以及DVB的SI的结构，及其在节目组织，选择中的应用，可以正确理解service information在DVB解码中的地位。对于我们做好对数字节目的复用，也能起到帮助作用。
1， MPEG-2 TS中的PSI
PSI信息主要包括以下的表：
    PAT（Program Association Table）：节目群丛表，该表的PID是固定的0x0000，它的主要作用是指出该传输流ID，以及该路传输流中所对应的几路节目流的 MAP 表和网络信息表的PID。
    PMT（Program Map Table）：节目映射表，该表的PID是由PAT提供给出的。通过该表可以得到一路节目中包含的信息，例如，该路节目由哪些流构成和这些流的类型（视频，音频，数据），指定节目中各流对应的PID，以及该节目的PCR所对应的PID。
    NIT（Network Information Table）：网络信息表，该表的PID是由PAT提供给出的。NIT的作用主要是对多路传输流的识别，NIT提供多路传输流，物理网络及网络传输的相关的一些信息，如用于调谐的频率信息以及编码方式。调制方式等参数方面的信息。
    CAT（Conditional Access Table）：条件访问表，PID － 0x0001。
除了上述的几种表外，mpeg-2还提供了私有字段，用于实现对MPEG-2的扩充。
2， DVB中的SI
Mpeg-2的PSI中提供了不少的相关节目组成和相互关系的信息，从而使得在接收端可以正确的对多路传输流进行分解。但是这些信息在实际使用时仍显得不够，为此在DVB中采用SI对PSI信息进行了进一步的扩展。
除了在PSI中的信息之外，DVB还定义了一些其他的一些表。
SI的主要用途有：
a、根据NIT，PAT，PMT等信息可以进行自动的频道调谐；
b、更方便的对节目进行选择和定位；
c、实现电子节目指南EPG(Electronic Program Guide)，等等。
PSI中的信息基本上都是与当前码流相关的，即它们所涉及的内容都与当前码流中的部分信息相关。与PSI不同的是，SI的信息可以包括不在当前码流中的一些服务和事件，允许用户进行更多的选择和了解更多的其他服务信息。
DVB规定携带SI信息的传输包必须用指定的PID，指定的PID如下表：
|TABLE TYPE|PID Value|
|----|----|
|PAT|0X0000|
|CAT|0X0001|
|TSDT|0X0002|
|RESERVED|0X0003 TO 0X000F|
|NIT，ST|0X0010|
|SDT，BAT，ST|0X0011|
|EIT，ST|0X0012|
|RST，ST|0X0013|
|TDT，TOT，ST|0X0014|
|Network Synchroniztion|0X0015|
|Reserved for future use|0X0016 TO 0X001B|
|Inband signaling|0X001C|
|Measurement|0X001D|
|DIT|0X001E|
|SIT|0X001F|
在该表中可以看到同一个PID可以对应不同的表，要把这样的表区分开来，需要进一步找到TableID 进行识别。TableID表如下：         
Value   Description 
0x00 Program_association_section
0x01 Conditional_access_section
0x02 Program_map_section
0x03 Transport_stream_description_section
0x04 TO 0x3F Reserved
0x40 Network_information_section-actual_network
0x41 Network_information_section-other_network
0x42 Service_description_section-actual_transport_stream
0x43 TO 0x45 Reserved for future use
0x46 Service_description_section-other_transport_stream
0x47 TO 0x49 Reserved for future use
0x4A Bouquet_association_section
0x4B TO 0x4D Reserved for future use
0x4E Event_information_section-actual_transport_stream,P/F
0x4F Event_information_section-other_transport_stream,P/F
0x50 TO 0x5F Event_information_section-actual_transport_stream,schedule
0x60 TO 0x6F Event_information_section-other_transport_stream,schedule
0x70 Time_data_section
0x71 Running_status_section
0x72 Stuffing_section
0x73 Time_offset_section
0x74 TO 0x7D Reserved for future use
0x7E Discontinuity_information_section
0x7F Selection_information_section
0x80 TO 0xFE User defined
0xFF Reserved
有了这两个ID我们可以在码流中找到我们想要的任何一张表。
SI主要包括以下一些信息表：
NIT（Network Information Table）：NIT的作用主要是对多路传输流的识别，NIT
提供多路传输流，物理网络及网络传输的相关的一些信息，如用于调谐的频率信息以
及编码方式。调制方式等参数方面的信息。根据此信息设置IRD（Integrated Receiver
Decoder）可以进行多路传输流之间的切换。 
SDT（Service Description Table）：用于描述系统中各路节目的名称，该节目的提
供者，是否有相应的时间描述表等方面的信息。该表可以描述当前传输流，也可以描
述其他的传输流，这由TableID进行区分。
EIT（Event Information Table）：该表示对某一路节目的更进一步的描述。它提供
事件的的名称，开始时间，时间长度，运行状态等。
TDT（Time and Data Table）：该表提供当前的时间信息，该表用来对IRD的解码
时钟进行更新
BAT（Bouquet Association Table）：该表提供一系列类似节目的集合。这些节目
可以不在同一个传输流中，利用该表可以很方便的进行相关节目或某一类节目的浏览
和选择。
RST（Running Status Table）：该表提供某一具体事件的的运行状态，可用于按时
自动的切换到指定的事件
TOT（Time Offset Table）：该表提供当地时间与TDT之间的关系，该表与TDT
配合使用。
TSDT（Transport Stream Description Table）：由PID0x0002标识,提供传输流的一
些参数.
ST（Stuffing Table）：该表表明其内容是无效的，只是作为填充字节。
那么，我们如何运用DVB中的SI 信息表呢？
DVB SI 的使用
1、网络信息表（NIT）
网络信息表提供一组传输流以及相应的调谐信息。NIT在IRD（整合接收解码器）启动程序中用到，并且调谐信息可以存储到不变的存储器中。NIT还可以用来标志调谐信息的变化。NIT遵从如下规定：
a、当前的传输系统必须包含NIT。
b、当且仅当NIT包含可用的传输系统描述符，当前传输系统才是有效的。这就指定了NIT包含有效信息的条件。在一些广播传输系统边界，TS中的NIT也被允许用来描述广播频道中的早期的网络。IRD不得不选择一个不同的机制来获得当前传输系统的相应的调谐信息。例如，一个卫星IRD接受到当前传输系统的一个卫星传输系统描述符，那么这个描述符被视为有效，如果一个电缆IRD接收到当前传输系统的一个电缆传输系统描述符也是有效的，但是，如果电缆IRD接收到当前传输系统的一个卫星传输系统描述符，那么这个描述符对于这个IRD就是无效的。
c、如果当前传输系统一个有效的NIT在SI比特流中出现，那么这个NIT应当列出当前传输系统中所有的TS。
d、SI流每10秒钟应当至少包含8个TS包来传输NIT或者空包。这个规定简化了广播传输边界中NIT的复位（replacement）。使用这样的复位机制，使得本地频率控制使用低廉设备成为可能。
SI使用两个标志来描述传输系统。它们分别是network_id和original_network_id。后者用来作为一个TS中的一个服务的唯一标志符，即使这个TS被传送到了其他的传输系统之中。一个TS可以被path：original_network_id/transport_stream_id唯一标识。一个服务可以被path：original_network_id/transport_stream_id/service_id唯一标识。显然，network_id不在这个path之内。另外，每个original_network_id中都有不同的service_id。当一个服务（同一个TS内）被传送到另一个传输系统后，只有network_id改变了，original_network_id不受影响。
下面举个例子，考虑如下情况：有两个服务（A和B），它们源于两个不同的传输系统并且碰巧有相同的service_id和transport_stream_id，它们被传输到一个新的传输系统。
在这个例子中，这两个服务被安排在新network中的不同的TS中。如果这两个服务被放在同一个TS中，那么必须修改服务的ID号，因为在同一个TS中一个service_id不能够分配给多个服务，并且一个TS仅仅可以对应一个original_network_id。
2、BAT信息
BAT提供一组服务，这些服务提供了一个基础，IRD在这些基础之上向用户展示有效的服务。BAT的传输是可选的。下面的规则提高了SI比特流的连贯性并且简化了IRD的工作。
SI比特流应当在每个BAT子表中列出集锦的所有服务。
注意：一个服务可以属于多个bouquet。这一规定使得IRD可以通过不同的TS得到一个服务。
如果IRD将bouquet中服务信息提供给用户，那么列在bouquet中的服务就得到保证，否则一些服务将会丢失。一个bouquet可以把不同网络传输的多个TS中的服务集中起来。如果BAT中所有服务都被列在SDT中，IRD对bouquet中所有服务信息的访问将会变得容易。同样的，如果NIT信息给出所有TS中service的容量，IRD对service的访问将会变得容易。
3、SDT信息
SDT用来罗列TS中service的名称以及其他参数。每一个TS中都有一个独立的SDT子表。为了提高对service的采集，定义了下列规则：
强制传输当前TS的SDT。
SDT列出的SI比特流至少包括TS中所有服务。
另外：
当前TS（例如table_id=0x46）中SDT描述其他TS时，应当列出这个TS的所有service。
强烈推荐service_id，在一个network中一旦把一个service_id分配给了一个特定的服务，那么这个service_id将不再变化，以便于IRD实现收藏频道列表之类的功能。
4.     EIT 信息
事件信息表用来传递当前的，将来的甚至更远的未来的事件的信息。每个Service都有自己独立的EIT子表。
4.1     EIT Present/Following 信息（一下简称EIT P/F）
下列规则简化了EIT P/F信息的获得。SI规范规定EIT段最大部超过4096字节。
SI流中的每个Service都有两段来描述EIT Present/Following，section_number 0x00用来描述当前事件，section_number 0x01描述下一个事件。这些约束不适用于NVOD引用的Service，这些Service在每段中可能有多个事件描述，并且在EIT Present/Following中可能有更多的段。推荐提供事件描述event_id的升序排列。
在一个section中，SI可以用最多4096个字节来描述一个单独的事件。
对于当前事件有如下规定：
a、同一时刻最多只有一个当前事件。
b、当存在一个当前事件时，该事件应当被描述在EIT Present/Following的section0中。
c、当前事件中的running_status应当被给出。如下表：
e、在同一时刻，最多有一个following event。
f、如果following event存在，该事件应当在EIT Present/Following的section1中。
g、如果following event不存在，则传输一个section1为空的EIT Present/Following。
h、following event的running_status应当给出，如下表：
事件的持续时间和EIT持续时间一样，必须包含事件被置为“not running”或者“pausing”。事件的开始时间和EIT start_time一样，应当是整个事件的开始时间，而不是从pause恢复后的时间。
注意：一个事件的开始时间加上它的持续时间可能比following event的开始时间要小。换句话说，允许事件之间有间隔。在这种情况下，following event被看作是间隔后的事件。这个事件应当编在EIT Present/Following的section1中。
注意：开始时间和持续时间都是预定的。一些广播服务提供商可能会更新这些信息。而另一些则更愿意保持开始时间不变。例如为了避免名为“8点新闻”的事件被误解，把信息中的开始时间从8：01：23改为8：00：00。
4.2     EIT Schedule信息
1）EIT Schedule结构
遵从如下规则：
a、EIT/Schedule分配了16个table_id，0x50-0x5F给当前TS，0x60-0x6F给其它TS，这些id按照时间顺序排列；
b、子表下的256个section被分为32段（segment），每8个section一个段（segment）。Segment＃1，从section0到7，segment＃2，从section8到15，等等；
c、每段包含三个小时内开始的事件信息；
d、段内事件信息按照时间排列；
e、如果一个段（segment）有n节（section），而n<8，这个信息必须放在段中前n个节中，还要显式指明最后一节的位置：S0＋n－1（S0是段中第一节），这个值在EIT的segment_last_section_number中。例如，第二段只有两节，那么segment_last_section_number包含值8＋2－1＝9；
f、如果段中有节的话，段的segment_last_section_number应当有值s0＋7；
g、完全空的段通过空节（不含任何loop over事件）表示，段的vsegment_last_section_number值为s0＋0；
h、段中事件的安排遵从一个时间t0。
t0是通用时间坐标（Universal Time Coordinated（UTC））的“last midnight”。
举个例子：UTC-6的下午5点，就是UTC-0的下午11点，即从“last midnight”算起23小时。因此对于UTC-6，t0就是前一天的下午6点；
i、table_id 0x50（对其它TS是0x60）的第0段，包含从午夜（UTC时间）到“今天”02：59：59（UTC时间）（三个小时）的事件信息。第1段包含从03：00：00到05：59：59（UTC时间）的事件信息，依此类推。这就意味着，第一个子表包含从“今天”UTC午夜时间算起前4天的信息；
j、last_section_number用来指明子表的结束位置；
k、last_table_id用来指明整个EIT/Schedule结构的结束位置；
l、与过去事件相关的段可以用空段代替，参见g规则；
m、EIT/Schedule包含的事件定义中的running_status应当设为“为定义”即0x00；
n、EIT/Schedule表不适用于NVOD涉及的服务，因为这些服务带有未定义开始时间的事件；
2）EIT 加密
EIT Schedule表格可以被加密。为了与条件接入相联系，必须分配一个service_id（＝MPEG-2 program_number）来描述加密的EIT Schedule Tables，这个service_id在PSI中。EIT在PMT中定义，service_id看成由一个private stream组成的各种电视节目（The EIT is identified in the Program Map Table (PMT) section for this service_id as a programme
consisting of one private stream），PMT包含一个或多个CA_descriptor来验证相关的CA码流。为达到这个目的，在DVB应用程序中service_id的值0xFFFF被保留。
