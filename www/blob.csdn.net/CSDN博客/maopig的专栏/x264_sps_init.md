# x264_sps_init - maopig的专栏 - CSDN博客
2011年08月26日 11:20:50[maopig](https://me.csdn.net/maopig)阅读数：2594
x264_sps_init此函数为序列量化集的初始化。主要对结构体x264_sps_t中参数的初始化。
void x264_sps_init( x264_sps_t *sps, int i_id, x264_param_t *param )
{
    sps->i_id = i_id;首先设置序列参数集的ID
b_qpprime_y_zero_transform_bypass判断码率控制方法是否是恒定质量方法并且量化值初始为0.
    sps->b_qpprime_y_zero_transform_bypass = param->rc.i_rc_method == X264_RC_CQP && param->rc.i_qp_constant == 0;
    if( sps->b_qpprime_y_zero_transform_bypass )
        sps->i_profile_idc  = PROFILE_HIGH444_PREDICTIVE;
    else if( param->analyse.b_transform_8x8 || param->i_cqm_preset != X264_CQM_FLAT )
        sps->i_profile_idc  = PROFILE_HIGH;
    else if( param->b_cabac || param->i_bframe > 0 || param->b_interlaced )
        sps->i_profile_idc  = PROFILE_MAIN;
    else
        sps->i_profile_idc  = PROFILE_BASELINE;
baseline：（1）b_qpprime_y_zero_transform_bypass不等于0（2）analyse.b_transform_8x8等于0并且param->i_cqm_preset = X264_CQM_FLAT（3）b_cabac等于0并且i_bframe小于等于0，并且interlaced=0
    sps->i_level_idc = param->i_level_idc;
定义本序列参数集的level值
    sps->b_constraint_set0  = sps->i_profile_idc == PROFILE_BASELINE;
    /* x264 doesn't support the features that are in Baseline and not in Main,
     * namely arbitrary_slice_order and slice_groups. */
    sps->b_constraint_set1  = sps->i_profile_idc <= PROFILE_MAIN;
    /* Never set constraint_set2, it is not necessary and not used in real world. */
    sps->b_constraint_set2  = 0;       ？附录A2.1
然后设置IDR帧之间的最大的帧数，这个帧数是以2的多少次幂计数的，它由参数的i_keyint_max（默认初始化为250）决定。
    sps->i_log2_max_frame_num = 4;  /* at least 4这个句法元素是为读取frame_num服务的 */
    while( (1 << sps->i_log2_max_frame_num) <= param->i_keyint_max )
    {
        sps->i_log2_max_frame_num++;
    }
    sps->i_log2_max_frame_num++;    /* just in case */
然后设置i_poc_type为0，POC类型指明了POC的编码方法，POC标识图象的播放顺序。由于H.264使用了B帧,使得图象的解码顺序并不一定等于播放顺序,但她们存在一定的映射关系。POC类型可以有Frame_num通过映射关系计算得来,也可以索性以编码器显示传送的方式传达给解码器. POC类型有3种方案.
   然后设置i_log2_max_poc_lsb，最大POC的什么东西，也搞不明白。
   POC是一个播放顺序，接受到一个IDR侦，POC会被清0
    sps->i_poc_type = 0;
    if( sps->i_poc_type == 0 )此处不懂？
    {
        sps->i_log2_max_poc_lsb = sps->i_log2_max_frame_num + 1;    /* max poc = 2*frame_num */
    }
    else if( sps->i_poc_type == 1 )
    {
        int i;
        /* FIXME */
        sps->b_delta_pic_order_always_zero = 1;
        sps->i_offset_for_non_ref_pic = 0;
        sps->i_offset_for_top_to_bottom_field = 0;
        sps->i_num_ref_frames_in_poc_cycle = 0;
        for( i = 0; i < sps->i_num_ref_frames_in_poc_cycle; i++ )
        {
            sps->i_offset_for_ref_frame[i] = 0;
        }
    }
    sps->b_vui = 1; 指明vui子结构是否出现在码流中（video usability information）
    sps->b_gaps_in_frame_num_value_allowed = 0; 这个句法元素等于时，表示允许句法元素frame_num可以不连续；当传输信道堵塞时，允许丢弃若干帧
    sps->i_mb_width = ( param->i_width + 15 ) / 16;
    sps->i_mb_height= ( param->i_height + 15 ) / 16;
    if( param->b_interlaced )
        sps->i_mb_height = ( sps->i_mb_height + 1 ) & ~1;
    sps->b_frame_mbs_only = ! param->b_interlaced;
    sps->b_mb_adaptive_frame_field = param->b_interlaced;
    sps->b_direct8x8_inference = 1; 指明b片的直接和skip模式下运动矢量的预测方法
    sps->crop.i_left   = 0;
    sps->crop.i_top    = 0;
    sps->crop.i_right  = sps->i_mb_width*16 - param->i_width;
    sps->crop.i_bottom = (sps->i_mb_height*16 - param->i_height) >> param->b_interlaced;
    sps->b_crop = sps->crop.i_left  || sps->crop.i_top ||
                  sps->crop.i_right || sps->crop.i_bottom;
指明解码器是否要将图像裁剪后输出，如果是的话，后面紧跟的四个句法元素分别指出左、右、上、下裁剪的宽度。此处不支持将图像剪裁后输出。
    sps->vui.b_aspect_ratio_info_present = 0;是否有目前长宽比信息。？
    if( param->vui.i_sar_width > 0 && param->vui.i_sar_height > 0 )
    {
        sps->vui.b_aspect_ratio_info_present = 1;
        sps->vui.i_sar_width = param->vui.i_sar_width;
        sps->vui.i_sar_height= param->vui.i_sar_height;
    }
    sps->vui.b_overscan_info_present = ( param->vui.i_overscan ? 1 : 0 );i_overscan类型 0=undef, 1=no overscan, 2=overscan
    if( sps->vui.b_overscan_info_present )
        sps->vui.b_overscan_info = ( param->vui.i_overscan == 2 ? 1 : 0 );
    sps->vui.b_signal_type_present = 0;
    sps->vui.i_vidformat = ( param->vui.i_vidformat <= 5 ? param->vui.i_vidformat : 5 );
    sps->vui.b_fullrange = ( param->vui.b_fullrange ? 1 : 0 );
    sps->vui.b_color_description_present = 0;
    sps->vui.i_colorprim = ( param->vui.i_colorprim <=  9 ? param->vui.i_colorprim : 2 ); /* undef原始色度格式*/
    sps->vui.i_transfer  = ( param->vui.i_transfer  <= 11 ? param->vui.i_transfer  : 2 ); /* undef 转换方式*/
    sps->vui.i_colmatrix = ( param->vui.i_colmatrix <=  9 ? param->vui.i_colmatrix : 2 ); /* undef 色度矩阵设置*/
    if( sps->vui.i_colorprim != 2 ||
        sps->vui.i_transfer  != 2 ||
        sps->vui.i_colmatrix != 2 )
    {
        sps->vui.b_color_description_present = 1;
    }
    if( sps->vui.i_vidformat != 5 ||
        sps->vui.b_fullrange ||
        sps->vui.b_color_description_present )
    {
        sps->vui.b_signal_type_present = 1;
    }
    /* FIXME: not sufficient for interlaced video */解释：对隔行扫描的视频是没有效。
    sps->vui.b_chroma_loc_info_present = ( param->vui.i_chroma_loc ? 1 : 0 );
    if( sps->vui.b_chroma_loc_info_present )判断是否有当前色度样本指定的信息。
    {
        sps->vui.i_chroma_loc_top = param->vui.i_chroma_loc;
        sps->vui.i_chroma_loc_bottom = param->vui.i_chroma_loc;
    }
    sps->vui.b_timing_info_present = 0; 264码流中时间相关信息是在vui中，x264生成的码流是有vui信息的，但jm等编码器默认情况下是不生成vui信息。这表示，其播放时间是不能给出的
    if( param->i_fps_num > 0 && param->i_fps_den > 0)
    {
        sps->vui.b_timing_info_present = 1;是否有目前的时间信息。
        sps->vui.i_num_units_in_tick = param->i_fps_den;在一个时间点的数量单位。
        sps->vui.i_time_scale = param->i_fps_num * 2;时间规模？
        sps->vui.b_fixed_frame_rate = 1;是否有恒定的帧率。/
    }
    sps->vui.i_num_reorder_frames = param->b_bframe_pyramid ? 2 : param->i_bframe ? 1 : 0; b_bframe_pyrami为允许部分B为参考帧。i_num_reorder_frames为重新排序的帧的数目。
    /* extra slot with pyramid so that we don't have to override the
l        order of forgetting old pictures */
l        额外的插入参考帧，以使得我们不用重写忘记的老的图像。
    sps->vui.i_max_dec_frame_buffering =
    sps->i_num_ref_frames = X264_MIN(16, X264_MAX(param->i_frame_reference, 1 + sps->vui.i_num_reorder_frames));
i_max_dec_frame_buffering最大的解码帧的缓冲区。
    sps->vui.b_bitstream_restriction = 1;是否比特流受限
    if( sps->vui.b_bitstream_restriction )
    {
        sps->vui.b_motion_vectors_over_pic_boundaries = 1; 是否有图像边界的运动矢量。
        sps->vui.i_max_bytes_per_pic_denom = 0; ?
        sps->vui.i_max_bits_per_mb_denom = 0;  ?
        sps->vui.i_log2_max_mv_length_horizontal =    宏块水平长度的最大值。
        sps->vui.i_log2_max_mv_length_vertical =      宏块垂直长度的最大值。(int)(log(param->analyse.i_mv_range*4-1)/log(2)) + 1;
    }
}
