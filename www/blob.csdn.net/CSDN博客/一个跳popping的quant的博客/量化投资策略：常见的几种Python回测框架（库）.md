# 量化投资策略：常见的几种Python回测框架（库） - 一个跳popping的quant的博客 - CSDN博客





2018年03月08日 11:04:48[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：3565
所属专栏：[一个用人工智能做量化的小白的进阶之路](https://blog.csdn.net/column/details/20644.html)









转自：http://blog.csdn.net/lawme/article/details/51454237

## 量化投资策略：常见的几种Python回测框架（库）

在实盘交易之前，必须对量化交易策略进行回测。在此，我们评价一下常用的Python回测框架（库）。评价的尺度包括用途范围（回测、虚盘交易、实盘交易），易用程度（结构良好、文档完整）和扩展性（速度快、用法简单、与其他框架库的兼容）。
- [*Zipline*](https://github.com/quantopian/zipline): 事件驱动的回测框架。[Quantopian](https://www.quantopian.com/) 正在使用它。- Zipline 拥有大型社区，文档完整，对著名经纪公司Interactive Broker (IB)有大力支持；整合了Pandas，语法清晰，易于学习掌握。
- 有大量例程examples。你若主要是为了交易美国证券，它是最好的选择。Quantopian 允许回测、分享并在其社区讨论交易策略。
- 不过，据我们的经验，[*Zipline*](https://github.com/quantopian/zipline) 速度极慢。这是它最大的短板。Quantopian 有些对策，如在云端作并行运行。若有兴趣，你可看看[这篇博文](http://blog.quantopian.com/zipline_in_the_cloud/) 。
- Zipline 似乎很难使用本地数据文件和非美数据。
- 它很难用于不同种类的金融资产。

- [*PyAlgoTrade*](http://gbeced.github.io/pyalgotrade/): 也是事件驱动的回测框架，支持虚盘和实盘两种交易。文档完整，整合了TA-Lib(技术分析库)。在速度和灵活方面，它比[*Zipline*](https://github.com/quantopian/zipline) 强。不过，它的一大硬伤是不支持 Pandas 的模块和对象。
- [*pybacktest*](https://github.com/ematvey/pybacktest): 它以处理向量数据的方式进行回测，非常简单轻便。2015年5月21日，这个项目有复活的迹象。
- [*TradingWithPython*](http://www.tradingwithpython.com/): 这位Jev Kuznetsov 扩展 *pybacktest* 形成自己的回测程序。这个库似乎在2015年2月更新了。不过，相关的文档和课程售价 $395。
- 其他项目: [ultra-finance](https://github.com/panpanpandas/ultrafinance)
||Zipline|PyAlgoTrade|TradingWithPython|pybacktest|
|----|----|----|----|----|
|类型|事件驱动|事件驱动|向量处理|向量处理|
|社区|较大|一般|无|无|
|云计算|Quantopian|无|无|无|
|支持 IB|是|否|否|否|
|数据源|Yahoo, Google, NinjaTrader|Yahoo, Google, NinjaTrader, Xignite, Bitstamp 实时提供数据|||
|文档|完整|完整|$395|很少|
|事件可定制|是|是|||
|速度|慢|快|||
|支持Pandas|是|否|是|是|
|交易日历|是|否|否|否|
|支持TA-Lib|是|是|是||
|适用于|仅用于美国证券交易|实盘交易虚盘交易|虚盘测试交易|虚盘测试交易|



Zipline 与 PyAlgoTrade 的对比评分
||Zipline|PyAlgoTrade|说明|
|----|----|----|----|
|虚盘交易|♦|♦ ♦ ♦|Zipline 似乎不能用非美数据和本地数据工作，而 PyAlgoTrade 可以使用任何类型的数据|
|实盘交易|♦ ♦|♦ ♦|二者都不错，但 Quantpian 的云计算编程很好|
|灵活性|♦ ♦|♦ ♦ ♦|PyAlgoTrade 支持各种高级定单，并有更多的业务事件。 Zipline 提供了简单的滑点模式|
|速度|♦|♦ ♦ ♦|Zipline 比 PyAlgoTrade 慢|
|易用性|♦ ♦ ♦|♦ ♦|PyAlgoTrade 不支持 pandas|



