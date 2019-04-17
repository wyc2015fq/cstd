# GBDT学习总结 - 蓁蓁尔的博客 - CSDN博客





2017年08月14日 20:29:19[蓁蓁尔](https://me.csdn.net/u013527419)阅读数：580








一、基本概念

1、GBDT(Gradient Boosting Decision Tree)又叫MART(Multiple Additive Regression Tree)，GBRT(Gradient Boost Regression Tree)，Tree Net等，是一种迭代的决策树算法，与Adboost相比，GBDT也是用了前向分步算法，但是其规定了弱学习器只能是回归树，每棵树拟合的是之前所有树结果的累加和与真实值之间的残差（这也是不能用分类树的原因，类别之间的差没什么意义），累加所有树的结果作为最终结果。

2、Shrinkage （缩减）思想 

Shrinkage 认为每棵树只学到了残差的一小部分，累加的时候只累加一小部分，通过多学几棵树弥补不足。，即 

没用Shrinkage时：（yi表示第i棵树上y的预测值， y(1~i)表示前i棵树y的综合预测值） 

y(i+1) = 残差(y1~yi)， 其中： 残差(y1~yi) =  y真实值 - y(1 ~ i) 

y(1 ~ i) = SUM(y1, …, yi) 

Shrinkage不改变第一个方程，只把第二个方程改为：  

y(1 ~ i) = **y(1 ~ i-1) + step * yi**
即 Shrinkage 仍然以残差作为学习目标，但对于残差学习出来的结果，只累加一小部分（step*残差）逐步逼近目标，step一般都比较小，**如0.01~0.001（注意该step非gradient的step）**，导致各个树的残差是渐变的而不是陡变的。本质上，Shrinkage为每棵树设置了一个weight，累加时要乘以这个weight，但和Gradient并没有关系。Shrinkage 认为，每次走一小步逐渐逼近结果的效果，要比每次迈一大步很快逼近结果的方式更容易避免过拟合。不过像Adaboost一样，Shrinkage能减少过拟合发生也是经验证明的，目前还没有看到从理论的证明。

3、GBDT的输出相当于对原始特征进行了特征组合得到高阶属性或者说是非线性映射，GBDT具有天然优势可以发现多种有区分性的特征以及特征组合。

二、推导

1、李航老师《统计学习方法》第151-152页，伪代码流程，此书中没涉及到Shrinkage思想。 
![这里写图片描述](https://img-blog.csdn.net/20170814185636252?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

（1）.初始化，估计使损失函数极小化的常数值，它是只有一个根节点的树，即ganma是一个常数。 

（2）. 第1到M个决策树函数:- 
（a）对于第i个样本（共N个样本） 

     计算损失函数的负梯度在当前模型的值，将它作为残差的近似值来拟合一个回归树。

- 
（b）用一棵树拟合上面的残差，穷举每一个feature的每一个阈值，得到第m棵树的叶节点区域R_mj（用贪心算法求）。

- （c）利用线性搜索估计叶节点区域R_mj的值gamma，使损失函数极小化。
- （d）更新回归树

（3）、得到输出的最终模型 f(x)

若需要预测值输出区间在[0,1]，可作如下转换： probability=1/（1+e−2∗predict）

2、详细推到部分。可以见：[https://pan.baidu.com/s/1c2aOM2k](https://pan.baidu.com/s/1c2aOM2k)

这个文章中相比较于原先的GBDT算法，加入了一些xgboost的思想，例如加入了过拟合项，损失函数用了（平方误差和log loss都进行了推导），优化时对损失函数在当前树下进行了泰勒二阶展开用了牛顿法

并且，在实际推导过程中，缩放因子p的求解被省略了，直接默认为p^*=1,在实际应用中可以直接采用 shrinkage 的策略通过参数设置步长,避免过拟合，最终求导结果好像有一点小错误，分母应该是H+λ。

3、应用 
[CTR预估中GBDT与LR融合方案](http://blog.csdn.net/lilyth_lilyth/article/details/48032119)

文中的ID类特征是不是可以先Embedding一下，再用GBDT。
参考： 

（1）[http://blog.csdn.net/w28971023/article/details/8240756](http://blog.csdn.net/w28971023/article/details/8240756)

（2）[http://www.jianshu.com/p/005a4e6ac775](http://www.jianshu.com/p/005a4e6ac775)

（3）[http://blog.csdn.net/lc013/article/details/56667157](http://blog.csdn.net/lc013/article/details/56667157)

（4）[http://www.cnblogs.com/bentuwuying/p/6667267.html](http://www.cnblogs.com/bentuwuying/p/6667267.html)

（5）[http://blog.csdn.net/lilyth_lilyth/article/details/48032119](http://blog.csdn.net/lilyth_lilyth/article/details/48032119)






