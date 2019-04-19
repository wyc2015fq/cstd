# 一步一步解析H.264码流的NALU(SPS,PSS,IDR)获取宽高和帧率 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年09月21日 16:40:35[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：807
分析H.264码流的工具
CodecVisa，StreamEye以及VM Analyzer
NALU是由NALU头和RBSP数据组成，而RBSP可能是SPS，PPS，Slice或SEI
而且SPS位于第一个NALU，PPS位于第二个NALU
另外值得说一下的就是从Headers Info拷贝出来的数据当中”na”就是未定义的，也就是if条件没有覆盖的情况。
sps
pic_width_in_mbs_minus1 = 21
pic_height_in_mbs_minus1 = 17
分别表示图像的宽和高，以宏块（16x16）为单位的值减1
因此，实际的宽为 (21+1)*16 = 352
以上是针对宽高是16的整数倍的情况，当不是16整数倍时，frame_cropping_flag值为1，frame_mbs_only_flag为1，公式如下：
// 宽高计算公式
width = (sps->pic_width_in_mbs_minus1+1) * 16;
height = (2 - sps->frame_mbs_only_flag)* (sps->pic_height_in_map_units_minus1 +1) * 16);
if(sps->frame_cropping_flag)
{
unsigned int crop_unit_x;
unsigned int crop_unit_y;
if (0 == sps->chroma_format_idc) // monochrome
{
crop_unit_x = 1;
crop_unit_y = 2 - sps->frame_mbs_only_flag;
}
else if (1 == sps->chroma_format_idc) // 4:2:0
{
crop_unit_x = 2;
crop_unit_y = 2 * (2 - sps->frame_mbs_only_flag);
}
else if (2 == sps->chroma_format_idc) // 4:2:2
{
crop_unit_x = 2;
crop_unit_y = 2 - sps->frame_mbs_only_flag;
}
else // 3 == sps.chroma_format_idc // 4:4:4
{
crop_unit_x = 1;
crop_unit_y = 2 - sps->frame_mbs_only_flag;
}
width -= crop_unit_x * (sps->frame_crop_left_offset + sps->frame_crop_right_offset);
height -= crop_unit_y * (sps->frame_crop_top_offset + sps->frame_crop_bottom_offset);
}

ff_h264_decode_seq_parameter_set
ff_h264_decode_picture_parameter_set
[https://cardinalpeak.com/blog/the-h-264-sequence-parameter-set/](https://cardinalpeak.com/blog/the-h-264-sequence-parameter-set/)
最好参考：H.264官方中文版.pdf7.3.2.1节对比查看
|Parameter Name|Type|Value|Comments|
|----|----|----|----|
|forbidden_zero_bit|u(1)|0|Despite being forbidden, it must be set to 0!|
|nal_ref_idc|u(2)|3|3 means it is “important” (this is an SPS)|
|nal_unit_type|u(5)|7|Indicates this is a sequence parameter set|
|||||
|profile_idc|u(8)|66|Baseline profile|
|||||
|constraint_set0_flag|u(1)|0|We’re not going to honor constraints|
|constraint_set1_flag|u(1)|0|We’re not going to honor constraints|
|constraint_set2_flag|u(1)|0|We’re not going to honor constraints|
|constraint_set3_flag|u(1)|0|We’re not going to honor constraints|
|reserved_zero_4bits|u(4)|0|Better set them to zero|
|||||
|level_idc|u(8)|10|Level 1, sec A.3.1|
|seq_parameter_set_id|ue(v)|0|We’ll just use id0.|
|log2_max_frame_num_minus4|ue(v)|0|Let’s have as few frame numbers as possible|
|pic_order_cnt_type|ue(v)|0|Keep things simple|
|log2_max_pic_order_cnt_lsb_minus4|ue(v)|0|Fewer is better.|
|num_ref_frames|ue(v)|0|We will only send I slices|
|gaps_in_frame_num_value_allowed_flag|u(1)|0|We will have no gaps|
|pic_width_in_mbs_minus_1|ue(v)|7|SQCIF is 8 macroblocks wide|
|pic_height_in_map_units_minus_1|ue(v)|5|SQCIF is 6 macroblocks high|
|frame_mbs_only_flag|u(1)|1|We will not to field/frame encoding|
|direct_8x8_inference_flag|u(1)|0|Used for B slices. We will not send B slices|
|frame_cropping_flag|u(1)|0|We will not do frame cropping|
|vui_prameters_present_flag|u(1)|0|We will not send VUI data|
|rbsp_stop_one_bit|u(1)|1|Stop bit. I missed this at first and it caused me much trouble.|
ff_h264_decode_seq_parameter_set解析：
`skip_bits(&h->gb, 2);`跳过两个位，表现为GetBitContext.index后移两个位置。
- 当前SPS的帧的宽 = (sps_info.pic_width_in_mbs_minus1 + 1) * 16
- 当前SPS的帧的高 = (sps_info.pic_height_in_map_units_minus1 + 1) * 16

extract the h.264 NAL units from the file using ffmpeg:
ffmpeg.exe -i Old Faithful.mp4 -vcodec copy -vbsf h264_mp4toannexb -an of.h264
获取帧率
https://github.com/leixiaohua1020/simplest_librtmp_example/blob/master/simplest_librtmp_send264/sps_decode.h 
fps=time_scale/(2*num_units_in_tick);
30/(2*1)=15fps 
[https://github.com/leixiaohua1020/h264_analysis](https://github.com/leixiaohua1020/h264_analysis)

参考：
[http://www.latelee.org/my-study/get-width-height-framerate-from-bitstream.html](http://www.latelee.org/my-study/get-width-height-framerate-from-bitstream.html)宽高计算公式
[](http://www.latelee.org/my-study/get-width-height-framerate-from-bitstream.html)
分析H.264码流的工具
CodecVisa，StreamEye以及VM Analyzer
NALU是由NALU头和RBSP数据组成，而RBSP可能是SPS，PPS，Slice或SEI
而且SPS位于第一个NALU，PPS位于第二个NALU
另外值得说一下的就是从Headers Info拷贝出来的数据当中”na”就是未定义的，也就是if条件没有覆盖的情况。
sps
pic_width_in_mbs_minus1 = 21
pic_height_in_mbs_minus1 = 17
分别表示图像的宽和高，以宏块（16x16）为单位的值减1
因此，实际的宽为 (21+1)*16 = 352
以上是针对宽高是16的整数倍的情况，当不是16整数倍时，frame_cropping_flag值为1，frame_mbs_only_flag为1，公式如下：
// 宽高计算公式
width = (sps->pic_width_in_mbs_minus1+1) * 16;
height = (2 - sps->frame_mbs_only_flag)* (sps->pic_height_in_map_units_minus1 +1) * 16);
if(sps->frame_cropping_flag)
{
unsigned int crop_unit_x;
unsigned int crop_unit_y;
if (0 == sps->chroma_format_idc) // monochrome
{
crop_unit_x = 1;
crop_unit_y = 2 - sps->frame_mbs_only_flag;
}
else if (1 == sps->chroma_format_idc) // 4:2:0
{
crop_unit_x = 2;
crop_unit_y = 2 * (2 - sps->frame_mbs_only_flag);
}
else if (2 == sps->chroma_format_idc) // 4:2:2
{
crop_unit_x = 2;
crop_unit_y = 2 - sps->frame_mbs_only_flag;
}
else // 3 == sps.chroma_format_idc // 4:4:4
{
crop_unit_x = 1;
crop_unit_y = 2 - sps->frame_mbs_only_flag;
}
width -= crop_unit_x * (sps->frame_crop_left_offset + sps->frame_crop_right_offset);
height -= crop_unit_y * (sps->frame_crop_top_offset + sps->frame_crop_bottom_offset);
}

ff_h264_decode_seq_parameter_set
ff_h264_decode_picture_parameter_set
[https://cardinalpeak.com/blog/the-h-264-sequence-parameter-set/](https://cardinalpeak.com/blog/the-h-264-sequence-parameter-set/)
最好参考：H.264官方中文版.pdf7.3.2.1节对比查看
|**Parameter Name**|**Type**|**Value**|**Comments**|
|----|----|----|----|
|forbidden_zero_bit|u(1)|0|Despite being forbidden, it must be set to 0!|
|nal_ref_idc|u(2)|3|3 means it is “important” (this is an SPS)|
|nal_unit_type|u(5)|7|Indicates this is a sequence parameter set|
|||||
|profile_idc|u(8)|66|Baseline profile|
|||||
|constraint_set0_flag|u(1)|0|We’re not going to honor constraints|
|constraint_set1_flag|u(1)|0|We’re not going to honor constraints|
|constraint_set2_flag|u(1)|0|We’re not going to honor constraints|
|constraint_set3_flag|u(1)|0|We’re not going to honor constraints|
|reserved_zero_4bits|u(4)|0|Better set them to zero|
|||||
|level_idc|u(8)|10|Level 1, sec A.3.1|
|seq_parameter_set_id|ue(v)|0|We’ll just use id0.|
|log2_max_frame_num_minus4|ue(v)|0|Let’s have as few frame numbers as possible|
|pic_order_cnt_type|ue(v)|0|Keep things simple|
|log2_max_pic_order_cnt_lsb_minus4|ue(v)|0|Fewer is better.|
|num_ref_frames|ue(v)|0|We will only send I slices|
|gaps_in_frame_num_value_allowed_flag|u(1)|0|We will have no gaps|
|pic_width_in_mbs_minus_1|ue(v)|7|SQCIF is 8 macroblocks wide|
|pic_height_in_map_units_minus_1|ue(v)|5|SQCIF is 6 macroblocks high|
|frame_mbs_only_flag|u(1)|1|We will not to field/frame encoding|
|direct_8x8_inference_flag|u(1)|0|Used for B slices. We will not send B slices|
|frame_cropping_flag|u(1)|0|We will not do frame cropping|
|vui_prameters_present_flag|u(1)|0|We will not send VUI data|
|rbsp_stop_one_bit|u(1)|1|Stop bit. I missed this at first and it caused me much trouble.|
ff_h264_decode_seq_parameter_set解析：
`skip_bits(&h->gb, 2);`跳过两个位，表现为GetBitContext.index后移两个位置。
- 当前SPS的帧的宽 = (sps_info.pic_width_in_mbs_minus1 + 1) * 16
- 当前SPS的帧的高 = (sps_info.pic_height_in_map_units_minus1 + 1) * 16

extract the h.264 NAL units from the file using ffmpeg:
ffmpeg.exe -i Old Faithful.mp4 -vcodec copy -vbsf h264_mp4toannexb -an of.h264
获取帧率
https://github.com/leixiaohua1020/simplest_librtmp_example/blob/master/simplest_librtmp_send264/sps_decode.h 
fps=time_scale/(2*num_units_in_tick);
30/(2*1)=15fps 
[https://github.com/leixiaohua1020/h264_analysis](https://github.com/leixiaohua1020/h264_analysis)

参考：
[http://www.latelee.org/my-study/get-width-height-framerate-from-bitstream.html](http://www.latelee.org/my-study/get-width-height-framerate-from-bitstream.html)宽高计算公式
[](http://www.latelee.org/my-study/get-width-height-framerate-from-bitstream.html)
