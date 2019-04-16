# QuantLib教程（二）QuantLib的Interest Rate - lyx的专栏 - CSDN博客





2017年05月10日 21:24:11[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：3029
所属专栏：[QuantLib的python使用教程](https://blog.csdn.net/column/details/15583.html)









QuantLib既然是一个金融类的库，那么既然讨论了时间，就不得不讨论利率了，毕竟，货币是有时间价值的。

了解一点利率知识的都知道，利率不是表面看起来那么简单，可以有单利、复利，复利还可以是连续复利。所以是，我们在折现，或者说计算货币的时间价值的时候，首先要定义好我们的利率计算方式。这就会QuantLib中InterestRate这个类的作用了。

好了，我们现在定义一个利率。首先，是年化报价的利率，譬如，0.05，也就是5%。然后是计息日子的方式，比如说，按照实际度过的calendar day算。然后是，单利还是复利，以及计息周期。

如果说，到这里未知，读者觉得不知所云，那么，只能说，读者把利率想的太简单了。

我们把上面这些参数设置一下，然后来生成一个InterestRate类。



```python
annualRate = 0.05 #名义报价的年化利率
dayCount = ql.ActualActual() #按照calendar day 计算计息日
compoundType = ql.Compounded #复利计算
frequency = ql.Annual #每年付息一次
interestRate = ql.InterestRate(annualRate, dayCount, compoundType, frequency) #构造interestRate类
```
然后我们来看看这个所谓的利率类怎么用吧。



假设现在投资1元，那么t年后，按照上面的利率，你的钱值多少钱呢？换句话说，就是如何计算终值。比如，t=2年.



```python
interestRate.compoundFactor(2.0)
```
当然，给出的结果是1.1025.其实就是1.05 * 1.05.



换句话说，InterestRate类的compoundFactor方法返回特定年限后的一元钱的终值，其实就是终值系数。

有终值，当然就有现值了。计算现值系数的方法是discountFactor()这个英文还是比较好理解的。不知道为什么终值系数叫做compundFactor.



