# 视频编码方案之间的比较（HEVC，H.264，MPEG2等） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月02日 16:38:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：30
个人分类：[视频质量评价																[视频编码](https://blog.csdn.net/leixiaohua1020/article/category/1638135)](https://blog.csdn.net/leixiaohua1020/article/category/1446693)









Jens-Rainer Ohm等人在文献《Comparison of the Coding Efficiency of Video Coding Standards—Including High Efficiency Video Coding (HEVC)》中对比了几种视频编码方案。编码方案包括：HEVC，H.264，MPEG4，H.263，MPEG2。在此我们展示他们针对1280x720，1920x1080两种分辨率的视频的测试结果。文献中使用PSNR（Peak Signal to Noise Ratio，峰值信噪比）衡量视频的质量。PSNR（Peak Signal to Noise Ratio，峰值信噪比）反映了压缩编码后的受损视频序列和原始序列之间的差别；两个序列之间差别越大，PSNR值越低，表明视频的质量越差。几种编码方案的码率与PSNR的对应关系如图所示。

有关PSNR的知识可以参考文章：

[全参考视频质量评价方法（PSNR，SSIM）以及相关数据库](http://blog.csdn.net/leixiaohua1020/article/details/11694369)

[全参考客观视频质量评价方法 （MSE， PSNR，SSIM）原理](http://blog.csdn.net/leixiaohua1020/article/details/11729289)

1280x720分辨率视频实验结果

![](https://img-blog.csdn.net/20140812194156498?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

1280x720分辨率下HEVC相对于其他编码标准节约的码率

![](https://img-blog.csdn.net/20140812195520820?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



1920x1080分辨率视频实验结果

![](https://img-blog.csdn.net/20140812195607651?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

1920x1080分辨率下HEVC相对于其他编码标准节约的码率

![](https://img-blog.csdn.net/20140812195449640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




由图可见，尽管视频序列内容的不同可能会影响同等码率下视频的质量，但是视频编码方案之间的关系是很确定的。

总体说来，在同等的码率下，不同视频压缩编码方式的视频质量有如下关系：


HEVC > H.264 > MPEG4 > H.263 > MPEG2







更新（2014.8.12）====================

上文说述用的是客观视频质量评价算法，在这里补充论文中的主观质量评价试验。

主观评价包含了HEVC和H.264两种编码标准。Kimono1视频序列结果如图所示。

![](https://img-blog.csdn.net/20140812195849252?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Cactus视频序列结果如图所示。

![](https://img-blog.csdn.net/20140812195852777?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

从主观评价试验可以看出，HEVC基本上比H.264节约了50%的码率。







