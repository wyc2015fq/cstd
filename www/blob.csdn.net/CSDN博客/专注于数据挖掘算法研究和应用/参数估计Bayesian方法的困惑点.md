# 参数估计Bayesian方法的困惑点 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年08月14日 09:49:56[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：154








1、参数估计三种方法



![](https://img-blog.csdn.net/20180814093642162?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

MLE和MAP是点估计，而第三种Bayesian方法则是求期望。

2、期望求解

![](https://img-blog.csdn.net/20180814093956264?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中：![](https://img-blog.csdn.net/20180814094023876?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这里面的困惑点是分母这一全概率可否作为常数不参与计算。

![](https://img-blog.csdn.net/20180814094218900?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如此，期望应等同于求解：

![](https://img-blog.csdn.net/20180814094812894?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如此，用gibbs或em变分求解，就是后验分布（=先验分布+条件分布）。

这一困惑点，目前暂未得到确认，有待后续求索。



