# python开源库——h5py快速指南 - 数据之美的博客 - CSDN博客
2017年03月03日 16:23:46[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：1146
### 1. 核心概念
一个HDF5文件是一种存放两类对象的容器：dataset和group. Dataset是类似于数组的数据集，而group是类似文件夹一样的容器，存放dataset和其他group。在使用h5py的时候需要牢记一句话：groups类比词典，dataset类比Numpy中的数组。 
HDF5的dataset虽然与Numpy的数组在接口上很相近，但是支持更多对外透明的存储特征，如数据压缩，误差检测，分块传输。
### 2. 读取和保存HDF5文件
#### 1) 读取HDF5文件的内容
首先我们应该打开文件：
```python
>>> import h5py
>>> f = h5py.File('mytestfile.hdf5', 'r')
```
- 1
- 2
- 1
- 2
请记住h5py.File类似[Python](http://lib.csdn.net/base/python)的词典对象，因此我们可以查看所有的键值：
```python
>>> f.keys()
[u'mydataset']
```
- 1
- 2
- 1
- 2
基于以上观测，文件中有名字为mydataset这样一个数据集。然后我们可以用类似词典的方法读取对应的dataset对象。
```python
>>> dset = f['mydataset']
```
- 1
- 1
Dset是一个HDF5的dataset对象，我们可以像Numpy的数组一样访问它的属性和数据。
```
>>> dset.shape
(100,)
>>> dset.dtype
dtype('int32')
>>> dset[...] = np.arange(100)
```
- 1
- 2
- 3
- 4
- 5
- 1
- 2
- 3
- 4
- 5
#### 2) 创建一个HDF5文件
我们用’w’模式打开文件
```python
>>> import h5py
>>> import numpy as np
>>> f = h5py.File("mytestfile.hdf5", "w")
```
- 1
- 2
- 3
- 1
- 2
- 3
然后我们借助文件对象的一系列方法添加数据。其中create_dataset用于创建给定形状和数据类型的空dataset
```python
>>> dset = f.create_dataset("mydataset", (100,), dtype='i')
```
- 1
- 1
我们也可以用现有的Numpy数组来初始化一个dataset
```
>>> arr = np.arange(100)
>>> dset = f.create_dataset("init", data=arr)
```
- 1
- 2
- 1
- 2
#### 3) 分块存储策略
在缺省设置下，HDF5数据集在内存中是连续布局的，也就是按照传统的C序。Dataset也可以在HDF5的分块存储布局下创建。也就是dataset被分为大小相同的若干块随意地分布在磁盘上，并使用B树建立索引。 
为了进行分块存储，将关键字设为一个元组来指示块的形状。
```python
>>> dset = f.create_dataset("chunked", (1000, 1000), chunks=(100, 100))
```
- 1
- 1
也可以自动分块，不必指定块的形状。
```python
>>> dset = f.create_dataset("autochunk", (1000, 1000), chunks=True)
```
- 1
- 1
### 3. HDF5的分层结构
“HDF”代表”Hierarchical Data Format”(分层数据格式). HDF5文件中group对象类似于文件夹，我们创建的文件对象本身就是一个group，称为root group.
```python
>>> f.name
u'/'
```
- 1
- 2
- 1
- 2
创建subgroup是使用create_group的方法实现的。但是我们需要先用读写模式打开文件：
```python
>>> f = h5py.File('mydataset.hdf5', 'r+')
>>> grp = f.create_group("subgroup")
```
- 1
- 2
- 1
- 2
然后grp就具有和f一样的方法了。 
我们在group上迭代从而得到group内所有的直接附属的成员（包括dataset和subgroup）
```
>>> for name in f:
...     print name
mydataset
subgroup
subgroup2
```
- 1
- 2
- 3
- 4
- 5
- 1
- 2
- 3
- 4
- 5
为了遍历一个group内的所有直接和间接成员，我们可以使用group的visit()和visititerms()方法，这些方法需要接收一个回调函数作为参数。
```python
>>> def printname(name):
...     print name
>>> f.visit(printname)
mydataset
subgroup
subgroup/another_dataset
subgroup2
subgroup2/dataset_three
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
### 4. 属性
HDF5的一个很棒的特点是你可以在数据旁边存储元数据。所有的group和dataset都支持叫做属性的数据形式。属性通过attrs成员访问，类似于python中词典格式。
```python
>>> dset.attrs['temperature'] = 99.5
>>> dset.attrs['temperature']
99.5
>>> 'temperature' in dset.attrs
True
```
- 1
- 2
- 3
- 4
- 5
- 1
- 2
- 3
- 4
- 5
### 5. 高级特征
#### 1) 滤波器组
HDF5的滤波器组能够对分块数组进行变换。最常用的变换是高保真压缩。使用一个特定的压缩滤波器创建dataset之后，读写都可以向平常一样，不必添加额外的步骤。 
用关键词compression来指定压缩滤波器，而滤波器的可选参数使用关键词compression_opt来指定：
```python
>>> dset = f.create_dataset("zipped", (100, 100), compression="gzip")
```
- 1
![](http://static.blog.csdn.net/images/save_snippets_01.png)
- 1
#### 2) HDF5文件的限制
a. HDF5文件本身大小没有限制，但是HDF5的一个dataset最高允许32个维，每个维度最多可有2^64个值，每个值大小理论上可以任意大 
b. 目前一个chunk允许的最大容量为2^32-1 byte (4GB). 大小固定的dataset的块的大小不能超过dataset的大小。
### 6. 参考文献
【1】 [http://docs.h5py.org/en/latest/quick.html](http://docs.h5py.org/en/latest/quick.html)
【2】 [http://docs.h5py.org/en/latest/high/dataset.html](http://docs.h5py.org/en/latest/high/dataset.html)
