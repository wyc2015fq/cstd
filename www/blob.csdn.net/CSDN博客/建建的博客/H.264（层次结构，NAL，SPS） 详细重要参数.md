# H.264（层次结构，NAL，SPS） 详细重要参数 - 建建的博客 - CSDN博客
2017年03月27日 16:59:32[纪建](https://me.csdn.net/u013898698)阅读数：259
个人分类：[视频编解码学习](https://blog.csdn.net/u013898698/article/category/6703370)
## 一 H.264句法
### 1.1元素分层结构
H.264编码器输出的Bit流中，每个Bit都隶属于某个句法元素。句法元素被组织成有层次的结构，分别描述各个层次的信息。
![](http://hi.csdn.net/attachment/201008/18/0_1282095853LJjW.gif)
图1
H.264分层结构由五层组成，分别是序列参数集、图像参数集、片（Slice）、和宏块和子块。参数集是一个独立的数据单位，不依赖于参数集外的其它句法元素。图2描述了参数集与参数集外的句法元素之间的关系。
![](http://hi.csdn.net/attachment/201008/17/0_1282036123wA4M.gif)
图2
一个参数集不对应某一个特定的图像或序列，同一序列参数集可以被多个图像参数集引用，同理，同一个图像参数集也可以被多个图像引用。只在编码器认为需要更新参数集的内容时，才会发出新的参数集。
在H.264中，图像以序列为单位进行组织。一个序列的第一个图像叫做IDR图像，IDR图像都是I帧，H.264引入IDR图像为了解码的同步，当解码器解码到IDR图像时，立即将参考帧队列清空，将已解码的数据全部输出或抛弃，重新查找参数集，开始一个新的序列。这样，如果前一个序列出现重大错误，在这里可以获得重新同步的机会。IDR图像之后的图像永远不会使用IDR之前的图像的数据来解码。
IDR是I帧，但I帧不一定是IDR。I帧之后的图像有可能会使用I帧之前的图像做运动参考。
### 1.2描述子
描述子描述从Bit流中取出句法元素的方法。
|编号|语法|说明|
|----|----|----|
|1|ae（e）|CABAC|
|2|b（8）|读进连续的8个Bit|
|3|ce（v）|CAVLC|
|4|f（n）|读进连续的n个Bit|
|5|i（n）/i（v）|读进连续的若干Bit，并把它们解释为有符号整数|
|6|me（v）|映射指数Golomb熵编码|
|7|se（v）|有符号指数Golomb熵编码|
|8|te（v）|截断指数Golomb熵编码|
|9|u（n）/u（v）|读进连续的若干Bit，并把它们解释为无符号整数|
|10|ue（v）|无符号指数Golomb熵编码|
表1
### 1.3句法的表示方法
句法元素的名称由小写字母和一系列下划线组成，变量名称是大小写字母组成，中间没有下划线。
## 二 句法表
定义了H.264的句法，指明在码流中依次出现的句法元素及它们出现的条件、提取描述子等。句法表是分层嵌套的。
句法表中的C字段表示该句法元素的分类，这是为片区服务，分类的具体含义如下表描述。
|**nal_unit_type**|**NAL类型**|**C**|
|----|----|----|
|0|未使用||
|1|不分区、非IDR的片|2,3,4|
|2|片分区A|2|
|3|片分区B|3|
|4|版分区C|4|
|5|IDR图像中的片|2,3|
|6|补充增强信息单元（SEI）|5|
|7|序列参数集|0|
|8|图像参数集|1|
|9|分界符|6|
|10|序列结束|7|
|11|码流结束|8|
|12|填充|9|
|13..23|保留||
|24..31|不保留||
表2
### 2.1 NAL语法
编码器将每个NAL各自独立、完整地放入一个分组，因为分组都有头部，解码器可以方便地检测出NAL的分界，并依次取出NAL进行解码。
每个NAL前有一个起始码 0x000001，解码器检测每个起始码，作为一个NAL的起始标识，当检测到下一个起始码时，当前NAL结束。同时H.264规定，当检测到0x000000时，也可以表征当前NAL的结束。对于NAL中数据出现0x000001或0x000000时，H.264引入了防止竞争机制，如果编码器检测到NAL数据存在0x000001或0x000000时，编码器会在最后个字节前插入一个新的字节0x03，这样：
0x000000－>0x00000300
0x000001－>0x00000301
0x000002－>0x00000302
0x000003－>0x00000303
解码器检测到0x000003时，把03抛弃，恢复原始数据。
解码器在解码时，首先逐个字节读取NAL的数据，统计NAL的长度，然后再开始解码。
|**句法**|**C**|**Desc**|
|----|----|----|
|nal_nuit(NumBytesInNALunit){/* NumBytesInNALunit为统计出来的数据长度 */|||
|forbidden_zero_bit /* 等于0 */|All|f(1)|
|nal_ref_idc/* 当前NAL的优先级，取值范围0-3 */|All|u(2)|
|nal_unit_type /* NAL类型，见表2描述 */|All|u(5)|
|NumBytesInRBSP=0|||
|for(i=1;i<NumBytesInNALunit;i++){|||
|if(i+2<NumBytesInNALunit && next_bits(24)==0x000003{|||
|/* 0x000003伪起始码，需要删除0x03这个字节 */|||
|rbsp_byte[NumBytesInRBSP++]|All|b(8)|
|rbsp_byte[NumBytesInRBSP++]|All|b(8)|
|i+=2/* 取出前两个0x00后，跳过0x03 */|||
|emulation_prevention_three_byte/* equal to 0x03 */|All|f(8)|
|}else{|||
|rbsp_byte[NumBytesInRBSP++] /* 继续读取后面的字节 */|All|b(8)|
|}|||
|}|||
表3
### 2.2序列参数集（SPS）
|**句法**|**C**|**Desc**|
|----|----|----|
|seq_parameter_set_rbsp(){|||
|profile_idc/* 指明所用的Profile */|0|u(8)|
|constraint_set0_flag|0|u(1)|
|constraint_set1_flag|0|u(1)|
|constraint_set1_flag|0|u(1)|
|reserved_zero_5bits /* equal to 0 */|0|u(5)|
|level_idc /* 指明所用的Level */|0|u(8)|
|seq_parameter_set_id /* 指明本序列参数集的id号，0-31，被图像集引用，编码需要产生新的序列集时，使用新的id，而不是改变原来参数集的内容 */|0|ue(v)|
|log2_max_frame_num_minus4/* 为读取元素frame_num服务，frame_num标识图像的解码顺序，frame_num的解码函数是ue(v)，其中v=log2_max_frame_num_minus4+4，该元素同时指明frame_num的最大值MaxFrameNum=2( log2_max_frame_num_minus4+4**)***/|0|ue(v)|
|pic_order_cnt_type /* 指明poc的编码方法，poc标识图像的播放顺序，poc可以由frame_num计算，也可以显示传送。poc共三种计算方式 */|0|ue(v)|
|if(pic_order_cnt_type==0)|||
|log2_max_pic_order_cnt_lsb_minus4 /* 指明变量MaxPicOrderCntLsb的值， MaxPicOrderCntLsb＝2(log2_max_pic_order_cnt_lsb_minus4+4) */|0|ue(v)|
|else if(pic_order_cnt_type==1){|||
|delta_pic_order_always_zero_flag /* 等于1时，元素delta_pic_order_cnt[0]和delta_pic_order_cnt[1]不在片头中出现，并且它们的默认值是0，等于0时，上述两元素出现的片头中 */|0|u(1)|
|offset_for_non_ref_pic /* 用来计算非参考帧或场的poc，[-231,231-1] */|0|se(v)|
|offset_for_top_to_bottom_field/* 计算帧的底场的poc */|0|se(v)|
|num_ref_frames_inpic_order_cnt_cycle /* 用来解码poc,[0.255] */|0|ue(v)|
|for(i=0;i<num_ref_frames_inpic_order_cnt_cycle;i++)|||
|offset_for_ref_frame[i]/* 用来解码poc，对于循环中的每个元素指定一个偏移 */|0|se(v)|
|}|||
|num_ref_frames /* 参考帧队列可达到的最大长度，[0,16] */|0|ue(v)|
|gaps_in_frame_num_value_allowed_flag /* 为1，允许slice header中的frame_num不连续 */|0|u(1)|
|pic_width_inmbs_minus1 /* 本元素加1，指明以宏块为单位的图像宽度PicWidthInMbs=pic_width_in_mbs_minus1+1 */|0|ue(v)|
|pic_height_in_map_units_minus1 /* 本元素加1，指明以宏块为单位的图像高宽度PicHeightInMapUnitsMbs=pic_height_in_map_units_minus1+1 */|0|ue(v)|
|frame_mbs_only_flag /* 等于0表示本序列中所有图像均为帧编码；等于1，表示可能是帧，也可能场或帧场自适应，具体编码方式由其它元素决定。结合前一元素：FrameHeightInMbs=(2-frame_mbs_only_flag)*PicHeightInMapUnits */|0|ue(v)|
|if(frame_mbs_only_flag)|||
|mb_adaptiv_frame_field_flag /* 指明本序列是否是帧场自适应模式：frame_mbs_only_flag=1，全部是帧frame_mbs_only_flag=0， mb_adaptiv_frame_field_flag=0，帧场共存frame_mbs_only_flag=0， mb_adaptiv_frame_field_flag=1，帧场自适应和场共存*/|0|u(1)|
|direct_8x8_inference_flag /* 用于指明B片的直接和skip模式下的运动矢量的计算方式 */|0|u(1)|
|frame_cropping_flag /* 解码器是否要将图像裁剪后输出，如果是，后面为裁剪的左右上下的宽度 */|0|u(1)|
|if(frame_cropping_flag){|||
|frame_crop_left_offset|0|ue(1)|
|frame_crop_right_offset|0|ue(1)|
|frame_crop_top_offset|0|ue(1)|
|frame_crop_bottom_offset|0|ue(1)|
|}|||
|vui_parameters_present_flag /* 指明vui子结构是否出现在码流中，vui子结构在附录中指明，用于表征视频格式的信息 */|0|u(1)|
|if(vui_parameters_present_flag)|||
|vui_parameters()|0||
|rbsp_trailing_bits()|0||
|}|||
[点击打开链接](http://blog.csdn.net/heanyu/article/details/6191576)
表4
