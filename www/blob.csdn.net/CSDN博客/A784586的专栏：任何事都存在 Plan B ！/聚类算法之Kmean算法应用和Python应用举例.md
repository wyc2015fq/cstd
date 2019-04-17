# 聚类算法之Kmean算法应用和Python应用举例 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年05月09日 17:44:25[QuJack](https://me.csdn.net/A784586)阅读数：414标签：[kmeans																[聚类																[机器学习算法实现](https://so.csdn.net/so/search/s.do?q=机器学习算法实现&t=blog)
个人分类：[机器学习深度学习](https://blog.csdn.net/A784586/article/category/6844322)





聚类算法之Kmean算法应用和Python应用举例

参考：http://croce.ggf.br/dados/K%20mean%20Clustering1.pdf

1. 归类：


          聚类(clustering) 属于非监督学习 (unsupervised learning)

          无类别标记(class label)




2. 举例：

![](https://img-blog.csdn.net/20170509172758540?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

3. K-means 算法：




     3.1 Clustering 中的经典算法，数据挖掘十大经典算法之一

     3.2 算法接受参数 k ；然后将事先输入的n个数据对象划分为 k个聚类以便使得所获得的聚类满足：同一

           聚类中的对象相似度较高；而不同聚类中的对象相似度较小。

     3.3 算法思想：

           以空间中k个点为中心进行聚类，对最靠近他们的对象归类。通过迭代的方法，逐次更新各聚类中心     

           的值，直至得到最好的聚类结果

     3.4 算法描述：



          （1）适当选择c个类的初始中心；

          （2）在第k次迭代中，对任意一个样本，求其到c各中心的距离，将该样本归到距离最短的中心所在     

                  的类；

          （3）利用均值等方法更新该类的中心值；

          （4）对于所有的c个聚类中心，如果利用（2）（3）的迭代法更新后，值保持不变，则迭代结束，

                   否则继续迭代。

     3.5 算法流程：

![](https://img-blog.csdn.net/20170509172803462?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

          输入：k, data[n];

          （1） 选择k个初始中心点，例如c[0]=data[0],…c[k-1]=data[k-1];

          （2） 对于data[0]….data[n], 分别与c[0]…c[k-1]比较，假定与c[i]差值最少，就标记为i;

          （3） 对于所有标记为i点，重新计算c[i]={ 所有标记为i的data[j]之和}/标记为i的个数；

          （4） 重复(2)(3),直到所有c[i]值的变化小于给定阈值。




4.举例子：

![](https://img-blog.csdn.net/20170509173129791?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170509173135604?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170509173140766?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


优点：速度快，简单

缺点：最终结果跟初始点选择相关，容易陷入局部最优，需直到k值

Python代码：

未完待续。。。。。。









优点：速度快，简单

缺点：最终结果跟初始点选择相关，容易陷入局部最优，需直到k值](https://so.csdn.net/so/search/s.do?q=聚类&t=blog)](https://so.csdn.net/so/search/s.do?q=kmeans&t=blog)




