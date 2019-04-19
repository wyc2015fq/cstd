# ffmpeg api升级到3.3 api变化 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年05月17日 11:21:25[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：703
[**void**](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) av_free_packet([**AVPacket**](http://ffmpeg.org/doxygen/trunk/structAVPacket.html) * pkt)
Use 
[**void**](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)** av_packet_unref**([**AVPacket**](http://ffmpeg.org/doxygen/trunk/structAVPacket.html) * pkt)
Wipe the packet.
Unreference the buffer referenced by the packet and reset the remaining packet fields to their default values.
Parameters
|pkt|The packet to be unreferenced.|
[**int**](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) avcodec_encode_video2([**AVCodecContext**](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) * avctx,[**AVPacket**](http://ffmpeg.org/doxygen/trunk/structAVPacket.html) * avpkt,const [**AVFrame**](http://ffmpeg.org/doxygen/trunk/structAVFrame.html) * frame,[**int**](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) * got_packet_ptr )
use [**avcodec_send_frame()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga9395cb802a5febf1f00df31497779169)/avcodec_receive_packet()
 instead
其中：
int avcodec_send_frame ( AVCodecContext * avctx,const AVFrame * frame ) ：
Supply a raw video
or audio frame to the encoder.Use [**avcodec_receive_packet()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga5b8eff59cf259747cf0b31563e38ded6) to
 retrieve buffered output packets.
frame为NULL时,  If the encoder still has packets buffered, it will return them
For audio: If AV_CODEC_CAP_VARIABLE_FRAME_SIZE is set, then each frame can
have any number of samples. If it is not set, frame->nb_samples must be equal to avctx->frame_size for all frames except the last.
 The final frame may be smaller than avctx->frame_size.
int avcodec_receive_packet ( AVCodecContext * avctx,AVPacket * avpkt )
Read encoded data from the encoder.
avpkt：This will be set to a reference-counted packet
allocated by the encoder. Note that the function will always call av_frame_unref(frame) before doing anything else.
音频：
int 
avcodec_encode_audio2 ( AVCodecContext * avctx,AVPacket * avpkt,const AVFrame * frame,int * got_packet_ptr )
Encode a frame of audio.
Takes input samples from frame and writes the next output packet, if available, to avpkt. The output packet does not necessarily contain data for the most recent frame, as encoders can delay,
 split, and combine input frames internally as needed.
Parameters
avpkt output AVPacket. The user can supply an output buffer by setting avpkt->data and avpkt->size prior to calling the function, but if the size of the user-provided data is not large enough,
 encoding will fail. 
If avpkt->data and avpkt->size are set, avpkt->destruct must also be set. All other AVPacket fields will be reset by the encoder using av_init_packet().
If avpkt->data is NULL, the encoder will allocate it. The encoder will set avpkt->size to the size of the output packet.
If this function fails or produces no output, avpkt will be freed using av_packet_unref().
[in] frame AVFrame containing the raw audio data to be encoded. May be NULL when flushing an encoder that has the AV_CODEC_CAP_DELAY capability set.
If AV_CODEC_CAP_VARIABLE_FRAME_SIZE is set, then each frame can have any number of samples. If it is not set, frame->nb_samples must be equal to avctx->frame_size for all frames except the
 last. 
The final frame may be smaller than avctx->frame_size.
[out] got_packet_ptr This field is set to 1 by libavcodec if the output packet is non-empty, and to 0 if it is empty. If the function returns an error, the packet can be assumed to be invalid,
and the value of got_packet_ptr is undefined and should not be used.
Returns:0 on success, negative error code on failure
Deprecated:
use avcodec_send_frame()/avcodec_receive_packet() instead


int 
avcodec_decode_audio4 ( AVCodecContext * avctx,AVFrame * frame,int * got_frame_ptr,const AVPacket * avpkt )
Decode the audio frame of size avpkt->size from avpkt->data into frame.
Some decoders may support multiple frames in a single AVPacket. Such decoders would then just decode the first frame and the return value would be less than the packet size.
In this case, avcodec_decode_audio4 has to be called again with an AVPacket containing the remaining data in order to decode the second frame, etc...
Even if no frames are returned, the packet needs to be fed to the decoder with remaining data until it is completely consumed or an error occurs.
Some decoders (those marked with AV_CODEC_CAP_DELAY) have a delay between input and output. This means that for some packets they will not immediately produce decoded output and need to be
 flushed at the end of decoding to get all the decoded data. 
Flushing is done by calling this function with packets with avpkt->data set to NULL and avpkt->size set to 0 until it stops returning samples.
It is safe to flush even those decoders that are not marked with AV_CODEC_CAP_DELAY, then no samples will be returned.
Warning
The input buffer, avpkt->data must be
AV_INPUT_BUFFER_PADDING_SIZE larger than the actual read bytes because some optimized bitstream readers read 32 or 64 bits at once and could read over the end.
Parameters
[out] frame The AVFrame in which to store decoded audio samples. The decoder will allocate a buffer for the decoded frame by calling the AVCodecContext.get_buffer2()
 callback. When AVCodecContext.refcounted_frames is set to 1, the frame is reference counted and the returned reference belongs to the caller. The
caller must release the frame using av_frame_unref() when the frame is no longer needed. The caller may safely write to the frame if
av_frame_is_writable() returns 1. When AVCodecContext.refcounted_frames is set to 0, the returned reference belongs to the
decoder and is valid only until the next call to this function or until closing or flushing the decoder. The caller may not write to it.
[out] got_frame_ptr Zero if no frame could be decoded, otherwise it is non-zero. Note that this field being set to zero does not mean that an error has occurred. For decoders with AV_CODEC_CAP_DELAY
 set, no given decode call is guaranteed to produce a frame.
[in] avpkt The input AVPacket containing the input buffer. At least avpkt->data and avpkt->size should be set. Some decoders might also require additional fields to be set.
Returns：A negative error code is returned if an error occurred during decoding, otherwise the number of bytes consumed from the input AVPacket is returned.
Deprecated:
Use avcodec_send_packet() and avcodec_receive_frame().
视频解码
int 
avcodec_decode_video2 ( AVCodecContext * avctx,AVFrame * picture,int * got_picture_ptr,const AVPacket * avpkt )
Decode the video frame of size avpkt->size from avpkt->data into picture.
Some decoders may support multiple frames in a single AVPacket, such decoders would then just decode the first frame.
Warning
The input buffer must be
AV_INPUT_BUFFER_PADDING_SIZE larger than the actual read bytes because some optimized bitstream readers read 32 or 64 bits at once and could read over the end.
The end of the input buffer buf should be set to 0 to ensure that no overreading happens for damaged MPEG streams.
Note
Codecs which have the AV_CODEC_CAP_DELAY capability set have a delay between input and output, these need to be fed with avpkt->data=NULL, avpkt->size=0 at the end to return the remaining frames.
Parameters
[out] picture The AVFrame in which the decoded video frame will be stored. Use
av_frame_alloc() to get an AVFrame. The codec will allocate memory for the actual bitmap by calling the AVCodecContext.get_buffer2() callback. When AVCodecContext.refcounted_frames is set to 1, the frame is reference
 counted and the returned reference belongs to the caller. The caller must release the frame using av_frame_unref() when the frame is no longer needed. The caller may safely write to the frame if av_frame_is_writable() returns 1.
When AVCodecContext.refcounted_frames is set to 0, the returned reference belongs to the decoder and is valid only until the next call to this function or until closing or flushing the decoder.
 The caller may not write to it.
[in] avpkt The input AVPacket containing the input buffer. You can create such packet with av_init_packet() and by then setting data and size, some decoders might in addition need other fields
 like flags&AV_PKT_FLAG_KEY. All decoders are designed to use the least fields possible.
[in,out] got_picture_ptr Zero if no frame could be decompressed, otherwise, it is nonzero.
Returns
On error a negative value is returned, otherwise the number of bytes used or zero if no frame could be decompressed.
Deprecated:
Use avcodec_send_packet() and avcodec_receive_frame().

int avcodec_send_packet ( AVCodecContext * avctx,const AVPacket * avpkt )
Supply raw packet data as input to a decoder.
Internally, this call will copy relevant AVCodecContext fields, which can influence decoding per-packet, and apply them when the packet is actually decoded. (For example AVCodecContext.skip_frame,
 which might direct the decoder to drop the frame contained by the packet sent with this function.)
Warning
The input buffer, avpkt->data must be AV_INPUT_BUFFER_PADDING_SIZE larger than the actual read bytes because some optimized bitstream readers read 32 or 64 bits at once and could read over
 the end.
Do not mix this API with the legacy API (like
avcodec_decode_video2()) on the same AVCodecContext. It will return unexpected results now or in future libavcodec versions.
Parameters
[in] avpkt The input AVPacket. Usually, this will be a single video frame, or several complete audio frames. Ownership of the packet remains with the caller, and the decoder will not write
 to the packet. The decoder may create a reference to the packet data (or copy it if the packet is not reference-counted).
Unlike with older APIs, the packet is always fully consumed, and if it contains multiple frames (e.g. some audio codecs), will require you to call avcodec_receive_frame() multiple times afterwards before you can send
 a new packet. It can be NULL (or an AVPacket with data set to NULL and size set to 0); in this case, it is considered a flush packet, which signals the end of the stream.
Sending the first flush packet will return success. Subsequent ones are unnecessary and will return AVERROR_EOF. If the decoder still has frames buffered, it will return them after sending a flush packet.
Returns
0 on success,
int avcodec_receive_frame ( AVCodecContext * avctx,AVFrame * frame )
Return decoded output data from a decoder.
Parameters
frame This will be set to a reference-counted video or audio frame (depending on the decoder type) allocated by the decoder. Note that the function will always call av_frame_unref(frame) before
 doing anything else.
Returns
0: success,
以前的用法：
pCodecCtx = pFormatCtx->streams[videoindex]->codec;
pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
下面是变动之后的用法：
pCodecCtx = avcodec_alloc_context3(NULL);
avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoStream]->codecpar);
pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
[http://ffmpeg.org/doxygen/trunk/group__lavc__encdec.html](http://ffmpeg.org/doxygen/trunk/group__lavc__encdec.html)
The [**avcodec_send_packet()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga58bc4bf1e0ac59e27362597e467efff3)/avcodec_receive_frame()/
avcodec_send_frame()/ [**avcodec_receive_packet()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga5b8eff59cf259747cf0b31563e38ded6) functions
 provide an encode/decode API, which decouples input and output.
The API is very similar for encoding/decoding and audio/video, and works as follows:
- 
Set up and open the [**AVCodecContext**](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html)as usual.
- 
Send valid input:
- 
For decoding, call [**avcodec_send_packet()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga58bc4bf1e0ac59e27362597e467efff3) to give the decoder raw compressed
 data in an [**AVPacket**](http://ffmpeg.org/doxygen/trunk/structAVPacket.html).
- 
For encoding, call [**avcodec_send_frame()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga9395cb802a5febf1f00df31497779169) to give the encoder an [**AVFrame**](http://ffmpeg.org/doxygen/trunk/structAVFrame.html) containing
 uncompressed audio or video. In both cases, it is 
recommended that AVPackets and AVFrames are refcounted, or libavcodec might have to
copy 
the input data. (libavformat always returns refcounted AVPackets, and [**av_frame_get_buffer()**](http://ffmpeg.org/doxygen/trunk/group__lavu__frame.html#ga6b1acbfa82c79bf7fd78d868572f0ceb) allocates
 refcounted AVFrames.)
- 
Receive output in a 
loop. Periodically **周期性地 **call one of the avcodec_receive_*() functions and process their output:
- 
For decoding, call [**avcodec_receive_frame()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga11e6542c4e66d3028668788a1a74217c). On success, it will return an [**AVFrame**](http://ffmpeg.org/doxygen/trunk/structAVFrame.html) containing
 uncompressed audio or video data.
- 
For encoding, call [**avcodec_receive_packet()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga5b8eff59cf259747cf0b31563e38ded6). On success, it will return an [**AVPacket**](http://ffmpeg.org/doxygen/trunk/structAVPacket.html) with
 a compressed frame. 
Repeat this call until it returns [**AVERROR(EAGAIN)**](http://ffmpeg.org/doxygen/trunk/group__lavu__error.html#gae4bb6f165973d09584e0ec0f335f69ca)or
an error. The [**AVERROR(EAGAIN)**](http://ffmpeg.org/doxygen/trunk/group__lavu__error.html#gae4bb6f165973d09584e0ec0f335f69ca) return value means that
new input data is required to return new output. In this case, continue with sending input. For each input frame/packet, the codec will
typically 
return 1 output frame/packet, 
but it can also be 0 or more than 1.
At the 
beginning of decoding or encoding, the codec might accept multiple input frames/packets without returning a frame, until its internal buffers are filled. This situation is handled transparently
**易觉察地 **if you follow the steps outlined above.
In theory, sending input can result in EAGAIN - this should happen only if not all output was received. You can use this to structure alternative decode or encode loops other than the one
 suggested above. For example, you could try sending new input on each iteration, and try to
receive output if that returns EAGAIN.
End of stream situations. These require "flushing" (aka draining) the codec, as the codec might buffer multiple frames or packets internally for
 performance or out of necessity (consider B-frames). This is handled as follows:
- 
Instead of valid input, send NULL to the [**avcodec_send_packet()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga58bc4bf1e0ac59e27362597e467efff3) (decoding)
 or [**avcodec_send_frame()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga9395cb802a5febf1f00df31497779169) (encoding) functions. This will enter draining mode.
- 
Call [**avcodec_receive_frame()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga11e6542c4e66d3028668788a1a74217c) (decoding) or [**avcodec_receive_packet()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga5b8eff59cf259747cf0b31563e38ded6) (encoding)
 in a loop until AVERROR_EOF is returned. The functions will not return [**AVERROR(EAGAIN)**](http://ffmpeg.org/doxygen/trunk/group__lavu__error.html#gae4bb6f165973d09584e0ec0f335f69ca),
 unless you forgot to enter draining mode.
- 
Before decoding can be resumed again, the codec has to be reset
with [**avcodec_flush_buffers()**](http://ffmpeg.org/doxygen/trunk/group__lavc__misc.html#gaf60b0e076f822abcb2700eb601d352a6).
Using the API as outlined above is highly recommended. But it is also possible to call functions outside of this rigid**严格的**
 schema. For example, you can call [**avcodec_send_packet()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga58bc4bf1e0ac59e27362597e467efff3) repeatedly without
 calling [**avcodec_receive_frame()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga11e6542c4e66d3028668788a1a74217c). In this case, [**avcodec_send_packet()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga58bc4bf1e0ac59e27362597e467efff3) will
 succeed until the codec's internal buffer has been filled up (which is typically of size 1 per output frame, after initial input), and then reject input with [**AVERROR(EAGAIN)**](http://ffmpeg.org/doxygen/trunk/group__lavu__error.html#gae4bb6f165973d09584e0ec0f335f69ca).
 Once it starts rejecting input, you have no choice but to read at least some output.
Not all codecs will follow a rigid and predictable**可预言的** dataflow; the only
 guarantee is that an [**AVERROR(EAGAIN)**](http://ffmpeg.org/doxygen/trunk/group__lavu__error.html#gae4bb6f165973d09584e0ec0f335f69ca) return value on a send/receive call on
 one end implies that a receive/send call on the other end will succeed, or at least will not fail with [**AVERROR(EAGAIN)**](http://ffmpeg.org/doxygen/trunk/group__lavu__error.html#gae4bb6f165973d09584e0ec0f335f69ca).
 In general, no codec will permit unlimited buffering of input or output.
This API replaces the following legacy functions:
- [**avcodec_decode_video2()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga3ac51525b7ad8bca4ced9f3446e96532) and [**avcodec_decode_audio4()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#gaaa1fbe477c04455cdc7a994090100db4):
 Use [**avcodec_send_packet()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga58bc4bf1e0ac59e27362597e467efff3) to feed input to the decoder, then use [**avcodec_receive_frame()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga11e6542c4e66d3028668788a1a74217c) to
 receive decoded frames after each packet. Unlike with the old video decoding API, multiple frames might result from a packet. For audio, splitting the input packet into frames by partially decoding packets becomes transparent to the API user. You never need
 to feed an [**AVPacket**](http://ffmpeg.org/doxygen/trunk/structAVPacket.html) to the API twice (unless it is rejected with [**AVERROR(EAGAIN)**](http://ffmpeg.org/doxygen/trunk/group__lavu__error.html#gae4bb6f165973d09584e0ec0f335f69ca) -
 then no data was read from the packet). Additionally, sending a flush/draining packet is required only once.
- [**avcodec_encode_video2()**](http://ffmpeg.org/doxygen/trunk/group__lavc__encoding.html#ga2c08a4729f72f9bdac41b5533c4f2642)/avcodec_encode_audio2(): Use [**avcodec_send_frame()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga9395cb802a5febf1f00df31497779169) to
 feed input to the encoder, then use [**avcodec_receive_packet()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga5b8eff59cf259747cf0b31563e38ded6) to receive
 encoded packets. Providing user-allocated buffers for [**avcodec_receive_packet()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga5b8eff59cf259747cf0b31563e38ded6) is
 not possible.
- 
The new API does not handle subtitles yet.
Mixing new and old function calls on the same [**AVCodecContext**](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html) is
 not allowed, and will result in undefined behavior.
Some codecs might require using the new API; using the old API will return an error when calling it. All codecs support the new API.
A codec is not allowed to return [**AVERROR(EAGAIN)**](http://ffmpeg.org/doxygen/trunk/group__lavu__error.html#gae4bb6f165973d09584e0ec0f335f69ca) for
 both sending and receiving. This would be an invalid state, which could put the codec user into an endless loop. The API has no concept of time either: it cannot happen that trying to do [**avcodec_send_packet()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga58bc4bf1e0ac59e27362597e467efff3) results
 in [**AVERROR(EAGAIN)**](http://ffmpeg.org/doxygen/trunk/group__lavu__error.html#gae4bb6f165973d09584e0ec0f335f69ca), but a repeated call 1 second later accepts the packet (with
 no other receive/flush API calls involved). The API is a strict state machine, and the passage of time is not supposed to influence it. Some timing-dependent behavior might still be deemed acceptable in certain cases. But it must never result in both send/receive
 returning EAGAIN at the same time at any point. It must also absolutely be avoided that the current state is "unstable" and can "flip-flop" between the send/receive APIs allowing progress. For example, it's not allowed that the codec randomly decides that
 it actually wants to consume a packet now instead of returning a frame, after it just returned [**AVERROR(EAGAIN)**](http://ffmpeg.org/doxygen/trunk/group__lavu__error.html#gae4bb6f165973d09584e0ec0f335f69ca) on
 an [**avcodec_send_packet()**](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga58bc4bf1e0ac59e27362597e467efff3) call.

ret = avcodec_send_packet(dec_ctx, &packet);
if (ret < 0) {
av_log(NULL, AV_LOG_ERROR, "Error while sending a packet to the decoder\n");
break;
}
while (ret >= 0) {
ret = avcodec_receive_frame(dec_ctx, frame);
if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
break;
} else if (ret < 0) {
av_log(NULL, AV_LOG_ERROR, "Error while receiving a frame from the decoder\n");
goto end;
}
if (ret >= 0) {
//ok
}
staticvoid[encode](http://ffmpeg.org/doxygen/trunk/encode__video_8c.html#a56739f6d2e501c2388279fcde2078ac6)([AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html)
 *enc_ctx, [AVFrame](http://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](http://ffmpeg.org/doxygen/trunk/demuxing__decoding_8c.html#ad7d33d579a8d4241a5e643e39287a209),
[AVPacket](http://ffmpeg.org/doxygen/trunk/structAVPacket.html) *[pkt](http://ffmpeg.org/doxygen/trunk/demuxing__decoding_8c.html#a3d4c6562f0b27cf0cacbbea5c038c090),
 FILE *[outfile](http://ffmpeg.org/doxygen/trunk/audiogen_8c.html#ae581849c67336453bd5b81e6518019a9))
{
int ret;
/* send the frame to the encoder */
ret =
[avcodec_send_frame](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga9395cb802a5febf1f00df31497779169)(enc_ctx, frame);
if (ret < 0) {
fprintf(stderr,
"Error sending a frame for encoding\n");
exit(1);
}
while (ret >= 0) {
ret =
[avcodec_receive_packet](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga5b8eff59cf259747cf0b31563e38ded6)(enc_ctx, pkt);
if (ret ==
[AVERROR](http://ffmpeg.org/doxygen/trunk/group__lavu__error.html#gae4bb6f165973d09584e0ec0f335f69ca)(EAGAIN) || ret ==
[AVERROR_EOF](http://ffmpeg.org/doxygen/trunk/group__lavu__error.html#gab4faa0afdf35076914824200331defff))
return;
elseif (ret < 0) {
fprintf(stderr,
"Error during encoding\n");
exit(1);
}
printf("Write frame %3"PRId64"
 (size=%5d)\n", pkt->[pts](http://ffmpeg.org/doxygen/trunk/structAVPacket.html#a73bde0a37f3b1efc839f11295bfbf42a),
 pkt->[size](http://ffmpeg.org/doxygen/trunk/structAVPacket.html#a4d1ea19f63eb107111fd650ca514d1f4));
fwrite(pkt->[data](http://ffmpeg.org/doxygen/trunk/structAVPacket.html#aaf4fe58dfcc7c232c1f2268b539d8367),
 1, pkt->[size](http://ffmpeg.org/doxygen/trunk/structAVPacket.html#a4d1ea19f63eb107111fd650ca514d1f4), outfile);
[av_packet_unref](http://ffmpeg.org/doxygen/trunk/group__lavc__packet.html#ga63d5a489b419bd5d45cfd09091cbcbc2)(pkt);
}
}

staticvoid[decode](http://ffmpeg.org/doxygen/trunk/decode__audio_8c.html#a1704ae53d7f426e1b67341785b5b2b58)([AVCodecContext](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html)
 *[dec_ctx](http://ffmpeg.org/doxygen/trunk/filtering__audio_8c.html#ac2bf6798f37e95d3d2ff2d44e4ab3c12),
[AVPacket](http://ffmpeg.org/doxygen/trunk/structAVPacket.html) *[pkt](http://ffmpeg.org/doxygen/trunk/demuxing__decoding_8c.html#a3d4c6562f0b27cf0cacbbea5c038c090),
[AVFrame](http://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](http://ffmpeg.org/doxygen/trunk/demuxing__decoding_8c.html#ad7d33d579a8d4241a5e643e39287a209),
 FILE *[outfile](http://ffmpeg.org/doxygen/trunk/audiogen_8c.html#ae581849c67336453bd5b81e6518019a9))
{
int i,
[ch](http://ffmpeg.org/doxygen/trunk/libswresample_2audioconvert_8c.html#abf2b9e9c039c88dcafa27a3014acd1c0);
int ret, data_size;
/* send the packet with the compressed data to the decoder */
ret =
[avcodec_send_packet](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga58bc4bf1e0ac59e27362597e467efff3)(dec_ctx, pkt);
if (ret < 0) {
fprintf(stderr,
"Error submitting the packet to the decoder\n");
exit(1);
}
/* read all the output frames (in general there may be any number of them */
while (ret >= 0) {
ret =
[avcodec_receive_frame](http://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga11e6542c4e66d3028668788a1a74217c)(dec_ctx, frame);
if (ret ==
[AVERROR](http://ffmpeg.org/doxygen/trunk/group__lavu__error.html#gae4bb6f165973d09584e0ec0f335f69ca)(EAGAIN) || ret ==
[AVERROR_EOF](http://ffmpeg.org/doxygen/trunk/group__lavu__error.html#gab4faa0afdf35076914824200331defff))
return;
elseif (ret < 0) {
fprintf(stderr,
"Error during decoding\n");
exit(1);
}
data_size =
[av_get_bytes_per_sample](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#ga0c3c218e1dd570ad4917c69a35a6c77d)(dec_ctx->[sample_fmt](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html#a1bdba69ea111e2a9d03fdaa7a46a4c45));
if (data_size < 0) {
/* This should not occur, checking just for paranoia */
fprintf(stderr,
"Failed to calculate data size\n");
exit(1);
}
for (i = 0; i < frame->[nb_samples](http://ffmpeg.org/doxygen/trunk/structAVFrame.html#a02f45ab8191aea1660159f1e464237ea);
 i++)
for (ch = 0; ch < dec_ctx->[channels](http://ffmpeg.org/doxygen/trunk/structAVCodecContext.html#ac1e6c2cd1269caa7570575725c682a49);
 ch++)
fwrite(frame->[data](http://ffmpeg.org/doxygen/trunk/structAVFrame.html#a1d0f65014a8d1bf78cec8cbed2304992)[ch]
 + data_size*i, 1, data_size, outfile);
}
}

网友示例：
int ret = avcodec_send_packet(aCodecCtx, &pkt);
if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
return -1;
ret = avcodec_receive_frame(aCodecCtx, frame);
if (ret < 0 && ret != AVERROR_EOF)
return -1;
参考：
[http://blog.csdn.net/zhangwu1241/article/details/53183590?utm_source=itdadao&utm_medium=referral](http://blog.csdn.net/zhangwu1241/article/details/53183590?utm_source=itdadao&utm_medium=referral)
