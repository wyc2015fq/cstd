# Pytorch之下载数据集 - qq_37385726的博客 - CSDN博客





2018年08月17日 09:04:46[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：1063
所属专栏：[Pytorch](https://blog.csdn.net/column/details/26172.html)









> 
如果你的torchvision还没有装好，可以参看[https://blog.csdn.net/qq_37385726/article/details/81744485](https://blog.csdn.net/qq_37385726/article/details/81744485)

（应对于WIndows下Python3.6，cuda=none）




# **目录**

[1.代码](https://blog.csdn.net/qq_37385726/article/details/81749890#1.%E4%BB%A3%E7%A0%81)

[MNIST](https://blog.csdn.net/qq_37385726/article/details/81749890#mnist)

[CIFAR](https://blog.csdn.net/qq_37385726/article/details/81749890#cifar)

[效果](https://blog.csdn.net/qq_37385726/article/details/81749890#%E6%95%88%E6%9E%9C)





# 1.代码

> 
Pytorch中有很多常用的数据集模块，预先保存在了torchvision.datasets中，要用的时候下载即可。


**`torchvision.datasets`中包含了以下数据集**
- MNIST
- COCO（用于图像标注和目标检测）(Captioning and Detection)
- LSUN Classification
- ImageFolder
- Imagenet-12
- CIFAR10 and CIFAR100
- STL10

现介绍MNIST和CIFAR的下载方式

## MNIST

```
dset.MNIST(root, train=True, transform=None, target_transform=None, download=False)
```

参数说明：

- root : `processed/training.pt` 和 `processed/test.pt` 的主目录

- train : `True` = 训练集, `False`= 测试集

- download : `True` = 从互联网上下载数据集，并把数据集放在`root`目录下. 如果数据集之前下载过，就赋值为False，不再重复下载

## CIFAR

```python
dset.CIFAR10(root, train=True, transform=None, target_transform=None, download=False)
 
dset.CIFAR100(root, train=True, transform=None, target_transform=None, download=False)
```

参数说明：

- root : `cifar-10-batches-py` 的根目录

- train : `True` = 训练集, `False` = 测试集

- transform : 定义对于下载到的数据的数据变化形式，利用torchvision.transforms中的数据变换函数处理

- download : `True` = 从互联上下载数据，并将其放在`root`目录下。如果数据集已经下载，什么都不干。



```python
import torchvision.datasets as dsets
import  torchvision.transforms as transforms
from  PIL import Image
 
#super parameters
DOWNLOAD = True
 
 
#定义数据变换
transform1 = transforms.ToTensor()  #可以把下载到的数据转化成张量格式
 
#transforms.Compose()定义多重数据变化
transform2 = transforms.Compose(
    [transforms.ToTensor(),
     transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))])  #归一化[-1,1]
 
mT_trainset = dsets.MNIST(root='./MNIST/Tensor/training',train=True,transform=transform1,download=DOWNLOAD)
mT_testset = dsets.MNIST(root='./MNIST/Tensor/test',train=False,transform=transform1,download=DOWNLOAD)
cT_trainset = dsets.CIFAR10(root='./CIFAR10/Tensor/training',train=True,transform=transform1,download=DOWNLOAD)
cT_testset = dsets.CIFAR10(root='./CIFAR10/Tensor/test',train=False,transform=transform1,download=DOWNLOAD)
 
 
mN_trainset = dsets.MNIST(root='./MNIST/Normal/training',train=True,transform=transform2,download=DOWNLOAD)
mN_testset = dsets.MNIST(root='./MNIST/Normal/test',train=False,transform=transform2,download=DOWNLOAD)
cN_trainset = dsets.CIFAR10(root='./CIFAR10/Normal/training',train=True,transform=transform2,download=DOWNLOAD)
cN_testset = dsets.CIFAR10(root='./CIFAR10/Normal/test',train=False,transform=transform2,download=DOWNLOAD)
```



# 效果

![](https://img-blog.csdn.net/20180817085935833?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



