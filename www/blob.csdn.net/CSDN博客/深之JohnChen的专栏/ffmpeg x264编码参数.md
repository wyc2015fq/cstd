# ffmpeg x264编码参数 - 深之JohnChen的专栏 - CSDN博客

2018年06月12日 12:32:45[byxdaz](https://me.csdn.net/byxdaz)阅读数：977


**ffmpeg x264编码参数对照表**
|x264||ffmpeg||说明|
|----|----|----|----|----|
|命令行|字段|命令行|字段||
|qpqp_constant|cqp||cqp|固定量化因子。取值范围0到51。经常取值在20-40之间，越小质量越好,要求的码率越高。0表示无损压缩|
|max-keyint|i_keyint_max|g|gop_size|关键帧的最大间隔帧数|
|min-keyint|i_keyint_min||keyint_min|关键帧的最小间隔帧数|
|level|i_level_idc||level|取值范围10-51。设置比特流的Level。默认40，即4.0。用来告诉解码器需要支持的什么级别的兼容性。只有在你知道自己在做什么的时候才设置该参数。|
|frameref|i_frame_reference||refs|B和P帧向前预测参考的帧数。取值范围1-16。该值不影响解码的速度，但是越大解码所需的内存越大。这个值在一般情况下越大效果越好，但是超过6以后效果就不明显了。|
|bframes|i_bframe||max_b_frames|最大B帧数.|
|b-adapt|b_bframe_adaptive||b_frame_strategy|如果为true，则自动决定什么时候需要插入B帧，最高达到设置的最大B帧数。如果设置为false,那么最大的B帧数被使用。|
|b-pyramid|b_bframe_pyramid||FLAGS2(CODEC_FLAG2_BPYRAMID)|当设置B帧>=2时候，通过开启这个选项可以获得质量的略微提高，但是没有任何的速度损失。|
||b_deblocking_filter||FLAGS(CODEC_FLAG_LOOP_FILTER)||
|deblock|i_deblocking_filter_alphac0||deblockalpha||
|cabac|b_cabac||coder_type(FF_CODER_TYPE_AC)|使用CABAC熵编码技术,为引起轻微的编码和解码的速度损失，但是可以提高10%-15%的编码质量。|
|qmin|i_qp_min||qmin|最小的量化因子。取值范围1-51。建议在10-30之间。|
|qmax|i_qp_max||qmax|最大的量化因子。取值范围1-51。建议在10-30之间。|
|qpstepqp-step|i_qp_step||max_qdiff|最大的在帧与帧之间进行切变的量化因子的变化量。|
|qcomp|f_qcompress||||
|vbv-maxrate|i_vbv_max_bitrate|b|rc_max_rate|允许的最大码流,x264里面以kbps为单位，ffmpeg以bps为单位|
|vbv-bufsize|i_vbv_buffer_size|bufsize|rc_buffer_size|在指定vbv-maxrate的时候必须设置该字段。|
|vbv-init|f_vbv_buffer_init||rc_initial_buffer_occupancy|初始的缓存占用量|
|qcomp|f_qcompress||qcompress|量化器压缩比率0-1.越小则比特率越区域固定，但是越高越使量化器参数越固定。|
|direct-preddirect|i_direct_mv_pred||directpred|B帧里面采用的运动侦测的方式。时间和空间方式大致PSNR和速度是一致的。设置为auto质量会好一些，但是速度会下降一些，设置为0，质量和速度都会下降.可以选择none, auto, temporal, spatial.|
|weightbweight-b|b_weighted_bipred||FLAGS2(CODEC_FLAG2_WPRED)|当B帧设置>1时使用|
|partitionsanalyse|inter|||X264_ANALYSE_I4x4X264_ANALYSE_I8x8X264_ANALYSE_PSUB16x16X264_ANALYSE_PSUB8x8X264_ANALYSE_BSUB16x16|
|8x8dct|b_transform_8x8||FLAGS(CODEC_FLAG2_8X8DCT)||
|me|i_me_method||me_method|运动侦测的方式ME_EPZSME_HEXME_UMHME_FULLME_ESA|
|me-rangemerange|i_me_range||me_range|运动侦测的半径|
|subqsubme|i_subpel_refine||me_subpel_quality|这个参数控制在运动估算过程中质量和速度的权衡。Subq=5可以压缩>10%于subq=1。1-7|
|mixed-refs|b_mixed_references||FLAGS2(CODEC_FLAG2_MIXED_REFS)|允许8*8,16*8运动块独立地选择参考帧，如果disable,则所有的宏块必须参考同一帧。需要frameref >  1|
|brdo|b_bframe_rdo||FLAGS2(CODEC_FLAG2_BRDO)|需要subq>6|
|bime|b_bidir_me||bidir_refine|取值范围：true,false.这个值在没有B帧的时候失效。在双向预测宏块中双向运动矢量使用。|
|trellis|i_trellis||trellis||
|deadzone-intra|i_luma_deadzone||没有对应值||
|deadzone-inter|i_luma_deadzone||没有对应值||
|fast-pskip|b_fast_pskip||FLAGS(CODEC_FLAG2_FASTPSKIP)|在P帧内执行早期快速跳跃探测。这个经常在没有任何损失的前提下提高了速度。|
|dct-decimate|b_dct_decimate||没有对应值||
|nr|i_noise_reduction||noise_reduction|0意味着关闭，对于噪声很大的内容你需要打开。范围：0-100000|
|interlaced|b_interlaced||没有对应值||
|global-header|b_repeat_headers||FLAGS(CODEC_FLAG_GLOBAL_HEADER)|使得SPS和PPS只在流的开始处产生一次。有些播放器，如SONY的PSP需要开启此参数。默认的设置使得SPS和PPS在每一个IDR帧开始出都进行重复。|
|aud|b_aud||FLAGS2(CODEC_FLAG2_AUD)||
|threads|i_threads||thread_count|将帧切分成块，由不同的线程进行分别编码。0-4。0 for auto|
|rc-eq|psz_rc_eq||rc_eq||
|--no-psnr|b_psnr||FLAGS(CODEC_FLAG_PSNR)|是否开启PSNR.|
|--no-ssim|b_ssim||没有对应值||
|--progress|b_progress||没有对应值||
|--bitrate|i_bitrate|b|bit_rate|编码输出的比特率,并启用ABR(Average Birtate 模式（i_rc_method),|
|qblur|f_qblur||qblur||
||f_complexity_blur||complexityblur||

 参数详细解释

bit_rate = 200000, // 目标码率，采样码率越大，目标文件越大

bit_rate_tolerance= 8000000 // 码率误差，允许的误差越大，视频越小

gop_size = 300 //多少帧内出现1个I帧

qmin = 30 // 最小量化系数

qmax = 50 //最大量化系数

max_b_frames = 300 //两个非B帧之间允许出现的最多b帧数

i_quant_factor = 0.1 // i 帧相对p帧的量化系数比，值越小，说明p帧的量化系数越大，视频越小

b_quant_factor = 4.9 // b 帧相对p帧的量化系数比，值越大，b帧的量化系数越大，视频越小

 Ps.B帧量化系数=b_quant_factor* p帧量化系数+b_quant_offset

**--preset**的参数主要调节编码速度和质量的平衡，有ultrafast、superfast、veryfast、faster、fast、medium、slow、slower、veryslow、placebo这10个选项，从快到慢。

**--tune**的参数主要配合视频类型和视觉优化的参数，或特别的情况。如果视频的内容符合其中一个可用的调整值又或者有其中需要，则可以使用此选项，否则建议不使用（如tune grain是为高比特率的编码而设计的）。

tune的值有： film：电影、真人类型；

animation：动画；

grain：需要保留大量的grain时用；

stillimage：静态图像编码时使用；

psnr：为提高psnr做了优化的参数；

ssim：为提高ssim做了优化的参数；

fastdecode：可以快速解码的参数；

zerolatency：零延迟，用在需要非常低的延迟的情况下，比如电视电话会议的编码。

一般发送网络流都需要增加这两个参数，否则播放图像的延时很大。

如果用代码，可以这样设置：

/**

 * ultrafast,superfast, veryfast, faster, fast, medium

 * slow, slower, veryslow, placebo.

 */

AVDictionary *param = 0;  

av_dict_set(¶m, "preset", "fast", 0); // av_opt_set(pCodecCtx->priv_data,"preset","fast",0);

av_dict_set(¶m, "tune", "zerolatency", 0);  

画质,分别是baseline, extended, main, high

                  1、BaselineProfile：基本画质。支持I/P 帧，只支持无交错（Progressive）和CAVLC；

                  2、Extendedprofile：进阶画质。支持I/P/B/SP/SI 帧，只支持无交错（Progressive）和CAVLC；(用的少)

                  3、Main profile：主流画质。提供I/P/B帧，支持无交错（Progressive）和交错（Interlaced），也支持CAVLC 和CABAC 的支持；

                  4、High profile：高级画质。在mainProfile 的基础上增加了8x8内部预测、自定义量化、无损视频编码和更多的YUV 格式；

         H.264 Baseline profile、Extended profile和Main profile都是针对8位样本数据、4:2:0格式(YUV)的视频序列。在相同配置情况下，

                  Highprofile（HP）可以比Main profile（MP）降低10%的码率。根据应用领域的不同，Baseline profile多应用于实时通信领域，

                  Mainprofile多应用于流媒体领域，High profile则多应用于广电和存储领域。

//av_dict_set(¶m, "profile","main", 0);  

 如何设置固定码率编码 ?
 bit_rate是平均码率，不一定能控制住
 AVCodecContext* pCodecCtx;
 //...
 pCodecCtx->bit_rate = 400000;
 pCodecCtx->rc_max_rate = 400000;
 pCodecCtx->rc_min_rate = 400000;
 提示  [libx264 @ 00c70be0] VBV maxrate specified, but no bufsize, ignored
 再设置  pCodecCtx->rc_buffer_size = 200000;  即可。如此控制后编码质量明显差了。

