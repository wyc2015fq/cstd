# ffmpeg库解码海思G726库编码音频数据 - 深之JohnChen的专栏 - CSDN博客

2017年11月02日 23:08:17[byxdaz](https://me.csdn.net/byxdaz)阅读数：1858


解码流程：

1、  读取海思g726音频数据，海思g726音频会多4个字节的海思头信息。

2、选择ffmpeg g726编码器进行解码。ffmpeg g726解码器包括：AV_CODEC_ID_ADPCM_G726、AV_CODEC_ID_ADPCM_G726LE。如果海思g726码流类型为asf时，请选择AV_CODEC_ID_ADPCM_G726解码器类型；如果海思g726码流类型为RFC3551标准时，请选择AV_CODEC_ID_ADPCM_G726LE解码器类型。

实例代码：

```cpp
#define  Hisi_AUDIO_HERDER_LEN			4					//hisi 音频数据头
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/frame.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}
//链接 ffmpeg lib库

AVCodec *codec;
	AVCodecContext *c= NULL;
	AVPacket avpkt;
	AVFrame *decoded_frame = NULL;
	avcodec_register_all();
	av_init_packet(&avpkt);
	/* find the mpeg audio decoder */
	/*
	ffmpeg g726编码器：AV_CODEC_ID_ADPCM_G726
	ffmpeg g726解码器包括：AV_CODEC_ID_ADPCM_G726、AV_CODEC_ID_ADPCM_G726LE
	如果海思g726码流类型为asf时，请选择AV_CODEC_ID_ADPCM_G726解码器类型
	如果海思g726码流类型为RFC3551标准时，请选择AV_CODEC_ID_ADPCM_G726LE解码器类型
	*/
	codec = avcodec_find_decoder(AV_CODEC_ID_ADPCM_G726LE);
	if (!codec) 
	{
		fprintf(stderr, "codec not found\n");
		return;
	}
	c = avcodec_alloc_context3(codec);
	//采样率= 8000 每个采样用的bit数= 16 通道数= 1
	/*
	bits_per_coded_sample:表示编码压缩bit值与采样率的bit值之比。
	如果为g726音频时，表示g726码流压缩与采样率比值。比如kbps码流压缩比为：k/8k = 5，kbps码流压缩比为k/8k = 2。
	*/
	c->bits_per_coded_sample = 5;
	c->channels = 1;
	c->sample_fmt = AV_SAMPLE_FMT_S16;
	c->sample_rate = 8000;
	c->codec_type = AVMEDIA_TYPE_AUDIO;
	//c->bit_rate = 16000;
	int iRet = avcodec_open2(c, codec,NULL);
	if ( iRet < 0 ) 
	{
		fprintf(stderr, "could not open codec\n");
		return;
	}

	CString filePath = "";
	CString newlFilePath = "";
	char szFilter[] = {"g726 Files (*.g726_hisi)|*.g726_hisi||"};
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter,NULL);
	if(dlg.DoModal() == IDOK)
	{
		filePath = dlg.GetPathName();
		newlFilePath = filePath;
		newlFilePath.Replace(".g726_hisi",".hisi2ff.pcm");
		BOOL bRet = 0;
		FILE * fpSrc = fopen(filePath.GetBuffer(filePath.GetLength()),"rb");
		FILE * fpDst = fopen(newlFilePath.GetBuffer(newlFilePath.GetLength()),"wb+");
		char szData[100] = {0};
		char szOutData[320] = {0};
		int  nDataLen = 100;
		int  nOutDataLen = 320;
		int  nReadedSize = 0;
		unsigned short usHisiHeader[2] = {0};
		if(fpSrc != NULL)
		{
			while(TRUE)
			{
				//读取头标记
				nDataLen = Hisi_AUDIO_HERDER_LEN;
				nReadedSize = fread(szData,sizeof(char),nDataLen,fpSrc);
				if(nReadedSize < nDataLen)
				{
					break;
				}
				memcpy(usHisiHeader,szData,Hisi_AUDIO_HERDER_LEN);
				int nAudioFrameDataLen = (usHisiHeader[1] & 0x00ff) * sizeof(unsigned short);
				nDataLen = nAudioFrameDataLen;
				//读取音频帧数据
				nReadedSize = fread(szData,sizeof(char),nDataLen,fpSrc);
				if(nReadedSize < nDataLen)
				{
					break;
				}
				avpkt.data = (uint8_t *)szData;
				avpkt.size = nReadedSize;
				int got_frame = 0;
				if (!decoded_frame) 
				{
					if (!(decoded_frame = avcodec_alloc_frame())) 
					{
						return;
					}
				} 
				else
				{
					avcodec_get_frame_defaults(decoded_frame);
				}
				int len = avcodec_decode_audio4(c, decoded_frame, &got_frame, &avpkt);
				if (len < 0) 
				{
					return;
				}
				if (got_frame) 
				{
					/* if a frame has been decoded, output it */
					int data_size = av_samples_get_buffer_size(NULL, c->channels,
						decoded_frame->nb_samples,
						c->sample_fmt, 1);

					fwrite(decoded_frame->data[0], 1, data_size, fpDst);
					
				}
			}

			fclose(fpSrc);
			fclose(fpDst);
			avcodec_close(c);
			av_free(c);
			av_free(decoded_frame);
		}

	}
```

[代码下载](http://download.csdn.net/download/byxdaz/10051164)

