# Pseudo-3D Residual Networks算法的pytorch代码 - AI之路 - CSDN博客





2018年01月06日 08:08:03[AI之路](https://me.csdn.net/u014380165)阅读数：4098








**本篇博客是对第三方实现的Pseudo-3D Residual Networks算法的pytorch代码进行介绍**，介绍顺序为代码调试顺序，建议先阅读论文或相关博客。 

论文：Learning Spatio-Temporal Representation with Pseudo-3D Residual Networks。 

代码地址：[https://github.com/qijiezhao/pseudo-3d-pytorch](https://github.com/qijiezhao/pseudo-3d-pytorch)
导入必须的模块

```python
from __future__ import print_function
import torch
import torch.nn as nn
import numpy as np
import torch.nn.functional as F
from torch.autograd import Variable
import math
from functools import partial

__all__ = ['P3D', 'P3D63', 'P3D131','P3D199']
```

**main函数中主要包括导入模型，准备数据，测试数据，打印结果这几个部分，其中导入模型和准备数据最重要**。导入模型部分通过调用P3D199得到199层的P3D网络，另外参数pretrained=true表明导入的是预训练的模型，这样模型的参数就可以通过预训练模型的参数来初始化了，另一个参数num_classes=400是类别数。P3D199后面会详细介绍。 
`data=torch.autograd.Variable(torch.rand(10,3,16,160,160)).cuda()`这一行是随机生成输入数据，第一个维度是10说明该输入数据包含10个clip，其中每个clip包含16帧图像，每帧图像是160*160的3通道图像。最后数据输入模型得到结果out。

```
if __name__ == '__main__':

    model = P3D199(pretrained=True,num_classes=400)
    model = model.cuda()

# if modality=='Flow', please change the 2nd dimension 3==>2
   data=torch.autograd.Variable(torch.rand(10,3,16,160,160)).cuda()
    out=model(data)
    print (out.size(),out)
```

**调用P3D199函数得到199层的P3D网络**。如果调用P3D63或P3D131则得到对应层数的P3D网络，不过由于P3D63和P3D131没有预训练模型，所以在实现中只有调用P3D类导入网络结构这一步。`model = P3D(Bottleneck, [3, 8, 36, 3], modality=modality,**kwargs)` 这一行通过调用P3D类获得网络结构，该函数的第二个参数[3, 8, 36, 3]表明其结构和ResNet-152对应，于是最后得到的就是199层的P3D网络。因为pretrained设置为True，所以pretrained_file就是准备好的预训练模型的压缩文件，通过`weights=torch.load(pretrained_file)['state_dict']`这一行读取预训练模型的参数，然后`model.load_state_dict(weights)`这一行将读取到的预训练模型的参数赋值给model这个网络结构，完成赋值。接下来介绍P3D这个类。

```python
def P3D199(pretrained=False,modality='RGB',**kwargs):
    """construct a P3D199 model based on a ResNet-152-3D model.
    """
    model = P3D(Bottleneck, [3, 8, 36, 3], modality=modality,**kwargs)
    if pretrained==True:
        if modality=='RGB':
            pretrained_file='p3d_rgb_199.checkpoint.pth.tar'
        elif modality=='Flow':
            pretrained_file='p3d_flow_199.checkpoint.pth.tar'
        weights=torch.load(pretrained_file)['state_dict']
        model.load_state_dict(weights)
    return model
```

**P3D这个类是构造网络结构的主体**，之前介绍过在PyTorch中定义网络结构的时候都要继承基类torch.nn.Module，这里也是这样。先看看`__init__`，这是因为前面调用P3D类生成对象的时候会先调用`__init__`进行初始化。 `self.input_channel = 3 if modality=='RGB' else 2`这一行表示如果输入是视频帧（也就是图像），那么输入channel就是3，如果输入是optical flow，那么输入channel就是2。`self.conv1_custom`是网络的第一个卷积层，原来在ResNet中是2D的7*7大小的卷积核。self.maxpool是对最后3维都做了pooling，也就是对这3维都做了减半。self.maxpool_2是对倒数第3维度做了pooling，也就是对该维度做了减半。self.layer1到self.layer4是P3D网络的4个block，这个和ResNet网络中的conv2_x到conv5_x对应，以`self.layer1 = self._make_layer(block, 64, layers[0], shortcut_type)`为例，通过调用P3D类的*make_layer方法来得到该block的层（后面会详细介绍_make_layer方法），一般函数名前面加上*表示类内部函数，或者叫私有函数。几个输入的含义如下：block是Bottleneck，layers是一个列表（长度就是block的数量，这里就是4），其中的每个值表示对应的那个block包含多少个重复的residual结构，shortcut_type是residual采取的形式，默认是’B’，指明了ResNet中residual的具体类型。self.avgpool和ResNet网络中最后的7*7的均值池化一样，只不过这里采用的是5*5，主要是因为输入帧的大小是160*160，和ResNet中224*224大小的图像不同。self.layer1和其他3个layer不同的是输入中没有stride=2，这是因为在self.layer1之前已经进行过一次pool操作了，所以这里不需要stride，这和ResNet中每个block的feature map尺寸缩减策略是一致的。`for m in self.modules():`这个循环是用来对构造好的网络结构进行参数初始化，这里只对卷积层和BN层进行初始化。`__init__`的最后三行先是指定了网络输入数据的尺寸，这里16表示16帧图像，然后指定了均值和标准差用于数据归一化。接下来介绍P3D类的`_make_layer`方法。

```
class P3D(nn.Module):

    def __init__(self, block, layers, modality='RGB',
        shortcut_type='B', num_classes=400,dropout=0.5,ST_struc=('A','B','C')):
        self.inplanes = 64
        super(P3D, self).__init__()
        # self.conv1 = nn.Conv3d(3, 64, kernel_size=7, stride=(1, 2, 2),
        #                        padding=(3, 3, 3), bias=False)
        self.input_channel = 3 if modality=='RGB' else 2  # 2 is for flow 
        self.ST_struc=ST_struc

        self.conv1_custom = nn.Conv3d(self.input_channel, 64, kernel_size=(1,7,7), stride=(1,2,2),
                                padding=(0,3,3), bias=False)

        self.depth_3d=sum(layers[:3])# C3D layers are only (res2,res3,res4),  res5 is C2D

        self.bn1 = nn.BatchNorm3d(64) # bn1 is followed by conv1
        self.cnt=0
        self.relu = nn.ReLU(inplace=True)
        self.maxpool = nn.MaxPool3d(kernel_size=(2, 3, 3), stride=2, padding=0)       # pooling layer for conv1.
        self.maxpool_2 = nn.MaxPool3d(kernel_size=(2,1,1),padding=0,stride=(2,1,1))   # pooling layer for res2, 3, 4.

        self.layer1 = self._make_layer(block, 64, layers[0], shortcut_type)
        self.layer2 = self._make_layer(block, 128, layers[1], shortcut_type, stride=2)
        self.layer3 = self._make_layer(block, 256, layers[2], shortcut_type, stride=2)
        self.layer4 = self._make_layer(block, 512, layers[3], shortcut_type, stride=2)

        self.avgpool = nn.AvgPool2d(kernel_size=(5, 5), stride=1)                              # pooling layer for res5.
        self.dropout=nn.Dropout(p=dropout)
        self.fc = nn.Linear(512 * block.expansion, num_classes)

        for m in self.modules():
            if isinstance(m, nn.Conv3d):
                n = m.kernel_size[0] * m.kernel_size[1] * m.out_channels
                m.weight.data.normal_(0, math.sqrt(2. / n))
            elif isinstance(m, nn.BatchNorm3d):
                m.weight.data.fill_(1)
                m.bias.data.zero_()

        # some private attribute
        self.input_size=(self.input_channel,16,160,160)       # input of the network
        self.input_mean = [0.485, 0.456, 0.406] if modality=='RGB' else [0.5]
        self.input_std = [0.229, 0.224, 0.225] if modality=='RGB' else [np.mean([0.229, 0.224, 0.225])]
```

**在`__init__`中提到构建网络的block时采用P3D类自定义的_make_layer方法**，该方法用来定义网络结构中90%的层和先后顺序，也就是定义了4个block的内容。`if self.cnt==0: stride_p=1  else:      stride_p=(1,2,2)` 这部分是用来区别layer1和layer2至layer4调用_make_layer时候的不同参数，在layer2到layer4中，用stride_p=(1,2,2)来对feature map的尺寸做缩减。因为shortcut_type是’B’，所以这里是通过else语句中的 `downsample = nn.Sequential( nn.Conv3d(self.inplanes, planes * block.expansion, kernel_size=1, stride=stride_p, bias=False), nn.BatchNorm3d(planes *block.expansion))`来得到downsample的。torch.nn.Sequential这个类是用来封装多个网络层的，并且封装的顺序就是数据流的顺序，就像这里是先进行一个卷积层，再进行一个BN层。 生成downsample层后，这一行`layers.append(block(self.inplanes, planes, stride, downsample,n_s=self.cnt,depth_3d=self.depth_3d,ST_struc=self.ST_struc))`主要是调用了Bottleneck类将一个block的第一个residual添加到layer列表中（关于Bottleneck的详细内容后面有介绍）。 `for i in range(1, blocks)`这个循环则是将该block的剩余residual结构添加到网络中，从而完成该block的所有层构造，因此对于layer1而言，这个输入blocks就是3，对于layer2而言，这个输入blocks就是8，range的起始是1正是跳过了前面单独添加的那个downsample层。最后将列表layers用torch.nn.Sequential类进行封装，成为一个网络子结构。

```
def _make_layer(self, block, planes, blocks, shortcut_type, stride=1):
        downsample = None
        stride_p=stride #especially for downsample branch.

        if self.cnt<self.depth_3d:
            if self.cnt==0:
                stride_p=1
            else:
                stride_p=(1,2,2)
            if stride != 1 or self.inplanes != planes * block.expansion:
                if shortcut_type == 'A':
                    downsample = partial(downsample_basic_block,
                                         planes=planes * block.expansion, stride=stride)
                else:
                    downsample = nn.Sequential(
                        nn.Conv3d(self.inplanes, planes * block.expansion,
                                  kernel_size=1, stride=stride_p, bias=False),
                        nn.BatchNorm3d(planes * block.expansion)
                    )

        else:
            if stride != 1 or self.inplanes != planes * block.expansion:
                if shortcut_type == 'A':
                    downsample = partial(downsample_basic_block,
                                         planes=planes * block.expansion, stride=stride)
                else:
                    downsample = nn.Sequential(
                        nn.Conv2d(self.inplanes, planes * block.expansion,
                                  kernel_size=1, stride=2, bias=False),
                        nn.BatchNorm2d(planes * block.expansion)
                    )
        layers = []
        layers.append(block(self.inplanes, planes, stride, downsample,n_s=self.cnt,depth_3d=self.depth_3d,ST_struc=self.ST_struc))
        self.cnt+=1

        self.inplanes = planes * block.expansion
        for i in range(1, blocks):
            layers.append(block(self.inplanes, planes,n_s=self.cnt,depth_3d=self.depth_3d,ST_struc=self.ST_struc))
            self.cnt+=1

        return nn.Sequential(*layers)
```

**Bottleneck类**的定义，从命名也可以看出是和ResNet中的bottleneck对应，也就是在residule子结构中的那三层卷积（卷积核大小分别是1*1，3*3，1*1，第一个1*1卷积用来缩减维度，这样3*3卷积的计算量就会下降，最后一个卷积用来恢复维度）。同样的，网络的定义还是继承torch.nn.Module这个基类。因此在`__init__`中的3个主要的if语句就是用来完成bottleneck中的3层卷积的定义。至于为什么有if和else，主要是为了区别layer1至layer3和layer4的区别，因为前面3个layer都是用3D卷积（if部分），而layer4是采用2D卷积（else部分）。 

1、`if n_s<self.depth_3d`是第一个卷积层相关的定义。这里主要就是一个1*1*1的卷积（用来缩减channel数）和一个BN层。else语句中就是2D的1*1卷积。 

2、`if self.id<self.depth_3d`是第二个卷积层相关的定义。首先`self.ST=list(self.ST_struc)[self.id%self.len_ST]`这一行就是用来确定采用的是’A’、’B’、’C’中的哪种P3D子结构。接下来的 `if self.id<self.depth_3d`条件语句就是完成具体的P3D子结构定义，具体而言就是确定了self.conv2和self.conv3的形式。同样，if部分是针对layer1到layer3，else部分是针对layer4。在if部分调用的conv_S卷积正是论文中介绍的1*3*3卷积，conv_T卷积正是论文中介绍的3*1*1卷积。因此self.conv2和self.conv3的不同组装顺序就构成了论文中的P3D的3种不同子结构，后面会详细介绍。 

3、最后这个`if n_s<self.depth_3d`语句是定义self.conv4，也就是bottleneck中第三个卷积层（卷积核大小为1*1*1，用来恢复被缩减的channel数）。同样else部分是2D的1*1卷积。
```
class Bottleneck(nn.Module):
    expansion = 4

    def __init__(self, inplanes, planes, stride=1, downsample=None,n_s=0,depth_3d=47,ST_struc=('A','B','C')):
        super(Bottleneck, self).__init__()
        self.downsample = downsample
        self.depth_3d=depth_3d
        self.ST_struc=ST_struc
        self.len_ST=len(self.ST_struc)

        stride_p=stride
        if not self.downsample ==None:
            stride_p=(1,2,2)
        if n_s<self.depth_3d:
            if n_s==0:
                stride_p=1
            self.conv1 = nn.Conv3d(inplanes, planes, kernel_size=1, bias=False,stride=stride_p)
            self.bn1 = nn.BatchNorm3d(planes)
        else:
            if n_s==self.depth_3d:
                stride_p=2
            else:
                stride_p=1
            self.conv1 = nn.Conv2d(inplanes, planes, kernel_size=1, bias=False,stride=stride_p)
            self.bn1 = nn.BatchNorm2d(planes)
        # self.conv2 = nn.Conv3d(planes, planes, kernel_size=3, stride=stride,
        #                        padding=1, bias=False)
        self.id=n_s
        self.ST=list(self.ST_struc)[self.id%self.len_ST]
        if self.id<self.depth_3d:
            self.conv2 = conv_S(planes,planes, stride=1,padding=(0,1,1))
            self.bn2 = nn.BatchNorm3d(planes)
            #
            self.conv3 = conv_T(planes,planes, stride=1,padding=(1,0,0))
            self.bn3 = nn.BatchNorm3d(planes)
        else:
            self.conv_normal = nn.Conv2d(planes, planes, kernel_size=3, stride=1,padding=1,bias=False)
            self.bn_normal = nn.BatchNorm2d(planes)

        if n_s<self.depth_3d:
            self.conv4 = nn.Conv3d(planes, planes * 4, kernel_size=1, bias=False)
            self.bn4 = nn.BatchNorm3d(planes * 4)
        else:
            self.conv4 = nn.Conv2d(planes, planes * 4, kernel_size=1, bias=False)
            self.bn4 = nn.BatchNorm2d(planes * 4)
        self.relu = nn.ReLU(inplace=True)

        self.stride = stride
```

conv_S和conv_T的定义如下：

```python
def conv_S(in_planes,out_planes,stride=1,padding=1):
    # as is descriped, conv S is 1x3x3
    return nn.Conv3d(in_planes,out_planes,kernel_size=(1,3,3),stride=1,
                     padding=padding,bias=False)

def conv_T(in_planes,out_planes,stride=1,padding=1):
    # conv T is 3x1x1
    return nn.Conv3d(in_planes,out_planes,kernel_size=(3,1,1),stride=1,
                     padding=padding,bias=False)
```

**接下来介绍P3D类中的forward方法**。前面介绍的P3D类或Bottleneck类的`__init__`只是定义了网络的层，但是层与层之间的连接关系一般是类中的forward方法来控制的。这里先介绍P3D类中的forward方法。P3D类中的forward方法是在main函数中运行out=model(data)时调用的。forward方法比较清晰，先是1*7*7的卷积这部分，这个卷积会将10*3*16*160*160的输入变成10*64*16*80*80的输出，随后的`x=self.maxpool(x)`进一步将输入x变成10*64*8*39*39大小。然后是4个layer，每个layer其实是一个block，里面包含多个layer的叠加，叠加的先后逻辑关系已经在`__init__`函数中通过调用self._make_layer方法确定了。当然这里当执行到layer内部的时候，以self.layer1(x)为例，会调用Bottleneck类的forward方法（这个在后面会详细介绍）。 
**介绍下forward方法中数据的维度变化**，数据都是Variable类型。forward方法的输入x维度是10*3*16*160*160，10是表示10个clip，3是3通道彩色，16表示帧数，160*160是每一帧图像的大小。x = `self.conv1_custom(x)`后x的维度是10*64*16*80*80。`x = self.maxpool(x)`后x的维度是10*64*8*39*39。`self.layer1(x)`后的维度是10*256*8*39*39。`x = self.maxpool_2(self.layer1(x))`后x的尺寸是10*256*4*39*39，可以看出self.maxpool_2是对倒数第3个维度做了减半。`self.layer2(x)`后的维度是10*512*4*20*20。`x = self.maxpool_2(self.layer2(x))`后x的尺寸是10*512*2*20*20。`self.layer3(x)`后的维度是10*1024*2*10*10。`x = self.maxpool_2(self.layer3(x))`后x的尺寸是10*1024*1*10*10。可以看出输入之所以采用16帧，是和网络的4次对该维度的减半对应。x.view是一个reshape操作，将5维的1*1024*1*10*10变成4维的10*1024*10*10。x = self.layer4(x)后x的维度是10*2048*5*5，因此self.layer4中的卷积都是二维卷积。`x = self.avgpool(x)`后x的维度是10*2048*1*1。`x = x.view(-1,self.fc.in_features)`中的`self.fc.in_features`是指定义的全连接层的输入channel数量，所以得到的x维度就是10*2048。再经过dropout层，最后经过全连接层：`x = self.fc(self.dropout(x))`，因为全连接层的输出channel设置为400，这是因为kinetics数据集的类别数是400，所以最后输出维度是10*400，如果数据集是sports-1M，那么类别数就是487。

```
def forward(self, x):
        x = self.conv1_custom(x)
        x = self.bn1(x)
        x = self.relu(x)
        x = self.maxpool(x)

        x = self.maxpool_2(self.layer1(x))  #  Part Res2
        x = self.maxpool_2(self.layer2(x))  #  Part Res3
        x = self.maxpool_2(self.layer3(x))  #  Part Res4

        sizes=x.size()
        x = x.view(-1,sizes[1],sizes[3],sizes[4])  #  Part Res5
        x = self.layer4(x)
        x = self.avgpool(x)

        x = x.view(-1,self.fc.in_features)
        x = self.fc(self.dropout(x))

        return x
```

**接下来介绍Bottleneck类中的forward方法**，这是在P3D类的forward方法中进行到类似self.layer1(x)这一步的时候会调用的（之所以用类似这个词是因为self.layer2(x)、self.layer3(x)等也会调用）。self.conv1就是channel缩减的卷积层。然后就根据self.ST的不同值来选择不同的P3D子结构（分别调用self.ST_A、self.ST_B、self.ST_C方法，这些方法也是定义在Bottleneck类中，后面有列出代码）。最后的self.conv4就是channel增加的卷积层。`out += residual`就是ResNet中skip connection的支路合并部分。

```
def forward(self, x):
        residual = x

        out = self.conv1(x)
        out = self.bn1(out)
        out = self.relu(out)

        # out = self.conv2(out)
        # out = self.bn2(out)
        # out = self.relu(out)
        if self.id<self.depth_3d: # C3D parts: 

            if self.ST=='A':
                out=self.ST_A(out)
            elif self.ST=='B':
                out=self.ST_B(out)
            elif self.ST=='C':
                out=self.ST_C(out)
        else:
            out = self.conv_normal(out)   # normal is res5 part, C2D all.
            out = self.bn_normal(out)
            out = self.relu(out)

        out = self.conv4(out)
        out = self.bn4(out)

        if self.downsample is not None:
            residual = self.downsample(x)

        out += residual
        out = self.relu(out)

        return out
```

**ST_A、ST_B和ST_C是论文中重要的子结构**，和论文对应，比较容易理解。

```python
def ST_A(self,x):
        x = self.conv2(x)
        x = self.bn2(x)
        x = self.relu(x)

        x = self.conv3(x)
        x = self.bn3(x)
        x = self.relu(x)

        return x

    def ST_B(self,x):
        tmp_x = self.conv2(x)
        tmp_x = self.bn2(tmp_x)
        tmp_x = self.relu(tmp_x)

        x = self.conv3(x)
        x = self.bn3(x)
        x = self.relu(x)

        return x+tmp_x

    def ST_C(self,x):
        x = self.conv2(x)
        x = self.bn2(x)
        x = self.relu(x)

        tmp_x = self.conv3(x)
        tmp_x = self.bn3(tmp_x)
        tmp_x = self.relu(tmp_x)

        return x+tmp_x
```





