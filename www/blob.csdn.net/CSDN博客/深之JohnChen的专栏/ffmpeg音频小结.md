# ffmpeg音频小结 - 深之JohnChen的专栏 - CSDN博客

2018年06月16日 15:54:33[byxdaz](https://me.csdn.net/byxdaz)阅读数：1286


                一、ffmpeg解码音频流程
1.注册所有组件
av_register_all();
2.拿到封装格式上下文
AVFormatContext *avFormatContext = avformat_alloc_context();
3.打开文件
avformat_open_input
4.查找音频流信息
avformat_find_stream_info(avFormatContext, NULL)
//音频解码，需要找到对应的AVStream所在的avFormatContext->streams的索引位置
5.查找解码器
avcodec_find_decoder(avCodecCtx->codec_id)
6.打开解码器
avcodec_open2(avCodecCtx, aVCodec, NULL)
7.读取一帧一帧的压缩音频数据
av_read_frame(avFormatContext, aVPacket)
8.开始解码
avcodec_decode_audio4(avCodecCtx, aVFrame, &got_frame_ptr,aVPacket)
9.解码得到的Frame数据，转成PCM
swr_convert(swrCtx, &outBuffer, out_nb_samples,aVFrame->data, aVFrame->nb_samples)

二、ffmpeg编码音频流程
1.注册所有组件
av_register_all();
2.拿到封装格式上下文
AVFormatContext *avFormatContext = avformat_alloc_context();
3.avio_open
avio_open(&avFormatContext->pb,out_file, AVIO_FLAG_READ_WRITE);
4.申请音频流
avformat_new_stream
5.查找编码器
avcodec_find_encoder
6.打开编码器
avcodec_open2(pCodecCtx, pCodec,NULL)
7.申请frame
pFrame = av_frame_alloc();
av_samples_get_buffer_size
avcodec_fill_audio_frame
8.写头信息
avformat_write_header
9.编码，写数据
avcodec_encode_audio2
av_write_frame
10.Flush Encoder
11.写尾信息
av_write_trailer

三、FFmpeg中音频编解码参数nb_smples,frame_size以及profile等介绍。
AVCodecContext int frame_size;//Samples per packet。对于ffmpeg音频的codec，表示编码器音频编码能力，每次只能编这个数量的采样点数。
AVFrame int nb_samples;//number of audio samples (per channel) described by this frame。对于ffmpeg音频的frame，表示frame中采样的数量。
一般设置AVFrame.nb_samples = AVCodecContext.frame_size;
所以，每次编码之前必须设置frame的参数，下面的sample是我从PCM文件中读取数据，并编码的程序片段。
    pAudioFrame = avcodec_alloc_frame();
    pAudioFrame->nb_samples= pAudioEncodeCtx->frame_size;
    pAudioFrame->format= pAudioEncodeCtx->sample_fmt;

    //依据channel，nb_sample，sample_fmt 计算frame的数据块的大小
    int size = av_samples_get_buffer_size(NULL, pAudioEncodeCtx->channels,pAudioEncodeCtx->frame_size,pAudioEncodeCtx->sample_fmt, 1);
    uint8_t * frame_buf = (uint8_t *)av_malloc(size);
    //依据channel，nb_sample，sample_fmt 及frame的数据块的大小，设置frame中的信息
    avcodec_fill_audio_frame(pAudioFrame, pAudioEncodeCtx->channels, pAudioEncodeCtx->sample_fmt,(const uint8_t*)frame_buf, size, 1);
编码过程中，如果nb_samples > frame_size时，通过分拆nb_samples为多个frame_size值，放到AUDIO FIFO队列中。如果不分拆直接放数据会出现“more samples than frame size”错误。

对于mp3
nb_samples和frame_size = 1152
一帧数据量：1152*2*av_get_bytes_per_sample(s16) = 4608个字节。
多少帧/秒：88200/(1152*2*av_get_bytes_per_sample(s16)) = 19.1帧数据

对于AAC（当aac编码级别是LC时frame_size 和nb_samples是1024）
nb_samples和frame_size = 1024
一帧数据量：1024*2*av_get_bytes_per_sample(s16) = 4096个字节。
多少帧/秒：88200/(1024*2*av_get_bytes_per_sample(s16)) = 21.5帧数据
对于AAC（当aac编码级别是HE时frame_size 和nb_samples是2048）
nb_samples和frame_size = 2048
一帧数据量：2048*2*av_get_bytes_per_sample(s16) = 8192个字节。
多少帧/秒：88200/(2048*2*av_get_bytes_per_sample(s16)) = 10.75帧数据  

AVCodecContext中的profile字段含义：对于音频，指定音频编解码格式。

    /**
     * profile
     * - encoding: Set by user.
     * - decoding: Set by libavcodec.
     */
int profile;
#define FF_PROFILE_UNKNOWN -99
#define FF_PROFILE_RESERVED -100

#define FF_PROFILE_AAC_MAIN 0
#define FF_PROFILE_AAC_LOW  1
#define FF_PROFILE_AAC_SSR  2
#define FF_PROFILE_AAC_LTP  3
#define FF_PROFILE_AAC_HE   4
#define FF_PROFILE_AAC_HE_V2 28
#define FF_PROFILE_AAC_LD   22
#define FF_PROFILE_AAC_ELD  38
#define FF_PROFILE_MPEG2_AAC_LOW 128
#define FF_PROFILE_MPEG2_AAC_HE  131

#define FF_PROFILE_DTS         20
#define FF_PROFILE_DTS_ES      30
#define FF_PROFILE_DTS_96_24   40
#define FF_PROFILE_DTS_HD_HRA  50
#define FF_PROFILE_DTS_HD_MA   60

#define FF_PROFILE_MPEG2_422    0
#define FF_PROFILE_MPEG2_HIGH   1
#define FF_PROFILE_MPEG2_SS     2
#define FF_PROFILE_MPEG2_SNR_SCALABLE  3
#define FF_PROFILE_MPEG2_MAIN   4
#define FF_PROFILE_MPEG2_SIMPLE 5

#define FF_PROFILE_H264_CONSTRAINED  (1<<9)  // 8+1; constraint_set1_flag
#define FF_PROFILE_H264_INTRA        (1<<11) // 8+3; constraint_set3_flag

#define FF_PROFILE_H264_BASELINE             66
#define FF_PROFILE_H264_CONSTRAINED_BASELINE (66|FF_PROFILE_H264_CONSTRAINED)
#define FF_PROFILE_H264_MAIN                 77
#define FF_PROFILE_H264_EXTENDED             88
#define FF_PROFILE_H264_HIGH                 100
#define FF_PROFILE_H264_HIGH_10              110
#define FF_PROFILE_H264_HIGH_10_INTRA        (110|FF_PROFILE_H264_INTRA)
#define FF_PROFILE_H264_HIGH_422             122
#define FF_PROFILE_H264_HIGH_422_INTRA       (122|FF_PROFILE_H264_INTRA)
#define FF_PROFILE_H264_HIGH_444             144
#define FF_PROFILE_H264_HIGH_444_PREDICTIVE  244
#define FF_PROFILE_H264_HIGH_444_INTRA       (244|FF_PROFILE_H264_INTRA)
#define FF_PROFILE_H264_CAVLC_444            44

#define FF_PROFILE_VC1_SIMPLE   0
#define FF_PROFILE_VC1_MAIN     1
#define FF_PROFILE_VC1_COMPLEX  2
#define FF_PROFILE_VC1_ADVANCED 3

#define FF_PROFILE_MPEG4_SIMPLE                     0
#define FF_PROFILE_MPEG4_SIMPLE_SCALABLE            1
#define FF_PROFILE_MPEG4_CORE                       2
#define FF_PROFILE_MPEG4_MAIN                       3
#define FF_PROFILE_MPEG4_N_BIT                      4
#define FF_PROFILE_MPEG4_SCALABLE_TEXTURE           5
#define FF_PROFILE_MPEG4_SIMPLE_FACE_ANIMATION      6
#define FF_PROFILE_MPEG4_BASIC_ANIMATED_TEXTURE     7
#define FF_PROFILE_MPEG4_HYBRID                     8
#define FF_PROFILE_MPEG4_ADVANCED_REAL_TIME         9
#define FF_PROFILE_MPEG4_CORE_SCALABLE             10
#define FF_PROFILE_MPEG4_ADVANCED_CODING           11
#define FF_PROFILE_MPEG4_ADVANCED_CORE             12
#define FF_PROFILE_MPEG4_ADVANCED_SCALABLE_TEXTURE 13
#define FF_PROFILE_MPEG4_SIMPLE_STUDIO             14
#define FF_PROFILE_MPEG4_ADVANCED_SIMPLE           15

#define FF_PROFILE_JPEG2000_CSTREAM_RESTRICTION_0   0
#define FF_PROFILE_JPEG2000_CSTREAM_RESTRICTION_1   1
#define FF_PROFILE_JPEG2000_CSTREAM_NO_RESTRICTION  2
#define FF_PROFILE_JPEG2000_DCINEMA_2K              3
#define FF_PROFILE_JPEG2000_DCINEMA_4K              4 

AVCodecContext结构中bits_per_coded_sample字段含义：表示编码压缩bit值与采样率的bit值之比。 
比如：如果为g726音频时，表示g726码流压缩与采样率比值。比如kbps码流压缩比为：40k/8k = 5，kbps码流压缩比为16k/8k = 2。

ffmpeg源码如下，各种音频格式nb_samples和frame_size其他如下：
switch (id) {  
case AV_CODEC_ID_ADPCM_ADX:    return   32;  
case AV_CODEC_ID_ADPCM_IMA_QT: return   64;  
case AV_CODEC_ID_ADPCM_EA_XAS: return  128;  
case AV_CODEC_ID_AMR_NB:  
case AV_CODEC_ID_EVRC:  
case AV_CODEC_ID_GSM:  
case AV_CODEC_ID_QCELP:  
case AV_CODEC_ID_RA_288:       return  160;  
case AV_CODEC_ID_AMR_WB:  
case AV_CODEC_ID_GSM_MS:       return  320;  
case AV_CODEC_ID_MP1:          return  384;  
case AV_CODEC_ID_ATRAC1:       return  512;  
case AV_CODEC_ID_ATRAC3:       return 1024 * framecount;  
case AV_CODEC_ID_ATRAC3P:      return 2048;  
case AV_CODEC_ID_MP2:  
case AV_CODEC_ID_MUSEPACK7:    return 1152;  
case AV_CODEC_ID_AC3:          return 1536;  
}

四、ffmpeg音频重采样有多种方法，一种是libswresample，另外一种是libavfilter。
使用libswresample方法，参考：https://blog.csdn.net/bixinwei22/article/details/78675376
使用libavfilter方法,参考：https://blog.csdn.net/TopsLuo/article/details/72782715

五、利用ffmpeg构造静音帧
AVFrame *alloc_silence_frame(int channels, int samplerate, int format)
{
    AVFrame *frame;
    int32_t ret;
    frame = av_frame_alloc();
    if(!frame)
    {
        return NULL;
    }

    frame->sample_rate = samplerate;
    frame->format = format; /*默认的format:AV_SAMPLE_FMT_FLTP*/
    frame->channel_layout = av_get_default_channel_layout(channels);
    frame->channels = channels;
    frame->nb_samples = AAC_NBSAMPLE; /*默认的sample大小:1024*/
    ret = av_frame_get_buffer(frame, 0);
    if(ret < 0)
    {
        av_frame_free(&frame);
        return NULL;
    }

    av_samples_set_silence(frame->data, 0, frame->nb_samples, frame->channels,frame->format);
    return frame;
}

六、无论要编码成AAC还MP3都需要用到ffmpeg的fifo或者AVAudioFifo做数据缓冲。
七、从音频设备采集到的音频的类型和编码器类型(aac ,amr)通常是不一致的。需要重采样转换。利用swr_convert 重新采样。
这时候我们可能会遇到另外一个问题。就是在encode_audio的时候遇到more samples than frame size (avcodec_encode_audio2)  的问题。
问题的原因在于 我们编码器的frame_size 比采集到的fram->nb_samples小
/* check for valid frame size */
    if (frame) {
        if (avctx->codec->capabilities & CODEC_CAP_SMALL_LAST_FRAME) {
            if (frame->nb_samples > avctx->frame_size) {
                av_log(avctx, AV_LOG_ERROR, "more samples than frame size (avcodec_encode_audio2)\n");
                return AVERROR(EINVAL);
            }
        } else if (!(avctx->codec->capabilities & CODEC_CAP_VARIABLE_FRAME_SIZE)) {
            if (frame->nb_samples < avctx->frame_size &&
                !avctx->internal->last_audio_frame) {
                ret = pad_last_frame(avctx, &padded_frame, frame);
                if (ret < 0)
                    return ret;

                frame = padded_frame;
                avctx->internal->last_audio_frame = 1;
            }

            if (frame->nb_samples != avctx->frame_size) {
                av_log(avctx, AV_LOG_ERROR, "nb_samples (%d) != frame_size (%d) (avcodec_encode_audio2)\n", frame->nb_samples, avctx->frame_size);
                ret = AVERROR(EINVAL);
                goto end;
            }
        }
    }

解决的方案是：每次采集到的frame数据保存到一个AUDIO FIFO中，每次等凑齐 刚好frame_size大小的数据(请注意这里是刚好，网上有人说拆分AVframe，本人在ffmpeg2.1用这个办法没办法解决)，接着把这些数据一口气encode。 
如果有剩余的数据就存储到下一次encode。可以使用audio_fifo_来操作数据。

八、音频播放
1、新版ffmpeg解码音频输出的格式可能不满足S16，如AAC解码后得到的是FLT（浮点型），AC3解码是FLTP（带平面）等，需要根据具体的情况决定是否需要convert，转换过程与上类似 。
编码依旧使用 AV_SAMPLE_FMT_S16格式 也就说，音频编码不能再像以前那样简单的处理，统一输入S16数据，而要根据具体的codec转化为其支持的格式，否则无论是编码还是解码输出的声音会莫名其妙，幸好，转换工作不用自己做，ffmpeg提供了相应的API，swr_convert（类似以前的audio_resample，只是audio_resample目前已不再推荐使用，因为swr_convert更强大）
2、m4a,aac文件可以直接播放。mp3文件需要调整SDL音频帧大小为4608（默认是4096），否则播放会不流畅。

九、常见音频格式介绍
1、AAC帧格式及编码

http://blog.csdn.net/sunnylgz/article/details/7676340

aac解码失败的问题
视音频分离器（Demuxer）,并不适用于一些格式。对于MP3编码的音频是没有问题的。但是在分离MP4/FLV/MKV等一些格式中的AAC编码的码流的时候，得到的AAC码流是不能播放的。原因是存储AAC数据的AVPacket的data字段中的数据是不包含7字节ADTS文件头的“砍头”的数据，是无法直接解码播放的（当然如果在这些数据前面手工加上7字节的ADTS文件头的话，就可以播放了）。
通过aac_adtstoasc过滤器转换增加adts头。或者手工写代码增加adts头（7字节）。
手工写代码增加adts头参考：
https://blog.csdn.net/godvmxi/article/details/52875107

2、mp3
https://blog.csdn.net/wlsfling/article/details/5875959
关于libmad开源mp3音频解码库的使用
https://blog.csdn.net/gepanqiang3020/article/details/73695483
3、AMR音频编解码
http://blog.csdn.net/dinggo/article/details/2002298
4、G726
PCM音频，采样率：8kHZ，位宽：16bit，声道：1单声道
G726编码16/24/32/40 采用40，将数据变成了80kpbs。
在80kbps下，每帧均为100个字节， 则：
每帧时间长度=(每帧大小*8)/音频流大小 * 1000
例如：sample_rate=8000HZ,bit_width=16,channels=1时,计算出的时长为10ms.
HaiSi解码库
G726 将一个64kbps数据转换压缩成16、24、32、40kbps数据。
私有海斯头4个字节（0、1、50、0）+100字节音频数据。
50*2+4 表示一包大小。

