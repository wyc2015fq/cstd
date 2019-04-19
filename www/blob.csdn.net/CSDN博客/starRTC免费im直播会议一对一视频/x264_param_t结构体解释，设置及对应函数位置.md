# x264_param_t结构体解释，设置及对应函数位置 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年08月21日 16:42:01[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：515
typedef struct x264_param_t
{
  /* CPU 标志位 */
  unsigned int cpu;
  int i_threads; /* 并行编码多帧 */
  int b_deterministic; /*是否允许非确定性时线程优化*/
  int i_sync_lookahead; /* 线程超前缓冲 */
  /* 视频属性 */
  int i_width; /*
宽度*/
  int i_height; /*
高度*/
  int i_csp; /* 编码比特流的CSP,仅支持i420，色彩空间设置 */
  int i_level_idc; /* level值的设置*/
  int i_frame_total; /* 编码帧的总数, 默认 0 */
/*Vui参数集视频可用性信息视频标准化选项 */
  struct
  {
  /* they will be reduced to be 0 < x <= 65535 and prime */
  int i_sar_height;
  int i_sar_width; /*
设置长宽比 */
  int i_overscan; /* 0=undef, 1=no overscan, 2=overscan 过扫描线，默认"undef"(不设置)，可选项：show(观看)/crop(去除)*/
  /*见以下的值h264附件E */
  Int i_vidformat;/* 视频格式，默认"undef"，component/pal/ntsc/secam/mac/undef*/
  int b_fullrange; /*Specify full range samples setting，默认"off"，可选项：off/on*/
  int i_colorprim; /*原始色度格式，默认"undef"，可选项：undef/bt709/bt470m/bt470bg，smpte170m/smpte240m/film*/
  int i_transfer; /*转换方式，默认"undef"，可选项：undef/bt709/bt470m/bt470bg/linear,log100/log316/smpte170m/smpte240m*/
  int i_colmatrix; /*色度矩阵设置，默认"undef",undef/bt709/fcc/bt470bg,smpte170m/smpte240m/GBR/YCgCo*/
  int i_chroma_loc; /* both top & bottom色度样本指定，范围0~5，默认0 */
  } vui;
  int i_fps_num;
  int i_fps_den;
/*这两个参数是由fps帧率确定的，赋值的过程见下：
{ float fps;   
if( sscanf( value, "%d/%d", &p->i_fps_num, &p->i_fps_den ) == 2 )
  ;
  else if( sscanf( value, "%f", &fps ) )
  {
  p->i_fps_num = (int)(fps * 1000 + .5);
  p->i_fps_den = 1000;
  }
  else
  b_error = 1;
  }
Value的值就是fps。*/

  /*流参数 */
  int i_frame_reference; /* 参考帧最大数目 */
  int
i_keyint_max; /* 在此间隔设置IDR关键帧 */
  int i_keyint_min; /* 场景切换少于次值编码位I, 而不是 IDR. */
  int i_scenecut_threshold; /*如何积极地插入额外的I帧 */
  int i_bframe; /*两个相关图像间P帧的数目 */
  int i_bframe_adaptive; /*自适应B帧判定*/
  int i_bframe_bias; /*控制插入B帧判定，范围-100~+100，越高越容易插入B帧，默认0*/
  int b_bframe_pyramid; /*允许部分B为参考帧 */
/*去块滤波器需要的参数*/
  int b_deblocking_filter;
  int i_deblocking_filter_alphac0; /* [-6, 6] -6 light filter, 6 strong */
  int i_deblocking_filter_beta; /* [-6, 6] idem */
  /*熵编码 */
  int b_cabac;
  int i_cabac_init_idc;
  int b_interlaced; /* 隔行扫描 */
  /*量化 */
  int i_cqm_preset; /*自定义量化矩阵(CQM),初始化量化模式为flat*/
  char *psz_cqm_file; /* JM format读取JM格式的外部量化矩阵文件，自动忽略其他—cqm 选项*/
  uint8_t cqm_4iy[16]; /* used only if i_cqm_preset == X264_CQM_CUSTOM */
  uint8_t cqm_4ic[16];
  uint8_t cqm_4py[16];
  uint8_t cqm_4pc[16];
  uint8_t cqm_8iy[64];
  uint8_t cqm_8py[64];
  /* 日志 */
  void (*pf_log)( void *, int i_level, const char *psz, va_list );
  void *p_log_private;
  int
i_log_level;
  int b_visualize;
  char *psz_dump_yuv; /* 重建帧的名字 */
  /* 编码分析参数*/
  struct
  {
  unsigned int intra; /* 帧间分区*/
  unsigned int inter; /* 帧内分区 */
  int b_transform_8x8; /* 帧间分区*/
  int b_weighted_bipred; /*为b帧隐式加权 */
  int i_direct_mv_pred; /*时间空间队运动预测 */
  int i_chroma_qp_offset; /*色度量化步长偏移量 */
  int i_me_method; /* 运动估计[算法](http://lib.csdn.net/base/31) (X264_ME_*)
 */
  int i_me_range; /* 整像素运动估计搜索范围 (from predicted mv) */
  int i_mv_range; /* 运动矢量最大长度(in pixels). -1 = auto, based on level */
  int i_mv_range_thread; /* 线程之间的最小空间. -1 = auto, based on number of threads. */
  int i_subpel_refine; /* 亚像素运动估计质量 */
  int b_chroma_me; /* 亚像素色度运动估计和P帧的模式选择 */
  int b_mixed_references; /*允许每个宏块的分区在P帧有它自己的参考号*/
  int i_trellis; /* Trellis量化，对每个8x8的块寻找合适的量化值，需要CABAC，默认0 0：关闭1：只在最后编码时使用2：一直使用*/
  int b_fast_pskip; /*快速P帧跳过检测*/
  int b_dct_decimate; /* 在P-frames转换参数域 */
  int i_noise_reduction; /*自适应伪盲区 */
  float f_psy_rd; /* Psy RD strength */
  float f_psy_trellis; /* Psy trellis strength */
  int b_psy; /* Toggle all psy optimizations */
  /*，亮度量化中使用的无效区大小*/
  int i_luma_deadzone[2]; /* {帧间, 帧内}
 */
  int b_psnr; /* 计算和打印PSNR信息
 */
  int b_ssim; /*计算和打印SSIM信息*/
  } analyse;
  /* 码率控制参数 */
  struct
  {
  int i_rc_method; /*
X264_RC_* */ 好像有3种，如X264_RC_ABR
  int i_qp_constant; /* 0-51 */
  int i_qp_min; /*允许的最小量化值
 */
  int i_qp_max; /*允许的最大量化值*/
  int i_qp_step; /*帧间最大量化步长
 */
  int i_bitrate; /*设置平均码率
 */
  float f_rf_constant; /* 1pass VBR, nominal QP */
  float f_rate_tolerance;
  int i_vbv_max_bitrate; /*平均码率模式下，最大瞬时码率，默认0(与-B设置相同)
 */
  int i_vbv_buffer_size; /*码率控制缓冲区的大小，单位kbit，默认0 */
  float f_vbv_buffer_init; /* <=1: fraction of buffer_size. >1: kbit码率控制缓冲区数据保留的最[大数据](http://lib.csdn.net/base/20)量与缓冲区大小之比，范围0~1.0，默认0.9*/
  float f_ip_factor;
  float f_pb_factor;
  int i_aq_mode; /* psy adaptive QP. (X264_AQ_*) */
  float f_aq_strength;
  int b_mb_tree; /* Macroblock-tree ratecontrol. */
  int i_lookahead;
  /* 2pass 多次压缩码率控制 */
  int b_stat_write; /* Enable stat writing in psz_stat_out */
  char *psz_stat_out;
  int b_stat_read; /* Read stat from psz_stat_in and use it */
  char *psz_stat_in;
  /* 2pass params (same as ffmpeg ones) */
  float f_qcompress; /* 0.0 => cbr, 1.0 => constant qp */
  float f_qblur; /*时间上模糊量化 */
  float f_complexity_blur; /* 时间上模糊复杂性 */
  x264_zone_t *zones; /* 码率控制覆盖 */
  int i_zones; /* number of zone_t's */
  char *psz_zones; /*指定区的另一种方法*/
  } rc;
  /* Muxing parameters */
  int b_aud; /*生成访问单元分隔符*/
  int b_repeat_headers; /*
在每个关键帧前放置SPS/PPS*/
  int i_sps_id; /* SPS 和 PPS id 号 */
  /*切片（像条）参数 */
  int i_slice_max_size; /* 每片字节的最大数，包括预计的NAL开销. */
  int i_slice_max_mbs; /* 每片宏块的最大数，重写 i_slice_count */
  int i_slice_count; /* 每帧的像条数目: 设置矩形像条. */
  /* Optional callback for freeing this x264_param_t when it is done being used.
  * Only used when the x264_param_t sits in memory for an indefinite period of time,
  * i.e. when an x264_param_t is passed to x264_t in an x264_picture_t or in zones.
  * Not used when x264_encoder_reconfig is called directly. */
  void (*param_free)( void* );
} x264_param_t;
**X264的参数设置**
帧类型选项：
-I/--keyint <整数> 最大IDR帧间距，默认250
 就是gop
-i/--min-keyint <整数>
最小IDR帧间距，默认25
--scenecut <整数> 画面动态变化限，当超出此值时插入I帧，默认40
-b/--bframes <整数>     在IP帧之间可插入的B帧数量最大值，范围0~16，默认0
--no-b-adapt            关闭自适应B帧判定(-b设为1时可用，其他不推荐)
--b-bias <整数> 控制插入B帧判定，范围-100~+100，越高越容易插入B帧，默认0
--b-pyramid             允许B帧做参考帧
--no-cabac              关闭内容自适应二进制算术编码(CABAC，高效率的熵编码)(会提高速度，但严重影响质量)
-r/--ref <整数>         最大参考帧数，范围0~16，默认1
--nf                    关闭环路滤波(一种除马赛克算法)
-f/--filter <alpha:beta>设置环路滤波的AlphaC和Beta的参数，范围-6-6，默认都为0
码率控制选项：
-q/--qp <整数> 固定量化模式并设置使用的量化值，范围0~51，0为无损压缩，默认26
-B/--bitrate <整数>
设置平均码率
--crf <整数> 质量模式，量化值动态可变(目前不太成熟，质量不如设置固定量化值)
--qpmin <整数> 设置最小量化值，范围0~51，默认10
--qpmax <整数> 设置最大量化值，范围0~51，默认51
--qpstep <整数> 设置相邻帧之间的量化值差，范围0~50，默认4
--ratetol <小数>
平均码率模式下，瞬时码率可以偏离的倍数，范围0.1~100.0，默认1.0
--vbv-maxrate <整数> 平均码率模式下，最大瞬时码率，默认0(与-B设置相同)
--vbv-bufsize <整数>
码率控制缓冲区的大小，单位kbit，默认0
--vbv-init <小数> 码率控制缓冲区数据保留的最大数据量与缓冲区大小之比，范围0~1.0，默认0.9
--ipratio <小数> I帧和P帧之间的量化系数，默认1.40
--pbratio <小数> P帧和B帧之间的量化系数，默认1.30
--chroma-qp-offset <整数> 色度和亮度之间的量化差，范围-12~+12，默认0
-p/--pass <1|2|3> 多次压缩码率控制
1：第一次压缩，创建统计文件
2：按建立的统计文件压缩并输出，不覆盖统计文件，
3：按建立的统计文件压缩，优化统计文件
--stats <字符串> 统计文件的名称，默认"x264_2pass.log"
--rceq <字符串> 速率控制公式，默认"blurCplx^(1-qComp)"
--qcomp <小数>
线性量化控制，0.0为固定码率，1.0为固定量化值，默认0.6，只用于2-pass和质量模式
--cplxblur <小数> 根据相邻帧平滑量化值比例的最大值，范围0~99.9，默认20.0，只用于2-pass和质量模式
--qblur <小数> 对统计文件结果平滑量化值比例的最大值,范围0~99.9，默认0.5，只用于2-pass
--zones <z0>/<z1>/… 分段量化，格式为：<开始帧>,<结束帧>,<选项>，可选项为：q=<整数>(量化值)或b=& lt;小数>(码率倍数)
--qpfile<字符串> 帧定义，可以在文件里定义每个帧的种类和Q值
输入、输出选项：
--level <字符串>
设定等级(as defined by Annex A)
--fps <小数|rational> 设定帧率
--seek <整数> 设定起始帧
--frames <整数> 最大编码帧数
-o/--output 指定输出文件
--threads <整数> 编码线程(使用分片技术)
--thread-input 在自有线程中运行Avisynth
--no-asm 关闭全部CPU优化指令
--no-psnr 关闭PSNR计算
--quiet 安静模式
-v/--verbose 显示每一个帧的信息
--progress 显示编码进程
--visualize 显示编码帧信息图，和XVID类似
--sps-id <整数> 设置SPS和PPS的ID值，默认0
--aud 使用数据定义符号
**x264函数说明**
函数名称                                  所在位置                               完成功能 
void x264_frame_filter                 common\mc.c                                                  帧滤波 
void x264_frame_init_lowres       common\mc.c                                               亮度1/2像素值初始化 
void x264_mc_init                        common\mc.c                                                  运动估计初始化 
static void motion_compensation_chroma      common\mc.c                           色度运动估计 
static uint8_t *get_ref                 common\mc.c                                                  获取参考亮度像素 
static void mc_luma                 common\mc.c                     亮度运动估计 
　 common\mc.c 　 
void    x264_param_default            common\common.c                                           设置缺省参数 
void x264_log                              common\common.c                                          定义log级别 
static void x264_log_default             common\common.c                                          设置缺省日志参数 
void x264_picture_alloc         common\common.c       设置picture参数,根据输出图像格式分配空间 
void x264_picture_clean           common\common.c                          释放分配的图像空间 
int x264_nal_encode                  common\common.c                                             nal单元编码 
int x264_nal_decode               common\common.c                                            nal单元解码 
void *x264_malloc               common\common.c                                     X264内部定义的内存分配 
void x264_free                                               common\common.c                                           X264内存释放 
void *x264_realloc                                         common\common.c                                     X264重新分配图像空间 
void x264_reduce_fraction                            common\common.c                                              分数化简 
char *x264_slurp_file                                     common\common.c                                     将文件读入分配的缓存区 
char *x264_param2string                              common\common.c                           转换参数为字符串,返回字符串存放的地址 
void x264_cabac_context_init                       common\cabac.c                                        CABAC上下文取值表初始化 
void x264_cabac_decode_init                        common\cabac.c                                            CABAC解码流初始化 
static inline void x264_cabac_decode_renorm common\cabac.c                                           CABAC解码重新标准化 
int  x264_cabac_decode_decision                  common\cabac.c 　 
int  x264_cabac_decode_bypass                   common\cabac.c 　 
int  x264_cabac_decode_terminal                  common\cabac.c 　 
void x264_cabac_encode_init                        common\cabac.c 　 
static inline void x264_cabac_putbit                common\cabac.c 　 
static inline void x264_cabac_encode_renorm common\cabac.c 　 
void x264_cabac_encode_decision                 common\cabac.c 　 
void x264_cabac_encode_bypass                  common\cabac.c 　 
void x264_cabac_encode_terminal                 common\cabac.c 　 
void x264_cabac_encode_flush                      common\cabac.c 　 
void x264_cabac_size_decision                       common\cabac.c 　 
int x264_cabac_size_decision2                       common\cabac.c 　 
int 264_cabac_size_decision_noup                 common\cabac.c 　 
static inline int clip_uint8                                 common\dct.c clip3(x,0,255) 
static void dct2x2dc                                      common\dct.c                          2*2直流系数的Hadamard变换 以4*4变换为基static void dct4x4dc             
                        common\dct.c                          4*4DC系数的Hadamard变换 
static void idct4x4dc                                    common\dct.c            每行每列一维蝶形快速算法,完成d[4][4]的Hadamard反变换 
static void sub4x4_dct                                common\dct.c                                    对4*4残差进行DCT变换 
static void sub8x8_dct                                common\dct.c                                 对8*8残差进行DCT变换 
static void sub16x16_dct                            common\dct.c                                 对16*16残差进行DCT变换 
static void add4x4_idct                              common\dct.c                         残差块DCT反变换后,加到预测块上,重构4*4块 
static void add8x8_idct                              common\dct.c                           残差块DCT反变换后,加到预测块上,重构8*8块 
static void add16x16_idct                          common\dct.c                         残差块DCT反变换后,加到预测块上,重构16*16块 
void x264_dct_init                                     common\dct.c                                             DCT运算初始化 
static void quant_8x8_core                       common\quant.c                                         8*8 AC量化 
static void quant_4x4_core                       common\quant.c                                        4*4 AC量化 
static void quant_4x4_dc_core                common\quant.c                                         4*4 亮度DC量化 
static void quant_2x2_dc_core                common\quant.c                                         2*2 色度DC量化 
static void dequant_4x4                          common\quant.c                                          4*4 AC反量化 
static void dequant_8x8                          common\quant.c                                          8*8 AC反量化 
void x264_mb_dequant_2x2_dc            common\quant.c                                          2*2 色度DC反量化 
void x264_mb_dequant_4x4_dc           common\quant.c                                           4*4 亮度DC反量化 
void x264_quant_init                             common\quant.c                                           量化参量初始化 
x264_frame_t *x264_frame_new          common\frame.c                                             创建新帧 
void x264_frame_delete                         common\frame.c                                           删除帧,释放空间 
void x264_frame_copy_picture             common\frame.c                                         将图像拷贝到帧中 
static void plane_expand_border           common\frame.c                                边界扩展(被其他具体的扩展函数调用) 
void x264_frame_expand_border         common\frame.c                                                帧边界扩展 
void x264_frame_expand_border_filtered common\frame.c                                     为滤波进行的边界扩展 
void x264_frame_expand_border_lowres common\frame.c                               为计算亮度半像素值进行边界扩展 
void x264_frame_expand_border_mod16 common\frame.c                                帧边界不是16整数倍时进行边界扩展 
static inline void deblock_luma_c           common\frame.c                                    bs=1~3时,修正亮度MB边界的p0和q0值 
static void deblock_v_luma_c               common\frame.c                                         亮度分量垂直边界去块滤波 
static void deblock_h_luma_c               common\frame.c                                           亮度分量水平边界去块滤波 
static inline void deblock_chroma_c      common\frame.c                                      bs=1~3时,修正色度MB边界的p0和q0值 
static void deblock_v_chroma_c          common\frame.c                                            色度分量垂直边界去块滤波 
static void deblock_h_chroma_c          common\frame.c                                           色度分量水平边界去块滤波 
static inline void deblock_luma_intra_c  common\frame.c                                          bs=4时,修正亮度MB边界的值 
static void deblock_v_luma_intra_c      common\frame.c                                           帧内亮度分量垂直边界去块滤波 
static void deblock_h_luma_intra_c      common\frame.c                                          帧内亮度分量水平边界去块滤波 
static inline void deblock_chroma_intra_c common\frame.c                                        bs=4时,修正色度MB边界的值 
static void deblock_v_chroma_intra_c  common\frame.c                                          帧内色度分量垂直边界去块滤波 
static void deblock_h_chroma_intra_c  common\frame.c                                          帧内色度分量水平边界去块滤波 
static inline void deblock_edge              common\frame.c                                           bs值确定 
void x264_frame_deblocking_filter       common\frame.c                                           帧去块滤波主函数 
void x264_deblock_init                        common\frame.c                                           去块滤波初始化 
int x264_mb_predict_intra4x4_mode  common\macroblock.c                                  帧内4*4块模式预测 
int x264_mb_predict_non_zero_code  common\macroblock.c                                      非零 
int x264_mb_transform_8x8_allowed  common\macroblock.c                               判断当前宏块是否允许8*8变换 
void x264_mb_predict_mv                  common\macroblock.c                                        宏块运动矢量预测 
void x264_mb_predict_mv_16x16      common\macroblock.c                                       16*16块MV预测 
void x264_mb_predict_mv_pskip         common\macroblock.c                                          Pskip块MV预测 
static int x264_mb_predict_mv_direct16x16_temporal common\macroblock.c              直接模式16*16块MV时间预测 
static int x264_mb_predict_mv_direct16x16_spatial common\macroblock.c                 直接模式16*16块MV空间预测 
int x264_mb_predict_mv_direct16x16 common\macroblock.c                                     直接模式16*16块MV预测 
void x264_mb_load_mv_direct8x8 common\macroblock.c                                           直接模式8*8块MV加载 
void x264_mb_predict_mv_ref16x16   common\macroblock.c                                          16*16参考块MV预测 
static inline void x264_mb_mc_0xywh  common\macroblock.c                                         前向宏块运动补偿 
static inline void x264_mb_mc_1xywh  common\macroblock.c                                        后向宏块运动补偿 
static inline void x264_mb_mc_01xywh common\macroblock.c                                          宏块双向运动补偿 
static void x264_mb_mc_direct8x8         common\macroblock.c                                   直接模式8*8块运动补偿 
void x264_mb_mc_8x8                       common\macroblock.c                                    各种类型8*8块及其分割的运动补偿 
void x264_mb_mc                               common\macroblock.c                                各种类型块运动补偿(调用以上各MC子函数) 
void x264_macroblock_cache_init        common\macroblock.c                                  初始化表征宏块的各变量,分配内存空间 
void x264_macroblock_cache_end       common\macroblock.c                                      释放为宏块分配的cache空间 
void x264_macroblock_slice_init            common\macroblock.c                                          初始MB与slice映射关系 
void x264_macroblock_cache_load       common\macroblock.c                                   宏块cache加载,所有宏块表征变量赋值 
void x264_macroblock_cache_save      common\macroblock.c                                          保存cache中的变量值 
void x264_macroblock_bipred_init        common\macroblock.c                                               宏块双向预测初始化 
int64_t x264_pixel_ssd_wxh                 common\pixel.c                                                          计算像素差值平方和 
static inline void pixel_sub_wxh              common\pixel.c                                                              计算像素差 
static int pixel_satd_wxh                        common\pixel.c                                          计算4*4hardmard变换后的绝对误差和 
static inline int pixel_sa8d_wxh               common\pixel.c                                             计算8*8hardmard变换后的绝对误差和 
static void predict_16x16_dc                common\predict.c                             上和左邻块可用时,帧内16*16亮度块DC模式预测 
static void predict_16x16_dc_left         common\predict.c                              左边邻块可用时,帧内16*16亮度块DC模式预测 
static void predict_16x16_dc_top        common\predict.c                             上边邻块可用时,帧内16*16亮度块DC模式预测 
static void predict_16x16_dc_128        common\predict.c             邻块均不可用时,帧内16*16亮度块预测DC模式,预测值为128 
static void predict_16x16_h                 common\predict.c                                        帧内16*16亮度块水平预测 
static void predict_16x16_v                 common\predict.c                                            帧内16*16亮度块垂直预测 
static void predict_16x16_p                  common\predict.c                                         帧内16*16亮度块平面预测 
static void predict_8x8c_dc_128         common\predict.c 　 
static void predict_8x8c_dc_left            common\predict.c 　 
static void predict_8x8c_dc_top            common\predict.c 　 
static void predict_8x8c_dc                  common\predict.c 　 
static void predict_8x8c_h                    common\predict.c 　 
static void predict_8x8c_v                    common\predict.c 　 
static void predict_8x8c_p                    common\predict.c 　 
static void predict_4x4c_dc_128          common\predict.c 　 
static void predict_4x4c_dc_left           common\predict.c 　 
static void predict_4x4c_dc_top           common\predict.c 　 
static void predict_4x4c_dc                 common\predict.c 　 
static void predict_4x4c_h                   common\predict.c 　 
static void predict_4x4c_v                   common\predict.c 　 
static void predict_4x4c_p                    common\predict.c 　 
static void predict_4x4_ddl                   common\predict.c                                             模式3 左下对角预测 
static void predict_4x4_ddr                  common\predict.c                                              模式4 右下对角预测 
static void predict_4x4_vr                   common\predict.c                                               模式5 垂直左下角 
static void predict_4x4_hd                   common\predict.c                                              模式6 水平斜下角 
static void predict_4x4_vl                     common\predict.c                                              模式7 垂直左下角 
static void predict_4x4_hu                   common\predict.c                                              模式8  水平斜上角 
static void predict_8x8c_dc_128             common\predict.c 　 
static void predict_8x8c_dc_left            common\predict.c 　 
static void predict_8x8c_dc_top          common\predict.c 　 
static void predict_8x8c_dc                  common\predict.c 　 
static void predict_8x8c_h                common\predict.c 　 
static void predict_8x8c_v                common\predict.c 　 
static void predict_8x8c_p                common\predict.c 　 
static void predict_8x8_ddl                 common\predict.c 　 
static void predict_8x8_ddr               common\predict.c 　 
static void predict_8x8_vr                   common\predict.c 　 
static void predict_8x8_hd                 common\predict.c 　 
static void predict_8x8_vl                  common\predict.c 　 
static void predict_8x8_hu              common\predict.c 　 
void x264_predict_16x16_init            common\predict.c                                                帧内16*16亮度块预测模式初始化 
void x264_predict_8x8c_init( int cpu, x264_predict8x8_t pf[7] ) common\predict.c   帧内8*8亮度块预测模式初始化(7种模式) 
void x264_predict_8x8c_init( int cpu, x264_predict8x8_t pf[12] ) common\predict.c 帧内8*8亮度块预测模式初始化(12种模式) 
void x264_predict_4x4_init             common\predict.c                                              帧内4*4亮度块预测模式初始化 
void x264_cqm_init                        common\set.c                                                                   量化矩阵初始化 
int x264_cqm_parse_jmlist                 common\set.c                                                         分析量化矩阵列表的正确性 
int x264_cqm_parse_file                 common\set.c                                                             分析量化矩阵文件的正确性 
static inline void x264_cabac_encode_ue_bypass       encoder\cabac.c                                    无符号指标GOLOMB编码 
static inline void x264_cabac_mb_type_intra              encoder\cabac.c                                     宏块帧内类型cabac编码 
static void x264_cabac_mb_type                               encoder\cabac.c                                     宏块类型cabac编码 
static void x264_cabac_mb_intra4x4_pred_mode      encoder\cabac.c                              4*4帧内亮度块预测模式cabac编码 
static void x264_cabac_mb_intra_chroma_pred_mode   encoder\cabac.c                                 帧内色度块预测模式cabac编码 
static void x264_cabac_mb_cbp_luman                    coder\cabac.c                                             宏块亮度cbp值cabac编码 
static void x264_cabac_mb_cbp_chroma                 encoder\cabac.c                                             宏块色度cbp值cabac编码 
static void x264_cabac_mb_qp_delta                       encoder\cabac.c                                           宏块增量量化参数cabac编码 
void x264_cabac_mb_skip                                       encoder\cabac.c                                               可跳过宏块cabac编码 
static inline void x264_cabac_mb_sub_p_partition   encoder\cabac.c                                           8*8子宏块分割cabac编码 
static inline void x264_cabac_mb_sub_b_partition   encoder\cabac.c                                    B帧8*8子宏块各种分割cabac编码 
static inline void x264_cabac_mb_transform_size     encoder\cabac.c                                           宏块变换块尺寸cabac编码 
static inline void x264_cabac_mb_ref                       encoder\cabac.c                                          参考宏块cabac编码 
static inline void x264_cabac_mb_mvd_cpn              encoder\cabac.c                                       宏块的mvd进行cabac编码 
static inline void x264_cabac_mb_mvd                   encoder\cabac.c  计算并保存宏块mvd,调用上个函数对mvd进行cabac编码 
static inline void x264_cabac_mb8x8_mvd             encoder\cabac.c                                    8*8宏块mvd的cabac编码 
static int x264_cabac_mb_cbf_ctxidxinc                encoder\cabac.c                          返回宏块左边和上边块非0像素的个数 
static void block_residual_write_cabac                     encoder\cabac.c                                                   残差块cabac编码 
void x264_macroblock_write_cabac                         encoder\cabac.c                      宏块cabac编码主函数(调用以上各子函数) 
定义RD时使用 encoder\cabac.c 　 
void x264_partition_size_cabac                                encoder\cabac.c                                       宏块分割cabac编码主函数 
static void x264_partition_i8x8_size_cabac                    encoder\cabac.c 　 
static void x264_partition_i4x4_size_cabac                 encoder\cabac.c 　 
static void block_residual_write_cavlc                           encoder\cavlc.c                                       残差块cavlc编码 
static void cavlc_qp_delta                                            encoder\cavlc.c                                  量化参数增量cavlc编码 
static void cavlc_mb_mvd                                           encoder\cavlc.c                                  运动矢量差值cavlc编码 
static void cavlc_mb8x8_mvd                                     encoder\cavlc.c                                  8*8子宏块运动矢量差值cavlc编码 
static inline void x264_macroblock_luma_write_cavlc   encoder\cavlc.c                                     亮度宏块cavlc编码 
void x264_macroblock_write_cavlc                            encoder\cavlc.c                                    宏块cavlc编码主函数 
#ifdef RDO_SKIP_BS时使用下列 encoder\cavlc.c 　 
int x264_partition_size_cavlc enc oder\cavlc.c 　 
static int cavlc_intra4x4_pred_size encoder\cavlc.c 　 
static int x264_partition_i8x8_size_cavlc encoder\cavlc.c 　 
static int x264_partition_i4x4_size_cavlc encoder\cavlc.c 　 
static double pop encoder\eval.c 出栈 
static void push encoder\eval.c 入栈 
static int strmatch encoder\eval.c 串匹配 
static void evalPrimary encoder\eval.c 一些常用计算 
static void evalPow encoder\eval.c 运算式分析或计算 
static void evalFactor encoder\eval.c 求幂运算 
static void evalTerm encoder\eval.c 　 
static void evalExpression encoder\eval.c 　 
double x264_eval encoder\eval.c 输入运算变量和运算符,功能调用运算表达式分析函数 
void x264_me_search_ref encoder\me.c 运动估计搜索 
void x264_me_refine_qpel 　 亚像素运动估计 
static void refine_subpel 　 亚像素运动估计搜索 
int x264_me_refine_bidir 　 双向运动估计 
void x264_me_refine_qpel_rd 　 有率失真的亚像素运动估计

[http://www.cnblogs.com/wainiwann/p/5647549.html](http://www.cnblogs.com/wainiwann/p/5647549.html)
