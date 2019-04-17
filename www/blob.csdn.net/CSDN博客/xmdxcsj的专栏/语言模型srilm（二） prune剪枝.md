# 语言模型srilm（二） prune剪枝 - xmdxcsj的专栏 - CSDN博客





2015年12月15日 22:29:58[xmucas](https://me.csdn.net/xmdxcsj)阅读数：4529








## 为什么引入剪枝

传统的N-gram backoff模型，提升性能的两条主要路径是增加阶数和增加语料，两者带来的共同副作用是增加了模型的大小，进而增加了语音识别解码器的内存占用。为了减少模型的大小，同时保证性能最大化，引入了prune。 

为了实现剪枝选取的准则需要满足以下三个条件：
- 
soundness 

The criterion should optimize some wellunderstood information-theoretic measure of language 

model quality.- 
efficiency 

高效、快速

- 
self-containedness 

从实用性角度考虑，只有当前的语言模型可用，没有其他多余的信息可用


srilm采用了基于相对熵的剪枝方法（N-gram pruning based on relative entropy）。

## 基于相对熵的剪枝

剪枝的主要目的是为了删除已有ngram，同时保证未删除的ngram不变，还需要重新计算回退概率。 

那么怎么样衡量剪枝以后语言模型性能的改变呢？ 
*一个想法*就是去最小化剪枝前后两个模型之间概率分布的距离。自然的，我们会选择相对熵或者KL距离，定义参考[链接](http://blog.csdn.net/xmdxcsj/article/details/50051579): 


$D(p||p')=-\sum_{w_i,h_j}p(w_i,h_j)[logp'(w_i|h_j)-logp(w_i|h_j)]$

其中$p$表示裁剪前语言模型的概率，$p'$表示裁剪后的模型。 

我们不可能遍历所有需要裁剪的ngram的集合（指数型的复杂度），从可操作性考虑，这里需要假设所以ngram对相对熵的影响都是独立的，有了这个假设，我们就可以计算删除每一条ngram对应的相对熵，然后按照大小进行排序，删除那些相对熵最小（剪掉以后对原语言模型影响最小）的ngram。 
*另外一个想法*就是根据模型裁剪前后ppl值的相对变化来衡量裁剪对模型的影响。 

原始模型的ppl值： 


$PPL=e^{-\sum_{h,w}p(h,w)logp(w|h)}$

裁剪后模型的ppl值： 


$PPL'=e^{-\sum_{h,w}p(h,w)logp'(w|h)}$

所以困惑度的相对变化可以表示为： 


$\frac{PPL'-PPL}{PPL}=e^{D(p||p')}-1$
所以，裁剪语言模型的流程如下： 

1. 给定一个困惑度相对变化的门限threshold 

2. 计算删除单独一条ngram，模型困惑度的相对变化 

3. 挑选出那些低于门限值的ngram，删除，然后重新计算回退权重
## 基于相对熵剪枝的计算方法

如果删除一条ngram(h,w)，h表示历史词，w表示当前词，h’表示历史词去掉第一个词，那么将会带来两方面的影响：
- 
h的回退概率$\alpha(h)$，将变为$\alpha'(h)$；与此同时，所有历史是h对应的回退概率将会改变，统一将这些ngram表示为$BO(w_i,h)$

- 
$p(w|h)$将变为$p'(w|h)=\alpha'(h)p(w|h')$


对于历史词不是h的所有ngram，概率值不变，所以这里只需要考虑： 

1. 历史词$h_i$： 
$h$

2. 当前词$w_i$

一个是$w$；另外一个是涉及回退概率h对应的w（$BO(w_i,h)$） 

所以相对熵可以写成： 


$D(p||p')=-\sum_{w_i,h_j}p(w_i,h_j)[logp'(w_i|h_j)-logp(w_i|h_j)]$


$=-p(w,h)[logp'(w|h)-logp(w|h)]-\sum_{w_i\in BO(w_i,h)}p(w_i,h)[logp'(w_i|h)-logp(w_i|h)]$


$=-p(h)\{p(w|h)[logp'(w|h)-logp(w|h)]-p(w_i|h)\sum_{w_i\in BO(w_i,h)}p(w_i,h)[logp'(w_i|h)-logp(w_i|h)]\}$

计算上式的后面一项，需要遍历词典，对于词典很大的语言模型，计算复杂度较高。可以考虑回退概率： 


$p'(w|h)=\alpha'(h)p(w|h')$


$logp'(w_i|h)-logp(w_i|h)=log\alpha'(h)+logp(w_i|h')-log\alpha(h)-logp(w_i|h')=log\alpha'(h)-log\alpha(h)$

所以，相对熵将变为： 


$D(p||p')=-p(h)\{p(w|h)[logp(w|h')+log\alpha'(h)-logp(w|h)]]+$


$[log\alpha'(h)-log\alpha(h)]\sum_{w_i\in BO(w_i,h)}p(w_i,h)$

到现在，未知量包括三项：$p(h)$、$\alpha'(h)$和BO对应的累加项。 

对于第一项，可以根据条件概率依次展开算出 

对于第二项，回退概率的计算公式如下： 


$\alpha(h)=\frac{1-\sum_{w_i\notin BO(w_i,h)}p(w_i|h)}{1-\sum_{w_i\notin BO(w_i,h)}p(w_i|h')}$

类似于$\alpha(h)$，$\alpha'(h)$只是在分子和分母分别多加上一个$p(w|h)$和$p(w|h')$，每一个词历史h只需要计算一次。 

对于第三项，也是每一个h只需要计算一次，如下 


$\sum_{w_i\in BO(w_i,h)}p(w_i,h)=1-\sum_{w_i\notin BO(w_i,h)}p(w_i,h)$
## 参考文献

[Entropy-based Pruning of Backoff Language Models](https://www.sri.com/sites/default/files/publications/entropy-based_pruning_of_backoff_language_models.pdf)








