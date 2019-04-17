# pytorch系列7  -----nn.Sequential讲解 - 墨流觞的博客 - CSDN博客





2018年10月09日 13:56:05[墨氲](https://me.csdn.net/dss_dssssd)阅读数：6314标签：[pytorch																[neural network																[python](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[pytorch 记录](https://blog.csdn.net/dss_dssssd/article/category/8109673)

所属专栏：[python3 pytorch0.4系列教程](https://blog.csdn.net/column/details/29714.html)](https://so.csdn.net/so/search/s.do?q=neural network&t=blog)




接下来想讲一下**参数初始化方式对训练的影响**，但是必须**要涉及到pytorch的自定义参数初始化**，然而参数初始化又包括**在不同结构定义**中初始化方式，因而先讲一下pytorch中的nn.Sequential

### nn.Sequential

A sequential container. Modules will be added to it in the order they are passed in the constructor. Alternatively, an ordered dict of modules can also be passed in.

一个有序的容器，神经网络模块将按照在传入构造器的顺序依次被添加到计算图中执行，同时以神经网络模块为元素的有序字典也可以作为传入参数。

看一下例子：

```python
# Example of using Sequential
        model = nn.Sequential(
                  nn.Conv2d(1,20,5),
                  nn.ReLU(),
                  nn.Conv2d(20,64,5),
                  nn.ReLU()
                )

        # Example of using Sequential with OrderedDict
        model = nn.Sequential(OrderedDict([
                  ('conv1', nn.Conv2d(1,20,5)),
                  ('relu1', nn.ReLU()),
                  ('conv2', nn.Conv2d(20,64,5)),
                  ('relu2', nn.ReLU())
                ]))
```

接下来看一下Sequential源码，是如何实现的:
[https://pytorch.org/docs/stable/_modules/torch/nn/modules/container.html#Sequential](https://pytorch.org/docs/stable/_modules/torch/nn/modules/container.html#Sequential)

先看一下初始化函数`__init__`,在初始化函数中，首先是if条件判断，如果传入的参数为1个，并且类型为OrderedDict，通过字典索引的方式将子模块添加到`self._module`中，否则，通过for循环遍历参数，将所有的子模块添加到`self._module`中。**注意，Sequential模块的初始换函数没有异常处理，所以在写的时候要注意，注意，注意了**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111131848862.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
接下来在看一下`forward`函数的实现：

因为每一个module都继承于nn.Module,都会实现`__call__`与`forward`函数，**[具体讲解点击这里](https://blog.csdn.net/dss_dssssd/article/details/82977170)**，所以forward函数中通过for循环依次调用添加到`self._module`中的子模块，最后输出经过所有神经网络层的结果
![在这里插入图片描述](https://img-blog.csdn.net/20181009134131281?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
下面是简单的三层网络结构的例子：

```python
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
            nn.ReLU(True)，
            nn.Linear(n_hidden_1, n_hidden_2)，
            nn.ReLU(True)，
            # 最后一层不需要添加激活函数
            nn.Linear(n_hidden_2, out_dim)
             )

  	def forward(self, x):
      	x = self.layer(x)
      	return x
```

上面的代码就是通过Squential将网络层和激活函数结合起来，输出激活后的网络节点。](https://so.csdn.net/so/search/s.do?q=pytorch&t=blog)




