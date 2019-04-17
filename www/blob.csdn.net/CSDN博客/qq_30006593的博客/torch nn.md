# torch.nn - qq_30006593的博客 - CSDN博客





2017年10月21日 12:04:12[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：145








## nn.Module

模型的基类
- add_module(name, module) 

等价于self.name = nn.xxx，模块使用名字来接触
- cpu() cuda() 

复制到设备上
- double() float() half()
- eval() train() 

只对BN和Dropout有用
- apply(fn) 

应用于children的返回值，即所有子模型，可用于init
- state_dict() load_state_dict()
- zero_grad()
- children() named_children() 

是一个generator，iterator的简化版。g使用yield，i需要next，iter，init 

列出子模型，引用为模型的名字 

name后为name，例如conv1 

name前是（module），name后是（name，module）- module() named_modules() 

列出所有模型，类似于resnet，conv3d 

name后为name.name.name 

配合isinstance和tensor的操作使用
```
for m in self.modules():
    if isinstance(m, nn.Conv3d):
        n = m.kernel_size[0] * m.kernel_size[1] * m.kernel_size[2] * m.out_channels
        m.weight.data.normal_(0, math.sqrt(2. / n))
        m.bias.data.zero_()
```
- parameter() named_parameters() 

列出每个parameter，类似于conv.weight， 引用为Paramerter，即通过[0][1]来获得。 

name后为name.name.name- register_xxx 

添加hook，buf等

## nn.Parameters

Variable的子类，在module类里可用Parameter()获取

## nn.init
- gain xavier 

uniform(-a ,a): $a = gain \times \sqrt{2 / (fan\_in + fan\_out)} \times \sqrt{3}$

normal(0, std): $std = gain \times \sqrt{2 / (fan\_in + fan\_out)} $- kaiming 

uniform(-a ,a): $a = \sqrt{2 / ((1 + a^2) \times fan\_in)} \times \sqrt{3}$

normal(0, std): $std = \sqrt{2 / ((1 + a^2) \times fan\_in)}$

适用于relu网络- orthogonal sparse uniform normal constant eye dirac








