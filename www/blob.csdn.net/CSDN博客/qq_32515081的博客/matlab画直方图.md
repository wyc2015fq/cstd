# matlab画直方图 - qq_32515081的博客 - CSDN博客
2018年03月09日 20:45:00[TengAdventure](https://me.csdn.net/qq_32515081)阅读数：858
matlab中的数值分析包括了直方图绘制，这里主要介绍hist和histfit命令
hist是直接绘制直方图，我们首先给出一个随机数组：
aa=randn(1000,1);
hist(aa);
%当然可以设定直方图内条形的数量：
hist(aa,20);
横坐标为数值的分布情况，纵坐标为在横坐标范围内的数据量。
hist还可以输出数据，比如下图：
其中，counts是表示将aa从小到大分成了十组数，
centers表示每组数据的中间值。
下面介绍histfit命令。直接用法：
histfit(aa);
如下图，比hist多了一条曲线，如果输入如下命令：
h = histfit(aa);
h即代表了那条曲线，可以打开h对曲线进行修改。
当然，histfit也可以指定直方图内条形的数量，默认的数量是根据数组的平方根确定。同上图一样，我们来指定一百个：
histfit(aa,100);
详见：https://jingyan.baidu.com/article/b24f6c820f1ba086bfe5da25.html
