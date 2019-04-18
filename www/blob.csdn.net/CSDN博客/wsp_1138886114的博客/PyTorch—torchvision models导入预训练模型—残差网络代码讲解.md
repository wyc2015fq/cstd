# PyTorch—torchvision.models导入预训练模型—残差网络代码讲解 - wsp_1138886114的博客 - CSDN博客





2018年11月06日 17:31:13[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：1033











### 文章目录
- [torchvision.models](#torchvisionmodels_5)
- [残差网络代码详解](#_10)
- [1. 模块调用](#1__17)
- [2. 源码解析](#2__31)
- [3. ResNet类](#3_ResNet_85)
- [4. BasicBlock类](#4_BasicBlock_182)
- [5. Bottlenect类](#5_Bottlenect_219)
- [6. 获取预训练模型](#6__258)



PyTorch框架中torchvision模块下有：torchvision.datasets、torchvision.models、torchvision.transforms这3个子包。

关于详情请参考官网：[http://pytorch.org/docs/master/torchvision/index.html。](http://pytorch.org/docs/master/torchvision/index.html%E3%80%82)

具体代码可以参考github：[https://github.com/pytorch/vision/tree/master/torchvision。](https://github.com/pytorch/vision/tree/master/torchvision%E3%80%82)


### torchvision.models

此模块下有常用的 alexnet、densenet、inception、resnet、squeezenet、vgg（[关于网络详情请查看](https://blog.csdn.net/wsp_1138886114/article/details/81386422)）等常用的网络结构，并且提供了预训练模型，我们可以通过简单调用来读取网络结构和预训练模型，同时使用fine tuning（微调）来使用。

关于 fine tuning 可以查看 [https://blog.csdn.net/hjxu2016/article/details/78424370](https://blog.csdn.net/hjxu2016/article/details/78424370)

今天我主要以残残差网路为例来讲解。
### 残差网络代码详解

ResNet主要有五种变形：Res18，Res34，Res50，Res101，Res152。

如下图所示，每个网络都包括三个主要部分：**输入部分**、**输出部分**和**中间卷积部分（中间卷积部分包括如图所示的Stage1到Stage4共计四个stage）**。尽管ResNet的变种形式丰富，但是都遵循上述的结构特点，网络之间的不同主要在于中间卷积部分的block参数和个数存在差异。

具体代码参考github：[https://github.com/pytorch/vision/blob/master/torchvision/models/resnet.py](https://github.com/pytorch/vision/blob/master/torchvision/models/resnet.py)

论文连接：[https://arxiv.org/abs/1512.03385](https://arxiv.org/abs/1512.03385)

##### 1. 模块调用

```python
import torchvision

"""
    如果你需要用预训练模型，设置pretrained=True
    如果你不需要用预训练模型，设置pretrained=False，默认是False，你可以不写
"""
model = torchvision.models.resnet50(pretrained=True) 
model = torchvision.models.resnet50() 

# 你也可以导入densenet模型。且不需要是预训练的模型
model = torchvision.models.densenet169(pretrained=False)
```

##### 2. 源码解析

以导入resnet50为例，介绍具体导入模型时候的源码。

运行 `model = torchvision.models.resnet50(pretrained=True)`的时候，是通过models包下的resnet.py脚本进行的，源码如下：

首先是导入必要的库，其中model_zoo是和导入预训练模型相关的包，另外all变量定义了可以从外部import的函数名或类名。这也是前面为什么可以用torchvision.models.resnet50()来调用的原因。

model_urls这个字典是预训练模型的下载地址。

```python
import torch.nn as nn
import math
import torch.utils.model_zoo as model_zoo

__all__ = ['ResNet', 'resnet18', 'resnet34', 'resnet50', 'resnet101',
           'resnet152']

model_urls = {
    'resnet18': 'https://download.pytorch.org/models/resnet18-5c106cde.pth',
    'resnet34': 'https://download.pytorch.org/models/resnet34-333f7ec4.pth',
    'resnet50': 'https://download.pytorch.org/models/resnet50-19c8e357.pth',
    'resnet101': 'https://download.pytorch.org/models/resnet101-5d3b4d8f.pth',
    'resnet152': 'https://download.pytorch.org/models/resnet152-b121ed2d.pth',
}
```

接下来就是resnet50这个函数了，参数pretrained默认是False。
- `model = ResNet(Bottleneck, [3, 4, 6, 3], **kwargs)`是构建网络结构，Bottleneck是另外一个构建bottleneck的类，在ResNet网络结构的构建中有很多重复的子结构，这些子结构就是通过Bottleneck类来构建的，后面会介绍。
- 如果参数pretrained是True，那么就会通过model_zoo.py中的load_url函数根据model_urls字典下载或导入相应的预训练模型。
- 通过调用model的`load_state_dict`方法用预训练的模型参数来初始化你构建的网络结构，这个方法就是PyTorch中通用的用一个模型的参数初始化另一个模型的层的操作。load_state_dict方法还有一个重要的参数是strict，该参数默认是True，表示预训练模型的层和你的网络结构层严格对应相等（比如层名和维度）。

```python
def resnet50(pretrained=False, **kwargs):
    model = ResNet(Bottleneck, [3, 4, 6, 3], **kwargs)
    if pretrained:
        model.load_state_dict(model_zoo.load_url(model_urls['resnet50']))
    return model
```

其他resnet18、resnet101等函数和resnet50基本类似。

差别主要是在：

1、构建网络结构的时候block的参数不一样，比如resnet18中是[2, 2, 2, 2]，resnet101中是[3, 4, 23, 3]。

2、调用的block类不一样，比如在resnet50、resnet101、resnet152中调用的是Bottleneck类，而在resnet18和resnet34中调用的是BasicBlock类，这两个类的区别主要是在residual结果中卷积层的数量不同，这个是和网络结构相关的，后面会详细介绍。

3、如果下载预训练模型的话，model_urls字典的键不一样，对应不同的预训练模型。因此接下来分别看看如何构建网络结构和如何导入预训练模型。
```python
# pretrained (bool): If True, returns a model pre-trained on ImageNet

def resnet18(pretrained=False, **kwargs):
    model = ResNet(BasicBlock, [2, 2, 2, 2], **kwargs)
    if pretrained:
        model.load_state_dict(model_zoo.load_url(model_urls['resnet18']))
    return model

def resnet101(pretrained=False, **kwargs):
    model = ResNet(Bottleneck, [3, 4, 23, 3], **kwargs)
    if pretrained:
        model.load_state_dict(model_zoo.load_url(model_urls['resnet101']))
    return model
```

##### 3. ResNet类

继承PyTorch中网络的基类：torch.nn.Module :
- 构建ResNet网络是通过ResNet这个类进行的。
- 其次主要的是重写初始化`__init__()`和`forward()`。
`__init __()`中主要是定义一些层的参数。
`forward()`中主要是定义数据在层之间的流动顺序，也就是层的连接顺序。

另外还可以在类中定义其他私有方法用来模块化一些操作，比如这里的`_make_layer()`是用来构建ResNet网络中的4个blocks。
`_make_layer()`:

第一个输入block是Bottleneck或BasicBlock类，

第二个输入是该blocks的输出channel，

第三个输入是每个blocks中包含多少个residual子结构，因此layers这个列表就是前面resnet50的[3, 4, 6, 3]。
`_make_layer()`方法中比较重要的两行代码是：

1、`layers.append(block(self.inplanes, planes, stride, downsample))`，该部分是将每个blocks的第一个residual结构保存在layers列表中。

2、 `for i in range(1, blocks): layers.append(block(self.inplanes, planes))，`该部分是将每个blocks的剩下residual 结构保存在layers列表中，这样就完成了一个blocks的构造。

这两行代码中都是通过Bottleneck这个类来完成每个residual的构建，接下来介绍Bottleneck类。
```python
class ResNet(nn.Module):
    def __init__(self, block, layers, num_classes=1000):
        self.inplanes = 64
        super(ResNet, self).__init__()
        # 网络输入部分
        self.conv1 = nn.Conv2d(3, 64, kernel_size=7, stride=2, padding=3,bias=False)
        self.bn1 = nn.BatchNorm2d(64)
        self.relu = nn.ReLU(inplace=True)
        self.maxpool = nn.MaxPool2d(kernel_size=3, stride=2, padding=1)
        # 中间卷积部分
        self.layer1 = self._make_layer(block, 64, layers[0])
        self.layer2 = self._make_layer(block, 128, layers[1], stride=2)
        self.layer3 = self._make_layer(block, 256, layers[2], stride=2)
        self.layer4 = self._make_layer(block, 512, layers[3], stride=2)
        # 平均池化和全连接层
        self.avgpool = nn.AvgPool2d(7, stride=1)
        self.fc = nn.Linear(512 * block.expansion, num_classes)

        for m in self.modules():
            if isinstance(m, nn.Conv2d):
                n = m.kernel_size[0] * m.kernel_size[1] * m.out_channels
                m.weight.data.normal_(0, math.sqrt(2. / n))
            elif isinstance(m, nn.BatchNorm2d):
                m.weight.data.fill_(1)
                m.bias.data.zero_()

    def _make_layer(self, block, planes, blocks, stride=1):
        downsample = None
        if stride != 1 or self.inplanes != planes * block.expansion:
            downsample = nn.Sequential(
                nn.Conv2d(self.inplanes, planes * block.expansion,
                          kernel_size=1, stride=stride, bias=False),
                nn.BatchNorm2d(planes * block.expansion),
            )

        layers = []
        layers.append(block(self.inplanes, planes, stride, downsample))
        self.inplanes = planes * block.expansion
        for i in range(1, blocks):
            layers.append(block(self.inplanes, planes))

        return nn.Sequential(*layers)

    def forward(self, x):
        x = self.conv1(x)
        x = self.bn1(x)
        x = self.relu(x)
        x = self.maxpool(x)

        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)
        x = self.layer4(x)

        x = self.avgpool(x)
        x = x.view(x.size(0), -1)
        x = self.fc(x)
        return x
```

如上`class ResNet(nn.Module)`代码详解如下：
- 
网络**整体流向**

在ResNet类中的forward( )函数规定了网络数据的流向：

（1）数据进入网络后先经过输入部分（conv1, bn1, relu, maxpool）；

（2）然后进入中间卷积部分（layer1, layer2, layer3, layer4，这里的layer对应我们之前所说的stage）；

（3）最后数据经过一个平均池化和全连接层（avgpool, fc）输出得到结果；

具体来说，resnet50和其他res系列网络的差异主要在于layer1~layer4，其他的部件都是相似的。- 
网络**输入部分**详解：

所有的ResNet网络输入部分是一个size=7x7, stride=2的大卷积核，以及一个size=3x3, stride=2的最大池化组成，通过这一步，一个224x224的输入图像就会变56x56大小的特征图，极大减少了存储所需大小。

- 
网络**中间卷积部分**

中间卷积部分主要是下图中的蓝框部分，通过3*3卷积的堆叠来实现信息的提取。红框中的[2, 2, 2, 2]和[3, 4, 6, 3]等则代表了bolck的重复堆叠次数。

上面我们调用的resnet50( )函数中有一句 ResNet(BasicBlock, [3, 4, 6, 3], **kwargs)，如果你将这行代码改为 ResNet(BasicBlock, [2, 2, 2, 2], **kwargs)， 那你就会得到一个res18网络。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190121100231390.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)- 
**残差块实现（BasicBlock类）**

残差块是怎么实现的？如下图所示的basic-block，输入数据分成两条路，一条路经过两个3*3卷积，另一条路直接短接，二者相加经过relu输出，十分简单。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190121102405276.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)- 
**网络输出部分**

网络输出部分很简单，通过全局自适应平滑池化，把所有的特征图拉成1*1，对于res18来说，就是1x512x7x7 的输入数据拉成 1x512x1x1，然后接全连接层输出，输出节点个数与预测类别个数一致。


##### 4. BasicBlock类

BasicBlock类和Bottleneck类类似，BasicBlock类主要是用来构建ResNet18和ResNet34网络，因为这两个网络的residual结构只包含两个卷积层，没有Bottleneck类中的bottleneck概念。因此在该类中，第一个卷积层采用的是kernel_size=3的卷积，如conv3x3函数所示。

```python
def conv3x3(in_planes, out_planes, stride=1):
    """3x3 convolution with padding"""
    return nn.Conv2d(in_planes, out_planes, kernel_size=3, stride=stride,
                     padding=1, bias=False)

class BasicBlock(nn.Module):
    expansion = 1
    def __init__(self, inplanes, planes, stride=1, downsample=None):
        super(BasicBlock, self).__init__()
        self.conv1 = conv3x3(inplanes, planes, stride)
        self.bn1 = nn.BatchNorm2d(planes)
        self.relu = nn.ReLU(inplace=True)
        self.conv2 = conv3x3(planes, planes)
        self.bn2 = nn.BatchNorm2d(planes)
        self.downsample = downsample   #对输入特征图大小进行减半处理
        self.stride = stride

    def forward(self, x):
        residual = x

        out = self.conv1(x)
        out = self.bn1(out)
        out = self.relu(out)

        out = self.conv2(out)
        out = self.bn2(out)

        if self.downsample is not None:
            residual = self.downsample(x)

        out += residual
        out = self.relu(out)
        return out
```

##### 5. Bottlenect类

从前面的ResNet类可以看出，在构造ResNet网络的时候，最重要的是Bottleneck这个类，因为ResNet是由residual结构组成的，而Bottleneck类就是完成residual结构的构建。同样Bottlenect还是继承了torch.nn.Module类，且重写了__init__和forward方法。从forward方法可以看出，bottleneck 就是我们熟悉的3个主要的卷积层、BN层和激活层，最后的out += residual就是element-wise add的操作。

```python
class Bottleneck(nn.Module):
    expansion = 4

    def __init__(self, inplanes, planes, stride=1, downsample=None):
        super(Bottleneck, self).__init__()
        self.conv1 = nn.Conv2d(inplanes, planes, kernel_size=1, bias=False)
        self.bn1 = nn.BatchNorm2d(planes)
        self.conv2 = nn.Conv2d(planes, planes, kernel_size=3, stride=stride,
                               padding=1, bias=False)
        self.bn2 = nn.BatchNorm2d(planes)
        self.conv3 = nn.Conv2d(planes, planes * 4, kernel_size=1, bias=False)
        self.bn3 = nn.BatchNorm2d(planes * 4)
        self.relu = nn.ReLU(inplace=True)
        self.downsample = downsample
        self.stride = stride

    def forward(self, x):
        residual = x
        out = self.conv1(x)
        out = self.bn1(out)
        out = self.relu(out)

        out = self.conv2(out)
        out = self.bn2(out)
        out = self.relu(out)

        out = self.conv3(out)
        out = self.bn3(out)

        if self.downsample is not None:
            residual = self.downsample(x)

        out += residual
        out = self.relu(out)
        return out
```

##### 6. 获取预训练模型

前面提到这一行代码：
`if pretrained: model.load_state_dict(model_zoo.load_url(model_urls['resnet50']))`，主要就是通过model_zoo.py中的load_url函数根据model_urls字典导入相应的预训练模型，models_zoo.py脚本的github地址：[https://github.com/pytorch/pytorch/blob/master/torch/utils/model_zoo.py。](https://github.com/pytorch/pytorch/blob/master/torch/utils/model_zoo.py%E3%80%82)

load_url函数源码如下。- 首先model_dir是下载模型保存地址，如果没有指定则保存在项目的.torch目录下，最好指定。cached_file是保存模型的路径加上模型名称。
- 接下来的 if not os.path.exists(cached_file)语句用来判断是否指定目录下已经存在要下载模型，如果已经存在，就直接调用torch.load接口导入模型，如果不存在，则从网上下载。
- 下载是通过`_download_url_to_file(url, cached_file, hash_prefix, progress=progress)`进行的，不再细讲。重点在于模型导入是通过torch.load()接口来进行的，不管你的模型是从网上下载的还是本地已有的。

```python
def load_url(url, model_dir=None, map_location=None, progress=True):
    """
    Args:
        url (string): URL of the object to download
        model_dir (string, optional): directory in which to save the object
        map_location (optional): a function or a dict specifying how to remap storage locations (see torch.load)
        progress (bool, optional): whether or not to display a progress bar to stderr

    Example:
        >>> state_dict = torch.utils.model_zoo.load_url('https://s3.amazonaws.com/pytorch/models/resnet18-5c106cde.pth')

    """
    if model_dir is None:
        torch_home = os.path.expanduser(os.getenv('TORCH_HOME', '~/.torch'))
        model_dir = os.getenv('TORCH_MODEL_ZOO', os.path.join(torch_home, 'models'))
    if not os.path.exists(model_dir):
        os.makedirs(model_dir)
    parts = urlparse(url)
    filename = os.path.basename(parts.path)
    cached_file = os.path.join(model_dir, filename)
    if not os.path.exists(cached_file):
        sys.stderr.write('Downloading: "{}" to {}\n'.format(url, cached_file))
        hash_prefix = HASH_REGEX.search(filename).group(1)
        _download_url_to_file(url, cached_file, hash_prefix, progress=progress)
    return torch.load(cached_file, map_location=map_location)
```

鸣谢
[https://blog.csdn.net/u014380165/article/details/79119664](https://blog.csdn.net/u014380165/article/details/79119664)
[https://zhuanlan.zhihu.com/p/54289848](https://zhuanlan.zhihu.com/p/54289848)













