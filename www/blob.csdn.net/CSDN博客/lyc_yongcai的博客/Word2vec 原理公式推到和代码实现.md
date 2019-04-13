
# Word2vec 原理公式推到和代码实现 - lyc_yongcai的博客 - CSDN博客


2017年06月15日 10:17:22[刷街兜风](https://me.csdn.net/lyc_yongcai)阅读数：3536


本文摘录整编了一些理论介绍，推导了word2vec中的数学原理；并考察了一些常见的word2vec实现，评测其准确率等性能，最后分析了word2vec原版C代码；针对没有好用的Java实现的现状，移植了原版C程序到Java。时间和水平有限，本文没有就其发展历史展开多谈，只记录了必要的知识点，并着重关注工程实践。
![word2vec_diagram.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5tns7711ej20j90dkdjx.jpg)
虽然我的Java方案速度比原版C程序高出1倍，在算法代码与原版C程序一致的情况下准确率仍然略低于原版C程序（不过依然是目前准确率最高的Java实现），并非完美，还有待改进。
本文的理论部分大量参考《word2vec中的数学原理详解》，按照我这种初学者方便理解的顺序重新编排、重新叙述。
## 背景

### 语言模型
在统计自然语言处理中，语言模型指的是计算一个句子的概率模型。
传统的语言模型中词的表示是原始的、面向字符串的。两个语义相似的词的字符串可能完全不同，比如“番茄”和“西红柿”。这给所有NLP任务都带来了挑战——字符串本身无法储存语义信息。该挑战突出表现在模型的平滑问题上：标注语料是有限的，而语言整体是无限的，传统模型无法借力未标注的海量语料，只能靠人工设计平滑算法，而这些算法往往效果甚微。
神经概率语言模型（Neural Probabilistic Language Model）中词的表示是向量形式、面向语义的。两个语义相似的词对应的向量也是相似的，具体反映在夹角或距离上。甚至一些语义相似的二元词组中的词语对应的向量做线性减法之后得到的向量依然是相似的。词的向量表示可以显著提高传统NLP任务的性能，例如《基于神经网络的高性能依存句法分析器》中介绍的词、词性、依存关系的向量化对正确率的提升等。
从向量的角度来看，字符串形式的词语其实是更高维、更稀疏的向量。若词汇表大小为N，每个字符串形式的词语字典序为i，则其被表示为一个N维向量，该向量的第i维为1，其他维都为0。汉语的词汇量大约在十万这个量级，十万维的向量对计算来讲绝对是个维度灾难。而word2vec得到的词的向量形式（下文简称“词向量”，更学术化的翻译是“词嵌入”）则可以自由控制维度，一般是100左右。
### word2vec
word2vec作为神经概率语言模型的输入，其本身其实是神经概率模型的副产品，是为了通过神经网络学习**某个语言模型**而产生的中间结果。具体来说，“某个语言模型”指的是“CBOW”和“Skip-gram”。具体学习过程会用到两个降低复杂度的近似方法——Hierarchical Softmax或Negative Sampling。两个模型乘以两种方法，一共有四种实现。这些内容就是本文理论部分要详细阐明的全部了。
## Hierarchical Softmax
### 模型共同点
无论是哪种模型，其基本网络结构都是在下图的基础上，省略掉hidden layer：
![屏幕快照 2016-07-14 下午7.30.23.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5to0uwydsj216i0ikdi3.jpg)
为什么要去掉这一层呢？据说是因为word2vec的作者嫌从hidden layer到output layer的矩阵运算太多了。于是两种模型的网络结构是：
![屏幕快照 2016-07-14 下午7.27.35.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5to6e5d9lj216c0qkwhk.jpg)
其中w(t)代表当前词语位于句子的位置t，同理定义其他记号。在窗口内（上图为窗口大小为5），除了当前词语之外的其他词语共同构成上下文。
### CBOW
#### 原理
CBOW 是 Continuous Bag-of-Words Model 的缩写，是一种根据上下文的词语预测当前词语的出现概率的模型。其图示如上图左。
CBOW是已知上下文，估算当前词语的语言模型。其学习目标是最大化对数似然函数：
![屏幕快照 2016-07-14 下午7.45.11.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5tocs4lykj20fa03aaa8.jpg)
其中，w表示语料库C中任意一个词。从上图可以看出，对于CBOW，
**输入层**是上下文的词语的词向量（什么！我们不是在训练词向量吗？不不不，我们是在训练CBOW模型，词向量只是个副产品，确切来说，是CBOW模型的一个参数。训练开始的时候，词向量是个随机值，随着训练的进行不断被更新）。
**投影层**对其求和，所谓求和，就是简单的向量加法。
**输出层**输出最可能的w。由于语料库中词汇量是固定的|C|个，所以上述过程其实可以看做一个多分类问题。给定特征，从|C|个分类中挑一个。
对于神经网络模型多分类，最朴素的做法是softmax回归：
![神经网络依存句法分析29.png](http://ww2.sinaimg.cn/large/6cbb8645gw1exxdsuugv1j20cl033jrj.jpg)
softmax回归需要对语料库中每个词语（类）都计算一遍输出概率并进行归一化，在几十万词汇量的语料上无疑是令人头疼的。
不用softmax怎么样？比如SVM中的多分类，我们都知道其多分类是由二分类组合而来的：
![svm_多分类.gif](http://ww1.sinaimg.cn/large/6cbb8645gw1f5wmvf9tbrg20bf08mq30.gif)
这是一种二叉树结构，应用到word2vec中被作者称为Hierarchical Softmax：
![屏幕快照 2016-07-17 上午9.13.40.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5wmy4jdnwj214w12a42v.jpg)
上图输出层的树形结构即为Hierarchical Softmax。
非叶子节点相当于一个神经元（感知机，我认为逻辑斯谛回归就是感知机的输出代入f(x)=1/(1+e^x)），二分类决策输出1或0，分别代表向下左转或向下右转；每个叶子节点代表语料库中的一个词语，于是每个词语都可以被01唯一地编码，并且其编码序列对应一个事件序列，于是我们可以计算条件概率![屏幕快照 2016-07-17 上午10.05.33.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5wofvax07j208u0283yg.jpg)。
在开始计算之前，还是得引入一些符号：
![屏幕快照 2016-07-17 上午9.59.45.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5wo9rldvej201o01kt8h.jpg)从根结点出发到达w对应叶子结点的路径.
![屏幕快照 2016-07-17 上午10.00.06.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5woa6hrx9j201c01ojr5.jpg)路径中包含结点的个数
![屏幕快照 2016-07-17 上午10.01.17.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5wobigm5sj207m01wmx3.jpg)路径![屏幕快照 2016-07-17 上午9.59.45.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5wo9rldvej201o01kt8h.jpg)中的各个节点
![屏幕快照 2016-07-17 上午10.02.33.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5woco1brlj20c001ywei.jpg)词w的编码，![屏幕快照 2016-07-17 上午10.03.27.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5wodgv934j201q020742.jpg)表示路径![屏幕快照 2016-07-17 上午9.59.45.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5wo9rldvej201o01kt8h.jpg)第j个节点对应的编码（根节点无编码）
![屏幕快照 2016-07-17 上午10.04.18.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5woefv8p6j20bo01udfv.jpg)路径![屏幕快照 2016-07-17 上午9.59.45.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5wo9rldvej201o01kt8h.jpg)中非叶节点对应的参数向量
于是可以给出w的条件概率：

![屏幕快照 2016-07-17 上午10.07.18.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5wohk3pi7j20ka042t91.jpg)
这是个简单明了的式子，从根节点到叶节点经过了![屏幕快照 2016-07-17 上午10.00.06.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5woa6hrx9j201c01ojr5.jpg)-1个节点，编码从下标2开始（根节点无编码），对应的参数向量下标从1开始（根节点为1）。
其中，每一项是一个逻辑斯谛回归：
![屏幕快照 2016-07-17 上午10.15.37.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5woq5oeojj20pm05yt9a.jpg)
考虑到d只有0和1两种取值，我们可以用指数形式方便地将其写到一起：
![屏幕快照 2016-07-17 上午10.21.31.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wowc2oi2j20qm02mjrs.jpg)
我们的目标函数取对数似然：
![屏幕快照 2016-07-17 上午10.23.25.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5woyikyp0j20fc03kgls.jpg)
将![屏幕快照 2016-07-17 上午10.05.33.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5wofvax07j208u0283yg.jpg)代入上式，有
![屏幕快照 2016-07-17 上午10.25.37.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5wp0mws5gj20zi086gn9.jpg)
这也很直白，连乘的对数换成求和。不过还是有点长，我们把每一项简记为：
![屏幕快照 2016-07-17 上午10.27.15.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5wp2ahoblj20ug02adge.jpg)
怎么最大化对数似然函数呢？分别最大化每一项即可（这应该是一种近似，最大化某一项不一定使整体增大，具体收敛的证明还不清楚）。怎么最大化每一项呢？先求函数对每个变量的偏导数，对每一个样本，代入偏导数表达式得到函数在该维度的增长梯度，然后让对应参数加上这个梯度，函数在这个维度上就增长了。这种白话描述的算法在学术上叫随机梯度上升法。
每一项有两个参数，一个是每个节点的参数向量![屏幕快照 2016-07-17 上午10.58.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wpyg370hj202i022t8i.jpg)，另一个是输出层的输入![屏幕快照 2016-07-17 上午10.52.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wps67xq7j201m01kmwx.jpg)，我们分别对其求偏导数：
![屏幕快照 2016-07-17 上午10.52.59.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5wpt4xg8tj212603q756.jpg)
因为sigmoid函数的导数有个很棒的形式：
![屏幕快照 2016-07-17 上午10.54.30.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5wpus333qj20bi02gmx5.jpg)
于是代入上上式得到：
![屏幕快照 2016-07-17 上午10.56.15.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5wpwixaknj20ne02kt93.jpg)
合并同类项得到：
![屏幕快照 2016-07-17 上午10.57.17.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5wpxj61wjj20co02k0ss.jpg)
于是![屏幕快照 2016-07-17 上午10.58.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wpyg370hj202i022t8i.jpg)的更新表达式就得到了：
![屏幕快照 2016-07-17 上午10.59.08.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5wpzj0m0zj20l2026t90.jpg)
其中，![屏幕快照 2016-07-17 上午10.59.48.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wq02oflgj201e01k741.jpg)是机器学习的老相好——学习率，通常取0-1之间的一个值。学习率越大训练速度越快，但目标函数容易在局部区域来回抖动。
再来![屏幕快照 2016-07-17 上午10.52.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wps67xq7j201m01kmwx.jpg)的偏导数，注意到![屏幕快照 2016-07-17 上午10.27.15.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5wp2ahoblj20ug02adge.jpg)中![屏幕快照 2016-07-17 上午10.52.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wps67xq7j201m01kmwx.jpg)和![屏幕快照 2016-07-17 上午10.58.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wpyg370hj202i022t8i.jpg)是对称的，所有直接将![屏幕快照 2016-07-17 上午10.58.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wpyg370hj202i022t8i.jpg)的偏导数中的![屏幕快照 2016-07-17 上午10.58.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wpyg370hj202i022t8i.jpg)替换为![屏幕快照 2016-07-17 上午10.52.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wps67xq7j201m01kmwx.jpg)，得到关于![屏幕快照 2016-07-17 上午10.52.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wps67xq7j201m01kmwx.jpg)的偏导数：
![屏幕快照 2016-07-17 上午11.04.49.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5wq5cfqknj20je03qt92.jpg)
于是![屏幕快照 2016-07-17 上午10.52.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wps67xq7j201m01kmwx.jpg)的更新表达式也得到了。
不过![屏幕快照 2016-07-17 上午10.52.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wps67xq7j201m01kmwx.jpg)是上下文的词向量的和，不是上下文单个词的词向量。怎么把这个更新量应用到单个词的词向量上去呢？word2vec采取的是直接将![屏幕快照 2016-07-17 上午10.52.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wps67xq7j201m01kmwx.jpg)的更新量整个应用到每个单词的词向量上去：
![屏幕快照 2016-07-17 上午11.11.33.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5wqcddww7j20q6042wf4.jpg)
其中，![屏幕快照 2016-07-17 上午11.11.46.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wqcuc7mvj202q01yjr7.jpg)代表上下文中某一个单词的词向量。我认为应该也可以将其平均后更新到每个词向量上去，无非是学习率的不同，欢迎指正。
#### 代码分析
于是就可以得到两个参数更新的伪码：
![屏幕快照 2016-07-17 上午11.15.50.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5wqgz0elqj20pm0qaq5a.jpg)
在原版C代码中的对应关系是：
f=0;
// Propagate hidden -> output
for(c=0;c<layer1_size;c++)
f+=neu1[c]*syn1[c+l2];
f对应q，neu1对应![屏幕快照 2016-07-17 上午10.52.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wps67xq7j201m01kmwx.jpg)，syn1对应![屏幕快照 2016-07-17 上午10.58.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wpyg370hj202i022t8i.jpg)。
// 'g' is the gradient multiplied by the learning rate
g=(1-vocab[word].code[d]-f)*alpha;
对应伪码中的g。
// Propagate errors output -> hidden
for(c=0;c<layer1_size;c++)
neu1e[c]+=g*syn1[c+l2];
对应伪码中的e。
// Learn weights hidden -> output
for(c=0;c<layer1_size;c++)
syn1[c+l2]+=g*neu1[c];
对应伪码中的![屏幕快照 2016-07-17 上午10.58.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wpyg370hj202i022t8i.jpg)。
### Skip-gram
原理
Skip-gram只是逆转了CBOW的因果关系而已，即已知当前词语，预测上下文。
其网络结构如下图所示：
![屏幕快照 2016-07-17 上午11.33.31.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5wqzg68u0j214a120wij.jpg)
上图与CBOW的两个不同在于
输入层不再是多个词向量，而是一个词向量
投影层其实什么事情都没干，直接将输入层的词向量传递给输出层
在对其推导之前需要引入一个新的记号：
u：表示w的上下文中的一个词语。
于是语言模型的概率函数可以写作：
![屏幕快照 2016-07-17 上午11.42.19.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5wr8gixuxj20ji03kq37.jpg)
注意这是一个词袋模型，所以每个u是无序的，或者说，互相独立的。
在Hierarchical Softmax思想下，每个u都可以编码为一条01路径：
![屏幕快照 2016-07-17 上午11.45.43.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5wrc0q0qbj20f80483yp.jpg)
类似地，每一项都是如下简写：
![屏幕快照 2016-07-17 上午11.47.23.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5wrdn000lj20v602st97.jpg)
把它们写到一起，得到目标函数：
![屏幕快照 2016-07-17 下午1.38.22.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wula1s3qj217g08q40h.jpg)
类似CBOW的做法，将每一项简记为：
![屏幕快照 2016-07-17 下午1.40.34.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5wungph30j20ze024wf2.jpg)
虽然上式对比CBOW多了一个u，但给定训练实例（一个词w和它的上下文{u}），u也是固定的。所以上式其实依然只有两个变量![屏幕快照 2016-07-17 上午10.52.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wps67xq7j201m01kmwx.jpg)和![屏幕快照 2016-07-17 上午10.58.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wpyg370hj202i022t8i.jpg)，对其求偏导数：
![屏幕快照 2016-07-17 下午1.47.40.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5wuuqjclej206s03waa1.jpg)![屏幕快照 2016-07-17 下午1.47.16.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wuufd3hcj20h601w0su.jpg)
具体求导过程类似CBOW，略过。
于是得到![屏幕快照 2016-07-17 上午10.58.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wpyg370hj202i022t8i.jpg)的更新表达式：
![屏幕快照 2016-07-17 下午1.49.16.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5wuwiun2hj20nq02a3yr.jpg)
同理利用对称性得到对![屏幕快照 2016-07-17 上午10.52.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wps67xq7j201m01kmwx.jpg)的偏导数：
![屏幕快照 2016-07-17 下午1.50.19.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5wuxki2icj20me03cdg7.jpg)
于是得到![屏幕快照 2016-07-17 上午10.52.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wps67xq7j201m01kmwx.jpg)的更新表达式：
![屏幕快照 2016-07-17 下午1.51.20.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5wuym3dfsj20ms04mgm3.jpg)
训练伪码如下：
![屏幕快照 2016-07-17 下午1.53.06.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5wv0ifddej20ok0ncgni.jpg)
word2vec源码中并没有等![屏幕快照 2016-07-17 上午10.58.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wpyg370hj202i022t8i.jpg)更新完再更新![屏幕快照 2016-07-17 上午10.52.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wps67xq7j201m01kmwx.jpg)，而是即时地更新：
![屏幕快照 2016-07-17 下午1.55.05.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5wv2jzxnfj20oe0ncdhp.jpg)
具体对应源码中的
// Propagate hidden -> output
for(c=0;c<layer1_size;c++)
f+=syn0[c+l1]*syn1[c+l2];
// 'g' is the gradient multiplied by the learning rate
g=(1-vocab[word].code[d]-f)*alpha;
// Propagate errors output -> hidden
for(c=0;c<layer1_size;c++)
neu1e[c]+=g*syn1[c+l2];
// Learn weights hidden -> output
for(c=0;c<layer1_size;c++)
syn1[c+l2]+=g*syn0[c+l1];
f对应q，syn0对应v，syn1对应![屏幕快照 2016-07-17 上午10.58.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wpyg370hj202i022t8i.jpg)，neu1e对应e。
## Negative Sampling
通过上一章的学习，我们知道无论是CBOW还是Skip-gram模型，其实都是分类模型。对于机器学习中的分类任务，在训练的时候不但要给正例，还要给负例。对于Hierarchical Softmax，负例放在二叉树的根节点上。对于Negative Sampling，负例是随机挑选出来的。据说Negative Sampling能提高速度、改进模型质量。
### CBOW
给定训练样本，即一个词w和它的上下文Context(w)，Context(w)是输入，w是输出。那么w就是正例，词汇表中其他的词语的就是负例。假设我们通过某种采样方法获得了负例子集NEG(w)。对于正负样本，分别定义一个标签：
![屏幕快照 2016-07-17 下午2.18.20.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5wvqrrxrkj20du04uaa6.jpg)
也即正样本为1，负样本为0。
对于给定正样本![屏幕快照 2016-07-17 下午2.20.18.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5wvstbibtj208k022mx4.jpg)，我们希望最大化：
![屏幕快照 2016-07-17 下午2.21.21.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5wvu0zlm8j20ja03s74k.jpg)
其中，
![屏幕快照 2016-07-17 下午2.22.46.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wvvi5wd1j20tk03caak.jpg)
也就是说，当u是正例时，![屏幕快照 2016-07-17 下午2.25.48.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5wvyjh18ij204e022mx0.jpg)越大越好，当u是负例时，![屏幕快照 2016-07-17 下午2.25.48.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5wvz1yo3mj204e022mx0.jpg)越小越好。因为![屏幕快照 2016-07-17 下午2.25.48.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5wvyjh18ij204e022mx0.jpg)等于模型预测样本为正例的概率，当答案就是正的时候，我们希望这个概率越大越好，当答案是负的时候，我们希望它越小越好，这样才能说明该模型是个明辨是非的好同志。
每个词都是如此，语料库有多个词，我们将g累积得到优化目标。因为对数方便计算，我们对其取对数得到目标函数：
![屏幕快照 2016-07-17 下午2.30.13.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5ww343093j202w01qjr5.jpg)![屏幕快照 2016-07-17 下午2.30.29.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5ww51ivztj20f0036t8v.jpg)
![屏幕快照 2016-07-17 下午2.30.41.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5ww6ewrf6j214c06u75z.jpg)
记双重求和中的每一项为：
![屏幕快照 2016-07-17 下午2.35.00.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5ww86shjmj20ww02adgb.jpg)
求梯度：
![屏幕快照 2016-07-17 下午2.35.20.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5ww8moi79j206w03qmx3.jpg)![屏幕快照 2016-07-17 下午2.36.01.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5ww9fhthaj20bi02cjre.jpg)
于是![屏幕快照 2016-07-17 下午2.37.02.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5wwa421k2j201a01mdfl.jpg)的更新方法为：
![屏幕快照 2016-07-17 下午2.37.34.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5wwbbbx5ij20hy022jri.jpg)
利用对称性得到关于![屏幕快照 2016-07-17 上午10.52.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wps67xq7j201m01kmwx.jpg)的梯度：
![屏幕快照 2016-07-17 下午2.38.12.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5wwbh2e7gj20hg03kmxe.jpg)
将该更新应用到每个词向量上去：
![屏幕快照 2016-07-17 下午2.40.01.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wwdsvi40j20uk0463z4.jpg)
训练伪码为：
![屏幕快照 2016-07-17 下午2.40.41.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5wwegdnryj20oi0q00v3.jpg)
对应原版C代码的片段：
f=0;
for(c=0;c<layer1_size;c++)
f+=neu1[c]*syn1neg[c+l2];
if(f>MAX_EXP)
g=(label-1)*alpha;
elseif(f<-MAX_EXP)
g=(label-0)*alpha;
else
g=(label-expTable[(int)((f+MAX_EXP)*(EXP_TABLE_SIZE/MAX_EXP/2))])*alpha;
for(c=0;c<layer1_size;c++)
neu1e[c]+=g*syn1neg[c+l2];
for(c=0;c<layer1_size;c++)
syn1neg[c+l2]+=g*neu1[c];

### Skip-gram
有了前三次的经验，这次轻车熟路地给出结论吧。颠倒样本的x和y部分，也即对![屏幕快照 2016-07-17 下午2.48.48.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5wwmf09odj208e01udfs.jpg)，我们希望最大化：
![屏幕快照 2016-07-17 下午2.49.32.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5wwn80xmpj20kw03qaae.jpg)
其中，
![屏幕快照 2016-07-17 下午2.50.05.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wwnsb8gwj20rs02q74p.jpg)
最终目标函数为：

![屏幕快照 2016-07-17 下午2.52.14.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5wwpygiykj203c01ujr5.jpg)![屏幕快照 2016-07-17 下午2.52.27.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5wwq6wiqgj20gc03iq35.jpg)![屏幕快照 2016-07-17 下午2.52.39.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5wwqfdz9dj205q02aglh.jpg)
其中，
![屏幕快照 2016-07-17 下午2.53.36.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5wwrfbv20j212a028q3k.jpg)
分别求出梯度：
![屏幕快照 2016-07-17 下午2.54.58.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5wwssk44nj2070036dfs.jpg)![屏幕快照 2016-07-17 下午2.55.09.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5wwt3b6slj20gc026dfy.jpg)
![屏幕快照 2016-07-17 下午2.57.39.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5wwvo04hhj20kc03u0t4.jpg)
得到两者的更新方法：
![屏幕快照 2016-07-17 下午3.00.14.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5wwybgglbj201k01odfl.jpg)+=![屏幕快照 2016-07-17 下午2.58.34.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5wwx8u1fdj20fa02k0sv.jpg)
![屏幕快照 2016-07-17 下午3.01.01.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5wwzexewnj20hi0440sx.jpg)![屏幕快照 2016-07-17 下午3.01.34.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5wwzp1b9nj20d002oaa4.jpg)
训练伪码为：
![屏幕快照 2016-07-17 下午3.02.35.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5wx0r6vbsj20oo0ngabz.jpg)
对应原版C代码片段：
f=0;
for(c=0;c<layer1_size;c++)
f+=syn0[c+l1]*syn1neg[c+l2];
if(f>MAX_EXP)
g=(label-1)*alpha;
elseif(f<-MAX_EXP)
g=(label-0)*alpha;
else
g=(label-expTable[(int)((f+MAX_EXP)*(EXP_TABLE_SIZE/MAX_EXP/2))])*alpha;
for(c=0;c<layer1_size;c++)
neu1e[c]+=g*syn1neg[c+l2];
for(c=0;c<layer1_size;c++)
syn1neg[c+l2]+=g*syn0[c+l1];
syn0对应![屏幕快照 2016-07-17 上午10.52.10.png](http://ww2.sinaimg.cn/large/6cbb8645gw1f5wps67xq7j201m01kmwx.jpg)，syn1neg对应![屏幕快照 2016-07-17 下午2.37.02.png](http://ww1.sinaimg.cn/large/6cbb8645gw1f5wwa421k2j201a01mdfl.jpg)，f运算后得到q，代码中有优化（后文分解），neu1e对应e。
## 更多细节
### Huffman树
上文一直在用二叉树描述Hierarchical Softmax，这是因为我不想仿照大部分tutorial那样一下子拿出Huffman这么具体的细节。初期对word2vec的大框架还没把握住的时候突然看到这些细节的话，人会抓不住重点，造成学习成本无谓的上升。我当时看到有些tutorial第一节就在讲Huffman编码，还以为实现word2vec一定要用Huffman树呢。
其实根本不是的，任何二叉树都可以。Huffman树只是二叉树中具体的一种，特别适合word2vec的训练。
word2vec训练的时候按照词频将每个词语Huffman编码，由于Huffman编码中词频越高的词语对应的编码越短。所以越高频的词语在Hierarchical Softmax过程中经过的二分类节点就越少，整体计算量就更少了。
### 负采样算法
任何采样算法都应该保证频次越高的样本越容易被采样出来。基本的思路是对于长度为1的线段，根据词语的词频将其公平地分配给每个词语：
![屏幕快照 2016-07-17 下午3.16.37.png](http://ww4.sinaimg.cn/large/6cbb8645gw1f5wxfljwsnj20cs04ajrn.jpg)
counter就是w的词频。
于是我们将该线段公平地分配了：
![屏幕快照 2016-07-17 下午3.18.09.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5wxhgjcnkj20xo03mwep.jpg)
接下来我们只要生成一个0-1之间的随机数，看看落到哪个区间，就能采样到该区间对应的单词了，很公平。
但怎么根据小数找区间呢？速度慢可不行。
word2vec用的是一种查表的方式，将上述线段标上M个“刻度”，刻度之间的间隔是相等的，即1/M：
![屏幕快照 2016-07-17 下午3.22.12.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5wxly3lgkj20yo0c0aba.jpg)
接着我们就不生成0-1之间的随机数了，我们生成0-M之间的整数，去这个刻度尺上一查就能抽中一个单词了。
在word2vec中，该“刻度尺”对应着table数组。具体实现时，对词频取了0.75次幂：
![屏幕快照 2016-07-17 下午4.02.32.png](http://ww3.sinaimg.cn/large/6cbb8645gw1f5wyr5o2p6j20ew04o74n.jpg)
这个幂实际上是一种“平滑”策略，能够让低频词多一些出场机会，高频词贡献一些出场机会，劫富济贫。
### sigmoid函数
类似的查表方法还有sigmoid函数的计算，因为该函数使用太频繁，而其值仅仅在靠近0的时候才会剧烈变化，远离0的方向很快趋近0和1。所以源码中也采用了“刻度查表”的方法，先算出了很多个刻度对应的函数值，运算中直接查表。这部分对应:
expTable=(real*)malloc((EXP_TABLE_SIZE+1)*sizeof(real));
for(i=0;i<EXP_TABLE_SIZE;i++)
{
expTable[i]=exp((i/(real)EXP_TABLE_SIZE*2-1)*MAX_EXP);// Precompute the exp() table
expTable[i]=expTable[i]/(expTable[i]+1);// Precompute f(x) = x / (x + 1)
}


