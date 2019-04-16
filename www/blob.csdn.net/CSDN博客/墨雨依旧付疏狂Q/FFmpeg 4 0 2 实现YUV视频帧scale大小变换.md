# FFmpeg 4.0.2 实现YUV视频帧scale大小变换 - 墨雨依旧付疏狂Q - CSDN博客





2018年08月31日 09:52:45[墨雨依旧付疏狂Q](https://me.csdn.net/hsq1596753614)阅读数：187
所属专栏：[FFmpeg 4.0.2](https://blog.csdn.net/column/details/26855.html)









```
int YUVFrameScale(AVFrame *srcYUVFrame, int nSrcW, int nSrcH, AVFrame *dstYUVFrame, int nDstW, int nDstH)
{
	// 目标缓冲区
	int dst_bufferSize = nDstW * nDstH * 3 / 2;
	uint8_t *dst_bufferPtr = (uint8_t *)malloc(dst_bufferSize * 2);
	if (dst_bufferPtr == NULL)
	{
		cout << "error when malloc for dst_bufferPtr" << endl;
		return -1;
	}
	// 分配空间
	av_image_fill_arrays(dstYUVFrame->data, dstYUVFrame->linesize, dst_bufferPtr, AV_PIX_FMT_YUV420P, nDstW, nDstH, 1);
	struct SwsContext* m_pSwsContext = sws_getContext(nSrcW, nSrcH, AV_PIX_FMT_YUV420P, nDstW, nDstH, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
	if (NULL == m_pSwsContext)
	{
		cout << "ffmpeg get context error!" << endl;
		return -1;
	}
	sws_scale(m_pSwsContext, srcYUVFrame->data, srcYUVFrame->linesize, 0, nSrcH, dstYUVFrame->data, dstYUVFrame->linesize);
	sws_freeContext(m_pSwsContext);
	return 0;
}
```



