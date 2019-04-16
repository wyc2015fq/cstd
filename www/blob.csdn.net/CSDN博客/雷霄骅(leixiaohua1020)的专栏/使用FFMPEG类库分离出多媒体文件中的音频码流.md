# 使用FFMPEG类库分离出多媒体文件中的音频码流 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月18日 11:13:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：52
个人分类：[FFMPEG](https://blog.csdn.net/leixiaohua1020/article/category/1360795)










在使用FFMPEG的类库进行编程的过程中，可以直接输出解复用之后的的音频数据码流。只需要在每次调用av_read_frame()之后将得到的视频的AVPacket存为本地文件即可。

 经试验，在分离MP3码流的时候，直接存储AVPacket即可。

 在分离AAC码流的时候，直接存储AVPacket后的文件可能是不能播放的。

 如果视音频复用格式是TS（MPEG2 Transport Stream），直接存储后的文件是可以播放的。

 复用格式是FLV，MP4等则不行。

 因为FLV，MP4这些属于“特殊容器”。经过仔细对比后发现，调用av_read_frame()后得到的AVPacket里面的内容是AAC纯数据，就是那种不包含ADTS文件头的AAC数据。因此如果想要得到可以播放的AAC文件，需要在每个AVPacket前面加上7字节ADTS文件头。具体的ADTS文件头的格式我还没有仔细研究，留在今后有时间在做研究了。




