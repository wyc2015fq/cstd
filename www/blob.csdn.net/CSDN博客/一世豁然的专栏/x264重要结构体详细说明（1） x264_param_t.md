# x264重要结构体详细说明（1）: x264_param_t - 一世豁然的专栏 - CSDN博客





2016年08月18日 11:12:04[一世豁然](https://me.csdn.net/Explorer_day)阅读数：2275








本博客转载于：[http://nkwavelet.blog.163.com/blog/static/2277560382013102010375030/](http://nkwavelet.blog.163.com/blog/static/2277560382013102010375030/)







结构体x264_param_t是x264中最重要的结构体之一，主要用于初始化编码器。以下给出了几乎每一个参数的含义，对这些参数的注释有的是参考了网上的资料，有的是自己的理解，还有的是对源代码的翻译，由于本人水平有限，错误难免，请高手指正。




/* 结构体x264_param_t定义在x264.h中 */

typedef struct x264_param_t

{

/* CPU 标志位 */

unsigned int cpu;

int  i_threads;                // 并行编码多帧; 线程数，为0则自动多线程编码

int  b_sliced_threads;        
// 如果为false，则一个slice只编码成一个NALU;

// 否则有几个线程，在编码成几个NALU。缺省为true。



int  b_deterministic;// 是否允许非确定性时线程优化

int  b_cpu_independent;// 强制采用典型行为，而不是采用独立于cpu的优化算法

int  i_sync_lookahead;// 线程超前缓存帧数






/* 视频属性 */

int  i_width;// 视频图像的宽

int  i_height;// 视频图像的高

int  i_csp;         // 编码比特流的CSP，仅支持i420，色彩空间设置

int  i_level_idc;// 指明作用的level值，可能与编码复杂度有关

int  i_frame_total; // 编码帧的总数, 默认 0






/* hrd : hypothetical reference decoder (假定参考解码器) , 检验编码器产生的符合

该标准的NAL单元流或字节流的偏差值。蓝光视频、电视广播及其它特殊领域有此要求 */

int  i_nal_hrd;






/* vui参数集 : 视频可用性信息、视频标准化选项 */

struct

{

   /* 宽高比的两个值相对互素，且在(0,  65535] 之间 */

    int  i_sar_height;// 样本宽高比的高度

int  i_sar_width;// 样本宽高比的宽度






/* 0=undef, 1=no overscan, 2=overscan 过扫描线，

    默认"undef"(不设置)，可选项：show(观看)/crop(去除) */



int  i_overscan;






/* 以下的值可以参见H264附录E */

int  i_vidformat;// 视频在编码/数字化之前是什么类型，默认"undef".

// 取值有：Component, PAL, NTSC, SECAM, MAC 等



int  b_fullrange;           // 样本亮度和色度的计算方式，默认"off"，可选项：off/on

int  i_colorprim;           // 原始色度格式，默认"undef"

int  i_transfer;            // 转换方式，默认"undef"

int  i_colmatrix;           // 设置从RGB计算得到亮度和色度所用的矩阵系数，默认"undef"

int  i_chroma_loc;          //设置色度采样位置，范围0~5，默认0

} vui;






/* 比特流参数 */

int  i_frame_reference;        // 最大参考帧数目

int  i_dpb_size;                // Decoded picture buffer size

int  i_keyint_max;              //设定IDR帧之间的最间隔，在此间隔设置IDR关键帧

int  i_keyint_min;            
//设定IDR帧之间的最小间隔, 场景切换小于此值编码位I帧, 而不是 IDR帧.

int  i_scenecut_threshold;      // 场景切换阈值，插入I帧

int  b_intra_refresh;           // 是否使用周期帧内刷新替代IDR帧



int  i_bframe;                  // 两个参考帧之间的B帧数目

int  i_bframe_adaptive;        
// 自适应B帧判定, 可选取值：X264_B_ADAPT_FAST等

int  i_bframe_bias;            
// 控制B帧替代P帧的概率，范围-100~+100，

// 该值越高越容易插入B帧，默认0.

int  i_bframe_pyramid;          // 允许部分B帧为参考帧，

// 可选取值：0=off,  1=strict hierarchical,  2=normal

int  b_open_gop;                // Close GOP是指帧间的预测都是在GOP中进行的。

// 使用Open GOP，后一个GOP会参考前一个GOP的信息。

int  b_bluray_compat;          // 支持蓝光碟






/*去块滤波器需要的参数, alpha和beta是去块滤波器参数 */

int  b_deblocking_filter;        // 去块滤波开关

int  i_deblocking_filter_alphac0;// [-6, 6] -6 light filter, 6 strong

int  i_deblocking_filter_beta;  // [-6, 6] 同上



int  b_cabac;                    // 自适应算术编码cabac开关

int  i_cabac_init_idc;          
// 给出算术编码初始化时表格的选择



int  b_interlaced;              
// 隔行扫描

int  b_constrained_intra;






 /*量化 */

int  i_cqm_preset;              // 自定义量化矩阵(CQM),初始化量化模式为flat

char *psz_cqm_file;             // 读取JM格式的外部量化矩阵文件，忽略其他cqm选项

uint8_t  cqm_4iy[16];           // used only if i_cqm_preset == X264_CQM_CUSTOM

uint8_t  cqm_4py[16];

uint8_t  cqm_4ic[16];

uint8_t  cqm_4pc[16];

uint8_t  cqm_8iy[64];

uint8_t  cqm_8py[64];

uint8_t  cqm_8ic[64];

uint8_t  cqm_8pc[64];






/* 日志 */

void  (*pf_log)( void *, int i_level, const char *psz, va_list );     // 日志函数

void  *p_log_private;

int    i_log_level;             // 日志级别，不需要打印编码信息时直接注释掉即可

int    b_visualize;             // 是否显示日志

char   *psz_dump_yuv;           //  重建帧的文件名






/* 编码分析参数*/

struct

{

unsigned int intra;            //  帧内分区

    unsigned int inter;            //  帧间分区



int  b_transform_8x8;           //

int  i_weighted_pred;           // P帧权重

int  b_weighted_bipred;        // B帧隐式加权

int  i_direct_mv_pred;          // 时间空间运动向量预测模式

int  i_chroma_qp_offset;        // 色度量化步长偏移量



int  i_me_method;               // 运动估计算法 (X264_ME_*)

int  i_me_range;                // 整像素运动估计搜索范围 (from predicted mv) 

int  i_mv_range;                // 运动矢量最大长度. -1 = auto, based on level

int i_mv_range_thread;         // 线程之间的最小运动向量缓冲.-1 = auto, based on number of threads.

int  i_subpel_refine;           // 亚像素运动估计质量

int  b_chroma_me;               // 亚像素色度运动估计和P帧的模式选择

int  b_mixed_references;      
// 允许每个宏块的分区有它自己的参考号

int  i_trellis;                // Trellis量化提高效率，对每个8x8的块寻找合适的量化值，需要CABAC，

// 0 ：即关闭  1：只在最后编码时使用2：在所有模式决策上启用

int  b_fast_pskip;            
// 快速P帧跳过检测

int  b_dct_decimate;          
// P帧变换系数阈值

int  i_noise_reduction;        
// 自适应伪盲区

int  b_psy;                    // Psy优化开关，可能会增强细节

float f_psy_rd;                // Psy RD强度

float  f_psy_trellis;           // Psy Trellis强度



int  i_luma_deadzone[2];        // 亮度量化中使用的盲区大小，{帧间, 帧内}

int  b_psnr;                    // 计算和打印PSNR信息

int  b_ssim;                    // 计算和打印SSIM信息

} analyse;






/* 码率控制参数 */

struct

{

int  i_rc_method;              // 码率控制方式 ： X264_RC_CQP恒定质量,
  

// X264_RC_CRF恒定码率,  X264_RC_ABR平均码率

int  i_qp_constant;            //指定P帧的量化值，0 - 51，0表示无损

int  i_qp_min;                  // 允许的最小量化值，默认10

int  i_qp_max;                
// 允许的最大量化值，默认51

int  i_qp_step;                 // 量化步长，即相邻两帧之间量化值之差的最大值



int  i_bitrate;              
// 平均码率大小

float  f_rf_constant;          // 1pass VBR, nominal QP. 实际质量，值越大图像越花,越小越清晰

float f_rf_constant_max;       // 最大码率因子，该选项仅在使用CRF并开启VBV时有效，

// 图像质量的最大值，可能会导致VBV下溢。

float  f_rate_tolerance;      
// 允许的误差

int    i_vbv_max_bitrate;      //平均码率模式下，最大瞬时码率，默认0

int    i_vbv_buffer_size;      //码率控制缓冲区的大小，单位kbit，默认0

float  f_vbv_buffer_init;       //设置码率控制缓冲区（VBV）缓冲达到多满(百分比)，才开始回放，

// 范围0~1.0，默认0.9

float  f_ip_factor;            // I帧和P帧之间的量化因子（QP）比值，默认1.4

float  f_pb_factor;            // P帧和B帧之间的量化因子（QP）比值，默认1.3



int   i_aq_mode;              
// 自适应量化（AQ）模式。 0：关闭AQ
  

// 1：允许AQ在整个视频中和帧内重新分配码

                                // 2：自方差AQ(实验阶段)，尝试逐帧调整强度



float  f_aq_strength;           // AQ强度，减少平趟和纹理区域的块效应和模糊度






/* MBTree File是一个临时文件，记录了每个P帧中每个MB被参考的情况。

目前mbtree只处理P帧的MB，同时也不支持b_pyramid. */

int   b_mb_tree;              
// 是否开启基于macroblock的qp控制方法

int   i_lookahead;            
// 决定mbtree向前预测的帧数






/* 2pass */

int   b_stat_write;            // 是否将统计数据写入到文件psz_stat_out中

char  *psz_stat_out;          
// 输出文件用于保存第一次编码统计数据

int   b_stat_read;            
// 是否从文件psz_stat_in中读入统计数据

char *psz_stat_in;             // 输入文件存有第一次编码的统计数据






/* 2pass params (same as ffmpeg ones) */

float  f_qcompress;             // 量化曲线(quantizer curve)压缩因子。

//0.0 => 恒定比特率，1.0 => 恒定量化值。

float  f_qblur;                // 时间上模糊量化，减少QP的波动(after curve compression)

float  f_complexity_blur;      // 时间上模糊复杂性，减少QP的波动(before curve compression)



x264_zone_t *zones;            // 码率控制覆盖

int   i_zones;                // number of zone_t's

char  *psz_zones;              // 指定区的另一种方法

} rc;






/* 裁剪矩形窗口参数：隐式添加到分辨率非16倍数的视频中 */

/* 在码流层指定一个切除(crop)矩形。若不想x264在编码时做crop，

   但希望解码器在回放时进行切除，可使用此项。单位为像素。*/

struct

{

unsigned int i_left;

    unsigned int i_top;

    unsigned int i_right;

    unsigned int i_bottom;

} crop_rect;






/* 编码3D视频时，此参数在码流中插入一个标志，告知解码器此3D视频是如何封装

  的。3D视频帧封装格式：帧封装、场交错、线交错、左右全景、左右半景、上下半

景、L+深度、L+深度+图形+图形深度*/

int i_frame_packing;






/* Muxing复用参数 */

int b_aud;                    // 生成访问单元分隔符

int b_repeat_headers;          // 是否复制sps和pps放在每个关键帧的前面

int b_annexb;                
// 值为true，则NALU之前是4字节前缀码0x00000001；

// 值为false，则NALU之前的4个字节为NALU长度

int i_sps_id;                  // sps和pps的id号

int b_vfr_input;               // VFR输入。1 ：时间基和时间戳用于码率控制
  0 ：仅帧率用于码率控制

uint32_t i_fps_num;          
// 帧率的分子

uint32_t i_fps_den;            // 帧率的分母

uint32_t i_timebase_num;      // 时间基的分子

uint32_t i_timebase_den;       // 时间基的分母






/* 以某个预设模式将输入流(隔行，恒定帧率)标记为软交错(soft telecine)。默认none. 可用预设有：

none, 22, 32, 64, double, triple, euro.
  使用除none以外任一预设，都会连带开启--pic-struct */

int b_pulldown;

int b_pic_struct;// 强制在Picture Timing SEI传送pic_struct. 默认是未开启






/* 将视频流标记为交错(隔行)，哪怕并非为交错式编码。可用于编码蓝光兼容的25p和30p视频。默认是未开启 */

int b_fake_interlaced;




/* 条带参数 */



int i_slice_max_size;
          // 每个slice的最大字节数，包括预计的NAL开销

int i_slice_max_mbs;           // 每个slice的最大宏块数，重写i_slice_count

int i_slice_count;            // 每帧slice的数目，每个slice必须是矩形



} x264_param_t;




