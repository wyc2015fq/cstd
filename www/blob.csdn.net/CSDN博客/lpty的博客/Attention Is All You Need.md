
# Attention Is All You Need - lpty的博客 - CSDN博客

2019年01月07日 21:59:16[lpty](https://me.csdn.net/sinat_33741547)阅读数：94



# 引言
google 在2017年发布的一篇论文，即是transformer，原文地址：[Attention Is All You Need](https://arxiv.org/pdf/1706.03762.pdf)
总结前面的工作，更加正式的定义了attention。此前，无论是基于序列还是卷积的网络，大都是在decoder端引入attention，可以称之为decoder attention机制，这种机制主要用来处理的是对齐问题。
而google这篇论文，直接在encoder端就引入了attention机制，同时在decoder端也实现了同样的一套attention结构，这个部分可以理解为是对文本语义表征的提取。
传统的利用序列的模型，需要一步步的由前往后计算出序列的表征，这样就存在长距离的依赖问题，同时当前词语能够获取到上下文信息其实比较有限，毕竟类似RNN这样的模型，本质上属于马尔可夫模型。
或者利用卷积网络，通过层叠卷积层来扩大感受野，从而获取全文信息。但是文本的处理不同于图像，图像从点到线再到组合，需要关注的特征都是在局部扩展而来，而文本在表征语义时，需要获取的信息可能来自距离很远的另一个词，所以也存在问题。
相比以上两者，直接通过attention的方式获取全局语义，这种做法就来的更加有效率。
# 模型
transformer的整体模型如下图所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190107220443392.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
整体上保持了Encoder-Decoder的机制，与前面工作一致的是在Decoder端也attend了Encoder的输出作为输入，最后通过softmax归一化概率矩阵。
[
](https://img-blog.csdnimg.cn/20190107220443392.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
## Encoder and Decoder Stacks
[
](https://img-blog.csdnimg.cn/20190107220443392.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
### Encoder
[
](https://img-blog.csdnimg.cn/20190107220443392.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)Encoder由N个层堆叠而成，N=6，其中的每个层都拥有两个子层。第一个子层称为Multi-head self-attention，第二个子层称为Position-wise Feed-Forward Networks。
同时，在这两个子层中还加入残差网络和layer normalization，所以每个子层的可以描述为以下公式：
$LayerNorm(x + Sublayer(x))$
其中$x$为残差，$Sublayer(x)$即是通过子层得到的输出，两者的维度均为$d_{model} = 512$
### Decoder
Decoder与前面一样，也是由N=6个层堆叠而成，这里每个层都拥有三个子层。其中前两个层与Encoder类似，需要注意的是为了防止序列获取未来信息，所以在attention时对未来信息加上了掩码。第三个层则是attend了Encoder的输出，类似在前面工作中提出过的align方式，需要注意这里N=6个层都需要attend前面Encoder的输出，~~所以与Encoder的N=6个层是一一对应的~~attend的值为stack后的最终结果。
类似的，Decoder中也加入残差网络和layer normalization。
## Scaled Dot-Product Attention
在前面的工作中，一般将Attention作为一个序列化编码的结果进行处理，在本论文中，作者给出了一个更一般的模式：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190107223411950.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
对于attention，可以将其形容为Q(query)、K(key)、V(value)三者的某种函数表达，比如在论文[Neural Machine Translation by Jointly Learning to Align and Translate](https://blog.csdn.net/sinat_33741547/article/details/85099103)中，$Q=H_{t-1}$，$K=V=\{h_1,...,h_t\}$，其中$H$为decoder隐层输出，$h$为encoder隐层输出。
在具体的任务中，Q、K、V可以有不同的表达方式，一般化的公式如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190107224315307.png)
其中$Q∈R^{n*d_k}$，$K∈R^{m*d_k}$，$V∈R^{m*d_v}$，K，V是一一对应的关系，上面的公式可以理解为$n*d_k$序列编码成了$n*d_v$的序列。
在使用乘法注意力机制时，attention可以表示为Q与K的矩阵相乘，其中$\sqrt {d_k}$为归一化因子。作者提到，假如处以$\sqrt {d_k}$，在$d_k$维度较小时，加法注意力机制与乘法注意力机制的表现类似，随着$d_k$增大，加法注意力的表现会更好。
因此加上了这个因子，猜测可能的原因是$d_k$增大导致attention的数值过大进入了softmax函数的饱和区，梯度因此变小导致模型效果变差。
下面是作者对此的一些解释：
> To illustrate why the dot products get large, assume that the components of q and k are independent random variables with mean 0 and variance 1. Then their dot product,
$$
q · k =\sum^{d_k}_{i=1} q_ik_i
$$
> , has mean 0 and variance dk.
因此，每个分量除以$\sqrt {d_k}$可以让点乘的方差变为1。
## Multi-Head Attention
根据上述的一般化模型，作者提出了利用多层的attention对输入特征进行抽取，如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190107232648448.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
这样能达到更好的效果，每层的参数不共享，最终得到$h$层数个$d_v$维输入，将所有输出拼接起来做个一个线性映射就能得到最终输出，非常直接，公式如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019010723145691.png)
其中$W^{Q}_{i} ∈ R^{d_{model}×d_k}$，$W^{K}_{i} ∈ R^{d_{model}×d_k}$,$W^{V}_{i} ∈ R^{d_{model}×d_v}$，$W^{O}∈ R^{h_v×d_{model}}$。
.这里取$h=8，d_k = d_v = d_{model}/h = 64$，最终得到的结果与输入句子的维度一致，由此可以进行更深层的特征抽取。
从计算复杂度来看，由于各个channel各自独立，因为可以并行化处理，与单独的一层attention在时间上所需时间是很相近的。
## Self-Attention
前面对Attention的描述都是一般化的方式，但实际在本论文中用所提到的attention大部分都是self-attention，称为自注意力，如下：
$Y=MultiHead(X, X, X)$
即Q=K=V=X，在encoder端，X为输入序列，在decoder端X为输出序列，这两个自注意力机制在对输入或者输出序列进行语义表征时，都能使得当前位置能够获取到全局序列的信息（decoder端进行了掩码处理）。
## Position-wise Feed-Forward Networks
前面说到，在Encoder与Decoder的子层中，都存在一个全连接层，这个层与一般的全连接不一样，对于矩阵的每个位置都使用相同的参数分别进行计算，看起来更像是两个w=1的卷积，公式如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190107234321823.png)
上述公式简单描述便是进行两个线性变换和一个Relu激活，将输入$d_{model}=512$的矩阵转为$d_{ff}=2048$，之后再转为原始大小。
[
](https://img-blog.csdnimg.cn/20190107234321823.png)
## Embedding
[
](https://img-blog.csdnimg.cn/20190107234321823.png)
### Word Embedding
[
](https://img-blog.csdnimg.cn/20190107234321823.png)关于词的分布式表达这里与其他模型一致，采用$d_{model}=512$的维度将输入句子表达为$n*d_{model}$的矩阵。
[
](https://img-blog.csdnimg.cn/20190107234321823.png)
### Positional Embedding
[
](https://img-blog.csdnimg.cn/20190107234321823.png)从上面的模型可以看到，只使用word embedding并不同将序列信息包含到模型中。此前的RNN模型可以由时序包含，CNN模型可以有卷积的多次采样扩展得到，而在transformer中，目前来看只是一个词袋模型，假如将词序打乱，在结果上不会有任何区别，因此作者加入了位置信息的编码，如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190107235546382.png)
位置信息$PE∈R^{n*d_{model}}$，其中$pos$为位置，$i$为维度。
对于位置pos的某个维度i，可以由以下公式进行编码，
$f(pos, i)=pos/10000^{2_i/d_{model}}$
之后，将偶数维度i=0, 2, 4…用sin函数激活，奇数维度i=1, 3, 5…用cos函数激活。
而选用上述经验函数的原因在于：
$sin(a+b) = sin(a)cos(b) + cos(b)sin(a)$
其中b为一个固定的offset常数，因此pos+k的编码可以通过pos进行线性表示，这样不仅能够表达绝对位置信息，在自然语言中，相对信息也很重要，用这种方式包含进来是很有必要的。
同时，这种方式也能够有效的扩展长序列的位置信息，即使在训练中没有出现过这种长度。
除了这种定义下来的位置信息，在以前的工作中也存在过使用位置信息的方式，但一般来说都是直接通过训练生成的向量。作者对比了两种方式，发现两者效果很接近，所以直接使用手工定义的位置信息。
最后将Word Embedding与Positional Embedding相加，形成输入的矩阵X。
## Why Self-Attention
对于使用self-attention的优势，作者总结如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190108001219877.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
#### Complexity
由上图可见，在复杂度上，对self-attention影响较大的是序列长度，而像RNN或者CNN都与向量维度更相关，对于一般的任务而言，向量维度都是远大于序列长度。
但是对于某种任务，比如阅读理解，这里可能出现序列的长度大于向量维度，所以作者提出了restricted-self-attention，假设句子中词语只与某个窗口大小r的长度有关。
#### Sequential Operations
在一般的RNN模型中，每一步的输入都依赖于上一步的输出，所以无法进行并行化处理。而CNN与self-attention都不依赖前词信息，所以在并行化方面有更大的优势。
#### Maximum Path
因为self-attention的每一个词都是直接与全局序列直接相连，所以在这里Path长度都是1，这样对处理长距离依赖方面有更大的好处。CNN模型需要通过更高层次的卷积扩大感受野，其Path为$log_k(n)$;而RNN模型则需要一步一步计算，其最大距离为n，因此RNN模型的长距离依赖问题也更严重。
# 结果
作者在WMT 2014 English-to-German及WMT 2014 English-to-French两个翻译任务上进行实验，均取得了SOTA的结果：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190108002706483.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
# 结论
1、提出了更一般化的attention机制，及相对应的self-attention，这种方式的attention抛弃了传统的RNN及CNN，在工程化方面有更好的应用，同时其结果也相当不错。
2、模型机制在某些方面与CNN很类似，比如Position-wise Feed-Forward Networks就与一维卷积类似，同时Multi Head也可以说成是CNN的channel形式
3、完全手工定义的position信息是否能够完全表达其实际意义呢？
4、self-attention每个词都顾及到了全局信息，但实际上语义理解上，并不是每个词都需要这么长的序列。
# 引用
1、Neural Machine Translation by Jointly Learning to Align and Translate
2、Attention Is All You Need

