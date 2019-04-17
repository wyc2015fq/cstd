# RFM模型及R语言实现 - 素质云笔记/Recorder... - CSDN博客





2016年02月14日 13:54:21[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：13711












**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————




# **一、基本概念**






根据美国数据库营销研究所Arthur Hughes的研究，客户数据库中有三个神奇的要素，这三个要素构成了数据分析最好的指标：最近一次消费(Recency)、消费频率(Frequency)、消费金额(Monetary)。

 RFM模型：R(Recency)表示客户最近一次购买的时间有多远，F(Frequency)表示客户在最近一段时间内购买的次数，M  (Monetary)表示客户在最近一段时间内购买的金额。

一般原始数据为3个字段：客户ID、购买时间（日期格式）、购买金额，用数据挖掘软件处理，加权（考虑权重）得到RFM得分，进而可以进行客户细分，客户等级分类，Customer Level Value得分排序等，实现数据库营销！




![](https://img-blog.csdn.net/20160214155848701?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





# 二、算法流程




拿到了RFM模型的Recency_Score、Frequency_Score、Monetary_Score和RFM_Score之后，对RFM得分进行了五等分切割（我个人觉得按照分位数来定等级，根据分位数曲线的拐点来分也可以），采用100、10、1加权得到RFM得分表明了125个RFM魔方块。

在R中实现比较简单，用cut 或 quantile 函数都可以实现。







# **三、深入分析**




传统的RFM模型到此也就完成了，但125个细分市场太多啦无法针对性营销也需要识别客户特征和行为，有必要进一步细分客户群。

可以进行不同块的对比分析：均值分析、块类别分析等等

![](http://1873.img.pp.sohu.com.cn/images/blog/2012/1/21/23/14/u129596371_135c0245567g213.jpg)

  接下来，我们继续采用挖掘工具对R、F、M三个字段进行聚类分析。

 这时候我们要考虑是直接用R(Recency)、F(Frequency)、M  (Monetary)三个变量还是要进行变换，因为R、F、M三个字段的测量尺度不同最好对三个变量进行标准化，例如：Z得分（实际情况可以选择线性插值法，比较法，对标法等标准化）！

另外一个考虑：就是R、F、M三个指标的权重该如何考虑，在现实营销中这三个指标重要性显然不同！有资料研究表明：对RFM各变量的指标权重问题,Hughes,Arthur认为RFM在衡量一个问题上的权重是一致的,因而并没有给予不同的划分。而Stone,Bob通过对信用卡的实证分析,认为各个指标的权重并不相同,应该给予频度最高,近度次之,值度最低的权重。

这里我们采用加权方法：WR=2 WF=3 WM=5的简单加权法（实际情况需要专家或营销人员测定）；具体选择哪种聚类方法和聚类数需要反复测试和评估，同时也要比较三种方法哪种方式更理想！

 输出结果后将R、F、M三个字段分类与该字段的均值进行比较，利用Excel软件的条件格式给出与均值比较的趋势！结合RFM模型魔方块的分类识别客户类型：通过RFM分析将客户群体划分成重要保持客户、重要发展客户、重要挽留客户、一般重要客户、一般客户、无价值客户等六个级别；（有可能某个级别不存在）。

 另外一个考虑是针对R、F、M三个指标的标准化得分按聚类结果进行加权计算，然后进行综合得分排名，识别各个类别的客户价值水平；

至此如果我们通过对RFM模型分析和进行的客户细分满意的话，可能分析就此结束。

如果我们还有客户背景资料信息库，可以将聚类结果和RFM得分作为自变量进行其他数据挖掘建模工作！




# **举例代码：**





```
library(magrittr)
library(dplyr)
sales=data.frame(sample(1000:1999,replace=T,size=10000),abs(round(rnorm(10000,28,13))))
#  rnorm(n, mean = 0, sd = 1),以上产生均值为28，方差为13的1万个数，用来模拟用户的消费情况.
#  sample(1000:1999,replace=T,size=10000), 从1000到1999这些数字中，有放回抽样进行取样，一共取1万个样本，平均每个样本取10次。
names(sales)=c("CustomerId","Sales Value")

# 查看生成的数据
# dplyr::arrange(dplyr::count(sales,CustomerId), desc(n))
# Source: local data frame [1,000 x 2]
# CustomerId     n
# 1        1052 24
# 2        1739 23
# 3        1843 21
# 4        1867 21
# > dplyr::arrange(dplyr::count(sales,CustomerId),n)
# Source: local data frame [1,000 x 2]
# CustomerId n
# 1        1962 1
# 2        1264 2
# 3        1566 2

#generating random dates
#runif(n, min = 0, max = 1), stats::runif(10000),产生1万个服从0,1上均匀分布的数
sales.dates <- as.Date("2012/1/1") + 700*sort(stats::runif(10000))

sales=cbind(sales,sales.dates)
str(sales)

sales$recency=round(as.numeric(difftime(Sys.Date(),sales[,3],units="days")) )

# 更改单列的列名
names(sales)<-c("CustomerId", "Purchase.Value", "sales.dates","recency")

## Creating Total Sales(Monetization),Frequency, Last Purchase date for each customer
#aggregate:首先将数据进行分组（按行），然后对每一组数据进行函数统计，最后把结果组合成一个比较nice的表格返回
# aggregate(x, by, FUN, ..., simplify = TRUE)

#统计每个用户的购买总值
salesM=aggregate(sales[,2],list(sales$CustomerId),sum)
names(salesM)=c("CustomerId","Monetization")

salesF=aggregate(sales[,2],list(sales$CustomerId),length)
names(salesF)=c("CustomerId","Frequency")

salesR=aggregate(sales[,4],list(sales$CustomerId),min)
names(salesR)=c("CustomerId","Recency")

##Merging R,F,M
# merge:将两个DF融合成一个DF，merge(x, y, by = intersect(names(x), names(y)) ）
test1=merge(salesF,salesR,"CustomerId")

salesRFM=merge(salesM,test1,"CustomerId")

##Creating R,F,M levels 
#切分成100份，实际只用前5份
salesRFM$rankR=cut(salesRFM$Recency, 100,labels=F) #rankR 1 is very recent while rankR 5 is least recent
salesRFM$rankF=cut(salesRFM$Frequency, 100,labels=F)#rankF 1 is least frequent while rankF 5 is most frequent
salesRFM$rankM=cut(salesRFM$Monetization, 100,labels=F)#rankM 1 is lowest sales while rankM 5 is highest sales

##Looking at RFM tables
# 以table形式查看
table(salesRFM[,5:6])
table(salesRFM[,6:7])
table(salesRFM[,5:7])
```



# 五、后续分析——针对性服务




1. R=短 F=高 M=高
　　 这类客户可以采用“重要保持”的措施。这类消费者的消费频次和客单价都高于均值，要保持这种消费者的有效方法是给予一定的长期优惠，如给予店铺VIP资格。
2. R=短 F=低 M=低
　　 这类用户最近有消费，但频次和客单价低于平均值。针对此客户重要做的是“重要发展”：给予满就送、搭配套餐式的促销组合推荐。如买了热水袋，可以推荐美臀坐垫、睡衣、美发卷等关联产品组合促销，拉升他们的消费频次。关联销售则可以拉动客单价的提升。
3. R=短 F=低 M=高
　　 这类消费者属于店铺销量的主要贡献者，最近有消费，消费频次低于平均值，但客单价却高于平均值。这些消费者的价值关键点在于可以通过提高消费频次来加大其对店铺的贡献，建议采用“重要价值”的方式来提升其贡献值。建议卖家分析这部分客户最近的消费产品，交叉定向发送差异品类产品及促销信息，促进再次购买。如客户麦乐电吹风，要针对性发送浴室相关的居家用品。
4. R=长 F=高 M=高


　　这类消费者的考虑时间较长，虽然购买频率和购买金额都较高，但容易“溜走”，需要卖家给予“重要挽留”式的措施去加以挽留。卖家要有有针对性的唤醒措施，可以发送定向优惠，在指定时间内满多少可享受折扣。






![](http://res.eshangke.com/uploads/538c50770abac.jpg)








# **六、划分客户生命周期**




　　根据客户不同回购周期所占有的客户比例，可以为客户生命周期的划分提供依据，可以把客户划分为： “活跃”、“沉默”、“睡眠”、“流失”四个生命周期。






![](http://res.eshangke.com/uploads/538c50e247cad.jpg)





参考文献：

1、http://blog.csdn.net/wa2003/article/details/48265783

















**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————




