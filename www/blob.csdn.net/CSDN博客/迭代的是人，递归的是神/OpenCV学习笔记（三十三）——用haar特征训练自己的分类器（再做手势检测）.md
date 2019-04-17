# OpenCV学习笔记（三十三）——用haar特征训练自己的分类器（再做手势检测） - 迭代的是人，递归的是神 - CSDN博客





2011年12月03日 09:40:36[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：53542
所属专栏：[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html)









之前介绍过一篇利用级联分类器对目标进行检测的文章[http://blog.csdn.net/yang_xian521/article/details/6973667](http://blog.csdn.net/yang_xian521/article/details/6973667)，用的就是haar特征。发现OpenCV自带的库里的haar特征只有人脸、人脸的器官和人的身体，最近又想玩一个人手的检测，之前用颜色特征做的，感觉很不靠谱，这次用haar特征再试一次。这就需要用haartraining这个工具训练自己的手。先介绍一些预备知识，推荐个网址[http://www.opencv.org.cn/index.php/%E7%89%B9%E5%BE%81%E6%A3%80%E6%B5%8B%E4%B8%93%E9%A2%98](http://www.opencv.org.cn/index.php/%E7%89%B9%E5%BE%81%E6%A3%80%E6%B5%8B%E4%B8%93%E9%A2%98)，读完相信对haar特征来龙去脉有个认识了，具体怎么使用，推荐看看这个[http://note.sonots.com/SciSoftware/haartraining/document.html](http://note.sonots.com/SciSoftware/haartraining/document.html)，再推荐这个网址[http://note.sonots.com/SciSoftware/haartraining.html](http://note.sonots.com/SciSoftware/haartraining.html)，都是英文哦，我就是按照这个英文介绍的教程训练自己的手分类器的。后来发现有人已经做了这个教程的翻译[http://blog.csdn.net/onlyyouandme/article/details/4722160](http://blog.csdn.net/onlyyouandme/article/details/4722160)和[http://blog.csdn.net/onlyyouandme/article/details/4722202](http://blog.csdn.net/onlyyouandme/article/details/4722202)（还是看英文原文比较详细），我也参考了这个[http://hi.baidu.com/andyzcj/blog/item/3b9575fc63c3201f09244d9a.html](http://hi.baidu.com/andyzcj/blog/item/3b9575fc63c3201f09244d9a.html)，都贴上来以备以后再训练时学习需要。训练过程相当痛苦漫长，累死我心爱的PC了。由于训练数据不是我的个人财富，所以不便上传，这里把我download的一个老外训练的拳头的手势分类器（拳头在英文手语里表示字母A）作为实验来源。

资料还是得看啊，又读了经典文献《Robust Real-Time Face Detection》，不愿意读原文的朋友可以看看[http://blog.csdn.net/hqw7286/article/details/5556767](http://blog.csdn.net/hqw7286/article/details/5556767)，作者把文中的要点基本也都总结出来了。OpenCV的实现过程也是在这篇文章的基础上，后来又不断完善的。

自己跟踪了一下代码，发现OpenCV的级联分类器的分为老版本和新版本，所有的haar级联分类器都是老版本的，只有一个lbp分类器是新版本的，而老版本的级联分类器的训练检测还是用老版本的数据结构来写的（让我很不爽，真想变得强大起来，用新数据结构写一下），为了这个新版本的级联分类器，多添加了大量的代码，可是用haartraining训练出来的分类器也是老版本的，该如何添加新版本的级联分类器啊，期待下一版本的OpenCV能够用新版本的haar级联分类器替代老版本。从这段代码中，我也深深体会到版本兼容的辛酸了。再这里也默默祝OpenCV越来越好，更规整，更强大。

最后上传一下效果图，再上传一段录制的视频（上镜了，很挫）[http://v.youku.com/v_show/id_XMzI4NTQ1OTQ4.html](http://v.youku.com/v_show/id_XMzI4NTQ1OTQ4.html)和代码下载地址[http://download.csdn.net/detail/yang_xian521/3873942](http://download.csdn.net/detail/yang_xian521/3873942)

![](http://hi.csdn.net/attachment/201112/3/0_1322875942yBM9.gif)




