# 使用RTSP判断摄像机设备是否在线以及快照抓取 - xcyl的口袋 - CSDN博客
2017年08月13日 23:18:13[xcyl](https://me.csdn.net/cai6811376)阅读数：1622
## 是否在线
EasyNVR可以使用Onvif/RTSP协议接入摄像机
在EasyNVR应用中非常重要的一环就是设备的在线状态判断和快照抓取
设备在线状态判断有很多种方法又没有什么方法
说有很多种方法指的是，我们可以使用ICMP协议对设备的进行ping操作，或者可以对设备的特定端口进行连接获取判断连接是否一次，再或者使用RTSP协议流程进行判定，但是都有个问题–不靠谱，禁ping，端口未知，RTSP协议实现不标准，都会导致–不靠谱
所以说，没什么方法
作为EasyNVR来说，肯定需要一个靠谱的，能够准确判定设备是否在线的方法
那就是–走RTSP流程拉流
拉流都成功了，你肯定在线，否则你不在线，不然在线联网又有什么用？我又不能用
## 快照抓取
快照抓取这件事就很简单了
使用我们的大杀器**EasyRTSPClient**，定时将拉取的I帧转码保存成图片，这很简洁明了
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
## 好的配合
我们何不把快照抓取和在线状态判定做到一起呢？
是的，抓取到快照我们判断为在线，否则为离线
这样，摄像机的在线状态是“真正的”在线状态
![这里写图片描述](https://img-blog.csdn.net/20170813231712959?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170813231723234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2017
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
