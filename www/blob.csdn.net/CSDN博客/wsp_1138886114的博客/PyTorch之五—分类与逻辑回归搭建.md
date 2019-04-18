# PyTorch之五—分类与逻辑回归搭建 - wsp_1138886114的博客 - CSDN博客





2019年03月02日 16:13:42[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：39











### 文章目录
- [简单的分类网络](#_1)
- [LogisticRegression网络](#LogisticRegression_67)
- [报错与问题解决](#_148)




### 简单的分类网络

这里我们用最简单的途径来实现神经网络是怎么进行事物的分类的。

```python
import torch
from torch.autograd import Variable
import torch.nn.functional as F
import matplotlib.pyplot as plt


# 创建数据
n_data = torch.ones(100,2)     # dtype=tensor;shape=100X2
x0 = torch.normal(2*n_data,1)  # dtype=tensor;shape=100X1
y0 = torch.zeros(100)          # dtype=tensor;shape=100X1
x1 = torch.normal(-2*n_data,1) # dtype=tensor;shape=100X2
y1 = torch.ones(100)           # dtype=tensor;shape=100X1

#print(n_data,x0)

x = torch.cat((x0, x1), 0).type(torch.FloatTensor)  # FloatTensor = 32-bit floating
y = torch.cat((y0, y1), ).type(torch.LongTensor)    # LongTensor = 64-bit integer

# 使用 Variable 封装tensor 用来训练
x, y = Variable(x), Variable(y)


class Net(torch.nn.Module):
    def __init__(self,n_feature,n_hidden,n_output):
        super(Net,self).__init__()
        self.hidden = torch.nn.Linear(n_feature,n_hidden,n_output)
        self.out = torch.nn.Linear(n_hidden,n_output)

    def forward(self, x):
        x = F.relu(self.hidden(x))
        x = self.out(x)
        return x

net = Net(n_feature=2,n_hidden=10,n_output=2)
print(net)

optimizer = torch.optim.SGD(net.parameters(),lr=0.01)
loss_func = torch.nn.CrossEntropyLoss()

plt.ion()
for step in range(100):
    out = net(x)
    loss = loss_func(out,y)

    optimizer.zero_grad()
    loss.backward()
    optimizer.step()

    if step%10 ==0:
        plt.cla()
        prediction = torch.max(out,1)[1]
        pred_y = prediction.data.numpy().squeeze()
        target_y = y.data.numpy()
        plt.scatter(x.data.numpy()[:, 0], x.data.numpy()[:, 1], c=pred_y, s=100, lw=0, cmap='RdYlGn')
        accuracy = sum(pred_y ==target_y)/200.0
        plt.text(1.5,-4,'Accuracy=%.2f' % accuracy, fontdict={'size': 16, 'color':  'red'})
        plt.pause(0.1)
plt.ioff()
plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190123211857514.gif)

关于快速搭建网络：[https://blog.csdn.net/Gavinmiaoc/article/details/80488148](https://blog.csdn.net/Gavinmiaoc/article/details/80488148)

### LogisticRegression网络

```python
import torch
import torch.nn
from torch.autograd import Variable
import matplotlib.pyplot as plt
import numpy as np

# 假数据
n_data = torch.ones(100, 2)        # 数据的基本形态
x0 = torch.normal(2 * n_data, 1)   # 类型0 x data (tensor), shape=(100, 2)
y0 = torch.zeros(100)              # 类型0 y data (tensor), shape=(100, 1)
x1 = torch.normal(-2 * n_data, 1)  # 类型1 x data (tensor), shape=(100, 1)
y_ = torch.ones(100)               # 类型1 y data (tensor), shape=(100, 1)
y1 = y_*torch.rand(100)

x = torch.cat((x0, x1), 0).type(torch.FloatTensor)  # FloatTensor = 32-bit floating
y = torch.cat((y0, y1), 0).type(torch.FloatTensor)  # LongTensor = 64-bit integer


class LogisticRegression(torch.nn.Module):
    def __init__(self):
        super(LogisticRegression, self).__init__()
        self.lr = torch.nn.Linear(2, 1)
        self.sm = torch.nn.Sigmoid()

    def forward(self, x):
        x = self.lr(x)
        x = self.sm(x)
        return x


logistic_model = LogisticRegression()
if torch.cuda.is_available():
    logistic_model.cuda()

# 定义损失函数和优化器
criterion = torch.nn.BCELoss()
optimizer = torch.optim.SGD(logistic_model.parameters(), lr=1e-3, momentum=0.9)

# 开始训练
plt.ion()
for epoch in range(1000):
    if torch.cuda.is_available():
        x_data = Variable(x).cuda()
        y_data = Variable(y).cuda()
    else:
        x_data = Variable(x)
        y_data = Variable(y)

    y_pred = logistic_model(x_data)
    loss = criterion(y_pred, y_data)

    print_loss = loss.data.item()
    mask = y_pred.ge(0.5).float()          # 以0.5为阈值进行分类
    correct = (mask == y_data).sum()        # 统计正确预测的样本个数
    acc = correct.item() / x_data.size(0)  # 计算精度：正确预测的样本个数占所有数据比

    optimizer.zero_grad()
    loss.backward()
    optimizer.step()

    if (epoch + 1) % 20 == 0:
        plt.cla()
        pred_y = y_pred.data.cpu().numpy().squeeze()
        plt.scatter(x.data.numpy()[:, 0], x.data.numpy()[:, 1], c=pred_y, s=100, lw=0, cmap='RdYlGn')
        plt.plot(x.data.numpy(), y_pred.data.cpu().numpy(), "r-", lw=3)
        print('epoch {} \t loss is {:.4f} \tacc is {:.4f}'.format(epoch + 1,print_loss,acc))  # 训练轮数
        plt.text(1.5, -4, 'acc=%.2f' % acc, fontdict={'size': 16, 'color': 'red'})
        plt.pause(0.05)
plt.ioff()
plt.show()

输出：
epoch 20 	 loss is 1.0888 	acc is 51.0000
epoch 40 	 loss is 0.7617 	acc is 59.0000
epoch 100 	 loss is 0.5108 	acc is 56.0000
epoch 120 	 loss is 0.4965 	acc is 55.0000
... ...
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/201901241129347.gif)

##### 报错与问题解决

上面对输出结果进行可视化时报错：

TypeError: can’t convert CUDA tensor to numpy. Use Tensor.cpu() to copy the tensor to host memory fi

报错原因：numpy不能读取CUDA tensor 需要将它转化为 CPU tensor。由于我的[运行环境](https://blog.csdn.net/wsp_1138886114/article/details/84864908)中安装了CUDA 这里使用了GPU进行运算，我们需要将数据转移到CPU才能对其使用 numpy()。

如下：将`y_pred.data.numpy()`改为`y_pred.data.cpu().numpy()`即可

```python
报错前代码：
pred_y = y_pred.data.numpy().squeeze()
plt.scatter(x.data.numpy()[:, 0], x.data.numpy()[:, 1], c=pred_y, s=100, lw=0, cmap='RdYlGn')
plt.plot(x.data.numpy(), y_pred.data.numpy(), "r-", lw=3)


修改后代码：
pred_y = y_pred.data.cpu().numpy().squeeze()
plt.scatter(x.data.numpy()[:, 0], x.data.numpy()[:, 1], c=pred_y, s=100, lw=0, cmap='RdYlGn')
plt.plot(x.data.numpy(), y_pred.data.cpu().numpy(), "r-", lw=3)
```

鸣谢
[https://blog.csdn.net/Gavinmiaoc/article/details/80488148](https://blog.csdn.net/Gavinmiaoc/article/details/80488148)
[https://blog.csdn.net/out_of_memory_error/article/details/81275651](https://blog.csdn.net/out_of_memory_error/article/details/81275651)



