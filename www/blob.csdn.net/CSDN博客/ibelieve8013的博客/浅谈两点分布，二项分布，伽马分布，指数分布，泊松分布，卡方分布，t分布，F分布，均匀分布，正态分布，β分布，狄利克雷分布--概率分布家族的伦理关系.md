# 浅谈两点分布，二项分布，伽马分布，指数分布，泊松分布，卡方分布，t分布，F分布，均匀分布，正态分布，β分布，狄利克雷分布--概率分布家族的伦理关系 - ibelieve8013的博客 - CSDN博客





2018年05月07日 11:27:34[ibelieve8013](https://me.csdn.net/ibelieve8013)阅读数：478








分布家族的伦理关系



浅谈两点分布，二项分布，伽马分布，指数分布，泊松分布，卡方分布，t分布，F分布，均匀分布，正态分布，β分布，狄利克雷分布。（红丸子，白丸子，四喜丸子。。。）



我们知道，在数理统计中，经常是和各种分布打交道，也经常搞清楚搞不清楚，我是谁，我在学什么，这些分布，到底是些什么关系？

最近在学随机过程又遇到了这个问题，虽然好像并没有什么太多关系，但是搞不清楚的，马马虎虎的感觉很不爽，而且什么鬼分布的概率密度函数，感觉记了一辈子都记不下来，记住又忘，记住又忘。所以本文的意图是要梳理这些分布的关系，以及他们代表的实际意义，还有加强对他们的概率密度函数，期望方差等的记忆。（很粗浅）

涉及的分布主要有：两点分布，二项分布，伽马分布，指数分布，泊松分布，卡方分布，t分布，F分布，均匀分布，正态分布，β分布，狄利克雷分布。（怎么这么多分布！）

好了，废话不多说，直接开干。

两点分布：

两点分布很简单，就是说一个实验有两种可能，非此即彼，概率分别是P和1-P，这个实验只做一次，ok，它就是服从两点分布。

二项分布：



![](https://img-blog.csdn.net/20180509122811625?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

但是，我要是要做多次呢，比如我要做n次独立重复的伯努利实验，那么我们就可以对实验成功的次数X构建一个二项分布，所以X的分布律是：



泊松分布：



说了二项分布，不得不说说泊松分布，这个分布是弄啥捏，就是描述一段时间内某个事件发生的次数的概率一个分布，故计数类的模型比较适合用泊松分布，等等，这个和二项分布有什么不一样吗?好像都是一个意思？其实确实是一个意思，只是我们知道，当

![](https://img-blog.csdn.net/20180509122854631?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



这个里面的n变得灰常大，这个公式就没眼看了，那么我们只能考虑一下变个形式：

![](https://img-blog.csdn.net/20180509122937936?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

注意，这里的lambda是个什么鬼？

其实![](https://img-blog.csdn.net/20180509123003235?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70),比如我描述一个事件发生的概率是0.0002，而我要知道1000个样本中事件发生的次数的概率，我可以直接用lambda来刻画这个模型，一个参数搞定两个参数的活，这不就节约了工钱吗？（显然可以看出，当k值变化是，![](https://img-blog.csdn.net/20180509123044783?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)是不改变的，所以有![](https://img-blog.csdn.net/20180509123122217?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)，其实后来看来，这个![](https://img-blog.csdn.net/20180509123044783?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)就是一个归一化因子而已鸭）其实泊松分布并没有那么严格要求n取非常大，一般只需要大于等于50，我们就可以用泊松分布。

伽马分布：

到了有名的伽马分布，其实我们见了很多次伽马分布，伽马分布的概率密度函数依然有可能写不出来，

![](https://img-blog.csdn.net/20180509123135427?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



其实这个分布实际上是什么意思呢？？描述什么样的一个过程呢？其实可以这样理解：伽马分布描述的是一个事件A第![](https://img-blog.csdn.net/20180509123209986?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)次发生所需要的时间，![](https://img-blog.csdn.net/20180509123216866?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)被我们称作形状参数，![](https://img-blog.csdn.net/2018050912322546?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)则是一个尺度参数。那么就是发生的概率，可把理解为一个描述发生频率的参数，发生一次需要的时间是![](https://img-blog.csdn.net/20180509123242978?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70),这也不难理解了为什么.

![](https://img-blog.csdn.net/20180509123251334?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

指数分布：

在伽马分布中，当![](https://img-blog.csdn.net/20180509123307202?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)意思就是发生一次需要的时间，很显然，这就是指数分布的含义。

指数分布的概率密度函数很简单，是![](https://img-blog.csdn.net/20180509123321349?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70),期望是![](https://img-blog.csdn.net/20180509123354139?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)我们可以用它来描述什么呢，比如一个故障发生的时间，可以用指数分布来描述。

卡方分布：

![è¿éåå¾çæè¿°](https://img-blog.csdn.net/20180819215913604?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl8zODY1Njg5MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180819215923394?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl8zODY1Njg5MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/2018050912340878?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)分布也可以看做是伽马分布的特例：![](https://img-blog.csdn.net/20180509123418243?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)可是这有什么意义呢？？好像从表面上看不出任何的意义，其实这可以从另一个角度去描述，比如有N个独立随机变量都是标准正态分布，那么他们的平方和就是卡方分布，这个分布常常用来构造假设检验的检验统计量中，检验统计量就是为了尽可能多的包含统计信息，而这个分布就很好的做到了呀。

t分布：

![](https://img-blog.csdn.net/20180819215954393?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl8zODY1Njg5MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180819220001737?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl8zODY1Njg5MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

t分布是个什么鬼？？我们也经常用t分布来构造检验统计量，简而言之，一个标准正态分布比上另一个卡方分布*Y*的![](https://img-blog.csdn.net/20180509123439833?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)，这就是t分布，那么我们可以想象，当n趋于无穷大时，这个分布是收敛于标准正态分布的。所以一般的t分布，它的概率密度图像比正态分布的尾部更大，就这个特点。

F分布：

F分布就是两个卡方分布的一通骚操作，分别用两个分布除以他们的自由度，再相比，就得到了F分布。同样的，这个分布主要用在假设检验中，构造检验统计量。

均匀分布：

到了均匀分布了，均匀分布没有什么特殊的地方，就是在区间内取任意值概率都是相等的。所以期望也就是区间的中点处。记住均值和方差：



![](https://img-blog.csdn.net/20180509123454516?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

正态分布：

正态分布有意思了，自然界很多现象都是服从正态分布的，比如人的身高，社会的财富，班上的成绩，一句经典的话叫做：实验工作者认为它是数学定理，数学工作者认为它是一个经验定理。有意思，这里有知乎大佬的解说：[https://www.zhihu.com/question/26854682](https://www.zhihu.com/question/26854682)

其实整正态分布呢，可以看做是在二项分布在n趋于无穷，进行无数次伯努利实验后结果的分布。如下图：

![](https://img-blog.csdn.net/20180509123510597?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

假设每次小球掉下来非左即右，那么N次实验之后，如下图结果：





![](https://img-blog.csdn.net/20180509123538694?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180509123547294?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这就是为什么是正态分布是这样，考虑简化一下模型，社会财富也是如此，大多数人在平庸，少数人在右边，你要是任由发展，无所作为，那么你大概率是平庸的，但是你如果在每次的选择中能够左右自己的道路，那你就越可能往正态分布的右边走，是不是很励志？



![](https://img-blog.csdn.net/20180509123602188?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)分布：



这是一个描述概率的概率分布，听起来有点抽象，![](https://img-blog.csdn.net/20180509123628467?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)中，假设我用击球的例子，击中次数是![](https://img-blog.csdn.net/20180509123646836?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)，未击中的次数是![](https://img-blog.csdn.net/20180509123656398?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70),那么击中率的概率密度是

![](https://img-blog.csdn.net/20180509123702828?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



那么假设又实验了300次，击中100次。分布变成了beta(),这里很有趣，看到了吗，在后面的实验数据上来之后，并没有改变原来函数的分布，计算变得十分简单，这种不改变函数本身所属family的特性，我们叫做共轭，于是密度函数变成了：

![](https://img-blog.csdn.net/20180509123714805?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

所以我们可以这样理解，我们可以用beta分布结合先验知识，通过后续的数据对先验结论进行修正，即：先合理假设，再科学修正。详细的请看原文大佬给力解释：

[https://blog.csdn.net/a358463121/article/details/52562940](https://blog.csdn.net/a358463121/article/details/52562940)

![](https://img-blog.csdn.net/20180509123725252?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180509123731548?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

狄利克雷分布：

在贝叶斯推理的先验分布中，Dirichlet分布是最常用的一种分布，刚才讨论了beta分布，那么聪明的你，是否想过，如果我的实验不是两种结果，比如我把击中未击中具体化到：击中10,9,8,7…环，这么多结果，k种结果分别发生的概率，如何去刻画每种概率发生的概率？理解到了这个意思，就理解到了狄利克雷分布。其余的就不再赘述，详细的可见知乎er的回答，比我这个详细一万倍。

[https://www.zhihu.com/question/26751755/answer/80931791](https://www.zhihu.com/question/26751755/answer/80931791)

作为本文草草的见解的草草的结尾，唯有祭出神图，以镇此文：

![](https://img-blog.csdn.net/20180507112923770)



