# Lindahl模型的一点讨论 - 知乎
# 

**这是本专栏的第 *35* 篇日记**

（回来写正经专栏啦~）

昨天高微课讲公共物品（Public Goods），介绍了Lindahl模型，结果后面引发了一点有趣的讨论。

首先说一下问题设定：

一个社区当中有I个居民（英文字母I，不是数字1），有两种商品：私人商品和公共商品。

每个家庭有各自的私人商品禀赋![\omega^i](https://www.zhihu.com/equation?tex=%5Comega%5Ei)，没有公共商品禀赋。

每个家庭将一部分私人商品交给社区，由社区生产公共商品。假设提交的私人商品总量是![z](https://www.zhihu.com/equation?tex=z)，那么生产的公共商品数量为![\xi = g(z)](https://www.zhihu.com/equation?tex=%5Cxi+%3D+g%28z%29)，生产函数![g(\cdot)](https://www.zhihu.com/equation?tex=g%28%5Ccdot%29)可微、单调增、**concave**。我们可以把生产函数写成反函数（由于不是严格单调增，准确地说应该是correspondence）的形式，也就是理解成成本函数![z=c(\xi)](https://www.zhihu.com/equation?tex=z%3Dc%28%5Cxi%29)，成本函数应该是convex的。

然后家庭消费剩下的私人商品![x^i](https://www.zhihu.com/equation?tex=x%5Ei)和共同拥有的公共商品![\xi](https://www.zhihu.com/equation?tex=%5Cxi)（注意公共商品没有对手性（non-rivalry），所以所有人消费的公共商品数量是一样的），得到的效用是![u^i(x^i,\xi)](https://www.zhihu.com/equation?tex=u%5Ei%28x%5Ei%2C%5Cxi%29)，效用函数可微、对各变量分别单调增、quasi-concave、满足Inada Condition（所以消费量必须是正的）。

可行分配（feasible allocation）需要满足资源约束
![\sum_{i\in I}x^i+z = \sum_{i\in I} \omega^i \stackrel {\Delta}{=}w](https://www.zhihu.com/equation?tex=%5Csum_%7Bi%5Cin+I%7Dx%5Ei%2Bz+%3D+%5Csum_%7Bi%5Cin+I%7D+%5Comega%5Ei+%5Cstackrel+%7B%5CDelta%7D%7B%3D%7Dw)

其中![z=c(\xi)](https://www.zhihu.com/equation?tex=z%3Dc%28%5Cxi%29)。

相应的，对于一个可行分配，如果不存在另外一个可行分配，使得在后者当中至少有一个家庭的效用比前者严格更高，而其它家庭的效用至少一样，那么前者就是帕累托最优分配，这也是一个常规的定义。

帕累托最优分配可以通过解社会规划者问题来求
![\max_{x,\xi}\sum_{i\in I}\alpha_i u^i(x^i,\xi)](https://www.zhihu.com/equation?tex=%5Cmax_%7Bx%2C%5Cxi%7D%5Csum_%7Bi%5Cin+I%7D%5Calpha_i+u%5Ei%28x%5Ei%2C%5Cxi%29)
![s.t. \sum_{i\in I}x^i + c(\xi) = w](https://www.zhihu.com/equation?tex=s.t.+%5Csum_%7Bi%5Cin+I%7Dx%5Ei+%2B+c%28%5Cxi%29+%3D+w)

其中![\alpha_i](https://www.zhihu.com/equation?tex=%5Calpha_i)是Pareto weights，表示社会规划者对各个家庭的所施加的权重。

用拉格朗日乘数法，求一阶条件后，我们可以知道，如下两个等式描述了这个问题的帕累托最优分配：

一个是上述资源约束；

另一个是![c'(\xi) = \sum_{i\in I} \frac{\partial u^i/\partial \xi}{\partial u^i/\partial x^i} \stackrel{\Delta}{=}\sum_{i\in I}p^i(x^i,\xi)](https://www.zhihu.com/equation?tex=c%27%28%5Cxi%29+%3D+%5Csum_%7Bi%5Cin+I%7D+%5Cfrac%7B%5Cpartial+u%5Ei%2F%5Cpartial+%5Cxi%7D%7B%5Cpartial+u%5Ei%2F%5Cpartial+x%5Ei%7D+%5Cstackrel%7B%5CDelta%7D%7B%3D%7D%5Csum_%7Bi%5Cin+I%7Dp%5Ei%28x%5Ei%2C%5Cxi%29)

后一个等式的经济学含义是很明显的：等式左侧是多生产一单位公共商品的成本，右边是多生产一单位公共商品（同时家庭少消费一部分私人商品）的福利，两者相等时总福利最大。右边定义的![p^i](https://www.zhihu.com/equation?tex=p%5Ei)函数就表示了多生产一单位公共商品给每个家庭带来的福利，也可以认为就是家庭对这额外一单位公共商品的估值（valuation）（注意是以私人物品为单位的）。

终于说到正题了。Lindahl模型的想法是，我们没有centralized的社会规划者，只有decentralized的家庭和生产，为了达到帕累托最优，我们要求每个家庭在使用公共商品时需要付出![p^i](https://www.zhihu.com/equation?tex=p%5Ei)的价格购买公共商品。于是，Lindahl均衡就是：

首先，家庭的效用最大化问题
![\max_{x^i,\xi} u^i(x^i,\xi)](https://www.zhihu.com/equation?tex=%5Cmax_%7Bx%5Ei%2C%5Cxi%7D+u%5Ei%28x%5Ei%2C%5Cxi%29)
![s.t. x^i + p^i\xi = \omega^i](https://www.zhihu.com/equation?tex=s.t.+x%5Ei+%2B+p%5Ei%5Cxi+%3D+%5Comega%5Ei)

其次，公共商品生产的利润最大化问题

![\max (\sum_{i\in I}p^i)\xi - c(\xi)](https://www.zhihu.com/equation?tex=%5Cmax+%28%5Csum_%7Bi%5Cin+I%7Dp%5Ei%29%5Cxi+-+c%28%5Cxi%29)

最后，市场出清

![\sum_{i \in I}x^i+c(\xi) = w](https://www.zhihu.com/equation?tex=%5Csum_%7Bi+%5Cin+I%7Dx%5Ei%2Bc%28%5Cxi%29+%3D+w)

可以证明，Lindahl均衡的分配如果存在，一定是帕累托最优分配。

这是因为家庭效用最大化问题的一阶条件要求![p^i = p^i(x^i,\xi)](https://www.zhihu.com/equation?tex=p%5Ei+%3D+p%5Ei%28x%5Ei%2C%5Cxi%29)，而生产利润最大化问题的一阶条件要求![\sum_{i\in I}p^i = c'(\xi)](https://www.zhihu.com/equation?tex=%5Csum_%7Bi%5Cin+I%7Dp%5Ei+%3D+c%27%28%5Cxi%29)，这两者一结合就是帕累托最优分配的第二个条件。而市场出清就是资源约束，也就是帕累托最优分配的第一个条件。

但是这只是看起来很美……事实上公共物品的问题就在于，它不仅是非对手性的（non-rivalry）的，而且是非排它性的（non-excludable）（有排它性的商品，Mankiw的书上叫club goods，俱乐部商品），所以你没法要求别人一定要付钱购买；所以公共物品面临搭便车（free ride）的问题，然后就引出来机制设计的问题。

不过这个不是我今天想讲的重点。上面这些就是我们上课的时候的内容，但是下课以后我就去找教授问问题了：**这个Lindahl模型的设定貌似错了吧？**

如果我们已经求出来一个Lindahl均衡的解了，那么它一定要满足

1. 所有的预算约束和资源约束

2. 所有的一阶条件

那么我们看，首先把所有家庭的预算约束相加
![\sum_{i\in I}x^i + (\sum_{i\in I}p^i)\xi = w](https://www.zhihu.com/equation?tex=%5Csum_%7Bi%5Cin+I%7Dx%5Ei+%2B+%28%5Csum_%7Bi%5Cin+I%7Dp%5Ei%29%5Cxi+%3D+w)

再减去资源约束
![(\sum_{i\in I}p^i)\xi - c(\xi) = 0](https://www.zhihu.com/equation?tex=%28%5Csum_%7Bi%5Cin+I%7Dp%5Ei%29%5Cxi+-+c%28%5Cxi%29+%3D+0)

这说明生产利润最大化时，利润为0。

然后，我们又根据生产利润最大化的一阶条件
![\sum_{i \in I}p^i = c'(\xi)](https://www.zhihu.com/equation?tex=%5Csum_%7Bi+%5Cin+I%7Dp%5Ei+%3D+c%27%28%5Cxi%29)

得到
![c'(\xi) \xi = c(\xi)](https://www.zhihu.com/equation?tex=c%27%28%5Cxi%29+%5Cxi+%3D+c%28%5Cxi%29)

重新排列一下
![c'(\xi) = \frac{c(\xi)}{\xi}](https://www.zhihu.com/equation?tex=c%27%28%5Cxi%29+%3D+%5Cfrac%7Bc%28%5Cxi%29%7D%7B%5Cxi%7D)

一个正常的想法是生产0个公共商品的总成本是0， 那么
![c'(\xi) = \frac{c(\xi) - c(0)}{\xi - 0}](https://www.zhihu.com/equation?tex=c%27%28%5Cxi%29+%3D+%5Cfrac%7Bc%28%5Cxi%29+-+c%280%29%7D%7B%5Cxi+-+0%7D)

但因为我们假定了生产函数是concave的，那么成本函数就是convex的，除非生产函数是线性的，否则这个式子不成立，但线性生产函数的要求又太强了……

那么我们能不能放弃生产0个公共商品的总成本是0这个假设呢？也就是说，生产是有固定成本的？

也不行，如果生产是有固定成本的，那么生产函数在投入小于固定成本的部分是线性的，大于固定成本之后的部分是concave的，在固定成本处就会有一个kinky，生产函数总体就不是concave的了，进而成本函数总体也就不是convex的了，这个时候就必须考虑边角解的情况了——如果固定成本足够大，也许不生产公共商品才是最优选择。

但是问题并不是不能解决的，实际上我们要做的不是在生产函数上做文章。

我回想起我大三上公共财政学刚开始学一般均衡的时候，我认为学到最重要的一句话是：所有的收益和损失都是人的，不是企业的——企业的收益和损失，最后还是要归结为投资人的收益和损失。

所以，我们可以允许生产利润不为0，但是相应的生产利润（或亏损）由家庭承担就行了。

既然每个家庭投入了![p^i](https://www.zhihu.com/equation?tex=p%5Ei)以购买公共商品，我们可以将其视为每个家庭入股了公共商品生产的企业，其份额为
![\tau_i=\frac{p^i}{\sum_{i\in I}p^i}](https://www.zhihu.com/equation?tex=%5Ctau_i%3D%5Cfrac%7Bp%5Ei%7D%7B%5Csum_%7Bi%5Cin+I%7Dp%5Ei%7D)

然后将企业利润的![\tau_i](https://www.zhihu.com/equation?tex=%5Ctau_i)部分以lump sum的形式返还给各个家庭就可以了。由于是lump sum，家庭效用最大化的一阶条件并不受影响，但是上述预算约束、资源约束、生产利润最大化的矛盾就自然消除了。

实际上lump sum返还的方式，之前在学税收如何影响福利的时候就学过。

政府收税降低效用，可能有两方面的原因，一方面是收税扭曲激励，另一方面是收税之后，一部分社会资源到了政府手中，家庭所拥有的总资源减少了，那么总福利自然降低了。

但是如果我们将tax以lump sum的形式返还，那么第二个方面的影响就不存在了——整个社会的资源还是那么多，政府收税是“取之于民用之于民”，那么如果这时候效用还是降低，就完全是因为收税扭曲激励的缘故了。

因此这种lump sum返还的方式，可以用来分解这两种效应。

但是在Lindahl模型的这个设定中，lump sum返还就更加重要了，没有这个返还，这个模型的解都有可能不存在，进一步的机制设计部分就更加无从谈起了。

*(Photo credit: [Oiluj Samall Zeid](https://link.zhihu.com/?target=https%3A//www.flickr.com/photos/92799712%40N04/15557830895/) via [VisualHunt.com](https://link.zhihu.com/?target=https%3A//visualhunt.com/re/196301) / [CC BY-NC-ND 2.0](https://link.zhihu.com/?target=http%3A//creativecommons.org/licenses/by-nc-nd/2.0/))*

