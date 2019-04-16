# FFmpeg 4.0.2 实现两个YUV序列拼接成一个YUV序列 - 墨雨依旧付疏狂Q - CSDN博客





2018年08月31日 16:42:11[墨雨依旧付疏狂Q](https://me.csdn.net/hsq1596753614)阅读数：186
所属专栏：[FFmpeg 4.0.2](https://blog.csdn.net/column/details/26855.html)









一、C++代码：

```
/*
 * 两个YUV拼接成一个YUV
 * FFmpeg：4.0.2
 */
int YUVCombine(AVFrame *srcFrame1, AVFrame *srcFrame2, AVFrame *dstFrame, int dstWidth, int dstHeight)
{
	// 合成后得到的帧
	int nDstSize = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, dstWidth * 2, dstHeight, 1);
	uint8_t *dstbuf = (uint8_t *)av_malloc(nDstSize);
	av_image_fill_arrays(dstFrame->data, dstFrame->linesize, dstbuf, AV_PIX_FMT_YUV420P, dstWidth * 2, dstHeight, 1);

	if (srcFrame1 && srcFrame2)
	{
		int nYIndex = 0, nUVIndex = 0;
		for (int i = 0; i < dstHeight; i++)
		{
			//Y  
			memcpy(dstFrame->data[0] + i * dstWidth * 2, srcFrame1->data[0] + nYIndex * dstWidth, dstWidth);
			memcpy(dstFrame->data[0] + dstWidth + i * dstWidth * 2, srcFrame2->data[0] + nYIndex * dstWidth, dstWidth);
			nYIndex++;
		}
		for (int i = 0; i < dstHeight / 4; i++)
		{
			//U
			memcpy(dstFrame->data[1] + i * dstWidth * 2, srcFrame1->data[1] + nUVIndex * dstWidth, dstWidth);
			memcpy(dstFrame->data[1] + dstWidth + i * dstWidth * 2, srcFrame2->data[1] + nUVIndex * dstWidth, dstWidth);
			//V  
			memcpy(dstFrame->data[2] + i * dstWidth * 2, srcFrame1->data[2] + nUVIndex * dstWidth, dstWidth);
			memcpy(dstFrame->data[2] + dstWidth + i * dstWidth * 2, srcFrame2->data[2] + nUVIndex * dstWidth, dstWidth);
			nUVIndex++;
		}
	}
	return 0;
}
```

二、实现效果如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20180831164122389?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hzcTE1OTY3NTM2MTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



