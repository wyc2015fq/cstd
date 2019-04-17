# ffmpeg 基本数据结构和对象： AVPacket、AVPicture、AVFrame - DoubleLi - 博客园







一、AVPacket



**[cpp]**[view plain](http://blog.csdn.net/chance_yin/article/details/16817957)[copy](http://blog.csdn.net/chance_yin/article/details/16817957)

- /** 
-  * AVPacket 作为解码器的输入 或 编码器的输出。 
-  * 当作为解码器的输入时，它由demuxer生成，然后传递给解码器 
-  * 当作为编码器的输出时，由编码器生成，然后传递给muxer 
-  * 在视频中，AVPacket 只能包含不大于1帧的内容，而视频的1帧可能要包含在多个AVPacket中，AVPacket < AVFrame 
-  *  
-  * 
-  * AVPacket 是ffmpeg中少数的几个公共ABI，它只能被libavcodec和libformat在栈上分配 
-  * 
-  * The side data is always allocated with av_malloc() and is freed in 
-  * av_free_packet(). 
-  */  
- typedef struct AVPacket {  
- /** 
-      * packet的内存空间来自于一个叫做“引用计数缓冲区”的地方，这个指针就指向一块引用计数缓冲区 
-      */  
-     AVBufferRef *buf;  
- /** 
-      * 显示时间戳 单位是 AVStream->time_base units 
-      */  
-     int64_t pts;  
- /** 
-      * 解压时间戳，在这个时刻该包需要被解码 
-      */  
-     int64_t dts;  
-     uint8_t *data;  
- int   size;  
- int   stream_index;  
- /** 
-      * A combination of AV_PKT_FLAG values 
-      */  
- int   flags;  
- /** 
-      * 存放额外的包信息 
-      */  
- struct {  
-         uint8_t *data;  
- int      size;  
- enum AVPacketSideDataType type;  
-     } *side_data;  
- int side_data_elems;  
- /** 
-      * 这个包的时间长度 in AVStream->time_base units, 设置0 表示未知. 
-      * duration = next_pts - this_pts i. 
-      */  
- int   duration;  
-     int64_t pos;                            ///< 在数据流中的字节偏移量, -1 if unknown  
-     int64_t convergence_duration;  
- } AVPacket;  




二、AVPicture



**[cpp]**[view plain](http://blog.csdn.net/chance_yin/article/details/16817957)[copy](http://blog.csdn.net/chance_yin/article/details/16817957)

- typedef struct AVPicture {  
-     uint8_t *data[AV_NUM_DATA_POINTERS];    ///< pointers to the image data planes  
- int linesize[AV_NUM_DATA_POINTERS];     ///< number of bytes per line  
- }  




三、AVFrame



**[cpp]**[view plain](http://blog.csdn.net/chance_yin/article/details/16817957)[copy](http://blog.csdn.net/chance_yin/article/details/16817957)

- /** 
-  * AVFrame表示解码过后的一个数据帧 
-  * 
-  * AVFrame 通过使用 av_frame_alloc()来创建. 这个函数只是创建了AVFrame结构本身，在结构 
-  * 中定义的指向其他内存块的缓冲区指针要用其他方法来分配 
-  * 使用 av_frame_free()来释放AVFrame. 
-  * 
-  */  
- typedef struct AVFrame {  
- #define AV_NUM_DATA_POINTERS 8  
- /** 
-      * pointer to the picture/channel planes. 
-      */  
-     uint8_t *data[AV_NUM_DATA_POINTERS];  
- /** 
-      * For video, size in bytes of each picture line. 
-      * For audio, size in bytes of each plane. 
-      */  
- int linesize[AV_NUM_DATA_POINTERS];  
- /** 
-      * pointers to the data planes/channels. 
-      */  
-     uint8_t **extended_data;  
- /** 
-      * width and height of the video frame 
-      */  
- int width, height;  
- /** 
-      * number of audio samples (per channel) described by this frame 
-      */  
- int nb_samples;  
- /** 
-      * format of the frame, -1 if unknown or unset 
-      */  
- int format;  
- /** 
-      * 1 -> keyframe, 0-> not 
-      */  
- int key_frame;  
- /** 
-      * Picture type of the frame. 
-      */  
- enum AVPictureType pict_type;  
- /** 
-      * Sample aspect ratio for the video frame, 0/1 if unknown/unspecified. 
-      */  
-     AVRational sample_aspect_ratio;  
- /** 
-      * Presentation timestamp in time_base units (time when frame should be shown to user). 
-      */  
-     int64_t pts;  
- /** 
-      * PTS copied from the AVPacket that was decoded to produce this frame. 
-      */  
-     int64_t pkt_pts;  
- /** 
-      * DTS copied from the AVPacket that triggered returning this frame. (if frame threading isnt used) 
-      * This is also the Presentation time of this AVFrame calculated from 
-      * only AVPacket.dts values without pts values. 
-      */  
-     int64_t pkt_dts;  
- /** 
-      * picture number in bitstream order 
-      */  
- int coded_picture_number;  
- /** 
-      * picture number in display order 
-      */  
- int display_picture_number;  
- /** 
-      * quality (between 1 (good) and FF_LAMBDA_MAX (bad)) 
-      */  
- int quality;  
- /** 
-      * for some private data of the user 
-      */  
- void *opaque;  
- /** 
-      * error 
-      */  
-     uint64_t error[AV_NUM_DATA_POINTERS];  
- /** 
-      * When decoding, this signals how much the picture must be delayed. 
-      * extra_delay = repeat_pict / (2*fps) 
-      */  
- int repeat_pict;  
- /** 
-      * The content of the picture is interlaced. 
-      */  
- int interlaced_frame;  
- /** 
-      * If the content is interlaced, is top field displayed first. 
-      */  
- int top_field_first;  
- /** 
-      * Tell user application that palette has changed from previous frame. 
-      */  
- int palette_has_changed;  
- /** 
-      * Sample rate of the audio data. 
-      */  
- int sample_rate;  
- /** 
-      * Channel layout of the audio data. 
-      */  
-     uint64_t channel_layout;  
- /** 
-      * 指向这个帧要用到的AVBuffer中的数据缓冲区. 
-      * 
-      * 一般每个图像位面（就时data[0],data[1] data[2]）只有一个指向AVBuffer的缓冲区, so for video this array 
-      * always contains all the references. For planar audio with more than 
-      * AV_NUM_DATA_POINTERS channels, there may be more buffers than can fit in 
-      * this array. Then the extra AVBufferRef pointers are stored in the 
-      * extended_buf array. 
-      */  
-     AVBufferRef *buf[AV_NUM_DATA_POINTERS];  
- /** 
-      * For planar audio which requires more than AV_NUM_DATA_POINTERS 
-      * AVBufferRef pointers, this array will hold all the references which 
-      * cannot fit into AVFrame.buf. 
-      */  
-     AVBufferRef **extended_buf;  
- /** 
-      * Number of elements in extended_buf. 
-      */  
- int        nb_extended_buf;  
-     AVFrameSideData **side_data;  
- int            nb_side_data;  
- /** 
-  * 可能因为解码错误，数据帧Frame会成为无效的帧，下面的结构就是当数据帧无效时使用的 
-  */  
- #define AV_FRAME_FLAG_CORRUPT       (1 << 0)  
- /** 
-      * Frame flags, a combination of AV_FRAME_FLAG_* 
-      */  
- int flags;  
-     int64_t best_effort_timestamp;  
-     int64_t pkt_pos;  
-     int64_t pkt_duration;  
-     AVDictionary *metadata;  
- int decode_error_flags;  
- #define FF_DECODE_ERROR_INVALID_BITSTREAM   1  
- #define FF_DECODE_ERROR_MISSING_REFERENCE   2  
- int channels;  
- int pkt_size;  
- enum AVColorSpace colorspace;  
- enum AVColorRange color_range;  
- /** 
-      * Not to be accessed directly from outside libavutil 
-      */  
-     AVBufferRef *qp_table_buf;  
- } AVFrame;  




#### from：http://blog.csdn.net/chance_yin/article/details/16817957










