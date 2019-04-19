# ffmpeg保存原始数据PCM YUV - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月20日 15:34:44[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：270
保存yuv
ffmpeg -i video.mp4-c:v rawvideo -pix_fmt yuv420pout.yuv
保存pcm
ffmpeg -i input.flv -f s16le -acodec pcm_s16le output.pcm
