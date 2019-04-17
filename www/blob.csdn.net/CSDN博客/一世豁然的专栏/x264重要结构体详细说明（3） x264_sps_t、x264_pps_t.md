# x264重要结构体详细说明（3）: x264_sps_t、x264_pps_t - 一世豁然的专栏 - CSDN博客





2016年08月18日 11:18:57[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1052








本博客转载于：[http://nkwavelet.blog.163.com/blog/static/227756038201431095653355/](http://nkwavelet.blog.163.com/blog/static/227756038201431095653355/)







/****************************************************************************************************************


x264_sps_t 序列参数集，该结构体定义在common/set.h中

*****************************************************************************************************************/


typedef struct

{

    int i_id;// sps的id号

    int i_profile_idc;// 指明所用的profile

    int i_level_idc;// 指明所用的level


/* 当他们值等于1时，表示必须遵守某些制约条件 */

    int b_constraint_set0;

    int b_constraint_set1;

    int b_constraint_set2;

    int b_constraint_set3;



    int i_log2_max_frame_num;// 表示图像解码顺序的最大取值 

    int i_poc_type;

    int i_log2_max_poc_lsb;// poc 0




    int i_num_ref_frames;// 指定参考帧队列的最大长度，h264规定最多可有16个参考帧

    int b_gaps_in_frame_num_value_allowed;

    int i_mb_width;

    int i_mb_height;

    int b_frame_mbs_only;

    int b_mb_adaptive_frame_field;

    int b_direct8x8_inference;// 指明B片的direct和skip模式下运动矢量的预测方法


 /* 图像裁剪参数 */

    int b_crop;


    struct

    {

        int i_left;

        int i_right;

        int i_top;

        int i_bottom;

    } crop;



    int b_vui;

    struct

    {

        int b_aspect_ratio_info_present;

        int i_sar_width;

        int i_sar_height;




        int b_overscan_info_present;

        int b_overscan_info;




        int b_signal_type_present;

        int i_vidformat;

        int b_fullrange;

        int b_color_description_present;

        int i_colorprim;

        int i_transfer;

        int i_colmatrix;




        int b_chroma_loc_info_present;

        int i_chroma_loc_top;

        int i_chroma_loc_bottom;




        int b_timing_info_present;

        uint32_t i_num_units_in_tick;

        uint32_t i_time_scale;

        int b_fixed_frame_rate;




        int b_nal_hrd_parameters_present;

        int b_vcl_hrd_parameters_present;




        struct

        {

            int i_cpb_cnt;

            int i_bit_rate_scale;

            int i_cpb_size_scale;

            int i_bit_rate_value;

            int i_cpb_size_value;

            int i_bit_rate_unscaled;

            int i_cpb_size_unscaled;

            int b_cbr_hrd;




            int i_initial_cpb_removal_delay_length;

            int i_cpb_removal_delay_length;

            int i_dpb_output_delay_length;

            int i_time_offset_length;

        } hrd;




        int b_pic_struct_present;

        int b_bitstream_restriction;

        int b_motion_vectors_over_pic_boundaries;

        int i_max_bytes_per_pic_denom;

        int i_max_bits_per_mb_denom;

        int i_log2_max_mv_length_horizontal;

        int i_log2_max_mv_length_vertical;

        int i_num_reorder_frames;

        int i_max_dec_frame_buffering;




        /* FIXME to complete */

    } vui;




    int b_qpprime_y_zero_transform_bypass;

    int i_chroma_format_idc;




} x264_sps_t;



/****************************************************************************************************************


x264_pps_t 图像参数集，该结构体定义在common/set.h中

*****************************************************************************************************************/


typedef struct

{

    int i_id;// pps的的序号，在片头被引用

    int i_sps_id;// 本图像参数集所引用的序列参数集的序号

    int b_cabac;// 是否采用cabac编码，为1就是用cabac，为0则使用calvc

    int b_pic_order;// poc的三种计算方法在片层还各需要用一些句法元素作为参数；
// 当等于1时，表示在片头会有句句法元素指明这些参数；

// 当不为1时，表示片头不会给出这些参数

    int i_num_slice_groups;// 加1表示图像中片组的个数




    int i_num_ref_idx_l0_default_active;// 指明目前参考帧队列的长度，即有多少个参考帧（短期和长期），用于list0

    int i_num_ref_idx_l1_default_active;// 指明目前参考帧队列的长度，即有多少个参考帧（短期和长期），用于list1




    int b_weighted_pred;// 是否开启P帧和SP帧的加权预测

    int b_weighted_bipred;// 是否开启B帧的加权预测




    int i_pic_init_qp;// 加26后用以指明亮度分量的量化参数的初始值

    int i_pic_init_qs;// 同上，只是用于SP和SI




    int i_chroma_qp_index_offset;//
色度分量的量化参数是根据亮度分量的量化参数计算出来的，本句法元素用以// 指明计算时用到的参数表示为在 QPC 值的表格中寻找 Cb色度分量而应加到
// 参数 QPY 和 QSY 上的偏移。该值应在-12 到 +12范围内（包括边界值）

    int b_deblocking_filter_control;// 编码器可以通过句法元素显式地控制去块滤波的强度

    int b_constrained_intra_pred;// 控制PB帧的帧内编码，在P和B帧中，帧内编码的宏块的邻近宏块可能采用帧间编码

    int b_redundant_pic_cnt;// 对于那些属于基本编码图像的条带和条带数据分割块，redundant_pic_cnt等于0；
// 在冗余编码图像中的编码条带和编码条带数据分割块，redundant_pic_cnt大于0；
// 当redundant_pic_cnt 不存在时，默认值为 0；redundant_pic_cnt的值应该在0到127之间。

    int b_transform_8x8_mode;

    int i_cqm_preset;// 外部量化矩阵的设置

    const uint8_t *scaling_list[8]; // 缩放比例列表，could be 12, but we don't allow separate Cb/Cr
 lists




} x264_pps_t;








