# 【学界】黑盒子凸优化，中心法和Khachiyan常数猜想 - 知乎
# 

> 作者： [@周君豹](https://www.zhihu.com/people/610df0a431b9fc570463e2449c3433c3)，北京大学在读博士，熟悉优化理论，研究使用优化技术解决无线通信中的波束成形和资源分配问题，对信息论和极化码感兴趣。

责任编辑：@乌鸦(NUAA研究生)
本篇文章是作者周君豹在知乎上的优秀文章（原文链接：黑盒子凸优化，中心法和Khachiyan常数猜想：[https://zhuanlan.zhihu.com/p/27668621](https://zhuanlan.zhihu.com/p/27668621)），通过【运筹OR帷幄】责任编辑整理而成。

敬请关注和扩散本专栏及同名公众号，会邀请**全球知名学者**陆续发布运筹学、人工智能中优化理论等相关干货、[知乎Live](https://www.zhihu.com/lives/users/961e8cc4f7512fda1ea6626ce9a05e8e)及行业动态：
[『运筹OR帷幄』大数据人工智能时代的运筹学](https://zhuanlan.zhihu.com/operations-research)

最近很迷凸几何(convex geometry)，以及基于几何直觉的许多凸优化算法，写篇文章记录下一些印象比较深刻的内容。

**1. 凸优化理论的简史**

在90年代苏联解体之前，凸优化理论的发展体现了深深的东西方冷战的烙印。苏联优化专家发展了凸优化的几何算法，比如N.Z. Shor的 次梯度法（Subgradient method），B.T Polyak的胖球法(Heavy-ball method )， A. Nemirovski 和 D.B. Yudin 的外接椭球法(circumscribed ellipsoid method),L.G. Khachiyan的内接椭球法(inscribed ellipsoid method); 而欧美优化专家发展了凸优化的分析理论，比如J.J. Moreau的邻近算子理论（proximal operator theory）， W. Fenchel的共轭分析和对偶理论(conjugate analysis, conjugate duality), 单调算子理论（monotone operator theory)，标志性的成果是R.T. Rockafellar的经典不朽的集大成之作《*Convex analysis》。*从名称上就可以看出两大阵营粗暴地分成了几何学派和分析学派。苏联解体后，大量苏联专家移居欧美，这两类专家有机会频繁交流和合作，此后的许多结果通常体现了几何和分析的深度融合，比如90年代成熟的内点算法理论，里面既有self-concordant function之类的分析概念，也有central path之类的几何概念。

这篇文章的主要内容就是总结记录几何学派的一些研究成果。苏联解体之后，很多苏联学术刊物都被撤销了，与此同时，很多经典论文目前在网上也很难找到，比如我最近一直想读 L.G. Khachiyan的论文The method of inscribed ellipsoids的全文，然而我死活都下载不到了，如果有好心人士能找到，烦请分享给学术界。所以，这篇文章的另外一个目的就是把几何学派的经典成果分享给大家。

**2. 黑盒子凸优化(Black-box convex optimization)**

黑盒子凸优化是与结构凸优化（Structural convex optimization）相对而言的。结构凸优化通常是指线性规划，二次规划，锥规划之类有具体结构特征的优化问题，从Karmarkar开启内点算法革命以来，结构凸优化就一直是凸优化算法设计的主流研究。而黑盒子优化则可以看作是70年代发展起来的计算机学科的计算复杂度理论在当时渗透和影响数值优化的结果，主要研究具体算法的效率问题，即最坏情况下的时间复杂度，迭代复杂度之类，以及具体某类问题的最佳算法及其复杂度。通常前一个问题称之为方法效率分析（method efficiency），后一个问题称之为问题复杂度分析（problem complexity）[1]。

举个例子，两个人玩猜数字游戏。游戏规则是：首先A在给定范围内，比如0到1000之间，任意出一个数字；B按照某种策略每次猜一个数字，如果猜中，游戏结束，否则A告知B正确数字比当前才的数字大或者小。现在有两类问题可以研究，（1）对于具体某种策略，比如每次猜的数字加10或减10，最坏情况下，需要猜多少次？（2）对于这个问题，最佳策略是什么？与此对应，最坏情况下，需要猜多少次？这个问题表述和最佳策略（二分法）对黑盒子凸优化都有重要的启示。

方便起见，只考虑凸可行问题（convex feasibility problem）。具体而言，黑盒子凸优化的表述是：A在已知的给定范围，比如宽度为L的N维超立方体(hypercube)，任意给一个“不太小”的N维凸体![K_0](https://www.zhihu.com/equation?tex=K_0)（convex body）。这里凸体是指实心的紧凸集:(solid compact convex set), “不太小”是指这个凸体体积满足![\text{Vol} (K_0)\ge \epsilon](https://www.zhihu.com/equation?tex=%5Ctext%7BVol%7D+%28K_0%29%5Cge+%5Cepsilon)。B每次猜一个点，如果点在凸体![K_0](https://www.zhihu.com/equation?tex=K_0)里，则结束，否则A给出一个通过该猜测点的超平面（hyperplane），及其对应的包含凸体![K_0](https://www.zhihu.com/equation?tex=K_0)的半空间（half space）。这个超平面通常称之为分离超平面（separation hyperplane）或者切平面（cutting hyperplane），因为它将该猜测点和凸体分离开来。现在的问题是，应该按照怎样的策略选择猜测点？最坏情况下，需要猜多少次？类比计算机程序，A是一个封装起来的子函数，对于函数B而言，A就是一个黑盒子。由于B是通过序贯地方式调用A, B可以依次把每次调用A的输出信息积累起来，然后综合判断，所以猜测技巧就在于如何积累信息，如何综合判断。简单起见，只考虑概念算法，也就是假定B有无穷的计算能力，并且忽略B每次综合处理的代价开销。

**3. 中心法**

注意到一个基本事实：不管B采用何种策略去猜测，B每次积累起来的半空间信息和已知范围![D](https://www.zhihu.com/equation?tex=D)的交集依然是一个凸体，我们称之为已知范围凸体![K](https://www.zhihu.com/equation?tex=K)，真实凸体![K_0](https://www.zhihu.com/equation?tex=K_0)必在![K](https://www.zhihu.com/equation?tex=K)内。当然，每次猜测之后，![K](https://www.zhihu.com/equation?tex=K)都会更新。【实际上，![K](https://www.zhihu.com/equation?tex=K)是有限个半空间的交集，所以![K](https://www.zhihu.com/equation?tex=K)是多胞体（polytope），也就是只有有限个面(facet)和有限个端点(extreme point)。】由此而来，最直觉的策略是，猜测![K](https://www.zhihu.com/equation?tex=K)的“**中心**”！

然而，问题是，什么是一个凸体![K](https://www.zhihu.com/equation?tex=K)的中心？以二维平面为例，对于圆，圆心可以当作中心；然而，对于最简单的缺乏对称性的凸体，比如不规则的三角形，就有重心，垂心，外心，内心，旁心之类的各种“中心”。所以，虽然“**中心**”是最简单的直觉，然而在没有明确地定义凸体的中心之前，“**中心法”**的说法毫无意义。容易忽略的一点是，即便给定”中心“的某种确切含义，当我们说"猜测中心"的时候，已经无意识地认定凸体的中心是唯一的。然而唯一性并未得到澄清，如果当中心不唯一时，"猜测中心"的说法依然是含义模糊的。

凸优化历史上，提出了多种凸体中心的概念，比如重心（centroid，center of gravity），外接椭球中心（circumcenter，center of circumscribed ellipsoid）, 内接椭球中心（incenter，center of inscribed ellipsoid），切比雪夫中心（Chebyshev center，center of inscribed ball）,单纯形中心（center of circumscribed simplex），分析中心（analytic center），体积中心（volumetric center）等。前五个中心是具有几何意义的，并且是唯一的；而后两个中心则依赖于代数表示，更加具体地说，同一个凸体，通过冗余的代数表示（比如同一个不等式重复多次），凸体的任意内点都可以成为分析中心或者体积中心。接下来将会分析猜测重心，外接椭球中心, 内接椭球中心的策略的效率。

**3.1 重心法**

1965年，苏联A.Y. Levin [3]和美国D.J. Newman [4]独立提出了重心法。对于凸体，其重心定义为![\vec{c}_g=\frac{1}{\text{Vol} (K)} \int_K \vec{x}dx_1\dots dx_n](https://www.zhihu.com/equation?tex=%5Cvec%7Bc%7D_g%3D%5Cfrac%7B1%7D%7B%5Ctext%7BVol%7D+%28K%29%7D+%5Cint_K+%5Cvec%7Bx%7Ddx_1%5Cdots+dx_n).重心法的策略就是每次猜测已知范围凸体![K](https://www.zhihu.com/equation?tex=K)的中心，返回的超平面将![K](https://www.zhihu.com/equation?tex=K)分为两个子凸体![K^+](https://www.zhihu.com/equation?tex=K%5E%2B)和![K^-](https://www.zhihu.com/equation?tex=K%5E-),不失一般性，将包含真实凸体![K_0](https://www.zhihu.com/equation?tex=K_0)的子凸体标记为![K^+](https://www.zhihu.com/equation?tex=K%5E%2B)。这个策略的有效性由美国B. Grünbaum [5]和苏联B. S.  Mityagin [6] 独立证明的凸体体积不等式保证：

  对于通过![n](https://www.zhihu.com/equation?tex=n)维凸体![K](https://www.zhihu.com/equation?tex=K)的重心![\vec{c}_g](https://www.zhihu.com/equation?tex=%5Cvec%7Bc%7D_g)的任意超平面及其生成的子凸体![K^+](https://www.zhihu.com/equation?tex=K%5E%2B)，有![\left(\frac{n}{n+1}\right)^n\le \frac{\text{Vol} (K^+)}{\text{Vol} (K)} \le 1-\left(\frac{n}{n+1}\right)^n \le 1-\frac{1}{e}](https://www.zhihu.com/equation?tex=%5Cleft%28%5Cfrac%7Bn%7D%7Bn%2B1%7D%5Cright%29%5En%5Cle+%5Cfrac%7B%5Ctext%7BVol%7D+%28K%5E%2B%29%7D%7B%5Ctext%7BVol%7D+%28K%29%7D+%5Cle+1-%5Cleft%28%5Cfrac%7Bn%7D%7Bn%2B1%7D%5Cright%29%5En+%5Cle+1-%5Cfrac%7B1%7D%7Be%7D).

所以，每猜测一次，已知范围的体积至少按照几何级数衰减，也就是线性收敛。更进一步，A. Nemirovski和D.B. Yudin [2] 证明，除去常数因子，线性收敛是最优的。

**3.2 外接椭球法**
1976年，A. Nemirovski和D.B. Yudin [7]提出了外接椭球法，通常大家都直接称之为椭球法。外接椭球法的策略是，每次猜测前，先构建外接于已知范围凸体![K](https://www.zhihu.com/equation?tex=K)的一个体积最小的椭球，然后猜测这个外接椭球的中心。这个策略的有效性由外接椭球体积不等式保证[8,Lemma 2.3]：

   对于![n](https://www.zhihu.com/equation?tex=n)维欧式空间中的任意椭球![W](https://www.zhihu.com/equation?tex=W)和通过椭球中心的任意超平面切割成的半椭球![\bar{W}](https://www.zhihu.com/equation?tex=%5Cbar%7BW%7D),包含半椭球![\bar{W}](https://www.zhihu.com/equation?tex=%5Cbar%7BW%7D)的体积最小的椭球![W^+](https://www.zhihu.com/equation?tex=W%5E%2B)满足![\frac{\text{Vol} (W^+)}{\text{Vol} (W)} =\frac{n}{n+1}\left(\frac{n}{\sqrt{n^2-1}}\right)^{n-1} \le \exp{\left(-\frac{1}{2n}\right)}](https://www.zhihu.com/equation?tex=%5Cfrac%7B%5Ctext%7BVol%7D+%28W%5E%2B%29%7D%7B%5Ctext%7BVol%7D+%28W%29%7D+%3D%5Cfrac%7Bn%7D%7Bn%2B1%7D%5Cleft%28%5Cfrac%7Bn%7D%7B%5Csqrt%7Bn%5E2-1%7D%7D%5Cright%29%5E%7Bn-1%7D+%5Cle+%5Cexp%7B%5Cleft%28-%5Cfrac%7B1%7D%7B2n%7D%5Cright%29%7D).

所以，外接椭球体积也是按照几何级数收敛的。然而，不幸的是，这个收敛因子是依赖于维度![n](https://www.zhihu.com/equation?tex=n)的。这个结果意味着，对于高维问题，外接椭球法的收敛会非常慢。这个外接椭球法有非常强的理论意义，一个直接推断就是如果一个凸问题的次梯度是多项式时间可计算的，那么这个凸问题可以在多项式时间内近似求解到任意给定精度的。更进一步，L.G. Khachiyan在1979年用此方法证明有理数据的线性规划可以在多项式时间内求得准确解[9][10]。

**3.3 内接椭球法**

1988年，S.P.Tarasov, L.G. Khachiyan和I.I.Èrlikh [1] 提出了内接椭球法。内接椭球法的策略是，每次猜测前，先构建内接于已知范围凸体![K](https://www.zhihu.com/equation?tex=K)的一个体积最大的椭球，然后猜测这个内接椭球的中心。令![v(K)](https://www.zhihu.com/equation?tex=v%28K%29)表示![n](https://www.zhihu.com/equation?tex=n)维凸体![K](https://www.zhihu.com/equation?tex=K)的所有内接椭球中体积最大的椭球的体积，通过该最大内接椭球中心的任意超平面切割成的子凸体记为![K^+](https://www.zhihu.com/equation?tex=K%5E%2B)，![v(K^+)](https://www.zhihu.com/equation?tex=v%28K%5E%2B%29)的含义与![v(K)](https://www.zhihu.com/equation?tex=v%28K%29)类似。内接椭球法策略的有效性由下述不等式保证：

存在常数![r<1](https://www.zhihu.com/equation?tex=r%3C1),使得对任意![n](https://www.zhihu.com/equation?tex=n)维凸体![K](https://www.zhihu.com/equation?tex=K)和对应的子凸体![K^+](https://www.zhihu.com/equation?tex=K%5E%2B)，有 ![\frac{v (K^+)}{v(K)} \le r](https://www.zhihu.com/equation?tex=%5Cfrac%7Bv+%28K%5E%2B%29%7D%7Bv%28K%29%7D+%5Cle+r)。

虽然现在难以阅读[1]原文，幸运的是，[11]提到[1]给出了一个常数![r=\frac{8}{9}=0.888\dots](https://www.zhihu.com/equation?tex=r%3D%5Cfrac%7B8%7D%7B9%7D%3D0.888%5Cdots),并且在其附录中补充了一个证明。显然，内接椭球法也是线性收敛的，与重心法类似，但比外接椭球法要好。从算法实现性角度考虑，内接椭球法也是多项式时间可任意近似的[13][14][15]，这与重心法截然不同。

**4. Khachiyan常数猜想**

对于内接椭球法的收敛因子，L.G. Khachiyan在[12]中证明了一个更好的结果![r=\max_{x>0} \frac{1}{x}\cdot \exp\left(2-\frac{\sqrt{4x+1}}{x}\right) =0.844 \dots](https://www.zhihu.com/equation?tex=r%3D%5Cmax_%7Bx%3E0%7D+%5Cfrac%7B1%7D%7Bx%7D%5Ccdot+%5Cexp%5Cleft%282-%5Cfrac%7B%5Csqrt%7B4x%2B1%7D%7D%7Bx%7D%5Cright%29+%3D0.844+%5Cdots).并且，他猜想，最佳常数是![r=0.5\sqrt{e}=0.824\dots](https://www.zhihu.com/equation?tex=r%3D0.5%5Csqrt%7Be%7D%3D0.824%5Cdots).

顶级优化大师L.G. Khachiyan在2005年去世，世界各地的专家们纷纷表示悼念，很多刊物和会议为他出版纪念专辑[16]。以示敬意，不妨将这个常数称之为Khachiyan常数，将这个猜想称之为Khachiyan常数猜想。

对于重心法和外接椭球法，对于任意维度![n](https://www.zhihu.com/equation?tex=n),对应的最优体积收缩因子都有明确的表达式，而我们对内接椭球法的最优体积收缩因子与维度![n](https://www.zhihu.com/equation?tex=n)的关系知之甚少，而![n](https://www.zhihu.com/equation?tex=n)趋于无穷时是否收敛到Khachiyan常数依然是个谜。

**5.参考文献**

[1] Tarasov S P, Khachiyan L G, Èrlikh, I. I. **The method of inscribed ellipsoids**[C]// Dokl. Akad. Nauk SSSR. 1988:1081-1085.

[2] Nemirovski A, Yudin D B. : **Problem complexity and method efficiency in optimization**[M]. Wiley, 1983:455-455.

[3] Levin A Y. **On an algorithm for the minimization of convex functions**[J]. Soviet Mathematics Doklady, 1965, 6.

[4] Newman D J. **Location of the Maximum on Unimodal Surfaces**[J]. ACM, 1965.

[5] Grünbaum B. **Partitions of mass-distributions and of convex bodies by hyperplanes**[J]. Pacific Journal of Mathematics, 1960, 10(4):1257-1261.

[6] Mitjagin B S. **Two inequalities for volumes of convex bodies**[J]. Mathematical Notes of the Academy of Sciences of the Ussr, 1969, 5(1):61-65.

[7] Judin D B, Nemirovskiĭ A S. **Informational complexity and effective methods for the solution of convex extremal problems**[J]. èkonom. i mat. metody, 1976(2):357-369.

[8] Bubeck S. **Convex Optimization: Algorithms and Complexity**[M]. Now Publishers Inc. 2015.

[9] Khachiyan L G. **A polynomial algorithm in linear programming**[J]. Ussr Computational Mathematics & Mathematical Physics, 1979, 20(80):1-3.

[10] Bengt Aspvall, Richard E Stone. **Khachiyan's linear programming algorithm** [J]. Journal of Algorithms, 1979, 1(1):1-13.

[11] Wada T, Fujisaki Y. **Sequential randomized algorithms: A probabilistic cutting plane technique based on maximum volume ellipsoid center**[C]// IEEE International Symposium on Computer-Aided Control System Design. IEEE, 2010:1533-1538.

[12] Khachiyan L G. **An inequality for the volume of inscribed ellipsoids**[J]. Discrete & Computational Geometry, 1990, 5(3):219-222.

[13] Khachiyan L G, Todd M J. **On the complexity of approximating the maximal inscribed ellipsoid for a polytope**[J]. Mathematical Programming, 1993, 61(1-3):137-159.

[14] Anstreicher K M.** Improved Complexity For Maximum Volume Inscribed Ellipsoids**[J]. Siam Journal on Optimization, 2006, 13(2):309--320.

[15] Zhang Y, Gao L. **On Numerical Solution of the Maximum Volume Ellipsoid Problem**[M]. Society for Industrial and Applied Mathematics, 2003.

[16] Boros E, Gurvich V. **Scientific contributions of Leo Khachiyan (a short overview)**[J]. Discrete Applied Mathematics, 2008, 156(11):2232-2240.

*欢迎原链接转发，转载请前往@*[https://www.zhihu.com/people/zhou-long-fei-91/activities](https://www.zhihu.com/people/zhou-long-fei-91/activities)*的主页获取信息，盗版必究。*

以上[『运筹OR帷幄』](https://zhuanlan.zhihu.com/operations-research)专栏所有文章都会同步发送至 [留德华叫兽的头条主页](https://link.zhihu.com/?target=https%3A//www.toutiao.com/c/user/62215957061/%23mid%3D1570625219325954)， 以及同名微信公众号，目前预计受众**10w +**

如果你是运筹学/人工智能硕博或在读，请在下图的公众号后台留言：**“加微信群”**。系统会自动辨认你的关键字，并提示您进一步的加群要求和步骤，邀请您进全球运筹或AI学者群（群内学界、业界大佬云集）。

同时我们有：【**运筹学|优化爱好者**】【**供应链|物流**】【**人工智能**】【**数据科学|分析**】千人QQ群，想入群的小伙伴可以关注下方公众号**点击“加入社区”按钮**，获得入群传送门。

学术界|工业界招聘、征稿等信息**免费发布**，请见下图：
![](https://pic3.zhimg.com/v2-892a7fe19713b890a096fca336a90522_b.jpg)



