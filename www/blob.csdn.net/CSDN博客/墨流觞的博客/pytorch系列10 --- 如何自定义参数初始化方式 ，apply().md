# pytorch系列10 --- 如何自定义参数初始化方式 ，apply() - 墨流觞的博客 - CSDN博客





2018年11月12日 15:26:28[墨氲](https://me.csdn.net/dss_dssssd)阅读数：2082
所属专栏：[python3 pytorch0.4系列教程](https://blog.csdn.net/column/details/29714.html)









本文主要内容：
- 单层网络初始化
- 多层网络初始化
- 使用apply和weight_init函数
- 在`__init__`函数使用`self.modules()`初始化


### 1.单层网络
- 在创建model后直接调用`torch.nn.innit`里的初始化函数

```python
layer1 = torch.nn.Linear(10,20)
torch.nn.init.xavier_uniform_(layer1.weight)
torch.nn.init.constant_(layer1.bias, 0)
```
- 也可以重写`reset_parameters()`方法，并不推荐

### 2. 使用tf.Squential 或自定义多层网络

tf.Sequential() ,请查看[pytorch系列7 -----nn.Sequential讲解](https://blog.csdn.net/dss_dssssd/article/details/82980222)
- 使用[torch.nn.Module.apply](https://pytorch.org/docs/master/nn.html#torch.nn.Module.apply).

`apply(fn)`： 看一下`apply`在Module的实现。![在这里插入图片描述](https://img-blog.csdnimg.cn/20181112142059482.png)

将函数`fn`递归的运用在每个子模块上，这些子模块由`self.children()`返回.
**关于self.children()和self.modules()的工作方式，请查看此篇博文**[pytorch系列8 --self.modules() 和 self.children()的区别](https://blog.csdn.net/dss_dssssd/article/details/83958518)

常被用来初始化网络层参数。注**意fn需要一个参数。**
具体步骤是：
- 定义weight_init函数，并在weight_init中通过判断模块的类型来进行不同的参数初始化定义类型。
- model=Net(…) 创建网络结构
- model.apply(weight_init),将weight_init初始化方式应用到submodels上

在以下的代码中只初始化线性层，**至于卷积层，批归一化层后面会讲到的**

示例：[https://github.com/pytorch/examples/blob/master/dcgan/main.py#L95](https://github.com/pytorch/examples/blob/master/dcgan/main.py#L95)

```python
# -*- coding: utf-8 -*-

import torch
from torch import nn

# hyper parameters
in_dim=1
n_hidden_1=1
n_hidden_2=1
out_dim=1

class Net(nn.Module):
    
    
    def __init__(self, in_dim, n_hidden_1, n_hidden_2, out_dim):
        super().__init__()

        self.layer = nn.Sequential(
            nn.Linear(in_dim, n_hidden_1), 
            nn.ReLU(True),
            nn.Linear(n_hidden_1, n_hidden_2),
            nn.ReLU(True),
            nn.Linear(n_hidden_2, out_dim)
            
             )    
    def forward(self, x):
        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)
        return x
 
 # 1. 根据网络层的不同定义不同的初始化方式     
def weight_init(m):
    if isinstance(m, nn.Linear):
        nn.init.xavier_normal_(m.weight)
        nn.init.constant_(m.bias, 0)
    # 也可以判断是否为conv2d，使用相应的初始化方式 
    elif isinstance(m, nn.Conv2d):
        nn.init.kaiming_normal_(m.weight, mode='fan_out', nonlinearity='relu')
     # 是否为批归一化层
    elif isinstance(m, nn.BatchNorm2d):
        nn.init.constant_(m.weight, 1)
        nn.init.constant_(m.bias, 0)
# 2. 初始化网络结构        
model = Net(in_dim, n_hidden_1, n_hidden_2, out_dim)
# 3. 将weight_init应用在子模块上
model.apply(weight_init)
```

**注意：**此种初始化方式采用的**递归**，*而在python中，对递归层数是有限制的，所以当网络结构很深时，可能会**递归层数过深**的错误*
- 在`__init__`中迭代循环`self.modules()`来初始化网络参数

此种方法的官方实例：[https://github.com/pytorch/vision/blob/master/torchvision/models/resnet.py#L112-L118](https://github.com/pytorch/vision/blob/master/torchvision/models/resnet.py#L112-L118)

**初始化后并使用apply函数将参数信息打印出来**

```python
import torch
from torch import nn

# hyper parameters
in_dim=1
n_hidden_1=1
n_hidden_2=1
out_dim=1

class Net(nn.Module):
    
    
    def __init__(self, in_dim, n_hidden_1, n_hidden_2, out_dim):
        super().__init__()

        self.layer = nn.Sequential(
            nn.Linear(in_dim, n_hidden_1), 
            nn.ReLU(True),
            nn.Linear(n_hidden_1, n_hidden_2),
            nn.ReLU(True),
            nn.Linear(n_hidden_2, out_dim)
            
             )
        # 迭代循环初始化参数
        for m in self.modules():
            if isinstance(m, nn.Linear):
                nn.init.constant_(m.weight, 1)
                nn.init.constant_(m.bias, -100)
            # 也可以判断是否为conv2d，使用相应的初始化方式 
            elif isinstance(m, nn.Conv2d):
                nn.init.kaiming_normal_(m.weight, mode='fan_out', nonlinearity='relu')
             
            elif isinstance(m, nn.BatchNorm2d):
                nn.init.constant_(m.weight.item(), 1)
                nn.init.constant_(m.bias.item(), 0)    
    def forward(self, x):
        x = self.layer(x)

        return x      
model = Net(in_dim, n_hidden_1, n_hidden_2, out_dim)

# 打印参数信息
def print_weight(m):
    if isinstance(m, nn.Linear):
        print("weight", m.weight.item())
        print("bias:", m.bias.item())
        print("next...")
model.apply(print_weight)
```

out:

> 
weight 1.0

bias: -100.0

next…
weight 1.0

bias: -100.0

next…
weight 1.0

bias: -100.0

next…
### 如果不想使用官方提供的Xavier和kaiming初始化呢？

自己计算std,然后调用`torch.nn.init.normal_(tensor, maen, std)`就OK了。

至于官方的kaiming和Xavier是如何计算fan_in(输入神经元个数)和fan_out(输出神经元个数),参考这篇博客[https://blog.csdn.net/dss_dssssd/article/details/83992701](https://blog.csdn.net/dss_dssssd/article/details/83992701)

还需要注意一点，**在pytorch 0.4中不存在Variable了，所以weight和bias就直接是tensor类型了。**
参考文章：
- [https://stackoverflow.com/questions/49433936/how-to-initialize-weights-in-pytorch](https://stackoverflow.com/questions/49433936/how-to-initialize-weights-in-pytorch)









