# pytorch系列13 ---优化算法optim类 - 墨流觞的博客 - CSDN博客





2018年12月01日 14:10:13[墨氲](https://me.csdn.net/dss_dssssd)阅读数：334
所属专栏：[python3 pytorch0.4系列教程](https://blog.csdn.net/column/details/29714.html)









本文主要讲解pytorch中的optim累以及lr_schdule类和optim优化器的配置，使用方法。

在[https://blog.csdn.net/dss_dssssd/article/details/83892824](https://blog.csdn.net/dss_dssssd/article/details/83892824)中提及优化算法的使用步骤，
- 
`optimer = optim.SGD()` 先初始化

- 
反向传播更新参数
- 将上次迭代计算的梯度值清0
`optimizer.zero_grad()`
- 反向传播，计算梯度值
`loss.backward()`
- 更新权值参数
`optimizer.step()`

## 一. 使用optimizer的步骤

SGD源码： [https://pytorch.org/docs/stable/_modules/torch/optim/sgd.html](https://pytorch.org/docs/stable/_modules/torch/optim/sgd.html)

### 1. 构造optimizer

##### 1.1 初始化函数`__init__`:

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181201154659740.png)

参数：
- 
params： 包含参数的可迭代对象，必须为Tensor

- 
其余的参数来配置学习过程中的行为


```python
optimizer = optim.SGD(model.parameters(), lr = 0.01, momentum=0.9)
optimizer = optim.Adam([var1, var2], lr = 0.0001)
```

##### 1.2. per-parameter 选择

不是传入Tensor的可迭代对象，而是传入
`dict`的可迭代对象，每一个字典定义一个独立的参数组，每一个`dict`必须包含一个`params`键，和一系列与该优化函数对应的参数。

```python
optim.SGD([
	                {'params': model.base.parameters()},
	                {'params': model.classifier.parameters(), 'lr': 1e-3}
	            ], lr=1e-2, momentum=0.9)
```

上述代码，`model.base`中的参数更新使用`learning rate`为`1e-2`的SGD算法，而`model.classifier`中的参数更新使用`learning rate` 为 `1e-2`, momentum为`0.9`的SGD算法

下面用一个两层的线性回归的例子说明一下：

输入为[11, 1], 第一层为[1, 10], 第二层为[10, 1],最后的输出为[11,1]。在优化算法中，第一层和第二层分别使用不同的优化器配置方案。

```python
class model_(nn.Module):

    def __init__(self, in_dim):
        super().__init__()
        self.first_layer = nn.Sequential(
            nn.Linear(in_dim, 10)
        )

        self.second_layer = nn.Sequential(
            nn.Linear(10, 1)
        )


    def forward(self, x):
        x = F.Relu(self.first_layer(x))
        x = self.second_layer(x)
        return x


x = np.arange(1, 11).reshape(-1, 1)
x = torch.from_numpy(x)
model =model_(1)
```

使用`parameters()`打印出weight和bias，注意函数返回的是迭代器

```python
print("the first params:")
# 返回的是迭代器
for param in model.first_layer.parameters():
    print(param)

print("the second params:")
print(list(model.second_layer.parameters()))
```

out:

> 
the first params:

Parameter containing:

tensor([[ 0.7895],

[ 0.4697],

[-0.0534],

[ 0.8223],

[ 0.9414],

[-0.5877],

[-0.4069],

[-0.6598],

[-0.1982],

[-0.2233]], requires_grad=True)

Parameter containing:

tensor([-0.9338, -0.9721,  0.3418, -0.4599,  0.1251, -0.2313,  0.9735, -0.1804,

0.0935,  0.9205], requires_grad=True)

the second params:

[Parameter containing:

tensor([[-0.0765, -0.1632,  0.0979,  0.2206, -0.0102, -0.0452, -0.3096,  0.0189,

-0.0240, -0.0900]], requires_grad=True), Parameter containing:

tensor([-0.1148], requires_grad=True)]
在接收了外部输入的参数params以后，优化器会把params存在self.param_groups里（**是一个字典的列表**）。每一个字典除了保存参数的[‘params’]键以外，Optimizer还维护着其他的优化系数，例如学习率和衰减率等

```python
optimizer = optim.SGD(
    [
     {'params': model.first_layer.parameters()},
	{'params': model.second_layer.parameters(), 'lr': 1e-3}
	],
     lr=1e-2, momentum=0.9)

print(optimizer.param_groups)
```

out：

> 
[**{‘params’**: [Parameter containing:

tensor([[ 0.7895],

[ 0.4697],

[-0.0534],

[ 0.8223],

[ 0.9414],

[-0.5877],

[-0.4069],

[-0.6598],

[-0.1982],

[-0.2233]], requires_grad=True), Parameter containing:

tensor([-0.9338, -0.9721,  0.3418, -0.4599,  0.1251, -0.2313,  0.9735, -0.1804,

0.0935,  0.9205], requires_grad=True)], ‘lr’: 0.01, ‘momentum’: 0.9, ‘dampening’: 0, ‘weight_decay’: 0, ‘nesterov’: False},
**{‘params’:** [Parameter containing:

tensor([[-0.0765, -0.1632,  0.0979,  0.2206, -0.0102, -0.0452, -0.3096,  0.0189,

-0.0240, -0.0900]], requires_grad=True), Parameter containing:

tensor([-0.1148], requires_grad=True)], ‘lr’: 0.001, ‘momentum’: 0.9, ‘dampening’: 0, ‘weight_decay’: 0, ‘nesterov’: False}]
### 2. optimizer.zero_grade()

在所有的优化算法的基类`Optimizer`[https://pytorch.org/docs/stable/_modules/torch/optim/optimizer.html](https://pytorch.org/docs/stable/_modules/torch/optim/optimizer.html)中的`zero_grad`函数对`self.param_group`中的所有的params清零

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181201120847560.png)

### 3. loss.backward()

### 4. optimizer.step()

##### 4.1. 直接调用step()而不传入`closure`参数，大多数优化算法会用这种方法调用ok，因为只涉及到一次调用

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181201132403563.png)

这个方法在Optimizer中是未定义，根据具体的优化器来单独实现。

以下通过SGD类中的`step()`函数来说明：
```python
def step(self, closure=None):
        """Performs a single optimization step.

        Arguments:
            closure (callable, optional): A closure that reevaluates the model
                and returns the loss.
        """
        loss = None
        if closure is not None:
            loss = closure()

        for group in self.param_groups:
            weight_decay = group['weight_decay']
            momentum = group['momentum']
            dampening = group['dampening']
            nesterov = group['nesterov']

            for p in group['params']:
                if p.grad is None:
                    continue
                d_p = p.grad.data
                if weight_decay != 0:
                    d_p.add_(weight_decay, p.data)
                if momentum != 0:
                    param_state = self.state[p]
                    if 'momentum_buffer' not in param_state:
                        buf = param_state['momentum_buffer'] = torch.zeros_like(p.data)
                        buf.mul_(momentum).add_(d_p)
                    else:
                        buf = param_state['momentum_buffer']
                        buf.mul_(momentum).add_(1 - dampening, d_p)
                    if nesterov:
                        d_p = d_p.add(momentum, buf)
                    else:
                        d_p = buf

                p.data.add_(-group['lr'], d_p)

        return loss
```

取出每个group中的params进行更新，

`d_p = p.grad.data`将`loss.backward()`计算的梯度取出，通过weight_decay,momentum，learning_rate等优化器配置方法更新`d_p`的值，最后赋值给`p`。

##### 4.2 对于Conjugate Gradient 和 LBFGS等优化算法，须有多次评估优化函数，因而要传入`closure`来多次计算模型。`closure`完成的操作：
- 清楚梯度值
- 计算损失loss
- 返回loss

```python
for input, target in dataset:
	def closure():
		optimizer.zero_grad()
	        output = model(input)
	        loss = loss_fn(output, target)
	        loss.backward()
	        return loss
	 optimizer.step(closure)
```









