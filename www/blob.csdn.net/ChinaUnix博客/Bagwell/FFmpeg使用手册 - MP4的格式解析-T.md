# FFmpeg使用手册 - MP4的格式解析-T-Bagwell-ChinaUnix博客






[FFmpeg使用手册 - MP4的格式解析](/uid-11344913-id-5750335.html)

****
分类： LINUX

2016-08-23 19:01:04






						视频文件转MP4

在互联网中常见的格式中，跨平台最好的，应该是MP4文件，因为MP4文件既可以在PC平台的Flashplayer中播放，又可以在移动平台的Android，IOS等平台中进行播放，而且是默认播放，那么就可以理解为MP4为最常见的多媒体文件格式，所以重点介绍MP4封装，说到MP4封装，下面简单介绍一下MP4封装的基本格式.

3.1.1 MP4格式标准介绍

MP4格式标准为ISO-14496 Part 12、ISO-14496 Part 14，标准内容并不是特别的多，下面着重介绍一些重要的信息。

如果要了解MP4的格式的信息，首先要清楚几个概念：

    1.	MP4文件由许多个Box与FullBox组成；

    2.	每个Box由Header和Data两部分组成

    3.	FullBox则是Box的扩展，Box结构的基础上在Header中增加8bit位 version标志和24bit位的flags标志

    4.	Header包含了整个Box的长度的大小(Size)和类型（Type），当size等于0时，代表这个Box是文件的最后一个Box；当size等于0时说明Box长度需要更多的bits位来描述，在后面会定义一个64bits位的largesize用来描述Box的长度；当Type为uuid时，说明这个Box中的数据是用户自定义扩展类型；

    5.	Data为Box的实际数据，可以是纯数据也可以是更多的子Box；

    6.	当一个Box中Data是一系列的子Box时，这个Box又可以称作为Container Box。


而MP4文件中的Box的组成，大概可以用下面的列表进行排列，下面列表中标记√ 的Box为必要Box，否则为可选Box。
|ftyp||||||√|file type and compatibility|
|----|----|----|----|----|----|----|----|
|pdin|||||||progressive download information|
|moov||||||√|container for all the metadata|
||mvhd|||||√|movie header, overall declarations|
||trak|||||√|container for an individual track  or stream|
|||tkhd||||√|track header, overall information  about the track|
|||tref|||||track reference container|
|||edts|||||edit list container|
||||elst||||an edit list|
|||mdia||||√|container for the media information  in a track|
||||mdhd|||√|media header, overall information  about the media|
||||hdlr|||√|handler, declares the media  (handler) type|
||||minf|||√|media information container|
|||||vmhd|||video media header, overall  information (video track only)|
|||||smhd|||sound media header, overall  information (sound track only)|
|||||hmhd|||hint media header, overall  information (hint track only)|
|||||nmhd|||Null media header, overall  information (some tracks only)|
|||||dinf||√|data information box, container|
||||||dref|√|data reference box, declares  source(s) of media data in track|
|||||stbl||√|sample table box, container for the  time/space map|
||||||stsd|√|sample descriptions (codec types,  initialization etc.)|
||||||stts|√|(decoding) time-to-sample|
||||||ctts||(composition) time to sample|
||||||stsc|√|sample-to-chunk, partial  data-offsetinformation|
||||||stsz||sample sizes (framing)|
||||||stz2||compact sample sizes (framing)|
||||||stco|√|chunk offset, partial data-offset  information|
||||||co64||64-bit chunk offset|
||||||stss||sync sample table (random access  points)|
||||||stsh||shadow sync sample table|
||||||padb||sample padding bits|
||||||stdp||sample degradation priority|
||||||sdtp||independent and disposable samples|
||||||sbgp||sample-to-group|
||||||sgpd||sample group description|
||||||subs||sub-sample information|
||mvex||||||movie extends box|
|||mehd|||||movie extends header box|
|||trex||||√|track extends defaults|
||ipmc||||||IPMP Control Box|
|moof|||||||movie fragment|
||mfhd|||||√|movie fragment header|
||traf||||||track fragment|
|||tfhd||||√|track fragment header|
|||trun|||||track fragment run|
|||sdtp|||||independent and disposable samples|
|||sbgp|||||sample-to-group|
|||subs|||||sub-sample information|
|mfra|||||||movie fragment random access|
||tfra||||||track fragment random access|
||mfro|||||√|movie fragment random access offset|
|mdat|||||||media data container|
|free|||||||free space|
|skip|||||||free space|
||udta||||||user-data|
|||cprt|||||copyright etc.|
|meta|||||||metadata|
||hdlr|||||√|handler, declares the metadata  (handler) type|
||dinf||||||data information box, container|
|||dref|||||data reference box, declares  source(s) of metadata items|
||ipmc||||||IPMP Control Box|
||iloc||||||item location|
||ipro||||||item protection|
|||sinf|||||protection scheme information box|
||||frma||||original format box|
||||imif||||IPMP Information box|
||||schm||||scheme type box|
||||schi||||scheme information box|
||iinf||||||item information|
||xml||||||XML container|
||bxml||||||binary XML container|
||pitm||||||primary item reference|
||fiin||||||file delivery item information|
|||paen|||||partition entry|
||||fpar||||file partition|
||||fecr||||FEC reservoir|
|||segr|||||file delivery session group|
|||gitn|||||group id to name|
|||tsel|||||track selection|
|meco|||||||additional metadata container|
||mere||||||metabox relation|


在MP4文件中显示的排版方式与上面表格描述基本无差别，当然，因为MP4的标准中描述的moov与mdat的存放位置前后并没有进行强制要求，所以有些时候moov这个Container在mdat的后面，有些时候moov被存放在mdat的前面，在互联网的视频点播中，如果希望MP4文件被快速的打开时，则需要moov Container存放在mdat的前面，如果放在后面，需要将MP4文件下载完成后才可以进行播放。

解析mp4多媒体文件时，需要一些关键的信息，下面介绍一下主要的信息；

1.	moov 容器，前面表格中已经介绍过，这个容器是定义了一个mp4文件中的数据信息，类型是moov，是一个容器atom，至少必须包含三种atom中的一种

    a)	mvhd标签，Movie Header Atom，存放未压缩过的影片信息的头容器；

    b)	cmov标签，Compressed Movie Atom，压缩过的电影信息容器；

    c)	rmra标签，Reference Movie Atom，参考电影信息容器。

也可以包含其他的容器信息，例如影片剪辑信息Clipping atom(clip)，一个或几个Trak Atom(trak)，一个Color Table Atom(ctab)，和一个User Data Atom(udta)。

    a)	mvhd中定义了整部电影的time scale，duration，以及display characteristics.

    b)	trak 中定义了电影中的一个track的信息，track就是电影中可以独立操作的媒体单位，例如一个声道就是一个track，一个视频流就是一个track。


下面来打开一个mp4文件查看其内容，通过举例来了解这里所讲到的mp4文件容器信息：
![](http://blog.chinaunix.net/attachment/201608/23/11344913_1471949027CaCZ.png)

关于读取这个moov容器的方式，可以参考下面这个列表
|字段|长度(字节)|描述|
|----|----|----|
|尺寸|4|这个movie header atom的字节数|
|类型|4|moov|

根据解析这个moov的容器的字节长度，可以看到该容器共包含0x00000ca3(3235)个字节，容器的类型为moov；下面继续在moov这个容器中往下解析，下一个容器为的大小为0x0000006c(108)个字节，类型为mvhd；然后继续从moov容器中往下解析；
![](http://blog.chinaunix.net/attachment/201608/23/11344913_1471949078xV4v.png)

分析完mvhd之后，下一个moov中的容器可以从上图中看到是一个trak标签，这个trak容器的大小是0x00000517（1303）个字节，类型是trak。解析完这个trak之后，有进入到moov容器中解析下一个trak，下一个trak解析的方式与这个trak的解析方式相同，可以看到下图trak的大小为0x0000069c(1692)个字节；
![](http://blog.chinaunix.net/attachment/201608/23/11344913_1471949119uHhh.png)

解析完这个音频的trak之后，接下来可以看到还有一个moov容器中的子容器，就是udta容器，这个udta容器解析方式与前面解析trak的方式基本相同，可以从下图看到，udta的大小为0x0000007c(124)个字节，
![](http://blog.chinaunix.net/attachment/201608/23/11344913_1471949156VXCr.png)

可以根据前面描述过的信息得知，udta+视频trak+音频trak+mvhd+moov描述大小之后的出来的总大小，刚好为3235个字节，与前面得出来的moov的大小相等。

前面描述了针对moov容器下面的子容器的解析，接下来继续解析moov子容器中的子容器。

1. 解析mvhd子容器：
![](http://blog.chinaunix.net/attachment/201608/23/11344913_1471949208HXuU.png)

从图中可以看到，mvhd这个容器的大小为0x0000006c个字节，mvhd的解析方式如下表：
|字段|长度(字节)|描述|
|----|----|----|
|尺寸|4|这个movie header atom的字节数|
|类型|4|mvhd|
|版本|1|这个movie header atom的版本|
|标志|3|扩展的movie header标志，这里为0|
|生成时间|4|Movie atom的起始时间。基准时间是1904-1-1 0:00 AM|
|修订时间|4|Movie atom的修订时间。基准时间是1904-1-1 0:00 AM|
|Time scale|4|时间计算单位，就像是系统时间单位换位为60秒一样|
|Duration|4|通过这个值计算后可以得到影片的播放长度时间值|
|播放速度|4|播放此movie的速度。1.0为正常播放速度(16.16的浮点表示)|
|播放音量|2|播放此movie的音量。1.0为最大音量(8.8的浮点表示)|
|保留|10|这里为0|
|矩阵结构|36|该矩阵定义了此movie中两个坐标空间的映射关系|
|预览时间|4|开始预览此movie的时间|
|预览duration|4|以movie的time scale为单位，预览的duration|
|Poster time|4|Poster的时间值.|
|Selection time|4|当前选择的时间的开始时间值|
|Selection duration|4|当前选择的时间的计算后的时间值|
|当前时间|4|当前时间|
|下一个track ID|4|下一个待添加track的ID值。0不是一个有效的ID值。|


按照上表的方式解析出来的mvhd的内容对应的信息如下：
|字段|结论值|
|----|----|
|尺寸|0x0000006c|
|类型|mvhd|
|版本|0x00|
|标志|0x000000|
|生成时间|0x00000000|
|修订时间|0x00000000|
|Time scale|0x000003E8 (1000)|
|Duration|0x00000770 (1904)|
|播放速度|0x00010000（1.0）|
|播放音量|0x0100(1.0)|
|保留|0x00 00 00 00 00 00 00 00 00 00|
|矩阵结构|0x00010000,0,0,0,0x00010000,0,0,0,0x40000000|
|预览时间|0x00000000|
|预览duration|0x00000000|
|Poster time|0x00000000|
|Selection time|0x00000000|
|Selection duration|0x00000000|
|当前时间|0x00000000|
|下一个track ID|0x00000003|


解析mvhd之后，可以看到下一个trakID为0x00000003，接下来就开始解析trak，解析trak的时候同样也包含了多个子容器；



2.	解析Track子容器

track 容器中定义了影片中的一个track的信息，一个影片文件中可以包含多个tracks，每个track都是独立的，各有各的时间和空间占用的信息，每个track容器都有与他关联的media容器描述信息。Track容器的主要使用目的如下：

    a)	包含媒体数据的引用和描述(media track)

    b)	包含modifier trackers信息

    c)	流媒体协议的打包信息(hint tracks)，hint tracks可以引用或者复制对应的媒体采样数据。

Hint tracks和modifier tracks必须保证完整性，同时和至少一个media track一起存在。

一个trak容器中要求必须有一个Track Header Atom(tkhd)，一个Media Atom(mdia)，其他的atom都是可选的，例如

    a)	Track Clipping Atom(clip);

    b)	Track Matte Atom(matt);

    c)	Edit Atom(edts);

    d)	Track Reference Atom(tref);

 &nbnbsp;  e)	Track Load Settings Atom(load);

    f)	Track Input Map Atom(imap);

    g)	User Data Atom(udta)



解析的方式如下表:
|字段|长度(字节)|描述|
|----|----|----|
|尺寸|4|这个atom的大小|
|类型|4|tkhd/mdia/clip/matt等|


如下图：
![](http://blog.chinaunix.net/attachment/201608/23/11344913_1471949337Lk8E.png)

从上图可以看到，这个track的大小为0x00000517(1303)字节，下面的子容器的大小为0x0000005c(92)字节，这个子容器的类型为tkhd；跳过92字节后，接下来读到的trak的子容器的大小为0x00000024(36)字节，这个子容器的类型为edts；跳过36字节后，接下来读到的trak子容器的大小为0x0000048f(1167)个字节，这个子容器的类型为mdia；可以分析得到trak+tkhd+edts+mdia子容器的大小加起来刚好为1303字节，trak的读取完毕。



3.	tkhd容器的解析

解析tkhd容器的方式可以参考下面表格：
|字段|长度(字节)|描述|
|----|----|----|
|尺寸|4|这个atom的字节数|
|类型|4|tkhd|
|版本|1|这个atom的版本|
|标志|3|有效的标志是·0x0001 - the track is enabled·0x0002 - the track is used in the movie·0x0004 - the track is used in the movie’s preview·0x0008 - the track is used in the movie’s poster|
|生成时间|4|Movie atom的起始时间。基准时间是1904-1-1 0:00 AM|
|修订时间|4|Movie atom的修订时间。基准时间是1904-1-1 0:00 AM|
|Track ID|4|唯一标志该track的一个非零值。|
|保留|4|这里为0|
|Duration|4|The duration of this track (in the  movie’s time coordinate system).Note that this property is derived from the  track’s edits. The value of this field is equal to the sum of the durations  of all of the track’s edits. If there is no edit list, then the duration is  the sum of the sample durations, converted into the movie timescale.|
|保留|8|这里为0|
|Layer|2|The track’s spatial priority in its  movie. The QuickTime Movie Toolbox uses this value to determine how tracks  overlay one another. Tracks with lower layer values are displayed in front of  tracks with higher layer values.|
|Alternate group|2|A collection of movie tracks that contain  alternate data for one another. QuickTime chooses one track from the group to  be used when the movie is played. The choice may be based on such  considerations as playback quality, language, or the capabilities of the  computer.|
|音量|2|播放此track的音量。1.0为正常音量|
|保留|2|这里为0|
|矩阵结构|36|该矩阵定义了此track中两个坐标空间的映射关系|
|宽度|4|如果该track是video track，此值为图像的宽度(16.16浮点表示)|
|高度|4|如果该track是video track，此值为图像的高度(16.16浮点表示)|

下面看看一下tkhd的内容，然后根据上面的表格做一个信息的对应
![](http://blog.chinaunix.net/attachment/201608/23/11344913_14719494187Mjd.png)

这个tkhd对应的值如下表：
|字段|长度(字节)|值|
|----|----|----|
|尺寸|4|0x0000005c(92)|
|类型|4|tkhd|
|版本|1|00|
|标志|3|0x000003(这个track生效并且用在这个影片中)|
|生成时间|4|0x00000000|
|修订时间|4|0x00000000|
|Track ID|4|0x00000001|
|保留|4|0x00000000|
|Duration|4|0x00000758|
|保留|8|0x00 00 00 00 00 00 00 00|
|Layer|2|0x0000|
|Alternate group|2|0x0000|
|音量|2|0x0000|
|保留|2|0x0000|
|矩阵结构|36|00 01  00 00 00 00 00 00    00 00 00 00 00 00  00 0000 01  00 00 00 00 00 00    00 00 00 00 00 00  00 0040 00  00 00|
|宽度|4|0x02800000 (640.00)|
|高度|4|0x01e00000 (480.00)|


以上表格为解析视频trak容器的tkhd，下面在分析一个音频的tkhd：
![](http://blog.chinaunix.net/attachment/201608/23/11344913_147194947498r0.png)

解析trak的方法前面已经讲过，这里现在重点解析音频的tkhd，并已表格形式将数据表示出来：
|字段|长度(字节)|值|
|----|----|----|
|尺寸|4|0x0000005c(92)|
|类型|4|tkhd|
|版本|1|00|
|标志|3|0x000003(这个track生效并且用在这个影片中)|
|生成时间|4|0x00000000|
|修订时间|4|0x00000000|
|Track ID|4|0x00000002|
|保留|4|0x00000000|
|Duration|4|0x00000770|
|保留|8|0x00 00 00 00 00 00 00 00|
|Layer|2|0x0000|
|Alternate group|2|0x0001|
|音量|2|0x0100|
|保留|2|0x0000|
|矩阵结构|36|00 01  00 00 00 00 00 00    00 00 00 00 00 00  00 0000 01  00 00 00 00 00 00    00 00 00 00 00 00  00 0040 00  00 00|
|宽度|4|0x00000000 (00.00)|
|高度|4|0x00000000 (00.00)|


从两个例子中可以看出，音频与视频的trak的tkhd的大小相同，里面的内容会随着音视频trak的类型不同有所不同。到这里trak的tkhd解析完毕。



4.	mdia容器的解析

解析完tkhd之后，接下来可以分析一下trak容器的子容器。

Media Atom的类型是mdia，是一个容器atom必须包含如下容器：

    a)	一个Media Header Atom(mdhd)

    b)	一个Handler Reference(hdlr)

    c)	一个媒体信息引用(minf)和User Data Atom（udta）

这个容器解析方式如下表：
|字段|长度(字节)|描述|
|----|----|----|
|尺寸|4|这个atom的大小|
|类型|4|mdia|

下面参考一下mp4文件的数据：
![](http://blog.chinaunix.net/attachment/201608/23/11344913_1471949556i4K9.png)

可以看到这个mdia容器的大小为0x0000048f(1167)个字节，mdia容器下面包含了三大子容器，分别为mdhd，hdlr和minf，其中mdhd大小为0x00000020(32)个字节；hdlr大小为0x0000002d(45)个字节；minf大小为0x0000043a(1082)个字节；mdia容器信息+mdhd+hdlr+minf容器大小刚好为1167字节；到这里mdia容器解析完毕。

4.1 mdhd容器的解析

mdhd容器中被包含在各个Track中，描述Media的Header，包含的信息如下表：
|字段|长度(字节)|描述|
|----|----|----|
|尺寸|4|这个atom的字节数|
|类型|4|mdhd|
|版本|1|这个atom的版本|
|标志|3|这里为0|
|生成时间|4|Movie  atom的起始时间。基准时间是1904-1-1 0:00 AM|
|修订时间|4|Movie  atom的修订时间。基准时间是1904-1-1 0:00 AM|
|Time  scale|4|时间计算单位|
|Duration|4|这个媒体Track的duration时长|
|语言|2|媒体的语言码|
|质量|2|媒体的回放质量|

根据ISO14496-Part2描述可以看到，当版本字段为0时，解析与当版本字段为1时解析稍微有所不同，这里介绍的为常见的解析方式。

下面根据表格的解析方式将对应的数据解析出来：
![](http://blog.chinaunix.net/attachment/201608/23/11344913_1471949640U3hu.png)

从图中可以逐一解析出来：
|字段|长度(字节)|值|
|----|----|----|
|尺寸|4|0x00000020(32)|
|类型|4|mdhd|
|版本|1|0x00|
|标志|3|0x000000|
|生成时间|4|0x00000000|
|修订时间|4|0x00000000|
|Time  scale|4|0x00003200(12800)|
|Duration|4|0x00005e00(24064)|
|语言|2|0x55c4|
|质量|2|0x0000|


从上表可以看出这个Media Header的大小是32字节，类型是mdhd，版本为0，生成时间与媒体修改时间都为0，计算单位时间是12800，媒体时间戳长度为24064，语言编码是0x55C4(具体代表的语言可以参考标准ISO 639-2/T)，到这里mdhd标签解析完毕。



4.2 hdlr 容器的解析

hdlr容器中描述了媒体流的播放过程，该容器中包含的内容如下表：
|字段|长度(字节)|描述|
|----|----|----|
|尺寸|4|这个atom的字节数|
|类型|4|hdlr|
|版本|1|这个atom的版本|
|标志|3|这里为0|
|Handle的类型|4|handler的类型。当前只有两种类型'mhlr'：media handlers'dhlr'：data handlers|
|Handle的子类型|4|media  handler or data handler的类型。如果component type是mhlr，这个字段定义了数据的类型，例如，'vide'是video数据，'soun'是sound数据如果component type是dhlr，这个字段定义了数据引用的类型，例如，'alis'是文件的别名|
|保留|12|保留字段，缺省为0|
|Component  name|可变|这个component的名字，也就是生成此media的media handler。该字段的长度可以为0|

根据这个表格的读取方式，读取示例文件中的内容数据，数据如下图：
![](http://blog.chinaunix.net/attachment/201608/23/11344913_147194971727s6.png)

根据图中的信息，可以将内容读取出来，对应的值如下：
|字段|长度(字节)|值|
|----|----|----|
|尺寸|4|0x0000002d(45)|
|类型|4|hdlr|
|版本|1|0x00|
|标志|3|0x00|
|Handle的预定义字段|4|0x00000000|
|Handle的子类型|4|Vide|
|保留|12|0x0000 0000 0000 0000 0000 0000|
|Component  name|可变|VideoHandler’\0’|

从上表中解析出来的对应的值可以看出来，这是一个视频的Track对应的数据，对应的组件的名称为VideoHandler和一个0x00结尾，hdlr容器解析完毕。


4.3 minf 容器的解析

minf容器中包含了很多重要的子容器，例如音视频相关的采样等信息相关的容器，minf容器中的信息将作为音视频数据的映射存在，其内容信息如下：

    a)	(Video Media Information Header)vmhd 子容器

    b)	(Sound Media Information Header)smhd 子容器

    c)	(Data Information)dinf 子容器

    d)	(Sample Table)stbl 子容器

解析minf的方式在前面已经介绍过，下面详细介绍一下解析vmhd、smhd、dinf以及stbl容器：

（1）	vmhd容器解析

vmhd容器内容的格式如下表：
|字段|长度(字节)|描述|
|----|----|----|
|尺寸|4|这个atom的字节数|
|类型|4|vmhd|
|版本|1|这个atom的版本|
|标志|3|这里总是0x000001|
|图形模式|2|传输模式，传输模式指定的布尔值|
|Opcolor|6|颜色值，RGB颜色值|

根据这个表格读取容器中的内容，进行解析，其数据如下图：
![](http://blog.chinaunix.net/attachment/201608/23/11344913_14719498157Zwp.png)

将数据解析出来，对应值如下表：
|字段|长度(字节)|值|
|----|----|----|
|尺寸|4|0x00000014|
|类型|4|vmhd|
|版本|1|0x00|
|标志|3|0x000001|
|图形模式|2|0x0000|
|Opcolor|6|0x0000  0000 0000|


上面这个表格为视频的Header的解析，下面看一下音频的Header解析

（2）	smhd容器解析

smhd容器的格式如下表：
|字段|长度(字节)|描述|
|----|----|----|
|尺寸|4|这个atom的字节数|
|类型|4|smhd|
|版本|1|这个atom的版本|
|标志|3|这里为0|
|均衡|2|音频的均衡是用来控制计算机的两个扬声器的声音混合效果，一般是0。一般值是0。|
|保留|2|保留字段，缺省为0|


根据这个表格解析文件中的音频对应的数据，数据如下图：
![](http://blog.chinaunix.net/attachment/201608/23/11344913_1471949876U021.png)

将数据解析出来后，对应的值如下表：
|字段|长度(字节)|值|
|----|----|----|
|尺寸|4|0x00000010|
|类型|4|smhd|
|版本|1|0x00|
|标志|3|0x000000|
|均衡|2|0x0000|
|保留|2|0x0000|



（3）	dinf容器解析

dinf容器是一个用以描述数据信息的容器，定义了音视频数据的信息，这是一个容器，它包含子容器dref。下面举一个解析dinf及其子容器dref的例子，dref解析方式如下表：
|字段|长度(字节)|描述|
|----|----|----|
|尺寸|4|这个atom的字节数|
|类型|4|dref|
|版本|1|这个atom的版本|
|标志|3|这里为0|
|条目数目|4|data  references的数目|
|数据参考||每个data reference就像容器的格式一样，包含以下的数据成员|
|尺寸|4|这个atom的字节数|
|类型|4|见下表|
|版本|1|这个data reference的版本|
|标志|3|目前只有一个标志：Self  referenceThis  flag indicates that the media’s data is in the same file as the movie atom.  On the Macintosh, and other file systems with multifork files, set this flag  to 1 even if the data resides in a different fork from the movie atom. This  flag’s value is 0x0001.|
|数据|可变|data  reference信息|



（4）	stbl容器解析

stbl容器为采样参数列表的容器(Sample Table atom)，该容器包含转化媒体时间到实际的sample的信息，也说明了解释sample的信息，例如，视频数据是否需要解压缩，解压缩算法是什么等信息。他包含子容器：

    a)	Sample Description Atom(stsd)

    b)	Time-To-Sample Atom(stts)

    c)	Sync Sample Atom(stss)

    d)	Sample-To-Chunk Atom(stsc)

    e)	Sample Size Atom(stsz)

    f)	Chunk Offset Atom(stco)

    g)	Shadow Sync Atom(stsh)

stbl包含track中media sample的所有时间和数据索引，利用这个表，就可以定位sample到媒体时间，决定其类型，大小，以及如何在其他容器中找到紧邻的sample。如果sample table atom所在的track没有引用任何数据，那么它就不是一个有用的media track，不需要包含任何子atom。

如果sample table atom所在的track引用了数据，那么必须包含以下的子atom：

    a)	Sample Description Atom(stsd)

    b)	Sample Size Atom(stsz)

    c)	Sample Size Atom(stsz)

    d)	Sample Size Atom(stsz)


所有的字表有相同的sample数目。

stbl是必不可少的一个atom，而且必须包含至少一个条目，因为它包含了数据引用atom检索media sample的目录信息。没有sample description，就不可能计算出media sample存储的位置。sync sample atom 是可选的，如果没有，表明所有的samples都是sync samples。


5.	edts容器的解析

edts容器定义了创建Movie影片中的一个track的一部分媒体，所有的edit都在一个表里，包括每一部分的时间偏移量和长度，如果没有该表，这个track就会被立即播放，一个空的edit用来便宜track的其实时间，如果没有edit atom或者edit list atom，则这个track会使用前部媒体。Edit atom这个容器很简单，解析方式如下表：
|字段|长度(字节)|描述|
|----|----|----|
|尺寸|4|这个atom的字节数|
|类型|4|edts|


Trak中的edts数据如下图：
![](http://blog.chinaunix.net/attachment/201608/23/11344913_14719500147g3Z.png)

这个Edit Atom的大小为0x00000024(36)个字节，类型为edta；其中包含了elst子容器，elst子容器的大小为0x0000001c(28)字节，edts容器+elst子容器的大小为36字节，到这里，edts容器解析完毕。








