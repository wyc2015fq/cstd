# 碎片︱R语言与深度学习 - 素质云笔记/Recorder... - CSDN博客





2016年03月11日 11:39:26[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：4788















笔者：受alphago影响，想看看深度学习，但是其在R语言中的应用包可谓少之又少，更多的是在matlab和python中或者是调用。整理一下目前我看到的R语言的材料：




————————————————————————————————————————————————————————————




近期，弗莱堡大学的Oksana Kutina 和 Stefan Feuerriegel发表了一篇名为《深入比较四个R中的深度学习包》的博文。其中，四个R包的综述如下：



- 
MXNet: MXNet深度学习库的R接口。（博文中指的是早期的包，MXNetR）

- 
darch: 深层架构和限制波耳兹曼机（Restricted Boltzmann Machines, RBM）的R包。

- 
deepnet: 实现前馈神经网络，限制波耳兹曼机，深度信念网络（Deep Belief Networks, DBN）和堆栈式自编码器的R包。

- 
h2o: H2O深度学习框架的R接口。





这篇博文对四个R包的功能做了详细的介绍，并比较它们的灵活性、易用性、支持并行处理框架（GPU集群）和执行性--根据下面的链接了解详情。文章中的结论如下：




> 
当前版本的deepnet可能代表着在可用架构方面的最不同的包。然而根据其实现，它可能不是最快的和最容易使用的一个选择。此外，和其他包相比，deepnet可能不能够提供尽可能多的调优参数。

相反，H2O和MXNetR则为用户提供了高度方便的使用体验。同时，它们还提供额外信息的输出,能够训练得更快并获得像样的结果。H2O可能更适合集群环境,数据科学家们可以在一个简单的条件下用它来做数据挖掘和探索。当更关注灵活性和原型设计的时候，MXNetR可能是最佳的选择。它提供了一个直观的符号工具,用于从头构建自定义网络体系结构。此外,通过利用多核CPU/GPU，MXNetR在个人电脑上运行时得到了很好的优化。

更新于2017-02-18 转载于雪晴数据网，来自烟台大学的张潇同学供稿，翻译自A comparison of deep learning packages for R



————————————————————————————————————————————————————————————




**Matt︱R语言调用深度学习架构系列引文**




[R语言︱H2o深度学习的一些R语言实践——H2o包](http://blog.csdn.net/sinat_26917383/article/details/51219025)




# [R用户的福音︱TensorFlow：TensorFlow的R接口](http://blog.csdn.net/sinat_26917383/article/details/52737913)



# [](http://blog.csdn.net/sinat_26917383/article/details/52737913)

# [mxnet：结合R与GPU加速深度学习](http://blog.csdn.net/sinat_26917383/article/details/52737901)





# [](http://blog.csdn.net/sinat_26917383/article/details/52737901)

# 碎片︱R语言与深度学习




—————————————————————————————————————





# 一、R结合H2O




## 1、H2O简介




一个开源的可扩展的库，支持Java, Python, Scala, and R（官网链接： http://www.h2o.ai/verticals/algos/deep-learning/）



# H2O是基于[大数据](http://www.jdon.com/bigdata.html)的统计分析 机器学习和数学库包，让用户基于核心的数学积木搭建应用块代码，采取类似R语言 Excel或JSON等熟悉接口，使的BigData爱好者和专家可以利用一系列简单的先进算法对数据集进行探索，建模和评估。数据收集是很容易，但是决策是很难的。 H2O使得能用更快更好的预测模型源实现快速和方便地数据的挖掘。 H2O愿意将在线评分和建模融合在一个单一平台上。




## 2、实践



```
1. 进入RStudio，输入安装
 install.packages("h2o", repos=(c("http://s3.amazonaws.com/h2o-release/h2o/rel-kahan/5/R", getOption("repos")))) 
2. 加装包，启动h2o本地环境
   library(h2o) 
载入需要的程辑包：rjson
载入需要的程辑包：statmod
载入需要的程辑包：tools


----------------------------------------------------------------------


Your next step is to start H2O and get a connection object (named
'localH2O', for example):
    > localH2O = h2o.init()


For H2O package documentation, first call init() and then ask for help:
    > localH2O = h2o.init()
    > ??h2o


To stop H2O you must explicitly call shutdown (either from R, as shown
here, or from the Web UI):
    > h2o.shutdown(localH2O)


After starting H2O, you can use the Web UI at http://localhost:54321
For more information visit http://docs.0xdata.com


----------------------------------------------------------------------




载入程辑包：‘h2o’


下列对象被屏蔽了from ‘package:base’:


    max, min, sum


Warning messages:
1: 程辑包‘h2o’是用R版本3.0.3 来建造的 
2: 程辑包‘rjson’是用R版本3.0.3 来建造的 
3: 程辑包‘statmod’是用R版本3.0.3 来建造的  


3.  观看下示例
localH2O = h2o.init(ip = "localhost", port = 54321, startH2O = TRUE,Xmx = '1g')
 
H2O is not running yet, starting it now...
Performing one-time download of h2o.jar from
     http://s3.amazonaws.com/h2o-release/h2o/rel-knuth/11/Rjar/h2o.jar 
(This could take a few minutes, please be patient...)


Note:  In case of errors look at the following log files:
           C:/TMP/h2o_huangqiang01_started_from_r.out
           C:/TMP/h2o_huangqiang01_started_from_r.err


java version "1.7.0_25"
Java(TM) SE Runtime Environment (build 1.7.0_25-b17)
Java HotSpot(TM) 64-Bit Server VM (build 23.25-b01, mixed mode)


Successfully connected to http://127.0.0.1:54321 
R is connected to H2O cluster:
    H2O cluster uptime:        3 seconds 408 milliseconds 
    H2O cluster version:       2.4.3.11 
    H2O cluster name:          H2O_started_from_R 
    H2O cluster total nodes:   1 
    H2O cluster total memory:  0.96 GB 
    H2O cluster total cores:   4 
    H2O cluster healthy:       TRUE 


 demo(h2o.glm)

4. 训练minist数据

下载 Train Dataset: http://www.pjreddie.com/media/files/mnist_train.csv
下载 Test Dataset: http://www.pjreddie.com/media/files/mnist_test.csv

res <- data.frame(Training = NA, Test = NA, Duration = NA)

#加载数据到h2o
 train_h2o <- h2o.importFile(localH2O, path = "C:/Users/jerry/Downloads/mnist_train.csv")
 test_h2o <- h2o.importFile(localH2O, path = "C:/Users/jerry/Downloads/mnist_test.csv")

 y_train <- as.factor(as.matrix(train_h2o[, 1]))
 y_test <- as.factor(as.matrix(test_h2o[, 1]))

##训练模型要很长一段时间，多个cpu使用率几乎是100%，风扇狂响。最后一行有相应的进度条可查看
model <- h2o.deeplearning(x = 2:785,  # column numbers for predictors
                          y = 1,   # column number for label
                          data = train_h2o,
                          activation = "Tanh",
                          balance_classes = TRUE,
                          hidden = c(100, 100, 100),  ## three hidden layers
                          epochs = 100)

#输出模型结果
> model
IP Address: localhost 
Port      : 54321 
Parsed Data Key: mnist_train.hex 
Deep Learning Model Key: DeepLearning_9c7831f93efb58b38c3fa08cb17d4e4e


Training classification error: 0
Training mean square error: Inf


Validation classification error: 0
Validation square error: Inf


Confusion matrix:
Reported on mnist_train.hex 
        Predicted
Actual      0    1    2    3    4    5    6    7    8    9 Error
  0      5923    0    0    0    0    0    0    0    0    0     0
  1         0 6742    0    0    0    0    0    0    0    0     0
  2         0    0 5958    0    0    0    0    0    0    0     0
  3         0    0    0 6131    0    0    0    0    0    0     0
  4         0    0    0    0 5842    0    0    0    0    0     0
  5         0    0    0    0    0 5421    0    0    0    0     0
  6         0    0    0    0    0    0 5918    0    0    0     0
  7         0    0    0    0    0    0    0 6265    0    0     0
  8         0    0    0    0    0    0    0    0 5851    0     0
  9         0    0    0    0    0    0    0    0    0 5949     0
  Totals 5923 6742 5958 6131 5842 5421 5918 6265 5851 5949     0
> 

> str(model)


## 评介性能
yhat_train <- h2o.predict(model, train_h2o)$predict
yhat_train <- as.factor(as.matrix(yhat_train))

yhat_test <- h2o.predict(model, test_h2o)$predict
yhat_test <- as.factor(as.matrix(yhat_test))

查看前100条预测与实际的数据相比较
> y_test[1:100]
  [1] 7 2 1 0 4 1 4 9 5 9 0 6 9 0 1 5 9 7 3 4 9 6 6 5 4 0 7 4 0 1 3 1 3 4 7 2 7 1 2 1 1 7 4 2 3 5 1 2 4 4 6 3 5 5 6 0 4 1 9 5 7 8 9 3 7 4
 [67] 6 4 3 0 7 0 2 9 1 7 3 2 9 7 7 6 2 7 8 4 7 3 6 1 3 6 9 3 1 4 1 7 6 9
Levels: 0 1 2 3 4 5 6 7 8 9
> 
> yhat_test[1:100]
  [1] 7 2 1 0 4 1 8 9 4 9 0 6 9 0 1 5 9 7 3 4 9 6 6 5 4 0 7 4 0 1 3 1 3 4 7 2 7 1 2 1 1 7 4 2 3 5 1 2 4 4 6 3 5 5 6 0 4 1 9 5 7 8 9 3 7 4
 [67] 6 4 3 0 7 0 2 9 1 7 3 2 9 7 7 6 2 7 8 4 7 3 6 1 3 6 9 3 1 4 1 7 6 9
Levels: 0 1 2 3 4 5 6 7 8 9
效果还可以


## 查看并保存结果
library(caret)
res[1, 1] <- round(h2o.confusionMatrix(yhat_train, y_train)$overall[1], 4)
res[1, 2] <- round(h2o.confusionMatrix(yhat_test, y_test)$overall[1], 4)
print(res)




(注意：程辑包‘h2o’是用R版本3.0.1 来建造的 ， 因此R base应该升级到相应版本， 不然就出现以下报错：

> library(h2o)
Error in eval(expr, envir, enclos) : 没有".getNamespace"这个函数
此外: 警告信息：
程辑包‘h2o’是用R版本3.0.1 来建造的 
Error : 程辑包‘h2o’里的R写碼载入失败
错误: ‘h2o’程辑包/名字空间载入失败

解决方法： 下载http://cran.r-project.org/bin/windows/base/old/3.0.1/R-3.0.1-win.exe 并安装， 更新其它包的 update.packages(ask=FALSE, checkBuilt = TRUE)
)
```






# 二、其他的一些深度学习包






## 1.      darch






[http://cran.um.ac.ir/web/packages/darch/index.html](http://cran.um.ac.ir/web/packages/darch/index.html)




Darch 是建立于Hinton和 Salakhutdinov的Matlab代码之上的，其实现方法基于Hinton两篇经典之作"A fast learning algorithm for deep beliefnets" (G. E. Hinton, S. Osindero, Y. W. Teh) 和"Reducingthe dimensionality of data with neural networks" (G. E. Hinton, R. R.Salakhutdinov)。该方法包括了对比散度的预训练和众所周知的训练算法（如反向传播法或共轭梯度法）的细调。







## 2.      deepnet




[http://cran.r-project.org/web/packages/deepnet/index.html](http://cran.r-project.org/web/packages/deepnet/index.html)




Deepnet ​实现了一些Deep Learning结构和Neural Network相关算法，包括BP，RBM训练，Deep Belief Net，Deep Auto-Encoder。作者称后续有时间会继续实现CNN和RNN算法等。




## 3.      Rdbn




[https://github.com/dankoc/Rdbn](https://github.com/dankoc/Rdbn)




Rdbn实现R环境的RBMs和DBNs的训练和学习。但目前还不能使用Rdbn，只能在github上参考。作者说正在测试和优化，要等排查完bug才能上CRAN，我也同样很期待这个包的上架。





## 4、MXNetR






You have found MXNet R Package! The MXNet R packages brings flexible and efficient GPU computing and state-of-art deep learning to R.
- It enables you to write seamless tensor/matrix computation with multiple GPUs in R.
- It also enables you to construct and customize the state-of-art deep learning models in R, and apply them to tasks such as image classification and data science challenges.

Sounds exciting? This page contains links to all the related documents on R package.

## [](http://www.tuicool.com/articles/ABR7ziy#)Resources
- [MXNet R Package Document](http://mxnet.readthedocs.org/en/latest/R-package/index.html)- Check this out for detailed documents, examples, installation guides.


## [](http://www.tuicool.com/articles/ABR7ziy#)Installation

Follow [Installation Guide](http://mxnet.readthedocs.org/en/latest/build.html)

## [](http://www.tuicool.com/articles/ABR7ziy#)License

MXNet R-package is licensed underBSD license.
















# 三、参考




1、[R语言和深度学习](http://blog.csdn.net/easonlv/article/details/23427809)：http://blog.csdn.net/easonlv/article/details/23427809

2、[R语言结合H2O做深度学习](http://blog.itpub.net/16582684/viewspace-1255976/) ：http://blog.itpub.net/16582684/viewspace-1255976/

3、MXNetR，原生态R语言深度学习，支持GPU计算，[https://github.com/dmlc/mxnet/tree/master/R-package](https://github.com/dmlc/mxnet/tree/master/R-package?utm_source=tuicool&utm_medium=referral)








































**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————




