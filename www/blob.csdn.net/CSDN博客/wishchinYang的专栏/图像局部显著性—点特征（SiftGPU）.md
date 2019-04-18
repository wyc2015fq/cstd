# 图像局部显著性—点特征（SiftGPU） - wishchinYang的专栏 - CSDN博客
2017年03月27日 17:04:48[wishchin](https://me.csdn.net/wishchin)阅读数：1343
          SIFT的计算复杂度较高。
          SiftGpu的主页：[SiftGPU: A GPU Implementation of ScaleInvariant Feature Transform (SIFT)](http://www.cs.unc.edu/~ccwu/siftgpu/)
         对SIFTGPU的时间分析：[GPU_KLT: A GPU-based Implementation of the Kanade-Lucas-Tomasi Feature Tracker](http://cs.unc.edu/~ssinha/Research/GPU_KLT/)
![](https://img-blog.csdn.net/20170404104020707?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170404103919979?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170404104050082?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170404104109622?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
后记：
       使用SiftGPU编译成功，但对于图像处理，整个流程未能见得明显的时间缩短，数据拷贝占据较长时间。
       参考：  [SiftGPU：编译SiftGPU出现问题-无法解析的外部符号 glutInit](http://blog.csdn.net/wishchin/article/details/69224112)
