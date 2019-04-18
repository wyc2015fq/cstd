# 逻辑回归（LR） - wsp_1138886114的博客 - CSDN博客





2018年07月18日 11:30:04[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：151










- - - [一、概念](#一概念)
- [二 构造sigmoid函数 及最大似然函数](#二-构造sigmoid函数-及最大似然函数)
- [三 损失函数](#三-损失函数)- - [3.1 损失函数可以大致分为两类：](#31-损失函数可以大致分为两类)
- [3.2 MSE 、MAE模型选择分析](#32-mse-mae模型选择分析)
- [3.3 Huber Loss，平滑的平均绝对误差](#33-huber-loss平滑的平均绝对误差)
- [3.4 Log-Cosh Loss](#34-log-cosh-loss)
- [3.5 Quantile Loss（分位数损失）](#35-quantile-loss分位数损失)
- [3.6 sklearn库 GradientBoostingRegression中的quantile loss](#36-sklearn库-gradientboostingregression中的quantile-loss)


- [四 总结](#四-总结)






### 一、概念
- 线性回归
例如 $y=f(x)=ax+b$，对于每一个输入的x，都有一个对应的y输出。输出一般是连续的。  

模型的定义域和值域都可以是$[-∞, +∞]$。- 逻辑回归
输入可以是连续的 $[-∞, +∞]$ 但输出一般是离散的，即只有有限多个输出值。 
![这里写图片描述](https://img-blog.csdn.net/20180718100726844?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 二 构造sigmoid函数 及最大似然函数

![这里写图片描述](https://img-blog.csdn.net/20180718100349768?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180718110438555?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180718111659657?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180718112552827?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180718112716914?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180801105031876?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 三 损失函数

```
机器学习中的所有算法都依赖于最小化或最大化某一个函数，我们称之为“目标函数”。  

最小化的这组函数被称为“损失函数”。 损失函数是衡量预测模型预测期望结果表现的指标。 

寻找函数最小值的最常用方法是“梯度下降”。 
    把损失函数想象成起伏的山脉，梯度下降就像从山顶滑下，目的是到达山脉的最低点。 

没有一个损失函数可以适用于所有类型的数据。损失函数的选择取决于许多因素，包括是否有离群点， 
机器学习算法的选择，运行梯度下降的时间效率，是否易于找到函数的导数，以及预测结果的置信度。
```

##### 3.1 损失函数可以大致分为两类：

分类损失（Classification Loss）和 回归损失（Regression Loss）。 
|分类损失|回归损失|
|----|----|
|log loss (对数损失)|Mean Square error /quadratic   loss   (均方误差/二次损失)|
|focal loss （焦点损失）|Mean Absolute Error (平均绝对误差)|
|exponential loss （指数损失）|Huber Loss/smooth Mean Absolute Error (胡贝尔损失/平滑平均绝对误差)|
|Hinge loss （铰链损耗）|log cosh loss (对数余弦损耗)|
|KL Dibergence/Relative  entropy（KL微分/相对熵）|Quantile loss(分位数损失)|
- 均方误差（MSE）L2损失

最常用的回归损失函数。MSE使目标变量与预测值之间距离平方之和。 


$MSE = \frac{\sum_{i=1}^{n}(y_i-y_i^p)^2}{n}$
- 平均绝对误差，L1损失（MAE, L1 Loss）

平均绝对误差（MAE）是另一种用于回归模型的损失函数。  

MAE是目标变量和预测变量之间差异绝对值之和。  

因此，它在一组预测中衡量误差的平均大小，而不考虑误差的方向。（如果我们也考虑方向，那将被称为平均偏差（Mean Bias Error, MBE），它是残差或误差之和）。损失范围也是$0$ 到 $∞$。  


$MAE = \frac{\sum_{i=1}^{n}|y_i-y_i^p|}{n}$
![这里写图片描述](https://img-blog.csdn.net/20180718171005675?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 3.2 MSE 、MAE模型选择分析

```
MSE（L2损失）为损失的模型会赋予更高的权重给离群点。牺牲其他正常数据点的预测效果，最终会降低模型的整体性能。 
    L2损失对异常值敏感，但给出了更稳定的闭式解（closed form solution）（通过将其导数设置为0）  

MAE（L1损失）损失适用于训练数据被离群点损坏的时候 （即，在训练数据而非测试数据中，我们错误地获得了不切实际的过大正值或负值）。  
    L1损失对异常值更加稳健，但其导数并不连续，因此求解效率很低。

直观来说，我们可以像这样考虑：对所有的观测数据，如果我们只给一个预测结果来最小化MSE， 
    那么该预测值应该是所有目标值的均值。但是如果我们试图最小化MAE，那么这个预测就是所有目标值的中位数。 
    我们知道中位数对于离群点比平均值更鲁棒，这使得MAE比MSE更加鲁棒。 

使用MAE损失（特别是对于神经网络）的一个大问题是它的梯度始终是相同的，这意味着即使对于小的损失值，其梯度也是大的。 
    这对模型的学习可不好。为了解决这个问题，我们可以使用随着接近最小值而减小的动态学习率。 
    MSE在这种情况下的表现很好，即使采用固定的学习率也会收敛。MSE损失的梯度在损失值较高时会比较大， 
    随着损失接近0时而下降，从而使其在训练结束时更加精确。
```

##### 3.3 Huber Loss，平滑的平均绝对误差
- Huber Loss对数据离群点的敏感度低于平方误差损失
它在0处也可导。基本上它是绝对误差， 当误差很小时，误差是二次形式的。误差何时需要变成二次形式取决于一个超参数：delta。
该超参数可以进行微调。当 $\delta $ ~ 0时， Huber Loss接近MAE，  

当 $\delta $ ~ $∞$（很大的数）时，Huber Loss接近 MSE。 

delta 的选择非常重要，因为它决定了你认为什么数据是离群点。大于delta的残差用L1最小化（对较大的离群点较不敏感），  

而小于delta的残差则可以“很合适地”用L2最小化。 

![这里写图片描述](https://img-blog.csdn.net/2018071817390712?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 训练神经网络时：
使用MAE训练神经网络的一个大问题是经常会遇到很大的梯度，使用梯度下降时可能导致训练结束时错过最小值。
对于MSE，梯度会随着损失接近最小值而降低，从而使其更加精确。 

在这种情况下，Huber Loss可能会非常有用，因为它会使最小值附近弯曲，从而降低梯度。  

另外它比MSE对异常值更鲁棒。因此，它结合了MSE和MAE的优良特性。  

但是，Huber Loss的问题是我们可能需要迭代地训练超参数delta。  
##### 3.4 Log-Cosh Loss

Log-cosh是用于回归任务的另一种损失函数，它比L2更加平滑。Log-cosh是预测误差的双曲余弦的对数。  
![这里写图片描述](https://img-blog.csdn.net/20180718175807297?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- 优点：
log(cosh(x))对于小的x来说，其大约等于 $\frac{x^2}{2}$，而对于大的 x 来说，  

其大约等于 abs(x) - log(2)。这意味着’logcosh’的作用大部分与均方误差一样，但不会受到偶尔出现的极端不正确预测的强烈影响。它具有Huber Loss的所有优点，和Huber Loss不同之处在于，其处处二次可导。
##### 3.5 Quantile Loss（分位数损失）

```
当我们有兴趣预测一个区间而不仅仅是预测一个点时，Quantile Loss函数就很有用。 
最小二乘回归的预测区间是基于这样一个假设：残差（y - y_hat）在独立变量的值之间具有不变的方差。 
我们不能相信线性回归模型，因为它违反了这一假设。 

当然，我们也不能仅仅认为这种情况一般使用非线性函数或基于树的模型就可以更好地建模， 
而简单地抛弃拟合线性回归模型作为基线的想法。这时，Quantile Loss就派上用场了。 

因为基于Quantile Loss的回归模型可以提供合理的预测区间，即使是对于具有非常数方差或非正态分布的残差亦是如此。
```

![这里写图片描述](https://img-blog.csdn.net/20180718181829375?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
基于Quantile回归的目的是，在给定预测变量的某些值时，估计因变量的条件“分位数”。 
Quantile Loss(分位数损失 )实际上只是MAE的扩展形式（当分位数是第50个百分位时，Quantile Loss退化为MAE）。
```

##### 3.6 sklearn库 GradientBoostingRegression中的quantile loss

(梯度提升回归分位数损失函数计算)  
![这里写图片描述](https://img-blog.csdn.net/20180718184404607?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180718185424709?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- 模拟实验中的一些观察结果：
以MAE为损失的模型预测较少受到脉冲噪声的影响，而以MSE为损失的模型的预测由于脉冲噪声造成的数据偏离而略有偏差。 

以Huber Loss为损失函数的模型，其预测对所选的超参数不太敏感。 

Quantile Loss对相应的置信水平给出了很好的估计。
### 四 总结

```
Logistic回归与多重线性回归实际上有很多相同之处，最大的区别就在于它们的因变量不同， 
其他的基本都差不多。正是因为如此，这两种回归可以归于同一个家族，即广义线性模型（generalizedlinear model）。
这一家族中的模型形式基本上都差不多，不同的就是因变量不同。这一家族中的模型形式基本上都差不多， 不同的就是因变量不同。 

如果是连续的，就是多重线性回归
如果是二项分布，就是Logistic回归
如果是Poisson分布，就是Poisson回归
如果是负二项分布，就是负二项回归
```








