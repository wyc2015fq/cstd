# PyTorch之—循环层，RNN，LSTM - wsp_1138886114的博客 - CSDN博客





2019年01月29日 16:30:06[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：221标签：[PyTorch之—循环层，LSTM](https://so.csdn.net/so/search/s.do?q=PyTorch之—循环层，LSTM&t=blog)
个人分类：[pytorch](https://blog.csdn.net/wsp_1138886114/article/category/8293033)












### 文章目录
- [一、RNN 循环神经网络 参数详解](#RNN___1)
- [二、LSTM 长短期记忆网络 参数详解](#LSTM___43)
- [三、词嵌入 Embedding](#_Embedding_99)
- [小案例](#_153)
- [使用RNN 训练模型](#RNN__199)
- [使用LSTM对文本进行词性标注](#LSTM_296)
- [基于LSTM的词性标注模型](#LSTM_406)




### 一、RNN 循环神经网络 参数详解
- `class torch.nn.RNN( args, * kwargs)`

将一个多层的 Elman RNN，激活函数为`tanh`或者`ReLU`，用于输入序列。

对输入序列中每个元素，RNN每层的计算公式为 $$ h_t=\tanh(w_{ih} x_t+b_{ih}+w_{hh} h_{t-1}+b_{hh}) $$
$h_t$ 是时刻 $t$ 的隐状态。 $x_t$ 是上一层时刻 $t$ 的隐状态，或者是第一层在时刻 $t$ 的输入。如果 nonlinearity=‘relu’, 那么将使用 relu 代替 tanh 作为激活函数。
- RNN模型（公式）参数:

weight_ih_l[k] – 第k层的 input-hidden 权重， 可学习，形状是(input_size x hidden_size)。

weight_hh_l[k] – 第k层的 hidden-hidden 权重， 可学习，形状是(hidden_size x hidden_size)

bias_ih_l[k] – 第k层的 input-hidden 偏置， 可学习，形状是(hidden_size)

bias_hh_l[k] – 第k层的 hidden-hidden 偏置， 可学习，形状是(hidden_size)- 参数说明:

`input_size` – 输入x的特征数量。
`hidden_size` – 隐层的特征数量。
`num_layers` – RNN的层数。
`nonlinearity` – 指定非线性函数使用`tanh`还是`relu`。默认是`tanh`。

bias – 如果是False，那么RNN层就不会使用偏置权重 $b_ih$和$b_hh$,默认是True
`batch_first` – 如果True的话，那么输入Tensor的shape应该是[batch_size, time_step, feature],输出也是这样。
`dropout` – 如果值非零，那么除了最后一层外，其它层的输出都会套上一个dropout层。
`bidirectional` – 如果True，将会变成一个**双向RNN**，默认为False。- RNN的输入： **(input, h_0)**

**input** (seq_len, batch, input_size): 保存输入序列特征的tensor。input可以是被填充的变长的序列。细节请看torch.nn.utils.rnn.pack_padded_sequence()

**h_0** (num_layers * num_directions, batch, hidden_size): 保存着初始隐状态的tensor
- RNN的输出： (output, h_n)

**output** (seq_len, batch, hidden_size * num_directions): 保存着RNN最后一层的输出特征。如果输入是被填充过的序列，那么输出也是被填充的序列。
**h_n** (num_layers * num_directions, batch, hidden_size): 保存着最后一个时刻隐状态。

示例：

```python
rnn = nn.RNN(10, 20, 2)                    # 实例化

input = Variable(torch.randn(5, 3, 10))    # 定义input
h0 = Variable(torch.randn(2, 3, 20))       # 定义h0
output, hn = rnn(input, h0)                # 使用网络(得到输出，最后隐藏状态)
```

### 二、LSTM 长短期记忆网络 参数详解

[关于网络的理论详情请点击](https://blog.csdn.net/wsp_1138886114/article/details/81476171)
- `class torch.nn.LSTM( args, * kwargs)`

将一个多层的 (LSTM) 应用到输入序列。

对输入序列的每个元素，LSTM的每层都会执行以下计算：
$$ \begin{aligned} i_t &= sigmoid(W_{ii}x_t+b_{ii}+W_{hi}h_{t-1}+b_{hi}) \ f_t \\&= sigmoid(W_{if}x_t+b_{if}+W_{hf}h_{t-1}+b_{hf}) \ o_t \\&= sigmoid(W_{io}x_t+b_{io}+W_{ho}h_{t-1}+b_{ho})\ g_t \\&= tanh(W_{ig}x_t+b_{ig}+W_{hg}h_{t-1}+b_{hg})\ c_t \\&= f_tc_{t-1}+i_tg_t\ h_t\\&= o_t*tanh(c_t)\end{aligned} $$
$h_t$ 是时刻 $t$ 的隐状态, $c_t$ 是时刻 $t$ 的细胞状态，$x_t$ 是上一层的在时刻$t$的隐状态或者是第一层在时刻$t$的输入。$i_t, f_t, g_t, o_t$ 分别代表 **输入门**，**遗忘门**，**细胞**和**输出门**。- **LSTM模型参数:**

weight_ih_l[k] – 第k层可学习的input-hidden权重($W_{ii}|W_{if}|W_{ig}|W_{io}$)，形状为(input_size x 4*hidden_size)

weight_hh_l[k] – 第k层可学习的hidden-hidden权重($W_{hi}|W_{hf}|W_{hg}|W_{ho}$)，形状为(hidden_size x 4*hidden_size)。

bias_ih_l[k] – 第k层可学习的input-hidden偏置($b_{ii}|b_{if}|b_{ig}|b_{io}$)，形状为( 4*hidden_size)

bias_hh_l[k] – 第k层可学习的hidden-hidden偏置($b_{hi}|b_{hf}|b_{hg}|b_{ho}$)，形状为( 4*hidden_size)。
- 参数说明:

`input_size` – 输入的特征维度
`hidden_size` – 隐状态的特征维度
`num_layers` – 层数（和时序展开要区分开）
`bias` – 如果为False，那么LSTM将不会使用$b_{ih},b_{hh}$，默认为True。
`batch_first` – 如果为True，那么输入和输出Tensor的形状为(batch, seq, feature)
`dropout` – 如果非零的话，将会在RNN的输出上加个dropout，最后一层除外。
`bidirectional` – 如果为True，将会变成一个双向RNN，默认为False。- LSTM输入: input, (h_0, c_0)

**input (`seq_len`, `batch`, `input_size`)**:

包含输入序列特征的Tensor。也可以是packed variable ，详见 torch.nn.utils.rnn.pack_padded_sequence(input, lengths, batch_first=False[source])

**h_0** (`num_layers * num_directions`, `batch`, `hidden_size`):保存着batch中每个元素的初始化隐状态的Tensor
**c_0** (`num_layers * num_directions`, `batch`, `hidden_size`): 保存着batch中每个元素的初始化细胞状态的Tensor
- LSTM输出 output, (h_n, c_n)

**output (`seq_len`, `batch`, `hidden_size * num_directions`)**: 保存RNN最后一层的输出的Tensor。 如果输入是torch.nn.utils.rnn.PackedSequence，那么输出也是torch.nn.utils.rnn.PackedSequence。

**h_n** (`num_layers * num_directions`, `batch`, `hidden_size`): Tensor，保存着RNN最后一个时间步的隐状态。

**c_n** (`num_layers * num_directions`, `batch`, `hidden_size`): Tensor，保存着RNN最后一个时间步的细胞状态。

示例:

```python
lstm = nn.LSTM(10, 20, 2)                  #  实例化（输入特征维度，隐状态的特征维度，层数）

input = Variable(torch.randn(5, 3, 10))    # 定义输入    
h0 = Variable(torch.randn(2, 3, 20))       # 定义初始化隐藏状态
c0 = Variable(torch.randn(2, 3, 20))       # 定义初始化细胞状态

output, hn = lstm(input, (h0, c0))         #  使用LSTM网络（输出，RNN最后一步的隐状态。）
```

### 三、词嵌入 Embedding
- `class torch.nn.Embedding(num_embeddings, embedding_dim, padding_idx=None, max_norm=None, norm_type=2, scale_grad_by_freq=False, sparse=False)`
一个保存了固定字典和大小的简单查找表。

这个模块常用来保存词嵌入和用下标检索它们。模块的输入是一个下标的列表，输出是对应的词嵌入。- 参数：
`num_embeddings (int)` - 嵌入字典的大小
`embedding_dim (int)` - 每个嵌入向量的大小
`padding_idx (int, optional)` - 如果提供的话，输出遇到此下标时用零填充
`max_norm (float, optional)` - 如果提供的话，会重新归一化词嵌入，使它们的范数小于提供的值
`norm_type (float, optional)` - 对于max_norm选项计算p范数时的p
`scale_grad_by_freq (boolean, optional)` - 如果提供的话，会根据字典中单词频率缩放梯度- 变量：
weight (Tensor) -形状为(`num_embeddings`, `embedding_dim`)的模块中可学习的权值- 形状：
输入： `LongTensor (N, W)`, N = mini-batch, W = 每个mini-batch中提取的下标数

输出： `(N, W, embedding_dim)`
例子：

```python
from torch import nn
from torch.autograd import Variable
import torch

embedding = nn.Embedding(10, 3)                            # 10个大小为3的tensor 嵌入模块
input = Variable(torch.LongTensor([[1,2,4,5],[4,3,2,9]]))  # 2批样本，每批4个
out1 = embedding(input)
print("out1:",out1)

# 使用 padding_idx 填充
embedding2 = nn.Embedding(10, 3, padding_idx=0)
input2 = Variable(torch.LongTensor([[0,2,0,5]]))
out2 = embedding2(input2)
print("out2:",out2)

输出：
out1: tensor([[[ 0.6806, -1.2532, -0.4786],
               [-0.6535, -1.4873,  1.9218],
               [-1.4653,  1.1515,  0.2497],
               [ 1.0350, -0.3353, -0.4966]],

              [[-1.4653,  1.1515,  0.2497],
               [ 0.9943,  0.1529,  1.8212],
               [-0.6535, -1.4873,  1.9218],
               [-1.3094,  0.0413, -0.4064]]], grad_fn=<EmbeddingBackward>)
         
out2: tensor([[[ 0.0000,  0.0000,  0.0000],
               [ 1.5210, -0.0973, -0.1743],
               [ 0.0000,  0.0000,  0.0000],
               [ 0.8931,  0.9085,  0.3323]]], grad_fn=<EmbeddingBackward>)
```

#### 小案例

LSTM单元接受的**输入**都必须是3维的张量（Tensors）：
- **第一维**体现的是序列（sequence）结构，**第二维**体现的是小块（mini-batch）结构，**第三维**体现的是输入的元素（elements of input）。如果在应用中不适用小块结构，那么可以将输入的张量中该维度设为1，但必须要体现出这个维度。（上面的参数详解说的很明白了）

```python
import torch
import torch.autograd as autograd # torch中自动计算梯度模块
import torch.nn as nn             # 神经网络模块

torch.manual_seed(1)


lstm = nn.LSTM(3, 3)  # lstm单元输入=输出维度都是3
# 生成一个长度为5，每一个元素为1*3的序列作为输入
inputs = [autograd.Variable(torch.randn((1, 3))) for _ in range(5)]

# 设置并初始化隐藏层的数据
hidden = (autograd.Variable(torch.randn(1, 1, 3)),
          autograd.Variable(torch.randn((1, 1, 3))))

 # 单步执行序列，一次一个元素。在每个步骤之后，hidden包含hidden状态。
for i in inputs:
    out, hidden = lstm(i.view(1, 1, -1), hidden)
print(out)
print(hidden)

inputs = torch.cat(inputs).view(len(inputs), 1, -1)
hidden = (autograd.Variable(torch.randn(1, 1, 3)),
          autograd.Variable(torch.randn((1, 1, 3))))  # 清除隐藏状态
out, hidden = lstm(inputs, hidden)
print(out)
print(hidden)

输出：
tensor([[[-0.3600,  0.0893,  0.0215]]], grad_fn=<StackBackward>)
(tensor([[[-0.3600,  0.0893,  0.0215]]], grad_fn=<StackBackward>), tensor([[[-1.1298,  0.4467,  0.0254]]], grad_fn=<StackBackward>))
tensor([[[-0.0187,  0.1713, -0.2944]],

        [[-0.3521,  0.1026, -0.2971]],

        [[-0.3191,  0.0781, -0.1957]],

        [[-0.1634,  0.0941, -0.1637]],

        [[-0.3368,  0.0959, -0.0538]]], grad_fn=<StackBackward>)
(tensor([[[-0.3368,  0.0959, -0.0538]]], grad_fn=<StackBackward>), tensor([[[-0.9825,  0.4715, -0.0633]]], grad_fn=<StackBackward>))
```

#### 使用RNN 训练模型

```python
import torch
import torch.nn as nn
import numpy as np
from matplotlib import pyplot as plt


# 通过Sin预测Cos
# 由于是使用sin和cos函数，所以这里不需要dataloader，我们直接使用Numpy生成数据，Pytorch没有π这个常量，所以所有操作都是用Numpy完成

TIME_STEP = 10     # rnn 时序步长数
INPUT_SIZE = 1     # rnn 的输入维度
DEVICE = torch.device("cuda" if torch.cuda.is_available() else "cpu")
H_SIZE = 64        # rnn 隐藏单元个数
EPOCHS=300         # 总共训练次数
h_state = None     # 隐藏层状态

steps = np.linspace(0,np.pi*2,256,dtype=np.float32)
x_np = np.sin(steps)
y_np = np.cos(steps)

# 可视化数据
# plt.figure(1)
# plt.suptitle('sin and cos',fontsize='18')
# plt.plot(steps,y_np,'r-',label='target(cos)')
# plt.plot(steps,x_np,'b-',label='input(sin)')
# plt.legend(loc='best')
# plt.show()

class RNN(torch.nn.Module):
    def __init__(self):
        super(RNN,self).__init__()
        self.rnn = nn.RNN(
            input_size=INPUT_SIZE,
            hidden_size=H_SIZE,
            num_layers=1,
            batch_first=True
        )
        self.out = nn.Linear(H_SIZE, 1)

    def forward(self, x,h_state):
        """
        :param x:       (batch, time_step, input_size)
        :param h_state: (n_layers, batch, hidden_size)
        :param r_out:   (batch, time_step, hidden_size)
        :return:
        """
        r_out,h_state = self.rnn(x,h_state)
        outs = []
        for time_step in range(r_out.size(1)): # 计算每一步长的预测值
            outs.append(self.out(r_out[:,time_step,:]))
        return torch.stack(outs,dim=1),h_state
         # 也可使用以下这样的返回值
         # r_out = r_out.view(-1, 32)
         # outs = self.out(r_out)
         # return outs, h_state


rnn = RNN().to(DEVICE)
optimizer = torch.optim.Adam(rnn.parameters()) # Adam优化，几乎不用调参
criterion = nn.MSELoss()                       # 因为最终的结果是一个数值，所以损失函数用均方误差


rnn.train()
plt.figure(2)
for step in range(EPOCHS):
    start,end = step*np.pi,(step+1)*np.pi      # 一个时间周期
    steps = np.linspace(start,end,TIME_STEP,dtype=np.float32)
    x_np = np.sin(steps)
    y_np = np.cos(steps)

    x = torch.from_numpy(x_np[np.newaxis,:,np.newaxis]).to(DEVICE) # shape (batch, time_step, input_size)
    y = torch.from_numpy(y_np[np.newaxis,:,np.newaxis]).to(DEVICE)

    prediction,h_state = rnn(x,h_state)
    h_state = h_state.data                    # 重置隐藏层的状态，切断和前一次迭代的链接
    loss = criterion(prediction,y)            # 评估模型

    optimizer.zero_grad()
    loss.backward()
    optimizer.step()

    if (step+1)%20 == 0:
        print("EPOCH:{},Loss:{:.4f}".format(step+1,loss))
        plt.plot(steps, y_np.flatten(), 'r-')
        plt.plot(steps, prediction.data.cpu().numpy().flatten(), 'b-')
        plt.draw()
        plt.pause(0.01)
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190215203702916.gif)

以上代码中，我用到了cuda,若你的机器没有GPU你需要更改：
```python
plt.plot(steps, prediction.data.cpu().numpy().flatten(), 'b-')
改为：
plt.plot(steps, prediction.data.numpy().flatten(), 'b-')
```

#### 使用LSTM对文本进行词性标注

我们会给网络输入一个由 $w_1,w_2,...,w_M$ 表示的时序数据，其中 $w_i \in V ， V$是我们的词汇表。同样我们有一个标签数据集合：$T$ ，同时用 $y_i$ 表示标注 单词$w_i$ ，表示该单词的一个性质，比如这里我们使用“词性”这个性质。我们的网络同时也会给出关于某个单词的词性的预测 $\hat y_i$ 。这相当于一个结构预测模型，其中输出也是一个序列 $\hat y_1, \hat y_2,..., \hat y_M$ ，其中 $\hat y_i \in T$ 。

这相当于我们一开始展示的RNN类型中的最后一类。即将一个序列送入RNN网络，对于每一个输入给出一个输出，但是输出的给出不是基于当时的输入而是基于所有已送入网络的输入的。如果我们用 h_i 表示第 i 个时间步网络的隐藏层的输出，并且用一个索引来表示该输出的类型，那么网络预测的输出可以是隐藏层输出的Softmax回归：

$$\hat y_i =argmax_j(\log Softmax(Ah_i + b))_j$$

可以将最大输出值的神经元索引代表当时输入单词的词性标注。可以看出隐藏层输出的维度是标签书记集合的大小。

training_data中的"DET", “NN”, "V"分别代表的是冠词、名词、和动词。我们要给句子中出现的每一个单词一个索引，依次来构建整个词汇表 V ；同样也需要对每一个出现的词性做一个索引，建立一个标签数据集 T：

```python
import torch
import torch.autograd as autograd # torch中自动计算梯度模块
import torch.nn as nn             # 神经网络模块
import torch.nn.functional as F   # 神经网络模块中的常用功能
import torch.optim as optim       # 模型优化器模块

torch.manual_seed(1)


training_data = [
    ("The dog ate the apple".split(), ["DET", "NN", "V", "DET", "NN"]),
    ("Everybody read that book".split(), ["NN", "V", "DET", "NN"])
]


word_to_ix = {}                        # 单词的索引字典
for sent, tags in training_data:
    for word in sent:
        if word not in word_to_ix:
            word_to_ix[word] = len(word_to_ix)
print(word_to_ix)
tag_to_ix = {"DET": 0, "NN": 1, "V": 2} # 手工设定词性标签数据字典


class LSTMTagger(nn.Module):
    def __init__(self, embedding_dim, hidden_dim, vocab_size, tagset_size):
        super(LSTMTagger, self).__init__()
        self.hidden_dim = hidden_dim
        self.word_embeddings = nn.Embedding(vocab_size, embedding_dim) #词汇量大小，每个词汇向量表示的向量维度
        self.lstm = nn.LSTM(embedding_dim, hidden_dim)
        self.hidden2tag = nn.Linear(hidden_dim, tagset_size)
        self.hidden = self.init_hidden()

    def init_hidden(self):
        return (autograd.Variable(torch.zeros(1, 1, self.hidden_dim)),
                autograd.Variable(torch.zeros(1, 1, self.hidden_dim)))

    def forward(self, sentence):
        embeds = self.word_embeddings(sentence)
        lstm_out, self.hidden = self.lstm(
            embeds.view(len(sentence), 1, -1), self.hidden)
        tag_space = self.hidden2tag(lstm_out.view(len(sentence), -1))
        tag_scores = F.log_softmax(tag_space)
        return tag_scores


# 实例化-> 定义损失函数 -> 定义优化器
EMBEDDING_DIM = 3
HIDDEN_DIM = 3
model = LSTMTagger(EMBEDDING_DIM, HIDDEN_DIM, len(word_to_ix), len(tag_to_ix))
loss_function = nn.NLLLoss()
optimizer = optim.SGD(model.parameters(), lr=0.1)

def prepare_sequence(seq, to_ix):     # 把以句子形式表示的输入转换成模型tensor
    idxs = [to_ix[w] for w in seq]
    tensor = torch.LongTensor(idxs)
    return autograd.Variable(tensor)

inputs = prepare_sequence(training_data[0][0], word_to_ix)
tag_scores = model(inputs)
print(training_data[0][0])
print("inputs:",inputs)
print("tag_scores:",tag_scores)


for epoch in range(300):                    # 我们要训练300次
    for sentence, tags in training_data:
        model.zero_grad()                   # 清除网络先前的梯度值，梯度值是变量才有的数据，张量没有
        model.hidden = model.init_hidden()  # 重新初始化隐藏层数据，避免受之前运行代码的干扰
        sentence_in = prepare_sequence(sentence, word_to_ix) # 输入数据和真实标签数据
        targets = prepare_sequence(tags, tag_to_ix)
        tag_scores = model(sentence_in)     # 运行模型
        loss = loss_function(tag_scores, targets)
        loss.backward()
        optimizer.step()

# 来检验下模型训练的结果
inputs = prepare_sequence(training_data[0][0], word_to_ix)
tag_scores = model(inputs)
print("test_tag_scores:",tag_scores)

输出：
{'The': 0, 'dog': 1, 'ate': 2, 'the': 3, 'apple': 4, 'Everybody': 5, 'read': 6, 'that': 7, 'book': 8}
['The', 'dog', 'ate', 'the', 'apple']
inputs: tensor([0, 1, 2, 3, 4])
tag_scores: tensor([[-1.0227, -0.8756, -1.4971],
E:/pytorch_AI/pytorch_LSTM_02.py:43: UserWarning: ...
        [-1.0451, -0.8657, -1.4803],
        [-1.0275, -0.8658, -1.5079],
        [-1.0644, -0.8610, -1.4596],
        [-1.0504, -0.8623, -1.4783]], grad_fn=<LogSoftmaxBackward>)
  tag_scores = F.log_softmax(tag_space)
test_tag_scores: tensor([[-0.1303, -2.3714, -3.5473],
        [-3.2184, -0.2852, -1.5696],
        [-3.4137, -0.4275, -1.1553],
        [-0.1034, -2.5672, -3.8420],
        [-2.7644, -0.3116, -1.5860]], grad_fn=<LogSoftmaxBackward>)
```

#### 基于LSTM的词性标注模型

```python
import torch
import gensim
from torch.autograd import Variable
torch.manual_seed(2)

datas=[('你 叫 什么 名字 ?','n v n n f'),('今天 天气 怎么样 ?','n n adj f'),]
words=[ data[0].split() for data in datas]
tags=[ data[1].split() for data in datas]

id2word=gensim.corpora.Dictionary(words)
word2id=id2word.token2id

id2tag=gensim.corpora.Dictionary(tags)
tag2id=id2tag.token2id

def sen2id(inputs):
    return [word2id[word] for word in inputs]
def tags2id(inputs):
    return [tag2id[word] for word in inputs]

def formart_input(inputs):
    return torch.autograd.Variable(torch.LongTensor(sen2id(inputs)))
def formart_tag(inputs):
    return torch.autograd.Variable(torch.LongTensor(tags2id(inputs)),)

class LSTMTagger(torch.nn.Module):
    def __init__(self,embedding_dim,hidden_dim,voacb_size,target_size):
        super(LSTMTagger,self).__init__()
        self.embedding_dim=embedding_dim
        self.hidden_dim=hidden_dim
        self.voacb_size=voacb_size
        self.target_size=target_size

        self.embedding=torch.nn.Embedding(self.voacb_size,self.embedding_dim) #(词汇量大小，词向量维度)
        self.hidden=(Variable(torch.zeros(1,1,self.hidden_dim)),Variable(torch.zeros(1,1,self.hidden_dim)))

        self.lstm = torch.nn.LSTM(self.embedding_dim, self.hidden_dim)
        self.out2tag=torch.nn.Linear(self.hidden_dim,self.target_size)
        self.log_softmax = torch.nn.LogSoftmax()

    def forward(self,inputs):
        input=self.embedding((inputs))
        out,self.hidden=self.lstm(input.view(-1,1,self.embedding_dim),self.hidden)
        tags=self.log_softmax(self.out2tag(out.view(-1,self.hidden_dim)))
        return tags

model=LSTMTagger(3,3,len(word2id),len(tag2id))
loss_function=torch.nn.NLLLoss()
optimizer=torch.optim.SGD(model.parameters(),lr=0.1)
for _ in range(100):
    model.zero_grad()
    input=formart_input('你 叫 什么 名字'.split())
    tags=formart_tag('n n adj f'.split())
    out=model(input)
    loss=loss_function(out,tags)
    loss.backward(retain_graph=True)
    optimizer.step()
    print(loss.data.item())
input=formart_input('今天 天气 怎么样'.split())
out=model(input)
out=torch.max(out,1)[1]
# print("out.data:",out.data)
# print("out.data.numpy():",out.data.numpy())

print([id2tag[out.data.numpy()[i]] for i in range(0,out.size()[0])])
```

特别鸣谢
[https://zhuanlan.zhihu.com/p/28448135](https://zhuanlan.zhihu.com/p/28448135)








