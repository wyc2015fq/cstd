# 媒体格式分析之flv -- 基于FFMPEG - 建建的博客 - CSDN博客
2017年08月30日 10:32:44[纪建](https://me.csdn.net/u013898698)阅读数：185
个人分类：[多媒体封装格式详解](https://blog.csdn.net/u013898698/article/category/7105689)

本来是应该先写一个媒体文件格式的简单讲解的，还没来得及写，以后再写。今天就先根据ffmpeg的flv.c的flv_demux这个结构体来讲解一下当前比较流行的媒体格式flv.
FLV 是FLASH VIDEO的简称，FLV流媒体格式是随着Flash MX的推出发展而来的视频格式。由于它形成的文件极小、加载速度极快，使得网络观看视频文件成为可能.当前主流的媒体网站像国内的优酷、国外youtube其标清格式的文件均采用flv的格式。
#### FLV文件结构解析
FLV是一个二进制文件，其文件格式如下图 ，由文件头（FLV header）和很多tag组成。tag又可以分成三类：audio,video,script，分别代表音频流，视频流，脚本流（关键字或者文件信息之类）。
![flv_struct](http://images.cnblogs.com/cnblogs_com/qingquan/201108/201108112157245333.jpg)
#### FLV Header
FLV的Header信息一般比较简单，包括文件类型之类的全局信息。如下图中解析：
![flv_header](http://images.cnblogs.com/cnblogs_com/qingquan/201108/201108112157286889.jpg)
文件类型3bytes 总是FLV（0x46 0x4C 0x56），否则就不是在ffmpeg中在没有指定文件格式的情况下，也是通过这个字段来探测文件是否属于FLV格式的。
版本1byte 一般是0x01，表示FLV version 1
流信息1byte 倒数第一bit是1表示有视频，倒数第三bit是1表示有音频，其他都应该是0（有些软件如flvtool2可能造成倒数第四bit是1，不过也没发现有什么不对）
header长度4bytes 整个文件头的长度，一般是9（3+1+1+4），当然头部字段也有可能包含其它信息这个时间其长度就不是9了。
#### FLV Body
FLV body就是由很多tag组成的，一个tag包括下列信息：
      previoustagsize 4bytes 前一个tag的长度，第一个tag就是0
tag类型1byte 共分为三类：
* 8 -- 音频tag
* 9 -- 视频tag
* 18 -- 脚本tag
数据区长度3bytes 时间戳3bytes 单位毫秒同时还有1bytes的扩展时间戳，放在最高位，大部分时间时间戳为媒体的[dts](http://www.cnblogs.com/qingquan/archive/2011/07/27/2118967.html)信息，如果是脚本tag就是0
streamsID 3bytes 总是0（不知道干啥用）
数据区：根据不同的tag类型就有不同的数据区
##### 脚本tag ：
脚本tag一般是用文本方式表示，如下图flv的metadata信息：
![flv_16](http://images.cnblogs.com/cnblogs_com/qingquan/201108/201108112157485822.jpg)
从中可以看出是通过文本的方式来标记的，其解析后其header信息为：
![meta_data_header](http://images.cnblogs.com/cnblogs_com/qingquan/201108/201108112157528807.jpg)
从中可以看出其type为18。time stamp为0.data size为33638.
metadata tag data信息解析后为：
![metadata](http://images.cnblogs.com/cnblogs_com/qingquan/201108/201108112158032667.jpg)
其中有一些媒体信息：
例如视频的：高和宽它的codec id。帧率。音频的信息例如：音频的sample rate，codec id,sample size及是否立体声。还有整个文件的大小等等。
##### 音频的tag信息：
音频的tag信息如下图：
![audio tag](http://images.cnblogs.com/cnblogs_com/qingquan/201108/201108112158052993.jpg)
其中time stamp 为0是因为其为第一个音频tag.
##### 视频tag
![video](http://images.cnblogs.com/cnblogs_com/qingquan/201108/201108112158075270.jpg)
这是文件中的第6个tag所以其time stamp不为0。因为其为视频tag所以其type为9。
#### ffmpeg中的flv文件格式解析的实现：
其中flv_read_header主要是从文件中读取一些头信息，同时作一些初始化化的工作
static int flv_read_header(AVFormatContext *s,AVFormatParameters *ap) 
{
         ……
    url_fskip(s->pb, 4); //将flv的头去掉。 
    flags = get_byte(s->pb);//读出flv的video和audio flag信息。
         ……  
        if(flags & FLV_HEADER_FLAG_HASVIDEO){ 
        if(!create_stream(s, 0))  //创建视频流 
            return AVERROR(ENOMEM); 
    } 
    if(flags & FLV_HEADER_FLAG_HASAUDIO){ 
        if(!create_stream(s, 1)) //创建音频流 
            return AVERROR(ENOMEM); 
    }
    offset = get_be32(s->pb); //获取文件头长度
        …… 
}
其它tag的读取：
static int flv_read_packet(AVFormatContext *s, AVPacket *pkt) 
{
     …… 
for(;;url_fskip(s->pb, 4)){ /* pkt size is repeated at end. skip it */ 
    pos = url_ftell(s->pb); 
    type = get_byte(s->pb); //获取tag的类型，前面已经提到flv的tag大概有以下三种 ：FLV_TAG_TYPE_AUDIO
 = 0x08,FLV_TAG_TYPE_VIDEO = 0x09,FLV_TAG_TYPE_META  = 0x12,
    size = get_be24(s->pb);//获取tag的长度 
    dts = get_be24(s->pb); 
    dts |= get_byte(s->pb) << 24; //计算tag的timestamp也就是dts信息
    …… 
    if (type == FLV_TAG_TYPE_AUDIO) { //判断是否为audio tag
       …… 
         } else if (type == FLV_TAG_TYPE_VIDEO) {//判断是否为video tag
       …… 
               if ((flags & 0xf0) == 0x50) /* video info / command frame */ 
            goto skip; 
    } else { 
        if (type == FLV_TAG_TYPE_META && size > 13+1+4)//判断是否为meta tag,如果是meta信息则会将信息存放在一个map表中。 
         ……
}
