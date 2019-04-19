# x264_param_t参数注解 - maopig的专栏 - CSDN博客
2011年08月25日 13:33:04[maopig](https://me.csdn.net/maopig)阅读数：6986标签：[struct																[filter																[buffer																[reference																[callback																[thread](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=reference&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=filter&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)
个人分类：[c/C++																[x264 h264																[ffmpeg](https://blog.csdn.net/maopig/article/category/847674)](https://blog.csdn.net/maopig/article/category/847676)](https://blog.csdn.net/maopig/article/category/847675)
typedef struct x264_param_t
{
    CPU 标志位
   unsigned int cpu;
   int        i_threads;       并行编码多帧
   int        b_deterministic; 是否允许非确定性时线程优化
   int        i_sync_lookahead;  线程超前缓冲
    视频属性
   int        i_width;  宽度
   int        i_height;  高度
   int        i_csp;  编码比特流的CSP,仅支持i420，色彩空间设置
   int        i_level_idc;  level值的设置
   int        i_frame_total;  编码帧的总数, 默认 0
Vui参数集视频可用性信息视频标准化选项
   struct
   {
        they will be reduced to be 0 < x <=65535 and prime
       int        i_sar_height;
       int        i_sar_width;  设置长宽比
       int        i_overscan;    0=undef, 1=no overscan, 2=overscan过扫描线，默认"undef"(不设置)，可选项：show(观看)/crop(去除)
       见以下的值h264附件E
       Int      i_vidformat; 视频格式，默认"undef"，component/pal/ntsc/secam/mac/undef
       int      b_fullrange; Specify full range samplessetting，默认"off"，可选项：off/on
       int      i_colorprim;原始色度格式，默认"undef"，可选项：undef/bt709/bt470m/bt470bg，smpte170m/smpte240m/film
       int   i_transfer;转换方式，默认"undef"，可选项：undef/bt709/bt470m/bt470bg/linear,log100/log316/smpte170m/smpte240m
       int  i_colmatrix;色度矩阵设置，默认"undef",undef/bt709/fcc/bt470bg,smpte170m/smpte240m/GBR/YCgCo
       int        i_chroma_loc;    both top & bottom色度样本指定，范围0~5，默认0
   } vui;
   int        i_fps_num;
   int        i_fps_den;
这两个参数是由fps帧率确定的，赋值的过程见下：
{       floatfps;      
 if( sscanf( value, "%d/%d",&p->i_fps_num,&p->i_fps_den ) == 2 )
           ;
       else if( sscanf( value, "%f", &fps ) )
       {
           p->i_fps_num = (int)(fps * 1000 + .5);
           p->i_fps_den = 1000;
       }
       else
           b_error = 1;
   }
Value的值就是fps。
   流参数
   int        i_frame_reference;  参考帧最大数目
   int        i_keyint_max;       在此间隔设置IDR关键帧
   int        i_keyint_min;       场景切换少于次值编码位I, 而不是 IDR.
   int        i_scenecut_threshold; 如何积极地插入额外的I帧// //
画面动态变化限，当超出此值时插入I帧，默认40
   int        i_bframe;  两个相关图像间P帧的数目
   int        i_bframe_adaptive; 自适应B帧判定
   int        i_bframe_bias; 控制插入B帧判定，范围-100~+100，越高越容易插入B帧，默认0
   int        b_bframe_pyramid;  允许部分B为参考帧
去块滤波器需要的参数
   int        b_deblocking_filter;////去块效应相关
   int        i_deblocking_filter_alphac0;    [-6, 6] -6 light filter, 6 strong
   int        i_deblocking_filter_beta;       [-6, 6]  idem
   熵编码
   int        b_cabac;
   int        i_cabac_init_idc;
   int        b_interlaced;  隔行扫描
   量化
   int        i_cqm_preset;        自定义量化矩阵(CQM),初始化量化模式为flat
   char       *psz_cqm_file;      JM format读取JM格式的外部量化矩阵文件，自动忽略其他—cqm 选项
   uint8_t    cqm_4iy[16];        used only if i_cqm_preset == X264_CQM_CUSTOM
   uint8_t    cqm_4ic[16];
   uint8_t    cqm_4py[16];
   uint8_t    cqm_4pc[16];
   uint8_t    cqm_8iy[64];
   uint8_t    cqm_8py[64];
    日志
   void       (*pf_log)( void *, int i_level, const char *psz, va_list );
   void       *p_log_private;
   int        i_log_level;
   int        b_visualize;
   char       *psz_dump_yuv;  重建帧的名字
    编码分析参数
   struct
   {
       unsigned intintra;     帧间分区
       unsigned intinter;     帧内分区
       int         b_transform_8x8;  帧间分区
       int         b_weighted_bipred; 为b帧隐式加权
       int         i_direct_mv_pred; 时间空间队运动预测
       int         i_chroma_qp_offset; 色度量化步长偏移量
       int         i_me_method;  运动估计算法 (X264_ME_*)
       int         i_me_range;  整像素运动估计搜索范围 (from predictedmv)
       int         i_mv_range;  运动矢量最大长度(in pixels). -1 =auto, based on level
       int         i_mv_range_thread;  线程之间的最小空间. -1 = auto,based on number of threads.
       int         i_subpel_refine;  亚像素运动估计质量
       int         b_chroma_me;  亚像素色度运动估计和P帧的模式选择
       int         b_mixed_references; 允许每个宏块的分区在P帧有它自己的参考号
       int         i_trellis;  Trellis量化，对每个8x8的块寻找合适的量化值，需要CABAC，默认0 0：关闭1：只在最后编码时使用2：一直使用
       int         b_fast_pskip; 快速P帧跳过检测
       int         b_dct_decimate; 在P-frames转换参数域
       int         i_noise_reduction; 自适应伪盲区
       float       f_psy_rd;  Psy RD strength
       float       f_psy_trellis;  Psy trellis strength
       int         b_psy;  Toggle all psy optimizations
       亮度量化中使用的无效区大小
       int         i_luma_deadzone[2];  {帧间, 帧内}
       int         b_psnr;    计算和打印PSNR信息
       int         b_ssim;   计算和打印SSIM信息
   } analyse;
    码率控制参数
   struct
   {
       int        i_rc_method;    X264_RC_*
       int        i_qp_constant;  0-51
       int        i_qp_min;      允许的最小量化值
       int        i_qp_max;      允许的最大量化值
       int        i_qp_step;     帧间最大量化步长
       int        i_bitrate;  设置平均码率
       float      f_rf_constant;  1pass VBR, nominal QP
       float      f_rate_tolerance;
       int        i_vbv_max_bitrate; 平均码率模式下，最大瞬时码率，默认0(与-B设置相同)
       int        i_vbv_buffer_size; 码率控制缓冲区的大小，单位kbit，默认0
       float      f_vbv_buffer_init;  <=1:fraction of buffer_size. >1:kbit码率控制缓冲区数据保留的最大数据量与缓冲区大小之比，范围0~1.0，默认0.9
       float      f_ip_factor;
       float      f_pb_factor;
       int        i_aq_mode;      psy adaptive
 QP. (X264_AQ_*)
       float      f_aq_strength;
       int        b_mb_tree;      Macroblock-tree
 ratecontrol.
       int        i_lookahead;
        2pass 多次压缩码率控制
       int        b_stat_write;  Enable stat writing in psz_stat_out
       char       *psz_stat_out;
       int        b_stat_read;    Read stat from psz_stat_in and
 use it
       char       *psz_stat_in;
        2pass params (same as ffmpeg ones)
       float      f_qcompress;    0.0 => cbr, 1.0 => constant qp
       float      f_qblur;       时间上模糊量化
       float      f_complexity_blur;  时间上模糊复杂性
       x264_zone_t*zones;         码率控制覆盖
       int        i_zones;        number
 of zone_t's
       char       *psz_zones;    指定区的另一种方法
   } rc;
    Muxing parameters
   intb_aud;                 生成访问单元分隔符
   intb_repeat_headers;       在每个关键帧前放置SPS/PPS
   inti_sps_id;               SPS 和 PPS id 号
   切片（像条）参数
   inti_slice_max_size;    每片字节的最大数，包括预计的NAL开销.
   inti_slice_max_mbs;     每片宏块的最大数，重写 i_slice_count
   inti_slice_count;       每帧的像条数目: 设置矩形像条.
    Optional callback for freeing this x264_param_t when it is donebeing used.
     Only used when the x264_param_t sits in memory for an indefiniteperiod of time,
     i.e. when an x264_param_t is passed to x264_t in an x264_picture_tor in zones.
     Not used when x264_encoder_reconfig is called directly.
   void (*param_free)( void* );
} x264_param_t;

