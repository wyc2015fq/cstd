# PyTorch源码解读之torchvision.models - AI之路 - CSDN博客





2018年01月21日 13:28:35[AI之路](https://me.csdn.net/u014380165)阅读数：28069
个人分类：[深度学习																[PyTorch](https://blog.csdn.net/u014380165/article/category/7286599)](https://blog.csdn.net/u014380165/article/category/6829229)

所属专栏：[PyTorch使用及源码解读](https://blog.csdn.net/column/details/19413.html)








**PyTorch框架中有一个非常重要且好用的包：torchvision，该包主要由3个子包组成，分别是：torchvision.datasets、torchvision.models、torchvision.transforms**。这3个子包的具体介绍可以参考官网：[http://pytorch.org/docs/master/torchvision/index.html](http://pytorch.org/docs/master/torchvision/index.html)。具体代码可以参考github：[https://github.com/pytorch/vision/tree/master/torchvision](https://github.com/pytorch/vision/tree/master/torchvision)。

**这篇博客介绍torchvision.models**。torchvision.models这个包中包含alexnet、densenet、inception、resnet、squeezenet、vgg等常用的网络结构，并且提供了预训练模型，可以通过简单调用来读取网络结构和预训练模型。

**使用例子：**

```python
import torchvision
model = torchvision.models.resnet50(pretrained=True)
```

这样就导入了resnet50的预训练模型了。如果只需要网络结构，不需要用预训练模型的参数来初始化，那么就是：
`model = torchvision.models.resnet50(pretrained=False)`
如果要导入densenet模型也是同样的道理，比如导入densenet169，且不需要是预训练的模型：
`model = torchvision.models.densenet169(pretrained=False)`
由于pretrained参数默认是False，所以等价于：
`model = torchvision.models.densenet169()`
不过为了代码清晰，最好还是加上参数赋值。

**接下来以导入resnet50为例介绍具体导入模型时候的源码**。运行`model = torchvision.models.resnet50(pretrained=True)`的时候，是通过models包下的resnet.py脚本进行的，源码如下：

首先是导入必要的库，其中model_zoo是和导入预训练模型相关的包，另外**all**变量定义了可以从外部import的函数名或类名。这也是前面为什么可以用torchvision.models.resnet50()来调用的原因。model_urls这个字典是预训练模型的下载地址。

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

接下来就是resnet50这个函数了，参数pretrained默认是False。首先`model = ResNet(Bottleneck, [3, 4, 6, 3], **kwargs)`是构建网络结构，Bottleneck是另外一个构建bottleneck的类，在ResNet网络结构的构建中有很多重复的子结构，这些子结构就是通过Bottleneck类来构建的，后面会介绍。然后如果参数pretrained是True，那么就会通过model_zoo.py中的load_url函数根据model_urls字典下载或导入相应的预训练模型。最后通过调用model的load_state_dict方法用预训练的模型参数来初始化你构建的网络结构，这个方法就是PyTorch中通用的用一个模型的参数初始化另一个模型的层的操作。load_state_dict方法还有一个重要的参数是strict，该参数默认是True，表示预训练模型的层和你的网络结构层严格对应相等（比如层名和维度）。

```python
def resnet50(pretrained=False, **kwargs):
    """Constructs a ResNet-50 model.

    Args:
        pretrained (bool): If True, returns a model pre-trained on ImageNet
    """
    model = ResNet(Bottleneck, [3, 4, 6, 3], **kwargs)
    if pretrained:
        model.load_state_dict(model_zoo.load_url(model_urls['resnet50']))
    return model
```

其他resnet18、resnet101等函数和resnet50基本类似，差别主要是在：1、构建网络结构的时候block的参数不一样，比如resnet18中是[2, 2, 2, 2]，resnet101中是[3, 4, 23, 3]。2、调用的block类不一样，比如在resnet50、resnet101、resnet152中调用的是Bottleneck类，而在resnet18和resnet34中调用的是BasicBlock类，这两个类的区别主要是在residual结果中卷积层的数量不同，这个是和网络结构相关的，后面会详细介绍。3、如果下载预训练模型的话，model_urls字典的键不一样，对应不同的预训练模型。因此接下来分别看看如何构建网络结构和如何导入预训练模型。

```python
def resnet18(pretrained=False, **kwargs):
    """Constructs a ResNet-18 model.

    Args:
        pretrained (bool): If True, returns a model pre-trained on ImageNet
    """
    model = ResNet(BasicBlock, [2, 2, 2, 2], **kwargs)
    if pretrained:
        model.load_state_dict(model_zoo.load_url(model_urls['resnet18']))
    return model

def resnet101(pretrained=False, **kwargs):
    """Constructs a ResNet-101 model.

    Args:
        pretrained (bool): If True, returns a model pre-trained on ImageNet
    """
    model = ResNet(Bottleneck, [3, 4, 23, 3], **kwargs)
    if pretrained:
        model.load_state_dict(model_zoo.load_url(model_urls['resnet101']))
    return model
```

构建ResNet网络是通过ResNet这个类进行的。首先还是继承PyTorch中网络的基类：torch.nn.Module，其次主要的是重写初始化`__init__`和forward方法。在初始化`__init__`中主要是定义一些层的参数。forward方法中主要是定义数据在层之间的流动顺序，也就是层的连接顺序。另外还可以在类中定义其他私有方法用来模块化一些操作，比如这里的_make_layer方法是用来构建ResNet网络中的4个blocks。_make_layer方法的第一个输入block是Bottleneck或BasicBlock类，第二个输入是该blocks的输出channel，第三个输入是每个blocks中包含多少个residual子结构，因此layers这个列表就是前面resnet50的[3, 4, 6, 3]。 

_make_layer方法中比较重要的两行代码是：1、layers.append(block(self.inplanes, planes, stride, downsample))，该部分是将每个blocks的第一个residual结构保存在layers列表中。2、 for i in range(1, blocks): layers.append(block(self.inplanes, planes))，该部分是将每个blocks的剩下residual 结构保存在layers列表中，这样就完成了一个blocks的构造。这两行代码中都是通过Bottleneck这个类来完成每个residual的构建，接下来介绍Bottleneck类。

```
class ResNet(nn.Module):

    def __init__(self, block, layers, num_classes=1000):
        self.inplanes = 64
        super(ResNet, self).__init__()
        self.conv1 = nn.Conv2d(3, 64, kernel_size=7, stride=2, padding=3,
                               bias=False)
        self.bn1 = nn.BatchNorm2d(64)
        self.relu = nn.ReLU(inplace=True)
        self.maxpool = nn.MaxPool2d(kernel_size=3, stride=2, padding=1)
        self.layer1 = self._make_layer(block, 64, layers[0])
        self.layer2 = self._make_layer(block, 128, layers[1], stride=2)
        self.layer3 = self._make_layer(block, 256, layers[2], stride=2)
        self.layer4 = self._make_layer(block, 512, layers[3], stride=2)
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

从前面的ResNet类可以看出，在构造ResNet网络的时候，最重要的是Bottleneck这个类，因为ResNet是由residual结构组成的，而Bottleneck类就是完成residual结构的构建。同样Bottlenect还是继承了torch.nn.Module类，且重写了`__init__`和forward方法。从forward方法可以看出，bottleneck就是我们熟悉的3个主要的卷积层、BN层和激活层，最后的out += residual就是element-wise add的操作。

```
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

BasicBlock类和Bottleneck类类似，前者主要是用来构建ResNet18和ResNet34网络，因为这两个网络的residual结构只包含两个卷积层，没有Bottleneck类中的bottleneck概念。因此在该类中，第一个卷积层采用的是kernel_size=3的卷积，如conv3x3函数所示。

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
        self.downsample = downsample
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

介绍完如何构建网络，接下来就是如何获取预训练模型。前面提到这一行代码：`if pretrained:     model.load_state_dict(model_zoo.load_url(model_urls['resnet50']))`，主要就是通过model_zoo.py中的load_url函数根据model_urls字典导入相应的预训练模型，models_zoo.py脚本的github地址：[https://github.com/pytorch/pytorch/blob/master/torch/utils/model_zoo.py](https://github.com/pytorch/pytorch/blob/master/torch/utils/model_zoo.py)。 

load_url函数源码如下。首先model_dir是下载下来的模型的保存地址，如果没有指定的话就会保存在项目的.torch目录下，最好指定。cached_file是保存模型的路径加上模型名称。接下来的 if not os.path.exists(cached_file)语句用来判断是否指定目录下已经存在要下载模型，如果已经存在，就直接调用torch.load接口导入模型，如果不存在，则从网上下载，下载是通过_download_url_to_file(url, cached_file, hash_prefix, progress=progress)进行的，不再细讲。重点在于模型导入是通过torch.load()接口来进行的，不管你的模型是从网上下载的还是本地已有的。

```python
def load_url(url, model_dir=None, map_location=None, progress=True):
    r"""Loads the Torch serialized object at the given URL.

    If the object is already present in `model_dir`, it's deserialized and
    returned. The filename part of the URL should follow the naming convention
    ``filename-<sha256>.ext`` where ``<sha256>`` is the first eight or more
    digits of the SHA256 hash of the contents of the file. The hash is used to
    ensure unique names and to verify the contents of the file.

    The default value of `model_dir` is ``$TORCH_HOME/models`` where
    ``$TORCH_HOME`` defaults to ``~/.torch``. The default directory can be
    overriden with the ``$TORCH_MODEL_ZOO`` environment variable.

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




