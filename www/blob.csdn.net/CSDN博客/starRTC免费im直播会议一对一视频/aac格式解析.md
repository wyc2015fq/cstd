# aac格式解析 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月16日 11:26:22[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：403
所属专栏：[编解码](https://blog.csdn.net/column/details/12721.html)
AAC格式有以下两种：
ADIF：Audio Data Interchange Format 音频数据交换格式。这种格式的特征是可以确定的找到这个音频数据的开始，不需进行在音频数据流中间开始的解码，即它的解码必须在明确定义的开始处进行。故这种格式常用在磁盘文件中。
ADTS：Audio Data Transport Stream 音频数据传输流。这种格式的特征是它是一个有同步字的比特流，解码可以在这个流中任何位置开始。它的特征类似于mp3数据流格式。
简言之。ADIF只有一个文件头，ADTS每个包前面有一个文件头。
ADIF格式见下图：
ADTS的一般格式见下图：
ADTS一帧的简明结构，其两边的空白矩形表示一帧前后的数据。
ADIF和ADTS的header是不同的。
AAC原始码流（又称为“裸流”）是由一个一个的ADTS frame组成的。他们的结构如下图所示。
其中每个ADTS frame之间通过syncword（同步字）进行分隔。同步字为0xFFF（二进制“111111111111”）。AAC码流解析的步骤就是首先从码流中搜索0x0FFF，分离出ADTS frame；然后再分析ADTS frame的首部各个字段。
参考
[http://blog.csdn.net/leixiaohua1020/article/details/11822537](http://blog.csdn.net/leixiaohua1020/article/details/11822537)
[](http://blog.csdn.net/leixiaohua1020/article/details/50535042)[AAC音频码流解析](http://blog.csdn.net/leixiaohua1020/article/details/50535042)
