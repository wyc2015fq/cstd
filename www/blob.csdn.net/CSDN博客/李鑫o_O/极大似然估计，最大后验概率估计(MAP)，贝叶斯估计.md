# 极大似然估计，最大后验概率估计(MAP)，贝叶斯估计 - 李鑫o_O - CSDN博客

置顶2016年04月13日 16:41:08[hustlx](https://me.csdn.net/HUSTLX)阅读数：6941


**最大似然估计学习总结------MadTurtle**

**1. 作用**

在已知试验结果（即是样本）的情况下，用来估计满足这些样本分布的参数，把可能性最大的那个参数![clip_image002](http://hi.csdn.net/attachment/201101/9/0_1294581116A52B.gif)作为真实![clip_image004](http://hi.csdn.net/attachment/201101/9/0_1294581117Rz2I.gif)的参数估计。

**2. 离散型**

设![clip_image006](http://hi.csdn.net/attachment/201101/9/0_1294581118mRqM.gif)为离散型随机变量，![clip_image008](http://hi.csdn.net/attachment/201101/9/0_1294581118eEZ5.gif)为多维参数向量，如果随机变量![clip_image010](http://hi.csdn.net/attachment/201101/9/0_1294581119z03E.gif)相互独立且概率计算式为P{![clip_image012](http://hi.csdn.net/attachment/201101/9/0_1294581119d664.gif)，则可得概率函数为P{![clip_image014](http://hi.csdn.net/attachment/201101/9/0_1294581120c9yY.gif)}=![clip_image016](http://hi.csdn.net/attachment/201101/9/0_12945811201NhH.gif)，在![clip_image008[1]](http://hi.csdn.net/attachment/201101/9/0_1294581121UGfp.gif)固定时，上式表示![clip_image014[1]](http://hi.csdn.net/attachment/201101/9/0_129458112119P9.gif)的概率；当![clip_image014[2]](http://hi.csdn.net/attachment/201101/9/0_1294581125YNfl.gif)已知的时候，它又变成![clip_image008[2]](http://hi.csdn.net/attachment/201101/9/0_1294581125RaAz.gif)的函数，可以把它记为![clip_image018](http://hi.csdn.net/attachment/201101/9/0_12945811264PRV.gif)，称此函数为似然函数。似然函数值的大小意味着该样本值出现的可能性的大小，既然已经得到了样本值![clip_image014[3]](http://hi.csdn.net/attachment/201101/9/0_1294581126qxp7.gif)，那么它出现的可能性应该是较大的，即似然函数的值也应该是比较大的，因而最大似然估计就是选择使![clip_image020](http://hi.csdn.net/attachment/201101/9/0_12945811271s87.gif)达到最大值的那个![clip_image002[1]](http://hi.csdn.net/attachment/201101/9/0_1294581127UBEI.gif)作为真实![clip_image004[1]](http://hi.csdn.net/attachment/201101/9/0_12945811285KBp.gif)的估计。

**3. 连续型**

设![clip_image006[1]](http://hi.csdn.net/attachment/201101/9/0_1294581129vRai.gif)为连续型随机变量，其概率密度函数为![clip_image022](http://hi.csdn.net/attachment/201101/9/0_1294581129hDCj.gif)，![clip_image010[1]](http://hi.csdn.net/attachment/201101/9/0_12945811302Rta.gif)为从该总体中抽出的样本，同样的如果![clip_image010[2]](http://hi.csdn.net/attachment/201101/9/0_1294581134t5U8.gif)相互独立且同分布，于是样本的联合概率密度为![clip_image024](http://hi.csdn.net/attachment/201101/9/0_1294581134bepZ.gif)。大致过程同离散型一样。

**4. 关于概率密度(PDF)**

我们来考虑个简单的情况(m=k=1)，即是参数和样本都为1的情况。假设进行一个实验，实验次数定为10次，每次实验成功率为0.2，那么不成功的概率为0.8，用y来表示成功的次数。由于前后的实验是相互独立的，所以可以计算得到成功的次数的概率密度为：

![clip_image026](http://hi.csdn.net/attachment/201101/9/0_1294581138UGYP.gif)=![clip_image028](http://hi.csdn.net/attachment/201101/9/0_1294581139nLy4.gif) 其中y![clip_image030](http://hi.csdn.net/attachment/201101/9/0_12945811394J7z.gif)

由于y的取值范围已定，而且![clip_image032](http://hi.csdn.net/attachment/201101/9/0_1294581143fzQB.gif)也为已知，所以图1显示了y取不同值时的概率分布情况，而图2显示了当![clip_image034](http://hi.csdn.net/attachment/201101/9/0_1294581143iQz6.gif)时的y值概率情况。

![clip_image036](http://hi.csdn.net/attachment/201101/9/0_1294581144B2Ov.gif)

图1 ![clip_image038](http://hi.csdn.net/attachment/201101/9/0_1294581148p17a.gif)时概率分布图

![clip_image040](http://hi.csdn.net/attachment/201101/9/0_12945811515969.gif)

图2 ![clip_image042](http://hi.csdn.net/attachment/201101/9/0_12945811556RPR.gif)时概率分布图

那么![clip_image032[1]](http://hi.csdn.net/attachment/201101/9/0_1294581156GRWm.gif)在[0,1]之间变化而形成的概率密度函数的集合就形成了一个模型。

**5. 最大似然估计的求法**

由上面的介绍可以知道，对于图1这种情况y=2是最有可能发生的事件。但是在现实中我们还会面临另外一种情况：我们已经知道了一系列的观察值和一个感兴趣的模型，现在需要找出是哪个PDF（具体来说参数![clip_image032[2]](http://hi.csdn.net/attachment/201101/9/0_1294581156sKdn.gif)为多少时）产生出来的这些观察值。要解决这个问题，就需要用到参数估计的方法，在最大似然估计法中，我们对调PDF中数据向量和参数向量的角色，于是可以得到似然函数的定义为：

![clip_image044](http://hi.csdn.net/attachment/201101/9/0_1294581157Ci33.gif)

该函数可以理解为，在给定了样本值的情况下，关于参数向量![clip_image032[3]](http://hi.csdn.net/attachment/201101/9/0_12945811574Vyu.gif)取值情况的函数。还是以上面的简单实验情况为例，若此时给定y为7，那么可以得到关于![clip_image032[4]](http://hi.csdn.net/attachment/201101/9/0_12945811589BkC.gif)的似然函数为：

![clip_image046](http://hi.csdn.net/attachment/201101/9/0_1294581159Z88r.gif)

继续回顾前面所讲，图1,2是在给定![clip_image032[5]](http://hi.csdn.net/attachment/201101/9/0_1294581159FIOB.gif)的情况下，样本向量y取值概率的分布情况；而图3是图1,2横纵坐标轴相交换而成，它所描述的似然函数图则指出在给定样本向量y的情况下，符合该取值样本分布的各种参数向量![clip_image032[6]](http://hi.csdn.net/attachment/201101/9/0_1294581160tkQ8.gif)的可能性。若![clip_image048](http://hi.csdn.net/attachment/201101/9/0_1294581160mcM2.gif)相比于![clip_image050](http://hi.csdn.net/attachment/201101/9/0_1294581161nSKC.gif)，使得y=7出现的可能性要高，那么理所当然的![clip_image048[1]](http://hi.csdn.net/attachment/201101/9/0_1294581161ZMVL.gif)要比![clip_image050[1]](http://hi.csdn.net/attachment/201101/9/0_1294581162AFf9.gif)更加接近于真正的估计参数。所以求![clip_image032[7]](http://hi.csdn.net/attachment/201101/9/0_1294581163PTVV.gif)的极大似然估计就归结为求似然函数![clip_image052](http://hi.csdn.net/attachment/201101/9/0_1294581163wRET.gif)的最大值点。那么![clip_image032[8]](http://hi.csdn.net/attachment/201101/9/0_1294581164rzq2.gif)取何值时似然函数![clip_image054](http://hi.csdn.net/attachment/201101/9/0_1294581164u2i2.gif)最大，这就需要用到高等数学中求导的概念，如果是多维参数向量那么就是求偏导。

![clip_image070](http://hi.csdn.net/attachment/201101/9/0_1294581165Rpo3.gif)

图3 ![clip_image072](http://hi.csdn.net/attachment/201101/9/0_1294581166SC29.gif)的似然函数分布图

主要注意的是多数情况下，直接对变量进行求导反而会使得计算式子更加的复杂，此时可以借用对数函数。由于对数函数是单调增函数，所以![clip_image056](http://hi.csdn.net/attachment/201101/9/0_1294581169XmXs.gif)与![clip_image058](http://hi.csdn.net/attachment/201101/9/0_1294581170XkxP.gif)具有相同的最大值点，而在许多情况下，求![clip_image060](http://hi.csdn.net/attachment/201101/9/0_12945811709wn6.gif)的最大值点比较简单。于是，我们将求![clip_image058[1]](http://hi.csdn.net/attachment/201101/9/0_1294581171k521.gif)的最大值点改为求![clip_image060[1]](http://hi.csdn.net/attachment/201101/9/0_1294581171PyaT.gif)的最大值点。

![clip_image062](http://hi.csdn.net/attachment/201101/9/0_1294581172sDp6.gif)

若该似然函数的导数存在，那么对![clip_image060[2]](http://hi.csdn.net/attachment/201101/9/0_1294581172owOX.gif)关于参数向量的各个参数求导数（当前情况向量维数为1），并命其等于零，得到方程组：

![clip_image064](http://hi.csdn.net/attachment/201101/9/0_1294581176HHIl.gif)

可以求得![clip_image034[1]](http://hi.csdn.net/attachment/201101/9/0_12945811762uU1.gif)时似然函数有极值，为了进一步判断该点位最大值而不是最小值，可以继续求二阶导来判断函数的凹凸性，如果![clip_image034[2]](http://hi.csdn.net/attachment/201101/9/0_12945811778mtd.gif)的二阶导为负数那么即是最大值，这里再不细说。

还要指出，若函数![clip_image022[1]](http://hi.csdn.net/attachment/201101/9/0_129458117750Va.gif)关于![clip_image066](http://hi.csdn.net/attachment/201101/9/0_1294581178l0eq.gif)的导数不存在，我们就无法得到似然方程组，这时就必须用其它的方法来求最大似然估计值，例如用有界函数的增减性去求![clip_image068](http://hi.csdn.net/attachment/201101/9/0_1294581178lLd3.gif)的最大值点

**最大似然估计：**

最大似然估计提供了一种给定观察数据来评估模型参数的方法，即：“模型已定，参数未知”。简单而言，假设我们要统计全国人口的身高，首先假设这个身高服从服从正态分布，但是该分布的均值与方差未知。我们没有人力与物力去统计全国每个人的身高，但是可以通过采样，获取部分人的身高，然后通过最大似然估计来获取上述假设中的正态分布的均值与方差。

最大似然估计中采样需满足一个很重要的假设，就是所有的采样都是独立同分布的。下面我们具体描述一下最大似然估计：

首先，假设![](http://pic002.cnblogs.com/images/2010/156169/2010112201390162.png)为独立同分布的采样，θ为模型参数,f为我们所使用的模型，遵循我们上述的独立同分布假设。参数为θ的模型f产生上述采样可表示为

![](http://pic002.cnblogs.com/images/2010/156169/2010112201405822.png)

回到上面的“模型已定，参数未知”的说法，此时，我们已知的为![](http://pic002.cnblogs.com/images/2010/156169/2010112201391980.png)，未知为θ，故似然定义为:

![](http://pic002.cnblogs.com/images/2010/156169/2010112201400153.png)

　　在实际应用中常用的是两边取对数，得到公式如下：

![](http://pic002.cnblogs.com/images/2010/156169/2010112201401577.png)

　　其中![](http://pic002.cnblogs.com/images/2010/156169/2010112201412749.png)称为对数似然，而![](http://pic002.cnblogs.com/images/2010/156169/2010112201414287.png)称为平均对数似然。而我们平时所称的最大似然为最大的对数平均似然，即：

![](http://pic002.cnblogs.com/images/2010/156169/2010112201420229.png)

举个别人博客中的例子，假如有一个罐子，里面有黑白两种颜色的球，数目多少不知，两种颜色的比例也不知。我 们想知道罐中白球和黑球的比例，但我们不能把罐中的球全部拿出来数。现在我们可以每次任意从已经摇匀的罐中拿一个球出来，记录球的颜色，然后把拿出来的球 再放回罐中。这个过程可以重复，我们可以用记录的球的颜色来估计罐中黑白球的比例。假如在前面的一百次重复记录中，有七十次是白球，请问罐中白球所占的比例最有可能是多少？很多人马上就有答案了：70%。而其后的理论支撑是什么呢？

我们假设罐中白球的比例是p，那么黑球的比例就是1-p。因为每抽一个球出来，在记录颜色之后，我们把抽出的球放回了罐中并摇匀，所以每次抽出来的球的颜
 色服从同一独立分布。这里我们把一次抽出来球的颜色称为一次抽样。题目中在一百次抽样中，七十次是白球的概率是P(Data | M)，这里Data是所有的数据，M是所给出的模型，表示每次抽出来的球是白色的概率为p。如果第一抽样的结果记为x1，第二抽样的结果记为x2... 那么Data
 = (x1,x2,…,x100)。这样，

P(Data | M)

　　　　　= P(x1,x2,…,x100|M)

　　　　　= P(x1|M)P(x2|M)…P(x100|M)

　　　　　= p^70(1-p)^30.

那么p在取什么值的时候，P(Data |M)的值最大呢？将p^70(1-p)^30对p求导，并其等于零。

70p^69(1-p)^30-p^70*30(1-p)^29=0。

　　　　解方程可以得到p=0.7。

在边界点p=0,1，P(Data|M)=0。所以当p=0.7时，P(Data|M)的值最大。这和我们常识中按抽样中的比例来计算的结果是一样的。

假如我们有一组连续变量的采样值（x1,x2,…,xn），我们知道这组数据服从正态分布，标准差已知。请问这个正态分布的期望值为多少时，产生这个已有数据的概率最大？

P(Data | M) = ？

根据公式

![](http://pic002.cnblogs.com/images/2010/156169/2010112201424677.png)

　　可得:![](http://pic002.cnblogs.com/images/2010/156169/2010112201431187.png)

　　对μ求导可得![](http://pic002.cnblogs.com/images/2010/156169/2010112201433061.png) ,则最大似然估计的结果为μ=(x1+x2+…+xn)/n

由上可知最大似然估计的一般求解过程：

　　（1） 写出似然函数；

　　（2） 对似然函数取对数，并整理；

　　（3） 求导数 ；

　　（4） 解似然方程

注意：最大似然估计只考虑某个模型能产生某个给定观察序列的概率。而未考虑该模型本身的概率。这点与贝叶斯估计区别。贝叶斯估计方法将在以后的博文中描述

本文参考

[http://en.wikipedia.org/wiki/Maximum_likelihood](http://en.wikipedia.org/wiki/Maximum_likelihood)

[http://www.shamoxia.com/html/y2010/1520.html](http://www.shamoxia.com/html/y2010/1520.html)

最大后验概率：

最大后验估计是根据经验数据获得对难以观察的量的点估计。与最大似然估计类似，但是最大的不同时，最大后验估计的融入了要估计量的先验分布在其中。故最大后验估计可以看做规则化的最大似然估计。

首先，我们回顾上篇文章中的最大似然估计，假设x为独立同分布的采样，θ为模型参数,f为我们所使用的模型。那么最大似然估计可以表示为：

![](http://pic002.cnblogs.com/images/2010/156169/2010112401595490.png)

现在，假设θ的先验分布为g。通过贝叶斯理论，对于θ的后验分布如下式所示：

![](http://pic002.cnblogs.com/images/2010/156169/2010112402003225.png)

最后验分布的目标为：

![](http://pic002.cnblogs.com/images/2010/156169/2010112402010597.png)

注：最大后验估计可以看做贝叶斯估计的一种特定形式。

　　举例来说：

　　假设有五个袋子，各袋中都有无限量的饼干(樱桃口味或柠檬口味)，已知五个袋子中两种口味的比例分别是

　　　　樱桃 100%

　　　　樱桃 75% + 柠檬 25%

　　　　樱桃 50% + 柠檬 50%

　　　　樱桃 25% + 柠檬 75%

　　　　柠檬 100%

　　如果只有如上所述条件，那问从同一个袋子中连续拿到2个柠檬饼干，那么这个袋子最有可能是上述五个的哪一个？

我们首先采用最大似然估计来解这个问题，写出似然函数。假设从袋子中能拿出柠檬饼干的概率为p(我们通过这个概率p来确定是从哪个袋子中拿出来的)，则似然函数可以写作

![](http://pic002.cnblogs.com/images/2010/156169/2010112402023487.png)

　　由于p的取值是一个离散值，即上面描述中的0,25%，50%，75%，1。我们只需要评估一下这五个值哪个值使得似然函数最大即可，得到为袋子5。这里便是最大似然估计的结果。

上述最大似然估计有一个问题，就是没有考虑到模型本身的概率分布，下面我们扩展这个饼干的问题。

假设拿到袋子1或5的机率都是0.1，拿到2或4的机率都是0.2，拿到3的机率是0.4，那同样上述问题的答案呢？这个时候就变MAP了。我们根据公式

![](http://pic002.cnblogs.com/images/2010/156169/2010112402041098.png)

写出我们的MAP函数。

![](http://pic002.cnblogs.com/images/2010/156169/2010112402052046.png)

根据题意的描述可知，p的取值分别为0,25%，50%，75%，1，g的取值分别为0.1，0.2,0.4,0.2,0.1.分别计算出MAP函数的结果为：0,0.0125,0.125,0.28125,0.1.由上可知，通过MAP估计可得结果是从第四个袋子中取得的最高。

　　上述都是离散的变量，那么连续的变量呢？假设![](http://pic002.cnblogs.com/images/2010/156169/2010112402055964.png)为独立同分布的![](http://pic002.cnblogs.com/images/2010/156169/2010112402065444.png)，μ有一个先验的概率分布为![](http://pic002.cnblogs.com/images/2010/156169/2010112402073966.png)。那么我们想根据![](http://pic002.cnblogs.com/images/2010/156169/2010112402055964.png)来找到μ的最大后验概率。根据前面的描述，写出MAP函数为：

![](http://pic002.cnblogs.com/images/2010/156169/2010112402173714.png)

　　此时我们在两边取对数可知。所求上式的最大值可以等同于求

![](http://pic002.cnblogs.com/images/2010/156169/2010112402085797.png)

　　的最小值。求导可得所求的μ为

![](http://pic002.cnblogs.com/images/2010/156169/2010112402090586.png)

　　以上便是对于连续变量的MAP求解的过程。

在MAP中我们应注意的是：

MAP与MLE最大区别是MAP中加入了模型参数本身的概率分布，或者说。MLE中认为模型参数本身的概率的是均匀的，即该概率为一个固定值。

1、贝叶斯公式

三种参数估计方法都和贝叶斯公式有关，因此首先从分析贝叶斯公式入手：

![](http://latex.codecogs.com/gif.latex?p(%5Ctheta&space;%7CX)=%5Cfrac%7Bp(X%7C%5Ctheta&space;)p(%5Ctheta&space;)%7D%7Bp(X)%7D)

贝叶斯公式可以表达为：

![](http://latex.codecogs.com/gif.latex?posterior=%5Cfrac%7Blikehood*prior%7D%7Bevidence%7D)

posterior：通过样本X得到参数![](http://latex.codecogs.com/gif.latex?%5Ctheta)的概率

likehood：通过参数![](http://latex.codecogs.com/gif.latex?%5Ctheta)得到样本X的概率

prior：参数![](http://latex.codecogs.com/gif.latex?%5Ctheta)的先验概率，一般是根据人的先验知识来得出的。比如人们倾向于认为抛硬币实验会符合先验分布：beta分布。当我们选择beta分布的参数![](http://latex.codecogs.com/gif.latex?%5Calpha=%5Cbeta=0.5)时，代表人们认为抛硬币得到正反面的概率都是0.5

evidence：![](http://latex.codecogs.com/gif.latex?p(X)=%5Cint&space;p(X%7C%5Ctheta&space;)p(%5Ctheta&space;)d%5Ctheta)，样本X发生的概率，是各种![](http://latex.codecogs.com/gif.latex?%5Ctheta)条件下发生的概率的积分

2、极大似然估计

目标是寻求能最大化likehood:的值。可以写出目标函数：![](http://latex.codecogs.com/gif.latex?p(X%7C%5Ctheta&space;)=%5Cprod_%7Bx1%7D%5E%7Bxn%7Dp(xi%7C%5Ctheta&space;))

一般使用对数来进行简化处理：![](http://latex.codecogs.com/gif.latex?p(X%7C%5Ctheta&space;)=%5Cprod_%7Bx1%7D%5E%7Bxn%7Dp(xi%7C%5Ctheta&space;)=%5Csum_%7Bx1%7D%5E%7Bxn%7Dlogp(xi%7C%5Ctheta&space;))

要最大化L，对L求导数并令导数为0即可求解。

3、最大后验估计（MAP）

和极大似然估计不同的是，MAP寻求的是能使后验概率![](http://latex.codecogs.com/gif.latex?p(%5Ctheta&space;%7CX))最大的![](http://latex.codecogs.com/gif.latex?%5Ctheta)值。

![](http://latex.codecogs.com/gif.latex?argmax&space;p(%5Ctheta&space;%7CX)&space;=argmax&space;%5Cfrac%7Bp(X%7C%5Ctheta&space;)p(%5Ctheta&space;)%7D%7Bp(X)%7D&space;=argmax&space;p(X%7C%5Ctheta&space;)p(%5Ctheta&space;)&space;=argmax&space;(%5Cprod_%7Bx1%7D%5E%7Bxn%7Dp(xi%7C%5Ctheta&space;))p(%5Ctheta&space;))

之所以可以省略分母p(X)，是因为p(X)和![](http://latex.codecogs.com/gif.latex?%5Ctheta)没有关系。

加上对数处理后，上面公式可以表达为：

![](http://latex.codecogs.com/gif.latex?argmax&space;(%5Csum_%7Bx1%7D%5E%7Bxn%7Dlogp(xi%7C%5Ctheta&space;)+logp(%5Ctheta&space;)))

![](http://latex.codecogs.com/gif.latex?%5Ctheta)的先验分布![](http://latex.codecogs.com/gif.latex?p(%5Ctheta))，我们可以按照实际情况来选择，比如抛硬币实验，我们就可以选择上面

说过的beta分布。

至于上面目标函数的求解，也和极大似然估计是一样的，对目标函数求导并令导数为0来求解。

MAP和极大似然的区别：

MAP允许我们把先验知识加入到估计模型中，这在样本很少的时候是很有用的，因为样本很少的时候我们的观测结果很可能出现偏差，此时先验知识会把估计的结果“拉”向先验，实际的预估结果将会在先验结果的两侧形成一个顶峰。通过调节先验分布的参数，比如beta分布的![](http://latex.codecogs.com/gif.latex?%5Calpha&space;,%5Cbeta)，我们还可以调节把估计的结果“拉”向先验的幅度，![](http://latex.codecogs.com/gif.latex?%5Calpha&space;,%5Cbeta)越大，这个顶峰越尖锐。这样的参数，我们叫做预估模型的“超参数”。

4、贝叶斯估计

贝叶斯估计和MAP挺像的，都是以最大化后验概率为目的。区别在于：

1）极大似然估计和MAP都是只返回了的预估值，就完事了

2）MAP在计算后验概率的时候，把分母p(X)给忽略了，在进行贝叶斯估计的时候则不能忽略

3）贝叶斯估计要计算整个后验概率的概率分布

还是回到这两个公式：

![](http://latex.codecogs.com/gif.latex?p(%5Ctheta&space;%7CX)=%5Cfrac%7Bp(X%7C%5Ctheta&space;)p(%5Ctheta&space;)%7D%7Bp(X)%7D)

![](http://latex.codecogs.com/gif.latex?p(X)=%5Cint&space;p(X%7C%5Ctheta&space;)p(%5Ctheta&space;)d%5Ctheta)

这里有一个技巧，对于一个特定的likehood，如果我们选择了一个先验概率分布，

通过上面两个公式的计算，得出的后验概率和先验概率是同分布的，这时候我们说这个先验分布是共轭先验。

可以举几个例子：

likehood为高斯分布，prior为高斯分布，则posterior也为高斯分布

likehood为伯努利分布（二项式分布），prior为beta分布，则posterior也为beta分布

likehood为多项式分布，prior为Dirichlet分布（beta分布的一个扩展），则posterior也为Dirichlet分布

根据上面的描述，在实践中我们往往会选择共轭先验来简化。在把后验概率推导为和先验概率一样的分布形式的时候，分母p(X)其实可以看做一个常数，往往充当了一个normalize，归一化的作用。

求解的时候，既然我们根据先验分布知道了后验是什么分布，那我们求出后验分布的期望值，即是需要估计的参数![](http://latex.codecogs.com/gif.latex?%5Ctheta)的值：

![](http://latex.codecogs.com/gif.latex?p=E%5Cleft&space;%5C%7B&space;%5Ctheta&space;%7CX&space;%5Cright&space;%5C%7D)

知道了后验是什么分布，那么求这个分布的期望值应该不是什么难事。

贝叶斯估计相对于最大后验估计的好处还在于，贝叶斯估计计算了整个后验概率的分布，从而也能求出其他一些比如分布的方差之类的值来供参考，比如计算出来方差太大的，我们可以认为分布不够好，从而把这个当做选择超参数的一个考虑因素。实际上，贝叶斯估计会比MAP把估计的结果往先验结果“拉”的程度还提高了一些，从而使估计结果更靠近先验结果。

5、和经验风险最小化和结构风险最小化的联系

来自《统计学习方法》

这里先解释一下经验风险最小化和结构风险最小化

经验风险最小化：

![](http://latex.codecogs.com/gif.latex?min%5Cfrac%7B1%7D%7BN%7D%5Csum_%7B1%7D%5E%7BN%7DL(yi,f(xi)))

结构风险最小化：

![](http://latex.codecogs.com/gif.latex?R_%7Bsrm%7D(f)=min%5Cfrac%7B1%7D%7BN%7D%5Csum_%7B1%7D%5E%7BN%7DL(yi,f(xi))+%5Clambda&space;J(f))

结构风险最小化等价于正则化，结构风险在经验风险上加上表示模型复杂度的正则化项。

正则化的好处将在下一篇文章中总结。

以下是来自《统计学习方法》的一些结论：

1、当模型是条件概率分布，损失函数是对数损失函数时，经验风险最小化等价于极大似然估计（对数损失函数的形式：![](http://latex.codecogs.com/gif.latex?L(Y,P(X%7CY))=-logP(Y%7CX))）

2、当模型是条件概率分布，损失函数是对数损失函数时，模型复杂度由模型的先验概率表示时，结构风险最小化等价于最大后验概率估计(MAP)

最大似然估计和贝叶斯参数估计的区别

参数估计问题是统计学中的经典问题。最常用的和有效的方法就是：最大似然和贝叶斯估计。

最大似然把待估的参数看做是确定性的量，只是其取值未知。最佳估计就是使得产生以观测到的样本的概率最大的那个值。

贝叶斯估计则把待估计的参数看成是符合某种先验概率分布的随机变量。对样本进行观测的过程，就是把先验概率密度转化为后验概率密度，这样就利用样本的信息修正了对参数的初始估计值。在贝叶斯估计中，一个典型的效果就是，每得到新的观测样本，都使得后验概率密度函数变得更加尖锐，使其在待估参数的真实值附近形成最大的尖峰。


**6. 总结**

最大似然估计，只是一种概率论在统计学的应用，它是参数估计的方法之一。说的是已知某个随机样本满足某种概率分布，但是其中具体的参数不清楚，参数估计就是通过若干次试验，观察其结果，利用结果推出参数的大概值。最大似然估计是建立在这样的思想上：已知某个参数能使这个样本出现的概率最大，我们当然不会再去选择其他小概率的样本，所以干脆就把这个参数作为估计的真实值。

求最大似然函数估计值的一般步骤： 

（1） 写出似然函数

（2） 对似然函数取对数，并整理

（3） 求导数

（4） 解似然方程

对于最大似然估计方法的应用，需要结合特定的环境，因为它需要你提供样本的已知模型进而来估算参数，例如在模式识别中，我们可以规定目标符合高斯模型。而且对于该算法，我理解为，“知道”和“能用”就行，没必要在程序设计时将该部分实现，因为在大多数程序中只会用到我最后推导出来的结果。个人建议，如有问题望有经验者指出。在文献[1]中讲解了本文的相关理论内容，在文献[2]附有3个推导例子。

**7. 参考文献**

[1]I.J. Myung. Tutorial on maximum likelihood estimation[J]. Journal of Mathematical Psychology, 2003, 90-100.

[2] http://edu6.teacher.com.cn/ttg006a/chap7/jiangjie/72.htm

