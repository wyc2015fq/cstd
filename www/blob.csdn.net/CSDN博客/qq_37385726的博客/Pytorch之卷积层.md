# Pytorch之卷积层 - qq_37385726的博客 - CSDN博客





2018年08月16日 13:45:40[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：1700
所属专栏：[Pytorch](https://blog.csdn.net/column/details/26172.html)









# **目录**

[1.简介(torch.nn下的)](#1.%E7%AE%80%E4%BB%8B(torch.nn%E4%B8%8B%E7%9A%84))

[2.简介(torch.nn.functional下的)](#2.%E7%AE%80%E4%BB%8B(torch.nn.functional%E4%B8%8B%E7%9A%84))

# 1.简介(torch.nn下的)

**卷积层主要使用的有3类，用于处理不同维度的数据**

> 
**参数 ****Parameters：**
- in_channels(`int`) – 输入信号的通道   
- out_channels(`int`) – 卷积产生的通道
- kerner_size(`int` or `tuple`) - 卷积核的尺寸
- stride(`int` or `tuple`, `optional`) - 卷积步长
- padding (`int` or `tuple`, `optional`)- 输入的每一条边补充0的层数   
- dilation(`int` or `tuple`, `optional``) – 卷积核元素之间的间距
- groups(`int`, `optional`) – 从输入通道到输出通道的阻塞连接数
- bias(`bool`, `optional`) - 如果`bias=True`，添加偏置

- 
**class torch.nn.Conv1d(in_channels, out_channels, kernel_size, stride=1, padding=0, dilation=1, groups=1, bias=True)**


                             一维卷积层。用于计算ECG等一维数据。

> 
**input**: (N,C_in,L_in)     N为批次，C_in即为in_channels，即一批内输入一维数据个数，L_in是是一维数据基数
**output**: (N,C_out,L_out)   N为批次，C_in即为out_channels，即一批内输出一维数据个数，L_out是一维数据基数



- 
**class torch.nn.Conv2d(in_channels, out_channels, kernel_size, stride=1, padding=0, dilation=1, groups=1, bias=True)**


                            二维卷积层。用于计算CT断层或MR断层，或二维超声图像，自然图像等二维数据。

```python
self.conv1 = nn.Conv2d( # 1*28*28 -> 32*28*28
            in_channels=1,
            out_channels=32,
            kernel_size=5,
            stride=1,
            padding=2  #padding是需要计算的，padding=（stride-1）/2
        )
```

> 
**`input`**: (N,C_in,H_in,W_in)      N为批次，C_in即为in_channels，即一批内输入二维数据个数，H_in是二维数据行数，W_in是二维数据的列数
**`output`**: (N,C_out,H_out,W_out)  N为批次，C_out即为out_channels，即一批内输出二维数据个数，H_out是二维数据行数，W_out是二维数据的列数


```python
con2 = nn.Conv2d(1,16,5,1,2)
# con2(np.empty([1,1,28,28]))  只能接受tensor/variable
con2(torch.Tensor(1,1,28,28))
con2(Variable(torch.Tensor(1,1,28,28)))
```
- 
**class torch.nn.Conv3d(in_channels, out_channels, kernel_size, stride=1, padding=0, dilation=1, groups=1, bias=True)**


                            三维卷积层。用于计算CT或MR等容积数据，视频数据等三维数据。

> 
`input`: (N,C_in,D_in,H_in,W_in) 
`output`: (N,C_out,D_out,H_out,W_out) 




# 2.简介(torch.nn.functional下的)

> 
在torch.nn.functional下也有卷积层，但是和torch.nn下的卷积层的区别在于，functional下的是函数，不是实际的卷积层，而是有卷积层功能的卷积层函数，所以它并不会出现在网络的图结构中。

- **torch.nn.functional.conv1d(input, weight, bias=None, stride=1, padding=0, dilation=1, groups=1)**

**参数：**

- **input** – 输入张量的形状 (minibatch x in_channels x iW)

- **weight** – 过滤器的形状 (out_channels, in_channels, kW)

- **bias** – 可选偏置的形状 (out_channels)

- **stride** – 卷积核的步长，默认为1

```python
>>> filters = autograd.Variable(torch.randn(33, 16, 3))
>>> inputs = autograd.Variable(torch.randn(20, 16, 50))
>>> F.conv1d(inputs, filters)
```
- **torch.nn.functional.conv2d(input, weight, bias=None, stride=1, padding=0, dilation=1, groups=1)**

```python
>>> # With square kernels and equal stride
>>> filters = autograd.Variable(torch.randn(8,4,3,3))
>>> inputs = autograd.Variable(torch.randn(1,4,5,5))
>>> F.conv2d(inputs, filters, padding=1)
```
- **torch.nn.functional.conv3d(input, weight, bias=None, stride=1, padding=0, dilation=1, groups=1)**

```python
>>> filters = autograd.Variable(torch.randn(33, 16, 3, 3, 3))
>>> inputs = autograd.Variable(torch.randn(20, 16, 50, 10, 20))
>>> F.conv3d(inputs, filters)
```





