# R语言︱缺失值处理 - 素质云笔记/Recorder... - CSDN博客





2016年04月08日 23:37:40[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：7546
所属专栏：[R的数据操作与清洗](https://blog.csdn.net/column/details/13587.html)












#缺失值


```
an=c(1,2,NA)
is.na(an)                         #会形成一个布尔向量
```

布尔向量就是一群像（FALSE,FALSE,TURE）这样的向量。




关于缺失值还有一个函数：complete.cases函数

该函数与is.na的区别在于：

1、输出数据格式不同。is.na按照数据框格式形成一个（FALSE,FALSE,TURE）列，而complete.cases形成是一个数列向量，不再是按照数据框格式；

2、输出数据内容不同。complete.cases输出的逻辑向量与is.na正好相反，is.na的TURE为是缺失值；complete.cases的TURE为完整值。



```
an=c(1,2,NA)
sum(complete.cases(an)) 
sum(!complete.cases(an))#==sum(is.na(an))
mean(!complete.cases(an))  #获得缺失比例==1/3
```

疑惑：为什么布尔向量，sum一下可以得到数值？   ——因为R默认将TURE、FALSE当做1、0





#计算缺失值个数

```
sum(is.na(an))                    #单数列，sum一下可以直接计算“Ture”的数值和
colSums(is.na(an),na.rm = T)      #多维数列，按列，na.rm为是否需要忽略缺失值，na.rm=T表示忽略，删除
rowSums(is.na(an),na.rm = T)      #多维数列，按行，na.rm为是否需要忽略缺失值，na.rm=T表示忽略，删除
```




#数据框中的缺失值操作
`#数据框中的缺失值操作`
```
y <- an[is.na(an)]               #选中缺失值
y<-  an[is.na(an)=="TRUE"]       #上同，选中缺失值
an[is.na(an)] <- 0               # 表示将向量x中所以NA元素用0来代替
an[(!is.na(an)) & x>0] -> z      #可以用 & 加入其他条件，进行筛选
```




————————————————————————————————————————————————————————————

缺失值检测解决方案：

关于缺失值的检测应该包括：缺失值数量、缺失值比例、缺失值与完整值数据筛选。



```
#缺失值解决方案
sum(complete.cases(saledata))         #is.na(saledata)
sum(!complete.cases(saledata))
mean(!complete.cases(saledata))       #1/201数字，缺失值比例
saledata[!complete.cases(saledata),]  #筛选出缺失值的数值
```







