# EasyNVR使用ffmpeg保存快照数据 - xcyl的口袋 - CSDN博客
2017年09月22日 11:42:58[xcyl](https://me.csdn.net/cai6811376)阅读数：353
EasyNVR提供快照预览功能，并且提供向EasyDSS云平台上传快照的功能
EasyNVR会定时向配置的摄像机抓取快照数据，保存图片用于预览，并且用于快照上传
**原理**
将从摄像机取出来的I帧数据编码成jpeg图片数据
**实现**
AVPacket管理，使用wrapper管理AVPacket的资源
```
class AVPacketWrapper
{
public:
    AVPacketWrapper()
        : packet_(new AVPacket)
    {
        av_init_packet(packet_);
        packet_->data = NULL;
        packet_->size = 0;
    }
    explicit AVPacketWrapper(size_t size)
        : packet_(new AVPacket)
    {
        av_new_packet(packet_, size);
    }
    ~AVPacketWrapper()
    {
        if (packet_)
        {
            av_packet_unref(packet_);
            delete packet_;
        }
    }
    AVPacket* getPacket() const
    {
        return packet_;
    }
private:
    AVPacketWrapper(const AVPacketWrapper&);
    AVPacketWrapper& operator =(const AVPacketWrapper&);
    AVPacket* packet_;
};
```
将h264数据解码
```cpp
int DecoderHelper::DecodeVideo(char* inBuff, int inBuffSize)
{
    if (!inBuff)
        return -1;
    if (1 > inBuffSize)
        return -1;
    if (!avCodecContext_)
    {
        releaseVideoDecoder();
        return -2;
    }
    AVPacketWrapper packetWrapper;
    AVPacket* packet = packetWrapper.getPacket();
    packet->size = inBuffSize;
    packet->data = reinterpret_cast<uint8_t*>(inBuff);
    int ret = avcodec_send_packet(avCodecContext_, packet);
    if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
    {
        return -3;
    }
    ret = avcodec_receive_frame(avCodecContext_, avFrame_);
    if (ret < 0 && ret != AVERROR_EOF)
    {
        return -3;
    }
    return 0;
}
```
编码成图片保存
```
int DecoderHelper::WriteJPEG(const char* out_file) const
{
    // 输出文件路径  
    if (!out_file)
        return -1;
    // 分配AVFormatContext对象  
    AVFormatContext* avFormatContext = avformat_alloc_context();
    // 设置输出文件格式  
    avFormatContext->oformat = av_guess_format("mjpeg", NULL, NULL);
    // 创建并初始化一个和该url相关的AVIOContext  
    if (avio_open(&avFormatContext->pb, out_file, AVIO_FLAG_READ_WRITE) < 0)
    {
        printf("Couldn't open output file.");
        return -2;
    }
    // 构建一个新stream  
    AVStream* avStream = avformat_new_stream(avFormatContext, NULL);
    if (!avStream)
    {
        return -1;
    }
    // Begin Output some information  
    //av_dump_format(pFormatCtx, 0, out_file, 1);
    // End Output some information  
    AVCodecParameters* codecpar = avStream->codecpar;
    codecpar->codec_id = avFormatContext->oformat->video_codec;
    // 查找解码器  
    AVCodec* codec = avcodec_find_encoder(codecpar->codec_id);
    if (!codec)
    {
        printf("Codec not found.");
        return -1;
    }
    // 设置该stream的信息  
    AVCodecContext* avCodecContext = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(avCodecContext, codecpar);
    avCodecContext->codec_type = AVMEDIA_TYPE_VIDEO;
    avCodecContext->pix_fmt = AV_PIX_FMT_YUVJ420P;
    avCodecContext->width = width_;
    avCodecContext->height = height_;
    avCodecContext->time_base.num = 1;
    avCodecContext->time_base.den = 25;
    // 设置pCodecCtx的解码器为pCodec  
    int ret = avcodec_open2(avCodecContext, codec, NULL);
    if (ret < 0)
    {
        printf("Could not open codec.ret=%d", ret);
        return -1;
    }
    //Write Header  
    avformat_write_header(avFormatContext, NULL);
    //Encode  
    // 给AVPacket分配足够大的空间  
    AVPacketWrapper packetWrapper(width_ * height_ * 3);
    AVPacket* packet = packetWrapper.getPacket();
    ret = avcodec_send_frame(avCodecContext, avFrame_);
    if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
    {
        return -3;
    }
    ret = avcodec_receive_packet(avCodecContext, packet);
    if (ret < 0 && ret != AVERROR_EOF)
    {
        return -3;
    }
    av_write_frame(avFormatContext, packet);
    //Write Trailer  
    av_write_trailer(avFormatContext);
    avio_close(avFormatContext->pb);
    avcodec_free_context(&avCodecContext);
    avformat_free_context(avFormatContext);
    return 0;
}
```
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2017
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
