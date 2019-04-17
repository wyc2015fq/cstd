# 笔记+R︱风控模型中变量粗筛（随机森林party包）+细筛（woe包） - 素质云笔记/Recorder... - CSDN博客





2016年06月21日 17:32:25[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：14095












**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————




本内容来源于CDA-DSC课程内容，原内容为《第16讲 汽车金融信用违约预测模型案例》。




建立违约预测模型的过程中，变量的筛选尤为重要。需要经历多次的筛选，在课程案例中通过了随机森林进行变量的粗筛，通过WOE转化+决策树模型进行变量细筛。




一、变量粗筛——随机森林模型






与randomForest包不同之处在于，party可以处理缺失值，而这个包可以。






**[html]**[view plain](http://blog.csdn.net/sinat_26917383/article/details/51308061#)[copy](http://blog.csdn.net/sinat_26917383/article/details/51308061#)

[print](http://blog.csdn.net/sinat_26917383/article/details/51308061#)[?](http://blog.csdn.net/sinat_26917383/article/details/51308061#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- library(party)  
-    #与randomForest包不同之处在于，party可以处理缺失值，而这个包可以  
- set.seed(42)  
- crf<-cforest(y~.,control = cforest_unbiased(mtry = 2, ntree = 50), data=step2_1)  
- varimpt<-data.frame(varimp(crf))  






party包中的随机森林建模函数为cforest函数，
mtry代表在每一棵树的每个节点处随机抽取mtry 个特征，通过计算每个特征蕴含的信息量，特征中选择一个最具有分类能力的特征进行节点分裂。


varimp代表重要性函数。( [R语言︱决策树族——随机森林算法](http://blog.csdn.net/sinat_26917383/article/details/51308061))





二、R语言实现WOE转化+变量细筛

R语言中有一个woe包，可以实现WOE转化的同时，通过WOE值进行y~x的决策树建立，应用决策树的重要性来进行变量细筛。

woe包需要从github中下载得到：



```
#library(devtools)
#install_github("riv","tomasgreif")
```


```
library(woe)          
IV<-iv.mult(step2_2,"y",TRUE)   #原理是以Y作为被解释变量，其他作为解释变量，建立决策树模型
iv.plot.summary(IV)
```


summary(step2_3)
不能只看统计量，还要仔细的察看每个变量的取值情况。一般WOE建模数据是经过抽样的，因此可能需要多建模几次，看看不同的变量特征变化，再来进行变量细筛。





