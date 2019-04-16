# 全参考客观视频质量评价方法 （MSE， PSNR，SSIM）原理 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月16日 12:53:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：67
个人分类：[视频质量评价](https://blog.csdn.net/leixiaohua1020/article/category/1446693)










全参考客观视频质量评价方法是指把原始参考视频与失真视频在每一个对应帧中的每一个对应像素之问进行比较。准确的讲，这种方法得到的并不是真正的视频质量，而是失真视频相对于原始视频的相似程度或保真程度。最简单的方法如均方误差MSE和峰值信噪比PSNR，其应用比较广泛。
最常见的全参考视频质量评价方法有以下三种：

(1) 均方误差MSE

![](https://img-blog.csdn.net/20130916124342703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

其中，fij，f'ij分别代表原始参考视频对应帧和失真视频对应帧，M,N分别表示视频帧的高和宽。

(2) 峰值信噪比PSNR

![](https://img-blog.csdn.net/20130916124351515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

PSNR本质上与MSE相同，是MSE的对数表示。

(3) 基于结构失真的SSIM评估方法

![](https://img-blog.csdn.net/20130916124738375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

SSIM模型旨在比较参考和受损信号的结构信息，研究感知结构的损伤，而不是感知误差。上节中基于HVS的模型，采用自底向上的方法，模拟人类视觉系统中每个与质量感知相关的器官的功能，然后将每个部分联结起来实现评价模型，而SSIM是一种自顶向下的方法，模拟的是HVS整体的功能。




