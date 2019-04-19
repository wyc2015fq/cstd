# H.264中的NAL单元类型 - 三少GG - CSDN博客
2012年04月17日 15:17:01[三少GG](https://me.csdn.net/scut1135)阅读数：1602
## +++++++++++++++++++++++++++++++
## +Grant推荐：                                                      +
## +不错的博客地址：[](http://shijuanfeng.blogbus.com/c3903086/)[http://blog.csdn.net/wangjiannuaa/article/category/747251/1](http://blog.csdn.net/wangjiannuaa/article/category/747251/1)
+关注标签 1. [多媒体](http://blog.csdn.net/wangjiannuaa/article/category/747251)(16) 
  2. [](http://shijuanfeng.blogbus.com/c3982052/)[](http://blog.csdn.net/dgglx/article/category/878651)[网络](http://blog.csdn.net/wangjiannuaa/article/category/771658)(32) 
 3.[VC++备忘录](http://blog.csdn.net/wangjiannuaa/article/category/732222)(40) +
## +++++++++++++++++++++++++++++++
### [H.264中的NAL单元类型 nal_unit_type](http://blog.csdn.net/wangjiannuaa/article/details/6966505)
分类： [多媒体](http://blog.csdn.net/wangjiannuaa/article/category/747251)2011-11-13
 20:21427人阅读[评论](http://blog.csdn.net/wangjiannuaa/article/details/6966505#comments)(0)[收藏](http://blog.csdn.net/wangjiannuaa/article/details/6966505)[举报](http://blog.csdn.net/wangjiannuaa/article/details/6966505#report)
|**nal_unit_type**|**NAL 单元和 RBSP 语法结构的内容 **||
|----|----|----|
|0|未指定||
|1|一个非IDR图像的编码条带 slice_layer_without_partitioning_rbsp( )||
|2|编码条带数据分割块A slice_data_partition_a_layer_rbsp( )||
|3|编码条带数据分割块B slice_data_partition_b_layer_rbsp( )||
|4|编码条带数据分割块C slice_data_partition_c_layer_rbsp( )||
|5|IDR图像的编码条带 slice_layer_without_partitioning_rbsp( )||
|6|辅助增强信息 (SEI) sei_rbsp( )||
|7|序列参数集 seq_parameter_set_rbsp( )||
|8|图像参数集 pic_parameter_set_rbsp( )||
|9|访问单元分隔符 access_unit_delimiter_rbsp( )||
|10|序列结尾 end_of_seq_rbsp( )||
|11|流结尾 end_of_stream_rbsp( )||
|12|填充数据 filler_data_rbsp( )||
|13|序列参数集扩展 seq_parameter_set_extension_rbsp( )||
|14...18|保留||
|19|未分割的辅助编码图像的编码条带 slice_layer_without_partitioning_rbsp( )||
|20...23|保留||
|24...31|未指定||


_____________________________________________________________________________________
nal_unit:
{
  forbidden_zero_bit
  nal_ref_idc 
  nal_unit_type
  RBSP（Raw Byte Sequence Payloads）
}
forbidden_zero_bit==0
nal_ref_idc  :不等于0 时，规定NAL 单元的内容包含一个序列参数集，或一个图像参数集，或一个参考图像条带，或一个参考图像的条带数据分割。
nal_unit_type :是指包含在NAL 单元中的RBSP 数据结构的类型，VCL NAL 单元是指那些 nal_unit_type值等于1到5（包括1和5）的NAL单元。所有其他的NAL单元都称作非VCL NAL单元
当一个编码图像条带NAL 单元的nal_unit_type 值等于5 时，编码的同一图像其他所有的VCL NAL 单元的
nal_unit_type值都应该为5。这样的图像称作IDR图像。
注 :— 条带数据分割不可以用于IDR图像
如果一个包含一个条带或条带数据分割的NAL 单元的nal_ref_idc 等于0 时，该条带或条带数据分割是一个非参考图像的一部分。
对于序列参数集或序列参数集扩展或图像参数集的NAL单元，nal_ref_idc不应等于0。当一个特定的图像的一个条带或条带数据分割NAL单元的nal_ref_idc等于0，该图像的所有条带或条带数据划分NAL单元都应该等于0。
IDR NAL单元的nal_ref_idc不应等于0，即nal_unit_type等于5的NAL单元。
所有nal_unit_type等于6、9、10、11或12的NAL单元其nal_ref_idc都应等于0。
### [H.264中的slice](http://blog.csdn.net/wangjiannuaa/article/details/6966417)
分类： [多媒体](http://blog.csdn.net/wangjiannuaa/article/category/747251)2011-11-13
 19:52165人阅读[评论](http://blog.csdn.net/wangjiannuaa/article/details/6966417#comments)(0)[收藏](http://blog.csdn.net/wangjiannuaa/article/details/6966417)[举报](http://blog.csdn.net/wangjiannuaa/article/details/6966417#report)
**slice - 条带：**
特定条带组内部按照光栅扫描顺序排列的整数个宏块或宏块对。对于基本编码图像，条带组分割为条带称为分割。虽然一个条带包含条带组内部按照光栅扫描顺序排列的整数个宏块或宏块对，但这些宏块或宏块对在图像内部并不一定是按照光栅扫描顺序连续排列的。宏块地址是通过条带第一个宏块的地址（条带头中描述）以及宏块到条带组的映射得到的。
**slice header - 条带头：**
编码条带的一部分，包含与该条带中第一个或者全部宏块有关的数据元素。 
**B slice - B 条带： **
根据同一条带中已解码样点帧内预测，**或**从先前解码的参考图像经过帧间预测得到的条带，对每个块进行预测时最多使用两个运动矢量和参考索引。
**I slice** -**帧内（I）条带：**
非 SI 条带，并且该条带在解码时仅使用同一个条带内部的编码样点进行预测。
**P slice** -**P 条带：**
可根据同一条带中已解码样点利用帧内预测进行解码，**或者**根据先前解码的参考图像利用帧间预测进行解码的条带，最多使用一个运动矢量和参考索引对每个块中的样点做预测。
**SI slice** -**SI 条带：**
仅使用同一条带中的已解码样点进行预测编码的条带，并使用预测样点的量化。SI条带可以被编码从而其中的已解码样点可以和 SP条带一样重建。
**SP slice** -**SP 条带：**
使用同一条带中的已解码参考样点进行帧间预测编码的条带，SP条带使用至少一个运动矢量和参考索引来预测每个块的样点值。SI 条带可以被编码从而其中的已解码样点可以和另一个 SP 条带或 SI条带一样重建。
![](http://hi.csdn.net/attachment/201111/22/0_1321976244SU8r.gif)
