# FFmpeg: AVCodecParameters 结构体分析 - 夜行过客 - 博客园







# [FFmpeg: AVCodecParameters 结构体分析](https://www.cnblogs.com/yongdaimi/p/10651658.html)





```
/**
 * This struct describes the properties of an encoded stream.
 *
 * sizeof(AVCodecParameters) is not a part of the public ABI, this struct must
 * be allocated with avcodec_parameters_alloc() and freed with
 * avcodec_parameters_free().
 */
typedef struct AVCodecParameters {
    /**
     * General type of the encoded data.
     */
    enum AVMediaType codec_type;
    /**
     * Specific type of the encoded data (the codec used).
     */
    enum AVCodecID   codec_id;
    /**
     * Additional information about the codec (corresponds to the AVI FOURCC).
     */
    uint32_t         codec_tag;

    /**
     * Extra binary data needed for initializing the decoder, codec-dependent.
     *
     * Must be allocated with av_malloc() and will be freed by
     * avcodec_parameters_free(). The allocated size of extradata must be at
     * least extradata_size + AV_INPUT_BUFFER_PADDING_SIZE, with the padding
     * bytes zeroed.
     */
    uint8_t *extradata;
    /**
     * Size of the extradata content in bytes.
     */
    int      extradata_size;

    /**
     * - video: the pixel format, the value corresponds to enum AVPixelFormat.
     * - audio: the sample format, the value corresponds to enum AVSampleFormat.
     */
    int format;

    /**
     * The average bitrate of the encoded data (in bits per second).
     */
    int64_t bit_rate;

    int bits_per_coded_sample;

    /**
     * Codec-specific bitstream restrictions that the stream conforms to.
     */
    int profile;
    int level;

    /**
     * Video only. The dimensions of the video frame in pixels.
     */
    int width;
    int height;

    /**
     * Video only. The aspect ratio (width / height) which a single pixel
     * should have when displayed.
     *
     * When the aspect ratio is unknown / undefined, the numerator should be
     * set to 0 (the denominator may have any value).
     */
    AVRational sample_aspect_ratio;

    /**
     * Video only. The order of the fields in interlaced video.
     */
    enum AVFieldOrder                  field_order;

    /**
     * Video only. Additional colorspace characteristics.
     */
    enum AVColorRange                  color_range;
    enum AVColorPrimaries              color_primaries;
    enum AVColorTransferCharacteristic color_trc;
    enum AVColorSpace                  color_space;
    enum AVChromaLocation              chroma_location;

    /**
     * Video only. Number of delayed frames.
     */
    int video_delay;

    /**
     * Audio only. The channel layout bitmask. May be 0 if the channel layout is
     * unknown or unspecified, otherwise the number of bits set must be equal to
     * the channels field.
     */
    uint64_t channel_layout;
    /**
     * Audio only. The number of audio channels.
     */
    int      channels;
    /**
     * Audio only. The number of audio samples per second.
     */
    int      sample_rate;
    /**
     * Audio only. The number of bytes per coded audio frame, required by some
     * formats.
     *
     * Corresponds to nBlockAlign in WAVEFORMATEX.
     */
    int      block_align;
    /**
     * Audio only. Audio frame size, if known. Required by some formats to be static.
     */
    int      frame_size;

    /**
     * Audio only. The amount of padding (in samples) inserted by the encoder at
     * the beginning of the audio. I.e. this number of leading decoded samples
     * must be discarded by the caller to get the original audio without leading
     * padding.
     */
    int initial_padding;
    /**
     * Audio only. The amount of padding (in samples) appended by the encoder to
     * the end of the audio. I.e. this number of decoded samples must be
     * discarded by the caller from the end of the stream to get the original
     * audio without any trailing padding.
     */
    int trailing_padding;
    /**
     * Audio only. Number of samples to skip after a discontinuity.
     */
    int seek_preroll;
} AVCodecParameters;
```

enum AVMediaType **codec_type**：编解码器的类型（视频，音频...）
enum AVCodecID   **codec_id**：标示特定的编解码器
int **format**：对于视频来说就是像素格式(参见AVPixelFormat,如0就代表YUV420p)，对于音频来说就是采样数据格式(参见AVSampleFormat)。
int **bit_rate**：平均比特率
uint8_t ***extradata**; int **extradata_size**：针对特定编码器包含的附加信息（例如对于H.264解码器来说，存储SPS，PPS等）
int **width**, **height**：如果是视频的话，代表视频帧的宽和高
int **refs**：运动估计参考帧的个数（H.264的话会有多帧，MPEG2这类的一般就没有了）
int **sample_rate**：采样率（音频）
uint64_t **channel_layout**：声道格式
int **channels**：声道数（音频）
int **profile**：型（H.264里面就有，其他编码标准应该也有）
int **level**：级（和profile差不太多）

使用方法，可以从 AVStream 上获取 AVCodecParameters  ，例：

```
for (int i = 0; i < ic->nb_streams; i++) {
        AVStream *as = ic->streams[i];
        if (as->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            LOGI("视频数据");
            videoStream = i;
            fps = r2d(as->avg_frame_rate);
            LOGI("fps = %d, width = %d, height = %d, codecid = %d, pixformat = %d, bit_rate = %lld", fps,
                    as->codecpar->width,
                    as->codecpar->height,
                    as->codecpar->codec_id,
                    as->codecpar->format,
                    as->codecpar->bit_rate
                    );
        }
        else {
            LOGI("音频数据");
            audioStream = i;
            LOGI("sample_rate = %d, channels = %d, sample_format = %d",
                    as->codecpar->sample_rate,
                    as->codecpar->channels,
                    as->codecpar->format
            );
        }
    }
```

打印：

2019-04-03 21:16:26.064 29619-29619/? I/xp.chen: 视频数据
2019-04-03 21:16:26.064 29619-29619/? I/xp.chen: fps = 25, width = 1920, height = 1080, codecid = 28, pixformat = 0, bit_rate = 3312568
2019-04-03 21:16:26.064 29619-29619/? I/xp.chen: 音频数据
2019-04-03 21:16:26.064 29619-29619/? I/xp.chen: sample_rate = 44100, channels = 2, sample_format = 8














