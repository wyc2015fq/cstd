# PyTorch之八—优化器&学习率 - wsp_1138886114的博客 - CSDN博客





2019年02月15日 20:49:41[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：117











### 文章目录
- [torch.optim 优化器](#torchoptim__1)
- [使用`torch.optim`:](#torchoptim_2)
- [进行单次优化 optimizer.step()](#_optimizerstep_22)
- [使用闭包优化 optimizer.step(closure)](#_optimizerstepclosure_32)
- [常用的优化器函数](#_46)
- [学习率 与 学习率调节](#___96)
- [小案例](#_196)




### torch.optim 优化器

##### 使用`torch.optim`:
- 构建一个`optimizer`对象。这个对象能够保持当前参数状态并基于计算得到的梯度进行参数更新。
- 你需要给它一个包含了需要优化的参数（必须都是`Variable`对象）的iterable。然后，你可以设置optimizer的参 数选项，比如学习率，权重衰减，等等。

```python
例如：
optimizer = optim.SGD(model.parameters(), lr = 0.01, momentum=0.9)
optimizer = optim.Adam([var1, var2], lr = 0.0001)
```

当然 你也可以为每个参数单独设置选项：

不直接传入Variable的iterable，而是传入dict的iterable。

```python
例如，当我们想指定每一层的学习率时，这是非常有用的：
optim.SGD([
               {'params': model.base.parameters()},
               {'params': model.classifier.parameters(), 'lr': 1e-3}
          ], 
          lr=1e-2, momentum=0.9)
```

这意味着model.base的参数将会使用1e-2的学习率，model.classifier的参数将会使用1e-3的学习率，并且0.9的momentum将会被用于所有的参数。

###### 进行单次优化 optimizer.step()

所有的optimizer都实现了step()方法，这个方法会更新所有的参数。一旦梯度被如backward()之类的函数计算好后，我们就可以调用这个函数。

```python
for input, target in dataset:
    optimizer.zero_grad()
    output = model(input)
    loss = loss_fn(output, target)
    loss.backward()
    optimizer.step()
```

###### 使用闭包优化 optimizer.step(closure)

一些优化算法例如Conjugate Gradient和LBFGS需要重复多次计算函数，因此你需要传入一个闭包去允许它们重新计算你的模型。这个闭包应当清空梯度， 计算损失，然后返回。

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

##### 常用的优化器函数

如下优化器我只写了几个常用的，[详情请查看请点击](https://pytorch.org/docs/stable/optim.html)
**`class torch.optim.Adadelta(params, lr=1.0, rho=0.9, eps=1e-06, weight_decay=0)`**
- 参数：
`params (iterable)` – 待优化参数的iterable或者是定义了参数组的dict
`rho (float, 可选)` – 用于计算平方梯度的运行平均值的系数（默认：0.9）
`eps (float, 可选)` – 为了增加数值计算的稳定性而加到分母里的项（默认：1e-6）
`lr (float, 可选)` – 在delta被应用到参数更新之前对它缩放的系数（默认：1.0）
`weight_decay (float, 可选)` – 权重衰减（L2惩罚）（默认: 0）
**`class torch.optim.Adagrad(params, lr=0.01, lr_decay=0, weight_decay=0)`**
- 参数：
`params (iterable)` – 待优化参数的iterable或者是定义了参数组的dict
`lr (float, 可选)` – 学习率（默认: 1e-2）
`lr_decay (float, 可选)` – 学习率衰减（默认: 0）
`weight_decay (float, 可选)` – 权重衰减（L2惩罚）（默认: 0）
**`class torch.optim.Adam(params, lr=0.001, betas=(0.9, 0.999), eps=1e-08, weight_decay=0)`**
- 参数：
`params (iterable)` – 待优化参数的iterable或者是定义了参数组的dict
`lr (float, 可选)` – 学习率（默认：1e-3）
`betas (Tuple[float, float], 可选)` – 用于计算梯度以及梯度平方的运行平均值的系数（默认：0.9，0.999）
`eps (float, 可选)` – 为了增加数值计算的稳定性而加到分母里的项（默认：1e-8）
`weight_decay (float, 可选)` – 权重衰减（L2惩罚）（默认: 0）
**`class torch.optim.SGD(params, lr=<必须参数>, momentum=0, dampening=0, weight_decay=0, nesterov=False)`**

实现随机梯度下降算法（momentum可选）
- 参数：
`params(iterable)` – 待优化参数的iterable或者是定义了参数组的dict
`lr (float)` – 学习率
`momentum (float, 可选)` – 动量因子（默认：0）
`weight_decay (float, 可选)` – 权重衰减（L2惩罚）（默认：0）
`dampening (float, 可选)`– 动量的抑制因子（默认：0）
`nesterov (bool, 可选)` – 使用Nesterov动量（默认：False）
```python
>>> optimizer = torch.optim.SGD(model.parameters(), lr=0.1, momentum=0.9)
>>> optimizer.zero_grad()
>>> loss_fn(model(input), target).backward()
>>> optimizer.step()
```

### 学习率 与 学习率调节

以下的学习率调节，用的较多的是前两个。

在PyTorch中关于调整学习率
**`torch.optim.lr_scheduler`** 提供了几种基于时期数调整学习率的方法

**`torch.optim.lr_scheduler.LambdaLR(optimizer，lr_lambda，last_epoch = -1 )`**

将每个参数组的学习速率设置为给定函数的初始lr倍。当last_epoch = -1时，将初始lr设置为lr。
- 参数：
`optimizer（Optimizer）` - 包装优化器。
`lr_lambda（函数或列表）` - 在给定整数参数epoch的情况下计算乘因子的函数或此类函数的列表，每个组对应一个优化器参数.param_groups。
`last_epoch（int）` - 最后一个epoch的索引。默认值：-1。
```python
# Assuming optimizer has two groups.(确保至少有两个优化组)
lambda1 = lambda epoch: epoch // 30
lambda2 = lambda epoch: 0.95 ** epoch
scheduler = LambdaLR(optimizer, lr_lambda=[lambda1, lambda2])
for epoch in range(100):
    scheduler.step()
    train(...)
    validate(...)
```

**`torch.optim.lr_scheduler.StepLR(optimizer, step_size, gamma=0.1, last_epoch=-1)`**

将每个参数组的学习速率设置为每个step_size epochs由gamma衰减的初始lr。当last_epoch = -1时，将初始lr设置为lr。
- 参数：
optimizer (Optimizer) – 包装优化器。

step_size (int) – 学习率衰减的时期。

gamma (float) – 学习率衰减的乘法因子。默认值：0.1。

last_epoch (int) – 最后一个epoch的索引。 默认值：-1。
```python
>>> # Assuming optimizer uses lr = 0.05 for all groups
>>> # lr = 0.05     if epoch < 30
>>> # lr = 0.005    if 30 <= epoch < 60
>>> # lr = 0.0005   if 60 <= epoch < 90
>>> # ...
>>> scheduler = StepLR(optimizer, step_size=30, gamma=0.1)
>>> for epoch in range(100):
>>>     scheduler.step()
>>>     train(...)
>>>     validate(...)
```

**`torch.optim.lr_scheduler.MultiStepLR(optimizer, milestones, gamma=0.1, last_epoch=-1)`**

将每个参数组的学习速率设置为初始学习速率，当epoch数达到某个里程碑时，该速率将被gamma衰减。当最后一个epoch=-1时，将初始lr设置为lr。
- 参数：
optimizer (Optimizer) – 包装优化器。

milestones (list) – 时期指数列表。必须增加。

gamma (float) – 学习率衰减的乘法因子。默认值：0.1。

last_epoch (int) – 最后一个epoch的索引。 默认值：-1。
```python
>>> # Assuming optimizer uses lr = 0.05 for all groups
>>> # lr = 0.05     if epoch < 30
>>> # lr = 0.005    if 30 <= epoch < 80
>>> # lr = 0.0005   if epoch >= 80
>>> scheduler = MultiStepLR(optimizer, milestones=[30,80], gamma=0.1)
>>> for epoch in range(100):
>>>     scheduler.step()
>>>     train(...)
>>>     validate(...)
```

**`torch.optim.lr_scheduler.ExponentialLR(optimizer, gamma, last_epoch=-1)`**

将每个参数组的学习速率设置为每个历代伽马衰减的初始学习速率。当最后一个epoch=-1时，将初始lr设置为lr。
- 参数：
optimizer （Optimizer） - 包装优化器。

gamma（float） - 学习率衰减的乘法因子。

last_epoch（int） - 上一个纪元的索引。默认值：-1。
**`torch.optim.lr_scheduler.CosineAnnealingLR(optimizer, T_max, eta_min=0, last_epoch=-1)`**

关于余弦退火计划（调度器）学习率曲线[请点击](https://blog.csdn.net/wsp_1138886114/article/details/86547243)

使用余弦退火计划设置每个参数组的学习速率，其中，$\eta_{max}$ 设置为初始学习速率，$T_{cur}$ 是自上次在SGDR中重新启动以来的 epoch 数：

$$\eta_t = \eta_{min} + \frac{1}{2}(\eta_{max} - \eta_{min})(1 + \cos(\frac{T_{cur}}{T_{max}}\pi))$$

当 last_epoch = -1时，将初始lr设置为lr。
- 参数：
optimizer （Optimizer） - 包装优化器。

T_max（int） - 最大迭代次数。

eta_min（浮动） - 最低学习率。默认值：0。

last_epoch（int） - 上一个纪元的索引。默认值：-1。
关于其他的学习率调度器请查看 [https://pytorch.org/docs/stable/optim.html](https://pytorch.org/docs/stable/optim.html)

### 小案例

```python
import torch
import torch.nn.functional as F
from torch.autograd import Variable
import matplotlib.pyplot as plt

# 超参数
torch.manual_seed(1)
LR = 0.01
Batch_size = 24
Epoch = 24

# 定义数据集
x = torch.unsqueeze(torch.linspace(-1,1,1000),dim=1)
y = x**2 + 0.1*torch.normal(torch.zeros(x.size()))

# 先转换成torch能识别的Dataset >> 再使用数据加载器加载数据
data_set = torch.utils.data.TensorDataset(x,y)
dataset_loader = torch.utils.data.DataLoader(dataset = data_set,
                                             batch_size = Batch_size,
                                             shuffle = True,
                                             num_workers = 2,)
# 定义神经网络
class Net(torch.nn.Module):
    def __init__(self):
        super(Net,self).__init__()
        self.hidden = torch.nn.Linear(1,20)
        self.predict = torch.nn.Linear(20,1)

    def forward(self, input):
        x = F.relu(self.hidden(input))
        x = self.predict(x)
        return x

net_SGD = Net()
net_Momentum = Net()
net_RMSprop = Net()
net_Adam = Net()
nets = [net_SGD,net_Momentum,net_RMSprop,net_Adam]

# 定义优化器（学习率相同）
opt_SGD         = torch.optim.SGD(net_SGD.parameters(), lr=LR)
opt_Momentum    = torch.optim.SGD(net_Momentum.parameters(), lr=LR, momentum=0.8)
opt_RMSprop     = torch.optim.RMSprop(net_RMSprop.parameters(), lr=LR, alpha=0.9)
opt_Adam        = torch.optim.Adam(net_Adam.parameters(), lr=LR, betas=(0.9, 0.99))
optimizers = [opt_SGD, opt_Momentum, opt_RMSprop, opt_Adam] # 放在list里面，可以用个for循环遍历

#回归误差
loss_func = torch.nn.MSELoss()
losses_his = [[], [], [], []]                  # 记录 training 时不同神经网络的 loss


if __name__ == '__main__':
    for epoch in range(Epoch):
        print('Epoch: ', epoch)
        for step, (batch_x, batch_y) in enumerate(dataset_loader):
            b_x = Variable(batch_x)            # 包装成Variable
            b_y = Variable(batch_y)

            # 对每个优化器, 优化属于他的神经网络
            for net, opt, l_his in zip(nets, optimizers, losses_his):  # 三个都是list形式zip打包处理
                output = net(b_x)              # get output for every net
                loss = loss_func(output, b_y)  # compute loss for every net
                opt.zero_grad()                # clear gradients for next train
                loss.backward()                # backpropagation, compute gradients
                opt.step()                     # apply gradients
                l_his.append(loss.data.item()) # loss recoder

        labels = ['SGD', 'Momentum', 'RMSprop', 'Adam']
        for i, l_his in enumerate(losses_his):
            plt.plot(l_his, label=labels[i])
        plt.legend(loc='best')
        plt.xlabel('Steps')
        plt.ylabel('Loss')
        plt.ylim(0, 0.2)
        plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190214212037109.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)



