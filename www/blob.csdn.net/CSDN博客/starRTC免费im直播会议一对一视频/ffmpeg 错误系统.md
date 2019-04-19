# ffmpeg 错误系统 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年11月22日 11:10:10[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：443
[avcodec_decode_video2](http://www.ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga99ee61b6dcffb7817a275d39da58cc74).
 returns -1094995529.
#define AVERROR_INVALIDDATA FFERRTAG( 'I','N','D','A')
The -1094995529 becomes -0x41444E49 and when you look at those letters, in ACSII, 0x41 = 'A', 0x44 = 'D', 0x4E = 'N, and
 0x49 = 'I'. 
[https://baike.baidu.com/item/ASCII?fromtitle=ascii%E7%A0%81%E8%A1%A8](https://baike.baidu.com/item/ASCII?fromtitle=ascii%E7%A0%81%E8%A1%A8)
libavutil/error.h

由此可以借鉴定义自己的错误系统
具体参见codec_error.h
在error_entries里面定义错误信息，在头文件里定义错误宏。
调用的时候用 codec_err2str
如
if ((ret = init_audio_encoder() ) < 0) {
logee("init_audio_encoder_failed:%s", codec_err2str(ret));
return -4;
}
在init_audio_encoder里面会返回 CODECERROR_ENC_AUIDO_INIT_NOT_SUPPORT 等错误宏。
只要有错误，都需要输出日志，不能屏蔽。
其它地方可以根据日志的等级进行输出。
[https://stackoverflow.com/questions/22944133/how-can-i-find-out-what-this-ffmpeg-error-code-means](https://stackoverflow.com/questions/22944133/how-can-i-find-out-what-this-ffmpeg-error-code-means)
