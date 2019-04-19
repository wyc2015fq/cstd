# opus 规范 与参数解析 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年11月07日 15:07:10[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：763

bytestream_put_buffer(&p, "OpusHead", 8);
bytestream_put_byte(&p, 1); /* Version */
bytestream_put_byte(&p, channels);
bytestream_put_le16(&p, avctx->initial_padding); /* Lookahead samples at 48kHz */
bytestream_put_le32(&p, avctx->sample_rate); /* Original sample rate */
Data Fields
1 int OpusHead::version 1个字节
The Ogg Opus format version, in the range 0...255.
The top 4 bits represent a "major" version, and the bottom four bits represent
backwards-compatible "minor" revisions.
The current specification describes version
1. This library will recognize versions up through 15 as backwards compatible with
the current specification. An earlier draft of the specification described a version 0, but
the only difference between version 1 and version 0 is that version 0 did not specify the
semantics for handling the version field.
2 int OpusHead::channel_count
The number of channels, in the range 1...255.
3 unsigned OpusHead::pre_skip =2字节
The number of samples that should be discarded**丢弃** from the beginning of the stream.
4 opus uint32 OpusHead::input_sample_rate
The sampling rate of the original input.
All Opus audio is coded at 48 kHz, and should also be decoded at 48 kHz for playback
(unless the target hardware does not support this sampling rate). However, this field
may be used to resample the audio back to the original sampling rate, for example,
when saving the output to a file.
5 int OpusHead::output_gain
The gain to apply to the decoded output, in dB, as a Q8 value in the range.
-32768...32767. The decoder will automatically scale the output by pow(10,output_-
gain/(20.0256)).
6 int OpusHead::mapping_family
The channel mapping family, in the range 0...255.
Channel mapping family 0 covers mono or stereo in a single stream. Channel mapping
family 1 covers 1 to 8 channels in one or more streams, using the Vorbis speaker
assignments. Channel mapping family 255 covers 1 to 255 channels in one or more
streams, but without any defined speaker assignment.
7 int OpusHead::stream_count
The number of Opus streams in each Ogg packet, in the range 1...255.
8 int OpusHead::coupled_count
The number of coupled Opus streams in each Ogg packet, in the range 0...127.
This must satisfy 0 <= coupled_count <= stream_count and coupled-
_count + stream_count <= 255. The coupled streams appear first, before
all uncoupled streams, in an Ogg Opus packet.
9 unsigned char OpusHead::mapping[OPUS_CHANNEL_COUNT_MAX]
The mapping from coded stream channels to output channels.
Let index=mapping[k] be the value for channel k. If index<2coupled_-
count, then it refers to the left channel from stream (index/2) if even, and the
right channel from stream (index/2) if odd. Otherwise, it refers to the output of the
uncoupled stream (index-coupled_count).

opus_head_parse
ffmpeg的extradata
#define OPUS_HEAD_SIZE 19
Total:      19 ( 0x00000013 )
4F, 70, 75, 73, 48, 65, 61, 64, 
01=version, 01=channels, （68, 00）, 80, 3E, 00, 00,
（00, 00）, 00,    //OpusHead..h..>.....
bytestream_put_le16
解析通道数和采样率
int ch = audioCtx->extradata[9];
 uint32_t hz=*(uint32_t*)&(audioCtx->extradata[12]);
aac 参数包大小4+11+2+2=19
opus=4+11+2+19=36
解析：
avc->channels = avc->extradata_size >= 10 ? avc->extradata[9] : (avc->channels == 1) ? 1 : 2;
参考：
opusfile_api-0.2(opus 规范).pdf
[https://www.opus-codec.org/docs/opusfile_api-0.2/structOpusHead.html](https://www.opus-codec.org/docs/opusfile_api-0.2/structOpusHead.html)
[https://www.opus-codec.org/docs/opusfile_api-0.2/group__header__info.html](https://www.opus-codec.org/docs/opusfile_api-0.2/group__header__info.html)
