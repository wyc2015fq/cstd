# SVM（支持向量机） - KinboSong的博客 - CSDN博客
2017年03月09日 09:51:20[songjinbo3](https://me.csdn.net/KinboSong)阅读数：176
个人分类：[机器学习算法](https://blog.csdn.net/KinboSong/article/category/6791887)
1、起源于“最大间隔分类器”。
2、定义“几何间隔”和“函数间隔”对问题进行建模，对原始优化问题的一步步简化成更简单可行的优化问题。
3、利用对偶和KKT条件将优化问题华为对偶优化问题。
4、利用序列最小最优（SMO）算法求解对偶问题，得到参数的最终结果。
1*：针对线性不可分的情况，提出了核（kernel）的概念，使得SVM具有非线性分类的能力
核函数的价值在于它虽然也是讲特征进行从低维到高维的转换，但核函数绝就绝在它事先在低维上进行计算，而将实质上的分类效果表现在了高维上
2*：为了提高SVM的泛化能力，使用soft margin算法，即给cost function 增加一个正则项。
综上所述：SVM =  最大间隔分类器 + kernel + soft margin
参考博客：[http://blog.csdn.net/macyang/article/details/38782399](http://blog.csdn.net/macyang/article/details/38782399)
