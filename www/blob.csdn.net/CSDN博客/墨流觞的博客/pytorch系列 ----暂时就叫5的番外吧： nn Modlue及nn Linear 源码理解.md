# pytorch系列  ----暂时就叫5的番外吧：  nn.Modlue及nn.Linear 源码理解 - 墨流觞的博客 - CSDN博客





2018年10月09日 09:49:04[墨氲](https://me.csdn.net/dss_dssssd)阅读数：3293
所属专栏：[python3 pytorch0.4系列教程](https://blog.csdn.net/column/details/29714.html)









先看一个列子：

```python
import torch
from torch import nn

m = nn.Linear(20, 30)
input = torch.randn(128, 20)
output = m(input)

output.size()
```

out:

> 
torch.Size([128, 30])


刚开始看这份代码是有点迷惑的，m是类对象，而直接像函数一样调用m，`m(input)`

**重点**：
- **nn.Module 是所有神经网络单元（neural network modules）的基类**
- pytorch在nn.Module中，**实现了`__call__`方法，而在`__call__`方法中调用了forward函数。**

经过以上两点。上述代码就不难理解。

接下来看一下源码：
[https://github.com/pytorch/pytorch/blob/master/torch/nn/modules/module.py](https://github.com/pytorch/pytorch/blob/master/torch/nn/modules/module.py)

![在这里插入图片描述](https://img-blog.csdn.net/20181009085619283?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

再来看一下**nn.Linear**
[https://pytorch.org/docs/stable/_modules/torch/nn/modules/linear.html](https://pytorch.org/docs/stable/_modules/torch/nn/modules/linear.html)

主要看一下forward函数：
![在这里插入图片描述](https://img-blog.csdn.net/20181009090450739?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
返回的是：
$$input * weight + bias$$的线性函数

此时再看一下这一份代码：

```python
import torch
from torch import nn

m = nn.Linear(20, 30)
input = torch.randn(128, 20)
output = m(input)

output.size()
```

首先创建类对象m，然后通过`m(input)`实际上调用`__call__(input)`，然后`__call__(input)`调用
`forward()`函数，最后返回计算结果为：
$$[128, 20] \times [20, 30] = [128, 30]$$
所以自己创建多层神经网络模块时，只需要在实现`__init__`和`forward`即可.

接下来看一个简单的三层神经网络的例子：

```python
# define three layers
class simpleNet(nn.Module):

    def __init__(self, in_dim, n_hidden_1, n_hidden_2, out_dim):
        super().__init__()
        self.layer1 = nn.Linear(in_dim, n_hidden_1)
        self.layer2 = nn.Linear(n_hidden_1, n_hidden_2)
        self.layer3 = nn.Linear(n_hidden_2, out_dim)

    def forward(self, x):
        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)

        return x
```

以下为各层神经元个数：

输入： in_dim

第一层： n_hidden_1

第二层：n_hidden_2

第三层（输出层）：out_dim





