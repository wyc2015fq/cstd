# 集成学习——Xgboost - wsp_1138886114的博客 - CSDN博客





2018年07月13日 14:04:15[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：482










- - - [一、集成学习](#一集成学习)- - [1.1 目标函数](#11-目标函数)
- [1.2 函数最优解](#12-函数最优解)


- [二. Xgboost的优势：](#二-xgboost的优势)
- [三、Xgboost 与 GBDT 的区别与联系](#三xgboost-与-gbdt-的区别与联系)- - [与GBDT的区别：](#与gbdt的区别)
- [代码实现](#代码实现)








### 一、集成学习

##### 1.1 目标函数

![这里写图片描述](https://img-blog.csdn.net/20180713104101830?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 1.2 函数最优解

预测过程： 
![这里写图片描述](https://img-blog.csdn.net/201807131130014?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180713130929435?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

考虑平方损失：  
![这里写图片描述](https://img-blog.csdn.net/20180713122720394?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180713140318138?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180713140333302?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180713140348869?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 二. Xgboost的优势：

```
1、正则化
    标准GBM的实现没有像XGBoost这样的正则化步骤。正则化对减少过拟合也是有帮助的。
    实际上，XGBoost以“正则化提升(regularized boosting)”技术而闻名。

2、并行处理
    XGBoost可以实现并行处理，相比GBM有了速度的飞跃。 
    LightGBM也是微软最新推出的一个速度提升的算法。  
    XGBoost也支持Hadoop实现。

3、高度的灵活性
    XGBoost 允许用户定义自定义优化目标和评价标准 。

4、缺失值处理
    XGBoost内置处理缺失值的规则。用户需要提供一个和其它样本不同的值，然后把它作为一个参数传进去， 
    以此来作为缺失值的取值。 
    XGBoost在不同节点遇到缺失值时采用不同的处理方法，并且会学习未来遇到缺失值时的处理方法。

5、剪枝
    当分裂时遇到一个负损失时，GBM会停止分裂。因此GBM实际上是一个贪心算法。

    XGBoost会一直分裂到指定的最大深度(max_depth)，然后回过头来剪枝。 
    如果某个节点之后不再有正值，它会去除这个分裂。
    这种做法的优点，当一个负损失（如-2）后面有个正损失（如+10）的时候，就显现出来了。 
    GBM会在-2处停下来，因为它遇到了一个负值。但是XGBoost会继续分裂，然后发现这两个分裂综合起来会得到+8， 
    因此会保留这两个分裂。

6、内置交叉验证
    XGBoost允许在每一轮boosting迭代中使用交叉验证。因此，可以方便地获得最优boosting迭代次数。
    而GBM使用网格搜索，只能检测有限个值。

7、在已有的模型基础上继续
    Xgboost可以在上一轮的结果上继续训练。
    sklearn中的GBM的实现也有这个功能，两种算法在这一点上是一致的
```

### 三、Xgboost 与 GBDT 的区别与联系

```
Xgboost 是华人大牛陈天奇开发的GB（Gradient Boosting）算法的高效实现
（CPU的多线程并行计算）。https://github.com/dmlc/xgboost
```

##### 与GBDT的区别：

```
XGBoost的基学习器 可以是CART（这个时候就是GBDT）也可以是线性分类器
XGBoost 在代价函数中加入了正则项，用于控制模型复杂度。
XGBoost 借鉴了随机森林的做法，支持特征抽样，不仅防止过拟合，还能减少计算
XGBoost 工具支持并行化
-----综上 XGBoost 的运算速度和精度都会优于GBDT

XGboost 目前 scikit-learn 中没有实现，自行安装，通过python调用
        百度搜索 xgboost python 安装
```

##### 代码实现

![这里写图片描述](https://img-blog.csdn.net/20180603103044437?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180603103101912?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




