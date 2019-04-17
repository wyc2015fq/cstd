# 自然语言处理的bert, GPT, GPT-2, transformer, ELMo, attention机制都是些何方神圣？？？ - ibelieve8013的博客 - CSDN博客





2019年03月07日 22:50:41[ibelieve8013](https://me.csdn.net/ibelieve8013)阅读数：267








2018年是NLP领域巨变的一年，这个好像我们都知道，但是究竟是哪里剧变了，哪里突破了？经常听大佬们若无其事地抛出一些高级的概念，你却插不上嘴，隐隐约约知道有这么个东西，刚要开口：噢！你说bert啊，我知道，就是一个预训练模型，然后，然后。。。然后就没有然后了，若是在面试场上，你三秒钟就结束了考官的提问，那尴尬的考官估计只能以你太快的理由拒绝给你发offer。

好的，闲话少说，我们来总结一下这么些高级的概念，究竟从何说起呢？首先要推荐大家读一读，[https://zhuanlan.zhihu.com/p/49271699](https://zhuanlan.zhihu.com/p/49271699)，张俊林大佬的解读。本文也会以该文的思路为主线，加上一些自己的总结和理解。



首先，说到以上这些概念，不得不提词向量。而词向量，顾名思义，就是把词向量化，这样有神马好处我们就不多说了，获得词在向量空间的表示，可以度量词之间的距离，相关度等等……我们一般有两种获取方式，一种是比较传统的，基于共现矩阵，SVD等等，另一种就是语言模型。

第一种，基于共现矩阵就稍微提一下，这个的意思呢，就是统计指定窗口大小各个词的共现频次，这样的思想就是：可以用某个词的周边词，来表示这个词。

比如经典的三句话：

I like deep learning.
I like NLP.

I enjoy flying.

表示成共现矩阵：

![https://upload-images.jianshu.io/upload_images/1803066-5dd560fdffefeabf.png?imageMogr2/auto-orient/](https://upload-images.jianshu.io/upload_images/1803066-5dd560fdffefeabf.png?imageMogr2/auto-orient/)

那我们不禁要问一句，为什么不用one-hot的方法来做呢，其实你可以想象，用one-hot来做，每个词之间的相似度似乎都是0，而且超级稀疏，那还玩个毛线啊，词之间的关联一点都刻画不出来，这对文本分析是个致命的问题。现在出现这么个共现矩阵，你说这下该可以坐享其成了吧，其实不是，这个的问题也很明显，因为，还是很稀疏呀，什么意思呢，就是说，Corpus里面有多少词，你就要多少维？？那显然会有维度灾难，并且也很稀疏。

好的，你说高维稀疏那么就好办呀，咱们的那些降维的技术拿出来呀，一个很经典的方法就是SVD,那么SVD做了什么工作呢，这里的SVD就不展开讲了，就好像数学老师下课的时候：最后讲两句，结果到上课铃响语文老师都进来了还没讲完。

我们只要知道，

![https://upload-images.jianshu.io/upload_images/1803066-114dc19577d80a98.png?imageMogr2/auto-orient/](https://upload-images.jianshu.io/upload_images/1803066-114dc19577d80a98.png?imageMogr2/auto-orient/)

现在X就是共现矩阵，分解之后，得到正交矩阵U，U归一化后作为现在每个词的词向量。

现在得到了词的稠密矩阵，语义相近的词在空间的距离很近，甚至一定程度达到了反映词之间的线性关系的效果。

以上是基于词的共现矩阵来获取词向量，这种方法在推荐系统里面也经常用，可以获取item和item之间的相似关系。

接下来重磅推出基于语言模型的词向量。

大家公认基于语言模型的词嵌入技术是在2003年Bengio提出的，就是经典的NNLM模型，但是又在2013年因为word2vec技术重新被认知。那么这些个模型，究竟有什么神奇的地方呢？

这个推荐大家看看《word2wec背后的数学原理》这个文章，写得很细致，在这里我们也不去推导其细节。

等等，我们是不是要先说说什么是语言模型？

一句话说：语言模型就是用来衡量一句话多大程度是人话的模型。好吧，这句话就很不人话。

详细一点，一句话概率，我们可以用这句话每个词的联合概率表示，进一步，又可以表示成多个条件概率的乘积。这个乘积越大，它这句话越可能是一句话。

![](https://img-blog.csdnimg.cn/20190307224920280.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)

我推荐看看宗成庆老师的统计自然语言处理，说得很清楚。

好了，要扯这边了，首先是NNLM, neural network of language model. 这个模型说来也是很有意思。现在咱不是可以根据语料对每个词进行one-hot编码吗，这个模型假设了一个C矩阵，用one-hot 乘以C矩阵，得到的就是该词的嵌入表示，（当然，这里的C矩阵大小维度和one-hot维度有关，也和嵌入后的维度有关。）然后，再用某词的上文词（context）嵌入后的稠密表示，做一个拼接，之后按照神经网络的思路，通过激活函数，然后全连接到原来one-hot的维度上，做一个softmax，就构建了一个神经网络。

说起来很抽象，看下图。

![https://pic2.zhimg.com/80/v2-e2842dd9bc442893bd53dd9fa32d6c9d_hd.jpg](https://pic2.zhimg.com/80/v2-e2842dd9bc442893bd53dd9fa32d6c9d_hd.jpg)

这里要特别说明一点的就是：这个C矩阵，是一个随机初始化的矩阵，然后可以通过不断的训练，得到最终的嵌入矩阵，神奇的就在这里，我们仅仅是想构建一个神经网络语言模型，根据前面几个词，预测后一个可能是什么词。C矩阵是我们要训练的参数，结果我们却发现，我们的这个C矩阵，就是每个词的嵌入向量，真是踏破铁鞋无觅处，得来真特瞄的费劲。

然后也不知道沉睡了多久，也不知还要多久才能睁开双眼。我们的word2vec出现在2013年，word2vec有两种训练方式，一种是CBOW: continuous bag-of-words. 一种是skip-gram.

这两个也很有意思，CBOW是根据周围的几个词预测中间的词，skip-gram是根据中间的一个词预测周围的几个词。他们和NNLM有什么关系吗？

有! 那CBOW来说，它相比NNLM，少了隐藏层，而且上下文通过投影层之后，是直接相加的，而我们上面说了，NNLM是通过拼接的方式。我们的CBOW就是奔着词向量去的。当然，这个不仅仅是这一点改进，最牛的还是w2v后面的大量运算的简化思想，这里也不说了，在《word2vec背后的数学原理》讲得十分的清楚，主要是两种手段：1, 层次化的softmax。 2, 负例采样。 至于这里的层次化softmax是怎么样用的哈夫曼树，负例采样的一些采样细节，我们也不在这里说了。



![](https://img-blog.csdnimg.cn/20190307225214744.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)

然后说得十分闹热，这个词向量我们已经获得了，那为什么不能把这个看成是一个预训练模型呢，其实是可以的，就和图像领域的预训练似乎差不多了，而且也有两种打开方式：一种是frozen, 另一种当然就是fine-tune. 说到这里，我们还是要指出word2vec的不足，就是，这个模型，不能够很好地做歧义消解，怎么说呢，摊牌了，就是说这是一个静态向量：那我们在谈论苹果的时候，我们究竟在谈论什么？对的，我们在谈论乔布斯的iPhone，还是谈论砸中牛阿顿的那个东西？很显然，无从得知，因为你的embedding是固定的，没法区分，对不对？所以才会有动态词向量这个概念产生咯。顾名思义，动态词向量，就是能够动的词向量，动体现在，随着上下文的不一致，我们产生的词向量也是不一致的，这好理解吧，就好比说，我们看苹果出现的时候，它周围出现的是乔布斯，还是牛阿顿，是一款，还是一斤？

这里就隆重地推出来ELMo, embedding from language models.

ELMo的官网：[https://allennlp.org/elmo](https://allennlp.org/elmo)，这个神器Elmo有什么好处呢，据说可以处理单词用法的复杂特性，如句法和语义（为什么可以？是由于ELMo的输入是字母为单位，而不是单词，所以这对于oov未登录的单词也有很好的效果）。它和word2vec的最大不同就是，它提供了一个动态的词向量，根据上下文的不同，模型产出的词向量是不同的。

说一说Elmo模型的结构，

![https://upload-images.jianshu.io/upload_images/1803066-4afb39af50810e65.png?imageMogr2/auto-orient/](https://upload-images.jianshu.io/upload_images/1803066-4afb39af50810e65.png?imageMogr2/auto-orient/)它是基于深度双向语言模型（biLM）训练的，即，通过双向的LSTM来提取文本特征，根据它们的内部状态学习到的函数，作为词向量。

这里还是说一下双向的意思，通俗的说，就是把文本正向反向都提取一遍，这个叫双向。上面的biLSTM内部结构可以再细化一点，就是下面这个样子，经过一个biLSTM之后，输出的是正向的和逆向的拼接向量。

ELMo的贡献还在于，它发现，在深层的RNN模型中，不同层次的RNN提取到的特征是有差异的。所以ELMO提出，赋予不同的层次一个可训练的参数，这个参数作为权重，以方便在做下游任务时，用这个加权和的词向量能更好适应任务。



![https://img-blog.csdn.net/2018082123011119?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70](https://img-blog.csdn.net/2018082123011119?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



Elmo可以通过pip install allennlp来安装，也可以download GitHub上的源码来做[[https://github.com/allenai/allennlp]](https://github.com/allenai/allennlp%5D)，当然这个项目是构建在pytorch上面的。

这个模型还可以自如的使用：

![https://upload-images.jianshu.io/upload_images/1803066-e43e5f32e191aba1.png?imageMogr2/auto-orient/](https://upload-images.jianshu.io/upload_images/1803066-e43e5f32e191aba1.png?imageMogr2/auto-orient/)

这个模型看起来十分不错，但是就是速度太慢了。加上我们站在这里来往前看历史，我们发现，LSTM的特征抽取的能力偏弱，而Google提出的transformer抽取器，则是一个更牛逼的存在。

Transformer

这里祭上大名鼎鼎的：Attention is all you need. [https://arxiv.org/abs/1706.03762](https://arxiv.org/abs/1706.03762).

要说到这个transformer，不得不提到Encoder-decoder模型，我们知道，Encoder-decoder经常被用来做无监督的特征提取工作。

![](https://img-blog.csdnimg.cn/20190307225239384.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)

当我们把编码组件给剖开，我们看看里面是何方神圣，我们发现编码组件里面包含很多个编码器，官方取的6个，当然你也可以取其他数量。而再细看，每个编码器又分为：self-attention 层和feed-forward层。

![](https://img-blog.csdnimg.cn/20190307225245923.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)那可以这么说，要弄懂transformer，就得先搞懂这两层：

自注意力层：

说到自注意力，我们都知道，通俗来说：自注意力是为给整个句子所有的单词一定的权重，得到一定的关注。此处，引入self-attention层，当然是为了让每个单词在进行编码时，都能关注要句子的其他单词。

然后自注意力层的输出送到前馈神经网络，注意，这里每个单词对应的前馈神经网络都是一样的。

后面再深入谈谈注意力机制。

【[https://blog.csdn.net/malefactor/article/details/50550211](https://blog.csdn.net/malefactor/article/details/50550211)，可以看这个博客，张俊林大佬，怎么又是这个大佬，很生动的用rnn的encoder-decoder模型阐述了自注意力模型AM,简而言之，比如说在翻译任务中，这个自注意力相当于是一个对齐概率，是原输入句子单词和目标单词之间的对齐概率，这个概率怎么来的？比如要输出yi的目标单词，那么这个对齐概率就是在i时刻的隐状态Hi，分别与输入句子单词的中间状态h1->hm的内积（这里的内积只是一个方式，也可以用其他的函数），再做softmax得到的。

![https://img-blog.csdn.net/20160120182108205](https://img-blog.csdn.net/20160120182108205)

反正大佬说这样做是非常有道理的，而且我也信了，就和传统的翻译系统中间有个词对齐的过程，在这里要翻译，也得大致知道对齐的是谁，这个概率分布，就是我们说的注意力。

![https://img-blog.csdn.net/20160120182219236](https://img-blog.csdn.net/20160120182219236)

】

【

这里抠一抠自注意力层的细节，比如说，现在有一个句子，这个句子输入进自注意力层，详细步骤是怎么样的呢？

首先声明一点概念，我们在自注意力层会训练三个权重矩阵，分别是WQ查询矩阵，WK键矩阵,WV值矩阵, 这里有没有想起老大哥NNLM那个午后的C矩阵？

我们训练这三个矩阵，是为了干嘛呢？就是分别乘以初始的词嵌入向量，得到三个向量，分别是q查询向量，k键向量，v值向量。

看图：

![](https://img-blog.csdnimg.cn/2019030722530676.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)

好了，大神画的这个图已经足够一目了然了。

得到了这三个向量，然后干嘛呢，仔细听：当我们要编码X1向量时，用q1向量乘以K1向量，用q1乘以k2,分别得到两个得分，而这两个得分softmax归一化后作为权重，乘以各自的值向量。而值向量再求和，得到的就是注意力层该位置的输出！如果参数被训练的足够好，那么这个编码是完全能够体现出自注意力的特性的。

![](https://img-blog.csdnimg.cn/20190307225311937.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)

但是呢，这样一个词一个词来，很慢，我们肯定要想办法提高速度，我们发现这个和RNN还不一样，各个词之间输入输出之间没有先后顺序，可以做并行，那么我们就会直接用矩阵的乘法来解决这个事情了：

![](https://img-blog.csdnimg.cn/20190307225318533.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190307225324765.png)

不用惊讶，这里的过程和上面我们说的是一模一样，除了根号dk，是在归一化之前的一个小处理，并不影响。

得到的Z，就是我们的带注意力的词编码，是自注意力层的输出。

进一步我们要多讨论一下，transformer里面有个概念叫多头，multi-headed attention, 这个说的是啥呢，很容易理解，就是大佬们觉得你一个Q, K, V矩阵好像不够啊，这怎么弄呢，就用多组Q, K, V矩阵，重复以上的操作

![](https://img-blog.csdnimg.cn/20190307225335798.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)

会输出多个Z矩阵，然后再进前馈层，但是前馈层是一样的，可以吧多个Z矩阵合并再进行操作，合并的细节：可以把Z矩阵们拼接，再乘以权重矩阵，得到最终Z矩阵。

看下图，更清晰：

![](https://img-blog.csdnimg.cn/20190307225340967.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)

】

位置编码！

这个transformer模型，没有LSTM的序列关系，那么是不是就意味着，全程都没有表示词的位置的信息？

为了解决这个问题，transformer特地用了一种位置编码的方式，赋予每个位置的单词一个位置编码，然后把这个位置编码和词嵌入相加。

Transformer的残差模块

的这里有小细节，需要注意，我们看原结构：

![](https://img-blog.csdnimg.cn/20190307225349812.png)

其实这个是简化的，啥意思呢，就是说，有细节没有画出来。

每一个编码器，到前馈神经网络输入的时候，其实是原向量和训练后的向量相加的结果：

![](https://img-blog.csdnimg.cn/2019030722535515.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)





在解码器中，除了编码器有的这两层，还多出一层编码-解码注意力层：

![](https://img-blog.csdnimg.cn/20190307225402719.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)

整个过程捋一遍：编码层在训练之后，得到许多个矩阵（Q,K,V），这些矩阵会送到解码层里面，解码的过程，就是每输入一个词，预测出下一个词，直到到达终止符。另外，就像我们对编码器的输入所做的那样，我们会嵌入并添加位置编码给那些解码器，来表示每个单词的位置。

这里的线性变换和softmax，是一个全连接层，从实数向量到一个很长的logits向量，然后softmax。



[https://www.jiqizhixin.com/articles/2019-01-09-18](https://www.jiqizhixin.com/articles/2019-01-09-18)，这里有机器之心更详尽的阐述。





这里基本上讲完了transformer，而我们接下来的任务就是bert了，先记一个小tip：为了表示各个单词的位置信息，我们知道transformer是有个位置编码的东西存在的，bert是给了每个单词一个position embedding，然后和单词本身的embedding相加起来得到最终的向量。



GPT：generative pre-train model

GPT的精髓可以说就是Transformer了，可惜的就是GPT采用的单向语言模型：

![https://pic1.zhimg.com/80/v2-5028b1de8fb50e6630cc9839f0b16568_hd.jpg](https://pic1.zhimg.com/80/v2-5028b1de8fb50e6630cc9839f0b16568_hd.jpg)

也是两阶段的模型，第一阶段：用语言模型做预训练，第二阶段通过fine-tuning的模式解决下游任务。

解决下游任务的时候注意，这里有比较大的不同，本来我们得到了预训练的词向量，可以任意设计自己的网络结构，但是这里，要根据GPT的网络结构，来改造下游任务的结构，使其适应现在的网络结构，这样有什么好处呢？我的理解就是，我们可以直接把预训练的参数拉过来，这就初始化了网络结构，然后，在这个基础上，可以直接fine-tuning，这个和图像的预训练模型是不是就很像了？

但是至于各个下游任务是怎么改造的，这个我还没怎么搞清楚，大佬的文章里写得很轻松，看下图：

![https://pic1.zhimg.com/80/v2-4c1dbed34a8f8469dc0fefe44b860edc_hd.jpg](https://pic1.zhimg.com/80/v2-4c1dbed34a8f8469dc0fefe44b860edc_hd.jpg)

“GPT论文给了一个改造施工图如上，其实也很简单：对于分类问题，不用怎么动，加上一个起始和终结符号即可；对于句子关系判断问题，比如Entailment，两个句子中间再加个分隔符即可；对文本相似性判断问题，把两个句子顺序颠倒下做出两个输入即可，这是为了告诉模型句子顺序不重要；对于多项选择问题，则多路输入，每一路把文章和答案选项拼接作为输入即可。从上图可看出，这种改造还是很方便的，不同任务只需要在输入部分施工即可。”

Bert：来到了bert，bert和GPT不一样的就是预训练采用的双向语言模型，其他的：也是两阶段，也是transformer。

![https://pic1.zhimg.com/80/v2-7aa8d891632fdd522499f96e7f14cac4_hd.jpg](https://pic1.zhimg.com/80/v2-7aa8d891632fdd522499f96e7f14cac4_hd.jpg)



其实bert还有很多技术细节，如masked LM等。

未完待续



