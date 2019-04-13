
# R︱Yandex的梯度提升CatBoost 算法（官方述：超越XGBoost-lightGBM-h2o） - 素质云笔记-Recorder... - CSDN博客

2017年07月21日 11:18:28[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：8479


俄罗斯搜索巨头 Yandex 昨日宣布开源  CatBoost ，这是一种支持类别特征，基于梯度提升决策树的机器学习方法。
CatBoost 是由 Yandex 的研究人员和工程师开发的，是 MatrixNet 算法的继承者，在公司内部广泛使用，用于排列任务、预测和提出建议。Yandex 称其是通用的，可应用于广泛的领域和各种各样的问题。
---笔者相关文章：

---[R+工业级GBDT︱微软开源 的LightGBM（R包已经开放）](http://blog.csdn.net/sinat_26917383/article/details/54092493)

---[R语言︱XGBoost极端梯度上升以及forecastxgb（预测）+xgboost（回归）双案例解读](http://blog.csdn.net/sinat_26917383/article/details/52623754)

---[R语言︱H2o深度学习的一些R语言实践——H2o包](http://blog.csdn.net/sinat_26917383/article/details/51219025)

---CatBoost 的主要优势：

---与其他库相比，质量上乘

---支持数字化和分类功能

---带有数据可视化工具

---官网：

---[https://tech.yandex.com/CatBoost/](https://tech.yandex.com/CatBoost/)

---github:

---[https://github.com/catboost/catboost](https://github.com/catboost/catboost)

---有R/python两个版本，官方自述超越现有的最好的三个ML库：XGBoost/lightGBM/h2o

---衡量标准为： Logloss 越小越好：

![这里写图片描述](https://img-blog.csdn.net/20170721110726853?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---默认参数解析（[github]

---(

---[https://github.com/catboost/benchmarks/blob/master/comparison_description.pdf](https://github.com/catboost/benchmarks/blob/master/comparison_description.pdf)

---)）：

![这里写图片描述](https://img-blog.csdn.net/20170721111321778?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---[ ](https://img-blog.csdn.net/20170721111321778?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170721111328463?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---安装

---在window笔者遇到了：

---* installing *

---source

---* package

---'catboost'

---...

---** libs
  running

---'src/Makefile.win'

---...

---/cygdrive/c/Users/mzheng50/Desktop/R-package/src/../../../ya.bat make -r -o ../../..
make: /cygdrive/c/Users/mzheng50/Desktop/R-package/src/../../../ya.bat: Command not found
make: *** [all] Error

---127

---警告: 运行命令

---'make --no-print-directory -f "Makefile.win"'

---的状态是

---2

---ERROR: compilation failed

---for

---package

---'catboost'

---* removing

---'C:/Users/mzheng50/Documents/R/win-library/3.1/catboost'

---Error: Command failed (

---1

---)

---在Linux用下面code可以一气呵成：

---devtools:

---:install_github

---(

---'catboost/catboost'

---, subdir =

---'catboost/R-package'

---)

---一个

---[官方案例](https://tech.yandex.com/catboost/doc/dg/concepts/r-usages-examples-docpage/)

---：

---library(caret)
library(titanic)
library(catboost)

---set

---.seed(

---12345

---)
data <-

---as

---.data.frame(

---as

---.matrix(titanic_train), stringsAsFactors =

---TRUE

---)
drop_columns = c(

---"PassengerId"

---,

---"Survived"

---,

---"Name"

---,

---"Ticket"

---,

---"Cabin"

---)
x <- data[,!(

---names

---(data) %

---in

---% drop_columns)]
y <- data[,c(

---"Survived"

---)]
fit_control <- trainControl(method =

---"cv"

---,

---number

---=

---4

---,
                            classProbs =

---TRUE

---)
grid <- expand.grid(depth = c(

---4

---,

---6

---,

---8

---),
                    learning_rate =

---0.1

---,
                    iterations =

---100

---,
                    l2_leaf_reg =

---1e-3

---,
                    rsm =

---0.95

---,
                    border_count =

---64

---)

---report <- train(x,

---as

---.factor(make.names(y)),
                method = catboost.caret,
                verbose =

---TRUE

---, preProc =

---NULL

---,
                tuneGrid = grid, trControl = fit_control)
print(report)

-----------------------------

---> Catboost
> 
>

---891

---samples

---7

---predictors

---2

---classes:

---'X0'

---,

---'X1'

---> 
> No pre-processing Resampling: Cross-Validated (

---4

---fold) Summary

---of

---> sample sizes:

---669

---,

---668

---,

---668

---,

---668

---Resampling results across tuning
> parameters:
> 
>   depth  Accuracy   Kappa

---4

---0.8091544

---0.5861049

---6

--->

---0.8035642

---0.5728401

---8

---0.7026674

---0.2672683

---> 
> Tuning parameter

---'learning_rate'

---was held

---constant

---at

---a

---value

---of

---0.1

---> 
> Tuning parameter

---'rsm'

---was held

---constant

---at

---a

---value

---of

---0.95

---Tuning 
> parameter

---'border_count'

---was held

---constant

---at

---a

---value

---of

---64

---Accuracy
> was used

---to

---select

---the

---optimal model

---using

---the

---largest

---value

---. The
> final values used

---for

---the

---model were depth =

---4

---, learning_rate =
>

---0.1

---, iterations =

---100

---, l2_leaf_reg =

---0.001

---, rsm =

---0.95

---and

---border_count =

---64.

---importance <- varImp(report, scale = FALSE)

---print(importance)
--------------------------

---custom variable importance

---Overall

---Fare      25.918
Parch     19.419
Sex       17.999
Pclass    17.410
Age       10.372
Embarked   5.879
SibSp      3.004

---公众号“素质云笔记”定期更新博客内容：

![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

