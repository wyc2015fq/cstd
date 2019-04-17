# mux复用 demux解复用 - DoubleLi - 博客园






保存音频包：

直接输出解复用之后的的音频数据码流。只需要在每次调用av_read_frame()之后将得到的音频的AVPacket存为本地文件即可。




但在分离AAC码流的时候，直接存储AVPacket后的文件可能是不能播放的。

分离H.264码流的时候，直接存储AVPacket后的文件可能是不能播放的。




复用格式是FLV，MP4等，直接存储后的文件是不能播放的。




因为FLV，MP4这些属于“特殊容器”。经过仔细对比后发现，调用av_read_frame()后得到的AVPacket里面的内容是AAC纯数据，就是那种不包含ADTS文件头的AAC数据。（存储AAC数据的AVPacket的data字段中的数据是不包含7字节ADTS文件头的数据）因此如果想要得到可以播放的AAC文件，需要在每个AVPacket前面加上7字节ADTS文件头。




demux分离时，h264需要h264_mp4toannexb，音频aac好像没有用到

mux时，h264也需要h264_mp4toannexb，aac用到aac_adtstoasc

mux时对于某些封装格式（例如MP4/FLV/MKV等）中的AAC，需要用到名称为“aac_adtstoasc”的bitstream filter。

============================================================

保存视频包：




分离某些封装格式（例如MP4/FLV/MKV等）中的H.264的时候，需要首先写入SPS和PPS







例如flv格式时，从av_read_frame中读出的包也是不包含video tag header的裸视频数据。




FLV，MP4这些“特殊容器”，需要经过以下2步处理才能得到可播放的H.264码流：

1.第一次存储AVPacket之前需要在前面加上H.264的SPS和PPS。这些信息存储在AVCodecContext的extradata里面。

并且需要使用FFMPEG中的名为"h264_mp4toannexb"的bitstream filter 进行处理。

然后将处理后的extradata存入文件

具体代码如下：

代码片段A
- 
unsigned **char** *dummy=NULL;   //输入的指针  
- **int** dummy_len;  
- 
AVBitStreamFilterContext* bsfc =  av_bitstream_filter_init("h264_mp4toannexb");    
- 
av_bitstream_filter_filter(bsfc, pCodecCtx, NULL, &dummy, &dummy_len, NULL, 0, 0);  
- 
fwrite(pCodecCtx->extradata,pCodecCtx-->extradata_size,1,fp);  
- 
av_bitstream_filter_close(bsfc);    
- 
free(dummy);  

2.通过查看FFMPEG源代码发现，AVPacket中的数据起始处没有分隔符(0x00000001),
 也不是0x65、0x67、0x68、0x41等字节，所以可以肯定AVPacket不是标准的nalu。其实，AVPacket前4个字表示的是nalu的长度，从第5个字节开始才是nalu的数据。所以直接将AVPacket前4个字节替换为0x00000001即可得到标准的nalu数据。

具体代码如下：

代码片段B



- **char** nal_start[]={0,0,0,1};  
- 
fwrite(nal_start,4,1,fp);  
- 
fwrite(pkt->data+4,pkt->size-4,1,fp);  
- 
fclose(fp); 




或
- **char** nal_start[]={0,0,0,1};  
- 
memcpy(packet->data,nal_start,4); 




经过以上两步处理之后，我们就得到了可以正常播放的H.264码流

====================================================







其它处理方式：

（1）使用bitstream filter处理每个AVPacket（简单）

把每个AVPacket中的数据（data字段）经过bitstream filter“过滤”一遍。关键函数是av_bitstream_filter_filter()。示例代码如下。
- 
AVBitStreamFilterContext* h264bsfc =  av_bitstream_filter_init("h264_mp4toannexb");   
- **while**(av_read_frame(ifmt_ctx, &pkt)>=0){  
- **if**(pkt.stream_index==videoindex){  
- 
        av_bitstream_filter_filter(h264bsfc, ifmt_ctx->streams[videoindex]->codec, NULL, &pkt.data, &pkt.size, pkt.data, pkt.size, 0);  
- 
        fwrite(pkt.data,1,pkt.size,fp_video);  
- 
        //...  
- 
    }  
- 
    av_free_packet(&pkt);  
- 
}  
- 
av_bitstream_filter_close(h264bsfc);  

代码中，把av_bitstream_filter_filter()的输入数据和输出数据（分别对应第4,5,6,7个参数）都设置成AVPacket的data字段就可以了。




经过上述代码处理之后，AVPacket中的数据有如下变化：

*每个AVPacket的data添加了H.264的NALU的起始码{0,0,0,1}

*每个IDR帧数据前面添加了SPS和PPS

（2）手工添加SPS，PPS（稍微复杂）

将AVCodecContext的extradata数据经过bitstream filter处理之后得到SPS、PPS，拷贝至每个IDR帧之前。“代码片段A”示例了写入SPS、PPS的过程。










然后修改AVPacket的data。把前4个字节改为起始码。参见：代码片段B













参考：

[**视音频分离器（demuxer）**](http://blog.csdn.net/leixiaohua1020/article/details/39802819)**:ts->h264+aac 利用**av_interleaved_write_frame




[**视音频复用器（muxer）**](http://blog.csdn.net/leixiaohua1020/article/details/39802913)**：h264+aac->mkv，用到**av_interleaved_write_frame

**分离**[**多媒体文件中的音频码流**](http://blog.csdn.net/leixiaohua1020/article/details/11800791)




**分离**[**多媒体文件中的H.264码流**](http://blog.csdn.net/leixiaohua1020/article/details/11800877)




[**视音频分离器简化版（demuxer-simple）**](http://blog.csdn.net/leixiaohua1020/article/details/39767055)**:flv->h264+aac**









