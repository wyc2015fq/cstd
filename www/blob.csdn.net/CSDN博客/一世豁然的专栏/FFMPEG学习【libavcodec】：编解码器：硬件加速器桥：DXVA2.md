# FFMPEG学习【libavcodec】：编解码器：硬件加速器桥：DXVA2 - 一世豁然的专栏 - CSDN博客





2017年05月08日 15:12:14[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1144








一、文件

[dxva2.h](http://ffmpeg.org/doxygen/trunk/dxva2_8h.html)


公共libavcodec DXVA2有文件。








二、数据结构



```cpp
struct  	dxva_context{
	IDirectXVideoDecoder * 	decoder；			//DXVA2解码器对象。
```

```cpp
const DXVA2_ConfigPictureDecode * 	cfg；		//DXVA2配置用来创建解码器。
```

```cpp
unsigned 	surface_count；				//表面阵列中的表面数组。
```

```cpp
LPDIRECT3DSURFACE9 * 	surface；			//用于创建解码器的Direct3D曲面阵列。
```

```cpp
uint64_t 	workaround；				//配置使用解码器所需的解决方法的一个字段。
```

```cpp
unsigned 	report_id；				//专用于FFmpeg AVHWAccel实现。
}
```





三、宏

#define FF_DXVA2_WORKAROUND_SCALING_LIST_ZIGZAG   1


解决DXVA2和旧的UVD / UVD + ATI显卡。





#define FF_DXVA2_WORKAROUND_INTEL_CLEARVIDEO 2


解决DXVA2和旧版英特尔GPU与ClearVideo接口。




