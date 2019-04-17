# FFMPEG学习【libavcodec】：编解码器：硬件加速器桥：VA API Decoding - 一世豁然的专栏 - CSDN博客





2017年05月09日 17:07:23[一世豁然](https://me.csdn.net/Explorer_day)阅读数：389








一、文件

[vaapi.h](http://ffmpeg.org/doxygen/trunk/vaapi_8h.html)


公共libavcodec VA API头。








二、数据结构

struct  vaapi_context{

void * display；

窗口系统相关数据。

编码：未使用。

解码：被用户使用。




uint32_t config_id；

配置ID。



编码：未使用。

解码：被用户使用。




uint32_t context_id；

Context
ID（视频解码流水线）



编码：未使用。

解码：被用户使用。




attribute_deprecated uint32_t
pic_param_buf_id

VAPictureParameterBuffer ID.



编码：未使用。

解码：被libavcodec设置。




attribute_deprecated uint32_t iq_matrix_buf_id

VAIQMatrixBuffer ID.



编码：未使用。

解码：被libavcodec设置。





attribute_deprecated uint32_t bitplane_buf_id

VABitPlaneBuffer ID（用于VC-1解码）



编码：未使用。

解码：被libavcodec设置。




attribute_deprecated uint32_t * slice_buf_ids；

切片参数/数据缓冲区ID。



编码：未使用。

解码：被libavcodec设置。




attribute_deprecated unsigned int n_slice_buf_ids；

要发送到HW的有效片缓冲区ID的数量。



编码：未使用。

解码：被libavcodec设置。




attribute_deprecated unsigned int slice_buf_ids_alloc；

预分配的slice_buf_ids的大小。



编码：未使用。

解码：被libavcodec设置。




attribute_deprecated void * slice_params；

指向VASliceParameterBuffers的指针。



编码：未使用。

解码：被libavcodec设置。




attribute_deprecated unsigned int slice_param_size；

VASliceParameterBuffer元素的大小。



编码：未使用。

解码：被libavcodec设置。




attribute_deprecated unsigned int slice_params_alloc；

预先分配的slice_params的大小。



编码：未使用。

解码：被libavcodec设置。




attribute_deprecated unsigned int slice_count；

当前填写的切片数.



编码：未使用。

解码：被libavcodec设置。




attribute_deprecated const  uint8_t * slice_data；

指向片数据缓冲区的指针。



编码：未使用。

解码：被libavcodec设置。




attribute_deprecated uint32_t slice_data_size；

切片数据的当前大小。



编码：未使用。

解码：被libavcodec设置。

}


该结构用于在FFmpeg库和客户端视频应用程序之间共享数据。




