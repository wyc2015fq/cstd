# pytorch 学习笔记（二十二）：关于 inplace operation - Keith - CSDN博客





2018年06月26日 18:58:54[ke1th](https://me.csdn.net/u012436149)阅读数：5217
所属专栏：[pytorch学习笔记](https://blog.csdn.net/column/details/15023.html)









原文发表在 [知乎上](https://zhuanlan.zhihu.com/p/38475183) 在这里就做一下同步吧。 

(本文章适用于 pytorch0.4.0 版本, 既然 Variable 和 Tensor merge 到一块了, 那就叫 Tensor吧)

在编写 pytorch 代码的时候, 如果模型很复杂, 代码写的很随意, 那么很有可能就会碰到由 inplace operation 导致的问题. 所以本文将对 pytorch 的 inplace operation 做一个简单的总结.

**在 pytorch 中, 有两种情况不能使用 inplace operation:**
- 对于 **requires_grad=True 的 叶子张量(leaf tensor)** 不能使用 inplace operation
- 对于在 **求梯度阶段需要用到的张量** 不能使用 inplace operation

下面将通过代码来说明以上两种情况:

第一种情况: requires_grad=True 的 leaf tensor

```python
import torch

w = torch.FloatTensor(10) # w 是个 leaf tensor
w.requires_grad = True    # 将 requires_grad 设置为 True
w.normal_()               # 在执行这句话就会报错
# 报错信息为
#  RuntimeError: a leaf Variable that requires grad has been used in an in-place operation.
```

很多人可能会有疑问, 模型的参数就是 requires_grad=true 的 leaf tensor, 那么模型参数的初始化应该怎么执行呢? 如果看一下 nn.Module._apply() 的代码, 这问题就会很清楚了

`w.data = w.data.normal() # 可以使用曲线救国的方法来初始化参数`

第二种情况: 求梯度阶段需要用到的张量

```python
import torch
x = torch.FloatTensor([[1., 2.]])
w1 = torch.FloatTensor([[2.], [1.]])
w2 = torch.FloatTensor([3.])
w1.requires_grad = True
w2.requires_grad = True

d = torch.matmul(x, w1)
f = torch.matmul(d, w2)
d[:] = 1 # 因为这句, 代码报错了 RuntimeError: one of the variables needed for gradient computation has been modified by an inplace operation

f.backward()
```

为什么呢?

因为 f=matmul(d, w2) , $\frac{\partial f}{\partial w2} = g(d)$ ,  f 对于 w2 的导数是关于 d 的函数:
- 在计算  f 的时候,  d  是等于某个值的, f 对于 w2 的导数是和这时候的 d 值相关的
- 但是计算完 f 之后, d 的值变了, 这就会导致 f.backward() 对于 w2 的导数计算出错误, 为了防止这种错误, pytorch 选择了报错的形式.
- 造成这个问题的主要原因是因为 在执行 f = torch.matmul(d, w2) 这句的时候, pytorch 的反向求导机制 保存了 d 的引用为了之后的 反向求导计算.

```python
import torch
x = torch.FloatTensor([[1., 2.]])
w1 = torch.FloatTensor([[2.], [1.]])
w2 = torch.FloatTensor([3.])
w1.requires_grad = True
w2.requires_grad = True

d = torch.matmul(x, w1)
d[:] = 1   # 稍微调换一下位置, 就没有问题了
f = torch.matmul(d, w2)
f.backward()
```

## 最后再提一下 .data 与 .detach(), (这部分翻译自 pytorch0.4.0 的 release note):

在 0.4.0 版本之前, `.data` 的语义是 获取 Variable 的 内部 Tensor, 在 0.4.0 版本将 Variable 和 Tensor merge 之后, .data 和之前有类似的 语义, 也是 内部的 Tensor 的概念.

`x.data` 与 `x.detach()` 返回的 tensor 有相同的地方, 也有不同的地方:

相同:
- 都和 x 共享同一块数据
- 都和 x 的 计算历史无关
- requires_grad = False

不同:
- y=x.data 在某些情况下不安全, 某些情况, 指的就是 上述 inplace operation 的第二种情况

```python
import torch
x = torch.FloatTensor([[1., 2.]])
w1 = torch.FloatTensor([[2.], [1.]])
w2 = torch.FloatTensor([3.])
w1.requires_grad = True
w2.requires_grad = True

d = torch.matmul(x, w1)

d_ = d.data

f = torch.matmul(d, w2)
d_[:] = 1

f.backward()

# 这段代码没有报错, 但是计算上的确错了
# 如果 打印 w2.grad 结果看一下的话, 得到 是 1, 但是正确的结果应该是 4.
```

上述代码应该报错, 因为:
- d_ 和 d 共享同一块数据,
- 改 d_ 就相当于 改 d 了
- 但是, 代码并没有报错 , 但是计算上的确错了

所以, release note 中指出, 如果想要 detach 的效果的话, 还是 detach() 安全一些.

```python
import torch
x = torch.FloatTensor([[1., 2.]])
w1 = torch.FloatTensor([[2.], [1.]])
w2 = torch.FloatTensor([3.])
w1.requires_grad = True
w2.requires_grad = True

d = torch.matmul(x, w1)

d_ = d.detach() # 换成 .detach(), 就可以看到 程序报错了...

f = torch.matmul(d, w2)
d_[:] = 1
f.backward()
```

Enjoy the pytorch :)



