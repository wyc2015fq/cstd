# 论文阅读：《Ask Me Anything: Dynamic Memory Networks for Natural Language Processing》 - Soul Joy Hub - CSDN博客

2018年11月08日 14:13:10[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：181


论文：[https://arxiv.org/pdf/1506.07285.pdf](https://arxiv.org/pdf/1506.07285.pdf)

原文：[http://www.hankcs.com/nlp/cs224n-dmn-question-answering.html](http://www.hankcs.com/nlp/cs224n-dmn-question-answering.html)

![hankcs.com 2017-07-12 上午10.52.33.png](http://wx2.sinaimg.cn/large/006Fmjmcly1fhgwvmstwnj313d0fjn4i.jpg)![hankcs.com 2017-07-12 上午11.22.45.png](http://wx3.sinaimg.cn/large/006Fmjmcly1fhgxr47rqij30t00maqb0.jpg)![hankcs.com 2017-07-12 下午4.35.36.png](http://wx3.sinaimg.cn/large/006Fmjmcly1fhh6tkhn8dj30vx0lajyc.jpg)![2017-07-12_16-49-43.png](http://wx4.sinaimg.cn/large/006Fmjmcly1fhh787ktfvj31050jtam8.jpg)![hankcs.com 2017-07-12 下午4.55.26.png](http://wx3.sinaimg.cn/large/006Fmjmcly1fhh7e2xunbj30vj0mkahe.jpg)![hankcs.com 2017-07-12 下午5.09.55.png](http://wx2.sinaimg.cn/large/006Fmjmcly1fhh7sc70gej31b20tghdt.jpg)![2017-07-12_17-14-50.png](http://wx4.sinaimg.cn/large/006Fmjmcly1fhh7xkksazj30mt0j7qn1.jpg)![2017-07-12_17-17-53.png](http://wx4.sinaimg.cn/large/006Fmjmcly1fhh81pk0ldj30i60afdjd.jpg)最有意思的一课，将所有NLP任务视作QA问题。模仿人类粗读文章和问题，再带着问题反复阅读文章的行为，利用DMN这个通用框架漂亮地解决了从词性标注、情感分析到机器翻译、QA等一系列任务。

## 是否所有NLP任务都可视作QA？

问答系统举例如下：

![hankcs.com 2017-07-12 上午10.08.22.png](http://wx1.sinaimg.cn/large/006Fmjmcly1fhgvlvr4clj31kw0ksqb8.jpg)

在old-school NLP系统中，必须手工整理一个“知识库”；然后在这个知识库上做规则推断。这节课介绍的DMN完全不同于这种小作坊，它能够直接从问答语料中学习所有必要的知识表达。

DMN还可以在问答中做情感分析、词性标注和机器翻译。

所以构建一个joint model用于通用QA成为终极目标。

要实现这个目标，有两个障碍。

### 前无古人

没有任何已有研究探讨过如何让单个模型学会这么多的任务。每种任务都有独特的特点，适合不同的神经网络来做：

![hankcs.com 2017-07-12 上午10.25.05.png](http://wx2.sinaimg.cn/large/006Fmjmcly1fhgw30wcnsj31cy0m4jvf.jpg)

### 全才难得

第二个障碍是，Fully joint multitask learning（同一个decoder/classifier，不仅仅共享词向量，而应该共享全部参数）非常困难。

有些不成功的研究发现，只能在低层（词向量）共享参数、如果任务之间没有直接关联则会顾此失彼。

## Dynamic Memory Networks

今天介绍的DMN仅仅解决了第一个问题。虽然有些超参数还是得因任务而异，但总算是个通用的架构了。

### 回答难题

假设有个阅读理解题目：

![hankcs.com 2017-07-12 上午10.36.56.png](http://wx1.sinaimg.cn/large/006Fmjmcly1fhgwfbvlumj30jm0gkwl7.jpg)

你无法记住全文，但看了问题之后，只要带着问题扫几眼原文，你就能找出答案。

这种现象启发了DMN。

### Dynamic Memory Networks

先来看big picture（接下来会对每个模块单独讲解）：

![hankcs.com 2017-07-12 上午10.52.33.png](http://wx2.sinaimg.cn/large/006Fmjmcly1fhgwvmstwnj313d0fjn4i.jpg)

左边输入input的每个句子每个单词的词向量，送入input module的GRU中。同样对于Question Module，也是一个GRU，两个GRU可以共享权值。

Question Module计算出一个Question Vector $q$，根据$q$应用attention机制，回顾input的不同时刻。根据attention强度的不同，忽略了一些input，而注意到另一些input。这些input进入Episodic Memory Module，注意到问题是关于足球位置的，那么所有与足球及位置的input被送入该模块。该模块每个隐藏状态输入Answer module，softmax得到答案序列。

有人问DMN能否学会新类型的问题，答案是否定的。Episodic Memory Module中有两条线，分别代表带着问题$q$第一次阅读input的记忆，以及带着问题$q$第二次阅读的记忆。

### The Modules: Input

开始讲解每个模块的细节了。

![hankcs.com 2017-07-12 上午11.08.20.png](http://wx3.sinaimg.cn/large/006Fmjmcly1fhgxc3r5gqj312j0fp46d.jpg)

输入模块接受 $T_I$ 个输入单词，输出$T_C$个“事实”的表示。如果输出是一系列词语，那么有$T_C = T_I$；如果输出是一系列句子，那么约定$T_C$表示句子的数量，$T_I$表示句子中单词的数量。我们使用简单的GRU读入句子，得到隐藏状态$h_t = \mathrm{GRU}(x_t, h_{t-1})$，其中$x_t = L[w_t]$，$L$是embedding matrix，$w_t$ 是时刻 $t$ 的词语。

事实上，还可以将这个Uni-GRU升级为Bi-GRU：

![hankcs.com 2017-07-12 上午11.22.45.png](http://wx3.sinaimg.cn/large/006Fmjmcly1fhgxr47rqij30t00maqb0.jpg)

每个fact被表示为双向隐藏状态的拼接。

### The Modules: Question

![hankcs.com 2017-07-12 上午11.24.52.png](http://wx4.sinaimg.cn/large/006Fmjmcly1fhgxtm2223j312p0fwdn0.jpg)

同样用一个标准的GRU读入问题，同样的embedding matrix $L$，得到最后一个隐藏状态$q_t = \mathrm{GRU}(L[w_t^Q], q_{t-1})$。

### The Modules: Episodic Memory

![hankcs.com 2017-07-12 上午11.29.57.png](http://wx4.sinaimg.cn/large/006Fmjmcly1fhgxyjl938j312s0fudni.jpg)

DMN与其他网络最大的不同之处在于，它会多次阅读输入句子，每次只注意句子的fact表示中的一个子集。

输入模块的Bi-GRU产生fact表示，Episodic Memory模块也使用Bi-GRU产生Episodic Memory表示。记第$i$个pass产生的episodic memory representation为 $m^i$ ，episode representation（由attention mechanism产生）为 $e^i$。

其中，episodic memory representation 初始化为 $m^0 = q$，并通过 GRU处理: $m^i = \mathrm{GRU}(e^i, m^{i-1})$。而 episode representation 使用input module的隐藏状态输出更新：

$$\begin{align*}

h_t^i &= g_t^i \mathrm{GRU}(c_t, h^i_{t-1}) + (1 - g_t^i) h_{t-1}^i \\

e_i &= h_{T_C}^i

\end{align*}$$

其中 $g$ 是 attention mechanism，有多种计算方式。在最初的DMN paper (Kumar et al. 2016)中，验证了下列公式是最好的：

$$\begin{align*}

g_t^i &= G(c_t, m^{i-1}, q) \\

G(c, m, q) &= \sigma(W^{(2)} \tanh (W^{(1)} z(c,m,q) + b^{(1)}) + b^{(2)})\\

z(c,m,q) &= [c, m, q, c \circ q, c \circ m , |c-q|, |c-m|, c^TW^{(b)}q, c^TW^{(b)}m]

\end{align*}$$

这里最重要的是$z$的计算，通过句子向量、问题向量、上一个记忆向量之间原始形式与element-wise乘积（相似性）的按行拼接，得到一个超长的列向量。然后非线性激活传播一层，softmax得到$g$，表示每次阅读对每个时刻（句子）的关注程度。

Richard说最初他们还尝试过$q^TWs$，后来发现并没有什么帮助。他的忠告是，如果模型中有多余的部分，那就去掉。

这里两条线代表两个GRU，阅读两遍。这也是个超参数。Richard说最开始多个pass的Memory也通过GRU合并，后来发现只留两个pass直接视作两层神经网络也能解决问题，能简单就不要复杂。

### The Modules: Answer

answer module就是一个简单的GRU decoder，接受上次输出的单词（应该是one-hot向量），以及episodic memory，输出一个单词：

$$\begin{align*}

y_t &= \mathrm{softmax}(W^{(a)}a_t) \\

a_t &= \mathrm{GRU}([y_{t-1}, q], a_{t-1})

\end{align*}$$

### 相关工作

有很多已有工作做了类似研究：

• Sequence to Sequence (Sutskever et al. 2014)

• Neural Turing Machines (Graves et al. 2014)

• ***Teaching Machines to Read and Comprehend (Hermann et al. 2015)***

• Learning to Transduce with Unbounded Memory (Grefenstette 2015)

• Structured Memory for Neural Turing Machines (Wei Zhang 2015)

• Memory Networks (Weston et al. 2015)

• End to end memory networks (Sukhbaatar et al. 2015) 

Richard吐槽说他不觉得机器能像加粗的论文标题所说的，跟人一样阅读理解。也许这篇论文有点标题党了。与DMN联系密切的是后面两篇论文，都有Memory Component。

### 与MemNets比较

**相同点**
- 
都有input, scoring, attention and response模块

**不同点**
- 
MemNets主要使用词袋，然后有一些embedding去encode位置

- 
MemNets迭代运行attention和response

这些不同点都是由于MemNets是个非sequence模型造成的。而DMN是个血统纯正的neural sequence model，天然适合序列标注等任务，比MemNets应用范围更广。

DMN的sequence能力来自GRU，虽然一开始用的是LSTM，后来发现GRU也能达到相同的效果，而且参数更少。

