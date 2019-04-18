# 隐马尔科夫模型(Hidden Markov Model,HMM) - wydbyxr的博客 - CSDN博客
2018年10月20日 11:55:35[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：31
个人分类：[机器学习具体算法](https://blog.csdn.net/wydbyxr/article/category/7945743)
所属专栏：[经典机器学习算法](https://blog.csdn.net/column/details/28812.html)
# 隐马尔科夫模型(Hidden Markov Model,HMM)
  在神经网络之前，语音识别用的都是Hidden Markov Model（HMM）。
## 原理说明
  观测到X1到Xt,y1到yt是隐藏的内部状态。模型有两个假设：
  1）第一个假设 Markov property：internal state 只由前一个状态决定，所以只走一步。
  2）第二个假设是当前的观测值只由当前的隐藏状态决定。它其实把时间序列的预测分为很多小部分，也是约束性很强的一个模型。模型虽然很简单，但是十分有效，因为自然界很多事物都遵循马尔科夫假设。
  stationary（不动的，固定的）是指系统经过一系列的变化又回到了原点。如果有趋势项，很可能就回不到原点，做了difference 之后就去掉趋势因素。
