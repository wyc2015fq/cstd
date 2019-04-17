# FFMPEG学习【libavcodec】：编码 - 一世豁然的专栏 - CSDN博客





2017年05月05日 21:49:56[一世豁然](https://me.csdn.net/Explorer_day)阅读数：826








一、数据结构




```cpp
struct  RcOverride{
int start_frame;
int end_frame;
int qscale;
float quality_factor;
}
```







二、宏

#define AV_INPUT_BUFFER_MIN_SIZE   16384


最小编码缓冲区大小：用于在头文件写入期间避免一些检查。








三、枚举

enum  Motion_Est_ID { 
ME_ZERO = 1, 
//没有搜索，那就是每当需要时使用0,0个矢量

ME_FULL, 

ME_LOG, 

ME_PHODS, 
ME_EPZS,
//增强预测区域搜索

ME_X1, 
//保留用于实验

ME_HEX, 
//六边形搜索

ME_UMH, 
//不均匀的六边形搜索
ME_TESA, 
//转换穷举搜索算法

ME_ITER =50 //迭代搜索

}


运动估计类型。





已弃用：使用编解码器私有选项代替







四、函数

1、AVCodec * avcodec_find_encoder (enum AVCodecID id)

注释：查找具有匹配编解码器ID的已注册编码器。

参数：id：请求的编码器的AVCodecID

返回：返回找到的编码器，否者返回NULL。




2、AVCodec * avcodec_find_encoder_by_name (const char *name)

注释：查找具有指定名称的注册编码器。

参数：name：所请求编码器的名称

返回：返回找到的编码器，否者返回NULL。




3、attribute_deprecated int avcodec_encode_audio2 (AVCodecContext *avctx, AVPacket *avpkt, const AVFrame *frame, int *got_packet_ptr)

注释：编码一帧音频。

从帧中输入样本，并将下一个输出数据包（如果可用）写入avpkt。 输出分组不一定包含最新帧的数据，因为编码器可以根据需要在内部延迟，分割和组合输入帧。


参数：avctx-编解码器上下文。

    avpkt-输出AVPacket。 用户可以在调用该函数之前设置avpkt-> data和avpkt-> size来提供输出缓冲区，但如果用户提供的数据大小不够大，则编码将失败。 如果设置了avpkt-> data和avpkt-> size，那么还必须设置avpkt-> destruct。 所有其他AVPacket字段将由编码器使用av_init_packet（）重置。 如果avpkt-> data为NULL，编码器将分配。
 编码器将将avpkt-> size设置为输出数据包的大小。

如果此函数失败或不产生输出，avpkt将使用av_packet_unref（）被释放。


    [in]frame-AVFrame包含要编码的原始音频数据。 刷新具有AV_CODEC_CAP_DELAY功能集的编码器时可能为空。 如果设置了AV_CODEC_CAP_VARIABLE_FRAME_SIZE，则每个帧可以有任意数量的样本。 如果未设置，对于除最后一个以外的所有帧，frame-> nb_samples必须等于avctx-> frame_size。 最后一帧可能小于avctx-> frame_size。

    [out]got_packet_ptr-如果输出数据包不为空，则该字段由libavcodec设置为1，如果为空，则将该字段设置为1。 如果函数返回错误，则可以假定该数据包无效，并且get_packet_ptr的值未定义，不应该被使用。

返回：返回0表示成功，负错误代码表示失败。

已废弃：请改用avcodec_send_frame（）/ avcodec_receive_packet（）




4、attribute_deprecated int avcodec_encode_video2 (AVCodecContext *avctx, AVPacket *avpkt, const AVFrame *frame, int *got_packet_ptr)

注释：编码一帧视频。

从帧中输入原始视频数据，并将下一个输出数据包（如果有的话）写入avpkt。 输出包不一定包含最近帧的数据，因为编码器可以根据需要在内部延迟和重新排序输入帧。


参数：avctx-编解码器上下文。

    avpkt-输出AVPacket。 用户可以在调用该函数之前设置avpkt-> data和avpkt-> size来提供输出缓冲区，但如果用户提供的数据大小不够大，则编码将失败。 所有其他AVPacket字段将由编码器使用av_init_packet（）重置。 如果avpkt-> data为NULL，编码器将分配。 编码器将将avpkt-> size设置为输出数据包的大小。 返回的数据（如果有）属于呼叫者，他负责释放它。

如果此函数失败或不产生输出，avpkt将使用av_packet_unref（）被释放。


    [in]frame-AVFrame包含要编码的原始视频数据。 刷新具有AV_CODEC_CAP_DELAY功能集的编码器时可能为空。

    [out]got_packet_ptr-如果输出数据包不为空，则该字段由libavcodec设置为1，如果为空，则将该字段设置为1。 如果函数返回错误，则可以假定该数据包无效，并且get_packet_ptr的值未定义，不应该被使用。

返回：返回0表示成功，负错误代码表示失败。

已废弃：请改用avcodec_send_frame（）/ avcodec_receive_packet（）




5、int avcodec_encode_subtitle（AVCodecContext * avctx，uint8_t * buf，int buf_size，const AVSubtitle * sub）






