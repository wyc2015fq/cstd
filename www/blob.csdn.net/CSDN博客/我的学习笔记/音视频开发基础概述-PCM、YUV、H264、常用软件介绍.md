# 音视频开发基础概述 - PCM、YUV、H264、常用软件介绍 - 我的学习笔记 - CSDN博客





2018年07月28日 00:56:56[zouzhiheng](https://me.csdn.net/u011330638)阅读数：871








# 前言

相对而言，音视频开发算是有些门槛的。记得我第一次接触的时候，看别人的博客都看不懂。特别是写代码的时候，非常痛苦，只能抄别人的代码，却不知道为什么要这么写，也不知道应该怎么调整。后来总结了一下，痛苦的原因是在写代码之前没有掌握相关的基础知识，因此现在特地写了这样一篇博客，希望对大家有所帮助。

另，这篇博客主要是对自己做的笔记的一些总结，前前后后参考了许多文章和一些书籍，内容之间的连贯性或许不够好，也可能存在一些错误，如果发现了还望评论指正。

# 基本概念

### 声波

声波有三要素：

1) 频率，代表音调，即常说的高音、低音。频率越低，波形越长，穿越障碍物时能量衰减越小，可传播距离越远。

2) 振幅，代表响度，即音量大小。

3) 波形，代表音色，同样的音调和响度，钢琴和小提琴的声音听起来完全不同，这就是音色。波的形状决定了声音的音色，钢琴和小提琴的音色不同就是因为它们的介质所产生的波形不同。

人类耳朵的听力有一个频率范围，大约是 20Hz ~ 20kHz，而根据奈奎斯特定理（采样定理），按比声音最高频率高两倍以上的频率对声音进行采样，能够保证采样声音能够数字化，所以采样频率一般为 44.1kHz。

### 数字音频

数字音频有三个重要概念：

1) 采样，将信号从连续时间域上的模拟信号转换到离散时间域上的离散信号的过程。

2) 量化，指将信号的连续取值（或者大量可能的离散取值）近似为有限多个（或较少的）离散值的过程：

![量化](https://img-blog.csdn.net/20180719222114381?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

一般用 8bit 或 16bit 来量化声音的一个采样。按 16bit 量化时有 65536 个可能取值的数字信号，比 8bit(256 个可能值) 更为精细。

3) 编码，即按照一定的格式压缩、存储采样和量化后的数据。裸数据格式通常为 PCM 格式。

### PCM

通常音频的裸数据格式为脉冲编码调制（Pulse-code modulation，PCM）数据，描述一段 PCM 数据一般需要一下几个概念：

1) 量化格式（sampleFormat），也被称为位深度，比如 16bit 表示的数据格式，有 65536 个可能值，则位深度为 65536。

2) 采样率（sampleRate），也称为采样速度或者采样频率，定义了每秒从连续信号中提取并组成离散信号的采样个数，用赫兹（Hz）来表示。一般为 44.1kHz，指 1 个声道 1 秒钟有 44.1k 个数据，注意单位为“个”，每个数据的单位可以是 8bit、16bit 或其它，这是采样精度。采样频率的倒数叫作采样周期或采样时间，它是采样之间的时间间隔。

3) 声道数（channel）。分为单声道（mono）和立体声（stereo）两种，单声道只记录一种音源；立体声把现场各个方位的声音单独记录下来，并在播放时模拟当时的场景，可以营造出现场的逼真氛围。其中立体声有双声道、4.1 环绕立体声、5.1 环绕立体声几种。音视频开发中常用的只有单声道、双声道两种。

### 比特率（码率）

比特率指每秒传送的比特数，单位为 bps(Bit Per Second)，比特率越高，传送数据速度越快。声音中的比特率是指将模拟声音信号转换成数字声音信号后，单位时间内的二进制数据量，是间接衡量音频质量的一个指标。 多媒体行业在指音频或者视频在单位时间内的数据传输率时通常使用码流或码率，单位是 kbps(千位每秒)。视频中的码率的概念与声音中的相同，都是指由模拟信号转换为数字信号后，单位时间内的二进制数据量。比如 1.44Mbps，就是 1 秒内到达的数据量为 1.44Mb。（注意，是 bit，不是 byte）。

音频中比特率的计算公式如下：

> 
比特率 = 采样率 * 采样精度 * 声道数目


视频中比特率的计算公式如下：

> 
比特率 = 帧率 * 每帧数据大小


码率是音视频开发中非常重要的一个考虑因素，如果太大，则图像所占的内存也随之上升；太小，视频清晰度又会不足，因此码率的值需要根据实际情况权衡确定。

### 音频编码

压缩编码的原理实际上是压缩掉冗余信号，冗余信号指不能被人耳感知到的信号，包含人耳听觉范围之外的音频信号以及被掩盖掉的音频信号等。视频中的音频编码码率一般为 128kbit/s。

常见的压缩编码算法有：

1) WAV 编码

WAV 不会压缩音频，其中一种实现是在 PCM 数据前面加上 44 字节，分别描述 PCM 的采样率、声道数、数据格式等信息。

特点：音质非常好，大量软件都支持。 

使用场合：多媒体开发的中间文件、保存音乐和音效素材。

PCM 转 WAV 的具体实现可以看我的另一篇博客：[使用 AudioRecord 录制 PCM、WAV 文件，AudioTrack 播放 PCM 文件](https://blog.csdn.net/u011330638/article/details/81053355)

2) MP3

具有不错的压缩比，音质在 128kbit/s 以上表现还不错，压缩比比较高，兼容性好。

3) AAC

主要有 LC-AAC、HE-AAC、HE-AAC v2 三种编码格式，分别应用于中高码率(>= 80kbit/s)、中低码率(<= 80kbit/s)、低码率(<= 48kbit/s)场景。

在 128kbit/s 以下表现优异，多用于视频中的音频编码。

4) Ogg

音质好，完全免费，在各种码率下都有优秀的变现，尤其是中低码率场景下，可以用更小的码率达到更好的音质，但目前还没有媒体软件服务的支持，兼容性不够好，适用于语音聊天的音频消息场景。

### 其它

1) 视频

静止的画面叫图像（picture）。连续的图像变化每秒超过24帧（frame）画面以上时，根椐视觉暂留原理，人眼无法辨别每付单独的静态画面，看上去是平滑连续的视觉效果。这样的连续画面叫视频。当连续图像变化每秒低于24帧画面时，人眼会有不连续的感觉，这叫动画（cartoon）

2) 流媒体

指将一连串的媒体数据压缩后，经过网络分段发送数据，在网络上即时传输影音以供观赏的一种技术与过程。这个技术使得数据包得以像流水一样发送，达到随时传送随时播放的效果，只是在开始时有些延迟。边下载边播入的流式传输方式不仅使启动延时大幅度地缩短，而且对系统缓存容量的需求也大大降低，极大地减少用户用在等待的时间。

3) 分辨率

分辨率是一个表示平面图像精细程度的概念，通常它是以横向和纵向点的数量来衡量的，表示成水平点数垂直点数的形式，在计算机显示领域我们也表示成“每英寸像素”（ppi）。在一个固定的平面内，分辨率越高，意味着可使用的点数越多，图像越细致

4) 帧率

帧/秒（frames per second）的缩写，可理解为1秒钟时间里刷新的图片的帧数，也可以理解为图形处理器每秒钟能够刷新几次，帧率越大，所显示的画面就会愈流畅。

5) 编码/解码

音视频的原始数据是很大的，比如一帧图像的大小就等于 宽 * 高 * 像素，对于一张 1280*960，采用 RGBA_8888 （实际开发中一般不会使用 RGBA  格式，这里只是举个例子）数据格式的图片，它的大小就等于 1280*960*4Byte = 4.6875M，假设视频帧率为 24帧/s，那么 1 分钟的视频大小为：4.6875 * 24 * 60 = 6750M。这么大的数据量实在难以在网络中进行传输，因此，有必要对原始数据按照一定规则进行压缩，这就是编码。比如音频中常用的是 AAC，视频中常用的是 H264，这就是编码算法。

编码的逆过程就是解码，即把压缩后的数据按照规则还原为原始的声音/图像数据，原始的数据格式通常为 PCM/YUV。

6) 封装/解封装

经过声音/图像的采集、编码步骤之后，就可以把音频数据、视频数据合成为一个文件了，这就是封装。

封装的逆过程就是解封装，即一帧一帧地把文件中的音频（AAC）、视频（H264）数据读取出来。

7) 直播/点播

常见的直播平台有：yy、战旗、斗鱼、熊猫等

常见的点播平台有：优酷/土豆、爱奇艺、腾讯视频等

# 图像的数值表示

### RGB

任何一幅图像都可以由 RGB 组成，常用的 RGB 表示法有两种：

1) 浮点表示，取值范围为 0.0 ~ 1.0，OpenGL ES 使用的就是这种方式

2) 整数表示，取值范围为 0 ~ 255 或者 00 ~ FF，很多平台用的都是这种表达方式，比如 RGB_565 就使用了 16bit 来表示一个像素，R 用 5bit，G 用 6bit，B 用 5bit。

![RGB](https://img-blog.csdn.net/20180719222315625?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### YUV

对于视频的裸数据而言，用得更多的是 YUV 数据格式，其中 “Y” 表示明亮度（Luminance 或 Luma），即灰度值；而 “U” 和 “V” 表示的则是色度（Chrominance 或 Chroma），作用是描述影像色彩及饱和度，指定像素的颜色。

YUV 主要用于电视系统以及模拟视频领域，它将亮度信息（Y）与色彩信息（UV）分离，没有 UV 信息一样可以显示完整的图像，只不过是黑白的，这样的设计很好地解决了彩色电视机与黑白电视的兼容问题。并且，YUV 不像RGB 那样要求三个独立的视频信号同时传输，所以用 YUV 数据占用的内容更少。

RGB 转 YUV （从 [ITU-R BT.601-7](https://www.itu.int/rec/R-REC-BT.601-7-201103-I/en) 标准中可以拿到推荐的相关系数）：

> 
Y = 0.299R + 0.587G + 0.114B  

  Cb = 0.564(B - Y)  

  Cr = 0.713(R - Y) 
上述就是 YCbCr 颜色模型的基本原理。YCbCr 是属于 YUV 家族的一员（Cb、Cr 的含义等同于 U、V ），是在计算机系统中应用最为广泛的颜色模型。在 YCbCr 中，Y 是指亮度分量，Cb 指蓝色色度分量，而 Cr 指红色色度分量。

YCbCr 转 RGB：

> 
R = Y + 1.402Cr 

  G = Y - 0.344Cb - 0.714Cr 

  B = Y + 1.772Cb
此时我们发现，YCbCr 仍然用了 3 个数字来表示颜色，那么它是如何节省了空间的呢？下面通过图片说明。

图片由类似下面的像素组成：

![YCbCr](https://img-blog.csdn.net/2018071922233272?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

一副图片就是一个像素矩阵：

![YUV444](https://img-blog.csdn.net/20180719222344409?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上图中，每个像素的 3 个分量的信息是完整的，Y : Cb : Cr = 4 : 4 : 4，属于 YUV444 格式。

而根据人类视觉系统对亮度信号比色度信号敏感的原理，我们可以省略图片的一些信息，对图片的质量影响却不会太大，比如：

![YUV422](https://img-blog.csdn.net/20180719222353841?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

此时，每两个 Y 共用一组 UV 分量，Y : Cb : Cr = 4 : 2 : 2，属于 YUV422 格式。

![YUV420](https://img-blog.csdn.net/20180719222405536?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上图中，每四个 Y 共用一组 UV 分量，Cb、Cr 交替出现，在第一行数据里 Y : Cb : Cr = 4 : 2 : 0；而在第二行数据，Y : Cb : Cr = 4 : 0 : 2，这就是最常见的 YUV420 格式。

### YUV420

YUV 格式有两大类：planar 和 packed。planar 先存储所有 Y，紧接着存储所有 U，最后是 V；而 packed 则是每个像素点的 Y、U、V 连续交叉存储。不过，packed 格式的已经不怎么能见到了，现在几乎都用 planar 格式。planar 的意思是平面，一个 planar 存储一个分量，因此 YUV 需要三个 planar 来存储图像信息。

最常用数据格式是 YUV420，其中又有 YUV420p、YUV420sp 两种：

![YUV420p](https://img-blog.csdn.net/20180728010541506?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![YUV420sp](https://img-blog.csdn.net/20180728010533329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
也有 UV 数据顺序相反的，比如 Android 平台中的 YV12、NV21 两种格式：

![YV12](https://img-blog.csdn.net/20180728010650537?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![NV21](https://img-blog.csdn.net/20180728010629592?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
还有一个和 YUV 数据相关的概念：stride——指内存中每行像素所占的空间。为了实现内存对齐，每行像素在内存中所占的空间不一定是图像的宽度。因此 stride 大于等于图像帧的宽度，同时，stride 为 4 的倍数。

# H264

和音频编码一样，图像数据的编码方式也有很多种，比如 ISO 制定的 MPEG2、MPEG4 标准，ITU-T 的 H.261、H.262、H.263、H.264、H.265，Google 的 VP8、VP9 ， AOM 的 AV1 等。其中 H265(HEVC)、VP9、AV1 是最新的几个编解码标准/格式，还在互相竞争中，现在用得最多的还是 H264。

H264 又称为 MPEG-4 Part 10, Advanced Video Coding，简写为 AVC。因为 ITU-T H.264 标准和 ISO/IEC MPEG-4 AVC 标准（正式名称是ISO/IEC 14496-10—MPEG-4 第十部分，高级视频编码）有相同的技术内容，故被共同管理。

H264 的相关概念有：序列、图像、片组、片、NALU、宏块、亚宏块、块、像素。

### 图像、帧、场

在 H.264 中，「图像」是个集合的概念，一帧或一场都可以称为图像。一帧通常就是一幅完整的图像。当采集视频信号时，如果采用逐行扫描，则每次扫描得到的信号就是一副图像，也就是一帧。而如果采用的是隔行扫描（奇、偶数行），则扫描下来的一帧图像就被分为了两个部分，这每一部分就称为「场」，根据次序分为：「顶场」和「底场」。「帧」和「场」的概念又带来了不同的编码方式：帧编码、场编码。逐行扫描适合于运动图像，所以对于运动图像采用帧编码更好；隔行扫描适合于非运动图像，所以对于非运动图像采用场编码更好。

![场](https://img-blog.csdn.net/20180719234245949?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### IPB 帧、IDR 帧

视频编码中，最常用的数据压缩算法是帧内预测、帧间预测。帧内预测是对单张图像本身进行数据压缩，比如 JPEG。帧间预测是利用视频图像帧间的相关性，来达到图像压缩的目的；因为同一个视频中的前后两帧图像差异很小，因此不必完整地保存这两帧图像的原始数据，只需要保存前一帧的图像数据，然后再保存后一帧画面与前一帧的差别即可。

对应的帧类型有 I 帧（intra picture）、P 帧（predictive-frame）、B 帧（bi-directional interpolated prediction frame）。I 帧是关键帧，使用帧内预测，无需借助其它帧的信息即可完整地呈现出一幅图像；P 帧没有完整的图像数据，只保存与前面的帧的差别，需要借助之前的帧数据生成图像；B 帧记录的是与前后帧的差别。

此外还有一个概念为 IDR 帧（instantaneous decoding refresh picture），因为 I 帧后的 P 帧可能会参考 I 帧之前的帧，这使得在随机访问的时候，可能即使找到了 I 帧，后面的 P 帧也无法解码。因此，IDR 会清空参考帧列表（DPB），IDR 帧之后的帧都不能引用任何 IDR 帧之前的帧数据。

### 片、NAL、宏块

H.264 的主要目标有两个：高视频压缩比、良好的网络亲和性。为此， H.264 的功能分为两层，即视频编码层（ VCL）和网络提取层（ NAL， Network Abstraction Layer）。 VCL 数据即编码处理的输出，它表示被压缩编码后的视频数据序列。在 VCL 数据传输或存储之前，这些编码的 VCL 数据，先被映射或封装进 NAL Unit 中。每个 NAL Unit 包括一个原始字节序列负荷（ RBSP）、一组对应于视频编码数据的 NAL 头信息：

![NALU](https://img-blog.csdn.net/20180727125748336?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

NAL Unit 的头占一个字节，由三部份組成，包括 forbidden_bit、nal_reference_idc 和 nal_unit_type。其中 forbidden_bit 占 1 bit，一般来说其值为 0；nal_reference_idc 占 2 bit，用于表示此 NAL 在重建过程中的重要程度。剩下 5 bit 表示 nal_unit_type，用于表示该 NAL Unit （RBSP）的类型：
|值|定义|
|----|----|
|0|Undefined|
|1|Slice layer without partitioning non IDR|
|2|Slice data partition A layer|
|3|Slice data partition B layer|
|4|Slice data partition C layer|
|5|Slice layer without partitioning IDR|
|6|Additional information (SEI)|
|7|Sequence parameter set(SPS)|
|8|Picture parameter set(PPS)|
|9|Access unit delimiter|
|10|End of sequence|
|11|End of stream|
|12|Filler data|
|13..23|Reserved|
|24..31|Undefined|

举例来说，若截取某一段 H.264 bitstream 为 00 00 00 01 67 42 e0 14 da 05 82 51。其中 00 00 00 01 为 startcode（起始码），每个NALU之间通过 startcode 进行分隔。之后才是 NAL 的数据，因为 67 = 0 11 00111，nal_unit_tye = 00111 = 7，所以这一段为 SPS。SPS 信息在整个视频编码序列中是不变的，用于描述一个视频编码序列；PPS 信息在一幅编码图像之内是不变的，用于描述一个或多个独立的图像。SPS、PPS 的作用是防止在某些数据丢失后，整幅图像都受到影响的情况。

一个视频图像可编码成一个或更多个片（Slice）： 
![片](https://img-blog.csdn.net/20180727132547383?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
每片包含整数个宏块（ Marco Block，以下简称 MB），即每片至少一个 MB，最多时每片包含整个图像的宏块。设片的目的是为了限制误码的扩散和传输，应使编码片相互间是独立的。某片的预测不能以其它片中的宏块为参考图像，这样某一片中的预测误差才不会传播到其它片中去。

一个宏块由一个 16×16 亮度像素和附加的一个 8×8 Cb 和一个 8×8 Cr 彩色像素块组成。每个图象中，若干宏块被排列成片的形式。I 片只包含 I 宏块， P 片可包含 P 和 I 宏块，而 B 片可包含 B 和 I 宏块。

![片](https://img-blog.csdn.net/20180727130917855?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### SPS、PPS

H.264 引入了参数集的概念，每个参数集包含了相应的编码图像的信息。序列参数集 SPS 包含 

的是针对一连续编码视频序列的参数，如标识符 seq_parameter_set_id、帧数及参考帧数目、解码图像尺寸等等。图像参数集 PPS 对应的是一个序列中某一幅图像或者某几幅图像 ，其参数如标识符 pic_parameter_set_id、可选的 seq_parameter_set_id、片组数目等等。

通常， SPS 和 PPS 在片的头信息和数据解码前传送至解码器。每个片的头信息对应一个pic_parameter_set_id， PPS 被其激活后一直有效到下一个 PPS 被激活；类似的，每个 PPS 对应一个 seq_parameter_set_id， SPS 被其激活以后将一直有效到下一个 SPS 被激活。

参数集机制将一些重要的、改变少的序列参数和图像参数与编码片分离，并在编码片之前传送至解码端，或者通过其他机制传输。

### H264 vs x264

H264 是一个标准，一种格式，定义了视频流应该如何被压缩编码

x264 是一个开源的编码器，用于产生 H264 格式的视频流

### h264 vs avc1

属于 MP4 封装的 H264 视频的两种格式，都是“H264”，区别为：

h264：带起始码 0x00 00 01 或 0x00 00 00 01

avc1：不带起始码

# 常用软件

1) MediaInfo

用于查看视频参数，比如： 
![MediaInfo](https://img-blog.csdn.net/20180728001808889?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2) VideoEye

雷神写的：[开源实时视频码流分析软件：VideoEye](https://blog.csdn.net/leixiaohua1020/article/details/34553607)

除了查看视频参数之外，还有音视频码流分析等功能

3) ffmpeg

它的三个命令行工具很好用，分别是： 

ffplay：用于播放音视频，包括 yuv、pcm 等裸数据 

ffprobe：用于查看媒体文件头信息 

ffmpeg：强大的媒体文件转换工具，还可以转换图片格式
下载链接：[https://www.ffmpeg.org/download.html](https://www.ffmpeg.org/download.html)

4) 其它

YUVPlayer：播放 yuv 裸数据 

VLC：多媒体播放器

最后，因为上面的内容不是同一时间写的，哪些内容来自哪些文章都忘得差不多了，这些只列出最重要的几个： 
[[总结]视音频编解码技术零基础学习方法](https://blog.csdn.net/leixiaohua1020/article/details/18893769) （雷神是真大神） 
[《音视频开发进阶指南》](https://book.douban.com/subject/30124646/)
[《新一代视频压缩编码标准》](https://book.douban.com/subject/1314942/)












