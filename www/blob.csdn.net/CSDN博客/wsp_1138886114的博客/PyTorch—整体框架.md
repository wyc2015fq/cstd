# PyTorch—整体框架 - wsp_1138886114的博客 - CSDN博客





2018年11月04日 17:45:28[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：293











### 文章目录
- [一、数据读取](#_8)
- [二、网络构建](#_41)
- [三、训练参数设置](#_54)
- [四、模型训练与更新](#_62)



PyTorch是Facebook的官方深度学习框架之一，相信使用过的人都会被其轻便和快速等特点深深吸引，它采用与python一样的编程思想，与tensorflow编译运行不同。

关于学习资料详情请查看：（以下网址可能会失效，请自行百度）

PyTorch的官方github地址：[https://github.com/pytorch/pytorch](https://github.com/pytorch/pytorch)

PyTorch官方文档：[https://pytorch.org/docs/stable/index.html](https://pytorch.org/docs/stable/index.html)

PyTorch中文文档：[http://pytorch-cn.readthedocs.io/zh/latest/#pytorch](http://pytorch-cn.readthedocs.io/zh/latest/#pytorch)


现以 pytorch图像分类模型 为模板来了解pytorch

### 一、数据读取
- **torch.utils.data.Dataset**- **torchvision.ImageFolder** :数据读取，将图像和标签数据转换成PyTorch框架的Tensor数据类型。这个接口针对的数据存放方式是每个文件夹包含一个类的图像。
- **自定义数据读取接口**，实际应用中可能你的数据是多标签的，或者其他更复杂的形式，那么就需要自定义一个数据读取接口。

比如比赛的图像文件一般是一个文件夹存放各个类别所有图像数据，用一个对应的标签文件（json/txt文件）来保存图像和标签的对应关系。在这种情况下需要自定义一个数据读取接口。


数据流：自定义数据读取 >> 数据预处理(torchvision.transforms)  >> 封装成迭代器（生成了Tensor类型的数据迭代器:torch.utils.data.DataLoader）

```python
"""
torchvision： PyTorch框架中的包
.
├── torchvision.datasets                自带的数据集
|   ├── MNIST、COCO(图像标注/目标检测)、CIFAR10/CIFAR100
|   └── LSUN Classification、ImageFolder、Imagenet-12、STL10
|
├── torchvision.models                  自带的当前流行模型
|   └──AlexNet、VGG、ResNet、SqueezeNet、DenseNet
|
├── torchvision.transforms              数据处理
|   └── transforms.Compose(transforms)  一系列的transforms 操作
|       └── data augmentation           包含resize、crop等常见数据增强操作
|           ├──transformas.py
|           └──functional.py
└─── torchvision.utils
     ├── torchvision.utils.make_grid 
     └── torchvision.utils.save_image          
"""
```

关于详情，请查看;[https://blog.csdn.net/wsp_1138886114/article/details/83620869](https://blog.csdn.net/wsp_1138886114/article/details/83620869)

详情你可以查看：[https://github.com/miraclewkf/ImageClassification-PyTorch](https://github.com/miraclewkf/ImageClassification-PyTorch)

### 二、网络构建

所有模型都基于**torch.nn.Module**
- **torchvision.models**

PyTorch框架中提供了一些常用的网络结构（VGG，ResNet，DenseNet）及预训练模型接口。直接导入指定的网络结构，并且可以选择是否用预训练模型初始化导入的网络结构。
[https://pytorch-cn.readthedocs.io/zh/latest/package_references/torch-nn/](https://pytorch-cn.readthedocs.io/zh/latest/package_references/torch-nn/)- 自定义网络结构

关于自定义网络结构可以参考：

1、[https://github.com/miraclewkf/MobileNetV2-PyTorch。](https://github.com/miraclewkf/MobileNetV2-PyTorch%E3%80%82)

或者 [https://pytorch-cn.readthedocs.io/zh/latest/package_references/torch-nn/](https://pytorch-cn.readthedocs.io/zh/latest/package_references/torch-nn/)

该项目中的MobileNetV2.py脚本自定义了网络结构。

2、[https://github.com/miraclewkf/SENet-PyTorch。](https://github.com/miraclewkf/SENet-PyTorch%E3%80%82) 该项目中的se_resnet.py和se_resnext.py脚本分别自定义了不同的网络结构。- **torch.load**接口导入预训练模型，调用自定义的网络结构对象的load_state_dict方式进行参数初始化，具体可以看 [https://github.com/miraclewkf/MobileNetV2-PyTorch](https://github.com/miraclewkf/MobileNetV2-PyTorch) 项目中的train.py脚本中if args.resume条件语句。

### 三、训练参数设置
- 优化函数—通过 **torch.optim** 包实现，比如`torch.optim.SGD()`接口表示随机梯度下降。更多优化函数可以看官方文档：[http://pytorch.org/docs/0.3.0/optim.html。](http://pytorch.org/docs/0.3.0/optim.html%E3%80%82)
- 学习率策略—通过 **torch.optim.lr_scheduler** 接口实现，比如`torch.optim.lr_scheduler.StepLR()`接口表示按指定epoch数减少学习率。更多学习率变化策略可以看官方文档：[http://pytorch.org/docs/0.3.0/optim.html。](http://pytorch.org/docs/0.3.0/optim.html%E3%80%82)
- 损失函数—通过 **torch.nn** 包实现，比如`torch.nn.CrossEntropyLoss()`接口表示交叉熵损失函数等。

或者查看 [https://pytorch-cn.readthedocs.io/zh/latest/package_references/torch-optim/](https://pytorch-cn.readthedocs.io/zh/latest/package_references/torch-optim/)
- 多GPU训练—通过 **torch.nn.DataParallel** 接口实现。

比如：`model = torch.nn.DataParallel(model, device_ids=[0,1])`表示在gpu0和1上训练模型

### 四、模型训练与更新

初始化网络参数，关于详情你可以查看：[https://pytorch-cn.readthedocs.io/zh/latest/search.html?q=state_dict](https://pytorch-cn.readthedocs.io/zh/latest/search.html?q=state_dict)
- 初始化模型参数：`best_model_wts = model.state_dict()` 以 dict 返回optimizer的状态，保存着 module 的所有状态（ state ）。 它包含两项:

state - 保存当前优化状态的dict。optimizer的类别不同，state的内容也会不同。

param_groups - 一个包含了全部参数组的dict。- 最佳准确率：best_acc = 0.0
- 更新 best_model_wts，`if phase == 'val' and epoch_acc > best_acc: best_acc = epoch_acc，best_model_wts = model.state_dict()` 保存当前module参数
- 用来加载模型参数：`model.load_state_dict(best_model_wts)`

**关于使用示例请看如下代码**：

以下使用默认的Dataset class（torchvision.datasets.ImageFolder）来读取Image。

```python
from __future__ import print_function, division

import torch
import torch.nn as nn
import torch.optim as optim
from torch.optim import lr_scheduler
from torch.autograd import Variable
import torchvision
from torchvision import datasets, models, transforms
import time
import os

def train_model(model, criterion, optimizer, scheduler, num_epochs=25):
    """
    在每个epoch开始时都需要如下更新：
    scheduler.step()                    模型调度 
    model.train(True)                   设置模型状态为训练状态
    optimizer.zero_grad()               将网络中的所有梯度置0
    outputs = model(inputs)             数据输入：网络的前向传播了
    torch.max(outputs.data, 1)          模型预测该样本属于哪个类别，torch.max（tensor格式，每一行的最大值）
    loss = criterion(outputs, labels)   输出outputs和原labels作为loss函数的输入就可以得到损失
    loss.backward()                     回传损失（只训练时用）
    optimizer.step()                    更新参数
    """
    since = time.time()

    best_model_wts = model.state_dict()
    best_acc = 0.0

    for epoch in range(num_epochs):
        print('Epoch {}/{}'.format(epoch, num_epochs - 1))
        print('-' * 10)

        # Each epoch has a training and validation phase
        for phase in ['train', 'val']:
            if phase == 'train':
                scheduler.step()
                model.train(True)  # Set model to training mode
            else:
                model.train(False)  # Set model to evaluate mode

            running_loss = 0.0
            running_corrects = 0

            # Iterate over data.
            for data in dataloders[phase]:
                inputs, labels = data

                # wrap them in Variable
                if use_gpu:
                    inputs = Variable(inputs.cuda())
                    labels = Variable(labels.cuda())
                else:
                    inputs, labels = Variable(inputs), Variable(labels)

                # zero the parameter gradients
                optimizer.zero_grad()

                # forward
                outputs = model(inputs)
                _, preds = torch.max(outputs.data, 1)
                loss = criterion(outputs, labels)

                # backward + optimize only if in training phase
                if phase == 'train':
                    loss.backward()
                    optimizer.step()

                # statistics
                running_loss += loss.data[0]
                running_corrects += torch.sum(preds == labels.data)

            epoch_loss = running_loss / dataset_sizes[phase]
            epoch_acc = running_corrects / dataset_sizes[phase]

            print('{} Loss: {:.4f} Acc: {:.4f}'.format(
                phase, epoch_loss, epoch_acc))

            # deep copy the model
            if phase == 'val' and epoch_acc > best_acc:
                best_acc = epoch_acc
                best_model_wts = model.state_dict()

    time_elapsed = time.time() - since
    print('Training complete in {:.0f}m {:.0f}s'.format(
        time_elapsed // 60, time_elapsed % 60))
    print('Best val Acc: {:4f}'.format(best_acc))

    # load best model weights
    model.load_state_dict(best_model_wts)
    return model

if __name__ == '__main__':

    """
    函数详情：
    data_transform:                  图像预处理，比如resize、crop等.
    torchvision.transforms.Compose： 用来管理所有transforms操作的。
    torchvision.transforms.RandomSizedCrop：输入对象是PIL Image
    transforms.RandomHorizontalFlip():      输入对象是PIL Image
    transforms.Normalize(): 传入tensor。该函数之前有一个 transforms.ToTensor()
	                    就是用来生成Tensor的。
    """
    data_transforms = {
        'train': transforms.Compose([
            transforms.RandomSizedCrop(224),
            transforms.RandomHorizontalFlip(),
            transforms.ToTensor(),
            transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
        ]),
        'val': transforms.Compose([
            transforms.Scale(256),
            transforms.CenterCrop(224),
            transforms.ToTensor(),
            transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
        ]),
    }


    """
    数据目录：./data 目录下两个文件夹：train和val，每个文件下面包含的子文件夹是分类类别数。
    数据导入：官方的 torchvision.datasets.ImageFolder接口实现数据导入，返回list
    将数据和标签封装成张量：用torch.utils.data.DataLoader封装成Tensor,才能训练
    """
    data_dir = '/data'
    image_datasets = {x: datasets.ImageFolder(os.path.join(data_dir, x),
                                              data_transforms[x]) for x in ['train', 'val']}
    dataloders = {x: torch.utils.data.DataLoader(image_datasets[x],
                                                 batch_size=4,
                                                 shuffle=True,
                                                 num_workers=4) for x in ['train', 'val']}

    dataset_sizes = {x: len(image_datasets[x]) for x in ['train', 'val']}

    # use gpu or not
    use_gpu = torch.cuda.is_available()

    """
    PyTorch准备了一些常用的网络结构，比如分类中的VGG，ResNet，DenseNet等等，
    可以用torchvision.models模块来导入。预训练网络一般是在1000类的ImageNet数据集上进行的，
    迁移到你自己数据集的2分类，需要替换最后的全连接层的输出。
    
    models.resnet18(pretrained=True):导入ResNet18，pretrained=True(已预训练)。
    model_ft.fc.in_features:         获取全连接层的输入channel个数
    nn.Linear(num_ftrs, 2):          分类类别数（这里是2）
    """
    model_ft = models.resnet18(pretrained=True)
    num_ftrs = model_ft.fc.in_features
    model_ft.fc = nn.Linear(num_ftrs, 2)

    if use_gpu:
        model_ft = model_ft.cuda()

    """
    torch.nn    模块来定义网络的所有层，比如卷积、降采样、损失层等等
    torch.optim 模块实现梯度下降。另外这里写(默认)SGD，但有momentum，所以是Adam的优化方式。
	        这个类的输入包括需要优化的参数：model.parameters(),lr,momentum参数。
    torch.optim.lr_scheduler 模块的 StepLR类实现变化学习率，
	                     表示每隔step_size个epoch就将学习率降为原来的gamma倍。
    1. 定义损失函数：交叉熵损失
    2. 优化器（梯度下降函数）：Adam
    3. 变化学习率：每7个epochs衰减0.1倍
    """
    criterion = nn.CrossEntropyLoss()
    optimizer_ft = optim.SGD(model_ft.parameters(), lr=0.001, momentum=0.9)
    exp_lr_scheduler = lr_scheduler.StepLR(optimizer_ft, step_size=7, gamma=0.1)

    model_ft = train_model(model=model_ft,
                           criterion=criterion,
                           optimizer=optimizer_ft,
                           scheduler=exp_lr_scheduler,
                           num_epochs=25)
```






