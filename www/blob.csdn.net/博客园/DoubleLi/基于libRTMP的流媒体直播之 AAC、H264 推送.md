# 基于libRTMP的流媒体直播之 AAC、H264 推送 - DoubleLi - 博客园






 这段时间在捣腾基于 RTMP 协议的流媒体直播框架，其间参考了众多博主的文章，剩下一些细节问题自行琢磨也算摸索出个门道，现将自己认为比较恼人的 AAC 音频帧的推送和解析、H264 码流的推送和解析以及网上没说清楚的地方分享给各位。

        RTMP 协议栈的实现，Bill 直接使用的 [libRTMP](http://rtmpdump.mplayerhq.hu/)，关于 libRTMP 的编译、基本使用方法，以及简单的流媒体直播框架，请参见博文[[C++实现RTMP协议发送H.264编码及AAC编码的音视频](http://www.cnblogs.com/haibindev/archive/2011/12/29/2305712.html)]，言简意赅，故不再赘述。

        言归正传，我们首先来看看 AAC 以及 H264 的推送。

        不论向 RTMP 服务器推送音频还是视频，都需要按照 FLV 的格式进行封包。因此，在我们向服务器推送第一个 AAC 或 H264 数据包之前，需要首先推送一个音频 Tag [AAC Sequence Header] 以下简称“音频同步包”，或者视频 Tag [AVC Sequence Header] 以下简称“视频同步包”。



AAC 音频帧的推送                                         

        我们首先来看看音频 Tag，根据 [FLV 标准](http://www.adobe.com/content/dam/Adobe/en/devnet/flv/pdfs/video_file_format_spec_v10.pdf) Audio Tags 一节的描述：



![wKioL1Qje6_ApXbFAALZEhnUQhw347.jpg](http://s3.51cto.com/wyfs02/M00/4A/29/wKioL1Qje6_ApXbFAALZEhnUQhw347.jpg)

![wKioL1Qje7CzZWgCAADA_wp5OpM894.jpg](http://s3.51cto.com/wyfs02/M02/4A/29/wKioL1Qje7CzZWgCAADA_wp5OpM894.jpg)

![wKiom1QjjFvDUS-PAADRrM6v_UU397.jpg](http://s3.51cto.com/wyfs02/M01/4A/29/wKiom1QjjFvDUS-PAADRrM6v_UU397.jpg)

        我们可以将其简化并得到 AAC 音频同步包的格式如下：

















![wKiom1Qj3lqRKafiAAKNXyQMvTU565.jpg](http://s3.51cto.com/wyfs02/M00/4A/31/wKiom1Qj3lqRKafiAAKNXyQMvTU565.jpg)

        音频同步包大小固定为 4 个字节。前两个字节被称为 [AACDecoderSpecificInfo]，用于描述这个音频包应当如何被解析。后两个字节称为 [AudioSpecificConfig]，更加详细的指定了音频格式。

        [AACDecoderSpecificInfo] 俩字节可以直接使用 FAAC 库的 faacEncGetDecoderSpecificInfo 函数来获取，也可以根据自己的音频源进行计算。一般情况下，双声道，44kHz 采样率的 AAC 音频，其值为 0xAF00，示例代码：







![wKioL1QjvBOTgyzaAAGVe-V9kmI359.jpg](http://s3.51cto.com/wyfs02/M02/4A/2E/wKioL1QjvBOTgyzaAAGVe-V9kmI359.jpg)

        根据 [FLV 标准](http://www.adobe.com/content/dam/Adobe/en/devnet/flv/pdfs/video_file_format_spec_v10.pdf) 不难得知，[AACDecoderSpecificInfo] 第 1 个字节高 4 位 |1010| 代表音频数据编码类型为 AAC，接下来 2 位 |11| 表示采样率为 44kHz，接下来 1 位 |1| 表示采样点位数16bit，最低 1 位 |1| 表示双声道。其第二个字节表示数据包类型，0 则为 AAC 音频同步包，1 则为普通 AAC 数据包。

        音频同步包的后两个字节 [AudioSpecificConfig] 的结构，援引其他博主图如下：



![wKioL1QiuO7zrhUwAAJxI9ZTnCM355.jpg](http://s3.51cto.com/wyfs02/M01/4A/1D/wKioL1QiuO7zrhUwAAJxI9ZTnCM355.jpg)

        我们只需参照上述结构计算出对应的值即可。至此，4 个字节的音频同步包组装完毕，便可推送至 RTMP 服务器，示例代码如下：









![wKiom1Qjwf_AhpYBAALewqMU8R4358.jpg](http://s3.51cto.com/wyfs02/M01/4A/2D/wKiom1Qjwf_AhpYBAALewqMU8R4358.jpg)

        网上有博主说音频采样率小于等于 44100 时 SamplingFrequencyIndex 应当选择3（48kHz），Bill 测试发现采样率等于 44100 时设置标记为 3 或 4 均能正常推送并在客户端播放，不过我们还是应当按照标准规定的行事，故此处的 SamplingFrequencyIndex 选 4。

        完成音频同步包的推送后，我们便可向服务器推送普通的 AAC 数据包，推送数据包时，[AACDecoderSpecificInfo] 则变为 0xAF01，向服务器说明这个包是普通 AAC 数据包。后面的数据为 AAC 原始数据去掉前 7 个字节（若存在 CRC 校验，则去掉前 9 个字节），我们同样以一张简化的表格加以阐释：









![wKiom1Qj3mqCw5lHAAIa-4cP-8I493.jpg](http://s3.51cto.com/wyfs02/M01/4A/31/wKiom1Qj3mqCw5lHAAIa-4cP-8I493.jpg)

        推送普通 AAC 数据包的示例代码：









![wKioL1QjwrvxaltsAAK8YUN-Lxc350.jpg](http://s3.51cto.com/wyfs02/M00/4A/2F/wKioL1QjwrvxaltsAAK8YUN-Lxc350.jpg)

        至此，我们便完成了 AAC 音频的推送流程。此时可尝试使用 VLC 或其他支持 RTMP 协议的播放器连接到服务器测试正在直播的 AAC 音频流。     





H264 码流的推送                                           

        前面提到过，向 RTMP 服务器发送 H264 码流，需要按照 FLV 格式进行封包，并且首先需要发送视频同步包 [AVC Sequence Header]。我们依旧先阅读 [FLV 标准](http://www.adobe.com/content/dam/Adobe/en/devnet/flv/pdfs/video_file_format_spec_v10.pdf) Video Tags 一节：

![wKioL1QjxnHgHEnEAAKJgSNqtus964.jpg](http://s3.51cto.com/wyfs02/M01/4A/30/wKioL1QjxnHgHEnEAAKJgSNqtus964.jpg)

![wKiom1QjxgTxHxcGAAHIvqsTyqY918.jpg](http://s3.51cto.com/wyfs02/M00/4A/2E/wKiom1QjxgTxHxcGAAHIvqsTyqY918.jpg)

        由于视频同步包前半部分比较简单易懂，仔细阅读上述标准便可明白如何操作，故 Bill 不另作图阐释。由上图可知，我们的视频同步包 FrameType == 1，CodecID == 7，VideoData == AVCVIDEOPACKET，继续展开 AVCVIDEOPACKET，我们可以得到 AVCPacketType == 0x00，CompositionTime == 0x000000，Data == AVCDecoderConfigurationRecord。

        因此构造视频同步包的关键点便是构造 AVCDecoderConfigurationRecord。同样，我们援引其他博主的图片来阐释这个结构的细节：

![wKiom1QjyPqD1WfpAAL6V06Ylu8204.jpg](http://s3.51cto.com/wyfs02/M01/4A/2E/wKiom1QjyPqD1WfpAAL6V06Ylu8204.jpg)

        其中需要额外计算的是 H264 码流的 Sps 以及 Pps，这两个关键数据可以在开始编码 H264 的时候提取出来并加以保存，在需要时直接使用即可。具体做法请读者自行 Google 或参见 [参考博文[2]](http://www.codeman.net/2014/01/439.html)，在此不再赘述。

        当我们得到本次 H264 码流的 Sps 以及 Pps 的相关信息后，我们便可以完成视频同步包的组装，示例代码如下：







![wKiom1Qjzaaji__hAAKucP6fUmk422.jpg](http://s3.51cto.com/wyfs02/M01/4A/2E/wKiom1Qjzaaji__hAAKucP6fUmk422.jpg)



![wKioL1Qj2FiScNksAAL966Ultw0411.jpg](http://s3.51cto.com/wyfs02/M02/4A/31/wKioL1Qj2FiScNksAAL966Ultw0411.jpg)



        至此，视频同步包便构造完毕并推送给 RTMP 服务器。接下来只需要将普通 H264 码流稍加封装便可实现 H264 直播，下面我们来看一下普通视频包的组装过程。

        回顾 [FLV 标准](http://www.adobe.com/content/dam/Adobe/en/devnet/flv/pdfs/video_file_format_spec_v10.pdf) 的 Video Tags 一节，我们可以得到 H264 普通数据包的封包信息，FrameType == （H264 I 帧 ? 1 : 2），CodecID == 7，VideoData == AVCVIDEOPACKET，继续展开，我们可以得到  AVCPacketType == 0x01，CompositionTime 此处仍然设置为 0x000000，具体原因 TODO(billhoo)，Data == H264 NALU Size + NALU Raw Data。

        构造视频数据包的示例代码如下：



![wKiom1Qj2_XiM6C9AAHC8RxCixU908.jpg](http://s3.51cto.com/wyfs02/M02/4A/30/wKiom1Qj2_XiM6C9AAHC8RxCixU908.jpg)

![wKioL1Qj3Brwx8vTAAF2JsPqjeg495.jpg](http://s3.51cto.com/wyfs02/M00/4A/32/wKioL1Qj3Brwx8vTAAF2JsPqjeg495.jpg)

        至此 H264 码流的整个推送流程便已完成，我们可以使用 VLC 或其他支持 RTMP 协议的播放器进行测试。



关于 AAC 音频帧及 H264 码流的时间戳         

        通过前文的步骤我们已经能够将 AAC 音频帧以及 H264 码流正常推送到 RTMP 直播服务器，并能够使用相关播放器进行播放。但播放的效果如何还取决于时间戳的设定。

        在网络良好的情况下，自己最开始使用的音频流时间戳为 AAC 编码器刚输出一帧的时间，视频流时间戳为 H264 编码器刚编码出来一帧的时间，VLC 播放端就频繁报异常，要么是重新缓冲，要么直接没声音或花屏。在排除了推送步骤实现有误的问题后，Bill 发现问题出在时间戳上。

        之后有网友说直播流的时间戳不论音频还是视频，在整体时间线上应当呈现递增趋势。由于 Bill最开始的时间戳计算方法是按照音视频分开计算，而音频时戳和视频时戳并不是在一条时间线上，这就有可能出现音频时戳在某一个时间点比对应的视频时戳小， 在某一个时间点又跳变到比对应的视频时戳大，导致播放端无法对齐。

        目前采用的时间戳为底层发送 RTMP 包的时间，不区分音频流还是视频流，统一使用即将发送RTMP 包的系统时间作为该包的时间戳。目前局域网测试播放效果良好，音视频同步且流畅。



参考博文

[1][[C++实现RTMP协议发送 H.264 编码及 AAC 编码的音视频](http://www.cnblogs.com/haibindev/archive/2011/12/29/2305712.html)]

[2][[使用 libRtmp 进行 H264 与 AAC 直播](http://www.codeman.net/2014/01/439.html)]

[3][[RTMP直播到FMS中的AAC音频直播](http://niulei20012001.blog.163.com/blog/static/7514721120130694144813/)]









