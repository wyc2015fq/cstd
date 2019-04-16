# FFmpeg: AVFormatContext 结构体分析 - 夜行过客 - 博客园







# [FFmpeg: AVFormatContext 结构体分析](https://www.cnblogs.com/yongdaimi/p/10649251.html)





AVFormatContext 结构体分析
这个结构体描述了一个媒体文件或媒体流的构成和基本信息。这是FFMpeg中最为基本的一个结构，是其他所有结构的根，是一个多媒体文件或流的根本抽象。
主要成员释义：


**nb_streams** ：输入视频的流的个数。
**duration** ：输入视频的时长（以微秒为单位，转换成秒须除以1000000）。
**bit_rate**：输入视频的比特率，转换成kpbs需要除以1000。
**streams**： 输入视频的流的数组。
**filename**: 输入视频的文件名。
**iformat**： 输入数据的封装格式。







参考链接：

1.[FFMPEG-数据结构解释(AVCodecContext,AVStream,AVFormatContext)](https://blog.csdn.net/yuan892173701/article/details/8702333)

2.[FFmpeg编程初步](https://blog.csdn.net/mandagod/article/details/52003566)














