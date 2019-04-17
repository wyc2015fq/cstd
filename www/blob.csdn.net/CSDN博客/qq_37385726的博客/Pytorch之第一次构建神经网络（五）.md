# Pytorch之第一次构建神经网络（五） - qq_37385726的博客 - CSDN博客





2018年08月16日 21:00:10[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：107
所属专栏：[Pytorch](https://blog.csdn.net/column/details/26172.html)









## 本文为第一次构建神经网络系列第五篇

> 
**探讨构建优化器来更新参数**


系列第一篇：[https://blog.csdn.net/qq_37385726/article/details/81740386](https://blog.csdn.net/qq_37385726/article/details/81740386)

系列第二篇：[https://blog.csdn.net/qq_37385726/article/details/81742247](https://blog.csdn.net/qq_37385726/article/details/81742247)

系列第三篇：[https://blog.csdn.net/qq_37385726/article/details/81744802](https://blog.csdn.net/qq_37385726/article/details/81744802)

系列第四篇：[https://blog.csdn.net/qq_37385726/article/details/81745510](https://blog.csdn.net/qq_37385726/article/details/81745510)

系列第五篇：[https://blog.csdn.net/qq_37385726/article/details/81748635](https://blog.csdn.net/qq_37385726/article/details/81748635)



# **目录**

[1.预构建网络](https://blog.csdn.net/qq_37385726/article/details/81744802#1.%E9%A2%84%E6%9E%84%E5%BB%BA%E7%BD%91%E7%BB%9C%EF%BC%88%E5%8D%B3%E7%B3%BB%E5%88%97%E7%AC%AC%E4%B8%80%E7%AF%87%E5%AE%8C%E6%88%90%E7%9A%84%E9%83%A8%E5%88%86%EF%BC%89)

[网络结构](https://blog.csdn.net/qq_37385726/article/details/81744802#%E8%BE%93%E5%87%BA)

[2.向网络传入输入，得到输出](https://blog.csdn.net/qq_37385726/article/details/81744802#2.%E5%90%91%E7%BD%91%E7%BB%9C%E4%BC%A0%E5%85%A5%E8%BE%93%E5%85%A5%EF%BC%8C%E5%BE%97%E5%88%B0%E8%BE%93%E5%87%BA)

[3.构建损失函数，计算误差](https://blog.csdn.net/qq_37385726/article/details/81744802#3.%E6%9E%84%E5%BB%BA%E6%8D%9F%E5%A4%B1%E5%87%BD%E6%95%B0%EF%BC%8C%E8%AE%A1%E7%AE%97%E8%AF%AF%E5%B7%AE)

[4. 反向传播，计算梯度](#4.%20%E5%8F%8D%E5%90%91%E4%BC%A0%E6%92%AD%EF%BC%8C%E8%AE%A1%E7%AE%97%E6%A2%AF%E5%BA%A6)

[5.构建优化器，更新参数](#5.%E6%9E%84%E5%BB%BA%E4%BC%98%E5%8C%96%E5%99%A8%EF%BC%8C%E6%9B%B4%E6%96%B0%E5%8F%82%E6%95%B0%C2%A0)

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


**先记住一点，在神经网络计算中接受的都是variable而不是tensor，所以在构建target的时候要包装成variable**



## 构造

```python
#Model预测输出
 
input = Variable(torch.rand(1,1,32,32))  #根据上述网络定义in_channels=1,所以输入为1，shape(32,32)
out = net(input)   #将输入作为参数传入网络返回值即为输出
print('out:\n',out)   
 
#对于input目标输出
 
target = torch.arange(1.0,11.0)
target = Variable(target,requires_grad = True)
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

 tensor([ 1.,  2.,  3.,  4.,  5.,  6.,  7.,  8.,  9., 10.], requires_grad=True)



loss:

 tensor(384.3105, grad_fn=<SumBackward0>)



# 4. 反向传播，计算梯度

关于反向传播，我们使用backward函数：

预知识可以查看： [Pytorch之浅入backward](https://blog.csdn.net/qq_37385726/article/details/81705621)

[Pytorch之深入backward](https://blog.csdn.net/qq_37385726/article/details/81707089)

看过之后，在想为什么要反向传播的，为什么要计算梯度，为什么要清零的，自行了解神经网络BP算法

```python
#基于loss，反向传播，计算梯度
net.zero_grad()   #将梯度均置0
loss.backward()
print('\n\ngrad:\n',input.grad)
```

**输出  （loss对于input的梯度）**

grad:

 tensor([[[[ 1.4667e-02,  1.0938e-02, -1.9200e-03,  ...,  3.0458e-02,

           -2.6207e-02,  4.9422e-04],

          [-1.4665e-02,  1.6973e-02,  3.3222e-03,  ..., -1.9088e-02,

           -3.2410e-02,  5.6499e-03],

          [ 1.6416e-02, -2.5757e-02,  1.3336e-02,  ..., -3.5478e-02,

            1.8651e-02, -2.6204e-02],

          ...,

          [ 9.0965e-03, -4.8741e-03, -1.5085e-02,  ..., -5.3854e-03,

            3.5103e-02, -7.2583e-03],

          [ 1.5662e-02,  2.1850e-03,  7.3939e-03,  ..., -6.1694e-03,

           -1.7021e-03,  3.7368e-03],

          [ 1.2588e-02,  2.3279e-02,  1.4130e-02,  ...,  2.2284e-03,

            2.8160e-02,  5.8273e-04]]]])





# 5.构建优化器，更新参数 

实践中最简单的更新规则是随机梯度下降(SGD)．

weight=weight−learning_rate∗gradientweight=weight−learning_rate∗gradient

我们可以使用简单的Python代码实现这个规则.

```
learning_rate = 0.01
for f in net.parameters():
    f.data.sub_(f.grad.data * learning_rate)
```

 但是对于更复杂的原则，书写Python代码就会更麻烦，所以Pytorch贴心的打了一个torch.optim的包，包中是用于更新参数的函数。
- **引入包 **  import torch.optim as optim
- **构建优化器 ** op = optim.原则(net.parametes(),lr = 0.5)   
- **更新参数**  op.step()

```python
#定义优化器,给定要改变的参数和学习效率
op = optim.SGD(net.parameters(),lr=0.2)

#利用优化器使梯度为0
op.zero_grad()

#基于loss，反向传播，计算梯度
loss.backward()

#更新参数
op.step()
```





