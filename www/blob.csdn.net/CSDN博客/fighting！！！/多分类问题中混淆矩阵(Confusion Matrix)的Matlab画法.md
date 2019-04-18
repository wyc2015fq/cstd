# 多分类问题中混淆矩阵(Confusion Matrix)的Matlab画法 - fighting！！！ - CSDN博客
2018年06月24日 11:15:25[dujiahei](https://me.csdn.net/dujiahei)阅读数：649
转自：https://www.cnblogs.com/xiangshancuizhu/archive/2011/08/04/2095533.html
在多分类问题中，有一种很实用的分类问题结果统计图。
比如说多类别文类问题，那么每一个类别分到其他类别都有一些数据，但是分到自己类别的毕竟多，这样计算百分比之后就形成了一个矩阵，如果分类正确率高的话，那么对角线上的元素的值，也就是自己到自己的那一部分，value就大。
我最近也在做多分类问题，要画这样的图，但是发现确实很少有代码，自己画的确实不好看，还牵扯到值的显示和x轴标签的旋转问题，所以一直自己也没空仔细研究，就去stackoverflow问了一下，马上就得到了答案，stackoverflow是个好站点啊，关于计算机领域等的问答系统。很多牛人在上面。
我把这个用Matlab通过分类的label计算混淆矩阵Confusion Matrix并且显示的函数做在了我的代码包中，有用的可以下载：
PG_Curve.zip: Matlab code for computing and visualization: Confusion Matrix, Precision/Recall Curve, ROC, Accuracy, F-Measure etc. for Classification.
只要一句代码就行了，方便。
[confusion_matrix]=compute_confusion_matrix(predict_label,num_in_class,name_class);
![](http://ir.sdu.edu.cn/bbs/attachments/month_1012/10121720336a39fac646afefd5.jpg)
代码地址：https://github.com/lipiji/PG_Curve            
