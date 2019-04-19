# FFmpeg与libx264  x264接口对应关系源代码分析 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年08月21日 16:06:50[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：239
源代码位于“libavcodec/libx264.c”中。正是有了这部分代码，使得FFmpeg可以调用libx264编码H.264视频。

 从图中可以看出，libx264对应的AVCodec结构体ff_libx264_encoder中设定编码器初始化函数是X264_init()，编码一帧数据的函数是X264_frame()，编码器关闭函数是X264_close()。
X264_init()调用了如下函数：
[libx264 API] x264_param_default()：设置默认参数。
[libx264 API] x264_param_default_preset()：设置默认preset。
convert_pix_fmt()：将FFmpeg像素格式转换为libx264像素格式。
[libx264 API] x264_param_apply_profile()：设置Profile。
[libx264 API]
x264_encoder_open()：打开编码器。
[libx264 API] x264_encoder_headers()：需要全局头的时候，输出头信息。
X264_frame()调用了如下函数：
[libx264 API] x264_encoder_encode()：编码一帧数据。
[libx264 API] x264_encoder_delayed_frames()：输出编码器中缓存的数据。
encode_nals()：将编码后得到的x264_nal_t转换为AVPacket。
X264_close()调用了如下函数：
[libx264 API] x264_encoder_close()：关闭编码器。
- //libx264对应的AVCodec结构体
- AVCodec ff_libx264_encoder = {  
-     .name             = "libx264",  
-     .long_name        = NULL_IF_CONFIG_SMALL("libx264 H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10"),  
-     .type             = AVMEDIA_TYPE_VIDEO,  
-     .id               = AV_CODEC_ID_H264,  
-     .priv_data_size   = **sizeof**(X264Context),  
-  .init             = X264_init,  
-     .encode2          = X264_frame,  
-     .close            = X264_close,  
-     .capabilities     = CODEC_CAP_DELAY | CODEC_CAP_AUTO_THREADS,  
-     .priv_class       = &x264_class,  
-     .defaults         = x264_defaults,  
-     .init_static_data = X264_init_static,  
- };  
priv_class指向一个x264_class静态结构体，该结构体是libx264对应的AVClass，定义如下：
- **static****const** AVClass x264_class = {  
-     .class_name = "libx264",  
-     .item_name  = av_default_item_name,  
-     .option     = options,//选项
-     .version    = LIBAVUTIL_VERSION_INT,  
- };  
option指向一个options[]静态数组，其中包含了libx264支持的AVOption选项
- //它是FFmpeg针对libx264提供的可以通过AVOption设置的选项
如：preset， tune，profile，level，crf，vbr,
cbr, qp，
x264opts：x264 options
x264-params：Override the x264 configuration using a :-separated list of key=value parameters
**X264_init：**
主要将各种选项值传递给libx264。这些选项有两个来源：AVCodecContext和
 它里面的X264Context。
AVCodecContext中包含了编码器的一些通用选项，而X264Context包含了一些libx264特有的选项。
X264Context *x4 = avctx->priv_data;  
//把AVCodecContext的值（主要是编码时候的一些通用选项）映射到x264_param_t  ：
- **if** (avctx->gop_size >= 0)  
-         x4->params.i_keyint_max         = avctx->gop_size;  
//把AVCodecContext的值（主要是编码时候的一些通用选项）映射到x264_param_t
//把X264Context中的信息（主要是针对于libx264的一些选项）映射到x264_param_t
- **if**(x4->x264opts){  
- **const****char** *p= x4->x264opts;  
- **while**(p){  
- **char** param[256]={0}, val[256]={0};  
- **if**(sscanf(p, "%255[^:=]=%255[^:]", param, val) == 1){  
-                 OPT_STR(param, "1");  
-             }**else**
-                 OPT_STR(param, val);  
-             p= strchr(p, ':');  
-             p+=!!p;  
-         }  
-     }  
- 
- **if** (x4->x264_params) {  
-         AVDictionary *dict    = NULL;  
-         AVDictionaryEntry *en = NULL;  
- 
- **if** (!av_dict_parse_string(&dict, x4->x264_params, "=", ":", 0)) {  
- **while** ((en = av_dict_get(dict, "", en, AV_DICT_IGNORE_SUFFIX))) {  
- **if** (x264_param_parse(&x4->params, en->key, en->value) < 0)  
-                     av_log(avctx, AV_LOG_WARNING,  
- "Error parsing option '%s = %s'.\n",  
-                             en->key, en->value);  
-             }  
- 
-             av_dict_free(&dict);  
-         }  
-     }  
**X264_frame：**
 AVFrame --> x264_picture_t --> x264_nal_t --> AVPacket
调用encode_nals()将x264_nal_t转换成了AVPacket（可能有多个NALU，作用就是将多个x264_nal_t合并为一个AVPacket）

[http://blog.csdn.net/leixiaohua1020/article/details/45960409](http://blog.csdn.net/leixiaohua1020/article/details/45960409)
