
# 极限学习机︱R语言快速深度学习进行回归预测 - 素质云笔记-Recorder... - CSDN博客

2016年08月06日 09:19:03[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：4077



本文转载于张聪的博客,链接：https://ask.hellobi.com/blog/zason/4543。


深度学习在过去几年，由于卷积神经网络的特征提取能力让这个算法又火了一下，其实在很多年以前早就有所出现，但是由于深度学习的计算复杂度问题，一直没有被广泛应用。
一般的，卷积层的计算形式为：
![Clipboard Image.png](https://o6v7e1w4j.qnssl.com/uploads/article/20160801/709a891f77528bcec41f9172d96b6290.png)
其中、x分别表示当前卷积层中第j个特征、前一层的第i个特征；k表示当前层的第j个特征与前一层的第i个特征之间的卷积核；M表示需要卷积的前一层的特征的集合，b表示当前卷积层中第j个卷积核对应的偏置。f为激活函数。
卷积层中的权值与阈值通过随机梯度下降法得到：
![Clipboard Image.png](https://o6v7e1w4j.qnssl.com/uploads/article/20160801/274d81b5e725532733788f8bce9f0e37.png)
式中，a为学习率。
损失函数对卷积层参数的梯度可通过链式求导来得到，如下：
![Clipboard Image.png](https://o6v7e1w4j.qnssl.com/uploads/article/20160801/09631e98b3e9c1b0b59bc2b5a617ecc1.png)
式中，![Clipboard Image.png](https://o6v7e1w4j.qnssl.com/uploads/article/20160801/3ec777502d43e7d2a87336a60c1b9351.png)表示前一层的梯度。
卷积神经网络中的激活函数有多种形式：
![Clipboard Image.png](https://o6v7e1w4j.qnssl.com/uploads/article/20160801/32a536b273b870faf822236571cede68.png)

式中a为固定的参数。
![Clipboard Image.png](https://o6v7e1w4j.qnssl.com/uploads/article/20160801/3acf33cb663b9af360bbdf9055a0b4dd.png)
式中![Clipboard Image.png](https://o6v7e1w4j.qnssl.com/uploads/article/20160801/bbb721b282018ed427cc535a9c213f7b.png)，每个batch训练样本中的都随机采样自均值分布，在测试中取![Clipboard Image.png](https://o6v7e1w4j.qnssl.com/uploads/article/20160801/dcd6be236baf4723ca4afee6d15f23a1.png)。
![Clipboard Image.png](https://o6v7e1w4j.qnssl.com/uploads/article/20160801/e7e6550b6fcfae0423ce025dede3dc9d.png)
从上述卷积神经网络看出，学习过程中需要进行梯度迭代，真正在实现工业检测等实际应用时时间复杂度极高，因此学术界进行了优化，优化后的一种单层神经网络极限学习机解决了此问题，在过去应用十分广泛。
为解决上述问题，出现了极限学习机。
用最小二乘法解决的一种特殊结果为，等价为一种矩阵求逆的形式
![Clipboard Image.png](https://o6v7e1w4j.qnssl.com/uploads/article/20160801/9345e1b93243ba4eb68784693c01792c.png)
![Clipboard Image.png](https://o6v7e1w4j.qnssl.com/uploads/article/20160801/7ad5a17a73d598dea06b2046c43b042e.png)为的Moore-Penrose广义逆。
1）由于极限学习机求取权值的时候只是计算一个广义逆，因此训练速度比基于梯度的学习算法快很多；
2）基于梯度的学习算法存在很多问题，比如学习速率难以确定、局部网络最小化等，极限学习机有效的改善了此类问题，在分类过程中取得了更好的效果；
3）与其他神经网络算法不同，极限学习机在训练过程中，选择激活函数过程中可以选择不可微函数。；
4）极限学习机算法训练过程并不复杂。极限学习机只需要三步就可以完成整个的学习过程。
以下用R代码讲解一下极限学习机
\#\#\#训练过程如下：
训练过程4步即可。
```cpp
elmtrain.
```
```cpp
default
```
```cpp
<-
```

```cpp
function(x,y,nhid,actfun,...) {
```

```cpp
require(MASS)
```

```cpp
if
```
```cpp
(nhid <
```
```cpp
1
```
```cpp
) stop(
```
```cpp
"ERROR: number of hidden neurons must be >= 1"
```
```cpp
)
```

```cpp
########
```
```cpp
1.
```
```cpp
选择数据，X与Y
```

```cpp
T <- t(y)
```

```cpp
P <- t(x)
```

```cpp
########
```
```cpp
2.
```
```cpp
随机产生权值，目的在于将X值进行变化
```

```cpp
inpweight <- randomMatrix(nrow(P),nhid,-
```
```cpp
1
```
```cpp
,
```
```cpp
1
```
```cpp
)
```

```cpp
tempH <- inpweight %*% P
```

```cpp
biashid <- runif(nhid,min=-
```
```cpp
1
```
```cpp
,max=
```
```cpp
1
```
```cpp
)
```

```cpp
biasMatrix <- matrix(rep(biashid, ncol(P)), nrow=nhid, ncol=ncol(P), byrow = F)
```

```cpp
tempH = tempH + biasMatrix
```

```cpp
########
```
```cpp
3.
```
```cpp
将变化后的X值进行高维映射，最常用是sig函数
```

```cpp
if
```
```cpp
(actfun ==
```
```cpp
"sig"
```
```cpp
) H =
```
```cpp
1
```
```cpp
/ (
```
```cpp
1
```
```cpp
+
```
```cpp
exp
```
```cpp
(-
```
```cpp
1
```
```cpp
*tempH))
```

```cpp
else
```
```cpp
{
```

```cpp
if
```
```cpp
(actfun ==
```
```cpp
"sin"
```
```cpp
) H =
```
```cpp
sin
```
```cpp
(tempH)
```

```cpp
else
```
```cpp
{
```

```cpp
if
```
```cpp
(actfun ==
```
```cpp
"radbas"
```
```cpp
) H =
```
```cpp
exp
```
```cpp
(-
```
```cpp
1
```
```cpp
*(tempH^
```
```cpp
2
```
```cpp
))
```

```cpp
else
```
```cpp
{
```

```cpp
if
```
```cpp
(actfun ==
```
```cpp
"hardlim"
```
```cpp
) H = hardlim(tempH)
```

```cpp
else
```
```cpp
{
```

```cpp
if
```
```cpp
(actfun ==
```
```cpp
"hardlims"
```
```cpp
) H = hardlims(tempH)
```

```cpp
else
```
```cpp
{
```

```cpp
if
```
```cpp
(actfun ==
```
```cpp
"satlins"
```
```cpp
) H = satlins(tempH)
```

```cpp
else
```
```cpp
{
```

```cpp
if
```
```cpp
(actfun ==
```
```cpp
"tansig"
```
```cpp
) H =
```
```cpp
2
```
```cpp
/(
```
```cpp
1
```
```cpp
+
```
```cpp
exp
```
```cpp
(-
```
```cpp
2
```
```cpp
*tempH))-
```
```cpp
1
```

```cpp
else
```
```cpp
{
```

```cpp
if
```
```cpp
(actfun ==
```
```cpp
"tribas"
```
```cpp
) H = tribas(tempH)
```

```cpp
else
```
```cpp
{
```

```cpp
if
```
```cpp
(actfun ==
```
```cpp
"poslin"
```
```cpp
) H = poslin(tempH)
```

```cpp
else
```
```cpp
{
```

```cpp
if
```
```cpp
(actfun ==
```
```cpp
"purelin"
```
```cpp
) H =
```
```cpp
tempH
```

```cpp
else
```
```cpp
stop
```
```cpp
(paste(
```
```cpp
"ERROR: "
```
```cpp
,actfun,
```
```cpp
" is not a valid activation function."
```
```cpp
,sep=
```
```cpp
""
```
```cpp
)
```
```cpp
)
```

```cpp
}
```

```cpp
}
```

```cpp
}
```

```cpp
}
```

```cpp
}
```

```cpp
}
```

```cpp
}
```

```cpp
}
```

```cpp
}
```

```cpp
########4.拟合出模型系数，即Y
```
```cpp
=AX中的A
```

```cpp
outweight <- ginv(t(H), tol =
```
```cpp
sqrt
```
```cpp
(.Machine$
```
```cpp
double
```
```cpp
.eps)) %*% t(T)
```

```cpp
Y <- t(t(H) %*% outweight)
```

```cpp
model =
```
```cpp
list
```
```cpp
(inpweight=inpweight,biashid=biashid,outweight=outweight,actfun=actfun,nhid=nhid,predictions=t(Y))
```

```cpp
model$fitted.values <- t(Y)
```

```cpp
model$residuals <- y - model$fitted.values
```

```cpp
model$call <- match.call()
```

```cpp
class
```
```cpp
(model) <-
```
```cpp
"elmNN"
```

```cpp
model
```

```cpp
}
```
测试过程，过程4步即可。
```cpp
function (object, newdata =
```
```cpp
NULL
```
```cpp
, ...)
```

```cpp
{
```

```cpp
if
```
```cpp
(is.null(newdata))
```

```cpp
predictions <- fitted(object)
```

```cpp
else
```
```cpp
{
```

```cpp
if
```
```cpp
(!is.null(object$formula)) {
```

```cpp
x <- model.matrix(object$formula, newdata)
```

```cpp
}
```

```cpp
else
```
```cpp
{
```

```cpp
x <- newdata
```

```cpp
}
```

```cpp
########
```
```cpp
1.
```
```cpp
获取训练模型中的参数
```

```cpp
inpweight <- object$inpweight
```

```cpp
biashid <- object$biashid
```

```cpp
outweight <- object$outweight
```

```cpp
actfun <- object$actfun
```

```cpp
nhid <- object$nhid
```

```cpp
TV.P <- t(x)
```

```cpp
########
```
```cpp
2.
```
```cpp
通过参数将X值进行变化
```

```cpp
tmpHTest = inpweight %*% TV.P
```

```cpp
biasMatrixTE <- matrix(rep(biashid, ncol(TV.P)), nrow = nhid,
```

```cpp
ncol = ncol(TV.P), byrow = F)
```

```cpp
tmpHTest = tmpHTest + biasMatrixTE
```

```cpp
########
```
```cpp
3.
```
```cpp
高维度映射，通常选择sig函数
```

```cpp
if
```
```cpp
(actfun ==
```
```cpp
"sig"
```
```cpp
)
```

```cpp
HTest =
```
```cpp
1
```
```cpp
/(
```
```cpp
1
```
```cpp
+
```
```cpp
exp
```
```cpp
(-
```
```cpp
1
```
```cpp
* tmpHTest))
```

```cpp
else
```
```cpp
{
```

```cpp
if
```
```cpp
(actfun ==
```
```cpp
"sin"
```
```cpp
)
```

```cpp
HTest =
```
```cpp
sin
```
```cpp
(tmpHTest)
```

```cpp
else
```
```cpp
{
```

```cpp
if
```
```cpp
(actfun ==
```
```cpp
"radbas"
```
```cpp
)
```

```cpp
HTest =
```
```cpp
exp
```
```cpp
(-
```
```cpp
1
```
```cpp
* (tmpHTest^
```
```cpp
2
```
```cpp
))
```

```cpp
else
```
```cpp
{
```

```cpp
if
```
```cpp
(actfun ==
```
```cpp
"hardlim"
```
```cpp
)
```

```cpp
HTest = hardlim(tmpHTest)
```

```cpp
else
```
```cpp
{
```

```cpp
if
```
```cpp
(actfun ==
```
```cpp
"hardlims"
```
```cpp
)
```

```cpp
HTest = hardlims(tmpHTest)
```

```cpp
else
```
```cpp
{
```

```cpp
if
```
```cpp
(actfun ==
```
```cpp
"satlins"
```
```cpp
)
```

```cpp
HTest = satlins(tmpHTest)
```

```cpp
else
```
```cpp
{
```

```cpp
if
```
```cpp
(actfun ==
```
```cpp
"tansig"
```
```cpp
)
```

```cpp
HTest =
```
```cpp
2
```
```cpp
/(
```
```cpp
1
```
```cpp
+
```
```cpp
exp
```
```cpp
(-
```
```cpp
2
```
```cpp
* tmpHTest)) -
```

```cpp
1
```

```cpp
else
```
```cpp
{
```

```cpp
if
```
```cpp
(actfun ==
```
```cpp
"tribas"
```
```cpp
)
```

```cpp
HTest = tribas(tmpHTest)
```

```cpp
else
```
```cpp
{
```

```cpp
if
```
```cpp
(actfun ==
```
```cpp
"poslin"
```
```cpp
)
```

```cpp
HTest = poslin(tmpHTest)
```

```cpp
else
```
```cpp
{
```

```cpp
if
```
```cpp
(actfun ==
```
```cpp
"purelin"
```
```cpp
)
```

```cpp
HTest =
```
```cpp
tmpHTest
```

```cpp
else
```
```cpp
stop
```
```cpp
(paste(
```
```cpp
"ERROR: "
```
```cpp
, actfun,
```

```cpp
" is not a valid activation function."
```
```cpp
,
```

```cpp
sep =
```
```cpp
""
```
```cpp
)
```
```cpp
)
```

```cpp
}
```

```cpp
}
```

```cpp
}
```

```cpp
}
```

```cpp
}
```

```cpp
}
```

```cpp
}
```

```cpp
}
```

```cpp
}
```

```cpp
########4.进行预测的值计算，即Y（预测）
```
```cpp
=AX
```

```cpp
TY = t(t(HTest) %*% outweight)
```

```cpp
predictions <- t(TY)
```

```cpp
}
```

```cpp
predictions
```

```cpp
}
```
通过R讲述了极限学习机的内部构造，以下是R自带的示例：通过极限学习机预测
`library(elmNN)`
`set``.``seed``(``1234``)`
`Var1 <- runif(``50``,``0``,``100``)`
`sqrt``.``data``<-``data``.frame(Var1,``Sqrt``=``sqrt``(Var1))`
`model``<- elmtrain.formula(``Sqrt``~Var1,``data``=``sqrt``.``data``, nhid=``10``, actfun=``"sig"``)`
`new``<-``data``.frame(``Sqrt``=``0``,Var1 = runif(``50``,``0``,``100``))`
`p``<- predict(``model``,`


