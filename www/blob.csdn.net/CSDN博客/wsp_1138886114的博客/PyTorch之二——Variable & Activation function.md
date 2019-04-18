# PyTorch之二——Variable & Activation function - wsp_1138886114的博客 - CSDN博客





2019年01月31日 20:52:57[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：35标签：[PyTorch之二——Variable &amp; Activation fu](https://so.csdn.net/so/search/s.do?q=PyTorch之二——Variable &amp; Activation fu&t=blog)
个人分类：[pytorch](https://blog.csdn.net/wsp_1138886114/article/category/8293033)












### 文章目录
- [Variable变量](#Variable_5)
- [Activation function](#Activation_function_45)


[上一篇](https://blog.csdn.net/wsp_1138886114/article/details/86593370)主要讲了PyTorch创建数据等一些入门知识点。本文主要是PyTorch中Variable变量的一些用法和激活函数

详情请查看：

Pytorch 中文文档：[http://pytorch-cn.readthedocs.io/zh/latest/#pytorch](http://pytorch-cn.readthedocs.io/zh/latest/#pytorch)

Pytorch 官方文档：[https://pytorch.org/docs/stable/index.html](https://pytorch.org/docs/stable/index.html)


### Variable变量

```python
import torch
from torch.autograd import Variable

tensor = torch.FloatTensor([[1, 2], [3, 4]])
variable = Variable(tensor, requires_grad=True) # requires_grad用来指定是否需要计算梯度

tensor_mean = torch.mean(tensor*tensor)         # tensor 计算(用来和variable对比)
variable_mean = torch.mean(variable*variable)   # Variable 计算
variable_mean.backward()                        # Variable 梯度


print("tensor:",tensor)
print("variable:",variable)

print("variable.data:",variable.data)                # 输出：tensor形式
print("variable.data.numpy:",variable.data.numpy())  # 输出：numpy形式

print("tensor_mean:",tensor_mean)
print("variable_mean:",variable_mean)
print("variable.grad:",variable.grad)

输出：
tensor: tensor([[1., 2.],
                [3., 4.]])
variable: tensor([[1., 2.],
                  [3., 4.]], requires_grad=True)
variable.data: tensor([[1., 2.],
                       [3., 4.]])
variable.data.numpy: [[1. 2.]
                      [3. 4.]]
tensor_mean: tensor(7.5000)
variable_mean: tensor(7.5000, grad_fn=<MeanBackward1>)
variable.grad: tensor([[0.5000, 1.0000],
                       [1.5000, 2.0000]])
```

到目前为止, 我们看不出什么不同。但是时刻记住, Variable 计算时, 它在背景幕布后面一步步默默地搭建着一个庞大的系统, 叫做计算图：computational graph。

这个图是用来干嘛的? 原来是将所有的计算步骤 (节点) 都连接起来, 最后进行误差反向传递的时候, 一次性将所有 variable 里面的修改幅度 (梯度) 都计算出来, 而 tensor 就没有这个能力。

### Activation function

Activation: 就是引入非线性，让神经网络可以描述更加复杂的问题

Torch 中的激励函数有很多, 不过我们平时要用到的就这几个. `relu`, `sigmoid`, `tanh`, `softplus`

```python
import torch
import torch.nn.functional as F
from torch.autograd import Variable
import matplotlib.pyplot as plt

x = torch.linspace(-5,5,200)   #生成(-5,5)之间均匀分布200个数（类似numpy）
x = Variable(x)
x_np = x.data.numpy()

# activation functions
function_dict = {
    "y_relu": F.relu(x),
    "y_sigmoid": F.sigmoid(x),
    "y_tanh": F.tanh(x),
    "y_softplus": F.softplus(x)
}

# 显示函数曲线
plt.figure(1, figsize=(8, 6))
plt.subplot(221)
plt.plot(x_np, function_dict["y_relu"].data.numpy(), c="red", label="relu")
plt.ylim((-1, 5))
plt.legend(loc='best')

plt.subplot(222)
plt.plot(x_np, function_dict["y_sigmoid"].data.numpy(), c="red", label="sigmoid")
plt.ylim((-0.2, 1.2))
plt.legend(loc="best")

plt.subplot(223)
plt.plot(x_np, function_dict["y_tanh"].data.numpy(), c="red", label="tanh")
plt.ylim((-1.2, 1.2))
plt.legend(loc="best")

plt.subplot(224)
plt.plot(x_np, function_dict["y_softplus"].data.numpy(), c="red", label="softplus")
plt.ylim((-0.2, 6))
plt.legend(loc="best")

plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190123105316303.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

关于激活函数详情你可以查看：[https://pytorch.org/docs/stable/nn.html](https://pytorch.org/docs/stable/nn.html)
鸣谢
[https://blog.csdn.net/hpulfc/article/details/81233554](https://blog.csdn.net/hpulfc/article/details/81233554)




