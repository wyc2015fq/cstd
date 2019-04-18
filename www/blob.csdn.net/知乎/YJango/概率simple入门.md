# 概率simple入门 - 知乎
# 

该篇内容是Joseph K. Blitzstein的《[Introduction to Probability](https://link.zhihu.com/?target=http%3A//projects.iq.harvard.edu/stat110/about)》中的第一章，该概率书非常适合入门。文章是加入自己理解后的总结。以服务于机器学习的理解视角切入。

通过线性代数，我们知道了该如何描述事物状态及其变化。遗憾的是，对一个微小的生物而言，世界并非确定性（nondeterministic）的，由于感知限制，很多事物是无法确定其状态的。然而为了更好的生存，预测未来状态以决定下一刻的行为至关重要。而概率给我们的决策提供了依据。

**一、什么是概率？**

> 概率是我们对事件处于哪个状态的**确信度**。
- **情景：**如何考虑转盘在未来停止后指针指向各个数字的可能性？为方便研究，需要总结出在任何情况都普遍适用的属性，并给予它们固定的名字。
1,2,3是可能被指到的三个结果（outcome）。在这里，这三个结果组成的集合也同时是样本空间（sample space），即无论事态如何发展，结果都不会出现在该集合之外（和向量空间一样）。样本空间的子集，如{1,2}叫做一个事件（event），表示指针指到1或2的情况。满足任何一个情况都算作该事件发生了（occurred）。所有事件发生的可能性都用值域为[0,1]间的实数表示，1表示必然发生，0表示不可能发生。{1}, {2,3}两个不相交的事件的概率和为1。[0,1]间的实数是概率得出的值，但并非概率的全部。概率是一个函数。
![](https://pic4.zhimg.com/v2-746ff9fde965cb61f90d83e712ba4c67_b.jpg)
> **概率：**概率是将样本空间内的子集投向概率空间的函数。
概率![P( )](https://www.zhihu.com/equation?tex=P%28+%29)将事件![A\subset S](https://www.zhihu.com/equation?tex=A%5Csubset+S)作为输入，并输出![[0,1]](https://www.zhihu.com/equation?tex=%5B0%2C1%5D)之间的实数表示其发生的可能性。该函数需要满足两个条件：
1.![P( \emptyset)=0, P(S)=1](https://www.zhihu.com/equation?tex=P%28+%5Cemptyset%29%3D0%2C+P%28S%29%3D1)，空集的概率为0，全集的概率为1。
2.![P(\bigcup\limits_{j=1}^{\infty}A_{j})=\sum\limits_{j=1}^{\infty}P(A_{j})](https://www.zhihu.com/equation?tex=P%28%5Cbigcup%5Climits_%7Bj%3D1%7D%5E%7B%5Cinfty%7DA_%7Bj%7D%29%3D%5Csum%5Climits_%7Bj%3D1%7D%5E%7B%5Cinfty%7DP%28A_%7Bj%7D%29)，不相交事件之间的并集事件的概率等于各个事件概率之和。
**结果：**可能到达的状态
**样本空间：**所有可能发生的结果所组成的集合。
**事件：**样本空间的子集。
当实际发生的结果![s_{outcome}\in A](https://www.zhihu.com/equation?tex=s_%7Boutcome%7D%5Cin+A)时，表示![A](https://www.zhihu.com/equation?tex=A)事件发生。

**二、朴素概率的计算以及和普遍概率的区别是什么？**

人们在计算概率时常常犯的错误就是不假思索的假定所有结果所发生的可能性都相同。并用事件的结果个数比上样本空间的结果个数。

> 朴素概率：![P_{naive}(A)=|A|/|S|](https://www.zhihu.com/equation?tex=P_%7Bnaive%7D%28A%29%3D%7CA%7C%2F%7CS%7C)，![|A|](https://www.zhihu.com/equation?tex=%7CA%7C)和![|S|](https://www.zhihu.com/equation?tex=%7CS%7C)表示集合中元素的个数。

但是这种假设并不严谨。
- 实例：在上图原盘问题中，如果使用朴素概率来计算指针停止时指向2的概率，就会得到![P_{naive}=|A|/|S|=1/3](https://www.zhihu.com/equation?tex=P_%7Bnaive%7D%3D%7CA%7C%2F%7CS%7C%3D1%2F3)的概率。但很明显，指向3的结果就占有原盘一半的空间，指向3的概率更大。使得各个结果发生的可能性并不相同。不可以使用朴素概率算法。从图中可以看出答案是![1/4](https://www.zhihu.com/equation?tex=1%2F4)。

**样本空间好比是总价为1的一筐苹果，一个事件就是一堆苹果，概率是将这堆苹果转换成实际价钱的函数。但苹果有大有小，只有当所有苹果都一模一样时，这堆苹果的价钱才是 苹果数/总个数。空集，即一个苹果都没有的话，价格为0。整框苹果的话，价格自然为1。把整框苹果分成几堆（事件之间不相交），价格的总和为1。**
![](https://pic3.zhimg.com/v2-c61d2008f6fbdb7f298d1d3ad16c8bae_b.jpg)
## **条件概率**

当我们获得更多信息后，新信息会对原始样本空间产生更新。

**三、条件概率又是什么？**

条件概率是新信息对样本空间进行调整后的概率情况。
- **实例：**从一副洗好的扑克里，不放回的依次抽两张卡片。事件![A](https://www.zhihu.com/equation?tex=A)表示第一张卡片是心，事件![B](https://www.zhihu.com/equation?tex=B)表示第二张卡片是红色。求事件![B](https://www.zhihu.com/equation?tex=B)发生的条件下，事件![A](https://www.zhihu.com/equation?tex=A)发生的概率![P(A|B)](https://www.zhihu.com/equation?tex=P%28A%7CB%29)。以及事件![A](https://www.zhihu.com/equation?tex=A)发生的条件下，事件![B](https://www.zhihu.com/equation?tex=B)发生的概率![P(B|A)](https://www.zhihu.com/equation?tex=P%28B%7CA%29)。
卡片都是均匀形状，可用朴素概率计算。最初的样本空间是 54∗53=2862 种。事件B发生后，样本空间被调整，所有第二张不是红色的结果都会从样本空间内去掉，变成 26∗53=1378种（可认为第二张先抓，顺序不影响组合结果）。其中第一张是心，且第二张是红色的结果有13∗25=325种。所以![P(A|B)](https://www.zhihu.com/equation?tex=P%28A%7CB%29)的概率为 325/1378≈0.236。
事件![A](https://www.zhihu.com/equation?tex=A)发生后，所有第一张不是心的结果都会从样本空间内去掉，变成13∗53=689种。其中第一张是心，且第二张是红色的结果有 13∗25=325种。所以![P(B|A)](https://www.zhihu.com/equation?tex=P%28B%7CA%29)的概率为325/689≈0.472。
![P(A|B)](https://www.zhihu.com/equation?tex=P%28A%7CB%29)和![P(B|A)](https://www.zhihu.com/equation?tex=P%28B%7CA%29)二者的条件对原始样本空间的调整不同，所以并不相等。同时“|”右边的事件并不意味着先发生，也并不意味着是左边事件的起因。
- **实例：**先后投两次硬币。原始样本空间是{正正，反反，正反，反正}。已知事件![A](https://www.zhihu.com/equation?tex=A)是第一次投得正面，事件![B](https://www.zhihu.com/equation?tex=B)是第二次投得正面。![P(B|A)](https://www.zhihu.com/equation?tex=P%28B%7CA%29)更新后的样本空间为{正正，正反}。但第二次投得正面的概率仍然是1/2。事件![A](https://www.zhihu.com/equation?tex=A)和事件![B](https://www.zhihu.com/equation?tex=B)彼此没有影响，叫做两个事件独立。

> **条件概率：**![P(A|B)=P(A∩B)/P(B)P(A|B)=P(A∩B)/P(B)](https://www.zhihu.com/equation?tex=P%28A%7CB%29%3DP%28A%E2%88%A9B%29%2FP%28B%29P%28A%7CB%29%3DP%28A%E2%88%A9B%29%2FP%28B%29)
![P(A|B)](https://www.zhihu.com/equation?tex=P%28A%7CB%29)表示![B](https://www.zhihu.com/equation?tex=B)事件条件下，![A](https://www.zhihu.com/equation?tex=A)发生的条件概率。
![P(A)](https://www.zhihu.com/equation?tex=P%28A%29)叫做先验概率（prior probability），即事态未更新时，![A](https://www.zhihu.com/equation?tex=A)事件的概率。
![P(A|B)](https://www.zhihu.com/equation?tex=P%28A%7CB%29)也叫做后验概率（posterior probability），即事态更新后，![A](https://www.zhihu.com/equation?tex=A)事件的概率。
![P(A∩B)](https://www.zhihu.com/equation?tex=P%28A%E2%88%A9B%29)是![B](https://www.zhihu.com/equation?tex=B)发生后![A](https://www.zhihu.com/equation?tex=A)的事件集合，而除以![P(B)](https://www.zhihu.com/equation?tex=P%28B%29)是在该基础上，将样本空间的总概率重新调整为1。
当事件![A](https://www.zhihu.com/equation?tex=A)与![B](https://www.zhihu.com/equation?tex=B)为独立事件时，其中一个事件的发生并不会对另一个事件的样本空间产生影响。即![P(A|B)=P(A)P(A|B)=P(A)](https://www.zhihu.com/equation?tex=P%28A%7CB%29%3DP%28A%29P%28A%7CB%29%3DP%28A%29)，![P(B|A)=P(B)P(B|A)=P(B)](https://www.zhihu.com/equation?tex=P%28B%7CA%29%3DP%28B%29P%28B%7CA%29%3DP%28B%29)。

## **贝叶斯公式**

人们经常将![P(A|B)](https://www.zhihu.com/equation?tex=P%28A%7CB%29)和![P(B|A)](https://www.zhihu.com/equation?tex=P%28B%7CA%29)搞混，把二者搞混的现象叫做检察官谬误（prosecutor's fallacy）。

**四、![P(A|B)](https://www.zhihu.com/equation?tex=P%28A%7CB%29)和![P(B|A)](https://www.zhihu.com/equation?tex=P%28B%7CA%29)两者之间的关系是什么？**
- **实例：**某机器对在所有人口中得病率为1%的癌症识别率为95%（有病的人被测出患病的概率和没病的人被测出健康的概率）。一个被测得有病的人真实患癌症的概率是多少？
得出答案是95%的人就是搞混了![P(A|B)](https://www.zhihu.com/equation?tex=P%28A%7CB%29)和![P(B|A)](https://www.zhihu.com/equation?tex=P%28B%7CA%29)。正确答案约等于16%。拿10000个人来思考。

真正的样本空间是由测得有病的癌症患者和测得有病的正常人组成，所以答案是95/(95+495)≈16%。

我们知道条件概率是新信息对样本空间进行调整后的概率情况，所以检察官谬误实际上是样本空间的更新产生了差错。不过我们可以从条件概率中寻找关系：通过变形条件概率的定义，就可以得出著名的贝叶斯公式和全概率公式。

> **贝叶斯公式（Bayes' theorem）：**![P(A|B)=P(B|A)P(A)/P(B)](https://www.zhihu.com/equation?tex=P%28A%7CB%29%3DP%28B%7CA%29P%28A%29%2FP%28B%29)
**全概率公式（Law of total probability）：**![P(B)=\sum\limits_{i}^{n}P(B|A_{i})P(A_{i})](https://www.zhihu.com/equation?tex=P%28B%29%3D%5Csum%5Climits_%7Bi%7D%5E%7Bn%7DP%28B%7CA_%7Bi%7D%29P%28A_%7Bi%7D%29)，
其中![A_{i}](https://www.zhihu.com/equation?tex=A_%7Bi%7D)是样本空间![S](https://www.zhihu.com/equation?tex=S)的分割(partition)，即彼此不相交，并且组成的并集是样本空间。
如下图：
![](https://pic1.zhimg.com/v2-4598c108df88f9f8119757eab0e318c8_b.jpg)
用这两个公式，我们重新计算上面的癌症问题：
- **实例： **其中 ![P(A) ](https://www.zhihu.com/equation?tex=P%28A%29+)是人口中患癌症的概率，为1%，![P(B)](https://www.zhihu.com/equation?tex=P%28B%29)是测得有病的概率。![P(A|B)](https://www.zhihu.com/equation?tex=P%28A%7CB%29)就是测得有病时，患癌症的概率。 ![P(B|A)](https://www.zhihu.com/equation?tex=P%28B%7CA%29)是有患癌症时，测得有病的概率，为95%。![P(B|AC)](https://www.zhihu.com/equation?tex=P%28B%7CAC%29)就是没病时却测得有癌症的概率，为5%。
想知道的是，当被测得有病时，真正患癌症的概率![P(A|B)](https://www.zhihu.com/equation?tex=P%28A%7CB%29)是多少。
由贝叶斯公式可以得到：![P(A|B)=P(B|A)P(A)/P(B)=0.95*0.01/P(B)](https://www.zhihu.com/equation?tex=P%28A%7CB%29%3DP%28B%7CA%29P%28A%29%2FP%28B%29%3D0.95%2A0.01%2FP%28B%29)
由全概率公式可以得到：![P(B)=P(B|A)P(A)+P(B|A^{C})P(A^{C})](https://www.zhihu.com/equation?tex=P%28B%29%3DP%28B%7CA%29P%28A%29%2BP%28B%7CA%5E%7BC%7D%29P%28A%5E%7BC%7D%29)
全部代入就得到：![0.95*0.01/(0.95*0.01+0.05*0.99)\approx 16](https://www.zhihu.com/equation?tex=0.95%2A0.01%2F%280.95%2A0.01%2B0.05%2A0.99%29%5Capprox+16)%

这两个公式在机器学习中非常重要。贝叶斯公式告诉了我们![P(A|B)](https://www.zhihu.com/equation?tex=P%28A%7CB%29)和![P(B|A)](https://www.zhihu.com/equation?tex=P%28B%7CA%29)两者之间的关系。很多时候，我们难以得出其中一个的时候，可以改求另一个。
- **实例：**语音识别中，听到某串声音的条件![o](https://www.zhihu.com/equation?tex=o)下，该声音是某段语音![s](https://www.zhihu.com/equation?tex=s)的条件概率最大的![\arg\max_{w} P(s|o)](https://www.zhihu.com/equation?tex=%5Carg%5Cmax_%7Bw%7D+P%28s%7Co%29)为识别结果。然而![P(s|o)](https://www.zhihu.com/equation?tex=P%28s%7Co%29)并不好求。所以改求![P(s|o)=P(o|s)P(s)/P(o)](https://www.zhihu.com/equation?tex=P%28s%7Co%29%3DP%28o%7Cs%29P%28s%29%2FP%28o%29)。![P(o)](https://www.zhihu.com/equation?tex=P%28o%29)对比较同一个![P(s|o)](https://www.zhihu.com/equation?tex=P%28s%7Co%29)时并没有影响，因为大家都有，则不需要考虑。剩下的![P(o|s)](https://www.zhihu.com/equation?tex=P%28o%7Cs%29)叫做声学模型，描述该段语音会发出什么样的声音。而![P(s)](https://www.zhihu.com/equation?tex=P%28s%29)叫做语言模型，包含着语法规则信息。

而全概率公式又是连接条件概率与非条件概率的桥梁。

> 全概率公式可以将非条件概率，分成若干块条件概率来计算。
- **实例：**[三门问题](https://link.zhihu.com/?target=http%3A//baike.baidu.com/item/%25E4%25B8%2589%25E9%2597%25A8%25E9%2597%25AE%25E9%25A2%2598)。三扇门中有一扇门后是汽车，其余是羊。参赛者会先被要求选择一扇门。这时主持人会打开后面是羊的一扇门，并给参赛者换到另一扇门的机会。问题是参赛者该不该换？ 应该换门。换门后获得汽车的概率为2/3，不换门的概率为1/3。
用全概率公式来思考该问题就可以将问题拆分成若干个相对简单的条件概率。
![P(get car)](https://www.zhihu.com/equation?tex=P%28get+car%29)获得汽车的概率可以用拆分成选择各个门可得汽车的概率。![P(D_{1})](https://www.zhihu.com/equation?tex=P%28D_%7B1%7D%29)为车在第一扇门的概率。 ![P(get car)=P(get car|D_{1})P(D_{1})+P(get car|D_{2})P(D_{2})+P(get car|D_{3})P(D_{3})](https://www.zhihu.com/equation?tex=P%28get+car%29%3DP%28get+car%7CD_%7B1%7D%29P%28D_%7B1%7D%29%2BP%28get+car%7CD_%7B2%7D%29P%28D_%7B2%7D%29%2BP%28get+car%7CD_%7B3%7D%29P%28D_%7B3%7D%29)
![P(get car)=P(get car|D_{1})*1/3+P(get car|D_{2})*1/3+P(get car|D_{3})*1/3](https://www.zhihu.com/equation?tex=P%28get+car%29%3DP%28get+car%7CD_%7B1%7D%29%2A1%2F3%2BP%28get+car%7CD_%7B2%7D%29%2A1%2F3%2BP%28get+car%7CD_%7B3%7D%29%2A1%2F3)如果不换门，得车的概率就是![P(D_1)](https://www.zhihu.com/equation?tex=P%28D_1%29)，即1/3.
若换门。当车在第一扇门后时，![P(get car|D_{1})*1/3](https://www.zhihu.com/equation?tex=P%28get+car%7CD_%7B1%7D%29%2A1%2F3)由于换门的选择而变成了0。
但当车在第二或第三扇门后时，由于主持人去掉了一扇后面为羊的门，换门的选择会100%得到车。
所以，![P(get car)=0*1/3+1*1/3+1*1/3=2/3](https://www.zhihu.com/equation?tex=P%28get+car%29%3D0%2A1%2F3%2B1%2A1%2F3%2B1%2A1%2F3%3D2%2F3)

## **随机变量**

**五、是否有更好的方式表达事件？**

随机变量是一种非常方便的事件表达方式。虽然它的名字叫做随机变量，但它实际上是一个函数。 我们在“什么是概率”的例子中已经应用了随机变量的概念。我们用数字去表达事件。比较一下不用随机变量的方式。
- **实例：**我们用文字去表达事件和概率。样本空间 ![\Omega](https://www.zhihu.com/equation?tex=%5COmega) = { 橘黄色，绿色，蓝色 }。
情况1：若仅仅是问转盘停止后指针指到某个颜色的概率还可以接受。如![P](https://www.zhihu.com/equation?tex=P)(指到橘黄色)。
情况2：如果是奖励游戏，转到橘黄、绿、蓝色分别奖励1、2、3元。转3次后，想知道奖励了多少钱的概率。3元的我们要写一次描述，4元的也要写一次描述。十分笨拙。如果想问的是美元呢？我们又没办法用事件去乘以汇率。

然而如果用随机变量，就变得非常方便。设![X_{r}](https://www.zhihu.com/equation?tex=X_%7Br%7D)表示转 ![r](https://www.zhihu.com/equation?tex=r)次后一共奖励了多少人民币。 ![c](https://www.zhihu.com/equation?tex=c)是人民币对美元汇率的话，![c \cdot X_{r}](https://www.zhihu.com/equation?tex=c+%5Ccdot+X_%7Br%7D)就表示表示转 ![r](https://www.zhihu.com/equation?tex=r)次后一共奖励了多少美元。 ![X_{r+1}-X(r)](https://www.zhihu.com/equation?tex=X_%7Br%2B1%7D-X%28r%29)就表示了下一局赢得了多少人民币。

> 随机变量：给定一个样本空间 ![\Omega](https://www.zhihu.com/equation?tex=%5COmega)一个随机变量(![r.v.](https://www.zhihu.com/equation?tex=r.v.))是将样本空间投射到实数域的函数。

一个样本空间可以有很多个随机变量。在最初的例子，我们就已经将样本空间![\Omega](https://www.zhihu.com/equation?tex=%5COmega)={橘黄色，绿色，蓝色}对应到了实数域中的1,2,3。
![](https://pic3.zhimg.com/v2-f328f131d915bd589d9c195bee4a3dc2_b.jpg)

随机变量作为函数而言是确定的。输入事件橘黄色，一定会得到1这个输出，函数本身并没有什么“随机”。“随机”是由于函数的输入的发生概率。

![X=3](https://www.zhihu.com/equation?tex=X%3D3)表达的是指针指到蓝色的事件。![P(X=3)](https://www.zhihu.com/equation?tex=P%28X%3D3%29)表达指针指到蓝色的事件的概率。

随机变量是认为事先选择的，非常灵活，好的随机变量会使问题简化许多。

根据随机变量投射后的值域是离散还是连续，随机变量可以分为离散随机变量和连续随机变量。

## **分布**

随机变量中的“随机”来自事件发生的概率。分布（distribution）是描述随机变量所对应的所有事件的发生概率的情况。
- 实例：上例随机变量![X_1](https://www.zhihu.com/equation?tex=X_1)（转1次奖励人民币数）的分布情况用概率质量函数（probability mass function，简写为PMF）表示就是：

![](https://pic2.zhimg.com/v2-8d4508f43698f933a1c54f0347c1999d_b.jpg)

## **概率五要件**
- **样本空间：**所有可能结果组成的集合。
- **随机变量：**将事件投向实数的函数。用数字代表事件。
- **事件：**样本空间的子集。
- **概率：**将事件投向![[0,1]](https://www.zhihu.com/equation?tex=%5B0%2C1%5D)实数域的函数。用实数表示确信度。
- **分布：**随机变量的取值情况。
![](https://pic1.zhimg.com/v2-c4bf85890918ede7952697c4d5e88618_b.jpg)


注意在应用中区分物理意义与数学定义。如随机变量虽然是以事件为输入，实数为输出。但是在用于表达概率![P(X=3)](https://www.zhihu.com/equation?tex=P%28X%3D3%29)是用3这个数字去表示事件，并得出该事件的概率，并不是将实数作为输入。又如概率的数学定义是事件投射到![[0,1]](https://www.zhihu.com/equation?tex=%5B0%2C1%5D)的实数上，但在物理意义中，是样本空间的内在情况决定了事件。上图中：
- **蓝线：**表示人们为了描述物理现象而定义的数学函数。箭头由输入空间指向输出空间。
- 概率函数：输入为事件，输出为![[0,1]](https://www.zhihu.com/equation?tex=%5B0%2C1%5D)实数
- 随机变量函数：输入为事件，输出为实数（但使用时，用实数代表事件）
- 概率质量函数：输入为实数，输出为![[0,1]](https://www.zhihu.com/equation?tex=%5B0%2C1%5D)实数

- **红线：**表示真实的物理现象。箭头由因指向果。
- 由确信度所反映的内在分布情况决定了事件的发生。
- 事件的发生决定了随机变量的输出值。

