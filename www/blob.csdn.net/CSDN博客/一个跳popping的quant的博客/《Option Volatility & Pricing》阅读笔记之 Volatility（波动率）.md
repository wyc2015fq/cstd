# 《Option Volatility & Pricing》阅读笔记之 Volatility（波动率） - 一个跳popping的quant的博客 - CSDN博客





2019年03月22日 20:24:48[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：153








#### Random Walks and Normal Distribution

一个期权交易者不仅仅需要对市场变动的方向敏感，同时还需要对市场变化的速度敏感。波动率（Volatility）就是作为一种**衡量市场变化速度**的指标。在相同时间的范围内，有的标的物的价格可以波动很大范围，有的几乎没有波动，那么它们的速度就自然不同了。

试想，将市场中标的物的价格作为一种随机漫步（Random Walk），以一个球在迷宫中的轨迹来模拟这个走势，那么最终球落地的位置就是一个正态分布（Normal Distribution）。改变将迷宫中挡板的方向，那么球的拓展方向都变大了，最终形成的正态分布就具有一个较低峰值，和更宽的水平延展。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190322201631149.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20190322201712584.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)

因为所有的正态分布都是对称的，所以分布的选择似乎无关紧要。波动性增加可能会增加大幅上升的可能性，但这应该被大幅下降的可能性所抵消。
从下面的图中一个标的物到期时价格的分布可以看出，如果我们要评估一张以较高行权价的 call option 的价值，随着波动率的增大，最终标的的价格位于一个更高价位的可能性不断增加，因此对应的 call option 的价值也对应增加。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190322201756383.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)

Louis Bachelier 第一个作出标的物价格服从正态分布的假设。这种假设存在逻辑问题。因此，多年来，这个假设已经被修改，使其更符合实际情况。它的修正形式是许多理论定价模型的基础，包括 Black-Scholes 模型。
在上一篇文中，BS模型需要期权的行权价、距离到期所剩的时间、目前标的物的价格、在期权持有期的利率和标的物的波动率，其中前四项的目的就是为了得到一个远期价格（Forward Price），然后以这个 forward price 作为均值，以最后的 volatility 作为标准差得到一个正态分布。

除此之外，BS模型是一个 **continuous-time** 模型，这就意味着它的 volatility 是连续复合而成的，也就是它和标的物的价格变化一样，不是上升就是下降。所以这就导致了当价格变化百分比呈正态分布时，这些价格变化的连续复合将导致到期时价格的呈现对数正态分布（**Lognormal Distribution**）。

这个可以通过一个例子来理解，如果我们投资了 $1000 按照12%的年利率获得利息，这样最后的总收益将取决于这12%是如何分配的。如果这12%的利息是一年后结算的，或者每天以12%/365复合得到利息，那么最后的总收益自然是不一样的。同样，如果-12%的亏损利率的结算频率也不一样，那么最后总收益结果也将不一样。所以最后的价格分布就会出现 **skew**。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190322201921886.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20190322201931918.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)

波动率在书中分成了两类，realized volatility （现实波动率）和 implied volatility（隐含波动率）。**现实波动率是和标的物的价格相联系的，隐含波动率是和期权相联系的。**
#### realized volatility

实现波动率是标的合同在一段时间内价格变动百分比的**年化标准差**。当我们计算现实波动率时，我们必须指定价格变化的区间和计算中使用的区间数。例如，我们可以讨论合约的50天波动性，或者我们可以讨论52周的合约波动。在前一种情况下，我们计算的是50天内每日价格变化的波动性；在后一种情况下，我们计算52周内每周价格变化的波动性。

在现实波动率图上，每个点都表示在指定时间段内使用指定区间内的价格变化的波动。举个例子，如果我们将一份合约的50天波动率作图，图上的每一点代表了过去50天每日价格变动的年化标准差；如果我们将一份合约的52周波动率作图，图上的每一点代表了过去52周每周价格变动的年化标准差。

现实波动率又分为两种，一种是 **future realized volatility （未来现实波动率）**和 **historical realized volatility（历史现实波动率）**。从字面意义上，就可以看出前者是未来数据，谁都不可能知道的，后者则是历史数据，已经知道的。

理论上来说，未来现实波动率最好地描述了未来合约价格变化的分布，将它输入理论定价模型可以得到最准确的理论价值，**如果交易者知道未来现实波动性，他就知道正确的概率**。这样就类似赌场一样，从长远看交易将获得正收益。

由于未来现实波动率无法获取，所以就出现了很多的模型以历史现实波动率来预测未来波动率，这些模型在后面的章节会提到。

#### implied volatility

隐含波动率与现实波动率不同，隐含波动率是**由市场上期权的价格计算出来的**，而现实波动率是由合约的价格变化计算出来的。

考虑对一只不支付股息的股票进行3个月105的看涨期权。如果我们有兴趣购买这个看涨期权，我们可以使用定价模型来确定期权的理论值。为了简单起见，我们假设这个选项是欧洲的(不能提前行权)，并且我们将使用Black-Scholes模型。通过输入BS模型所需要的期权的行权价、距离到期所剩的时间、目前标的物的价格、在期权持有期的利率和标的物的波动率可以得到这个期权的理论价值，其中波动率是通过历史现实数据估计得到的。假设当前股价是98.50，三个月的利率是6.00%，我们估计的未来现实波动率是25%，这样可以得到一个理论值为2.94，但是实际上，我们发现105看涨期权在3.60的价位上交易非常活跃。

造成这个结果的主要原因就是我们估计的未来现实波动率不准确，所以我们可以通过市场上得到的理论值（市场价）与其他的输入值推算出波动率，这个**波动率隐含在期权的定价上**，所以称作隐含波动率。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190322202126887.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)

在计算隐含波动率的时候，有几个问题需要注意，一是有些理论定价模型不能进行反推，进而也就得不到隐含波动率；二是输入的值不是同时刻的，当时的价格是98$可以得到一个波动率，但是如果一个小时后的价格变为了100$，此时得到的波动率就是另一个值了，所以我们看到的期权市场行情中**隐含波动率都是在不断变化的**。而且由于供求关系的影响，也会导致期权价格的变化。由于隐含波动率是从期权的市场价或者说权利金（premium）中得到的，所以交易员有时会交替使用权利金和隐含波动率，如果当前隐含波动率以历史标准衡量偏高，或相对于标的合约最近的历史波动率较高，交易者可能会说权利金水平较高；如果隐含波动率相对低，他可能会说权利金水平很低。所以就可以选择去做多或者做空波动率。
在实际中，期权的隐含波动率要比总点数来考虑期权价格通常更有用。从一个期权交易员的角度来看，**隐含波动率通常是一个更有用的期权价格表达方式**，而不是以货币单位表示的实际价格。







