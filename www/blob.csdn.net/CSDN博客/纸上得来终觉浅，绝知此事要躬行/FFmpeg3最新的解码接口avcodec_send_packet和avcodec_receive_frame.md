# FFmpeg3最新的解码接口avcodec_send_packet和avcodec_receive_frame - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年06月06日 17:39:21[boonya](https://me.csdn.net/boonya)阅读数：1527








原文地址：[http://www.bubuko.com/infodetail-2106896.html](http://www.bubuko.com/infodetail-2106896.html)


ffmpeg3版本的解码接口做了不少调整，之前的视频解码接口avcodec_decode_video2和avcodec_decode_audio4音频解码被设置为deprecated，对这两个接口做了合并，使用统一的接口。并且将音视频解码步骤分为了两步，第一步avcodec_send_packet，第二步avcodec_receive_frame，通过接口名字我们就可以知道第一步是发送编码数据包，第二步是接收解码后数据。新版本是否只是做了接口的变化，还有有哪些我们需要注意的事项，我们来分析一下。

首先我们先看一下这两个接口。

## avcodec_send_packet
- 
**接口源码**



```cpp
/**
 * Supply raw packet data as input to a decoder.
 *
 * Internally, this call will copy relevant AVCodecContext fields, which can
 * influence decoding per-packet, and apply them when the packet is actually
 * decoded. (For example AVCodecContext.skip_frame, which might direct the
 * decoder to drop the frame contained by the packet sent with this function.)
 *
 * @warning The input buffer, avpkt->data must be AV_INPUT_BUFFER_PADDING_SIZE
 *          larger than the actual read bytes because some optimized bitstream
 *          readers read 32 or 64 bits at once and could read over the end.
 *
 * @warning Do not mix this API with the legacy API (like avcodec_decode_video2())
 *          on the same AVCodecContext. It will return unexpected results now
 *          or in future libavcodec versions.
 *
 * @note The AVCodecContext MUST have been opened with @ref avcodec_open2()
 *       before packets may be fed to the decoder.
 *
 * @param avctx codec context
 * @param[in] avpkt The input AVPacket. Usually, this will be a single video
 *                  frame, or several complete audio frames.
 *                  Ownership of the packet remains with the caller, and the
 *                  decoder will not write to the packet. The decoder may create
 *                  a reference to the packet data (or copy it if the packet is
 *                  not reference-counted).
 *                  Unlike with older APIs, the packet is always fully consumed,
 *                  and if it contains multiple frames (e.g. some audio codecs),
 *                  will require you to call avcodec_receive_frame() multiple
 *                  times afterwards before you can send a new packet.
 *                  It can be NULL (or an AVPacket with data set to NULL and
 *                  size set to 0); in this case, it is considered a flush
 *                  packet, which signals the end of the stream. Sending the
 *                  first flush packet will return success. Subsequent ones are
 *                  unnecessary and will return AVERROR_EOF. If the decoder
 *                  still has frames buffered, it will return them after sending
 *                  a flush packet.
 *
 * @return 0 on success, otherwise negative error code:
 *      AVERROR(EAGAIN):   input is not accepted right now - the packet must be
 *                         resent after trying to read output
 *      AVERROR_EOF:       the decoder has been flushed, and no new packets can
 *                         be sent to it (also returned if more than 1 flush
 *                         packet is sent)
 *      AVERROR(EINVAL):   codec not opened, it is an encoder, or requires flush
 *      AVERROR(ENOMEM):   failed to add packet to internal queue, or similar
 *      other errors: legitimate decoding errors

 */

int avcodec_send_packet(AVCodecContext *avctx, const AVPacket *avpkt);
```
- 
**参数分析**



AVCodecContext *avctx：第一个参数与旧的接口一致，是视频解码的上下文，包含解码器。

const AVPacket *avpkt： 编码的音视频帧数据
- 
**为什么要传递空的avpkt**


这里有一个说明是可以传递NULL，什么情况下需要传递NULL，你平时看一些视频播放器，播放经常会少最后几帧，很多情况就是因为没有处理好缓冲帧的问题，ffmpeg内部会缓冲几帧，要想取出来就需要传递空的AVPacket进去。

## avcodec_receive_frame
- 
**接口源码**


```cpp
/**
 * Return decoded output data from a decoder.
 *
 * @param avctx codec context
 * @param frame This will be set to a reference-counted video or audio
 *              frame (depending on the decoder type) allocated by the
 *              decoder. Note that the function will always call
 *              av_frame_unref(frame) before doing anything else.
 *
 * @return
 *      0:                 success, a frame was returned
 *      AVERROR(EAGAIN):   output is not available right now - user must try
 *                         to send new input
 *      AVERROR_EOF:       the decoder has been fully flushed, and there will be
 *                         no more output frames
 *      AVERROR(EINVAL):   codec not opened, or it is an encoder
 *      other negative values: legitimate decoding errors

 */

int avcodec_receive_frame(AVCodecContext *avctx, AVFrame *frame);
```


- 
**参数分析**


AVCodecContext *avctx：第一个参数视频解码的上下文，与上面接口一致。

AVFrame *frame：解码后的视频帧数据。
- 
**空间申请和释放问题**


解码后图像空间由函数内部申请，你所做的只需要分配 AVFrame 对象空间，如果你每次调用avcodec_receive_frame传递同一个对象，接口内部会判断空间是否已经分配，如果没有分配会在函数内部分配。

avcodec_send_packet和avcodec_receive_frame调用关系并不一定是一对一的，比如一些音频数据一个AVPacket中包含了1秒钟的音频，调用一次avcodec_send_packet之后，可能需要调用25次 avcodec_receive_frame才能获取全部的解码音频数据，所以要做如下处理：

int re = avcodec_send_packet(codec, pkt);

if (re != 0)

{

    return;

}

while( avcodec_receive_frame(codec, frame) >= 0)

{

    //读取到一帧音频或者视频

    //处理解码后音视频 frame

}




更多的资料也可以关注我51CTO上的视频课程

[夏老师的课堂](http://edu.51cto.com/lecturer/index/user_id-12016059.html) http://edu.51cto.com/lecturer/index/user_id-12016059.html

[手把手教您开发视频播放器](http://edu.51cto.com/course/course_id-8059.html)http://edu.51cto.com/course/course_id-8059.html

转载这篇文章之前并没有使用while去接收数据，一直没成功过，终于知道原因了




