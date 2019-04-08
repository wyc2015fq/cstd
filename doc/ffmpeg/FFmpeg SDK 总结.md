# FFmpeg SDK 总结



# （一）

FFMpeg 中比较重要的函数以及数据结构如下:

### 1. 数据结构:

- (1) AVFormatContext
  此结构包含了一个视频流的格式内容。其中存有了 AVInputFormat(or AVOutputFormat 同一时间 AVFormatContext 内只能存在其中一个)，和 AVStream、AVPacket 这几个重要的数据结构以及一些其他的 相关信息，比如 title,author,copyright 等。还有一些可能在编解码中会用到的信息，诸如:duration, file_size, bit_rate 等。参考 avformat.h 头文件。
- (2) AVOutputFormat
- (3) AVInputFormat
- (4) AVCodecContext
  此结构在Ffmpeg SDK中的注释是:main external api structure其重要性可见一斑。而且在avcodec它的定 义处，对其每个成员变量，都给出了十分详细的介绍。应该说AVCodecContext的初始化是Codec使用中最重 要的一环。虽然在前面的AVStream中已经有所提及，但是这里还是要在说一遍。AVCodecContext作为 Avstream的一个成员结构，必须要在Avstream初始化後(30)再对其初始化(AVStream的初始化用到 AVFormatContex)。虽然成员变量比较多，但是这里只说一下在output_example.c中用到了，其他的请查阅 avcodec.h文件中介绍。
- (5) AVCodec
  结构 AVCodec 中成员变量和成员函数比较少，但是很重要。他包含了 CodecID，也就是用哪个 Codec 像素格式信息。还有前面提到过的 5 个函数(init、encode、close、decoder、flush)。顺便提一下，虽然在 参考代码 output_example.c 中的编码函数用的是 avcodec_encode_video()，我怀疑在其中就是调用了 AVCodec 的 encode 函数，他们传递的参数和返回值都是一致的，当然还没有得到确认，有兴趣可以看看 ffmpeg 源代码。在参考代码中，AVCodec 的初始化後的使用都是依附于 AVCodecContex，前者是后者的成 员。在 AVCodecContext 初始化後(add_video_stream())，AVCodec 也就能很好的初始化了
- (6) AVFrame
  AVFrame 是作为一个描述“原始图像”(也就是 YUV 或是 RGB…还有其他的吗?)的结构，他的头两 个成员数据，uint8_t *data[4]，int linesize[4]，第一个存放的是 Y、Cb、Cr(yuv 格式)，linesize 是啥?由这 两个数据还能提取处另外一个数据结构。
- (7) AVPacket
  AVPacket 的存在是作为写入文件的基本单元而存在的。我们可能会认为直接把编码后的比特流写入文 件不就可以了，为什么还要麻烦设置一个 AVPacket 结构。在我看来这样的编码设置是十分有必要的，特别 是在做视频实时传输，同步、边界问题可以通过 AVPacket 来解决。AVPacket 的成员数据有两个时间戳、 数据 data(通常是编码后数据)、大小 size 等等(参见 avformat.h 48 行)。讲 AVPacket 的用法就不得不提到 编解码函数，因为 AVPacket 的好些信息只有在编解码后才能的知。在参考代码中(ouput_example.c 从 362 到 394 行)，做的一个判断分支。如果输出文件格式是 RAW 图像(即 YUV 或 RGB)那么就没有编码函数， 直接写入文件(因为程序本身生成一个 YUV 文件)，这里的代码虽然在此看来没什么价值，但是如果是解 码函数解出 yuv 文件(或 rgb)那么基本的写文件操作就是这样的
- (8) AVPicture
  AVPicture 的存在有以下原因，AVPicture 将 Picture 的概念从 Frame 中提取出来，就只 由Picture(图片)本身的信息，亮度、色度和行大小。而Frame还有如是否是key frame之类的信息。这样的类似“分级”是整个概念更加清晰。
- (9) AVStream
  AVStream作为继AVFormatContext後第二个贯穿始终的结构是有其理由的。他的成员数据中有 AVCodecContext这基本的上是对所使用的Video Codec的参数进行设定的(包括bit rate、分辨率等重要信息)。 同时作为“Stream”，它包含了“流”这个概念中的一些数据，比如:帧率(r_frame_rate)、基本时间计量 单位(time_base)、(需要编解码的)首帧位置(start_time)、持续时间(duration)、帧数(nb_frames)以及 一些ip信息。当然后面的这些信息中有些不是必须要初始化的，但是AVCodecContex是一定要初始化的，而 且就是作为初始化AVStream最重要的一个部分。

### 2. 初始化函数:

- (1) av_register_all()
  usage: initialize ibavcoded, and register all codecs and formats
  每个使用 FFMpeg SDK 的工程都必须调用的函数。进行 codec 和 format 的注册，然后才能使用。声明在 allformats.c 中，都是宏有兴趣看看。
- (2) avcodec_open()
- (3) avcodec_close()
- (4) av_open_input_file()
- (5) av_find_input_format()
- (6) av_find_stream_info()
- (7) av_close_input_file()

### 3. 音视频编解码函数:

- (1) avcodec_find_decoder()
- (2) avcodec_alloc_frame()
- (3) avpicture_get_size()
- (4) avpicture_fill()
- (5) img_convert()
- (6) avcodec_alloc_context()
- (7) avcodec_decode_video()
- (8) av_free_packet()
- (9) av_free()

### 4. 文件操作:

- (1) avnew_steam()
- (2) av_read_frame()
- (3) av_write_frame()
- (4) dump_format()

### 5. 其他函数:

- (1) avpicture_deinterlace()
- (2) ImgReSampleContext()







# （二）

 

在学习FFmpeg的过程中，特意再去了解了下 FFmpeg 的背景

### FFmpeg 背景介绍

- FFmpeg是一套可以用来记录、转换数字音频、视频，并能将其转化为流的开源计算机程序。采用LGPL或GPL许可证。它提供了录制、转换以及流化音视频的完整解决方案。它包含了非常先进的音频/视频编解码库libavcodec，为了保证高可移植性和编解码质量，libavcodec里很多codec都是从头开发的。
- FFmpeg在Linux平台下开发，但它同样也可以在其它操作系统环境中编译运行，包括Windows、Mac OS X等。这个项目最早由Fabrice Bellard发起，现在由Michael Niedermayer维护。许多FFmpeg的开发人员都来自MPlayer项目，而且当前FFmpeg也是放在MPlayer项目组的服务器上。项目的名称来自MPEG视频编码标准，前面的"FF"代表"Fast Forward"。

### FFmpeg源码模块

- libavformat：用于各种音视频封装格式的生成和解析，包括获取解码所需信息以生成解码上下文结构和读取音视频帧等功能；
- libavcodec：用于各种类型声音/图像编解码；
- libavutil：包含一些公共的工具函数；
- libswscale：用于视频场景比例缩放、色彩映射转换；
- libpostproc：用于后期效果处理；
- ffmpeg：该项目提供的一个工具，可用于格式转换、解码或电视卡即时编码等；
- ffsever：一个 HTTP 多媒体即时广播串流服务器；
- ffplay：是一个简单的播放器，使用ffmpeg 库解析和解码，通过SDL显示；





# （三）

 

这一篇主要对一些编解码过程中会遇到的概念做了解：

### H.264编码原理I／B／P帧

**三种帧的说明**
**I帧**:帧内编码帧 ，I帧表示关键帧，你可以理解为这一帧画面的完整保留；解码时只需要本帧数据就可以完成（因为包含完整画面）
I帧特点:
1.它是一个全帧压缩编码帧。它将全帧图像信息进行JPEG压缩编码及传输;
2.解码时仅用I帧的数据就可重构完整图像;
3.I帧描述了图像背景和运动主体的详情;
4.I帧不需要参考其他画面而生成;
5.I帧是P帧和B帧的参考帧(其质量直接影响到同组中以后各帧的质量);
6.I帧是帧组GOP的基础帧(第一帧),在一组中只有一个I帧;
7.I帧不需要考虑运动矢量;
8.I帧所占数据的信息量比较大。

**P帧**:前向预测编码帧。P帧表示的是这一帧跟之前的一个关键帧（或P帧）的差别，解码时需要用之前缓存的画面叠加上本帧定义的差别，生成最终画面。（也就是差别帧，P帧没有完整画面数据，只有与前一帧的画面差别的数据）

P帧的预测与重构:P帧是以I帧为参考帧,在I帧中找出P帧“某点”的预测值和运动矢量,取预测差值和运动矢量一起传送。在接收端根据运动矢量从I帧中找出P帧“某点”的预测值并与差值相加以得到P帧“某点”样值,从而可得到完整的P帧。
P帧特点:
1.P帧是I帧后面相隔1~2帧的编码帧;
2.P帧采用运动补偿的方法传送它与前面的I或P帧的差值及运动矢量(预测误差);
3.解码时必须将I帧中的预测值与预测误差求和后才能重构完整的P帧图像;
4.P帧属于前向预测的帧间编码。它只参考前面最靠近它的I帧或P帧;
5.P帧可以是其后面P帧的参考帧,也可以是其前后的B帧的参考帧;
6.由于P帧是参考帧,它可能造成解码错误的扩散;
7.由于是差值传送,P帧的压缩比较高。

**B帧**:双向预测内插编码帧。B帧是双向差别帧，也就是B帧记录的是本帧与前后帧的差别（具体比较复杂，有4种情况，但我这样说简单些），换言之，要解码B帧，不仅要取得之前的缓存画面，还要解码之后的画面，通过前后画面的与本帧数据的叠加取得最终的画面。B帧压缩率高，但是解码时CPU会比较累。

B帧的预测与重构
B帧以前面的I或P帧和后面的P帧为参考帧,“找出”B帧“某点”的预测值和两个运动矢量,并取预测差值和运动矢量传送。接收端根据运动矢量在两个参考帧中“找出(算出)”预测值并与差值求和,得到B帧“某点”样值,从而可得到完整的B帧。
B帧特点
1.B帧是由前面的I或P帧和后面的P帧来进行预测的;
2.B帧传送的是它与前面的I或P帧和后面的P帧之间的预测误差及运动矢量;
3.B帧是双向预测编码帧;
4.B帧压缩比最高,因为它只反映丙参考帧间运动主体的变化情况,预测比较准确;
5.B帧不是参考帧,不会造成解码错误的扩散。

注:**I、B、P各帧是根据压缩算法的需要，是人为定义的,它们都是实实在在的物理帧。一般来说，I帧的压缩率是7（跟JPG差不多），P帧是20，B帧可以达到50。可见使用B帧能节省大量空间，节省出来的空间可以用来保存多一些I帧，这样在相同码率下，可以提供更好的画质。**

### h264的压缩方法

- 1.分组:把几帧图像分为一组(GOP，也就是一个序列),为防止运动变化,帧数不宜取多。
- 2.定义帧:将每组内各帧图像定义为三种类型,即I帧、B帧和P帧;
- 3.预测帧:以I帧做为基础帧,以I帧预测P帧,再由I帧和P帧预测B帧;
- 4.数据传输:最后将I帧数据与预测的差值信息进行存储和传输。

### h264的压缩原理

- 帧内（Intraframe）压缩也称为空间压缩（Spatial compression）。当压缩一帧图像时，仅考虑本帧的数据而不考虑相邻帧之间的冗余信息，这实际上与静态图像压缩类似。帧内一般采用有损压缩算法，由于帧内压缩是编码一个完整的图像，所以可以独立的解码、显示。帧内压缩一般达不到很高的压缩，跟编码jpeg差不多。
- 帧间（Interframe）压缩的原理是：相邻几帧的数据有很大的相关性，或者说前后两帧信息变化很小的特点。也即连续的视频其相邻帧之间具有冗余信息,根据这一特性，压缩相邻帧之间的冗余量就可以进一步提高压缩量，减小压缩比。帧间压缩也称为时间压缩（Temporal compression），它通过比较时间轴上不同帧之间的数据进行压缩。帧间压缩一般是无损的。帧差值（Frame differencing）算法是一种典型的时间压缩法，它通过比较本帧与相邻帧之间的差异，仅记录本帧与其相邻帧的差值，这样可以大大减少数据量。
- 顺便说下有损（Lossy ）压缩和无损（Lossy less）压缩。无损压缩也即压缩前和解压缩后的数据完全一致。多数的无损压缩都采用RLE行程编码算法。有损压缩意味着解压缩后的数据与压缩前的数据不一致。在压缩的过程中要丢失一些人眼和人耳所不敏感的图像或音频信息,而且丢失的信息不可恢复。几乎所有高压缩的算法都采用有损压缩,这样才能达到低数据率的目标。丢失的数据率与压缩比有关,压缩比越小，丢失的数据越多,解压缩后的效果一般越差。此外,某些有损压缩算法采用多次重复压缩的方式,这样还会引起额外的数据丢失。

### H264 NAL头解析

如果NALU对应的Slice为一帧的开始，则用4字节表示，即0x00000001；否则用3字节表示，0x000001。
NAL Header：forbidden_bit，nal_reference_bit（优先级）2bit，nal_unit_type（类型）5bit。 标识NAL单元中的RBSP数据类型，其中，nal_unit_type为1， 2， 3， 4， 5的NAL单元称为VCL的NAL单元，其他类型的NAL单元为非VCL的NAL单元。
0：未规定
1：非IDR图像中不采用数据划分的片段
2：非IDR图像中A类数据划分片段
3：非IDR图像中B类数据划分片段
4：非IDR图像中C类数据划分片段
5：IDR图像的片段
6：补充增强信息（SEI）
7：序列参数集（SPS）
8：图像参数集（PPS）
9：分割符
10：序列结束符
11：流结束符
12：填充数据
13：序列参数集扩展
14：带前缀的NAL单元
15：子序列参数集
16 – 18：保留
19：不采用数据划分的辅助编码图像片段
20：编码片段扩展
21 – 23：保留
24 – 31：未规定

H.264的SPS和PPS串，包含了初始化H.264解码器所需要的信息参数，包括编码所用的profile，level，图像的宽和高，deblock滤波器等。

### 音频编码压缩

AAC（Advanced Audio Coding），中文名：高级音频编码，出现于1997年，基于MPEG-2的音频编码技术。由Fraunhofer IIS、杜比实验室、AT&T、Sony等公司共同开发，目的是取代MP3格式。2000年，MPEG-4标准出现后，AAC重新集成了其特性，加入了SBR技术和PS技术，为了区别于传统的MPEG-2 AAC又称为MPEG-4 AAC。

- 优点：相对于mp3，AAC格式的音质更佳，文件更小。
- 不足：AAC属于有损压缩的格式，与时下流行的APE、FLAC等无损格式相比音质存在“本质上”的差距。加之，传输速度更快的USB3.0和16G以上大容量MP3正在加速普及，也使得AAC头上“小巧”的光环不复存在了。

音频采样率是指录音设备在一秒钟内对声音信号的采样次数，采样频率越高声音的还原就越真实越自然。在当今的主流采集卡上，采样频率一般共分为22.05KHz、44.1KHz、48KHz三个等级，22.05KHz只能达到FM广播的声音品质，44.1KHz则是理论上的CD音质界限，48KHz则更加精确一些。

比特率是指每秒传送的比特(bit)数。单位为 bps(Bit Per Second)，比特率越高，传送数据速度越快。声音中的比特率是指将模拟声音信号转换成数字声音信号后，单位时间内的二进制数据量，是间接衡量音频质量的一个指标。 视频中的比特率（码率）原理与声音中的相同，都是指由模拟信号转换为数字信号后，单位时间内的二进制数据量。
信道编码中，K符号大小的信源数据块通过编码映射为N符号大小的码字，则K/N成为码率，其中假设编码前后的符号表没有变化。









# （四）

2019年03月12日 22:24:34 [HugoforAndroid](https://me.csdn.net/lipengshiwo) 阅读数：73



版权声明：本文为博主hugo.lee原创文章，欢迎转载，请注明来源。	https://blog.csdn.net/lipengshiwo/article/details/88430730

之前 [FFmpeg SDK 开发（一）](https://blog.csdn.net/lipengshiwo/article/details/88226939) 列举了FFmpeg 中比较重要的函数以及数据结构，并概念性的介绍了其中一些名词。这边将深入进行到具体的结构内容。

### FFMPEG中结构体分类

FFMPEG中结构体很多。最关键的结构体可以分成以下几类：

- 解协议（http,rtsp,rtmp,mms）
  AVIOContext，URLProtocol，URLContext主要存储视音频使用的协议的类型以及状态。URLProtocol存储输入视音频使用的封装格式。每种协议都对应一个URLProtocol结构。（注意：FFMPEG中文件也被当做一种协议“file”）
- 解封装（flv,avi,rmvb,mp4）
  AVFormatContext主要存储视音频封装格式中包含的信息；AVInputFormat存储输入视音频使用的封装格式。每种视音频封装格式都对应一个AVInputFormat 结构。
- 解码（h264,mpeg2,aac,mp3）
  每个AVStream存储一个视频/音频流的相关数据；每个AVStream对应一个AVCodecContext，存储该视频/音频流使用解码方式的相关数据；每个AVCodecContext中对应一个AVCodec，包含该视频/音频对应的解码器。每种解码器都对应一个AVCodec结构。
- 存数据
  视频的话，每个结构一般是存一帧；音频可能有好几帧
  解码前数据：AVPacket
  解码后数据：AVFrame

### AVCodec

AVCodec是存储编解码器信息的结构体
const char *name：编解码器的名字，比较短
const char *long_name：编解码器的名字，全称，比较长
enum AVMediaType type：指明了类型，是视频，音频，还是字幕
enum AVCodecID id：ID，不重复
const AVRational *supported_framerates：支持的帧率（仅视频）
const enum AVPixelFormat *pix_fmts：支持的像素格式（仅视频）
const int *supported_samplerates：支持的采样率（仅音频）
const enum AVSampleFormat *sample_fmts：支持的采样格式（仅音频）
const uint64_t *channel_layouts：支持的声道数（仅音频）int priv_data_size：私有数据的大小1.注册所有编解码器：av_register_all();2.声明一个AVCodec类型的指针，比如说AVCodec* first_c;
3.调用av_codec_next()函数，即可获得指向链表下一个解码器的指针，循环往复可以获得所有解码器的信息。注意，如果想要获得指向第一个解码器的指针，则需要将该函数的参数设置为NULL。

### AVCodecContext

这是一个描述编解码器上下文的数据结构，包含了众多编解码器需要的参数信息
如果是单纯使用libavcodec，这部分信息需要调用者进行初始化；如果是使用整个FFMPEG库，这部分信息在调用 av_open_input_file和av_find_stream_info的过程中根据文件的头信息及媒体流内的头部信息完成初始化。其中几个主要 域的释义如下：
extradata/extradata_size： 这个buffer中存放了解码器可能会用到的额外信息，在av_read_frame中填充。一般来说，首先，某种具体格式的demuxer在读取格式头 信息的时候会填充extradata，其次，如果demuxer没有做这个事情，比如可能在头部压根儿就没有相关的编解码信息，则相应的parser会继 续从已经解复用出来的媒体流中继续寻找。在没有找到任何额外信息的情况下，这个buffer指针为空。
time_base：
width/height：视频的宽和高。
sample_rate/channels：音频的采样率和信道数目。
sample_fmt： 音频的原始采样格式。
codec_name/codec_type/codec_id/codec_tag：编解码器的信息。

### AVStream

该结构体描述一个媒体流
主要域的释义如下，其中大部分域的值可以由av_open_input_file根据文件头的信息确定，缺少的信息需要通过调用av_find_stream_info读帧及软解码进一步获取：
index/id：index对应流的索引，这个数字是自动生成的，根据index可以从AVFormatContext::streams表中索引到该流；而id则是流的标识，依赖于具体的容器格式。比如对于MPEG TS格式，id就是pid。
time_base：流的时间基准，是一个实数，该流中媒体数据的pts和dts都将以这个时间基准为粒度。通常，使用av_rescale/av_rescale_q可以实现不同时间基准的转换。
start_time：流的起始时间，以流的时间基准为单位，通常是该流中第一个帧的pts。
duration：流的总时间，以流的时间基准为单位。
need_parsing：对该流parsing过程的控制域。
nb_frames：流内的帧数目。
r_frame_rate/framerate/avg_frame_rate：帧率相关。
codec：指向该流对应的AVCodecContext结构，调用av_open_input_file时生成。
parser：指向该流对应的AVCodecParserContext结构，调用av_find_stream_info时生成。

### AVFormatContext

这个结构体描述了一个媒体文件或媒体流的构成和基本信息
这是FFMpeg中最为基本的一个结构，是其他所有结构的根，是一个多媒体文件或流的根本抽象。其中:nb_streams和streams所表示的AVStream结构指针数组包含了所有内嵌媒体流的描述；iformat和oformat指向对应的demuxer和muxer指针；pb则指向一个控制底层数据读写的ByteIOContext结构。
start_time和duration是从streams数组的各个AVStream中推断出的多媒体文件的起始时间和长度，以微妙为单位。
通常，这个结构由av_open_input_file在内部创建并以缺省值初始化部分成员。但是，如果调用者希望自己创建该结构，则需要显式为该结构的一些成员置缺省值——如果没有缺省值的话，会导致之后的动作产生异常。以下成员需要被关注：
probesize
mux_rate
packet_size
flags
max_analyze_duration
key
max_index_size
max_picture_buffer
max_delay

### AVPacket

AVPacket定义在avcodec.h中
FFMPEG使用AVPacket来暂存解复用之后、解码之前的媒体数据（一个音/视频帧、一个字幕包等）及附加信息（解码时间戳、显示时间戳、时长等）。其中：
dts 表示解码时间戳，pts表示显示时间戳，它们的单位是所属媒体流的时间基准。
stream_index 给出所属媒体流的索引；
data 为数据缓冲区指针，size为长度；
duration 为数据的时长，也是以所属媒体流的时间基准为单位；
pos 表示该数据在媒体流中的字节偏移量；
destruct 为用于释放数据缓冲区的函数指针；
flags 为标志域，其中，最低为置1表示该数据是一个关键帧。
AVPacket 结构本身只是个容器，它使用data成员指向实际的数据缓冲区，这个缓冲区可以通过av_new_packet创建，可以通过 av_dup_packet 拷贝，也可以由FFMPEG的API产生（如av_read_frame），使用之后需要通过调用av_free_packet释放。
av_free_packet调用的是结构体本身的destruct函数，它的值有两种情况：(1)av_destruct_packet_nofree或 0；
(2)av_destruct_packet，其中，前者仅仅是将data和size的值清0而已，后者才会真正地释放缓冲区。FFMPEG内部使用 AVPacket结构建立缓冲区装载数据，同时提供destruct函数，如果FFMPEG打算自己维护缓冲区，则将destruct设为 av_destruct_packet_nofree，用户调用av_free_packet清理缓冲区时并不能够将其释放；如果FFMPEG不会再使用 该缓冲区，则将destruct设为av_destruct_packet，表示它能够被释放。对于缓冲区不能够被释放的AVPackt，用户在使用之前 最好调用av_dup_packet进行缓冲区的克隆，将其转化为缓冲区能够被释放的AVPacket，以免对缓冲区的不当占用造成异常错误。而 av_dup_packet会为destruct指针为av_destruct_packet_nofree的AVPacket新建一个缓冲区，然后将原 缓冲区的数据拷贝至新缓冲区，置data的值为新缓冲区的地址，同时设destruct指针为av_destruct_packet。

### AVFrame

构体保存的是解码后和原始的音视频信息。AVFrame通过函数av_frame_alloc()初始化，该函数仅仅分配AVFrame实例本身，而没有分配其内部的缓存。AVFrame实例由av_frame_free()释放；AVFrame实例通常分配一次，重复使用，如分配一个AVFrame实例来保留解码器中输出的视频帧（此时应在恰当的时候使用av_frame_unref()清理参考帧并将AVFrame归零）。该类所描述的数据通常由AVBuffer的API来保存一个引用计数，并保存于AVFrame.buf
/AVFrame.extended_buf，在至少存在一个参考的时候（如AVFrame.buf[0] != NULL），则该对象被标记为“被引用”。在此情况下，AVFrame所包含的每一组数据必须包含于AVFrame的缓存中。





