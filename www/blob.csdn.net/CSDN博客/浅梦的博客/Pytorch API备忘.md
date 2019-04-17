# Pytorch API备忘 - 浅梦的博客 - CSDN博客





2018年09月11日 14:49:05[浅梦s](https://me.csdn.net/u012151283)阅读数：376标签：[Pytorch](https://so.csdn.net/so/search/s.do?q=Pytorch&t=blog)
个人分类：[深度学习](https://blog.csdn.net/u012151283/article/category/6698461)









之前一直用tensorflow和keras，最近在看一些CV领域的paper，发现相关的开源代码很多是pytorch实现的，于是打算学下pytorch。 

以下主要根据《深度学习入门之Pytorch》这本书对一些常用API进行记录。

# Pytorch基础

## Tensor张量
|名称|类型|
|----|----|
|torch.FloatTensor(**默认类型**)|32位浮点|
|torch.DoubleTensor|64位浮点|
|torch.ShortTensor|16位整型|
|torch.IntTensor|32位整型|
|torch.LongTensor|64位整型|
`a = torch.Tensor([[1,2],[3,4]])`
创建全是0的空Tensor,01均匀分布或以正态分布作为初始值的Tensor

```
c = torch.zeros((3,2))
t = torch.rand((3,2))#0-1均匀分布
d = torch.randn((3,2))#正态分布
r = torch.range(1,10)#生成区间浮点数
```

像numpy一样通过索引的方式取得其中的元素，或改变其中的值
`a[0,1]=100`
Tensor与numpy.ndarray之间相互转换

```
numpy_b = b.numpy()
e = np.array([[2,3],[4,5]])
torch_e = torch.from_numpy(e)
f_torch_e = torch_e.float()
```

使用GPU进行加速

```
if torch.cuda.is_available():
    a_cuda = a.cuda()
```

## Variable变量

Variable在`torch.autograd.Variable`中，通过调用`Variable(a)`可以将一个tensor a变成Variable。 

构建时需要传递一个参数`requires_grad=True`，这个参数表示是否对这个变量求梯度，默认的是 False。 

Variable中包括三个重要属性,`data`,`grad`,`grad_fn`。|属性|含义|
|----|----|
|data|Variable里面的tensor的值|
|grad|Variable反向传播梯度|
|grad_fn|得到这个Variable的操作|

## Tensor运算
|操作|功能|
|----|----|
|torch.abs|绝对值|
|torch.add||
|torch.clamp|裁剪|
|torch.div||
|torch.mul|逐元素求积|
|torch.pow||
|torch.mm|矩阵乘法|
|torch.mv|矩阵向量乘法|

## Dataset数据集

通过`torch.utils.data.DataLoader`定义迭代器,`collate_fn`是表示如何取样本的，可以定义自己的函数来准确地实现想要的功能，默认的函数在一般情况下都是可以使用的。
`dataiter = DataLoader(myDataset,batch_size=32,shuffle=True,collate_fn=default_collate)`
在`torchvision`中还有一个更高级的关于CV的数据读取类`ImageFolder`，主要功能是处理图片，且要求图片形式如下：

```
root/dog/xxx.png
root/dog/xxy.png
root/dog/xxz/png
...
root/cat/123.png
```
`dset = ImageFolder(root='root_path',transform=None,loader=default_loader)`
其中root是根目录，目录下每个文件夹表示一个类别，`transform`和`target_transform`是图片增强,`loader`是图片读取的办法。

## nn.Module模组

pytorch中所有的层结构和损失函数都来自于`torch.nn`，所有的模型构建都从这个基类`nn.Module`继承来的，模板如下

```python
class net_name(nn.Module):
    def __init__(self,other_arguments):
        super(net_name,self).__init__()
        self.conv1 = nn.Conv2d(in_channels,out_channels,kernel_size)
        # other network layer
    def forward(self,x):
        x = self.conv1(x)
        return x
criterion = nn.CrossEntropyLoss()
loss = criterion(output,target)
```

## torch.optim 优化
`optimizer = torch.optim.SGD(model.parameters(),lr=0.01,momentum=0.9)`
在优化之前需先将梯度归零，即`optimizer.zeros()`，然后通过`loss.backward()`反向传播，自动求导得到每个参数的梯度，最后调用`optimizer.step()`就可以通过梯度做一步参数更新。

## 模型的保存和加载

使用`torch.save`来保存模型的结构和参数，有两种保存方式
- 保存整个模型的结构信息和参数信息，保存的对象是模型model
- 
保存模型的参数，保存的对象是模型的状态`model.state_dict()`

```bash
torch.save(model,'./model.pth')
torch.save(model.state_dict(),'./model_state.pth')
```


加载模型有两种方式对应于保存模型的方式:
- 
加载完整的模型结构和参数使用
`load_model = torch.load('model.pth')`
，在网络较大的时候加载的时间比较长，同时还存储空间也比较大

- 加载模型参数信息，需要先导入模型的结构，然后通过
`model.load_state_dic(torch.load('model_staet.pth'))`
导入

# Pytorch卷积模块
- 
卷积层

`nn.Conv2d()`是卷积模块，里面常用的参数有5个
|参数名|说明|
|----|----|
|in_channels|输入数据的深度|
|out_channels|输出数据的深度|
|kernel_size|卷积核大小|
|stride|滑动的步长|
|padding|0不填充，1进行1个像素点的0填充|
|bias|默认bias=True|
|groups|输出数据深度和输入数据深度上的联系，默认groups=1，表示所有输出和输入是关联的。groups=2,表示输入的深度被分割成两份，输出的深度也被分割成两份，它们之间分别对应起来，要求输出和输入都必须能被groups整除|
|dilation|表示卷积堆输入数据体的空间间隔，默认=1（**没太明白**）|

- 
池化层 
`nn.MaxPool2d()`表示最大值池化，其中的参数有`kernel_size`,`stride`,`padding`,`dilation`,`return_indices`,`ceil_mode`
|参数名|含义|
|----|----|
|return_indices|是否返回最大值所处的下标，默认return_indices=False|
|ceil_mode|表示使用一些方格代替层结构，默认为False|


`nn.AvgPool2d()`均值池化，多一个参数`count_include_pad`表示计算均值时候是否包含零填充，默认为True 

其他池化层如`nn.LPPool2d`和`nn.AdaptiveMaxPool2d()`等。

## 提取层结构

`nn.Module`有几个重要属性。 
`children()`，这个会返回下一级模块的迭代器，不会返回它们内部的东西。 
`modules()`会返回模型中所有模块的迭代器，能够访问到最内层。 
`named_children()`和`named_modules()`不仅返回模块的迭代器，还会返回网络层的名字。
## 提取参数及自定义初始化

`nn.Module`里面有两个特别重要的关于参数的属性，分别是`named_parameters()`和`parameters()`。`named_parameters()`给出网络层的名字和参数的迭代器，`parameters()`会给出一个网络的全部参数的迭代器。

```
for parm in model.named_parameters():
    print(param[0])
```

权重是一个`Variable`，去除其中的`data`属性，对它进行所需要的处理就可以

```
for m in model.modules():
    if isinstance(m,nn.Conv2d):
        init.normal(m.weight.data)
        init.xavier_noraml(m.weight.data)
        init.kaiming_normal(m.weight.data)
        m.bias.data.fill_(0)
    elif isinstance(m,nn.Linear):
        m.weight.data.normal_()
```

# Pytorch循环神经网络模块

## 标准RNN

`nn.RNN()`
|参数名|说明|
|----|----|
|input_size|输入$x_t$的特征维度|
|hidden_size|输出$h_t$的特征维度|
|num_layers|网络层数，默认1层|
|nonlinearity|非线性激活函数,默认tanh,可选relu|
|bias|默认bias=True|
|batch_first|默认(seq,batch,feature)，可选(batch,seq,feature)|
|dropout|在除了最后一层之外的其他输出层加上dropout|
|bidirectional|默认False|

网络接收一个序列输入$x_t$和记忆输入$h_0$，$x_t$的维度是$(seq,batch,feature)$，$h_0$的维度是$(layers*direction,batch,hidden)$

网络输出$output$和$h_n$，$output$维度是$(seq,batch,hidden*direction)$,$h_n$表示记忆单元，维度是$(layer*direction,batch,hidden)$

## Embedding词嵌入

通过`nn.Embedding(m,n)`实现，m表示所有的单词数目，n表示词嵌入的维度

```
word_to_idx = {'hello':0,'world':1}
embeds = nn.Embedding(2,5)
hello_idx = torch.LongTensor([word_to_ix['hello']])
hello_idx = Variable(hello_idx)
hello_embed = embeds(hello_idx)
print(hello_embed)
```





