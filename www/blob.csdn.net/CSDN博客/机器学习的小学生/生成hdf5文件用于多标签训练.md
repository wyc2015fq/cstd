
# 生成hdf5文件用于多标签训练 - 机器学习的小学生 - CSDN博客


2017年03月23日 11:03:34[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：1706个人分类：[Caffe																](https://blog.csdn.net/xuluhui123/article/category/6777566)[深度学习\deep learning																](https://blog.csdn.net/xuluhui123/article/category/6473095)[
							](https://blog.csdn.net/xuluhui123/article/category/6777566)



## 1.Python
**注**：以下内容摘自参考文献[1].
导入相关库
```python
import
```
```python
random
```
```python
from
```
```python
PIL
```
```python
import
```
```python
Image
```
```python
import
```
```python
numpy
```
```python
as
```
```python
np
```
```python
import
```
```python
h5py
```
文件配置
```python
IMAGE_DIR = [
```
```python
'image_train'
```
```python
,
```
```python
'image_test'
```
```python
]
HDF5_FILE = [
```
```python
'hdf5_train.h5'
```
```python
,
```
```python
'hdf5_test.h5'
```
```python
]
LIST_FILE = [
```
```python
'list_train.txt'
```
```python
,
```
```python
'list_test.txt'
```
```python
]
```
label配置
```python
LABELS = dict(
    # (kind_1, kind_2)
```
```python
A_0
```
```python
= (
```
```python
0
```
```python
,
```
```python
0
```
```python
),
    B_0 = (
```
```python
1
```
```python
,
```
```python
0
```
```python
),
```
```python
A_1
```
```python
= (
```
```python
0
```
```python
,
```
```python
1
```
```python
),
    B_1 = (
```
```python
1
```
```python
,
```
```python
1
```
```python
),
```
```python
A_2
```
```python
= (
```
```python
0
```
```python
,
```
```python
2
```
```python
),
    B_2 = (
```
```python
1
```
```python
,
```
```python
2
```
```python
),
)
```
生成hdf5文件
```python
print
```
```python
'\nplease wait...'
```
```python
for
```
```python
kk, image_dir
```
```python
in
```
```python
enumerate(IMAGE_DIR):
```
```python
# 读取文件列表于file_list
```
```python
file_list =
```
```python
...
```
```python
# 文件列表乱序
```
```python
random.shuffle(file_list)
```
```python
# 标签类别
```
```python
kind_index =
```
```python
...
```
```python
# 图片大小为96*32，单通道
```
```python
datas = np.zeros((len(file_list),
```
```python
1
```
```python
,
```
```python
32
```
```python
,
```
```python
96
```
```python
))
```
```python
# label大小为1*2
```
```python
labels = np.zeros((len(file_list),
```
```python
2
```
```python
))
```
```python
for
```
```python
ii, _file
```
```python
in
```
```python
enumerate(file_list):
```
```python
# hdf5文件要求数据是float或者double格式
```
```python
# 同时caffe中Hdf5DataLayer不允许使用transform_param，
```
```python
# 所以要手动除以256
```
```python
datas[ii, :, :, :] = \
            np.array(Image.open(_file)).astype(np.float32) /
```
```python
256
```
```python
labels[ii, :] = np.array(LABELS[kind_index ]).astype(np.int)
```
```python
# 写入hdf5文件
```
```python
with h5py.File(HDF5_FILE[kk],
```
```python
'w'
```
```python
) as f:
        f[
```
```python
'data'
```
```python
] = datas
        f[
```
```python
'labels'
```
```python
] = labels
        f.close()
```
```python
# 写入列表文件，可以有多个hdf5文件
```
```python
with open(LIST_FILE[kk],
```
```python
'w'
```
```python
) as f:
        f.write(os.path.abspath(HDF5_FILE[kk]) +
```
```python
'\n'
```
```python
)
        f.close()
print
```
```python
'\ndone...'
```
注意
1.caffe中要求1个hdf5文件大小不超过2GB，所以如果数据量太大，建议生成  多个hdf5文件
2.我用的5万张图片，大小一共30几兆，生成的hdf5文件是1.8GB

## 2.Matlab
**注**：以下内容摘自参考文献[2].
有些时候，我们的输入不是标准的图像，而是其它一些格式，比如：频谱图、特征向量等等，这种情况下LMDB、Leveldb以及ImageData layer等就不好使了，这时候我们就需要一个新的输入接口——HDF5Data[2].
**(1) 在Matlab中生成HDF5文件**
这里直接以例子的形式说明如何生成：
```python
问题：输入
```
```python
data
```
```python
是512*1的矢量，共1000个样本，label是标量。
```
代码如下：
```python
% 创建HDF5文件，包含data和label两个变量，数据类型是caffe支持的float型数据
```
```python
h5create(
```
```python
'train.h5'
```
```python
,
```
```python
'/data'
```
```python
,
```
```python
[
```
```python
1
```
```python
1
```
```python
512
```
```python
1000
```
```python
]
```
```python
,
```
```python
'Datatype'
```
```python
,
```
```python
'single'
```
```python
);
h5create(
```
```python
'train.h5'
```
```python
,
```
```python
'/label'
```
```python
,
```
```python
[
```
```python
1
```
```python
1
```
```python
1
```
```python
1000
```
```python
]
```
```python
,
```
```python
'Datatype'
```
```python
,
```
```python
'single'
```
```python
);
```
```python
%reshape: width x height x channels x num，注意MATLAB读数据是列优先，是和C++里面相反的。所以写数据的时候也要倒着写。
```
```python
train_data  =
```
```python
reshape
```
```python
(train_data,
```
```python
[
```
```python
1
```
```python
1
```
```python
512
```
```python
1000
```
```python
]
```
```python
);
train_label =
```
```python
reshape
```
```python
(train_label,
```
```python
[
```
```python
1
```
```python
1
```
```python
1
```
```python
1000
```
```python
]
```
```python
);
h5write(
```
```python
'train.h5'
```
```python
,
```
```python
'/data'
```
```python
, single(train_data));
h5write(
```
```python
'train.h5'
```
```python
,
```
```python
'/label'
```
```python
, single(train_label));
```
**(2) caffe中HDF5层的用法**
**注意**：由于单个HDF5文件大小有限制，MATLAB下好像最大只能生成5G大小的文件，因此当我们的训练数据较多的时候，往往需要将数据分别写入多个H5文件中。
下面直接给出caffe中HDF5Data-layer的使用示例：
```python
layer
```
```python
{
```
```python
name
```
```python
:
```
```python
"data"
```
```python
type:
```
```python
"HDF5Data"
```
```python
top:
```
```python
"data"
```
```python
top:
```
```python
"label"
```
```python
include {
    phase: TRAIN
```
```python
}
```
```python
hdf5_data_param
```
```python
{
```
```python
source
```
```python
:
```
```python
"train.txt"
```
```python
batch_size:
```
```python
128
```
```python
shuffle: true
```
```python
}
}
```
由于可能存在多个H5文件，所以HDF5Data的输入是从一个TXT文件读取的列表，train.txt内容示例如下：
```python
train1.h5
train2.h5
```
```python
...
```
**(3) 实例**
参考文献：
1.[http://blog.csdn.net/u011762313/article/details/48830561](http://blog.csdn.net/u011762313/article/details/48830561)[ 生成hdf5文件用于多标签训练]
2.[http://blog.csdn.net/shuzfan/article/details/52503683](http://blog.csdn.net/shuzfan/article/details/52503683)[ caffe HDF5Data 层使用及数据生成]

