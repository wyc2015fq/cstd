# 评分卡模型开发--WOE值计算 - 工作笔记 - CSDN博客





2018年11月23日 10:43:57[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5245








转自：[https://cloud.tencent.com/developer/article/1016331](https://cloud.tencent.com/developer/article/1016331)

对入模的定量和定性指标，分别进行连续变量分段（对定量指标进行分段），以便于计算定量指标的WOE和对离散变量进行必要的降维。对连续变量的分段方法通常分为等距分段和最优分段两种方法。等距分段是指将连续变量分为等距离的若干区间，然后在分别计算每个区间的WOE值。最优分段是指根据变量的分布属性，并结合该变量对违约状态变量预测能力的变化，按照一定的规则将属性接近的数值聚在一起，形成距离不相等的若干区间，最终得到对违约状态变量预测能力最强的最优分段。 我们首先选择对连续变量进行最优分段，在连续变量的分布不满足最优分段的要求时，在考虑对连续变量进行等距分段。此处，我们讲述的连续变量最优分段算法是基于条件推理树（conditional inference trees, Ctree）的递归分割算法，其基本原理是根据自变量的连续分布与因变量的二元分布之间的关系，采用递归的回归分析方法，逐层递归满足给定的显著性水平，此时获取的分段结果（位于Ctree的叶节点上）即为连续变量的最优分段。其核心算法用函数ctree()表示。 根据表3.13所示的定量入模指标，我们采用上述最优分段算法，得到的最优分段结果分别如下。 对变量“duration”进行最优分段：

```
#对duration进行最优分段
library(smbinning)
result<-smbinning(df=data,y="credit_risk",x="duration",p=0.05)
result$ivtable
```

变量“duration”的最优分段结果，如表3.14所示

![](https://ask.qcloudimg.com/http-save/yehe-1165572/xfyjlqlx3v.png?imageView2/2/w/1620)

```
#对amount进行最优分段
result<-smbinning(df=data,y="credit_risk",x="amount")
result$ivtable
```

![](https://ask.qcloudimg.com/http-save/yehe-1165572/rllvv5eab8.png?imageView2/2/w/1620)

```
#对变量age进行最优分段
result<-smbinning(df=data,y="credit_risk",x="age")
result$ivtable
```

![](https://ask.qcloudimg.com/http-save/yehe-1165572/ovoj5kcl85.png?imageView2/2/w/1620)

由于变量“installment_rate”的取值只有四个值，不适用于最优分段算法，只能采用等距分段，等距分段结果如表3.17 所示：

```
#对变量“installment_rate"的等距分段
install_data<-data[,c("installment_rate","credit_risk")]
tb1<-table(install_data)
total<-list()
for(i in 1:nrow(tb1))
{
  total[i]<-sum(tb1[i,])
}
t.tb1<-cbind(tb1,total)
goodrate<-as.numeric(t.tb1[,"0"])/as.numeric(t.tb1[,"total"])
badrate<-as.numeric(t.tb1[,"1"])/as.numeric(t.tb1[,"total"])
gb.tbl<-cbind(t.tb1,goodrate,badrate)
Odds<-goodrate/badrate
LnOdds<-log(Odds)
tt.tb1<-cbind(gb.tbl,Odds,LnOdds)
WoE<-log((as.numeric(tt.tb1[,"0"])/700)/(as.numeric(tt.tb1[,"1"])/300))
all.tb1<-cbind(tt.tb1,WoE)
all.tb1
```

![](https://ask.qcloudimg.com/http-save/yehe-1165572/puxpqm27gw.png?imageView2/2/w/1620)

结束对连续变量的分段及其WOE值的计算，接下来我们需要对离散变量做必要的降维处理及其WOE值得计算。我们首先查看下入模的定性指标的概况，如表3.18所示，代码如下：

```
discrete_data<-data[,c("status","credit_history","savings","purpose",
                       "property","credit_risk")]
summary(discrete_data)
```

![](https://ask.qcloudimg.com/http-save/yehe-1165572/h38mkq9r9y.jpeg?imageView2/2/w/1620)

由表3.18所示的入模定性指标的概况可知，定性指标“status” “credit_history”“savings”和“property”的维数最高为5维，最低为4维，而定性指标“purpose”的维数为10维，跟其他定性指标相比，明显高出很多。此时，通常会造成“维数灾难”，需要降维处理。在评级模型开发中的降维处理方法，通常是将属性相似的合并处理，以达到降维的目的。

```
#对purpose指标进行降维
x<-discrete_data[,c("purpose","credit_risk")]
d<-as.matrix(x)
for(i in 1:nrow(d))
{
  if(as.character(d[i,"purpose"])=="car (new)")
  {
    d[i,"purpose"]<-as.character("car(new/used)")
  }
  if(as.character(d[i,"purpose"])=="car (used)")
  {
    d[i,"purpose"]<-as.character("car(new/used)")
  }
  if(as.character(d[i,"purpose"])=="radio/television")
  {
    d[i,"purpose"]<-as.character("radio/television/furniture/equipment")
  }
  if(as.character(d[i,"purpose"])=="furniture/equipment")
  {
    d[i,"purpose"]<-as.character("radio/television/furniture/equipment")
  }
  if(as.character(d[i,"purpose"])=="others")
  {
    d[i,"purpose"]<-as.character("others/repairs/business")
  }
  if(as.character(d[i,"purpose"])=="repairs")
  {
    d[i,"purpose"]<-as.character("others/repairs/business")
  }
  if(as.character(d[i,"purpose"])=="business")
  {
    d[i,"purpose"]<-as.character("others/repairs/business")
  }
  if(as.character(d[i,"purpose"])=="retraining")
  {
    d[i,"purpose"]<-as.character("retraining/education")
  }
  if(as.character(d[i,"purpose"])=="education")
  {
    d[i,"purpose"]<-as.character("retraining/education")
  }
}

new_data<-cbind(discrete_data[,c(-4,-6)],d)
woemodel<-woe(credit_risk~.,data = new_data,zeroadj=0.5,applyontrain=TRUE)
woemodel$woe
```

![](https://ask.qcloudimg.com/http-save/yehe-1165572/0xtfpriiza.png?imageView2/2/w/1620)

至此，整个模型开发过程中第四步的工作，我们已经基本完成了。可见，该步骤在整个模型开发过程中占据非常重要的位置，定量和定性入模指标的筛选及其WOE值的计算，都会对整个信用风险评分卡产生重要的影响。在模型开发的第五步，我们将使用入模定量指标和入模定性指标的WOE值进行逻辑回归，并详细讲述生成信用风险评级模型标准评分卡的过程。



