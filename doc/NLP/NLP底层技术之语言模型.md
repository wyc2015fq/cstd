# NLP底层技术之语言模型

2018年03月01日 20:46:04 [Vico_Men](https://me.csdn.net/qq_28031525) 阅读数：552



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/qq_28031525/article/details/79417164

　　本文结合cs224n:Natural Language Processing with Deep Learning的Lecture 8、Lecture 9内容，从语言模型（Language Model）讲到N-Gram LM（N-Gram Language Model）再到RNN-LM（RNN-Language Model）。



- - - [Language Model　　](https://blog.csdn.net/qq_28031525/article/details/79417164#language-model)
    - [N-Gram Language Model](https://blog.csdn.net/qq_28031525/article/details/79417164#n-gram-language-model)
    - RNN Language Model
      - [Perplexity](https://blog.csdn.net/qq_28031525/article/details/79417164#perplexity)
    - Smoothing
      - [Linear interpolation](https://blog.csdn.net/qq_28031525/article/details/79417164#1)



### Language Model　　

　　关于语言模型有两种说法，一种认为语言模型是计算某个句子概率的模型（A language model computes a probability for a sequence of words: p(w1,w2,w3...wn))p(w1,w2,w3...wn))，概率计算一般基于马尔科夫假设。 
　　在cs224课程中，语言模型被认为是用来预测下一个单词。 
　　![这里写图片描述](https://img-blog.csdn.net/20180301200602884?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgwMzE1MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
　　就笔者看来，这两种说法也具有统一性，因为：　
$$
p(w_{1},w_{2},w_{3}...w_{n})=p(w_{1})p(w_{2}|w_{1})p(w_{3}|w_{2},w_{1})...p(w_{n}|w_{1},w_{2},w_{3},..,w_{n})
$$
在计算整个句子的概率的时候，也需具体到$p(w_{n}|w_{1},w_{2},w_{3},..,w_{n})$的计算，这时就是在整个词表中用来预测下一个单词的概率。

 

　日常生活中，我们在如输入法和搜索引擎中，都用到了语言模型。语言模型也作为自然语言理解的基础任务。

 





### N-Gram Language Model

![这里写图片描述](https://img-blog.csdn.net/2018030119400216?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgwMzE1MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
N-gram存在的问题 
\1. Sparsity Problem（稀疏性问题）——可通过数据平滑技术如[线性插值](https://blog.csdn.net/qq_28031525/article/details/79417164#1)、Discounting Methods或者back-off方法解决： 
![这里写图片描述](https://img-blog.csdn.net/20180301194116453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgwMzE1MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
\2. Gigantic RAM requirement（参数空间过大），随着n的增大，存储空间需求也变大。 
![这里写图片描述](https://img-blog.csdn.net/20180301194201613?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgwMzE1MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### RNN Language Model

　　利用RNN对序列建模，复用不同时刻的线性非线性单元和权值。理论上之前所有的单词都会影响到当前的值。 
　　所需内存只与序列长度有关。 
　　![这里写图片描述](https://img-blog.csdn.net/20180301194356282?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgwMzE1MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
　　x(t) is the word vector at the t time step. h(0) is some initialization vector at time step 0. The size of the output y(t) is |V|,which is the length of the vocabulary（整个词表|V|上的概率分布)。 
　　![这里写图片描述](https://img-blog.csdn.net/20180301194438340?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgwMzE1MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
　　RNN语言模型的损失函数 
　　平均所有时刻上的损失，每个时刻的损失通过交叉熵损失函数定义。 
　　![这里写图片描述](https://img-blog.csdn.net/2018030119452260?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgwMzE1MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
　　![这里写图片描述](https://img-blog.csdn.net/20180301194559312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgwMzE1MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### Perplexity

　　使用交叉熵损失函数定义困惑度，越小越好。 
　　![这里写图片描述](https://img-blog.csdn.net/20180301194632361?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgwMzE1MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
　　RNN-LM的优缺点如下，其中RNN难以获取很多时间步之前的信息源自于RNN中的梯度弥散（gradient vanishing），将会在下一篇博客中介绍。 
　　![这里写图片描述](https://img-blog.csdn.net/20180301194702185?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgwMzE1MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### Smoothing

#### Linear interpolation

　　这里线性插值（Linear interpolation）通过trigram model介绍，trigram、bigram和unigram的最大似然估计如下： 
$$
q_{ML}(w|u,v)=\frac{c(w,u,v)}{c(u,v)}\\
q_{ML}(w|v)=\frac{c(w,v)}{c(v)}\\
q_{ML}(w)=\frac{c(w)}{c()}
$$
　　对上述符号进行介绍，c(w) 表示在训练语料库中单词w出现的次数，c() 表示训练语料库中单词出现的所有次数。 
　　其中trigram、bigram和unigram都有不同的优缺点，unigram不会遇到分子或者分母为零的情况，但是unigram估计完全忽略了上下文信息，因而抛弃了有用的信息，而trigram则会遇到分子或者分母为0的情况，bigram在这两种极端情况之间。 
　　 
　　线性插值则是利用这三种估计，通过定义trigram： 
$$
q_{ML}(w|u,v)=\lambda _{1}q_{ML}(w|u,v)+\lambda _{2}q_{ML}(w|v)+\lambda _{3}q_{ML}(w)
$$
　　其中λ1、λ2、λ3是三个参数，且满足： 
$$
\lambda _{1}\geq 0, \lambda _{2}\geq 0, \lambda _{3}\geq 0\\
\lambda _{1}+\lambda_{2}+\lambda _{3}=1
$$
　　因此是对这三种估计的加权平均。

