# 图像形状特征（六）--AR模型形状描述子 - 工作笔记 - CSDN博客





2016年12月09日 10:38:48[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6942
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









1. 自回归模型的定义

          自回归模型（Autoregressive Model）是用自身做回归变量的过程，即利用前期若干时刻的随机变量的线性组合来描述以后某时刻随机变量的线性回归模型[1]，它是时间序列中的一种常见形式[2]。

![](https://img-blog.csdn.net/20161209103719616?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


2.  AR模型的状态空间形式（AR-Process in State Space Form）

AR模型可以写成状态空间模型的形式[4] [5] [6]，令： 
![](https://img-blog.csdn.net/20161209103730382?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



3.  AR模型的求解

AR模型可以采用Yule-Walker方程的形式进行求解[3]。考虑p阶AR模型有相应的AR特征多项式和相应的AR特征方程：

![](https://img-blog.csdn.net/20161209103740726?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



![](https://img-blog.csdn.net/20161209103751838?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


4. AR模型示例
![](https://img-blog.csdn.net/20161209103803836?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

     两维的形状可以用一维的实数或复数函数表示, 把这个函数看成一个随机过程实现, 通 过估计得到的模型参数就可以作为形状描述子 . 用自回归模型[127]分析闭合形状是 Kashyap 和Chellappa 在 1981 年首先提出来的, 他 们用自回归系数作特征矢量来刻划形状 . 在Dubois 和 Glanz[128]的实验中得到了很好的结 果, 在质心与轮廓之间以等角间距采样得到径向量的长度序列, 然后对此序列应用AR 模 型 . Kartikeyan[129]等人认为线性AR 模型只适用于识别那些形状明显不同的图形而对形状 差别较小的图形识别能力较差, 为此构造了非因果二次Volterra 模型 . 但是这种模型的计 算量很大, 模型阶数不易选择, 从而使特征集的形式很难统一并增大了模式分类的难度 . Das[130]等人采用了二维双变量AR 模型, 这种方法要估计的模型系数是相应同阶次一维模 型的四倍, 冗余性大 . 另外, 此模型只是简单地将直角坐标 x , y 作为双变量来处理, 忽视了 它们之间的正交特性, 导致模型系数并不直接具备旋转不变性 . 为了克服双变量AR 模型的 固有缺点, Sekita[131]等人提出了复数域AR (CAR)模型, 实验研究表明识别效果优于不变矩 和傅立叶描述符方法, 尤其在有噪声的情况下 . 但是由于该模型是线性的, 它不能反映图形 轮廓的二维非线性封闭循环特性和局部特性, 所以在识别某些相似图形时效果不好 . 自回归模型的主要缺点就是它只使用一个预测参数集来描述整个形状, 如果形状中有 很多角点或者形状变化非常剧烈, 那么这个形状就变得难以预测 . 为此, He 和 Kundu[132]把 AR 模型和隐M arkov 模型结合起来用于形状分析, 把形状边界分成若干段, 每一段用AR 模型描述, 所得到的向量再用隐M arkov 模型分析


参考文献

[1]R. H. Shumway  and D. S. Stoffer, Time Series Analysis and Its Applications With R Examples, Third ed.: Springer, 2011.

[2]A. V. M. a. P. S. P. Cowpertwait, Introductory Time Series with R: Springer, 2009.

[3]J. D. Cryer and K. S. Chan, Time Series Analysis with With Applications in R(Second Edition): Springer, 2008.

[4]M. Wildi, "An Intro duction to State Space Mo dels," 2013.

[5]J. Durbin and S. J. Koopman, Time Series Analysis by State Space Methods: Second Edition: OUP Oxford, 2012.



[6] J. J. F. Commandeur and S. J. Koopman, An Introduction to State Space Time Series Analysis: OUP Oxford, 2007.





