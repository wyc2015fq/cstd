# MP3文件格式解析 - xiahouzuoxin - CSDN博客





2012年08月13日 15:50:18[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：4548标签：[header																[layer																[extension																[存储																[audio																[扩展](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)
个人分类：[Image/Audio/ML...](https://blog.csdn.net/xiahouzuoxin/article/category/1203619)






**转一：**笔者根据德国Fraunhofer  
 IIS   (http://www.iis.fhg.de/amm/   )公布的MPEG   Audio解码程序进行简化后编写了一个最简MP3解码程序。有兴趣的读者可以参考对应的源程序(http://www.see.online.

sh.cn/ch/sw/self/rainplay/layer3.zip)。

　　MP3文件由多个帧组成，也就是说帧是MP3音乐文件的最小组成单位。每个帧又由帧头和帧数据组成。每个帧头长4字节。其数据结构如下：

　　typedef   struct   _tagHeader   {

　　unsigned   int   sync:12;   //同步信息

　　unsigned   int   version:1;   //版本

　　unsigned   int   layer:2;   //层

　　unsigned   int   error_protection:1;   //CRC校正

　　unsigned   int   bit_rate_index:4;   //位率索引

　　unsigned   int   sample_rate_index:2;   //采样率索引

　　unsigned   int   padding:1;   //空白字

　　unsigned   int   extension:1;   //私有标志

　　unsigned   int   channel_mode:2;   //立体声模式

　　unsigned   int   mode   extension:2;   //保留

　　unsigned   int   copyright:1;   //版权标志

　　unsigned   int   original:1;   //原始媒体

　　unsigned   int   emphasis:2;   //强调方式

　　}   HEADER,   ＊LPHEADER;

　　其中帧同步标记为0xFFF。

　　在帧头后边是Side   Info(姑且称之为通道信息)。对标准的立体声MP3文件来说其长度为32字节。通道信息后面是Scale   factor(增益因子)信息。当解码器在读到上述信息后，就可以进行解码了。

　　当MP3文件被打开后，播放器首先试图对帧进行同步，然后分别读取通道信息及增益因子等数据，再进行霍夫曼解码，至此我们已经获得解压后的数据。但这些数据仍然不能进行播放，它们还处于频域，要想听到歌曲还要将它由频域通过特定的手段转换到时域。接下来的处理分别为立体化处理；抗锯齿处理；IMDCT变换；IDCT变换及窗口化滑动处理。

**转二**

转载自：http://blog.csdn.net/xiaoyao888/archive/2007/01/26/1493761.aspx

最近又在做MP3相关的程序，需要了解mp3的文件格式，于是就从网上找了一些资料，并将内容整理如下，加入了自己的一些理解。



　　MP3 文件是由帧(frame)构成的，帧是MP3 文件最小的组成单位。MP3 的全称应为MPEG1 Layer-3 音频文件，MPEG(Moving Picture Experts Group)在汉语中译为活动图像专家组，特指活动影音压缩标准，MPEG 音频文件是MPEG1 标准中的声音部分，也叫MPEG 音频层，它根据压缩质量和编码复杂程度划分为三层，即 Layer-1、Layer2、Layer3，且分别对应MP1、MP2、MP3
 这三种声音文件，并根据不同的用途，使用不同层次的编码。MPEG音频编码的层次越高，编码器越复杂，压缩率也越高，MP1 和MP2 的压缩率分别为4：1 和 6：1-8：1，而MP3 的压缩率则高达10：1-12：1，也就是说，一分钟CD 音质的音乐，未经压缩需要10MB的存储空间，而经过MP3 压缩编码后只有1MB 左右。不过MP3 对音频信号采用的是有损压缩方式，为了降低声音失真度，MP3 采取了“感官编码技术”，即编码时先对音频文件进行频谱分析，然后用过滤器滤掉噪音电平，接着通过量化的方式将剩下的每一位打散排列，最后形成具有较高压缩比的MP3
 文件，并使压缩后的文件在回放时能够达到比较接近原音源的声音效果。


**一、MPEG音频压缩基础**

　　在众多音频压缩方法中，这些方法在保持声音质量的同时尽量压缩数字音频使之占用更小的存储空间。MPEG压缩是该领域中效果最好的一个。这种压缩是有损压缩，这意味着，当运用这一方法压缩时肯定会丢失一部分音频信息。但是，由于压缩方法的控制很难发现这种损失。使用几个非常复杂和苛刻的数学算法，使得只有原始音频中几乎听不到的部分损失掉。这就给重要的信息剩下了更多的空间。通过这种方法可以将音频压缩12倍（可以选择压缩率），效果显著。正是应为他的质量，MPEG音频变得流行起来。

MPEG-1，MPEG-2和MPEG-4都是人们熟悉的MPEG标准，MP3只涉及到前两中，另外还有一个非官方标准MPEG-2.5用于扩展MPEG-2/LSF到更低的采样率。

MPEG-1音频（ISO/IEC 11172-3）描述了具有如下属性的三层音频编码：

1或2个声道

采样频率为32kHz，44.1kHz或48kHz

位率从32kbps到448kbps

每一层都有自己的优点。

MPEG-2音频（ISO/IEC 13818-3）有两个MPEG-1的扩展，通常叫做MPEG-2/LSF和MPEG-2/Multichannel

MPEG-2/LSF有如下特点：

1或2个声道

采样频率为MPEG-1的一半

波特率从8kbps256kbps

MPEG-2/Mutichannel有如下特点：

多达5个声道和1个LFE-通道（低频增强 不是重低音）

同MPEG-1一样的采样频率

5.1的最高波特率可能达到1Mbps

**二、MPEG Layer3编/解码的基本原理**

　　音乐CD具有44.1KHz 16Bits 立体声的音频质量，一张CD可以存储74分钟的歌曲(大约15首左右)。如何将这些歌曲无损或基本无损地进行压缩,以使在同样的媒体上存储更多的歌曲,一直困扰着软件业。当MPEG协会提出MPEG Audio Layer1～Layer3后，机会产生了。通过使用MPEG1 Layer3编码技术，制作者得以用大约12∶1的压缩率记录16KHz带宽的有损音乐信号。不过,同CD原声区别不大。人的听力系统具有非常优越的性能，其动态范围超过96dB。你既可以听到扣子掉在地上这样小的声音，也可以听到波音747的强大的轰鸣声。但当我们站在飞机场听着波音747的轰鸣时,
 你还能分辨出扣子掉在地上的声音吗？不可能。人的听力系统适应声音的动态变化，人们对这种适应及屏蔽特性音质研究后得出对声音压缩非常有用的理论。人们很早以前就知道利用这种特性来为磁带录音降低噪音了(当没有音乐时嘶嘶声很容易听到，而当音乐信号电平很高时嘶嘶声不容易听到)。当声音较强时产生屏蔽效应。在阈值曲线下的噪音或小信号声音无法被人耳听到。在较强信号出现时，允许通过更多的信号。在此时增加被量化过的小信号数据(使用无用的位来携带更多的信息)可以达到一定程度的压缩的目的。通常情况下,MP3压缩器将原始声音通过FFT(快速傅立叶变换)变化到频域，然后通过一定的算法算出何种频率声音可以携带更多的信息。而在还原时解码器所需要做的仅仅是将其从频域再变换回来。

**三、整个MP3文件结构：**

MP3文件大体分为三部分：TAG_V2(ID3V2)，Frame, TAG_V1(ID3V1)


|ID3V2|包含了作者，作曲，专辑等信息，长度不固定，扩展了ID3V1的信息量。|
|----|----|
|Frame**.****.****.**Frame|一系列的帧，个数由文件大小和帧长决定每个FRAME的长度可能不固定，也可能固定，由位率bitrate决定每个FRAME又分为帧头和数据实体两部分帧头记录了mp3的位率，采样率，版本等信息，每个帧之间相互独立|
|ID3V1|包含了作者，作曲，专辑等信息，长度为128BYTE。|



**四、MPEG音频帧格式**

　　一个MPEG音频文件是许多的称为帧的较小部分组成的，通常，帧是独立的组成部分。每一帧都拥有自己的头和音频信息。没有文件头。所以，我们可以剪切MPEG文件的任何部分并且能够正常播放（当然要分割到帧的结束处尽管许多程序会处理错误头）。在LayerIII中就并不是100%正确的。这是因为在MPEG-1LayerIII文件中的数据组织中，帧常常是互相关联的并且不能那样随便裁切。

　　当你想读取MPEG文件的信息时，通常只找到第一帧就足够了，读取它的头信息然后假设其它帧是相同的就可以。但这也不是所有情况。变比特率的MPEG文件使用使用所谓比特变换，也就是说每一帧的比特率依照具体内容变化。这种方法没有减少声音质量的帧将应用较低的波特率。这样就允许更好的压缩质量的同时又保证了高质量的音质。

帧头由每一帧的前4个字节（32位）组成。帧头的前11比特（或前12个位，见下文关于帧同步）总是固定的称作“帧同步”。因此，可以在整个文件中查找第一个帧同步（即：必须找到一个值为255的且其后跟着三到四个最高位置1的字节。）然后读取整个头检查值是否正确。关于头中每一个比特的具体含义应该验证那一个值的有效性可以操看下面的表格，如果存在被定义为保留，无效，损坏或不允许的值表明该头已经损坏。记住，光有这些是不够的，帧同步能在许多二进制文件里面的应用是很广的。而且，MPEG文件可能在开头包含可能有错误同步信息的垃圾，所以我们必须检查两个或者更多一些帧来确定我们现在读取的文件是一个MPEG文件。

帧可能还有CRC校验。如果存在的话，CRC校验紧跟在帧头之后，长为16比特。CRC校验之后是音频数据。计算出帧长度，如果你需要读取其他头或者计算该帧的CRC值，可以使用它比较文件中读出来的帧。验证MPEG头的有效性这是一个非常好的方法。

**1、帧头格式**


下面是一个头内容图示，使用字符A到M表示不同的区域。在表格中你可以看到每一区域的详细内容。

**AAAAAAAA AAABBCCD EEEEFFGH IIJJKLMM**


|符号i|长度(bits)|位置(bits)|描述| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
|A|11|(31-21)|帧同步（所有位置1）| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|B|2|(20,19)|MPEG 音频版本ID00 – MPEG 2.501 – 保留10 – MPEG 2 （ISO/IEC 13818-3）11 – MPEG 1 （ISO/IEC 11172-3）注：MPEG 2.5不是官方标准。帧头第20个比特用来表示2.5版本。不支持该版本的应用程序一般认为该比特位置位为帧同步位，也就是说帧同步（A）的长度为12而不是这里规定的11，这样B也就变成了1位（第19个位）。推荐使用该表的方法因为这样允许你可以区分三个版本以获得最高兼容性。| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|C|2|(18,17)|Layer描述00 - 保留01 - Layer III10 - Layer II11 - Layer I| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|D|1|(16)|校验位0 - 紧跟帧头后有16位即2个字节用作CRC校验1 - 没有校验| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|E|4|(15,12)|位率索引|索引值|MPEG 1|MPEG 2, 2.5 (LSF)| | | ||----|----|----|----|----|----||Layer I|Layer II|Layer III|Layer I|Layer II & III| ||0000|Free| | | | ||0001|32|32|32|32|8||0010|64|48|40|48|16||0011|96|56|48|56|24||0100|128|64|56|64|32||0101|160|80|64|80|40||0110|192|96|80|96|48||0111|224|112|96|112|56||1000|256|128|112|128|64||1001|288|160|128|144|80||1010|320|192|160|160|96||1011|352|224|192|176|112||1100|384|256|224|192|128||1101|416|320|256|224|144||1110|448|384|320|256|160||1111|Bad| | | | |注：所有值单位为kbps，而且1kbit=1000bit而不是1024bitFree表示空闲，如果固定比特率（这种文件不能变换比特率）和上表定义的不同，应该有应用程序决定。这种情况的实现应该只用于内部目的因为第三方应用程序是没有办法找出正确比特率的。但是这么做并不是很重要况且还浪费精力。Bad表示该值无效。MPEG文件可以有VBR。表示文件的比特率可以变化。我已经知道了两种惯用方法：比特率变换(bitrate switching)：每一帧都创建成不同的比特率。可以应用在任何层。LayerIII解码器必须支持该方法。LayerI和LayerII也可以支持。比特池(bit reservoir)：比特率可以使从前面的帧中借来的（受限），以便腾出空间来容纳输入信号部分。然而这样就导致各帧之间不再相互独立，意味着不能随便分割文件。这种方法只有LayerIII支持。LyaerII中有一些不被允许比特率组合和模式。下表是允许的组合。|bitrate|allowed modes||----|----||free|all||32|single channel||48|single channel||56|single channel||64|all||80|single channel||96|all||112|all||128|all||160|all||192|all||224|stereo, intensity stereo, dual channel||256|stereo, intensity stereo, dual channel||320|stereo, intensity stereo, dual channel||384|stereo, intensity stereo, dual channel||索引值|MPEG 1|MPEG 2, 2.5 (LSF)|Layer I|Layer II|Layer III|Layer I|Layer II & III|0000|Free|0001|32|32|32|32|8|0010|64|48|40|48|16|0011|96|56|48|56|24|0100|128|64|56|64|32|0101|160|80|64|80|40|0110|192|96|80|96|48|0111|224|112|96|112|56|1000|256|128|112|128|64|1001|288|160|128|144|80|1010|320|192|160|160|96|1011|352|224|192|176|112|1100|384|256|224|192|128|1101|416|320|256|224|144|1110|448|384|320|256|160|1111|Bad|bitrate|allowed modes|free|all|32|single channel|48|single channel|56|single channel|64|all|80|single channel|96|all|112|all|128|all|160|all|192|all|224|stereo, intensity stereo, dual channel|256|stereo, intensity stereo, dual channel|320|stereo, intensity stereo, dual channel|384|stereo, intensity stereo, dual channel|
|索引值|MPEG 1|MPEG 2, 2.5 (LSF)| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|Layer I|Layer II|Layer III|Layer I|Layer II & III| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|0000|Free| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|0001|32|32|32|32|8| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|0010|64|48|40|48|16| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|0011|96|56|48|56|24| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|0100|128|64|56|64|32| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|0101|160|80|64|80|40| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|0110|192|96|80|96|48| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|0111|224|112|96|112|56| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|1000|256|128|112|128|64| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|1001|288|160|128|144|80| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|1010|320|192|160|160|96| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|1011|352|224|192|176|112| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|1100|384|256|224|192|128| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|1101|416|320|256|224|144| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|1110|448|384|320|256|160| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|1111|Bad| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|bitrate|allowed modes| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|free|all| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|32|single channel| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|48|single channel| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|56|single channel| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|64|all| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|80|single channel| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|96|all| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|112|all| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|128|all| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|160|all| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|192|all| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|224|stereo, intensity stereo, dual channel| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|256|stereo, intensity stereo, dual channel| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|320|stereo, intensity stereo, dual channel| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|384|stereo, intensity stereo, dual channel| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|F|2|(11,10)|采样频率（单位：Hz）|bits|MPEG1|MPEG2|MPEG2.5||----|----|----|----||00|44100|22050|11025||01|48000|24000|12000||10|32000|16000|8000||11|保留| | ||bits|MPEG1|MPEG2|MPEG2.5|00|44100|22050|11025|01|48000|24000|12000|10|32000|16000|8000|11|保留| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|bits|MPEG1|MPEG2|MPEG2.5| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|00|44100|22050|11025| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|01|48000|24000|12000| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|10|32000|16000|8000| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|11|保留| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|G|1|(9)|填充位0 – 没有填充1 – 填充了一个额外的空位填充用来达到正确的比特率。例如：128k 44.1kHz LayerII使用了很多418bit或417bit长的帧来达到正确的128k比特率。LyaerI的空位有32bit长，LayerII和LayerIII的空位有8bit长。| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|H|1|(8)|私有bit，可以用来做特殊应用。例如可以用来触发应用程序的特殊事件。| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|I|2|(7,6)|声道00 立体声01 联合立体声（立体声）10 双声道（立体声）11 单声道（单声）注：双声道文件由二个独立的单声道组成。 每一个声道使用整个文件一半的位率。大多数的解码器把它当作立体声来输出，但是它并不总是这种情况。按我的理解就是是两个声道的信息是完全相同的，并不能把它当作立体声看待。| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|J|2|(5,4)|扩展模式（仅在联合立体声时有效）扩展模式用来连接对立体声效果无用的信息，来减少所需的资源。这两个位在联合立体声模式下有编码器动态指定。完整的MPEG文件的频率序列分成有32个子带。在LayerI和LayerII中这两个位确定强度立体声应用的频带。LayerIII中这两个位确定应用了哪一种联合立体声（M/S stereo或者Intensity stereo）频带由解压算法决定。|值|Layer I & II|Layer III| ||----|----|----|----||M/S stereo|Intensity stereo| | ||00|bands 4 to 31|off|off||01|bands 8 to 31|off|on||10|bands 12 to 31|on|off||11|bands 16 to 31|on|on||值|Layer I & II|Layer III|M/S stereo|Intensity stereo|00|bands 4 to 31|off|off|01|bands 8 to 31|off|on|10|bands 12 to 31|on|off|11|bands 16 to 31|on|on| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|值|Layer I & II|Layer III| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|M/S stereo|Intensity stereo| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|00|bands 4 to 31|off|off| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|01|bands 8 to 31|off|on| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|10|bands 12 to 31|on|off| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|11|bands 16 to 31|on|on| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|K|1|(3)|版权0无版权1有版权| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|L|1|(2)|原创0 原创拷贝1 原创| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|M|2|(1,0)|强调00 - 无01 - 50/15 ms10 - 保留11 - CCIT J.17| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |



关于读取帧头我使用了下面的方法

定义一个结构体

typedef struct frameHeader

{

unsigned int sync1:8; //同步信息1

unsigned int error_protection:1; //CRC校验

unsigned int layer:2; //层

unsigned int version:2; //版本

unsigned int sync2:3; //同步信息2

unsigned int extension:1; //版权

unsigned int padding:1; //填充空白字

unsigned int sample_rate_index:2; //采样率索引

unsigned int bit_rate_index:4; //位率索引

unsigned int emphasis:2; //强调方式

unsigned int original:1; //原始媒体

unsigned int copyright:1; //版权标志

unsigned int mode_extension:2; //扩展模式，仅用于联合立体声

unsigned int channel_mode:2; //声道模式

}FHEADER, *pFHEADER;

请注意我的同步信息分成了两个部分，而且其他的位的顺序也和上表列出的有所差别，这个主要是因为c语言在存取数据时总是从低位开始，而这个帧头是需要从高位来读取的。

读取方式如下

FHEADER header;

fread( &header, sizeof( FHEADER ), 1, streams );//这里假设文件已打开，读取位置已经指向帧头所在的位置

这样一次就可以读入帧头的所有信息了。

**2、如何计算帧长度**


我们首先区分两个术语：帧大小和帧长度。帧大小即每帧采样数表示一帧中采样的个数，这是恒定值。其值入下表所示


||MPEG 1|MPEG 2 (LSF)|MPEG 2.5 (LSF)|
|----|----|----|----|
|Layer I|384|384|384|
|Layer II|1152|1152|1152|
|Layer III|1152|576|576|



帧长度是压缩时每一帧的长度，包括帧头。它将填充的空位也计算在内。LayerI的一个空位长4字节，LayerII和LayerIII的空位是1字节。当读取MPEG文件时必须计算该值以便找到相邻的帧。

注意：因为有填充和比特率变换，帧长度可能变化。

从头中读取比特率，采样频率和填充，

LyaerI使用公式：

帧长度（字节） = (( 每帧采样数 / 8 * 比特率 ) / 采样频率 ) + 填充 * 4

LyerII和LyaerIII使用公式：

帧长度（字节）= (( 每帧采样数 / 8 * 比特率 ) / 采样频率 ) + 填充

例：

LayerIII 比特率 128000，采样频率 44100，填充0

=〉帧大小 417字节

**3、每帧的持续时间**


之前看了一些文章都说mp3的一帧的持续时间是26ms，结果在实际程序的编写中发现无法正确按时间定位到帧，然后又查了一些文章才知道，所谓26ms一帧只是针对MPEG1 Layer III而且采样率为44.1KHz来说是对的，但mp3文件并不都是如此，其实这个时间也是可以通过计算来获得，下面给出计算公式

每帧持续时间(毫秒) = 每帧采样数 / 采样频率 * 1000

这样通过计算可知 MPEG1 Layer III 采样率为44.1KHz的一帧持续时间为26.12...不是整数，不过我们权且认为它就是26毫秒吧。

如果是MPEG2 Layer III 采样率为16KHz的话那一帧要持续36毫秒，这个相差还是蛮大的，所以还是应该通过计算来获的，当然可以按MPEG版本，层数和采样率来建一个表，这样直接查表就可以知道时间了。

**4、CRC校验**

如果帧头的校验位为0，则帧头后就有一个16位的CRC值，这个值是big-endian的值，把这个值和该帧通过计算得出的CRC值进行比较就可以得知该帧是否有效。

关于CRC校验下面给出我找到的英文原文，我的英文水平不高，翻译的不行。


If the protection bit in the header is not set, the frame contains a 16 bit CRC (Cyclic Redundancy Checksum). This checksum directly follows the frame header and is a big-endian WORD. To verify this checksum you have to calculate it for the frame and compare
 the calculated CRC with the stored CRC. If they aren't equal probably a transfer error has appeared. It is also helpful to check the CRC to verify that you really found the beginning of a frame, because the sync bits do in same cases also occur within the
 data section of a frame.

The CRC is calculated by applying the CRC-16 algorithm (with the generator polynom 0x8005) to a part of the frame. The following data is considered for the CRC: the last two bytes of the header and a number of bits
 from the audio data which follows the checksum after the header. The checksum itself must be skipped for CRC calculation. Unfortunately there is no easy way to compute the number of frames which are necessary for the checksum calculation in Layer II. Therefore
 I left it out in the code. You would need other information apart from the header to calculate the necessary bits. However it is possible to compute the number of protected bits in Layer I and Layer III only with the information from the header.

For Layer III, you consider the complete side information for the CRC calculation. The side information follows the header or the CRC in Layer III files. It contains information about the general decoding of the
 frame, but doesn't contain the actual encoded audio samples. The following table shows the size of the side information for all Layer III files.


||MPEG 1|MPEG 2/2.5 (LSF)|
|----|----|----|
|Stereo, Joint Stereo, Dual Channel|32|17|
|Mono|17|9|



For Layer I files, you must consider the mode extension from the header. Then you can calculate the**number of bits**which are necessary for CRC calculation by applying the following formula:
4 * (number of channels * bound of intensity stereo + (32 - bound of intensity stereo));
This can be read as two times the number of stereo subbands plus the number of mono subbands and the result multiplied with 4. For simple mono frames, this equals 128, because the number of channels is one and the
 bound of intensity stereo is 32, meaning that there is no intensity stereo. For stereo frames this is 256. For more information have a look at the CRC code in the class CMPAFrame.

**5、帧数据**

在帧头后边是Side Info(姑且称之为通道信息)。对标准的立体声MP3文件来说其长度为32字节。通道信息后面是Scale factor(增益因子)信息。当解码器在读到上述信息后，就可以进行解码了。当MP3文件被打开后，播放器首先试图对帧进行同步，然后分别读取通道信息及增益因子等数据，再进行霍夫曼解码，至此我们已经获得解压后的数据。但这些数据仍然不能进行播放，它们还处于频域，要想听到歌曲还要将它由频域通过特定的手段转换到时域。接下来的处理分别为立体化处理；抗锯齿处理；IMDCT变换；IDCT变换及窗口化滑动处理。

我们知道，对于mp3来说现在有两种编码方式，一种是CBR，也就是固定位率，固定位率的帧的大小在整个文件中都是是固定的（公式如上所述），只要知道文件总长度，和从第一帧帧头读出的信息，就都可以通过计算得出这个mp3文件的信息，比如总的帧数，总的播放时间等等，要定位到某一帧或某个时间点也很方便，这种编码方式不需要文件头，第一帧开始就是音频数据。另一种是VBR，就是可变位率，VBR 是XING 公司推出的算法，所以在MP3 的FRAME
 里会有“Xing"这个关键字（也有用"Info"来标识的，现在很多流行的小软件也可以进行VBR 压缩，它们是否遵守这个约定，那就不得而知了），它存放在MP3文件中的第一个有效帧的数据区里，它标识了这个MP3文件是VBR的。同时第一个帧里存放了MP3 文件的帧的总个数，这就很容易获得了播放总时间，同时还有100个字节存放了播放总时间的100个时间分段的帧索引，假设4 分钟的MP3 歌曲，240S，分成100 段，每两个相邻INDEX 的时间差就是2.4S，所以通过这个INDEX，只要前后处理少数的FRAME，就能快速找出我们需要快进的帧头。其实这第一帧就相当于文件头了。不过现在有些编码器在编码CBR文件时也像VBR那样将信息记入第一帧，比如著名的lame，它使用"Info"来做CBR的标记。

**6、VBR 头**

这里列出VBR的第一帧存储文件信息的头的格式。有两种格式，一种是常见的XING Header（头部包含字符‘Xing’），另一种是VBRI Header（头部包含字符‘VBRI’）鉴于VBRI Header不常见，下面只说XING Header，关于VBRI Header请看[http://www.codeproject.com/audio/MPEGAudioInfo.asp](http://mpgedit.org/mpgedit/mpeg_format/mpeghdr.htm)。

XING Header的起始位置，相对于第一帧帧头的位置，单位是字节

36-39 "Xing" 文件为MPEG1并且不是单声道(大多数VBR的mp3文件都是如此)

21-24 "Xing" 文件为MPEG1并且是单声道

21-24 "Xing" 文件为MPEG2并且不是单声道

13-16 "Xing" 文件为MPEG2并且是单声道

XING Header格式


|位置（从‘Xing’标记开始)|长度|含义|举例|
|----|----|----|----|
|0|4|VBR头标记，4个字节的ASCII字符，内容为 'Xing' 或者 'Info'|'Xing'|
|4|4|指示VBR头具体内容的标记, 组合方式为逻辑或. 区域是强制的.0x0001 - 总帧数存储区域设置为存在，不包括第一帧0x0002 - 文件长度存储区域设置为存在，不包括标签0x0004 - TOC 索引存储区域设置为存在0x0008 - 质量指示存储区域设置为存在|0x0007(意味总帧数，文件长度，TOC的存储区有效)|
|8|4|存储总帧数的Big-Endian值|7344|
|8 or 12|4|存储文件长度Big-Endian值，单位为字节|45000|
|8, 12 or 16|100|100字节的 TOC 索引，用于快速定位对于这个区域的存储内容，我认为可有可无，因为用1个字节来索引一个几兆文件的一帧是不可能做到准确定位的，就我所见基本上所有的VBR的mp3文件的 TOC都几乎是相同的，就是把256平均分成100份然后填进去，其实和正确的值差不到哪里去，如果懒的话这么做也成吧，反正也是不准确的定位。TCO索引的计算方式如下(TOC[i] / 256) * 文件长度比如文件持续240秒，我需要跳到60秒，文件长度为5000000字节计算如下TOC[(60/240)*100] = TOC[25]然后相对于文件中的位置大约是在(TOC[25]/256) * 5000000如果要自己重建的话，基本是把这个步骤反过来做就可以了。要求准确的话，就需要根据时间点找到正确帧的位置然后再计算，我定位帧的做法都是从第一帧开始搜索，这样偏差我认为不会超过1帧，也比较准确，不过计算出来的TOC的值还是和偷懒的做法大同小异。||
|8, 12, 16, 108, 112 or 116|4|质量指示器，为0(最好)-100(最差)的Big-Endian值|0|



这样算来，XING Header包括帧头一共最多只需要156个字节就够了。当然也可以在XING Header后面存储编码器的信息，比如lame在其后就是存储其版本，这需要给第一帧留足够的空间才行。


至于mp3的信息用从XING Header读出的信息就可以计算

比如

总持续时间 = 总帧数 * 每帧采样数 / 采样率 （结果为秒）

平均位率 = 文件长度 / 总持续时间 * 8

**五、MPEG音频标签**

MPEG音频标签分为两种，一种是ID3v1，存在文件尾部，长度128字节，另一种是ID3v2，是对ID3v1的扩展，存在文件头部，长度不定。

**1、ID3v1**

ID3v1标签用来描述MPEG音频文件。包含艺术家，标题，唱片集，发布年代和流派。另外还有额外的注释空间。位于音频文件的最后固定为128字节。可以读取该文件的最后这128字节获得标签。

结构如下

AAABBBBB BBBBBBBB BBBBBBBB BBBBBBBB

BCCCCCCC CCCCCCCC CCCCCCCC CCCCCCCD

DDDDDDDD DDDDDDDD DDDDDDDD DDDDDEEE

EFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFG


|符号|长度 (bytes)|位置 (bytes)|描述|
|----|----|----|----|
|A|3|(0-2)|标签标志。如果存在标签并且正确的话，必须包含'TAG'。|
|B|30|(3-32)|标题|
|C|30|(33-62)|艺术家|
|D|30|(63-92)|唱片集|
|E|4|(93-96)|年代|
|F|30|(97-126)|注释|
|G|1|(127)|流派|

该规格要求所有的空间必须以空字符(ASCII 0)填充。但是并不是所有的应用程序遵循该规则，比如winamp就用空格(ASCII 32)代替之。

在ID3v1.1结构中有些改变。注释部分的最后一个字节用来定义唱片集中的轨道号。如果不知道该信息时可以用空字符(ASCII 0)代替。

流派使用原码表示，为下列数字之一：
|0|'Blues'|20|'Alternative'|40|'AlternRock'|60|'Top 40'|
|----|----|----|----|----|----|----|----|
|1|'Classic Rock'|21|'Ska'|41|'Bass'|61|'Christian Rap'|
|2|'Country'|22|'Death Metal'|42|'Soul'|62|'Pop/Funk'|
|3|'Dance'|23|'Pranks'|43|'Punk'|63|'Jungle'|
|4|'Disco'|24|'Soundtrack'|44|'Space'|64|'Native American'|
|5|'Funk'|25|'Euro-Techno'|45|'Meditative'|65|'Cabaret'|
|6|'Grunge'|26|'Ambient'|46|'Instrumental Pop'|66|'New Wave'|
|7|'Hip-Hop'|27|'Trip-Hop'|47|'Instrumental Rock'|67|'Psychadelic'|
|8|'Jazz'|28|'Vocal'|48|'Ethnic'|68|'Rave'|
|9|'Metal'|29|'Jazz+Funk'|49|'Gothic'|69|'Showtunes'|
|10|'New Age'|30|'Fusion'|50|'Darkwave'|70|'Trailer'|
|11|'Oldies'|31|'Trance'|51|'Techno-Industrial'|71|'Lo-Fi'|
|12|'Other'|32|'Classical'|52|'Electronic'|72|'Tribal'|
|13|'Pop'|33|'Instrumental'|53|'Pop-Folk'|73|'Acid Punk'|
|14|'R&B'|34|'Acid'|54|'Eurodance'|74|'Acid Jazz'|
|15|'Rap'|35|'House'|55|'Dream'|75|'Polka'|
|16|'Reggae'|36|'Game'|56|'Southern Rock'|76|'Retro'|
|17|'Rock'|37|'Sound Clip'|57|'Comedy'|77|'Musical'|
|18|'Techno'|38|'Gospel'|58|'Cult'|78|'Rock & Roll'|
|19|'Industrial'|39|'Noise'|59|'Gangsta'|79|'Hard Rock'|

Winamp扩充了这个表
|80|'Folk'|92|'Progressive Rock'|104|'Chamber Music'|116|'Ballad'|
|----|----|----|----|----|----|----|----|
|81|'Folk-Rock'|93|'Psychedelic Rock'|105|'Sonata'|117|'Poweer Ballad'|
|82|'National Folk'|94|'Symphonic Rock'|106|'Symphony'|118|'Rhytmic Soul'|
|83|'Swing'|95|'Slow Rock'|107|'Booty Brass'|119|'Freestyle'|
|84|'Fast Fusion'|96|'Big Band'|108|'Primus'|120|'Duet'|
|85|'Bebob'|97|'Chorus'|109|'Porn Groove'|121|'Punk Rock'|
|86|'Latin'|98|'Easy Listening'|110|'Satire'|122|'Drum Solo'|
|87|'Revival'|99|'Acoustic'|111|'Slow Jam'|123|'A Capela'|
|88|'Celtic'|100|'Humour'|112|'Club'|124|'Euro-House'|
|89|'Bluegrass'|101|'Speech'|113|'Tango'|125|'Dance Hall'|
|90|'Avantgarde'|102|'Chanson'|114|'Samba'|||
|91|'Gothic Rock'|103|'Opera'|115|'Folklore'| | |

其他扩充
|126|'Goa'|132|'BritPop'|138|'BlackMetal'|144|'TrashMetal'|
|----|----|----|----|----|----|----|----|
|127|'Drum&Bass'|133|'Negerpunk'|139|'Crossover'|145|'Anime'|
|128|'Club-House'|134|'PolskPunk'|140|'ContemporaryChristian'|146|'JPop'|
|129|'Hardcore'|135|'Beat'|141|'ChristianRock'|147|'Synthpop'|
|130|'Terror'|136|'ChristianGangstaRap'|142|'Merengue'|||
|131|'Indie'|137|'HeavyMetal'|143|'Salsa'|||



其他任何的数值都认为是“unknown”

**2、ID3V2**

ID3V2 到现在一共有4 个版本，但流行的播放软件一般只支持第3 版，既ID3v2.3。由于ID3V1 记录在MP3 文件的末尾，ID3V2 就只好记录在MP3 文件的首部了(如果有一天发布ID3V3，真不知道该记录在哪里)。也正是由于这个原因，对ID3V2 的操作比ID3V1 要慢。而且ID3V2 结构比ID3V1 的结构要复杂得多，但比前者全面且可以伸缩和扩展。

下面就介绍一下ID3V2.3。

每个ID3V2.3 的标签都一个标签头和若干个标签帧或一个扩展标签头组成。关于曲目的信息如标题、作者等都存放在不同的标签帧中，扩展标签头和标签帧并不是必要的，但每个标签至少要有一个标签帧。标签头和标签帧一起顺序存放在MP3 文件的首部。


（一）、标签头


在文件的首部顺序记录10 个字节的ID3V2.3 的头部。数据结构如下：


char Header[3]; /*必须为"ID3"否则认为标签不存在*/

char Ver; /*版本号ID3V2.3 就记录3*/

char Revision; /*副版本号此版本记录为0*/

char Flag; /*存放标志的字节，这个版本只定义了三位，稍后详细解说*/

char Size[4]; /*标签大小，包括标签头的10 个字节和所有的标签帧的大小*/
注:对这里我有疑惑，因为在实际寻找首帧的过程中，我发现有的mp3文件的标签大小是不包含标签头的，但有的又是包含的，可能是某些mp3编码器写标签的BUG，所以为了兼容只好认为其是包含的，如果按大小找不到，再向后搜索，直到找到首帧为止。


（1）.标志字节


标志字节一般为0，定义如下：

abc00000

a -- 表示是否使用Unsynchronisation(这个单词不知道是什么意思，字典里也没有找到，一般不设置)

b -- 表示是否有扩展头部，一般没有(至少Winamp 没有记录)，所以一般也不设置

c -- 表示是否为测试标签(99.99%的标签都不是测试用的啦，所以一般也不设置)

（2）.标签大小

一共四个字节，但每个字节只用7 位，最高位不使用恒为0。所以格式如下

0xxxxxxx 0xxxxxxx 0xxxxxxx 0xxxxxxx

计算大小时要将0 去掉，得到一个28 位的二进制数，就是标签大小(不懂为什么要这样做)，计算公式如

下：

int total_size;

total_size = (Size[0]&0x7F)*0x200000

+(Size[1]&0x7F)*0x4000

+(Size[2]&0x7F)*0x80

+(Size[3]&0x7F)


（二）、标签帧


每个标签帧都有一个10 个字节的帧头和至少一个字节的不固定长度的内容组成。它们也是顺序存放在文件

中，和标签头和其他的标签帧也没有特殊的字符分隔。得到一个完整的帧的内容只有从帧头中的到内容大

小后才能读出，读取时要注意大小，不要将其他帧的内容或帧头读入。

帧头的定义如下：

char FrameID[4]; /*用四个字符标识一个帧，说明其内容，稍后有常用的标识对照表*/

char Size[4]; /*帧内容的大小，不包括帧头，不得小于1*/

char Flags[2]; /*存放标志，只定义了6 位，稍后详细解说*/


（1）.帧标识


用四个字符标识一个帧，说明一个帧的内容含义，常用的对照如下：

TIT2=标题 表示内容为这首歌的标题，下同

TPE1=作者

TALB=专集

TRCK=音轨 格式：N/M 其中N 为专集中的第N 首，M 为专集中共M 首，N 和M 为ASCII 码表示的数字

TYER=年代 是用ASCII 码表示的数字

TCON=类型 直接用字符串表示

COMM=备注 格式："eng/0 备注内容"，其中eng 表示备注所使用的自然语言


（2）.大小


这个可没有标签头的算法那么麻烦，每个字节的8 位全用，格式如下

xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx

算法如下：

int FSize;

FSize = Size[0]*0x100000000

+Size[1]*0x10000

+Size[2]*0x100

+Size[3];


（3）.标志


只定义了6 位，另外的10 位为0，但大部分的情况下16 位都为0 就可以了。格式如下：

abc00000 ijk00000

a -- 标签保护标志，设置时认为此帧作废

b -- 文件保护标志，设置时认为此帧作废

c -- 只读标志，设置时认为此帧不能修改(但我没有找到一个软件理会这个标志)

i -- 压缩标志，设置时一个字节存放两个BCD 码表示数字

j -- 加密标志(没有见过哪个MP3 文件的标签用了加密)

k -- 组标志，设置时说明此帧和其他的某帧是一组

值得一提的是winamp 在保存和读取帧内容的时候会在内容前面加个'/0'，并把这个字节计算在帧内容的

大小中。


附：帧标识的含义

（4）. Declared ID3v2 frames


The following frames are declared in this draft.

AENC Audio encryption

APIC Attached picture

COMM Comments

COMR Commercial frame

ENCR Encryption method registration

EQUA Equalization

ETCO Event timing codes

GEOB General encapsulated object

GRID Group identification registration

IPLS Involved people list

LINK Linked information

MCDI Music CD identifier

MLLT MPEG location lookup table

OWNE Ownership frame

PRIV Private frame

PCNT Play counter

POPM Popularimeter

POSS Position synchronisation frame

RBUF Recommended buffer size

RVAD Relative volume adjustment

RVRB Reverb

SYLT Synchronized lyric/text

SYTC Synchronized tempo codes

TALB Album/Movie/Show title

TBPM BPM (beats per minute)

TCOM Composer

TCON Content type

TCOP Copyright message

TDAT Date

TDLY Playlist delay

TENC Encoded by

TEXT Lyricist/Text writer

TFLT File type

TIME Time

TIT1 Content group description

TIT2 Title/songname/content description

TIT3 Subtitle/Description refinement

TKEY Initial key

TLAN Language(s)

TLEN Length

TMED Media type

TOAL Original album/movie/show title

TOFN Original filename

TOLY Original lyricist(s)/text writer(s)

TOPE Original artist(s)/performer(s)

TORY Original release year

TOWN File owner/licensee

TPE1 Lead performer(s)/Soloist(s)

TPE2 Band/orchestra/accompaniment

TPE3 Conductor/performer refinement

TPE4 Interpreted, remixed, or otherwise modified by

TPOS Part of a set

TPUB Publisher

TRCK Track number/Position in set

TRDA Recording dates

TRSN Internet radio station name

TRSO Internet radio station owner

TSIZ Size

TSRC ISRC (international standard recording code)

TSSE Software/Hardware and settings used for encoding

TYER Year

TXXX User defined text information frame

UFID Unique file identifier

USER Terms of use

USLT Unsychronized lyric/text transcription

WCOM Commercial information

WCOP Copyright/Legal information

WOAF Official audio file webpage

WOAR Official artist/performer webpage

WOAS Official audio source webpage

WORS Official internet radio station homepage

WPAY Payment

WPUB Publishers official webpage

WXXX User defined URL link frame

以上文字绝大多数来源于网络，当中也包含一些我自己的理解，如果有错请指正。

部分参考文章的网址

[http://mpgedit.org/mpgedit/mpeg_format/mpeghdr.htm](http://mpgedit.org/mpgedit/mpeg_format/mpeghdr.htm)
[http://www.codeproject.com/audio/MPEGAudioInfo.asp](http://www.codeproject.com/audio/MPEGAudioInfo.asp)
[http://le-hacker.org/hacks/mpeg-drafts/11172-3.pdf (ISO/IEC 11172-3 我想这个应该有很多人找吧，不过这里面定义的帧同步位为12位，因为是老标准)](http://le-hacker.org/hacks/mpeg-drafts/11172-3.pdf)
[http://webstore.iec.ch/preview/info_isoiec13818-3%7Bed2.0%7Den.pdf （ISO/IEC 13818-3
 网站似乎是收费的，不过直接可下，应该不会有人找我麻烦吧）](http://webstore.iec.ch/preview/info_isoiec13818-3%7Bed2.0%7Den.pdf)](https://so.csdn.net/so/search/s.do?q=audio&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=extension&t=blog)](https://so.csdn.net/so/search/s.do?q=layer&t=blog)](https://so.csdn.net/so/search/s.do?q=header&t=blog)




