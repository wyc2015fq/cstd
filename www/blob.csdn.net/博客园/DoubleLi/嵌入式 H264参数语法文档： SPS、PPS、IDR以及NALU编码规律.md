# 嵌入式 H264参数语法文档： SPS、PPS、IDR以及NALU编码规律 - DoubleLi - 博客园






**// 【h264编码出的NALU规律】// 第一帧 SPS【0 0 0 1 0x67】 PPS【0 0 0 1 0x68】 SEI【0 0 0 1 0x6】 IDR【0 0 0 1 0x65】// p帧      P【0 0 0 1 0x61】// I帧    SPS【0 0 0 1 0x67】 PPS【0 0 0 1 0x68】 IDR【0 0 0 1 0x65】// 【mp4v2封装函数MP4WriteSample】// 此函数接收I/P nalu,该nalu需要用4字节的数据大小头替换原有的起始头，并且数据大小为big-endian格式**

**示例文件下载地址：“[NALU中SPS、PPS、SEI、IDR、P帧标识](http://download.csdn.net/detail/skdkjxy/7994069)”**

**H.264码流第一个 NALU 是 SPS（序列参数集Sequence Parameter Set）对应H264标准文档 7.3.2.1 序列参数集的语法进行解析**

```
![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)
**SPS参数解析**// fill sps with content of p





**[cpp]** [view plain](http://blog.csdn.net/heanyu/article/details/6205390)[copy](http://blog.csdn.net/heanyu/article/details/6205390)

 






- int InterpretSPS (VideoParameters *p_Vid, DataPartition *p, seq_parameter_set_rbsp_t *sps)  

- {  

-   unsigned i;  

-   unsigned n_ScalingList;  

-   int reserved_zero;  

-   Bitstream *s = p->bitstream;  

-   

-   assert (p != NULL);  

-   assert (p->bitstream != NULL);  

-   assert (p->bitstream->streamBuffer != 0);  

-   assert (sps != NULL);  

-   

-   p_Dec->UsedBits = 0;  

-   

-   sps->profile_idc                            = u_v  (8, "SPS: profile_idc"                           , s);  

-   

-   if ((sps->profile_idc!=BASELINE       ) &&  

-       (sps->profile_idc!=MAIN           ) &&  

-       (sps->profile_idc!=EXTENDED       ) &&  

-       (sps->profile_idc!=FREXT_HP       ) &&  

-       (sps->profile_idc!=FREXT_Hi10P    ) &&  

-       (sps->profile_idc!=FREXT_Hi422    ) &&  

-       (sps->profile_idc!=FREXT_Hi444    ) &&  

-       (sps->profile_idc!=FREXT_CAVLC444 )  

- #if (MVC_EXTENSION_ENABLE)  

-       && (sps->profile_idc!=MVC_HIGH)  

-       && (sps->profile_idc!=STEREO_HIGH)  

- #endif  

-       )  

-   {  

-     printf("Invalid Profile IDC (%d) encountered. /n", sps->profile_idc);  

-     return p_Dec->UsedBits;  

-   }  

-   

-   sps->constrained_set0_flag                  = u_1  (   "SPS: constrained_set0_flag"                 , s);  

-   sps->constrained_set1_flag                  = u_1  (   "SPS: constrained_set1_flag"                 , s);  

-   sps->constrained_set2_flag                  = u_1  (   "SPS: constrained_set2_flag"                 , s);  

-   sps->constrained_set3_flag                  = u_1  (   "SPS: constrained_set3_flag"                 , s);  

- #if (MVC_EXTENSION_ENABLE)  

-   sps->constrained_set4_flag                  = u_1  (   "SPS: constrained_set4_flag"                 , s);  

-   reserved_zero                               = u_v  (3, "SPS: reserved_zero_3bits"                   , s);  

- #else  

-   reserved_zero                               = u_v  (4, "SPS: reserved_zero_4bits"                   , s);  

- #endif  

-   assert (reserved_zero==0);  

-   

-   sps->level_idc                              = u_v  (8, "SPS: level_idc"                             , s);  

-   

-   sps->seq_parameter_set_id                   = ue_v ("SPS: seq_parameter_set_id"                     , s);  

-   

-   // Fidelity Range Extensions stuff  

-   sps->chroma_format_idc = 1;  

-   sps->bit_depth_luma_minus8   = 0;  

-   sps->bit_depth_chroma_minus8 = 0;  

-   p_Vid->lossless_qpprime_flag   = 0;  

-   sps->separate_colour_plane_flag = 0;  

-   

-   if((sps->profile_idc==FREXT_HP   ) ||  

-      (sps->profile_idc==FREXT_Hi10P) ||  

-      (sps->profile_idc==FREXT_Hi422) ||  

-      (sps->profile_idc==FREXT_Hi444) ||  

-      (sps->profile_idc==FREXT_CAVLC444)  

- #if (MVC_EXTENSION_ENABLE)  

-      || (sps->profile_idc==MVC_HIGH)  

-      || (sps->profile_idc==STEREO_HIGH)  

- #endif  

-      )  

-   {  

-     sps->chroma_format_idc                      = ue_v ("SPS: chroma_format_idc"                       , s);  

-   

-     if(sps->chroma_format_idc == YUV444)  

-     {  

-       sps->separate_colour_plane_flag           = u_1  ("SPS: separate_colour_plane_flag"              , s);  

-     }  

-   

-     sps->bit_depth_luma_minus8                  = ue_v ("SPS: bit_depth_luma_minus8"                   , s);  

-     sps->bit_depth_chroma_minus8                = ue_v ("SPS: bit_depth_chroma_minus8"                 , s);  

-     //checking;  

-     if((sps->bit_depth_luma_minus8+8 > sizeof(imgpel)*8) || (sps->bit_depth_chroma_minus8+8> sizeof(imgpel)*8))  

-       error ("Source picture has higher bit depth than imgpel data type. /nPlease recompile with larger data type for imgpel.", 500);  

-   

-     p_Vid->lossless_qpprime_flag                  = u_1  ("SPS: lossless_qpprime_y_zero_flag"            , s);  

-   

-     sps->seq_scaling_matrix_present_flag        = u_1  (   "SPS: seq_scaling_matrix_present_flag"       , s);  

-       

-     if(sps->seq_scaling_matrix_present_flag)  

-     {  

-       n_ScalingList = (sps->chroma_format_idc != YUV444) ? 8 : 12;  

-       for(i=0; iseq_scaling_list_present_flag[i]   = u_1  (   "SPS: seq_scaling_list_present_flag"         , s);  

-         if(sps->seq_scaling_list_present_flag[i])  

-         {  

-           if(i<6) scaling_list="">ScalingList4x4[i], 16, &sps->UseDefaultScalingMatrix4x4Flag[i], s);  

-           else  

-             Scaling_List(sps->ScalingList8x8[i-6], 64, &sps->UseDefaultScalingMatrix8x8Flag[i-6], s);  

-         }  

-       }  

-     }  

-   }  

-   

-   sps->log2_max_frame_num_minus4              = ue_v ("SPS: log2_max_frame_num_minus4"                , s);  

-   sps->pic_order_cnt_type                     = ue_v ("SPS: pic_order_cnt_type"                       , s);  

-   

-   if (sps->pic_order_cnt_type == 0)  

-     sps->log2_max_pic_order_cnt_lsb_minus4 = ue_v ("SPS: log2_max_pic_order_cnt_lsb_minus4"           , s);  

-   else if (sps->pic_order_cnt_type == 1)  

-   {  

-     sps->delta_pic_order_always_zero_flag      = u_1  ("SPS: delta_pic_order_always_zero_flag"       , s);  

-     sps->offset_for_non_ref_pic                = se_v ("SPS: offset_for_non_ref_pic"                 , s);  

-     sps->offset_for_top_to_bottom_field        = se_v ("SPS: offset_for_top_to_bottom_field"         , s);  

-     sps->num_ref_frames_in_pic_order_cnt_cycle = ue_v ("SPS: num_ref_frames_in_pic_order_cnt_cycle"  , s);  

-     for(i=0; inum_ref_frames_in_pic_order_cnt_cycle; i++)  

-       sps->offset_for_ref_frame[i]               = se_v ("SPS: offset_for_ref_frame[i]"              , s);  

-   }  

-   sps->num_ref_frames                        = ue_v ("SPS: num_ref_frames"                         , s);  

-   sps->gaps_in_frame_num_value_allowed_flag  = u_1  ("SPS: gaps_in_frame_num_value_allowed_flag"   , s);  

-   sps->pic_width_in_mbs_minus1               = ue_v ("SPS: pic_width_in_mbs_minus1"                , s);  

-   sps->pic_height_in_map_units_minus1        = ue_v ("SPS: pic_height_in_map_units_minus1"         , s);  

-   sps->frame_mbs_only_flag                   = u_1  ("SPS: frame_mbs_only_flag"                    , s);  

-   if (!sps->frame_mbs_only_flag)  

-   {  

-     sps->mb_adaptive_frame_field_flag        = u_1  ("SPS: mb_adaptive_frame_field_flag"           , s);  

-   }  

-   sps->direct_8x8_inference_flag             = u_1  ("SPS: direct_8x8_inference_flag"              , s);  

-   sps->frame_cropping_flag                   = u_1  ("SPS: frame_cropping_flag"                    , s);  

-   

-   if (sps->frame_cropping_flag)  

-   {  

-     sps->frame_cropping_rect_left_offset      = ue_v ("SPS: frame_cropping_rect_left_offset"           , s);  

-     sps->frame_cropping_rect_right_offset     = ue_v ("SPS: frame_cropping_rect_right_offset"          , s);  

-     sps->frame_cropping_rect_top_offset       = ue_v ("SPS: frame_cropping_rect_top_offset"            , s);  

-     sps->frame_cropping_rect_bottom_offset    = ue_v ("SPS: frame_cropping_rect_bottom_offset"         , s);  

-   }  

-   sps->vui_parameters_present_flag           = (Boolean) u_1  ("SPS: vui_parameters_present_flag"      , s);  

-   

-   InitVUI(sps);  

-   ReadVUI(p, sps);  

-   

-   sps->Valid = TRUE;  

-   return p_Dec->UsedBits;  

- }
```

H.264码流第二个 NALU 是 PPS（图像参数集Picture Parameter Set）
对应H264标准文档 7.3.2.2 序列参数集的语法进行解析

```
![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)
**PPS参数解析**





**[cpp]** [view plain](http://blog.csdn.net/heanyu/article/details/6205390)[copy](http://blog.csdn.net/heanyu/article/details/6205390)

 






- int InterpretPPS (VideoParameters *p_Vid, DataPartition *p, pic_parameter_set_rbsp_t *pps)  

- {  

-   unsigned i;  

-   unsigned n_ScalingList;  

-   int chroma_format_idc;  

-   int NumberBitsPerSliceGroupId;  

-   Bitstream *s = p->bitstream;  

-   

-   assert (p != NULL);  

-   assert (p->bitstream != NULL);  

-   assert (p->bitstream->streamBuffer != 0);  

-   assert (pps != NULL);  

-   

-   p_Dec->UsedBits = 0;  

-   

-   pps->pic_parameter_set_id                  = ue_v ("PPS: pic_parameter_set_id"                   , s);  

-   pps->seq_parameter_set_id                  = ue_v ("PPS: seq_parameter_set_id"                   , s);  

-   pps->entropy_coding_mode_flag              = u_1  ("PPS: entropy_coding_mode_flag"               , s);  

-   

-   //! Note: as per JVT-F078 the following bit is unconditional.  If F078 is not accepted, then  

-   //! one has to fetch the correct SPS to check whether the bit is present (hopefully there is  

-   //! no consistency problem :-(  

-   //! The current encoder code handles this in the same way.  When you change this, don't forget  

-   //! the encoder!  StW, 12/8/02  

-   pps->bottom_field_pic_order_in_frame_present_flag                = u_1  ("PPS: bottom_field_pic_order_in_frame_present_flag"                 , s);  

-   

-   pps->num_slice_groups_minus1               = ue_v ("PPS: num_slice_groups_minus1"                , s);  

-   

-   // FMO stuff begins here  

-   if (pps->num_slice_groups_minus1 > 0)  

-   {  

-     pps->slice_group_map_type               = ue_v ("PPS: slice_group_map_type"                , s);  

-     if (pps->slice_group_map_type == 0)  

-     {  

-       for (i=0; i<=pps->num_slice_groups_minus1; i++)  

-         pps->run_length_minus1 [i]                  = ue_v ("PPS: run_length_minus1 [i]"              , s);  

-     }  

-     else if (pps->slice_group_map_type == 2)  

-     {  

-       for (i=0; inum_slice_groups_minus1; i++)  

-       {  

-         //! JVT-F078: avoid reference of SPS by using ue(v) instead of u(v)  

-         pps->top_left [i]                          = ue_v ("PPS: top_left [i]"                        , s);  

-         pps->bottom_right [i]                      = ue_v ("PPS: bottom_right [i]"                    , s);  

-       }  

-     }  

-     else if (pps->slice_group_map_type == 3 ||  

-              pps->slice_group_map_type == 4 ||  

-              pps->slice_group_map_type == 5)  

-     {  

-       pps->slice_group_change_direction_flag     = u_1  ("PPS: slice_group_change_direction_flag"      , s);  

-       pps->slice_group_change_rate_minus1        = ue_v ("PPS: slice_group_change_rate_minus1"         , s);  

-     }  

-     else if (pps->slice_group_map_type == 6)  

-     {  

-       if (pps->num_slice_groups_minus1+1 >4)  

-         NumberBitsPerSliceGroupId = 3;  

-       else if (pps->num_slice_groups_minus1+1 > 2)  

-         NumberBitsPerSliceGroupId = 2;  

-       else  

-         NumberBitsPerSliceGroupId = 1;  

-       pps->pic_size_in_map_units_minus1      = ue_v ("PPS: pic_size_in_map_units_minus1"               , s);  

-       if ((pps->slice_group_id = calloc (pps->pic_size_in_map_units_minus1+1, 1)) == NULL)  

-         no_mem_exit ("InterpretPPS: slice_group_id");  

-       for (i=0; i<=pps->pic_size_in_map_units_minus1; i++)  

-         pps->slice_group_id[i] = (byte) u_v (NumberBitsPerSliceGroupId, "slice_group_id[i]", s);  

-     }  

-   }  

-   

-   // End of FMO stuff  

-   

-   pps->num_ref_idx_l0_active_minus1          = ue_v ("PPS: num_ref_idx_l0_active_minus1"           , s);  

-   pps->num_ref_idx_l1_active_minus1          = ue_v ("PPS: num_ref_idx_l1_active_minus1"           , s);  

-   pps->weighted_pred_flag                    = u_1  ("PPS: weighted_pred_flag"                     , s);  

-   pps->weighted_bipred_idc                   = u_v  ( 2, "PPS: weighted_bipred_idc"                , s);  

-   pps->pic_init_qp_minus26                   = se_v ("PPS: pic_init_qp_minus26"                    , s);  

-   pps->pic_init_qs_minus26                   = se_v ("PPS: pic_init_qs_minus26"                    , s);  

-   

-   pps->chroma_qp_index_offset                = se_v ("PPS: chroma_qp_index_offset"                 , s);  

-   

-   pps->deblocking_filter_control_present_flag = u_1 ("PPS: deblocking_filter_control_present_flag" , s);  

-   pps->constrained_intra_pred_flag           = u_1  ("PPS: constrained_intra_pred_flag"            , s);  

-   pps->redundant_pic_cnt_present_flag        = u_1  ("PPS: redundant_pic_cnt_present_flag"         , s);  

-   

-   if(more_rbsp_data(s->streamBuffer, s->frame_bitoffset,s->bitstream_length)) // more_data_in_rbsp()  

-   {  

-     //Fidelity Range Extensions Stuff  

-     pps->transform_8x8_mode_flag           = u_1  ("PPS: transform_8x8_mode_flag"                , s);  

-     pps->pic_scaling_matrix_present_flag   =  u_1  ("PPS: pic_scaling_matrix_present_flag"        , s);  

-   

-     if(pps->pic_scaling_matrix_present_flag)  

-     {  

-       chroma_format_idc = p_Vid->SeqParSet[pps->seq_parameter_set_id].chroma_format_idc;  

-       n_ScalingList = 6 + ((chroma_format_idc != YUV444) ? 2 : 6) * pps->transform_8x8_mode_flag;  

-       for(i=0; ipic_scaling_list_present_flag[i]= u_1  ("PPS: pic_scaling_list_present_flag"          , s);  

-   

-         if(pps->pic_scaling_list_present_flag[i])  

-         {  

-           if(i<6) scaling_list="">ScalingList4x4[i], 16, &pps->UseDefaultScalingMatrix4x4Flag[i], s);  

-           else  

-             Scaling_List(pps->ScalingList8x8[i-6], 64, &pps->UseDefaultScalingMatrix8x8Flag[i-6], s);  

-         }  

-       }  

-     }  

-     pps->second_chroma_qp_index_offset      = se_v ("PPS: second_chroma_qp_index_offset"          , s);  

-   }  

-   else  

-   {  

-     pps->second_chroma_qp_index_offset      = pps->chroma_qp_index_offset;  

-   }  

-   

-   pps->Valid = TRUE;  

-   return p_Dec->UsedBits;  

- }
```

H.264码流第三个 NALU 是 IDR（即时解码器刷新）
对应H264标准文档 7.3.3 序列参数集的语法进行解析

```
![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)
IDR参数解析





**[cpp]** [view plain](http://blog.csdn.net/heanyu/article/details/6205390)[copy](http://blog.csdn.net/heanyu/article/details/6205390)

 






- case NALU_TYPE_IDR:  

-         img->idr_flag = (nalu->nal_unit_type == NALU_TYPE_IDR);  

-         img->nal_reference_idc = nalu->nal_reference_idc;  

-         img->disposable_flag = (nalu->nal_reference_idc == NALU_PRIORITY_DISPOSABLE);  

-         currSlice->dp_mode = PAR_DP_1;   //++ dp_mode：数据分割模式；PAR_DP_1=0：没有数据分割  

-         currSlice->max_part_nr = 1;  

-         currSlice->ei_flag = 0;  //++ 该处赋值直接影响decode_slice()函数中对decode_one_slice()函数的调用  

-                                     //++ 该值不为0，表明当前片出错，解码程序将忽略当前片的解码过程，而使用错误隐藏  

-         currStream = currSlice->partArr[0].bitstream;  

-         currStream->ei_flag = 0; //++ 此处的赋值为最终赋值，以后不再改变。该值将对每个宏块的ei_flag产生影响  

-                                     //++ 参见macroblock.c文件read_one_macroblock()函数的如下语句：  

-                                     //++        :if(!dP->bitstream->ei_flag)      :currMB->ei_flag = 0;  

-                                     //++ 该值还在macroblock.c文件if(IS_INTRA (currMB) && dP->bitstream->ei_flag && img->number)中用到  

-         currStream->frame_bitoffset = currStream->read_len = 0;  

-         memcpy (currStream->streamBuffer, &nalu->buf[1], nalu->len-1);  

-         currStream->code_len = currStream->bitstream_length = RBSPtoSODB(currStream->streamBuffer, nalu->len-1);  

-   

-         // Some syntax of the Slice Header depends on the parameter set, which depends on  

-         // the parameter set ID of the SLice header.  Hence, read the pic_parameter_set_id  

-         // of the slice header first, then setup the active parameter sets, and then read  

-         // the rest of the slice header  

-         BitsUsedByHeader = FirstPartOfSliceHeader();    //++ 参见标准7.3.3  

-         UseParameterSet (currSlice->pic_parameter_set_id);  

-         BitsUsedByHeader+= RestOfSliceHeader ();    //++ 参见标准7.3.3  

-         //++ BitsUsedByHeader在程序中没有实际用处，而且BitsUsedByHeader+= RestOfSliceHeader ()  

-         //++ 重复计算了FirstPartOfSliceHeader()所用到的比特数。因为在FirstPartOfSliceHeader()  

-         //++ 之后，变量UsedBits值并未被置零就代入RestOfSliceHeader()运算，从而RestOfSliceHeader ()  

-         //++ 在返回时，BitsUsedByHeader+= RestOfSliceHeader()多加了一个BitsUsedByHeader值  

-   

-         FmoInit (active_pps, active_sps);  

-   

-         if(is_new_picture())  

-         {  

-           init_picture(img, input);  

-             

-           current_header = SOP;  

-           //check zero_byte if it is also the first NAL unit in the access unit  

-           CheckZeroByteVCL(nalu, &ret);  

-         }  

-         else  

-           current_header = SOS;  

-     

-         init_lists(img->type, img->currentSlice->structure);  

-         reorder_lists (img->type, img->currentSlice);  

-   

-         if (img->structure==FRAME)  

-         {  

-           init_mbaff_lists();  

-         }  

-   

- /*        if (img->frame_num==1) // write a reference list 

-         { 

-           count ++; 

-           if (count==1) 

-             for (i=0; i 

-  

-         // From here on, active_sps, active_pps and the slice header are valid 

-         if (img->MbaffFrameFlag) 

-           img->current_mb_nr = currSlice->start_mb_nr << 1="" style="color: #0000ff">else 

-           img->current_mb_nr = currSlice->start_mb_nr; 

-  

-         if (active_pps->entropy_coding_mode_flag) 

-         { 

-           int ByteStartPosition = currStream->frame_bitoffset/8; 

-           if (currStream->frame_bitoffset%8 != 0)  

-           { 

-             ByteStartPosition++; 

-           } 

-           arideco_start_decoding (&currSlice->partArr[0].de_cabac, currStream->streamBuffer, ByteStartPosition, &currStream->read_len, img->type); 

-         } 

- // printf ("read_new_slice: returning %s/n", current_header == SOP?"SOP":"SOS"); 

-         FreeNALU(nalu); 

-         return current_header; 

-         break; 

-       case NALU_TYPE_DPA: 

-         //! The state machine here should follow the same ideas as the old readSliceRTP() 

-         //! basically: 

-         //! work on DPA (as above) 

-         //! read and process all following SEI/SPS/PPS/PD/Filler NALUs 

-         //! if next video NALU is dpB,  

-         //!   then read and check whether it belongs to DPA, if yes, use it 

-         //! else 

-         //!   ;   // nothing 

-         //! read and process all following SEI/SPS/PPS/PD/Filler NALUs 

-         //! if next video NALU is dpC 

-         //!   then read and check whether it belongs to DPA (and DPB, if present), if yes, use it, done 

-         //! else 

-         //!   use the DPA (and the DPB if present) 

-  

-         /*  

-             LC: inserting the code related to DP processing, mainly copying some of the parts 

-             related to NALU_TYPE_SLICE, NALU_TYPE_IDR. 

-         */  

-   

-         if(expected_slice_type != NALU_TYPE_DPA)  

-         {  

-           /* oops... we found the next slice, go back! */  

-           fseek(bits, ftell_position, SEEK_SET);  

-           FreeNALU(nalu);  

-           return current_header;  

-         }  

-   

-         img->idr_flag          = (nalu->nal_unit_type == NALU_TYPE_IDR);  

-         img->nal_reference_idc = nalu->nal_reference_idc;  

-         img->disposable_flag   = (nalu->nal_reference_idc == NALU_PRIORITY_DISPOSABLE);  

-         currSlice->dp_mode     = PAR_DP_3;  

-         currSlice->max_part_nr = 3;  

-         currSlice->ei_flag     = 0;  

-         currStream             = currSlice->partArr[0].bitstream;  

-         currStream->ei_flag    = 0;  

-         currStream->frame_bitoffset = currStream->read_len = 0;  

-         memcpy (currStream->streamBuffer, &nalu->buf[1], nalu->len-1);  

-         currStream->code_len = currStream->bitstream_length = RBSPtoSODB(currStream->streamBuffer, nalu->len-1);    //++ 剔除停止比特和填充比特  

-           

-         BitsUsedByHeader     = FirstPartOfSliceHeader();  

-         UseParameterSet (currSlice->pic_parameter_set_id);  

-         BitsUsedByHeader    += RestOfSliceHeader ();  

-           

-         FmoInit (active_pps, active_sps);  

-   

-         if(is_new_picture())  

-         {  

-           init_picture(img, input);  

-           current_header = SOP;  

-           CheckZeroByteVCL(nalu, &ret);  

-         }  

-         else  

-           current_header = SOS;  

-   

-           

-         init_lists(img->type, img->currentSlice->structure);  

-         reorder_lists (img->type, img->currentSlice);  

-           

-         if (img->structure==FRAME)  

-         {  

-           init_mbaff_lists();  

-         }  

-   

-         // From here on, active_sps, active_pps and the slice header are valid  

-         if (img->MbaffFrameFlag)  

-           img->current_mb_nr = currSlice->start_mb_nr << 1="" style="color: #0000ff">else  

-    

-          img->current_mb_nr = currSlice->start_mb_nr;  

-   

-   

-         /*  

-            LC: 

-               Now I need to read the slice ID, which depends on the value of  

-               redundant_pic_cnt_present_flag (pag.49).  

-         */  

-   

-         slice_id_a  = ue_v("NALU:SLICE_A slice_idr", currStream);  

-         if (active_pps->entropy_coding_mode_flag)  

-         {  

-           int ByteStartPosition = currStream->frame_bitoffset/8;  

-           if (currStream->frame_bitoffset%8 != 0)   

-           {  

-             ByteStartPosition++;  

-           }  

-           arideco_start_decoding (&currSlice->partArr[0].de_cabac, currStream->streamBuffer, ByteStartPosition, &currStream->read_len, img->type);  

-         }  

- // printf ("read_new_slice: returning %s/n", current_header == SOP?"SOP":"SOS");  

-         break;  

-       case NALU_TYPE_DPB:  

-         /* LC: inserting the code related to DP processing */  

-   

-         currStream             = currSlice->partArr[1].bitstream;  

-         currStream->ei_flag    = 0;  

-         currStream->frame_bitoffset = currStream->read_len = 0;  

-         memcpy (currStream->streamBuffer, &nalu->buf[1], nalu->len-1);  

-         currStream->code_len = currStream->bitstream_length = RBSPtoSODB(currStream->streamBuffer, nalu->len-1);  

-   

-         slice_id_b  = ue_v("NALU:SLICE_B slice_idr", currStream);  

-         if (active_pps->redundant_pic_cnt_present_flag)  

-           redundant_pic_cnt_b = ue_v("NALU:SLICE_B redudand_pic_cnt", currStream);  

-         else  

-           redundant_pic_cnt_b = 0;  

-           

-         /*  LC: Initializing CABAC for the current data stream. */  

-   

-         if (active_pps->entropy_coding_mode_flag)  

-         {  

-           int ByteStartPosition = currStream->frame_bitoffset/8;  

-           if (currStream->frame_bitoffset % 8 != 0)   

-             ByteStartPosition++;  

-             

-           arideco_start_decoding (&currSlice->partArr[1].de_cabac, currStream->streamBuffer,   

-             ByteStartPosition, &currStream->read_len, img->type);  

-             

-         }  

-   

-         /* LC: resilience code to be inserted */  

-         /*         FreeNALU(nalu); */  

-         /*         return current_header; */  

-   

-         break;
```

```
![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)
IDR参数解析/*!





**[cpp]** [view plain](http://blog.csdn.net/heanyu/article/details/6205390)[copy](http://blog.csdn.net/heanyu/article/details/6205390)

 






- <pre name="code" class="cpp"> ************************************************************************  

-  * /brief  

-  *    read the first part of the header (only the pic_parameter_set_id)  

-  * /return  

-  *    Length of the first part of the slice header (in bits)  

-  ************************************************************************  

-  */  

- //++ 参见标准7.3.3  

- int FirstPartOfSliceHeader()  

- {  

-   Slice *currSlice = img->currentSlice;  

-   int dP_nr = assignSE2partition[currSlice->dp_mode][SE_HEADER];  

-   DataPartition *partition = &(currSlice->partArr[dP_nr]);  

-   Bitstream *currStream = partition->bitstream;  

-   int tmp;  

-   

-   UsedBits= partition->bitstream->frame_bitoffset; // was hardcoded to 31 for previous start-code. This is better.  

-   

-   // Get first_mb_in_slice  

-   currSlice->start_mb_nr = ue_v ("SH: first_mb_in_slice", currStream);  

-   

-   tmp = ue_v ("SH: slice_type", currStream);  

-     

-   if (tmp>4) tmp -=5;  

-   

-   img->type = currSlice->picture_type = (SliceType) tmp;  

-   

-   currSlice->pic_parameter_set_id = ue_v ("SH: pic_parameter_set_id", currStream);  

-     

-   return UsedBits;  

- }  

- </pre><br><br>
```









