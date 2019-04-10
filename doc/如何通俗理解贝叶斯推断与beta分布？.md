# 如何通俗理解贝叶斯推断与beta分布？

 																				2018年07月19日 14:32:45 					[马同学高等数学](https://me.csdn.net/ccnt_2012) 						阅读数：1635 										

 									

有一枚硬币（不知道它是否公平），假如抛了三次，三次都是“花”：

![img](https://img-blog.csdn.net/20180720093536556?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

能够说明它两面都是“花”吗？

![img](https://img-blog.csdn.net/20180720093547860?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

1 贝叶斯推断

按照传统的算法，抛了三次得到三次“花”，那么“花”的概率应该是：

![p=\frac{3}{3}=100\%\\](https://www.zhihu.com/equation?tex=p%3D%5Cfrac%7B3%7D%7B3%7D%3D100%5C%25%5C%5C)

但是抛三次实在太少了，完全有可能是运气问题。我们应该怎么办？

![img](https://img-blog.csdn.net/20180720093601510?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

托马斯·贝叶斯（1702－1761），18世纪英国数学家，1742年成为英国皇家学会会员。

贝叶斯认为在实验之前，应根据不同的情况对硬币有所假设。不同的假设会得到不同的推断。

比如和滑不溜手的韦小宝玩。韦小宝可能拿出各种做过手脚的硬币，让我们猜不透，只能假设对硬币一无所知。这种假设之下，我们就只能根据实验结果来猜测。

因此，实验结果是“扔三次，三次花”，倾向于认为韦小宝有可能作弊：

![img](https://img-blog.csdn.net/20180720093613223?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

大侠陈近南用的可能是公平硬币：

![img](https://img-blog.csdn.net/20180720093622903?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

而憨坏的多隆，真的有可能用两面“花”来和你玩：

![img](https://img-blog.csdn.net/20180720093634167?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

各种假设称为先验分布，结合刚才“扔三次，三次花”的实验数据，推断出硬币的后验分布，这就是贝叶斯推断：

![先验分布+实验数据\implies后验分布\\](https://www.zhihu.com/equation?tex=%E5%85%88%E9%AA%8C%E5%88%86%E5%B8%83%2B%E5%AE%9E%E9%AA%8C%E6%95%B0%E6%8D%AE%5Cimplies%E5%90%8E%E9%AA%8C%E5%88%86%E5%B8%83%5C%5C)

这里补充一下，可能大家觉得再多抛几次硬币就可以了，何必弄什么贝叶斯推断。不过现实生活中有一些事件不是能够多“抛”几次的，比如地震、彗星撞击地球等等。这里只是借着硬币来讨论问题。

2 ![\textrm{Beta}](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D) 分布

那么问题来了，“先验分布”，“后验分布”用数学怎么表示：

![\underbrace{先验分布}_{\color{red}{?}}+实验数据\implies\underbrace{后验分布}_{\color{red}{?}}\\](https://www.zhihu.com/equation?tex=%5Cunderbrace%7B%E5%85%88%E9%AA%8C%E5%88%86%E5%B8%83%7D_%7B%5Ccolor%7Bred%7D%7B%3F%7D%7D%2B%E5%AE%9E%E9%AA%8C%E6%95%B0%E6%8D%AE%5Cimplies%5Cunderbrace%7B%E5%90%8E%E9%AA%8C%E5%88%86%E5%B8%83%7D_%7B%5Ccolor%7Bred%7D%7B%3F%7D%7D%5C%5C)

对于扔硬币，![\textrm{Beta}](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D) 分布非常适合用来完成这个任务。

2.1 先验分布

![\textrm{Beta}](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D) 分布简记为（这一节里面的所有细节会在后面给出）：

![\textrm{Beta}(a,b)\\](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D(a%2Cb)%5C%5C)

根据![a,b](https://www.zhihu.com/equation?tex=a%2Cb) 参数的不同，形态各异：

![img](https://img-blog.csdn.net/20180720093648793?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这个特性非常适合用来做先验分布。比如，在韦小宝面前，我们对硬币一无所知。

贝叶斯说，一无所知也就是意味着任何概率都是一样的，都是有可能的，所以选用均匀分布（所谓的无信息先验，可以参看这篇[文章](https://projecteuclid.org/download/pdfview_1/euclid.ejs/1320416981)）：

![img](https://img-blog.csdn.net/20180720093705559?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![\textrm{Beta}(1,1)](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D(1%2C1)) 正好就是均匀分布：

![img](https://img-blog.csdn.net/201807200937176?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

正直的陈近南，可能用的是公平硬币，也就是说概率在0、1之间（0表示“字”，1表示“花”），![\textrm{Beta}(5,5)](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D(5%2C5)) 可以表示这样的分布：

![img](https://img-blog.csdn.net/20180720093726392?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

而憨坏的多隆，可能用了两面花，也就是说概率可能集中到1附近，![\textrm{Beta}(5,1)](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D(5%2C1)) 可以表示这样的分布：

![img](https://img-blog.csdn.net/2018072009373643?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

也就是说可以用![\textrm{Beta}](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D) 分布来模拟各种先验分布：

- 一无所知：![\textrm{Beta}(1,1)](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D(1%2C1))
- 公平硬币：![\textrm{Beta}(5,5)](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D(5%2C5))
- 两面花：![\textrm{Beta}(5,1)](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D(5%2C1))

2.2 后验分布

用![\textrm{Beta}](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D) 分布来模拟扔硬币的先验分布之后，通过贝叶斯推断，得到的后验分布依然是![\textrm{Beta}](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D) 分布：

![\textrm{Beta}(a,b)+实验数据\implies\textrm{Beta}(m,n)\\](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D(a%2Cb)%2B%E5%AE%9E%E9%AA%8C%E6%95%B0%E6%8D%AE%5Cimplies%5Ctextrm%7BBeta%7D(m%2Cn)%5C%5C)

具体到这里：

![\textrm{Beta}(a,b)+实验数据\implies\textrm{Beta}(a+花,b+字)\\](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D(a%2Cb)%2B%E5%AE%9E%E9%AA%8C%E6%95%B0%E6%8D%AE%5Cimplies%5Ctextrm%7BBeta%7D(a%2B%E8%8A%B1%2Cb%2B%E5%AD%97)%5C%5C)

再具体到韦小宝的情况就是：

![\textrm{Beta}(1,1)+(3,0)\implies\textrm{Beta}(4,1)\\](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D(1%2C1)%2B(3%2C0)%5Cimplies%5Ctextrm%7BBeta%7D(4%2C1)%5C%5C)

其中，用![(3,0)](https://www.zhihu.com/equation?tex=(3%2C0))来表示实验数据，意思是3次花，0次字（![(2,1)](https://www.zhihu.com/equation?tex=(2%2C1)) 就是2次花，1次字）。

图像上的变化就是：

![img](https://img-blog.csdn.net/20180720093750410?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可以看到，作弊的可能性还是比较大的。

陈近南的情况：

![img](https://img-blog.csdn.net/20180720093801335?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

结合实验数据之后，图像的中心从0.5往0.6方向移动了，作弊可能性有所增加，不过总体来看应该还是公平硬币的可能性大。

多隆的情况：

![img](https://img-blog.csdn.net/2018072009381184?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

更向1集中，作弊的可能性非常高。

3 代数细节

3.1 贝叶斯推断

贝叶斯推断：

![先验分布+实验数据=后验分布\\](https://www.zhihu.com/equation?tex=%E5%85%88%E9%AA%8C%E5%88%86%E5%B8%83%2B%E5%AE%9E%E9%AA%8C%E6%95%B0%E6%8D%AE%3D%E5%90%8E%E9%AA%8C%E5%88%86%E5%B8%83%5C%5C)

的应用到二项式分布的数学细节如下。假设实验数据![X|p](https://www.zhihu.com/equation?tex=X%7Cp) 服从二项分布：

![X|p\sim bin(n,p)\\](https://www.zhihu.com/equation?tex=X%7Cp%5Csim%20bin(n%2Cp)%5C%5C)

上面的式子根据贝叶斯定理（离散贝叶斯可以参看“[如何理解贝叶斯定理](https://www.matongxue.com/madocs/279.html)？”，连续贝叶斯可以参看[这里](https://zh.wikipedia.org/wiki/%E8%B4%9D%E5%8F%B6%E6%96%AF%E5%AE%9A%E7%90%86#cite_note-2)）可以表示为：

![\underbrace{f(p|X=k)}_{后验分布}=\frac{\overbrace{P(X=k|p)}^{实验数据}\overbrace{f(p)}^{先验分布}}{\underbrace{P(X=k)}_{常数}}\\](https://www.zhihu.com/equation?tex=%5Cunderbrace%7Bf(p%7CX%3Dk)%7D_%7B%E5%90%8E%E9%AA%8C%E5%88%86%E5%B8%83%7D%3D%5Cfrac%7B%5Coverbrace%7BP(X%3Dk%7Cp)%7D%5E%7B%E5%AE%9E%E9%AA%8C%E6%95%B0%E6%8D%AE%7D%5Coverbrace%7Bf(p)%7D%5E%7B%E5%85%88%E9%AA%8C%E5%88%86%E5%B8%83%7D%7D%7B%5Cunderbrace%7BP(X%3Dk)%7D_%7B%E5%B8%B8%E6%95%B0%7D%7D%5C%5C)

其中 ![k](https://www.zhihu.com/equation?tex=k) 为“花”的次数。分母与实验数据无关，可以视作常数：

因此，写成下面这样更容易看清楚重点（其中 ![\propto](https://www.zhihu.com/equation?tex=%5Cpropto) 表示两者之间成比例）：

![\underbrace{f(p|X=k)}_{后验分布}\quad\propto\quad\overbrace{P(X=k|p)}^{实验数据}\underbrace{f(p)}_{先验分布}\\](https://www.zhihu.com/equation?tex=%5Cunderbrace%7Bf(p%7CX%3Dk)%7D_%7B%E5%90%8E%E9%AA%8C%E5%88%86%E5%B8%83%7D%5Cquad%5Cpropto%5Cquad%5Coverbrace%7BP(X%3Dk%7Cp)%7D%5E%7B%E5%AE%9E%E9%AA%8C%E6%95%B0%E6%8D%AE%7D%5Cunderbrace%7Bf(p)%7D_%7B%E5%85%88%E9%AA%8C%E5%88%86%E5%B8%83%7D%5C%5C)

3.2 ![\textrm{Beta}](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D) 分布

![\textrm{Beta}](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D) 长成这个样子：

![\textrm{Beta}(a,b)=\frac{1}{\textrm{B}(a,b)}x^{{a -1}}(1-x)^{{b -1}}\\](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D(a%2Cb)%3D%5Cfrac%7B1%7D%7B%5Ctextrm%7BB%7D(a%2Cb)%7Dx%5E%7B%7Ba%20-1%7D%7D(1-x)%5E%7B%7Bb%20-1%7D%7D%5C%5C)

其中，![B](https://www.zhihu.com/equation?tex=B)B 为![\textrm{Beta}](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D) 函数。

随着![a,b](https://www.zhihu.com/equation?tex=a%2Cb) 的变换，![\textrm{Beta}](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D) 分布形态各异：

![img](https://img-blog.csdn.net/20180720093828595?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3.3 共轭先验

对于二项式分布，用![\textrm{Beta}](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D) 分布作为先验分布，通过贝叶斯推断之后，后验分布依然是![\textrm{Beta}](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D) 分布：

![\textrm{Beta}(a,b)+实验数据\implies\textrm{Beta}(m,n)\\](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D(a%2Cb)%2B%E5%AE%9E%E9%AA%8C%E6%95%B0%E6%8D%AE%5Cimplies%5Ctextrm%7BBeta%7D(m%2Cn)%5C%5C)

这种特性称为[共轭先验](https://zh.wikipedia.org/wiki/%E5%85%B1%E8%BD%AD%E5%85%88%E9%AA%8C)。

并且：

![\textrm{Beta}(a,b)+实验数据\implies\textrm{Beta}(a+花,b+字)\\](https://www.zhihu.com/equation?tex=%5Ctextrm%7BBeta%7D(a%2Cb)%2B%E5%AE%9E%E9%AA%8C%E6%95%B0%E6%8D%AE%5Cimplies%5Ctextrm%7BBeta%7D(a%2B%E8%8A%B1%2Cb%2B%E5%AD%97)%5C%5C)

关于这点的证明参看[这里](https://www.youtube.com/watch?v=hKYvZF9wXkk)，需要科学上网。

文章最新版本在（有可能会有后续更新）：[如何理解贝叶斯推断，beta分布？](https://www.matongxue.com/madocs/910.html)