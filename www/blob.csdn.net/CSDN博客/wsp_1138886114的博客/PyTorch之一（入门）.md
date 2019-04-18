# PyTorch之一（入门） - wsp_1138886114的博客 - CSDN博客





2019年01月27日 09:48:37[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：102











### 文章目录
- [一、Pytorch 简介](#Pytorch__1)
- [二、Pytorch 常用工具包](#Pytorch__22)
- [三、pytorch 安装](#pytorch__31)
- [四、torch 模块—数学操作符](#torch__45)
- [4.1 简单的数据构建（ torch.cat()矩阵拼接）](#41__torchcat_60)
- [4.2 简单的 tensor 运算（numpy桥，CUDA）](#42__tensor_numpyCUDA_121)
- [五、反向传播](#_170)
- [自动微分](#_171)
- [梯度(Gradients) out.backward()](#Gradients_outbackward_193)
- [六、神经网络](#_235)




### 一、Pytorch 简介

Pytorch是Facebook 的 AI 研究团队发布了一个基于 Python的科学计算包。

关于 Pytorch 的学习详情你可以查看资料：

Pytorch 中文文档：[http://pytorch-cn.readthedocs.io/zh/latest/#pytorch](http://pytorch-cn.readthedocs.io/zh/latest/#pytorch)

Pytorch 官方文档：[https://pytorch.org/docs/stable/index.html](https://pytorch.org/docs/stable/index.html)- 服务场景

替代numpy发挥GPU潜能(在线环境暂时不支持GPU)

一个提供了高度灵活性和效率的深度学习实验性平台
- Pytorch特点及优势

PyTorch 提供了运行在 GPU/CPU 之上、基础的张量操作库(优点)

可以内置的神经网络库；

提供模型训练功能；

支持共享内存的多进程并发（multiprocessing ）库等；(优点)
处于机器学习第一大语言 Python 的生态圈之中，使得开发者能使用广大的 **Python 库**和软件；如 **NumPy**、SciPy 和 Cython（为了速度把 Python 编译成 C 语言）；

（最大优势）改进现有的神经网络，提供了更快速的方法——不需要从头重新构建整个网络，这是由于 PyTorch 采用了动态计算图（dynamic computational graph）结构，而不是大多数开源框架（TensorFlow、Caffe、CNTK、Theano 等）采用的静态计算图。

### 二、Pytorch 常用工具包
- **`torch`** ：类似 NumPy 的张量库，强 GPU 支持 ；
- **`torch.autograd`** ：基于 tape 的自动区别库，支持 torch 之中的所有可区分张量运行；
- **`torch.nn`** ：为最大化灵活性未涉及、与 autograd 深度整合的神经网络库；
- **`torch.optim`**：与 torch.nn 一起使用的优化包，包含 SGD、RMSProp、LBFGS、Adam 等标准优化方式；
- **`torch.multiprocessing`**： python 多进程并发，进程之间 torch Tensors 的内存共享；
- **`torch.utils`**：数据载入器。具有训练器和其他便利功能（用于图像加载等）；
- **`torch.legacy(.nn/.optim)`** ：处于向后兼容性考虑，从 Torch 移植来的 legacy 代码；

### 三、pytorch 安装

官方网站：[https://pytorch.org/](https://pytorch.org/) 或者 [https://pytorch.org/get-started/locally/](https://pytorch.org/get-started/locally/)

这里我的环境是 **win10 | Anaconda | cuda80 | python ==3.6**

关于CUDA的[安装详情你可以点击](https://blog.csdn.net/wsp_1138886114/article/details/84864908)。
[创建Anaconda虚拟环境](https://blog.csdn.net/wsp_1138886114/article/details/80438993)，并激活。

执行如下命令安装：
```python
conda install pytorch -c pytorch
pip install torchvision

或者：
conda install pytorch torchvision cuda80 -c pytorch
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190122141027129.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

### 四、torch 模块—数学操作符

**torch 大部分关于数据/矩阵的操作 与numpy 一样**

要了解更多请查看：[https://pytorch.org/docs/stable/torch.html](https://pytorch.org/docs/stable/torch.html)
- 
**`torch.numel()`** 返回一个tensor变量内 **所有元素个数**，可以理解为矩阵内元素的个数

- 
**`torch.squeeze()`** 对于tensor变量进行维度压缩，去除维数为1的的维度。例如一矩阵维度为A*1*B*C*1*D，通过squeeze()返回向量的维度为A*B*C*D。squeeze(a)，表示将a的维数位1的维度删掉，squeeze(a,N)表示，如果第N维维数为1，则压缩去掉，否则a矩阵不变

- 
**`torch.unsqueeze()`** 是squeeze()的反向操作，增加一个维度，该维度维数为1，可以指定添加的维度。例如unsqueeze(a,1)表示在1这个维度进行添加

- 
**`torch.stack(sequence, dim=0, out=None)`**，做tensor的拼接。sequence表示Tensor列表，dim表示拼接的维度，注意这个函数和concatenate是不同的，torch的concatenate函数是torch.cat，是在已有的维度上拼接，而stack是建立一个新的维度，然后再在该纬度上进行拼接。

- 
**`expand_as(a)`** 这是tensor变量的一个内置方法，如果使用b.expand_as(a)就是将b进行扩充，扩充到a的维度，需要说明的是a的低维度需要比b大，例如b的shape是3*1，如果a的shape是3*2不会出错，但是是2*2就会报错了。

如上函数操作详情：[https://blog.csdn.net/zw__chen/article/details/82801364](https://blog.csdn.net/zw__chen/article/details/82801364)


##### 4.1 简单的数据构建（ torch.cat()矩阵拼接）

```python
import torch     

# 与numpy一样构造tensor数据：我这里只举了几个例自
test1 = torch.randn(2,3)       # 构造一个标准正态分布(均值为0，方差为 1)随机数的2X3的tensor矩阵
test2 = torch.zeros(3,3)       # 构造全为0的3X3的tensor矩阵

print("test1:",test1)
print("test2:",test2)

test3 = torch.Tensor([[1,2,3],[2,3,4]])
shape = test3.size()
X_range = torch.range(1,5)

print("test3:",test3)
print(shape)
print("test3[0]:",test3[0])
print("test3[1]:",test3[1])
print("X_range:",X_range)

输出：
test1: tensor([[ 0.9483, -0.5510,  2.2674],
               [ 0.9016, -0.7576, -0.5092]])
test2: tensor([[0., 0., 0.],
               [0., 0., 0.],
               [0., 0., 0.]])
test3: tensor([[1., 2., 3.],
               [2., 3., 4.]])
torch.Size([2, 3])
test3[0]: tensor([1., 2., 3.])
test3[1]: tensor([2., 3., 4.])
X_range:  tensor([1., 2., 3., 4., 5.])
```

**`torch.cat()`** tensor 矩阵拼接

```python
import torch

A = torch.ones([2,3])
B = torch.ones([2,3])

cat1_A_B = torch.cat([A,B],1) #按列拼接
cat0_A_B = torch.cat([A,B],0) #按行拼接，可写成 torch.cat([A,B])

print("A:",A)
print("B:",B)
print("cat1_A_B:",cat1_A_B)
print("cat0_A_B:",cat0_A_B)

输出：
A: tensor([[1., 1., 1.],
           [1., 1., 1.]])
B: tensor([[1., 1., 1.],
           [1., 1., 1.]])
cat1_A_B: tensor([[1., 1., 1., 1., 1., 1.],
                  [1., 1., 1., 1., 1., 1.]])
cat0_A_B: tensor([[1., 1., 1.],
                  [1., 1., 1.],
                  [1., 1., 1.],
                  [1., 1., 1.]])
```

##### 4.2 简单的 tensor 运算（numpy桥，CUDA）

**tensor 相乘，转置**

```python
x = torch.rand(3,4)     # 创建一个3X4 的随机数矩阵
y = torch.ones(3,4)     # 创建一个4X3 全为1的矩阵
result = x.mm(y.t())    # x 乘以 y 的转置（x矩阵行与y矩阵列要相同）

print("result",result)
输出：
result：tensor([[3.0228, 3.0228, 3.0228],
		        [2.1409, 2.1409, 2.1409],
		        [2.2339, 2.2339, 2.2339]])
```

**numpy 桥**

```python
# torch和numpy的数据类型相互转化 （它们之间共享存储空间，所有的运算也会相应被改变）
One = torch.ones(3,3)
arr = One.numpy()              # torch --> numpy
print("arr",arr)

Two = np.ones((3,3))
tensor = torch.from_numpy(Two) # numpy --> torch
print("tensor",tensor)

输出：
arr [[1. 1. 1.]
    [1. 1. 1.]
    [1. 1. 1.]]
tensor tensor([[1., 1., 1.],
               [1., 1., 1.],
               [1., 1., 1.]], dtype=torch.float64)
```

**CUDA 转移数据GPU运行**

```python
import torch

x = torch.rand(3,4)     # 创建一个3X4 的随机数矩阵
y = torch.ones(3,4)     # 创建一个4X3 全为1的矩阵

# 使用CUDA函数将Tensor转到GPU上来使用（当CUDA可用时，进行GPU运算）
if torch.cuda.is_available():
    x = x.cuda()
    y = y.cuda()
    print(x+y)
输出：
tensor([[1.7110, 1.9558, 1.0765, 1.6623],
        [1.9142, 1.5933, 1.0391, 1.9717],
        [1.1714, 1.2906, 1.3377, 1.1677]], device='cuda:0')
```

### 五、反向传播

##### 自动微分

```python
import torch
from torch.autograd import Variable

X = Variable(torch.ones(2,2),requires_grad = True)  # 定义自动微分变量
Y = X+2
Z = torch.mean(Y*Y)
print("Y:",Y)
print("Z:",Z)
print("Z.data:",Z.data)
print("Y.grad_fn:",Y.grad_fn)   # Y 是运算创建的，有grad_fn
print("X.grad_fn",X.grad_fn)    # X 是用户创建的，为None

输出：
Y: tensor([[3., 3.],
           [3., 3.]], grad_fn=<AddBackward0>)
Z: tensor(9., grad_fn=<MeanBackward1>)
Z.data: tensor(9.)
Y.grad_fn: <AddBackward0 object at 0x00000264CD6F2CC0>
X.grad_fn None
```

##### 梯度(Gradients) out.backward()

在 pytorch 中一般调用backward默认进行一次自动求导，如果再次调用backward，则会报错。因为PyTorch默认做完一次自动求导后，计算图就被丢弃了，所以两次及以上自动求导需要手动设置一个参数 来进行多次自动求导
- 设置 retain_graph 为 True 来保留计算图

y.backward(retain_graph=True)

```python
import torch
from torch.autograd import Variable

X = Variable(torch.ones(2,2),requires_grad = True)  # 定义自动微分变量
S = Variable(torch.FloatTensor([[0.01,0.02]]),requires_grad = True)
for i in range(15):
    S = S.mm(X)          # tensor 矩阵相乘(15连乘)
Z= torch.mean(S)

Z.backward()             # backward()求导计算
print("Z:",Z)
print("X.grad:",X.grad)
print("S.grad:",S.grad)
输出：
Z: tensor(491.5200, grad_fn=<MeanBackward1>)
X.grad: tensor([[1802.2400, 1802.2400],
                [1884.1599, 1884.1599]])
S.grad: None
```

举例：如下的一些操作，数学公式 $z = x^2+3$

那么从 z 到 x 求导的结果就是：$\frac{\partial z}{\partial x}=2(x+2) = 2(2+2)=8$

```python
import torch
from torch.autograd import Variable

x = Variable(torch.Tensor([2]),requires_grad=True)
y = x+2
z = y**2+3

z.backward()      # 使用自动求导
print("z:",z)
print("x.grad:",x.grad)
输出：
z: tensor([19.], grad_fn=<AddBackward0>)
x.grad: tensor([8.])
```

### 六、神经网络

用 torch.nn 包可以进行神经网络的构建。

nn建立在autograd的基础上来进行模型的定义和微分。 nn.Module中包含着神经网络的层，同时forward(input)方法能够将output进行返回。

一个典型的神经网络的训练过程：
- 定义一个有着可学习的参数（或者权重）的神经网络
- 对着一个输入的数据集进行迭代:
- 用神经网络对输入进行处理
- 计算代价值：输出结果和正确值的差距大小 (对输出值的修正到底有多少)
- 将梯度传播回神经网络的参数中
- 更新网络中的权重 通常使用简单的更新规则:

weight = weight + learning_rate * gradient

定义一个网络

```python
import torch
from torch.autograd import Variable
import torch.nn as nn
import torch.nn.functional as F

class Net(nn.Module):
    def __init__(self):
        super(Net,self).__init__()
        self.conv1 = nn.Conv2d(1,6,5) # input_channal=1；output_channels=6；conv_kernel=5x5
        self.conv2 = nn.Conv2d(6, 16, 5)
        self.fc1   = nn.Linear(16*5*5, 120) # an affine operation: y = Wx + b
        self.fc2   = nn.Linear(120, 84)
        self.fc3   = nn.Linear(84, 10)

    def forward(self, x):
        x = F.max_pool2d(F.relu(self.conv1(x)),(2,2)) # 卷积池化
        x = F.max_pool2d(F.relu(self.conv2(x)),2)     # 卷积池化
        x = x.view(-1,self.num_flat_features(x))   # 展平
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x

    def num_flat_features(self,x):
        size = x.size()[1:]
        num_features = 1
        for s in size:
            num_features *=s
        return num_features

net = Net()
print(net)

# 输出参数：模型中可学习的参数会由net.parameters()返回。
params = list(net.parameters())
for i in range(len(params)):
    print(params[i].size())

# forward的输入和输出都是 autograd.Variable
input = Variable(torch.rand(1,1,32,32))
out = net(input)
print(out)

# 将所有的参数的梯度缓存清零，然后进行随机反向传播
net.zero_grad()
out.backward(torch.randn(1,10))

输出：
Net(
  (conv1): Conv2d(1, 6, kernel_size=(5, 5), stride=(1, 1))
  (conv2): Conv2d(6, 16, kernel_size=(5, 5), stride=(1, 1))
  (fc1): Linear(in_features=400, out_features=120, bias=True)
  (fc2): Linear(in_features=120, out_features=84, bias=True)
  (fc3): Linear(in_features=84, out_features=10, bias=True)
)
torch.Size([6, 1, 5, 5])
torch.Size([6])
torch.Size([16, 6, 5, 5])
torch.Size([16])
torch.Size([120, 400])
torch.Size([120])
torch.Size([84, 120])
torch.Size([84])
torch.Size([10, 84])
torch.Size([10])
tensor([[ 0.1371,  0.0422, -0.0763, -0.1114, -0.0088,  0.0838, -0.0175, -0.1475,
          0.0531, -0.0619]], grad_fn=<AddmmBackward>)
```

PyTorch看起来真的像一个框架了，而纯TensorFlow看起来更像是一个库，而不是框架：所有的操作都在低层次进行，因此你不得不编写大量的样板代码（我们需要一次又一次地定义这些偏差和权重等等）。但后者，在使用TensorFlow以及选择适合任务的框架上有着很大的自由度，你可以在这些框架中选择：TFLearn、tf.contrib.learn、Sonnet、Keras、或者纯tf.layers等等。

所以，TensorFlow和PyTorch都提供了有用的抽象来减少样板代码的数量并加快模型的开发。它们之间主要的区别在于，PyTorch可能感觉更“Python化”，并且有面向对象的方法，而TensorFlow则有多个框架可供选择。

鸣谢
[https://pytorch.org/docs/stable/torch.html](https://pytorch.org/docs/stable/torch.html)
[https://blog.csdn.net/Gavinmiaoc/article/details/80375837](https://blog.csdn.net/Gavinmiaoc/article/details/80375837)






