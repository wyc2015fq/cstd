# R语言︱噪声数据处理、数据分组——分箱法（离散化、等级化） - 素质云笔记/Recorder... - CSDN博客





2016年11月25日 17:06:55[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：24885
所属专栏：[R的数据操作与清洗](https://blog.csdn.net/column/details/13587.html)













**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————


**分箱法在实际案例操作过程中较为常见，能够将一些数据离散化，等级化**，比如年龄段，我们并不想知道确切的几岁，于是乎可以将其分组、分段。

基础函数中cut能够进行简单分组，并且可以用于等宽分箱法。

cut函数：cut(x, n)：将连续型变量x分割为有着n个水平的因子.(参考来自：[R语言︱数据集分组、筛选](http://blog.csdn.net/sinat_26917383/article/details/50688431))







**[plain]**[view plain](http://blog.csdn.net/sinat_26917383/article/details/50688431#)[copy](http://blog.csdn.net/sinat_26917383/article/details/50688431#)

[print](http://blog.csdn.net/sinat_26917383/article/details/50688431#)[?](http://blog.csdn.net/sinat_26917383/article/details/50688431#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- cut(x, breaks, labels = NULL,  
-     include.lowest = FALSE, right = TRUE, dig.lab = 3,  
-     ordered_result = FALSE, ...)  
- 


分箱法分为等深分箱（样本量一致，比等宽好）、等宽分箱（cut函数可以直接获取）。







![](https://img-blog.csdn.net/20160607163442242)











着重看一下等深分箱法,笔者在这根据CDA DSC课程code之上，自己编译了一个等深分箱函数sbdeep。






```
sbdeep=function(data,parts,xiaoz){
  parts<-parts         #分几个箱
  xiaoz<-xiaoz         #极小值
    value<-quantile(data,probs = seq(0,1,1/parts))  #这里以data等比分为4段，步长为1/4
  number<-mapply(function(x){
    for (i in 1:(parts-1)) 
    {
      if(x>=(value[i]-xiaoz)&x<value[i+1])
      {
        return(i)
      }
    }
    if(x+xiaoz>value[parts])
    {
      return(parts)
    }
    return(-1)
  },data)
  #打标签L1L2L3L4
  return(list(degree=paste("L",number,sep=""),degreevalue=number,value=table(value),number=table(number)))               #将连续变量转化成定序变量，此时为L1,L2,L3,L4...根据parts
}
```

该函数是对单个序列数据进行等深分箱，可以返回四类：


一个基于L1L2L3....的每个指标标签序列degree；

标签序列值degreevalue，

每个百分位数对应的变量值value，

不同百分点的数量number。




————————————————————————————————————

# 应用一：R语言等宽分箱小案例







      R语言的等宽分箱法一般都是用cut来获取，但是用法来说在网上还是比较少见的。**譬如这里有一个需求就是把连续数列，根据等宽分箱的办法切分开来。**这个应该怎么做呢？




      来看一个cut的案例：



```
> a <- c(1,2,3,4,5,6,4,3,2,1)
> cut(a,10)
 [1] (0.995,1.5] (1.5,2]     (2.5,3]     (3.5,4]     (4.5,5]     (5.5,6]     (3.5,4]     (2.5,3]     (1.5,2]     (0.995,1.5]
Levels: (0.995,1.5] (1.5,2] (2,2.5] (2.5,3] (3,3.5] (3.5,4] (4,4.5] (4.5,5] (5,5.5] (5.5,6]
> cut(a,10,labels=F)
 [1]  1  2  4  6  8 10  6  4  2  1
```

      一个数列，简单的cut滞后，就变成一个levels，因子型的一个区间范围，但是这个结果一般不是我们想要的，我们想要对连续数据进行切割。那么就是用R语言中的cut函数的，labels参数。


      可以从案例中看到，labels=F之后，就变成了一系列等级型的分组序号，就像聚类一样，模型跑出来之后，就给数列打了一个标签。那么就可以这样选择你想要的，譬如我要选择连续变量的数值上的前10%的数值：


`a[cut(a,10,labels=F)==10]`




**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————




