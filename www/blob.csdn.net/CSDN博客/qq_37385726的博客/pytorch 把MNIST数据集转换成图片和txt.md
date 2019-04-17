# pytorch 把MNIST数据集转换成图片和txt - qq_37385726的博客 - CSDN博客





2018年11月25日 16:06:58[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：97








# **目录**

[1.下载Mnist 数据集](#1.%E4%B8%8B%E8%BD%BDMnist%20%E6%95%B0%E6%8D%AE%E9%9B%86)

[2. 保存为图片和txt](#2.%20%E4%BF%9D%E5%AD%98%E4%B8%BA%E5%9B%BE%E7%89%87%E5%92%8Ctxt)

## 1.下载Mnist 数据集

```python
import os
 
# third-party library
import torch
import torch.nn as nn
from torch.autograd import Variable
import torch.utils.data as Data
import torchvision
import matplotlib.pyplot as plt
 
# torch.manual_seed(1)    # reproducible
 
 
DOWNLOAD_MNIST = False
 
 
# Mnist digits dataset
if not(os.path.exists('./mnist/')) or not os.listdir('./mnist/'):
    # not mnist dir or mnist is empyt dir
    DOWNLOAD_MNIST = True
 
train_data = torchvision.datasets.MNIST(
    root='./mnist/',
    train=True,                                     # this is training data
    transform=torchvision.transforms.ToTensor(),    # Converts a PIL.Image or numpy.ndarray to
                                                    # torch.FloatTensor of shape (C x H x W) and normalize in the range [0.0, 1.0]
    download=DOWNLOAD_MNIST,
)
```



下载下来的其实可以直接用了，但是我们这边想把它们转换成图片和txt，这样好看些，为后面用自己的图片和txt作为准备

## 2. 保存为图片和txt

![](https://img-blog.csdnimg.cn/20181125160625319.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2,size_16,color_FFFFFF,t_70)

```python
import os
from skimage import io
import torchvision.datasets.mnist as mnist
import numpy
 
 
 
root = "./mnist/raw/"
 
train_set = (
    mnist.read_image_file(os.path.join(root, 'train-images-idx3-ubyte')),
    mnist.read_label_file(os.path.join(root, 'train-labels-idx1-ubyte'))
)
 
test_set = (
    mnist.read_image_file(os.path.join(root,'t10k-images-idx3-ubyte')),
    mnist.read_label_file(os.path.join(root,'t10k-labels-idx1-ubyte'))
)
 
print("train set:", train_set[0].size())
print("test set:", test_set[0].size())
 
 
def convert_to_img(train=True):
    if(train):
        f = open(root + 'train.txt', 'w')
        data_path = root + '/train/'
        if(not os.path.exists(data_path)):
            os.makedirs(data_path)
        for i, (img, label) in enumerate(zip(train_set[0], train_set[1])):
            img_path = data_path + str(i) + '.jpg'
            io.imsave(img_path, img.numpy())
            f.write(img_path + ' ' + str(label) + '\n')
        f.close()
    else:
        f = open(root + 'test.txt', 'w')
        data_path = root + '/test/'
        if (not os.path.exists(data_path)):
            os.makedirs(data_path)
        for i, (img, label) in enumerate(zip(test_set[0], test_set[1])):
            img_path = data_path + str(i) + '.jpg'
            io.imsave(img_path, img.numpy())
            f.write(img_path + ' ' + str(label) + '\n')
        f.close()
 
 
convert_to_img(True)
convert_to_img(False)
```





