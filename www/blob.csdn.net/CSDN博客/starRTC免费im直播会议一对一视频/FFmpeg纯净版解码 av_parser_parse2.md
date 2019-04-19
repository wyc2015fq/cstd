# FFmpeg纯净版解码 av_parser_parse2 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年12月07日 18:36:17[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1962
主要是通过av_parser_parse2拿到AVPaket数据，跟av_read_frame类似。
输入必须是只包含视频编码数据“裸流”（例如H.264、HEVC码流文件），而不能是包含封装格式的媒体数据（例如AVI、MKV、MP4）。
av_parser_init()：初始化AVCodecParserContext。其参数是codec_id,所以同时只能解析一种
AVCodecParser用于解析输入的数据流并把它们分成一帧一帧的压缩编码数据。比较形象的说法就是把长长的一段连续的数据“切割”成一段段的数据。核心函数是av_parser_parse2()：
av_parser_parse2()：解析数据获得一个Packet， 从输入的数据流中分离出一帧一帧的压缩编码数据。
- /**
-  * Parse a packet.
-  *
-  * @param s             parser context.
-  * @param avctx         codec context.
-  * @param poutbuf       set to pointer to parsed buffer or NULL if not yet finished.
-  * @param poutbuf_size  set to size of parsed buffer or zero if not yet finished.
-  * @param buf           input buffer.
-  * @param buf_size      input length, to signal EOF, this should be 0 (so that the last frame can be output).
-  * @param pts           input presentation timestamp.
-  * @param dts           input decoding timestamp.
-  * @param pos           input byte position in stream.
-  * @return the number of bytes of the input bitstream used.
-  *
-  * Example:
-  * @code
-  *   while(in_len){
-  *       len = av_parser_parse2(myparser, AVCodecContext, &data, &size,
-  *                                        in_data, in_len,
-  *                                        pts, dts, pos);
-  *       in_data += len;
-  *       in_len  -= len;
-  *
-  *       if(size)
-  *          decode_frame(data, size);
-  *   }
-  * @endcode
-  */
- **int** av_parser_parse2(AVCodecParserContext *s,  
-                      AVCodecContext *avctx,  
-                      uint8_t **poutbuf, **int** *poutbuf_size,  
- **const** uint8_t *buf, **int** buf_size,  
-                      int64_t pts, int64_t dts,  
-                      int64_t pos);  
其中poutbuf指向解析后输出的压缩编码数据帧，buf指向输入的压缩编码数据。如果函数执行完后输出数据为空（poutbuf_size为0），则代表解析还没有完成，还需要再次调用av_parser_parse2()解析一部分数据才可以得到解析后的数据帧。当函数执行完后输出数据不为空的时候，代表解析完成，可以将poutbuf中的这帧数据取出来做后续处理。

avformat_open_input()会调用avformat_new_stream()创建AVStream
avformat_new_stream()中又会调用avcodec_alloc_context3()创建AVCodecContext
av_read_frame()：获取媒体的一帧压缩编码数据。其中调用了av_parser_parse2()。
“纯净”的解码器中，通过avcodec_decode_video2()成功解码第一帧之后，才能获取到宽高等信息

解析出来的数据，可通过下面的方法判断帧类型：
AVCodecParserContext->pict_type  ：AV_PICTURE_TYPE_I，AV_PICTURE_TYPE_P
参考：
[**解码器-纯净版（不包含libavformat）**](http://blog.csdn.net/leixiaohua1020/article/details/42181571)
