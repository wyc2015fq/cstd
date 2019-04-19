# TS流中BAT与SDT等表的解析 - 建建的博客 - CSDN博客
2017年12月21日 14:23:10[纪建](https://me.csdn.net/u013898698)阅读数：2015
TS流的文章不少，但都比较难读；TS码流分析的软件挺多，但思路都雷同。
本文章从另外一个角度来理解和分析TS流，不过只是我个人的理解，错误在所难免，不当之处，请大家多多指教(本文可能经常更新)。
          由于传输的文件比较大，文件分片传输，形成包（也可以叫帧）；由各个包组合在一起，就能形成这完整的文件。不过，TS流传输按DVB标准，需要打包为固定格式的包，同时传输一些表（PSI/SI表，共9种表），以便解析这些数据时获得相应的一些信息，包括一些私有的信息，若不符合这些表，将不能被正确解析出内容。由于传输环境复杂，会加上一系列纠错，验证机制，比如CRC32,前向纠错等，不过，初学就不需要了解太细。
本人遵循由浅入深的方式展开，撇开繁重的标准（但标准才是标准，需要时一定得查；还有就是用码流分析软件多看看TS流，了解其结构）。
一、ts包的认识（一般拿188包的说事儿）：
        1.ts流包的大小可能分为188和204字节的包（我认为是204字节是调制出来的包；204个字节，在188个字节后加上了16字节的CRC校验数据）；TS文件判断的方法一般为前5个包都是188字节(0x47开头，2..5*188都是0x47)，则认为是包大小为188字节。
        2.TS包结构，4个字节的ts包头，184字节内容(包数据)；Packet Header（包头）主要告诉唯一网络标识符PID；而在Packet Data（包数据）中会给出数据，并且给出table-id。
PID(包号，占用12位)和table-id(表id，一个字节，8位)的组合，就能唯一确定一类PSI/SI发的是哪一种表（DVB的标准规定）。注意：不是一个pid=0x11了就一定是SDT，还得看table-id,若table-id,=0x4A，则是BAT表。看标准，可能就更清楚一些的了。没办法，经常都遇到不是1个整的字节表示，而用位来表示的，以节约传输的开销。
      3.PSI/SI表：PSI:PAT,PMT,CAT,NIT四种，SI: SDT/BAT,EIT,TDT/TOT,ST,SIT,DIT,RST九种，具体代表意义查标准，或者附的资料。需提示的是SDT/BAT,pid都为0x11(但table-id不同)；TDT/TOT，pid都为0x14(但table-id不同)；ST填充表(table-id=0x72)，可能存在于包0x10~0x14的包中，我看很多码流分析软件都没剖析出ST(我的软件也懒得做的了)。../资料/包pid.doc里面可以查到。这些表有些是强制传输的（一般为当前流--actual），有些的选择性传输的。
       同时，引入一个概念，子表(书上没怎么注明)。比如SDT表可由多个sub_SDT子表组成，而各个子表可由1~n个段组成。
       另外，为避免忘记，提示一下204字节的包（以前程序算段长不正确的原因）：注意，204字节的包最后16字节是FEC前向纠错，他们都不计入段长的（以前看的标准没说清楚，结果计入而导致PSICreator解析不出有续帧的204包内容）。听说，204包是由解码器还原为188字节后再解析的，我用的PSICreator则直接略过最后16字节分析就正常工作的了。
二、表和section(段)
     先我们来看一个子表非常齐全的ts流，具有典型的代表意义。
![](https://img-blog.csdn.net/20141114133849203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmVsbDMzNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       先用TSExpert来打开一个ts流中的表结构（很齐全的各种表，最多估计也就这13个子表的了,顶多还加上TOT，ST这15个子表就没有的了）。
     其次，我们查阅一下《 中文SI版本1.0.doc》中的定义：
      (1).表(table):由具有相同的表标识符（table_id）的一系列子表构成。
     (2).子表sub_table
       子表是指具有相同表标识符（table_id）的段的集合，并且
对网络信息表（NIT）： 具有相同的table_id_extension（network_id）和version_number；
对业务群关联表（BAT）： 具有相同的table_id_extension（bouquet_id）和version_number；
对业务描述表（SDT）： 具有相同的table_id_extension（transport_stream_id），相同的original_network_id 和version_number；
对事件信息表（EIT）：具有相同的table_id_extension （service_id），相同的transport_stream_id 、original_network_id和version_number。
      (3).段  section: 段是一个语法结构。
用于将本标准中定义的所有业务信息映射成为GB/T 17975.1-2000的传输流包。
     (4)、续帧，一个section段需要多个包来装完的时候，除了第1个包，后面2~n个包都为续帧。（EIT表n值最大24，其余n值为最大6）          
提醒：从TSExpert中，我们可以看见，PAT,CAT,PMT,BAT，TDT等是一个子表（table_id只有1种）；
          NIT和SDT有2个子表（当前流Current和其他流Other，table_id有2种）,EIT有4个（当前流Current/其他流Other，当前/后续；当前流Current/其他流Other，事件时间表信息，table_id有4种，EIT的table_id值判别是区间值）。
    （1）、子表的区分首先由Table_id的不同来判别。查阅各种表结构，可以看见table_id的定义值或区间值。从另外一个角度来看，子表想描述的是一个独立的表内容，该内容主要由table_id和table_id_extension等来区分，例如SDT子表描述的各个ts流(transport_stream_id)含有的不容内容，该子表描述内容很长的话，可以用续帧或者兄弟段，总之一定能将该子表内容装完的。
   （2）、相同pid的子表的构成一类表，比如当前传输流NIT子表，其他传输流NIT子表共计2个子表构成了NIT表，即current
 Nit和other Nit构成NIT表。
   （3）、section段是个语法结构，一个子表可能由1~n个段构成。表头信息除了段号不同外，其他相同的，就是我所提出的兄弟段(brother_section),有的叫分段（但不好理解），后面会提到该概念。是PSI分析的基本概念之一，它可能由1到n个包（一般包号之间互有间隔）构成的。见下图：
![](https://img-blog.csdn.net/20141114150555384?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmVsbDMzNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
兄弟段: 只是sectionNumber不同的段，他们共同描述了一个流的具体内容，图上红框部分这3个段互为兄弟段。
剩下2个段table_id_extension的不同，让其为独立的段。
三、section段的理解（段是码流分析中，很基本的概念，由基本的段构成子表和表）
4.根据前面所述，子表由1~n个段组成。哪个PAT表举例,一个段都有长度限制，并含在section语法结构中的，一般为1021字节，具体后面会有解释：
 (1).1个包（188字节）装完此setion段，那ok,没说的；
(2).1个包不能装完，但1021字节可以装完（一个段可以装完，2~6个包装完）,比如描述了很多套节目，比如50套节目的描述:那么第1个包的包头会用Payload_unit_start_indicator=1来告诉我们这是section的起始的第1个包头，而后续的第2~n个包的包头部分，这字段都为0的，表示是section的续帧，内容都是184字节。
(3),若一个段都还装不完，那么我们用多个段来装，最多255个段(因为段结构中sectionNumber为1B，即最大0xFF),那最大长度=255*1021字节,约255M，完全足够的了。因为这些段就是sectionNumber的不同，所以，我们叫他们为兄弟段更容易理解和记忆。
      一般情况下，几个包就能传输完一个段的了了（当然表的大小有限度的，具体的得查附录的标准，“除EIT表的section外，每个表中的段限长为1024字节，但EIT中的段限长4096字节。一个子表最多也就能容纳6-1个续帧，因为6*(188-4)>1024,EIT最大也只能24-1个续帧。”）。
       续帧的进一步理解：比如，一个SDT表，由多个续帧组成，包头的payload_unit_start_indicator告诉了我们，是否为该表的第1帧，payload_unit_start_indicator=1,则是第1帧，后续的(payload_unit_start_indicator=0)都是它的续帧，即后续帧，后续帧只有pid，而没有table-id的。一般情况下，表的第1帧包头47
 40...,而后续帧包头为47 00... 。
在码流分析仪上，分析的码流是一眼看不出一个段是否有续帧的，那如何判断呢？一种方法是读包的软件，比如我写的类似可以看每个包内容的软件TsDataReader，它的续帧的包头位的payload_unit_start_indicator=0；另外一种，码流分析仪上看，段长度大于171字节section-length>171(0xAB),那么必有续帧[计算的方法为：188-(4字节包头+1字节适配域+8字节Section_length+4字节CRC)=171字节]。
     兄弟段的进一步理解：内容中的last_section_number告诉了我们本子表总共有几个section，section_number告诉了当前的section处于第几个section。        
       在传相同pid的表的时候，比如SDT和BAT表，都由多个续帧组成，会不会在码流中看见一个SDT段中夹杂了BAT的续帧的呢？答案是不会的。因为发表是发完一张表才发下一张表的，所以，发一张SDT表的时候，中间不会插入一些BAT或者其他表的帧(包括后续帧)，续帧和续帧之间可能插入别的pid，可能是空包或者视频包，而且相隔得不会几百帧，码流看见，具体原因由标准规定的。
故：判断一帧是否为SDT还是BAT，先查找此帧pid=0x11,payload_unit_start_indicator=1是单元载荷开始帧，再开table-id=0x4A，就是BAT，否则为SDT；payload_unit_start_indicator=0为续帧，该续帧往前找到（直到）payload_unit_start_indicator=1的帧，看为SDT还是BAT，续帧肯定就和开始帧一致为SDT或者BAT。
 SDT/BAT是比较难的判别，其他的表，甚至可以直接看pid就判断他们为什么表的。TDT/TOT一般是没续帧的，所以，也就是看table-id即可判断的了。
注意：我看的，网上大多数有代码的案例，还是没解析PMT,PAT有多个续帧的。
     6.table-id:其实它的位置不是固定的，初学很容易犯错的。通常是在ipos=05,第6个字节处。但具体应该按以下算法：
      一般是看包头的payload_unit_start_indicator=1是单元载荷开始帧(才有table-id)，若包头的adaptation_field_control(2bit)=1，则该帧的第5个字节内容为偏移量为0，table-id位置为第6个字节的内容(ipos=5,因为ipos有0开始的字节),码流中多数都为此种情况。若包头的adaptation_field_control=3，则该帧的第5个字节内容为偏移量为xx(0~0xFF)，table-id位置为6+xx(ipos=5+xx)；adaptation_field_control(2bit)=2,4都无效的。
   7.找section语法是痛苦的事情，一位一位的核对(包头才4个字节，但都看着麻烦)。一般包数据按section语法解析，比如PAT,PMT,NIT语法等等，不过翻阅了资料，到从table-id到Last_section_number之间为8字节，Last_section_number之后就一般是描述或者内容的了。
     附录的软件，能比较好帮助核对位的问题，点开【TS syntax】页，可以看见包头和数据(因各种表不同，只解析前8个字节)的解析，右下方的ts_table Filed列表中单击某一value，该数据的位置所在的数据将被显示出来，并且，可以选中某一数据，显示为2，10，16进制的数据，这样，核对，很方便。
      8.说说PMT，PMT为节目映射表，它的table-id=0x02,但pid是不固定的。原因为不能规定哪个pid就是代表了某1个节目所使用，故一般是PAT表解析出各个节目的PMT-pid(包括program-id)，遇到pid等于PMT-pid列表中的pid的时候，那必定为PMT表的。
解析PMT的时候，PMT表自身也含有program-id，若PAT表中的某一program-id与PMT表中的当前program-id相同，则是同一套节目，可解析PMT表的pcr-pid和音视频pid。
所以，一般的解析方法是先解析【一帧完整的PAT】获得PMT列表，再从PMT列表中解析出pcr-pid和音视频pid(一般pcr-pid等于视频pid)。
      当然，也可以撇开PAT，直接解析PMT的哦，只是麻烦一些。方法：在一段足够长的周期内(比如1~2分钟)，查找所有table-id=2的包，若能解析出program-id和pcr,音视频pid，则是PMT表，构建出该PMT-pid,program-id和pcr,音视频pid。若是静态的ts流文件，好构建一些的。
 9.全局表：NIT,SDT/BAT,EIT； 局部表：PAT、PMT和CAT表。那这是什么意思的呢？
全局表，那就应该是整个TS流传输系统中都有效，区分的方式分为当前流和其他流，比如有actual SDT section (0x42 service_description_section - actual_transport_stream) 和 others
 SDT section (0x46 service_description_section - other_transport_stream )等来区分；
局部表，也就意味着只在当前的传输流中有效，比如一个频点的流（或者可以看做复用器复用多路节目后的多节目流MPTS），当前的一个TS流有一个ts_id(不规范的系统里面可能存在几个相同ts_id的流)；一个TS流里有只对自身有效的局部表，比如PAT只描述的当前节目流内的节目，一般一个TS流里面的节目数可能<32,而全局表SDT表里面的可能上百套的了。
17:06 2014-8-10
附录：
TS包头定义（4个字节）：
typedef struct TS_packet_header
{
    unsigned sync_byte                        : 8; //同步字节, 固定为0x47,表示后面的是一个TS分组
    unsigned transport_error_indicator        : 1; //传输误码指示符
    unsigned payload_unit_start_indicator    : 1; //有效荷载单元起始指示符
    unsigned transport_priority              : 1; //传输优先, 1表示高优先级,传输机制可能用到，解码用不着
    unsigned PID                            : 13; //PID
    unsigned transport_scrambling_control    : 2; //传输加扰控制 
    unsigned adaption_field_control            : 2; //自适应控制 01仅含有效负载，10仅含调整字段，11含有调整字段和有效负载。为00解码器不进行处理
    unsigned continuity_counter                : 4; //连续计数器 一个4bit的计数器，范围0-15
} TS_packet_header;
 为方便查阅，写上adaption_field_control 判断的代码：
If (adapaction_field_control= =’10’ || --------------调整字段（一般视频或音频含pcr的包中）
           adapaction_field_control= =’11’)---调整字段+有效载荷 
             adapcation_field()     
        If (adapaction_field_control= =’01’ || -----//无调整字段，仅净荷
          adapaction_field_control= =’00’)     ---保留 
            data_byte() 
//3.1 确定净荷起始位置（4字节固定首部+适配字段长度,adaption_field_control）
      if  adaptation_field_control=1                                //0B:01
      then                                      //无调整字段，仅净荷;即Data[4]=0x00；
        adapLen :=0  //TS分组适配字段长度为0
      else if  adaptation_field_control=3 then //有调整字段和净荷,0~182B长度的调整字段后为有效净荷    0B:11
        adapLen :=buffer[4];          //调整字段(自适应字段) 长度
在判断pcr的时候用，if  adaptation_field_control=2，则可能为PCR包，但还需要判断PCR_flag=1.
不过用Delphi的很少的了，用来写ts分析的更是少之又少的，共享资源的D7源码（含相关解析资料，能帮助理解TS流），其实做的功能还是不少的{含了可执行文件exe的，可以查找同一类pid后，逐个看每种包，导出等功能。其实，功能还是非常多的！（比如直接粘贴复制Hex字符串和转码功能等，导出指定表及其pid，查看包头,PCR分析，pid修改等等）}：
1.源代码和exe文件： [http://pan.baidu.com/s/1sjnuOxf](http://pan.baidu.com/s/1sjnuOxf)[](http://pan.baidu.com/s/1c0GO1aw)[](http://pan.baidu.com/s/1gd5SY47)
2.PSI/SI标准共享（中文）：[http://pan.baidu.com/s/1o6hYje6](http://pan.baidu.com/s/1o6hYje6)
2014.12.31
对了，提醒一下：写程序的时候，可以将段的头部8字节作为一个结构体，剩余为字段内容解析。NIT和BAT的结构完全一样，所以解析方法就是改名字而已，比较方便！
21:29 2015-7-24
四、新理解：
曾经以为自己理解PSI差不多的了，也写了提取分析psi的程序。但还差呢！
1.码流分析仪：总以为码流分析仪分析的就正确，但包括蓝拓扑在内的有些分析的内容仍是错误的。需要提醒的是，新发一个表到码流中，需要“停止”码流分析仪后，再进行分析，否则，结果可能叠加（比如bat只有1个表，换1个表发后，可能显示2个bat表，有些表甚至不显示出来）。即，分析新码流需要停止后再分析。
2.BAT和SDT表：写程序的时候，BAT和SDT一起发，就出现连续计数错误。经过研究码流，的确：SDT和BAT的连续计数器是共用的（不是独立计数的哈）。意味着：0x11的表都是连续计数的，bat包号为0x13，则下一个sdt包需继续0x14，若下一个又是SDT或者bat则为0x15。其实，就归结为1句话，相同的pid，需要连续计数。
3.发表策略：省网码流，可能并非1个周期内发所有表，而是以下方式，应该更合理的：
1.PAT,PMT,SDT
2.PAT,PMT,NIT
3.PAT,PMT,BAT
4.TDT表：注意，TDT表是没有crc32校验的；就是发送像这样的时间信息2015-09-24
 14:30:12,共5字节（2字节MJD+3字节时间信息显示为143012）。有意思的是，成都码流就是把TOT时间偏离表的内容copy到TDT里面一起发出来，部分码流分析仪也认，估计机顶盒解析只要按此方式，就可以一起解析TDT和TOT的了。
14:31 2015-9-24
五。PMT表：
卫星上的江苏卫视和优漫卡通2套节目都是使用的同一个PMT_pid, 类似的还有陕西卫视和农林卫视；
一个PMT表的区分是相同的PMT_pid和相同的 Program_number ，所以这2套节目的PMT_pid相同，而 Program_number 不同，无问题。即同一个TS流中存在2个甚至多个PMT_pid相同的流，它们靠Program_number
 不同来区分。
故靠解析PMT_pid来区分不同的PMT表是不正确的。
10:31 2017/6/23
