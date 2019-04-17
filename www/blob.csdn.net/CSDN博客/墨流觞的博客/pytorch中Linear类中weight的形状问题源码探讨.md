# pytorch中Linear类中weight的形状问题源码探讨 - 墨流觞的博客 - CSDN博客





2018年10月30日 10:23:54[墨氲](https://me.csdn.net/dss_dssssd)阅读数：365








```python
import torch
from torch import nn

m = nn.Linear(20, 30)
input = torch.randn(128, 20)
output = m(input)

print(output.size())
print(m.weight.shape)
```

来看一下输出：

out:

> 
torch.Size([128, 30])

torch.Size([30, 20])


发现weight的形状是[30,20]而非[20, 30]？

所以具体看一下源码的实现方式：
- Linear类的源码网址：[https://pytorch.org/docs/stable/_modules/torch/nn/modules/linear.html](https://pytorch.org/docs/stable/_modules/torch/nn/modules/linear.html)
- functional模块的源码网址：
[https://pytorch.org/docs/stable/_modules/torch/nn/functional.html](https://pytorch.org/docs/stable/_modules/torch/nn/functional.html)
- 在Linear类中的`__init__`函数中，weight形状为`[out_features, in_features]`
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181030101502219.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)- 在`forward`函数中调用`F.linear`函数，实现单层线性神经网络层的计算
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181030101815731.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)- 在F.linear函数中，使用的是`weight.t()`，也就是将weight转置，再传入matmul计算。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181030101854593.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
通过以上三步，pytorch就完成weight形状的维护。简单的说就是，**在定义时使用的是[out_features, in_features]，而在单层线性神经网络计算时使用的是weight的转置矩阵。**






