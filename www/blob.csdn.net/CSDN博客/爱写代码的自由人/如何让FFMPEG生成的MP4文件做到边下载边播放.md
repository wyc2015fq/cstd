# 如何让FFMPEG生成的MP4文件做到边下载边播放 - 爱写代码的自由人 - CSDN博客





2018年02月22日 11:46:00[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：2145








     一般通过FFmpeg生成的MP4文件如果没有经过特殊处理在播放的时候是要下载完整个文件才能播放，但是我们想将文件用于点播，比如放到Darwin RTSP Server的媒体目录里让访问的客户端播放，必须让它支持边下载边播放。将MP4文件转成可以逐渐播放的操作叫做“流化”，那如何对一个MP4文件进行“流化”呢？我们可以借助ffmpeg带的一个命令行工具--qt-faststart。

  qt-faststart会把文件的meta信息移到文件头部，这样转换后的MP4就可以边下载边播放。

  用法: /usr/bin/qt-faststart   INPUT OUTPUT

        qt-faststart安装方法：

  进入FFMpeg安装目录，执行命令：

```
$ cd ffmpeg
    $ ./configure
    $ make tools/qt-faststart
    $ cp -a tools/qt-faststart /usr/bin/
```

   这样qt-faststart就安装好了。

   假设转码后的文件名字为a.mp4,执行以下命令：
`    $ /usr/bin/qt-faststart a.mp4 b.mp4`
  再播放b.mp4就可以边加载边播放了。

  我的资源里提供了这个工具的代码下载，大家编译后就能用。[点击打开链接](http://download.csdn.net/download/zhoubotong2012/10242454)

  上面是通过命令行编辑MP4文件的方法，需要把原来的MP4文件生成后，然后再通过工具转换。那能不能通过API让生成的MP4直接就是流化的格式？

  如果你是调用FFmpeg的API来写MP4文件，可以加上rtphint+faststart参数来使生成的MP4成为“流化”的结构，下面是关键的代码：

  //AVFormatContext *m_oc;


#if 1
    AVDictionary* options = NULL;
    av_dict_set(&options, "movflags", "rtphint+faststart", 0); 
    int nRet = avformat_write_header(m_oc, &options);
    ASSERT(nRet == 0);
#else
    int nRet = avformat_write_header(m_oc, NULL);
#endif




