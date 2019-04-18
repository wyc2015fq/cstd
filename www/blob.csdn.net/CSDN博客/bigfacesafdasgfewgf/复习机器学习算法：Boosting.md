# 复习机器学习算法：Boosting - bigfacesafdasgfewgf - CSDN博客





2015年03月05日 10:40:33[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：1075标签：[Boosting																[机器学习																[集成学习](https://so.csdn.net/so/search/s.do?q=集成学习&t=blog)
个人分类：[Machine Learning&&Data Mining](https://blog.csdn.net/puqutogether/article/category/2254295)







Boosting的思想是集成学习，把许多个弱分类器结合起来，构成一个强分类器。



首先输入原始的训练样本，得到一个弱分类器，可以知道它的正确率和错误率。计算该弱分类器的权重，如下：

![](https://img-blog.csdn.net/20150305103852039?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




然后提高错误分类样本的权重，让后面的分类器focus它们，调整样本的权重：

如果原本分类正确：![](https://img-blog.csdn.net/20150305103918154?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

如果样本分类错误：![](https://img-blog.csdn.net/20150305103942182?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



把新的样本输入到后面学习，重复这个过程，得到许多个弱分类器，及其分类器的权重。



注意，Boosting算法中有两个权重，一个是分类器的权重，一个是样本的权重。





Boosting算法的优点：性能好，可以防止过拟合，可以综合多个分类器的优势。

缺点：对离群点比较敏感。](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)](https://so.csdn.net/so/search/s.do?q=Boosting&t=blog)




