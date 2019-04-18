# PyTorch之—卷积层、激活层、BN - wsp_1138886114的博客 - CSDN博客





2019年01月28日 09:29:25[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：313











### 文章目录
- [一、卷积层](#_1)
- [二、池化层](#_30)
- [三、归一化（BN）](#BN_57)




### 一、卷积层
- `class torch.nn.Conv2d(in_channels, out_channels, kernel_size, stride=1, padding=0, dilation=1, groups=1, bias=True)`
二维卷积层，输入的尺度是(N, C_in,H,W)，输出尺度（ N,C_out,H_out,W_out）的计算方式：
$$\rm out(N_i, C_{out_j})=bias(C_{out_j})+\sum^{C_{in}-1}_{k=0}weight(C{out_j},k)\bigotimes input(N_i,k)$$
- 参数：
`in_channels(int)` – 输入信号的通道
`out_channels(int)` – 卷积产生的通道
`kerner_size(int or tuple)` - 卷积核的尺寸
`stride(int or tuple, optional)` - 卷积步长
`padding (int or tuple, optional)`- 输入的每一条边补充0的层数
`dilation(int or tuple, optional)` – 卷积核元素之间的间距
`groups(int, optional)` – 从输入通道到输出通道的阻塞连接数
`bias(bool, optional)` - 如果bias=True，添加偏置- shape:
input: $(N,C_{in},H_{in},W_{in})$

output: $(N,C_{out},H_{out},W_{out})$
$$\rm H_{out}=floor((H_{in}+2padding[0]-dilation[0](kernerl\_size[0]-1)-1)/stride[0]+1)$$

$$\rm W_{out}=floor((W_{in}+2padding[1]-dilation[1](kernerl\_size[1]-1)-1)/stride[1]+1)$$
- 变量:
weight(tensor) - 卷积的权重，大小是(out_channels, in_channels,kernel_size)

bias(tensor) - 卷积的偏置系数，大小是（out_channel）
### 二、池化层
- `class torch.nn.MaxPool2d(kernel_size, stride=None, padding=0, dilation=1, return_indices=False, ceil_mode=False)`

对于输入信号的输入通道，提供2维最大池化（max pooling）操作

如果输入的大小是(N,C,H,W)，那么输出的大小是(N,C,H_out,W_out)和池化窗口大小(kH,kW)的关系是：
$$\rm out(N_i, C_j,k)=max^{kH-1}_{m=0}max^{kW-1}_{m=0}input(N_{i},C_j,stride[0]h+m,stride[1]w+n)$$

如果padding不是0，会在输入的每一边添加**相应数目**的0

dilation用于控制内核点之间的距离。
- 参数：

`kernel_size(int or tuple)` - max pooling的窗口大小
`stride(int or tuple, optional)` - max pooling的窗口移动的步长。默认值是kernel_size
`padding(int or tuple, optional)`- 输入的每一条边补充0的层数
`dilation(int or tuple, optional)` – 一个控制窗口中元素步幅的参数
`return_indices` - 如果等于True，会返回输出最大值的序号，对于上采样操作会有帮助
`ceil_mode` - 如果等于True，计算输出信号大小的时候，会使用向上取整，代替默认的向下取整的操作- shape:

输入: $(N,C,H_{in},W_{in})$

输出: $(N,C,H_{out},W_{out})$

$$\rm H_{out}=floor((H_{in} + 2padding[0] - dilation[0](kernel\_size[0] - 1) - 1)/stride[0] + 1$$

$$\rm W_{out}=floor((W_{in} + 2padding[1] - dilation[1](kernel\_size[1] - 1) - 1)/stride[1] + 1$$

### 三、归一化（BN）
- `class torch.nn.BatchNorm2d(num_features, eps=1e-05, momentum=0.1, affine=True)`

对小批量(mini-batch)3d数据组成的4d输入进行批标准化(Batch Normalization)操作
- $$y=\gamma *\frac{x−mean[x]}{\sqrt{Var[x]}+ϵ}+\beta$$

在每一个小批量（mini-batch）数据中，计算输入各个维度的均值和标准差。gamma与beta是可学习的大小为C的参数向量（C为输入大小）

在训练时，该层计算每次输入的均值与方差，并进行移动平均。移动平均默认的动量值为0.1。

在验证时，训练求得的均值/方差将用于标准化验证数据。
- 参数：

`num_features`： 来自期望输入的特征数，该期望输入的大小为 **batch_size x num_features x height x width**
`eps`： 为保证数值稳定性（分母不能趋近或取0）,给分母加上的值。默认为1e-5。
`momentum`： 动态均值和动态方差所使用的动量。默认为0.1。
`affine`： 一个布尔值，当设为true，给该层添加可学习的仿射变换参数。
`Shape`：- 输入输出相同（N, C，H, W)
例子

```python
m = nn.BatchNorm2d(100)                  # With Learnable Parameters
m = nn.BatchNorm2d(100, affine=False)    # Without Learnable Parameters

input = autograd.Variable(torch.randn(20, 100, 35, 45))
output = m(input)
```





