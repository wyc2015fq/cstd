# ffmpeg源码分析一 - 建建的博客 - CSDN博客
2017年03月11日 22:23:30[纪建](https://me.csdn.net/u013898698)阅读数：129
原帖地址：[http://blog.csdn.net/austinblog/article/details/24800381](http://blog.csdn.net/austinblog/article/details/24800381)
首先先看ffmpeg.c文件，有类似于如下的一些变量：
```
InputStream **input_streams = NULL;
int        nb_input_streams = 0;
InputFile   **input_files   = NULL;
int        nb_input_files   = 0;
OutputStream **output_streams = NULL;
int         nb_output_streams = 0;
OutputFile   **output_files   = NULL;
int         nb_output_files   = 0;
```
其中： input_streams是输入流的数组，nb_input_streams是输入流的个数。 input_files是输入文件（也可能是设备）的数组，nb_input_files是输入文件的个数。 下面的输出相关的变量们就不用解释了。
可以看出，文件和流是分别保存的。于是，可以想象，结构InputStream中应有其所属的文件在InputFile中的序号（file_index）。输入流数组应是这样填充的：每当在输入文件中找到一个流时，就把它添加到input_streams中，所以一个输入文件对应的输入流在input_streams中是紧靠着的，于是InputFile结构中应有其第一个流在input_streams中的开始序号（ist_index）和被放在input_streams中的流的总个数（nb_streams）。
InputStream和InputFile结构体如下：
```
typedef struct InputStream {
    int file_index;
    AVStream *st;
    int discard;             /* true if stream data should be discarded */
    int decoding_needed;     /* true if the packets must be decoded in 'raw_fifo' */
    AVCodec *dec;
    AVFrame *decoded_frame;
    AVFrame *filter_frame; /* a ref of decoded_frame, to be sent to filters */
    int64_t       start;     /* time when read started */
    /* predicted dts of the next packet read for this stream or (when there are
     * several frames in a packet) of the next frame in current packet (in AV_TIME_BASE units) */
    int64_t       next_dts;
    int64_t       dts;       ///< dts of the last packet read for this stream (in AV_TIME_BASE units)
    int64_t       next_pts;  ///< synthetic pts for the next decode frame (in AV_TIME_BASE units)
    int64_t       pts;       ///< current pts of the decoded frame  (in AV_TIME_BASE units)
    int           wrap_correction_done;
    int64_t filter_in_rescale_delta_last;
    double ts_scale;
    int saw_first_ts;
    int showed_multi_packet_warning;
    AVDictionary *opts;
    AVRational framerate;               /* framerate forced with -r */
    int top_field_first;
    int guess_layout_max;
    int resample_height;
    int resample_width;
    int resample_pix_fmt;
    int      resample_sample_fmt;
    int      resample_sample_rate;
    int      resample_channels;
    uint64_t resample_channel_layout;
    int fix_sub_duration;
    struct { /* previous decoded subtitle and related variables */
        int got_output;
        int ret;
        AVSubtitle subtitle;
    } prev_sub;
    struct sub2video {
        int64_t last_pts;
        int64_t end_pts;
        AVFrame *frame;
        int w, h;
    } sub2video;
    int dr1;
    /* decoded data from this stream goes into all those filters
     * currently video and audio only */
    InputFilter **filters;
    int        nb_filters;
    int reinit_filters;
    /* hwaccel options */
    enum HWAccelID hwaccel_id;
    char  *hwaccel_device;
    /* hwaccel context */
    enum HWAccelID active_hwaccel_id;
    void  *hwaccel_ctx;
    void (*hwaccel_uninit)(AVCodecContext *s);
    int  (*hwaccel_get_buffer)(AVCodecContext *s, AVFrame *frame, int flags);
    int  (*hwaccel_retrieve_data)(AVCodecContext *s, AVFrame *frame);
    enum AVPixelFormat hwaccel_pix_fmt;
    enum AVPixelFormat hwaccel_retrieved_pix_fmt;
} InputStream;
typedef struct InputFile {
    AVFormatContext *ctx;
    int eof_reached;      /* true if eof reached */
    int eagain;           /* true if last read attempt returned EAGAIN */
    int ist_index;        /* index of first stream in input_streams */
    int64_t input_ts_offset;
    int64_t ts_offset;
    int64_t last_ts;
    int64_t start_time;   /* user-specified start time in AV_TIME_BASE or AV_NOPTS_VALUE */
    int64_t recording_time;
    int nb_streams;       /* number of stream that ffmpeg is aware of; may be different
                             from ctx.nb_streams if new streams appear during av_read_frame() */
    int nb_streams_warn;  /* number of streams that the user was warned of */
    int rate_emu;
    int accurate_seek;
    int shared_encoding;
#if HAVE_PTHREADS
    pthread_t thread;           /* thread reading from this file */
    int non_blocking;           /* reading packets from the thread should not block */
    int finished;               /* the thread has exited */
    int joined;                 /* the thread has been joined */
    pthread_mutex_t fifo_lock;  /* lock for access to fifo */
    pthread_cond_t  fifo_cond;  /* the main thread will signal on this cond after reading from fifo */
    AVFifoBuffer *fifo;         /* demuxed packets are stored here; freed by the main thread */
#endif
} InputFile;
```
在输出流output_streams中，除了要保存其所在的输出文件在output_files中的序号（index），还应保存其对应的输入流在input_streams中的序号（ source_index），也应保存其在所属输出文件中的流序号（file_index）。而输出文件中呢，只需保存它的第一个流在output_streams中的序号（ost_index），但是为啥不保存输出文件中流的个数呢？我也不道，但我知道肯定不用保存也不影响实现功能（嘿嘿，相当于没说）．
OutputStream和OutputFile结构体如下：
```
typedef struct OutputStream {
    int file_index;          /* file index */
    int index;               /* stream index in the output file */
    int source_index;        /* InputStream index */
    AVStream *st;            /* stream in the output file */
    int encoding_needed;     /* true if encoding needed for this stream */
    int frame_number;
    int encoded_frame_number;
    /* input pts and corresponding output pts
       for A/V sync */
    struct InputStream *sync_ist; /* input stream to sync against */
    int64_t sync_opts;       /* output frame counter, could be changed to some true timestamp */ // FIXME look at frame_number
    /* pts of the first frame encoded for this stream, used for limiting
     * recording time */
    int64_t first_pts;
    /* dts of the last packet sent to the muxer */
    int64_t last_mux_dts;
    AVBitStreamFilterContext *bitstream_filters;
    AVCodec *enc;
    int64_t max_frames;
    AVFrame *filtered_frame;
    /* video only */
    AVRational frame_rate;
    int force_fps;
    int top_field_first;
    AVRational frame_aspect_ratio;
    /* forced key frames */
    int64_t *forced_kf_pts;
    int forced_kf_count;
    int forced_kf_index;
    char *forced_keyframes;
    AVExpr *forced_keyframes_pexpr;
    double forced_keyframes_expr_const_values[FKF_NB];
    /* audio only */
    int audio_channels_map[SWR_CH_MAX];  /* list of the channels id to pick from the source stream */
    int audio_channels_mapped;           /* number of channels in audio_channels_map */
    char *logfile_prefix;
    FILE *logfile;
    OutputFilter *filter;
    char *avfilter;
    char *filters;         ///< filtergraph associated to the -filter option
    char *filters_script;  ///< filtergraph script associated to the -filter_script option
    int64_t sws_flags;
    AVDictionary *opts;
    AVDictionary *swr_opts;
    AVDictionary *resample_opts;
    char *apad;
    OSTFinished finished;        /* no more packets should be written for this stream */
    int unavailable;                     /* true if the steram is unavailable (possibly temporarily) */
    int stream_copy;
    const char *attachment_filename;
    int copy_initial_nonkeyframes;
    int copy_prior_start;
    int keep_pix_fmt;
    AVCodecParserContext *parser;
} OutputStream;
typedef struct OutputFile {
    AVFormatContext *ctx;
    AVDictionary *opts;
    int ost_index;       /* index of the first stream in output_streams */
    int64_t recording_time;  ///< desired length of the resulting file in microseconds == AV_TIME_BASE units
    int64_t start_time;      ///< start time in microseconds == AV_TIME_BASE units
    uint64_t limit_filesize; /* filesize limit expressed in bytes */
    int shortest;
} OutputFile;
```
流和文件都准备好了，下面就是转换，那么转换过程是怎样的呢？还是我来猜一猜吧：
首先打开输入文件们，然后跟据输入流们准备并打开解码器们，然后跟据输出流们准备并打开编码器们，然后创建输出文件们，然后为所有输出文件们写好头部，然后就在循环中把输入流转换到输出流并写入输出文件中，转换完后跳出循环，然后写入文件尾，最后关闭所有的输出文件．
概述就先到这里吧，后面会对几个重要函数做详细分析。
