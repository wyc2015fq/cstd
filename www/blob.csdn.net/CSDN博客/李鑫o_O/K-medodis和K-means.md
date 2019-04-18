# K-medodis和K-means - 李鑫o_O - CSDN博客

2016年07月20日 21:01:37[hustlx](https://me.csdn.net/HUSTLX)阅读数：906
个人分类：[机器学习](https://blog.csdn.net/HUSTLX/article/category/6122182)



K-medodis看起来和K-means比较相似，但是K-medoids和K-means是有区别的，不一样的地方在于中心点的选取，在K-means中，我们将中心点取为当前cluster中所有数据点的平均值，在 K-medoids算法中，我们将从当前cluster 中选取这样一个点——它到其他所有（当前cluster中的）点的距离之和最小——作为中心点。

重点内容K-MEANS算法的缺点：产生类的大小相差不会很大，对于脏数据很敏感。

改进的算法：K-medoids方法。 

这儿选取一个对象叫做mediod来代替上面的中心的作用，这样的一个medoid就标识了这个类。 

K-MEDODIS的具体流程如下： 

1）任意选取K个对象作为medoids（O1,O2,…Oi…Ok）。　　 

2）将余下的对象分到各个类中去（根据与medoid最相近的原则）；　　 

3）对于每个类（Oi）中，顺序选取一个Or，计算用Or代替Oi后的消耗—E（Or）。选择E最小的那个Or来代替Oi。这样K个medoids就改变了。 

4）重复2、3步直到K个medoids固定下来。　　 

不容易受到那些由于误差之类的原因产生的脏数据的影响，但计算量显然要比K-means要大，一般只适合小数据量。

