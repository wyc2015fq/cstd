# 隐马尔可夫模型（HMM） - Joe的博客 - CSDN博客





2017年06月01日 15:08:40[Joe-Han](https://me.csdn.net/u010089444)阅读数：7061








# **1. 背景知识**

## **1.1随机过程**

随机过程是随机变量的集合，其在随机变量的基础上引入时间的概念（可简单理解为随机变量关于时间的函数）。例如，$x_1(t),x_2(t),x_3(t),x_4(t)$ 都是时间的函数，我们将其称为样本函数，样本函数的集合便是一个随即过程。

其定义如下：设：$(Ω, F , P )$为一概率空间，集合$T $为一指标集合。如果对于所有$t ∈ T$，均有一随机变量$ξ_t (ω)$定义于概率空间$(Ω, F , P )$，则集合$ \begin{Bmatrix} ξ_t (ω)|t ∈ T \end{Bmatrix} $为一随机过程。

## **1.2 马尔可夫性质**

**马尔可夫性质**是概率论中的一个概念：当一个随机过程在给定现在状态及所有过去状态的情况下，其未来状态的条件概率分布仅依赖于当前状态，简而言之，随即过程中某状态的发生只取決它上一个时刻的状态，那么此随机过程即具有马尔可夫性质。数学上，如果$X（t）,t>0$为一个随机过程，则马尔可夫性质的定义如下：
- $Pr[X(t+h)=y|X(s)=x(s),s \leqslant  t] = Pr[X(t+h)=y|X(t)=x(t)], \forall h >0$

一般来说，具备马尔可夫性质的随机过程是不具备记忆特质的。在这个系统中，现在的条件概率和过去及未来的状态都是独立且不相关的，具备马尔可夫性质的过程通常称为马尔可夫过程。

## **1.3 马尔可夫链**

具备离散状态的马尔可夫过程，通常被称为马尔可夫链 。该过程中，在给定当前知识或信息的情况下，只有当前的状态用来预测将来，过去（即历史状态）对于预测未来（即当前之后的状态）是无关的。在马尔可夫链的每一步。系统根据概率分布，可以从一个状态变到另一个状态，也可以保持当前状态。状态的改变称为“迁移”，与不同的状态改变相关的概率称为“状态迁移概率”。马尔可夫链的典型例子包括：**随即游走**，**PageRank算法**。 

下图给出了一个在两个状态间进行迁移的马尔可夫链模型示意图：


![图片名称](https://img-blog.csdn.net/20170516100345852?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


其数学定义如下：马尔可夫链是随机变量$X_1 , X_2 , X_3 , . . .$的一个数列。这些变量的范围，即它们所有可能取值的集合被称为“状态空间”，而$X_n$的值则是在时间n的状态。如果$X_｛n+1｝$对于过去状态的条件概率分布仅是$X_n$的一个函数，则：
- $P(X_{n+1} = x | X_0,X_1,X_2,...,X_n) = P(X_{n+1} = x | X_n)$

这里$x$为过程中的某个状态。上面这个等式也可看作马尔可夫性质的定义。

## **1.4 模式的形成**

**确定性模式：**

考虑一套交通信号灯，灯的颜色变化序列依次是“红色→红色→黄色→绿色→黄色→红色”，如下图所示。 
![这里写图片描述](https://img-blog.csdn.net/20170516140331430?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中，每个状态都唯一地依赖于前一个状态。因此，如果当前交通灯为绿色，那么随后的交通灯颜色将只能是黄色，也就是说，该系统是确定性的。


**非确定性模式：**

马尔可夫链就是一种非确定性模式，状态之间的迁移不是确定的。下图给出了一阶天气状态变迁图，其中包含三个状态（晴天、雨天、多云），通过今天的天气状态可预测明天某天气状态出现的概率。 
![图片名称](https://img-blog.csdn.net/20170516141436789?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

对于$M$个状态的一阶马尔可夫过程，共有$M^2$个状态转移，因为其中的每一个状态都可能是当前状态下一步迁移的目标，我们为每一个状态迁移事件关联一个概率值，称为状态迁移概率。下面的状态转移矩阵显示的是天气例子中可能的状态迁移概率。 
![图片名称](https://img-blog.csdn.net/20170516142708916?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


可见，如果今天是晴天，那么明天是晴天的概率为0.5，多云的概率为0.375，下雨的概率为0.125。**该过程假设概率值不随时间发生变化。**

# **2. 隐马尔可夫模型**

首先用参考资料中一个简单的例子来阐述：假设我手里有三个不同的骰子。第一个骰子是我们平常见的骰子（称这个骰子为D6），6个面，每个面（1，2，3，4，5，6）出现的概率是1/6。第二个骰子是个四面体（称这个骰子为D4），每个面（1，2，3，4）出现的概率是1/4。第三个骰子有八个面（称这个骰子为D8），每个面（1，2，3，4，5，6，7，8）出现的概率是1/8。


![图片名称](https://img-blog.csdn.net/20170516125237675?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


假设我们开始掷骰子，我们先从三个骰子里挑一个，挑到每一个骰子的概率都是1/3。然后我们掷骰子，得到一个数字，1，2，3，4，5，6，7，8中的一个。不停的重复上述过程，我们会得到一串数字，每个数字都是1，2，3，4，5，6，7，8中的一个。例如我们可能得到这么一串数字（掷骰子10次）：1 6 3 5 2 7 3 5 2 4。这串数字叫做可见状态链。但是在隐马尔可夫模型中，我们不仅仅有这么一串可见状态链，还有一串隐含状态链。在这个例子里，这串隐含状态链就是你用的骰子的序列。比如，隐含状态链有可能是：D6 D8 D8 D6 D4 D8 D6 D6 D4 D8。

一般来说，HMM中说到的马尔可夫链其实是指隐含状态链，因为隐含状态（骰子）之间存在转换概率（transition probability）。在我们这个例子里，D6的下一个状态是D4，D6，D8的概率都是1/3。D4，D8的下一个状态是D4，D6，D8的转换概率也都一样是1/3。这样设定是为了最开始容易说清楚，但是我们其实是可以随意设定转换概率的。比如，我们可以这样定义，D6后面不能接D4，D6后面是D6的概率是0.9，是D8的概率是0.1。这样就是一个新的HMM。

同样的，尽管可见状态之间没有转换概率，但是隐含状态和可见状态之间有一个概率叫做输出概率（emission probability）。就我们的例子来说，六面骰（D6）产生1的输出概率是1/6。产生2，3，4，5，6的概率也都是1/6。我们同样可以对输出概率进行其他定义。比如，我有一个被赌场动过手脚的六面骰子，掷出来是1的概率更大，是1/2，掷出来是2，3，4，5，6的概率是1/10。 

![图片名称](https://img-blog.csdn.net/20170516125733839?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## **2.1马尔可夫过程的局限性**

回到天气的例子，实际情况中，观察者无法直接获取天气状况的观测值，但他们手中有一些海藻，民谚告诉我们，海藻的状态或许与天气的状态有密切的关系。由此我们得到了两种状态：可观测状态（海藻的干湿程度：dry，dryish，damp，soggy），和隐含状态（天气状况：sunny，cloudy，rainy）。我们希望基于马尔可夫假设和海藻的状态设计出某种算法，从而实现在缺乏天气历史观测数据的情况下，对未来的天气情况进行预测。另一个更实际的例子是语音识别问题，一些语音识别系统的工作原理是将语音所对应的单词视为隐状态，而采集到的声音视为观测状态。

根据海藻的干湿程度的观测值，我们对天气的例子进行重新设计，下图给出了其对应的隐马尔可夫模型： 

![图片名称](https://img-blog.csdn.net/20170516145222232?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


隐藏状态相互之间是关联的，隐藏状态和观察状态之间的链接表示：在当前马尔可夫过程所处的特定隐藏状态下，生成特定观测状态的概率。因此，对每个观测状态而言，在所有隐藏状态下观测到该状态的概率值之和应该为1，即$Pr(Obs|Sunny) + Pr(Obs|Cloudy) + Pr(Obs|Rainy)= 1 $。由此，我们也可定义输出矩阵（Emission Matrix），又称为混淆矩阵（Confusion Matrix），该矩阵的元素为：给定隐藏状态，得到某一观测状态的可能性，如下图所示：


![图片名称](https://img-blog.csdn.net/20170516150410606?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## **2.2 隐马尔可夫模型定义**

隐马尔可夫模型（Hidden Markov Model，HMM）是一个统计模型，它用来描述一个含有隐含未知参数的马尔可夫过程。其难点是从可观察的参数中确定该过程的隐含参数。然后利用这些参数来作进一步的分析，例如模式识别。在一般的马尔可夫模型中，状态对于观察者来说是直接可见的。这样状态的转换概率便是全部的参数。而在隐马尔可夫模型中，状态并不是直接可见的，但受状态影响的某些变量则是可见的。每一个状态在可能输出的符号上都有一个概率分布。因此输出符号的序列能够透露出状态序列的一些信息。

一个隐马尔可夫过程是一个三元组$（\Pi ,A,B）$:
- $\Pi = (\pi_i）$隐状态的初始概率向量
- $A = (a_{ij}）$ 状态转移概率，$Pr(x_{i t}|x_{j t-1})$
- $B = (b_{ij}）$ 观测状态发生的概率，$Pr(y_{i}|x_{j})$

在实际应用中，人们常关注隐马尔可夫模型的三个基本问题：
- **评估问题：** 给定模型$\lambda =（\Pi ,A,B）$，如何计算其产生观测序列的概率$P（y|\lambda ）$。换而言之，如何评估模型与观测序列的匹配程度（采用forward算法）。
- **解码问题：** 给定模型$\lambda =（\Pi ,A,B）$和观测序列，如何找到与此观测序列最匹配的隐状态序列（采用viterbi算法算法）。
- **学习问题：** 给定观测序列，如何调整模型参数$\lambda =（\Pi ,A,B）$使得该序列出现的概率$P（x|\lambda）$最大。换而言之，如何训练模型使其能最好的描述观测数据（采用forward-backward算法）。

下文将结合天气的例子，重点介绍forward算法和viterbi算法。

# **3. forward算法**

首先定义一个局部概率(partial probability) $\alpha _t（j）$，表示从观测起点出发，到达任意一个中间观测值的某个隐藏状态的概率。以此为基础，考察当time=1 和 time = n (n > 1) 的时候如何计算这些局部概率。假设我们有一个长度为T的观测序列：$(Y_{k_1} ; Y_{k_2} ，...， Y_{k_T} )$

## **3.1 局部概率**

我们可以通过对从初始状态到达到达某个中间状态的所有路径进行求和得到该状态的概率值。例如，t=2时刻，隐状态为cloudy的概率可用三条路径的概率之和来表示： 

![图片名称](https://img-blog.csdn.net/20170516160010700?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


我们用 $α_t(j)$ 来表示在 t 时刻是状态 j 的局部概率：$\alpha_t(j) = Pr($observation | hidden state  is j$)  Pr($all paths to state j at time t$)$

最终观测状态的局部概率值为：通过所有可能的路径到达该观测值对应的各种隐藏状态的概率。例如，最终的局部概率通过如下路径得到：


![这里写图片描述](https://img-blog.csdn.net/20170516161436517?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

对这些最终的局部概率进行求和，相当于对所有可能的路径进行求和，由此也就得到了从该HMM模型假设出发，生成指定观测序列的概率。


## **3.2 计算t = 1时刻的α值**

当 t=1 的时候，没有通向该观测值对应的隐状态的路径，所以这里初始概率值，即：
- $Pr(state | t = 1) = \pi (state)$

相应地，各隐藏状态的局部概率值可以通过隐藏状态概率初始值与观测状态的发生概率相乘得到：
- $\alpha_1（j）  = \pi (j) \cdot b_{jk_1}$

因此，系统初始时刻处于状态j的局部概率同时依赖于：该状态的初始概率和该时刻我们观察到相应观测值的概率

## **3.3 计算t > 1时候的α值**

回顾一下局部概率的计算公式：$\alpha_t(j) = Pr($observation | hidden state  is j$)  Pr($all paths to state j at time t$)$。这个等式的右边第一项是从输出矩阵中已知的，我只需要计算右边第二项，很显然右边是所有路径的和： 

![图片名称](https://img-blog.csdn.net/20170516163958241?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

需要计算的路径数是和观察序列的长度的平方相关，但是 t 时刻的局部概率已经包含了之前的所有路径，所以在 t+1 时刻只需要根据 t 时刻的概率来计算就可以了，即：

- $\alpha_{t+1}(j) = b_{jk_t+1} \sum_{i=1}^{n} \alpha_{t}(i) a_{ij}$

# **4. viterbi算法**

在很多情况下，我们对隐藏状态更有兴趣，因为其包含了一些不能被直接观察到的有价值的信息。比如说在海藻和天气的例子中，一个人只能看到海藻的状态，但是他想知道天气的状态。这时候我们就可以使用 Viterbi 算法来根据可观察序列得到最有可能的隐藏状态的序列。在给定了一个可观察序列和HMM的情况下，我们可以考虑递推的来寻找最可能的隐藏序列。

## **4.1 局部概率与局部最优路径**

考虑下面这个图以及可观察序列 (dry, damp, soggy) 的一阶转移： 

![图片名称](https://img-blog.csdn.net/20170516203930805?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

对于每一个中间状态和终止状态 (t=3) 都有一个最可能的路径。比如说，在 t=3 时刻的三个状态都有一个如下的最可能的路径： 
![图片名称](https://img-blog.csdn.net/20170516204059846?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


我们可以称这些路径为局部最优路径。这些局部最优路径都有一个概率，也就是局部概率 $δ_t(i)$，表示在时刻t到达指定状态i经由的最可能的路径的概率。接下来我们将讨论如何计算 t=1 和 t=n (n>1) 的局部概率。**注意：**这里的局部概率和forward算法中的局部概率不一样，这里的局部概率表示的是在t时刻到达某个状态的最可能的一条路径的概率，而不是所有概率之和。

## **4.2 计算t = 1时刻的δ值**

当前当 t=1 时刻的时候，此时已知的仅有当前观测值，到达某个状态最大可能的路径还不存在，可结合初始状态概率和这个状态到可观察序列 $k_1$的输出概率来计算δ的值：
- $δ_1(i) = \pi (i)b_{ik_1}$

## **4.3 计算t > 1时刻的δ值**

如下图所示，我们希望计算出在时刻t到达状态X的最可能的路径，而该路径在t-1时刻有可能经过三种状态（A，B，C）中的某一个，因此我们希望从所有以AX，BX，CX结尾的路径中找出概率最大者。 

![图片名称](https://img-blog.csdn.net/20170516194646107?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


回顾一阶马尔可夫假设：当前系统处于某特定状态的概率仅仅取决于此前的1个状态。因此结合上例有：以AX结尾的最可能的路径，也就是以最大概率抵达A后再去往X的路径的概率为：
- $ Pr($most probable path to A$)  Pr($X | A$)  Pr($observation | X$)$

因此，以X为终点的最可能路径的概率定义如下：
- $ Pr($X at time t) = $\underset{i\in {A,B,C}}{max}Pr($i at time (t - 1$)) \times  Pr(X | i)  Pr($observations at time t | $X)$

其中等号右边第一项由t-1时刻的$δ_{t-1}(i)$给出，第二项来自状态迁移矩阵，第三项来自混淆矩阵。对上式进行推广：当t时刻的观测值$k_t$已知的情况下，在该时刻抵达状态i的局部最优路径的概率可由下式计算：
- $δt(i) = \underset{j}{max} (δ_{t-1}(j)  a_{ji} b_{ik_t}))$

## **4.4 反向指针**

对于每一个中间状态和结束状态我们可以得到局部概率$δ_{t}(i)$，然而我们的目标是对给定的观测序列找出与之对应的最可能的隐藏状态序列，因此我们需要采用某种方法来记住部分最优路径。通过前文我们知道，为了计算t时刻某特定隐藏状态的$δ_{t}(i)$值，只需要知道t-1时刻各隐藏状态的δ值。在完成计算的同时也就知道了究竟是哪个前驱状态导致了$δ_{t}(i)$的产生。为了记录前序状态，引入一个反向指针$\phi $，该指针指向促使当前状态发生的那个最优的直接前驱，定义如下：
- $\phi_t(i) = \underset{j}{argmax}(δ_{t-1}(j)  a_{ji})$

注意：$\phi $ 值的计算仅仅依赖于前一步各状态的δ值和状态转移概率，而不包括在给定状态下观测到特定观测状态的概率（与δ的计算公式相区别），这是因为我们希望通过计算$\phi $ 回答如下问题：如果系统当前处隐藏状态s，那么最可能是经由哪条路径来到这里？由于该问题仅涉及到隐藏状态，因此可以将观测概率忽略掉。

# **5. 参考资料**
- [http://www.comp.leeds.ac.uk/roger/HiddenMarkovModels/html_dev/main.html](http://www.comp.leeds.ac.uk/roger/HiddenMarkovModels/html_dev/main.html)
- [http://blog.csdn.net/likelet/article/details/7056068](http://blog.csdn.net/likelet/article/details/7056068)
- [http://www.cnblogs.com/skyme/p/4651331.html](http://www.cnblogs.com/skyme/p/4651331.html)





