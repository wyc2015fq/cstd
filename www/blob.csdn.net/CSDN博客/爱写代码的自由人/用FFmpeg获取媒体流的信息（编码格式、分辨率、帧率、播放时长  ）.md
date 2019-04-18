# 用FFmpeg获取媒体流的信息（编码格式、分辨率、帧率、播放时长...） - 爱写代码的自由人 - CSDN博客





2018年02月20日 20:15:05[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：10774










  我们经常需要知道一个媒体文件所包含的媒体流的信息，比如视频编码格式，视频分辨率，帧率，音频编码格式等信息。如何使用FFmpeg API获取这些信息呢？下面简要列出相关的代码。

### 1. 定义相关的变量

```cpp
AVFormatContext* m_inputAVFormatCxt;
int          m_videoStreamIndex;
int          m_audioStreamIndex;
string       m_filePath; //打开的文件路径
int          m_coded_width, m_coded_height; //视频宽高
LONGLONG     m_Duration; //文件播放时长，单位：毫秒
```

### 2. 打开一个文件

```cpp
int res = 0;
    if ((res = avformat_open_input(&m_inputAVFormatCxt, m_filePath.c_str(), 0, NULL)) < 0)
    {

    }
    if(res < 0)
    {
        string strError = "can not open file:" + m_filePath + ",errcode:" + std::to_string((int)res) + ",err msg:" + av_make_error_string(m_tmpErrString, AV_ERROR_MAX_STRING_SIZE, res);
        TRACE("%s \n", strError.c_str());
        return false;
    }
    if (avformat_find_stream_info(m_inputAVFormatCxt, 0) < 0)
    {
       TRACE("can not find stream info \n");
       return false;
    }
    av_dump_format(m_inputAVFormatCxt, 0, m_filePath.c_str(), 0);
```

### 3. 获取视频的编码格式、视频宽高

```cpp
for (int i = 0; i < m_inputAVFormatCxt->nb_streams; i++)
    {
        AVStream *in_stream = m_inputAVFormatCxt->streams[i];

		TRACE("codec id: %d, \n", in_stream->codec->codec_id);

		if(in_stream->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			m_videoStreamIndex = i;
			TRACE("video stream index: %d, width: %d, height: %d \n", m_videoStreamIndex, in_stream->codec->width, in_stream->codec->height);

			m_coded_width = in_stream->codec->width;
			m_coded_height = in_stream->codec->height;

			if(in_stream->codec->codec_id == AV_CODEC_ID_H264)
			{
			}	
		}	
}//for
```

上面代码中我们先取得AVStream类型的指针， in_stream->codec->codec_id获得表示视频（或音频）的CodecID，这个CodecID是一个枚举类型，表示一种编码格式，AV_CODEC_ID_H264表示H264格式。 in_stream->codec->width、 in_stream->codec->height是视频的宽高。

### 4. 获得音频的编码格式

音频的编码格式的获取方法跟视频的相同

```cpp
for (int i = 0; i < m_inputAVFormatCxt->nb_streams; i++)
{
	AVStream *in_stream = m_inputAVFormatCxt->streams[i];

	TRACE("codec id: %d, \n", in_stream->codec->codec_id);

	if(in_stream->codec->codec_type == AVMEDIA_TYPE_AUDIO)
	{
		m_audioStreamIndex = i;

		if(in_stream->codec->codec_id == AV_CODEC_ID_AAC)
		{
		}
		else if(in_stream->codec->codec_id == AV_CODEC_ID_AMR_NB)
		{
		}
	}
}//for
```

### 5.  获取音频的采样格式、采样率、声道数

```cpp
AVSampleFormat  sample_fmt;
int  sample_rate,  channels;

sample_fmt = m_inputAVFormatCxt->streams[m_audioindex]->codec->sample_fmt;
sample_rate = m_inputAVFormatCxt->streams[m_audioindex]->codec->sample_rate;
channels = m_inputAVFormatCxt->streams[m_audioindex]->codec->channels;
```

### 6. 获得整个文件的播放时长

```cpp
if (m_inputAVFormatCxt->duration > 0)
{
	m_Duration = m_inputAVFormatCxt->duration / 1000LL;
}
else
{
	m_Duration = 0;
}
```

### 7. 获取视频的帧率

```cpp
int  frame_rate = 0;
AVStream *stream = m_inputAVFormatCxt->streams[m_videoStreamIndex ];  

if(stream->r_frame_rate.den > 0)
{
    frame_rate = stream->r_frame_rate.num/stream->r_frame_rate.den;
}
else if(stream->codec->framerate.den > 0)
{
    frame_rate = stream->codec->framerate.num/stream->codec->framerate.den;
}
```

### 8. 获取视频的帧数

这个有点麻烦，除了遍历视频的每一帧，暂时没找到其他方法。

```cpp
m_nFrameCount =0;
AVPacket pkt;
av_init_packet(&pkt);

//一帧一帧读取视频
while (av_read_frame(m_inputAVFormatCxt, &pkt) >= 0)
{
	if(pkt.stream_index == m_videoStreamIndex)
	{
		m_nFrameCount++;
	}
	av_free_packet(&pkt);
}
//读完要把文件指针移到文件开始位置
av_seek_frame(m_inputAVFormatCxt,m_videoStreamIndex, 0*1000, 0);
```






