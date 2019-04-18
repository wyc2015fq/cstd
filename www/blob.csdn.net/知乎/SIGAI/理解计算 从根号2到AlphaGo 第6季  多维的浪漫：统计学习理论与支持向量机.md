# 理解计算 从根号2到AlphaGo:第6季   多维的浪漫：统计学习理论与支持向量机 - 知乎
# 

## **理解计算 从根号2到AlphaGo:第6季   多维的浪漫：统计学习理论与支持向量机**

本文及其它机器学习、深度学习算法的全面系统讲解可以阅读《机器学习与应用》，清华大学出版社，雷明著，由SIGAI公众号作者倾力打造，自2019年1月出版以来已重印3次。
- [书的购买链接](https://link.zhihu.com/?target=https%3A//item.jd.com/12504554.html)
- [书的勘误，优化，源代码资源](https://link.zhihu.com/?target=http%3A//www.tensorinfinity.com/paper_78.html)

**PDF全文链接：**[理解计算 从根号2到AlphaGo:第6季   多维的浪漫：统计学习理论与支持向量机](https://link.zhihu.com/?target=http%3A//www.tensorinfinity.com/paper_141.html)

## **1 引子**

1884年，英国著名的艺术兼神学家埃德温•A•艾勃特以科幻小说的形式，出版了一本非常有趣的小书《平面国: 一个多维的传奇故事 Flatland: A Romance of Many Dimensions》。他怎么也想不到，这本通俗有趣的小册子将成为他最为著名的著作而流芳百世，这本小说是如此的伟大，以至于必须给他挂上“数学”科幻小说的头衔才行。这本书具有强烈的英国维多利亚时期的风格，英国人的讽刺幽默再一次清晰有力的展现出 “批判现实主义”的写作风格。艾勃特则将这种“批判”借助于描述一种虚构的简单到让人吃惊的世界-平面世界来映射当时的社会现象。

![](https://pic1.zhimg.com/v2-3d9fe574a2599508eb8642972745785c_b.png)图1 平面国(左)及其改编的动画电影

平面国的女人都是线段，男人则是平面上的形状并且具有严格的等级，等级是随着边的数目(或内角和)的增大而晋升的：等边三角形或者等腰三角形是比较低级的士兵和工人，中产阶层是四边形，等级越高就拥有越多的边角，贵族,教士和僧侣则是拥有无穷多个“边”的圆。由于平面国的女人(线段)两头尖的属性导致在平面国中一旦有人不小心碰到她们就会立刻被刺死，因此平面国的法律规定女人在公共场合行走和站立时必须不停的摆动身体，好让其他人看到她们。

以上的描述已经让人惊叹，但是真正让这本书成为经典的则是书的第二部分，一个普通的四边形中产阶级的梦。在梦中可怜的正方形来到了“直线国”， 在一维世界的人看来，二维的四边形就是神，因为他能突然出现又突然消失（移到直线的左边或右边， 直线国的人就看不见你了）。正方形在梦中也遇到了“立体国”的“球体”，并被球体带离平面世界，使他认知立体概念。但等他向自己同胞阐述有三维世界存在时，却被视作异端判处终身监禁。由于《平面国》的内容既通俗又神奇，容易让人忽略一个重要的问题，平面国的人们是二维的吗？事情可没这么简单，他们是存在于二维世界中的意识体，他们那扁平的大脑的运动，让一些人通过脑洞大开(做梦)意识到高维世界存在的可能性。对于存在于三维世界的唯一具有高级智慧的人类，也并不像我们身边的这些无意识的三维物质一样，而是拥有了一种到现在都说不清道不明的神奇能力“意识”。在意识层面，我们和虚构的平面国的人们没有区别。在研究物质世界的科学(物理学)中，为了避免陷于神秘的宗教意识的束缚，物理学家通常排除意识问题或将这个问题推到“上帝”身上。

当《平面国》阅读结束的时候，除去对神奇的“意识”的依赖，依然会有一个可怕的问题接踵而来：“在我们这个三维世界能突然出现或消失的，是神还是四维世界的某种东西呢？”只是在当时，19-20世纪之交的时候，“物理大厦已经落成，所剩只是一些修饰工作。” 物理研究工作中还没有发现第四维东西来预示高维物质的存在。碰巧的是，就在《平面国》出版之后没过多久的1896年，受到伦琴发现X射线的启发，法国物理学家安东尼•亨利•贝克勒尔把铀盐放近被不透光的纸包封着的照相底片时，发现照相底片被曝光。他最终确证铀辐射乃是原子自身的一种天然作用：只要有铀这种元素存在，就不断有这种辐射产生。放射现象的产生具有极其深远的影响，原子的随机放射现象就像一种天启一样，预示着符合常识的以决定论为根基的牛顿运动定律在微观世界的无效性。

![](https://pic3.zhimg.com/v2-aecfb7e9761046fce399f898e0e8ad42_b.png)图2 艾勃特(1838-1926)，贝克勒尔(1852-1908)与希尔伯特(1862～1943)

从此放射现象作为量子时代开始的标志，也将开启高维物质存在性的讨论。更加巧合的是，同一时代德国著名的数学家.大卫•希尔伯特（David Hilbert，1862～1943），他在对积分方程的研究中定义了一种特殊的抽象的数学空间，冯•诺伊曼将其称为 “希尔伯特空间”。希尔伯特空间是欧几里德空间的一个推广，最大的特点在于不再局限于有限维的情形，使量子理论从一开始就幸运地拥有了能够被完美描述的数学工具了。需要强调的是，只有回望过去，我们才将几乎同一时期的《平面国》对高维世界的思考，放射现象的出现以及纯数学的希尔伯特空间这些概念联系在一起。

在本季中我将回到平面国中对维度的思考，仅仅从维度的角度出发，带领大家了解高维与低维的之间的转换技巧，就像平面国中的正方形先生被“直线国”的人看做“异类”一样，我们将看到一种计算模型就是利用到这样的特性，在高维空间构造一个威力强大的工具，解决低维空间中存在的问题，这种思路简直棒极了。如果你看过《平面国》的书或影片，你应该明白三维世界中的意识体对二维世界中意识体的统治力。

## **2 有限数据情况下的学习**

在依靠数据驱动的学科中，数据一直是一个核心资产，这些数据都是在特定的环境中得到的，例如遥感图像或是红外图像，获得数据的成本一般很高。苏联老大哥Viadimir N.Vapnik(弗拉基米尔•万普尼克)等人在20世纪60年代就开始研究有限样本情况下的机器学习问题。苏联解体后，万普尼克于1991至2001年间在AT&T贝尔实验室工作，并和他的同事们一起发明了本季的主角之一-支持向量机，这应该算是是贝尔实验室最后阶段的辉煌成果了。现在Viadimir N.Vapnik就职于Facebook，他再次与贝尔实验室的老同事，卷积神经网络的发明人Yann LeCun成为同事。
![](https://pic1.zhimg.com/v2-5a77c844009ec5e90aea939df22ce79c_b.png)图3 Vapnik

Vapnik做出了两个非凡的贡献。一个是统计学习理论，它奠定了基于数据进行模型学习的理论基础，另一个则是在此基础上提出了支持向量机的机器学习算法，这个算法具有极其巧妙地设计思想。

## **3 统计学习理论**

统计学习理论的问题一般是，给定一堆带标签的训练样本集，训练(学习)出一个分类器(或预测器)，对新的样本进行预测使得预测结果尽可能的接近它的真实标签。为达到这个目的，统计学习理论对这个一般问题进行了约束：假设训练样本集合是从一个未知但固定的分布中独立同分布的抽取出来，学习的目标则是从一个分类器集合中挑选出一个分类器，使得这个分类器的对从同一个分布中随机抽取的样本分类的风险最小。

风险这个词比较模糊，在实际风险定义中会依赖一个所谓的损失函数，损失函数就一个具体的样本而言，指的是模型预测的值与真实值之间差距的度量。对于一个样本 ![(x_{i},y_{i})](https://www.zhihu.com/equation?tex=%28x_%7Bi%7D%2Cy_%7Bi%7D%29) 其中 ![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D) 为真实标记，而 ![f（x_{i}）](https://www.zhihu.com/equation?tex=f%EF%BC%88x_%7Bi%7D%EF%BC%89) 为我们的预测值。使用损失函数 ![L(f(x_{i}),y_{i})](https://www.zhihu.com/equation?tex=L%28f%28x_%7Bi%7D%29%2Cy_%7Bi%7D%29) 来表示真实值和预测值之间的差距。两者差距越小越好，最理想的情况是预测值刚好等于真实值。 我们给出一个最简单的0-1损失函数来说明统计学习理论研究的问题:

![L(f(x),y)=\left\{\begin{matrix} 1,y\neq f(x) & \\  0,y=f(x) &  \end{matrix}\right.](https://www.zhihu.com/equation?tex=L%28f%28x%29%2Cy%29%3D%5Cleft%5C%7B%5Cbegin%7Bmatrix%7D+1%2Cy%5Cneq+f%28x%29+%26+%5C%5C++0%2Cy%3Df%28x%29+%26++%5Cend%7Bmatrix%7D%5Cright.)

其中，y≠f(x)表示分类器分类错误，损失为1，否则，无损失。

训练分类器的数据集总是有限的，而这个数据集服从的分布中采样的样本则是无限的，我们希望利用有限样本构建的分类器在未来无限多个样本的情况下进行分类，这个问题实际上是不可能完成的任务。一个分类器性能的好坏依赖于它对未知的无限样本的分类能力，这个能力被称为“泛性化”能力。泛性化是所有学习问题的核心，不只是机器学习，对我们人类学习也一样。

## **3.1 经验风险与期望风险**

给定一个分类器f，如果用损失表示它的分类(预测)风险的话，它在无限样本的分类能力在统计学上讲就是对f所有样本的期望损失，也称期望风险，从平均意义上看,即:

![R_{exp}(f)=\frac{1}{N}\int_{x\ast y}L(y,f(x))\rho (x.y)dxdy](https://www.zhihu.com/equation?tex=R_%7Bexp%7D%28f%29%3D%5Cfrac%7B1%7D%7BN%7D%5Cint_%7Bx%5Cast+y%7DL%28y%2Cf%28x%29%29%5Crho+%28x.y%29dxdy)

尽管我们的终极目标是希望分类器f(x)的期望风险最小，期望风险可以看作是分类器真实风险最好的表达，但是，由于因为我们无法获得无限的训练样本对(x, y)，以及p(x.y)，所以期望风险无法真正获得。

在样本有限的情况下，期望风险退化成有限个样本的损失的和的平均，即经验风险，这种经验是有限个(n)训练样本带给我们的：

![R_{emp}(f)=\frac{1}{N}\sum_{n=1}^{N}L(y,f(x))](https://www.zhihu.com/equation?tex=R_%7Bemp%7D%28f%29%3D%5Cfrac%7B1%7D%7BN%7D%5Csum_%7Bn%3D1%7D%5E%7BN%7DL%28y%2Cf%28x%29%29)

经验风险最小化即对训练集中的所有样本点损失函数的平均最小化。经验风险越小说明模型f(x)对训练集的拟合程度越好。实际上，只要f足够复杂，我们总能让经验风险降到最低的零。但是这几乎是一种灾难，因为我们的终极目标是建立对其他的几乎无限多个样本的分类能力即期望损失最小。对所有训练样本都完美区分，并不意味着对其他样本的区分，甚至会出现对其他大量未知样本的错误分类，这个问题被称之过拟合，过拟合意味着训练的模型具有较低的泛化能力。

给定f的情况下，经验风险和期望风险之间有如下不同点：

(1)经验风险是局部的，基于训练样本的损失函数最小化，经验风险的优化问题是现实的可求的, 从式子可以看出经验风险仅依赖于数据(x,y)及损失函数L的定义(依赖于数据y和f(x))。

(2)期望风险是全局的，可以看做是真实风险的合理表示。它基于所有样本损失的最小化，期望风险是全局最优，是理想化的不可求的, 从式子可以看出期望风险依赖于数据的联合分布p(x,y)以及损失函数L的定义(依赖于数据y和f(x))

## **3.2 最优分类器与理想的统计学习**

在对于任意的数据集（X,Y）损失函数L以及联合概率分布p(x,y)，那么是否存在一个能将期望风险降到最低的最优分类器？实际上，这个分类器是存在的，即贝叶斯分类器$$f\_{b}$$。它是各种分类器中分类错误概率最小或者在预先给定代价L的情况下平均风险最小的分类器, 它的设计方法是按照贝叶斯决策理论进行设计的一种最优分类器。我在本季中不展开讲解贝叶斯理论，大家只需要知道设计贝叶斯分类器的关键是要知道样本 x的各种后验概率密度函数，这些要求决定了在实际情况下贝叶斯分类器的实际使用范围很有限，因为后验概率密度函数几乎与联合概率密度函数一样复杂且未知。

在前面的例子中我们给定了一个分类器，并了解了这个分类器对应的经验风险和期望(真实)风险。统计学习理论的目标就是在有限n个数据样本的情况下，学习某个分类器 ![f_{n}](https://www.zhihu.com/equation?tex=f_%7Bn%7D) ，同时需要证明fn的性能与与贝叶斯最优分类器 ![f_{b}](https://www.zhihu.com/equation?tex=f_%7Bb%7D) 性能的之间的差距可以通过某种方式逐渐接近。例如在样本数趋于无穷的时候，我们学习的分类器 ![f_{n}](https://www.zhihu.com/equation?tex=f_%7Bn%7D) 的性能如果够无限接近贝叶斯分类器 ![f_{b}](https://www.zhihu.com/equation?tex=f_%7Bb%7D) 的性能。在理论上来说，就是分类器 ![f_{n}](https://www.zhihu.com/equation?tex=f_%7Bn%7D) 在样本无限多时能以任意大的概率和任意小的差距逼近贝叶斯最优分类器的性能，且对联合概率分布p (x,y)没有任何假设，这是我们最理想的统计学习策略。

最优学习策略说明只要不断增加数据，就能使学习得到的 ![f_{n}](https://www.zhihu.com/equation?tex=f_%7Bn%7D) 的分类性能(风险)就能够逐渐接近最优分类器 ![f_{b}](https://www.zhihu.com/equation?tex=f_%7Bb%7D) 的分类性能 (风险)。在这里我们依然采用风险来度量分类器的性能。然而这个目标依然非常严格，达到这个目标非常困难。这是因为，我们的学习算法实际上只是从一个函数空间F(假设空间)中选择某个函数f，但是我们的这个函数空间F很大程度上可能并不包含贝叶斯分类器，那么理论上在这个空间中，(随着样本数目的增加)任何学习算法都不可能获得接近fb性能的分类器，如图所示假设空间2不包含贝叶斯分类器 ![f_{b}](https://www.zhihu.com/equation?tex=f_%7Bb%7D) 。

![](https://pic1.zhimg.com/v2-0827931848e6514655be9d535f43f854_b.png)图4 无法学习到的最优分类器[1]

尽管可能无法学习到贝叶斯分类器，我们依然希望了解采用某种方式学习得来的分类器fn和贝叶斯分类器fb之间的差距到底在哪里？ 为了进一步了解，我们看下面的图：

![](https://pic1.zhimg.com/v2-f1220b9d918aad06cdb486129f407ea4_b.png)图5 学习的分类器与最优分类器的性能差距[1]

橙色点是贝叶斯分类器fb，绿色点是我们的假设空间中最优的分类器fF,， 即可以看作是空间F分类器中使期望风险最低的分类器f ̅  ，灰色点是我们的学习算法在利用n个样本训练得到的分类器fn , fn∈F，此时相较于贝叶斯最优分类器fb，性能差距为：

![R(f_{n})-R(f_{b})=(R(f_{n})-R(f_{F}))+(R(f_{F})-R(f_{b}))](https://www.zhihu.com/equation?tex=R%28f_%7Bn%7D%29-R%28f_%7Bb%7D%29%3D%28R%28f_%7Bn%7D%29-R%28f_%7BF%7D%29%29%2B%28R%28f_%7BF%7D%29-R%28f_%7Bb%7D%29%29)

其中R表示某种风险(Risk)度量标准，需要注意的是这里用分类器风险的差距度量分类器本身的性能接近程度。上边右边第一项$$ ![(R(f_{n})-R(f_{F}))](https://www.zhihu.com/equation?tex=%28R%28f_%7Bn%7D%29-R%28f_%7BF%7D%29%29) $$被称为方差，表示我们学到的分类器fn和假设空间中最优分类器fF的差距，右边第二项 ![R(f_{F})-R(f_{b})](https://www.zhihu.com/equation?tex=R%28f_%7BF%7D%29-R%28f_%7Bb%7D%29) 就是偏差，表示假设空间中最优分类器 ![f_{F}](https://www.zhihu.com/equation?tex=f_%7BF%7D) 与贝叶斯分类器$$f\_{b}$$的差距。

为了降低fn与理论最优分类器的差距 ![R(f_{b})-R(f_{n})](https://www.zhihu.com/equation?tex=R%28f_%7Bb%7D%29-R%28f_%7Bn%7D%29) ，看起来只需要让方差和偏差都降低就可以了。然而，偏差-方差存在一种奇妙的尴尬窘境 (bias-variance dilemma)。为了缩小偏差，我们需要一个更大的假设空间F，而更大的假设空间则意味着更大的方差，甚至有可能导致学习过程不收敛（不管多少样本， ![R(f_{n})](https://www.zhihu.com/equation?tex=R%28f_%7Bn%7D%29) 无法与 ![R(f_{F})](https://www.zhihu.com/equation?tex=R%28f_%7BF%7D%29) 接近）。而为了缩小方差，我们希望空间F能够小一点，而这又往往意味着大的偏差。总而言之，在这个框架下，一般来说实践中无法同时将方差和偏差无限优化，需要在两者间做一个权衡。

现在我们更加严格的利用风险来考虑 ![f_{n}](https://www.zhihu.com/equation?tex=f_%7Bn%7D)  和 ![f_{F}](https://www.zhihu.com/equation?tex=f_%7BF%7D)  以及 ![f_{b}](https://www.zhihu.com/equation?tex=f_%7Bb%7D) 三者之间的关系了。在假设空间F和联合概率分布p(x,y) 给定的情况下，如果随着n逐渐增大， ![f_{n}](https://www.zhihu.com/equation?tex=f_%7Bn%7D) 的风险能够依概率收敛到 ![f_{F}](https://www.zhihu.com/equation?tex=f_%7BF%7D) 的风险(你完全可忽略依概率的含义，只需知道他们得分类性能会以某个度量标准逐渐减小差距)，则表明这个学习算法具有F一致性：

![P(R(f_{n})-R(f_{F})> \varepsilon )\rightarrow 0,when \qquad n\rightarrow \infty ](https://www.zhihu.com/equation?tex=P%28R%28f_%7Bn%7D%29-R%28f_%7BF%7D%29%3E+%5Cvarepsilon+%29%5Crightarrow+0%2Cwhen+%5Cqquad+n%5Crightarrow+%5Cinfty+)

如果随着n逐渐增大， ![f_{n}](https://www.zhihu.com/equation?tex=f_%7Bn%7D) 的风险能够依概率收敛(接近)到 ![f_{b}](https://www.zhihu.com/equation?tex=f_%7Bb%7D) 的风险, 则表明学习算法具有贝叶斯一致性。

![P(R(f_{n})-R(f_{b})> \varepsilon )\rightarrow 0,when \qquad n\rightarrow \infty ](https://www.zhihu.com/equation?tex=P%28R%28f_%7Bn%7D%29-R%28f_%7Bb%7D%29%3E+%5Cvarepsilon+%29%5Crightarrow+0%2Cwhen+%5Cqquad+n%5Crightarrow+%5Cinfty+)

在假设F给定的情况下，如果对任意的概率分布p(x,y)，学习算法均能够贝叶斯一致，那么这个学习算法具有全局一致性。

随着前提条件越来越放松，一致性的要求越来越高。别说贝叶斯一致了，就是在假设空间F中一致性实际上也并不容易实现。全局一致的算法看起来更难找到。神奇的是，在1977年科学家们证明了一种最简单的K近邻算法(K-Nearest Neighbor,KNN)在K为无穷大时，全局一致。这个算法非常简单，如果一个实例在特征空间中的K个最相似（即特征空间中最近邻）的实例中的大多数属于某一个类别，则该实例也属于这个类别。这也从侧面证明了这个简单算法的强大之处，但是K为无穷大时的全局一致性只能是一种理论存在。

## **3.3 经验风险学习的性能**

尽管我们介绍了 ![f_{n}](https://www.zhihu.com/equation?tex=f_%7Bn%7D) 和 ![f_{F}](https://www.zhihu.com/equation?tex=f_%7BF%7D) 以及fb之间的关系，并且说明了学习方法应该具有一致性。但是依然忽略了采用哪种风险来学习得到 ![f_{n}](https://www.zhihu.com/equation?tex=f_%7Bn%7D) 。实际上，我们手上只有经验风险看起来比较容易得到，如果利用经验风险最小化的方法(ERM)去学习 ![f_{n}](https://www.zhihu.com/equation?tex=f_%7Bn%7D) ，ERM学习是否能够满足一致性的要求，能够满足哪种一致性要求呢？现在我们只考虑最宽松的一致性，即在ERM定义的经验风险 ![R_{emp}](https://www.zhihu.com/equation?tex=R_%7Bemp%7D) 的情况下，随着样本数目n的增加， ![R_{emp}(fn)](https://www.zhihu.com/equation?tex=R_%7Bemp%7D%28fn%29) 是否能无限逼近F中期望风险最小的最优分类器 ![R_{(fF)}](https://www.zhihu.com/equation?tex=R_%7B%28fF%29%7D) ， 我们将看到，F空间的大小起到了很重要的作用。

## **3.1.1 单一函数假设空间|F|=1**

在经验风险情况下，一种极端情况，在分类器的假设空间中只有一种选择f，那么无论何时 ![f_{n}=f_{F}=f](https://www.zhihu.com/equation?tex=f_%7Bn%7D%3Df_%7BF%7D%3Df) 都成立。显然，任何学习算法都能在F中一致。我们现在关注的问题是，当n → ∞时， ![R_{emp}(f) → R_{(f)}](https://www.zhihu.com/equation?tex=R_%7Bemp%7D%28f%29+%E2%86%92+R_%7B%28f%29%7D)吗？换言之，当样本增多时，f的经验风险 ![R_{emp}(f)](https://www.zhihu.com/equation?tex=R_%7Bemp%7D%28f%29) 是否可以无限逼近真实风险 ![R_{(f)}](https://www.zhihu.com/equation?tex=R_%7B%28f%29%7D) ？

从之前3.1节中的经验风险和期望风险的定义可以看出，当f固定时， ![R_{emp}(f) ](https://www.zhihu.com/equation?tex=R_%7Bemp%7D%28f%29+) 是损失L的样本均值，真实(期望)风险 ![R_{(f)}](https://www.zhihu.com/equation?tex=R_%7B%28f%29%7D) 是损失L的期望, 在n逐渐增多时，利用统计学中一个重要的定理：辛钦大数定律(样本数量很大的时候，样本均值和真实均值(期望)充分接近)直接得出结论：当f固定时，当n趋于无穷时，经验风险 ![R_{emp}(fn)](https://www.zhihu.com/equation?tex=R_%7Bemp%7D%28fn%29) 可以依概率收敛到真实(期望)风险 ![R_{(fF)}](https://www.zhihu.com/equation?tex=R_%7B%28fF%29%7D) 。

![](https://pic1.zhimg.com/v2-714f0a3f8b74f83d07f3eb8dadd65e18_b.png)图6 单一函数空间中学习的一致性

## **3.1.2 有限假设空间|F|=n**

当F中有多个函数时，由于fn是一个依赖于样本集的非固定函数，大数定律失效，但是大数定理的基本思想是统计学习理论最核心的依据。我们希望能将大数定律推广到假设空间是多个函数的情况。我们必须寻找一种统一看待多个函数的方法，即通过定义一个F中所有函数f的真实风险R和经验风险误差 ![R_{emf}](https://www.zhihu.com/equation?tex=R_%7Bemf%7D) 的上界：

![\left | R(f)-R_{emp}(f) \right |\leq sup_{f\in F}\left | R(f) -R_{emp}(f)\right |](https://www.zhihu.com/equation?tex=%5Cleft+%7C+R%28f%29-R_%7Bemp%7D%28f%29+%5Cright+%7C%5Cleq+sup_%7Bf%5Cin+F%7D%5Cleft+%7C+R%28f%29+-R_%7Bemp%7D%28f%29%5Cright+%7C)

![P(\left | R(f_{n})-R_{emp}(f_{n}) \right |\geq \varepsilon )\leq P(sup_{f\in F}\left | R(f)-R_{emp}(f) \right |\geq \epsilon )](https://www.zhihu.com/equation?tex=P%28%5Cleft+%7C+R%28f_%7Bn%7D%29-R_%7Bemp%7D%28f_%7Bn%7D%29+%5Cright+%7C%5Cgeq+%5Cvarepsilon+%29%5Cleq+P%28sup_%7Bf%5Cin+F%7D%5Cleft+%7C+R%28f%29-R_%7Bemp%7D%28f%29+%5Cright+%7C%5Cgeq+%5Cepsilon+%29)

由此可以看出，如果上式子右边依概率收敛，则左边也依概率收敛，即

![R_{emp}(f_{n})\overset{P}{\rightarrow}R(f_{n})](https://www.zhihu.com/equation?tex=R_%7Bemp%7D%28f_%7Bn%7D%29%5Coverset%7BP%7D%7B%5Crightarrow%7DR%28f_%7Bn%7D%29)

现在考虑F一致性：

![\left | R(f_{n})-R(f_{F}) \right |\begin{aligned}  &=R(f_{n})-R(f_{F}) \\   &=R(f_{n})+(-R_{emp}(f_{n})+R_{emp}(f_{n})-R_{emp}(f_{F})+R_{emp}(f_{F}))-R(f_{F}) \\   &\leq (R(f_{n})-R_{emp}(f_{n}))+(R(f_{F})-R_{emp}(f_{F}))\\   &\leq 2sup_{f\in F}\left | R(f)-R_{emp}(f) \right |  \end{aligned}](https://www.zhihu.com/equation?tex=%5Cleft+%7C+R%28f_%7Bn%7D%29-R%28f_%7BF%7D%29+%5Cright+%7C%5Cbegin%7Baligned%7D++%26%3DR%28f_%7Bn%7D%29-R%28f_%7BF%7D%29+%5C%5C+++%26%3DR%28f_%7Bn%7D%29%2B%28-R_%7Bemp%7D%28f_%7Bn%7D%29%2BR_%7Bemp%7D%28f_%7Bn%7D%29-R_%7Bemp%7D%28f_%7BF%7D%29%2BR_%7Bemp%7D%28f_%7BF%7D%29%29-R%28f_%7BF%7D%29+%5C%5C+++%26%5Cleq+%28R%28f_%7Bn%7D%29-R_%7Bemp%7D%28f_%7Bn%7D%29%29%2B%28R%28f_%7BF%7D%29-R_%7Bemp%7D%28f_%7BF%7D%29%29%5C%5C+++%26%5Cleq+2sup_%7Bf%5Cin+F%7D%5Cleft+%7C+R%28f%29-R_%7Bemp%7D%28f%29+%5Cright+%7C++%5Cend%7Baligned%7D)

可得：

![P(\left | R(f_{n})-R(f_{F}) \right |\geq \varepsilon )\leq P(sup_{f\in F}\left | R(f)-R_{emp}(f) \right |\geq \frac{\epsilon }{2})](https://www.zhihu.com/equation?tex=P%28%5Cleft+%7C+R%28f_%7Bn%7D%29-R%28f_%7BF%7D%29+%5Cright+%7C%5Cgeq+%5Cvarepsilon+%29%5Cleq+P%28sup_%7Bf%5Cin+F%7D%5Cleft+%7C+R%28f%29-R_%7Bemp%7D%28f%29+%5Cright+%7C%5Cgeq+%5Cfrac%7B%5Cepsilon+%7D%7B2%7D%29)

上式也表明，只要能够说明这个上界supf∈F能够依概率收敛，则可以表明这个学习算法具有F一致性，即：

![R(f_{n})\overset{P}{\rightarrow}R(f_{F})](https://www.zhihu.com/equation?tex=R%28f_%7Bn%7D%29%5Coverset%7BP%7D%7B%5Crightarrow%7DR%28f_%7BF%7D%29)

通过定义一个上界，我们将多个f的情况简化为检查上界是否收敛的问题。如果把n趋于无穷时，上界依概率收敛成立定义为空间F是一致收敛的（Uniform convergence）。那么我们可以就可以换一个说法，如果假设空间F一致收敛，则ERM学习在F上一致，即：

![R_{emp}(f_{n})\overset{P}{\rightarrow}R(f_{F})](https://www.zhihu.com/equation?tex=R_%7Bemp%7D%28f_%7Bn%7D%29%5Coverset%7BP%7D%7B%5Crightarrow%7DR%28f_%7BF%7D%29)

现在的问题是如何证明空间F一致收敛？ 幸运的是，我们只需要借助于一个类似于大数定理的Chernoff bound定理，即可证明，详细的细节可参考文献[1]。

![](https://pic1.zhimg.com/v2-048489a34c5c64cdc6f3e6ca076c1b04_b.png)图7 有限函数空间中学习的一致性

## **3.1.3 无限假设空间|F|=∞**

当函数F的空间包含无限个假设的情况下， 问题看起来变得更加棘手。我们希望寻求一个办法，将无限个假设的情况通过一种转换，使其变成含有有限个假设的函数空间，然后就可以利用之前的结果。

## **(1)Symmetrization(平衡化)**

在之前的介绍中，我们所有的一致性依赖f的真实(期望)损失R(f)。我们之前已经说过，真实风险是无法计算得到的，这是因为我们无法获得数据的分布，这个分布蕴含着无限多数据才能展现的信息，我们必须把这个东西用可观测的量替代掉。这时，我们需要一个叫Symmetrization的技巧。简单来说，Symmetrization的意思是，如果我们除了 ![D_{n}](https://www.zhihu.com/equation?tex=D_%7Bn%7D) ，还有一组独立于Dn且同样大小的iid抽样 ![D_{n}^{'}](https://www.zhihu.com/equation?tex=D_%7Bn%7D%5E%7B%27%7D) ，定义其上的经验风险是 ![R_{emp}'(f)](https://www.zhihu.com/equation?tex=R_%7Bemp%7D%27%28f%29) ，则有如下定理(这个证明很复杂，我们只需知道结论即可)：

![P(sup_{f\in F}\left | R(f)- R_{emp}(f)\right |\geq \epsilon )\leq 2P(sup_{f\in F}\left | R_{emp}(f)-R_{emp}^{'}(f) \right |\geq \frac{\epsilon }{2})](https://www.zhihu.com/equation?tex=P%28sup_%7Bf%5Cin+F%7D%5Cleft+%7C+R%28f%29-+R_%7Bemp%7D%28f%29%5Cright+%7C%5Cgeq+%5Cepsilon+%29%5Cleq+2P%28sup_%7Bf%5Cin+F%7D%5Cleft+%7C+R_%7Bemp%7D%28f%29-R_%7Bemp%7D%5E%7B%27%7D%28f%29+%5Cright+%7C%5Cgeq+%5Cfrac%7B%5Cepsilon+%7D%7B2%7D%29)

这时，上界的收敛性不再依赖不可观测的 ![R(f)](https://www.zhihu.com/equation?tex=R%28f%29) 了，而只依赖可以观测的经验风险。现在的情况是，如果我们能找到一些条件使得：

![sup_{f\in F}\left | R_{emp}(f)-R_{emp}^{'} (f)\right |](https://www.zhihu.com/equation?tex=sup_%7Bf%5Cin+F%7D%5Cleft+%7C+R_%7Bemp%7D%28f%29-R_%7Bemp%7D%5E%7B%27%7D+%28f%29%5Cright+%7C)

能够依概率收敛，那么 ![f_{n}](https://www.zhihu.com/equation?tex=f_%7Bn%7D) 的一致性也将得到保证。

上式将风险的一致性转换到仅仅依赖经验风险上，这种转换非常重要，因为在这种情况下， 即使F的空间大小是无限的，但是对于特定的n个样本(有限的)，经验风险的取值将是有限的。

## **(2) Shattering Coefficient (打散系数)**

考虑一个二分类问题，给出一个有n个样本的样本集，不论你的F中有多少函数，最终输出最多只能有 ![2^n](https://www.zhihu.com/equation?tex=2%5En) 种可能（¨一个样本输出要么输出0，要么输出1）。如果只考虑经验风险，任何F在面对n个样本时，在ERM评价下其等价于一个最多有 ![2^n](https://www.zhihu.com/equation?tex=2%5En) 个假设函数的有限假设空间(即n个样本最多有 ![2^n](https://www.zhihu.com/equation?tex=2%5En)  种分法)。我们将问题从考虑无穷个假设的F空间转化为F对n个有限数据有限的分类结果上，这简直太棒了。实际上，情况还要更好！现在我们要考察样本数为n时，F(包含有无穷的假设)所能给出的分类个数，我们将这个度量表示为N(F,n)，即F对n个样本产生的不同分类结果。

一个输入空间在[0,1]上的二分类问题：它选择一个点θ ∈ [0,1]，将区间分为[0,θ]和(θ,1]两部分，然后将两个部分分别作为一个类别。简单来说，这个模型就是将输入空间一分为二，一边赋值为0，一边赋值为1，因为θ有无数种选择，所以F是一个无限集合。可以看出，能输出的分类结果与样本点的个数相关。我们在下图展示了随着数据点个数的增加，分类模型所能产生的分类结果数目即N=(F,n)。其中垂直于坐标轴的箭头为分类器，红色箭头表示若样本在其左侧，则将其标记为0；同样蓝色箭头表示样本若在其右侧，则将其标记为1。注意每一个时刻仅允许存在一个分类器。当n=1时，无论样本如何标记，总能把它分开。当样本个数为n=2时，无论样本怎样标记，只需要四个分类器就能把他们分开。但是，当n=3时，数据样本的标记有8种情况，但是，分类器最多仅有6种输出(六个垂直的小箭头)。当n=4时，数据样本有16种标记情况，分类器最多能给出8个输出。通过以上直观的分析，当[0,1]上有n个数据时，实际的标记有2n，我们的分类器最多有2n个输出。对于这个简单的分类模型，他的Shattering Coefficient 表示为
![](https://pic4.zhimg.com/v2-7cbbf3fae25e6bd57569d33dc0f6dbbb_b.png)图 8 直线上分类模型的打散系数

Shattering Coefficient反映了模型随着n的增加，能够应对的输出的分类结果的个数。

基于在两个容量为n的样本集上，其经验风险差值的输出只有 ![2^{2n}](https://www.zhihu.com/equation?tex=2%5E%7B2n%7D) 种可能，将F等价为一个容量为 ![2^{2n}](https://www.zhihu.com/equation?tex=2%5E%7B2n%7D) 的有限假设空间。也就是说，我们把

![sup_{f\in F}\left | R_{emp}(f)-R_{emp}^{'} (f)\right |](https://www.zhihu.com/equation?tex=sup_%7Bf%5Cin+F%7D%5Cleft+%7C+R_%7Bemp%7D%28f%29-R_%7Bemp%7D%5E%7B%27%7D+%28f%29%5Cright+%7C)

中可能出现的无穷个f，转换成 ![2^{2n}](https://www.zhihu.com/equation?tex=2%5E%7B2n%7D) 个有限假设的空间了，然后基于前面提到的有限假设空间的Chernoff bound，我们就能得到ERM是一致的结论，现在我们直接给出这个结论，若：

![n \to \infty ,2N(F,2n)exp(-n\varepsilon ^{2}/4)\rightarrow 0](https://www.zhihu.com/equation?tex=n+%5Cto+%5Cinfty+%2C2N%28F%2C2n%29exp%28-n%5Cvarepsilon+%5E%7B2%7D%2F4%29%5Crightarrow+0)


则ERM是一致。现在一致性终于落在了N=(F,2n)身上。已经证明，如果N(F, 2n)随着n的增长呈多项式级别增长，则ERM是一致的。其他的情况更加复杂，但是不管怎样，通过一些数学技巧，也已经证明 ERM在无限F空间中也是可以达到一致，只要N=(F, 2n)增长的不是特别快。整个过程如图所示：

![](https://pic1.zhimg.com/v2-ef181e770e0d19e5b1512bf1eface54c_b.png)图 9 无限假设空间的一致性

经过漫长的历程，我们简要说明了ERM的一致性，这样我们就能对利用数据进行模型的学习有了基本的理论依据，我们终于明白，尽管假设可能有无穷个，只要它对数据的分类输出的能力并不是指数级增长的，或者说并不是能够对n个样本的所有情况都能输出分类标签的话，我们的学习就是有理论支撑的。

## **3.4 VC 维**

由此可见，随着数据n的增长，对数据分类的能力反映了模型的性能。在之前，我们用Shattering Coefficient 即N(F,n)度量模型对数据的分类能力。实际上， 还有另一个更加著名的度量标准，VC维。此概念由Vladimir Vapnik与Alexey Chervonenkis提出。

定义：对于一个假设空间F，如果存在n个数据样本能够被假设空间F中的函数按所有可能的2n种形式(每个数据的标签为0或1)分开 ，则称假设空间F能够把n个数据样本打散（shatter）。假设空间F的VC维就是能打散的最大数据样本数目m。若对任意数目的数据样本都有函数能将它们shatter，则假设空间F的VC维为无穷大。

通过定义，我们可以再一次看看之前在直线上的分类模型对应的VC维，如下图所示：
![](https://pic1.zhimg.com/v2-8ff3857840faee78598764c1e05dfb64_b.png)图10 直线上分类器的VC维

n=1时，分类器能够将数据类别的两种情况输出c=21=2。n=2时，分类器依然能够输出所有四种c=22=4输出，这两种情况被称为打散(shatter)，但数据个数为3时，这种分类模型只能输出6种分类结果，而实际上样本却又8种(c=23=8)标签组合，我们把第一个不满足shatter的n值称为break point，满足shatter的最大的值称为VC维。

接下来，我们讨论更有价值的一种分类模型，二维平面上的分类线。

![](https://pic3.zhimg.com/v2-197e2d8e0d7aad3f76a5d49666ec952a_b.png)

![](https://pic3.zhimg.com/v2-d1286e5d09e3184e54db1827a38c4122_b.png)图11 平面上分类线模型的VC维[1]

注意，这里我们不需要对于所有容量为3的样本都可以完全分开（若三个样本共线，则存在线性分类器分不开的情况），我们只需要存在这样一组样本即可。当样本容量为4时，则无法构造一个排布令16种情况都可以被分开，因为无论如何排不，都会遇到如图b类似如下的情况，此时数据将无法被一条线分开。所以，二维平面上线性分类器的VC维为3。

如果break point存在（有限的正整数，意味着VC也有限，因为VC=breakpoint-1），N(F,n)是多项式级别的。由此，借助Shattering Coefficient的N(F,n)的一致性说明，我们给出这样一个ERM一致的充分必要条件：

ERM对于假设空间F一致，当且仅当F的VC维有限 。

以上过程可以表示为：

\
![](https://pic4.zhimg.com/v2-a561093fe18fe3f97c70a49c5f94f68b_b.png)图12 基于VC维的一致性说明

VC维反映了假设空间F 的强大程度(powerfulness)，VC 维越大，F也越强，因为它可以打散(shatter)更多的点。在实践中，一种符合直觉的规律是： f的自由参数越多，VC维就越大：VC 维与假设中自由变量数目大约相等即：VC = nfree_parameters. 目前深度神经网络的参数已经达到百万，千万甚至亿级。这应该是深度神经网络成功的根本原因。

由于是充要条件，VC维也用于判断一个ERM是否有效，可以证明，对于任何有限的维数，线性分类器的VC维都是有限的，所以线性分类器总是ERM一致。这就为我们使用ERM准则训练线性模型做分类找到了理论依据。

## **4 支持向量机**

## **4.1 结构风险最小化**

我们前面讨论了经验风险的一致性，一个基本的假设是当样本数据的个数n趋于无穷时(实际情况是容量足够大)时，经验风险最小化能保证很好的学习效果。然而，当样本空间很小时，单纯考虑经验风险最小化学习的效果就未必很好，会产生“过拟合(over-fitting)”现象。主要是因为在样本集个数n小于break point时, 即总能找到一个假设使得分类器将所有训练样本正确分类。换句话说，分类器复杂度高(性能太好)，导致了过拟合。那么既然分类器复杂程度高会导致学习性能下降， 我们就可以把度量分类器复杂程度的一个量（惩罚），加到经验风险损失上。这样，学习问题可以看作致力于同时降低经验风险和分类器复杂度惩罚的工作。经验风险的降低需要样本数据更多，而分类器复杂度度量则要求分类器的复杂性不能太高。这也导致我们得出一个基本的结论，当分类器的复杂度确定以后，如果想降低损失，或是提高学习性能，唯一能做的就是增加数据，使得经验风险降低。在经验风险上加上表示模型复杂度的正则项(regularizer)或者罚项(penalty term) 就构成了所谓结构风险。在假设空间F、损失函数以及训练数据集确定的情况下，结构风险的定义为：

![R_{srm}(f)=\frac{1}{N}\sum_{i=1}^{N}L(y_{i},f(x_{i}))+\lambda J(f)](https://www.zhihu.com/equation?tex=R_%7Bsrm%7D%28f%29%3D%5Cfrac%7B1%7D%7BN%7D%5Csum_%7Bi%3D1%7D%5E%7BN%7DL%28y_%7Bi%7D%2Cf%28x_%7Bi%7D%29%29%2B%5Clambda+J%28f%29)

其中，J(f)表示模型的复杂度，λ表示其权重。将分类器复杂度与经验风险一起考虑的优化问题，被定义为结构风险最小化。

![min\frac{1}{N}\sum_{i=1}^{N}L(y_{i},f(x_{i}))+\lambda J(f)](https://www.zhihu.com/equation?tex=min%5Cfrac%7B1%7D%7BN%7D%5Csum_%7Bi%3D1%7D%5E%7BN%7DL%28y_%7Bi%7D%2Cf%28x_%7Bi%7D%29%29%2B%5Clambda+J%28f%29)

结构风险最小化的策略认为结构风险最小化的模型是最优的模型。这样监督学习问题就变成了经验风险或者结构风险函数的最优化问题。

## **4.2 支持向量机**

VC维保证了在理想情况下，直线分类器总是ERM一致,这样我们就可以大胆的利用数据去训练直线这种简单的分类器。

## **4.2.1 线性可分与最大间隔**

如果一个线性函数能够将样本分开，称这些数据样本是线性可分的。那么什么是线性函数呢？在二维空间中就是一条直线，在三维空间中就是一个平面，以此类推，如果不考虑空间维数，这样的线性函数统称为超平面。我们看一个简单的二维空间的例子，红色代表正类，蓝色代表负类，样本是线性可分的，但是很显然不只有这一条直线可以将样本分开，而是有无数条，我们所说的线性可分支持向量机就对应着能将数据正确划分并且间隔最大的直线。

支持向量机是一个特殊类型的线性分类器，在这个系列的第一季，我们已经介绍了感知机的学习规则，即只要样本线性可分，随机初始化的一条直线，一定能够把两类样本分开。感知机最终的目标仅仅是将其分开，初始化直线的不同，将导致不同的分类直线。在样本线性可分的情况下， 有没有一种理论上最优的分类器呢？ 

![](https://pic3.zhimg.com/v2-a4430617fdc85d2ba19bbe7d10b46ca6_b.png)图 13 不同方向上的最优分类面

上图的(b)和(c)分别给出了两种不同的分类方案，其中黑色实线为分界线，术语称为“决策面”。每个决策面对应了一个线性分类器。虽然从分类结果上看，分类器A和分类器B的效果是相同的，但是他们的性能是有差距的，看下图：

![](https://pic3.zhimg.com/v2-a4430617fdc85d2ba19bbe7d10b46ca6_b.png)图14 间隔最大的分类面具有较好的泛化能力

图中添加了一个红点。可以看到，(b)的分类器A依然能很好的分类结果，而(c)中的分类器B则出现了分类错误。这个例子表明分类器B的"决策面"放置的位置优于分类器C的"决策面"放置的位置，从图上可以看出主要是分类器A的分类间隔(虚线的宽度)比分类器B的分类间隔(虚线的宽度)大。如虚线所示。虚线的间隔越宽，表明这条分类线越好，直觉上是因为这个区域越宽，未来样本落在这个区域被分类正确的概率就越大。在每一个可能把数据集正确分开的方向都有一个最优决策面，而不同方向的最优决策面的分类间隔通常是不同的，那个具有“最大间隔”的最优决策面就是要寻找的最好的分类器，这个分类器被称为支持向量机(也被称为最大间隔分类器)，主要原因是这个最大间隔分类器对应的两侧虚线所穿过的样本点被称为"支持向量"。因此，支持向量机可以看作是“优中选最优”得到的。作为支持向量的样本点非常重要，可以直观的看到，最大间隔分类器仅仅由支持向量点确定，那些不是支持向量点的样本存在的价值不大。

关于如何推到寻找最大间隔的决策面的问题一般被看做在满足样本被正确分类情况下的一个最大化样本点到分类面间隔的问题。

argmax： 分类间隔

subject to：所有样本保持与分类面某种单位距离

关于这个问题的形式化描述和推导已经有很多文献可以查阅，例如SIGAI中的用一张图理解SVM的脉络[10]。需要强调的是，在线性可分的情况下， 支持向量机具有唯一性，这个优化问题实际上是一个简单的凸优化问题。

## **4.2.2线性不可分与核函数**

对于非线性问题，线性可分支持向量机并不能有效解决，例如下图的例子,例如桌子上放了两种口味的巧克力球，蓝色和咖啡色，我们无法在保证巧克力球不动的情况下，在桌子上画一条线将他们分开，他们是线性不可分的。

![](https://pic1.zhimg.com/v2-1dd2efb663e005bf9bd5bb2eb9c6011c_b.png)图15 线性不可分问题

现在假设我们俯视着桌面上两种口味的巧克力，突然咖啡色的巧克力球全部垂直悬浮在空中，朝你面前飞去，视觉上，你几乎察觉不到他们在桌面上的移动，但是这个时候，两类巧克力球已经在三维空间分开了，我们只需要找一个透明的盘子，接住那些咖啡色的巧克力。也就是说我们在桌面上无法分开的巧克力，只要我们将他们置于更高的三维空间中，区分他们简直易容反掌，实际效果下图所示，我们把巧克力原来所处的桌面(空间)称为输入空间，能够利用透明盘子分类的三维世界(空间)称为特征空间。

![](https://pic2.zhimg.com/v2-ea064d3340159ee4b905c05ffa471c75_b.png)图16 低维到高维的映射

我们在特征空间中我们只需要进行线性分类，对于平面国中的人们而言，就好像在二维平面上建立如下图所示的非线性的分类线。

![](https://pic4.zhimg.com/v2-96e86ce71b2891961454cc0c0a1a07ef_b.png)图17低维世界中看到的高维超平面的分类线

这种从原始空间X到特征空间F的转换被称为映射ϕ， 即原始空间的样本x,被映射到ϕ(x) 三维空间中。所有样本点都是二维点$$x\_{i}$$，其值分别为$$(x\_{i},y\_{i})$$,我们可以明确的给出某个从二维到三维的映射函数ϕ的形式：

![\phi ((x,y))=(x^{2},\sqrt{2}xy,y^{2})](https://www.zhihu.com/equation?tex=%5Cphi+%28%28x%2Cy%29%29%3D%28x%5E%7B2%7D%2C%5Csqrt%7B2%7Dxy%2Cy%5E%7B2%7D%29)

可以看出，每一个样本点都可以利用这个映射，从二维变为三维空间中的点。我们可以在三维空间中进行线性分类器(面)的训练。但是对更高维的样本点而言，明确给出这种映射关系实际上不现实的也是没必要的。这主要是因为在训练支持向量机的超平面参数时，我们实际上对数据的操作是样本点对的内积的运算，即 ![\left \langle  x^{i}, x^{j}\right \rangle](https://www.zhihu.com/equation?tex=%5Cleft+%5Clangle++x%5E%7Bi%7D%2C+x%5E%7Bj%7D%5Cright+%5Crangle) ，关于如何得到这一点同样可以参考文献[9,10]，简单老说是通过将支持向量机的原始优化问题转化成其所谓对偶问题得到的最关键结果。在原始空间数据线性不可分时， 我们转而在特征空间F中训练一个支持向量机，因此我们需要计算新的特征空间中样本的内积，即 ![< ϕ(x^{i}), ϕ(x^{j})>](https://www.zhihu.com/equation?tex=%3C+%CF%95%28x%5E%7Bi%7D%29%2C+%CF%95%28x%5E%7Bj%7D%29%3E) 。这里需要说明的是，一般而言原始特征空间样本的维数本来就很高，将其往高维空间进行转换后，ϕ(xi)的维数更高。高维空间非常神奇，并不是二维到三维仅仅增加一维就能解决的，可能需要的维度更高，甚至是无穷维，就我们这个例子而言，既然我们只需要得到 ![< ϕ(x^{i}), ϕ(x^{j})>](https://www.zhihu.com/equation?tex=%3C+%CF%95%28x%5E%7Bi%7D%29%2C+%CF%95%28x%5E%7Bj%7D%29%3E) 的值，在上面的例子中， 特征空间的内积可表示成：

![\left \langle \phi (x_{1}),\phi (x_{2}) \right \rangle\begin{aligned}  &= \left \langle (x_{1}^{2},\sqrt{2}x_{1}y_{1},y_{1}^{2}),(x_{2}^{2},\sqrt{2}x_{2}y_{2},y_{2}^{2}) \right \rangle\\   &=x_{1}^{2}x_{2}^{2}+2x_{1}y_{1}x_{2}y_{2}+y_{1}^{2}y_{2}^{2} \\   &= (x_{1}x_{2}+y_{1}y_{2})^{2}\\   &= \left \langle x_{1},x_{2} \right \rangle^{2}\\   &= f(x_{1},x_{2}) \end{aligned}](https://www.zhihu.com/equation?tex=%5Cleft+%5Clangle+%5Cphi+%28x_%7B1%7D%29%2C%5Cphi+%28x_%7B2%7D%29+%5Cright+%5Crangle%5Cbegin%7Baligned%7D++%26%3D+%5Cleft+%5Clangle+%28x_%7B1%7D%5E%7B2%7D%2C%5Csqrt%7B2%7Dx_%7B1%7Dy_%7B1%7D%2Cy_%7B1%7D%5E%7B2%7D%29%2C%28x_%7B2%7D%5E%7B2%7D%2C%5Csqrt%7B2%7Dx_%7B2%7Dy_%7B2%7D%2Cy_%7B2%7D%5E%7B2%7D%29+%5Cright+%5Crangle%5C%5C+++%26%3Dx_%7B1%7D%5E%7B2%7Dx_%7B2%7D%5E%7B2%7D%2B2x_%7B1%7Dy_%7B1%7Dx_%7B2%7Dy_%7B2%7D%2By_%7B1%7D%5E%7B2%7Dy_%7B2%7D%5E%7B2%7D+%5C%5C+++%26%3D+%28x_%7B1%7Dx_%7B2%7D%2By_%7B1%7Dy_%7B2%7D%29%5E%7B2%7D%5C%5C+++%26%3D+%5Cleft+%5Clangle+x_%7B1%7D%2Cx_%7B2%7D+%5Cright+%5Crangle%5E%7B2%7D%5C%5C+++%26%3D+f%28x_%7B1%7D%2Cx_%7B2%7D%29+%5Cend%7Baligned%7D)

注意最后两项， ![\left \langle \phi (x^{1}),\phi (x^{2}) \right \rangle=f(x^{1},x^{2})=\left \langle x^{1}x^{2} \right \rangle^{2}](https://www.zhihu.com/equation?tex=%5Cleft+%5Clangle+%5Cphi+%28x%5E%7B1%7D%29%2C%5Cphi+%28x%5E%7B2%7D%29+%5Cright+%5Crangle%3Df%28x%5E%7B1%7D%2Cx%5E%7B2%7D%29%3D%5Cleft+%5Clangle+x%5E%7B1%7Dx%5E%7B2%7D+%5Cright+%5Crangle%5E%7B2%7D) , 也就是说我们将特征空间的内积表示成如下的形式： ![k(x^{1},x^{2})=f(x^{1},x^{2})=\left \langle x^{1},x^{2} \right \rangle^{2}](https://www.zhihu.com/equation?tex=k%28x%5E%7B1%7D%2Cx%5E%7B2%7D%29%3Df%28x%5E%7B1%7D%2Cx%5E%7B2%7D%29%3D%5Cleft+%5Clangle+x%5E%7B1%7D%2Cx%5E%7B2%7D+%5Cright+%5Crangle%5E%7B2%7D) ，最重要的是这个形式是原始空间中内积的一个函数，或者说是原始空间中样本对 ![(x_{1},x_{2})](https://www.zhihu.com/equation?tex=%28x_%7B1%7D%2Cx_%7B2%7D%29)的一个函数，这个函数k被称为核函数，因此核函数本身就是特征空间中的内积。

![k(x^{i},x^{j})=\left \langle \phi (x^{i}),\phi (x^{j}) \right \rangle](https://www.zhihu.com/equation?tex=k%28x%5E%7Bi%7D%2Cx%5E%7Bj%7D%29%3D%5Cleft+%5Clangle+%5Cphi+%28x%5E%7Bi%7D%29%2C%5Cphi+%28x%5E%7Bj%7D%29+%5Cright+%5Crangle)

接下来稍微有点奇怪，也就是说如果我们定义了某个核函数 ![k(x^{i},x^{j})=\left \langle x^{i},x^{j} \right \rangle^{2}](https://www.zhihu.com/equation?tex=k%28x%5E%7Bi%7D%2Cx%5E%7Bj%7D%29%3D%5Cleft+%5Clangle+x%5E%7Bi%7D%2Cx%5E%7Bj%7D+%5Cright+%5Crangle%5E%7B2%7D) 意味着我们将样本从二维映射到三维了，其实这个时候就不需要知道 ![ϕ(x_{i)}](https://www.zhihu.com/equation?tex=%CF%95%28x_%7Bi%29%7D) 的具体形式了(尽管在上面的例子中，我们从ϕ的具体形式推导过来的)，这一点非常重要也最具有迷惑性。从现在开始，我们不想给出低维到高维的映射的具体形式ϕ，因为这太难了。我们希望直接利用一对样本在原始空间中的某个函数k，来表示在高维空间中的内积运算。这又是一种将多维变为低维的变化，或者说将无穷变为有限的操作。

问题是我们怎么知道什么函数可以满足这个条件呢？由于样本点的个数n是一个有限值，核函数仅定义在这些有限的n个样本点对的内积上。我们可以将 ![k_{ij}\left \langle x^{i},x^{j} \right \rangle=f(x^{i},x^{j})](https://www.zhihu.com/equation?tex=k_%7Bij%7D%5Cleft+%5Clangle+x%5E%7Bi%7D%2Cx%5E%7Bj%7D+%5Cright+%5Crangle%3Df%28x%5E%7Bi%7D%2Cx%5E%7Bj%7D%29) 全部写出来，这样就构成了一个核矩阵K，有一个很强大的Mercer定理告诉我们只要核矩阵K是对称半正定的，这个核就是一种高维空间中的内积运算。这个定理的牛逼之处在于不用去寻找ϕ，只需要在训练集上求出各个 ![k_{ij}](https://www.zhihu.com/equation?tex=k_%7Bij%7D) ，然后判断矩阵K是否是半正定即可，这对数学家来说根本就不是问题。

几种常见的核函数形式是 ![k(x^{i},x^{j})=f(\left \langle x^{i},x^{j} \right \rangle)](https://www.zhihu.com/equation?tex=k%28x%5E%7Bi%7D%2Cx%5E%7Bj%7D%29%3Df%28%5Cleft+%5Clangle+x%5E%7Bi%7D%2Cx%5E%7Bj%7D+%5Cright+%5Crangle%29) 型，例如：

线性核

![k(x_{1},x_{2})=\left \langle x_{1},x_{2} \right \rangle](https://www.zhihu.com/equation?tex=k%28x_%7B1%7D%2Cx_%7B2%7D%29%3D%5Cleft+%5Clangle+x_%7B1%7D%2Cx_%7B2%7D+%5Cright+%5Crangle)

多项式核

![k(x_{1},x_{2})=(\gamma \left \langle x_{1},x_{2} \right \rangle+c)^{n}](https://www.zhihu.com/equation?tex=k%28x_%7B1%7D%2Cx_%7B2%7D%29%3D%28%5Cgamma+%5Cleft+%5Clangle+x_%7B1%7D%2Cx_%7B2%7D+%5Cright+%5Crangle%2Bc%29%5E%7Bn%7D)

sigmod核函数：

![k(x_{1},x_{2})=tanh(\gamma \left \langle x_{1},x_{2} \right \rangle+c)](https://www.zhihu.com/equation?tex=k%28x_%7B1%7D%2Cx_%7B2%7D%29%3Dtanh%28%5Cgamma+%5Cleft+%5Clangle+x_%7B1%7D%2Cx_%7B2%7D+%5Cright+%5Crangle%2Bc%29)

这三种核函数都是采用了 ![f\left ( \left \langle x^{i},x^{j} \right \rangle \right )](https://www.zhihu.com/equation?tex=f%5Cleft+%28+%5Cleft+%5Clangle+x%5E%7Bi%7D%2Cx%5E%7Bj%7D+%5Cright+%5Crangle+%5Cright+%29) 的形式，它们直接借助原始空间内积，定义特征空间中内积。

还有另外两个常见的核函数以两位著名的数学家命名：

高斯核函数

![k(x_{1},x_{2})=exp(-\frac{\left \| x_{1}-x_{2} \right \|}{2\sigma ^{2}})](https://www.zhihu.com/equation?tex=k%28x_%7B1%7D%2Cx_%7B2%7D%29%3Dexp%28-%5Cfrac%7B%5Cleft+%5C%7C+x_%7B1%7D-x_%7B2%7D+%5Cright+%5C%7C%7D%7B2%5Csigma+%5E%7B2%7D%7D%29)

拉普拉斯核函数

![k(x_{1},x_{2})=exp(-\frac{\left \| x_{1}-x_{2} \right \|}{\sigma })](https://www.zhihu.com/equation?tex=k%28x_%7B1%7D%2Cx_%7B2%7D%29%3Dexp%28-%5Cfrac%7B%5Cleft+%5C%7C+x_%7B1%7D-x_%7B2%7D+%5Cright+%5C%7C%7D%7B%5Csigma+%7D%29)

它们则采用了一对样本之间的差的某个函数表示特征空间中的内积，更重要的是，我们可以通过对高斯核的数学变换，写出高斯核对应的高维空间的具体变换ϕ(x)：

高斯核是无穷的

![e^{x}=1+x+\frac{x^{2}}{2!}+\frac{x^{3}}{3!}+...+\frac{x^{n}}{n!}+R_{n}](https://www.zhihu.com/equation?tex=e%5E%7Bx%7D%3D1%2Bx%2B%5Cfrac%7Bx%5E%7B2%7D%7D%7B2%21%7D%2B%5Cfrac%7Bx%5E%7B3%7D%7D%7B3%21%7D%2B...%2B%5Cfrac%7Bx%5E%7Bn%7D%7D%7Bn%21%7D%2BR_%7Bn%7D)

![\begin{aligned} & k(x_{1},x_{2})=e^{\frac{\left \| x_{1}-x_{2} \right \|^{2}}{2\sigma ^{2}}}=e^{\frac{(x_{1}-x_{2})^{2}}{2\sigma ^{2}}}=e^{-\frac{x_{1}^{2}+x_{2}^{2}-2x_{1}x{2}}{2\sigma ^{2}}}=e^{-\frac{x_{1}^{2}+x_{2}^{2}}{2\sigma ^{2}}}\cdot e^{\frac{x_{1}x_{2}}{\sigma ^{2}}} \\   &= e^{-\frac{x_{1}^{2}+x_{2}^{2}}{2\sigma ^{2}}}\cdot \left ( 1+\frac{1}{\sigma ^{2}}\cdot \frac{x_{1}x_{2}}{1!}+(\frac{1}{\sigma ^{2}}^{2})\cdot \frac{(x_{1}x_{2})^{2}}{2!}+(\frac{1}{\sigma ^{2}})^{3}\cdot \frac{(x_{1}x_{2})^{3}}{3!}+...+(\frac{1}{\sigma ^{2}})^{n}\cdot \frac{(x_{1}x_{2})^{n}}{n!}+... \right )\\   &= e^{-\frac{x_{1}^{2}+x_{2}^{2}}{2\sigma ^{2}}}\cdot (1\cdot 1+\frac{1}{1!}\frac{x_{1}}{\sigma }\cdot \frac{x_{2}}{\sigma }+\frac{1}{2!}\frac{x_{1}^{2}}{\sigma ^{2}}\cdot \frac{x_{2}^{2}}{\sigma ^{2}}+\frac{1}{3!}\frac{x_{1}^{3}}{\sigma ^{3}}\cdot \frac{x_{2}^{3}}{\sigma ^{3}})+...+\frac{1}{n!}\frac{x_{1}^{n}}{\sigma ^{n}}\cdot \frac{x_{2}^{n}}{\sigma ^{n}})\\   &= \phi (x_{1})^{T}\cdot \phi (x_{2}) \end{aligned}](https://www.zhihu.com/equation?tex=%5Cbegin%7Baligned%7D+%26+k%28x_%7B1%7D%2Cx_%7B2%7D%29%3De%5E%7B%5Cfrac%7B%5Cleft+%5C%7C+x_%7B1%7D-x_%7B2%7D+%5Cright+%5C%7C%5E%7B2%7D%7D%7B2%5Csigma+%5E%7B2%7D%7D%7D%3De%5E%7B%5Cfrac%7B%28x_%7B1%7D-x_%7B2%7D%29%5E%7B2%7D%7D%7B2%5Csigma+%5E%7B2%7D%7D%7D%3De%5E%7B-%5Cfrac%7Bx_%7B1%7D%5E%7B2%7D%2Bx_%7B2%7D%5E%7B2%7D-2x_%7B1%7Dx%7B2%7D%7D%7B2%5Csigma+%5E%7B2%7D%7D%7D%3De%5E%7B-%5Cfrac%7Bx_%7B1%7D%5E%7B2%7D%2Bx_%7B2%7D%5E%7B2%7D%7D%7B2%5Csigma+%5E%7B2%7D%7D%7D%5Ccdot+e%5E%7B%5Cfrac%7Bx_%7B1%7Dx_%7B2%7D%7D%7B%5Csigma+%5E%7B2%7D%7D%7D+%5C%5C+++%26%3D+e%5E%7B-%5Cfrac%7Bx_%7B1%7D%5E%7B2%7D%2Bx_%7B2%7D%5E%7B2%7D%7D%7B2%5Csigma+%5E%7B2%7D%7D%7D%5Ccdot+%5Cleft+%28+1%2B%5Cfrac%7B1%7D%7B%5Csigma+%5E%7B2%7D%7D%5Ccdot+%5Cfrac%7Bx_%7B1%7Dx_%7B2%7D%7D%7B1%21%7D%2B%28%5Cfrac%7B1%7D%7B%5Csigma+%5E%7B2%7D%7D%5E%7B2%7D%29%5Ccdot+%5Cfrac%7B%28x_%7B1%7Dx_%7B2%7D%29%5E%7B2%7D%7D%7B2%21%7D%2B%28%5Cfrac%7B1%7D%7B%5Csigma+%5E%7B2%7D%7D%29%5E%7B3%7D%5Ccdot+%5Cfrac%7B%28x_%7B1%7Dx_%7B2%7D%29%5E%7B3%7D%7D%7B3%21%7D%2B...%2B%28%5Cfrac%7B1%7D%7B%5Csigma+%5E%7B2%7D%7D%29%5E%7Bn%7D%5Ccdot+%5Cfrac%7B%28x_%7B1%7Dx_%7B2%7D%29%5E%7Bn%7D%7D%7Bn%21%7D%2B...+%5Cright+%29%5C%5C+++%26%3D+e%5E%7B-%5Cfrac%7Bx_%7B1%7D%5E%7B2%7D%2Bx_%7B2%7D%5E%7B2%7D%7D%7B2%5Csigma+%5E%7B2%7D%7D%7D%5Ccdot+%281%5Ccdot+1%2B%5Cfrac%7B1%7D%7B1%21%7D%5Cfrac%7Bx_%7B1%7D%7D%7B%5Csigma+%7D%5Ccdot+%5Cfrac%7Bx_%7B2%7D%7D%7B%5Csigma+%7D%2B%5Cfrac%7B1%7D%7B2%21%7D%5Cfrac%7Bx_%7B1%7D%5E%7B2%7D%7D%7B%5Csigma+%5E%7B2%7D%7D%5Ccdot+%5Cfrac%7Bx_%7B2%7D%5E%7B2%7D%7D%7B%5Csigma+%5E%7B2%7D%7D%2B%5Cfrac%7B1%7D%7B3%21%7D%5Cfrac%7Bx_%7B1%7D%5E%7B3%7D%7D%7B%5Csigma+%5E%7B3%7D%7D%5Ccdot+%5Cfrac%7Bx_%7B2%7D%5E%7B3%7D%7D%7B%5Csigma+%5E%7B3%7D%7D%29%2B...%2B%5Cfrac%7B1%7D%7Bn%21%7D%5Cfrac%7Bx_%7B1%7D%5E%7Bn%7D%7D%7B%5Csigma+%5E%7Bn%7D%7D%5Ccdot+%5Cfrac%7Bx_%7B2%7D%5E%7Bn%7D%7D%7B%5Csigma+%5E%7Bn%7D%7D%29%5C%5C+++%26%3D+%5Cphi+%28x_%7B1%7D%29%5E%7BT%7D%5Ccdot+%5Cphi+%28x_%7B2%7D%29+%5Cend%7Baligned%7D)

其中

![\phi （x）=e^{-\frac{x^{2}}{2\sigma ^{2}}}\left ( 1,\sqrt{\frac{1}{1!}} \frac{x}{\sigma },\sqrt{\frac{1}{2!}}\frac{x^{2}}{\sigma ^{2}},\sqrt{\frac{1}{3!}}\frac{x^{3}}{\sigma ^{3}},...,\sqrt{\frac{1}{n!}}\frac{x^{n}}{\sigma ^{n}},...\right )](https://www.zhihu.com/equation?tex=%5Cphi+%EF%BC%88x%EF%BC%89%3De%5E%7B-%5Cfrac%7Bx%5E%7B2%7D%7D%7B2%5Csigma+%5E%7B2%7D%7D%7D%5Cleft+%28+1%2C%5Csqrt%7B%5Cfrac%7B1%7D%7B1%21%7D%7D+%5Cfrac%7Bx%7D%7B%5Csigma+%7D%2C%5Csqrt%7B%5Cfrac%7B1%7D%7B2%21%7D%7D%5Cfrac%7Bx%5E%7B2%7D%7D%7B%5Csigma+%5E%7B2%7D%7D%2C%5Csqrt%7B%5Cfrac%7B1%7D%7B3%21%7D%7D%5Cfrac%7Bx%5E%7B3%7D%7D%7B%5Csigma+%5E%7B3%7D%7D%2C...%2C%5Csqrt%7B%5Cfrac%7B1%7D%7Bn%21%7D%7D%5Cfrac%7Bx%5E%7Bn%7D%7D%7B%5Csigma+%5E%7Bn%7D%7D%2C...%5Cright+%29)

可以看到，采用高斯核的特征空间是无穷维。核函数的出现的原因是我们希望将高维特征空间的内积转换原始空间中有限维的上关于两个样本对的某个函数，从而避免明确的给出原始空间中有限维到更高维的明确的映射关系ϕ。核方法的作用如下图所示：

![](https://pic3.zhimg.com/v2-1e3b7fef7153931777d81c6894f2c73a_b.png)图18 核函数的作用

上图的核心在于训练支持向量机的过程中，我们只需要关心的是样本的内积操作就可以完成实现训练过程，而不是单个样本本身。我们的目标是将有限维提升到无穷维以训练分类器，幸运的是我们只需要在有限维空间中进行简单的运算就可以实现无穷维空间中的内积，从而实现在无限维空间中训练支持向量机。 

## **4.2.3 线性不可分与软间隔**

现在看来使用高斯核简直可以解决所有问题了，但是这种变换如果放到原始空间中观察，就好像利用高斯核训练的分类器具有无穷的分类能力，按照前一节所说，这意味着这个模型的复杂度很高（VC维为无穷大）。由于过拟合，我们没有信心认为模型训练的足够好，特别是按照结构风险最小化的说法，我们希望在经验风险和结构风险的和降到最低(模型复杂度之间寻找平衡点)。当采用无穷维的高斯核时，经验风险可以降到最低，但结构风险陡然增加。

这是由支持向量机的最大间隔训练而产生的， 之前的支持向量机要求所有的样本均被最佳超平面正确划分，也被称为硬间隔。但是如果不允许出错，那么采用核函数训练的支持向量机在原始空间中将具有极高的复杂度(必须映射到更高维的空间中)。软间隔支持向量机允许某些样本点不满足正确分类的条件。这种情况实际上是利用允许分类出错的代价，换取模型的复杂度的降低。如图所示：

![](https://pic3.zhimg.com/v2-a47cbc342728f2c53c04dccbb6819602_b.png)图19软间隔分类器

红色圆圈所示的点为错分样本点。当然，在最大化间隔的同时，不满足约束的样本应尽可能少，或者说错分样本引起的代价应该足够低，软间隔的SVM的基本思想是：

argmin：   1/分类间隔 + 错分样本引入的代价

subject to： 允许所有样本存在一个宽松的距离度量。

此时，优化问题是一个最小值优化，注意目标函数包含了分类间隔的倒数。

## **5 结语**

我参考了几篇统计学习和SVM的博客文章，忽略了一些详细的数学推导和精确形式化表示，转而采取一种相对直观的方法，阐述统计学习理论和支持向量机的核心思想，那些复杂的推导依然非常重要。Vapnik在发明SVM时，曾与Yann Lecun打赌，声称最迟到2000年没有人将会继续使用神经网络结构（意思是都会转而使用支持向量机）,结果显而易见。

有限与无限的相互转换是一种重要的技巧和智慧，人类智慧最大的特点也是如此。利用有限的工具，力求实现无限的能力。人类的语言就是这种智慧最大的体现。我们在统计学习理论的发展中，看到了将无限的假设空间转化为有限空间的证明技巧。同时，在支持向量机中，我们看到了如何将低维数据提升到高维(无穷)维。通过在高维世界中构成的模型，解决低维世界中的问题。有意思的是，试图在高维世界构建模型所利用的“核方法”，使我们在低维空间就能完成高维建模工作。这些变换难免会让人眼花缭乱，但是展现出的技巧深刻的反映了人类的智慧的伟大。

这些将一个领域内的困难问题通过某种技巧转换成另一个领域内可解决问题的思路，值得每一个人借鉴。

参考文献

[1] 统计学习理论的数理基础 [统计学习理论的数理基础 - 为程序员服务](https://link.zhihu.com/?target=http%3A//ju.outofmemory.cn/entry/370094)

[2] SVM中高斯核函数为什么能映射到无穷维度[https://blog.csdn.net/weixin_40479663/article/details/83149532](https://link.zhihu.com/?target=https%3A//blog.csdn.net/weixin_40479663/article/details/83149532)

[3] svm核函数的理解和选择 [https://blog.csdn.net/u011746554/article/details/70941587](https://link.zhihu.com/?target=https%3A//blog.csdn.net/u011746554/article/details/70941587)

[4]SVM支持向量机入门及数学原理 [https://blog.csdn.net/qq_35992440/article/details/80987664](https://link.zhihu.com/?target=https%3A//blog.csdn.net/qq_35992440/article/details/80987664)

[5] 从核函数到SVM原理 [从核函数到SVM原理--sklearn-SVM实现 - 一逍倾城 - 博客园](https://link.zhihu.com/?target=https%3A//www.cnblogs.com/xiaotan-code/p/6695049.html)

[6] Vladimir N. Vapnik  统计学习理论的本质 张学工译 清华大学出版社2000

[7] VC维的来龙去脉 [解读机器学习基础概念：VC维的来龙去脉](https://link.zhihu.com/?target=http%3A//www.mamicode.com/info-detail-1148920.html)

[8] 支持向量机（SVM）入门理解与推导[https://blog.csdn.net/sinat_20177327/article/details/79729551](https://link.zhihu.com/?target=https%3A//blog.csdn.net/sinat_20177327/article/details/79729551)

[9] 雷明，机器学习与应用 清华大学出版社 2018 

[10] 用一张图理解SVM的脉络， [https://blog.csdn.net/SIGAI_CSDN/article/details/80751208](https://link.zhihu.com/?target=https%3A//blog.csdn.net/SIGAI_CSDN/article/details/80751208)

