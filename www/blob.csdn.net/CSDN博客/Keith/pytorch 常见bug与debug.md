# pytorch: 常见bug与debug - Keith - CSDN博客





2017年08月09日 19:33:26[ke1th](https://me.csdn.net/u012436149)阅读数：6195
所属专栏：[pytorch学习笔记](https://blog.csdn.net/column/details/15023.html)









本博文用来记录自己的 pytorch 踩坑过程，虽然 pytorch 命令式编程，声称容易 debug，可是 代码给出的错误提示可是令人相当头疼，所以在本文中记录一下在编写 pytorch 代码过程中碰到的 坑，和如何 填坑。
- 
`TypeError: ××× received an invalid combination of arguments`

如果检查过了数据类型的正确性之后（float32, int） 。下一步要关心的就是 `op` 操作的两个 `Variable/Tensor` 是不是在同一个 设备上 ，如果一个在 `cpu` 上，一个在 `gpu` 上就有可能会报错

- 
注意 `op` 的参数要求，有些是  要求 `Tensor` 有些 是要求 `Variable` ，有些是 都可以。

- 当需要 求梯度时，一个 op 的两个输入都必须是要 Variable：

```python
# 这段代码，如果 requires_grad=False,  a 是 Tensor，则是没错的
# 但是 requires_grad=True, a 是 Tensor，则会报错
# 这时的报错信息是 
# save_for_backward can only save input or output tensors, but argument 0 doesn't satisfy this condition
# requires_grad=True,  a是 Variable， 则不会报错
import torch
from torch.autograd import Variable

v1 = Variable(torch.FloatTensor([1., 2., 3.]), requires_grad=True)
a = Variable(torch.FloatTensor([1., 0., 0.]).type(new_type=torch.ByteTensor))

res = torch.masked_select(v1, a)
res = 3 * res

res.backward(torch.FloatTensor([1.]))
print(v1.grad)
```
- 卷积层 -> 全连接层，中间一定要 view 一下，否则会shape不匹配



