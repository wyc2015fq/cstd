# Android MediaCodec小结 - u013366022的专栏 - CSDN博客
2015年04月11日 13:03:47[slitaz](https://me.csdn.net/u013366022)阅读数：1186
 MediaCodec进行编解码的大体逻辑是这样的：
![](https://img-blog.csdn.net/20140528100114687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbW91c2VfMTg5NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
    主要函数的调用逻辑如下：
![](https://img-blog.csdn.net/20140528102358468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbW91c2VfMTg5NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
    需要做的东西是一个媒体文件合成器，功能是先截取出多段媒体文件中的某些片段，再将这些片段合成为一个新的媒体文件。实际上就是先用解码器将多段outputBuffer数据拿出来，然后再用编码器将它们放到一个文件里去。如果还要加上叠加字幕、特技处理等的效果，实际上也是先拿到outputBuffer，然后处理，最后将处理后的数据喂给编码器，生成最终文件。
