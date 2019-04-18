# Haar、pico、npd、dlib等多种人脸检测特征及算法结果比较 - wishchinYang的专栏 - CSDN博客
2017年05月04日 11:44:33[wishchin](https://me.csdn.net/wishchin)阅读数：3864
## 原文：ope[ncv、pico、npd、dlib、face++等多种人脸检测算法结果比较](http://www.thinkface.cn/thread-4459-1-1.html)
![](https://img-blog.csdn.net/20170504113504326?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**NDP检测结果**：
![](https://img-blog.csdn.net/20170509114854951?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
结果分析：
Pico（Pixel Intensity Comparison-based Object detection）发表于2014年，它也继承于Viola-Jones算法并对其做了一部分改进，最大的不同在于特征提取方式，不同于Viola-Jones的Haar特征，pico则是提取点对特征，对两个像素点进行对比。实验表明这种特征比Haar特征更为有效，且运算时间更短。
这就意味着PICO的抗噪性能极差，场景可扩展性不强。
NDP特征有以下几个特点：
- 其特征是反对称的，也就是说 f (x, y) 或者f (y, x) 都可以表述 x 和 y 两点的特征，举个例子来说，对于一张 p = h*w 大小的图片，其特征池大小为 p * (p-1)/2 。
- 其特征是有符号的，也就是说其特征表述是有方向性的。
- 其特征是尺度鲁棒的， 也就是说由于其特征分子是两像素点差值，所以对于光照具有较强的鲁棒性。
- 其特征值是归一化的。
最后指出，通过特征池是可以重建出原图的，也就是说特征池包含了原图片中的所有信息。
        重要参考文章：[NPD实现及其与pico一脉相承的关系](http://blog.csdn.net/u010333076/article/details/51397332)
        此文详细的介绍了PICO和NDP的理论和实验，以及递进关系。赞一个！！！
        GitHub代码下载：[https://github.com/wincle/NPD](https://github.com/wincle/NPD)
~~ NDP的官网及代码下载：[http://www.cbsr.ia.ac.cn/users/scliao/projects/npdface/](http://www.cbsr.ia.ac.cn/users/scliao/projects/npdface/)~~
