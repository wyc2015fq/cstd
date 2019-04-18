# FFMPEG学习【libavcodec】：解码 - 一世豁然的专栏 - CSDN博客





2017年05月02日 14:42:08[一世豁然](https://me.csdn.net/Explorer_day)阅读数：719








一、模块

[帧解析](http://blog.csdn.net/explorer_day/article/details/71081151)








二、宏



```cpp
#define 	AV_INPUT_BUFFER_PADDING_SIZE   32
```

注释：在输入比特流结尾的要求附加分配字节的数量上进行解码。

这主要是因为一些优化的比特流读取器一次读取32位或64位，并且可以读取结束字节之外。


注意：如果附加字节的前23位不为0，则损坏的MPEG比特流可能会导致重读和segfault。








三、枚举



```cpp
enum  	AVDiscard { 
  AVDISCARD_NONE =-16,		//无丢弃
```

```cpp
AVDISCARD_DEFAULT = 0, 	//在avi中丢弃如0个大小的无用的数据包
```

```cpp
AVDISCARD_NONREF = 8, 	//丢弃所有非参考
```

```cpp
AVDISCARD_BIDIR = 16, 	//丢弃所有双向帧
  AVDISCARD_NONINTRA = 24,	//丢弃所有非内帧
```

```cpp
AVDISCARD_NONKEY = 32, 	//丢弃除关键帧之外的所有帧
```

```cpp
AVDISCARD_ALL = 48 		//丢弃所有
}
```







四、函数

1、AVCodec * avcodec_find_decoder（enum AVCodecID id）

注释：查找匹配编解码器ID的注册解码器。

参数：id-所请求解码器的AVCodecID

返回：返回找到的解码器，否则返回空。




2、AVCodec * avcodec_find_decoder_by_name (const char *name)

注释：查找指定名称的注册解码器。




3、int avcodec_default_get_buffer2 (AVCodecContext *s, AVFrame *frame, int flags)

注释：AVCodecContext.get_buffer2（）的默认回调。

参数：name-请求的解码器的名称

返回：返回找到的解码器，否则返回空。




4、attribute_deprecated unsigned avcodec_get_edge_width（void）

注释：在不具有CODEC_FLAG_EMU_EDGE标志的编解码器的情况下，返回get_buffer回调必须围绕图像边缘提供的填充量。【已废除】

此被设置成公开，因此可以通过自定义get_buffer2（）实现来对无AVCODEC_CAP_DR1设置的解码器进行调用。

返回：需要在像素中的填充

CODEC_FLAG_EMU_EDGE已被弃用，因此不再需要此函数





5、void avcodec_align_dimensions（AVCodecContext * s，int * width，int * height）

注释：修改宽度和高度值，以便如果不使用任何水平填充，它们将导致编解码器可接受的内存缓冲区。

只有在早已打开的具有AV_CODEC_CAP_DR1的编解码器才能使用。




6、void avcodec_align_dimensions2（AVCodecContext * s，int * width，int * height，int linesize_align [AV_NUM_DATA_POINTERS]）

注释：修改宽度和高度值，以便如果还要确保所有行大小都是相应的linesize_align [i]的倍数，它们将导致编解码器可接受的内存缓冲区。

只有在早已打开的具有AV_CODEC_CAP_DR1的编解码器才能使用。





7、int avcodec_enum_to_chroma_pos（int * xpos，int * ypos，enum AVChromaLocation pos）

注释：将AVChromaLocation转换为swscale x / y色度位置。

位置表示坐标系中的色度（0,0）位置，其中亮度（0,0）表示原点，亮度（1,1）表示256,256


参数：xpos-水平色度样本位置

            ypos-垂直色度样本位置




8、enum AVChromaLocation avcodec_chroma_pos_to_enum (int xpos, int ypos)

注释：将swscale x / y色度位置转换为AVChromaLocation。



位置表示坐标系中的色度（0,0）位置，其中亮度（0,0）表示原点，亮度（1,1）表示256,256


参数：xpos-水平色度样本位置

            ypos-垂直色度样本位置




9、attribute_deprecated int avcodec_decode_audio4（AVCodecContext * avctx，AVFrame * frame，int * got_frame_ptr，const AVPacket * avpkt）

注释：将来自avpkt->data大小为avpkt->size的音频帧解码到frame。【已废除】

一些解码器可以支持单个AVPacket中的多个帧。 这样的解码器将解码第一帧，并且返回值将小于分组大小。 在这种情况下，必须再次使用包含剩余数据的AVPacket来再次调用avcodec_decode_audio4，以便对第二帧进行解码等。即使没有帧被返回，数据包也需要用剩余的数据提供给解码器，直到它被完全消耗或发生错误。


一些解码器（标有AV_CODEC_CAP_DELAY的解码器）在输入和输出之间有延迟。 这意味着对于某些数据包，它们不会立即产生解码输出，并且需要在解码结束时刷新以获得所有解码的数据。 通过使用avpkt->数据设置为NULL并将avpkt-> size设置为0的数据包调用此函数来完成刷新，直到它停止返回样本。 即使是那些没有标记AV_CODEC_CAP_DELAY的解码器也是安全的，所以不会返回样品。


警告：输入缓冲区avpkt->data必须大于实际读取字节的AV_INPUT_BUFFER_PADDING_SIZE，因为一些优化的位流读取器同时读取32位或64位，并可以超界阅读。

注意：AVCodecContext必须在avcodec_open2（）之前打开，才能将数据包送入解码器。

参数：avctx-编解码器上下文

            [out]frame-用于存储解码音频样本的AVFrame。 解码器将通过调用AVCodecContext.get_buffer2（）回调为解码帧分配一个缓冲区。 当AVCodecContext.refcounted_frames设置为1时，帧被引用计数，返回的引用属于呼叫者。 当不再需要帧时，呼叫者必须使用av_frame_unref（）释放帧。 如果av_frame_is_writable（）返回1，则呼叫者可以安全地写入帧。当AVCodecContext.refcounted_frames设置为0时，返回的引用属于解码器，并且只有在下一次调用此函数或直到关闭或刷新解码器之前才有效。
 调用者可能不会写入。

            [out]got_frame_ptr：如果不能解码帧，则为零，否则为非零。 请注意，该字段设置为零并不意味着发生错误。 对于AV_CODEC_CAP_DELAY设置的解码器，没有给定的解码调用保证产生一个帧。

            [out]avpkt-输入AVPacket包含输入缓冲区。 应至少设置avpkt-> data和avpkt-> size。 某些解码器可能还需要设置其他字段。

返回：如果在解码期间发生错误，则返回负错误代码，否则返回从输入AVPacket消耗的字节数。

废弃：使用avcodec_send_packet（）和avcodec_receive_frame（）。




10、attribute_deprecated int avcodec_decode_video2（AVCodecContext * avctx，AVFrame * picture，int * got_picture_ptr，const AVPacket * avpkt）

注释：将来自avpkt->data大小为avpkt->size的音频帧解码到picture。【已废除】

一些解码器可以支持单个AVPacket中的多个帧，这样的解码器将仅解码第一帧。


警告：输入缓冲区avpkt->data必须大于实际读取字节的AV_INPUT_BUFFER_PADDING_SIZE，因为一些优化的位流读取器同时读取32位或64位，并可以超界阅读。输入缓冲区buf的结尾应设置为0，以确保损坏的MPEG流不会发生超线程。

注意：具有AV_CODEC_CAP_DELAY功能集的编解码器在输入和输出之间具有延迟，这些需要在末尾馈送avpkt-> data = NULL，avpkt-> size = 0以返回剩余的帧。AVCodecContext必须在avcodec_open2（）之前打开，才能将数据包送入解码器。

参数：avctx-编解码器上下文

    [out]picture-将存储解码的视频帧的AVFrame。使用av_frame_alloc（）获取AVFrame。编解码器将通过调用AVCodecContext.get_buffer2（）回调为实际位图分配内存。当AVCodecContext.refcounted_frames设置为1时，帧被引用计数，返回的引用属于调用者的参数。如果av_frame_is_writable（）返回1，则调用者可以安全地写入帧。当AVCodecContext.refcounted_frames设置为0时，返回的引用属于解码器，并且只有在下一次调用此函数或直到关闭或刷新解码器之前才有效。调用者可能不会写入。

            [in]avpkt-输入AVPacket包含输入缓冲区。 您可以使用av_init_packet（）创建此类数据包，然后设置数据和大小，某些解码器可能需要其他字段，如flags＆AV_PKT_FLAG_KEY。 所有解码器都设计为尽可能少地使用。

            [in，out]got_picture_ptr-如果不能解压缩帧，则为零，否则为非零。

返回：在错误时，返回一个负值，否则返回使用的字节数，如果没有帧可以被解压缩，则返回零。

废弃：使用avcodec_send_packet（）和avcodec_receive_frame（）。




11、int avcodec_decode_subtitle2（AVCodecContext * avctx，AVSubtitle * sub，int * got_sub_ptr，AVPacket * avpkt）

注释：解码字幕讯息。

返回错误的负值，否则返回使用的字节数。 如果没有字幕可以被解压缩，则get_sub_ptr为零。 否则，字幕存储在* sub中。 请注意，AV_CODEC_CAP_DR1不适用于字幕编解码器。 这是为了简单起见，因为性能差异是可以忽略的，并且由于潜在的非常不同的分配模式，重用为视频编解码器编写的get_buffer可能会执行不良。


一些解码器（标有AV_CODEC_CAP_DELAY的解码器）在输入和输出之间有延迟。 这意味着对于某些数据包，它们不会立即产生解码输出，并且需要在解码结束时刷新以获得所有解码的数据。 通过使用avpkt->data设置为NULL并将avpkt-> size设置为0的数据包调用此函数完成刷新，直到它停止返回字幕。 即使是没有标纪AV_CODEC_CAP_DELAY的解码器也是安全的，则不会返回任何字幕。

注意：AVCodecContext必须在avcodec_open2（）之前打开，才能将数据包送入解码器。

参数：avctx-编解码器上下文

    [out]sub-已解码的字幕存储在其中的预分配AVSubtitle，如果设置了* got_sub_ptr，则必须使用avsubtitle_free释放。

    [in，out]got_sub_ptr-如果没有字幕可以解压缩，则为零，否则为非零。

            [in]avpkt-输入AVPacket包含输入缓冲区。




12、int avcodec_send_packet（AVCodecContext * avctx，const AVPacket * avpkt）

注释：将原始分组数据作为输入提供给解码器。

在内部，此调用将复制相关的AVCodecContext字段，这可以影响每个数据包的解码，并在数据包实际解码时应用它们。 （例如AVCodecContext.skip_frame，它可能会指示解码器丢弃由此函数发送的数据包所包含的帧。）


警告：输入缓冲区avpkt->data必须大于实际读取字节的AV_INPUT_BUFFER_PADDING_SIZE，因为一些优化的位流读取器同时读取32位或64位，并可以越界读取。不要将该API与旧版API（如avcodec_decode_video2（））混合在同一AVCodecContext上。 它将会在现在或将来的libavcodec版本中返回意外的结果。

注意：AVCodecContext必须在avcodec_open2（）之前打开，才能将数据包送入解码器。

参数：avctx-编解码器上下文

     [in]avpkt-输入AVPacket。 通常，这将是一个单一的视频帧，或几个完整的音频帧。 分组的所有权仍然在呼叫者身上，解码器将不会写入数据包。 解码器可以创建对分组数据的引用（或者如果分组未被引用计数则复制它）。 与旧的API不同，数据包总是完全消耗，如果包含多个帧（例如一些音频编解码器），则需要多次调用avcodec_receive_frame（），然后再发送一个新的数据包。 它可以为NULL（或者AVPacket的数据设置为NULL，大小设置为0）;
 在这种情况下，它被认为是一个flush数据包，它表示流的结尾。 发送第一个flush数据包将返回成功。 后续的是不必要的，并将返回AVERROR_EOF。 如果解码器仍然有帧缓冲，它将在发送flush数据包后返回它们。

返回：0成功，否则为否定错误代码：AVERROR（EAGAIN）：在当前状态下不接受输入 - 用户必须使用avcodec_receive_frame（）读取输出（一旦读取所有输出，应重新发送数据包，并且调用不会EAGAIN失败。AVERROR_EOF：解码器已被刷新，并且不会向其发送新的数据包（如果发送多于1个数据包，则返回）。AVERROR（EINVAL）：编解码器未打开，它是编码器，或需要刷新。AVERROR（ENOMEM） ：无法向内部队列添加数据包，或类似的其他错误：合法的解码错误。




13、int avcodec_receive_frame（AVCodecContext * avctx，AVFrame * frame）

注释：从解码器中返回解码的输出数据。

参数：avctx-编解码器上下文。

    frame-这将被设置为由解码器分配的参考计数的视频或音频帧（取决于解码器类型）。 请注意，在执行其他任何操作之前，该函数将始终调用av_frame_unref（frame）。

返回：0：成功，一个帧被返回。AVERROR（EAGAIN）：输出在这种状态不可用 - 用户必须尝试发送新的输入。AVERROR_EOF：解码器已被完全刷新，并且不会有更多的输出帧。AVERROR（EINVAL）： 编解码器未打开，否则为编码器其他负值：合法解码错误。




14、int avcodec_send_frame（AVCodecContext * avctx，const AVFrame * frame）

注释：向编码器提供原始视频或音频帧。

使用avcodec_receive_packet（）来检索缓冲的输出数据包。


参数：avctx-编解码器上下文。

    [in]frame-AVFrame包含要编码的原始音频或视频帧。 帧的所有权保留在调用者身上，编码器不会写入帧。 编码器可以创建对帧数据的引用（或者如果帧不被引用计数，则其复制）。 它可以是NULL，在这种情况下，它被认为是一个flush数据包。 这表示流的结尾。 如果编码器仍然有缓冲的数据包，它将在此调用后返回。 一旦输入了刷新模式，将忽略其他刷新数据包，发送帧将返回AVERROR_EOF。

对于音频：如果设置了AV_CODEC_CAP_VARIABLE_FRAME_SIZE，则每个帧可以有任意数量的样本。 如果未设置，对于除最后一个以外的所有帧，frame-> nb_samples必须等于avctx-> frame_size。 最后一帧可能小于avctx-> frame_size。


返回：0成功，否则为错误代码。AVERROR（EAGAIN）：在当前状态下不输入输入 - 用户必须使用avcodec_receive_packet（）读取输出（一旦读取所有输出，应重新发送数据包，并且调用不会EAGAIN失败 ）。 AVERROR_EOF：编码器已被刷新，并且没有新的帧可以发送到它。AVERROR（EINVAL）：编解码器未打开，refcounted_frames未设置，它是解码器，或需要刷新。AVERROR（ENOMEM）：无法向内部队列添加数据包 ，或类似的其他错误：合法的解码错误。







15、int avcodec_receive_packet（AVCodecContext * avctx，AVPacket * avpkt）

注释：从编码器读取编码数据。

参数：avctx-编解码器上下文。

    avpkt-这将被设置为由编码器分配的引用计数的分组。 请注意，在执行其他任何操作之前，该函数将始终调用av_frame_unref（frame）。

返回：0成功，否则为错误代码。AVERROR（EAGAIN）：输出在当前状态下不可用 - 用户必须尝试发送输入。AVERROR_EOF：编码器已经完全刷新，并且不会有更多的输出数据包。AVERROR（EINVAL） ：编解码器未打开，否则为编码器其他错误：合法解码错误。






