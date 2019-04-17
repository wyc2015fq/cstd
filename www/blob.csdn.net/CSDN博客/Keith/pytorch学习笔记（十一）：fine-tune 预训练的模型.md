# pytorch学习笔记（十一）：fine-tune 预训练的模型 - Keith - CSDN博客





2017年09月20日 10:42:59[ke1th](https://me.csdn.net/u012436149)阅读数：14256
所属专栏：[pytorch学习笔记](https://blog.csdn.net/column/details/15023.html)









torchvision 中包含了很多预训练好的模型，这样就使得 fine-tune 非常容易。本文主要介绍如何 fine-tune torchvision 中预训练好的模型。

## 安装

```
pip install torchvision
```

## 如何 fine-tune

以 resnet18 为例：

```python
from torchvision import models
from torch import nn
from torch import optim

resnet_model = models.resnet18(pretrained=True) 
# pretrained 设置为 True，会自动下载模型 所对应权重，并加载到模型中
# 也可以自己下载 权重，然后 load 到 模型中，源码中有 权重的地址。

# 假设 我们的 分类任务只需要 分 100 类，那么我们应该做的是
# 1. 查看 resnet 的源码
# 2. 看最后一层的 名字是啥 （在 resnet 里是 self.fc = nn.Linear(512 * block.expansion, num_classes)）
# 3. 在外面替换掉这个层
resnet_model.fc= nn.Linear(in_features=..., out_features=100)

# 这样就 哦了，修改后的模型除了输出层的参数是 随机初始化的，其他层都是用预训练的参数初始化的。

# 如果只想训练 最后一层的话，应该做的是：
# 1. 将其它层的参数 requires_grad 设置为 False
# 2. 构建一个 optimizer， optimizer 管理的参数只有最后一层的参数
# 3. 然后 backward， step 就可以了

# 这一步可以节省大量的时间，因为多数的参数不需要计算梯度
for para in list(resnet_model.parameters())[:-1]:
    para.requires_grad=False 

optimizer = optim.SGD(params=[resnet_model.fc.weight, resnet_model.fc.bias], lr=1e-3)

...
```

## 为什么

这里介绍下 运行`resnet_model.fc= nn.Linear(in_features=..., out_features=100)`时 框架内发生了什么

这时应该看 `nn.Module` 源码的 `__setattr__` 部分，因为 `setattr` 时都会调用这个方法：

```python
def __setattr__(self, name, value):
    def remove_from(*dicts):
        for d in dicts:
            if name in d:
                del d[name]
```

首先映入眼帘就是 `remove_from` 这个函数，这个函数的目的就是，如果出现了 同名的属性，就将旧的属性移除。 用刚才举的例子就是：
- 预训练的模型中 有个  名字叫`fc` 的 Module。
- 在类定义外，我们 将另一个 Module 重新 赋值给了 `fc`。
- 类定义内的 `fc` 对应的 Module 就会从 模型中 删除。



