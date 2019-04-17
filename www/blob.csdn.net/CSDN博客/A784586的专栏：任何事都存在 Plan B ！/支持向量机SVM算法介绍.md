# 支持向量机SVM算法介绍 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年04月24日 17:00:00[QuJack](https://me.csdn.net/A784586)阅读数：555








**支持向量机SVM算法介绍**

**1. 背景：**




     1.1 最早是由 Vladimir N. Vapnik 和 Alexey Ya. Chervonenkis 在1963年提出

     1.2 目前的版本(soft margin)是由Corinna Cortes 和 Vapnik在1993年提出，并在1995年发表

     1.3 深度学习（2012）出现之前，SVM被认为机器学习中近十几年来最成功，表现最好的算法

2. 机器学习的一般框架：

**  训练集 => 提取特征向量=> 结合一定的算法（分类器：比如决策树，KNN）=>得到结果**

3. 介绍：

     3.1 例子：

![](https://img-blog.csdn.net/20170424163555199?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



两类？哪条线最好？


 3.2 SVM寻找区分两类的超平面（hyper plane), 使边际(margin)最大
![](https://img-blog.csdn.net/20170424163654029?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




       总共可以有多少个可能的超平面？无数条

**如何选取使边际(margin)最大的超平面 (Max Margin Hyperplane)？**

      超平面到一侧最近点的距离等于到另一侧最近点的距离，两侧的两个超平面平行





3. 线性可区分(linear separable) 和 
线性不可区分 （linear inseparable) 
![](https://img-blog.csdn.net/20170424163913064?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20170424163939821?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




4. 定义与公式建立

          超平面可以定义为：

  W: weight vectot,   ,
 n 是特征值的个数

               X: 训练实例

               b: bias

![](https://img-blog.csdn.net/20170424163702632?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




4.1 假设2维特征向量：X = (x1, X2)

 把 b 想象为额外的 wight

**超平面方程**变为： 

![](https://img-blog.csdn.net/20170424164251572?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


                所有超平面右**上方**的点满足：

![](https://img-blog.csdn.net/20170424164429885?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

                所有超平面左**下方**的点满足：

![](https://img-blog.csdn.net/20170424164451260?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

               调整weight，使超平面定义边际的两边：

![](https://img-blog.csdn.net/20170424164507274?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

               综合以上两式，得到： 

![](https://img-blog.csdn.net/20170424164539808?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20170424164539808?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

              所有坐落在边际的两边的的超平面上的被称作”支持向量(support vectors)"

               分界的超平面和H1或H2上任意一点的距离为 ：**1/||W||**
  (i.e.: 其中||W||是向量的范数(norm))     

![](https://img-blog.csdn.net/20170424164836387?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

                所以，最大边际距离为：**2/||W||**






5. 求解

     5.1  SVM如何找出最大边际的超平面呢(MMH)？              

              利用一些数学推倒，以上公式 （1）可变为有限制的凸优化问题(convex quadratic optimization)

               利用 Karush-Kuhn-Tucker (KKT)条件和拉格朗日公式，可以推出MMH可以被表示为以下“决定边     

               界 (decision boundary)”    

![](https://img-blog.csdn.net/20170424165000872?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20170424165137565?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


都是单一数值型参数，由以上提到的最有算法得出 L 是支持向量点的个数.

5.2  对于任何测试（要归类的）实例，带入以上公式，得出的符号是正还是负决定。

6. 例子：

![](https://img-blog.csdn.net/20170424165513266?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20170424165559680?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





上述**例子代码实现**见相关博客。




------------------- 线性不可分的情况 ------------------------------

1.SVN算法特性

1.1 训练好的模型的算法复杂度是由支持向量的个数决定的，而不是由数据的维度决定的。所以SVM不太容易产生overfitting
     1.2 SVM训练出来的模型完全依赖于支持向量(Support Vectors), 即使训练集里面所有非支持向量的点都被去除，重复训练过程，结果仍然会得到完全一样的模型。

     1.3 一个SVM如果训练得出的支持向量个数比较小，SVM训练出的模型比较容易被泛化。


2.**线性不可分的情况** （linearly inseparable case)


2.1 数据集在空间中对应的向量不可被一个超平面区分开

2.2 两个步骤来解决：
2.2.1 利用一个非线性的映射把原数据集中的向量点转化到一个更高维度的空间中

2.2.2 在这个高维度的空间中找一个线性的超平面来根据线性可分的情况处理


![](https://img-blog.csdn.net/20170425155257287?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170425155432835?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170425155440483?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


































