# R语言︱关联规则+时间因素=序贯关联规则 - 素质云笔记/Recorder... - CSDN博客





2016年06月12日 14:59:42[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：2171











序贯模型=关联规则+时间因素。

了解这个模型可以参考李明老师的《R语言与网站分析 [李明著][机械工业出版社][2014.04][446页]》，第九章，第二节的“序列模型关联分析”。

以下贴一个网络中关于序贯模型，**R语言的arulesSequences包**：转载于网易博客：Tony Woo




**__________________________________________________________________________________________**







今天下午基本上把通过arulesSequences来进行序列模式挖掘搞明白了，晚上又把arulesSequences中最重要的函数cspade查看了一下。Mark一下。

还是先简单写一个模式挖掘的例子。




# 1、数据准备




假设数据存放在E盘下的test.txt，而且E盘中的数据为：


1 10 2 C D A F H E

1 15 3 A B C E A F

1 20 3 A B F D C F

1 25 4 A C D F D D

2 15 3 A B F 

2 20 1 E

3 10 3 B F

4 10 3 D G H

4 20 2 B F

4 25 3 A G H F

4 30 12 A H H H A F F F A G G G





数据比zaki稍微复杂一些，不过只是多添加了一些个别内容。







# 2、建模





```
>x=read_baskets(con="E:/zaki.txt",info=c("sequenceID","eventID","SIZE"))
>s1 <- cspade(x, parameter = list(support = 0.6,maxlen=3), control = list(verbose = TRUE))
>as(s1,"data.frame")
```






主要就这么三步，就完成了序列模式挖掘。现在需要看一下核心的函数cspade()。







# 3、cspade函数解释




根据文档，cspade函数结构如下：

`cspade(data, parameter = NULL, control = NULL, tmpdir = tempdir())`


其实，参数data没啥可说的，就导入transactions类型的数据就可以了。

parameter是设定各种参数，这个还需要认真了解一下。




parameter中，可选的参数有如下几个：




**support：**0-1之间的一个数值，代表得到的高频序列的最小支持度。

        支持度其实是这样计算的：看上面的data中有4个序列，比如我们要计算｛A｝的支持度，则直接看｛A｝在4个序列中出现过几次，用次数再除以4就得到了支持度。至于一次订单中出现多少次A，则对序列挖掘是没啥影响的。




**maxsize：**一个整数值，代表在寻找高频序列的过程中，任意一个序列里面的每一个元素的最多能有几个项。

        举个例子， <{D,H},{B,F},{A}> 是我们通过序列挖掘得到的一个序列s，那么序列s包含3个元素element，其中第一个元素又包含2个项item。通过设定maxsize，可以在序列挖掘中设定1对1或多对1的不同挖掘方式。




**maxlen：**一个整数值，代表挖掘的序列最大可以是多长，也即一个序列最多可以有几个元素。

        比如，如果s1 <- cspade(x, parameter = list(support = 0.6,maxlen=2), control = list(verbose = TRUE))  ，那么最终得到的序列可能为： <{D,H},{B}><{A,F,H},{A,F}>



  因此，通过maxlen参数可以去挖掘较短的序列。

mingap：一个整数值，确定两个连续的订单之间的最小时间差值，默认为none。

maxgap：一个整数值，确定两个连续的订单之间的最大时间差值，默认为none。

maxwin：一个整数值，确定一个序列中任意两个订单之间的最大时间差值，默认为none。







control其实是对内存了什么的控制，一般用不到，第四个也是用不到。因此，cspade函数主要就parameter的设定。







