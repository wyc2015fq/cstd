# 将h264和aac码流合成flv文件 - DoubleLi - 博客园






在视频应用中，经常需要将接收到h264和aac数据保存成文件。 本来想用mp4格式，但是mp4在没有正常关闭的情况下会导致文件打不开，而在实际应用中经常会出现设备直接拔电，程序不是正常结束的情况。于是想用采用flv格式，flv相对mp4简单很多，参照ffmpeg里的flv自己封装了一个简易的flv合成文件。

使用流程：

//初始化文件





**[cpp]**[view plain](https://blog.csdn.net/lipku/article/details/78502582#)[copy](https://blog.csdn.net/lipku/article/details/78502582#)



- flvHandle_= flv_init( filename, 30, 1920, 960);  






//每收到一帧视频数据写入文件，此处写入的h264码流每个nal前需要添加00000001





**[cpp]**[view plain](https://blog.csdn.net/lipku/article/details/78502582#)[copy](https://blog.csdn.net/lipku/article/details/78502582#)



- flv_write_video_packet(flvHandle_, iskeyframe, buffer, bufsize, timestamp);  






//每收到一帧aac音频数据写入文件，注意aac音频数据需要将adts头去掉





**[cpp]**[view plain](https://blog.csdn.net/lipku/article/details/78502582#)[copy](https://blog.csdn.net/lipku/article/details/78502582#)



- flv_write_audio_packet(flvHandle_, buffer+7, bufsize-7, timestamp);  






//关闭文件，即使不调用该函数关闭文件，录像文件也能打开播放





**[cpp]**[view plain](https://blog.csdn.net/lipku/article/details/78502582#)[copy](https://blog.csdn.net/lipku/article/details/78502582#)



- flv_write_trailer(flvHandle_);  




flv合成代码下载地址 http://download.csdn.net/download/lipku/10121166









