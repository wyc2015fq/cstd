# 【pyTorch】torch下的网络如何对文本进行embedding操作 - zkq_1986的博客 - CSDN博客





2018年11月16日 10:15:27[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：416








torch下的网络对文本进行embedding操作的代码示例如下：

```python
from torch import nn
import torch
from torch.nn import functional as F


class TextNet(nn.Module):
    def __init__(self, vocab_size, seq_len,embedding_len, num_classes=2):
        super(TextNet, self).__init__()
        self.seq_len=seq_len
        self.vocab_size = vocab_size
        self.embedding_len = embedding_len
        self.word_embeddings = nn.Embedding(vocab_size, embedding_len)

    def forward(self, x):
        x = self.word_embeddings(x)
        return x


if __name__ == '__main__':
    model = TextNet(vocab_size=5000, seq_len=600,embedding_len=2)
    x=[[1,2,2,4]]
    input = torch.autograd.Variable(torch.LongTensor(x))
    o = model(input)
    print(o)
    print(o.size())

    x = [[1, 3, 2, 4]]
    input = torch.autograd.Variable(torch.LongTensor(x))
    o = model(input)
    print(o)
    print(o.size())
```

输出结果：

tensor([[[-0.6614,  0.1508],
 [ 0.6160, -0.2825],

         [ 0.6160, -0.2825],

         [ 1.3361, -1.4880]]])

torch.Size([1, 4, 2])

tensor([[[-0.6614,  0.1508],

         [ 1.1087,  1.0002],

         [ 0.6160, -0.2825],

         [ 1.3361, -1.4880]]])

torch.Size([1, 4, 2])





