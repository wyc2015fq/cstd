# Fast-RCNN - 李鑫o_O - CSDN博客

2017年06月05日 18:40:20[hustlx](https://me.csdn.net/HUSTLX)阅读数：358


![](https://img-blog.csdn.net/20170605182743039?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSFVTVExY/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

相比于SPP，实际上就是用了一层确定输出大小的pooling层，然后后面识别与定位一起training。

在调优训练时，每一个mini-batch中首先加入N张完整图片，而后加入从N张图片中选取的R个候选框。这R个候选框可以复用N张图片前5个阶段的网络特征。 

实际选择N=2， R=128。

对比回来SPP-Net，可以看出FRCN大致就是一个joint training版本的SPP-Net，改进如下：

SPP-Net在实现上无法同时tuning在SPP layer两边的卷积层和全连接层。

SPP-Net后面的需要将第二层FC的特征放到硬盘上训练SVM，之后再额外训练bbox regressor。

优点：

1、更高的mAP较RCNN和SPP

2、分类和定位一次性完成

3、训练的时候更新整个网络

4、不需要磁盘存储特征

