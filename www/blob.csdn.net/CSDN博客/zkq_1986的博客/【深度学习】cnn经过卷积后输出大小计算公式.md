# 【深度学习】cnn经过卷积后输出大小计算公式 - zkq_1986的博客 - CSDN博客





2018年11月16日 10:53:05[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：66








cnn经过卷积后输出大小计算公式：

![\left \lfloor \frac{in\_size - kernel\_size + 2*padding)}{stide} \right \rfloor + 1](https://private.codecogs.com/gif.latex?%5Cleft%20%5Clfloor%20%5Cfrac%7Bin%5C_size%20-%20kernel%5C_size%20&plus;%202*padding%29%7D%7Bstide%7D%20%5Cright%20%5Crfloor%20&plus;%201)

如下代码

```python
from torch import nn
import torch

class ResNet(nn.Module):
    def __init__(self, vocab_size, seq_len,embedding_len, num_classes=2):
        super(ResNet, self).__init__()
        self.seq_len=seq_len
        self.vocab_size = vocab_size
        self.embedding_len = embedding_len
        self.word_embeddings = nn.Embedding(vocab_size, embedding_len)

        self.conv1=nn.Conv2d(in_channels=1, out_channels=64, kernel_size=7, stride=2, padding=3, bias=False)

    def forward(self, x):
        x = self.word_embeddings(x)
        x = x.view(x.size(0), 1, self.seq_len, self.embedding_len)
        x=self.conv1(x)
        return x


if __name__ == '__main__':
    model = ResNet(vocab_size=50, seq_len=25,embedding_len=16)
    x=[[1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5]]
    input = torch.autograd.Variable(torch.LongTensor(x))
    o = model(input)
    print(o.size())
```

输出结果：

torch.Size([1, 64, 13, 9])



