# 全参考视频质量评价方法（PSNR，SSIM）以及相关数据库 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月14日 21:20:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：50
个人分类：[视频质量评价](https://blog.csdn.net/leixiaohua1020/article/category/1446693)










最常用的全参考视频质量评价方法有以下2种：

PSNR（峰值信噪比）：用得最多，但是其值不能很好地反映人眼主观感受。一般取值范围：20-40.值越大，视频质量越好。

SSIM（结构相似性）：计算稍复杂，其值可以较好地反映人眼主观感受。一般取值范围：0-1.值越大，视频质量越好。

偶然发现了一个很好的网站。里面包含了大量主观评价算法的数据，导入到Matlab中就可以使用。

[http://sse.tongji.edu.cn/linzhang/IQA/IQA.htm](http://sse.tongji.edu.cn/linzhang/IQA/IQA.htm)

注：MOS（Mean Opnion Score，平均意见分）是主观评价实验之后，得到的主观分数，取值0-100，值越大，代表主观感受越好。



以下实验数据来自Live数据库：[http://live.ece.utexas.edu/research/Quality/](http://live.ece.utexas.edu/research/Quality/)

PSNR主观感受与客观分数之间的关系：

![](https://img-blog.csdn.net/20130914211738484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

更新（2014.7.31）===============

拟合结果：MOS = -24.3816 *( 0.5 - 1./(1 + exp(-0.56962 * (PSNR - 27.49855)))) + 1.9663*PSNR -2.37071;





SSIM主观感受与客观分数之间的关系：

![](https://img-blog.csdn.net/20130914211910968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

拟合结果：

MOS = 2062.3 * (1/(1 + EXP(-11.8 * (SSIM - 1.3)))+0.5) + 40.6*SSIM-1035.6










