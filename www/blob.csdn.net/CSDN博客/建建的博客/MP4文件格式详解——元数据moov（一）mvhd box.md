# MP4文件格式详解——元数据moov（一）mvhd box - 建建的博客 - CSDN博客
2017年08月12日 19:53:49[纪建](https://me.csdn.net/u013898698)阅读数：246
个人分类：[Darwin](https://blog.csdn.net/u013898698/article/category/7009805)
元数据moov（一）（ISO-14496-12）
**Author:Pirate Leo**
**Email:[codeevoship@gmail.com](mailto:codeevoship@gmail.com)**
ISO 14496 - 12 定义了一种封装媒体数据的基础文件格式，mp4、3gp、ismv等我们常见媒体封装格式都是以这种基础文件格式为基础衍生的。
如果从全局角度了解基础文件格式，请看我之前的博文《[MP4文件格式详解——结构概述](http://blog.csdn.net/pirateleo/article/details/7061452)》。
本系列文档从MP4文件入手，对文件中重要的box进行解析。
<======================================================================>
**本次解析moov box，也称movie box。**
movie box —— container box whose sub-boxes define the metadata for a presentation (‘moov’) 
moov包含的一系列次级box中存储着媒体播放所需的元数据（metadata）。
两点疑问：什么是元数据？moov有哪些次级box？
**1）元数据：描述数据的数据**。针对媒体文件而言元数据都有哪些呢？为了让大家直观了解：
![](https://img-my.csdn.net/uploads/201205/22/1337656687_8345.JPG)
上图是使用“格式工厂”获取某MP4文件的媒体信息，这些媒体信息基本都包含在moov中。
视频包括编码等级、分辨率、色域、码率、帧率、位深、时长等等……
音频又包括声道、采样率等音频特有属性。
这些元数据对于我们的价值在于：我们的系统（比如PC播放器，高清播放机）可以通过对moov box的解析，自动适配运行在某种模式下去播放影片。在嵌入式领域，由于DSP或ARM的Ram空间有限，经常需要动态加载本次播放所需的解码器（算法程序），通过自适配可以用最廉价的CPU，完成一款支持多码率多格式的全能播放器。
2）**moov有哪些次级box用来保存这些信息？**
看过我第一篇文章《[MP4文件格式详解——结构概述](http://blog.csdn.net/pirateleo/article/details/7061452)》的人肯定对moov
 box的组成有一定直观认识，这里引用一段以前的内容。
|moov||||||√|container for all the metadata|
|----|----|----|----|----|----|----|----|
||mvhd|||||√|movie header, overall declarations|
||trak|||||√|container for an individual track or stream|
|||tkhd||||√|track header, overall information about the track|
|||tref|||||track reference container|
|||edts|||||edit list container|
||||elst||||an edit list|
|||mdia||||√|container for the media information in a track|
||||mdhd|||√|media header, overall information about the media|
||||hdlr|||√|handler, declares the media (handler) type|
||||minf|||√|media information container|
|||||vmhd|||video media header, overall information (video track only)|
|||||smhd|||sound media header, overall information (sound track only)|
|||||hmhd|||hint media header, overall information (hint track only)|
|||||nmhd|||Null media header, overall information (some tracks only)|
|||||dinf||√|data information box, container|
||||||dref|√|data reference box, declares source(s) of media data in track|
|||||stbl||√|sample table box, container for the time/space map|
||||||stsd|√|sample descriptions (codec types, initialization etc.)|
||||||stts|√|(decoding) time-to-sample|
||||||ctts||(composition) time to sample|
||||||stsc|√|sample-to-chunk, partial data-offsetinformation|
||||||stsz||sample sizes (framing)|
||||||stz2||compact sample sizes (framing)|
||||||stco|√|chunk offset, partial data-offset information|
||||||co64||64-bit chunk offset|
||||||stss||sync sample table (random access points)|
||||||stsh||shadow sync sample table|
||||||padb||sample padding bits|
||||||stdp||sample degradation priority|
||||||sdtp||independent and disposable samples|
||||||sbgp||sample-to-group|
||||||sgpd||sample group description|
||||||subs||sub-sample information|
先对两个重要的box作功能上的描述，每个box具体组成细节在以后的篇幅中将详细介绍。
**mvhd **——This box defines overall information which is media-independent, and relevant to the entire presentation.
全文件唯一的（一个文件中只能包含一个mvhd box），对整个文件所包含的媒体数据作全面的全局的描述。包含了媒体的创建与修改时间时间刻度、默认音量、色域、时长等信息。
**[cpp]**[view plain](http://blog.csdn.net/pirateleo/article/details/7590056#)[copy](http://blog.csdn.net/pirateleo/article/details/7590056#)[print](http://blog.csdn.net/pirateleo/article/details/7590056#)[?](http://blog.csdn.net/pirateleo/article/details/7590056#)
- aligned(8) class MovieHeaderBox extends FullBox(‘mvhd’, version, 0)   
- {   
- if (version==1)   
-     {   
-         unsigned int(64) creation_time;   
-         unsigned int(64) modification_time;   
-         unsigned int(32) timescale;   
-         unsigned int(64) duration;   
-     }   
- else
-     { // version==0 
-         unsigned int(32) creation_time;   
-         unsigned int(32) modification_time;   
-         unsigned int(32) timescale;   
-         unsigned int(32) duration;   
-     }   
- templateint(32)  rate = 0x00010000; // typically 1.0 
- templateint(16)  volume = 0x0100;  // typically, full volume 
- const bit(16)  reserved = 0;   
- const unsigned int(32)[2]  reserved = 0;   
- templateint(32)[9]  matrix = { 0x00010000,0,0,0,0x00010000,0,0,0,0x40000000 };   
- // Unity matrix 
-     bit(32)[6]  pre_defined = 0;   
-     unsigned int(32) next_track_ID;   
- }   
首先mvhd是个Full Box，引用我第一篇文中解释：“ FullBox，是Box的扩展，Box结构的基础上在Header中增加8bits version和24bits flags。”，见下图：
![](https://img-my.csdn.net/uploads/201205/22/1337659249_6769.JPG)
图中标红的是moov box的长度与标识（其实这种box结构类似TLV，称为LTV更直观）。
标蓝的是mvhd的长度0x6c，表黄的是mvhd的标识与内容：
**0x6D766864 **是mvhd的ASCII标识；
**0x00000000 **是FullBox扩展出来的标识位，这里是全0，Version和flags都是0，参见前面的结构定义可知后面的时间与时长采用了32bit表示方式。
第一个**0x7C25B080**是创建时间，第二个**0x7C25B080**是最后修改时间。可见媒体未被修改过。这两个数值是怎么描述具体时间的呢？
 time is an integer that declares the creation time of the presentation (in seconds since midnight, Jan. 1, 1904, in UTC time) 
即，从UTC时间的1904年1月1日0点至今的秒数。我们手动算一下：
0x7C25B080 = 2082844800秒，大概是66.0465年（每年按365天算，不考虑闰年），1904+66=1970年中。看来这个媒体文件生成的并不规范，没有按照ISO的规范填写创建与修改时间（by the way，该文件是一年前使用iKu转码生成）。不过这里的时间并不影响播放器识别并播放影片。 
我们假设4字节的时间描述取其最大值0xFFFFFFFF，通过计算最多支持到2040年。如果，**假设该参数是播放视频所必备的参数**，那么到2040年后，也许所有的MP4文件一夜间就无法播放了~（0 == Version这种）。
**0x000003E8 **是timescale，该数值表示本文件的所有时间描述所采用的单位。0x3E8 = 1000，即将1s平均分为1000份，每份1ms。
**0x000A06A2** 是duration，媒体可播放时长，0xA06A2 =  657058，这个数值的单位与实际时间的对应关系就要通过上面的timescale参数。
**duration / timescale = 可播放时长（s）**。这里算出该视频能播放657.058s。使用MPC打开，时长与我们计算的一致。
![](https://img-my.csdn.net/uploads/201205/22/1337660970_9963.JPG)
timescale时间刻度贯穿在整个文件中，所有对于时间的描述都要以其为参照，例如解码时间DTS，展示时间PTS等最重要的时间描述。
0x00010000 媒体速率，这个值代表原始倍速。
0x0100 媒体音量，这个值代表满音量。
接下来的一系列值都是结构中的预定义值，参见结构定义即可。
**trak**——This is a container box for a single track of a presentation. A presentation consists of one or more tracks. Each track is independent of the other tracks in the presentation and carries its own temporal and spatial information. Each
 track will contain its associated Media Box.
其次级box中包含了单个媒体轨道（Track）所特有的描述信息。
我们知道一部视频一般都有多个轨道组成。
例如，《让子弹飞》的正版DVD，1）有一条视频轨用于电影画面。2）至少有两条音频轨分别提供了普通话与四川话版，实际上为了营造更加逼真的现场效果，为了配合多声道家庭影院该影片还独有一条音效轨。3）多条字幕轨，简体中文，繁体中文，英文……。从中我们可以理解为什么trak box可以有多个：每个track都是独立的，具有自我特征与属性的，因此需要各自描述互不干涉。
我们一般所看的MP4文件中moov下都至少是mvhd-trak-trak-trak...这种构成。
后续将对trak下的一系列子box进行解析
