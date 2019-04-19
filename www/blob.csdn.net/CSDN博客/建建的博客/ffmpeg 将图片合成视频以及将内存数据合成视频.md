# ffmpeg  将图片合成视频以及将内存数据合成视频 - 建建的博客 - CSDN博客
2017年01月24日 17:30:09[纪建](https://me.csdn.net/u013898698)阅读数：3175
av_register_all();//初始化解码器和复用器
AVFormatContext*  ofmt_ctx = NULL;//其包含码流参数较多，是一个贯穿始终的数据结构，很多函数都要用到它作为参数
//首先创建一个封装格式，
avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, out_filename.c_str());//初始化一个用于输出的AVFormatContext结构体，视频帧率和宽高在此函数里面设置
 AVStream *st;//构造一个输出流
AVCodec *codec;
st = avformat_new_stream(oc, NULL);//初始化输出流
codec = avcodec_find_encoder(codec_id);//查找mjpeg解码器
avcodec_get_context_defaults3(st->codec, codec);//申请AVStream->codec(AVCodecContext对象)空间并设置默认值(由avcodec_get_context_defaults3()设置
st->codec->bit_rate = 400000;//设置采样参数，即比特率  
st->codec->width = 640;//设置视频宽高，这里跟图片的宽高保存一致即可
st->codec->height = 360;
st->codec->time_base.den = 25;//设置帧率
st->codec->time_base.num = 1;
st->codec->pix_fmt = PIX_FMT_YUV420P;//设置像素格式  
st->codec->codec_tag = 0;
if (oc->oformat->flags & AVFMT_GLOBALHEADER)//一些格式需要视频流数据头分开
st->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
if (!(ofmt_ctx->oformat->flags & AVFMT_NOFILE))//打开输出视频文件
{
ret = avio_open(&ofmt_ctx->pb, out_filename.c_str(), AVIO_FLAG_WRITE);
if (ret < 0) {
printf("Could not open output file '%s'", out_filename.c_str());
return;
}
}
if (avformat_write_header(ofmt_ctx, NULL) < 0)//写文件头（Write file header）
{
printf("Error occurred when opening output file\n");
return;
}
FILE *file;//打开一张jpeg图像并读取其数据，在这里图像最大为1M,如果超过1M，则需要修改1024*1024这里
fopen_s(&file, "123.jpg", "rb");
pkt.size = fread(mydata, 1, DATASIZE, file);
pkt.data = mydata;
fclose(file);
if (av_interleaved_write_frame(ofmt_ctx, &pkt) < 0) //写入图像到视频
{
printf("Error muxing packet\n");
return;
}
这个地方读取图片可以直接替换成其他内存，但是图片编码格式要一致，这里是yuv
av_free_packet(&pkt);//释放掉帧数据包对象
av_write_trailer(ofmt_ctx);//写文件尾（Write file trailer）
//delete[]mydata;//释放数据对象
if (ofmt_ctx && !(ofmt_ctx->oformat->flags & AVFMT_NOFILE))
avio_close(ofmt_ctx->pb);//关闭视频文件
avformat_free_context(ofmt_ctx);//释放输出视频相关数据结构
delete mydata;
最重要的是视频编码解码参数设置

