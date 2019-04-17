# Pytorch入门笔记 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
import torch.nn as nn
import torch.nn.functional as F

class Net(nn.Module):
    def __init__(self):
        #nn.Module子类的函数必须在构造函数中执行父类的构造函数
        #下式等价于nn.Module.__init__(self)
        super(Net,self).__init__()
        # 卷积层‘1’表示输入图片为单通道，‘6’表示输出通道数，‘5’表示卷积核为5*5
        self.conv1 = nn.Conv2d(1,6,5)
        #卷积层
        self.conv2 = nn.Conv2d(6*1*1,16,5)
        #仿射层/全连接层，y = wx + b
        self.fc1 = nn.Linear(16*5*5,120)#输入是16不能改变，5应该是自己定义的卷积核
        self.fc2 = nn.Linear(120,84)
        self.fc3 = nn.Linear(84,10)


    def forward(self, x):
        # 卷积 ->激活 -> 池化
        x = F.max_pool2d(F.relu(self.conv1(x)),(2,2))
        x = F.max_pool2d(F.relu(self.conv2(x)),2)
        #reshape, '-1'表示自适应
        x = x.view(x.size()[0],-1)
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x

net = Net()
print(net)
```

__

Net(
  (conv1): Conv2d(1, 6, kernel_size=(5, 5), stride=(1, 1))
  (conv2): Conv2d(6, 16, kernel_size=(5, 5), stride=(1, 1))
  (fc1): Linear(in_features=400, out_features=120, bias=True)
  (fc2): Linear(in_features=120, out_features=84, bias=True)
  (fc3): Linear(in_features=84, out_features=10, bias=True)
)

——

 CIFAR-10分类

下载数据集：

```
import torch as t
import torchvision as tv
import torchvision.transforms as transforms
from torchvision.transforms import ToPILImage
show = ToPILImage() #可以把Tensor 转成Image,方便可视化

#第一次运行torchvision 会自动下载CIFAR-10数据集

#定义对数据的预处理
transform = transforms.Compose([
    transforms.ToTensor, # 转为Tensor
    transforms.Normalize((0.5,0.5,0.5),(0.5,0.5,0.5)), #归一化
])

#训练集
trainset = tv.datasets.CIFAR10(
    root='./data',
    train=True,
    download=True,
    transform=transform
)

trainloader = t.utils.data.DataLoader(
    trainset,
    batch_size=4,
    shuffle=True,
    num_workers=2
)

#测试集
testset = tv.datasets.CIFAR10(
    './data',
    train=False,
    download=True,
    transform=transform
)

testloader = t.utils.data.DataLoader(
    testset,
    batch_size=4,
    shuffle=False,
    num_workers=2
)

classes = (
    'plane','car','bird','cat','deer','dog','frog','horse','ship','truck'
)
```

——

需要FQ，不然下载非常的慢

Downloading https://www.cs.toronto.edu/~kriz/cifar-10-python.tar.gz to ./data\cifar-10-python.tar.gz
Files already downloaded and verified

——

全代码：

在CPU上训练，训练了5个epoch，准确率大概在60%，时间在170s左右

windows上不支持多线程

```
import torch as t
import torchvision as tv
import torchvision.transforms as transforms
from torchvision.transforms import ToPILImage
import numpy as np
import matplotlib.pyplot as plt
show = ToPILImage() #可以把Tensor 转成Image,方便可视化

#第一次运行torchvision 会自动下载CIFAR-10数据集

#定义对数据的预处理
transform = transforms.Compose([
    transforms.ToTensor(), # 转为Tensor
    transforms.Normalize((0.5,0.5,0.5),(0.5,0.5,0.5)), #归一化
])

#训练集
trainset = tv.datasets.CIFAR10(
    root='./data',
    train=True,
    download=True,
    transform=transform
)
#注意windows现在还不支持多线程，所以num_workers=0
trainloader = t.utils.data.DataLoader(
    trainset,
    batch_size=4,
    shuffle=True,
    num_workers=0
)

#测试集
testset = tv.datasets.CIFAR10(
    './data',
    train=False,
    download=True,
    transform=transform
)

testloader = t.utils.data.DataLoader(
    testset,
    batch_size=4,
    shuffle=False,
    num_workers=0
)

classes = (
    'plane','car','bird','cat','deer','dog','frog','horse','ship','truck'
)

(data,label) = trainset[100]
print(classes[label])

# （data+1)/2是为了还原被归一化的数据
show((data+1)/2).resize((100,100))

# dataiter=iter(trainloader)
# images,labels=dataiter.next()
# print(''.join('11%s'%classes[labels[j]] for j in range(4)))
# show(tv.utils.make_grid(images+1)/2).resize((400,100))
def imshow(img):
    img = img / 2 + 0.5
    npimg = img.numpy()
    plt.imshow(np.transpose(npimg, (1, 2, 0)))

dataiter = iter(trainloader)
images, labels = dataiter.next()
print(images.size())
imshow(tv.utils.make_grid(images))
plt.show()#关掉图片才能往后继续算

#卷积网络
import torch.nn as nn
import torch.nn.functional as F

class Net(nn.Module):
    def __init__(self):
        super(Net,self).__init__()
        self.conv1 = nn.Conv2d(3,6,5)
        self.conv2 = nn.Conv2d(6,16,5)
        self.fc1 = nn.Linear(16*5*5,120)
        self.fc2 = nn.Linear(120,84)
        self.fc3 = nn.Linear(84,10)


    def forward(self, x):
        x = F.max_pool2d(F.relu(self.conv1(x)),(2,2))
        x = F.max_pool2d(F.relu(self.conv2(x)),2)

        x = x.view(x.size()[0],-1)
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x

net = Net()
print(net)

#损失函数和优化器
from torch import optim
criterion = nn.CrossEntropyLoss()#交叉熵损失函数
optimizer = optim.SGD(net.parameters(),lr=0.001,momentum=0.9)

#训练网络
from torch.autograd import Variable
import time

tstart = time.time()
for epoch in range(5):
    running_loss = 0.0
    for i, data in enumerate(trainloader,0): #enumerate 用法见注释
        #输入数据
        inputs,labels = data
        inputs,labels = Variable(inputs),Variable(labels)

        #梯度清零
        optimizer.zero_grad()
        #forward+backward
        outputs = net(inputs)
        loss = criterion(outputs,labels)
        loss.backward()
        #更新参数
        optimizer.step()

        #打印log（日志）信息
        running_loss += loss.data[0]
        if i%2000 == 1999: #每2000个batch打印一次训练状态
            print('[%d,%5d] loss:%.3f' %(epoch+1,i+1,running_loss/2000))
            running_loss = 0.0
print('finished')
tend = time.time()
print('Spend time = ',tend - tstart)


correct = 0 #正确数
total = 0 #总数
for data in testloader:
    images,labels = data
    outputs = net(Variable(images))
    _,predicted = t.max(outputs.data,1)
    total += labels.size(0)
    correct += (predicted == labels).sum()


print('%d %%' %(100*correct/total)
```

——

补充注释：enumerate() 函数

enumerate() 函数用于将一个可遍历的数据对象(如列表、元组或字符串)组合为一个索引序列，同时列出数据和数据下标，一般用在 for 循环当中。

Python 2.3. 以上版本可用，2.6 添加 start 参数。

### 语法

以下是 enumerate() 方法的语法:
enumerate(sequence,[start=0])
### 参数
- sequence -- 一个序列、迭代器或其他支持迭代对象。
- start -- 下标起始位置。

### 返回值

返回 enumerate(枚举) 对象。

## 实例

以下展示了使用 enumerate() 方法的实例：



>>>seasons = ['Spring', 'Summer', 'Fall', 'Winter'] >>> list(enumerate(seasons))[(0, 'Spring'), (1, 'Summer'), (2, 'Fall'), (3, 'Winter')] >>> list(enumerate(seasons, start=1))# 小标从 1 开始[(1, 'Spring'), (2, 'Summer'), (3, 'Fall'), (4, 'Winter')]



 ——

GPU版本，跑的比CPU还慢？？

```
import torch as t
import torchvision as tv
import torchvision.transforms as transforms
from torchvision.transforms import ToPILImage
import numpy as np
import matplotlib.pyplot as plt
show = ToPILImage() #可以把Tensor 转成Image,方便可视化

#第一次运行torchvision 会自动下载CIFAR-10数据集

#定义对数据的预处理
transform = transforms.Compose([
    transforms.ToTensor(), # 转为Tensor
    transforms.Normalize((0.5,0.5,0.5),(0.5,0.5,0.5)), #归一化
])

#训练集
trainset = tv.datasets.CIFAR10(
    root='./data',
    train=True,
    download=True,
    transform=transform
)
#注意windows现在还不支持多线程，所以num_workers=0
trainloader = t.utils.data.DataLoader(
    trainset,
    batch_size=4,
    shuffle=True,
    num_workers=0
)

#测试集
testset = tv.datasets.CIFAR10(
    './data',
    train=False,
    download=True,
    transform=transform
)

testloader = t.utils.data.DataLoader(
    testset,
    batch_size=4,
    shuffle=False,
    num_workers=0
)

classes = (
    'plane','car','bird','cat','deer','dog','frog','horse','ship','truck'
)

(data,label) = trainset[100]
print(classes[label])

# （data+1)/2是为了还原被归一化的数据
show((data+1)/2).resize((100,100))

# dataiter=iter(trainloader)
# images,labels=dataiter.next()
# print(''.join('11%s'%classes[labels[j]] for j in range(4)))
# show(tv.utils.make_grid(images+1)/2).resize((400,100))
def imshow(img):
    img = img / 2 + 0.5
    npimg = img.numpy()
    plt.imshow(np.transpose(npimg, (1, 2, 0)))

dataiter = iter(trainloader)
images, labels = dataiter.next()
print(images.size())
imshow(tv.utils.make_grid(images))
plt.show()#关掉图片才能往后继续算

#卷积网络
import torch.nn as nn
import torch.nn.functional as F

class Net(nn.Module):
    def __init__(self):
        super(Net,self).__init__()
        self.conv1 = nn.Conv2d(3,6,5)
        self.conv2 = nn.Conv2d(6,16,5)
        self.fc1 = nn.Linear(16*5*5,120)
        self.fc2 = nn.Linear(120,84)
        self.fc3 = nn.Linear(84,10)


    def forward(self, x):
        x = F.max_pool2d(F.relu(self.conv1(x)),(2,2))
        x = F.max_pool2d(F.relu(self.conv2(x)),2)

        x = x.view(x.size()[0],-1)
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x

net = Net()
if t.cuda.is_available():
    net.cuda()
print(net)

#损失函数和优化器
from torch import optim
criterion = nn.CrossEntropyLoss()#交叉熵损失函数
optimizer = optim.SGD(net.parameters(),lr=0.001,momentum=0.9)

#训练网络
from torch.autograd import Variable
import time

tstart = time.time()
for epoch in range(5):
    running_loss = 0.0
    for i, data in enumerate(trainloader,0): #enumerate 用法见注释
        #输入数据
        inputs,labels = data
        inputs,labels = Variable(inputs),Variable(labels)
        if t.cuda.is_available():
            inputs = inputs.cuda()
            labels = labels.cuda()
        #梯度清零
        optimizer.zero_grad()
        #forward+backward
        outputs = net(inputs)
        loss = criterion(outputs,labels)
        loss.backward()
        #更新参数
        optimizer.step()

        #打印log（日志）信息
        running_loss += loss.data[0]
        if i%2000 == 1999: #每2000个batch打印一次训练状态
            print('[%d,%5d] loss:%.3f' %(epoch+1,i+1,running_loss/2000))
            running_loss = 0.0
print('finished')
tend = time.time()
print('Spend time = ',tend - tstart)


correct = 0 #正确数
total = 0 #总数
for data in testloader:
    images,labels = data
    images = Variable(images)

    if t.cuda.is_available():
        images = images.cuda()

    outputs = net(images)
    _,predicted = t.max(outputs.data,1)
    total += labels.size(0)
    correct += (predicted == labels.cuda()).sum()


print('%d %%' %(100*correct/total))
```

____

 CNN + MNIST

```
import torch
from torchvision import datasets, transforms
import matplotlib.pyplot as plt
import os
import torchvision
import numpy as np
from torch.autograd import Variable

#数据预处理
transform = transforms.Compose([transforms.ToTensor(),
                               transforms.Normalize(mean=[0.5,0.5,0.5],std=[0.5,0.5,0.5])])
#下载MNIST
data_train = datasets.MNIST(root = "./data1",
                            transform=transform,
                            train = True,
                            download = True)

data_test = datasets.MNIST(root="./data1",
                           transform = transform,
                           train = False)

data_loader_train = torch.utils.data.DataLoader(dataset=data_train,
                                                batch_size = 64,
                                                shuffle = True,
                                                 num_workers=0)

data_loader_test = torch.utils.data.DataLoader(dataset=data_test,
                                               batch_size = 64,
                                               shuffle = True,
                                                num_workers=0)

#定义卷积网络
# 这里构建的是一个包含了卷积层和全连接层的神经网络，
# 其中卷积层使用torch.nn.Conv2d来构建，
# 激活层使用torch.nn.ReLU来构建，
# 池化层使用torch.nn.MaxPool2d来构建，
# 全连接层使用torch.nn.Linear来构建(dense)
class Model(torch.nn.Module):

    def __init__(self):
        super(Model, self).__init__()
        self.conv1 = torch.nn.Sequential(torch.nn.Conv2d(1, 4, kernel_size=3, stride=1, padding=1),
                                         torch.nn.ReLU(),
                                         torch.nn.Conv2d(4, 16, kernel_size=3, stride=1, padding=1),
                                         torch.nn.ReLU(),
                                         torch.nn.MaxPool2d(stride=2, kernel_size=2))
        #
        self.dense = torch.nn.Sequential(torch.nn.Linear(14 * 14 * 16, 33),  #只有这个14*14不能瞎改不知道为啥
                                         torch.nn.ReLU(),
                                         torch.nn.Dropout(p=0.5),#防止过拟合
                                         torch.nn.Linear(33, 10))

    def forward(self, x):
        x = self.conv1(x)
        # x = self.conv2(x)
        x = x.view(-1, 14 * 14 * 16)
        x = self.dense(x)
        return x

model = Model().cuda()
print(model)


cost = torch.nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(model.parameters())
n_epochs = 5

for epoch in range(n_epochs):
    running_loss = 0.0
    running_correct = 0
    print("Epoch {}/{}".format(epoch, n_epochs))
    print("-" * 10)
    for data in data_loader_train:
        X_train, y_train = data
        X_train, y_train = Variable(X_train).cuda(), Variable(y_train).cuda()
        outputs = model(X_train)
        _, pred = torch.max(outputs.data, 1)
        optimizer.zero_grad()
        loss = cost(outputs, y_train)

        loss.backward()
        optimizer.step()
        running_loss += loss.data[0]
        running_correct += torch.sum(pred == y_train.data)
    testing_correct = 0
    for data in data_loader_test:
        X_test, y_test = data
        X_test, y_test = Variable(X_test).cuda(), Variable(y_test).cuda()
        outputs = model(X_test)
        _, pred = torch.max(outputs.data, 1)
        testing_correct += torch.sum(pred == y_test.data)
    print("Loss is:{:.4f}, Train Accuracy is:{:.4f}%, Test Accuracy is:{:.4f}".format(running_loss / len(data_train),
                                                                                      100 * running_correct / len(
                                                                                          data_train),
                                                                                      100 * testing_correct / len(
                                                                                          data_test)))
```

___













