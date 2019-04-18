# 理解计算：从√2到AlphaGo ——第1季 从√2谈起 - 知乎
# 


```
SIGAI特约作者：twinlj77
原创声明：本文为 SIGAI 原创文章，仅供个人学习使用，未经允许，不能用于商业目的。
```

> 导言
计算的概念看似简单却又十分宽泛，它实际上是计算机学科永远不变的核心内容，就算现在所谓的人工智能，在我看来也不过是一种计算或计算结果的应用。本文将从简单的例子出发，逐步推广到目前人工智能的前沿研究领域，阐述我理解的计算的概念，希望借此培养大家的计算式思维方式，我们将看到这种思维方式是可以上升到一种行为方式的。

## 什么是计算？

维基百科中，计算(Calculation) 是将一个或多个输入，利用称之为变量的东西，转换为一个或多个结果的一个精妙的过程。注意精妙这两个字，这里蕴含了一个重要的信息，计算需要一种巧妙的人类智慧。

在认知学领域中，计算是一种知识。知识按照表达方式可分为两大类：一是陈述性知识，主要用于描述实物的属性及相互关系的知识。第二类是过程式知识，主要用于表示推理和搜索相关事实等运用知识的过程。计算作为一种知识实际上就是一类过程式知识。为了更好的理解这两类知识的关系，我们从一个简单的例子开始我们对计算概念的理解。

首先看一个陈述性知识的例子: 

当y的平方等于x，且y为正时，y是x的平方根。

这一句话给出了关于什么是平方根的陈述性知识。接下来，我们来看一看，如何求解一个数的平方根？比如2的平方根是多少呢？平方根的定义本身并不能给我们太多的帮助。

## 从 ![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D) 谈起

2的平方根是多少？这个问题是否太简单？ 它可真不像你想的那样简单。有很多学生都会说这还用算吗，就是![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)？或者说就是1.414。是吗？![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)是一个无理数，求2的平方根指的是获得![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)的一个近似值，1.414无非是一种精确到小数点后3位的一个近似值罢了。如果我们要求更高一点呢，比如精确到小数点后10位的![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)的近似值是多少？或者的你可以换一种问法，1.414或更高精度的近似值到底是如何得到的呢？有很多学生感觉无从下手。一些学生则敢于迈出第一步，他们会利用平方根的定义，勇敢的说：先猜一个，![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)一定是介于1到2之间的值，因此从1.5开始猜，然后利用大家稍微动动脑子就能想到的类似折半的思想逐步逼近一个精度更高的近似值。

在这里我必须强调，很多(如果不是所有的话)计算的第一步都是猜一个值G，并且认为这个猜的值G就是你想要的。然后，通过定义一个检查的标准，在这个例子中我们可以用| ![G^{2}](https://www.zhihu.com/equation?tex=G%5E%7B2%7D) -2|<0.0000000001(或其他你想定义的任意小的值) 来判断，你所猜测的G是否表示满足这个标准。绝大多数情况下，刚开始的猜测不太可能满足这个标准。因此你需要再猜一个，你突然发现这种随机的猜测不会有太多的帮助，然后你试图利用你之所以为人的所特有的东西，大脑，来设计一个大多数人都能想象得到的方案，比如折半的思想，从而更新你的猜测，这次你根据自定的规则，猜的值为1.25。然后这个过程可以持续下去，最终将找到足够精度的![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)近似值。

你沾沾自喜，认为已经堪比数学家或者计算机科学家了。但是你要了解，他们追求的可是极致，他们的大脑才代表了人类的最高智慧。因为在猜测1.25的时候，你就已经输了，因为你清楚的知道1.5更接近![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)，但你依然退回到1.25这个离![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)更远的地方去了。你的方案存在的问题是，并不是每一步猜测都能越来越接近![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)，尽管宏观上是在逼近，因此需要更多的猜测(关于这一点还有争议，你可以先这么理解)。

因此，我们需要一种更加专业的知识来解决这个问题，古希腊数学家西罗给出了他的解决方案：
![](https://pic1.zhimg.com/v2-ff32ec5b7202a5e86b35d0e92f954290_b.jpg)
他只是修改了重新猜测的更新规则，使得每一步猜测都能逼近![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)罢了。大家可以自行分析这个式子，他甚至跟我们一样，也用了折半的思想，但却巧妙地设计了这个折半区间的上下界。这个过程可以表示为如下的流程图的形式。你可以修改那个评价标准，来获取你想要的任意精度的![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)的近似值。
![](https://pic4.zhimg.com/v2-e5beb59ea8c76777967345713046a9d7_b.jpg)图1 的计算过程
以上流程图是如此的重要，它实际上是一大类计算的基础框架，我们接下来就由此定义计算的概念，如下图2所示：
![](https://pic3.zhimg.com/v2-6a5e5cb0cb2bc2f459b5b758eab627a6_b.jpg)图2 计算：一种过程式知识
这种类型的计算首先需要设定一个计算的目标，通过定义一个可以量化的终止条件，然后从一个猜测的初始值开始，通过检验初始值是否满足计算目标所期望的条件，从而开始这个过程。如果满足条件，恭喜你，这个过程就结束了。否则，你需要更新你的猜测，这种更新对大多数人来讲都是困难的，需要人们(科学家们)智慧的灵光闪现。大多数计算机科学家所取得成就就是在“更新”上体现的。整个计算过程中，更新的好坏和计算终止条件的标准的高低，决定了你是否能够在足够短的时间内获得足够好的结果。

从宏观上看，计算就是一个逐步逼近满足条件的目标的过程，如下图3所示，需要注意的是，一般情况下， 我们的计算目标并不是唯一值，而是一个集合，由于初始值选取的不同，满足计算终止条件的计算结果不完全一样，但是他们都被认为是解空间中的正确解。

![](https://pic3.zhimg.com/v2-6f7a4b9d3201f96bc5a53aa6a8820646_b.jpg)图3 计算结果的更新过程
在本节的最后，需要指出的是图1或图2的流程图主要是为计算机实现做准备的，学过程序设计的同学都应该熟悉这种表示方法。这个流程图的结构是一个循环结构，这是计算机程序设计最伟大的发明了。我们用Python或C可以很快的实现这个程序。我认为你可以把它看成计算领域的”Hello world”。你现在应该知道，在计算还没有出现的古希腊时代，人类已经掌握了如何进行计算这样的精妙过程。现代电子计算机的出现只是让这个过程变成一瞬间。最后希望你能从两个方面感叹人类的伟大，1)总有聪明人设计出更好的解决方案，2)还有一些聪明人发明了计算机，从而让这个方案自动的执行。

不就是计算个![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)吗？好吧，我承认这个例子很简单，但也十分重要，我会将它推广一下(我们后面会发现很多机器学习的方法都可以在这个框架下解释，这也是本文的一个目标)，让时间回到1957年，看一看60年前人工智能发展的初期，计算的概念是如何在人工智能上体现的。

## 感知机

在计算机科学发展的初期，就有一些人开始考虑机器是否能替代人了。这些先驱们大概分为两派，一派人致力于让机器学会逻辑推理和知识表示，比如从已知条件自动获得某个知识(自动定理证明)，还有一些人则致力于让机器模拟大脑，从而具有智能，这一派被称之为神经网络学派。

我们先回到一个具体的问题：模式识别(人工智能的一个基本任务)，让计算机像人一样具有分辨物体类别的能力。例如，我们只需用苹果和橘子的图片或实物，告诉三岁的儿童一两次，当再次看到一种水果的时候，连小孩子都可以很快的认清这个水果是苹果还是橘子。这个过程现在最顶级的脑科学家估计都没搞清楚。但是勇敢的计算机科学家们还是准备让计算机完成这个任务，如图4(a)所示的二维平面上的两类数据，X代表苹果，O代表橘子。这些带有类别标签的数据是我们辛苦收集来的。计算机如何能够利用这些数据，对新来的未知类别的水果，如图4(b)所示的绿色三角，给出一个自己的判断呢？
![](https://pic1.zhimg.com/v2-d8b124c7ddd053094ebb667b8464d574_b.jpg)图 4 二分类问题
现在，让我们扮演计算机科学家的身份，开始解决这个问题。直观上看，为了让计算机识别二维平面上的两类数据，最简单的就是在二维平面上划一条线,如图4(c)所示，让这条直线把两类数据分开，然后就用这条直线作为对未知样本的分类线。首先确立模型的样式(直线)，然后用已知数据 (样本)，来确定模型的参数(直线的斜率和截距)，用这个确定的直线来预测未知数据的类别信息，这就是线性模式分类问题。

为什么是一条直线呢？当然也可以是其他的，但是目前，因为直线是在二维平面上除了点以外，最简单的一个模型，这个模型只需要用(斜率，截距)构成的点对来表示，即 就可以表示直线。现在我们的问题就是如何找到这条直线的参数。不要说这太简单了！大多数人把这种人类能做的看起来稀松平常的事看的太简单。对计算机而言，寻找这条直线的过程是使他们具有智能的第一步。那么，对计算机而言，到底如何找到这条直线？

如果你对上一节的![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)的计算过程真的理解的话， 你应该知道这就是一个计算过程，对计算机而言，第一步也只能是猜一个，猜一个点对如图5(a)，这就表示一条二维平面上的一条直线。评价标准呢？很简单，就是让直线把两类样本分开，可是你的运气实在不好，这条直线简直太糟糕了，不管是人还是计算机，通过简单的判断就知道这条直线不是我们要的。然后你就必须更新你的猜测，这个问题非常类似于求解，但是这一次，凭你的经验，折半思想在这里没用了， 因为在二维平面上，我们无法简单明了表示某个区域。你也可以重复的，无标准的猜下去，这样就全凭运气了，如图5(b)所示。
![](https://pic3.zhimg.com/v2-1ddd465fdeadf747388a5deea642b446_b.jpg)图 5 猜测的直线
其实科学家们所做的工作就是在杂乱无章的看似全凭运气的混乱世界中找到某种规律性的东西。解决这个问题的科学家就是康奈尔大学的伟大的人工智能先驱（也是一位心理学家）Frank Rosenblatt，他提出了一个寻找(计算)这条直线的方法，这种方法在当时感觉非常神奇，也使它的发明人名利双收。Rosenblatt的方法被称为感知机(Perceptron)。大家都知道现在有很多机器学习方法也被成为某某机，比如支持向量机，玻尔兹曼机。这些所谓的机器其实就是一个计算模型。感知机的方法很简单，但却开创了用数据本身进行“学习”的历史。(关于学习的概念，我们后面会详细说明。)
![](https://pic3.zhimg.com/v2-97d0f8e5d78bb8b3719eb2a880938c3a_b.jpg)![](https://pic1.zhimg.com/v2-7066dbbb68f1dba77fa5aba3b7f62510_b.jpg)
 感知机与计算![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)一样，第一步也是“猜”一条直线，然后检查这条直线是否能把两类样本能否分开。如果不能，则利用那些错分的样本来更新这条直线的参数。然后循环进行这个过程，直到找到一条直线把两类样本完全分开。Rosenblatt证明了只要样本是线性可分的，那么一定能找到这条直线，这种证明被称为收敛性证明。这种证明很重要，也很好理解，如果不能在理论上保证计算过程在合适的时间能够找到满足条件的解，那么这个计算的价值就不大。有意思的是，密码学的研究人员正好在做相反的事，他们就是在寻找一种没有更好计算模型的方案，这个方案被称为“计算困难问题”，依赖这种困难问题，来保护个人隐私的安全，也就是说他们就是想让你只能“瞎猜”，而无法找到一个有效的计算过程。你可以认为他们都是“反计算”派。

下面我们将重点阐述感知机计算过程中最重要的两个问题，1)计算的评价标准：如何判断直线是否把两类样本分开这个终止条件？2)更新过程：如何利用错分样本来更新这条直线的参数。第二点这是感知机最核心的内容。我会用简单且非严格的形式来说明这两个问题。

1) 如何判断直线两类样本分开

计算![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)的评价标准非常简单，对于感知机而言则稍微复杂一些，利用了直线的一些基本知识。在几何上，直线更多的表示成:
![](https://pic1.zhimg.com/v2-f18f9c5a4beab4ec125f26603129b580_b.jpg)
![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D) 其中 表示截距b， ![w_{1}/w_{2}](https://www.zhihu.com/equation?tex=w_%7B1%7D%2Fw_%7B2%7D) 是斜率w。给定,就能确定一条直线。 ![(w_{0},w_{1},w_{2})](https://www.zhihu.com/equation?tex=%28w_%7B0%7D%2Cw_%7B1%7D%2Cw_%7B2%7D%29) 满足这个方程的![(x_{1},x_{2})](https://www.zhihu.com/equation?tex=%28x_%7B1%7D%2Cx_%7B2%7D%29)构成了一条直线。那么在平面上的其他点 ![(x_{1}^{'},x_{2}^{'})](https://www.zhihu.com/equation?tex=%28x_%7B1%7D%5E%7B%27%7D%2Cx_%7B2%7D%5E%7B%27%7D%29) ，带入这个式子要么大于0，要么小于零，这可以看作是其他点到给定直线![(w_{0},w_{1},w_{2})](https://www.zhihu.com/equation?tex=%28w_%7B0%7D%2Cw_%7B1%7D%2Cw_%7B2%7D%29)的一种带符号的距离。感知机寻找的直线把样本分开，其实就是让某一类的数据点满足到直线的距离大于0，同时让另一类的点到直线的距离小于0. 由于我们不用关心点到直线距离的具体数值，因此采用一个函数将距离大于零的点标记成1，距离小于零的点，标记成-1. 也就是说我们需要一个函数：
![](https://pic4.zhimg.com/v2-bbb857c2cb789676c3d9a2806b0542af_b.jpg)
这个函数根据点到直线![(w_{0},w_{1},w_{2})](https://www.zhihu.com/equation?tex=%28w_%7B0%7D%2Cw_%7B1%7D%2Cw_%7B2%7D%29)的距离，大于零输出+1和小于零输出-1。现在如果我们给这些已知的数据带上标记，例如，O(+1),X(-1) ， 那么我们现在要做的就是要根据已有的数据点，来寻找这样的一条直线，使所有的点都符合它自己本身的标记。实际上，关于+1，-1是哪一类完全根据你的意愿。
![](https://pic4.zhimg.com/v2-4ad73ee99781166589b62812207522f3_b.jpg)图 6 分类直线
你一眼就能看到图6(a)的直线不满足这个条件，而图6(b)满足。

关键的问题是计算机是如何知道自己做的好不好呢？我们必须再定义一个指示标记，让计算机自己了解它是否正确，这个指标被称为代价函数和损失函数，一般而言，这个函数的输出值表示了再当前参数情况下，模型对数据处理效果的好坏，损失或代价越小，则表明模型的参数的选择对已知数据的处理效果越好。感知机的损失函数很简单，既然我们希望直线的输出结果h(x)尽量满足O(+1),X(-1)。我们把数据自带的标签记作y，则感知机的代价函数或损失函数就变成：
![](https://pic1.zhimg.com/v2-0d21f2951709ad2dfd58773a33db3f44_b.jpg)
这是由于所有被正确分类的样本，无论是橘子还是苹果，y*h(x)都是1，增加一个负号再和零取最大值，则表明那些被正确分类的样本没有让直线产生损失，只有那些错分样本，会出现-y*h(x)>0, 他们会让感知机猜测的当前直线产生损失(代价)，损失越大，这条直线就越不舒服，它必须进行变化。现在我们应该已经清楚，对于能够用直线分开的数据样本而言，这条之间一定能够找到， 而且满足条件的直线产生的代价是0。

更新过程

下图是一个动态的图，是错分的样本(绿色数据点)让感知机不舒服(代价太大)，他必须做出改变，直到分类成功，才会停止。
![](https://pic4.zhimg.com/v2-1e65d9f31f2d968b653bfd9c1f0af117_b.jpg)图7 感知机动态更新
那么这种眼花缭乱的更新到底是怎么进行的？感知机的更新策略就是依赖这些错分样本，来更新直线的参数，假设**p**是一个数据点,这里我们把数据点(扩展成完全是为了跟直线参数对应，把前面的系数1显示的表示出来。根据直线与的关系, 参数的更新过程如下：
![](https://pic4.zhimg.com/v2-1057d48e42d177ee9109c2928ec97703_b.jpg)
稍微整理下， 就可以写成下面的式子：
![](https://pic2.zhimg.com/v2-5e0a8ffd0053eef1b3f6c9c8c9f0685d_b.jpg)
完全依赖错分样本来进行更新,也就是说，如果分类错误，则直线就被错分点更新，分类正确，则这个点对直线没有什么影数据点，因此检查更新的时候需要逐点检查，遇到错误的点就更新，然后重新检查所有的点。下面这个图8展示了这个基本过程，其中紫色的向量表示当前直线的法向量**w**，错分样本将会更新法向量**w**：

![](https://pic3.zhimg.com/v2-81d81fb5b4b1d3a4c328795fdcea520e_b.jpg)图 8 感知机更新过程
这个过程也可以用通俗的比喻来解释，假设你和朋友们在玩一个游戏，把你自己想象成一条直线，你的朋友对你来说分为两类，男性朋友和女性朋友，并且他们所在的位置可以由一条直线分开。现在你从天而降，随机的散布在朋友中间，你需要达成一个目标就是让你的女性朋友都在你眼前，而让你男性朋友都在你背后，但是你自己不能转动以及移动，必须依赖你周围的朋友们完成这个工作。那些在你背后的女性朋友会拉你一把让你面向她，在你面前的男性朋友则会推你一把，让你背对他，每一次只有一个被你错分的朋友才能让你旋转和移动。如果你的朋友不够多的话，在你还能站稳的时候，你就可以停下来欣赏她们了。如果你善于交际，估计停下来的时候，你也眼花了。你可以立刻开始这个游戏，然后自豪的告诉他们，这是在玩最古老的人工智能游戏-感知机。

我们现在来谈一谈感知机与求解![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)的类似之处，从图9的结构上看几乎没什么区别：
![](https://pic4.zhimg.com/v2-cef332603b85b074b1af47c445871187_b.jpg)图9 感知机计算过程
两者都是从一个初始值开始，然后定义一个标准，![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)的标准就是距离，你可以把它看作是跟感知机一样由猜测值产生的代价。你猜的离真实值越远，代价越大。当代价小于一个很小的值的时候，证明猜对了。在感知机中，是利用错分样本来更新，这种更新基于数据。而在![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)的更新中，其实也可以看作是利用错误去更新，你可以想象得到，当你猜测的值偏离![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)越远(错的越远)，下一次更新时，新的值改进的幅度就越大。还有一个相同点，就是满足条件的的近似值与能够把样本分开的直线都有无穷多个。![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)与感知机的区别在于，![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)不是基于数据的，而是依赖一种仔细的观察。不管怎么说，这两者的更新方式都依赖于人类的智慧。

但是，以上两者本质上却有截然不同的内涵，如果我们认为感知机只是来寻找直线的话那么感知机和![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)确实没什么太多不同。感知机的根本目标其实不是只是为了寻找这条直线。感知机更宏大的目标是要利用这条直线来处理未来的未知样本。这是机器学习最为核心的概念。利用数据计算(估计)模型的参数，只完成了一步，更为重要的是利用这个拥有较好参数的模型来处理未知数据。如果处理未知数据的能力较差的话，之前得到的计算结果也是毫无意义的，在这一点上这就是感知机和![\sqrt{2}](https://www.zhihu.com/equation?tex=%5Csqrt%7B2%7D)最本质的区别。每当出现这种毫无意义结果的时候，专业人士把这种无用功称之为对已知数据的“过拟合”。这里我们可以停下来，简单谈谈计算与学习的关系。在机器学习领域，计算很大程度上只是一种引入了人类智慧的一种固定模式，比如用于获得某种模型在某种代价下的一个参数，当这个过程结束的时候，我们可以认为计算结束了。但是，当我们开始利用这个带有参数的模型处理真正的问题的时候，考验学习效果的旅程才刚刚开始。

推荐阅读

[1]  [机器学习-波澜壮阔40年](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483705%26idx%3D1%26sn%3Dc6e7c4a2e14a2469308b41eb60f155ac%26chksm%3Dfdb69caecac115b8712653600e526e99a3f6976fdaa2f6b6a09388fa6f9677ccb57b40c40ae3%26scene%3D21%23wechat_redirect) SIGAI 2018.4.13.

[2]  [学好机器学习需要哪些数学知识？](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483713%26idx%3D1%26sn%3D1e7c81381d16806ac73e15691fe17aec%26chksm%3Dfdb69cd6cac115c05f1f90b0407e3f8ae9be8719e454f908074ac0d079885b5c134e2d60fd64%26scene%3D21%23wechat_redirect)SIGAI 2018.4.17.

[3]  [人脸识别算法演化史](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483726%26idx%3D1%26sn%3D9fef4cc1766ea4258749f8d40cc71a6e%26chksm%3Dfdb69cd9cac115cf4eba16081780c3b64c75e1e55a40bf2782783d5c28f00c6f143426e6f0aa%26scene%3D21%23wechat_redirect) SIGAI 2018.4.20.

[4]  [基于深度学习的目标检测算法综述](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483731%26idx%3D1%26sn%3D237c52bc9ddfe65779b73ef8b5507f3c%26chksm%3Dfdb69cc4cac115d2ca505e0deb975960a792a0106a5314ffe3052f8e02a75c9fef458fd3aca2%26scene%3D21%23wechat_redirect) SIGAI 2018.4.24.

[5]  [卷积神经网络为什么能够称霸计算机视觉领域？](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483816%26idx%3D1%26sn%3Dfc52765b012771d4736c9be4109f910e%26chksm%3Dfdb69c3fcac115290020c3dd0d677d987086a031c1bde3429339bb3b5bbc0aa154e76325c225%26scene%3D21%23wechat_redirect) SIGAI 2018.4.26.

[6] [用一张图理解SVM的脉络](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483937%26idx%3D1%26sn%3D84a5acf12e96727b13fd7d456c414c12%26chksm%3Dfdb69fb6cac116a02dc68d948958ee731a4ae2b6c3d81196822b665224d9dab21d0f2fccb329%26scene%3D21%23wechat_redirect)SIGAI 2018.4.28.

[7] [人脸检测算法综述](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483950%26idx%3D1%26sn%3Da3a5b7907b2552c233f654a529931776%26chksm%3Dfdb69fb9cac116af5dd237cf987e56d12b0d2e54c5c565aab752f3e366c0c45bfefa76f5ed16%26scene%3D21%23wechat_redirect) SIGAI 2018.5.3.

[8] [理解神经网络的激活函数](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483977%26idx%3D1%26sn%3D401b211bf72bc70f733d6ac90f7352cc%26chksm%3Dfdb69fdecac116c81aad9e5adae42142d67f50258106f501af07dc651d2c1473c52fad8678c3%26scene%3D21%23wechat_redirect) SIGAI 2018.5.5.

[9] [深度卷积神经网络演化历史及结构改进脉络-40页长文全面解读](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484037%26idx%3D1%26sn%3D13ad0d521b6a3578ff031e14950b41f4%26chksm%3Dfdb69f12cac11604a42ccb37913c56001a11c65a8d1125c4a9aeba1aed570a751cb400d276b6%26scene%3D21%23wechat_redirect) SIGAI        2018.5.8.

[10] [理解梯度下降法](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484111%26idx%3D1%26sn%3D4ed4480e849298a0aff828611e18f1a8%26chksm%3Dfdb69f58cac1164e844726bd429862eb7b38d22509eb4d1826eb851036460cb7ca5a8de7b9bb%26scene%3D21%23wechat_redirect) SIGAI 2018.5.11

[11] [循环神经网络综述—语音识别与自然语言处理的利器](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484310%26idx%3D1%26sn%3D0fc55a2784a894100a1ae64d7dbfa23d%26chksm%3Dfdb69e01cac1171758cb021fc8779952e55de41032a66ee5417bd3e826bf703247e243654bd0%26scene%3D21%23wechat_redirect) SIGAI 2018.5.15

[12] [理解凸优化](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484439%26idx%3D1%26sn%3D4fa8c71ae9cb777d6e97ebd0dd8672e7%26chksm%3Dfdb69980cac110960e08c63061e0719a8dc7945606eeef460404dc2eb21b4f5bdb434fb56f92%26scene%3D21%23wechat_redirect)  SIGAI 2018.5.18

[13][【实验】理解SVM的核函数和参数](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484495%26idx%3D1%26sn%3D4f3a6ce21cdd1a048e402ed05c9ead91%26chksm%3Dfdb699d8cac110ce53f4fc5e417e107f839059cb76d3cbf640c6f56620f90f8fb4e7f6ee02f9%26scene%3D21%23wechat_redirect) SIGAI 2018.5.22

[14][【SIGAI综述】 行人检测算法](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484523%26idx%3D1%26sn%3Dddaa70c4b92f6005d9bbd6b3a3fe4571%26chksm%3Dfdb699fccac110ea14e6adeb873a00d6ee86dd4145ddf8e90c9b878b908ac7b7655cfa51dab6%26scene%3D21%23wechat_redirect) SIGAI 2018.5.25

[15] [机器学习在自动驾驶中的应用—以百度阿波罗平台为例](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484540%26idx%3D1%26sn%3D733332042c31e1e18ad800f7f527893b%26chksm%3Dfdb699ebcac110fd6607c1c99bc7ebed1594a8d00bc454b63d7f518191bd72274f7e61ca5187%23rd)（上） SIGAI 2018.5.29 

[16] [理解牛顿法](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484651%26idx%3D1%26sn%3Da0e4ca5edb868fe3eae9101b71dd7103%26chksm%3Dfdb6997ccac1106a61f51fe9f8fd532045cc5d13f6c75c2cbbf1a7c94c58bcdf5f2a6661facd%26scene%3D21%23wechat_redirect) SIGAI 2018.5.31

[17] [【群话题精华】5月集锦—机器学习和深度学习中一些值得思考的问题](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484658%26idx%3D1%26sn%3Df5c9f92c272c75883bf8e6f532559f11%26chksm%3Dfdb69965cac11073f49048caef5d7b9129614090a363d9ef7f3d1b9bc59948d2217d2bca7b7b%26scene%3D21%23wechat_redirect)SIGAI 2018.6.1

[18] [大话Adaboost算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484692%26idx%3D1%26sn%3D9b389aa65208c778dddf17c601afbee1%26chksm%3Dfdb69883cac1119593934734e94c3b71aa68de67bda8a946c1f9f9e1209c3b6f0bf18fed99b8%23rd) SIGAI 2018.6.1

[19] [FlowNet到FlowNet2.0：基于卷积神经网络的光流预测算法](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484711%26idx%3D1%26sn%3Dbb7644e101b5924f54d6800b952dc3aa%26chksm%3Dfdb698b0cac111a6605f5b9b6f0478bf21a8527cfad2342dbaaf624b4e9dcc43c0d85ae06deb%26scene%3D21%23wechat_redirect) SIGAI 2018.6.4

[20] [理解主成分分析法(PCA)](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484754%26idx%3D1%26sn%3Db2c0d6798f44e13956bb42373e51d18c%26chksm%3Dfdb698c5cac111d3e3dca24c50aafbfb61e5b05c5df5b603067bb7edec8db049370b73046b24%23rd) SIGAI 2018.6.6

[21]  [人体骨骼关键点检测](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484784%26idx%3D1%26sn%3Dceafb54203f4e930ae457ad392b9f89c%26chksm%3Dfdb698e7cac111f13d8229d7dcc00b4a7305d66de3da1bd41e7ecc1d29bfa7be520d205c53e9%23rd) SIGAI 2018.6.8

[22] [理解决策树](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484827%26idx%3D1%26sn%3D043d7d0159baaddfbf92ed78ee5b1124%26chksm%3Dfdb6980ccac1111a9faeae7f517fee46a1dfab19612f76ccfe5417487b3f090ab8fc702d18b8%23rd) SIGAI 2018.6.11

[23] [用一句话总结各种机器学习算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484859%26idx%3D1%26sn%3D2c4db22fb538953a62a90983e3e1f99d%26chksm%3Dfdb6982ccac1113a82e92be325bb07a947d54090274654375f3b50e11e1abd809fb7358bde16%23rd) SIGAI 2018.6.13

[24] [目标检测算法之YOLO](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484909%26idx%3D1%26sn%3Dc02ee17e5175230ed39ad63e73249f5c%26chksm%3Dfdb6987acac1116c0108ec28424baf4ea16ca11d2b13f20d4a825d7b2b82fb8765720ebd1063%23rd) SIGAI 2018.6.15

[25] [理解过拟合](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484954%26idx%3D1%26sn%3Dc28b7f07c22466e91b1ef90e9dbe3ad1%26chksm%3Dfdb69b8dcac1129bc6e78fca1d550e2b18238ad1c240c73b280d4e529f9f93c4626b3ac45ea2%23rd) SIGAI 2018.6.18

[http://weixin.qq.com/r/Li727lLEuQ-GrRaN93s8](https://link.zhihu.com/?target=http%3A//weixin.qq.com/r/Li727lLEuQ-GrRaN93s8) (二维码自动识别)

