# ffmpeg 和 x264的参数对照 - tifentan的专栏 - CSDN博客

2018年04月13日 17:24:15[露蛇](https://me.csdn.net/tifentan)阅读数：81


[http://www.cnblogs.com/soief/p/3471465.html](http://www.cnblogs.com/soief/p/3471465.html)拓展一下
|x264命令行|x264字段|ffmpeg命令行|ffmpeg字段|说明|
|----|----|----|----|----|
|qp qp_constant|cqp||cqp|固定量化因子。取值范围0到51。经常取值在20-40之间，越小质量越好,要求的码率越高。0表示无损压缩|
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
|qpstep qp-step|i_qp_step||max_qdiff|最大的在帧与帧之间进行切变的量化因子的变化量。|
|qcomp|f_qcompress||||
|vbv-maxrate|i_vbv_max_bitrate|b|rc_max_rate|允许的最大码流,x264里面以kbps为单位，ffmpeg以bps为单位|
|vbv-bufsize|i_vbv_buffer_size|bufsize|rc_buffer_size|在指定vbv-maxrate的时候必须设置该字段。|
|vbv-init|f_vbv_buffer_init||rc_initial_buffer_occupancy|初始的缓存占用量|
|qcomp|f_qcompress||qcompress|量化器压缩比率0-1.越小则比特率越区域固定，但是越高越使量化器参数越固定。|
|direct-pred direct|i_direct_mv_pred||directpred|B帧里面采用的运动侦测的方式。时间和空间方式大致PSNR和速度是一致的。设置为auto质量会好一些，但是速度会下降一些，设置为0，质量和速度都会下降.可以选择none, auto, temporal, spatial.|
|weightb weight-b|b_weighted_bipred||FLAGS2(CODEC_FLAG2_WPRED)|当B帧设置>1时使用|
|partitions analyse|inter|||X264_ANALYSE_I4x4 X264_ANALYSE_I8x8 X264_ANALYSE_PSUB16x16 X264_ANALYSE_PSUB8x8 X264_ANALYSE_BSUB16x16|
|8x8dct|b_transform_8x8||FLAGS(CODEC_FLAG2_8X8DCT)||
|me|i_me_method||me_method|运动侦测的方式 ME_EPZS ME_HEX ME_UMH ME_FULL ME_ESA|
|me-range|merange|i_me_range|me_range|运动侦测的半径|
|subq subme|i_subpel_refine||me_subpel_quality|这个参数控制在运动估算过程中质量和速度的权衡。Subq=5可以压缩>10%于subq=1。1-7|
|mixed-refs|b_mixed_references||FLAGS2(CODEC_FLAG2_MIXED_REFS)|允许8*8,16*8运动块独立地选择参考帧，如果disable,则所有的宏块必须参考同一帧。需要frameref > 1|
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
|–no-psnr|b_psnr||FLAGS(CODEC_FLAG_PSNR)|是否开启PSNR.|
|–no-ssim|b_ssim||没有对应值||
|–progress|b_progress||没有对应值||
|–bitrate|i_bitrate|b|bit_rate|编码输出的比特率,并启用ABR(Average Birtate 模式（i_rc_method),如果想crf请设0.|
|qblur|f_qblur||qblur||
||f_complexity_blur||complexityblur||
|chroma_me|||||
|trellis|||trellis|trellis：0、1、2。执行Trellis quantization来提高效率。0、停用；1、只在一个宏区块的最终编码上启用；2、在所有模式决策上启用。|

