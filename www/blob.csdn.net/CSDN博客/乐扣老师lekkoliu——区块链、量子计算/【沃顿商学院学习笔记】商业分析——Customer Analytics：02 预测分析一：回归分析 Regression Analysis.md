
# 【沃顿商学院学习笔记】商业分析——Customer Analytics：02 预测分析一：回归分析 Regression Analysis - 乐扣老师lekkoliu——区块链、量子计算 - CSDN博客

2019年02月20日 20:18:49[乐扣老师lekkoliu](https://me.csdn.net/lsttoy)阅读数：34



## 商业进阶——预测分析一：回归分析
本章主要是从数据分析的短期预测方法：回归分析的角度进行学习。
## 回归分析 Regression Analysis
回归的目的是什么？
回归分析是量化两个或多个变量之间的关系。如：![Y=a+b×X +e](https://img-blog.csdnimg.cn/20190220201032445.png)
其中，从一组谓词变量中解释一个因变量，称为自变量。同时我们通过使用因变量和自变量之间的线性加性关系来做描述。
[
](https://img-blog.csdnimg.cn/20190220201032445.png)
## 需求分析回归案例 Case of Demand Analysis Regression
[
](https://img-blog.csdnimg.cn/20190220201032445.png)**需求分析**
[
](https://img-blog.csdnimg.cn/20190220201032445.png)Salest = a + b1 Pricet + et
简单回归关系为：
![Y=a+b×X +e](https://img-blog.csdnimg.cn/20190220201032445.png)
最优定价
•由于可以针对不同的价格进行预测，我们还可以确定最优价格
•最优价格 - 最大化整体利润的价格
直觉 - 对于每个价格，我们预测需求
需求 - >收入和利润
多重回归 Multiple Regression
即存在多个自变量。
•  Example
– Salest =A+B1 Pricet +B2*Advt +Et
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190220201453609.png)
从回归到数据挖掘 From Regression to Data mining
[
](https://img-blog.csdnimg.cn/20190220201453609.png)•回归是进行预测分析的一种方式
•存在许多不同的方法
CART
MARS
神经网络
结论
•回归是理解需求驱动因素和需求预测的理想工具。
•它可用于确定最优价格。
•适合对于短期精准的预测研究。
•对于更精准的预测我们可以通过进行更深入的数据挖掘来进行关系建模。
上课心得
现在回归，正如我所提到的，只是做出需求预测的一个例子。 这是理解需求驱动因素的好工具， 做出需求预测，谈论最优价格。回归分析是一个简单易懂而且短期有效的工具。

