# Pytorch之第一次构建神经网络（三） - qq_37385726的博客 - CSDN博客





2018年08月16日 17:08:37[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：149
所属专栏：[Pytorch](https://blog.csdn.net/column/details/26172.html)









## 本文为第一次构建神经网络系列第三篇

> 
**探讨如何构建损失函数和基于损失函数计算误差**


系列第一篇：[https://blog.csdn.net/qq_37385726/article/details/81740386](https://blog.csdn.net/qq_37385726/article/details/81740386)

系列第二篇：[https://blog.csdn.net/qq_37385726/article/details/81742247](https://blog.csdn.net/qq_37385726/article/details/81742247)

系列第三篇：[https://blog.csdn.net/qq_37385726/article/details/81744802](https://blog.csdn.net/qq_37385726/article/details/81744802)

系列第四篇：[https://blog.csdn.net/qq_37385726/article/details/81745510](https://blog.csdn.net/qq_37385726/article/details/81745510)

系列第五篇：[https://blog.csdn.net/qq_37385726/article/details/81748635](https://blog.csdn.net/qq_37385726/article/details/81748635)





# **目录**

[1.预构建网络](#1.%E9%A2%84%E6%9E%84%E5%BB%BA%E7%BD%91%E7%BB%9C%EF%BC%88%E5%8D%B3%E7%B3%BB%E5%88%97%E7%AC%AC%E4%B8%80%E7%AF%87%E5%AE%8C%E6%88%90%E7%9A%84%E9%83%A8%E5%88%86%EF%BC%89)

[网络结构](#%E8%BE%93%E5%87%BA)

[2.向网络传入输入，得到输出](#2.%E5%90%91%E7%BD%91%E7%BB%9C%E4%BC%A0%E5%85%A5%E8%BE%93%E5%85%A5%EF%BC%8C%E5%BE%97%E5%88%B0%E8%BE%93%E5%87%BA)

[3.构建损失函数，计算误差](#3.%E6%9E%84%E5%BB%BA%E6%8D%9F%E5%A4%B1%E5%87%BD%E6%95%B0%EF%BC%8C%E8%AE%A1%E7%AE%97%E8%AF%AF%E5%B7%AE)

[中间的曲折（不看跳过）](#%E4%B8%AD%E9%97%B4%E7%9A%84%E6%9B%B2%E6%8A%98%EF%BC%88%E4%B8%8D%E7%9C%8B%E8%B7%B3%E8%BF%87%EF%BC%89)

[构造](#%E6%9E%84%E9%80%A0)

# 1.预构建网络 

```python
class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        # 1 input image channel, 6 output channels, 5*5 square convolution
        # kernel

        self.conv1 = nn.Conv2d(in_channels=1, out_channels=32, kernel_size=5, stride=1, padding=2)
        self.conv2 = nn.Conv2d(in_channels=32, out_channels=64, kernel_size=5, stride=1, padding=2)
        # an affine operation: y = Wx + b
        self.fc1 = nn.Linear(64 * 8 * 8, 120)
        self.fc2 = nn.Linear(120, 84)
        self.fc3 = nn.Linear(84, 10)

    def forward(self, x):
        # max pooling over a (2, 2) window
        x = self.conv1(x)
        x = F.max_pool2d(F.relu(x), (2, 2))   #32*16*16
        # If size is a square you can only specify a single number
        x = F.max_pool2d(F.relu(self.conv2(x)), 2)   #64*8*8
        x = x.view(-1, self.num_flat_features(x))
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x

    def num_flat_features(self, x):
        size = x.size()[1:] # all dimensions except the batch dimension
        num_features = 1
        for s in size:
            num_features *= s
        return num_features

net = Net()
```



### 网络结构

Net(

  (conv1): Conv2d(1, 32, kernel_size=(5, 5), stride=(1, 1), padding=(2, 2))

  (conv2): Conv2d(32, 64, kernel_size=(5, 5), stride=(1, 1), padding=(2, 2))

  (fc1): Linear(in_features=4096, out_features=120, bias=True)

  (fc2): Linear(in_features=120, out_features=84, bias=True)

  (fc3): Linear(in_features=84, out_features=10, bias=True)

)





# 2.向网络传入输入，得到输出
- **传入输入的方式**

       将输入的variable作为参数传入到net中，即net(input)
- **得到输出的方式**

输出即为net(input)调用后的返回值

```python
input = Variable(torch.Tensor(1,1,32,32), requires_grad = True)  
out = net(input)   #将输入作为参数传入网络返回值即为输出
print(out)
```

**输出为**   tensor([[-0.1163,  0.0099,  0.0055, -0.0484,  0.1090, -0.0102, -0.1381,  0.0693,

                 -0.0400, -0.0166]], grad_fn=<ThAddmmBackward>)



# 3.构建损失函数，计算误差

> 
**损失函数定义在nn中，在这里我们使用MSELoss()即最小均方误差作为损失函数**

**Simply&& Generall**

**在回归问题上，我们使用 nn.MSELoss() 作为损失函数**

**在分类问题上，我们使用****nn.CrossEntropyLoss 作为损失函数**






## **中间的曲折（不看跳过）**
- **起初的构建方式**

```python
target = torch.arange(1,11)  #利用arange函数生成[10,]的张量，包装成variable
target = Variable(target,requires_grad = True)
```

  看起来很美好嘛，没什么问题，但是在我的pycharm上报错了【不同配置报不报错不一定】：报错的意思是说只有FloatTensor才能在requires_grad上为True。也就是说torch.arange(1,11)生成的是int型的张量，但是我看别人生成的明明是Float的嘛。。。（我装的是windows下无GPU，python3.6,Anaconda4.0的Pytorch）

  总之，反正报错就不能这样写。

  于是，我改成了

```python
target = torch.Tensor([0,1,2,3,4,5,6,7,8,9])
target = Variable(target,requires_grad = True)
```

  嗯，一看我这改法，我就认输了23333



 后来发现，可以用arange，但是参数改成浮点就行

```python
target = torch.arange(1.0,11.0)
target = Variable(target,requires_grad = True)
```

但其实target本来就不用requires_grad，考虑到loss_func接受的需要为FloatTensor，所以还是应该这样。





## 构造

```python
#Model预测输出

input = Variable(torch.rand(1,1,32,32))  #根据上述网络定义in_channels=1,所以输入为1，shape(32,32)
out = net(input)   #将输入作为参数传入网络返回值即为输出
print('out:\n',out)   

#对于input目标输出

target = torch.arange(1.0,11.0).reshape(1,10)
print('\n\ntarget:\n',target)

#损失函数

#定义损失函数，选择MSE最小均方误差作为损失函数
loss_func = nn.MSELoss()

#计算误差，误差即为利用损失函数计算out和target间的误差
loss = loss_func(out, target)
print('\n\nloss:\n',loss)
```

### **输出**

out:

 tensor([[-0.0423,  0.0614,  0.0607,  0.0778,  0.0255, -0.0705,  0.0049,  0.0573,

         -0.1050,  0.0537]], grad_fn=<ThAddmmBackward>)



target:

 tensor([ 1.,  2.,  3.,  4.,  5.,  6.,  7.,  8.,  9., 10.])



loss:

 tensor(384.3105, grad_fn=<SumBackward0>)



