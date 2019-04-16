# AAC格式简介 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月19日 00:34:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：70









AAC的音频文件格式有以下两种：

ADIF：Audio Data Interchange Format 音频数据交换格式。这种格式的特征是可以确定的找到这个音频数据的开始，不需进行在音频数据流中间开始的解码，即它的解码必须在明确定义的开始处进行。故这种格式常用在磁盘文件中。

ADTS：Audio Data Transport Stream 音频数据传输流。这种格式的特征是它是一个有同步字的比特流，解码可以在这个流中任何位置开始。它的特征类似于mp3数据流格式。这种格式可以用于广播电视。

简言之。ADIF只有一个文件头，ADTS每个包前面有一个文件头。

AAC的ADIF格式见下图：



![](https://img-blog.csdn.net/20130919002815812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

AAC的ADTS的一般格式见下图：



![](https://img-blog.csdn.net/20130919002821750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

图中表示出了ADTS一帧的简明结构，其两边的空白矩形表示一帧前后的数据。

ADIF和ADTS的header是不同的。它们分别如下所示：



ADIF的头信息

**![](https://img-blog.csdn.net/20130919002831078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**



ADTS的固定头信息（7字节）

**![](https://img-blog.csdn.net/20130919002836453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**



ADTS的可变头信息（略）



开源AAC解码器faad官方网站：

[http://www.audiocoding.com/](http://www.audiocoding.com/)

faad2源代码（VC工程）下载地址：

[http://download.csdn.net/detail/leixiaohua1020/6374877](http://download.csdn.net/detail/leixiaohua1020/6374877)




