# R语言︱排序问题 - 素质云笔记/Recorder... - CSDN博客





2016年04月18日 16:25:09[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：40669
所属专栏：[R的数据操作与清洗](https://blog.csdn.net/column/details/13587.html)
















# 数据排序



1、sort()，rank()，order()函数





|Sort|排序（默认升序，decreasing=T时为降序）|
|----|----|
|Order|排序（默认升序，decreasing=T时为降序）|




在R中，和排序相关的函数主要有三个：sort()，rank()，order()。

    sort(x)是对向量x进行排序，返回值排序后的数值向量。rank()是求秩的函数，它的返回值是这个向量中对应元素的“排名”。而order()的返回值是对应“排名”的元素所在向量中的位置。


    下面以一小段R代码来举例说明：




**[plain]**[view plain](http://blog.csdn.net/sinat_26917383/article/details/50688431#)[copy](http://blog.csdn.net/sinat_26917383/article/details/50688431#)

[print](http://blog.csdn.net/sinat_26917383/article/details/50688431#)[?](http://blog.csdn.net/sinat_26917383/article/details/50688431#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- x<-c(97,93,85,74,32,100,99,67)  
- sort(x)  
- [1]  32  67  74  85  93  97  99 100  
- order(x)  #order()的返回值是各个排名的学生成绩所在向量中的位置  
- [1] 5 8 4 3 2 1 7 6  
- rank(x)  #rank()的返回值是这组学生所对应的排名  
- [1] 6 5 4 3 1 8 7 2  





**深入理解一下:**

sort（）在单变量排序中，效果较好；

**order()≈原序号(sort())**  因为可以标记排序好之后的下标，在数据框中的排序操作，实用性超强，可以实现：

1、整个数据集按照某个变量（比如：按月份大小）排序；

2、整个数据集其中某个变量依据第二个变量（比如：月份）排序。



```
iris;iris[1:10,]
names(iris)

#单数据列，两者相同
sort(iris$Sepal.Length)
iris$Sepal.Length[order(iris$Sepal.Length)] 

#多数据列，order有奇效
iris[order(iris$setosa),]                 #按照setosa的大小，重排整个数据集
iris$Sepal.Length[order(iris$setosa)]     #按照照setosa的大小，重排Sepal.Length数据列
iris[order(iris$setosa),]$Sepal.Length    #与上句异曲同工
```

与which有一些地方的相似，which可以实现返回服从条件观测的行数。which又与subset子集筛选有关。（[详见which、subset子集筛选用法](http://blog.csdn.net/sinat_26917383/article/details/50688431)）



```
data$V1[which(data$V2<0)]                 #筛选出V1中，V2小于0的数字，跟order的作用些许相似
#order用法
iris$Sepal.Length[order(iris$setosa)]     #按照照setosa的大小，重排Sepal.Length数据列
```







2、dplyr包的一些应用








**[plain]**[view plain](http://blog.csdn.net/sinat_26917383/article/details/50688431#)[copy](http://blog.csdn.net/sinat_26917383/article/details/50688431#)

[print](http://blog.csdn.net/sinat_26917383/article/details/50688431#)[?](http://blog.csdn.net/sinat_26917383/article/details/50688431#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- #dplyr中基本函数 arrange——数据排序  
- Hdma_dat[order(Hdma_dat$survived),] #传统方法用order排序  
- arrange(Hdma_dat,survived) #将survived从小到大排序  
- arrange(Hdma_dat,desc(survived) #将survived从大到小排序  
- arrange(Hdma_dat,pclass,desc(survived) #先将pclass从小到大排序，再在那个数据基础上让survived从大到小排序  
- 





**使用场景（我经常搞错...）：**

譬如我想知道一组数：b = c(0.9984616870 ,1.0177739597 ,0.0004250664 ,0.0015771710, 1.0177739597)

场景一：最大值的位置信息，我想拿到的数据是每个数值对应的大小次序，升序，理应（3 4.5 1 2 4.5）

那么：



```
rank(b)
order(b)
```
如果降序，就不一样了：

```
> order(c(0.9984616870 ,1.0177739597 ,0.0004250664 ,0.0015771710, 1.0177739597) ,decreasing = T)
[1] 2 5 1 4 3
```
order=rank+sort功能=次序信息（rank）+次序排序（sort）


接下来的两个用法是我很容易搞错的：

我想拿到 降序 + 不排序的次序信息，rank不适合；

               降序 + 排序的次序信息，order适合


若：

（1）按照某行顺序从大到小重排另一行：


`data$x1[order(data$x2)]`
（2）按照某行最大值对位的另一行：

`data$x1[order(data$x2)[1] ]`
第二种办法：


`data$x1[rank(data$x2) == max值]`
注意区别。            


