# PyTorch 更改训练好的模型 继续训练 - Snoopy_Dream - CSDN博客





2018年12月27日 10:33:52[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：636
个人分类：[pytorch](https://blog.csdn.net/e01528/article/category/8254835)









**目录**

[直接加载预训练模型](#%E7%9B%B4%E6%8E%A5%E5%8A%A0%E8%BD%BD%E9%A2%84%E8%AE%AD%E7%BB%83%E6%A8%A1%E5%9E%8B)

[加载部分预训练模型](#%E5%8A%A0%E8%BD%BD%E9%83%A8%E5%88%86%E9%A2%84%E8%AE%AD%E7%BB%83%E6%A8%A1%E5%9E%8B)

[冻结部分参数，训练另一部分参数](#%E5%86%BB%E7%BB%93%E9%83%A8%E5%88%86%E5%8F%82%E6%95%B0%EF%BC%8C%E8%AE%AD%E7%BB%83%E5%8F%A6%E4%B8%80%E9%83%A8%E5%88%86%E5%8F%82%E6%95%B0)

[微改基础模型预训练](#%E5%BE%AE%E6%94%B9%E5%9F%BA%E7%A1%80%E6%A8%A1%E5%9E%8B%E9%A2%84%E8%AE%AD%E7%BB%83)

[微改基础模型](#%E5%BE%AE%E6%94%B9%E5%9F%BA%E7%A1%80%E6%A8%A1%E5%9E%8B)

[简单预训练](#%E7%AE%80%E5%8D%95%E9%A2%84%E8%AE%AD%E7%BB%83)

# 直接加载预训练模型

如果我们使用的模型和原模型完全一样，那么我们可以直接加载别人训练好的模型：

```
my_resnet = MyResNet(*args, **kwargs)
my_resnet.load_state_dict(torch.load("my_resnet.pth"))
```

当然这样的加载方法是基于PyTorch推荐的存储模型的方法：

```
torch.save(my_resnet.state_dict(), "my_resnet.pth")
```

还有第二种加载方法：

```
my_resnet = torch.load("my_resnet.pth")
```

# 加载部分预训练模型

其实大多数时候我们需要根据我们的任务调节我们的模型，所以很难保证模型和公开的模型完全一样，但是预训练模型的参数确实有助于提高训练的准确率，为了结合二者的优点，就需要我们加载部分预训练模型。

```python
pretrained_dict = model_zoo.load_url(model_urls['resnet152'])//预训练的模型
model_dict = model.state_dict()//你现有的模型，比方说你的模型随机初始化的权重参数
# 将pretrained_dict里不属于model_dict的键剔除掉
pretrained_dict = {k: v for k, v in pretrained_dict.items() if k in model_dict}#如果你的k在预备训练当中，那么你的参数需要做转换，否则为原先的
# 更新现有的model_dict
model_dict.update(pretrained_dict)//利用预训练模型的参数，更新你的模型
# 加载我们真正需要的state_dict
model.load_state_dict(model_dict)
```



来个我的实际例子把，上面还不是很清楚。下面的情况是我用resnet20训练完后，时间不够了，想在这基础上训练resnet34的，当然这个需要你把resnet模型内部的名称keys，弄好，搞成新加层的形式。

```python
pre_model = se_resnet20_v2() #之前的模型结构的初始化

pre_model.load_state_dict(torch.load('/home/zj/senetial/save_models/SENet20/SEnet_model_51.pth')) #之前的模型的加载

model=se_resnet34_v2() #现在模型结构

pre_dict = {k: v for k, v in pre_model.items() if k in model.state_dict()} #把resnet20的参数放进去，其他的参数还是对应的随机初始化的参数

model.load_state_dict(model.state_dict().update(pre_dict))#加载模型（模型的随机初始化模型（部分更新为预训练的模型））
```

因为需要剔除原模型中不匹配的键，也就是层的名字，所以我们的新模型改变了的层需要和原模型对应层的名字不一样，比如：resnet最后一层的名字是fc(PyTorch中)，那么我们修改过的resnet的最后一层就不能取这个名字，可以叫fc_

# 冻结部分参数，训练另一部分参数

1）添加下面一句话到模型中

```
for p in self.parameters():
    p.requires_grad = False
```

比如加载了resnet预训练模型之后，在resenet的基础上连接了新的模快，**resenet模块那部分**可以先暂时冻结不更新，只更新其他部分的参数，那么可以在下面加入上面那句话

```python
class RESNET_MF(nn.Module):
    def __init__(self, model, pretrained):
        super(RESNET_MF, self).__init__()
        self.resnet = model(pretrained)
        for p in self.parameters():
            p.requires_grad = False
        self.f = SpectralNorm(nn.Conv2d(2048, 512, 1))
        self.g = SpectralNorm(nn.Conv2d(2048, 512, 1))
        self.h = SpectralNorm(nn.Conv2d(2048, 2048, 1))
        ...
```

同时在优化器中添加：**filter(lambda p: p.requires_grad, model.parameters())**

```python
#过滤掉requires_grad=false的层
optimizer = optim.Adam(filter(lambda p: p.requires_grad, model.parameters()), lr=0.001, betas=(0.9, 0.999),
                               eps=1e-08, weight_decay=1e-5)#
```

2) 参数保存在有序的字典中，那么**可以通过查找参数的名字对应的id值**，进行冻结

查找的代码：

```python
model_dict = torch.load('net.pth.tar').state_dict()
    dict_name = list(model_dict)
    for i, p in enumerate(dict_name):
        print(i, p)
```

保存一下这个文件，可以看到大致是这个样子的：

```
0 gamma
1 resnet.conv1.weight
2 resnet.bn1.weight
3 resnet.bn1.bias
4 resnet.bn1.running_mean
5 resnet.bn1.running_var
6 resnet.layer1.0.conv1.weight
7 resnet.layer1.0.bn1.weight
8 resnet.layer1.0.bn1.bias
9 resnet.layer1.0.bn1.running_mean
....
```

同样在模型中添加这样的代码：

```python
for i,p in enumerate(net.parameters()):
    if i < 165:
        p.requires_grad = False
```

在优化器中添加上面的那句话可以实现参数的屏蔽

另外一个**小技巧**就是在nn.Module里，可以在中间插入这个

```python
for p in self.parameters():
    p.requires_grad=False
```



这样前面的参数就是False，而后面的不变

```python
class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.conv1 = nn.Conv2d(1, 6, 5)
        self.conv2 = nn.Conv2d(6, 16, 5)

        for p in self.parameters():
            p.requires_grad=False

        self.fc1 = nn.Linear(16 * 5 * 5, 120)
        self.fc2 = nn.Linear(120, 84)
        self.fc3 = nn.Linear(84, 10)
```




# 微改基础模型预训练

对于改动比较大的模型，我们可能需要自己实现一下再加载别人的预训练参数。但是，对于一些基本模型PyTorch中已经有了，而且我只想进行一些小的改动那么怎么办呢？难道我又去实现一遍吗？当然不是。

我们首先看看怎么进行微改模型。

## 微改基础模型

PyTorch中的torchvision里已经有很多常用的模型了，可以直接调用：
- AlexNet
- VGG
- ResNet
- SqueezeNet
- DenseNet

```
import torchvision.models as models
resnet18 = models.resnet18()
alexnet = models.alexnet()
squeezenet = models.squeezenet1_0()
densenet = models.densenet_161()
```

但是对于我们的任务而言有些层并不是直接能用，需要我们微微改一下，比如，resnet最后的全连接层是分1000类，而我们只有21类；又比如，resnet第一层卷积接收的通道是3， 我们可能输入图片的通道是4，那么可以通过以下方法修改：

```
resnet.conv1 = nn.Conv2d(4, 64, kernel_size=7, stride=2, padding=3, bias=False)
resnet.fc = nn.Linear(2048, 21)
```

## 简单预训练

模型已经改完了，接下来我们就进行简单预训练吧。 

我们先从torchvision中调用基本模型，加载预训练模型，然后，重点来了，**将其中的层直接替换为我们需要的层即可**：

```
resnet = torchvision.models.resnet152(pretrained=True)
# 原本为1000类，改为10类
resnet.fc = torch.nn.Linear(2048, 10)
```

其中使用了pretrained参数，会直接加载预训练模型，内部实现和前文提到的加载预训练的方法一样。因为是先加载的预训练参数，相当于模型中已经有参数了，所以替换掉最后一层即可。OK！

if pretrained:这里可以看出这里的pretrained其实就和上面的一样

        model.load_state_dict(model_zoo.load_url(model_urls['alexnet']))





