# FTRL在线学习算法的前世今生-从SGD到TG再到FOBOS与RDA - Machine Learning with Peppa - CSDN博客





2019年02月28日 17:54:50[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：165








现在做在线学习和CTR常常会用到逻辑回归（ Logistic Regression），而传统的批量（batch）算法无法有效地处理超大规模的数据集和在线数据流，google先后三年时间（2010年-2013年）从理论研究到实际工程化实现的**FTRL（Follow-the-regularized-Leader）**算法，在处理诸如逻辑回归之类的带非光滑正则化项（例如1范数，做模型复杂度控制和稀疏化）的凸优化问题上性能非常出色，据闻国内各大互联网公司都第一时间应用到了实际产品中，我们的系统也使用了该算法。这里对FTRL相关发展背景和工程实现的一些指导点做一些介绍，凸优化的理论细节不做详细介绍，感兴趣可以去查阅相应paper，相关paper列表会在文后附上。机器学习并非本人在校时的专业方向，不过在校期间积累的基础不算太差，而且很多东西也是相通的，钻研一下基本意思都还能搞明白。当然，有不准确的地方欢迎大家讨论指正。

    本文主要会分三个部分介绍，如果对理论产生背景不感兴趣的话，可以直接看第3部分的工程实现（这一部分google13年那篇工程化的paper介绍得很详细）：
- 相关背景：包括通用性的问题描述、批量算法、传统在线学习算法等
- 简单介绍与FTRL关系比较密切的Truncated Gradient、FOBOS以及RDA（Regularized Dual Averaging）等算法
- FTRL理论公式以及工程实现（对前因后果和理论方面不感兴趣的可以直接看这一小节的工程实现部分）

**一、相关背景**

** 【问题描述】**

     对于loss函数+正则化的结构风险最小化的优化问题（逻辑回归也是这种形式）有两种等价的描述形式，以1范数为例，分别是：

　　a、无约束优化形式的soft regularization formulation：

![](https://images0.cnblogs.com/i/417893/201406/261924193462591.jpg)

      b、带约束项的凸优化问题convex constraint formulation：

![](https://images0.cnblogs.com/i/417893/201406/261926446888126.jpg)

       当合理地选择g时，二者是等价的。这里提这两种形式的问题描述，原因在于引出下面无约束优化和带约束优化问题的不同算法，对于不同的描述形式，会有一系列相关算法。

**   【批量（batch）算法】**

      批量算法中每次迭代对全体训练数据集进行计算（例如计算全局梯度），优点是精度和收敛还可以，缺点是无法有效处理大数据集（此时全局梯度计算代价太大），且没法应用于数据流做在线学习。这里分无约束优化形式和约束优化（与上面问题描述可以对应起来）两方面简单介绍一下一些传统批量算法。

      a、无约束优化形式：1、全局梯度下降![](https://images0.cnblogs.com/i/417893/201406/261934091316872.jpg)，很常用的算法，就不细说了，每一步求一个目标函数的全局梯度，用非增学习率进行迭代；2、牛顿法（切线近似）、LBFGS（割线拟牛顿，用之前迭代结果近似Hessian黑塞矩阵的逆矩阵，BFGS似乎是几个人名的首字母的简称）等方法。牛顿和拟牛顿等方法一般对于光滑的正则约束项（例如2范数）效果很好，据说是求解2范数约束的逻辑回归类问题最好的方法，应用也比较广，但是当目标函数带L1非光滑、带不可微点的约束项后，牛顿类方法比较无力，理论上需要做修改。感兴趣的可以去查查无约束优化的相关数值计算的书，我也没有更深入研究相关细节，这里不做重点关注。

      b、不等式约束凸优化形式：1、传统的不等式约束优化算法内点法等；2、投影梯度下降（约束优化表示下），gt是subgradient，直观含义是每步迭代后，迭代结果可能位于约束集合之外，然后取该迭代结果在约束凸集合上的投影作为新的迭代结果（第二个公式中那个符号标识向X的投影）：

![](https://images0.cnblogs.com/i/417893/201406/261944126931563.jpg)



![](https://images0.cnblogs.com/i/417893/201406/261941243543067.jpg)

**【在线算法】**

　　如上所述，批量算法有自身的局限性，而在线学习算法的特点是：每来一个训练样本，就用该样本产生的loss和梯度对模型迭代一次，一个一个数据地进行训练，因此可以处理大数据量训练和在线训练。常用的有在线梯度下降（OGD）和随机梯度下降（SGD）等，本质思想是对上面【问题描述】中的**未加和的单个数据的loss函数 L（w，zi)做梯度下降，**因为每一步的方向并不是全局最优的，所以整体呈现出来的会是一个看似随机的下降路线。典型迭代公式如下：

![](https://images0.cnblogs.com/i/417893/201406/261955531922170.jpg)

这里使用混合正则化项：![](https://images0.cnblogs.com/i/417893/201406/261955580516988.jpg)，例如可能是1范数与2范数强凸项的混合![](https://images0.cnblogs.com/i/417893/201406/261956031711649.jpg)（后面会看到其实很多都是这种混合正则化的格式，而且是有一定直观含义的）。迭代公式中：gt是loss函数（单点的loss，未加和）的subgradient，与gt相加的那一项是混合正则化项中的第二项的梯度，投影集合C是约束空间（例如可能是1范数的约束空间），跟上面介绍的投影梯度下降类似的做法。

　　梯度下降类的方法的优点是精度确实不错，但是不足相关paper主要提到两点：

　　1、简单的在线梯度下降很难产生真正稀疏的解，稀疏性在机器学习中是很看重的事情，尤其我们做工程应用，稀疏的特征会大大减少predict时的内存和复杂度。这一点其实很容易理解，说白了，即便加入L1范数（L1范数能引入稀疏解的简单示例可以产看PRML那本书的第二章，我前面一篇blog的ppt里也大概提了），因为是浮点运算，训练出的w向量也很难出现绝对的零。到这里，大家可能会想说，那还不容易，当计算出的w对应维度的值很小时，我们就强制置为零不就稀疏了么。对的，其实不少人就是这么做的，后面的Truncated Gradient和FOBOS都是类似思想的应用；

　　2、对于不可微点的迭代会存在一些问题，具体有什么问题，有一篇paper是这么说的：the iterates of the subgradient method are very rarely at the points of non-differentiability。我前后看了半天也没看明白，有熟悉的同学可以指导一下。



**二、Truncated Gradient、FOBOS以及RDA（Regularized Dual Averaging）**

　　上面提到了，稀疏性在机器学习中是很重要的一件事情，下面给出常见的三种做稀疏解的途径：

**　1）、简单加入L1范数**

　　　　–局限如上面所提，a+b两个float数很难绝对等于零，无法产生真正稀疏的特征权重

**2）、在1范数的基础上做截断**，最直观没技术含量的思路，那就设定一个阈值，做截断来保证稀疏，可以结合L1范数

　　　　–简单截断方法，每online训练K个数据截断一次，对OGD的迭代结果，每K步做一次截断置零：

![](https://images0.cnblogs.com/i/417893/201406/262019497931524.jpg)

![](https://images0.cnblogs.com/i/417893/201406/262019559244301.jpg)

　　　　但是简单截断方法有问题：权重小，可能是确实是无用特征，还或者可能是该特征才刚被更新一次（例如训练刚开始的阶段、或者训练数据中包含该特征的样本数本来就很少），另外，简单rounding技术太aggressive了，可能会破坏在线训练算法的理论完备性。

　　　　-简单截断基础上，不太aggressive的Truncated gradient （09年的工作），其实后面的FOBOS也可以归为这一类：

![](https://images0.cnblogs.com/i/417893/201406/262023382526323.jpg)

![](https://images0.cnblogs.com/i/417893/201406/262023454424585.jpg)

**　3）、Black-box wrapper approaches：**

　　　　–黑盒的方法去除一些特征，然后重新训练的看被消去的特征是否有效。

　　　　–需要在数据集上对算法跑多次，所以不太实用

      下面会提一下FOBOS（Forward-Backward Splitting method ，其实应该叫FOBAS的，历史原因）以及RDA，因为后面的FTRL其实相当于综合了这两种算法的优点：

**　a、FOBOS，google和伯克利09年的工作：**

　　　　–可以看作truncated gradient的一种特殊形式

　　　　–基本思想：跟projected subgradient方法类似，不过将每一个数据的迭代过程，分解成一个经验损失梯度下降迭代和一个最优化问题。分解出的第二个最优化问题，有两项：第一项2范数那一项表示不能离第一步loss损失迭代结果太远，第二项是正则化项，用来限定模型复杂度抑制过拟合和做稀疏化等。这个最优化问题有一些特殊的性质，从而保证了最终结果的稀疏性和理论上的完备，具体细节感兴趣的可以查看对应paper。我这里更多关注直观含义和工程实现，忽略理论方面的内容。

![](https://images0.cnblogs.com/i/417893/201406/262027576305730.jpg)

**b、RDA（Regularized dual averaging），微软10年的工作**，更加理论性一些，这里就直接略过去了，仅对其特点做一个简单介绍：

　　　　–非梯度下降类方法，属于更加通用的一个primal-dual algorithmic schema的一个应用



　　　　–克服了SGD类方法所欠缺的exploiting problem structure，especially for problems with explicit regularization。



　　　　–能够更好地在精度和稀疏性之间做trade-off

　　ok，背景和一些铺垫终于完成了，下面重点进入FTRL的部分。。。



**三、FTRL (Follow-the-regularized-Leader)**

**【发展历程】**

　　FTRL的理论推进和工程应用首先要感谢这个人：H. Brendan McMahan, google这哥们儿护了三年的坑，直到13年工程性paper出来。发展历程和基本说明如下：

　　　 –10年理论性paper，但未显式地支持正则化项迭代；11年证明regret bound以及引入通用的正则化项；11年另一篇的paper揭示OGD、FOBOS、RDA等算法与FTRL关系；13年的paper给出了工程性实现，并且附带了详细的伪代码，开始被大规模应用。

　　　 –可以看作RDA和FOBOS的混合，但在L1范数或者其他非光滑的正则项下，FTRL比前两者更加有效



**【基本思想及迭代公式】**





　　我简单画了个图：

![](https://images0.cnblogs.com/i/417893/201406/262038582382250.png)

　　与其他在线算法的迭代公式的对比（其实OGD如何一步步到类似形式的迭代公式的过程，限于时间，这里就不细说了，最后我会附一篇自己做分享会时做的ppt，里面有，感兴趣的可以下载看看），不同的方法在这种统一的描述形式下，区别点仅在第二项和第三项的处理方式：

![](https://images0.cnblogs.com/i/417893/201406/262042005547614.jpg)



　　–第一项：梯度或累积梯度；

　　–第二项：L1正则化项的处理；

　　–第三项：这个累积加和限定了新的迭代结果x不要离已迭代过的解太远（也即FTRL-Proximal中proximal的含义），或者离0太远（central），这一项其实也是low regret的需求



**【工程实现】**

　　大家对上面那一大坨前因后果和公式都不感兴趣，ok，没关系，google非常贴心地在13年给出了一篇工程性很强的paper，其实大部分公司使用FTRL的，根本不会关心上面那一大段东西，直接按着伪代码写，调调参，看结果很不错就可以了。我们公司开始就是这么搞的，哈哈，不过人总是要有点儿好奇心的不是，深究一下前因后果和基本的理论公式感觉还是挺不同的。

　　逻辑回归下的per-coordinate FTRL_Proximal的伪代码如下，在公式表达的基础上做了一些变换和实现上的trick，细节paper里有，大家在自己做实现的时候，可以在实际数据集上再并行加加速：

![](https://images0.cnblogs.com/i/417893/201406/262049050552702.jpg)

　　四个参数的设定结合paper里的指导意见以及反复实验测试，找一组适合自己问题的参数就可以了。这里我想提一点，即上面所谓的**per-coordinate**，其意思是**FTRL是对w每一维分开训练更新的，每一维使用的是不同的学习速率**，也是上面代码中lamda2之前的那一项。与w所有特征维度使用统一的学习速率相比，**这种方法考虑了训练样本本身在不同特征上分布的不均匀性**，如果包含w某一个维度特征的训练样本很少，每一个样本都很珍贵，那么该特征维度对应的训练速率可以独自保持比较大的值，每来一个包含该特征的样本，就可以在该样本的梯度上前进一大步，而不需要与其他特征维度的前进步调强行保持一致。



**【工程实现中的memory saving策略】**

　　这里对google所提的一些节省内存的实现细节做一个介绍
- **Predict时的memory saving：**

　　　　–L1范数加策略，训练结果w很稀疏，在用w做predict的时候节省了内存，很直观，不细说了


- **Training时的memory saving：**
- 在线丢弃训练数据中很少出现的特征(probabilistic feature inclusion)，但是对于online set，对全数据进行pre-process查看哪些特征出现地很少、或者哪些特征无用，是代价很大的事情，所以要想训练的时候就做稀疏化，就要想一些在线的方法（FTRL分开更新的w各维度，每一维不同的步长，per-coordinate）

　　　　1）Poisson Inclusion：对某一维度特征所来的训练样本，以p的概率接受并更新模型；

　　　　2）Bloom Filter Inclusion：用bloom filter从概率上做某一特征出现k次才更新

　　2. 浮点数重新编码

　　　　1)　　特征权重不需要用32bit或64bit的浮点数存储，存储浪费空间

　　　　2)　　16bit encoding，但是要注意处理rounding技术对regret带来的影响

　　3. 训练若干相似model

　　　　1）对同一份训练数据序列，同时训练多个相似的model

　　　　2）这些model有各自独享的一些feature，也有一些共享的feature

　　　　3）出发点：有的特征维度可以是各个模型独享的，而有的各个模型共享的特征，可以用同样的数据训练。

　　4. Single Value Structure（据说有公司已经在实际中这么搞，大数据量下也能够保证不错的auc）

  　　　 1）多个model公用一个feature存储（例如放到cbase或redis中），各个model都更新这个共有的feature结构

　　　   2）对于某一个model，对于他所训练的特征向量的某一维，直接计算一个迭代结果并与旧值做一个平均

　　5.  使用正负样本的数目来计算梯度的和（所有的model具有同样的N和P）
![](https://images0.cnblogs.com/i/417893/201406/262104336926606.jpg)

　　6. Subsampling Training Data



　　　　1）在实际中，CTR远小于50%，所以正样本更加有价值。通过对训练数据集进行subsampling，可以大大减小训练数据集的大小



　　　　2）正样本全部采（至少有一个广告被点击的query数据），负样本使用一个比例r采样（完全没有广告被点击的query数据）。但是直接在这种采样上进行训练，会导致比较大的biased prediction

　　　　3）解决办法：训练的时候，对样本再乘一个权重。权重直接乘到loss上面，从而梯度也会乘以这个权重。

![](https://images0.cnblogs.com/i/417893/201406/262106275498191.jpg)

**　先采样减少负样本数目，在训练的时候再用权重弥补负样本，非常不错的想法。**



【References】

我大概标注了一下各篇paper的主要内容，感兴趣的可以有选择性地看一下，如果只关注工程实现，看标红的那篇就ok了：

[1] J. Langford, L. Li, and T. Zhang. Sparse online learning via truncated gradient.JMLR, 10, 2009. （截断梯度的paper）

[2] H. B. McMahan. Follow-the-regularized-leader and mirror descent: Equivalence theorems and L1 regularization. In AISTATS, 2011 （FOBOS、RDA、FTRL等各种方法对比的paper）

[3] L. Xiao. Dual averaging method for regularized stochastic learning and online optimization. In NIPS, 2009 （RDA方法）

[4] J. Duchi and Y. Singer. Efficient learning using forward-backward splitting. In Advances in Neural Information Processing Systems 22, pages 495{503. 2009. （FOBOS方法）

[5] H. Brendan McMahan, Gary Holt, D. Sculley, Michael Young, Dietmar Ebner, Julian Grady, Lan Nie, Todd Phillips, Eugene Davydov, Daniel Golovin, Sharat Chikkerur, Dan Liu, Martin Wattenberg, Arnar Mar Hrafnkelsson, Tom Boulos, Jeremy Kubica, Ad Click Prediction: a View from the Trenches, Proceedings of the 19th ACM SIGKDD International Conference on Knowledge Discovery and Data Mining (KDD) (2013) **（这篇是那篇工程性的paper）**

[6] H. Brendan McMahan. A unied analysis of regular-ized dual averaging and composite mirror descent with implicit updates. Submitted, 2011 （FTRL理论发展，regret bound和加入通用正则化项）

[7] H. Brendan McMahan and Matthew Streeter. Adap-tive bound optimization for online convex optimiza-tion. InCOLT, 2010 （开始的那篇理论性paper）



后面附上我在组里分享时做的ppt，感兴趣的可以看看：[http://pan.baidu.com/s/1eQvfo6e](http://pan.baidu.com/s/1eQvfo6e)



