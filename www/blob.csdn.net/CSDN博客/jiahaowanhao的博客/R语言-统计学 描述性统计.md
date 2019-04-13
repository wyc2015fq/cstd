
# R语言-统计学 描述性统计 - jiahaowanhao的博客 - CSDN博客


2018年02月26日 21:01:38[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：377


[R语言-统计学 描述性统计](http://cda.pinggu.org/view/24845.html)
描述定量数据的数值方法：中心趋势度量   变异的度量   相对位置的度量。
1.中心趋势度量 ： 算数平均 中位数   众数
1.1  在R中计算平均数的函数  mean( )
常规的mean() 函数用法
mean(x, trim = 0, na.rm = FALSE, ...)
参数说明： x 对象名称
trim  过滤掉异常值 ，按照距离平均值的远近距离排除，如对象中含有10个数据，排除最高和最低值，trim=0.2
na.rm  默认为F   表示是否计入空值
实例1： 做一个稍微复杂点的操作，用r做数据透视表并把结果转换为matrix ，对行列求和。(仅娱乐，无实用价值)
demo <- mtcars[1:6,] \# 调用R自带函数集，并去前6行
toushi <- aggregate(mtcars[,5:6] ,by = list(cyl = mtcars$cyl),sum) \# 数据透视表求和
toushi <- as.matrix(toushi)  \# 将结果的数据框转化成矩阵
\#(toushi <- apply(toushi,c(1,2),sum))
(rowSums(toushi))  \# 行求和
(colSums(toushi))  \# 列求和
toushi <- rbind(toushi,rowSums(toushi))  \#将行求和结果并入最后一行
toushi <- cbind(toushi,colSums(toushi))  \#将列求和结果并入最后一列
1.2 中位数和众数
对于偏度极大的数据集，中位数能更好的描述数据分布的中心。
很少用众数作为数据数据趋势的度量，只有当对y出现的相对频率感兴趣时，才会考虑到众数。
R实现中位数  ：
median(x, na.rm = FALSE)
R中没有直接插找众数的命令
which.max(table(x))
2.变异的度量 ： 极差  方差  标准差
2.1 .极差 = max（）- min（）
2.2  方差和标准差
对一个有n个测量值的有限总体来说，方差计算公式的分母为n。关于样本方差和总体方差分母的差异原因，可自行百度搜索。
R语言计算方差的函数： var(x,)
w<-c(75.0,64.0,47.4,66.9,62.2,62.2,58.7,63,5,66.6,64.0,57.0,69.0,56.9,50.0,72.0)
var(w)
\# 附加指数点：标准差的两个有用法则：经验法则 和 切比雪夫法则，共同说明一个问题，对于任意大于1的正数k，至少有（1-1/k^2）的测试值落在平均值的k个标准值范围内。
3.变异的度量 ： 百分位数  Z得分
3.1 .最常见的四分位数（一般从大到小）
quantile(x, probs = seq(0, 1, 0.25), na.rm = FALSE,names = TRUE, type = 7, ...)

