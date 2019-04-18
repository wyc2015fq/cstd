# x264_param_default_preset（）源码分析 - 简书


# x264_param_default_preset（）源码分析



### 相关

[x264编码示例](https://www.jianshu.com/p/7e2e02500a3a)

### 源码分析

h264编码原理复杂，参数众多。为了方便使用无论x264还是其他编码的实现框架，都封装了几种现有的编码模型，只需要根据编码速度的要求和视频质量的要求选择模型，并修改部分视频参数即可编码。

模型的选择就是通过x264_param_default_preset（）实现。
`int x264_param_default_preset( x264_param_t *param, const char *preset, const char *tune ) { x264_param_default( param ); if( preset && x264_param_apply_preset( param, preset ) < 0 ) return -1; if( tune && x264_param_apply_tune( param, tune )< 0 ) return -1; return 0; }`
代码可见设置该函数分为三步处理。
- 
第一步为调用x264_param_default（）为param设置默认值。
`void x264_param_default( x264_param_t \*param ) { memset( param, 0, sizeof( x264_param_t ) ); // CPU自动检测 param->cpu = x264_cpu_detect(); param->i_threads = X264_THREADS_AUTO; // 并行编码线程为0 param->b_deterministic = 1; //允许非确定性时线程优化 param->i_sync_lookahead = X264_SYNC_LOOKAHEAD_AUTO; //自动选择线程超前缓冲大小-1 // 视频属性 param->i_csp = X264_CSP_I420; //设置输入的视频采样的格式0x0001yuv 4:2:0 planar param->i_width = 0; // 宽度 param->i_height = 0; // 高度 param->vui.i_sar_width = 0; param->vui.i_sar_height= 0; //设置长宽比 param->vui.i_overscan = 0; // 过扫描线，默认undef(不设置)，可选：show(观看)crop(去除) param->vui.i_vidformat = 5; //undef视频格式 param->vui.b_fullrange = 0; //off param->vui.i_colorprim = 2; //undef原始色度格式 param->vui.i_transfer = 2; // undef 转换方式 param->vui.i_colmatrix = 2; // undef 色度矩阵设置 param->vui.i_chroma_loc= 0; // left center 色度样本指定，范围0~5，默认0 param->i_fps_num = 25; //帧率 param->i_fps_den = 1; //用两个整型的数的比值，来表示帧率 param->i_level_idc = -1; // level值的设置 param->i_slice_max_size = 0; // 每片字节的最大数，包括预计的NAL开销. param->i_slice_max_mbs = 0; // 每片宏块的最大数，重写 i_slice_count param->i_slice_count = 0; //每帧的像条数目: 设置矩形像条. //编码参数 param->i_frame_reference = 3; //参考帧的最大帧数。 param->i_keyint_max = 250; // 在此间隔设置IDR关键帧 param->i_keyint_min = 25; // 场景切换少于次值编码位I, 而不是 IDR. param->i_bframe = 3; //两个参考帧之间的B帧数目 param->i_scenecut_threshold = 40; //如何积极地插入额外的I帧 param->i_bframe_adaptive = X264_B_ADAPT_FAST; /*自适应B帧判定1 param->i_bframe_bias = 0; //控制插入B帧判定，范围-100~+100，越高越容易插入B帧 param->b_bframe_pyramid = 0; //允许部分B为参考帧 param->b_deblocking_filter = 1; //去块效应相关 param->i_deblocking_filter_alphac0 = 0; // [-6, 6] -6 亮度滤波器, 6 强 param->i_deblocking_filter_beta = 0; // [-6, 6] 同上 param->b_cabac = 1; /*cabac的开关 param->i_cabac_init_idc = 0; //码率控制 param->rc.i_rc_method = X264_RC_CRF;;//恒定码率 param->rc.i_bitrate = 0;//设置平均码率大小 param->rc.f_rate_tolerance = 1.0; param->rc.i_vbv_max_bitrate = 0; //平均码率模式下，最大瞬时码率，默认0(与-B设置相同) param->rc.i_vbv_buffer_size = 0; //码率控制缓冲区的大小，单位kbit，默认0 param->rc.f_vbv_buffer_init = 0.9; // 码率控制缓冲区数据保留的最大数据量与缓冲区大小之比，范围0~1.0，默认0.9 param->rc.i_qp_constant = 23;;/最小qp值 param->rc.f_rf_constant = 23; param->rc.i_qp_min = 10; //允许的最小量化值 param->rc.i_qp_max = 51; //允许的最大量化值 param->rc.i_qp_step = 4; //帧间最大量化步长 param->rc.f_ip_factor = 1.4; param->rc.f_pb_factor = 1.3; param->rc.i_aq_mode = X264_AQ_VARIANCE; /* psy adaptive QP. (X264_AQ_\*) param->rc.f_aq_strength = 1.0; param->rc.i_lookahead = 40; param->rc.b_stat_write = 0; //Enable stat writing in psz_stat_out param->rc.psz_stat_out = "x264_2pass.log"; param->rc.b_stat_read = 0; param->rc.psz_stat_in = "x264_2pass.log"; param->rc.f_qcompress = 0.6; /* 0.0 => cbr, 1.0 => constant qp param->rc.f_qblur = 0.5; //时间上模糊量化 param->rc.f_complexity_blur = 20; // 时间上模糊复杂性 param->rc.i_zones = 0; // number of zone_t's param->rc.b_mb_tree = 1; //Macroblock-tree ratecontrol. // 日志 param->pf_log = x264_log_default; param->p_log_private = NULL; param->i_log_level = X264_LOG_INFO;//默认为“Info” //分析 param->analyse.intra = X264_ANALYSE_I4x4 | X264_ANALYSE_I8x8; param->analyse.inter = X264_ANALYSE_I4x4 | X264_ANALYSE_I8x8 | X264_ANALYSE_PSUB16x16| X264_ANALYSE_BSUB16x16; param->analyse.i_direct_mv_pred = X264_DIRECT_PRED_SPATIAL;//空间预测模式 param->analyse.i_me_method = X264_ME_HEX;//运动估计算法HEX param->analyse.f_psy_rd = 1.0; param->analyse.b_psy = 1; param->analyse.f_psy_trellis = 0; param->analyse.i_me_range = 16;//运动估计范围 param->analyse.i_subpel_refine = 7; // 亚像素运动估计质量 param->analyse.b_mixed_references = 1; //允许每个宏块的分区在P帧有它自己的参考号 param->analyse.b_chroma_me = 1; // 亚像素色度运动估计和P帧的模式选择 param->analyse.i_mv_range_thread = -1; // 线程之间的最小空间. -1 = auto, based on number of threads. param->analyse.i_mv_range = -1; //运动矢量最大长度set from level_idc param->analyse.i_chroma_qp_offset = 0; //色度量化步长偏移量 param->analyse.b_fast_pskip = 1; //快速P帧跳过检测 param->analyse.b_weighted_bipred = 1; //为b帧隐式加权 param->analyse.b_dct_decimate = 1; // 在P-frames转换参数域 param->analyse.b_transform_8x8 = 1; // 帧间分区 param->analyse.i_trellis = 1; //Trellis量化，对每个8x8的块寻找合适的量化值，需要CABAC，默认0 0：关闭1：只在最后编码时使用2：一直使用 param->analyse.i_luma_deadzone[0] = 21; //帧间亮度量化中使用的无效区大小 param->analyse.i_luma_deadzone[1] = 11; //帧内亮度量化中使用的无效区大小 param->analyse.b_psnr = 0;//是否显示PSNR param->analyse.b_ssim = 0;//是否显示SSIM //量化 param->i_cqm_preset = X264_CQM_FLAT; //自定义量化矩阵(CQM),初始化量化模式为flat 0 memset( param->cqm_4iy, 16, 16 ); memset( param->cqm_4ic, 16, 16 ); memset( param->cqm_4py, 16, 16 ); memset( param->cqm_4pc, 16, 16 ); memset( param->cqm_8iy, 16, 64 ); memset( param->cqm_8py, 16, 64 );//开辟空间 param->b_repeat_headers = 1; // 在每个关键帧前放置SPS/PPS param->b_aud = 0; //生成访问单元分隔符 }`
[注：以上代码和注释出处](https://link.jianshu.com?t=http://blog.csdn.net/vblittleboy/article/details/8028923)- 
第二部步根据preset值通过x264_param_apply_preset（x264_param_t *param, const char *preset )函数设置设置编码参数。

preset 接受取值为：
`x264_preset_names[] = { "ultrafast", "superfast", "veryfast", "faster", "fast", "medium", "slow", "slower", "veryslow", "placebo", 0 };`

此步骤通过速度设定某些参数。
```
static int x264_param_apply_preset( x264_param_t *param, const char *preset )
{
   char *end;
   int i = strtol( preset, &end, 10 );
   if( *end == 0 && i >= 0 && i < sizeof(x264_preset_names)/sizeof(*x264_preset_names)-1 )
       preset = x264_preset_names[i];
  if( !strcasecmp( preset, "ultrafast" ) )
 {
  param->i_frame_reference = 1;//参考帧的最大帧数设为1
  param->i_scenecut_threshold = 0;
  param->b_deblocking_filter = 0;//不使用去块滤波  
  param->b_cabac = 0;//关闭cabac
  param->i_bframe = 0;//关闭b帧
  param->analyse.intra = 0;
  param->analyse.inter = 0;
  param->analyse.b_transform_8x8 = 0;
  param->analyse.i_me_method = X264_ME_DIA;;////运动估算法的选择 
  param->analyse.i_subpel_refine = 0;
  param->rc.i_aq_mode = 0;
  param->analyse.b_mixed_references = 0;
  param->analyse.i_trellis = 0;
  param->i_bframe_adaptive = X264_B_ADAPT_NONE;
  //关闭b帧判定选项
  param->rc.b_mb_tree = 0;
  param->analyse.i_weighted_pred = X264_WEIGHTP_NONE;
  param->analyse.b_weighted_bipred = 0;
  param->rc.i_lookahead = 0;
}
else if( !strcasecmp( preset, "superfast" ) )
{
  param->analyse.inter = X264_ANALYSE_I8x8|X264_ANALYSE_I4x4;
  param->analyse.i_me_method = X264_ME_DIA;;//钻石模板  
  param->analyse.i_subpel_refine = 1;/亚像素运动估计质量为1  
  param->i_frame_reference = 1;//参考帧的最大帧数设为1
  param->analyse.b_mixed_references = 0;
  param->analyse.i_trellis = 0;
  param->rc.b_mb_tree = 0;
  param->analyse.i_weighted_pred = X264_WEIGHTP_SIMPLE;
  param->rc.i_lookahead = 0;
}
else if( !strcasecmp( preset, "veryfast" ) )
{
  param->analyse.i_me_method = X264_ME_HEX;
  param->analyse.i_subpel_refine = 2;
  param->i_frame_reference = 1;//参考帧的最大帧数设为1
  param->analyse.b_mixed_references = 0;
  param->analyse.i_trellis = 0;
  param->analyse.i_weighted_pred = X264_WEIGHTP_SIMPLE;
  param->rc.i_lookahead = 10;
}
else if( !strcasecmp( preset, "faster" ) )
{
  param->analyse.b_mixed_references = 0;
  param->i_frame_reference = 2;//参考帧的最大帧数设为2
  param->analyse.i_subpel_refine = 4;
  param->analyse.i_weighted_pred = X264_WEIGHTP_SIMPLE;
  param->rc.i_lookahead = 20;
}
 else if( !strcasecmp( preset, "fast" ) )
 {
  param->i_frame_reference = 2;//参考帧的最大帧数设为2
  param->analyse.i_subpel_refine = 6;
  param->analyse.i_weighted_pred = X264_WEIGHTP_SIMPLE;
  param->rc.i_lookahead = 30;
  }
  else if( !strcasecmp( preset, "medium" ) )
  {
  /* Default is medium 
     *默认参考 set_param_default();
      */
  }
  else if( !strcasecmp( preset, "slow" ) )
  {
  param->analyse.i_me_method = X264_ME_UMH;//运动估算发的选择
  param->analyse.i_subpel_refine = 8;
  param->i_frame_reference = 5;//参考帧的最大帧数设为5
  param->i_bframe_adaptive = X264_B_ADAPT_TRELLIS;
  param->analyse.i_direct_mv_pred = X264_DIRECT_PRED_AUTO;
  param->rc.i_lookahead = 50;
}
else if( !strcasecmp( preset, "slower" ) )
{
  param->analyse.i_me_method = X264_ME_UMH;//运动估算发的选择
  param->analyse.i_subpel_refine = 9;
  param->i_frame_reference = 8;//参考帧的最大帧数设为8
  param->i_bframe_adaptive = X264_B_ADAPT_TRELLIS;
  param->analyse.i_direct_mv_pred = X264_DIRECT_PRED_AUTO;
  param->analyse.inter |= X264_ANALYSE_PSUB8x8;
  param->analyse.i_trellis = 2;
  param->rc.i_lookahead = 60;
}
else if( !strcasecmp( preset, "veryslow" ) )
{
  param->analyse.i_me_method = X264_ME_UMH;//运动估算发的选择
  param->analyse.i_subpel_refine = 10;
  param->analyse.i_me_range = 24;
  param->i_frame_reference = 16;//参考帧的最大帧数设为16
  param->i_bframe_adaptive = X264_B_ADAPT_TRELLIS;
  param->analyse.i_direct_mv_pred = X264_DIRECT_PRED_AUTO;
  param->analyse.inter |= X264_ANALYSE_PSUB8x8;
  param->analyse.i_trellis = 2;
  param->i_bframe = 8;//两个参考帧之间b帧为8
  param->rc.i_lookahead = 60;
}
else if( !strcasecmp( preset, "placebo" ) )
{
  param->analyse.i_me_method = X264_ME_TESA;//运动估算发的选择
  param->analyse.i_subpel_refine = 11;
  param->analyse.i_me_range = 24;//运动估计范围设为24
  param->i_frame_reference = 16;//参考帧的最大帧数设为16
  param->i_bframe_adaptive = X264_B_ADAPT_TRELLIS;
  param->analyse.i_direct_mv_pred = X264_DIRECT_PRED_AUTO;
  param->analyse.inter |= X264_ANALYSE_PSUB8x8;
  param->analyse.b_fast_pskip = 0;
  param->analyse.i_trellis = 2;
  param->i_bframe = 16;//参考帧之间b帧为16
  param->rc.i_lookahead = 60;
}
else
{
  x264_log( NULL, X264_LOG_ERROR, "invalid preset '%s'\n", preset );
  return -1;
}
return 0;
}
```

已加入注释。选用ultrafast会关闭b帧，但是同等质量也会增大码率。

veryfast 之后会启用各种保护质量的算法， 很大程度上降低编码速度。
- 第三步是根据 tune值，通过x264_param_apply_tune（）函数设定编码质量相关值。

static int x264_param_apply_tune( x264_param_t *param, const char *tune )

{

char *tmp = x264_malloc( strlen( tune ) + 1 );if( !tmp )return -1;tmp = strcpy( tmp, tune );char *s = strtok( tmp, ",./-+" );int psy_tuning_used = 0;while( s ){if( !strncasecmp( s, "film", 4 ) ){if( psy_tuning_used++ ) goto psy_failure;param->i_deblocking_filter_alphac0 = -1;param->i_deblocking_filter_beta = -1;param->analyse.f_psy_trellis = 0.15;}else if( !strncasecmp( s, "animation", 9 ) ){if( psy_tuning_used++ ) goto psy_failure;param->i_frame_reference = param->i_frame_reference > 1 ? param-                  >i_frame_reference2 : 1;param->i_deblocking_filter_alphac0 = 1;param->i_deblocking_filter_beta = 1;param->analyse.f_psy_rd = 0.4;param->rc.f_aq_strength = 0.6;param->i_bframe += 2;//增加b帧}else if( !strncasecmp( s, "grain", 5 ) ){if( psy_tuning_used++ ) goto psy_failure;param->i_deblocking_filter_alphac0 = -2;param->i_deblocking_filter_beta = -2;param->analyse.f_psy_trellis = 0.25;param->analyse.b_dct_decimate = 0;param->rc.f_pb_factor = 1.1;param->rc.f_ip_factor = 1.1;param->rc.f_aq_strength = 0.5;param->analyse.i_luma_deadzone[0] = 6;param->analyse.i_luma_deadzone[1] = 6;param->rc.f_qcompress = 0.8;}else if( !strncasecmp( s, "stillimage", 10 ) ){if( psy_tuning_used++ ) goto psy_failure;param->i_deblocking_filter_alphac0 = -3;param->i_deblocking_filter_beta = -3;param->analyse.f_psy_rd = 2.0;param->analyse.f_psy_trellis = 0.7;param->rc.f_aq_strength = 1.2;}else if( !strncasecmp( s, "psnr", 4 ) ){if( psy_tuning_used++ ) goto psy_failure;param->rc.i_aq_mode = X264_AQ_NONE;param->analyse.b_psy = 0;}else if( !strncasecmp( s, "ssim", 4 ) ){if( psy_tuning_used++ ) goto psy_failure;param->rc.i_aq_mode = X264_AQ_AUTOVARIANCE;param->analyse.b_psy = 0;}else if( !strncasecmp( s, "fastdecode", 10 ) ){param->b_deblocking_filter = 0;param->b_cabac = 0;param->analyse.b_weighted_bipred = 0;param->analyse.i_weighted_pred = X264_WEIGHTP_NONE;}else if( !strncasecmp( s, "zerolatency", 11 ) )//关闭b帧{param->rc.i_lookahead = 0;param->i_sync_lookahead = 0;param->i_bframe = 0; // 关闭b帧param->b_sliced_threads = 1;param->b_vfr_input = 0;param->rc.b_mb_tree = 0;}else if( !strncasecmp( s, "touhou", 6 ) ){if( psy_tuning_used++ ) goto psy_failure;param->i_frame_reference = param->i_frame_reference > 1 ? param-      >i_frame_reference2 : 1;param->i_deblocking_filter_alphac0 = -1;param->i_deblocking_filter_beta = -1;param->analyse.f_psy_trellis = 0.2;param->rc.f_aq_strength = 1.3;if( param->analyse.inter & X264_ANALYSE_PSUB16x16 )param->analyse.inter |= X264_ANALYSE_PSUB8x8;}else{x264_log( NULL, X264_LOG_ERROR, "invalid tune '%s'\n", s );x264_free( tmp );return -1;}if( 0 ){psy_failure:x264_log( NULL, X264_LOG_WARNING, "only 1 psy tuning can be used: ignoring tune %s\n", s );}s = strtok( NULL, ",./-+" );}x264_free( tmp );return 0;}使用zerolatency编码即刻返回编码后的输出，不用flush，问题详细参看：解析ffmpeg 视频流编解码末尾丢帧问题h->frames.i_delay =                                 param->i_sync_lookahead +                        // 前向考虑帧数                                max ( param->i_bframe,                               // B帧数量                                          param->rc.i_lookahead)  +                 // 码率控制前向考虑帧数而zerolatency 将所有设置为0；param->rc.i_lookahead = 0;param->i_sync_lookahead = 0;param->i_bframe = 0; // 关闭b帧param->b_sliced_threads = 1;param->b_vfr_input = 0;param->rc.b_mb_tree = 0;命令行使用 $x264 --fullhelp 会显示出对每种模式的说明。Presets:``  --profile <string>      Force the limits of an H.264 profile                              Overrides all settings.                              - baseline:                                --no-8x8dct --bframes 0 --no-cabac                                --cqm flat --weightp 0                                No interlaced.                                No lossless.                              - main:                                --no-8x8dct --cqm flat                                No lossless.                              - high:                                No lossless.                              - high10:                                No lossless.                                Support for bit depth 8-10.                              - high422:                                No lossless.                                Support for bit depth 8-10.                                Support for 4:2:0/4:2:2 chroma subsampling.                              - high444:                                Support for bit depth 8-10.                                Support for 4:2:0/4:2:2/4:4:4 chroma subsampling.  --preset <string>       Use a preset to select encoding settings [medium]                              Overridden by user settings.                              - ultrafast:                                --no-8x8dct --aq-mode 0 --b-adapt 0                                --bframes 0 --no-cabac --no-deblock                                --no-mbtree --me dia --no-mixed-refs                                --partitions none --rc-lookahead 0 --ref 1                                --scenecut 0 --subme 0 --trellis 0                                --no-weightb --weightp 0                              - superfast:                                --no-mbtree --me dia --no-mixed-refs                                --partitions i8x8,i4x4 --rc-lookahead 0                                --ref 1 --subme 1 --trellis 0 --weightp 1                              - veryfast:                                --no-mixed-refs --rc-lookahead 10                                --ref 1 --subme 2 --trellis 0 --weightp 1                              - faster:                                --no-mixed-refs --rc-lookahead 20                                --ref 2 --subme 4 --weightp 1                              - fast:                                --rc-lookahead 30 --ref 2 --subme 6                                --weightp 1                              - medium:                                Default settings apply.                              - slow:                                --b-adapt 2 --direct auto --me umh                                --rc-lookahead 50 --ref 5 --subme 8                              - slower:                                --b-adapt 2 --direct auto --me umh                                --partitions all --rc-lookahead 60                                --ref 8 --subme 9 --trellis 2                              - veryslow:                                --b-adapt 2 --bframes 8 --direct auto                                --me umh --merange 24 --partitions all                                --ref 16 --subme 10 --trellis 2                                --rc-lookahead 60                              - placebo:                                --bframes 16 --b-adapt 2 --direct auto                                --slow-firstpass --no-fast-pskip                                --me tesa --merange 24 --partitions all                                --rc-lookahead 60 --ref 16 --subme 11                                --trellis 2  --tune <string>         Tune the settings for a particular type of source                          or situation                              Overridden by user settings.                              Multiple tunings are separated by commas.                              Only one psy tuning can be used at a time.                              - film (psy tuning):                                --deblock -1:-1 --psy-rd <unset>:0.15                              - animation (psy tuning):                                --bframes {+2} --deblock 1:1                                --psy-rd 0.4:<unset> --aq-strength 0.6                                --ref {Double if >1 else 1}                              - grain (psy tuning):                                --aq-strength 0.5 --no-dct-decimate                                --deadzone-inter 6 --deadzone-intra 6                                --deblock -2:-2 --ipratio 1.1                                 --pbratio 1.1 --psy-rd <unset>:0.25                                --qcomp 0.8                              - stillimage (psy tuning):                                --aq-strength 1.2 --deblock -3:-3                                --psy-rd 2.0:0.7                              - psnr (psy tuning):                                --aq-mode 0 --no-psy                              - ssim (psy tuning):                                --aq-mode 2 --no-psy                              - fastdecode:                                --no-cabac --no-deblock --no-weightb                                --weightp 0                              - zerolatency:                                --bframes 0 --force-cfr --no-mbtree                                --sync-lookahead 0 --sliced-threads                                --rc-lookahead 0  --slow-firstpass        Don't force these faster settings with --pass 1:                              --no-8x8dct --me dia --partitions none                              --ref 1 --subme {2 if >2 else unchanged}                              --trellis 0 --fast-psk**








