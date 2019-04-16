# 风险控制之VaR - 一个跳popping的quant的博客 - CSDN博客





2018年09月29日 15:58:10[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：1040








### 什么是VaR

VaR是value  of risk的缩写称为风险价值，或者受险价值，**指的是在一定的概率下，一个金融资产在未来一段时间内的最大可能损失。**常用于金融机构的风险管理。它的数学定义为：

![P(Loss_{\Delta t}\leq VaR)=a](https://private.codecogs.com/gif.latex?P%28Loss_%7B%5CDelta%20t%7D%5Cleq%20VaR%29%3Da)

其中，![Loss_{\Delta t}](https://private.codecogs.com/gif.latex?Loss_%7B%5CDelta%20t%7D)的含义是金融资产在持有期![\Delta t](https://private.codecogs.com/gif.latex?%5CDelta%20t)的时间内的价值损失。总的意义就是在金融资产的收益有1-p的概率不会小于VaR。

所以从定义出发，要确定VaR的值或者建立VaR的模型，必须要确定三个系数：
- 持有期![\Delta t](https://private.codecogs.com/gif.latex?%5CDelta%20t)。也就是持有资产是在哪一段时间内的最大亏损值。一些流动性比较强的交易头寸需要以日为周期计算；而一些周期较长的如养老基金之类的则是以月为周期来计算。
- 置信水平a。置信区间的选择在一定程度上反映了金融机构对风险的不同偏好。较大的置信水平意味着对风险的厌恶程度就大。J.P. Morgan与[美洲银行](https://baike.baidu.com/item/%E7%BE%8E%E6%B4%B2%E9%93%B6%E8%A1%8C)选择95%，[花旗银行](https://baike.baidu.com/item/%E8%8A%B1%E6%97%97%E9%93%B6%E8%A1%8C)选择95.4%，大通曼哈顿选择97.5%，Bankers Trust选择99%。作为金融监管部门的[巴塞尔委员会](https://baike.baidu.com/item/%E5%B7%B4%E5%A1%9E%E5%B0%94%E5%A7%94%E5%91%98%E4%BC%9A)则要求采用99%的[置信区间](https://baike.baidu.com/item/%E7%BD%AE%E4%BF%A1%E5%8C%BA%E9%97%B4)，这与其稳健的风格是一致的。
- 观察周期（Obervation Period）。也就是整个数据选取的时间范围。



### VaR的作用
- 用于[风险控制](https://baike.baidu.com/item/%E9%A3%8E%E9%99%A9%E6%8E%A7%E5%88%B6)。目前已有超过1000家的银行、保险公司、投资基金、[养老金](https://baike.baidu.com/item/%E5%85%BB%E8%80%81%E9%87%91)基金及非金融公司采用VaR方法作为[金融衍生工具](https://baike.baidu.com/item/%E9%87%91%E8%9E%8D%E8%A1%8D%E7%94%9F%E5%B7%A5%E5%85%B7)风险管理的手段。利用VaR方法进行风险控制，可以使每个[交易员](https://baike.baidu.com/item/%E4%BA%A4%E6%98%93%E5%91%98)或交易单位都能确切地明了他们在进行有多大风险的[金融交易](https://baike.baidu.com/item/%E9%87%91%E8%9E%8D%E4%BA%A4%E6%98%93)，并可以为每个交易员或交易单位设置VaR限额，以防止过度投机行为的出现。如果执行严格的VaR管理，一些金融交易的重大亏损也许就可以完全避免。
- 用于业绩评估。在[金融投资](https://baike.baidu.com/item/%E9%87%91%E8%9E%8D%E6%8A%95%E8%B5%84)中，高收益总是伴随着高风险，交易员可能不惜冒巨大的风险去追逐巨额利润。公司出于稳健经营的需要，必须对[交易员](https://baike.baidu.com/item/%E4%BA%A4%E6%98%93%E5%91%98)可能的过度投机行为进行限制。所以，有必要引入考虑风险因素的[业绩评价](https://baike.baidu.com/item/%E4%B8%9A%E7%BB%A9%E8%AF%84%E4%BB%B7)指标。

### 常用来估计VaR的方法

下面介绍三种估计VaR的方法：来自[python爱好者社区](https://mp.weixin.qq.com/s?__biz=MzI5NDY1MjQzNA==&mid=2247487433&idx=1&sn=af44c77f5037acb50b95cedc0ff9704a&chksm=ec5ed0b4db2959a266b20678d4bca35e3e7a1c70ef7c90e26192247f38fac2a58af59cc20649&mpshare=1&scene=23&srcid=0922mMFMby1zjSeAOT3W18lZ#rd)

1、HS方法

HS方法称为历史模拟法（Historical Simulation），它的思想是通过每次取一定长度的历史数据作为样本，将样本的分布看作是整体的分布，然后在置信度p下，只需要找出这些历史数据的p-分位数，认为这些历史数据的p-分位数就可以表示VaR。

> 
**p-分位数**：分位数（Quantile）也称为分位点。**指将一个随机变量的概率分布范围分为几个等份的数值点**，常用的分位数有中位数（二分位数）、四分位数、百分位数。分位数的计算则是先将n个数据进行升序排列，然后p分位数就是取其中的第n*p位置上的数。


2、WHS方法

WHS方法称为加权历史模拟法（Weighted Historical Simulation）,它与HS方法的思想类似，只不过HS方法认为过去每一天的数据包含的信息是一样的，没有考虑时间的因素。**而WHS则是认为距离当天越近的数据对与当天的影响更大，所以应该赋予更高的权重，因此对p-分位数进行加权来表示VaR。**



### Ref

[百度百科](https://baike.baidu.com/item/VAR%E6%96%B9%E6%B3%95/4366363?fr=aladdin)

[python爱好者社区-VaR专题](https://mp.weixin.qq.com/s?__biz=MzI5NDY1MjQzNA==&mid=2247487433&idx=1&sn=af44c77f5037acb50b95cedc0ff9704a&chksm=ec5ed0b4db2959a266b20678d4bca35e3e7a1c70ef7c90e26192247f38fac2a58af59cc20649&mpshare=1&scene=23&srcid=0922mMFMby1zjSeAOT3W18lZ#rd)



