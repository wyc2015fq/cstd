# ffmpeg多个不同帧率不同格式的视频合并 - 建建的博客 - CSDN博客
2017年02月07日 17:19:06[纪建](https://me.csdn.net/u013898698)阅读数：5375
个人分类：[ffmpeg学习](https://blog.csdn.net/u013898698/article/category/6692312)
相同视频编码格式之间是很好合并的，只需要按照流程打开文件，然后一帧一帧的读取，连生成视频的参数都不需要变得，唯一需要的就是改下pts就好了但是这块我一般不会拷贝原有的视频流参数，因为那样时间戳不好控制，
不同编码格式的视频合并就需要先解码每一帧数据，解码成最原始的YUV格式，然后在编码成你想要的编码格式。这样几乎所有的数据都一样了
FFMPEG解码流程：
  1. 注册所有容器格式和CODEC: av_register_all()
  2. 打开文件: av_open_input_file()
  3. 从文件中提取流信息: av_find_stream_info()
  4. 穷举所有的流，查找其中种类为CODEC_TYPE_VIDEO
  5. 查找对应的解码器: avcodec_find_decoder()
  6. 打开编解码器: avcodec_open()
  7. 为解码帧分配内存: avcodec_alloc_frame()
  8. 不停地从码流中提取出帧数据: av_read_frame()
  9. 判断帧的类型，对于视频帧调用: avcodec_decode_video()
  10. 解码完后，释放解码器: avcodec_close()
  11. 关闭输入文件: avformat_close_input_file()
基本概念:
   编解码器、数据帧、媒体流和容器是数字媒体处理系统的四个基本概念。
首先需要统一术语：
    容器／文件（Conainer/File）：即特定格式的多媒体文件。
    媒体流（Stream）：指时间轴上的一段连续数据，如一段声音数据，一段视频数据或一段字幕数据，可以是压缩的，也可以是非压缩的，压缩的数据需要关联特定的编解码器。
    数据帧／数据包（Frame/Packet）：通常，一个媒体流由大量的数据帧组成，对于压缩数据，帧对应着编解码器的最小处理单元。通常，分属于不同媒体流的数据帧交错复用于容器之中，参见交错。
    编解码器：编解码器以帧为单位实现压缩数据和原始数据之间的相互转换。
在FFMPEG中，使用AVFormatContext、AVStream、AVCodecContext、AVCodec及AVPacket等结构来抽象这些基本要素，它们的关系如上图所示：
AVCodecContext：
    这是一个描述编解码器上下文的数据结构，包含了众多编解码器需要的参数信息，如下列出了部分比较重要的域：
typedef struct AVCodecContext {
 / **
     *一些编解码器需要/可以像使用extradata Huffman表。
     * MJPEG：Huffman表
     * RV10其他标志
     * MPEG4：全球头（也可以是在比特流或这里）
     *分配的内存应该是FF_INPUT_BUFFER_PADDING_SIZE字节较大
     *，比extradata_size避免比特流器，如果它与读prolems。
     * extradata按字节的内容必须不依赖于架构或CPU的字节顺序。
     * - 编码：设置/分配/释放由libavcodec的。
     * - 解码：由用户设置/分配/释放。
     * /
    uint8_t *extradata;
    int extradata_size;
   / **
     *这是时间的基本单位，在条件（以秒为单位）
     *帧时间戳派代表出席了会议。对于固定fps的内容，
     *基应该1/framerate和时间戳的增量应该
     *相同的1。
     * - 编码：必须由用户设置。
     * - 解码：libavcodec的设置。
     * /
    AVRational time_base;
    enum CodecID codec_id;
 / **
     *的fourcc（LSB在前，所以“的ABCD” - >（“D”<< 24）（“C”<< 16）（“B”<< 8）+“A”）。
     *这是用来解决一些编码错误。
     *分路器应设置什么是编解码器用于识别领域中。
     *如果有分路器等多个领域，在一个容器，然后选择一个
     *最大化使用的编解码器有关的信息。
     *如果在容器中的编解码器标记字段然后32位大分路器应该
     *重新映射到一个表或其他结构的32位编号。也可选择新
     * extra_codec_tag+大小可以添加，但必须证明这是一个明显的优势
     *第一。
     * - 编码：由用户设置，如果没有则默认基础上codec_id将使用。
     * - 解码：由用户设置，将被转换成在初始化libavcodec的大写。
     * /
    unsigned int codec_tag;
    ......
    / **
     *在解码器的帧重排序缓冲区的大小。
     *对于MPEG-2，这是IPB1或0低延时IP。
     * - 编码：libavcodec的设置。
     * - 解码：libavcodec的设置。
     * /
    int has_b_frames;
   / **
     *每包的字节数，如果常量和已知或0
     *用于一些WAV的音频编解码器。
     * /
    int block_align;
    / **
     *从分路器位每个样品/像素（huffyuv需要）。
     * - 编码：libavcodec的设置。
     * - 解码：由用户设置。
     * /
     int bits_per_coded_sample;
     .....
} AVCodecContext;
如 果是单纯使用libavcodec，这部分信息需要调用者进行初始化；如果是使用整个FFMPEG库，这部分信息在调用 avformat_open_input和avformat_find_stream_info的过程中根据文件的头信息及媒体流内的头部信息完成初始 化。其中几个主要域的释义如下：
    extradata/extradata_size：这个buffer中存放了解码器可能会用到的额外信 息，在av_read_frame中填充。一般来说，首先，某种具体格式的demuxer在读取格式头信息的时候会填充extradata，其次，如果 demuxer没有做这个事情，比如可能在头部压根儿就没有相关的编解码信息，则相应的parser会继续从已经解复用出来的媒体流中继续寻找。在没有找
 到任何额外信息的情况下，这个buffer指针为空。
    time_base：
    width/height：视频的宽和高。
    sample_rate/channels：音频的采样率和信道数目。
    sample_fmt： 音频的原始采样格式。
        codec_name/codec_type/codec_id/codec_tag：编解码器的信息。
AVStrea
   该结构体描述一个媒体流，定义如下：
typedef struct AVStream {
    int index;
    AVCodecContext *codec;
    / **
     *流的实时帧率基地。
     *这是所有时间戳可以最低帧率
     *准确代表（它是所有的最小公倍数
     *流的帧率）。请注意，这个值只是一个猜测！
     *例如，如果时间基数为1/90000和所有帧
     *约3600或1800计时器刻度，，然后r_frame_rate将是50/1。
     * /
    AVRational r_frame_rate;
   / **
     *这是时间的基本单位，在条件（以秒为单位）
     *帧时间戳派代表出席了会议。对于固定fps的内容，
     *时基应该是1/framerate的时间戳的增量应为1。
     * /
    AVRational time_base;
    ......
    / **
     *解码流量的第一帧，在流量时-base分。
     *如果你是绝对100％的把握，设定值
     *它真的是第一帧点。
     *这可能是未定义（AV_NOPTS_VALUE）的。
     *@注意的业余头不弱者受制与正确的START_TIME的业余
     *分路器必须不设定此。
     * /
    int64_t start_time;
    / **
     *解码：时间流流时基。
     *如果源文件中没有指定的时间，但不指定
     *比特率，这个值将被从码率和文件大小的估计。
     * /
    int64_t duration;
#if LIBAVFORMAT_VERSION_INT < (53<<16)
    char language[4];
#endif
            / *流信息* /
            int64_t timestamp;
        #if LIBAVFORMAT_VERSION_INT < (53<<16)
            char title[512];
            char author[512];
            char copyright[512];
            char comment[512];
            char album[512];
            int year;
            int track;
            char genre[32];
        #endif
            int ctx_flags;
            int64_t data_offset;
            int index_built;
            int mux_rate;
            unsigned int packet_size;
            int preload;
            int max_delay;
        #define AVFMT_NOOUTPUTLOOP -1
        #define AVFMT_INFINITEOUTPUTLOOP 0
            int loop_output;
            int flags;
        #define AVFMT_FLAG_GENPTS 0x0001 ///< 生成失踪分，即使它需要解析未来框架。
        #define AVFMT_FLAG_IGNIDX 0x0002 ///< 忽略指数。
        #define AVFMT_FLAG_NONBLOCK 0x0004 ///<从输入中读取数据包时，不要阻止。
        #define AVFMT_FLAG_IGNDTS 0x0008 ///< 忽略帧的DTS包含DTS与PTS
        #define AVFMT_FLAG_NOFILLIN 0x0010 ///< 不要从任何其他值推断值，只是返回存储在容器中
        #define AVFMT_FLAG_NOPARSE 0x0020 ///< 不要使用AVParsers，你还必须设置为FILLIN帧代码的工作，没有解析AVFMT_FLAG_NOFILLIN - >无帧。也在寻求框架不能工作，如果找到帧边界的解析已被禁用
        #define AVFMT_FLAG_RTP_HINT 0x0040 ///< 暗示到输出文件添加的RTP
            int loop_input;
            CODEC_ID_MPEG1VIDEO,
            CODEC_ID_MPEG2VIDEO, ///< preferred ID for MPEG-1/2 video decoding
            CODEC_ID_MPEG2VIDEO_XVMC,
            CODEC_ID_H261,
            CODEC_ID_H263,
            ...
        };
        通 常，如果某种媒体格式具备完备而正确的头信息，调用avformat_open_input即可以得到这两个参数，但若是因某种原因 avformat_open_input无法获取它们，这一任务将由avformat_find_stream_info完成。
        其次还要获取各媒体流对应编解码器的时间基准。
        此外，对于音频编解码器，还需要得到：
            采样率，
            声道数，
            位宽，
           帧长度（对于某些编解码器是必要的），
        对于视频编解码器，则是：
            图像大小，
            色彩空间及格式，
        av_read_frame
        int av_read_frame(AVFormatContext *s, AVPacket *pkt);
        这 个函数用于从多媒体文件或多媒体流中读取媒体数据，获取的数据由AVPacket结构pkt来存放。对于音频数据，如果是固定比特率，则pkt中装载着一 个或多个音频帧；如果是可变比特率，则pkt中装载有一个音频帧。对于视频数据，pkt中装载有一个视频帧。需要注意的是：再次调用本函数之前，必须使用
 av_free_packet释放pkt所占用的资源。
        通过pkt→stream_index可以查到获取的媒体数据的类型，从而将数据送交相应的解码器进行后续处理。
        av_seek_frame
        int av_seek_frame(AVFormatContext *s, int stream_index, int64_t timestamp, int flags);
        这个函数通过改变媒体文件的读写指针来实现对媒体文件的随机访问，支持以下三种方式：
            基于时间的随机访问：具体而言就是将媒体文件读写指针定位到某个给定的时间点上，则之后调用av_read_frame时能够读到时间标签等于给定时间点的媒体数据，通常用于实现媒体播放器的快进、快退等功能。
            基于文件偏移的随机访问：相当于普通文件的seek函数，timestamp也成为文件的偏移量。
            基于帧号的随机访问：timestamp为要访问的媒体数据的帧号。
        关于参数：
            s：是个AVFormatContext指针，就是avformat_open_input返回的那个结构。
            stream_index： 指定媒体流，如果是基于时间的随机访问，则第三个参数timestamp将以此媒体流的时间基准为单位；如果设为负数，则相当于不指定具体的媒体 流，FFMPEG会按照特定的算法寻找缺省的媒体流，此时，timestamp的单位为AV_TIME_BASE（微秒）。
            timestamp：时间标签，单位取决于其他参数。
            flags：定位方式，AVSEEK_FLAG_BYTE表示基于字节偏移，AVSEEK_FLAG_FRAME表示基于帧号，其它表示基于时间。
av_close_input_file:
        void av_close_input_file(AVFormatContext *s);
        关闭一个媒体文件：释放资源，关闭物理IO。
avcodec_find_decoder:
        AVCodec *avcodec_find_decoder(enum CodecID id);
        AVCodec *avcodec_find_decoder_by_name(const char *name);
        根据给定的codec id或解码器名称从系统中搜寻并返回一个AVCodec结构的指针。
avcodec_open:
        int avcodec_open(AVCodecContext *avctx, AVCodec *codec);
        此 函数根据输入的AVCodec指针具体化AVCodecContext结构。在调用该函数之前，需要首先调用avcodec_alloc_context 分配一个AVCodecContext结构，或调用avformat_open_input获取媒体文件中对应媒体流的AVCodecContext结
 构；此外还需要通过avcodec_find_decoder获取AVCodec结构。
        这一函数还将初始化对应的解码器。
avcodec_decode_video2
        int avcodec_decode_video2(AVCodecContext *avctx, AVFrame *picture, int *got_picture_ptr, AVPacket *avpkt);
        解码一个视频帧。got_picture_ptr指示是否有解码数据输出。
        输入数据在AVPacket结构中，输出数据在AVFrame结构中。AVFrame是定义在avcodec.h中的一个数据结构：
        typedef struct AVFrame {
            FF_COMMON_FRAME
        } AVFrame;
        FF_COMMON_FRAME定义了诸多数据域，大部分由FFMpeg内部使用，对于用户来说，比较重要的主要包括：
        #define FF_COMMON_FRAME \
        ......
            uint8_t *data[4];\
            int linesize[4];\
            int key_frame;\
            int pict_type;\
            int64_t pts;\
            int reference;\
        ......
        FFMpeg内部以planar的方式存储原始图像数据，即将图像像素分为多个平面（R/G/B或Y/U/V），data数组内的指针分别指向四个像素平面的起始位置，linesize数组则存放各个存贮各个平面的缓冲区的行宽：
        +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        +++data[0]->#################################++++++++++++
        ++++++++++++###########picture data##########++++++++++++
                    ........................
        ++++++++++++#################################++++++++++++
        |<-------------------line_size[0]---------------------->|
        此 外，key_frame标识该图像是否是关键帧；pict_type表示该图像的编码类型：I(1)/P(2)/B(3)……；pts是以 time_base为单位的时间标签，对于部分解码器如H.261、H.263和MPEG4，可以从头信息中获取；reference表示该图像是否被用 作参考。
avcodec_decode_audio4
        int avcodec_decode_audio4(AVCodecContext *avctx, AVFrame *frame, int *got_frame_ptr, AVPacket *avpkt);
        解码一个音频帧。输入数据在AVPacket结构中，输出数据在frame中，got_frame_ptr表示是否有数据输出。
avcodec_close
        int avcodec_close(AVCodecContext *avctx);
        关闭解码器，释放avcodec_open中分配的资源。
我做的一个测试程序
#include "stdafx.h"
#include <iostream>
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavfilter/avfiltergraph.h"
#include "libavfilter/avcodec.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
#include "libavutil/avutil.h"
#include "libavutil/opt.h"
#include "libavutil/pixdesc.h"
};
AVFormatContext* m_outFormatCtx;
AVOutputFormat* m_fmt;
AVStream* video_st;
AVCodecContext* m_pCodecCtx;
AVCodec* pCodec;
AVPacket pkt;
uint8_t* picture_buf;
AVFrame* picture;
int size;
int framenum = 50;
int flush_encoder(AVFormatContext *fmt_ctx, unsigned int stream_index)
{
int ret;
int got_frame;
AVPacket enc_pkt;
if (!(fmt_ctx->streams[stream_index]->codec->codec->capabilities &
CODEC_CAP_DELAY))
return 0;
while (1) {
printf("Flushing stream #%u encoder\n", stream_index);
//ret = encode_write_frame(NULL, stream_index, &got_frame);
enc_pkt.data = NULL;
enc_pkt.size = 0;
av_init_packet(&enc_pkt);
ret = avcodec_encode_video2(fmt_ctx->streams[stream_index]->codec, &enc_pkt,
NULL, &got_frame);
av_frame_free(NULL);
if (ret < 0)
break;
if (!got_frame)
{
ret = 0; break;
}
printf("编码成功1帧！\n");
/* mux encoded frame */
ret = av_write_frame(fmt_ctx, &enc_pkt);
if (ret < 0)
break;
}
return ret;
}
void CloseVideo() 
{
//写文件尾
av_write_trailer(m_outFormatCtx);
//清理
if (video_st)
{
avcodec_close(video_st->codec);
av_free(picture);
av_free(picture_buf);
}
avio_close(m_outFormatCtx->pb);
avformat_free_context(m_outFormatCtx);
}
int InitOutputVideo(int width,int height) 
{
const char* out_file = "src01.h264";//输出文件路径
av_register_all();
//方法1.组合使用几个函数
m_outFormatCtx = avformat_alloc_context();
//猜格式
m_fmt = av_guess_format(NULL, out_file, NULL);
m_outFormatCtx->oformat = m_fmt;
//方法2.更加自动化一些
//avformat_alloc_output_context2(&pFormatCtx, NULL, NULL, out_file);
//fmt = pFormatCtx->oformat;
//注意输出路径
if (avio_open(&m_outFormatCtx->pb, out_file, AVIO_FLAG_READ_WRITE) < 0)
{
printf("输出文件打开失败");
return -1;
}
video_st = av_new_stream(m_outFormatCtx, 0);
if (video_st == NULL)
{
return -1;
}
m_pCodecCtx = video_st->codec;
m_pCodecCtx->codec_id = m_fmt->video_codec;
m_pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
m_pCodecCtx->pix_fmt = PIX_FMT_YUV420P;
m_pCodecCtx->width = width;
m_pCodecCtx->height = height;
m_pCodecCtx->time_base.num = 1;
m_pCodecCtx->time_base.den = 25;
m_pCodecCtx->bit_rate = 400000;
m_pCodecCtx->gop_size = 250;
//H264
//pCodecCtx->me_range = 16;
//pCodecCtx->max_qdiff = 4;
m_pCodecCtx->qmin = 10;
m_pCodecCtx->qmax = 51;
//pCodecCtx->qcompress = 0.6;
//输出格式信息
av_dump_format(m_outFormatCtx, 0, out_file, 1);
pCodec = avcodec_find_encoder(m_pCodecCtx->codec_id);
if (!pCodec)
{
printf("没有找到合适的编码器！\n");
return -1;
}
if (avcodec_open2(m_pCodecCtx, pCodec, NULL) < 0)
{
printf("编码器打开失败！\n");
return -1;
}
picture = avcodec_alloc_frame();
size = avpicture_get_size(m_pCodecCtx->pix_fmt, m_pCodecCtx->width, m_pCodecCtx->height);
picture_buf = (uint8_t *)av_malloc(size);
avpicture_fill((AVPicture *)picture, picture_buf, m_pCodecCtx->pix_fmt, m_pCodecCtx->width, m_pCodecCtx->height);
//写文件头
avformat_write_header(m_outFormatCtx, NULL);
int y_size = m_pCodecCtx->width * m_pCodecCtx->height;
av_new_packet(&pkt, y_size * 3);
return 1;
}
int  _tmain(int argc, _TCHAR* argv[])
{
AVFormatContext *m_formatctx;//文件格式封装上下文
AVStream *m_videostream;//视频流
AVCodecContext *m_videocodeccontext;//编码格式上下文
//AVCodec* m_videocodec；
AVFrame* m_pFrame;//
AVFrame* m_pFrameRGB;//
AVPacket* m_packet;//
int m_videostreamindex;//
int m_singleframedur;//单帧需要多长时间
int m_dbFrameRate;//帧率
int m_dswidth;//宽度
int m_dsheight;//高度
int m_gotpicture;
std::string m_filepath="cuc_ieschool.ts";
m_formatctx = NULL;
av_register_all();
if (avformat_open_input(&m_formatctx, m_filepath.c_str(), NULL, NULL) != 0)
{
return -1;
}
if (avformat_find_stream_info(m_formatctx, NULL) < 0)
{
return -1;
}
//得到文件总长度
m_singleframedur = m_formatctx->duration / 1000.0 / 1000.0; //从纳秒转换成秒、
if (m_singleframedur < 0)
{
m_singleframedur = 0;
}
int videoindex = 0;
for (int i = 0; i < m_formatctx->nb_streams; i++)
{
if (m_formatctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
{
videoindex = i;
break;
}
}
m_videostreamindex = videoindex;
m_videostream = m_formatctx->streams[m_videostreamindex];
m_videocodeccontext = m_videostream->codec;
AVCodec* localcodec = avcodec_find_decoder(m_videocodeccontext->codec_id);
InitOutputVideo(m_videostream->codec->width, m_videostream->codec->height);
//视频的帧率
m_dbFrameRate = av_q2d(m_videostream->r_frame_rate);//内部时间基
//这里说明是裸流
if (m_dbFrameRate < 15 || m_dbFrameRate > 50)
{
//这种是MP3 有图片的
m_dbFrameRate = 25.0;
}
//这里加一个判断是用于对不同的格式进行pts单位的统一化：统一到 “秒”,可扩展其它格式
if (strstr(m_formatctx->iformat->name, "mpegts") != NULL)
{
double time_base = 90 * 1000;
m_singleframedur = time_base / m_dbFrameRate / 100000;
}
else
{
m_singleframedur = 1 / m_dbFrameRate;
}
if (localcodec == NULL)
{
return -1;
}
if (avcodec_open2(m_videocodeccontext, localcodec, NULL) < 0)
{
return -1;
}
m_pFrame = av_frame_alloc();
m_pFrameRGB = av_frame_alloc();
m_packet = (AVPacket *)av_malloc(sizeof(AVPacket));
int i = 0;
while (av_read_frame(m_formatctx, m_packet) >= 0)
{
if (m_packet->stream_index == m_videostreamindex)
{
int rec = avcodec_decode_video2(m_videocodeccontext, picture, &m_gotpicture, m_packet);
if (rec > 0)
{
if (m_gotpicture)
{
picture->pts = i;
i++;
int got_picture = 0;
//编码
int ret = avcodec_encode_video2(m_pCodecCtx, &pkt, picture, &got_picture);
if (ret < 0)
{
printf("编码错误！\n");
return -1;
}
if (got_picture == 1)
{
printf("编码成功1帧！\n");
pkt.stream_index = video_st->index;
ret = av_write_frame(m_outFormatCtx, &pkt);
av_free_packet(&pkt);
}
}
}
}
av_free_packet(m_packet);
}
int ret = flush_encoder(m_outFormatCtx, 0);
if (ret < 0) {
printf("Flushing encoder failed\n");
return -1;
}
CloseVideo();
m_formatctx = NULL;
m_videostream = NULL;
avcodec_close(m_videocodeccontext);
av_free_packet(m_packet);
av_free(m_pFrame);
av_free(m_pFrameRGB);
return 1;
}
