# 深度学习基础--注意力机制(attention) - wydbyxr的博客 - CSDN博客
2018年11月12日 11:11:48[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：215
# 注意力机制(attention)
## 在机器翻译中的应用
  在Deep learning火起来后，最常见的建模方式是用Recurrent Neural Networks (RNN) 将上下文"“编码”"，然后再"“解码”"成目标文本。以机器翻译为例。
  Google最近的论文中，用一个 RNN encoder读入context， 得到一个context vector（RNN的最后一个hidden state）；然后另一个RNN decoder以这个hidden state为起始state，依次生成target的每一个单词。But,这种做法的缺点是，无论之前的context有多长，包含多少信息量，最终都要被压缩成一个几百维的vector。这意味着context越大，最终的state vector会丢失越多的信息。
  输入sentence长度增加后，最终decoder翻译的结果会显著变差。事实上，因为context在输入时已知，一个模型完全可以在decode的过程中利用context的全部信息，而不仅仅是最后一个state。Attention based model的核心思想就是如此。
## 具体做法
  首先，在生成target side的states时 ()，所有context vectors ()都会被当做输入。
  其次，并不是所有context都对下一个状态的生成产生影响。例如，当翻译英文文章的时候，我们要关注的是“当前翻译的那个部分”，而不是整篇文章。“Attention”的意思就是选择恰当的context并用它生成下一个状态。
  在大部分的论文中，Attention是一个权重vector（通常是softmax的输出），其维度等于context的长度。越大的权重代表对应位置的context越重要。不同论文对attention权重的计算方式不同，但其核心抛不开上述两点。
  注意力机制的实现流程：
  1）将 sentence embeeding 连接到到每个 location 的 image feature，
  2）然后通过一个 MLP 获得每个 location 的分值，
  3）然后再通过一个 softmax 获得 attention map，
  4）然后以后的 visual feature 就是每一个 location 的 feature 的加权平均。"
## 应用
  用于NLP，LSTM，captioning 任务等。
