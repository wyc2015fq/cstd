# 用Excel进行基金业绩评价 - lyx的专栏 - CSDN博客





2016年09月17日 23:40:00[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：4046








        基金业绩评价这种事，无非也就是那么几个指标，Sharpe ratio，Treynor Ratio，InformationRatio，Jensen‘s alpha，当然啦，还有一些特立独行的，不过一般教科书上就是这几个。

        做基金业绩评价这件事之前呢，我们先要完成一个CAPM模型中，用于度量系统性风险的beta的求取，某种意义上来说，beta是一个基金风格的指标。

## 1.关于数据

假设我们有如下数据：


![](https://img-blog.csdn.net/20160916225815002?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        第二列asset表示的是当前账户的总资产，SH列是该段时期沪深300指数，asset-normal和SH index normal是归一化后的数值，也就是前面两列分别除以10000.后面两列，分别是该账户和沪深三百指数的每日收益率。由于在后面的capm模型中需要引入无风险收益率这样一个概念，如果时间限度为day的话，后面的无风险收益率将会很小，以至于产生很大的误差，所以这里我们进行月化，严谨的方式是使用复利，这里我们贪图方便就直接乘以20，作为月收益率。接下来两列分别是年化的无风险收益率和月化的无风险收益率。这里，收益率我们取SHIBOR时长为一月的利率，这都可以在LIBOR的官网查到。最后一列是市场风险溢价，就是月化的沪深300收益与无风险收益率的差。

## 2.净值曲线

        接下来做第一步，比较基金净值表现和市场表现，简而言之，就是做两条净值曲线。


![](https://img-blog.csdn.net/20160916225820505?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


## 3.CAPM模型

        然后引入CAPM模型，来计算beta，也就是系统性风险。CAPM的公式就是![](https://img-blog.csdn.net/20160916230827235?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，我们的目标就是回归出beta。后面括号里的就是市场风险溢价，也就是market
 premium这一列，等式左边的就是portfolio return monthly。


![](https://img-blog.csdn.net/20160916225825440?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        首先，我们在excel上绘制出散点图，如上图所示，然后修改横坐标和纵坐标的数据，分别是markrt premiun和portfolio return monthly。接下来，点击一个点，右击后选择“添加趋势线”这一选项。

![](https://img-blog.csdn.net/20160916231421683?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





        在弹出的设置窗口将截距设置为Rf，也就是月化的无风险利率，同时在“显示公式”和“显示R平方值”两个选项前面打上勾，这样我们就能在结果中看到具体的回归公式和决定系数R平方。这里解释一下R平方。我们可以把R平方理解为这一回归过程的可靠程度，取值为0到1。

![](https://img-blog.csdn.net/20160917232831675?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        这就是我们的结果，可以看到，R平方的值还是比较好的。

## 4.Sharpe Ratio计算

        接下来我们算一下sharpe ratio。利用H列这一月化后的基金收益率，计算出标准差（standard deviation）和均值，然后根据sharpe ratio的式子，![](https://img-blog.csdn.net/20160917233438672?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，好了，这就可以很好的计算了。均值减去无风险利率，然后除以标准差。这里我们得到的sharpe
 ratio是0.642。


![](https://img-blog.csdn.net/20160917233322859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




