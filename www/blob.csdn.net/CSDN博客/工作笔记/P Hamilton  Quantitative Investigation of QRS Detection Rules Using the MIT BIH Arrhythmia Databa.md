# P. Hamilton / Quantitative Investigation of QRS Detection Rules Using the MIT/BIH Arrhythmia Databa - 工作笔记 - CSDN博客





2012年01月17日 14:20:05[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5408标签：[算法																[数据库																[优化																[测试](https://so.csdn.net/so/search/s.do?q=测试&t=blog)
个人分类：[信号处理](https://blog.csdn.net/App_12062011/article/category/1063270)





# Quantitative Investigation of QRS Detection Rules Using the MIT/BIH Arrhythmia Database

Hamilton, Patrick S.;   Tompkins, Willis J.;   

Department of Electrical and Computer Engineering, University of Wisconsin-Madison 




论文网址： [http://www.engr.wisc.edu/bme/faculty/tompkins_willis/Quant_Invest_Hamilton.pdf](http://www.engr.wisc.edu/bme/faculty/tompkins_willis/Quant_Invest_Hamilton.pdf)



 如题所诉，文章 利用MIT/BIH 数据库定量研究QRS检测准则。



试验步骤：

1. 引用 Pan JiaPu 论文中预处理过程，略作修改。

（1）低通滤波----> 高通滤波（修改了Pan 文中打印错误）----->级联成带通滤波，带通信号；

（2）求导（有修改，等价于五点最小二乘线性拟合）；

（3）平方；

（4）移动窗口积分（窗口宽度32点）； ------------块状信号（lump），移动平均信号

（5）在移动平均信号中，用信号峰检测算法，包括噪声和QRS产生的块状信号；

（6）在带通信号中，利用基准点标记算法；

2. 判定准则的优化。

（1）预处理及峰检测算法 获得整个数据库的二维事件向量集合，每个事件向量包括步骤1（5）中获得的幅度和步骤1（6）中获得基准点距离前一个基准点的时间，另外还带一个标记，表示此事件是由QRS波产生还是由噪声产生；

（2）通过逐步添加判定准则的方式研究各判定准则的作用；

（3）优化的准则是总的误检测和漏检测数量最小；

（4）信号峰水平估计，比较均值，中值，迭代三种方法的信号幅度预测能力，均值和中值依赖于点数，迭代依赖于迭代系数；

（5）统计三种方法的均方预测误差，对QRS事件或噪声事件幅度预测中，中值法表现最好；

（6）研究幅度估计对QRS检测性能影响；检测阈值和峰水平估计成比例；---------仅依赖幅度估计这个因素考察QRS检测性能

（7）均值和中值依赖于用于计算的点数，迭代依赖于迭代系数，同时还有检测阈值比例系数可变；

（8）对每个固定预测系数（点数和迭代系数），阈值系数变化；------对应Fig 8 

（9）对相似阈值系数，预测系数变化           ----------------------------对应,Fig9

（10）试验确定均值（和中值）在最少检测误差时的点数和阈值系数， 确定迭代法在最少检测误差时的迭代系数和阈值系数。结论是中值法最优。接下来用中值方法估计峰幅度；

（11）研究检测阈值条件，利用Pan JiaPu 文中格式。中值法估计QRS事件和噪声事件幅度，可变因素有中值计算用点数 和 阈值系数。点数 有可分为 QRS事件和噪声事件两种情况下的点数，为简单，使用相同点数。

（12）试验确定了最优点数及阈值系数；

（13）研究不应期条件作用。增加不应期后试验确定 最优中值计算点数和阈值系数。

（14）回查判定准则的作用。包含两个方面内容，第一，间期预测，第二，回查阈值和正常检测阈值关系；

1）均值，中值，迭代三种方法的间期预测能力 (-------Fig 11)。确定用8点中值进行间期预测；

2)1.5 倍 平均RR预测间期未找到下一个QRS波，进行回查。回查阈值试验了两种方式：

第一种，回查与正常检测格式相同，正常检测阈值系数 和 回查阈值系数两者变化；

第二种，回查阈值和正常阈值成比例，正常检测阈值系数变化，回查比例系数变化。

这两种在各自最优情况下，误检数量相同。

（15）总结各种判定准则的影响及作用大小；

（16）算法实现，略作调整。

（17）用MIT/BIH数据库测试性能；

（18）总结。](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)




