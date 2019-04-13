
# 【沃顿商学院学习笔记】商业基础——Financing：06 期限结构 Term Structure - 乐扣老师lekkoliu——区块链、量子计算 - CSDN博客

2019年02月13日 10:53:08[乐扣老师lekkoliu](https://me.csdn.net/lsttoy)阅读数：71所属专栏：[沃顿商学院/Wharton Business School](https://blog.csdn.net/column/details/33347.html)



## 商业基础——期限结构
本章主要是从期限结构的相关概念和计算方法进行学习。
## 期限结构 Term Structure
到目前为止，我们假设折扣率随时间不变。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213104026906.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)
可以说，期限结构(Term Structure)是投资期限与利率之间的关系。
[
](https://img-blog.csdnimg.cn/20190213104026906.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)
## 收益率曲线 Yield Curve
[
](https://img-blog.csdnimg.cn/20190213104026906.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)收益率曲线(Yield Curve)是投资期限与利率之间关系的图表。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213104427403.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)
**什么是收益(Yield)？**
收益率y是一种贴现率，当应用于证券的承诺现金流时，可以收回证券的价格。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213104320703.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)
建立收益率曲线只需计算不同期限证券的收益率，其与计算不同到期日证券的贴现率相同。
[
](https://img-blog.csdnimg.cn/20190213104320703.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)**国债收益率 Treasury Yield Curves**
国债收益率曲线描绘了无风险贷款利率与贷款期限之间的关系。
其他收益率曲线描绘了风险贷款利率与贷款期限之间的关系。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213104615281.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)
**可以说，收益率取决于成熟度和风险。**
以上所有这些利率都被称为**即期利率**，即期汇率是今天贷款的利率。
对于不同期限和风险的贷款，通常采用不同的即期汇率。
对于不同的即期汇率我们可以采取一个近似计算：
用R名义贴现率来替代不同的即期汇率。
即通过
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019021310492879.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)
来近似计算
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213104950393.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)
## 知识点总结
1.期限结构是指利率与投资期限之间的关系，不同期限（期限）的贷款（储蓄）通常具有不同的利率
2.收益率曲线描绘了利率与投资期限之间的关系，利率因投资风险而异。
3.即期汇率是今天贷款的利率，即期汇率来自收益率曲线，不同期限贷款（和风险）通常有不同的即期汇率
## 上课心得
通过理解即期汇率和名义贴现率对整个净现值进行估算，理解由于不同利率和投资期限的关系而长生的期限结构。不同的风险和成熟度也造成了利率的大小差异。

[
](https://img-blog.csdnimg.cn/2019021310492879.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)
