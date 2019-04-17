# Pytorch之第一次构建神经网络（二） - qq_37385726的博客 - CSDN博客





2018年08月16日 15:27:13[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：274
所属专栏：[Pytorch](https://blog.csdn.net/column/details/26172.html)









## 本文为第一次构建神经网络系列第二篇

> 
**探讨如何向神经网络中传入参数，得到返回结果**


系列第一篇：[https://blog.csdn.net/qq_37385726/article/details/81740386](https://blog.csdn.net/qq_37385726/article/details/81740386)

系列第二篇：[https://blog.csdn.net/qq_37385726/article/details/81742247](https://blog.csdn.net/qq_37385726/article/details/81742247)

系列第三篇：[https://blog.csdn.net/qq_37385726/article/details/81744802](https://blog.csdn.net/qq_37385726/article/details/81744802)

系列第四篇：[https://blog.csdn.net/qq_37385726/article/details/81745510](https://blog.csdn.net/qq_37385726/article/details/81745510)

系列第五篇：[https://blog.csdn.net/qq_37385726/article/details/81748635](https://blog.csdn.net/qq_37385726/article/details/81748635)





# **目录**

[1.预构建网络](#1.%E9%A2%84%E6%9E%84%E5%BB%BA%E7%BD%91%E7%BB%9C%EF%BC%88%E5%8D%B3%E7%B3%BB%E5%88%97%E7%AC%AC%E4%B8%80%E7%AF%87%E5%AE%8C%E6%88%90%E7%9A%84%E9%83%A8%E5%88%86%EF%BC%89)

[网络结构](#%E8%BE%93%E5%87%BA)

[2.向网络传入输入，得到输出](#2.%E5%90%91%E7%BD%91%E7%BB%9C%E4%BC%A0%E5%85%A5%E8%BE%93%E5%85%A5%EF%BC%8C%E5%BE%97%E5%88%B0%E8%BE%93%E5%87%BA)



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

> 
**对于网络类定义时实现的forward函数，该函数的input必须是variable类型的，所以对于网络的输入也要去是variable类型。**

**所以很多时候，我们可以看见会有一个Variable包起tensor的操作**


**input的size: **

第一项是batch index

根据上述网络定义in_channels=1,所以输入第二项为1，shape(32,32) 所以3，4项为32，32
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







