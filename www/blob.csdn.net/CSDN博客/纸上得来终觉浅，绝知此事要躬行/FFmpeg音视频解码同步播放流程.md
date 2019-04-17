# FFmpeg音视频解码同步播放流程 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年01月16日 10:38:21[boonya](https://me.csdn.net/boonya)阅读数：1419








数据接收后处理流程及阶段示意图：

![](https://img-blog.csdn.net/20180116104009947?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


1、接收导数据(Trans Stage)

2、新建音、视频解码线程（Parse Stage）

3、将解码好的数据分别放入队列(Store Stage)

4、使用有序的数据结构对音视频进行时间戳排序处理(Synch Stage)

5、开启不同的线程播放音视频数据(Play Stage)






