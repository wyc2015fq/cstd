# Attention模型方法综述 | 多篇经典论文解读 - Paper weekly - CSDN博客





2018年06月11日 14:40:23[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：3735









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




**本文基于几篇经典的论文，对 Attention 模型的不同结构进行分析、拆解。**




先简单谈一谈 attention 模型的引入。以基于 seq2seq 模型的机器翻译为例，如果 decoder 只用 encoder 最后一个时刻输出的 hidden state，可能会有两个问题（我个人的理解）。




1. encoder 最后一个 hidden state，与句子末端词汇的关联较大，难以保留句子起始部分的信息；




2. encoder 按顺序依次接受输入，可以认为 encoder 产出的 hidden state 包含有词序信息。所以一定程度上 decoder 的翻译也基本上沿着原始句子的顺序依次进行，但实际中翻译却未必如此，以下是一个翻译的例子：




> 
**英文原句：**space and oceans are the new world which scientists are trying to explore.




**翻译结果：**空间和海洋是科学家试图探索的新世界。 





词汇对照如下：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loQNwkgxziavsDjwOO9wu5BtQOoYEsRp9k2SYkUHbOq0qHaO49paSqkxA/640)




可以看到，翻译的过程并不总是沿着原句从左至右依次进行翻译，例如上面例子的定语从句。 




为了一定程度上解决以上的问题，14 年的一篇文章 **S****equence to Sequence Learning with Neural Networks **提出了一个有意思的 trick，即在模型训练的过程中将原始句子进行反转，取得了一定的效果。 




为了更好地解决问题，attention 模型开始得到广泛重视和应用。 




下面进入正题，进行对 attention 的介绍。




# Show, Attend and Tell




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lolT2TlhmR51zgJpVDP9pw5iabWicXykoJic1EibeDvRW9F3VbLuRkMVOKVQ/640)




■ 论文 | Show, Attend and Tell: Neural Image Caption Generation with Visual Attention

■ 链接 | https://www.paperweekly.site/papers/812

■ 源码 | https://github.com/kelvinxu/arctic-captions




文章讨论的场景是图像描述生成（Image Caption Generation），对于这种场景，先放一张图，感受一下 attention 的框架。





![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loehibqJqnxz4hhm1shadbL1JJn8W4YYGibibx4yKUoNpzNOZ89XUkXOQSg/640)




文章提出了两种 attention 模式，即 hard attention 和 soft attention，来感受一下这两种 attention。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lowh8YsYklgIvkd0KhYHIJGjbLib6PPcNeicjsz8kGGTWhbicTucxux8hsQ/640)




可以看到，hard attention 会专注于很小的区域，而 soft attention 的注意力相对发散。模型的 encoder 利用 CNN (VGG net)，提取出图像的 L 个 D 维的向量![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lol7FZMzJ4vr05ObrQ5ib02FqwtIqiaTzhXrUF1UnOnfGMMWIs2am6yqNQ/640)，每个向量表示图像的一部分信息。




decoder 是一个 LSTM，每个 timestep t 的输入包含三个部分，即 context vector Zt 、前一个 timestep 的 hidden state![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lo3y9WnL3JV2Lk2w7avssYRsnibaKbY7GRXgSOjwUIzXhKh36IIP03KCw/640)、前一个 timestep 的 output![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loE0ib7lUv5QiayoWGk2XR7pDX1Gia91hbA2ib8iaxBu1nQWEjN9QkQMxNiaRw/640)。 Zt 由 {ai} 和权重 {αti} 通过加权得到。这里的权重 αti 通过attention模型 *f*att 来计算得到，而本文中的 *f*att 是一个多层感知机（multilayer perceptron）。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lo3LlZElvscR5cyEhn17kwPvQvtl25piaON4l6nUJdn3rhpiaZcDxVD0kw/640)




从而可以计算![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loeAMsdSVyyv3K8Ggtg2VBdpbkqzVa1RcBoMEPLw5rBVnz8S6OcMERjA/640)。接下来文章重点讨论 hard（也叫 stochastic attention）和 soft（也叫 deterministic）两种 attention 模式。




**1. Stochastic “Hard” Attention**




记 St 为 decoder 第 t 个时刻的 attention 所关注的位置编号， Sti 表示第 t 时刻 attention 是否关注位置 i ， Sti 服从多元伯努利分布（multinoulli distribution）， 对于任意的 t ，Sti,i=1,2,...,L 中有且只有取 1，其余全部为 0，所以 [St1,St2,...,stL] 是 one-hot 形式。这种 attention 每次只 focus 一个位置的做法，就是“hard”称谓的来源。 Zt 也就被视为一个变量，计算如下：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lo6c2o95GPUpjdzYZfQAwxDaCsOdibKC2N13TOyO9RxIetNicq0wEhEhAA/640)




问题是 αti 怎么算呢？把 αti 视为隐变量，研究模型的目标函数，进而研究目标函数对参数的梯度。直观理解，模型要根据 a=(a1,...,aL) 来生成序列 y=(y1,...,yC) ，所以目标可以是最大化 log p(y|a) ，但这里没有显式的包含 s ，所以作者利用著名的 Jensen 不等式（Jensen's inequality）对目标函数做了转化，得到了目标函数的一个 lower bound，如下：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7logiaj93SfuAodEHcmf8cN4qm1a3etyfoYic2DvZhTNF8xfTpuLq1iaA0DA/640)




这里的 s ={ s1,...,sC }，是时间轴上的重点 focus 的序列，理论上这种序列共有![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loAwb2oq0ylHKbD0eJatLbeafSAODl4CrNeFqyg7ia6LBuNkflMf0Dkibw/640)个。 然后就用 log p(y|a) 代替原始的目标函数，对模型的参数 W 算 gradient。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loBGySIsHQceEfHggpYLEJujeRoRs5HO3kX2d4YWUeEl5IM5eNJkic6lA/640)




然后利用蒙特卡洛方法对 s 进行抽样，我们做 N 次这样的抽样实验，记每次取到的序列是![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loic8RPyLrNWCDTKPWo6AOr2TCgcPEeBYiagrNBKvOz5dlAqBr239zJK8w/640)，易知![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loic8RPyLrNWCDTKPWo6AOr2TCgcPEeBYiagrNBKvOz5dlAqBr239zJK8w/640)的概率为![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loialZ7dVH8KDj45G7mEDibDWPtINr3u73cPugBBPdBvnf05pcUelhjuWg/640)，所以上面的求 gradient 的结果即为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7logcUqxG75YPrq4ILmDcSpqAQxfOpyLGhDCujNE8xzXCATmavqrEJgvA/640)




接下来的一些细节涉及reinforcement learning，感兴趣的同学可以去看这篇 paper。




**2. Deterministic “Soft” Attention **




说完“硬”的 attention，再来说说“软”的 attention。 相对来说 soft attention 很好理解，在 hard attention 里面，每个时刻 t 模型的序列 [ St1,...,StL ] 只有一个取 1，其余全部为 0，也就是说每次只 focus 一个位置，而 soft attention 每次会照顾到全部的位置，只是不同位置的权重不同罢了。这时 Zt 即为 ai 的加权求和：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lo6JuNYVOUmaChp1QWvKpwVODXsQ5ufxFQVibcqGiaMAcicgmJI5qnOQkVw/640)




这样 soft attention 是光滑的且可微的（即目标函数，也就是 LSTM 的目标函数对权重 αti 是可微的，原因很简单，因为目标函数对 Zt 可微，而 Zt 对 αti 可微，根据 chain rule 可得目标函数对 αti 可微）。




文章还对这种 soft attention 做了微调：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loeLwJlFS6TJn9oaGJWQoMWe0FHvMaKyn3eOFq4CCzkeS4RfTdyjMnXQ/640)




其中![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lod1A0J790XNX9uLHFfDK5upFIwSZJ6QcgvIGLbiaww9LHm6BHOhun5VQ/640)，用来调节 context vector 在 LSTM 中的比重（相对于![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lo3y9WnL3JV2Lk2w7avssYRsnibaKbY7GRXgSOjwUIzXhKh36IIP03KCw/640)、 ![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loE0ib7lUv5QiayoWGk2XR7pDX1Gia91hbA2ib8iaxBu1nQWEjN9QkQMxNiaRw/640)的比重）。




btw，模型的 loss function 加入了 αti 的正则项。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lo05cQwwnlH87EAhiaauHqebsibMBgD1UODlAuJFVzib1LWyxjzwW3feMicA/640)




# Attention-based NMT




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lo3gicCWUZY5t7ZCaj4cN13iaBsMtEjxSZD63vbA1ibHnx62rm5icVY0VMIQ/640)




■ 论文 | Effective Approaches to Attention-based Neural Machine Translation

■ 链接 | https://www.paperweekly.site/papers/806

■ 源码 | https://github.com/lmthang/nmt.matlab




文章提出了两种 attention 的改进版本，即 global attention 和 local attention。先感受一下 global attention 和 local attention 长什么样子。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lohgiaJq0ZQ9KUrzfdTqp9BemfquS3xZS7PLA4t6Kc4ERmIFnoAFtcqqg/640)

**▲**Global Attention

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loqyKWz7mHIwAzQfkTLKgWcLkia34mP5P6A0tIQezIeXTUJye7vMDargA/640)

**▲**Local Attention




文章指出，local attention 可以视为 hard attention 和 soft attention 的混合体（优势上的混合），因为它的计算复杂度要低于 global attention、soft attention，而且与 hard attention 不同的是，local attention 几乎处处可微，易与训练。 文章以机器翻译为场景， x1,...,xn 为 source sentence， y1,...,ym 为 target sentence， c1,...,cm 为 encoder 产生的 context vector，objective function 为：





![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7logdYgGSND4J3nANAIpcxVjrAHQicicPhZ67cYdsuzziczT0rcqVwibWyNBQ/640)




Ct 来源于 encoder 中多个 source position 所产生的 hidden states，global attention 和 local attention 的主要区别在于 attention 所 forcus 的 source positions 数目的不同：如果 attention forcus 全部的 position，则是 global attention，反之，若只 focus 一部分 position，则为 local attention。 




由此可见，这里的 global attention、local attention 和 soft attention 并无本质上的区别，两篇 paper 模型的差别只是在 LSTM 结构上有微小的差别。 




在 decoder 的时刻 t ，在利用 global attention 或 local attention 得到 context vector Ct 之后，结合 ht ，对二者做 concatenate 操作，得到 attention hidden state。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lo7UndPPgZBGJia2t56sAef3cjXmGG2ecJS46efsTxnL2AezZCsYnWI7w/640)




最后利用 softmax 产出该时刻的输出：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loOJgNz5R3CeXDIcWL41flrA0GVqWCDickJJKicuL56WlbNDbRWV9o5QdA/640)




下面重点介绍 global attention、local attention。




**1. global attention **




global attention 在计算 context vector ct 的时候会考虑 encoder 所产生的全部hidden state。记 decoder 时刻 t 的 target hidden为 ht，encoder 的全部 hidden state 为![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loFLwG2vk96XtnReiakX2AtOCFWE2MJIrLxzwcLQkaia5f8I7JAibKbHrcQ/640)，对于其中任意![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loVWfD3TKURkhfpbmOzib2SRfLeWpZeJjxRE1I1c5hs9Xr7ic78OdGr7WQ/640)，其权重 αts 为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lo3Hzhib8fQ9zBpwG9c2riamQz0HEbUtMtTr6z7351OyZRAiaTuzjNMGgxg/640)




而其中的![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7logtyXhKB21fAb1AF9EBZRGKBHJvsYsdQp4vZJUgsTe50JdgRibG4hHKA/640)，文章给出了四种种计算方法（文章称为 alignment function）：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lofI4fwNWxiaUxHOPsgN7jDoGJ8ibQlJlpxKEHkcGA5nSic0O6TL864H0uA/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lo5ibWrqBUicViaAia3GkpRY4Kwuz3fcuDGGuxT2ibrKHGIe0Iib7IUicMDEZwg/640)




四种方法都比较直观、简单。在得到这些权重后， ct 的计算是很自然的，即为![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loVWfD3TKURkhfpbmOzib2SRfLeWpZeJjxRE1I1c5hs9Xr7ic78OdGr7WQ/640)的 weighted summation。



**2. local attention **




global attention 可能的缺点在于每次都要扫描全部的 source hidden state，计算开销较大，对于长句翻译不利，为了提升效率，提出 local attention，每次只 focus 一小部分的 source position。 




这里，context vector ct 的计算只 focus 窗口 [pt-D,pt+D] 内的 2D+1 个source hidden states（若发生越界，则忽略界外的 source hidden states）。




其中 pt 是一个 source position index，可以理解为 attention 的“焦点”，作为模型的参数， D 根据经验来选择（文章选用 10）。 关于 pt 的计算，文章给出了两种计算方案：



- 
**Monotonic alignment (local-m)**





![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lo1jOrpjt3kz28rfiajLs08Pmiaicm6jZMrnSOKibf5KiciapSzic9NtyAmY1cw/640)



- 
**Predictive alignment (local-p)**





![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loCtwsMEus3icfnaH7g5bWUtQ7PuEDQqSCfdy4489OOhdDINIib74su1RQ/640)




其中 Wp 和 vp 是模型的参数， S 是 source sentence 的长度，易知 pt∈[0,S] 。 权重 αt(s) 的计算如下：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loxIA1ibhdXiau7KThIOORsa3ZicgvTx9Lbm7UrvkgEOxia3PZIdAbhHMFlw/640)




可以看出，距离中心 pt 越远的位置，其位置上的 source hidden state 对应的权重就会被压缩地越厉害。




# Jointly Learning




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loQmLml8WY8seURgdWQLpCdBicibM2D85BTFp6dxLTLhg5nRzpYicLEdsdw/640)




■ 论文 | Neural Machine Translation by Jointly Learning to Align and Translate

■ 链接 | https://www.paperweekly.site/papers/434

■ 源码 | https://github.com/spro/torch-seq2seq-attention




这篇文章没有使用新的 attention 结构，其 attention 就是 soft attention 的形式。文章给出了一些 attention 的可视化效果图。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lolCnCcQKZpqYzwIEibDibwC1beXQZDxEvRfF9B0qNNvlKBLarGAqROvsg/640)




上面 4 幅图中，x 轴代表原始英文句子，y 轴代表翻译为法文的结果。每个像素代表的是纵轴的相应位置的 target hidden state 与横轴相应位置的 source hidden state 计算得到的权重 αij，权重越大，对应的像素点越亮。可以看到，亮斑基本处在对角线上，符合预期，毕竟翻译的过程基本是沿着原始句子从左至右依次进行翻译。




# Attention Is All You Need




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lo30Ye1BPmYlUzUcxdpEsQvAw7NXdtCiaVKQhPsWOcGJsC7ql06jVtQzA/640)




■ 论文 | Attention Is All You Need

■ 链接 | https://www.paperweekly.site/papers/224

■ 源码 | https://github.com/Kyubyong/transformer




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lojd57mSOh4kNcH8oDBJTib2Xa7N02C7vMgzwIF6aib6JucnPzrTTUQLZw/640)




■ 论文 | Weighted Transformer Network for Machine Translation

■ 链接 | https://www.paperweekly.site/papers/2013

■ 源码 | https://github.com/JayParks/transformer




作者首先指出，结合了 RNN（及其变体）和注意力机制的模型在序列建模领域取得了不错的成绩，但由于 RNN 的循环特性导致其不利于并行计算，所以模型的训练时间往往较长，在 GPU 上一个大一点的 seq2seq 模型通常要跑上几天，所以作者对 RNN 深恶痛绝，遂决定舍弃 RNN，只用注意力模型来进行序列的建模。 




作者提出一种新型的网络结构，并起了个名字 Transformer，里面所包含的注意力机制称之为 self-attention。作者骄傲地宣称他这套 Transformer 是能够计算 input 和 output 的 representation 而不借助 RNN 的唯一的 model，所以作者说有 attention 就够了。




模型同样包含 encoder 和 decoder 两个 stage，encoder 和 decoder 都是抛弃 RNN，而是用堆叠起来的 self-attention，和 fully-connected layer 来完成，模型的架构如下：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loLrKGgMncCvHX9QCLJuOApGmgqQXEGoPUbPEJjYiaa9YXaxH5YjB4YkA/640)




从图中可以看出，模型共包含三个 attention 成分，分别是 encoder 的 self-attention，decoder 的 self-attention，以及连接 encoder 和 decoder 的 attention。  




这三个 attention block 都是 multi-head attention 的形式，输入都是 query Q 、key K 、value V 三个元素，只是 Q 、 K 、 V 的取值不同罢了。接下来重点讨论最核心的模块 multi-head attention（多头注意力）。 




multi-head attention 由多个 scaled dot-product attention 这样的基础单元经过 stack 而成。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lobdCxhyiaZUWpwoq6FLnNsVTIIaSJmxleDPTt9VFkTsicia5Gg3sFQpia2A/640)




那重点就变成 scaled dot-product attention 是什么鬼了。按字面意思理解，scaled dot-product attention 即缩放了的点乘注意力，我们来对它进行研究。 




在这之前，我们先回顾一下上文提到的传统的 attention 方法（例如 global attention，score 采用 dot 形式）。




记 decoder 时刻 t 的 target hidden state 为 ht，encoder 得到的全部 source hidden state为![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loqDxAT2FDm3IOMAw0HkD02hhI03eWZDdMK5f6MSDiaFfMpicLRFamOOkg/640)，则 decoder 的 context vector ct 的计算过程如下：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lo02Wd9TxmvkCMr6ZhVe8slVlJXoUtNfqEmD9AED1FrLLLtgS3QZleNw/640)




作者先抛出三个名词 query Q、key K、value V，然后计算这三个元素的 attention。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loDvue0icGsrhb95cian9Y6mvgehg3XFpIiaXoRFUaTZ8hrhstGRtXrr5cw/640)




我的写法与论文有细微差别，但为了接下来说明的简便，我姑且简化成这样。这个 Attention 的计算跟上面的 (*) 式有几分相似。




那么 Q、K、V 到底是什么？论文里讲的比较晦涩，说说我的理解。encoder 里的 attention 叫 self-attention，顾名思义，就是自己和自己做 attention。




抛开这篇论文的做法，让我们激活自己的创造力，在传统的 seq2seq 中的 encoder 阶段，我们得到 n 个时刻的 hidden states 之后，可以用每一时刻的 hidden state hi，去分别和任意的 hidden state hj,j=1,2,...,n 计算 attention，这就有点 self-attention 的意思。




回到当前的模型，由于抛弃了 RNN，encoder 过程就没了 hidden states，那拿什么做 self-attention 来自嗨呢？




可以想到，假如作为 input 的 sequence 共有 n 个 word，那么我可以先对每一个 word 做 embedding 吧？就得到 n 个 embedding，然后我就可以用 embedding 代替 hidden state 来做 self-attention 了。所以 Q 这个矩阵里面装的就是全部的 word embedding，K、V 也是一样。




所以为什么管 Q 叫query？就是你每次拿一个 word embedding，去“查询”其和任意的 word embedding 的 match 程度（也就是 attention 的大小），你一共要做 n 轮这样的操作。 




我们记 word embedding 的 dimension 为 dmodel ，所以 Q 的 shape 就是 n*dmodel， K、V 也是一样，第 i 个 word 的 embedding 为 vi，所以该 word 的 attention 应为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loL5tGT7P0icDLoYcWicZsIBQzAtFn5dha8x3zYf42pSSCC2xAb1dQjHsg/640)




 那同时做全部 word 的 attention，则是：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7lo3ibqyiaJk89jt37GmCticMP2ZQAmEiaZeSVhicVfN4WTDDXEJZltBcwSz5A/640)




scaled dot-product attention 基本就是这样了。基于 RNN 的传统 encoder 在每个时刻会有输入和输出，而现在 encoder 由于抛弃了 RNN 序列模型，所以可以一下子把序列的全部内容输进去，来一次 self-attention 的自嗨。




理解了 scaled dot-product attention 之后，multi-head attention 就好理解了，因为就是 scaled dot-product attention 的 stacking。




先把 Q、K、V 做 linear transformation，然后对新生成的 Q'、K'、V' 算 attention，重复这样的操作 h 次，然后把 h 次的结果做 concat，最后再做一次 linear transformation，就是 multi-head attention 这个小 block 的输出了。 




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loicib9tWtMEcRjDzhELOQBgTIe6AS8nmbAJWicic2QHk8ZxGLdQkj0miaUwQ/640)




以上介绍了 encoder 的 self-attention。decoder 中的 encoder-decoder attention 道理类似，可以理解为用 decoder 中的每个 vi 对 encoder 中的 vj 做一种交叉 attention。




decoder 中的 self-attention 也一样的道理，只是要注意一点，decoder 中你在用 vi 对 vj 做 attention 时，有一些 pair 是不合法的。原因在于，虽然 encoder 阶段你可以把序列的全部 word 一次全输入进去，但是 decoder 阶段却并不总是可以，想象一下你在做 inference，decoder 的产出还是按从左至右的顺序，所以你的 vi 是没机会和 vj ( j>i ) 做 attention 的。




那怎么将这一点体现在 attention 的计算中呢？文中说只需要令 score(vi,vj)=-∞ 即可。为何？因为这样的话：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnibstvlzgKdFfnJiaIrUM7loQadzKmZeTg2zax7TibBkwj3hDpiaOA272LOmcEoyibYBEA5OWg4CD3L8g/640)




所以在计算 vi 的 self-attention 的时候，就能够把 vj 屏蔽掉。所以这个问题也就解决了。




模型的其他模块，诸如 position-wise feed-forward networks、position encoding、layer normalization、residual connection 等，相对容易理解，感兴趣的同学可以去看 paper，此处不再赘述。




# 总结




本文对 attention 的五种结构，即 hard attention、soft attention、global attention、local attention、self-attention 进行了具体分析。五种 attention 在计算复杂度、部署难度、模型效果上会有一定差异，实际中还需根据业务实际合理选择模型。



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)




**点击标题查看更多相关文章：**




- 
[自然语言处理中的自注意力机制](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488035&idx=1&sn=9d0568f58cd85d628fa60ddc33d266e9&chksm=96e9cda3a19e44b5e7ce784d08508ad6d03dcd93c96491dd660af4312b9c67b67457486475ea&scene=21#wechat_redirect)

- 
[从2017年顶会论文看Attention Model](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247485853&idx=1&sn=7e1dae6b690d718d17bebd8c37d2689b&chksm=96e9d61da19e5f0bafe59a848feccd340ce4fe1ff98cc5dfe24ea08ade644e176b45b8da64a7&scene=21#wechat_redirect)

- 
[一文读懂「Attention is All You Need」](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247486960&idx=1&sn=1b4b9d7ec7a9f40fa8a9df6b6f53bbfb&chksm=96e9d270a19e5b668875392da1d1aaa28ffd0af17d44f7ee81c2754c78cc35edf2e35be2c6a1&scene=21#wechat_redirect)

- 
[基于文档级问答任务的新注意力模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487494&idx=1&sn=c6ca1f1b8efea1347a00f7eddca5f004&chksm=96e9cf86a19e46906b421deec91d711f1e12db28e557b14c05f4c3ec604813c58b2e7635a7e8&scene=21#wechat_redirect)

- 
[层叠注意力模型 - 实现机器阅读的正确姿势](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247484683&idx=1&sn=0033e491852330a8662f37be325952b0&chksm=96e9da8ba19e539df182ad3f468416b99a412779222d6518a842dcdacc2ad415c89b9d7e5aea&scene=21#wechat_redirect)





[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488603&idx=2&sn=7320cb23efba3e7b5a381be83b7fe3ad&chksm=96e9cbdba19e42cd5840d3d51e86da4709b3d5273b2cf2512c32d84ab2b42ac4e7f13bf9ba63&scene=21#wechat_redirect)

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgmsvubgibQtWV5t7M3ETKt3bbXiaAothCErMicibic9QCUBpxkuibuht62MGcCTcLyAxqGrsUXbv254InDA/640?)

**▲**戳我查看招募详情




**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****作 者 招 募#**



****[让你的文字被很多很多人看到，喜欢我们不如加入我们](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487954&idx=1&sn=d247e5b99ecb2c37e85d962d7f93d7d7&chksm=96e9ce52a19e474457e04affae41dc6b6fe521154f95ae7122260b46ec91f55ae7c8fb472c3c&scene=21#wechat_redirect)****







******关于PaperWeekly******




PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/640?)

▽ 点击 | 阅读原文| 加入社区刷论文




