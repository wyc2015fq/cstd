# ffmpeg库G726编解码实例 - 深之JohnChen的专栏 - CSDN博客

2017年11月02日 23:04:55[byxdaz](https://me.csdn.net/byxdaz)阅读数：2041


ffmpeg g726编码器：AV_CODEC_ID_ADPCM_G726

ffmpeg g726解码器包括：AV_CODEC_ID_ADPCM_G726、AV_CODEC_ID_ADPCM_G726LE

AV_CODEC_ID_ADPCM_G726，无法指定码流，默认是16kps；AV_CODEC_ID_ADPCM_G726LE，可以指导码流，AVCodecContext中的bits_per_coded_sample:表示编码压缩bit值与采样率的bit值之比。如果为g726音频时，表示g726码流压缩与采样率比值。比如kbps码流压缩比为：40k/8k = 5，kbps码流压缩比为16k/8k = 2。

 //编码

```cpp
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/frame.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}
//链接ffmpeg lib库

//pcm to g726
	AVCodec *codec;
	AVCodecContext *c= NULL;
	AVPacket avpkt;
	AVFrame *decoded_frame = NULL;

	/* register all the codecs */
	avcodec_register_all();
	av_init_packet(&avpkt);

	avpkt.data = NULL;
	avpkt.size = 0;
	/* find the mpeg audio decoder */
	codec = avcodec_find_encoder(AV_CODEC_ID_ADPCM_G726);
	if (!codec) 
	{
		fprintf(stderr, "codec not found\n");
		return;
	}
	c = avcodec_alloc_context3(codec);
	//put sample parameters 
	c->bits_per_coded_sample = 2;
	c->bit_rate = 16000;    //
	c->sample_rate = 8000;    //采样率
	c->channels = 1;     //通道数
	c->sample_fmt = AV_SAMPLE_FMT_S16;//采样位数
	/* open it */
	int iRet = avcodec_open2(c, codec,NULL);
	if ( iRet < 0 ) 
	{
		fprintf(stderr, "could not open codec\n");
		return;
	}

	CString filePath = "";
	CString newlFilePath = "";
	char szFilter[] = {"Pcm Files (*.pcm)|*.pcm||"};
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter,NULL);
	if(dlg.DoModal() == IDOK)
	{
		filePath = dlg.GetPathName();
		newlFilePath = filePath;
		newlFilePath.Replace(".pcm","_ff.g726");
		FILE * fpSrc = fopen(filePath.GetBuffer(filePath.GetLength()),"rb");
		FILE * fpDst = fopen(newlFilePath.GetBuffer(newlFilePath.GetLength()),"wb+");
		char szData[320] = {0};
		char szOutData[200] = {0};
		int  nDataLen = 320;
		int  nOutDataLen = 200;
		int  nReadedSize = 0;
		int  ret = 0;
		if(fpSrc != NULL)
		{
			while(TRUE)
			{
				nReadedSize = fread(szData,sizeof(char),nDataLen,fpSrc);
				if(nReadedSize < nDataLen)
				{
					break;
				}
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

				decoded_frame->nb_samples = nReadedSize/(c->channels*av_get_bytes_per_sample(c->sample_fmt));
				ret = avcodec_fill_audio_frame(decoded_frame, c->channels, c->sample_fmt, (unsigned char *)szData,nReadedSize, 1);
				if (ret < 0) 
				{
					return;
				}
				int got_packet = 0;
				if (avcodec_encode_audio2(c, &avpkt, decoded_frame, &got_packet) < 0) 
				{
					return;
				}
				if (got_packet) 
				{
					int iiiii = fwrite( avpkt.data, 1, avpkt.size, fpDst );
				}
			}

			fclose(fpSrc);
			fclose(fpDst);
		}

	}
```

//解码

```cpp
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/frame.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}
//链接ffmpeg lib库

//g726 to pcm
	AVCodec *codec;
	AVCodecContext *c= NULL;
	AVPacket avpkt;
	AVFrame *decoded_frame = NULL;
	avcodec_register_all();
	av_init_packet(&avpkt);
	/* find the mpeg audio decoder */
	codec = avcodec_find_decoder(AV_CODEC_ID_ADPCM_G726);
	if (!codec) 
	{
		fprintf(stderr, "codec not found\n");
		return;
	}
	c = avcodec_alloc_context3(codec);
	//采样率= 8000 每个采样用的bit数= 16 通道数= 1
	c->bits_per_coded_sample = 2;   //g726压缩比为8:1 编码前采样用bit数为那么编码后应该占/8 = 2
	c->channels = 1;
	c->sample_fmt = AV_SAMPLE_FMT_S16;
	c->sample_rate = 8000;
	c->codec_type = AVMEDIA_TYPE_AUDIO;
	c->bit_rate = 16000;
	int iRet = avcodec_open2(c, codec,NULL);
	if ( iRet < 0 ) 
	{
		fprintf(stderr, "could not open codec\n");
		return;
	}

	CString filePath = "";
	CString newlFilePath = "";
	char szFilter[] = {"g726 Files (*.g726)|*.g726||"};
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter,NULL);
	if(dlg.DoModal() == IDOK)
	{
		filePath = dlg.GetPathName();
		newlFilePath = filePath;
		newlFilePath.Replace(".g726",".pcm");
		BOOL bRet = 0;
		FILE * fpSrc = fopen(filePath.GetBuffer(filePath.GetLength()),"rb");
		FILE * fpDst = fopen(newlFilePath.GetBuffer(newlFilePath.GetLength()),"wb+");
		char szData[100] = {0};
		char szOutData[320] = {0};
		int  nDataLen = c->bits_per_coded_sample*20;
		int  nOutDataLen = 320;
		int  nReadedSize = 0;
		if(fpSrc != NULL)
		{
			while(TRUE)
			{
				nDataLen = 40;
				nReadedSize = fread(szData,sizeof(char),nDataLen,fpSrc);
				if(nReadedSize < nDataLen)
				{
					break;
				}
				avpkt.data = (uint8_t *)szData;
				avpkt.size = nDataLen;
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

