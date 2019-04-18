# PyTorch之九— cifar-10数据集 图像分类 - wsp_1138886114的博客 - CSDN博客





2019年03月01日 15:15:43[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：58标签：[PyTorch之九— cifar-10数据集 图像分类](https://so.csdn.net/so/search/s.do?q=PyTorch之九— cifar-10数据集 图像分类&t=blog)
个人分类：[pytorch](https://blog.csdn.net/wsp_1138886114/article/category/8293033)












### 文章目录
- [一、准备](#_2)
- [可视化保存](#_32)
- [查看数据结构](#_76)



这是一个入门级的图像分类模型，主要是用来熟悉模型。


### 一、准备

**cifar-10 数据集**
- 
数据集下载：[http://www.cs.toronto.edu/~kriz/cifar.html](http://www.cs.toronto.edu/~kriz/cifar.html)

若是下载太慢了，[你可以点击CSDN下载链接]()

- 
数据集组成：本数据及包含了6万张分辨率为32x32的图片，一共10类，分别为：飞机、汽车、鸟、猫、鹿、狗、青蛙、马、船、货车。

其中，5万张作为训练集，1万张作为测试集。

- 
训练集被分为了5批训练和1批测试。每一批都是1万张。

测试集是从每一种分类中随机抽取出来1000张组成。

训练集从10个分类中各自随机抽取5000张，一共5万张。- 
数据的结构

首先看一下数据集解压之后的样子：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190125141456668.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)那些没有后缀名的文件，其实都是用python的`cpickle`库打包好的，这个库就是用来将python中的变量原封不动地存到本地使用的，当再次使用的时候，可以读取出来。

读取方式：
```python
def unpickle(file):
    import pickle
    with open(file, 'rb') as fo:
        dict = pickle.load(fo, encoding='bytes')
    return dict
```

batches.meta，这个文件保存的就是索引与名字之间的对应。

如：`label_names[0] == "airplane", label_names[1] == "automobile"`
|键|值|
|----|----|
|data|一个10000x3072的numpy数组，每一行都存储着一个分辨率为32x32的图片|
|labels|标签，长度是10000，每个都是0-9的数字，是一个列表。其索引和data里面的索引相互对应。|

##### 可视化保存

```python
import pickle as p
import numpy as np
import matplotlib.image as plimg
from PIL import Image


def load_CIFAR_batch(filename):
    with open(filename, 'rb')as f:
        datadict = p.load(f, encoding='latin1')
        X = datadict['data']
        Y = datadict['labels']
        X = X.reshape(10000, 3, 32, 32)
        Y = np.array(Y)
        return X, Y

def load_CIFAR_Labels(filename):
    with open(filename, 'rb') as f:
        lines = [x for x in f.readlines()]
        print(lines)


if __name__ == "__main__":
    load_CIFAR_Labels("E:/pytorch_AI/data/cifar-10-batches-py/batches.meta")
    imgX, imgY = load_CIFAR_batch("E:/pytorch_AI/data/cifar-10-batches-py/data_batch_1")
    print(imgX.shape)
    print("正在保存图片:")
    for i in range(30):
        imgs = imgX[i]                         # 遍历获取图像
        i0 = Image.fromarray(imgs[0])          # 通道1
        i1 = Image.fromarray(imgs[1])          # 通道2
        i2 = Image.fromarray(imgs[2])          # 通道3
        img = Image.merge("RGB", (i0, i1, i2)) # 从数据，合并三个通道，生成image对象
        name = "img" + str(i) + '.png'
        img.save("./data/cifar-10-batches-py_img/" + name, "png")        # 文件夹下是RGB融合后的图像
        for j in range(imgs.shape[0]):
            img = imgs[j]
            name = "img" + str(i) + str(j) + ".png"
            print("正在保存图片" + name)
            plimg.imsave("./data/cifar-10-batches-py_img_0/" + name, img)  # 文件夹下是RGB分离的图像
    print("保存完毕.")
```

##### 查看数据结构

```python
def unpickle(file):
    import pickle
    with open(file, 'rb') as fo:
        dict = pickle.load(fo, encoding='bytes')
    return dict

if __name__ == '__main__':
    file = "E:/pytorch_AI/data/cifar-10-batches-py/data_batch_1"
    dict = unpickle(file)
    dict_keys = dict.keys()
    print("dict_keys:           \t",dict_keys)                 # dict.keys()
    print("dict[b'batch_label']:\t" , dict[b'batch_label'])    # key=b'batch_label'的内容
    print("dict[b'labels']_type:\t" , type(dict[b'labels']))   # key=b'labels'的数据类型

    print("dict[b'filenames'][0]:\t" , dict[b'filenames'][0])  # 查看第一个图片名
    print("dict[b'data'].shape():\t" , dict[b'data'].shape)    # 查看 b'data' 的数据形状
    print("dict[b'data'][0]     :\t", dict[b'data'][0])        # 查看 b'data'的第一个数据

输出：
dict_keys:           	 dict_keys([b'batch_label', b'labels', b'data', b'filenames'])
dict[b'batch_label']:	 b'training batch 1 of 5'
dict[b'labels']_type:	 <class 'list'>
dict[b'filenames'][0]:	 b'leptodactylus_pentadactylus_s_000004.png'
dict[b'data'].shape():	 (10000, 3072)
dict[b'data'][0]     :	 [ 59  43  50 ... 140  84  72]
```

待续







