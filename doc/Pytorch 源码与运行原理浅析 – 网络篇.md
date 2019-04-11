# Pytorch 源码与运行原理浅析 – 网络篇

作者: [PyTorch 中文网	](https://www.pytorchtutorial.com/author/zlmoment/)发布: 2017年8月31日	 6,337*阅读* [ 0*评论*](https://www.pytorchtutorial.com/pytorch-source-code-reading-network/#respond)



记得当时刚刚接触的时候一直搞不明白，为什么自己只是定义了几个网络，就可以完整的训练整个模型，它背后的机制又是如何，搞明白了这个，才有可能去做更多的定制的更改，比如更改loss，反传方式，梯度下降机制，甚至自定义参数更新速率(比如学习率随着迭代轮数下降)，文章比较浅显，希望各位大神不吝赐教。

文章目录 [[隐藏](https://www.pytorchtutorial.com/pytorch-source-code-reading-network/#)]

- [1 知识储备](https://www.pytorchtutorial.com/pytorch-source-code-reading-network/#i)
- [2 为什么不用TensorFlow](https://www.pytorchtutorial.com/pytorch-source-code-reading-network/#TensorFlow)
- 3 从MNIST说起
  - [3.1 网络定义篇](https://www.pytorchtutorial.com/pytorch-source-code-reading-network/#i-2)
  - [3.2 torch.nn.Module](https://www.pytorchtutorial.com/pytorch-source-code-reading-network/#torchnnModule)
- [4 结语](https://www.pytorchtutorial.com/pytorch-source-code-reading-network/#i-3)

## 知识储备

看此文章的前提，大概需要你写过一个利用pytorch的训练程序，哪怕官网上的MNIST。

因为本文目的是告诉你为什么这么写

## 为什么不用TensorFlow

其实我之前是有用TF的，但是，emmmmmmmm…….

之后接触了Pytorch，那一整天都在感叹”还有这种操作？”

个人感觉TF不是一个易于理解和易于扩展的框架。

比如说，我想实现学习率随迭代轮数降低，需要修改哪些？

那么，让我们开始吧

## 从MNIST说起

### 网络定义篇

```
import torch.nn as nn
class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.conv1 = nn.Conv2d(1, 10, kernel_size=5)
        self.conv2 = nn.Conv2d(10, 20, kernel_size=5)
        self.conv2_drop = nn.Dropout2d()
        self.fc1 = nn.Linear(320, 50)
        self.fc2 = nn.Linear(50, 10)
 
    def forward(self, x):
        x = F.relu(F.max_pool2d(self.conv1(x), 2))
        x = F.relu(F.max_pool2d(self.conv2_drop(self.conv2(x)), 2))
        x = x.view(-1, 320)
        x = F.relu(self.fc1(x))
        x = F.dropout(x, training=self.training)
        x = self.fc2(x)
        return F.log_softmax(x)
```

这一段是MNIST给的定义Net的代码，那么，让我们看一看，这一段代码说明了什么，首先，__init__方法直接定义了你的网络，这就是你的模型中含有的全部的东西，你的模型本身也只有__init__ 中的属性会被每一次训练的时候更改，可以说这个思路是十分的清晰了。



之后，是forward方法，这里定义了如何处理传入的数据(就是那个x)，返回这个神经网络的output

这里，我把它比作名词和动词的关系，__init__()方法定义了网络本身，或者说定义了一个个的名词，而我们也需要一系列的”猜测”过程，猜出这些名词是什么。而forward()方法，则是一个个的动词，它提供了如何处理这些名词的方式。

而之后，我们来看看，运行的时候，发生了什么

首先，我们看看torch.nn.Module，看看它是如何定义的。

### torch.nn.Module

```
class Module(object):
    dump_patches = False

    def __init__(self):
        self._backend = thnn_backend
        self._parameters = OrderedDict()
        self._buffers = OrderedDict()
        self._backward_hooks = OrderedDict()
        self._forward_hooks = OrderedDict()
        self._forward_pre_hooks = OrderedDict()
        self._modules = OrderedDict()
        self.training = True

    def forward(self, *input):
        raise NotImplementedError
```



(代码不完整，只截取了一段)



可以看到，Module类定义了一系列训练时使用的变量比如参数(感觉这是是缓存的参数，用来之后做参数更新用的)，buffers，几个hooks(个人感觉这些hooks是之后与loss，反传之类的步骤通讯数据用的)

反传里面是有一个判断的逻辑，判断你的子类有没有定义网络，没有就报错(讲真，这个想法很棒啊QwQ，子类重写父类方法，没有重写就是个报错hhhhhh)

```
def register_buffer(self, name, tensor):
    self._buffers[name] = tensor
 
def register_parameter(self, name, param):
    if '_parameters' not in self.__dict__:
        raise AttributeError(
            "cannot assign parameter before Module.__init__() call")
    if param is None:
        self._parameters[name] = None
    elif not isinstance(param, Parameter):
        raise TypeError("cannot assign '{}' object to parameter '{}' "
                        "(torch.nn.Parameter or None required)"
                        .format(torch.typename(param), name))
    elif param.grad_fn:
        raise ValueError(
            "Cannot assign non-leaf Variable to parameter '{0}'. Model "
            "parameters must be created explicitly. To express '{0}' "
            "as a function of another variable, compute the value in "
            "the forward() method.".format(name))
    else:
        self._parameters[name] = param
```



buffer和parameter的注册，这里有一点需要提醒，在你自定义的网络中，如果你用了类似

```
self.some_dict['keys'] = nn.Conv2d(10, 20, kernel_size=5)
```

这种语句的话，pytorch是没有办法这个变量的，也不会参与之后的传参之类的

在定义了上面那句话之后你必须用类似

```
# method 1
setattr(self, 'some_name', self.some_dict['keys'])
# method 2
self.register_parameter('some_name', self.some_dict['keys'])
```

比如笔者自己的代码

```
self.LocalConv1 = {i + 1: nn.Conv2d(32, 32, 3, stride=1, padding=0) for i in range(4)}
    for i in self.LocalConv1:
        setattr(self, 'LocalConvPart%d' % i, self.LocalConv1[i])
    self.GlobalFullConnect = nn.Linear(7 * 2 * 32, 400)
    self.LocalFullConnect = {i + 1: nn.Linear(32 * 23 * 16, 100) for i in range(4)}
    for i in self.LocalFullConnect:
        setattr(self, 'LocalFullConnectPart%d' % i, self.LocalFullConnect[i])
```

建议使用方法1，因为Module类重载了__setattr__()方法，如下

```
def __setattr__(self, name, value):
    def remove_from(*dicts):
        for d in dicts:
            if name in d:
                del d[name]
 
    params = self.__dict__.get('_parameters')
    if isinstance(value, Parameter):
        if params is None:
            raise AttributeError(
                "cannot assign parameters before Module.__init__() call")
        remove_from(self.__dict__, self._buffers, self._modules)
        self.register_parameter(name, value)
    elif params is not None and name in params:
        if value is not None:
            raise TypeError("cannot assign '{}' as parameter '{}' (torch.nn.Parameter or None expected)".format(torch.typename(value), name))
            self.register_parameter(name, value)
    else:
        modules = self.__dict__.get('_modules')
        if isinstance(value, Module):
            if modules is None:
                raise AttributeError(
                    "cannot assign module before Module.__init__() call")
                remove_from(self.__dict__, self._parameters, self._buffers)
                modules[name] = value
        elif modules is not None and name in modules:
            if value is not None:
                raise TypeError("cannot assign '{}' as child module '{}' "
                                "(torch.nn.Module or None expected)"
                                    .format(torch.typename(value), name))
            modules[name] = value
        else:
            buffers = self.__dict__.get('_buffers')
            if buffers is not None and name in buffers:
                if value is not None and not torch.is_tensor(value):
                    raise TypeError("cannot assign '{}' as buffer '{}' "
                                        "(torch.Tensor or None expected)"
                                        .format(torch.typename(value), name))
                buffers[name] = value
            else:
                object.__setattr__(self, name, value)
```

其实差别不大，可以看到加了很多判断。

然后之后apply()方法

```
def _apply(self, fn):
    for module in self.children():
        module._apply(fn)
 
    for param in self._parameters.values():
        if param is not None:
            param.data = fn(param.data)
            if param._grad is not None:
                param._grad.data = fn(param._grad.data)
 
    for key, buf in self._buffers.items():
        if buf is not None:
            self._buffers[key] = fn(buf)
    return self
 
def apply(self, fn):
    for module in self.children():
        module.apply(fn)
    fn(self)
    return self
```

这两个方法就是更新参数的核心过程了，pytorch的更新参数最底层的方法都是这两个方法定义的。

之后的cpu(),cuda()之类的方法大家都知道是干什么的，我就不赘述了，啊，顺带提一句，这个cuda()方法是对每个变量都covert to cuda的，十分的方便。

```
def register_backward_hook(self, hook):
    handle = hooks.RemovableHandle(self._backward_hooks)
    self._backward_hooks[handle.id] = hook
    return handle
 
def register_forward_pre_hook(self, hook):
    handle = hooks.RemovableHandle(self._forward_pre_hooks)
    self._forward_pre_hooks[handle.id] = hook
    return handle
 
def register_forward_hook(self, hook):
    handle = hooks.RemovableHandle(self._forward_hooks)
    self._forward_hooks[handle.id] = hook
    return handle
```

训练过程的参数传递，这些方法完成了神经网络，Loss，梯度下降等算法等等一系列计算的之间的数据通信。

## 结语

先写到这里，nn.Module大概写了一半左右吧，希望各位大佬们给出建议QwQ。