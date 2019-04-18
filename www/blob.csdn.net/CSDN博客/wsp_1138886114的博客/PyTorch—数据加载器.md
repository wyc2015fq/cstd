# PyTorch—数据加载器 - wsp_1138886114的博客 - CSDN博客





2019年02月21日 20:20:42[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：168








我们之前示范的许多模型示例使用的数据集都是比较少的，但是一旦涉及到需要加载大量的数据集时，会面临内存不够用的局面，这里就涉及到了大数据集的加载问题。
- 大数据量数据量：
将所有的图像数据直接加载到numpy数据中会占用大量的内存

由于需要对数据进行导入，每次训练的时候在数据读取阶段会占用大量的时间

只使用了单线程去读取，读取效率比较低下

拓展性很差，如果需要对数据进行一些预处理，例如数据增强时会消耗大量的等待时间。
我们可以根据Pytorch官方提供的数据加载模板，去编写自己的加载数据集类，从而实现高效稳定地加载我们的数据集。

关于如何自定义图像加载类 [详情请点击](https://blog.csdn.net/wsp_1138886114/article/details/83620869)。

### Dataset类

Dataset类是Pytorch中所有数据集加载类中应该继承的父类。其中父类中的两个私有成员函数    `def __getitem__(self, index):`、`def __len(self)__:`必须被重载，否则将会触发错误提示。

图像数据读取方式：[https://oldpan.me/archives/pytorch-transforms-opencv-scikit-image](https://oldpan.me/archives/pytorch-transforms-opencv-scikit-image)

##### 定义自己的数据集类

首先继承上面的`dataset`类。

然后在`__init__()`方法中得到图像的路径，然后将图像路径组成一个数组。

然后在`__getitim__()`中就可以直接读取
```python
import os
import numpy as np
from PIL import Image
from torch.utils.data import Dataset,DataLoader
import torchvision.transforms.ToTensor as to_tensor


class custom_Dataset(Dataset):
    """
     root：   图像存放地址根路径
     augment：是否需要图像增强
    """
    def __init__(self, root, augment=None):
        suffix = (".jpg",".png",".jpeg",)
        self.image_files = np.array([x.path for x in os.scandir(root)if x.name.endswith(suffix)])
        self.augment = augment               # 是否需要图像增强

    def __getitem__(self, index):
        if self.augment:                     # 使用图像增强
            image = Image.open(self.image_files[index])
            image = self.augment(image)
            return to_tensor(image)
        else:
            return to_tensor(Image.open(self.image_files[index]))

    def __len__(self):
        return len(self.image_files)         # 返回图像的数量
```




