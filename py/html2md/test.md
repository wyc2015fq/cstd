[
			](https://x-algo.cn)- [首页](http://x-algo.cn)
- [推荐系统](https://x-algo.cn/index.php/category/rcm/)
- [计算广告](https://x-algo.cn/index.php/category/adalgo/)
- [NLP](https://x-algo.cn/index.php/category/nlp/)
- [图像](https://x-algo.cn/index.php/category/tuxiang/)
- [模型](https://x-algo.cn/index.php/category/model/)
- [最优化](https://x-algo.cn/index.php/category/opt/)
- [读书](https://x-algo.cn/index.php/category/read/)- 

	- [强化学习](https://x-algo.cn/index.php/category/read/qianghuaxuexi/)
	- [线性代数](https://x-algo.cn/index.php/category/read/line-algebra/)
- 

							[
					](javascript:;)
								- 
					- [关注本站 ](javascript:;)- 
					
																																				- [ RSS订阅](http://x-algo.cn/index.php/feed/)					- 
				
			
# [语音识别解码器(1)—自动机与半环](https://x-algo.cn/index.php/2017/04/29/speech-recognition-decoder-1-automata-and-semi-circular/)
2017-04-29分类：[语音识别](https://x-algo.cn/index.php/category/deep-learning/yuyinshibie/)阅读(5074)评论(0)
主流的语音识别解码器都是根据H(HMM)，C(上下文)，L(发音字典)，G(语言模型)构建一个WFST解码网络，介绍语音识别解码器之前，有必要先了解自动机的定义、基本概率、操作等。

文章目录
[1](#Finite-State_AcceptorFSA)-  有限状态接收器（Finite-State Acceptor，FSA）[2](#Finite-State_Transducers_FST)-  有限状态转移器（Finite-State Transducers, FST）[3](#Weighted_Finite-State_Acceptors_WFSA)-  加权有限状态接收机（Weighted Finite-State Acceptors, WFSA）[4](#Weighted_Finite-State_Transducers_WFST)-  加权有限状态转移器（Weighted Finite-State Transducers, WFST）[5](#Semiring)-  半环（Semiring）[6](#i)-  半环的性质[7](#i-2)-  总结[8](#i-3)-  参考文献**有限状态接收器（Finite-State Acceptor，FSA）**
有限自动机（Finite Automata, FA）是由一组有限的状态和状态转移的集合组成，其每一个转移都至少有一个标签。最基本的FA是FSA。对于给定的输入序列，FSA返回“接收”或者“不接收”两种状态。

图1（a）是一个FSA的示例，其节点和弧分别对应状态与状态的转移。例如，FSA可通过0,1,1,2,5接收一个符号序列“a,b,c,d”，但是无法接收到“a,b,d”序列，因此FSA表示了一组能被接收到的符号序列集合。图1（a）的FSA对应于正则表达式“ab*cd|bcd*”。我们假设状态0代表初始状态，状态5代表终止状态，如果不特别指出，本文中粗线单线圆代表初始状态，细线双线圆代表终止状态。


 
**有限状态转移器（Finite-State Transducers, FST）**
FST是FSA的扩展，其每一次状态转移时都有一个输出标签，叫做输入输出标签对，如图1（b）就是一个FST的例子。通过这样的标签对，FST可描述一组规则的转换或一组符号序列到另一组符号序列的转换。图1（b）的FST将符号序列“a,b,c,d”转换为另一个符号序列“z,y,x,w”。
**加权有限状态接收机（Weighted Finite-State Acceptors, WFSA）**
WFSA在每一次状态转移时都有一个权重，在每次的初始状态都有初始权重，在每次的终止状态都有终止权重，权重一般是转移或初始/终止状态的概率或损失，权重会延每条路径进行累积，并在不同路径进行累加。图1（c）是一个WFSA的例子，每次状态转移的权重表示为“输入-标签/权重”，而初始和终止状态的权重表示为“状态 数量/权重”，在上图中，初始状态0的初始权重为0.5，终止状态5的终止权重为0.1。例如，上图中的WFSA沿着转移状态“0,1,1,2,5”以累积权重0.5*1.2*0.7*3*2*0.1=0.252接收到一个序列“a,b,c,d”。
**加权有限状态转移器（Weighted Finite-State Transducers, WFST）**
WFST在每次状态转移时同时具有输出标签和权重，同时具有FST和WFSA的特性，图1（d）是一个WFST的例子，这里每次的状态转移标签都以“输入-标签: 输出-标签/权重”的形式进行转移，初始和终止状态也相应的分类了权重。在该图中的WFST，将符号序列“a,b,c,d”以0.252的累积权重转换为序列“z,y,x,w”。

WFST通过一个8元组 
\((\sum ,\Lambda ,Q,I,F,E,\lambda ,\rho)\)
(\sum ,\Lambda ,Q,I,F,E,\lambda ,\rho)

 定义，其中 
\(\mathbb{K}\)
\mathbb{K}

 是一组权重元素集合：

(1)  
\(\sum\)
\sum

 是一组有限的输入标签；

(2)  
\(\Lambda\)
\Lambda

 是一组有限的输出标签；

(3)  
\(Q\)
Q

 是一组有限的状态；

(4) 
\(I\subseteq Q\)
I\subseteq Q

  是一组初始状态；

(5)  
\(F\subseteq Q\)
F\subseteq Q

 是一组终止状态；

(6)  
\(E\subseteq Q\times (\sum\cup \{\epsilon \})\times\mathbb{K}\times Q\)
E\subseteq Q\times (\sum\cup \{\epsilon \})\times\mathbb{K}\times Q

 是多组有限的转移；

(7)  
\(\lambda :I\rightarrow \mathbb{K}\)
\lambda :I\rightarrow \mathbb{K}

 是权重初始函数；

(8)  
\(\rho :F\rightarrow \mathbb{K}\)
\rho :F\rightarrow \mathbb{K}

 是权重终止函数；

" 
\(\epsilon\)
\epsilon

 "是一个元符号标签，它代表了无符号的输入输出，通过以上定义，图1（d）的WFST可以被定义如下：

(1)  
\(\sum = \{a,b,c,d,e\}\)
\sum = \{a,b,c,d,e\}

 

(2)  
\(\Lambda = \{v,x,y,w,z\}\)
\Lambda = \{v,x,y,w,z\}

 

(3)  
\(Q = \{0,1,2,3,4,5\}\)
Q = \{0,1,2,3,4,5\}

 

(4) 
\(I = \{0\}\)
I = \{0\}

 

(5)  
\(F = \{5\}\)
F = \{5\}

 

(6)  
\(E = \{(0,a,z,1.2,1),(0,b,y,0.8,3),(1,b,y,0.7,1),(1,c,x,3,2),(2,d,w,2,5),(3,c,x,0.2,4),(4,d,w,1.2,4),(4,e,v,0.6,5)\}\)
E = \{(0,a,z,1.2,1),(0,b,y,0.8,3),(1,b,y,0.7,1),(1,c,x,3,2),(2,d,w,2,5),(3,c,x,0.2,4),(4,d,w,1.2,4),(4,e,v,0.6,5)\}

 

(7)  
\(\lambda (0) = 0.5\)
\lambda (0) = 0.5

 

(8)  
\(\rho (5) = 0.1\)
\rho (5) = 0.1

 

其中 
\(E\)
E

 中的每次转移由（源状态，输入标签，输出标签，权重，目标状态）组成，可以从以上描述中可知，FSA, FST, WFSA都是WFST的特殊形式，这些形式也可以由上述相似的元组表示。
**半环（Semiring）**
半环是一种抽象代数的代数结构，在此用来定义WFST的加权值。半环一般定义为 
\((\mathbb {K},\bigoplus ,\bigotimes ,\bar{0},\bar{1})\)
(\mathbb {K},\bigoplus ,\bigotimes ,\bar{0},\bar{1})

 ，这里 
\(\mathbb {K}\)
\mathbb {K}

 是一组数集， 
\(\bigoplus\)
\bigoplus

 和 
\(\bigotimes\)
\bigotimes

 是定义在 
\(\mathbb {K}\)
\mathbb {K}

 上的两种二元运算，例如“加法”和“乘法”（不一定是真正的加法和乘法）， 
\(\bar{0}\)
\bar{0}

 和 
\(\bar{1}\)
\bar{1}

 是特定的零元素和幺元素（不一定是真正的数0和数1）。形成半环的条件如下：

WFST中常用的半环类型包括概率半环、对数半环、热带半环等，下表是这几种半环的定义：

其中， 
\(\bigoplus _{log}\)
\bigoplus _{log}

 表示 
\(\mathbb {K}\)
\mathbb {K}

 上的任意 
\(x\)
x

 和 
\(y\)
y

 的二元操作 
\(x\bigoplus _{log}y=-log(e^{-x}+e^{-y})\)
x\bigoplus _{log}y=-log(e^{-x}+e^{-y})

 。在语音识别中主要使用热带半环，热带半环是由一组实数权重值组成，其“加法”操作对应取两数最小值，“乘法”操作就是普通的加法运算，而在一些WFST的优化步骤中也经常使用对数半环。
**半环的性质**
(1) 交换律

如果半环 
\(\mathbb {K},\bigoplus,\bigotimes,\bar{0},\bar{1}\)
\mathbb {K},\bigoplus,\bigotimes,\bar{0},\bar{1}

 中的 
\(\bigotimes\)
\bigotimes

 满足交换律，则半环满足交换律，例如，对于所有 
\(x,y\sqsubseteq?\mathbb {K}\)
x,y\sqsubseteq?\mathbb {K}

 ，有： 
\(x\bigotimes y=y\bigotimes x\)
x\bigotimes y=y\bigotimes x

 

显然，热带半环和对数半环都满足交换律。

(2) 幂等律

如果半环 
\(\mathbb {K},\bigoplus,\bigotimes,\bar{0},\bar{1}\)
\mathbb {K},\bigoplus,\bigotimes,\bar{0},\bar{1}

 中的 
\(\bigoplus\)
\bigoplus

 满足 
\(x=x\bigoplus x\)
x=x\bigoplus x

 ，则半环就满足幂等律。其中热带半环就满足幂等律，例如： 
\(x=min(x,x)\)
x=min(x,x)

 ,而对数半环不满足，例如： 
\(x\neq x\oplus _{log}x\)
x\neq x\oplus _{log}x

 。

(3) k闭半环

对于大于等于0的整数k，对于所有 
\(x\subset \mathbb {K}\)
x\subset \mathbb {K}

 ，当 
\(\bigoplus _{n=0}^{k+1}x^{n}=\bigoplus _{n=0}^{k}x^{n}\)
\bigoplus _{n=0}^{k+1}x^{n}=\bigoplus _{n=0}^{k}x^{n}

 时，半环 
\(\mathbb {K},\bigoplus,\bigotimes,\bar{0},\bar{1}\)
\mathbb {K},\bigoplus,\bigotimes,\bar{0},\bar{1}

 是k闭半环。

(4) 弱左整除

对于 
\(x,y\subseteq \mathbb {K}\)
x,y\subseteq \mathbb {K}

 ，如果 
\(x\bigoplus y\neq \bar{0}\)
x\bigoplus y\neq \bar{0}

 ,并且存在至少一个 
\(z\)
z

 ，满足：

 
\(x=(x\bigoplus y)\bigotimes z\)
x=(x\bigoplus y)\bigotimes z

 ，则半环 
\(\mathbb {K},\bigoplus,\bigotimes,\bar{0},\bar{1}\)
\mathbb {K},\bigoplus,\bigotimes,\bar{0},\bar{1}

 是弱左整除，

显然热带半环和对数半环都是弱左整除。

(5) 零和自由

如果半环 
\(\mathbb {K},\bigoplus,\bigotimes,\bar{0},\bar{1}\)
\mathbb {K},\bigoplus,\bigotimes,\bar{0},\bar{1}

 能从 
\(x\bigoplus y\neq \bar{0}\)
x\bigoplus y\neq \bar{0}

 推导出 
\(x=y=\bar{0}\)
x=y=\bar{0}

 ，

则半环是零和自由。
**总结**
本文简单介绍了自动机的基本类型、半环的概念和基本性质，对群论（我也不知道这是个什么鬼）有一个初步认识，下篇将介绍WFST的基本操作、转移器的组成。
**参考文献**
Speech Recognition Algorithms Using Weighted Finite-State Transducers.

未经允许不得转载：

[大数据算法](https://x-algo.cn)

 » 

[语音识别解码器(1)—自动机与半环](https://x-algo.cn/index.php/2017/04/29/speech-recognition-decoder-1-automata-and-semi-circular/)

### 相关推荐
- [CTC原理](https://x-algo.cn/index.php/2017/05/31/2345/)- [语音识别解码器(2)—WFST基本操作](https://x-algo.cn/index.php/2017/05/29/2380/)
### 评论 

### 抢沙发

[取消](javascript:;)
[*](#)
							- 昵称昵称 (必填)
							- 邮箱邮箱 (必填)
							- 网址网址
						
### 热门标签

### [分类目录](https://x-algo.cn/index.php/tag/css/)

	- [Deep Learning](https://x-algo.cn/index.php/category/deep-learning/)-  (29)

	- [CNN](https://x-algo.cn/index.php/category/deep-learning/cnn/)-  (12)

	- [LSTM](https://x-algo.cn/index.php/category/deep-learning/lstm/)-  (2)

	- [RNN](https://x-algo.cn/index.php/category/deep-learning/rnn/)-  (3)

	- [语音识别](https://x-algo.cn/index.php/category/deep-learning/yuyinshibie/)-  (3)

- 

	- [GAN](https://x-algo.cn/index.php/category/gan/)-  (3)

	- [GPU编程](https://x-algo.cn/index.php/category/gpucoding/)-  (4)

	- [NLP](https://x-algo.cn/index.php/category/nlp/)-  (24)

	- [Machine comprehension](https://x-algo.cn/index.php/category/nlp/machine-comprehension/)-  (1)

- 

	- [图像](https://x-algo.cn/index.php/category/tuxiang/)-  (11)

	- [推荐系统](https://x-algo.cn/index.php/category/rcm/)-  (9)

	- [数学](https://x-algo.cn/index.php/category/%e6%95%b0%e5%ad%a6/)-  (1)

	- [线性代数](https://x-algo.cn/index.php/category/%e6%95%b0%e5%ad%a6/%e7%ba%bf%e6%80%a7%e4%bb%a3%e6%95%b0/)-  (1)

- 

	- [数据结构](https://x-algo.cn/index.php/category/datastruct/)-  (1)

	- [树](https://x-algo.cn/index.php/category/datastruct/tree/)-  (1)

- 

	- [最优化](https://x-algo.cn/index.php/category/opt/)-  (10)

	- [杂](https://x-algo.cn/index.php/category/%e6%9d%82/)-  (1)

	- [模型](https://x-algo.cn/index.php/category/model/)-  (18)

	- [算法](https://x-algo.cn/index.php/category/algo/)-  (7)

	- [动态规划](https://x-algo.cn/index.php/category/algo/dp/)-  (1)

	- [搜索](https://x-algo.cn/index.php/category/algo/sousuo/)-  (2)

	- [语言入门](https://x-algo.cn/index.php/category/algo/yuyanrumen/)-  (2)

	- [贪心算法](https://x-algo.cn/index.php/category/algo/tanxinsuanfa/)-  (2)

- 

	- [计算广告](https://x-algo.cn/index.php/category/adalgo/)-  (2)

	- [词表](https://x-algo.cn/index.php/category/word_dic/)-  (2)

	- [读书](https://x-algo.cn/index.php/category/read/)-  (19)

	- [强化学习](https://x-algo.cn/index.php/category/read/qianghuaxuexi/)-  (17)

	- [线性代数](https://x-algo.cn/index.php/category/read/line-algebra/)-  (2)

- 

	- [调度系统](https://x-algo.cn/index.php/category/schedule/)-  (1)

		
### 文章归档

			- [2018年十一月](https://x-algo.cn/index.php/2018/11/)
	- [2018年七月](https://x-algo.cn/index.php/2018/07/)
	- [2018年六月](https://x-algo.cn/index.php/2018/06/)
	- [2018年五月](https://x-algo.cn/index.php/2018/05/)
	- [2018年四月](https://x-algo.cn/index.php/2018/04/)
	- [2018年三月](https://x-algo.cn/index.php/2018/03/)
	- [2018年二月](https://x-algo.cn/index.php/2018/02/)
	- [2017年十月](https://x-algo.cn/index.php/2017/10/)
	- [2017年八月](https://x-algo.cn/index.php/2017/08/)
	- [2017年七月](https://x-algo.cn/index.php/2017/07/)
	- [2017年五月](https://x-algo.cn/index.php/2017/05/)
	- [2017年四月](https://x-algo.cn/index.php/2017/04/)
	- [2017年三月](https://x-algo.cn/index.php/2017/03/)
	- [2017年二月](https://x-algo.cn/index.php/2017/02/)
	- [2017年一月](https://x-algo.cn/index.php/2017/01/)
	- [2016年十二月](https://x-algo.cn/index.php/2016/12/)
	- [2016年十一月](https://x-algo.cn/index.php/2016/11/)
	- [2016年九月](https://x-algo.cn/index.php/2016/09/)
	- [2016年八月](https://x-algo.cn/index.php/2016/08/)
	- [2016年七月](https://x-algo.cn/index.php/2016/07/)
	- [2016年六月](https://x-algo.cn/index.php/2016/06/)
	- [2016年四月](https://x-algo.cn/index.php/2016/04/)
	- [2016年三月](https://x-algo.cn/index.php/2016/03/)
	- [2016年二月](https://x-algo.cn/index.php/2016/02/)
		
## 关注大数据算法在工业界应用

[© 2019 ](http://x-algo.cn/index.php/about-site)

[大数据算法](https://x-algo.cn)

   

[网站地图](https://x-algo.cn/sitemap.xml)

 
   


[
 京ICP备16006270号-1
](http://www.miitbeian.gov.cn/)
