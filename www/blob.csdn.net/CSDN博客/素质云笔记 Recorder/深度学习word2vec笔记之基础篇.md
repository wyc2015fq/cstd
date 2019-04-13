
# 深度学习word2vec笔记之基础篇 - 素质云笔记-Recorder... - CSDN博客

2016年05月04日 22:40:24[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：3179


作者为[falao_beiliu](http://my.csdn.net/mytestmy)。

作者：杨超
链接：http://www.zhihu.com/question/21661274/answer/19331979
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
最近几位google的研究人员发布了一个工具包叫word2vec，利用神经网络为单词寻找一个连续向量空间中的表示。这里整理一下思路，供有兴趣的同学参考。
这里先回顾一下大家比较熟悉的N-gram语言模型。
在自然语言任务里我们经常要计算一句话的概率。比如语音识别中对于一个语音段O，需要找到一个使P(W|O)最大的文本段W。利用贝叶斯公式：
P(W|O) = P(O|W)*P(W)/P(O)
其中 P(O|W)是用hmm-gmm或者hmm-dnn建模的声学模型，而P(W)称为语言模型。
P(W)可以分解如下：
其中：
为了简化问题，可以做一个n-1阶Markov假设，即每个单词只和其前n-1个单词相关，则：
这样，只要算出这些条件概率，任意的句子的概率我们都可以计算，而这些条件概率可以通过对语料建立一个多项分布模型用最大似然法求得。
这种方法我们称为N-gram语言模型。
但是N-gram存在一个问题，若训练语料里面有些n元组没有出现过，其对应的条件概率就是0，这会导致计算一整句话的概率为0。
解决这个问题有两种常用方法：
一种是平滑法，最简单的是将出现k次的某个n元组看做出现了k+1次，这样出现0次的n元组就变成了出现1次。或者用更复杂的其他平滑方法。其本质都是将模型编程贝叶斯模型，为参数引入了先验分布，从而解决最大似然法的问题。
另一种是回退法，即如果n元的概率不到，那就用n-1元的概率乘上一个权重来模拟。
**一．神经网络语言模型**
在Bengio03的文章中，他们提到除了上面说的问题之外，n-gram还存在其他问题：
1.n-gram语言模型无法建模更远的关系，语料的不足使得无法训练更高阶的语言模型。（这篇文章发表时，基本都是trigram，还没有高阶的模型，不过几年后，互联网的海量数据使得可以训练10几阶的语言模型）
2.这种模型无法建模出词之间的相似度，有时候两个具有某种相似性的词，如果一个词经常出现在某段词之后，那么也许另一个词出现在这段词后面的概率也比较大,比如
The cat is walking in the bedroom
A dog was running in a room
如果第一句话里的元组在语料中出现的很多，训练的很充分，第二句话中的元组在语料中出现的少，训练的不充分，那么使用语言模型计算第一句话的概率就比较高，而第二句话的概率就低。
如果有一种方法，能知道The和a相似，cat和dog相似等等，并且会给相似的词类似的语言模型概率，那么第二句话也可以得到高概率。
**另一种模型**
既然我们想要的就是P(wt|Wp)，那么可以用一个神经网络直接去建模这个概率，网络的输入是前N-1单词，输出是V个节点，其中第i个节点的输出值就是P(wi|Wp).本文用Wp表示wt前面的N-1的个词.
具体的模型如下：
1.每个单词i表示成一个向量Ci
2.把单词的向量表示Ci排列成一个更大的向量作为神经网络输入，输出是第n个单词。
3.训练出C和神经网络的参数
&amp;lt;img src=&quot;https://pic3.zhimg.com/61f31ca272dcd40ad0a3d05efc3172a2_b.jpg&quot; data-rawwidth=&quot;871&quot; data-rawheight=&quot;690&quot; class=&quot;origin_image zh-lightbox-thumb&quot; width=&quot;871&quot; data-original=&quot;https://pic3.zhimg.com/61f31ca272dcd40ad0a3d05efc3172a2_r.jpg&quot;&amp;gt;![](https://pic3.zhimg.com/61f31ca272dcd40ad0a3d05efc3172a2_b.jpg)
如果不考虑虚线（输入跳过隐层直接到输出的连接），那么红框内是个标准的三层前向神经网络，输入层是单词的向量表示，隐层使用tanh激活函数，输出层用softmax做概率归一化。
这个模型也可以看做是4层的网络，输入层每个节点到层C之间的权重就是这个节点对应的单词i的向量表示Ci。
对于每个样本，输入是Wp，输出是一个1 of V的向量，即V个输出节点，只有wt对应的那个节点为1，其他都为0.
使用BP算法训练这个网络即可得到所有的参数值。
虽然这篇文章的方法是为了计算语言模型，但也同时获得一种单词在向量空间上的表示。而这个副作用才是google的word2vec的真正目标。
**二.层次soft-max**
虽然基于神经网络的语言模型在效果上表现的很不错，但是其训练和预测的时间较长，影响实际的应用。
这个模型的输出节点个数是V，在训练时，对于一个样本(Wp,wt)，我们的目标是最大化P(wt|Wp)，模型的输出是V个没有归一化的值，所以需要对其做soft-max，得到P(wt|Wp).
这就需要前向时计算所有节点的输出，并且要更新隐层到所有输出节点的权重。隐层个数是H，则这一计算量是正比于H*V的。
可不可以每次只对到标注是1的节点的权重进行更新，而不管那些标注是0的节点呢，这样每次训练时计算量不就是正比于H了吗？这是可以的，只是训练会起来很慢。
为什么呢，用一种直观但不严谨的说法来解释：
“只对到输出是1的节点的权重进行更新，而不管那些输出是0的节点”这种做法，是让模型在每次迭代时只增加我看到的东西的概率，并没有降低我没看到东西的概率（这里说的概率是非归一化概率，即没做soft-max之前输出节点的输出值，按照归一化概率而言，只要增加了看到东西的概率，就必然会降低没看到的东西的概率）。而如果让模型在每次迭代都能在增加我看到的东西的概率的同时，降低我没看到东西的概率，那么模型会更快的收敛，所以训练时隐层到输出是0的节点间的权重也要做更新。
这里提前说一下，在word2vec里面，除了h soft-max，还有另一种方法叫negative sampling，就是这种思路，他每次并不对所有的标注是0的节点的权重进行更新，而随机选取一些标注是0的节点更新权重，随机选取的量是n，这样每次就把H*V降到H*n。
现在回到上面的问题，我希望增大的不是标注为1的节点的输出概率，而是该输出在和其他节点一同进行归一化后的概率，是否有什么可以减少计算量的方法呢？
我们可以考虑这么一个问题，如果我们有100个数组成的分布，但我们只有他们的非归一化概率，这样想计算出某个数的归一概率，这样我们需要计算100次（把他们加起来）。而如果我们把他们分为10类，每类10个数。如果我们知道这10个类的非归一化的概率，以及每类的10个数的非归一化概率，那么我们计算某个数的归一化概率就只要10+10次，这就提高了5倍的速度。
更极致的方法是分更多的层，每次用2分法，这样可以把效率提高的极致。这可以等价于对输出节点进行某种2进制的编码，第i位表示这个词在第i个层次上面的分类。
这样模型的输出层就如下图所示：
&amp;lt;img src=&quot;https://pic3.zhimg.com/c8bb89a9b12cfb0969cf496555b67122_b.jpg&quot; data-rawwidth=&quot;937&quot; data-rawheight=&quot;625&quot; class=&quot;origin_image zh-lightbox-thumb&quot; width=&quot;937&quot; data-original=&quot;https://pic3.zhimg.com/c8bb89a9b12cfb0969cf496555b67122_r.jpg&quot;&amp;gt;![](https://pic3.zhimg.com/c8bb89a9b12cfb0969cf496555b67122_b.jpg)
同学可能会有疑问：这样做和每次只对输出是1的节点更新有什么区别呢？后者只需要H的复杂度，这样还需要H*Log2(V)呢？
这是有区别的，但这里需要每一层分类的法则具有一定的意义，不能是随机划分。
那么在训练时，每次训练，设样本里的输出单词为wt，你增加了p(wt|Wp)的概率，本质上是增加了他在每个层次上的他属于的类别的概率，由于在每个层次上每个类别节点都是2分的，那么增加wt属于的类别概率，就是降低wt不属于的另一类别的概率。这样那些V中和wt同类的节点概率被增加，不同类的节点概率被降低，如果类别是用真是意义的，那么每次训练，那些不同类的单词就相当于负例了，而做n=0的negative sampling是没有这个效果的，他只会增加wt的输出概率。
另外在进行预测时，这种分层soft-max方法也可以降低时间复杂度，但前提是只预测一个P(wt|Wp)或者一部分（比如语音识别只要计算候选集里的即可），但如果想要预测此表中所有的V，那么分层soft-max的复杂度和直接soft-max是一样的.
**三．Google的word2vec**
&amp;lt;img src=&quot;https://pic3.zhimg.com/ac3d96ff826948aa5542ac2a76fd020e_b.jpg&quot; data-rawwidth=&quot;811&quot; data-rawheight=&quot;477&quot; class=&quot;origin_image zh-lightbox-thumb&quot; width=&quot;811&quot; data-original=&quot;https://pic3.zhimg.com/ac3d96ff826948aa5542ac2a76fd020e_r.jpg&quot;&amp;gt;![](https://pic3.zhimg.com/ac3d96ff826948aa5542ac2a76fd020e_b.jpg)
有了上面的知识，word2Vec就很好理解，只是对上面的框架上做了些小改动。
其中的改动有：
1.映射层不再是将输入单词的向量表示按顺序排列，而是将他们相加。减少计算量。
2.去掉了隐层，减少计算量，同时效果并不差。
3.由于这里的目的是寻找单词的向量表示，而不是语言模型，所以可以利用一些其他的判别准则，比如，两个模型分别对应上图的CBOW和Skip-gram.这样真的是利用了上下文而不是上文了。
4.对于分层softmax，并没有wordnet进行编码，而是根据词频用huffman编码。我们提到过，使用分层softmax，要求分类是有一定意义的，如果说用人类的先验知识如wordnet或者某些无监督学习的方法去做分层，倒还有道理，但是huffman编码只用到了词频特征去分层，这样的分层为什么有效呢？作者说：
&amp;lt;img src=&quot;https://pic4.zhimg.com/c3dc9fdfcd7fa47e2624966363e24dbb_b.jpg&quot; data-rawwidth=&quot;848&quot; data-rawheight=&quot;69&quot; class=&quot;origin_image zh-lightbox-thumb&quot; width=&quot;848&quot; data-original=&quot;https://pic4.zhimg.com/c3dc9fdfcd7fa47e2624966363e24dbb_r.jpg&quot;&amp;gt;![](https://pic4.zhimg.com/c3dc9fdfcd7fa47e2624966363e24dbb_b.jpg)Ok，实践是检验真理的唯一标准。
5.除了分层softmax，作者还使用了negative sampling的方法。

——————————————————————————————————————————————————————————————————————

# 深度学习word2vec笔记之基础篇

声明：
1）该博文是多位博主以及多位文档资料的主人所无私奉献的论文资料整理的。具体引用的资料请看参考文献。具体的版本声明也参考原文献
2）本文仅供学术交流，非商用。所以每一部分具体的参考资料并没有详细对应，更有些部分本来就是直接从其他博客复制过来的。如果某部分不小心侵犯了大家的利益，还望海涵，并联系老衲删除或修改，直到相关人士满意为止。
3）本人才疏学浅，整理总结的时候难免出错，还望各位前辈不吝指正，谢谢。
4）阅读本文需要[机器学习](http://lib.csdn.net/base/2)、语言模型等等基础（如果没有也没关系了，没有就看看，当做跟同学们吹牛的本钱）。
5）本人对语言模型也是新人，中间有些结论未必是正确的，各位如有发现，请指出，本人会尽量改正，避免误导读者。
6）此属于第一版本，若有错误，还需继续修正与增删。还望大家多多指点。请直接回帖，本人来想办法处理。
7）本人手上有word版的和pdf版的，已上传到csdn，下载url：http://download.csdn.net/detail/mytestmy/8565955，或者http://download.csdn.net/detail/mytestmy/8565959，资源分1分，评论后据说可以返还的，就有劳各位帮忙攒点分吧。如果有必要可以回复或者发邮件到邮箱beiliude@163.com，将按各位需求回复。

## 一．前言
伴随着深度学习的大红大紫，只要是在自己的成果里打上deep learning字样，总会有人去看。深度学习可以称为当今机器学习领域的当之无愧的巨星，也特别得到工业界的青睐。
在各种大举深度学习大旗的公司中，Google公司无疑是旗举得最高的，口号喊得最响亮的那一个。2013年末，Google发布的word2vec工具引起了一帮人的热捧，大家几乎都认为它是深度学习在自然语言领域的一项了不起的应用，各种欢呼“深度学习在自然语言领域开始发力了”。
互联网界很多公司也开始跟进，使用word2vec产出了不少成果。身为一个互联网民工，有必要对这种炙手可热的技术进行一定程度的理解。
好在word2vec也算是比较简单的，只是一个简单三层神经网络。在浏览了多位大牛的博客，随笔和笔记后，整理成自己的博文，或者说抄出来自己的博文。

## 二．背景知识
### 2.1词向量
自然语言处理（NLP）相关任务中，要将自然语言交给机器学习中的算法来处理，通常需要首先将语言数学化，因为机器不是人，机器只认数学符号。向量是人把自然界的东西抽象出来交给机器处理的东西，基本上可以说向量是人对机器输入的主要方式了。
词向量就是用来将语言中的词进行数学化的一种方式，顾名思义，词向量就是把一个词表示成一个向量。
主要有两种表示方式，下面分别介绍，主要参考了@皮果提在知乎上的问答，也就是参考文献【2】。
#### 2.1.1 One-Hot Representation
一种最简单的词向量方式是 one-hotrepresentation，就是用一个很长的向量来表示一个词，向量的长度为词典的大小，向量的分量只有一个 1，其他全为 0， 1 的位置对应该词在词典中的位置。举个例子，
“话筒”表示为 [0 0 0 1 00 0 0 0 0 0 0 0 0 0 0 ...]
“麦克”表示为 [0 0 0 0 00 0 0 1 0 0 0 0 0 0 0 ...]
每个词都是茫茫 0 海中的一个 1。
这种 One-hotRepresentation 如果采用稀疏方式存储，会是非常的简洁：也就是给每个词分配一个数字 ID。比如刚才的例子中，话筒记为 3，麦克记为 8（假设从 0 开始记）。如果要编程实现的话，用 Hash 表给每个词分配一个编号就可以了。这么简洁的表示方法配合上最大熵、SVM、CRF 等等算法已经很好地完成了 NLP 领域的各种主流任务。
但这种词表示有两个缺点：（1）容易受维数灾难的困扰，尤其是将其用于 Deep Learning 的一些算法时；（2）不能很好地刻画词与词之间的相似性（术语好像叫做“词汇鸿沟”）：任意两个词之间都是孤立的。光从这两个向量中看不出两个词是否有关系，哪怕是话筒和麦克这样的同义词也不能幸免于难。
所以会寻求发展，用另外的方式表示，就是下面这种。
#### 2.1.2 Distributed Representation
另一种就是DistributedRepresentation 这种表示，它最早是 Hinton 于 1986 年提出的，可以克服 one-hot representation 的缺点。其基本想法是直接用一个普通的向量表示一个词，这种向量一般长成这个样子：[0.792, −0.177, −0.107, 0.109, −0.542, ...]，也就是普通的向量表示形式。维度以 50 维和 100 维比较常见。
当然一个词怎么表示成这么样的一个向量是要经过一番训练的，训练方法较多，word2vec是其中一种，在后面会提到，这里先说它的意义。还要注意的是每个词在不同的语料库和不同的训练方法下，得到的词向量可能是不一样的。
词向量一般维数不高，很少有人闲着没事训练的时候定义一个10000维以上的维数，所以用起来维数灾难的机会现对于one-hot representation表示就大大减少了。
由于是用向量表示，而且用较好的训练算法得到的词向量的向量一般是有空间上的意义的，也就是说，将所有这些向量放在一起形成一个词向量空间，而每一向量则为该空间中的一个点，在这个空间上的词向量之间的距离度量也可以表示对应的两个词之间的“距离”。所谓两个词之间的“距离”，就是这两个词之间的语法，语义之间的相似性。
一个比较爽的应用方法是，得到词向量后，假如对于某个词A，想找出这个词最相似的词，这个场景对人来说都不轻松，毕竟比较主观，但是对于建立好词向量后的情况，对计算机来说，只要拿这个词的词向量跟其他词的词向量一一计算欧式距离或者cos距离，得到距离最小的那个词，就是它最相似的。
这样的特性使得词向量很有意义，自然就会吸引比较多的人去研究，前有Bengio发表在JMLR上的论文《A Neural Probabilistic Language Model》，又有Hinton的层次化Log-Bilinear模型，还有google的TomasMikolov 团队搞的word2vec，等等。
词向量在机器翻译领域的一个应用，就是google的TomasMikolov 团队开发了一种词典和术语表的自动生成技术，该技术通过向量空间，把一种语言转变成另一种语言，实验中对英语和西班牙语间的翻译准确率高达90%。
介绍算法工作原理的时候举了一个例子：考虑英语和西班牙语两种语言，通过训练分别得到它们对应的词向量空间 E 和 S。从英语中取出五个词 one，two，three，four，five，设其在 E 中对应的词向量分别为 v1，v2，v3，v4，v5，为方便作图，利用主成分分析（PCA）降维，得到相应的二维向量 u1，u2，u3，u4，u5，在二维平面上将这五个点描出来，如下图左图所示。类似地，在西班牙语中取出（与 one，two，three，four，five 对应的） uno，dos，tres，cuatro，cinco，设其在 S 中对应的词向量分别为 s1，s2，s3，s4，s5，用 PCA 降维后的二维向量分别为 t1，t2，t3，t4，t5，将它们在二维平面上描出来（可能还需作适当的旋转），如下图右图所示：
![](https://img-blog.csdn.net/20140525152517359)

观察左、右两幅图，容易发现：五个词在两个向量空间中的相对位置差不多，这说明两种不同语言对应向量空间的结构之间具有相似性，从而进一步说明了在词向量空间中利用距离刻画词之间相似性的合理性。


### 2.2语言模型
#### 2.2.1基本概念
语言模型其实就是看一句话是不是正常人说出来的。这玩意很有用，比如机器翻译、语音识别得到若干候选之后，可以利用语言模型挑一个尽量靠谱的结果。在 NLP 的其它任务里也都能用到。
语言模型形式化的描述就是给定一个T个词的字符串s，看它是自然语言的概率P(w1,w2,…,wt)。w1 到 wT 依次表示这句话中的各个词。有个很简单的推论是：
![{\rm{p}}\left( s \right) = {\rm{p}}\left( {{w_1},{w_2}, \cdots {w_T}} \right)](http://chart.apis.google.com/chart?cht=tx&chs=1x0&chf=bg,s,FFFFFF00&chco=000000&chl=%7B%5Crm%7Bp%7D%7D%5Cleft%28%20s%20%5Cright%29%20%3D%20%7B%5Crm%7Bp%7D%7D%5Cleft%28%20%7B%7Bw_1%7D%2C%7Bw_2%7D%2C%20%5Ccdots%20%7Bw_T%7D%7D%20%5Cright%29)![= {\rm{p}}\left( {{w_1}} \right){\rm{p(}}{w_2}{\rm{|}}{w_1}){\rm{p}}({w_3}|{w_1},{w_2}) \cdots {\rm{p}}({w_t}|{w_1},{w_2}, \cdots {w_{T - 1}})](http://chart.apis.google.com/chart?cht=tx&chs=1x0&chf=bg,s,FFFFFF00&chco=000000&chl=%20%3D%20%7B%5Crm%7Bp%7D%7D%5Cleft%28%20%7B%7Bw_1%7D%7D%20%5Cright%29%7B%5Crm%7Bp%28%7D%7D%7Bw_2%7D%7B%5Crm%7B%7C%7D%7D%7Bw_1%7D%29%7B%5Crm%7Bp%7D%7D%28%7Bw_3%7D%7C%7Bw_1%7D%2C%7Bw_2%7D%29%20%5Ccdots%20%7B%5Crm%7Bp%7D%7D%28%7Bw_t%7D%7C%7Bw_1%7D%2C%7Bw_2%7D%2C%20%5Ccdots%20%7Bw_%7BT%20-%201%7D%7D%29)(1)
上面那个概率表示的意义是：第一个词确定后，看后面的词在前面的词出现的情况下出现的概率。如一句话“大家喜欢吃苹果”，总共四个词“大家”，“喜欢”，“吃”，“苹果”，怎么分词现在不讨论，总之词已经分好，就这四个。那么这句话是一个自然语言的概率是：
P(大家，喜欢，吃，苹果)=p(大家)p(喜欢|大家)p(吃|大家,喜欢)p(苹果|大家,喜欢,吃)
p(大家)表示“大家”这个词在语料库里面出现的概率；
p(喜欢|大家)表示“喜欢”这个词出现在“大家”后面的概率;
p(吃|大家，喜欢)表示“吃”这个词出现在“大家喜欢”后面的概率；
p(苹果|大家,喜欢,吃)表示“苹果”这个词出现在“大家喜欢吃”后面的概率。
把这些概率连乘起来，得到的就是这句话平时出现的概率。
如果这个概率特别低，说明这句话不常出现，那么就不算是一句自然语言，因为在语料库里面很少出现。如果出现的概率高，就说明是一句自然语言。
看到了上面的计算，看有多麻烦：只有四个词的一句话，需要计算的是p(大家)，p(喜欢|大家)，p(吃|大家,喜欢)，p(苹果|大家,喜欢,吃)这四个概率，这四个概率还要预先计算好，考虑词的数量，成千上万个，再考虑组合数，p(吃|大家,喜欢)这个有“大家”、“喜欢”和“吃”的组合，总共会上亿种情况吧；再考虑p(苹果|大家,喜欢,吃)这个概率，总共也会超过万亿种。
从上面的情况看来，计算起来是非常麻烦的，一般都用偷懒的方式。
为了表示简单，上面的公式（1）用下面的方式表示
![{\rm{p}}\left( {\rm{s}} \right) = {\rm{p}}\left( {{w_1},{w_2}, \cdots {w_T}} \right) =  \prod \limits_{i = 1}^T p({w_i}|Contex{t_i})](http://chart.apis.google.com/chart?cht=tx&chs=1x0&chf=bg,s,FFFFFF00&chco=000000&chl=%7B%5Crm%7Bp%7D%7D%5Cleft%28%20%7B%5Crm%7Bs%7D%7D%20%5Cright%29%20%3D%20%7B%5Crm%7Bp%7D%7D%5Cleft%28%20%7B%7Bw_1%7D%2C%7Bw_2%7D%2C%20%5Ccdots%20%7Bw_T%7D%7D%20%5Cright%29%20%3D%20%20%5Cprod%20%5Climits_%7Bi%20%3D%201%7D%5ET%20p%28%7Bw_i%7D%7CContex%7Bt_i%7D%29)
其中，如果Contexti是空的话，就是它自己p(w)，另外如“吃”的Context就是“大家”、“喜欢”，其余的对号入座。
符号搞清楚了，就看怎么偷懒了。

#### 2.2.2 N-gram模型
接下来说怎么计算![p({w_i}|Contex{t_i})](http://chart.apis.google.com/chart?cht=tx&chs=1x0&chf=bg,s,FFFFFF00&chco=000000&chl=p%28%7Bw_i%7D%7CContex%7Bt_i%7D%29)，上面看的是跟据这句话前面的所有词来计算，那么就得计算很多了，比如就得把语料库里面p(苹果|大家,喜欢,吃)这种情况全部统计一遍，那么为了计算这句话的概率，就上面那个例子，都得扫描四次语料库。这样一句话有多少个词就得扫描多少趟，语料库一般都比较大，越大的语料库越能提供准确的判断。这样的计算速度在真正使用的时候是万万不可接受的，线上扫描一篇文章是不是一推乱七八糟的没有序列的文字都得扫描很久，这样的应用根本没人考虑。
最好的办法就是直接把所有的![p({w_i}|Contex{t_i})](http://chart.apis.google.com/chart?cht=tx&chs=1x0&chf=bg,s,FFFFFF00&chco=000000&chl=p%28%7Bw_i%7D%7CContex%7Bt_i%7D%29)提前算好了，那么根据排列组上面的来算，对于一个只有四个词的语料库，总共就有4!+3!+2!+1!个情况要计算，那就是24个情况要计算；换成1000个词的语料库，就是![\sum \limits_{i = 1}^{1000} i!](http://chart.apis.google.com/chart?cht=tx&chs=1x0&chf=bg,s,FFFFFF00&chco=000000&chl=%20%5Csum%20%5Climits_%7Bi%20%3D%201%7D%5E%7B1000%7D%20i%21)个情况需要统计，对于计算机来说，计算这些东西简直是开玩笑。
这就诞生了很多偷懒的方法，N-gram模型是其中之一了。N-gram什么情况呢？上面的context都是这句话中这个词前面的所有词作为条件的概率，N-gram就是只管这个词前面的n-1个词，加上它自己，总共n个词，计算![p({w_i}|Contex{t_i})](http://chart.apis.google.com/chart?cht=tx&chs=1x0&chf=bg,s,FFFFFF00&chco=000000&chl=p%28%7Bw_i%7D%7CContex%7Bt_i%7D%29)只考虑用这n个词来算，换成数学的公式来表示，就是
![{\rm{p}}\left( {{w_i}{\rm{|}}Contex{t_i}} \right) = {\rm{p}}({w_i}|{w_{i - n + 1}},{w_{i - n + 2}}, \cdots ,{w_{i - 1}})](http://chart.apis.google.com/chart?cht=tx&chs=1x0&chf=bg,s,FFFFFF00&chco=000000&chl=%7B%5Crm%7Bp%7D%7D%5Cleft%28%20%7B%7Bw_i%7D%7B%5Crm%7B%7C%7D%7DContex%7Bt_i%7D%7D%20%5Cright%29%20%3D%20%7B%5Crm%7Bp%7D%7D%28%7Bw_i%7D%7C%7Bw_%7Bi%20-%20n%20%2B%201%7D%7D%2C%7Bw_%7Bi%20-%20n%20%2B%202%7D%7D%2C%20%5Ccdots%20%2C%7Bw_%7Bi%20-%201%7D%7D%29)
这里如果n取得比较小的话，就比较省事了，当然也要看到n取得太小，会特别影响效果的，有可能计算出来的那个概率很不准。怎么平衡这个效果和计算就是大牛们的事情了，据大牛们的核算，n取2效果都还凑合，n取3就相当不错了，n取4就顶不住了。看下面的一些数据，假设词表中词的个数 |V| = 20,000 词，那么有下面的一些数据。
![](https://img-blog.csdn.net/20140525154453750)
照图中的数据看去，取n=3是目前计算能力的上限了。在实践中用的最多的就是bigram和trigram了，而且效果也基本够了。
N-gram模型也会有写问题，总结如下：
1、n不能取太大，取大了语料库经常不足，所以基本是用降级的方法
2、无法建模出词之间的相似度，就是有两个词经常出现在同一个context后面，但是模型是没法体现这个相似性的。
3、有些n元组（n个词的组合，跟顺序有关的）在语料库里面没有出现过，对应出来的条件概率就是0，这样一整句话的概率都是0了，这是不对的，解决的方法主要是两种：平滑法（基本上是分子分母都加一个数）和回退法（利用n-1的元组的概率去代替n元组的概率）

#### 2.2.3N-pos模型
当然学术是无止境的，有些大牛觉得这还不行，因为第i个词很多情况下是条件依赖于它前面的词的语法功能的，所以又弄出来一个n-pos模型，n-pos模型也是用来计算的，但是有所改变，先对词按照词性(Part-of-Speech，POS)进行了分类，具体的数学表达是
![{\rm{p}}\left( {{w_i}{\rm{|}}Contex{t_i}} \right) =](http://chart.apis.google.com/chart?cht=tx&chs=1x0&chf=bg,s,FFFFFF00&chco=000000&chl=%7B%5Crm%7Bp%7D%7D%5Cleft%28%20%7B%7Bw_i%7D%7B%5Crm%7B%7C%7D%7DContex%7Bt_i%7D%7D%20%5Cright%29%20%3D%20)![{\rm{p}}\left( {{w_i}|{\rm{c}}\left( {{w_{i - n + 1}}} \right),{\rm{c}}\left( {{w_{i - n + 2}}} \right), \cdots ,{\rm{c}}\left( {{w_{i - 1}}} \right)} \right)](http://chart.apis.google.com/chart?cht=tx&chs=1x0&chf=bg,s,FFFFFF00&chco=000000&chl=%7B%5Crm%7Bp%7D%7D%5Cleft%28%20%7B%7Bw_i%7D%7C%7B%5Crm%7Bc%7D%7D%5Cleft%28%20%7B%7Bw_%7Bi%20-%20n%20%2B%201%7D%7D%7D%20%5Cright%29%2C%7B%5Crm%7Bc%7D%7D%5Cleft%28%20%7B%7Bw_%7Bi%20-%20n%20%2B%202%7D%7D%7D%20%5Cright%29%2C%20%5Ccdots%20%2C%7B%5Crm%7Bc%7D%7D%5Cleft%28%20%7B%7Bw_%7Bi%20-%201%7D%7D%7D%20%5Cright%29%7D%20%5Cright%29)
其中c是类别映射函数，功能是把V个词映射到K个类别（1=<K<=V）。这样搞的话，原来的V个词本来有![{V^n}](http://chart.apis.google.com/chart?cht=tx&chs=1x0&chf=bg,s,FFFFFF00&chco=000000&chl=%7BV%5En%7D)种n元组减少到了![{\rm{V}} \times {{\rm{K}}^{n - 1}}](http://chart.apis.google.com/chart?cht=tx&chs=1x0&chf=bg,s,FFFFFF00&chco=000000&chl=%7B%5Crm%7BV%7D%7D%20%5Ctimes%20%7B%7B%5Crm%7BK%7D%7D%5E%7Bn%20-%201%7D%7D)种。
其他的模型还很多，不一一介绍了。

#### 2.2.4模型的问题与目标
如果是原始的直接统计语料库的语言模型，那是没有参数的，所有的概率直接统计就得到了。但现实往往会带一些参数，所有语言模型也能使用极大似然作为目标函数来建立模型。下面就讨论这个。
假设语料库是一个由T个词组成的词序列s（这里可以保留疑问的，因为从很多资料看来是不管什么多少篇文档，也不管句子什么的，整个语料库就是一长串词连起来的，或许可以根据情况拆成句子什么的，这里就往简单里说），其中有V个词，则可以构建下面的极大似然函数
![{\rm{L}} =  \prod \limits_{i = 1}^T p\left( {{w_i}{\rm{|}}Contex{t_i}} \right)](http://chart.apis.google.com/chart?cht=tx&chs=1x0&chf=bg,s,FFFFFF00&chco=000000&chl=%7B%5Crm%7BL%7D%7D%20%3D%20%20%5Cprod%20%5Climits_%7Bi%20%3D%201%7D%5ET%20p%5Cleft%28%20%7B%7Bw_i%7D%7B%5Crm%7B%7C%7D%7DContex%7Bt_i%7D%7D%20%5Cright%29)

另外，做一下对数似然
![{\rm{l}} = {\rm{logL}} = \frac{1}{V} \sum \limits_{i = 1}^T logp\left( {{w_i}{\rm{|}}Contex{t_i}} \right)](http://chart.apis.google.com/chart?cht=tx&chs=1x0&chf=bg,s,FFFFFF00&chco=000000&chl=%7B%5Crm%7Bl%7D%7D%20%3D%20%7B%5Crm%7BlogL%7D%7D%20%3D%20%5Cfrac%7B1%7D%7BV%7D%20%5Csum%20%5Climits_%7Bi%20%3D%201%7D%5ET%20logp%5Cleft%28%20%7B%7Bw_i%7D%7B%5Crm%7B%7C%7D%7DContex%7Bt_i%7D%7D%20%5Cright%29)

对数似然还有些人称为交叉熵，这里不纠结也不介绍。
上面的问题跟正常的情况不太符合，来看看下一种表达。假设语料库是有S个句子组成的一个句子序列（顺序不重要），同样是有V个词，似然函数就会构建成下面的样子
![{\rm{L}} =  \prod \limits_j^{\rm{S}} \left( { \prod \limits_{{i_j} = 1}^{{T_j}} p\left( {{w_{{i_j}}}{\rm{|}}Contex{t_{{i_j}}}} \right)} \right)](http://chart.apis.google.com/chart?cht=tx&chs=1x0&chf=bg,s,FFFFFF00&chco=000000&chl=%7B%5Crm%7BL%7D%7D%20%3D%20%20%5Cprod%20%5Climits_j%5E%7B%5Crm%7BS%7D%7D%20%5Cleft%28%20%7B%20%5Cprod%20%5Climits_%7B%7Bi_j%7D%20%3D%201%7D%5E%7B%7BT_j%7D%7D%20p%5Cleft%28%20%7B%7Bw_%7B%7Bi_j%7D%7D%7D%7B%5Crm%7B%7C%7D%7DContex%7Bt_%7B%7Bi_j%7D%7D%7D%7D%20%5Cright%29%7D%20%5Cright%29)

对数似然就会是下面的样子
![{\rm{l}} = {\rm{logL}} = \frac{1}{V} \sum \limits_{j = 1}^{\rm{S}} \left( { \sum \limits_{{i_j} = 1}^{{T_j}} logp\left( {{w_{{i_j}}}{\rm{|}}Contex{t_{{i_j}}}} \right)} \right)](http://chart.apis.google.com/chart?cht=tx&chs=1x0&chf=bg,s,FFFFFF00&chco=000000&chl=%7B%5Crm%7Bl%7D%7D%20%3D%20%7B%5Crm%7BlogL%7D%7D%20%3D%20%5Cfrac%7B1%7D%7BV%7D%20%5Csum%20%5Climits_%7Bj%20%3D%201%7D%5E%7B%5Crm%7BS%7D%7D%20%5Cleft%28%20%7B%20%5Csum%20%5Climits_%7B%7Bi_j%7D%20%3D%201%7D%5E%7B%7BT_j%7D%7D%20logp%5Cleft%28%20%7B%7Bw_%7B%7Bi_j%7D%7D%7D%7B%5Crm%7B%7C%7D%7DContex%7Bt_%7B%7Bi_j%7D%7D%7D%7D%20%5Cright%29%7D%20%5Cright%29)

有意向的同学可以扩展到有文档的样子，这里就不介绍了。
为啥要注意这个问题呢？原因有多种，计算![p({w_i}|Contex{t_i})](http://chart.apis.google.com/chart?cht=tx&chs=1x0&chf=bg,s,FFFFFF00&chco=000000&chl=p%28%7Bw_i%7D%7CContex%7Bt_i%7D%29)这个东西的参数是主要的原因。
为啥会有参数呢？在计算![p({w_i}|Contex{t_i})](http://chart.apis.google.com/chart?cht=tx&chs=1x0&chf=bg,s,FFFFFF00&chco=000000&chl=p%28%7Bw_i%7D%7CContex%7Bt_i%7D%29)这个东西的过程中，有非常多的方法被开发出来了，如上面的平滑法，回退法上面的，但这些都是硬统计一下基本就完了；这就带来一些需要求的参数，如平滑法中使用的分子分母分别加上的常数是什么？
这还不够，假如用的是trigram，还得存储一个巨大的元组与概率的映射（如果不存储，就得再进行使用的时候实际统计，那太慢了），存这个东西可需要很大的内存，对计算机是个大难题。
这都难不倒大牛们，他们考虑的工作是利用函数来拟合计算![p({w_i}|Contex{t_i})](http://chart.apis.google.com/chart?cht=tx&chs=1x0&chf=bg,s,FFFFFF00&chco=000000&chl=p%28%7Bw_i%7D%7CContex%7Bt_i%7D%29)，换句话说，不是根据语料库统计出来的，而是直接把context和wi代到一个函数里面计算出来的，这样在使用的时候就不用去查那个巨大的映射集了（或者取语料库里面统计这个概率）。用数学的方法描述就是
![p\left( {{w_i}{\rm{|}}Contex{t_i}} \right) = {\rm{f}}\left( {{w_i},Contex{t_i};{\rm{\theta }}} \right)](http://chart.apis.google.com/chart?cht=tx&chs=1x0&chf=bg,s,FFFFFF00&chco=000000&chl=p%5Cleft%28%20%7B%7Bw_i%7D%7B%5Crm%7B%7C%7D%7DContex%7Bt_i%7D%7D%20%5Cright%29%20%3D%20%7B%5Crm%7Bf%7D%7D%5Cleft%28%20%7B%7Bw_i%7D%2CContex%7Bt_i%7D%3B%7B%5Crm%7B%5Ctheta%20%7D%7D%7D%20%5Cright%29)
这样的工作也体现了科学家们的价值——这帮人终于有点东西可以忙了。
那么探索这个函数的具体形式就是主要的工作了，也是后面word2vec的工作的主要内容。函数的形式实在太多了，线性的还好，非线性真叫一个多，高维非线性的就更多了。
探索一个函数的具体形式的术语叫做拟合。
然后就有人提出了用神经网络来拟合这个函数，就有了各种方法，word2vec是其中的一种。

## 致谢
多位Google公司的研究员无私公开的资料。
多位博主的博客资料。

## 参考文献
[1][http://techblog.youdao.com/?p=915](http://techblog.youdao.com/?p=915)Deep Learning实战之word2vec，网易有道的pdf
[2][http://www.zhihu.com/question/21714667/answer/19433618](http://www.zhihu.com/question/21714667/answer/19433618)@皮果提在知乎上的问答
[3][http://www.zhihu.com/question/21661274/answer/19331979](http://www.zhihu.com/question/21661274/answer/19331979)@杨超在知乎上的问答《Word2Vec的一些理解》
[4] ​第​五​章​ ​n​-​g​r​a​m​语​言​模​型  百度文库上的一个资料
[5] 主题：统计自然语言处理的数学基础  百度文库上的一个资料









