# pytorch 基础系列（四）——nn.module - Snoopy_Dream - CSDN博客





2018年11月14日 22:22:00[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：447








torch.nn的核心数据结构是`Module`，它是一个抽象概念，既可以表示神经网络中的某个层（layer），也可以表示一个包含很多层的神经网络。无需纠结variable和tensor了，0.4版本已经将两个类彻底合并了。

在实际使用中，最常见的做法是继承`nn.Module`，撰写自己的网络/层。
- 自定义层`Linear`必须继承`nn.Module`，并且在其构造函数中需调用`nn.Module`的构造函数，即`super(Linear, self).__init__()` 或`nn.Module.__init__(self)`，推荐使用第一种用法。
- 在构造函数**`__init__`中必须**自己**定义可学习的参数**，并封装成**`Parameter`**，你比如 _FasterRcnn类init中定义了  self.RCNN_loss_cls = 0  和 self.RCNN_loss_bbox = 0  还有在本例中我们把`w`和`b`封装成`parameter`。**`parameter`是一种特殊的`Variable`**，但其**默认需要求导**（requires_grad = True）。
- `forward`函数实现前向传播过程，其输入可以是一个或多个variable，对x的任何操作也必须是variable支持的操作。
- **无需写反向传播函数，因其前向传播都是对variable进行操作，nn.Module能够利用autograd自动实现反向传播，这点比Function简单许多。 faster rcnn等中反向传播直接写pass的原因。**
- 使用时，直观上可将layer看成数学概念中的函数，**调用layer(input)**即可得到input对应的结果。它等价于`layers.__call__(input)`，在`__call__`函数中，**主要调用的是 `layer.forward(x)`，另外还对钩子做了一些处理。所以在实际使用中应尽量使用`layer(x)`而不是使用`layer.forward(x)`**。
- `Module`中的**可学习参数**可以**通过`named_parameters()`或者`parameters()`返回迭代器**，前者会给每个parameter都附上名字，使其更具有辨识度。

### **Module能够自动检测到自己的`Parameter`，并将其作为学习参数。**

可见利用Module**实现的全连接层**，比利用`Function`实现的更为简单，因其不再需要写反向传播函数。
|12345678910111213141516171819202122|`import``torch as t``from``torch ``import``nn``from``torch.autograd ``import``Variable as V``class``**Linear**(nn.Module):``    ``def``__init__(``self``, in_features, out_features):``        ``# nn.Module.__init__(self)``        ``super``(Linear, ``self``).__init__()``        ``self``.w ``=``**nn.Parameter**(t.randn(in_features, out_features)) ``# nn.Parameter是特殊Variable``        ``self``.b ``=``**nn.Parameter**(t.randn(out_features))``        ``    ``def``forward(``self``, x):``        ``x ``=``x.mm(``self``.w)``        ``return``x ``+``self``.b``**layer** ``=``**Linear**(``4``, ``3``)``input``=``V(t.randn(``2``, ``4``))``output ``=``layer(``input``)``print``(output)``for``name, Parameter ``in``**layer**.**named_parameters**():``    ``print``(name, Parameter)`|
|----|----|

> 
Variable containing:

 4.1151  2.4139  3.5544

-0.4792 -0.9400 -7.6010

[torch.FloatTensor of size 2x3]

w Parameter containing:

 1.1856  0.9246  1.1707

 0.2632 -0.1697  0.7543

-0.4501 -0.2762 -3.1405

-1.1943  1.2800  1.0712

[torch.FloatTensor of size 4x3]


b Parameter containing:

 1.9577

 1.8570

 0.5249

[torch.FloatTensor of size 3]




# 递归

除了`parameter`之外，Module还包含子`Module`，主Module能够**递归查找子`Module`中的`parameter`**。
- 构造函数`__init__`中，可利用前面自定义的Linear层(module)，作为当前module对象的一个子module，它的可学习参数，也会成为当前module的可学习参数。
- 在前向传播函数中，我们有意识地将输出变量都命名成`x`，是为了能让Python回收一些中间层的输出，从而节省内存。但并不是所有都会被回收，有些variable虽然名字被覆盖，但其在反向传播仍需要用到，此时Python的内存回收模块将通过检查引用计数，不会回收这一部分内存。

module中parameter的命名规范：
- 对于类似`self.param_name = nn.Parameter(t.randn(3, 4))`，命名为`param_name`
- 对于子Module中的parameter，会其名字之前加上当前Module的名字。如对于`self.sub_module = SubModel()`，SubModel中有个parameter的名字叫做param_name，那么二者拼接而成的parameter name 就是`sub_module.param_name`。

下面再来看看稍微复杂一点的网络，多层感知机：
|1234567891011121314|`class``Perceptron (nn.Module):``    ``def``__init__(``self``, in_features, hidden_features, out_features):``        ``nn.Module.__init__(``self``)``        ``self``.**layer1** ``=``**Linear**(in_features, hidden_features)``        ``self``.**layer2** ``=``**Linear**(hidden_features, out_features)``        ``    ``def``forward(``self``, x):``        ``x ``=``self``.layer1(x)``        ``x ``=``t.sigmoid(x)``        ``return``self``.layer2(x)``per ``=``Perceptron(``3``, ``4``, ``1``)``for``name, param ``in``per.named_parameters():``    ``print``(name, param.size())`|
|----|----|

> 
('**layer1**.**w**', torch.Size([3, 4]))
('**layer1.b**', torch.Size([4]))
('layer2.w', torch.Size([4, 1]))
('layer2.b', torch.Size([1]))








