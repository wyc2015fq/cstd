# 强化学习系列13：深度学习框架pytorch - kittyzc的博客 - CSDN博客
2019年02月12日 14:50:59[IE06](https://me.csdn.net/kittyzc)阅读数：47个人分类：[python																[算法](https://blog.csdn.net/kittyzc/article/category/7940151)](https://blog.csdn.net/kittyzc/article/category/7940149)所属专栏：[强化学习系列](https://blog.csdn.net/column/details/33845.html)
# 1. pytorch介绍

深度学习的框架基本都是基于计算图的，其中静态计算图先定义再运行（比如前面介绍的tensorflow），而动态图则是一边定义一边运行（比如本篇介绍的pytorch）。在tensorflow中，计算图一旦定义好了就不能修改，因此需要在最开始就把所有的情况都包含到计算图中；在pytorch中，每一次运行代码（也就是开始进行前向传播）时，都会创建一副新的计算图，相比之下非常灵活。pytorch的框架遵循张量tensor(从0.4起, Variable 正式合并入Tensor)->神经网络nn.Module的层次，比tensorflow简洁很多。

pytorch有如下几个明显的优点：1. python化，易用；2. 框架简洁；3. 运行速度快。

# 2. pytorch安装与使用

安装很简单：

```python
pip install pytorch torchvision
```

注意在python中使用的时候引用的包是torch，不是pytorch。

torchvision实现了常用的图像数据加载功能，例如Imagenet、CIFAR10、MNIST等，以及常用的数据转换操作。

## 2.1 Tensor

第一个基本概念是Tensor，可认为是一个高维数组。它可以是一个数（标量）、一维数组（向量）、二维数组（矩阵）以及更高维的数组。Tensor和Numpy的ndarrays类似，但Tensor可以使用GPU进行加速。

需要注意的是，t.tensor()总是会进行数据拷贝，新tensor和原来的数据不再共享内存。所以如果你想共享内存的话，建议使用torch.from_numpy()或者tensor.detach()来新建一个tensor, 二者共享内存。

在新版本中，Tensor可以直接使用autograd功能（不需要再封装为Variable），只需要设置tensor.requries_grad=True。注意使用.backward进行反向传播时，如果输出变量不是标量，需要制定一个尺寸相同的tensor作为输入变量，当做每一个分量的权重。需要重新计算时，使用下面的方法将梯度清零：x.grad.data.zero_()

在预测的时候，可以关闭autograd功能，提高速度，节约内存，方法是：with t.no_grad()

Tensor的很多操作，例如div、mul、pow、fmod等，PyTorch都实现了运算符重载，所以可以直接使用运算符。如a ** 2 等价于torch.pow(a,2), a * 2等价于torch.mul(a,2)。Tensor的比较运算符也都实现了重载，比如a>=b、a>b、a!=b、a==b，其返回结果是一个ByteTensor，可用来选取元素。
## 2.2 Network

torch.nn的核心数据结构是Module，它是一个抽象概念，既可以表示神经网络中的某个层（layer），也可以表示一个包含很多层的神经网络。nn.Module基类的构造函数如下：

```python
def __init__(self):
    self._parameters = OrderedDict()
    self._modules = OrderedDict()
    self._buffers = OrderedDict()
    self._backward_hooks = OrderedDict()
    self._forward_hooks = OrderedDict()
    self.training = True
```

其中每个属性的解释如下：
- _parameters：字典，保存用户直接设置的parameter，self.param = nn.Parameter(t.randn(3, 3))会被检测到，在字典中加入一个key为’param’，value为对应parameter的item。
- _modules：子module，通过self.submodel = nn.Linear(3, 4)指定的子module会保存于此。
- _buffers：缓存。如batchnorm使用momentum机制，每次前向传播需用到上一次前向传播的结果。
- _backward_hooks与_forward_hooks：钩子技术，用来提取中间变量，类似variable的hook。
- training：BatchNorm与Dropout层在训练阶段和测试阶段中采取的策略不同，通过判断training值来决定前向传播策略。

上述几个属性中，_parameters、_modules和_buffers这三个字典中的键值，都可以通过self.key方式获得，效果等价于self._parameters[‘key’].

定义网络时，需要继承nn.Module，并实现它的forward方法，把网络中具有可学习参数的层放在构造函数__init__中。如果某一层(如ReLU)不具有可学习的参数，则既可以放在构造函数中，也可以不放，但建议不放在其中，而在forward中使用nn.functional代替。下面是LeNet的例子：

```python
import torch.nn as nn
import torch.nn.functional as F

class Net(nn.Module):
    def __init__(self):
        # nn.Module子类的函数必须在构造函数中执行父类的构造函数
        # 下式等价于nn.Module.__init__(self)
        super(Net, self).__init__()
        
        # 卷积层 '1'表示输入图片为单通道, '6'表示输出通道数，'5'表示卷积核为5*5
        self.conv1 = nn.Conv2d(1, 6, 5) 
        # 卷积层
        self.conv2 = nn.Conv2d(6, 16, 5) 
        # 仿射层/全连接层，y = Wx + b
        self.fc1   = nn.Linear(16*5*5, 120) 
        self.fc2   = nn.Linear(120, 84)
        self.fc3   = nn.Linear(84, 10)

    def forward(self, x): 
        # 卷积 -> 激活 -> 池化 
        x = F.max_pool2d(F.relu(self.conv1(x)), (2, 2))
        x = F.max_pool2d(F.relu(self.conv2(x)), 2) 
        # reshape，‘-1’表示自适应
        x = x.view(x.size()[0], -1) 
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)        
        return x

net = Net()
print(net)
```

只要在nn.Module的子类中定义了forward函数，backward函数就会自动被实现(利用autograd)。forward函数的输入和输出都是Tensor。在forward 函数中可使用任何tensor支持的函数，还可以使用if、for循环、print、log等Python语法，写法和标准的Python写法一致。网络的可学习参数通过net.parameters()返回，net.named_parameters可同时返回可学习的参数及名称。

```python
for name,parameters in net.named_parameters():
print(name,':',parameters.size())
```

输出为：

```
conv1.weight : torch.Size([6, 1, 5, 5])
conv1.bias : torch.Size([6])
conv2.weight : torch.Size([16, 6, 5, 5])
conv2.bias : torch.Size([16])
fc1.weight : torch.Size([120, 400])
fc1.bias : torch.Size([120])
fc2.weight : torch.Size([84, 120])
fc2.bias : torch.Size([84])
fc3.weight : torch.Size([10, 84])
fc3.bias : torch.Size([10])
```

调用forward方法和直接将input做参数得到的结果是一样的。

```python
input = t.randn(1, 1, 32, 32)
out1 = net(input)
out2 = net.forward(input)
net.zero_grad() # 所有参数的梯度清零
out1.backward(t.ones(1,10)) # 反向传播
```

需要注意的是，torch.nn只支持mini-batches，不支持一次只输入一个样本，即一次必须是一个batch。但如果只想输入一个样本，则用 input.unsqueeze(0)将batch_size设为１。

nn实现了神经网络中大多数的损失函数，例如nn.MSELoss用来计算均方误差，nn.CrossEntropyLoss用来计算交叉熵损失。

在反向传播计算完所有参数的梯度后，还需要使用优化方法来更新网络的权重和参数，例如随机梯度下降法(SGD)的更新策略如下：

weight = weight - learning_rate * gradient

torch.optim中实现了深度学习中绝大多数的优化方法，例如RMSProp、Adam、SGD等。优化器一般要指定要调整的参数和学习率，日不optimizer = optim.SGD(net.parameters(), lr = 0.01)，更新参数用optimizer.step()即可。

用如下方式将代码转移到GPU上运行：
```python
device = t.device("cuda:0" if t.cuda.is_available() else "cpu")

net.to(device)
images = images.to(device)
labels = labels.to(device)
```

在PyTorch中保存模型十分简单，所有的Module对象都具有state_dict()函数，返回当前Module所有的状态数据。将这些状态数据保存后，下次使用模型时即可利用model.load_state_dict()函数将状态加载进来。优化器（optimizer）也有类似的机制，不过一般并不需要保存优化器的运行状态。

```python
# 保存模型
t.save(net.state_dict(), 'net.pth')
# 加载已保存的模型
net2 = Net()
net2.load_state_dict(t.load('net.pth'))
```

## 2.3 function

nn中还有一个很常用的模块：nn.functional，nn中的大多数layer，在functional中都有一个与之相对应的函数。nn.functional中的函数和nn.Module的主要区别在于，用nn.Module实现的layers是一个特殊的类，都是由class layer(nn.Module)定义，会自动提取可学习的参数。而nn.functional中的函数更像是纯函数，由def function(input)定义。如果模型有可学习的参数，最好用nn.Module，否则既可以使用nn.functional也可以使用nn.Module，二者在性能上没有太大差异。


