# GMM - 三少GG - CSDN博客
2012年01月04日 21:53:28[三少GG](https://me.csdn.net/scut1135)阅读数：868标签：[reference																[360](https://so.csdn.net/so/search/s.do?q=360&t=blog)](https://so.csdn.net/so/search/s.do?q=reference&t=blog)
个人分类：[PKU Research](https://blog.csdn.net/scut1135/article/category/801457)
Reference articles
混合高斯模型使用K（基本为3到5个）个高斯模型来表征图像中各个像素点的特征,在新一帧图像获得后更新混合高斯模型, 用当前图像中的每个像素点与混合高斯模型匹配,如果成功则判定该点为背景点, 否则为前景点。 通观整个高斯模型，主要是有方差和均值两个参数决定，对均值和方差的学习，采取不同的学习机制,将直接影响到模型的稳定性、精确性和收敛性
 。由于我们是对运动目标的背景提取建模，因此需要对高斯模型中方差和均值两个参数实时更新。为提高模型的学习能力,改进方法对均值和方差的更新采用不同的学习率;为提高在繁忙的场景下,大而慢的运动目标的检测效果,引入权值均值的概念,建立背景图像并实时更新,然后结合权值、权值均值和背景图像对像素点进行前景和背景的分类。
1. [http://apps.hi.baidu.com/share/detail/50919356](http://apps.hi.baidu.com/share/detail/50919356)
2. [http://www.opencv.org.cn/forum/viewtopic.php?f=1&t=3056](http://www.opencv.org.cn/forum/viewtopic.php?f=1&t=3056)
3. [http://hi.baidu.com/mailrabbit/blog/item/5225b0d27a7d6b38970a16cf.html](http://hi.baidu.com/mailrabbit/blog/item/5225b0d27a7d6b38970a16cf.html)
4.  [http://www.doc88.com/p-90157928048.html](http://www.doc88.com/p-90157928048.html)
5. [http://www.360doc.com/content/11/0331/09/6290398_106061327.shtml](http://www.360doc.com/content/11/0331/09/6290398_106061327.shtml)
6. [http://blog.163.com/fk1007@126/blog/static/300367392009223101913563/](http://blog.163.com/fk1007@126/blog/static/300367392009223101913563/)
7. [http://blog.csdn.net/lgt81/article/details/4419711](http://blog.csdn.net/lgt81/article/details/4419711)
