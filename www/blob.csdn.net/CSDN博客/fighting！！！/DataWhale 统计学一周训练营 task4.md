# DataWhale 统计学一周训练营 task4 - fighting！！！ - CSDN博客
2019年03月05日 15:39:09[dujiahei](https://me.csdn.net/dujiahei)阅读数：29
- 参考
[https://blog.csdn.net/Datawhale/article/details/81504090](https://blog.csdn.net/Datawhale/article/details/81504090)
[https://blog.csdn.net/Datawhale/article/details/81303868](https://blog.csdn.net/Datawhale/article/details/81303868)
[https://www.jianshu.com/p/ed3172b2379b](https://www.jianshu.com/p/ed3172b2379b)
[http://www.cnblogs.com/Joeyyoung/p/10212733.html](http://www.cnblogs.com/Joeyyoung/p/10212733.html)​​
[https://blog.csdn.net/flowingflying/article/details/8070181](https://blog.csdn.net/flowingflying/article/details/8070181)​
	《商务与经济统计》（安德森）​​
	​《应用概率统计》（天津大学出版社）
	​
方差分析与回归分析（参数估计和假设检验理论的应用与延伸）
方差分析：从方差的角度分析试验数据，对具有相同（未知）方差的多个正态总体的均值进行统计推断的方法。
回归分析：确定两种或两种以上变数间互相依赖的定量关系的一种统计分析方法。​
- 卡方分布与方差分析
	- 卡方分布
		随机变量X是独立的标准正态分布变量，X～N（0，1），即E(X)=0, Var(X)=1。
		Q1=X12，Q1是一个Chi-Square分布，记为，degree of freedom is 1
		Q2=X12+ X22，Q2是一个Chi-Square分布，记为 ，degree of freedom is 2
		以此类推。图为Chi-Square的分布图。有χ table可供查询，例如P(Q2>2.41) = 0.3
- 皮尔逊卡方检验
		实际观察次数O与某理论次数(E又称期望次数)之差的平方再除以理论次数乃是一个与抽样分布之一的χ2分布非常近似的次数分布。
		而自由度则是不相干事件得数目。
		如同n足够大是，二项分布和正态分布非常吻合一样，这里也不做理解证明，由法国数学家Pearson给出，就当给了个工具，我们相信工具有效，来使用工具，常用于检查出现频率。
- 列表卡方检验
- 一元线性回归分析
	- 一元线性回归模型
		
![](https://img.mubu.com/document_image/1f1e296b-948a-4738-9808-22f8a8bb07f7-2329912.jpg)
		线性回归是基于某个变量X（自变量）来预测变量Y（因变量）的值。
		​
- 最小二乘法
		最小二乘法（又称最小平方法）是一种数学优化技术。它通过最小化误差的平方和寻找数据的最佳函数匹配。利用最小二乘法可以简便地求得未知的数据，并使得这些求得的数据与实际数据之间误差的平方和为最小。
- 决定系数R^2
		决定系数（coefficient ofdetermination），有的教材上翻译为判定系数，也称为拟合优度。
		决定系数反应了y的波动有多少百分比能被x的波动所描述，即表征依变数Y的变异中有多少百分比,可由控制的自变数X来解释.
		表达式：R^2=SSR/SST=1-SSE/SST
		其中：SST=SSR+SSE，SST(total sum of squares)为总平方和，SSR(regression sum of squares)为回归平方和，SSE(error sum of squares) 为残差平方和。
		注：（不同书命名不同）
		回归平方和：SSR(Sum of Squares forregression) = ESS (explained sum of squares)
		残差平方和：SSE（Sum of Squares for Error） = RSS(residual sum of squares)
		总离差平方和：SST(Sum of Squares fortotal) = TSS(total sum of squares)
		SSE+SSR=SST RSS+ESS=TSS
		意义：拟合优度越大，自变量对因变量的解释程度越高，自变量引起的变动占总变动的百分比高。观察点在回归直线附近越密集。
		取值范围：0-1.​
- 协方差与相关系数
		度量各个维度偏离其均值的程度。协方差是为多维变量创立的，目的是为了描述两个变量的关系（正相关，负向关。相互独立）。需注意协方差只能两个维度算，多个维度的协方差形成协方差矩阵。
		​协方差Covariance，Cov(X,Y)=E[(X-E(X))(Y-E(Y))]，观察X与其均值之差X-E(X)以及Y与其均值之差E(Y)之间的同步关系，是否X-E(X)上升，Y-E(Y)也上升，两者之间的关联。
		Cov(X,Y)=E[(X-E(X))(Y-E(Y))]=E[XY-XE(Y)-YE(X)+E(X)E(Y)]
