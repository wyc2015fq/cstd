# Pytorch之第一次构建神经网络（一） - qq_37385726的博客 - CSDN博客





2018年08月16日 14:06:01[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：172
所属专栏：[Pytorch](https://blog.csdn.net/column/details/26172.html)









## 本文为第一次构建神经网络系列第一篇

> 
**探讨如何构建神经网络中关键的class Net**


系列第一篇：[https://blog.csdn.net/qq_37385726/article/details/81740386](https://blog.csdn.net/qq_37385726/article/details/81740386)

系列第二篇：[https://blog.csdn.net/qq_37385726/article/details/81742247](https://blog.csdn.net/qq_37385726/article/details/81742247)

系列第三篇：[https://blog.csdn.net/qq_37385726/article/details/81744802](https://blog.csdn.net/qq_37385726/article/details/81744802)

系列第四篇：[https://blog.csdn.net/qq_37385726/article/details/81745510](https://blog.csdn.net/qq_37385726/article/details/81745510)

系列第五篇：[https://blog.csdn.net/qq_37385726/article/details/81748635](https://blog.csdn.net/qq_37385726/article/details/81748635)

# **目录**

[代码](#%E4%BB%A3%E7%A0%81)

[输出](#%E8%BE%93%E5%87%BA)

> 
本文根据[https://blog.csdn.net/qq_37385726/article/details/81740233](https://blog.csdn.net/qq_37385726/article/details/81740233)所述的前两种方法构建Net类，并打印网络结构

**其中def是用于定义网络中的层结构，forward用于前向传递构建网络图**
- **卷积 Relu（rectify linear unit 用作非线性处理） Pooling（池化/亚采样） **
- **#展平（关键操作，利用view函数实现，用于构建nn.Linear层的输入）# **

       关于view函数介绍请参见[https://blog.csdn.net/qq_37385726/article/details/81738518](https://blog.csdn.net/qq_37385726/article/details/81738518)
- **全连接层**
- **输出**




# 代码

```
class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()

        #利用nn中的Con2d卷积层函数创建卷积层
        self.conv1 = nn.Conv2d( # 1*28*28 -> 32*28*28
            in_channels=1,
            out_channels=32,
            kernel_size=5,
            stride=1,
            padding=2
        )
        #经过relu()
        #jing经过pool()  32*28*28 -> 32*14*14
     #利用nn中的Sequential函数快速创建神经网络,每一个都没有姓名
        self.all = nn.Sequential(
            nn.Conv2d(32,64,5,1,2),    #32*14*14 -> 64*14*14
            nn.ReLU(),  #64*14*14 -> 64*14*14
            nn.MaxPool2d(kernel_size=2,stride=2) #64*14*14 -> 64*14*14
        )

        self.connect1 = nn.Linear(64*14*14,128)
        self.connect2 = nn.Linear(128,10)

    def forward(self,x):
        x = self.conv1(x)
        x = F.relu(x)
        x = F.max_pool2d(x)
        x = self.all(x)
#对于Linear层输入为线性，所以要将原来的64*14*14的input利用view函数展平成1维的64*14*14 tensor
        x = x.view(-1, self.num_flat_features(x))  
        x = self.connect1(x)
        x = F.relu(x)
        x = self.connect2(x)
        return x

     #计算x的有的数据个数，用于展平二维tensor
     def num_flat_features(self, x):
        size = x.size()[1:] # all dimensions except the batch dimension
        num_features = 1
        for s in size:
            num_features *= s
        return num_features

net=Net()
print(net)  #打印网络结构
```



# 输出

Net(

  (conv1): Conv2d(1, 32, kernel_size=(5, 5), stride=(1, 1), padding=(2, 2))

  (all): Sequential(

    (0): Conv2d(32, 64, kernel_size=(5, 5), stride=(1, 1), padding=(2, 2))

    (1): ReLU()

    (2): MaxPool2d(kernel_size=2, stride=2, padding=0, dilation=1, ceil_mode=False)

  )

  (connect1): Linear(in_features=12544, out_features=128, bias=True)

  (connect2): Linear(in_features=128, out_features=10, bias=True)

)







