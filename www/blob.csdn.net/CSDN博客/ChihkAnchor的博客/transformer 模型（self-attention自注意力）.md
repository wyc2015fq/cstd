# transformer 模型（self-attention自注意力） - ChihkAnchor的博客 - CSDN博客





2019年01月08日 19:57:21[Chihk-Anchor](https://me.csdn.net/weixin_40871455)阅读数：818








transformer模型在《Attention is all you need》论文中提出

这篇论文主要亮点在于：1）不同于以往主流机器翻译使用基于RNN的seq2seq模型框架，该论文用attention机制代替了RNN搭建了整个模型框架。2）提出了多头注意力（Multi-headed attention）机制方法，在编码器和解码器中大量的使用了多头自注意力机制（Multi-headed self-attention）。3）在WMT2014语料中的英德和英法任务上取得了先进结果，并且训练速度比主流模型更快。

模型的整体结构如下图，还是由编码器和解码器组成，在编码器的一个网络块中，由一个多头attention子层和一个前馈神经网络子层组成，整个编码器栈式搭建了N个块。解码器类似于编码器，只是解码器的一个网络块中多了一个多头attention层。为了更好的优化深度网络，整个网络使用了残差连接和对层进行了规范化（Add&Norm）

我们在这里设定输入的句子为（x1,x2,x3,......xn）；经过input embedding之后的序列为Z = （z1,z2,z3,......zn）；经过encoder编码后输入decoder，然后输出的序列（y1,y2,y3,.......yn）

![](https://img-blog.csdnimg.cn/20190108170056422.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

**Encoder**: 这里面有 N=6 个 一样的layers, 每一层包含了两个sub-layers. 第一个sublayer 就是多头注意力层（multi-head attention layer） 然后是一个简单的全连接层。 这里还有一个残差连接 （residual connection), 在这个基础上， 还有一个layer norm.  这里的注意力层会在下文详细解释。

**Decoder**: 这里同样是有六个一样的Layer是，但是这里的layer 和encoder 不一样， 这里的layer 包含了三个sub-layers,  其中有 一个self-attention layer, encoder-decoder attention layer ，最后是一个全连接层。 前两个sub-layer 都是基于multi-head attention layer.  这里有个特别点就是masking,  masking 的作用就是防止在训练的时候 使用未来的输出的单词。 比如训练时， 第一个单词是不能参考第二个单词的生成结果的。 Masking就会把这个信息变成0， 用来保证预测位置 i 的信息只能参考比 i 小的输出。

### Multi-Head Attention

在介绍多头attention之前，我们先看一下论文中提到的放缩点积attention（scaled dot-Product attention）。对比attention的一般形式，scaled dot-Product attention就是我们常用的使用点积进行相似度计算的attention，只是多除了一个（为*K*的维度）起到调节作用，使得内积不至于太大

![](https://img-blog.csdnimg.cn/20190108172021359.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

多头attention（Multi-head attention）结构如下图，Query，Key，Value首先进过一个线性变换，然后输入到放缩点积attention (如上图)，注意这里要做h次，其实也就是所谓的多头，每一次算一个头。而且每次Q，K，V进行线性变换的参数W是不一样的。在input embedding结束后得到的Z直接赋值给Q,K,V，即Q=K=V=Z

每个头的计算方式：![](https://img-blog.csdnimg.cn/20190108174047857.png) 这里的W是三个不同的待训练的权值参数矩阵，由于有h个头，所以有h个维度，每个维度又是一个向量，![](https://img-blog.csdnimg.cn/20190108173908571.png)指的是W矩阵的第i个维度，是一个向量。

然后将h次的放缩点积attention结果进行拼接（下图的黄色concat），再进行一次线性变换得到的值作为多头attention的结果。可以看到，google提出来的多头attention的不同之处在于进行了h次计算而不仅仅算一次，论文中说到这样的好处是可以允许模型在不同的表示子空间里学习到相关的信息 

![](https://img-blog.csdnimg.cn/2019010817335051.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

回看图一，encoder有N个子层叠加而成，encoder左边有个N，说明有n层，我们的论文中取N=6；对于encoder的每个子层中，进行multi-head attention计算后会进行残差连接以及归一化，然后送入全连接层，进行非线性变换，得到子层的输出，所用的激活函数为ReLU，注：这里在encoder的第一层会对Z拷贝head*3 份，分别作为K,V,Q的输入。然后在将上述的结构堆叠N层，就可以得到最终的Encoder的输出。

再次回看图一，我们发现Decoder也是由N层组成，将视野切换回单层的decoder中，我们发现decoder多了一个multi-head attention，该层的输入的Q为self-attention的输出，K和V来自Encoder（对应层）的输出，特别注意是对应层，什么意思呢，encoder的第一层的输出作为decoder第一层的输出，将图一立起来，从上往下，他们的堆叠关系如下图：

![](https://img-blog.csdnimg.cn/2019010818070941.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

然后我们来看一些decoder中的masked multi head attention，所谓masked就是输出层在计算第j个词与其他词的attention时，只考虑j前面的词，因为j后面的词我们不知道是什么，我们需要预测后面的词，如果不掩盖的话，训练的时候相当于模型能看到后面的答案，而我们要求的是，decoder给定一个开始标志后，一步一步向后计算接下来的单词，所以需要mask

### 位置信息embedding

这里指的是 input embedding、output embedding 之后的有一个positional encoding

![](https://img-blog.csdnimg.cn/2019010819413884.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

这里使用了两个函数， sin, cos， pos 用来表示单词的位置信息， 比如 第一个单词， 第二个单词什么的。 而 i  用来表达dimension。 论文中， d_{model} 是512， 那 i 应该是 0 到255.  这里呢， 为了好说明， 如果2i= d_{model}, PE 的函数就是sin（pos/10000）, 那它的波长就是10000*2pi,  如果i =0, 那么他的波长 就是2pi.  这样的sin, cosin 的函数 是可以通过线性关系互相表达的。

### Auto recursive decoding

![](https://img-blog.csdnimg.cn/20190108195309101.gif)

这个图的encoding  过程， 主要是self attention,  有三层。 接下来是decoding过程， 也是有三层， 第一个预测结果 <start> 符号， 是完全通过encoding 里的attention vector 做出的决策。 而第二个预测结果Je, 是基于encoding attention vector &  <start> attention vector  做出的决策。按照这个逻辑，新翻译的单词不仅仅依赖 encoding attention vector， 也依赖过去翻译好的单词的attention vector。 随着翻译出来的句子越来越多，翻译下一个单词的运算量也就会相应增加。 如果详细分析，复杂度是 （n^2d）, 其中n是翻译句子的长度，d是word vector 的维度。





