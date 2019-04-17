# Ffmpeg转码研究一 - DoubleLi - 博客园






Ffmpeg是一款功能强大的视频处理工具，那么转码肯定不是问题的，因为项目的需求，对转码进行了研究。刚开始首先去看了ffmpeg源代码中的一个例子transcode.c，但是发现该例子更应该称之为remux的一个例子，并没有具体的涉及到转码。所以转而去ffmpeg工具中寻求帮助了。

## ffmpeg转码流程：



![](http://img.blog.csdn.net/20150129145235006?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcm9vdHVzZXJz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 举例：

输入文件为ts格式，视频编码为H.264，音频编码为AAC，要求的输出文件封装格式为AVI，视频编码为MPEG2，音频编码为mp3。

## Ffmpeg命令行实现：

1.将视频编码为mpeg2的ts文件，重新编码为h264，音频编码为aac

ffmpeg -y -i style.ts -vcodec libx264 -acodec aac -ac 2 strict -2 test.avi



-y 如果test.avi文件存在则覆盖

-i  输入文件

-vcodec 指定视频编码器

-acodec 指定音频编码器

-ac         设定声道数，1是单声道，2是立体声



某些情况下的转码，只需要转码音频，而视频直接复制，这时候可以用到copy命令。

ffmpeg -y -i style.ts -codec:v copy -codec:a copy test.avi



上面的命令行实际上是换了一个封装格式。

 from:http://blog.csdn.net/rootusers/article/details/43272641









