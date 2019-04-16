# pyfolio的information_ratio无法使用的问题 - lyx的专栏 - CSDN博客





2017年07月05日 21:44:06[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1377








        pyfolio随着版本的更新，逐渐不在自己计算一些评价指标，譬如sharpe_ratio,information_ratio等等，目的是为了和zipline保持一致。那么要算这些怎么办呢？使用emprical这个包里面的计算方法。

        但是，在0.3.0的emprical版本中，没有information_ratio这个指标，所以，在使用pyfolio的时候，会有不能在module中找到information_ratio的错误提示。

       解决的办法就是吧emprical降到0.2.2，然后就可以使用pyfolio了。



