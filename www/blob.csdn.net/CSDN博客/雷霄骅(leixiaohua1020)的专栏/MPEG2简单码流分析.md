# MPEG2简单码流分析 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月20日 14:03:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：33
个人分类：[视频编码](https://blog.csdn.net/leixiaohua1020/article/category/1638135)










今天使用Elecard Stream Eye打开了一个MPEG2视频编码的TS封装格式的文件。

分析的结果如图所示：

P帧：

像条：

![](https://img-blog.csdn.net/20130920135826468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

宏块划分（只有一种）：

![](https://img-blog.csdn.net/20130920135826593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

运动矢量：

![](https://img-blog.csdn.net/20130920135950500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

宏块类型（红：I宏块；蓝：P宏块；黄：skip宏块）：

![](https://img-blog.csdn.net/20130920140012984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

宏块大小（颜色越浅，数据量越大）：

![](https://img-blog.csdn.net/20130920140031265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

I帧：

宏块类型：

![](https://img-blog.csdn.net/20130920140258140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




