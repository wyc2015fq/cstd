
# 使用PaddleFluid和TensorFlow训练RNN语言模型 - Paper weekly - CSDN博客


2018年07月04日 08:16:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：563


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

**专栏介绍：**Paddle Fluid 是用来让用户像 PyTorch 和 Tensorflow Eager Execution 一样执行程序。在这些系统中，不再有模型这个概念，应用也不再包含一个用于描述 Operator 图或者一系列层的符号描述，而是像通用程序那样描述训练或者预测的过程。

本专栏将推出一系列技术文章，从框架的概念、使用上对比分析 TensorFlow 和 Paddle Fluid，为对 PaddlePaddle 感兴趣的同学提供一些指导。
在图像领域，最流行的 building block 大多以卷积网络为主。上一篇我们介绍了[如何在 PaddleFluid 和 TensorFlow 上训练图像分类任务](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490048&idx=1&sn=6c5b5ec85b2972bd476317d488da2dcd&chksm=96e9c580a19e4c965b860922c5587c433d280a80a4208f3a6b9a55f1c360d9a86dc4e6dd15a6&scene=21#wechat_redirect)。卷积网络本质上依然是一个前馈网络，在神经网络基本单元中循环神经网络是建模序列问题最有力的工具， 有着非常重要的价值。自然语言天生是一个序列，在自然语言处理领域（Nature Language Processing，NLP）中，许多经典模型都基于循环神经网络单元。可以说自然语言处理领域是 RNN 的天下。

**这一篇以 NLP 领域的 RNN 语言模型（RNN Language Model，RNN LM）为实验任务，对比如何使用 PaddleFluid 和 TensorFlow 两个平台实现序列模型**。 这一篇中我们会看到 PaddleFluid 和 TensorFlow 在处理序列输入时有着较大的差异：PaddleFluid 默认支持非填充的 RNN 单元，在如何组织 mini-batch 数据提供序列输入上也简化很多。

# 如何使用代码

本篇文章配套有完整可运行的代码，**请从随时从 github****[1]****上获取最新代码**。代码包括以下几个文件：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnMlogCKOIRozsnsuJmXB3XNHRnzI949q0g5icZsws6DibRhmJSbAu8Qy4yibbD4aicWZXDhIicVEZu0Ug/640)

**注意：在运行模型训练之前，请首先进入 data 文件夹，在终端运行****sh download.sh****下载训练数据。**

在终端运行以下命令便可以使用默认结构和默认参数运行 PaddleFluid 训练 RNN LM。


```python
python rnnlm_fluid.py
```

在终端运行以下命令便可以使用默认结构和默认参数运行 TensorFlow 训练 RNN LM。

```python
python rnnlm_tensorflow.py
```

# 背景介绍

**one-hot和词向量表示法**

计算机如何表示语言是处理 NLP 任务的首要问题。这里介绍将会使用到的 one-hot 和词向量表示法。

**one-hot 表示方法：**一个编码单元表示一个个体，也就是一个词。于是，一个词被表示成一个长度为字典大小的实数向量，每个维度对应字典里的一个词，除了该词对应维度上的值是 1，其余维度都是 0。

**词向量表示法：**与 one-hot 表示相对的是 distributed representation ，也就是常说的词向量：用一个更低维度的实向量表示词语，向量的每个维度在实数域 RR 取值。

在自然语言处理任务中，一套好的词向量能够提供丰富的领域知识，可以通过预训练获取，或者与最终任务端到端学习而来。

**循环神经网络**

循环神经网络（Recurrent Neural Network）是一种对序列数据建模的重要单元，模拟了离散时间（这里我们只考虑离散时间）动态系统的状态演化。“循环” 两字刻画了模型的核心：上一时刻的输出作为下一个时刻的输入，始终留在系统中如下面的图 1 所示，这种循环反馈能够形成复杂的历史。自然语言是一个天生的序列输入，RNN 恰好有能力去刻画词汇与词汇之间的前后关联关系，因此，在自然语言处理任务中占有重要的地位。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnMlogCKOIRozsnsuJmXB3XecGQqZL1XZ2GFiaS8xFzRpaiaEvXXr6F1VLo6licUXXBOz2OnfqN4TYEA/640)
▲图1. 最简单的RNN单元

RNN 形成“循环反馈” 的过程是一个函数不断复合的过程，可以等价为一个层数等于输入序列长度的前馈神经网络，如果输入序列有 100 个时间步，相当于一个 100 层的前馈网络，梯度消失和梯度爆炸的问题对 RNN 尤为严峻。

直觉上大于 1 的数连乘越乘越大，极端时会引起梯度爆炸；小于 1 的数连乘越乘越小，极端时会引起梯度消失。梯度消失也会令在循环神经网络中，后面时间步的信息总是会”压过”前面时间步。如果 t 时刻隐层状态依赖于 t 之前所有时刻，梯度需要通过所有的中间隐层逐时间步回传，这会形成如图 2 所示的一个很深的求导链。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnMlogCKOIRozsnsuJmXB3X3J9Itt3FgNEfm2AYACvEbSoCqux97b3UVuodMCaz1IxbN6s6vTTBpQ/640)
▲图2. t时刻依赖t时刻之前所有时刻

在许多实际问题中时间步之间相互依赖的链条并没有那么长，t 时刻也许仅仅依赖于它之前有限的若干时刻。很自然会联想到：如果模型能够自适应地学习出一些如图 3 所示的信息传播捷径来缩短梯度的传播路径，是不是可以一定程度减梯度消失和梯度爆炸呢？答案是肯定的，这也就是 LSTM 和 GRU 这类带有 “门控”思想的神经网络单元。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnMlogCKOIRozsnsuJmXB3X6yjkXcaxnaSp8GU9DGyxhnMCr56Hf0fZbtIkicQ9vAXQV5uLqmIzLWA/640)
▲图3. 自适应地形成一些信息传播的“捷径”

关于 LSTM 更详细的介绍请参考文献[2]，这里不再赘述，只需了解 LSTM/GUR 这些门控循环神经网络单元提出的动机即可。

**RNN LM**

语言模型是 NLP 领域的基础任务之一。语言模型是**计算一个序列的概率，判断一个序列是否属于一个语言的模型**，描述了这样一个条件概率![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnMlogCKOIRozsnsuJmXB3XTTibctYsKWmdRZbZlhzmFCmOYicFicTjXjGzL7BlrDP0bwF7UhPibSAibBg/640)，其中![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnMlogCKOIRozsnsuJmXB3XwGSrwdRaMAdR4bprrcdicx0WgWBrvIhnVuRzomLqDGJQCcjQmy2BrBA/640)是输入序列中的 T 个词语，用 one-hot 表示法表示。

言模型顾名思义是建模一种语言的模型，这一过程如图 4 所示：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnMlogCKOIRozsnsuJmXB3X5ib9Wbsv0A0aqCpvEWYCpQqrQLdantu8nJ9O1UydhgBS8E7oXZRicqXQ/640)
▲图4. RNN语言模型

RNN LM的工作流程如下：

1. 给定一段 one-hot 表示的输入序列 {x1,x2,...,xT}，将它们嵌入到实向量空间，得到词向量表示 ：{ω1,ω2,...,ωt}。

2. 以词向量序列为输入，使用 RNN 模型（可以选择LSTM或者GRU），计算输入序列到 t 时刻的编码 ht。

3. softmax 层以 ht 为输入，预测下一个最可能的词![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnMlogCKOIRozsnsuJmXB3XYBoj7Dl6o5W4mCapNAWvicwn3uh8DO999xnRveGtH23VU4SzTCcia47A/640)的概率![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnMlogCKOIRozsnsuJmXB3Xy5Iqx2rH4FiatoNcPiapQT4FnD5nn0bLJssqKF1qiaAoYKMlwLNhcqbdA/640)。

4.![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnMlogCKOIRozsnsuJmXB3X5CI8RqEgKfWqPsibFHUHCVjiax1yGoFfJ2Tqw1ogWulWTxF6gPuvynpg/640)，根据![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnMlogCKOIRozsnsuJmXB3XkupgKFfwPd7EdK2ibPuwcgWGUHGjNJm5PLThColTbjrXibHq3uVAq6RQ/640)和![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnMlogCKOIRozsnsuJmXB3XYBoj7Dl6o5W4mCapNAWvicwn3uh8DO999xnRveGtH23VU4SzTCcia47A/640)计算误差信号。

# PTB数据集介绍

至此，介绍完 RNN LM 模型的原理和基本结构，下面准备开始分别使用 PaddleFluid 和 TensorFlow 来构建我们的 训练任务。这里首先介绍这一篇我们使用 Mikolov 与处理过的 PTB 数据，这是语言模型任务中使用最为广泛的公开数据之一。 PTB 数据集包含 10000 个不同的词语（包含句子结束符<eos>，以及表示 低频词的特殊符号<unk>）。

通过运行data目录下的download.sh下载数据，我们将使用其中的ptb.train.txt文件进行训练，文件中一行是一句话，文本中的低频词已经全部被替换为<unk>预处理时我们会在 每一行的末尾附加上句子结束符<e>。

# 程序结构

这一节我们首先整体总结一下使用 PaddleFluid 平台和 TensorFlow 运行自己的神经网络模型都有哪些事情需要完成。

**PaddleFluid**

1. 调用 PaddleFluid API 描述神经网络模型。PaddleFluid 中一个神经网络训练任务被称之为一段Fluid Program。

2. 定义Fluid Program执行设备：place。常见的有fluid.CUDAPlace(0)和fluid.CPUPlace()

```python
place = fluid.CUDAPlace(
```
```python
0
```
```python
)
```
```python
if
```
```python
conf.use_gpu
```
```python
else
```
```python
fluid.CPUPlace()
```

注：PaddleFluid 支持混合设备运行，一些运算（operator）没有特定设备实现，或者为了提高全局资源利用率，可以为他们指定不同的计算设备。

3. 创建 PaddleFluid 执行器（Executor），需要为执行器指定运行设备。

```python
exe = fluid.Executor(place)
```

让执行器执行fluid.default_startup_program()，初始化神经网络中的可学习参数，完成必要的初始化工作。

5. 定义 DataFeeder，编写 data reader，**只需要关注如何返回一条训练/测试数据**。

6. 进入训练的双层循环（外层在 epoch 上循环，内层在 mini-batch 上循环），直到训练结束。

**TensorFlow**

1. 调用 TensorFlow API 描述神经网络模型。 TensorFlow 中一个神经网络模型是一个 Computation Graph。

2. 创建TensorFlow Session用来执行计算图。

```python
sess = tf.Session()
```

3. 调用sess.run(tf.global_variables_initializer())初始化神经网络中的可学习参数。

4. 编写**返回每个 mini-batch 数据**的数据读取脚本。

5. 进入训练的双层循环（外层在 epoch 上循环，内层在 mini-batch 上循环），直到训练结束。

如果不显示地指定使用何种设备进行训练，TensorFlow 会对机器硬件进行检测（是否有 GPU）， 选择能够尽可能利用机器硬件资源的方式运行。

从以上的总结中可以看到，PaddleFluid 程序和 TensorFlow 程序的整体结构非常相似，使用经验可以非常容易的迁移。

# 构建网络结构及运行训练

**加载训练数据**

**PaddleFluid**

**定义 输入data layers**

PaddleFluid 模型通过fluid.layers.data来接收输入数据。图像分类网络以图片以及图片对应的类别标签作为网络的输入：

```python
word = fluid.layers.data(
```

```python
name=
```
```python
"current_word"
```
```python
, shape=[
```
```python
1
```
```python
], dtype=
```
```python
"int64"
```
```python
, lod_level=
```
```python
1
```
```python
)
```

```python
lbl = fluid.layers.data(
```

```python
name=
```
```python
"next_word"
```
```python
, shape=[
```
```python
1
```
```python
], dtype=
```
```python
"int64"
```
```python
, lod_level=
```
```python
1
```
```python
)
```

1. 定义 data layer 的核心是指定输入 Tensor 的形状（shape）和类型。

2. RNN LM 使用 one-hot 作为输入，一个词用一个和字典大小相同的向量表示，每一个位置对应了字典中的 一个词语。one-hot 向量仅有一个维度为 1， 其余全部为 0。因此为了节约存储空间，通常都直接用一个整型数表示给出词语在字典中的 id，而不是真的创建一个和词典同样大小的向量 ，因此在上面定义的 data layer 中word和lbl的形状都是 1，类型是int64。

3. 需要特别说明的是，实际上word和lbl是两个[batch_size x 1]的向量，这里的batch size是指一个 mini-batch 中序列中的总词数。对序列学习任务， mini-batch 中每个序列长度 总是在发生变化，因此实际的batch_size只有在运行时才可以确定。batch size总是一个输入 Tensor 的第 0 维，在 PaddleFluid 中指定 data layer 的 shape 时，不需要指定batch size的大小，也不需要考虑占位。框架会自动补充占位符，并且在运行时 设置正确的维度信息。因此，上面的两个 data layer 的 shape 都只需要设置第二个维度，也就是 1。

**LoD Tensor和Non-Padding的序列输入**

与前两篇文章中的任务相比，在上面的代码片段中定义 data layer 时，出现了一个新的lod_level字段，并设置为 1。这里就要介绍在 Fluid 系统中表示序列输入的一个重要概念 LoDTensor。

那么，什么是 LoD（Level-of-Detail） Tensor 呢？

1. Tensor 是 nn-dimensional arry 的推广，LoDTensor 是在 Tensor 基础上附加了序列信息。

2. Fluid 中输入、输出，网络中的可学习参数全部统一使用 LoDTensor（n-dimension array）表示，对非序列数据，LoD 信息为空。一个 mini-batch 输入数据是一个 LoDTensor。

3. 在 Fluid 中，**RNN 处理变长序列无需 padding，得益于 LoDTensor表示**。

4. 可以简单将 LoD 理解为：std::vector<std::vector>。

下图是 LoDTensor 示意图（图片来自 Paddle 官方文档）：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnMlogCKOIRozsnsuJmXB3XmUuJDPiaribyfgaP3ibiaos6XrnE5GVia9nyMXJoS563MOrWsdynmxBM3ibA/640)
▲图5. LoD Tensor示意图

LoD 信息是附着在一个 Tensor 的第 0 维（也就是 batch size 对应的维度），来对一个 batch 中的数据进一步进行划分，表示了一个序列在整个 batch 中的起始位置。

LoD 信息可以嵌套，形成嵌套序列。例如，NLP 领域中的段落是一种天然的嵌套序列，段落是句子的序列，句子是词语的序列。

LoD 中的 level 就表示了序列信息的嵌套：

图 (a) 的 LoD 信息[0, 5, 8, 10, 14]：这个 batch 中共含有 4 条序列。

图 (b) 的 LoD 信息[[0, 5, 8, 10, 14] /*level=1*/, [0, 2, 3, 5, 7, 8, 10, 13, 14] /*level=2*/]：这个 batch 中含有嵌套的双层序列。

**有了 LoDTensor 这样的数据表示方式，用户不需要对输入序列进行填充，框架会自动完成 RNN 的并行计算处理。**

**如何构造序列输入信息**

明白了 LoD Tensor 的概念之后，另一个重要的问题是应该如何构造序列输入。在 PaddleFluid 中，通过 DataFeeder 模块来为网络中的 data layer 提供数据，调用方式如下面的代码所示：

```python
train_reader = paddle.batch(
```

```python
paddle.reader.shuffle(train_data, buf_size=
```
```python
51200
```
```python
),
```

```python
batch_size=conf.batch_size)
```

```python
place = fluid.CUDAPlace(
```
```python
0
```
```python
)
```
```python
if
```
```python
conf.use_gpu
```
```python
else
```
```python
fluid.CPUPlace()
```

```python
feeder = fluid.DataFeeder(feed_list=[word, lbl], place=place)
```

观察以上代码，需要用户完成的仅有：编写一个**实现读取一条数据**的 python 函数：train_data。train_data的代码非常简单，我们再来看一下它的具体实现[3]：

```python
def
```
```python
train_data
```
```python
(data_dir=
```
```python
"data"
```
```python
)
```
```python
:
```

```python
data_path = os.path.join(data_dir,
```
```python
"ptb.train.txt"
```
```python
)
```

```python
_, word_to_id = build_vocab(data_path)
```

```python
with
```
```python
open(data_path,
```
```python
"r"
```
```python
)
```
```python
as
```
```python
ftrain:
```

```python
for
```
```python
line
```
```python
in
```
```python
ftrain:
```

```python
words = line.strip().split()
```

```python
word_ids = [word_to_id[w]
```
```python
for
```
```python
w
```
```python
in
```
```python
words]
```

```python
yield
```
```python
word_ids[
```
```python
0
```
```python
:
```
```python
-1
```
```python
], word_ids[
```
```python
1
```
```python
:]
```

在上面的代码中：

1.train_data是一个 python generator ，函数名字可以任意指定，无需固定。

2.train_data打开原始数据数据文件，读取一行（一行既是一条数据），返回一个 python list，这个 python list 既是序列中所有时间步。具体的数据组织方式如下表所示（其中，f 代表一个浮点数，i 代表一个整数）：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnMlogCKOIRozsnsuJmXB3XVc3klANl1xLDKObicOGYCYQfMlLRRU69p40Mgicg5y0taclXw4XibuicAg/640)

3.paddle.batch()接口用来构造 mini-batch 输入，会调用train_data将数据读入一个 pool 中，对 pool 中的数据进行 shuffle，然后依次返回每个 mini-batch 的数据。

**TensorFlow**

TensorFlow 中使用占位符 placeholder 接收 训练数据，可以认为其概念等价于 PaddleFluid 中的 data layer。同样的，我们定义了如下两个 placeholder 用于接收当前词与下一个词语：

```python
def
```
```python
placeholders
```
```python
(self)
```
```python
:
```

```python
self._inputs = tf.placeholder(tf.int32,
```

```python
[
```
```python
None
```
```python
, self.max_sequence_length])
```

```python
self._targets = tf.placeholder(tf.int32, [
```
```python
None
```
```python
, self.vocab_size])
```

1. placeholder 只存储一个 mini-batch 的输入数据。与 PaddleFluid 中相同，_inputs这里接收的是 one-hot 输入，也就是该词语在词典中的 index，one-hot 表示 会进一步通过此词向量层的作用转化为实值的词向量表示。

2. 需要注意的是，TensorFlow 模型中网络输入数据需要进行填充，保证一个 mini-batch 中序列长度 相等。也就是一个 mini-batch 中的数据长度都是max_seq_length，这一点与 PaddleFluid 非常不同。

通常做法 是对不等长序列进行填充，在这一篇示例中我们使用一种简化的做法，每条训练样本都按照max_sequence_length来切割，保证一个 mini-batch 中的序列是等长的。

于是，_input的shape=[batch_size, max_sequence_length]。max_sequence_length即为 RNN 可以展开长度。

**构建网络结构**

**PaddleFluid****RNN LM**

这里主要关注最核心的 LSTM 单元如何定义：

```python
def
```
```python
__rnn
```
```python
(self, input)
```
```python
:
```

```python
for
```
```python
i
```
```python
in
```
```python
range(self.num_layers):
```

```python
hidden = fluid.layers.fc(
```

```python
size=self.hidden_dim *
```
```python
4
```
```python
,
```

```python
bias_attr=fluid.ParamAttr(
```

```python
initializer=NormalInitializer(loc=
```
```python
0.0
```
```python
, scale=
```
```python
1.0
```
```python
)),
```

```python
input=hidden
```
```python
if
```
```python
i
```
```python
else
```
```python
input)
```

```python
lstm = fluid.layers.dynamic_lstm(
```

```python
input=hidden,
```

```python
size=self.hidden_dim *
```
```python
4
```
```python
,
```

```python
candidate_activation=
```
```python
"tanh"
```
```python
,
```

```python
gate_activation=
```
```python
"sigmoid"
```
```python
,
```

```python
cell_activation=
```
```python
"sigmoid"
```
```python
,
```

```python
bias_attr=fluid.ParamAttr(
```

```python
initializer=NormalInitializer(loc=
```
```python
0.0
```
```python
, scale=
```
```python
1.0
```
```python
)),
```

```python
is_reverse=
```
```python
False
```
```python
)
```

```python
return
```
```python
lstm
```

PaddleFluid 中的所有 RNN 单元（RNN/LSTM/GRU）都支持非填充序列作为输入，框架会自动完成不等长序列的并行处理。当需要堆叠多个 LSTM 作为输入时，只需利用 Python 的for循环语句，让一个 LSTM 的输出成为下一个 LSTM 的输入即可。在上面的代码片段中有一点需要特别注意：**PaddleFluid 中的 LSTM 单元是由****fluid.layers.****fc****+****fluid.layers.dynamic_lstm**共同构成的。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnMlogCKOIRozsnsuJmXB3XU2hHTgibv71ThcVEkvWMaY3rmNiavporWMtmHEiapd9icvjE6GktaFhMtw/640)
▲图6. LSTM计算公式

图 6 是 LSTM 计算公式，图中用红色圈起来的计算是每一时刻输入矩阵流入三个门和 memory cell 的之前的映射。PaddleFluid 将这个四个矩阵运算合并为一个大矩阵一次性计算完毕，fluid.layers.dynamic_lstm不包含这部分运算。因此：

**1. PaddleFluid 中的 LSTM 单元是由****fluid.layers.fc****+****fluid.layers.dynamic_lstm****。**

**2. 假设 LSTM 单元的隐层大小是 128 维，****fluid.layers.****fc****和****fluid.layers.dynamic_lstm****的 size 都应该设置为 128 * 4，而不是 128。**

**TensorFlow RNN LM**

这里主要关注最核心的 LSTM 单元如何定义：

```python
def
```
```python
rnn
```
```python
(self)
```
```python
:
```

```python
def
```
```python
lstm_cell
```
```python
()
```
```python
:
```

```python
return
```
```python
tf.contrib.rnn.BasicLSTMCell(
```

```python
self.hidden_dim, state_is_tuple=
```
```python
True
```
```python
)
```

```python
cells = [lstm_cell()
```
```python
for
```
```python
_
```
```python
in
```
```python
range(self.num_layers)]
```

```python
cell = tf.contrib.rnn.MultiRNNCell(cells, state_is_tuple=
```
```python
True
```
```python
)
```

```python
_inputs = self.input_embedding()
```

```python
_outputs, _ = tf.nn.dynamic_rnn(
```

```python
cell=cell, inputs=_inputs, dtype=tf.float32)
```

```python
last = _outputs[:,
```
```python
-1
```
```python
, :]
```

```python
logits = tf.layers.dense(inputs=last, units=self.vocab_size)
```

```python
prediction = tf.nn.softmax(logits)
```

tf.nn.rnn_cell.BasicLSTMCell(n_hidden, state_is_tuple=True): 是最基本的 LSTM 单元。n_hidden表示 LSTM 单元隐层大小。state_is_tuple=True表示返回的状态用一个元祖表示。

tf.contrib.rnn.MultiRNNCell: 用来 wrap 一组序列调用的 RNN 单元的 wrapper。

tf.nn.dynamic_rnn: 通过指定 mini-batch 中序列的长度，可以跳过 padding 部分的计算，减少计算量。这一篇的例子中由于我们对输入数据进行了处理，将它们都按照max_sequence_length切割。

但是，dynamic_rnn可以让不同 mini-batch 的 batch size 长度不同，但同一次迭代一个 batch 内部的所有数据长度仍然是固定的。

**运行训练**

运行训练任务对两个平台都是常规流程，可以参考上文在程序结构一节介绍的流程，以及代码部分：PaddleFluid vs. TensorFlow，这里不再赘述。

# 总结

这一篇我们第一次接触 PaddleFluid 和 TensorFlow 平台的序列模型。了解 PaddleFluid 和 TensorFlow 在接受序列输入，序列处理策略上的不同。序列模型是神经网络模型中较为复杂的一类模型结构，可以衍生出非常复杂的模型结构。

不论是 PaddleFluid 以及 TensorFlow 都实现了多种不同的序列建模单元，如何选择使用这些不同的序列建模单元有很大的学问。到目前为止平台使用的一些其它重要主题：例如多线程多卡，如何利用混合设备计算等我们还尚未涉及。接下来的篇章将会继续深入 PaddleFluid 和 TensorFlow 平台的序列模型处理机制，以及更多重要功能如何在两个平台之间实现。

# 参考文献

**[1]. 本文配套代码**
https://github.com/JohnRabbbit/TF2Fluid/tree/master/03_rnnlm
**[2].****Understanding LSTM Networks**
http://colah.github.io/posts/2015-08-Understanding-LSTMs/
**[3].****train_data具体实现**
https://github.com/JohnRabbbit/TF2Fluid/blob/master/03_rnnlm/load_data_fluid.py


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)
▽ 点击 |阅读原文| 加入社区刷论文


