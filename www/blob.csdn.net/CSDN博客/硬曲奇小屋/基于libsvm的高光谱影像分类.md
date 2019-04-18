# 基于libsvm的高光谱影像分类 - 硬曲奇小屋 - CSDN博客





2017年08月29日 20:31:10[HardCookies](https://me.csdn.net/james_616)阅读数：756








近两周来，一直在看关于高光谱影像分类及目标检测的文章。仅仅是看，感觉并没有真正学会什么，不如自己动手实现一下。所以，从最简单的开始，用libsvm来实现高光谱影像的分类。 

高光谱数据是来自ROSIS-03的[Pavia Univeisity数据](http://www.ehu.eus/ccwintco/index.php?title=Hyperspectral_Remote_Sensing_Scenes%20%E4%B8%8B%E8%BD%BD%E5%9C%B0%E5%9D%80)，共103个通道，包括9种地物类别。由PCA(主成分分析)得到的三个主成分组成的假彩色图像如下图所示： 

![pseudocolor](https://img-blog.csdn.net/20170829202436901?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



数据同时还提供了地面真实值，如下图所示： 


![groundtruth](https://img-blog.csdn.net/20170829202102325?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



从这些真实中选出一些构成训练集，剩下的作为测试集。我从各个类别中随机选出500个样本点最为训练集，用剩余的验证得到精度为73.8217%。然后用训练得到的model来对整个数据进行分类，得到以下分类结果。 


![classified](https://img-blog.csdn.net/20170829202815227?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



可以看出有些地物分类的正确率较高，有些则不行，可能是跟选取的训练样本数目有关。下面我将进一步研究如何提高SVM的分类正确率。




