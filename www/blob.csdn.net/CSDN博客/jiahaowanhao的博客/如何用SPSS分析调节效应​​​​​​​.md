
# 如何用SPSS分析调节效应​​​​​​​ - jiahaowanhao的博客 - CSDN博客


2018年08月13日 21:31:10[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：615


**[如何用SPSS分析调节效应](http://cda.pinggu.org/view/26359.html)**
用回归，回归也有两种方法来检验调节效应，看下面的两个方程，y是因变量，x是自变量，m是调节变量，mx是调节变量和自变量的交互项，系数是a b c c'。检验两个方程的R方该变量，如果该变量显著，说明调节作用显著，也可以直接检验c'的显著性，如果显著也可以说明调节作用。
![](http://cda.pinggu.org/uploadfile/image/20180813/20180813064100_44059.png)
2.在spss中，打开线性回归的菜单，如图所示：
![](http://cda.pinggu.org/uploadfile/image/20180813/20180813064053_60108.png)
3.先将因变量【职业探索】、自变量【自我概念】、调节变量【社会支持】放入各自的框框。
![](http://cda.pinggu.org/uploadfile/image/20180813/20180813064045_53187.png)
4.点击下一层，设置第二个方程。
![](http://cda.pinggu.org/uploadfile/image/20180813/20180813064038_68479.png)
5.这第二层比第一层增加了一个交互项。
![](http://cda.pinggu.org/uploadfile/image/20180813/20180813064031_38421.png)
6.点击statistic，设置输出什么参数。
![](http://cda.pinggu.org/uploadfile/image/20180813/20180813064021_76463.png)
7.一定要选择R方改变量，点击continue，然后点击就好。
![](http://cda.pinggu.org/uploadfile/image/20180813/20180813064014_60203.png)
8.可以看r方的该变量，第二个方程，sig F change值小于0.05，证明调节效应存在。
![](http://cda.pinggu.org/uploadfile/image/20180813/20180813064007_46907.png)
9.看输出的结果，第一个红框是系数，也就是前面介绍的abcc'，sig值是他们的显著性水平，交互项系数的sig值小于0.05，说明存在调节效应。
![](http://cda.pinggu.org/uploadfile/image/20180813/20180813064000_91153.png)

