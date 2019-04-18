# 机器学习——支持向量机（SVM） - wsp_1138886114的博客 - CSDN博客





2018年05月28日 20:47:56[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：207
所属专栏：[机器学习算法](https://blog.csdn.net/column/details/24447.html)











- - - [一、SVM 概念](#一svm-概念)
- [二、支持向量机算法基本原理](#二支持向量机算法基本原理)- - [2.1  线性 SVM](#21-线性-svm)
- [2.1 非线性（SVM）超平面](#21-非线性svm超平面)
- [2.2 超平面公式推导](#22-超平面公式推导)- [超平面之间的距离](#超平面之间的距离)
- [样本点到超平面之间的距离](#样本点到超平面之间的距离)
- [样本的正确分类 - 拉格朗日方法(对偶算法)：](#样本的正确分类-拉格朗日方法对偶算法)

- [2.3 拉格朗日乘子法-超平面推论](#23-拉格朗日乘子法-超平面推论)
- [2.4 示例：拉格朗日乘子法求超平面](#24-示例拉格朗日乘子法求超平面)
- [2.5 松弛因子](#25-松弛因子)


- [三、核函数](#三核函数)- - [3.1 核函数定义](#31-核函数定义)
- [3.2 常用核函数](#32-常用核函数)
- [3.3 核矩阵](#33-核矩阵)


- [三、支持向量机代码演示](#三支持向量机代码演示)
- [四、支持向量机参数优化](#四支持向量机参数优化)
- [五、支持向量机总结](#五支持向量机总结)






### 一、SVM 概念
- 将样本的两种特征用 一条直线或者超平面分开，并且间隔最大。
- 非线性问题，因为其复杂性，需要使用复杂模型，参数多，容易过拟合，而 SVM既能解决复杂问题，又不容易过拟合(但不能保证不会过拟合) 
![这里写图片描述](https://img-blog.csdn.net/20180705173915441?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 二、支持向量机算法基本原理

##### 2.1  线性 SVM

```
线性（SVM）：找到最好的决策边界
最大化 Margin: 决策边界最近的距离
最小的Margin之和最大化
```


![这里写图片描述](https://img-blog.csdn.net/20180527172922354?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


##### 2.1 非线性（SVM）超平面

```
低维映射到高维再处理，找到最优的（核方法）
    一条直线方程，其中m是斜率，c是直线在y轴的截距：y= mx +c 
    二维空间里面，一条直线的方程可以表示为：Ax+By+C=0
    三维空间里面，平面的方程可以表示为：Ax+By+Cz+D=0
    那么超平面方程：
```



$w^T x = 0  也可以写作（w^T·x+b=0）$

```
其中 w 和 x 是向量，w^T是两个向量的点积。向量w通常被称为权重。
w , x皆为向量， wx+b=0就是a1*x1+a2*x2+...an*xn+b=0
```

##### 2.2 超平面公式推导

###### 超平面之间的距离

![这里写图片描述](https://img-blog.csdn.net/20180714202143782?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

###### 样本点到超平面之间的距离

![这里写图片描述](https://img-blog.csdn.net/2018071318331660?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

###### 样本的正确分类 - 拉格朗日方法(对偶算法)：
- **约束最优化问题**

对于线（w,b）可以通过缩放使得其结果值|y|>=1 $~~~$$y_i(w^ T\Phi (x_i)+b)\geqslant 1$


$arg max_{w,b} \left \{ \frac{1}{{\left \| w \right \|}}min_i[y_i(w^ T\Phi (x_i)+b)] \right \}$

即（目标函数）：$arg max_{w,b}\frac{1}{{\left \| w \right \|}}$ 且 $y_i(w^ T\Phi (x_i)+b)\geqslant 1$
$~~~~~~转换成求最小值：arg min_{w,b}\frac{1}{2}w^2$ 且 $y_i(w^ T\Phi (x_i)+b)\geqslant 1$
- **拉格朗日乘子法标准格式：**
$min f(x)$
$s.t  g_i(x) \leqslant  0  ,i = 1,2...m$- **样本正确分类(拉格朗日方法)：**
- $f(x) = w^Tx + b$
- 构造拉格朗日方程： 
$s.t.y_i( w^Tx + b) \geq 1,~~~i = 1,2,3...m$
$g_i(x) =1- y_i( w^Tx_i + b) \leq 0,~~~i = 1,2,3...m$
$~~~~~~~~~~~~~~~~~~~~~L(w,b,\alpha) = \frac{1}{2} || w||^2+\sum_{i=1}^{m}\alpha_i(1- y_i( w^Tx_i + b))$

##### 2.3 拉格朗日乘子法-超平面推论

![对偶算法-1](https://img-blog.csdn.net/20180713191016202?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![对偶算法-2](https://img-blog.csdn.net/20180713191022724?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![对偶算法-3](https://img-blog.csdn.net/20180713191215357?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![对偶算法-4](https://img-blog.csdn.net/20180713191346853?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![对偶算法-5](https://img-blog.csdn.net/20180713191401614?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
##### 2.4 示例：拉格朗日乘子法求超平面

例题：已知如图训练集: 

正例点$x_1=(3,3)^T,x_2=(4,3)^T,负例点 x_3=(1,1)^T$。  

试求最大间隔分离超平面。  
![这里写图片描述](https://img-blog.csdn.net/20180713191602186?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![对偶算法-6](https://img-blog.csdn.net/20180713191610983?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![对偶算法-7](https://img-blog.csdn.net/20180713191634641?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
##### 2.5 松弛因子

![这里写图片描述](https://img-blog.csdn.net/20180714210408591?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180714210423878?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180714211010380?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```
在 SVM 中设定一个参数「C」；从而你可以在两种结果中权衡：
    1. 拥有很宽的间隔；
    2. 精确分离训练数据；
C 的值越大，意味着在训练数据中允许的误差越少。
必需强调一下这是一个权衡的过程。如果想要更好地分类训练数据，那么代价就是间隔会更宽。 
以下几个图展示了在不同的 C 值中分类器和间隔的变化（未显示支持向量）。
```

![这里写图片描述](https://img-blog.csdn.net/20180714212455111?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 三、核函数
- 核函数特点
如果数据集中有 n 个点，SVM 只需要将所有点两两配对的点积以寻找分类器。仅此而已。  

当我们需要将数据映射到高维空间的时候也是这样，**不需要向 SVM 提供准确的映射**，  
**而是提供映射空间中所有点两两配对的点积**。
##### 3.1 核函数定义

```
假设X是输入空间，H是特征空间，存在一个映射ϕ使得X中的点x能够计算得到H空间中的点h ，对于所有的X中的点都成立:
```



$h=\phi(x)$

```
若x，z是X空间中的点，函数k(x,z)满足下述条件，那么都成立，则称k为核函数，而ϕ为映射函数：
```



$k(x,z)=\phi (x)\cdot \phi (z)$

##### 3.2 常用核函数
- 线性核
线性核，主要用于线性可分的情况，我们可以看到特征空间到输入空间的维度是 

一样的，其参数少速度快，对于线性可分数据，其分类效果很理想。  


$K(x_i,x_j) = x_i^T,x_j$- 多项式核
将低维的输入空间映射到高纬的特征空间，但是多项式核函数的参数多，当多项式的阶数比较高的时候，核矩阵的元素值将趋于无穷大，计算复杂度会大到无法计算。（d≥1为多项式的次数）  


$K(x_i,x_j) = (x_i^T,x_j)^d$- 高斯（RBF）核函数
高斯径向基函数是一种局部性强的核函数，可以将一个样本映射到一个更高维的空间内，该核函数是应用最广的一个，无论大样本还是小样本都有比较好的性能，(相对于多项式核函数参数要少)。 

因此大多数情况下在不知道用什么核函数的时候，优先使用高斯核函数多项式核。  
$(\sigma $>0 为高斯核的宽带)  


$K(x_i,x_j) = exp(- \frac{||x_i^T,x_j||^2}{2\sigma^2})$
![这里写图片描述](https://img-blog.csdn.net/20180714223829709?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- sigmoid核函数
采用sigmoid核函数，支持向量机实现的就是一种多层神经网络。  

（tanh为双曲正切函数，β>0,θ<0）  


$K(x_i,x_j) = tanh(βx_i^Tx_j+θ)$- 核函数选择依据

如果特征的数量大到和样本数量差不多，则选用LR或者线性核的SVM； 

如果特征的数量小，样本的数量正常，则选用SVM+高斯核函数； 

如果特征的数量小，而样本的数量很大，则需要手工添加一些特征从而变成第一种情况。 
![这里写图片描述](https://img-blog.csdn.net/20180714232631524?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180714232648753?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
##### 3.3 核矩阵

![这里写图片描述](https://img-blog.csdn.net/20180714225059110?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**对于一个二位空间 映射到 三维空间**:$P(x,y)=(x^2,\sqrt{2}xy,y^2)$
![这里写图片描述](https://img-blog.csdn.net/20180705172127795?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

考虑到核函数:$K(v1,v2)=<v1,v2>^2，$即“内积平方” 

设二维空间存在：v1=(x1,y1),v2=(x2,y2)v1=(x1,y1),v2=(x2,y2)v1=(x_1,y_1)  , v2=(x_2,y_2) 两点: 

可证 
![这里写图片描述](https://img-blog.csdn.net/20180616104546977?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)三、[](https://blog.csdn.net/wsp_1138886114/article/details/81054209)支持向量机代码演示``from sklearn.svm.import SVC
svc = SVC(
            C = 1.0,
            lernel = 'rbf',
            degree = 3,
            gamma = 'auto',
            coef0 = 0.0,
            shrinking = True,
            probability = False,
            tol = 0.001,
            cache_size = 200,
            class_weight = None,
            verbose = False,
            max_iter = -1,
            decision_function_shape = None,
            random_state = None
        )
一些重要的参数：
C  --误差项惩罚参数，C越大，越容易过拟合
kernel  --核参数，'linear','poly','rbf','sigmoid'
gamma   --当kernel为'poly','rbf','sigmoid'时，默认1/n_feature四、支持向量机参数优化
```bash

```
parameters = {
               'C':[0.001,0.01,0.1,1,10,1000],
               'kernel':['linear','poly','rbf','sigmoid']
               'gamma':[0.0001,0.001]
             }
svm= SVC()
grid_search = GridSearchCV(svm,parameters,scoring = 'accuracy',cv = 5)
grid_search.fit(x,y)五、支持向量机总结``支持向量机是一个‘超平面分类算法’
最佳超平面-->支持向量Margin（间隔）最大的超平面
支持向量就是离超平面最近的数据点
数据低维--kernel() -->高维,使其线性分开
SVM 主要参数调优：C，gamma，kernel
SVM只支持数值型变量，分类型变量-->onehot编码
SVM对缺失值敏感，需提取处理


![这里写图片描述](https://img-blog.csdn.net/20180616105134801?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

SVM没有处理缺失值的策略（决策树有）。而SVM希望样本在特征空间中线性可分，所以特征空间的好坏对SVM的性能很重要。缺失特征数据将影响训练结果的好坏。













