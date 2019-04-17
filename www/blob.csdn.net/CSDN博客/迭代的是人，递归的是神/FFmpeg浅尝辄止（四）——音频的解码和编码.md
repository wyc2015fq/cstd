# FFmpeg浅尝辄止（四）——音频的解码和编码 - 迭代的是人，递归的是神 - CSDN博客





2012年06月28日 16:45:46[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：32128








音频和视频其实是一样的，在文件中寻找音频流，然后解压出来，得到音频帧的数据，同样也可以按照设定的编码格式进行压缩，我这里把音频的解码和编码做成了两个工程，也是直接上代码：



```cpp
#include <stdio.h>
#include <stdlib.h>

extern "C"
{
#include <libavcodec\avcodec.h>
#include <libavformat\avformat.h>
#include <libswscale\swscale.h>
}

int main(char arg,char *argv[])
{
	char *filename = argv[1];

	av_register_all();	//注册所有可解码类型
	AVFormatContext *pInFmtCtx=NULL;	//文件格式
	AVCodecContext *pInCodecCtx=NULL;	//编码格式 
	if (av_open_input_file(&pInFmtCtx, filename, NULL, 0, NULL)!=0)	//获取文件格式
		printf("av_open_input_file error\n");
	if (av_find_stream_info(pInFmtCtx) < 0)	//获取文件内音视频流的信息
		printf("av_find_stream_info error\n");

	unsigned int j;
	// Find the first audio stream

	int audioStream = -1;
	for (j=0; j<pInFmtCtx->nb_streams; j++)	//找到音频对应的stream
	{
		if (pInFmtCtx->streams[j]->codec->codec_type == CODEC_TYPE_AUDIO)
		{
			audioStream = j;
			break;
		}
	}
	if (audioStream == -1)
	{
		printf("input file has no audio stream\n");
		return 0; // Didn't find a audio stream
	}
	printf("audio stream num: %d\n",audioStream);
	pInCodecCtx = pInFmtCtx->streams[audioStream]->codec; //音频的编码上下文
	AVCodec *pInCodec = NULL;

	pInCodec = avcodec_find_decoder(pInCodecCtx->codec_id); //根据编码ID找到用于解码的结构体
	if (pInCodec == NULL)
	{
		printf("error no Codec found\n");
		return -1 ; // Codec not found
	}

	if(avcodec_open(pInCodecCtx, pInCodec)<0)//将两者结合以便在下面的解码函数中调用pInCodec中的对应解码函数
	{
		printf("error avcodec_open failed.\n");
		return -1; // Could not open codec

	}

	static AVPacket packet;

	printf(" bit_rate = %d \r\n", pInCodecCtx->bit_rate);
	printf(" sample_rate = %d \r\n", pInCodecCtx->sample_rate);
	printf(" channels = %d \r\n", pInCodecCtx->channels);
	printf(" code_name = %s \r\n", pInCodecCtx->codec->name);
	printf(" block_align = %d\n",pInCodecCtx->block_align);

	uint8_t *pktdata;
	int pktsize;
	int out_size = AVCODEC_MAX_AUDIO_FRAME_SIZE*100;
	uint8_t * inbuf = (uint8_t *)malloc(out_size);
	FILE* pcm;
	pcm = fopen("result.pcm","wb");
	long start = clock();
	while (av_read_frame(pInFmtCtx, &packet) >= 0)//pInFmtCtx中调用对应格式的packet获取函数
	{
		if(packet.stream_index==audioStream)//如果是音频
		{
			pktdata = packet.data;
			pktsize = packet.size;
			while(pktsize>0)
			{
				out_size = AVCODEC_MAX_AUDIO_FRAME_SIZE*100;
				//解码
				int len = avcodec_decode_audio2(pInCodecCtx, (short*)inbuf, &out_size, pktdata, pktsize);
				if (len < 0)
				{
					printf("Error while decoding.\n");
					break;
				}
				if(out_size > 0)
				{
					fwrite(inbuf,1,out_size,pcm);//pcm记录
					fflush(pcm);
				}
				pktsize -= len;
				pktdata += len;
			}
		} 
		av_free_packet(&packet);
	}
	long end = clock();
	printf("cost time :%f\n",double(end-start)/(double)CLOCKS_PER_SEC);
	free(inbuf);
	fclose(pcm);
	if (pInCodecCtx!=NULL)
	{
		avcodec_close(pInCodecCtx);
	}
	av_close_input_file(pInFmtCtx);

	return 0;
}
```


解码后的文件保存为result.pcm中，现在用这个文件压缩出新的音频文件，代码如下：





```cpp
void main()
{
	int16_t *samples;
	uint8_t *audio_outbuf;
	int audio_outbuf_size;
	int audio_input_frame_size;
	double audio_pts;
	
	const char* filename = "test.wav";
	FILE *fin = fopen("result.pcm", "rb"); //音频源文件 
	AVOutputFormat *fmt;
	AVFormatContext *oc;
	AVStream * audio_st;
	av_register_all();
	fmt = guess_format(NULL, filename, NULL);
	oc = av_alloc_format_context();
	oc->oformat = fmt;
	snprintf(oc->filename, sizeof(oc->filename), "%s", filename);
	audio_st = NULL;

	if (fmt->audio_codec != CODEC_ID_NONE)
	{
		AVCodecContext *c;
		audio_st = av_new_stream(oc, 1);
		c = audio_st->codec;
		c->codec_id = fmt->audio_codec;
		c->codec_type = CODEC_TYPE_AUDIO;
		c->bit_rate = 128000;
		c->sample_rate = 44100;
		c->channels = 2;
	}
	if (av_set_parameters(oc, NULL) < 0)
	{
		return;
	}
	dump_format(oc, 0, filename, 1);
	if (audio_st)
	{
		AVCodecContext* c;
		AVCodec* codec;
		c = audio_st->codec;
		codec = avcodec_find_encoder(c->codec_id);
		avcodec_open(c, codec);
		audio_outbuf_size = 10000;
		audio_outbuf = (uint8_t*)av_malloc(audio_outbuf_size);
		if (c->frame_size <= 1)
		{
			audio_input_frame_size = audio_outbuf_size / c->channels;
			switch (audio_st->codec->codec_id)
			{
			case CODEC_ID_PCM_S16LE:
			case CODEC_ID_PCM_S16BE:
			case CODEC_ID_PCM_U16LE:
			case CODEC_ID_PCM_U16BE:
				audio_input_frame_size >>= 1;
				break;
			default:
				break;
			}
		}
		else
		{
			audio_input_frame_size = c->frame_size;
		}
		samples = (int16_t*)av_malloc(audio_input_frame_size*2*c->channels);
	}
	if (!fmt->flags & AVFMT_NOFILE)
	{
		if (url_fopen(&oc->pb, filename, URL_WRONLY) < 0)
		{
			return;
		}
	}
	av_write_header(oc);
	for (;;)
	{
		if (audio_st)
		{
			audio_pts = (double)audio_st->pts.val * audio_st->time_base.num / audio_st->time_base.den;
		}
		else
		{
			audio_pts = 0.0;
		}
		if (!audio_st || audio_pts >= 360.0)
		{
			break;
		}
		if (fread(samples, 1, audio_input_frame_size*2*audio_st->codec->channels, fin) <= 0)
		{
			break;
		}
		AVCodecContext* c;
		AVPacket pkt;
		av_init_packet(&pkt);
		c = audio_st->codec;
		pkt.size = avcodec_encode_audio(c, audio_outbuf, audio_outbuf_size, samples);
		pkt.pts = av_rescale_q(c->coded_frame->pts, c->time_base, audio_st->time_base);
		pkt.flags |= PKT_FLAG_KEY;
		pkt.stream_index = audio_st->index;
		pkt.data = audio_outbuf;
		if (av_write_frame(oc, &pkt) != 0)
		{
			return;
		}
	}
	if (audio_st)
	{
		avcodec_close(audio_st->codec);
		av_free(samples);
		av_free(audio_outbuf);
	}
	av_write_trailer(oc);
	for (int i=0; i<oc->nb_streams; i++)
	{
		av_freep(&oc->streams[i]->codec);
		av_freep(&oc->streams[i]);
	}
	if (!(fmt->flags & AVFMT_NOFILE))
	{
		url_fclose(oc->pb);
	}
	av_free(oc);
	fclose(fin);
}
```


对应的下载链接：



音频解码：[http://download.csdn.net/detail/yang_xian521/4399219](http://download.csdn.net/detail/yang_xian521/4399219)

音频编码：[http://download.csdn.net/detail/yang_xian521/4399293](http://download.csdn.net/detail/yang_xian521/4399293)


至此，我已经实现了视频的解码编码，音频的解码编码，相信有心人肯定可以在此基础上实现视频音频的同步压缩，不过要再看一些网上的资料，也是很轻松的。至于视频的显示播放，大多数是要结合SDL实现，由于我只是浅尝辄止，只实现了图片的显示和简单的视频播放，比起我之前推荐的几个链接是弱爆了的，就不写下去了，这个系列就到这里吧，希望能给像我一样的一些入门级选手点帮助。也欢迎大神来多多指教。



