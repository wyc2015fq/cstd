# ffmpeg 源代码简单分析 ： avcodec_decode_video2() - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月13日 16:07:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：68
个人分类：[FFMPEG](https://blog.csdn.net/leixiaohua1020/article/category/1360795)












此前写了好几篇ffmpeg源代码分析文章，列表如下：

[图解FFMPEG打开媒体的函数avformat_open_input](http://blog.csdn.net/leixiaohua1020/article/details/8661601)
[ffmpeg 源代码简单分析 ： av_register_all()](http://blog.csdn.net/leixiaohua1020/article/details/12677129)
[ffmpeg 源代码简单分析 ： avcodec_register_all()](http://blog.csdn.net/leixiaohua1020/article/details/12677265)
[ffmpeg 源代码简单分析 ： av_read_frame()](http://blog.csdn.net/leixiaohua1020/article/details/12678577)
[ffmpeg 源代码简单分析 ： avcodec_decode_video2()](http://blog.csdn.net/leixiaohua1020/article/details/12679719)

============================



ffmpeg中的avcodec_decode_video2()的作用是解码一帧视频数据。输入一个压缩编码的结构体AVPacket，输出一个解码后的结构体AVFrame。



查看源代码之后发现，这个函数竟然十分的简单，源代码如下：



```cpp
int avcodec_decode_video2(AVCodecContext *avctx, AVFrame *picture,
                         int *got_picture_ptr,
                         const AVPacket *avpkt)
{
    int ret;
    // copy to ensure we do not change avpkt
    AVPacket tmp = *avpkt;

    *got_picture_ptr= 0;
    if((avctx->coded_width||avctx->coded_height) && av_image_check_size(avctx->coded_width, avctx->coded_height, 0, avctx))
        return -1;

    if((avctx->codec->capabilities & CODEC_CAP_DELAY) || avpkt->size || (avctx->active_thread_type&FF_THREAD_FRAME)){
        int did_split = av_packet_split_side_data(&tmp);
        apply_param_change(avctx, &tmp);
        avctx->pkt = &tmp;
        if (HAVE_THREADS && avctx->active_thread_type&FF_THREAD_FRAME)
             ret = ff_thread_decode_frame(avctx, picture, got_picture_ptr,
                                          &tmp);
        else {
            ret = avctx->codec->decode(avctx, picture, got_picture_ptr,
                              &tmp);
            picture->pkt_dts= avpkt->dts;

            if(!avctx->has_b_frames){
            picture->pkt_pos= avpkt->pos;
            }
            //FIXME these should be under if(!avctx->has_b_frames)
            if (!picture->sample_aspect_ratio.num)
                picture->sample_aspect_ratio = avctx->sample_aspect_ratio;
            if (!picture->width)
                picture->width = avctx->width;
            if (!picture->height)
                picture->height = avctx->height;
            if (picture->format == PIX_FMT_NONE)
                picture->format = avctx->pix_fmt;
        }

        emms_c(); //needed to avoid an emms_c() call before every return;

        avctx->pkt = NULL;
        if (did_split)
            ff_packet_free_side_data(&tmp);

        if (*got_picture_ptr){
            avctx->frame_number++;
            picture->best_effort_timestamp = guess_correct_pts(avctx,
                                                            picture->pkt_pts,
                                                            picture->pkt_dts);
        }
    }else
        ret= 0;

    return ret;
}
```



从代码中可以看出，通过ret = avctx->codec->decode(avctx, picture, got_picture_ptr,&tmp)这句代码，调用了相应解码器的decode()函数，完成了解码操作。








