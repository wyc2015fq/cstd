# FFMPEG学习【libavcodec】：编解码器：硬件加速器桥：Direct3D11 - 一世豁然的专栏 - CSDN博客





2017年05月08日 15:01:22[一世豁然](https://me.csdn.net/Explorer_day)阅读数：2350








一、数据结构



```cpp
struct	AVD3D11VAContext{
	ID3D11VideoDecoder * 	decoder；					//D3D11解码器对象。
	ID3D11VideoContext * 	video_context；					//D3D11视频上下文
	D3D11_VIDEO_DECODER_CONFIG * 	cfg					//D3D11配置用于创建解码器。
	unsigned 	surface_count						//表面阵列中的表面数组。
	ID3D11VideoDecoderOutputView ** 	surface;			//用于创建解码器的Direct3D曲面阵列。
	uint64_t 		workaround;					//配置使用解码器所需的解决方法的一个字段。
	unsigned 	report_id;						//专用于FFmpeg AVHWAccel实现。
	HANDLE 	context_mutex;							//互斥体访问video_context。
}
```






二、宏

#define FF_DXVA2_WORKAROUND_SCALING_LIST_ZIGZAG 1


适用于Direct3D11和旧的UVD / UVD + ATI显卡。





#define FF_DXVA2_WORKAROUND_INTEL_CLEARVIDEO 2


解决Direct3D11和具有ClearVideo界面的旧Intel GPU。








三、函数

AVD3D11VAContext * av_d3d11va_alloc_context (void)

解析：分配一个AVD3D11VAContext。



