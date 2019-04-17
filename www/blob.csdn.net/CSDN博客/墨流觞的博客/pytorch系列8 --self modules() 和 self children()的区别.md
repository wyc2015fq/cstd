# pytorch系列8  --self.modules() 和 self.children()的区别 - 墨流觞的博客 - CSDN博客





2018年11月11日 14:46:30[墨氲](https://me.csdn.net/dss_dssssd)阅读数：652
所属专栏：[python3 pytorch0.4系列教程](https://blog.csdn.net/column/details/29714.html)









本文主要讲述：
- self.modue和self.children的区别与联系

说实话，我真的只想讲参数初始化方式，但总感觉在偏离的道路上越走越远。。。

在看一些pytorch文章讲述自定义参数初始化方式时，使用到了`self.modules()`和`self.children()`函数，觉得还是需要讲解一下的。

不如直接看一下代码：

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
            nn.ReLU(True)
             )
        self.layer2 = nn.Sequential(
                nn.Linear(n_hidden_1, n_hidden_2),
                nn.ReLU(True),
            )
        self.layer3 = nn.Linear(n_hidden_2, out_dim)
        # print(self.modules())
        print("children")
        
        for i, module in enumerate( self.children()):
            print(i, module)
        print("modules")
        for i, module in enumerate( self.modules()):
            print(i, module)

    
    def forward(self, x):
        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)
        return x
      
        
model = Net(in_dim, n_hidden_1, n_hidden_2, out_dim)
```

##### 网络结构解读：

这是一个三层的网络结构，将第一层的线性层和激活层放在一个`nn.Sequential`层中，将第二层的线性层和激活函数放在第二个`nn.Sequential`中，最后一个线性层作为单独第三层。

整个网络结构如下图所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111114314447.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
接下来看一下代码`__init__`的print函数的打印信息：
- `self.children()`

out:

> 
children

0 Sequential(

(0): Linear(in_features=1, out_features=1, bias=True)

(1): ReLU(inplace)

)

1 Sequential(

(0): Linear(in_features=1, out_features=1, bias=True)

(1): ReLU(inplace)

)

2 Linear(in_features=1, out_features=1, bias=True)
可以看出，`self.children()`存储网络结构的子层模块，也就是`net's children`那一层。
- `self.modules()`

out:

> 
modules

0 Net(

(layer): Sequential(

(0): Linear(in_features=1, out_features=1, bias=True)

(1): ReLU(inplace)

)

(layer2): Sequential(

(0): Linear(in_features=1, out_features=1, bias=True)

(1): ReLU(inplace)

)

(layer3): Linear(in_features=1, out_features=1, bias=True)

)

1 Sequential(

(0): Linear(in_features=1, out_features=1, bias=True)

(1): ReLU(inplace)

)

2 Linear(in_features=1, out_features=1, bias=True)

3 ReLU(inplace)

4 Sequential(

(0): Linear(in_features=1, out_features=1, bias=True)

(1): ReLU(inplace)

)

5 Linear(in_features=1, out_features=1, bias=True)

6 ReLU(inplace)

7 Linear(in_features=1, out_features=1, bias=True)
可以看出，self.modules()采用**深度优先遍历**的方式，存储了net的所有模块，包括`net itself`,`net's children`, `children of net's children`。

**conclusion：**

self.children()只包括网络模块的第一代儿子模块，而self.modules()包含网络模块的自己本身和所有后代模块。

参考：
[https://discuss.pytorch.org/t/module-children-vs-module-modules/4551/3](https://discuss.pytorch.org/t/module-children-vs-module-modules/4551/3)








