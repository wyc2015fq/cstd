# 极限学习机︱R语言快速深度学习进行回归预测 - 素质云笔记/Recorder... - CSDN博客





2016年08月06日 09:19:03[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：4117











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

###训练过程如下：

训练过程4步即可。

```cpp
elmtrain.default <-
function(x,y,nhid,actfun,...) {
  require(MASS)

if(nhid < 1) stop("ERROR: number of hidden neurons must be >= 1")
########1.选择数据，X与Y  
  T <- t(y)
  P <- t(x)
########2.随机产生权值，目的在于将X值进行变化  

  inpweight <- randomMatrix(nrow(P),nhid,-1,1)
  tempH <- inpweight %*% P
  biashid <- runif(nhid,min=-1,max=1)
  biasMatrix <- matrix(rep(biashid, ncol(P)), nrow=nhid, ncol=ncol(P), byrow = F) 

  tempH = tempH + biasMatrix
########3.将变化后的X值进行高维映射，最常用是sig函数   
if(actfun == "sig") H = 1 / (1 + exp(-1*tempH))
else {
if(actfun == "sin") H = sin(tempH)
else {
if(actfun == "radbas") H = exp(-1*(tempH^2))
else {
if(actfun == "hardlim") H = hardlim(tempH)
else {
if(actfun == "hardlims") H = hardlims(tempH)
else {
if(actfun == "satlins") H = satlins(tempH)
else {
if(actfun == "tansig") H = 2/(1+exp(-2*tempH))-1
else {
if(actfun == "tribas") H = tribas(tempH)
else {
if(actfun == "poslin") H = poslin(tempH)
else {
if(actfun == "purelin") H = tempH
else stop(paste("ERROR: ",actfun," is not a valid activation function.",sep=""))
                  }
                }
              }
            }
          }
        }
      }
    }
  }

########4.拟合出模型系数，即Y=AX中的A   
  outweight <- ginv(t(H), tol = sqrt(.Machine$double.eps)) %*% t(T)
  Y <- t(t(H) %*% outweight)
  model = list(inpweight=inpweight,biashid=biashid,outweight=outweight,actfun=actfun,nhid=nhid,predictions=t(Y))
  model$fitted.values <- t(Y)
  model$residuals <- y - model$fitted.values
  model$call <- match.call()
class(model) <- "elmNN"
  model
}
```

测试过程，过程4步即可。

```cpp
function (object, newdata = NULL, ...) 
{
if (is.null(newdata)) 
    predictions <- fitted(object)
else {
if (!is.null(object$formula)) {
      x <- model.matrix(object$formula, newdata)
    }
else {
      x <- newdata
    }

########1.获取训练模型中的参数
    inpweight <- object$inpweight
    biashid <- object$biashid
    outweight <- object$outweight
    actfun <- object$actfun
    nhid <- object$nhid
    TV.P <- t(x)

########2.通过参数将X值进行变化  

    tmpHTest = inpweight %*% TV.P
    biasMatrixTE <- matrix(rep(biashid, ncol(TV.P)), nrow = nhid, 
                           ncol = ncol(TV.P), byrow = F)
    tmpHTest = tmpHTest + biasMatrixTE

########3.高维度映射，通常选择sig函数
if (actfun == "sig") 
      HTest = 1/(1 + exp(-1 * tmpHTest))
else {
if (actfun == "sin") 
        HTest = sin(tmpHTest)
else {
if (actfun == "radbas") 
          HTest = exp(-1 * (tmpHTest^2))
else {
if (actfun == "hardlim") 
            HTest = hardlim(tmpHTest)
else {
if (actfun == "hardlims") 
              HTest = hardlims(tmpHTest)
else {
if (actfun == "satlins") 
                HTest = satlins(tmpHTest)
else {
if (actfun == "tansig") 
                  HTest = 2/(1 + exp(-2 * tmpHTest)) - 
1
else {
if (actfun == "tribas") 
                    HTest = tribas(tmpHTest)
else {
if (actfun == "poslin") 
                      HTest = poslin(tmpHTest)
else {
if (actfun == "purelin") 
                        HTest = tmpHTest
else stop(paste("ERROR: ", actfun, 
" is not a valid activation function.", 
                                      sep = ""))
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

########4.进行预测的值计算，即Y（预测）=AX
    TY = t(t(HTest) %*% outweight)
    predictions <- t(TY)
  }
  predictions
}
```

通过R讲述了极限学习机的内部构造，以下是R自带的示例：通过极限学习机预测

```
library(elmNN)
set.seed(1234)
Var1 <- runif(50, 0, 100) 
sqrt.data <- data.frame(Var1, Sqrt=sqrt(Var1))
model <- elmtrain.formula(Sqrt~Var1, data=sqrt.data, nhid=10, actfun="sig")
new <- data.frame(Sqrt=0,Var1 = runif(50,0,100))
p <- predict(model,
```







