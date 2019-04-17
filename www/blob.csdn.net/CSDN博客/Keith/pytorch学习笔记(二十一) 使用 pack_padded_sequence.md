# pytorch学习笔记(二十一): 使用 pack_padded_sequence - Keith - CSDN博客





2018年03月29日 21:20:40[ke1th](https://me.csdn.net/u012436149)阅读数：6001
所属专栏：[pytorch学习笔记](https://blog.csdn.net/column/details/15023.html)









下面附上一张 `pack_padded_sequence` 原理图（其实只是将三维的输入去掉PAD的部分搞成了二维的。在RNN前向的时候，根据`batch_sizes`参数取对应的时间步计算。）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190310114909619.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI0MzYxNDk=,size_16,color_FFFFFF,t_70)
在使用 pytorch 的 RNN 模块的时候, 有时会不可避免的使用到 `pack_padded_sequence` 和 `pad_packed_sequence`,  **当使用双向RNN的时候, 必须要使用 pack_padded_sequence !!** .否则的话, pytorch 是无法获得 序列的长度, 这样也无法正确的计算双向 `RNN/GRU/LSTM` 的结果.

但是在使用 `pack_padded_sequence` 时有个问题, 即输入 `mini-batch` 序列的长度必须是从长到短排序好的, 当mini-batch 中的样本的顺序非常的重要的话, 这就有点棘手了. 比如说, 每个 sample 是个 单词的 字母级表示, 一个 `mini-batch` 保存了一句话的 words. 例如：`[['p', 'y', 't', 'o', 'r', 'c', 'h'], ['i', 's'], ['g'. 'o', 'o', 'd']]` 为一个 `mini-batch`。这种情况在`char-level` 的模型中非常常见，先对 `char-sequence`进行编码，然后再对`word-sequence` 进行编码。

在这种情况下, 我们依然要使用 `pack_padded_sequence`, 所以需要先将 mini-batch 中样本排序, 之后输入到 `RNN/LSTM/GRU` 计算，最后再恢复成以前的顺序.

下面的代码将用来实现这种方法:

```python
import torch
from torch import nn
from torch.autograd import Variable

def rnn_forwarder(rnn, inputs, seq_lengths):
    """
    :param rnn: RNN instance
    :param inputs: FloatTensor, shape [batch, time, dim] if rnn.batch_first else [time, batch, dim]
    :param seq_lengths: LongTensor shape [batch]
    :return: the result of rnn layer,
    """
    batch_first = rnn.batch_first
    # assume seq_lengths = [3, 5, 2]
	# 对序列长度进行排序(降序), sorted_seq_lengths = [5, 3, 2]
    # indices 为 [1, 0, 2], indices 的值可以这么用语言表述
    # 原来 batch 中在 0 位置的值, 现在在位置 1 上.
    # 原来 batch 中在 1 位置的值, 现在在位置 0 上.
    # 原来 batch 中在 2 位置的值, 现在在位置 2 上.
    sorted_seq_lengths, indices = torch.sort(seq_lengths, descending=True)
	
	# 如果我们想要将计算的结果恢复排序前的顺序的话, 
    # 只需要对 indices 再次排序(升序),会得到 [0, 1, 2],  
    # desorted_indices 的结果就是 [1, 0, 2]
    # 使用 desorted_indices 对计算结果进行索引就可以了.
    _, desorted_indices = torch.sort(indices, descending=False)
	
	# 对原始序列进行排序
    if batch_first:
        inputs = inputs[indices]
    else:
        inputs = inputs[:, indices]
    packed_inputs = nn.utils.rnn.pack_padded_sequence(inputs,
                                                      sorted_seq_lengths.cpu().numpy(),
                                                      batch_first=batch_first)

    res, state = rnn(packed_inputs)

    padded_res, _ = nn.utils.rnn.pad_packed_sequence(res, batch_first=batch_first)
	
	# 恢复排序前的样本顺序
    if batch_first:
        desorted_res = padded_res[desorted_indices]
    else:
        desorted_res = padded_res[:, desorted_indices]
    return desorted_res

if __name__ == "__main__":
    bs = 3
    max_time_step = 5
    feat_size = 15
    hidden_size = 7
    seq_lengths = [3, 5, 2]

    rnn = nn.GRU(input_size=feat_size, 
                 hidden_size=hidden_size, batch_first=True, bidirectional=True)
    x = Variable(torch.FloatTensor(bs, max_time_step, feat_size).normal_())
	
    using_packed_res = rnn_forwarder(rnn, x, seq_lengths)
    print(using_packed_res)
	
    # 不使用 pack_paded, 用来和上面的结果对比一下.
    not_packed_res, _ = rnn(x)
    print(not_packed_res)
```




