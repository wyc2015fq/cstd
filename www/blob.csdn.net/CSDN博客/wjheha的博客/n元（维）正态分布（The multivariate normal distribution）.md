# n元（维）正态分布（The multivariate normal distribution） - wjheha的博客 - CSDN博客
2017年12月18日 14:51:13[wjheha](https://me.csdn.net/wjheha)阅读数：3428
  在学习高斯判别分析（Gaussian discriminant analysis）时，出现了n元正态分布的密度函数，函数中出现了矩阵，弄得大家一头雾水。其实这个公式在大部分概率论书籍中都没有提到，不过，简要推导一下，就可以得到结果。
  茆诗松《概率论与数理统计教程》第二版中介绍了协方差矩阵和n元正态分布的密度函数，截图大家看一下，推荐身边准备这本书！ 
![这里写图片描述](https://img-blog.csdn.net/20171218134504949?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2poZWhh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171218134527847?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2poZWhh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## *********************************************************
如上图所示，下面简要推导一下公式：
### 一元正态分布：
![这里写图片描述](https://img-blog.csdn.net/20171218135012575?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2poZWhh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 二元正态分布：
![这里写图片描述](https://img-blog.csdn.net/20171218135340962?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2poZWhh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
公式中，有一个相关系数（读作rou，符号形状和p相似，为了便于书写，后文就用p来表示）。二元正态分布满足下面***两个性质（推广到n元正态分布也有类型的性质）：***
- 二元正态分布的边缘分布（边际分布）为一元正态分布，且二元正态分布的边际分布中不含参数p
- 
二元正态分布中除含有各分量的边际分布外，还含有两个分量间相互关联的信息。描述这种相互关联程度的一个特征数就是协方差。
***从上面两条信息可得，已知n元联合正态分布，推导其变量的边际分布很容易；已知各个变量边际分布，推导n元联合正态分布往往不可行，因为联合正太分布中还含有两个分量间相互关联的信息。***
#### 协方差
![这里写图片描述](https://img-blog.csdn.net/20171218140849631?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2poZWhh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
#### 相关系数
![这里写图片描述](https://img-blog.csdn.net/20171218141152548?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2poZWhh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
***由此可见，相关系数和协方差刻画的是同一个特征（相互关联程度）！***
## ********************************************************************
## 二元正态分布密度函数推导
如上所述，已经得到了二元正态分布的联合概率密度： 
![这里写图片描述](https://img-blog.csdn.net/20171218142314596?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2poZWhh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
为了方便推导，我们假设X与Y相互独立，则p=0,与p有关的项都消掉了，就变成下述这样： 
![这里写图片描述](https://img-blog.csdn.net/20171218144926006?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2poZWhh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171218145008833?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2poZWhh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这样，就得到了整个公式，n元正态分布，类似的对比过去就可以了！
