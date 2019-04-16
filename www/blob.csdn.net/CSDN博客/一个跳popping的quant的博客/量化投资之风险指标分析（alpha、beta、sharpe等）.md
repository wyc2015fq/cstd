# 量化投资之风险指标分析（alpha、beta、sharpe等） - 一个跳popping的quant的博客 - CSDN博客





2018年12月26日 21:16:08[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：1777








很多风险指标虽然经常在各种回测平台中见到，但是它们背后的一些计算方法以及应用背景并没有认真研究过，所以打算整理下。

整理的几种风险指标摘自聚宽的回测平台：[https://www.joinquant.com/help/api/help?name=api#%E9%A3%8E%E9%99%A9%E6%8C%87%E6%A0%87](https://www.joinquant.com/help/api/help?name=api#%E9%A3%8E%E9%99%A9%E6%8C%87%E6%A0%87)



### 策略收益（Total Returns）

最容易理解的一个概念，策略收益也就是策略开始到结束，总资产的变化率。

![\large Total Returns:\\ \begin{center}Total Returns = (P_{end} - P_{start})/P_{start} * 100\%\end{center}](https://img-blog.csdnimg.cn/20181226155200851)

### **策略年化收益（Total Annualized Returns）**

年化收益率是指把当前的收益率（日收益率、周收益率、月收益率等）换算成年收益率来计算的，**是一种理论收益，并不是真正已经取得的收益率。**年化收益率需要和年收益率区分开，年收益率是指策略执行一年的收益率，是实际的收益。

![\large Total Annualized Returns:\\ \begin{center}Total Annualized Returns = ((1+P)^{250/n}-1) * 100\%\end{center}](https://img-blog.csdnimg.cn/20181226155200873)

其中上式中n是策略执行的时间，P是这n时间内的策略收益率。



### 基准收益（Benchmark Returns）

如果一个策略一年赚了50%，而这一年来上证指数上涨了100%，所以要评判一个策略的好坏，不过是要看它的收益率，还需要一个基准来衡量它的优劣性，这个准基就是准基收益率。

对于股票的策略如果高于上证指数，那么就跑赢了基准收益率，也就是跑赢了大盘；低于上证指数，那么就是跑输了基准收益率。**所以说一个好的策略至少要高于基准收益。**



### 贝塔（Beta）

代表了策略表现对大盘变化的敏感性，也即是策略与大盘的相关性。

例如一个策略的Beta为1.5，则大盘涨1%的时候，策略可能涨1.5%，反之亦然；如果一个策略的Beta为-1.5，说明大盘涨1%的时候，策略可能跌1.5%，反之亦然。

![\large Beta\\ \begin{center}Beta = Cov(D_{p},D_{m})/Var(D_{m}) \end{center}](https://img-blog.csdnimg.cn/20181226211609810)

![\large D_{p} \ D_{m}](https://img-blog.csdnimg.cn/20181226211609832) 分别是策略的每日收益和基准的每日收益



### 阿尔法（Alpha）

alpha是**超额收益**，它与市场波动无关，也就是说不是靠系统性的上涨而获得收益。

![\large Alpha\\ \begin{center}Alpha = R_{p} - [R_{f} + \beta*(R_{m} - R_{f})] \end{center}](https://img-blog.csdnimg.cn/20181226211609847)

![\large R_{p} \ R_{m} \ R_{f}](https://img-blog.csdnimg.cn/20181226211609865) 分别是策略年化收益率、基准年化收益率和无风险利率（默认0.04）。

通过预测方向或者其他可解释原因的策略也即是alpha策略；而通过波动率来带来利润的策略就是beta策略。



### 夏普比率（Sharpe）

描述的是策略在单位总风险下所能获得的超额收益。

![\large Sharpe Ratio:\\ \begin{center}Sharpe Ratio = (R_{p} - R_{f}) / \sigma _{p} \end{center}](https://img-blog.csdnimg.cn/20181226211609880)

![\large \sigma _{p}](https://img-blog.csdnimg.cn/20181226211609898)是策略收益波动率，也即是策略收益率的年化标准差。

### 所提诺比率（Sortino）

描述的是策略在单位下行风险下所能获得的超额收益。

![\large Sortino Ratio:\\ \begin{center}Sortino Ratio = (R_{p} - R_{f}) / \sigma _{pd} \end{center}](https://private.codecogs.com/gif.latex?%5Cfn_cm%20%5Clarge%20Sortino%20Ratio%3A%5C%5C%20%5Cbegin%7Bcenter%7DSortino%20Ratio%20%3D%20%28R_%7Bp%7D%20-%20R_%7Bf%7D%29%20/%20%5Csigma%20_%7Bpd%7D%20%5Cend%7Bcenter%7D)

![\large \sigma _{p}](https://img-blog.csdnimg.cn/20181226211609898)是策略下行波动率。



### 信息比率（Information Ratio）

描述的是策略在单位超额风险下的超额收益。

![\large Information Ratio:\\ \begin{center}Information Ratio= (R_{p} - R_{f}) / \sigma _{t} \end{center}](https://img-blog.csdnimg.cn/20181226211609914)

![\large \sigma _{t}](https://img-blog.csdnimg.cn/20181226211609932)是策略与基准每日收益差值的年化标准差。



### 最大回撤（Max Drawdown）

描述的策略最大的亏损情况。**最大回撤通常越小越好。**

![\large Max Drawdown:\\ \begin{center}Max Drawdown= (P_{x} - P_{y}) / P_{x}\end{center}](https://img-blog.csdnimg.cn/20181226211609948)

![\large P_{x} \ P_{y}](https://img-blog.csdnimg.cn/20181226211609965)是策略两日的累计收益。



### 策略波动率（Algorithm Volatility）

用来测量策略的风险性，波动越大代表策略风险越高。

![\large Algorithm Volatility:\\ \begin{center}Algorithm Volatility= \sqrt{[250/(n-1)]\sum_{n}^{i=1}(r_{i}-\bar{r})^{2}} \end{center}](https://img-blog.csdnimg.cn/20181226211609983)

![\large r_{i} \ \bar{r} \ n](https://private.codecogs.com/gif.latex?%5Cfn_cm%20%5Clarge%20r_%7Bi%7D%20%5C%20%5Cbar%7Br%7D%20%5C%20n) 分别是每日的策略收益率、平均策略收益率以及策略的执行天数。



### 基准波动率（Bechmark Volatility）

用来测量基准的风险性，波动越大代表基准风险越高。

![\large Bechmark Volatility:\\ \begin{center}Bechmark Volatility= \sqrt{[250/(n-1)]\sum_{n}^{m=1}(r_{m}-\bar{r})^{2}} \end{center}](https://img-blog.csdnimg.cn/201812262116105)

![\large r_{m} \ \bar{r} \ n](https://img-blog.csdnimg.cn/2018122621161022) 分别是每日的基准收益率、平均基准收益率以及策略的执行天数。



### 胜率

盈利次数在总交易次数中的占比。

                                     胜率 = 盈利交易次数 / 总交易次数

### 盈亏比

周期盈利亏损的比例。

                                    盈亏比 = 总盈利额 / 总亏损额



