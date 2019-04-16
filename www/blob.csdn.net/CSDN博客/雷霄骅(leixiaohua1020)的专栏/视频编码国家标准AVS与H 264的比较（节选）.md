# 视频编码国家标准AVS与H.264的比较（节选） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月18日 13:41:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：32
个人分类：[视频质量评价																[视频编码](https://blog.csdn.net/leixiaohua1020/article/category/1638135)](https://blog.csdn.net/leixiaohua1020/article/category/1446693)









注：这个报告是官方报告，时间比较早了，AVS和H.264版本更新了很多，仅仅作为参考。

对视频编码标准进行客观评价的常用方法是峰值信噪比PSNR。表9、表10分别给出了AVS与MPEG-2标准以及AVS与MPEG-4 AVC/H.264标准main profile的客观编码性能。结果为相同码率条件下峰值信噪比PSNR的增益。可以看出，AVS相对于MPEG-2标准编码效率平均提高2.56dB，相比于H.264标准编码效率略低，平均有0.11dB的损失。

![](https://img-blog.csdn.net/20131018133633187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下面是另一组视频序列的AVS和H.264的性能比较实验结果。实验使用的AVS视频编码器是RM 5.0a，H.264编码器是JM 6.1e。

编码参数如下所示：

![](https://img-blog.csdn.net/20131018133648359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

实验结果（分为几个序列）：

![](https://img-blog.csdn.net/20131018133727375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20131018133746203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20131018133800359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20131018133803703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

从上面的数据可以看出，在逐行编码方面，AVS视频标准的性能与H.264基本一致；在隔行编码方面，由于AVS视频标准目前只支持图像级帧/场自适应编码，平均有0.5dB的性能差距。



报告地址：[http://download.csdn.net/detail/leixiaohua1020/6418329](http://download.csdn.net/detail/leixiaohua1020/6418329)





