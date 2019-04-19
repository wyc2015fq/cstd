# x264_param_default - maopig的专栏 - CSDN博客
2011年08月19日 17:29:21[maopig](https://me.csdn.net/maopig)阅读数：2617标签：[filter																[buffer																[reference																[thread																[tree																[hex](https://so.csdn.net/so/search/s.do?q=hex&t=blog)](https://so.csdn.net/so/search/s.do?q=tree&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=reference&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=filter&t=blog)
个人分类：[x264 h264																[ffmpeg](https://blog.csdn.net/maopig/article/category/847674)](https://blog.csdn.net/maopig/article/category/847676)
                
void    x264_param_default( x264_param_t *param )
{
 /* 开辟内存空间*/
    memset( param, 0, sizeof( x264_param_t ) );      
 /* CPU自动检测 */
    param->cpu = x264_cpu_detect();
    param->i_threads = X264_THREADS_AUTO; 
/* 并行编码线程为0 */
    param->b_deterministic = 1; /*允许非确定性时线程优化*/
    param->i_sync_lookahead = X264_SYNC_LOOKAHEAD_AUTO;/* 自动选择线程超前缓冲大小-1 */
/* 视频属性 */
    param->i_csp           = X264_CSP_I420; 
/*设置输入的视频采样的格式0x0001yuv 4:2:0 planar*/
    param->i_width         = 0; /* 宽度*/
    param->i_height        = 0; 
/* 高度*/
    param->vui.i_sar_width = 0;
    param->vui.i_sar_height= 0; 
/* 设置长宽比 */
    param->vui.i_overscan  = 0;  
/* 过扫描线，默认undef(不设置)，可选：show(观看)crop(去除) */
    param->vui.i_vidformat = 5;  /* undef视频格式 */
    param->vui.b_fullrange = 0;  /* off */
    param->vui.i_colorprim = 2;  
/* undef原始色度格式*/
    param->vui.i_transfer  = 2;  /* undef 转换方式*/
    param->vui.i_colmatrix = 2;  
/* undef 色度矩阵设置*/
    param->vui.i_chroma_loc= 0;  
/* left center 色度样本指定，范围0~5，默认0*/
    param->i_fps_num       = 25; /*帧率*/
    param->i_fps_den       = 1; 
/*用两个整型的数的比值，来表示帧率*/
    param->i_level_idc     = -1; /* level值的设置*/
    param->i_slice_max_size = 0; 
/* 每片字节的最大数，包括预计的NAL开销. */
    param->i_slice_max_mbs = 0; 
/* 每片宏块的最大数，重写 i_slice_count */
    param->i_slice_count = 0; 
/* 每帧的像条数目: 设置矩形像条. */
 /* 编码参数 */
    param->i_frame_reference = 3; 
/*参考帧的最大帧数。*/
    param->i_keyint_max = 250; 
/* 在此间隔设置IDR关键帧 */
    param->i_keyint_min = 25; 
/* 场景切换少于次值编码位I, 而不是 IDR. */
    param->i_bframe = 3; /*两个参考帧之间的B帧数目*/
    param->i_scenecut_threshold = 40; 
/*如何积极地插入额外的I帧 */
    param->i_bframe_adaptive = X264_B_ADAPT_FAST; /*自适应B帧判定1*/
    param->i_bframe_bias = 0; /*控制插入B帧判定，范围-100~+100，越高越容易插入B帧*/
    param->b_bframe_pyramid = 0; 
/*允许部分B为参考帧 */
    param->b_deblocking_filter = 1; 
/*去块效应相关*/
    param->i_deblocking_filter_alphac0 = 0; /* [-6, 6] -6 亮度滤波器, 6 强 */
    param->i_deblocking_filter_beta = 0; /* [-6, 6]  同上 */
    param->b_cabac = 1; /*cabac的开关*/
    param->i_cabac_init_idc = 0;
/*码率控制*/
    param->rc.i_rc_method = X264_RC_CRF;;/*恒定码率*/
    param->rc.i_bitrate = 0;/*设置平均码率大小*/
    param->rc.f_rate_tolerance = 1.0;
    param->rc.i_vbv_max_bitrate = 0; 
/*平均码率模式下，最大瞬时码率，默认0(与-B设置相同) */
    param->rc.i_vbv_buffer_size = 0; 
/*码率控制缓冲区的大小，单位kbit，默认0 */
    param->rc.f_vbv_buffer_init = 0.9; /* <=1: fraction of buffer_size. >1: kbit码率控制缓冲区数据保留的最大数据量与缓冲区大小之比，范围0~1.0，默认0.9*/
    param->rc.i_qp_constant = 23;;/*最小qp值*/
    param->rc.f_rf_constant = 23;
    param->rc.i_qp_min = 10; /*允许的最小量化值 */
    param->rc.i_qp_max = 51; /*允许的最大量化值*/
    param->rc.i_qp_step = 4; 
/*帧间最大量化步长 */
    param->rc.f_ip_factor = 1.4;
    param->rc.f_pb_factor = 1.3;
    param->rc.i_aq_mode = X264_AQ_VARIANCE; /* psy adaptive QP. (X264_AQ_*) *//通过减少色度量化偏移量的方法来提高亮度的总体质量。adaptive QP.
    param->rc.f_aq_strength = 1.0;
    param->rc.i_lookahead = 40;
    param->rc.b_stat_write = 0; 
/* Enable stat writing in psz_stat_out */
    param->rc.psz_stat_out = "x264_2pass.log";
    param->rc.b_stat_read = 0;
    param->rc.psz_stat_in = "x264_2pass.log";
param->rc.f_qcompress = 0.6; /* 0.0 => cbr, 1.0 => constant qp */  
 param->rc.f_qblur = 0.5;   /*时间上模糊量化 */
    param->rc.f_complexity_blur = 20; 
/* 时间上模糊复杂性 */
    param->rc.i_zones = 0; 
/* number of zone_t's */
    param->rc.b_mb_tree = 1; /* Macroblock-tree ratecontrol. */
 /* 日志 */
    param->pf_log = x264_log_default;
    param->p_log_private = NULL;
    param->i_log_level = X264_LOG_INFO;/*默认为“Info”*/
    /*分析 */
    param->analyse.intra = X264_ANALYSE_I4x4 | X264_ANALYSE_I8x8;
    param->analyse.inter = X264_ANALYSE_I4x4 | X264_ANALYSE_I8x8
                         | X264_ANALYSE_PSUB16x16 | X264_ANALYSE_BSUB16x16;
    param->analyse.i_direct_mv_pred = X264_DIRECT_PRED_SPATIAL;/*空间预测模式*/
    param->analyse.i_me_method = X264_ME_HEX;/*运动估计算法HEX*/
    param->analyse.f_psy_rd = 1.0;
    param->analyse.b_psy = 1;
    param->analyse.f_psy_trellis = 0;
    param->analyse.i_me_range = 16;/*运动估计范围*/
    param->analyse.i_subpel_refine = 7; /* 亚像素运动估计质量 */
    param->analyse.b_mixed_references = 1; 
/*允许每个宏块的分区在P帧有它自己的参考号*/
    param->analyse.b_chroma_me = 1; 
/* 亚像素色度运动估计和P帧的模式选择 */
    param->analyse.i_mv_range_thread = -1; /* 线程之间的最小空间. -1 = auto, based on number of threads. */
    param->analyse.i_mv_range = -1; /*运动矢量最大长度set from level_idc*/
    param->analyse.i_chroma_qp_offset = 0; /*色度量化步长偏移量 */
    param->analyse.b_fast_pskip = 1; 
/*快速P帧跳过检测*/
    param->analyse.b_weighted_bipred = 1; /*为b帧隐式加权 */
    param->analyse.b_dct_decimate = 1; /* 在P-frames转换参数域 */
    param->analyse.b_transform_8x8 = 1; /* 帧间分区*/
    param->analyse.i_trellis = 1; 
/* Trellis量化，对每个8x8的块寻找合适的量化值，需要CABAC，默认0 0：关闭1：只在最后编码时使用2：一直使用*/
    param->analyse.i_luma_deadzone[0] = 21; /*帧间亮度量化中使用的无效区大小*/
    param->analyse.i_luma_deadzone[1] = 11; 
/*帧内亮度量化中使用的无效区大小*/
    param->analyse.b_psnr = 0;/*是否显示PSNR*/
    param->analyse.b_ssim = 0;/*是否显示SSIM*/
/*量化*/
    param->i_cqm_preset = X264_CQM_FLAT; 
/*自定义量化矩阵(CQM),初始化量化模式为flat 0*/
    memset( param->cqm_4iy, 16, 16 );
    memset( param->cqm_4ic, 16, 16 );
    memset( param->cqm_4py, 16, 16 );
    memset( param->cqm_4pc, 16, 16 );
    memset( param->cqm_8iy, 16, 64 );
    memset( param->cqm_8py, 16, 64 
);/*开辟空间*/
   /*muxing*/
    param->b_repeat_headers = 1; 
/* 在每个关键帧前放置SPS/PPS*/
param->b_aud = 0; /*生成访问单元分隔符*/
}
