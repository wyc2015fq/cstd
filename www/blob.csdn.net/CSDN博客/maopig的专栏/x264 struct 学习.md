# x264 struct 学习 - maopig的专栏 - CSDN博客
2011年08月31日 10:16:47[maopig](https://me.csdn.net/maopig)阅读数：1316标签：[struct																[table																[function																[float																[codec																[list](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=codec&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)
个人分类：[ffmpeg																[x264 h264](https://blog.csdn.net/maopig/article/category/847676)](https://blog.csdn.net/maopig/article/category/847674)
[x264_t结构体维护着CODEC的诸多重要信息](http://compression.blog.sohu.com/89785801.html#)
其中成员frames是一个指示和控制帧编码过程的结构。其中current是已经准备就绪可以编码的帧，其类型已经确定；next是尚未确定类型的帧；unused用于回收不使用的frame结构体以备今后再次使用。
structx264_t                                 ////
{
   unsigned int  cpu;
   struct
   {
       int        i_nal;        //定义一个网络提取层//
       x264_nal_t nal[3];        
       int        i_bitstream;   
       uint8_t    *p_bitstream;  
       bs_t       bs;
   } out;
   x264_param_t   param;
   int            i_frame;           //一个指示和控制帧编码过程的结构
   int            i_poc;
   int            i_frame_offset;
   int            i_frame_num;   
   int            i_poc_msb;     
   int            i_poc_lsb;     
   x264_sps_t     sps_array[32];
   x264_sps_t     *sps;
   x264_pps_t     pps_array[256];
   x264_pps_t     *pps;
   int            i_idr_pic_id;
   x264_slice_header_t sh;
   x264_cabac_t   cabac;
   x264_picture_t  *picture;
   x264_frame_t   *bframe_current[X264_BFRAME_MAX];
   x264_frame_t   *frame_next[X264_BFRAME_MAX+1];  这个是定义下一个帧,但不一定是B帧
   x264_frame_t   *frame_unused[X264_BFRAME_MAX+1];
   x264_frame_t   *fdec;
   struct
   {
       DECLARE_ALIGNED( int, luma16×16_dc[16], 16 );
       DECLARE_ALIGNED( int, chroma_dc[2][4], 16 );//色度
       struct
       {
           DECLARE_ALIGNED( int, residual_ac[15], 16 );
           DECLARE_ALIGNED( int, luma4×4[16], 16 );
       } block[16+8];16*8的宏快的取用情况
   } dct;离散余弦变换
   struct
   {
       int    i_mb_stride;
       int    i_mb_x;
       int    i_mb_y;
       int    i_mb_xy;
       unsigned int i_neighbour;
       int8_t *type;                     
       int8_t *qp;                       
       int16_t*cbp;      ？               
       int8_t (*intra4×4_pred_mode)[7];  
       uint8_t (*non_zero_count)[16+4+4]; 
       int8_t *chroma_pred_mode;         
       int16_t(*mv[2])[2];               
       int16_t(*mvd[2])[2];              
       int8_t  *ref[2];                  
       int    i_type;
       int   i_partition;
       int    i_sub_partition[4];
       int    i_cbp_luma;
       int    i_cbp_chroma;
       int    i_intra16×16_pred_mode;
       int    i_chroma_pred_mode;
       struct
       {
           uint8_t *p_img[3];
           int     i_img[3];
           uint8_t *p_fdec[3];
           uint8_t*p_fref[2][16][3];
           int    i_fdec[3];
       } pic;
       struct
       {
           int    intra4×4_pred_mode[X264_SCAN8_SIZE];色度预测模式
           int    non_zero_count[X264_SCAN8_SIZE];
           int8_t ref[2][X264_SCAN8_SIZE];
           int16_t mv[2][X264_SCAN8_SIZE][2];
           int16_t mvd[2][X264_SCAN8_SIZE][2];运动矢量与真实值之间的差
       } cache;
       int    i_last_qp;
       int    i_last_dqp;
   } mb;宏快的预测以及运动补偿和运动估计
   x264_frame_t     *freference[16+1]; 
   int              i_ref0; 参考帧列表一（定义为lsit0意为向前预测编码）
   x264_frame_t     *fref0[16];      
   int              i_ref1;参考帧列表二（定义为lsit1意为向后预测编码）
   x264_frame_t     *fref1[16];      
   x264_ratecontrol_t *rc;
   struct
   {
       int  i_slice_count[5];
       int  i_slice_size[5];
       float f_psnr_y[5];
       float f_psnr_u[5];
       float f_psnr_v[5];
       int  i_mb_count[5][18];
   } stat;声明每一片的参数变量
   x264_predict_t     predict_16×16[4+3];
   x264_predict_t     predict_8×8[4+3];
   x264_predict_t     predict_4×4[9+3];
   x264_pixel_function_t pixf;
   x264_mc_function_t   mc[2];
   x264_dct_function_t  dctf;
   x264_vlc_table_t *x264_coeff_token_lookup[5];
   x264_vlc_table_t *x264_level_prefix_lookup;
   x264_vlc_table_t *x264_total_zeros_lookup[15];
   x264_vlc_table_t *x264_total_zeros_dc_lookup[3];
   x264_vlc_table_t *x264_run_before_lookup[7];
};
#endif
给出编码过程中的各个参数的初始化
x264_param_t
typedef struct
{
   unsigned int cpu;
   int        i_width;
   int        i_height;
   struct
   {
       int        i_sar_height;
       int        i_sar_width;
   } vui;？
   float      f_fps;
   int        i_frame_reference; 
   int        i_idrframe;
   int        i_iframe;  
   int        i_bframe;  
   int        b_deblocking_filter;
   int        b_cabac;
   int        i_cabac_init_idc;给出熵编码初始化时表格的选择
   int        i_qp_constant; 
   int        i_bitrate;     
   struct
   {
       unsigned intintra;   
       unsigned intinter;   
   } analyse;
}
x264_param_t给出编码过程中的各个参数的初始化
sps序列参数集
typedef struct
{
   int i_id;
   int i_profile_idc;指明所用profile
   int i_level_idc; 指明所用level
    int b_constraint_set0;
   int b_constraint_set1;
   int b_constraint_set2; 当他们值等于1时，表示必须遵守某些制约条件
   int i_log2_max_frame_num;表示图像解码顺序的最大取值
   inti_poc_type;   
   inti_log2_max_poc_lsb;   
   intb_delta_pic_order_always_zero;其值等于一时句法元素delta_pic_order_cnt[0]和delta_pic_order_cnt[1]不再片头出现，且他们的默认值都为0。为0时上述则出现。
   int i_offset_for_non_ref_pic;用来计算非参考帧或场的picture order count,其值应在[-2e31,2e31-1]
   int i_offset_for_top_to_bottom_field; 用来计算帧的底场的picture order count其值应在[-2e31,2e31-1]
   int i_num_ref_frames_in_poc_cycle;用来解码picture order count取值应在[0，255]之间
   int i_offset_for_ref_frame[256]; 当picture order counttype=1时用来解码poc，这句语法对循环num_ref_frames_in_poc_cycle中的每一个元素指定了一个偏移
   int i_num_ref_frames;指定参考帧队列的最大长度 h264规定最多可为16个参考帧
   int b_gaps_in_frame_num_value_allowed;值为1时表示允许句法frame_num可以不连续
   int i_mb_width;
   int i_mb_height;
   int b_frame_mbs_only;
   int b_mb_adaptive_frame_field;
   int b_direct8×8_inference;指明b片的直接和skip模式下运动矢量的预测方法
   int b_crop;
   struct
   {
       int i_left;
       int i_right;
       int i_top;
       int i_bottom;
   } crop;图像剪彩后输出的参数
   int b_vui;
   struct
   {
       int i_sar_width;
       int i_sar_height;
   } vui;
}
x264_sps_t定义序列参考队列的参数以及初始化
pps图像参数集
typedef struct
{
   int i_id;
   int i_sps_id;
   int b_cabac;
   int b_pic_order;
   int i_num_slice_groups; 加一表示图像中片组的个数
inti_slice_group_map_type;片组分割类型   
   int i_run_length[256]; 指明片组类型为2时每个片组连续的map_units的个数   
   inti_top_left[256];       
   inti_bottom_right[256];   
   int b_slice_group_change_direction; 当类型为345时与下句语法一起指明片组分割方法
   inti_slice_group_change_rate;   
   int i_pic_size_in_map_units;类型为6时指明图像以map_units为单位的大小
   int i_slice_group_id[256]; 指明map_units属于哪个片组
   int i_num_ref_idx_l0_active;加1为参考帧队列的长度 此为list0
   int i_num_ref_idx_l1_active;此为list1
   int b_weighted_pred;用以指明是否允许P和SP片的加权预测
   int b_weighted_bipred; 用以指明是否允许B片的加权预测
    int i_pic_init_qp;加26后用以指明亮度分量的量化参数的初始值
   int i_pic_init_qs;同上只是用于SP和SI
   int i_chroma_qp_index_offset;指明计算使用时用到的参数
   int b_deblocking_filter_control;
   int b_constrained_intra_pred;控制PB帧的帧内编码
   int b_redundant_pic_cnt;是否出现redundant_pic_cnt句法元素
} x264_pps_t
