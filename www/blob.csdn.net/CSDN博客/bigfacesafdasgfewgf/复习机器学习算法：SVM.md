# 复习机器学习算法：SVM - bigfacesafdasgfewgf - CSDN博客





2015年03月05日 10:34:18[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：1143标签：[SVM																[机器学习																[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
个人分类：[Machine Learning&&Data Mining](https://blog.csdn.net/puqutogether/article/category/2254295)







SVM的本质是寻找最大间隔的分割超平面。

SVM的假设也是在样本的每个属性前面乘以一个系数，代价函数是基于LR演化而来的。LR中是S型函数的假设，SVM中是分段函数cost的形式，再包括正则项，最后SVM的代价函数为：

![](https://img-blog.csdn.net/20150305103453291?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



当C一般要设置地很大,这样子的SVM才能够找到最大间隔的超平面，thetaT*x>=1或者thetaT*x<=-1的时候，我们可以把第一项忽略。这样，转换之后的SVM的目标函数就是我们熟悉的：



![](https://img-blog.csdn.net/20150305103501068?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


我们构建拉格朗日函数：

![](https://img-blog.csdn.net/20150305103601137?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




分别对w和b求偏导，令偏导等于0，回代，可以得到w和b的最优解，如下：

![](https://img-blog.csdn.net/20150305103622978?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




W和b都是alpha表示的，我们使用SMO算法求得alpha。





利用核函数的概念，解决数据的线性不可分问题。核函数的作用，把显示映射到高维空间转换到隐式映射到高维空间。



SVM还可以扩展到多类分类中，利用one vs. one或者one vs. other的规则。



SVM的优点：效果较好，既可以适用线性数据，也可以非线性。

缺点：对参数选择敏感。](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)](https://so.csdn.net/so/search/s.do?q=SVM&t=blog)




