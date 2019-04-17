# word2vec - Keith - CSDN博客





2016年11月18日 11:03:42[ke1th](https://me.csdn.net/u012436149)阅读数：1141标签：[word																[深度学习																[机器学习](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)
个人分类：[MachineLearning																[NLP																[deeplearning](https://blog.csdn.net/u012436149/article/category/6416551)




# word2vec

## 为什么要进行embedding

`word2vec`就是对`word`进行`embedding`

首先,我们知道,在`机器学习`和`深度学习`中,对`word`的最简单的表示就是使用`one-hot`([0,0,1,0,0…..]来表示一个`word`). 但是用`one-hot`表示一个`word`的话,会有一些弊端:从向量中无法看出`word`之间的关系($(w^{word_a})^Tw^{word_b}=0$),而且向量也太稀疏. 所以一些人就想着能否用更小的向量来表示一个`word`,希望这些向量能够承载一些`语法`和`语义`上的信息, 这就产生了`word2vec`

## Language Model(Unigrams, Bigrams, Trigrams..etc)

`language model` 对`序列`的概率建模 
**Unigram**: 

假设句子中,各个`word`是独立的 
$ P(w_1,w_2,w_3..,w_n) = \prod_{i=1}^{n}P(w_i) $
**Bigram**

假设句子中,每个`word`只和之前的一个`word`有关系 
$ P(w_1,w_2,w_3..,w_n) = \prod_{i=2}^{n}P(w_i|w_{i-1}) $
**Trigram**

假设句子中,每个`word`和前两个`word`有关系 
$ P(w_1,w_2,w_3..,w_n) = \prod_{i=1}^{n}P(w_i|w_{i-1},w_{i-2}) $
上面的模型都基于很强的假设,而实际上,句子中的每个`word`,是和整个句子有关系的,不仅仅只是考虑前一个或前两个

## Continuous Bags of Words Model (CBOW)

这个模型是上面几种模型的扩展.`CBOW`不是简单的只考虑前一个词或前两个词,它是考虑了单词的上下文(`context`).在`CBOW`,我们的目标是$max P(w|context(w))$. 
![这里写图片描述](https://img-blog.csdn.net/20161118110536056)

首先,模型的输入(context)是`one-hot`’s ,模型的输出(w)是`one-hot`,$one\_hot\in R^{|V|}$,这个是已知的.我们要创建两个矩阵$E=Matrix(embedding)\in R^{|V|* {embedding\_size}}$ ,  $P=Matrix(projection)\in R^{{embedding\_size}* |V|}$, 这两个矩阵是需要训练的.$|V|$是字典的大小,$embedding\_size$是任意值(代表你想把onehot压缩成几维表示). 


$context = \begin{matrix}0 & 0 & 0 &\cdots & 1 & 0 \\1 & 0 & 0 &\cdots & 0 & 0 \\\vdots                    \\0 & 0 & 1 &\cdots & 0 & 0 \\0 & 0 & 0 &\cdots & 0 & 1\end{matrix}\in R^{|context(w)|* |V|}$


$Embed = \begin{matrix}0.1 & 0.3 & 0.2 &\cdots & 0.1 & 0.3 \\0.1 & 0.1 & 0.0 &\cdots & 0.6 & 0.5 \\0.1 & 0.3 & 0.2 &\cdots & 0.1 & 0.3 \\0.1 & 0.1 & 0.0 &\cdots & 0.6 & 0.5 \\\vdots                    \\0.1 & 0.3 & 0.2 &\cdots & 0.1 & 0.3 \\0.1 & 0.1 & 0.0 &\cdots & 0.6 & 0.5 \\0.1 & 0.3 & 0.2 &\cdots & 0.1 & 0.3 \\0.1 & 0.1 & 0.0 &\cdots & 0.6 & 0.5\end{matrix}\in R^{|V|* embedding\_size}$


$Proj = \begin{matrix}0.1 & 0.3 & 0.2 &\cdots & 0.1 & 0.3 \\0.1 & 0.1 & 0.0 &\cdots & 0.6 & 0.5 \\0.1 & 0.3 & 0.2 &\cdots & 0.1 & 0.3 \\0.1 & 0.1 & 0.0 &\cdots & 0.6 & 0.5 \\\vdots                    \\0.1 & 0.3 & 0.2 &\cdots & 0.1 & 0.3 \\0.1 & 0.1 & 0.0 &\cdots & 0.6 & 0.5 \\0.1 & 0.3 & 0.2 &\cdots & 0.1 & 0.3 \\0.1 & 0.1 & 0.0 &\cdots & 0.6 & 0.5\end{matrix}\in R^{embedding\_size*|V|}$
**模型运作步骤:**

(1) 生成`context`的`one-hot`矩阵 

(2) 计算出`context`的`embedding matrix`, $context * E \in R^{context(w)* |V|}$

(3) 将获取的`matrix`平均, $\hat v=reduce_mean(context*E, 1)$

(4) 生成评分向量 $z=reduce\_mean(context*E,1)* P \in R^{|V|}$

(5) 将评分向量转成概率分布 $\hat y = softmax(z)$
$\hat y$ 的分布和实际分布$y$越相近,则模型学习的越好, 如何描述两个分布的相似性呢?借用信息论中的交叉熵$H(\hat y,y)=-\sum_{i=1}^{|V|}y_ilog(\hat y_i)$作为目标函数, 然后使用梯度下降来更新参数. 


$\begin{aligned}minimize\space J &= -logP(w_c|w_{c-m},w_{c-m+1},...,w_{c-1},w_{c+1},...,w_{c+m})\\&= -logP(proj_c|\hat v)\\&= -log\frac{exp(proj_c\hat v^T)}{\sum_{i=1}^{|V|}proj_i\hat v^T}\end{aligned} $
`m`:窗口大小
## skip-gram Model

`skip-gram model`和`CBOW`结构相反, `CBOW`输入上下文, 输出中间的`word`.`skip-gram`输入中间的`word`,输出上下文. 
![这里写图片描述](https://img-blog.csdn.net/20161118110557726)

需要学习的依旧是两个矩阵, $Embed\in R^{|V|* embedding\_size}$和$Proj\in R^{embedding\_size* |V|}$


$\begin{aligned}minimize\space J&=-logP(w_{c-m},w_{c-m+1},...,w_{c-1},w_{c+1},...,w_{c+m}|w_c)\\&= -log \prod_{i=c-m,i \neq c}^{c+m}P(w_i|w_c)\\&= -log\prod_{i=c-m,i \neq c}^{c+m}P(proj_i|embed_c)\end{aligned}$
## Negtive Sampling

看公式$\sum_{i=1}^{|V|}proj_i\hat v^T$,如果$|V|$很大,那么运算量是相当大的,为了减少运算量,就提出了`Negtive Sampling`. 
`Negtive Sampling`基于`skip-gram model`. 

考虑一个(w,c)对,其中`w`是中心单词,`c`为`w`上下文中的一个单词,$P(D=1|w,c,\theta)$表示`c`是`w`上下文中单词的概率,$P(D=0|w,c,\theta)$表示`c`不是`w`上下文中单词的概率.我们先对$P(D=1|w,c,\theta)$进行建模: 


$P(D=1|w,c,\theta)=\frac{1}{1+exp(-(proj_c)(embed_w^T))} \space\space embed_w\in R^{embedding\_size}$

相比`CBOW`和`skip-gram`,`Negtive Sampling`思想是,如果`c`是`w`的上下文中的单词,就最大$P(D=1|w,c,\theta)$,如果不是,就最大化$P(D=0|w,c,\theta)$,$\theta$就是$Embed, Proj$


$\begin{aligned}\theta &= argmax_{\theta} \prod_{(w,c)\in D}P(D=1|w,c,\theta)\prod_{(w,c)\notin D}P(D=0|w,c,\theta) \\&= argmax_{\theta} \sum_{(w,c)\in D}logP(D=1|w,c,\theta)+\sum_{(w,c)\notin D}log(1-P(D=0|w,c,\theta)) \\&= argmax_{\theta}\sum_{(w,c)\in D}log\frac{1}{1+exp(-(proj_c)(embed_w^T))}+\sum_{(w,c)\notin D}log(1-\frac{1}{1+exp(-(proj_c)(embed_w^T)))}\\\end{aligned}$

这样运算量就被减小了. 

x训练之后,对$Embed$和$Proj$有多种处理方式: 

(1) 求和 

(2)平均 

(3)连接起来 

问题: 

(1): `Negtive Sampling`只考虑了上下文关系,没有考虑单词之间的顺序关系,如果考虑进去的话,效果会不会更好? 

(2): `word2vec`,优化的都是`proj_c`和`embed_w`的距离,让这两个向量尽量的近,这个代表了什么? 

(3):对于$Embed$,感觉更新的频率不够](https://blog.csdn.net/u012436149/article/category/6526896)](https://blog.csdn.net/u012436149/article/category/6504450)](https://so.csdn.net/so/search/s.do?q=深度学习&t=blog)](https://so.csdn.net/so/search/s.do?q=word&t=blog)




