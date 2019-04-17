# flv格式详解+实例剖析 - DoubleLi - 博客园







## 简介

FLV（Flash Video）是现在非常流行的流媒体格式，由于其视频文件体积轻巧、封装播放简单等特点，使其很适合在网络上进行应用，目前主流的视频网站无一例外地使用了FLV格式。另外由于当前浏览器与Flash Player紧密的结合，使得网页播放FLV视频轻而易举，也是FLV流行的原因之一。

FLV是流媒体封装格式，我们可以将其数据看为二进制字节流。总体上看，FLV包括文件头（File Header）和文件体（File Body）两部分，其中文件体由一系列的Tag及Tag Size对组成。






![](https://upload-images.jianshu.io/upload_images/2111324-520e9b3f32512aa4.jpg)


flv.jpg


## FLV格式解析

先来一张图，这是《东风破》——周杰伦（[下载](https://link.jianshu.com?t=http://ovjkwgfx6.bkt.clouddn.com/dongfengpo.flv)）的一个MV视频。我使用的是Binary Viewer的二进制查看工具。






![](https://upload-images.jianshu.io/upload_images/2111324-0f7238b17864341b.png)


1.png




### header

头部分由一下几部分组成

Signature(3 Byte)+Version(1 Byte)+Flags(1 Bypte)+DataOffset(4 Byte)
- signature 占3个字节

固定FLV三个字符作为标示。一般发现前三个字符为FLV时就认为他是flv文件。
- Version 占1个字节

标示FLV的版本号。这里我们看到是1
- Flags 占1个字节

内容标示。第0位和第2位,分别表示 video 与 audio 存在的情况.(1表示存在,0表示不存在)。截图看到是`0x05`，也就是`00000101`，代表既有视频，也有音频。
- DataOffset 4个字节

表示FLV的header长度。这里可以看到固定是9

### body

FLV的body部分是由一系列的back-pointers + tag构成
- back-pointers 固定4个字节，表示前一个tag的size。
- tag 分三种类型，video、audio、scripts。

#### tag组成

`tag type`+`tag data size`+`Timestamp`+`TimestampExtended`+`stream id`+ `tag data`
- type 1个字节。8为Audio,9为Video,18为scripts
- tag data size 3个字节。表示tag data的长度。从streamd id 后算起。
- Timestreamp 3个字节。时间戳
- TimestampExtended 1个字节。时间戳扩展字段
- stream id 3个字节。总是0
- tag data 数据部分

我们根据实例来分析：

看到第一个TAG

type=`0x12`=18。这里应该是一个scripts。

size=`0x000125`=293。长度为293。

timestreamp=`0x000000`。这里是scripts，所以为0

TimestampExtended =`0x00`。

stream id =`0x000000`

我们看一下TAG的data部分：






![](https://upload-images.jianshu.io/upload_images/2111324-bfc5319817e9d389.png)


3.png




#### tag的划分

图中红色部分是我标出的两个back-pointers，都是4个字节。而中间就是第一个TAG。那是怎么计算的呢？我们就以这个做个示例。
- 首先第一个back-pointers是`0x00000000`，那是因为后面是第一个TAG。所以他为0。
- 然后根据我们我们前面格式获取到size是`0x000125`。也就是说从stream id后面再加上293个字节就到了第一个TAG的末尾，我们数一下一下。stream id以前总共有24个字节。那么到第一个TAG结束，总共有293+24=137=`0x13D`。
- 接下来我们找到`0x13D`的地址，从工具上很容易找到，正好就是红色下划线的前面。红色部分是`0x00000130`=304，这代表的是上一个TAG的大小。
- 最后我们计算一下，上一个TAG数据部分是293个字节，前面type、stream id等字段占了11个字节。正好是匹配的。

上面我们已经知道了怎么取划分每个TAG。接下来我们就看TAG的具体内容

#### tag的内容

前面已经提到tag分3种。我们一个个看

##### script

脚本Tag一般只有一个，是flv的第一个Tag，用于存放flv的信息，比如duration、audiodatarate、creator、width等。

首先介绍下脚本的数据类型。所有数据都是以数据类型+（数据长度）+数据的格式出现的，数据类型占1byte，数据长度看数据类型是否存在，后面才是数据。

一般来说，该Tag Data结构包含两个AMF包。AMF（Action Message Format）是Adobe设计的一种通用数据封装格式，在Adobe的很多产品中应用，简单来说，AMF将不同类型的数据用统一的格式来描述。第一个AMF包封装字符串类型数据，用来装入一个“onMetaData”标志，这个标志与Adobe的一些API调用有，在此不细述。第二个AMF包封装一个数组类型，这个数组中包含了音视频信息项的名称和值。具体说明如下，大家可以参照图片上的数据进行理解。
|值|类型|说明|
|----|----|----|
|0|Number type|8 Bypte Double|
|1|Boolean type|1 Bypte bool|
|2|String type|后面2个字节为长度|
|3|Object type||
|4|MovieClip type||
|5|Null type||
|6|Undefined type||
|7|Reference type||
|8|ECMA array type|数组,类似Map|
|10|Strict array type||
|11|Date type||
|12|Long string type|后面4个字节为长度|





![](https://upload-images.jianshu.io/upload_images/2111324-62dc2554ef155833.png)


4.png



上图为第一个AMF包
- type=`0x02`对应String
- size=`0A`=10
- 
value=onMetaData 正好是10个字节。






![](https://upload-images.jianshu.io/upload_images/2111324-e4db97c919d6c886.png)


5.png



上图为第二个AMF

- type=`0x08` 对应ECMA array type。

> 
表示数组，类似Map。后面4个字节为数组的个数。然后是键值对，第一个为键，2个字节为长度。后面跟具体的内容。接着3个字节表示值的类型，然后根据类型判断长度。

上图我们可以判断，总共有13个键值对。

第一个长度为8个字节是duration。值类型是`0x004073`，第一个字节是00，所以是double，8个字节。

第二个长度5个字节是width。值也是double类型，8个字节。

依次解析下去...


到处，我们已经知道了如何解析FLV中Tag为script的数据。

##### video







![](https://upload-images.jianshu.io/upload_images/2111324-a75e5295c4096f01.png)


6.png



type=`0x09`=9。这里应该是一个video。

size=`0x000030`=48。长度为48。

timestreamp=`0x000000`。

TimestampExtended =`0x00`。

stream id =`0x000000`

我们看到数据部分：

视频信息+数据



###### 视频信息，1个字节。

前4位为帧类型Frame Type
|值|类型|
|----|----|
|1|keyframe (for AVC, a seekable frame)   关键帧|
|2|inter frame (for AVC, a non-seekable frame)|
|3|disposable inter frame (H.263 only)|
|4|generated keyframe (reserved for server use only)|
|5|video info/command frame|

后4位为编码ID    (CodecID)
|值|类型|
|----|----|
|1|JPEG (currently unused)|
|2|Sorenson H.263|
|3|Screen video|
|4|On2 VP6|
|5|On2 VP6 with alpha channel|
|6|Screen video version 2|
|7|AVC|

###### 特殊情况

视频的格式(**CodecID**)是AVC（H.264）的话，VideoTagHeader会多出4个字节的信息，AVCPacketType 和CompositionTime。
- AVCPacketType 占1个字节
|值|类型|
|----|----|
|0|AVCDecoderConfigurationRecord(AVC sequence header)|
|1|AVC NALU|
|2|AVC end of sequence (lower level NALU sequence ender is not required or supported)|

**AVCDecoderConfigurationRecord**.包含着是H.264解码相关比较重要的**sps**和**pps**信息，再给AVC解码器送数据流之前一定要把sps和pps信息送出，否则的话解码器不能正常解码。而且在解码器stop之后再次start之前，如seek、快进快退状态切换等，都需要重新送一遍sps和pps的信息.AVCDecoderConfigurationRecord在FLV文件中一般情况也是**出现1次**，也就是**第一个video tag**.
- CompositionTime 占3个字节
|条件|值|
|----|----|
|AVCPacketType ==1|Composition time offset|
|AVCPacketType !=1|0|

我们看第一个video tag，也就是前面那张图。我们看到AVCPacketType =0。而后面三个字节也是0。说明这个tag记录的是AVCDecoderConfigurationRecord。包含sps和pps数据。

再看到第二个video tag







![](https://upload-images.jianshu.io/upload_images/2111324-81454e31e2208b83.png)


8.png



我们看到 AVCPacketType =1，而后面三个字节为`000043`。这是一个视频帧数据。



解析到的数据完全符合上面的理论。

###### sps pps

前面我们提到第一个video 一般存放的是sps和pps。这里我们具体解析下sps和pps内容。先看下存储的格式（**图6**）：
`0x01`+`sps[1]`+`sps[2]`+`sps[3]`+`0xFF`+`0xE1`+`sps size`+`sps`+`01`+`pps size`+`pps`

我们看到图7 。

sps[1]=`0x64`

sps[2]=`00`

sps[3]=`0D`

sps size=`0x001B`=27

跳过27个字节后，是`0x01`

pps size=`0x0005`=118

跳过5个字节，就到了back-pointers。

###### 视频帧数据

解析出sps和pps tag后，后面的video tag就是真正的视频数据内容了






![](https://upload-images.jianshu.io/upload_images/2111324-2a5a21a2d867698d.png)


9.png



这是第二个video tag其实和图8一样，只是我圈出来关键信息。先看下格式

frametype=`0x17`=`00010111`

AVCPacketType =1

Composition Time=`0x000043`

后面就是NALU DATA



##### Audio

与视频格式类似

前4位为音频格式
|值|类型|
|----|----|
|0|Linear PCM, platform endian|
|1|ADPCM|
|2|MP3|
|3|Linear PCM, little endian|
|4|Nellymoser 16-kHz mono|
|5|Nellymoser 8-kHz mono|
|6|Nellymoser|
|7|G.711 A-law logarithmic PCM|
|8|G.711 mu-law logarithmic PCM|
|9|reserved|
|10|AAC|
|11|Speex|
|14|MP3 8-Khz|
|15|Device-specific sound|

接着2位为采样率
|值|类型|
|----|----|
|0|5.5-kHz|
|1|11-kHz|
|2|22-kHz|
|3|44-kHz|

> 
对于AAC总是3


接着1位为采样的长度
|值|类型|
|----|----|
|0|snd8Bit|
|1|snd16Bit|

> 
压缩过的音频都是16bit


接着1位为音频类型
|值|类型|
|----|----|
|0|sndMono|
|1|sndStereo|

> 
对于AAC总是1


我们看到第三个TAG





![](https://upload-images.jianshu.io/upload_images/2111324-eceef7ad27b503ba.png)


7.png



这个留给大家自己来解析吧。




from：https://www.jianshu.com/p/7ffaec7b3be6









