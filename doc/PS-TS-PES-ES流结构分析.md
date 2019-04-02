# PS-TS-PES-ES流结构分析

2019年02月24日 00:13:52

 

knowledgebao

 

阅读数：18

 

标签： [TS](http://so.csdn.net/so/search/s.do?q=TS&t=blog)[PS](http://so.csdn.net/so/search/s.do?q=PS&t=blog)[PES](http://so.csdn.net/so/search/s.do?q=PES&t=blog)[ES](http://so.csdn.net/so/search/s.do?q=ES&t=blog)[PCR](http://so.csdn.net/so/search/s.do?q=PCR&t=blog) 更多

个人分类： [音视频相关](https://blog.csdn.net/knowledgebao/article/category/8403818)



相关链接：<https://blog.csdn.net/knowledgebao/article/details/84776869>

------

**目录**

[概念普及：](https://blog.csdn.net/knowledgebao/article/details/86535637#%E6%A6%82%E5%BF%B5%E6%99%AE%E5%8F%8A%EF%BC%9A)

[TS层(ts header+adaption+PAT/PMT)](https://blog.csdn.net/knowledgebao/article/details/86535637#TS%E5%B1%82(ts%20header%2Badaption%2BPAT%2FPMT))

[ts header](https://blog.csdn.net/knowledgebao/article/details/86535637#ts%20header)

[adaption](https://blog.csdn.net/knowledgebao/article/details/86535637#adaption)

[PAT/PMT](https://blog.csdn.net/knowledgebao/article/details/86535637#PAT%2FPMT)

[pes层](https://blog.csdn.net/knowledgebao/article/details/86535637#pes%E5%B1%82)

[es层](https://blog.csdn.net/knowledgebao/article/details/86535637#es%E5%B1%82)

------

# 概念普及：

- PS：是MPEG2的一种分装格式，目前可用于GB28181等协议中，PS格式包括：PS header，PS system header，PS system map，PES层，ES层，具体可以参考[PS封包到rtp中](https://blog.csdn.net/knowledgebao/article/details/84964586)，适用于没有误差产生的媒体存储，如DVD等存储介质；
- TS：是ISOIEC 13818-1的标准，主要用于数字高清电视以及HLS直播协议等，TS层三个部分：ts header、adaptation field、payload以及一些表，比如PAT/PMT等。ts header固定4个字节；adaptation field可能存在也可能不存在，主要作用是给不足188字节的数据做填充；payload是pes数据。PAT/PMT跟在ts header之后，PAT表主要的作用就是指明了PMT表的PID值。PMT表主要的作用就是指明了音视频流的PID值。适用于有信道噪声产生的传输，目前TS流广泛应用于广播电视中，如机顶盒，HLS等。其中PAT和PMT是节目专用信息（**Program Special Information， PSI**）的一部分
- PES：是Packetized Elementary Stream的简称，是将原始ES流打包后形成的，再将PES经过不同的打包方式可以组成MPEG program stream 和 MPEG transport stream，即PS流和TS流。也就是说TS和PS下边的PES封包是一致的。
- ES: 层指的就是音视频编码后数据，比如h.264视频和aac音频等。

**TS流和PS流的区别：**TS流的包结构长度是固定的；PS流的包结构是可变长度的。这导致了TS流的**抵抗传输误码**的能力强于PS流（TS码流由于采用了固定长度的包结构，当传输误码破坏了某一TS包的同步信息时，接收机可在固定的位置检测它后面包中的同步信息，从而恢复同步，避免了信息丢失。而PS包由于长度是变化的，一旦某一 PS包的同步信息丢失，接收机无法确定下一包的同步位置，就会造成失步，导致严重的信息丢失。因此，在信道环境较为恶劣，传输误码较高时，一般采用TS码流；而在信道环境较好，传输误码较低时，一般采用PS码流。由于TS码流具有较强的抵抗传输误码的能力，因此目前在传输媒体中进行传输的MPEG-2码流基本上都采用了TS码流的包格。

所以关系如下表：

| PS流 | PS header | PS system header             | PES层 | ES层 |      |
| ---- | --------- | ---------------------------- | ----- | ---- | ---- |
| ES流 | TS header | (PAT/PMT...)adaptation field | PES层 | ES层 |      |

 

 

 

这个只只介绍TS相关，其中PS相关，可以参考[PS封包到rtp中](https://blog.csdn.net/knowledgebao/article/details/84964586)

**TS流背景介绍：**

在介绍具体字段，参数这些头疼，烦人的东西之前，我觉得有必要先介绍下TS流的应用背景，有了这个概念，再去深入学习，将如虎添翼。TS流最经典的应用就是我们平时生活中的数字高清电视。我们看的电视码流就是TS封装格式的码流，电视码流发送过来后，就会由我们的机顶盒进行解封装，解码，然后传给电视机进行播放。这里就有一个问题，我们看电视，有很多的频道，节目，对应码流是怎么区分的呢？（TIPS，频道和节目的关系，比如我们有中央电视台综合频道，下属CCTV-1~CCTV14这些节目）TS流引入了PAT和PMT两张表格的概念来解决这个问题。

TS流是以每188字节为一包，我们可以称为ts packet。这个ts packet有可能是音视频数据，也有可能是表格。举例说明，TS流的包顺序为：PAT，PMT，DATA，DATA，，，，，，PAT，PMT，DATA，DATA，，，，，，每隔一段时间，发送一张PAT表，紧接着发送一张PMT表，接着发送DATA（音视频）数据。那么你可能要问了，有了这2张表格怎么区分频道，节目呢？PAT表格里面包含所有PMT表格的信息，一个PMT表格对应一个频道，比如中央电视台综合频道。而一个PMT里面包含所有节目的信息，比如CCTV1~CCTV14。在实际情况中我们是有很多频道的，所以PMT表格可不止一张，有可能是PAT，PMT，PMT，PMT，，，DATA，DATA，，，，PAT，PMT，PMT，，，DATA，DATA这样的形式。除了这个设定外，每个频道或节目都有自己的标识符（PID），这样当我们拿到一个DATA，解析出里面的PID，就知道是什么节目，并且也知道所属频道是什么了。我们看电视的时候，会收到所有节目的DATA，当我们正在看某个节目的时候，机顶盒会把这个节目的DATA单独过滤出来，其它的舍弃。

TS层三个部分：ts header、adaptation field、payload以及一些表，比如PAT/PMT等。ts header固定4个字节；adaptation field可能存在也可能不存在，主要作用是给不足188字节的数据做填充；payload是pes数据。PAT/PMT表跟在ts header之后，PAT表主要的作用就是指明了PMT表的PID值。PMT表主要的作用就是指明了音视频流的PID值。

![img](http://static.oschina.net/uploads/img/201604/27100307_wUox.png)

## TS层(ts header+adaption+PAT/PMT)

ts层     ts包大小固定为188字节，ts层分为三个部分：ts header、adaptation field、payload。ts header固定4个字节；adaptation field可能存在也可能不存在，主要作用是给不足188字节的数据做填充；payload是pes数据。

### ts header

| sync_byte                    | 8b   | 同步字节，固定为0x47                                         |
| ---------------------------- | ---- | ------------------------------------------------------------ |
| transport_error_indicator    | 1b   | 传输错误指示符，表明在ts头的adapt域后由一个无用字节，通常都为0，这个字节算在adapt域长度内 |
| payload_unit_start_indicator | 1b   | 负载单元起始标示符，一个完整的数据包开始时标记为1            |
| transport_priority           | 1b   | 传输优先级，0为低优先级，1为高优先级，通常取0                |
| pid                          | 13b  | pid值                                                        |
| transport_scrambling_control | 2b   | 传输加扰控制，00表示未加密                                   |
| adaptation_field_control     | 2b   | 是否包含自适应区，‘00’保留；‘01’为无自适应域，仅含有效负载；‘10’为仅含自适应域，无有效负载；‘11’为同时带有自适应域和有效负载。 |
| continuity_counter           | 4b   | 递增计数器，从0-f，起始值不一定取0，但必须是连续的           |

 ts层的内容是通过PID值来标识的，主要内容包括：PAT表、PMT表、音频流、视频流。解析ts流要先找到PAT表，只要找到PAT就可以找到PMT，然后就可以找到音视频流了。PAT表的PID值固定为0。PAT表和PMT表需要定期插入ts流，因为用户随时可能加入ts流，这个间隔比较小，通常每隔几个视频帧就要加入PAT和PMT。PAT和PMT表是必须的，还可以加入其它表如SDT（业务描述表）等，不过hls流只要有PAT和PMT就可以播放了。

- PAT表：他主要的作用就是指明了PMT表的PID值。
- PMT表：他主要的作用就是指明了音视频流的PID值。
- 音频流/视频流：承载音视频内容。

### adaption

| adaptation_field_length | 1B   | 自适应域长度，后面的字节数                                   |
| ----------------------- | ---- | ------------------------------------------------------------ |
| flag                    | 1B   | 取0x50表示包含PCR或0x40表示不包含PCR                         |
| PCR                     | 5B   | Program Clock Reference，节目时钟参考，用于恢复出与编码端一致的系统时序时钟STC（System Time Clock）。 |
| stuffing_bytes          | xB   | 填充字节，取值0xff                                           |

  自适应区的长度要包含传输错误指示符标识的一个字节。pcr是节目时钟参考，pcr、dts、pts都是对同一个系统时钟的采样值，pcr是递增的，因此可以将其设置为dts值，音频数据不需要pcr。如果没有字段，[ipad](https://www.baidu.com/s?wd=ipad&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)是可以播放的，但vlc无法播放。打包ts流时PAT和PMT表是没有adaptation field的，不够的长度直接补0xff即可。视频流和音频流都需要加adaptation field，通常加在一个帧的第一个ts包和最后一个ts包里，中间的ts包不加。

**PCR关于同步说明：**

PCR用来同步前端编码器和后端[机顶盒](https://www.baidu.com/s?wd=%E6%9C%BA%E9%A1%B6%E7%9B%92&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)的时钟，在TS的传输过程中，一般DTS和PCR差值会在一个合适的范围，这个差值就是要设置的视音频Buffer的大小，一般情况下视频DTS和PCR的差值在700ms~1200ms之间，音频差值在200ms-700ms之间，具体详见参考文档3.

[![img](http://static.oschina.net/uploads/img/201604/27100307_FmWL.png)](http://bbs.baofengcloud.com/data/attachment/album/201502/26/135937lzdslzno7wzowyyz.png%5C%22+target=%5C%22_blank)

### PAT/PMT

| table_id                 | 8b   | PAT表固定为0x00                                           |
| ------------------------ | ---- | --------------------------------------------------------- |
| section_syntax_indicator | 1b   | 固定为1                                                   |
| zero                     | 1b   | 固定为0                                                   |
| reserved                 | 2b   | 固定为11                                                  |
| section_length           | 12b  | 后面数据的长度                                            |
| transport_stream_id      | 16b  | 传输流ID，固定为0x0001                                    |
| reserved                 | 2b   | 固定为11                                                  |
| version_number           | 5b   | 版本号，固定为00000，如果PAT有变化则版本号加1             |
| current_next_indicator   | 1b   | 固定为1，表示这个PAT表可以用，如果为0则要等待下一个PAT表  |
| section_number           | 8b   | 固定为0x00                                                |
| last_section_number      | 8b   | 固定为0x00                                                |
| 开始循环                 |      |                                                           |
| program_number           | 16b  | 节目号为0x0000时表示这是NIT，节目号为0x0001时,表示这是PMT |
| reserved                 | 3b   | 固定为111                                                 |
| PID                      | 13b  | 节目号对应内容的PID值                                     |
| 结束循环                 |      |                                                           |
| CRC32                    | 32b  | 前面数据的CRC32校验码                                     |

PMT格式

| table_id                 | 8b   | PMT表取值随意，0x02                                          |
| ------------------------ | ---- | ------------------------------------------------------------ |
| section_syntax_indicator | 1b   | 固定为1                                                      |
| zero                     | 1b   | 固定为0                                                      |
| reserved                 | 2b   | 固定为11                                                     |
| section_length           | 12b  | 后面数据的长度                                               |
| program_number           | 16b  | 频道号码，表示当前的PMT关联到的频道，取值0x0001              |
| reserved                 | 2b   | 固定为11                                                     |
| version_number           | 5b   | 版本号，固定为00000，如果PAT有变化则版本号加1                |
| current_next_indicator   | 1b   | 固定为1                                                      |
| section_number           | 8b   | 固定为0x00                                                   |
| last_section_number      | 8b   | 固定为0x00                                                   |
| reserved                 | 3b   | 固定为111                                                    |
| PCR_PID                  | 13b  | PCR(节目参考时钟)所在TS分组的PID，指定为视频PID              |
| reserved                 | 4b   | 固定为1111                                                   |
| program_info_length      | 12b  | 节目描述信息，指定为0x000表示没有                            |
| 开始循环                 |      |                                                              |
| stream_type              | 8b   | 流类型，标志是Video还是Audio还是其他数据，h.264编码对应0x1b，aac编码对应0x0f，mp3编码对应0x03 |
| reserved                 | 3b   | 固定为111                                                    |
| elementary_PID           | 13b  | 与stream_type对应的PID                                       |
| reserved                 | 4b   | 固定为1111                                                   |
| ES_info_length           | 12b  | 描述信息，指定为0x000表示没有                                |
| 结束循环                 |      |                                                              |
| CRC32                    | 32b  | 前面数据的CRC32校验码                                        |

## pes层

pes层是在每一个视频/音频帧上加入了时间戳等信息，pes包内容很多，我们只留下最常用的。

[![img](http://static.oschina.net/uploads/img/201604/27100307_NNv8.png)](http://bbs.baofengcloud.com/data/attachment/album/201502/26/140117aqkyndu59ou54dab.png%5C%22+target=%5C%22_blank)

| pes start code    | 3B   | 开始码，固定为0x000001                                       |
| ----------------- | ---- | ------------------------------------------------------------ |
| stream id         | 1B   | 音频取值（0xc0-0xdf），通常为0xc0 视频取值（0xe0-0xef），通常为0xe0 |
| pes packet length | 2B   | 后面pes数据的长度，0表示长度不限制， 只有视频数据长度会超过0xffff |
| flag              | 1B   | 通常取值0x80，表示数据不加密、无优先级、备份的数据           |
| flag              | 1B   | 取值0x80表示只含有pts，取值0xc0表示含有pts和dts              |
| pes data length   | 1B   | 后面数据的长度，取值5或10                                    |
| pts               | 5B   | 33bit值                                                      |
| dts               | 5B   | 33bit值                                                      |

​     pts是显示时间戳、dts是解码时间戳，视频数据两种时间戳都需要，音频数据的pts和dts相同，所以只需要pts。有pts和dts两种时间戳是B帧引起的，I帧和P帧的pts等于dts。如果一个视频没有B帧，则pts永远和dts相同。从文件中顺序读取视频帧，取出的帧顺序和dts顺序相同。dts算法比较简单，初始值 + 增量即可，pts计算比较复杂，需要在dts的基础上加偏移量。

​     音频的pes中只有pts（同dts），视频的I、P帧两种时间戳都要有，视频B帧只要pts（同dts）。打包pts和dts就需要知道视频帧类型，但是通过容器格式我们是无法判断帧类型的，必须解析h.264内容才可以获取帧类型。

举例说明：

​                         I          P          B          B          B          P

读取顺序：         1         2          3          4          5          6

dts顺序：           1         2          3          4          5          6

pts顺序：           1         5          3          2          4          6

**点播视频dts算法：**

dts = 初始值 + 90000 / video_frame_rate，初始值可以随便指定，但是最好不要取0，video_frame_rate就是帧率，比如23、30。

pts和dts是以timescale为单位的，1s = 90000 time scale , [一帧](https://www.baidu.com/s?wd=%E4%B8%80%E5%B8%A7&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)就应该是90000/video_frame_rate 个timescale。

用一帧的timescale除以采样频率就可以转换为一帧的播放时长

**点播音频dts算法：**

dts = 初始值 + (90000 * audio_samples_per_frame) / audio_sample_rate，audio_samples_per_frame这个值与编解码相关，aac取值1024，mp3取值1158，audio_sample_rate是采样率，比如24000、41000。AAC一帧解码出来是每声道1024个sample，也就是说一帧的时长为1024/sample_rate秒。所以每一帧时间戳依次0，1024/sample_rate，...，1024*n/sample_rate秒。

**直播视频的dts和pts**应该直接用直播数据流中的时间，不应该按公式计算。

## es层

es层指的就是音视频数据，我们只介绍h.264视频。

**h.264视频：**

​     打包h.264数据我们必须给视频数据加上一个nalu（Network Abstraction Layer unit），nalu包括nalu header和nalu type，nalu header固定为0x00000001（帧开始）或0x000001（帧中）。h.264的数据是由slice组成的，slice的内容包括：视频、sps、[pps](https://www.baidu.com/s?wd=pps&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)等。nalu type决定了后面的h.264数据内容。nalu占8个字节，具体内容如下：

| F    | 1b   | forbidden_zero_bit，h.264规定必须取0                         |
| ---- | ---- | ------------------------------------------------------------ |
| NRI  | 2b   | nal_ref_idc，取值0~3，指示这个nalu的重要性，I帧、sps、pps通常取3，P帧通常取2，B帧通常取0 |
| Type | 5b   | 参考下表                                                     |

| nal_unit_type | 说明                     |
| ------------- | ------------------------ |
| 0             | 未使用                   |
| 1             | 非IDR图像片，IDR指关键帧 |
| 2             | 片分区A                  |
| 3             | 片分区B                  |
| 4             | 片分区C                  |
| 5             | IDR图像片，即关键帧      |
| 6             | 补充增强信息单元(SEI)    |
| 7             | SPS序列参数集            |
| 8             | PPS图像参数集            |
| 9             | 分解符                   |
| 10            | 序列结束                 |
| 11            | 码流结束                 |
| 12            | 填充                     |
| 13~23         | 保留                     |
| 24~31         | 未使用                   |

​     nalType为1，5，6，7，8，9是最常用的，打包es层数据时pes头和es数据之间要加入一个type=9的nalu，关键帧slice前必须要加入type=7和type=8的nalu，而且是紧邻。

 

参考资料：

1. [TS流格式小白入门解读](https://blog.csdn.net/yuhengyue/article/details/78134329)
2. [ts流格式详解](https://blog.csdn.net/dxpqxb/article/details/79654004)
3. [TS之PCR](https://blog.csdn.net/evsqiezi/article/details/51781057)

------

有任何问题，请联系knowledgebao@163.com

 