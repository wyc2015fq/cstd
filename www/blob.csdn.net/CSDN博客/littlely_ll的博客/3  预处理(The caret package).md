# 3. 预处理(The caret package) - littlely_ll的博客 - CSDN博客





2017年03月02日 14:39:32[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：1463
所属专栏：[R语言caret包应用](https://blog.csdn.net/column/details/29369.html)









[1. 简介(The caret package )](http://blog.csdn.net/littlely_ll/article/details/58592298)
[2. 可视化(The caret package)](http://blog.csdn.net/littlely_ll/article/details/58596562)

# 3. 预处理
- 创建虚拟变量
- 零方差和近似零方差特征变量
- 识别相关的特征变量
- 线性相关性
- `preProcess`函数
- 中心化和标准化
- 插补
- 特征变量变换
- 整合在一起
- 类别距离计算

caret包含很多函数来对特征变量进行预处理，它假设所有的数据为数值型数据（例如：因子变量通过`model.matrix`,`dummyVars`或其他方法转化为虚拟变量）。

## 3.1 创建虚拟变量

‘dummyVars’函数可以由一个或多个因子产生一个完整的虚拟变量集合。这个函数输入一个方程和一个数据集，会生成一个可以使用predict方法创建虚拟变量的对象。 

例如：`earth`包中的`etitanic`数据集包含了两个因子：pclass(1st,2nd,3rd)和sex(female,male)。R的基础函数`model.matrix`会产生下列变量：

```
library(earth)
data(etitanic)
head(model.matrix(survived ~ ., data = etitanic))
```
 (Intercept) pclass2nd pclass3rd sexmale     age sibsp parch
1           1         0         0       0 29.0000     0     0
2           1         0         0       1  0.9167     1     2
3           1         0         0       0  2.0000     1     2
4           1         0         0       1 30.0000     1     2
5           1         0         0       0 25.0000     1     2
6           1         0         0       1 48.0000     0     0

使用`dummyVars`：

```
dummies <- dummyVars(survived~., data = etitanic)
head(predict(dummies, newdata = etitanic))
```
 pclass.1st pclass.2nd pclass.3rd sex.female sex.male     age sibsp parch
1          1          0          0          1        0 29.0000     0     0
2          1          0          0          0        1  0.9167     1     2
3          1          0          0          1        0  2.0000     1     2
4          1          0          0          0        1 30.0000     1     2
5          1          0          0          1        0 25.0000     1     2
6          1          0          0          0        1 48.0000     0     0

注意到这儿没有截距，每个因子都有对应各个水平的虚拟变量，所以这种参数化在一些模型函数中可能并没用，比如`lm`函数。

## 3.2 零方差和近似零方差特征变量

在某些情况下，数据产生机制只创建具有单一值的特征变量（例如零方差特征变量）。这对于大多数模型（不包括树模型）来说，这可能造成模型的破坏，或者数据拟合的不稳定。 

同样的，特征变量中可能有一些出现频率非常低的唯一值，例如在drug resistance数据中，变量nR11的数值型数据非常不平衡：

```
data(mdrr)
data.frame(table(mdrrDescr$nR11))

##   Var1 Freq
## 1    0  501
## 2    1    4
## 3    2   23
```

这里我们关心的是，当这样的数据使用交叉验证（cross-validation）或自助法（bootstrap）抽样或一些样本可能对模型有过度影响的时候，这些特征变量可能变成零方差的特征变量。这些“近似零方差”的特征变量可能需要识别并且在建模前去除掉。 

要辨别这种类型的特征变量，可以使用一下两种方法： 

 * 最大频率数值的频数比上第二频率数值的频数（称作频数比率（frequency ratio）），对于平衡的特征变量其值接近1，对于高度不平衡的数据其值会非常大。 

 * 唯一值比例就是唯一值的个数除以样本的总数再乘以100， 

它随着数据粒度的增大而几近于0。 

如果频数比率大于预先设定的阈值，并且唯一值的比例小于一个阈值，我们可以认为这个特征变量为近似零方差。 

我们不想错误的识别拥有细粒度但是分布均匀的数据，像服从离散均匀分布的数据。应用这两个准则不应该错误的检测这些特征变量。 

应用MDRR数据，`nearZeroVar`函数能够用来识别近似零方差的变量（`saveMetrics`参数用来展示细节并且默认为FALSE）：
```
nzv <- nearZeroVar(mdrrDescr, saveMetrics= TRUE)
nzv[nzv$nzv,][1:10,]

##        freqRatio percentUnique zeroVar  nzv
## nTB     23.00000     0.3787879   FALSE TRUE
## nBR    131.00000     0.3787879   FALSE TRUE
## nI     527.00000     0.3787879   FALSE TRUE
## nR03   527.00000     0.3787879   FALSE TRUE
## nR08   527.00000     0.3787879   FALSE TRUE
## nR11    21.78261     0.5681818   FALSE TRUE
## nR12    57.66667     0.3787879   FALSE TRUE
## D.Dr03 527.00000     0.3787879   FALSE TRUE
## D.Dr07 123.50000     5.8712121   FALSE TRUE
## D.Dr08 527.00000     0.3787879   FALSE TRUE
```

```
dim(mdrrDescr)

## [1] 528 342

nzv <- nearZeroVar(mdrrDescr)
filteredDescr <- mdrrDescr[, -nzv]
dim(filteredDescr)

## [1] 528 297
```

默认的，`nearZeroVar`返回标记的近似零方差变量的位置。

## 3.3 识别相关的特征变量

有些模型依赖与相关的特征变量（像pls），而有些模型能够得益于变量之间的相关性减少。 

给一个相关矩阵，`findCorrelation`函数应用一以下算法标记要剔除的变量：

```
descrCor <-  cor(filteredDescr)
highCorr <- sum(abs(descrCor[upper.tri(descrCor)]) > .999)
```

对于先前的MDRR数据，有`I(highCorr)`个几乎完全相关（|相关性| >0.999）的变量，例如“全信息原子分解指数”（IAC）和“全信息文本指数”（TIC0）。下面的代码块展示了去除相关性的绝对值大于0.75的变量的影响。

```
descrCor <- cor(filteredDescr)
summary(descrCor[upper.tri(descrCor)])

##     Min.  1st Qu.   Median     Mean  3rd Qu.     Max. 
## -0.99610 -0.05373  0.25010  0.26080  0.65530  1.00000

highlyCorDescr <- findCorrelation(descrCor, cutoff = .75)
filteredDescr <- filteredDescr[,-highlyCorDescr]
descrCor2 <- cor(filteredDescr)
summary(descrCor2[upper.tri(descrCor2)])

##     Min.  1st Qu.   Median     Mean  3rd Qu.     Max. 
## -0.70730 -0.05378  0.04418  0.06692  0.18860  0.74460
```

## 3.4 线性相关性

函数`findLinearCombos`使用QR分解来枚举线性组合的集合（如果存在的话）。例如：考虑到下面的矩阵由非满秩参数的双向实验生成。

```
ltfrDesign <- matrix(0, nrow=6, ncol=6)
ltfrDesign[,1] <- c(1, 1, 1, 1, 1, 1)
ltfrDesign[,2] <- c(1, 1, 1, 0, 0, 0)
ltfrDesign[,3] <- c(0, 0, 0, 1, 1, 1)
ltfrDesign[,4] <- c(1, 0, 0, 1, 0, 0)
ltfrDesign[,5] <- c(0, 1, 0, 0, 1, 0)
ltfrDesign[,6] <- c(0, 0, 1, 0, 0, 1)
```

注意到第2列和第3列加起来为第1列。同样，第4，5，6列加起来等于第1列。`findLiearCombos`会返回那些相关性变量的列表。对于每一个线性组合，它会逐个从矩阵中移除列并检验是否解决了相关性。`findLiearCombos`也会返回那些移除后能消除线性相关性的变量的位置。

```
comboInfo <- findLinearCombos(ltfrDesign)
comboInfo

## $linearCombos
## $linearCombos[[1]]
## [1] 3 1 2
## 
## $linearCombos[[2]]
## [1] 6 1 4 5
## 
## 
## $remove
## [1] 3 6

ltfrDesign[, -comboInfo$remove]

##      [,1] [,2] [,3] [,4]
## [1,]    1    1    1    0
## [2,]    1    1    0    1
## [3,]    1    1    0    0
## [4,]    1    0    1    0
## [5,]    1    0    0    1
## [6,]    1    0    0    0
```

当大量的二进制医学指纹来描述分子结构时，这些相关性的类型就起作用了。

## 3.5 `preProcess`函数

`preProcess`函数可以对特征变量施行很多操作，包括中心化和标准化。`preProcess`函数每次操作都估计所需要的参数，并且`predict.preProcess`应用于指定的数据集。这个函数可以与调用的`train`函数相接口。 

一些技术类型将在下几章节介绍，另一个例子将会说明多元方法是怎样使用的。注意到，在所有的案例中，`preProcess`函数从设定的数据集（例如训练集）中估计它所能获得的参数，并且把这些变换应用于任何数据集且无需重复计算数值。

## 3.6 中心化和标准化

下面的例子中，用MDRR一半的数据对特征变量进行标准化。`preProcess`函数并不能真正的预处理数据，而`predict.preProcess`函数是对这数据及其他数据进行预处理。

```
set.seed(96)
inTrain <- sample(seq(along = mdrrClass), length(mdrrClass)/2)

training <- filteredDescr[inTrain,]
test <- filteredDescr[-inTrain,]
trainMDRR <- mdrrClass[inTrain]
testMDRR <- mdrrClass[-inTrain]

preProcValues <- preProcess(training, method = c("center", "scale"))

trainTransformed <- predict(preProcValues, training)
testTransformed <- predict(preProcValues, test)
```

`preProcess`函数method选项”ranges”是对数据进行0-1标准化处理。

## 3.7 插补

`preProcess`函数只基于训练集的信息技能对数据进行插补。一种方法就是使用K近邻。对任意一个样本，在训练集中找到最近邻，并用这些值（例如均值）取插补特征变量。用这种方法将会自动对数据用`preProcess`函数进行中心化和标准化，而不用管`preProcess`函数中参数`method`的设置。另外，袋装树（bagged tree）也可以被用来插值。对于数据中每一个特征变量，训练集中所有其他的特征变量都被用来创建袋装树。当一个新样本有一个缺失值的特征变量，袋装树模型就用来预测数值。在理论上，这是插值中的一个很强大的方法，但是计算开销要比K近邻要大很多。

## 3.8 特征变量变换

在一些例子中，有必要使用PCA把数据变换到一个更小的子空间，在这个子空间中，新变量之间彼此不相关。`preProcess`函数通过设置`method`参数中的“pca”来进行变换。这样做的话会迫使特征变量进行标准化。当使用PCA方法时，`predict.preProcess`函数将会把列名改为PC1，PC2等等。 

同样ICA（Independent component analysis）也可发现在原始数据中呈线性组合的新变量，新变量将会标为IC1，IC2等等。 

空间符号（spatial sign）变换把特征变量的数据映射到p维的单位圆上，这儿p是特征变量的个数。下面两张图展示了在空间符号变换前后从数据MDRR中得到的两组中心化和标准化的特征向量。特征变量在应用这个变换前需要进行中心化和标准化。
```
library(AppliedPredictiveModeling)
transparentTheme(trans = .4)

plotSubset <- data.frame(scale(mdrrDescr[, c("nC", "X4v")])) 
xyplot(nC ~ X4v,
       data = plotSubset,
       groups = mdrrClass, 
       auto.key = list(columns = 2))
```

![这里写图片描述](https://img-blog.csdn.net/20170302130419878?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在空间符号变换后：
```
transformed <- spatialSign(plotSubset)
transformed <- as.data.frame(transformed)
xyplot(nC ~ X4v, 
       data = transformed, 
       groups = mdrrClass, 
       auto.key = list(columns = 2))
```

![这里写图片描述](https://img-blog.csdn.net/20170302130522265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

另一个`method`选项，“BoxCox”，在数据大于0时将会对特征变量进行Box-Cox变换。

```
preProcValues2 <- preProcess(training, method = "BoxCox")
trainBC <- predict(preProcValues2, training)
testBC <- predict(preProcValues2, test)
preProcValues2

## Created from 264 samples and 31 variables
## 
## Pre-processing:
##   - Box-Cox transformation (31)
##   - ignored (0)
## 
## Lambda estimates for Box-Cox transformation:
##    Min. 1st Qu.  Median    Mean 3rd Qu.    Max. 
## -2.0000 -0.2000  0.3000  0.4097  1.7000  2.0000
```

对应特征变量的NA值不能被变换。这种转换要求数据大于0。与Box-Cox相似的两个变换——“Yeo-Johnnson”变换和指数变换——也能应用到`preProcess`的`method`中。

## 3.9 整合在一起

在《应用预测建模》一书中有一个案例研究，它预测了在高性能计算环境中工作的执行时间。数据是：

```
library(AppliedPredictiveModeling)
data(schedulingData)
str(schedulingData)

## 'data.frame':    4331 obs. of  8 variables:
##  $ Protocol   : Factor w/ 14 levels "A","C","D","E",..: 4 4 4 4 4 4 4 4 4 4 ...
##  $ Compounds  : num  997 97 101 93 100 100 105 98 101 95 ...
##  $ InputFields: num  137 103 75 76 82 82 88 95 91 92 ...
##  $ Iterations : num  20 20 10 20 20 20 20 20 20 20 ...
##  $ NumPending : num  0 0 0 0 0 0 0 0 0 0 ...
##  $ Hour       : num  14 13.8 13.8 10.1 10.4 ...
##  $ Day        : Factor w/ 7 levels "Mon","Tue","Wed",..: 2 2 4 5 5 3 5 5 5 3 ...
##  $ Class      : Factor w/ 4 levels "VF","F","M","L": 2 1 1 1 1 1 1 1 1 1 ...
```

数据既有名义变量又有数值变量。假设我们用Yeo-Johnson方法对连续变量进行变换并进行中心化和标准化。我们还假设运行一个基于树的模型，让因子变量保持不变（而不是创建虚拟变量）。除了最后一列结果变量外，我们把该函数运用到所有的列。

```
pp_hpc <- preProcess(schedulingData[, -8], 
                     method = c("center", "scale", "YeoJohnson"))
pp_hpc

## Created from 4331 samples and 7 variables
## 
## Pre-processing:
##   - centered (5)
##   - ignored (2)
##   - scaled (5)
##   - Yeo-Johnson transformation (5)
## 
## Lambda estimates for Yeo-Johnson transformation:
## -0.08, -0.03, -1.05, -1.1, 1.44
```

```
transformed <- predict(pp_hpc, newdata = schedulingData[, -8])
head(transformed)

##   Protocol  Compounds InputFields  Iterations NumPending         Hour Day
## 1        E  1.2289589  -0.6324538 -0.06155877  -0.554123  0.004586502 Tue
## 2        E -0.6065822  -0.8120451 -0.06155877  -0.554123 -0.043733215 Tue
## 3        E -0.5719530  -1.0131509 -2.78949011  -0.554123 -0.034967191 Thu
## 4        E -0.6427734  -1.0047281 -0.06155877  -0.554123 -0.964170760 Fri
## 5        E -0.5804710  -0.9564501 -0.06155877  -0.554123 -0.902085029 Fri
## 6        E -0.5804710  -0.9564501 -0.06155877  -0.554123  0.698108779 Wed
```

在输出结果中，两个标有“ignore”的特征变量就是那两个因子变量。他们没有变换而那些数值变量进行了变换。然而，如果特征变量没有变换，那么它是具有稀疏和非平稳分布的：

```bash
mean(schedulingData$NumPending == 0)

## [1] 0.7561764
```

对于其他一些模型，这可能是一个问题（特别是在我们重抽样数据时）。在进行预处理前，我们可以加一个滤波来检查是否有零或近似零方差的特征变量：

```
pp_no_nzv <- preProcess(schedulingData[, -8], 
                        method = c("center", "scale", "YeoJohnson", "nzv"))
pp_no_nzv

## Created from 4331 samples and 7 variables
## 
## Pre-processing:
##   - centered (4)
##   - ignored (2)
##   - removed (1)
##   - scaled (4)
##   - Yeo-Johnson transformation (4)
## 
## Lambda estimates for Yeo-Johnson transformation:
## -0.08, -0.03, -1.05, 1.44
```

注意到有一个特征变量被标记为“removed”，预处理后的数据就会去除这个系数特征变量。

## 3.10 类别距离计算

caret包包括一些函数，这些函数能够产生基于类别形心距离（和LDA工作原理相似）的新的特征变量。对一个因子变量的每一水平，都要计算类别形心和协方差矩阵。对于新样本，计算每一个形心的马氏距离，并且这个马氏距离也可作为一个新的特征变量。当真正的决策边界确实是线性时，这对于非线性模型也是有帮助的。 

在一些案例中，有更多的特征变量存在于一个类别中而不是样本中，`classDist`函数有`pca`和`keep`参数，对于每一类别的主成分分析来说，用它可以避免奇异协方差矩阵的问题。 
`predict.classDist`函数能产生类别距离。默认的距离取对数，但是这可以通过`predict.classDist`的参数`trans`来改变。 

作为一个例子，我们使用MDRR数据。
```
centroids <- classDist(trainBC, trainMDRR)
distances <- predict(centroids, testBC)
distances <- as.data.frame(distances)
head(distances)

##                dist.Active dist.Inactive
## PROMETHAZINE      5.810607      4.098229
## ACEPROMETAZINE    4.272003      4.169292
## PYRATHIAZINE      4.570192      4.224053
## THIORIDAZINE      4.548315      5.064125
## MESORIDAZINE      4.621708      5.080362
## SULFORIDAZINE     5.344699      5.145311
```

下面这个图片展示了样本外类别距离的散点图矩阵：

```
xyplot(dist.Active ~ dist.Inactive,
       data = distances, 
       groups = testMDRR, 
       auto.key = list(columns = 2))
```

![这里写图片描述](https://img-blog.csdn.net/20170302143636562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







