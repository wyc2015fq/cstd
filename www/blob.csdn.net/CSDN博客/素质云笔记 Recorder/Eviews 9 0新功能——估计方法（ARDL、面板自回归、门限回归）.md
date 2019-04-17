# Eviews 9.0新功能——估计方法（ARDL、面板自回归、门限回归） - 素质云笔记/Recorder... - CSDN博客





2016年05月12日 20:03:16[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：11898标签：[eviews 9.0																[eviews9.0																[门限回归																[新功能																[估计](https://so.csdn.net/so/search/s.do?q=估计&t=blog)
个人分类：[Eviews 8.0＆9.0](https://blog.csdn.net/sinat_26917383/article/category/6120590)









**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————


9.2 估计功能




**eviews9.0下载链接：[[软件]EViews 9 的时代已经来临！（附安装包、升级包、破解补丁、教程）](http://bbs.pinggu.org/thread-3653180-1-1.html)**





# 一、自回归分布滞后模型




EViews 9提供了ARDL，自回归分布滞后模型的工具，变量包括了滞后变量和常规解释变量。

其中，EViews 9有三大新功能：

内置了滞后阶数的选择；协整估计；长期趋势的Bounds检验。






# 二、ARMA的ML和GLS估计




现在可以在ARMA基础上利用ML、GLS估计（极大似然与广义最小二乘法）。其中可以用卡尔曼滤波法估计似然值（Hamilton 1994）。

操作的时候，要先估计一个普通的OLS估计，之后才会显示ML和GLS页面。





# 三、ARFIMA

ARFIMA模型通过ML和GLS实现的极大似然估计，是一种非常高效地算法（Sowell (1992) and Doornik and Ooms (2003)）。

其中一个特征是集成参数d自动初始化，其是通过对数周期回归模型（Geweke and Porter-Hundlak (1983) ）与关于系数与结构的似然解的概率密度。





# 四、面板自回归分布滞后模型ARDL与PMG（pooled mean group）估计




EViews 9 现在支持Pesaran, Shin and Smith (PSS, 1999) 在ARDL个体效应模型中的PMG估计。这一模型在面板模型中较为流行，一般在模型中时间较长时候使用，因为此时GMM估计并不特别适用。

PMG是单一ARDL模型的协整形式，并且面板模型中允许存在截距、短期系数和不同横截面的协整项。



# 五、门限回归




EViews 9提供了门限回归，其中包括了较为流行的门限自回归模型（TAR）。TR门限回归描述了一个简单的线性分段和结构转变的非线性回归。TR非常流行的原因是因为易于估计与解释，并能够产生有趣的非线性动态过程。

其中，门限自回归模型（TAR）与自激励门限回归（SETAR）是较为流行的两种（Hansen 1999, 2011; Potter 2003）。





# 六、新的优化引擎




在Eviews 9中我们增加了需要新的估计方法。它支持 Broyden-Fletcher-Goldfarb-Shanno (BFGS), Gauss-Newton/BHHH, Newton-Raphson, and Fisher Scoring algorithms这些估计方式。

此外，还允许二阶导数的运算，提供了一些替代普通与稳健回归的协方差估计方式。



以下是新支持的工具：

Single equation nonlinear least squares and ARMA

Binary

Count

Ordered

Censored

ARCH (equation and system)

Switching Regression

GLM

Heckman Selection

FIML

State Space

Logl




[〖素质笔记〗Eviews 9 三大新功能——估计功能（二）](http://bbs.pinggu.org/forum.php?mod=viewthread&tid=3883933&fromuid=3495972)



**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————](https://so.csdn.net/so/search/s.do?q=新功能&t=blog)](https://so.csdn.net/so/search/s.do?q=门限回归&t=blog)](https://so.csdn.net/so/search/s.do?q=eviews9.0&t=blog)](https://so.csdn.net/so/search/s.do?q=eviews 9.0&t=blog)




