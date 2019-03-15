# 机器学习面试之Attention

> RNN做机器翻译有它自身的弱点，Attention正是为了克服这个弱点而出现的。所以，要理解Attention，就要搞明白两件事：
> \- RNN在做机器翻译时有什么弱点
> \- Attention是如何克服这个弱点的
> 本文试图从解答这两个问题的角度来理解Attention机制。如果你对RNN还不是很熟悉，建议看看这篇关于LSTM的文章<https://www.jianshu.com/p/d6714b732927>

# 1 RNN做机器翻译的经典思路 encoder-decoder

用RNN做机器翻译时，通常需要两个RNN网络，一个用来将接收待翻译语句，对其进行编码，最后输出一个vector，这个网络叫encoder。然后，该vector会作为输入，传给另一个RNN网络，该网络用来根据vector产生目标语言的翻译语句，这个网络叫做decoder。如下图所示：



![img](https://upload-images.jianshu.io/upload_images/1371984-80daa60fa850af8f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/335/format/webp)

encoder-decoder.png

上图中间的Context就是我们这里说的第一个RNN产生的vector。

# 2 encoder-decoder的缺点在哪里？

encoder-decoder最大的缺点是，encoder接收了不管多长的语句，最后输出的只是最后一个vector，当语句很长时，这个vector能否有效地表示该语句是很值得怀疑的。
如何解决这个问题呢？我们很自然会想到，第一个RNN其实在中间会产生很多输出，这些输出都被我们抛弃了，我们只用了最后的一个。如果能利用上中间的输出，兴许可以解决问题。Attention正是利用上了这些中间的输出。

# 3 Attention是如何利用中间的输出的

先上图，再来解释：



![img](https://upload-images.jianshu.io/upload_images/1371984-fc9258530977dd12.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)

屏幕快照 2018-09-01 下午7.20.56.png

上图中的A是我们的encoder， B是我们的decoder。
可以想象，A网络接收了一个四个字的句子，对每个字都产生了一个输出（这些输出都是一个vector），我们称其为s1，s2，s3，s4。

我们看上图的B网络，在第一个B产生的hidden state（称其为h1）除了传给下一个cell外，还传到了A网络，这里就是Attention发挥作用的地方，我们来看看发生了什么。

**第一步**：
h1 分别与s1，s2，s3，s4做点积，产生了四个数，称其为m1，m2，m3，m4（这些都是标量，不是向量了！）

**第二步**：
m1，m2，m3，m4 传到一个softmax层，产生一个概率分布a1，a2，a3， a4。

**第三步**：
将a1，a2，a3， a4 与s1，s2，s3，s4分别相乘，再相加，得到得到一个vector，称其为Attention vector。

**第四步**：

Attention vector 将作为输入传到B网络的第二个cell中，参与预测。

以上就是Attention机制的基本思想了。我们看到，Attention vector 实际上融合了s1，s2，s3，s4的信息，具体的融合是用一个概率分布来达到的，而这个概率分布又是通过B网络上一个cell的hidden state与s1，s2，s3，s4进行点乘得到的。
Attention vector实际上达到了让B网络聚焦于A网络输出的某一部分的作用。

# 4 Attention中产生概率分布的两种方法

在第3部分中，我们的概率分布来自于h与s的点积再做softmax，这只是最基本的方式。在实际中，我们可以有不同的方法来产生这个概率分布，每一种方法都代表了一种具体的Attention机制。

- 1 加法Attention
  在加法Attention中，我们不再让h与s做点积，而是做如下的运算：

  

  ![img]()

  屏幕快照 2018-09-01 下午7.51.04.png

va和Wa都是可以训练的参数。h与s之间的分号表示将二者接到一起产生一个更长的vector。这样产生的数再送往softmax层，进而产生一个概率分布。

当然，我们还可以这么做：



![img]()

屏幕快照 2018-09-01 下午7.55.26.png

这里只是不再把h与s接到一起而已，本质上没有什么区别的。

- 2 乘法Attention
  乘法Attention将h与s做如下的运算：

  

  ![img]()

  屏幕快照 2018-09-01 下午7.56.47.png

显然，乘法Attention的参数更少，效率自然也会更高一些。

# 5 Attention机制的扩展

Attention机制的核心在于对一个序列数据进行聚焦，这个聚焦是通过一个概率分布来实现的。这种机制其实有很强的普适性，可以用在各个方面。

比如，根据图片产生描述该图片的文字， 首先，图片会经过CNN进行特征的提取，提取的数据会输入到产生描述文字的RNN中，这里，我们可以引入Attention机制，让我们在产生下一个文字时，聚焦于我们正在描述的图片部位。

其次，在句子表示中，self Attention机制是成功扩展的Attention的范例。其基本原理如下：

假如我们用一个RNN读入了一个句子，产生了h1， h2，h3，h4四个hidden state。
为了得到该句子的摘要，我们可以这样做：
对每一个h计算一个分数：



![img](https://upload-images.jianshu.io/upload_images/1371984-4ef8e9b65fb2c0dc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/452/format/webp)

屏幕快照 2018-09-01 下午8.24.04.png

四个h共产生了4个分数，将这四个分数送入一个softmax层，产生一个概率分布，根据这个概率分布对四个h进行加和，得到句子摘要的第一个vector。如下图所示：



![img](https://upload-images.jianshu.io/upload_images/1371984-2b8f2cefebfa958a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/530/format/webp)

屏幕快照 2018-09-01 下午8.26.44.png

为了得到更多的vector，我们可以把上面图中的小写va换成一个矩阵，然后，我们的a也就变成了多个概率分布组成的矩阵，每个概率分布都可以用来与h进行加和产生一个vector，这样我们就产生了摘要的多个vector，如下图所示：



![img](https://upload-images.jianshu.io/upload_images/1371984-655ee59d0e4d4e72.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/590/format/webp)

屏幕快照 2018-09-01 下午8.28.06.png

# 6 无总结不进步

通过以上的内容，我们了解到，Attention机制最初用来克服RNN做机器翻译时的缺点，然后，人们发现，Attention机制具有广泛的适用性，于是它又被扩展到了产生图片描述，做句子摘要等任务上。
我们也清楚了，不同的Attention机制的核心区别在于产生概率分布的方法不同。