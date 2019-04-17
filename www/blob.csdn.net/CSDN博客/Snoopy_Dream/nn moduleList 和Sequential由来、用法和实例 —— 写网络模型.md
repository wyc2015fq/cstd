# nn.moduleList 和Sequential由来、用法和实例 —— 写网络模型 - Snoopy_Dream - CSDN博客





2018年11月23日 17:20:39[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：2237
所属专栏：[faster-rcnn.pytorch](https://blog.csdn.net/column/details/25674.html)









对于cnn前馈神经网络如果前馈一次写一个forward函数会有些麻烦，在此就有两种简化方式，ModuleList和Sequential。其中Sequential是一个特殊的module，它包含几个子Module，前向传播时会将输入一层接一层的传递下去。ModuleList也是一个特殊的module，可以包含几个子module，可以像用list一样使用它，但不能直接把输入传给ModuleList。下面举例说明。

**目录**

[一、nn.Sequential()对象](#main-toc)

[1、模型建立方式](#1%E3%80%81%E6%A8%A1%E5%9E%8B%E5%BB%BA%E7%AB%8B%E6%96%B9%E5%BC%8F)

[第一种写法：](#%E7%AC%AC%E4%B8%80%E7%A7%8D%E5%86%99%E6%B3%95%EF%BC%9A)

[第二种写法：](#%C2%A0%E7%AC%AC%E4%BA%8C%E7%A7%8D%E5%86%99%E6%B3%95%EF%BC%9A)

[第三种写法：](#%E7%AC%AC%E4%B8%89%E7%A7%8D%E5%86%99%E6%B3%95%EF%BC%9A)

[2、检查以及调用模型](#2%E3%80%81%E6%A3%80%E6%9F%A5%E4%BB%A5%E5%8F%8A%E8%B0%83%E7%94%A8%E6%A8%A1%E5%9E%8B)

[查看模型](#%E6%9F%A5%E7%9C%8B%E6%A8%A1%E5%9E%8B)

[根据名字或序号提取子Module对象](#%E6%A0%B9%E6%8D%AE%E5%90%8D%E5%AD%97%E6%88%96%E5%BA%8F%E5%8F%B7%E6%8F%90%E5%8F%96%E5%AD%90Module%E5%AF%B9%E8%B1%A1)

[调用模型](#%E8%B0%83%E7%94%A8%E6%A8%A1%E5%9E%8B)

[二、nn.ModuleList()对象](#%E4%BA%8C%E3%80%81nn.ModuleList()%E5%AF%B9%E8%B1%A1)

[为什么有他？](#%E4%B8%BA%E4%BB%80%E4%B9%88%E6%9C%89%E4%BB%96%EF%BC%9F)

[什么时候用？](#%E4%BB%80%E4%B9%88%E6%97%B6%E5%80%99%E7%94%A8%EF%BC%9F)

[和list的区别？](#%E5%92%8Clist%E7%9A%84%E5%8C%BA%E5%88%AB%EF%BC%9F)

[1. extend和append方法](#1.%20extend%E5%92%8Cappend%E6%96%B9%E6%B3%95)

[2. 建立以及使用方法](#2.%20%E5%BB%BA%E7%AB%8B%E4%BB%A5%E5%8F%8A%E4%BD%BF%E7%94%A8%E6%96%B9%E6%B3%95)

[3. yolo v3构建网络](#3.%20yolo%20v3%E6%9E%84%E5%BB%BA%E7%BD%91%E7%BB%9C)

# 一、nn.Sequential()对象

建立nn.Sequential()对象，必须小心确保一个块的输出大小与下一个块的输入大小匹配。基本上，它的行为就像一个nn.Module。



## 1、模型建立方式



### 第一种写法：

nn.Sequential()对象.add_module(层名，层class的实例）
|1234|`net1 ``=``nn.Sequential()``net1.add_module(``'conv'``, nn.Conv2d(``3``, ``3``, ``3``))``net1.add_module(``'batchnorm'``, nn.BatchNorm2d(``3``))``net1.add_module(``'activation_layer'``, nn.ReLU())`|
|----|----|



###  第二种写法：

nn.Sequential(*多个层class的实例)
|12345|`net2 ``=``nn.Sequential(``        ``nn.Conv2d(``3``, ``3``, ``3``),``        ``nn.BatchNorm2d(``3``),``        ``nn.ReLU()``        ``)`|
|----|----|



### 第三种写法：

nn.Sequential(OrderedDict([*多个(层名，层class的实例)]))
|123456|`from``collections ``import``OrderedDict``net3``=``nn.Sequential(OrderedDict([``          ``(``'conv'``, nn.Conv2d(``3``, ``3``, ``3``)),``          ``(``'batchnorm'``, nn.BatchNorm2d(``3``)),``          ``(``'activation_layer'``, nn.ReLU())``        ``]))`|
|----|----|





## 2、检查以及调用模型



### 查看模型

print对象即可
|123|`print``(``'net1:'``, net1)``print``(``'net2:'``, net2)``print``(``'net3:'``, net3)`|
|----|----|

> 
```
net1: Sequential(
  (conv): Conv2d (3, 3, kernel_size=(3, 3), stride=(1, 1))
  (batchnorm): BatchNorm2d(3, eps=1e-05, momentum=0.1, affine=True)
  (activation_layer): ReLU()
)
net2: Sequential(
  (0): Conv2d (3, 3, kernel_size=(3, 3), stride=(1, 1))
  (1): BatchNorm2d(3, eps=1e-05, momentum=0.1, affine=True)
  (2): ReLU()
)
net3: Sequential(
  (conv): Conv2d (3, 3, kernel_size=(3, 3), stride=(1, 1))
  (batchnorm): BatchNorm2d(3, eps=1e-05, momentum=0.1, affine=True)
  (activation_layer): ReLU()
)
```




### `根据名字或序号`提取子Module对象
|12|`# 可根据名字或序号取出子module``net1.conv, net2[``0``], net3.conv`|
|----|----|

> 
```
(Conv2d (3, 3, kernel_size=(3, 3), stride=(1, 1)),
 Conv2d (3, 3, kernel_size=(3, 3), stride=(1, 1)),
 Conv2d (3, 3, kernel_size=(3, 3), stride=(1, 1)))
```




### 调用模型

可以直接网络对象(输入数据)，也可以使用上面的Module子对象分别传入(input)。
|12345|`input``=``V(t.rand(``1``, ``3``, ``4``, ``4``))``output ``=``net1(``input``)``output ``=``net2(``input``)``output ``=``net3(``input``)``output ``=``net3.activation_layer(net1.batchnorm(net1.conv(``input``)))`|
|----|----|



# 二、nn.ModuleList()对象

## 为什么有他？

写一个module然后就写foreword函数很麻烦，所以就有了这两个。它被设计用来存储任意数量的nn. module。

## 什么时候用？

如果在构造函数`__init__`中用到list、tuple、dict等对象时，一定要思考是否应该用ModuleList或ParameterList代替。

如果你想设计一个神经网络的层数作为输入传递。

## 和list的区别？

`ModuleList`是`Module`的子类，当在`Module`中使用它的时候，就能自动识别为子module。

当添加 nn.ModuleList 作为 nn.Module 对象的一个成员时（**即当我们添加模块到我们的网络时**），**所有 nn.ModuleList 内部的 nn.Module 的 parameter 也被添加作为 我们的网络的 parameter。**



```python
class MyModule(nn.Module):
    def __init__(self):
        super(MyModule, self).__init__()
        self.linears = nn.ModuleList([nn.Linear(10, 10) for i in range(10)])

    def forward(self, x):
        # ModuleList can act as an iterable, or be indexed         using ints
        for i, l in enumerate(self.linears):
            x = self.linears[i // 2](x) + l(x)
        return x
```

## **1. extend和append方法**

**nn.moduleList定义对象后，有extend和append方法，****用法和python中一样，****extend是添加另一个modulelist  append是添加另一个module**

```python
class LinearNet(nn.Module):
  def __init__(self, input_size, num_layers, layers_size, output_size):
     super(LinearNet, self).__init__()
 
     self.linears = nn.ModuleList([nn.Linear(input_size, layers_size)])
     self.linears.extend([nn.Linear(layers_size, layers_size) for i in range(1, self.num_layers-1)])
     self.linears.append(nn.Linear(layers_size, output_size)
```

## 2. 建立以及使用方法

建立以及使用方法如下，
|123456|`modellist ``=``nn.ModuleList([nn.Linear(``3``,``4``), nn.ReLU(), nn.Linear(``4``,``2``)])``input``=``V(t.randn(``1``, ``3``))``for``model ``in``modellist:``    ``input``=``model(``input``)``# 下面会报错,因为modellist没有实现forward方法``# output = modelist(input)`|
|----|----|



和普通list不一样，它和torch的其他机制结合紧密，继承了nn.Module的网络模型class可以使用nn.ModuleList并识别其中的parameters，当然这只是个list，不会自动实现forward方法，
|123456789|`class``MyModule(nn.Module):``    ``def``__init__(``self``):``        ``super``(MyModule, ``self``).__init__()``        ``self``.``list``=``[nn.Linear(``3``, ``4``), nn.ReLU()]``        ``self``.module_list ``=`**`nn.ModuleList([nn.Conv2d(``3``, ``3``, ``3``), nn.ReLU()])`**`    ``def``forward(``self``):``        ``pass``model ``=``MyModule()``print``(model)`|
|----|----|

> 
```
MyModule(
  (module_list): ModuleList(
    (0): Conv2d (3, 3, kernel_size=(3, 3), stride=(1, 1))
    (1): ReLU()
  )
)
```

|12|`for``name, param ``in``**model.named_parameters**():``    ``print``(name, param.size())`|
|----|----|

> 
```
('module_list.0.weight', torch.Size([3, 3, 3, 3]))
('module_list.0.bias', torch.Size([3]))
```


可见，普通**list中的子module并不能被主module所识别**，而**ModuleList中的子module能够被主module所识别**。这意味着如果用list保存子module，将无法调整其参数，因其未加入到主module的参数中。

**除ModuleList之外还有ParameterList**，其是一个可以包含多个parameter的类list对象。在实际应用中，使用方式与ModuleList类似。



## 3. yolo v3构建网络

首先module_list = nn.ModuleList()

然后 

  for index, x in enumerate(blocks[1:]):#根据不同的block 遍历module

        module = nn.Sequential()

        然后根据cfg读进来的数据，

        module.add_module("batch_norm_{0}".format(index), bn)

        module.add_module("conv_{0}".format(index), conv)

         等等

**   module_list.append(module)**





