
# 集成学习——Adaboost分类 - 喜欢打酱油的老鸟 - CSDN博客


2019年04月02日 08:42:19[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：18标签：[集成学习																](https://so.csdn.net/so/search/s.do?q=集成学习&t=blog)[Bagging																](https://so.csdn.net/so/search/s.do?q=Bagging&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=集成学习&t=blog)个人分类：[人工智能																](https://blog.csdn.net/weixin_42137700/article/category/7820233)


[https://www.toutiao.com/a6674839167580504587/](https://www.toutiao.com/a6674839167580504587/)
上一期分享了集成学习之Bagging分类的方法，这一期分享它的另外一种方法，**Adaboost分类方**。
Adaboost分类方法和Bagging分类类似，不同之处在于，Adaboost每次用自助法抽样来构建树时，都根据前一棵树的结果对于误判的观测值增加抽样权重，使得下一棵树能够对误判的观测值有更多的代表性。
最终的结果由所有的树加权投票得到，权重根据各棵树的精度来确定。
具体来说，如果在上一次分类器的预测中，样本i被错误分类了，那么，在这一次分类器的训练样本抽取过程中，样本i就会被赋予较高的权重，以使其能够以较大的可能被抽中，从而提高其被正确分类的概率。
它显著的提高了集成分类器的稳定性和准确性。
下面就让我们开启Adaboost分类算法之旅吧！
假定样本为（X1,Y1),(X2,Y2),...(Xn,Yn)，为描述简单，假定因变量为二分类变量，Y∈{0，1}，即只有两类。
Adaboost分类具体步骤如下：
（1）对观测值点选择初始的自助法抽样权
![集成学习——Adaboost分类](http://p1.pstatp.com/large/pgc-image/787381beae2048c18f2048fe8799dc8e)
设m=0；
（2）把m增加1，用基本方法（这里是分类树）拟合加权抽样的数据，权重：
![集成学习——Adaboost分类](http://p3.pstatp.com/large/pgc-image/faf58e9b7f704702aa0365464b50df9e)
，产生分类器：
![集成学习——Adaboost分类](http://p3.pstatp.com/large/pgc-image/a471692e0d4f47a2a860c932e2a686d7)
（3）计算样本内的加权误判率：
![集成学习——Adaboost分类](http://p3.pstatp.com/large/pgc-image/85f2fd56648e475f8e36e8dcee459397)
![集成学习——Adaboost分类](http://p3.pstatp.com/large/pgc-image/25b3c3719c6644d3ae20a558addf5dbe)
，然后更新权重：
![集成学习——Adaboost分类](http://p3.pstatp.com/large/pgc-image/c13b0a87ae3449159f47203409c6be8d)
![集成学习——Adaboost分类](http://p3.pstatp.com/large/pgc-image/47a6ebf567274c81b591b425bc4b6c13)
，重复（2）和（3）步，直到预先设定的步数m=m_stop，这样就立了根据加权投票来分类的组合分类器：
![集成学习——Adaboost分类](http://p1.pstatp.com/large/pgc-image/bdf84b0ab40044f1838099ef5f08b20e)
哇塞，说了这么多，是不是都糊涂了，这是什么鬼，没事，我们下面看一道案例，重点学会运用就可以啦！
依然使用上一期的数据。我们使用**Bank Marketing数据集**，可以在网上下载到：http://archive.ics.uci.edu/ml/datasets/Bank+Marketing.
该数据集来自于葡萄牙银行机构的一个基于电话跟踪的商业营销项目，其中收录了包括银行客户个人信息及与电话跟踪咨询结果有关的16个自变量，以及1个因变量，即该客户是否订阅了银行的定期存款。
首先我们看一下数据集：
setwd("C:\Users\dell\Desktop\集成学习")
data=read.csv("bank.csv",header=TRUE,sep=";")
head(data)
dim(data)
summary(data)
sum(data$y=="yes"); sum(data$y=="no")那么我们使用数据集的四分之一的样本作为测试集来评价分类器的性能：
sub=sample(1:nrow(data),round(nrow(data)/4))
length(sub)
data_train=data[-sub,]
data_test=data[sub,]
dim(data_train);dim(data_test)即训练集和测试集样本数分别为3391和1130.
![集成学习——Adaboost分类](http://p1.pstatp.com/large/pgc-image/40129999e7084b24b925f4303ab9321d)
下面建立Adaboost分类模型：
需要加载相应的包：
install.packages("adabag")
install.packages("rpart")
library(adabag)
library(rpart)其中Bagging算法格式：
boosting(formula, data, boos = TRUE, mfinal = 100, coeflearn = 'Breiman', control,...)其中**boos参数**用于选择在当下的迭代过程中，是否用各观测样本的相应权重来抽取boostrap样本，默认为TRUE。如果是FALSE，则每个观测样本都以其相应权重在迭代过程中被使用。coeflearn用于选择权重更新系数alpha的计算方式，默认取Breiman.另外也可设置为"Freund","Zhu"。
建立模型如下：
boo=boosting(y~.,data_train,mfinal=5)
pre_boo=predict(boo,data_test)计算总体错误率：
err_boo=sum(pre_boo$class!=data_test$y)/nrow(data_test)通过混淆矩阵发现，选择no的人多，yes的人少，严重的数据不平衡，因此，我们把两类分开来看各自的预测错误率的结果；
sub_minor=which(data_test$y=="yes")
sub_major=which(data_test$y=="no")
length(sub_minor); length(sub_major)首先计算少数类"yes"的错误率：
err_minor_boo=sum(pre_boo$class[sub_minor]!=data_test$y[sub_minor])/length(sub_minor)其次计算多数类"no"的错误率：
err_major_boo=sum(pre_boo$class[sub_major]!=data_test$y[sub_major])/length(sub_major)
err_boo; err_minor_boo; err_major_boo结果如下：
![集成学习——Adaboost分类](http://p3.pstatp.com/large/pgc-image/88643b4ca1654237be2f7cbbebe296df)
与上一期结果比较：
![集成学习——Adaboost分类](http://p1.pstatp.com/large/pgc-image/2549009185c840bfa59ecc53d9d65cd7)
发现除了少数类的预测错误率相对于Bagging有所提升，整体和多数类错误率均有所下降，Adaboost确实可以修正数据集的不平衡性，但总体来说，与Bagging的效果差别不大。关于这两种算法预测效果差异的评价需要在更丰富的数据分享经验基础上给出，大家可以查找更多的资料了解。
集成学习两种算法的内容就讲到这里啦，大家有任何疑问都可以在下方评论区提出，大家一起讨论。愿与君共同进步！

