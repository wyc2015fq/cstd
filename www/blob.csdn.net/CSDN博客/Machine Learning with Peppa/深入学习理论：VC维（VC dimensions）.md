# 深入学习理论：VC维（VC dimensions） - Machine Learning with Peppa - CSDN博客





2018年04月20日 15:49:33[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：1085








## 统计学习的目标


> 在统计学习中，我们可以用两个式子来描述学习的目标。对于我们所学习的模型![](http://latex.codecogs.com/gif.latex?%5Chat%7Bf%7D)，希望存在一个很小的正数![](http://latex.codecogs.com/gif.latex?%5Cvarepsilon)，满足下面式子：

> ![](http://latex.codecogs.com/gif.latex?R_%7Bexp%7D(%5Chat%7Bf%7D)%5Crightarrow&space;0)


> ![](http://latex.codecogs.com/gif.latex?P(%7CR_%7Bexp%7D(%5Chat%7Bf%7D)-R_%7Bemp%7D(%5Chat%7Bf%7D)%7C)

> 其中，![](http://latex.codecogs.com/gif.latex?%5Chat%7Bf%7D)是我们学习的模型，![](http://latex.codecogs.com/gif.latex?R_%7Bexp%7D(%5Chat%7Bf%7D))是模型的期望损失，![](http://latex.codecogs.com/gif.latex?R_%7Bemp%7D(%5Chat%7Bf%7D))是模型的经验损失。

> 这两个式子所表示的意思：


> 

> 1.第一个式子反映了平均意义下，我们所训练的模型预测的好坏，趋于0就代表模型预测的效果越好。

> 2.就像博客：[统计学习三要素](http://blog.csdn.net/lucylove3943/article/details/47057893)所说，![](http://latex.codecogs.com/gif.latex?R_%7Bexp%7D(%5Chat%7Bf%7D))不可计算，所以我们希望![](http://latex.codecogs.com/gif.latex?R_%7Bemp%7D(%5Chat%7Bf%7D))尽可能的接近![](http://latex.codecogs.com/gif.latex?R_%7Bexp%7D(%5Chat%7Bf%7D))。

> 3.如果![](http://latex.codecogs.com/gif.latex?R_%7Bemp%7D(%5Chat%7Bf%7D))和![](http://latex.codecogs.com/gif.latex?R_%7Bexp%7D(%5Chat%7Bf%7D))接近，并且![](http://latex.codecogs.com/gif.latex?R_%7Bexp%7D(%5Chat%7Bf%7D))很小，那么我们所学习的模型![](http://latex.codecogs.com/gif.latex?%5Chat%7Bf%7D)就与真实的![](http://latex.codecogs.com/gif.latex?P(y%7Cx))很接近。




> 这篇文章主要是从第二个式子来讨论问题，就是主要分析模型的泛化能力，推导模型的复杂程度和泛化能力之间的关系（也就是表征系统复杂度的vc维和泛化误差之间的关系）。

> 


> 


## Hoeffding不等式


> 
对于上面的第二个式子，只是这样定性的描述我们的目标，其实并没有什么卵用![吐舌头](http://static.blog.csdn.net/xheditor/xheditor_emot/default/tongue.gif)，于是，有人想到了Hoeffding不等式：

若：


> 
> 
1.若![](http://latex.codecogs.com/gif.latex?x)服从参数为![](http://latex.codecogs.com/gif.latex?p)的伯努利分布的随机变量。

2.![](http://latex.codecogs.com/gif.latex?x_%7B1%7D,x_%7B2%7D,...,x_%7Bn%7D)是从母体![](http://latex.codecogs.com/gif.latex?x)中取的子样。


那么：

![](http://latex.codecogs.com/gif.latex?P(%7C%5Cbra%7Bx%7D-E(X)%7C)


其中：

![](http://latex.codecogs.com/gif.latex?%5Cbar%7Bx%7D=%5Cfrac%7Bx_%7B1%7D+x_%7B2%7D+...+x_%7Bn%7D%7D%7Bn%7D)

有了Hoeffding不等式，上面的第二个式子就可以写为：

[](http://www.codecogs.com/eqnedit.php?latex=P(%7CR_%7Bexp%7D(%5Chat%7Bf%7D)-R_%7Bemp%7D(%5Chat%7Bf%7D)%7C))![](http://latex.codecogs.com/gif.latex?P(%7CR_%7Bemp%7D(%5Chat%7Bf%7D)-R_%7Bexp%7D(%5Chat%7Bf%7D)%7C%3E%5Cvarepsilon&space;)%5Cleq&space;2e%5E%7B-2%5Cvarepsilon%5E%7B2%7DN%7D)


其中![](http://latex.codecogs.com/gif.latex?N)为计算![](http://latex.codecogs.com/gif.latex?R_%7Bemp%7D(%5Chat%7Bf%7D))训练集中数据的个数。

这时，上面的第二个式子就是一个上界可以用数学来表示的量了。我们只需要尽量的减小上界![](http://latex.codecogs.com/gif.latex?2e%5E%7B-2%5Cvarepsilon%5E%7B2%7Dn%7D)，那么就可以实现：

[](http://www.codecogs.com/eqnedit.php?latex=P(%7CR_%7Bemp%7D(%5Chat%7Bf%7D)-R_%7Bexp%7D(%5Chat%7Bf%7D)%7C)![](http://latex.codecogs.com/gif.latex?P(%7CR_%7Bexp%7D(%5Chat%7Bf%7D)-R_%7Bemp%7D(%5Chat%7Bf%7D)%7C%3C%5Cvarepsilon)%5Crightarrow&space;1)












## Hoeffding不等式的“放大”



> 
既然我们现在得到了这个式子：

[](http://www.codecogs.com/eqnedit.php?latex=P(%7CR_%7Bexp%7D(%5Chat%7Bf%7D)-R_%7Bemp%7D(%5Chat%7Bf%7D)%7C)%3C2e%5E%7B-2%5Cvarepsilon%5E%7B2%7DN%7D)![](http://latex.codecogs.com/gif.latex?P(%7CR_%7Bemp%7D(%5Chat%7Bf%7D)-R_%7Bexp%7D(%5Chat%7Bf%7D)%7C%3E%5Cvarepsilon&space;)%5Cleq&space;2e%5E%7B-2%5Cvarepsilon%5E%7B2%7DN%7D)


现在应该考虑的就是怎么样去减小上界。自然而然的，首先想到的是尽量的让![](http://latex.codecogs.com/gif.latex?N)值很大。但是这里有两个问题：


> 
> 
1.训练集的数据是有限的，并且训练也是有成本的，所以不能让![](http://latex.codecogs.com/gif.latex?N)无限制的增大。并且对于一次统计学习来说，训练集的大小![](http://latex.codecogs.com/gif.latex?N)已经是固定的了。



> 
> 
2.并且，训练集数据的个数![](http://latex.codecogs.com/gif.latex?N)并不是一个反应模型复杂程度的量。




> 所以，这样来看，在上述式子中只有![](http://latex.codecogs.com/gif.latex?N)这个参数，对于分析模型复杂度和泛化能力的关系并没有什么卵用![难过](http://static.blog.csdn.net/xheditor/xheditor_emot/default/sad.gif)。

> 于是![可怜](http://static.blog.csdn.net/xheditor/xheditor_emot/default/cute.gif)，有人决定把这个式子进行放大，对于一个假设空间![](http://latex.codecogs.com/gif.latex?F=%5Cbegin%7BBmatrix%7D&space;f_%7B1%7D,f_%7B2%7D,....f_%7BM%7D&space;%5Cend%7BBmatrix%7D)来讨论这个问题，那么上面的式子就可以写成：

> ![](http://latex.codecogs.com/gif.latex?%5Csum_%7Bi=1%7D%5E%7BM%7DP(%7CR_%7Bemp%7D(%5Chat%7Bf_%7Bi%7D%7D)-R_%7Bexp%7D(%5Chat%7Bf_%7Bi%7D%7D)%7C)

> 这样放大以后多了一个参数![](http://latex.codecogs.com/gif.latex?M)，这对于统计学习的讨论是很有意义的，因为![](http://latex.codecogs.com/gif.latex?M)的引入把整个假设空间都拉进了这个式子里，我们在此基础上再想办法引入变量来描述假设空间里模型的复杂程度，不就可以向目标靠近了吗？



> 




> 


> 


> 


## 有效的![](http://latex.codecogs.com/gif.latex?M)值

> 怎么取描述假设空间里面模型的复杂程度呢？

> 或许我们可以对上式的![](http://latex.codecogs.com/gif.latex?M)来做文章，用一个叫“有效”的概念在一定程度上表示假设空间里模型的复杂度。

> 怎么理解有效呢？


> 

> 假设现在讨论的是一个二分问题，现在一共有![](http://latex.codecogs.com/gif.latex?N)个数据。那么可以知道，对于整个训练集，最多的分类可能性有![](http://latex.codecogs.com/gif.latex?2%5E%7BN%7D)个。



> 
> 
> 1.如果我们不限制模型的种类，那么，对于这个训练集来说，一个假设空间![](http://latex.codecogs.com/gif.latex?F=%5Cbegin%7BBmatrix%7D&space;f_%7B1%7D,f_%7B2%7D,....f_%7BM%7D&space;%5Cend%7BBmatrix%7D)，不管![](http://latex.codecogs.com/gif.latex?M)取的有多大，它的有效的模型个数只有![](http://latex.codecogs.com/gif.latex?2%5E%7BN%7D)个。因为在大于![](http://latex.codecogs.com/gif.latex?2%5E%7BN%7D)个的假设空间里，一定会有两个模型的分类结果是一模一样的。

> 2.（这一个不看也不会影响后面vc维的推导）如果我们限制了模型的种类，比如说，如果现在规定了模型必须只能是直线，那么对于刚刚所说的![](http://latex.codecogs.com/gif.latex?2%5E%7BN%7D)个可能性，它并不总是可分的。比如对于下图，当![](http://latex.codecogs.com/gif.latex?N)为4时，模型为直线时，此时这四个点就是可分的。

![](https://img-blog.csdn.net/20150805154250543?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



> 
> 
> 所以当考虑到这种情况，一个假设空间![](http://latex.codecogs.com/gif.latex?F=%5Cbegin%7BBmatrix%7D&space;f_%7B1%7D,f_%7B2%7D,....f_%7BM%7D&space;%5Cend%7BBmatrix%7D)，不管![](http://latex.codecogs.com/gif.latex?M)取的有多大，实际上它的有效的模型应该是小于![](http://latex.codecogs.com/gif.latex?2%5E%7BN%7D)的。那么当假设空间![](http://latex.codecogs.com/gif.latex?F=%5Cbegin%7BBmatrix%7D&space;f_%7B1%7D,f_%7B2%7D,....f_%7BM%7D&space;%5Cend%7BBmatrix%7D)的![](http://latex.codecogs.com/gif.latex?M)并不是无限大的时候，我们也可以相应的推出，对于一个训练集![](http://latex.codecogs.com/gif.latex?N)来说，这个假设空间也只有小于等于![](http://latex.codecogs.com/gif.latex?M)个有效模型的个数。



> 
我们将对于含有![](http://latex.codecogs.com/gif.latex?N)个数据的训练集，假设空间[](http://www.codecogs.com/eqnedit.php?latex=F=%5Cbegin%7BBmatrix%7D&space;f_%7B1%7D,f_%7B2%7D,....f_%7BM%7D&space;%5Cend%7BBmatrix%7D)![](http://latex.codecogs.com/gif.latex?F=%5Cbegin%7BBmatrix%7D&space;f_%7B1%7D,f_%7B2%7D,....f_%7BM%7D&space;%5Cend%7BBmatrix%7D)中有效模型的个数记作[](http://www.codecogs.com/eqnedit.php?latex=m_%7Beff%7D(N))![](http://latex.codecogs.com/gif.latex?m_%7Beff%7D(N))。从上面的讨论可以看出，它是一个与![](http://latex.codecogs.com/gif.latex?N)、假设空间、还有假设空间里模型的种类有关的量。（其实在这里我们可以换一种说法来描述“模型的种类”——模型的复杂度，从上面可以看出，模型越复杂的时候，![](http://latex.codecogs.com/gif.latex?m_%7Beff%7D(N))可能越大）所以现在我们可以说：![](http://latex.codecogs.com/gif.latex?m_%7Beff%7D(N))是一个与![](http://latex.codecogs.com/gif.latex?N)、假设空间、还有假设空间里模型复杂度有关的量。


> 
我们这里定性的讨论了![](http://latex.codecogs.com/gif.latex?m_%7Beff%7D(N))是一个小于等于![](http://latex.codecogs.com/gif.latex?2%5E%7BN%7D)，小于等于![](http://latex.codecogs.com/gif.latex?M)的量。在[VC维的来龙去脉](http://dataunion.org/14581.html)这篇文章中，定性的给出了![](http://latex.codecogs.com/gif.latex?m_%7Beff%7D(N))的上界，并且说明了对于给定的假设空间![](http://latex.codecogs.com/gif.latex?F=%5Cbegin%7BBmatrix%7D&space;f_%7B1%7D,f_%7B2%7D,....f_%7BM%7D&space;%5Cend%7BBmatrix%7D)，当![](http://latex.codecogs.com/gif.latex?N)从一开始慢慢变大时，![](http://latex.codecogs.com/gif.latex?m_%7Beff%7D(N))是以多项式的速度增长的，感兴趣可以去看一下。











## vc bound


> 


> 得到这个有效模型个数有什么用呢，可以把它直接用来替换下式中的![](http://latex.codecogs.com/gif.latex?M)吗？

> [](http://www.codecogs.com/eqnedit.php?latex=%5Csum_%7Bi=1%7D%5E%7BM%7DP(%7CR_%7Bexp%7D(%5Chat%7Bf%7D_%7Bi%7D)-R_%7Bemp%7D(%5Chat%7Bf%7D_%7Bi%7D)%7C)%5Cleq&space;2Me%5E%7B-2%5Cvarepsilon%5E%7B2%7DN%7D)![](http://latex.codecogs.com/gif.latex?%5Csum_%7Bi=1%7D%5E%7BM%7DP(%7CR_%7Bemp%7D(%5Chat%7Bf_%7Bi%7D%7D)-R_%7Bexp%7D(%5Chat%7Bf_%7Bi%7D%7D)%7C%3E%5Cvarepsilon&space;)%5Cleq&space;2Me%5E%7B-2%5Cvarepsilon%5E%7B2%7DN%7D)


> 好像是不行的哦。那么我们把等式在左边缩小一下呢，对于假设空间![](http://latex.codecogs.com/gif.latex?F=%5Cbegin%7BBmatrix%7D&space;f_%7B1%7D,f_%7B2%7D,....f_%7BM%7D&space;%5Cend%7BBmatrix%7D)中的任意一个![](http://latex.codecogs.com/gif.latex?f_%7Bany%7D)，都有：

> ![](http://latex.codecogs.com/gif.latex?P(%7CR_%7Bexp%7D(f_%7Bany%7D)-R_%7Bemp%7D(f_%7Bany%7D)%7C)


> 由于![](http://latex.codecogs.com/gif.latex?m_%7Beff%7D(N))是大于等于一的数，所以上式是成立的。经过证明（证明过程可参见[training versus testing](http://blog.csdn.net/xyd0512/article/details/43484749)），得到了下面的式子：

> ![](http://latex.codecogs.com/gif.latex?P(%7CR_%7Bexp%7D(f_%7Bany%7D)-R_%7Bemp%7D(f_%7Bany%7D)%7C)


> 它被称为VC bound。

> 现在来看这个式子：


> 

> 1.式子的右边里面有一项![](http://latex.codecogs.com/gif.latex?m_%7Beff%7D(2N))，如前面所说，它是一个随![](http://latex.codecogs.com/gif.latex?N)以多项式的速度增大的，而后面的![](http://latex.codecogs.com/gif.latex?e%5E%7B-%5Cfrac%7B1%7D%7B8%7D%5Cvarepsilon%5E%7B2%7DN%7D)是随![](http://latex.codecogs.com/gif.latex?N)的增大以指数的速度在减小的。所以随![](http://latex.codecogs.com/gif.latex?N)的增大，右边是越来越小的。

> 2.这个式子还告诉我们，当![](http://latex.codecogs.com/gif.latex?N)足够大的时候，对于假设空间里面的任意一个模型![](http://latex.codecogs.com/gif.latex?f_%7Bany%7D)，它的经验风险![](http://latex.codecogs.com/gif.latex?R_%7Bemp%7D(f_%7Bany%7D))和![](http://latex.codecogs.com/gif.latex?R_%7Bexp%7D(f_%7Bany%7D))都是非常接近的。




> 


> 


## VC dimension（VC 维）

> 正如前面所说，![](http://latex.codecogs.com/gif.latex?m_%7Beff%7D(N))是一个与![](http://latex.codecogs.com/gif.latex?N)、假设空间、还有假设空间里模型复杂度有关的量。我们的主要目的是讨论模型复杂度对泛化能力的影响，所以要想办法剔除掉训练集大小![](http://latex.codecogs.com/gif.latex?N)对于泛化能力的影响，所以在这里引入了vc维。

> 终于，在我自己都快把自己绕晕的时候，终于进入正题了。![哭](http://static.blog.csdn.net/xheditor/xheditor_emot/default/cry.gif)

> 


> 定义VC维为：

> ![](http://latex.codecogs.com/gif.latex?d_%7Bvc%7D=arg&space;%5Cquad&space;%5Cunderset%7BN%7D%7Bmax%7D(m_%7Beff%7D(N)=2%5E%7BN%7D))


> 它所表示的意思是，对于一个假设空间![](http://latex.codecogs.com/gif.latex?F=%5Cbegin%7BBmatrix%7D&space;f_%7B1%7D,f_%7B2%7D,....f_%7BM%7D&space;%5Cend%7BBmatrix%7D)，它的有效模型个数刚好能把含有![](http://latex.codecogs.com/gif.latex?N)个数据的训练集中所有的可能性（![](http://latex.codecogs.com/gif.latex?2%5E%7BN%7D)）穷尽的最大![](http://latex.codecogs.com/gif.latex?N)。根据定义可以看出来，VC维是一个只与假设空间和模型复杂度有关的量。

> 


> 前方高能预警，将出现一大波繁杂的公式，不想看可调至最后结果：


> 

> ![](http://latex.codecogs.com/gif.latex?m_%7Beff%7D(N))表示的是：含有![](http://latex.codecogs.com/gif.latex?N)个数据的训练集，假设空间[](http://www.codecogs.com/eqnedit.php?latex=F=%5Cbegin%7BBmatrix%7D&space;f_%7B1%7D,f_%7B2%7D,....f_%7BM%7D&space;%5Cend%7BBmatrix%7D)![](http://latex.codecogs.com/gif.latex?F=%5Cbegin%7BBmatrix%7D&space;f_%7B1%7D,f_%7B2%7D,....f_%7BM%7D&space;%5Cend%7BBmatrix%7D)中有效模型的个数，所以对于给定的假设空间：




> ![](http://latex.codecogs.com/gif.latex?m_%7Beff%7D(N))



> 

> 进一步的：




> ![](http://latex.codecogs.com/gif.latex?m_%7Beff%7D(2N))



> 

> 再更进一步：




> ![](http://latex.codecogs.com/gif.latex?m_%7Beff%7D(2N))



> 

> 那么，之前的式子：




> ![](http://latex.codecogs.com/gif.latex?P(%7CR_%7Bexp%7D(f_%7Bany%7D)-R_%7Bemp%7D(f_%7Bany%7D)%7C%3E%5Cvarepsilon)%5Cleq&space;4m_%7Beff%7D(2N)e%5E%7B-%5Cfrac%7B1%7D%7B8%7D%5Cvarepsilon%5E%7B2%7DN%7D)



> 

> 我们现在可以进一步放大为：




> ![](http://latex.codecogs.com/gif.latex?P(%7CR_%7Bexp%7D(f_%7Bany%7D-R_%7Bemp%7D(f_%7Bany%7D)%7C)



> 

> 现在令：




> ![](http://latex.codecogs.com/gif.latex?%5Cdelta&space;=4(2N)%5E%7Bd_%7Bvc%7D%7De%5E%7B-%5Cfrac%7B1%7D%7B8%7D%5Cvarepsilon%5E%7B2%7DN%7D)


> 



> 

> 可以反解出：




> ![](http://latex.codecogs.com/gif.latex?%5Cvarepsilon&space;=&space;%5Csqrt%7B%5Cfrac%7B8%7D%7BN%7Dln(%5Cfrac%7B4(2N)%5E%7Bd_%7Bvc%7D%7D%7D%7B%5Cdelta&space;%7D)%7D)

> （我就是最后结果）由此，结合前面红色加粗的结论，可以得到，当![](http://latex.codecogs.com/gif.latex?N)很大的时候，有很大的概率出现：



> ![](http://latex.codecogs.com/gif.latex?%7CR_%7Bexp%7D(f_%7Bany%7D)-R_%7Bemp%7D(f_%7Bany%7D)%7C)


> 上面这个式子就是[模型的评估与选择](http://blog.csdn.net/lucylove3943/article/details/47059599)这篇文章中提到的泛化误差上界。（![哭](http://static.blog.csdn.net/xheditor/xheditor_emot/default/cry.gif)喜极而泣，大费周章，终于把这个坑给填了）

> vc维在这里面起到了一个惩罚项的作用，它所表征的是模型的复杂程度，当模型越复杂的时候，vc维越大，泛化能力就越差；当模型越简单的时候，vc维越小，经验损失函数![](http://latex.codecogs.com/gif.latex?R_%7Bemp%7D(%5Chat%7Bf%7D))和期望损失函数![](http://latex.codecogs.com/gif.latex?R_%7Bexp%7D(%5Chat%7Bf%7D))越接近，泛化能力越好。

> 


> 


> 


> 



### 写在后面的话：


> 
上面有一些观点是我自己开的脑洞![吐舌头](http://static.blog.csdn.net/xheditor/xheditor_emot/default/tongue.gif)，比如说为什么要在假设集上来讨论Hoeffding不等式，还有高能预警的推导，如果您有别的看法，欢迎讨论。




我自己学这一段的时候也有一些疑问，如果您有什么想法，跪求解答：



> 
> 
1.它的推导过程是针对的二分问题，如果是高维的话，还work吗？

2.好，就算是二分问题，那么对于二分问题来说，假设的![](http://latex.codecogs.com/gif.latex?P(X,Y))就一定是服从伯努利分布的吗？不是的话，就不能用Hoeffding不等式呀，那后面的讨论有什么意义呢？

3.我个人觉得用vc维来描述模型的复杂程度有点麻烦了，难道就不能用别的更为简单和直观的变量来表征模型复杂度吗？对于高能预警的那一片推导中有![](http://latex.codecogs.com/gif.latex?m_%7Beff%7D(2N)%3C&space;2%5E%7Bdvc+1%7D)，我有很大疑问，为什么一定要用vc维？应该还是存在别的更简单更直观的描述模型复杂度的量吧。





