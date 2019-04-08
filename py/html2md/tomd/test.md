
			- [首页](http://x-algo.cn)

- [推荐系统](https://x-algo.cn/index.php/category/rcm/)

- [计算广告](https://x-algo.cn/index.php/category/adalgo/)

- [NLP](https://x-algo.cn/index.php/category/nlp/)

- [图像](https://x-algo.cn/index.php/category/tuxiang/)

- [模型](https://x-algo.cn/index.php/category/model/)

- [最优化](https://x-algo.cn/index.php/category/opt/)

<li id="menu-item-1184" class="menu-item menu-item-type-taxonomy menu-item-object-category menu-item-has-children menu-item-1184">[读书](https://x-algo.cn/index.php/category/read/)
<ul class="sub-menu">
	- [强化学习](https://x-algo.cn/index.php/category/read/qianghuaxuexi/)

	- [线性代数](https://x-algo.cn/index.php/category/read/line-algebra/)


								<li class="menusns">
					[关注本站 **](javascript:;)
					<ul class="sub-menu">
																																				- [** RSS订阅](http://x-algo.cn/index.php/feed/)
					
# [语音识别解码器(1)—自动机与半环](https://x-algo.cn/index.php/2017/04/29/speech-recognition-decoder-1-automata-and-semi-circular/)

主流的语音识别解码器都是根据H(HMM)，C(上下文)，L(发音字典)，G(语言模型)构建一个WFST解码网络，介绍语音识别解码器之前，有必要先了解自动机的定义、基本概率、操作等。

文章目录
- [1 有限状态接收器（Finite-State Acceptor，FSA）](#Finite-State_AcceptorFSA)
- [2 有限状态转移器（Finite-State Transducers, FST）](#Finite-State_Transducers_FST)
- [3 加权有限状态接收机（Weighted Finite-State Acceptors, WFSA）](#Weighted_Finite-State_Acceptors_WFSA)
- [4 加权有限状态转移器（Weighted Finite-State Transducers, WFST）](#Weighted_Finite-State_Transducers_WFST)
- [5 半环（Semiring）](#Semiring)
- [6 半环的性质](#i)
- [7 总结](#i-2)
- [8 参考文献](#i-3)

## **有限状态接收器（Finite-State Acceptor，FSA）**

有限自动机（Finite Automata, FA）是由一组有限的状态和状态转移的集合组成，其每一个转移都至少有一个标签。最基本的FA是FSA。对于给定的输入序列，FSA返回“接收”或者“不接收”两种状态。

图1（a）是一个FSA的示例，其节点和弧分别对应状态与状态的转移。例如，FSA可通过0,1,1,2,5接收一个符号序列“a,b,c,d”，但是无法接收到“a,b,d”序列，因此FSA表示了一组能被接收到的符号序列集合。图1（a）的FSA对应于正则表达式“ab*cd|bcd*”。我们假设状态0代表初始状态，状态5代表终止状态，如果不特别指出，本文中粗线单线圆代表初始状态，细线双线圆代表终止状态。
![](http://x-algo.cn/wp-content/uploads/2017/04/1-1024x541.jpg)

&nbsp;

## **有限状态转移器（Finite-State Transducers, FST）**

FST是FSA的扩展，其每一次状态转移时都有一个输出标签，叫做输入输出标签对，如图1（b）就是一个FST的例子。通过这样的标签对，FST可描述一组规则的转换或一组符号序列到另一组符号序列的转换。图1（b）的FST将符号序列“a,b,c,d”转换为另一个符号序列“z,y,x,w”。

## **加权有限状态接收机（Weighted Finite-State Acceptors, WFSA）**

WFSA在每一次状态转移时都有一个权重，在每次的初始状态都有初始权重，在每次的终止状态都有终止权重，权重一般是转移或初始/终止状态的概率或损失，权重会延每条路径进行累积，并在不同路径进行累加。图1（c）是一个WFSA的例子，每次状态转移的权重表示为“输入-标签/权重”，而初始和终止状态的权重表示为“状态 数量/权重”，在上图中，初始状态0的初始权重为0.5，终止状态5的终止权重为0.1。例如，上图中的WFSA沿着转移状态“0,1,1,2,5”以累积权重0.5*1.2*0.7*3*2*0.1=0.252接收到一个序列“a,b,c,d”。

## **加权有限状态转移器（Weighted Finite-State Transducers, WFST）**

WFST在每次状态转移时同时具有输出标签和权重，同时具有FST和WFSA的特性，图1（d）是一个WFST的例子，这里每次的状态转移标签都以“输入-标签: 输出-标签/权重”的形式进行转移，初始和终止状态也相应的分类了权重。在该图中的WFST，将符号序列“a,b,c,d”以0.252的累积权重转换为序列“z,y,x,w”。

WFST通过一个8元组 \((\sum ,\Lambda ,Q,I,F,E,\lambda ,\rho)\)<script type='math/tex'>(\sum ,\Lambda ,Q,I,F,E,\lambda ,\rho)</script> 定义，其中 \(\mathbb{K}\)<script type='math/tex'>\mathbb{K}</script> 是一组权重元素集合：

(1)  \(\sum\)<script type='math/tex'>\sum</script> 是一组有限的输入标签；

(2)  \(\Lambda\)<script type='math/tex'>\Lambda</script> 是一组有限的输出标签；

(3)  \(Q\)<script type='math/tex'>Q</script> 是一组有限的状态；

(4) \(I\subseteq Q\)<script type='math/tex'>I\subseteq Q</script>  是一组初始状态；

(5)  \(F\subseteq Q\)<script type='math/tex'>F\subseteq Q</script> 是一组终止状态；

(6)  \(E\subseteq Q\times (\sum\cup \{\epsilon \})\times\mathbb{K}\times Q\)<script type='math/tex'>E\subseteq Q\times (\sum\cup \{\epsilon \})\times\mathbb{K}\times Q</script> 是多组有限的转移；

(7)  \(\lambda :I\rightarrow \mathbb{K}\)<script type='math/tex'>\lambda :I\rightarrow \mathbb{K}</script> 是权重初始函数；

(8)  \(\rho :F\rightarrow \mathbb{K}\)<script type='math/tex'>\rho :F\rightarrow \mathbb{K}</script> 是权重终止函数；

" \(\epsilon\)<script type='math/tex'>\epsilon</script> "是一个元符号标签，它代表了无符号的输入输出，通过以上定义，图1（d）的WFST可以被定义如下：

(1)  \(\sum = \{a,b,c,d,e\}\)<script type='math/tex'>\sum = \{a,b,c,d,e\}</script> 

(2)  \(\Lambda = \{v,x,y,w,z\}\)<script type='math/tex'>\Lambda = \{v,x,y,w,z\}</script> 

(3)  \(Q = \{0,1,2,3,4,5\}\)<script type='math/tex'>Q = \{0,1,2,3,4,5\}</script> 

(4) \(I = \{0\}\)<script type='math/tex'>I = \{0\}</script> 

(5)  \(F = \{5\}\)<script type='math/tex'>F = \{5\}</script> 

(6)  \(E = \{(0,a,z,1.2,1),(0,b,y,0.8,3),(1,b,y,0.7,1),(1,c,x,3,2),(2,d,w,2,5),(3,c,x,0.2,4),(4,d,w,1.2,4),(4,e,v,0.6,5)\}\)<script type='math/tex'>E = \{(0,a,z,1.2,1),(0,b,y,0.8,3),(1,b,y,0.7,1),(1,c,x,3,2),(2,d,w,2,5),(3,c,x,0.2,4),(4,d,w,1.2,4),(4,e,v,0.6,5)\}</script> 

(7)  \(\lambda (0) = 0.5\)<script type='math/tex'>\lambda (0) = 0.5</script> 

(8)  \(\rho (5) = 0.1\)<script type='math/tex'>\rho (5) = 0.1</script> 

其中 \(E\)<script type='math/tex'>E</script> 中的每次转移由（源状态，输入标签，输出标签，权重，目标状态）组成，可以从以上描述中可知，FSA, FST, WFSA都是WFST的特殊形式，这些形式也可以由上述相似的元组表示。

## **半环（Semiring）**

半环是一种抽象代数的代数结构，在此用来定义WFST的加权值。半环一般定义为 \((\mathbb {K},\bigoplus ,\bigotimes ,\bar{0},\bar{1})\)<script type='math/tex'>(\mathbb {K},\bigoplus ,\bigotimes ,\bar{0},\bar{1})</script> ，这里 \(\mathbb {K}\)<script type='math/tex'>\mathbb {K}</script> 是一组数集， \(\bigoplus\)<script type='math/tex'>\bigoplus</script> 和 \(\bigotimes\)<script type='math/tex'>\bigotimes</script> 是定义在 \(\mathbb {K}\)<script type='math/tex'>\mathbb {K}</script> 上的两种二元运算，例如“加法”和“乘法”（不一定是真正的加法和乘法）， \(\bar{0}\)<script type='math/tex'>\bar{0}</script> 和 \(\bar{1}\)<script type='math/tex'>\bar{1}</script> 是特定的零元素和幺元素（不一定是真正的数0和数1）。形成半环的条件如下：

![](http://x-algo.cn/wp-content/uploads/2017/04/2.jpg)

WFST中常用的半环类型包括概率半环、对数半环、热带半环等，下表是这几种半环的定义：

![](http://x-algo.cn/wp-content/uploads/2017/04/3-1024x274.jpg)

其中， \(\bigoplus _{log}\)<script type='math/tex'>\bigoplus _{log}</script> 表示 \(\mathbb {K}\)<script type='math/tex'>\mathbb {K}</script> 上的任意 \(x\)<script type='math/tex'>x</script> 和 \(y\)<script type='math/tex'>y</script> 的二元操作 \(x\bigoplus _{log}y=-log(e^{-x}+e^{-y})\)<script type='math/tex'>x\bigoplus _{log}y=-log(e^{-x}+e^{-y})</script> 。在语音识别中主要使用热带半环，热带半环是由一组实数权重值组成，其“加法”操作对应取两数最小值，“乘法”操作就是普通的加法运算，而在一些WFST的优化步骤中也经常使用对数半环。

## **半环的性质**

(1) 交换律

如果半环 \(\mathbb {K},\bigoplus,\bigotimes,\bar{0},\bar{1}\)<script type='math/tex'>\mathbb {K},\bigoplus,\bigotimes,\bar{0},\bar{1}</script> 中的 \(\bigotimes\)<script type='math/tex'>\bigotimes</script> 满足交换律，则半环满足交换律，例如，对于所有 \(x,y\sqsubseteq?\mathbb {K}\)<script type='math/tex'>x,y\sqsubseteq?\mathbb {K}</script> ，有： \(x\bigotimes y=y\bigotimes x\)<script type='math/tex'>x\bigotimes y=y\bigotimes x</script> 

显然，热带半环和对数半环都满足交换律。

(2) 幂等律

如果半环 \(\mathbb {K},\bigoplus,\bigotimes,\bar{0},\bar{1}\)<script type='math/tex'>\mathbb {K},\bigoplus,\bigotimes,\bar{0},\bar{1}</script> 中的 \(\bigoplus\)<script type='math/tex'>\bigoplus</script> 满足 \(x=x\bigoplus x\)<script type='math/tex'>x=x\bigoplus x</script> ，则半环就满足幂等律。其中热带半环就满足幂等律，例如： \(x=min(x,x)\)<script type='math/tex'>x=min(x,x)</script> ,而对数半环不满足，例如： \(x\neq x\oplus _{log}x\)<script type='math/tex'>x\neq x\oplus _{log}x</script> 。

(3) k闭半环

对于大于等于0的整数k，对于所有 \(x\subset \mathbb {K}\)<script type='math/tex'>x\subset \mathbb {K}</script> ，当 \(\bigoplus _{n=0}^{k+1}x^{n}=\bigoplus _{n=0}^{k}x^{n}\)<script type='math/tex'>\bigoplus _{n=0}^{k+1}x^{n}=\bigoplus _{n=0}^{k}x^{n}</script> 时，半环 \(\mathbb {K},\bigoplus,\bigotimes,\bar{0},\bar{1}\)<script type='math/tex'>\mathbb {K},\bigoplus,\bigotimes,\bar{0},\bar{1}</script> 是k闭半环。

(4) 弱左整除

对于 \(x,y\subseteq \mathbb {K}\)<script type='math/tex'>x,y\subseteq \mathbb {K}</script> ，如果 \(x\bigoplus y\neq \bar{0}\)<script type='math/tex'>x\bigoplus y\neq \bar{0}</script> ,并且存在至少一个 \(z\)<script type='math/tex'>z</script> ，满足：

 \(x=(x\bigoplus y)\bigotimes z\)<script type='math/tex'>x=(x\bigoplus y)\bigotimes z</script> ，则半环 \(\mathbb {K},\bigoplus,\bigotimes,\bar{0},\bar{1}\)<script type='math/tex'>\mathbb {K},\bigoplus,\bigotimes,\bar{0},\bar{1}</script> 是弱左整除，

显然热带半环和对数半环都是弱左整除。

(5) 零和自由

如果半环 \(\mathbb {K},\bigoplus,\bigotimes,\bar{0},\bar{1}\)<script type='math/tex'>\mathbb {K},\bigoplus,\bigotimes,\bar{0},\bar{1}</script> 能从 \(x\bigoplus y\neq \bar{0}\)<script type='math/tex'>x\bigoplus y\neq \bar{0}</script> 推导出 \(x=y=\bar{0}\)<script type='math/tex'>x=y=\bar{0}</script> ，

则半环是零和自由。

## **总结**

本文简单介绍了自动机的基本类型、半环的概念和基本性质，对群论（我也不知道这是个什么鬼）有一个初步认识，下篇将介绍WFST的基本操作、转移器的组成。

## **参考文献**

Speech Recognition Algorithms Using Weighted Finite-State Transducers.

未经允许不得转载：[大数据算法](https://x-algo.cn) &raquo; [语音识别解码器(1)—自动机与半环](https://x-algo.cn/index.php/2017/04/29/speech-recognition-decoder-1-automata-and-semi-circular/)

### 相关推荐
- [CTC原理](https://x-algo.cn/index.php/2017/05/31/2345/)
- [语音识别解码器(2)—WFST基本操作](https://x-algo.cn/index.php/2017/05/29/2380/)

### 评论 <small>抢沙发</small>

[取消](javascript:;)

<input type="hidden" id="akismet_comment_nonce" name="akismet_comment_nonce" value="b992907238" />

<input type="hidden" id="ak_js" name="ak_js" value="53"/>

							- <label class="hide" for="author">昵称</label><input class="ipt" type="text" name="author" id="author" value="" tabindex="2" placeholder="昵称">昵称 (必填)

							- <label class="hide" for="email">邮箱</label><input class="ipt" type="text" name="email" id="email" value="" tabindex="3" placeholder="邮箱">邮箱 (必填)

							- <label class="hide" for="url">网址</label><input class="ipt" type="text" name="url" id="url" value="" tabindex="4" placeholder="网址">网址

						
### 热门标签

### 分类目录

	<li class="cat-item cat-item-51">[Deep Learning](https://x-algo.cn/index.php/category/deep-learning/) (29)
<ul class='children'>
	<li class="cat-item cat-item-87">[CNN](https://x-algo.cn/index.php/category/deep-learning/cnn/) (12)
</li>
	<li class="cat-item cat-item-121">[LSTM](https://x-algo.cn/index.php/category/deep-learning/lstm/) (2)
</li>
	<li class="cat-item cat-item-55">[RNN](https://x-algo.cn/index.php/category/deep-learning/rnn/) (3)
</li>
	<li class="cat-item cat-item-126">[语音识别](https://x-algo.cn/index.php/category/deep-learning/yuyinshibie/) (3)
</li>

	<li class="cat-item cat-item-139">[Machine comprehension](https://x-algo.cn/index.php/category/nlp/machine-comprehension/) (1)
</li>

	<li class="cat-item cat-item-81">[线性代数](https://x-algo.cn/index.php/category/%e6%95%b0%e5%ad%a6/%e7%ba%bf%e6%80%a7%e4%bb%a3%e6%95%b0/) (1)
</li>

	<li class="cat-item cat-item-11">[树](https://x-algo.cn/index.php/category/datastruct/tree/) (1)
</li>

	<li class="cat-item cat-item-18">[动态规划](https://x-algo.cn/index.php/category/algo/dp/) (1)
</li>
	<li class="cat-item cat-item-19">[搜索](https://x-algo.cn/index.php/category/algo/sousuo/) (2)
</li>
	<li class="cat-item cat-item-15">[语言入门](https://x-algo.cn/index.php/category/algo/yuyanrumen/) (2)
</li>
	<li class="cat-item cat-item-17">[贪心算法](https://x-algo.cn/index.php/category/algo/tanxinsuanfa/) (2)
</li>

	<li class="cat-item cat-item-124">[强化学习](https://x-algo.cn/index.php/category/read/qianghuaxuexi/) (17)
</li>
	<li class="cat-item cat-item-83">[线性代数](https://x-algo.cn/index.php/category/read/line-algebra/) (2)
</li>

### 文章归档

			- <a href='https://x-algo.cn/index.php/2018/11/'>2018年十一月</a>

	- <a href='https://x-algo.cn/index.php/2018/07/'>2018年七月</a>

	- <a href='https://x-algo.cn/index.php/2018/06/'>2018年六月</a>

	- <a href='https://x-algo.cn/index.php/2018/05/'>2018年五月</a>

	- <a href='https://x-algo.cn/index.php/2018/04/'>2018年四月</a>

	- <a href='https://x-algo.cn/index.php/2018/03/'>2018年三月</a>

	- <a href='https://x-algo.cn/index.php/2018/02/'>2018年二月</a>

	- <a href='https://x-algo.cn/index.php/2017/10/'>2017年十月</a>

	- <a href='https://x-algo.cn/index.php/2017/08/'>2017年八月</a>

	- <a href='https://x-algo.cn/index.php/2017/07/'>2017年七月</a>

	- <a href='https://x-algo.cn/index.php/2017/05/'>2017年五月</a>

	- <a href='https://x-algo.cn/index.php/2017/04/'>2017年四月</a>

	- <a href='https://x-algo.cn/index.php/2017/03/'>2017年三月</a>

	- <a href='https://x-algo.cn/index.php/2017/02/'>2017年二月</a>

	- <a href='https://x-algo.cn/index.php/2017/01/'>2017年一月</a>

	- <a href='https://x-algo.cn/index.php/2016/12/'>2016年十二月</a>

	- <a href='https://x-algo.cn/index.php/2016/11/'>2016年十一月</a>

	- <a href='https://x-algo.cn/index.php/2016/09/'>2016年九月</a>

	- <a href='https://x-algo.cn/index.php/2016/08/'>2016年八月</a>

	- <a href='https://x-algo.cn/index.php/2016/07/'>2016年七月</a>

	- <a href='https://x-algo.cn/index.php/2016/06/'>2016年六月</a>

	- <a href='https://x-algo.cn/index.php/2016/04/'>2016年四月</a>

	- <a href='https://x-algo.cn/index.php/2016/03/'>2016年三月</a>

	- <a href='https://x-algo.cn/index.php/2016/02/'>2016年二月</a>

		
## 关注大数据算法在工业界应用

&copy; 2019 [大数据算法](https://x-algo.cn) &nbsp; [网站地图](https://x-algo.cn/sitemap.xml) 
 &nbsp; 
<a href="http://www.miitbeian.gov.cn/" target="_blank"  >
 京ICP备16006270号-1
</a>
