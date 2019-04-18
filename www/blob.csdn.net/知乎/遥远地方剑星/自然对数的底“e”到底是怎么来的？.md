# 自然对数的底“e”到底是怎么来的？ - 知乎
# 



> 从我第一次知道“e”，在我的头脑中就产生了一个疑问，人们到底是怎么发现“e”这个常数的呢？小时候，我曾经问过许多人，但是都没有给出让我心里踏实的回答。教科书上给出的是e的极限形式的定义， ![e=\lim_{n \rightarrow \infty}{(1+\frac{1}{n})^n}](https://www.zhihu.com/equation?tex=e%3D%5Clim_%7Bn+%5Crightarrow+%5Cinfty%7D%7B%281%2B%5Cfrac%7B1%7D%7Bn%7D%29%5En%7D) ，可是这并没有解决“它是怎么来的”这个问题，各种存在的极限那么多，凭什么人们要特别定义这个极限呢？
对比另一个著名常数 ![\pi](https://www.zhihu.com/equation?tex=%5Cpi) ，给人们的感受就非常直观。 ![\pi](https://www.zhihu.com/equation?tex=%5Cpi) 是圆的周长与直径的比值，它不随着圆的大小而发生变化，很容易理解人们定义这样一个常数。
作为一个数学爱好者，总想着有机会找到儿时的一些有趣问题的答案。经过一些偶然的和特意的努力，我把个人了解到的、对这个有趣问题的解释和大家分享。

其实，人们认识到e的过程还是很曲折的，不像认识并理解 ![\pi](https://www.zhihu.com/equation?tex=%5Cpi) 那样简单明了。有的普及文章认为，人们是通过“利滚利”的高利贷，并不断缩短计息周期而发现e的。从我了解的数学历史情况来看，应该不是这样。事实上，利滚利的极限应该是在人们发现了e之后才认识到的，而不是相反。

人们认识到e，还要从认识到对数说起。我们今天知道，对数是指数的逆运算，可是，人们最早认识并朴素的定义出对数的时候，还完全没有意识到这是指数的逆运算。人们是为了简化计算（特别是天文计算）而发现对数的！

**一、神奇的“加减术”**

今天，当我们计算一些复杂数字的时候是很容易的。智能手机中的计算器app、一些办公室常用的计算器等，都可以非常方便的计算加减乘除和开方、乘方的结果。可是，如果我们把时钟调回500年前，那时的人们要想计算一些复杂数字还是很不容易的。

比如，要手工计算0.258819*0.984808的结果，需要多次计算乘法、加法。当然，只算一次这样的数还可以，如果有成百上千次的类似手工计算，显然是让人崩溃的。

聪明的人们在认识到三角函数以后，就利用三角函数表和三角函数之间的关系，发明了一种将乘除计算转化为加减计算的方法，被称之为“加减术”。还以上面两个数字相乘为例：

> 已知 ![2sinA\cdot cosB=sin(A+B)+sin(A-B)](https://www.zhihu.com/equation?tex=2sinA%5Ccdot+cosB%3Dsin%28A%2BB%29%2Bsin%28A-B%29)
 查三角函数表得， ![0.258819\approx sin15^\circ](https://www.zhihu.com/equation?tex=0.258819%5Capprox+sin15%5E%5Ccirc) ， ![0.984808\approx cos10^\circ](https://www.zhihu.com/equation?tex=0.984808%5Capprox+cos10%5E%5Ccirc)
于是得到 ![0.258819\times 0.984808\approx sin15^\circ \times cos10^\circ=\frac{1}{2}(sin25^\circ +sin5^\circ)](https://www.zhihu.com/equation?tex=0.258819%5Ctimes+0.984808%5Capprox+sin15%5E%5Ccirc+%5Ctimes+cos10%5E%5Ccirc%3D%5Cfrac%7B1%7D%7B2%7D%28sin25%5E%5Ccirc+%2Bsin5%5E%5Ccirc%29)
再查三角函数表得， ![sin25^\circ \approx 0.422618](https://www.zhihu.com/equation?tex=sin25%5E%5Ccirc+%5Capprox+0.422618) ， ![sin5^\circ \approx 0.087156](https://www.zhihu.com/equation?tex=sin5%5E%5Ccirc+%5Capprox+0.087156)
于是，人们将乘除法的计算通过查找三角函数表转化为了加减法的计算，得到
![0.258819\times 0.984808\approx \frac{0.422618+0.087156}{2}=0.254887](https://www.zhihu.com/equation?tex=0.258819%5Ctimes+0.984808%5Capprox+%5Cfrac%7B0.422618%2B0.087156%7D%7B2%7D%3D0.254887)
这个计算结果和直接用计算器计算得到的结果0.254887021752是一致的。

通过这种“加减术”，在大量计算的时候，人们可以节省一半以上的工作量。这在当时那个时代，是一种巨大的进步。当然，这个进步的基础，是要制作出足够精确的三角函数表。

**二、约翰.纳皮尔（John Napier）的伟大贡献——发明对数**

约翰.纳皮尔是苏格兰数学家、天文学家，出生于1550年，过世于1617年。他为了简化天文计算，一直潜心研究简化计算的方法。大概在1594年，他从一个国王的御医那里了解到了丹麦天文学家、数学家第谷采用的“加减术”，受到了启发，并最终给出了关于“对数”的构想。他的关于对数的著作《奇妙的对数表说明书》（英文名《*A Description of the Wonderful Table of Logarithms*》，原文名《*Mirifici logarithmorum canonis descriptio*》）于1614年6月出版，他也因此一举成名。

纳皮尔用来描述他所定义的对数的方式是很有意思的，到今天，人们也没有弄清楚到底是基于怎样的思考，纳皮尔竟然用几何运动相关的模型来描述对数。以下是纳皮尔用来描述对数的“运动模型”：

> 纳皮尔构造了两个粒子的运动，粒子b在一条无穷长的射线上做匀速运动；粒子 ![\beta](https://www.zhihu.com/equation?tex=%5Cbeta) 在一条固定长度线段上做变速运动，其运动速度在数值上与 ![\beta](https://www.zhihu.com/equation?tex=%5Cbeta) 粒子到线段终点的距离相同。两个粒子的初始运动速度相同。（参见下图）
![](https://pic4.zhimg.com/v2-f5b5f6f088d30c8aef0aa06bd8929137_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='514' height='338'></svg>)纳皮尔构造运动模型定义对数的示意图【此图来源于Kathleen M. Clark (The Florida State University) and Clemency Montelle (University of Canterbury)在MMA上的文章《Logarithms: The Early History of a Familiar Function》】
> 纳皮尔定义，在某一时刻b粒子所运动的距离（例如上图中的y=AG）是 ![\beta](https://www.zhihu.com/equation?tex=%5Cbeta) 粒子到线段终点距离（对应上图中的 x=![\eta\ \omega](https://www.zhihu.com/equation?tex=%5Ceta%5C+%5Comega) ）的“对数”。后来，人们把这个“对数”关系叫做纳皮尔对数。

下面，我们用现代数学来计算一下，到底“纳皮尔对数”是个啥？
设 ![\beta](https://www.zhihu.com/equation?tex=%5Cbeta) 粒子运动的线段长度为 ![p_0](https://www.zhihu.com/equation?tex=p_0) ，那么 ![\beta](https://www.zhihu.com/equation?tex=%5Cbeta) 粒子一开始距终点的距离就是 ![p_0](https://www.zhihu.com/equation?tex=p_0) ， ![\beta](https://www.zhihu.com/equation?tex=%5Cbeta) 粒子运动的初始速度也是 ![p_0](https://www.zhihu.com/equation?tex=p_0) ，根据纳皮尔的设定，b粒子运动的初始速度（也就是b粒子的持续运动速度）也是 ![p_0](https://www.zhihu.com/equation?tex=p_0) 。
再设 ![\beta](https://www.zhihu.com/equation?tex=%5Cbeta) 粒子在t时刻距终点的距离（也就是t时刻 ![\beta](https://www.zhihu.com/equation?tex=%5Cbeta) 粒子的速度）为 ![x=p(t)](https://www.zhihu.com/equation?tex=x%3Dp%28t%29) 。
于是，t时刻 ![\beta](https://www.zhihu.com/equation?tex=%5Cbeta) 粒子所走过的路程就是 ![p_0-p(t)](https://www.zhihu.com/equation?tex=p_0-p%28t%29) ，对这个路程微分就得到了t时刻 ![\beta](https://www.zhihu.com/equation?tex=%5Cbeta) 粒子的速度，这个速度应该等于 ![p(t)](https://www.zhihu.com/equation?tex=p%28t%29) 。由此列出的微分方程如下，
![\frac{d(p_0-p(t))}{dt}=-\frac{dp(t)}{dt}=p(t)](https://www.zhihu.com/equation?tex=%5Cfrac%7Bd%28p_0-p%28t%29%29%7D%7Bdt%7D%3D-%5Cfrac%7Bdp%28t%29%7D%7Bdt%7D%3Dp%28t%29)
把这个微分方程变换一下，得
![-\frac{dp(t)}{p(t)}=dt](https://www.zhihu.com/equation?tex=-%5Cfrac%7Bdp%28t%29%7D%7Bp%28t%29%7D%3Ddt)
两边做不定积分，得
![-\int_{}^{}\frac{dp(t)}{p(t)}=\int_{}^{}dt\ \Rightarrow \ -ln(p(t))=t+C](https://www.zhihu.com/equation?tex=-%5Cint_%7B%7D%5E%7B%7D%5Cfrac%7Bdp%28t%29%7D%7Bp%28t%29%7D%3D%5Cint_%7B%7D%5E%7B%7Ddt%5C+%5CRightarrow+%5C+-ln%28p%28t%29%29%3Dt%2BC)
将t=0时 ![p(0)=p_0](https://www.zhihu.com/equation?tex=p%280%29%3Dp_0) 带入，计算得到 ![C=-ln(p_0)](https://www.zhihu.com/equation?tex=C%3D-ln%28p_0%29) ，于是有
![t=ln(p_0)-ln(p(t))=ln(\frac{p_0}{p(t)})](https://www.zhihu.com/equation?tex=t%3Dln%28p_0%29-ln%28p%28t%29%29%3Dln%28%5Cfrac%7Bp_0%7D%7Bp%28t%29%7D%29)
再看b粒子，它在t时刻走过的路程为 ![y=p_0\cdot t](https://www.zhihu.com/equation?tex=y%3Dp_0%5Ccdot+t) ，于是我们可以得到y与x的关系为
![y=p_0\cdot t=p_0\cdot ln(\frac{p_0}{p(t)})=p_0\cdot ln(\frac{p_0}{x})](https://www.zhihu.com/equation?tex=y%3Dp_0%5Ccdot+t%3Dp_0%5Ccdot+ln%28%5Cfrac%7Bp_0%7D%7Bp%28t%29%7D%29%3Dp_0%5Ccdot+ln%28%5Cfrac%7Bp_0%7D%7Bx%7D%29) ... ...（1）
这个关系就是纳皮尔给出的“纳皮尔对数”。我们由此定义纳皮尔对数为
![NapLog(x)=p_0\cdot ln(\frac{p_0}{x})](https://www.zhihu.com/equation?tex=NapLog%28x%29%3Dp_0%5Ccdot+ln%28%5Cfrac%7Bp_0%7D%7Bx%7D%29)
如果我们再把式（1）变换一下，得到
![\frac{y}{p_0}=ln(\frac{p_0}{x})=-ln(\frac{x}{p_0})=\frac{\log_{e}{(\frac{x}{p_0})}}{\log_{e}{\frac{1}{e}}}=\log_{\frac{1}{e}}{\frac{x}{p_0}}](https://www.zhihu.com/equation?tex=%5Cfrac%7By%7D%7Bp_0%7D%3Dln%28%5Cfrac%7Bp_0%7D%7Bx%7D%29%3D-ln%28%5Cfrac%7Bx%7D%7Bp_0%7D%29%3D%5Cfrac%7B%5Clog_%7Be%7D%7B%28%5Cfrac%7Bx%7D%7Bp_0%7D%29%7D%7D%7B%5Clog_%7Be%7D%7B%5Cfrac%7B1%7D%7Be%7D%7D%7D%3D%5Clog_%7B%5Cfrac%7B1%7D%7Be%7D%7D%7B%5Cfrac%7Bx%7D%7Bp_0%7D%7D)
也就是说，纳皮尔对数其实是以 ![\frac{1}{e}](https://www.zhihu.com/equation?tex=%5Cfrac%7B1%7D%7Be%7D) 为底数的对数。

纳皮尔费这么大劲搞出来的纳皮尔对数，是要用来化简计算的。纳皮尔大约从1590年就研究纳皮尔对数，花了20多年的时间，在1614年才发表其结果，主要原因是大部分时间都用来计算并制作纳皮尔对数表了。由此可见，那个时代，算数即是一项重要工作，也是一项艰难的工作，今天的我们是很难有切身体会的。

纳皮尔取 ![p_0=10^7](https://www.zhihu.com/equation?tex=p_0%3D10%5E7) ，并由此逐一计算了x从10000000开始，使得纳皮尔对数值y为0、1、2、3、......的一系列x值，形成了纳皮尔对数表。之所以取 ![p_0=10^7](https://www.zhihu.com/equation?tex=p_0%3D10%5E7) ，是因为纳皮尔还深深受到三角函数表的影响，当时的割圆术计算三角函数取值的表中，可以把圆分为 ![10^7](https://www.zhihu.com/equation?tex=10%5E7) 份，计算精度大概也是小数点后6～7位。而且纳皮尔在自己的对数表中，还把 ![\frac{x}{p_0}](https://www.zhihu.com/equation?tex=%5Cfrac%7Bx%7D%7Bp_0%7D) 对应回正弦函数。按照纳皮尔的工作，我大概整理了纳皮尔对数表的示例如下。
![](https://pic1.zhimg.com/v2-aa9e9db83f7c9118004b12f083ea1c6c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='745' height='631'></svg>)
纳皮尔费尽心血整理的对数表，可以用来简化乘积开方运算。比如上图中，如果需要计算 ![\sqrt{8184478.5\times 2135938.9}](https://www.zhihu.com/equation?tex=%5Csqrt%7B8184478.5%5Ctimes+2135938.9%7D) ，那么就可以找到这两个数的纳皮尔对数，分别是2003456和15436788，然后将这两个纳皮尔对数求和再除以二，得到(2003456+15436788)/2=8720122。之后再去纳皮尔对数表中找到纳皮尔对数为8720122对应的x，得到4181093.8，这个数字就是要计算的开方结果，和我们用计算器计算得到的开方结果4181093.8765非常接近。

至于为什么是这样，根据上面得到的纳皮尔对数的现代数学关系，学过指数和对数的中学生应该就可以推导得出了，我就不再赘述了。

当然，与纳皮尔同时期的一位瑞士的教师比尔吉（Joost Bürgi，1552-1632）也曾经（实际上可能更早些）制作出了对数表。不过，他的成果是在1620年发表的，比纳皮尔晚了6年。不像纳皮尔，比尔吉是通过代数的方法得出对数关系的。

关于谁才是对数的发明人，数学史学家们有过一些争论，但是现在主流的观点认为，纳皮尔正式发表成果在先，而且纳皮尔的著作传播得更广，纳皮尔对数的概念也更加深刻一些，因此，公认纳皮尔为对数的发明人。当然，也有认为纳皮尔和比尔吉都是对数的发明人的。

**三、e在哪里？是如何出现并逐步确认的呢？**

有朋友会问了，你说了这半天，e到底在哪里呢？

其实纳皮尔在手工计算对数的时候，所用到底数（当然，那时候完全没有底数这个概念）就是 ![(1-10^{-7})^{10^{7}}](https://www.zhihu.com/equation?tex=%281-10%5E%7B-7%7D%29%5E%7B10%5E%7B7%7D%7D) ，有了现代数学概念，我们很容易知道这个数非常接近1/e。事实上，前面已经介绍了，本质上纳皮尔对数就是以1/e为底数的一种对数。只不过纳皮尔还没有清楚的认识到伟大的“e”。

比尔吉在他的对数表中所涉及到的底数是 ![(1+10^{-4})^{10^4}](https://www.zhihu.com/equation?tex=%281%2B10%5E%7B-4%7D%29%5E%7B10%5E4%7D) ，这个数字非常接近e了。当然，比尔吉也没有认识到伟大的“e”。

所以，e被人们认识并不是一蹴而就的。

当然，考究历史非常困难，我们今天很难确定“e”被人们认识的准确过程。

首先，在1665-1668年，大科学家牛顿、尼古拉斯·麦卡托分别独立得到了e的无穷级数，也即 ![e=\frac{1}{0!}+\frac{1}{1!}+\frac{1}{2!}+...](https://www.zhihu.com/equation?tex=e%3D%5Cfrac%7B1%7D%7B0%21%7D%2B%5Cfrac%7B1%7D%7B1%21%7D%2B%5Cfrac%7B1%7D%7B2%21%7D%2B...) （当时还没有明确地用字母e来表示这个数字）。麦卡托还在1668年出版的《Logarithmotechnia》（《对数术》）中提到了“自然对数”这个名字。

其次，在卡约里的《数学符号史》中提到，1690-1691年间，莱布尼兹给惠更斯的信中提到了今天e这个常数，不过当时莱布尼兹使用的字母是b。这说明当时e的表示方式尚未得到确定，大家各自用自己想用的字母来表示e。

之后，在大数学家欧拉的1727-1728年手稿中，专门使用了字母e表示了这个常数，并且给出了这个常数的数值2.7182817...。在1731年11月25日，欧拉写给哥德巴赫的信中，又一次明确提到了e，并且指出e是使双曲对数（就是今天的自然对数）值为1的那个数（“e denotes that number whose hyperbolic logarithm is = 1.”）

到了1742年，终于由英国数学家琼斯给出了实数范围内对数的定义，这也正是我们今天关于对数的定义：已知a是不等于1的正数，如果a的b次幂等于N，那么b叫做以a为底的N的对数。

从上述历史过程可以看到，e被人们认识是伴随着对数被人们日益清楚的认识而自然而然发生的。历史上人们至少从两个角度不断推进对e的认识的。

（一）在制作对数表的过程中更加深入认识e

可能有朋友问，问什么纳皮尔要选择 ![p_0=10^7](https://www.zhihu.com/equation?tex=p_0%3D10%5E7) 这么大呢？这是因为如果选择太小的 ![p_0](https://www.zhihu.com/equation?tex=p_0) ，那么制作出来的对数表的数据密度就会很低，很多数字从中找不到，不能很好的发挥计算工具的作用。

比如，如果选择2为对数的底数，那么对数值为1-10这10个数字的时候，对应的指数原值就从 ![2^0=1](https://www.zhihu.com/equation?tex=2%5E0%3D1) 快速增长到 ![2^{10}=1024](https://www.zhihu.com/equation?tex=2%5E%7B10%7D%3D1024) ，那么如果希望用到798这样的数字，就找不到接近的对数原值了。

因此，选择对数的底数制作对数表的时候，理想情况是选择一个比1稍大一点点的数。后来，人们在制作对数表的时候，就越来越倾向于选择 ![1+\frac{1}{10^{n}}](https://www.zhihu.com/equation?tex=1%2B%5Cfrac%7B1%7D%7B10%5E%7Bn%7D%7D) 这样的底数。n选择的越大，数据密度（某种意义上也体现了计算精度）就越大，利用价值就越大。

于是，就必然出现 ![y=\log_{(1+10^{-n})}{x}](https://www.zhihu.com/equation?tex=y%3D%5Clog_%7B%281%2B10%5E%7B-n%7D%29%7D%7Bx%7D) 的对数。当y取到 ![10^n](https://www.zhihu.com/equation?tex=10%5En) 时，反推出来的x就会等于 ![(1+\frac{1}{10^n})^{10^n}](https://www.zhihu.com/equation?tex=%281%2B%5Cfrac%7B1%7D%7B10%5En%7D%29%5E%7B10%5En%7D) 。人们自然就会发现，随着n不断增加，这个数越来越趋向于一个确定的值，从而认识到这个数列存在极限，也就是e。

（二）为了使微分或求导更加方便而认识e

另一个角度是在研究对数函数的微分时候认识到e的。

令 ![y=\log_{a}{x}](https://www.zhihu.com/equation?tex=y%3D%5Clog_%7Ba%7D%7Bx%7D) ，当我们求y的导数的时候，会得到

![\frac{dy}{dx}](https://www.zhihu.com/equation?tex=%5Cfrac%7Bdy%7D%7Bdx%7D)

![=\lim_{\Delta x \rightarrow 0}{\frac{\log_{a}{(x+\Delta x)}-\log_{a}{x}}{\Delta x}}](https://www.zhihu.com/equation?tex=%3D%5Clim_%7B%5CDelta+x+%5Crightarrow+0%7D%7B%5Cfrac%7B%5Clog_%7Ba%7D%7B%28x%2B%5CDelta+x%29%7D-%5Clog_%7Ba%7D%7Bx%7D%7D%7B%5CDelta+x%7D%7D)

![=\lim_{\Delta x \rightarrow 0}{\frac{1}{\Delta x}}\log_{a}{\frac{x+\Delta x}{x}}](https://www.zhihu.com/equation?tex=%3D%5Clim_%7B%5CDelta+x+%5Crightarrow+0%7D%7B%5Cfrac%7B1%7D%7B%5CDelta+x%7D%7D%5Clog_%7Ba%7D%7B%5Cfrac%7Bx%2B%5CDelta+x%7D%7Bx%7D%7D)

![=\lim_{\Delta x \rightarrow 0}{\log_{a}{(1+\frac{\Delta x}{x})^{\frac{1}{\Delta x}}}}](https://www.zhihu.com/equation?tex=%3D%5Clim_%7B%5CDelta+x+%5Crightarrow+0%7D%7B%5Clog_%7Ba%7D%7B%281%2B%5Cfrac%7B%5CDelta+x%7D%7Bx%7D%29%5E%7B%5Cfrac%7B1%7D%7B%5CDelta+x%7D%7D%7D%7D)

![=\lim_{\Delta x \rightarrow 0}{\frac{1}{x}\log_{a}{(1+\frac{\Delta x}{x})^{\frac{x}{\Delta x}}}}](https://www.zhihu.com/equation?tex=%3D%5Clim_%7B%5CDelta+x+%5Crightarrow+0%7D%7B%5Cfrac%7B1%7D%7Bx%7D%5Clog_%7Ba%7D%7B%281%2B%5Cfrac%7B%5CDelta+x%7D%7Bx%7D%29%5E%7B%5Cfrac%7Bx%7D%7B%5CDelta+x%7D%7D%7D%7D)

![=\frac{1}{x}\log_a{\lim_{\Delta x \rightarrow 0}{(1+\frac{\Delta x}{x})^{\frac{x}{\Delta x}}}}](https://www.zhihu.com/equation?tex=%3D%5Cfrac%7B1%7D%7Bx%7D%5Clog_a%7B%5Clim_%7B%5CDelta+x+%5Crightarrow+0%7D%7B%281%2B%5Cfrac%7B%5CDelta+x%7D%7Bx%7D%29%5E%7B%5Cfrac%7Bx%7D%7B%5CDelta+x%7D%7D%7D%7D)

于是，又一次出现了类似的极限 ![\lim_{\Delta x \rightarrow 0}{(1+\frac{\Delta x}{x})^{\frac{x}{\Delta x}}}=\lim_{n \rightarrow \infty }{(1+\frac{1}{n})^{n}}](https://www.zhihu.com/equation?tex=%5Clim_%7B%5CDelta+x+%5Crightarrow+0%7D%7B%281%2B%5Cfrac%7B%5CDelta+x%7D%7Bx%7D%29%5E%7B%5Cfrac%7Bx%7D%7B%5CDelta+x%7D%7D%7D%3D%5Clim_%7Bn+%5Crightarrow+%5Cinfty+%7D%7B%281%2B%5Cfrac%7B1%7D%7Bn%7D%29%5E%7Bn%7D%7D) 。

当然，大家都知道，这个极限就是e。因此，如果把对数的底数a取为e的时候，就会得到最简洁、自然的形式， ![(\log_e x)'=\frac{1}{x}](https://www.zhihu.com/equation?tex=%28%5Clog_e+x%29%27%3D%5Cfrac%7B1%7D%7Bx%7D) 。于是，人们把 ![log_e x](https://www.zhihu.com/equation?tex=log_e+x) 定义为 ![\ln x](https://www.zhihu.com/equation?tex=%5Cln+x) ，并取名叫做自然对数，因为这样取底数得到的导数最简洁、最自然。

无论通过哪个角度，人们最终认识到了自然对数的底数——e。随着数学不断发展，人们日益发现e的身影无处不在，e的作用伟大而神奇。终于，e在人类认识到的各种常数中脱颖而出，成为了和圆周率 ![\pi](https://www.zhihu.com/equation?tex=%5Cpi) 齐名的伟大的数学常数！

**四、尾声——对数发现的特殊性、e的极限的证明、e的广泛存在**

（一）对数发现的特殊性

对数的发现过程中，最奇怪的一点就是，当时欧洲的代数学还十分“落后”（指相对于现在），连指数、底数这些基本的概念都还没有建立，因此，人们根本不是基于 ![a^x=b](https://www.zhihu.com/equation?tex=a%5Ex%3Db) 这样的代数关系发现对数的。事实上，纳皮尔是从几何运动的角度发现了对数关系的；比尔吉是从代数的级数对应角度发现对数关系的。

我们今天很容易理解的对数，初中学生就已经开始学习的对数，在那个年代是非常深奥、复杂的数学概念和理论。有数学史学家曾经指出“对数的发现早于指数的应用这个事实，是数学史上的反常现象之一。”

纪念纳皮尔的文集的序言中写道“这项发明是孤立的，它没有借助其他智力工作，也没有遵循原有的数学思想路线，就突然闯到人类思想中来了。”

（二）关于e的极限存在的证明

作为一篇数学科普文章，既然提到了e的极限定义公式，如果不给出些证明，似乎不太够意思。下面，提供一个相对巧妙的方法，证明 ![\lim_{n \rightarrow \infty}{(1+\frac{1}{n})^n}](https://www.zhihu.com/equation?tex=%5Clim_%7Bn+%5Crightarrow+%5Cinfty%7D%7B%281%2B%5Cfrac%7B1%7D%7Bn%7D%29%5En%7D) 存在。因为只有这个极限存在，才能定义其为常数e。

> 第一步，我们证明一个不等式，
对于任意满足b>a>0的实数a和b，不等式 ![b^n-a^n<(b-a)nb^{n-1}](https://www.zhihu.com/equation?tex=b%5En-a%5En%3C%28b-a%29nb%5E%7Bn-1%7D) 成立。
这是因为 ![b^n-a^n=(b-a)(b^{n-1}+b^{n-2}a+b^{n-3}a^2+...+b\cdot a^{n-2}+a^{n-1})](https://www.zhihu.com/equation?tex=b%5En-a%5En%3D%28b-a%29%28b%5E%7Bn-1%7D%2Bb%5E%7Bn-2%7Da%2Bb%5E%7Bn-3%7Da%5E2%2B...%2Bb%5Ccdot+a%5E%7Bn-2%7D%2Ba%5E%7Bn-1%7D%29)
又因为a<b，所以将上式中第二个括号内的a全部换成b，必会使结果变大，从而待证明不等式成立。
再将此不等式整理为 ![a^n>b^{n-1}[ b-(b-a)n ]](https://www.zhihu.com/equation?tex=a%5En%3Eb%5E%7Bn-1%7D%5B+b-%28b-a%29n+%5D) ...... （2）

第二步，设整数n>1，令 ![a=1+\frac{1}{n}](https://www.zhihu.com/equation?tex=a%3D1%2B%5Cfrac%7B1%7D%7Bn%7D) ， ![b=1+\frac{1}{n-1}](https://www.zhihu.com/equation?tex=b%3D1%2B%5Cfrac%7B1%7D%7Bn-1%7D) ，此时仍满足b>a>0的前提条件，则式（2）仍成立。将其带入式（2），得到
![(1+\frac{1}{n})^n>(1+\frac{1}{n-1})^{n-1}](https://www.zhihu.com/equation?tex=%281%2B%5Cfrac%7B1%7D%7Bn%7D%29%5En%3E%281%2B%5Cfrac%7B1%7D%7Bn-1%7D%29%5E%7Bn-1%7D)
这说明 ![(1+\frac{1}{n})^n](https://www.zhihu.com/equation?tex=%281%2B%5Cfrac%7B1%7D%7Bn%7D%29%5En) 随着n单调递增。

第三步，令 ![a=1](https://www.zhihu.com/equation?tex=a%3D1) ，![b=1+\frac{1}{2(n-1)}](https://www.zhihu.com/equation?tex=b%3D1%2B%5Cfrac%7B1%7D%7B2%28n-1%29%7D)，带入式（2），得到
![2>(1+\frac{1}{2(n-1)})^{n-1}](https://www.zhihu.com/equation?tex=2%3E%281%2B%5Cfrac%7B1%7D%7B2%28n-1%29%7D%29%5E%7Bn-1%7D) ，两边平方，得到
![4>(1+\frac{1}{2(n-1)})^{2(n-1)}](https://www.zhihu.com/equation?tex=4%3E%281%2B%5Cfrac%7B1%7D%7B2%28n-1%29%7D%29%5E%7B2%28n-1%29%7D)
因为n是大于1的任意整数，说明此数列有上界。

单调递增数列有上界，则极限必存在。

由此，我们可以定义![e=\lim_{n \rightarrow \infty}{(1+\frac{1}{n})^n}](https://www.zhihu.com/equation?tex=e%3D%5Clim_%7Bn+%5Crightarrow+%5Cinfty%7D%7B%281%2B%5Cfrac%7B1%7D%7Bn%7D%29%5En%7D)。

（三）与e有关的各种数学定理、公式

与e有关的数学定理、公式太多了，可以说多如牛毛、数不胜数。这也是为什么e已经成为科学各学科领域中最重要的常数之一了。

1、欧拉公式

![e^{i\pi}+1=0](https://www.zhihu.com/equation?tex=e%5E%7Bi%5Cpi%7D%2B1%3D0) ，欧拉公式，号称最优美数学恒等式，它将e、 ![\pi](https://www.zhihu.com/equation?tex=%5Cpi) 、i、1和0组合在了一起，简洁、优美，含义深刻。

2、素数定理

![\lim_{x \rightarrow \infty}{\frac{\pi (x)}{x/ln\ x}}=1](https://www.zhihu.com/equation?tex=%5Clim_%7Bx+%5Crightarrow+%5Cinfty%7D%7B%5Cfrac%7B%5Cpi+%28x%29%7D%7Bx%2Fln%5C+x%7D%7D%3D1) ，或者 ![\lim_{x \rightarrow \infty}{\frac{\pi (x)}{\int_{2}^{x}\frac{dx}{ln\ x}}}=1](https://www.zhihu.com/equation?tex=%5Clim_%7Bx+%5Crightarrow+%5Cinfty%7D%7B%5Cfrac%7B%5Cpi+%28x%29%7D%7B%5Cint_%7B2%7D%5E%7Bx%7D%5Cfrac%7Bdx%7D%7Bln%5C+x%7D%7D%7D%3D1) 。这两个式子是等价的， ![\pi(x)](https://www.zhihu.com/equation?tex=%5Cpi%28x%29) 是小于等于x的素数的个数。这个公式中虽然没有显式出现e，但是出现了ln，其实就是隐式的出现了e。素数和e的这种联系很奇特，要知道素数是整数范畴的概念，属于离散数学，而e是分析范畴，属于极限和连续领域。它们之间居然有这么紧密的联系，很不寻常。

3、高斯正态分布

正态分布的概率密度 ![f(x)=(\sqrt{2\pi}\cdot\sigma)^{-1}\cdot e^{-\frac{(x-a)^2}{2\sigma^2}}](https://www.zhihu.com/equation?tex=f%28x%29%3D%28%5Csqrt%7B2%5Cpi%7D%5Ccdot%5Csigma%29%5E%7B-1%7D%5Ccdot+e%5E%7B-%5Cfrac%7B%28x-a%29%5E2%7D%7B2%5Csigma%5E2%7D%7D) ，其中a是正态分布的平均值， ![\sigma](https://www.zhihu.com/equation?tex=%5Csigma) 是标准差， ![\sigma^2](https://www.zhihu.com/equation?tex=%5Csigma%5E2) 是方差。正态分布用处太广泛了，而且根据中心极限定理，任何大量的独立变量之和都趋于正态分布。这里面e当仁不让的占据着核心地位。

除了数学领域，物理学领域也有大量的公式和定律中出现e。例如麦克斯韦速率分布定律、气体在重力场中的玻尔兹曼分布、布朗运动规律、放射性元素衰变等等等等。




e是一个美妙而神奇的常数，而且是不容易被发现和认识到的常数。感谢历史上诸多伟大的数学家，使我们了解了这样一个神奇的常数，并且推动着科学不断向前发展。




