# PyTorch入门学习（一） - 站在巨人的肩膀上coding - CSDN博客





2018年04月27日 13:26:25[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：281








# PyTorch入门学习（一）

目录


- [PyTorch入门学习一](https://blog.csdn.net/cuclxt/article/details/71305222?utm_source=itdadao&utm_medium=referral#pytorch%E5%85%A5%E9%97%A8%E5%AD%A6%E4%B9%A0%E4%B8%80)- [安装](https://blog.csdn.net/cuclxt/article/details/71305222?utm_source=itdadao&utm_medium=referral#%E5%AE%89%E8%A3%85)
- [基本操作](https://blog.csdn.net/cuclxt/article/details/71305222?utm_source=itdadao&utm_medium=referral#%E5%9F%BA%E6%9C%AC%E6%93%8D%E4%BD%9C)- [Tensor](https://blog.csdn.net/cuclxt/article/details/71305222?utm_source=itdadao&utm_medium=referral#tensor)
- [Autograd](https://blog.csdn.net/cuclxt/article/details/71305222?utm_source=itdadao&utm_medium=referral#autograd)
- [autogradVariable](https://blog.csdn.net/cuclxt/article/details/71305222?utm_source=itdadao&utm_medium=referral#autogradvariable)
- [Function](https://blog.csdn.net/cuclxt/article/details/71305222?utm_source=itdadao&utm_medium=referral#function)

- [Neural Networks示例](https://blog.csdn.net/cuclxt/article/details/71305222?utm_source=itdadao&utm_medium=referral#neural-networks%E7%A4%BA%E4%BE%8B)- [Loss函数](https://blog.csdn.net/cuclxt/article/details/71305222?utm_source=itdadao&utm_medium=referral#loss%E5%87%BD%E6%95%B0)
- [Backprop](https://blog.csdn.net/cuclxt/article/details/71305222?utm_source=itdadao&utm_medium=referral#backprop)
- [权值更新](https://blog.csdn.net/cuclxt/article/details/71305222?utm_source=itdadao&utm_medium=referral#%E6%9D%83%E5%80%BC%E6%9B%B4%E6%96%B0)





[PyTorch](http://pytorch.org/)是一个很流行的开源深度学习平台，风头一度胜过[Tensorflow](http://www.tensorflow.org/),近期对对抗生成网络[GAN](https://arxiv.org/abs/1406.2661)比较感兴趣，GAN采用生成式和判别式网络，生成模型G捕捉样本数据的分布，判别模型D是一个二分类器，估计一个样本来自于训练数据（而非生成数据）的概率，最终，G 能估测出样本数据的分布，判别模型D的准确率等于50%左右。启发自博弈论中的零和博弈，也同周伯通的左右互搏有相通之处。近期的一些GAN的变种的论文都是由PyTorch实现的，欲善其事，先利其器，先把工具给学习了。

## 安装

目前PyTorch只支持linux和OSX,采用pip或者conda，在官网首页选好自己软件相应版本，可用两行命令安装。

> 
pip install [http://download.pytorch.org/whl/cu80/torch-0.1.12.post2-cp27-none-linux_x86_64.whl](http://download.pytorch.org/whl/cu80/torch-0.1.12.post2-cp27-none-linux_x86_64.whl)
  pip install torchvision


我的版本是ubuntu+python 2.7+pip

## 基本操作

python常用的数值计算的库numpy不能通过GPU加速计算，所以计算速度较慢，PyTorch就可以，并且可以和numpy的数据类型转换。

### Tensor

```python
Tensor张量，简单理解：一维张量是向量，二维张量是矩阵
from __future__ import print_function
import torch
x=torch.Tensor(3,5) #实际为3*5的矩阵
x.size()  #Out:torch.Size([3, 5])
y = torch.rand(3, 5) #产生随机数
z=torch.ones(5)   #5个1
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7

把Tensor放到GPU上

```python
# let us run this cell only if CUDA is available
if torch.cuda.is_available():
    x = x.cuda()
    y = y.cuda()
    x + y
切片访问：x[:,1]    #x的第二行
```
- 1
- 2
- 3
- 4
- 5
- 6

### Autograd

自动求导数包，这是PyTorch对神经网络求导的核心包。可以提供对Tensor各种操作的自动求导。

### autograd.Variable

这个类是自动求导包的核心类，该类wraps了Tensor，支持Tensor的全部计算操作，计算完后调用.backward()函数时即能够自动计算梯度。 
sample:计算 1441322
对
的导数

```python
import torch
from torch.autograd import Variable
x = Variable(torch.ones(2, 2), requires_grad=True)
y = (x+2) * (x+2) * 3
out = y.mean()
print(y, out)
out.backward() #out.backward(torch.Tensor([1.0]))
print(x.grad)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8

> 
Out:Variable containing: 
   4.5000  4.5000 
   4.5000  4.5000 
  [torch.FloatTensor of size 2x2] 
  即y对每个x的导数都是4.5，一个简单的求偏导数


### Function

Function类和Variable类相互联系组成了有向无环图，组成了整个计算过程。每个variable都有一个属性.creator，它引用了创造该variable的Function，用户自己创造的variable的.creator属性是None. 
更多细节见Variable 和 Function 的 [Documentation](http://pytorch.org/docs/autograd)

## Neural Networks示例

神经网络的构建主要用torch.nn包。nn依赖于autograd包来计算导数，nn.Module类定义了层和前向传播函数forward(input)，来返回一个输出output.

一个定义卷积神经网络的Sample: 
该例程输入为32*32随机矩阵，迭代了10次，仅做函数功能测试，无实际意义

```python
import torch
from torch.autograd import Variable
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim

class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        # 建立了两个卷积层，第一层1 个通道输入, 6个输出通道, 5x5 卷积核
        self.conv1 = nn.Conv2d(1, 6, 5)
        self.conv2 = nn.Conv2d(6, 16, 5)
        #三个全连接层，y = Wx + b 这里没有做激活/非线性操作
        self.fc1 = nn.Linear(16 * 5 * 5, 120)
        self.fc2 = nn.Linear(120, 84)
        self.fc3 = nn.Linear(84, 10)

    def forward(self, x): #2D卷积层的输入data维数是 batchsize*channel*height*width
        # 最大池化
        x = F.max_pool2d(F.relu(self.conv1(x)), (2, 2))
        x = F.max_pool2d(F.relu(self.conv2(x)), 2)
        x = x.view(-1, self.num_flat_features(x))
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x

    def num_flat_features(self, x):
        size = x.size()[1:]  # all dimensions except the batch dimension
        num_features = 1
        for s in size:
            num_features *= s
        return num_features
net = Net()

# create your optimizer #训练网络用，除了SGD,还有SGD, Nesterov-SGD, Adam, RMSProp
optimizer = optim.SGD(net.parameters(), lr = 0.01)

# in your training loop:
for i in range(10):
    input = Variable(torch.randn(1, 1, 32, 32))
    optimizer.zero_grad() # zero the gradient buffers
    output = net(input)
    criterion=nn.MSELoss()
    target = Variable(torch.range(1, 10))
    loss = criterion(output, target)
    loss.backward()
    optimizer.step() # Does the update，i.e. Variable.data -= learning_rate*Variable.grad
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48

print（net） 
Net ( 
  (conv1): Conv2d(1, 6, kernel_size=(5, 5), stride=(1, 1)) 
  (conv2): Conv2d(6, 16, kernel_size=(5, 5), stride=(1, 1)) 
  (fc1): Linear (400 -> 120) 
  (fc2): Linear (120 -> 84) 
  (fc3): Linear (84 -> 10) 
)

### Loss函数

```python
output = net(input)
target = Variable(torch.range(1, 10))  # a dummy target, for example
criterion = nn.MSELoss()  #mean-squared error between the input and the target.
loss = criterion(output, target)
print(loss)
```
- 1
- 2
- 3
- 4
- 5

### Backprop

```python
net.zero_grad()
print('conv1.bias.grad before backward')
print(net.conv1.bias.grad)#访问第一个卷积层的偏置的梯度的方法
loss.backward()
print('conv1.bias.grad after backward')
print(net.conv1.bias.grad)
```
- 1
- 2
- 3
- 4
- 5
- 6

### 权值更新

权值更新方法：

> 
weight = weight + learning_rate * gradient


```python
learning_rate = 0.01
for f in net.parameters():
    f.data.sub_(f.grad.data * learning_rate)
```



