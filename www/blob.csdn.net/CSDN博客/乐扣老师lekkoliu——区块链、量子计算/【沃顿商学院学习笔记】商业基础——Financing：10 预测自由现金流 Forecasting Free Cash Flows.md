
# 【沃顿商学院学习笔记】商业基础——Financing：10 预测自由现金流 Forecasting Free Cash Flows - 乐扣老师lekkoliu——区块链、量子计算 - CSDN博客

2019年02月16日 11:01:21[乐扣老师lekkoliu](https://me.csdn.net/lsttoy)阅读数：92所属专栏：[沃顿商学院/Wharton Business School](https://blog.csdn.net/column/details/33347.html)



## 商业基础——预测自由现金流量
本章主要是从预测自由现金流量的相关概念和计算方法进行学习。
## 预测自由现金流量 Forecasting Free Cash Flows
我们从自由现金流的计算公式中来解构驱动因素：
FCF = (Revenue – Costs – Depreciation) x (1 – Tc) + Depreciation – Capital Expenditures – Change in Net Working Capital
我们从其中四个要素（Revenue、Costs、Capital Expenditures、Change in Net Working Capital）来进行现金流的具体流量分析。
## 收入 Revenue
对于收入，我们有如下公式：
收入=市场规模x市场份额x价格
Revenue = Market Size x Market Share x Price
## 支出 Costs
Costs我们解构运营成本为三个维度：COGS、SG&A、和R&D。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190216104555116.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)
对于COGS：
COGS = (COGS / Sales) x Sales 则理解为销售成本。
对于管理费：
根据上表我们可以知道第一年和后续递增的年限管理费：
Year 1: SG&A = 1% of ‘08 SG&A
Year 2 – 5: SG&A = 25% Annual Growth Rate
对于研发费用R&D:
线性递增即可。
综合收入和支出，我们可以计算出EBITDA=Sales-COGS-SG&A-R&D
## 资本支出 Capital Expenditures
主要集中在初始投资和对未来的投资费用上。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213115951533.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)
从上面三个维度我们可以得出资本支出是一个常态，因此我们要考虑累计资本支出和累计折旧这两个层面。在这几个要素的综合下我们能够得到以下三个维度的数据：
资本支出的预期价值：Book Value
流动性价值：Liquidation Value（LV）
以及税后预期资产折价：After tax proceeds from assets sales
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190216105515953.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)
根据计算关系我们可以知道以下三个关系式：
[
](https://img-blog.csdnimg.cn/20190216105515953.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)**Book Value = Accum CapEx – Accum Depreciatio**
[
](https://img-blog.csdnimg.cn/20190216105515953.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)**LV = Book Value x Recovery Rate**
[
](https://img-blog.csdnimg.cn/20190216105515953.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)**After-tax proceeds = LV – (LV – Book Value) x Tax Rate**
[
](https://img-blog.csdnimg.cn/20190216105515953.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)
## 净营运资金变动值 Change in Net Working Capital
[
](https://img-blog.csdnimg.cn/20190216105515953.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)关系公式如下：
净营运资本=现金 + 库存 + 应收资本  -  应付资本
Net Working Capital = Cash + Inventory + AR – AP
其中四个因素的结构成分如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213120301833.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)
其中：Δ净营运资本=净营运资本（t） - 净营运资本（t-1）
Change in NWC = ΔNWC = NWC(t) – NWC(t-1)
其中Δ=一个周期的变化
## 知识点总结
1.预测自由现金流是将我们的预测驱动因素转换为美元预测的问题
2.贴现现金流量（DCF）是财报的两个基本输入之一
## 上课心得
关于对未来预测自由现金流的学习可以深入了解构成预测自由现金流的四个要素和计算方式。对未来财务状况的预期有很直观的作用。

[
  ](https://img-blog.csdnimg.cn/20190213120301833.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)